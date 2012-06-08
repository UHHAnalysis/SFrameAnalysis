#include <iostream>

using namespace std;

// Local include(s):
#include "include/ZprimeSelectionCycle.h"
#include "include/SelectionModules.h"
#include "include/ExampleHists.h"
#include "include/ObjectHandler.h"
#include "JetCorrectorParameters.h"

ClassImp( ZprimeSelectionCycle );

ZprimeSelectionCycle::ZprimeSelectionCycle()
   : AnalysisCycle() {

  // constructor, declare additional variables that should be 
  // obtained from the steering-xml file
  
  // set the integrated luminosity per bin for the lumi-yield control plots
  SetIntLumiPerBin(25.);

}

ZprimeSelectionCycle::~ZprimeSelectionCycle() 
{
  // destructor
}

void ZprimeSelectionCycle::BeginCycle() throw( SError ) 
{
  // Start of the job, general set-up and definition of 
  // objects are done here

  // Important: first call BeginCycle of base class
  AnalysisCycle::BeginCycle();

  return;

}

void ZprimeSelectionCycle::EndCycle() throw( SError ) 
{
  // clean-up, info messages and final calculations after the analysis

  
  // call the base cycle class for all standard methods
  // and a summary of the made selections
  AnalysisCycle::EndCycle();

  return;

}

void ZprimeSelectionCycle::BeginInputData( const SInputData& id ) throw( SError ) 
{
  // declaration of histograms and selections

  // Important: first call BeginInputData of base class
  AnalysisCycle::BeginInputData( id );

  // -------------------- set up the selections ---------------------------

  //Set-Up Selection
  first_selection= new Selection("first_selection");

  first_selection->addSelectionModule(new NPrimaryVertexSelection(1)); //at least one good PV
  first_selection->addSelectionModule(new NJetSelection(2,int_infinity(),50,2.4));//at least two jets
  first_selection->addSelectionModule(new NElectronSelection(1,int_infinity(),70,2.5));//at least one electron
  first_selection->addSelectionModule(new NElectronSelection(1,1,70,2.5));//exactly one electron 
  first_selection->addSelectionModule(new NMuonSelection(0,0,35,2.1));//no muons
  first_selection->addSelectionModule(new TwoDCut()); 

  second_selection= new Selection("second_selection");

  second_selection->addSelectionModule(new NJetSelection(1,int_infinity(),150,2.4));//leading jet with pt>150 GeV
  second_selection->addSelectionModule(new NBTagSelection(1)); //at least one b tag
  //second_selection->addSelectionModule(new NBTagSelection(0,0)); //no b tags
  second_selection->addSelectionModule(new HTlepCut(150));
  second_selection->addSelectionModule(new TriangularCut());
  second_selection->addSelectionModule(new METCut(50));
 
  RegisterSelection(first_selection);
  RegisterSelection(second_selection);

  std::vector<JetCorrectorParameters> pars;

  //see https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookJetEnergyCorrections#GetTxtFiles how to get the txt files with jet energy corrections from the database

  pars.push_back(JetCorrectorParameters("/afs/naf.desy.de/user/p/peiffer/SFrame/SFrameAnalysis/config/GR_R_52_V7_L1FastJet_AK5PF.txt"));
  pars.push_back(JetCorrectorParameters("/afs/naf.desy.de/user/p/peiffer/SFrame/SFrameAnalysis/config/GR_R_52_V7_L2Relative_AK5PF.txt"));  
  pars.push_back(JetCorrectorParameters("/afs/naf.desy.de/user/p/peiffer/SFrame/SFrameAnalysis/config/GR_R_52_V7_L3Absolute_AK5PF.txt")); 

  if(!addGenInfo()) pars.push_back(JetCorrectorParameters("/afs/naf.desy.de/user/p/peiffer/SFrame/SFrameAnalysis/config/GR_R_52_V7_L2L3Residual_AK5PF.txt")); 

  m_corrector = new FactorizedJetCorrector(pars);

  return;

}

void ZprimeSelectionCycle::EndInputData( const SInputData& id ) throw( SError ) 
{

  AnalysisCycle::EndInputData( id );
  return;

}

void ZprimeSelectionCycle::BeginInputFile( const SInputData& id ) throw( SError ) 
{
  // Connect all variables from the Ntuple file with the ones needed for the analysis
  // The variables are commonly stored in the BaseCycleContaincer

  // important: call to base function to connect all variables to Ntuples from the input tree
  AnalysisCycle::BeginInputFile( id );

  return;

}

void ZprimeSelectionCycle::ExecuteEvent( const SInputData& id, Double_t weight) throw( SError ) 
{
  // this is the most important part: here the full analysis happens
  // user should implement selections, filling of histograms and results

  // first step: call Execute event of base class to perform basic consistency checks
  // also, the good-run selection is performed there and the calculator is reset
  AnalysisCycle::ExecuteEvent( id, weight);

//   Selection* first_selection = GetSelection("first_selection");
//   Selection* second_selection = GetSelection("second_selection");
 
  m_cleaner = new Cleaner();

  ObjectHandler* objs = ObjectHandler::Instance();
  BaseCycleContainer* bcc = objs->GetBaseCycleContainer();

  if(bcc->pvs)  m_cleaner->PrimaryVertexCleaner(4, 24., 2.);
  if(bcc->electrons) m_cleaner->ElectronCleaner_noIso(70,2.5);
  if(bcc->muons) m_cleaner->MuonCleaner_noIso(35,2.1);  
  if(bcc->jets) m_cleaner->JetLeptonSubtractor(m_corrector);
  if(!bcc->isRealData && bcc->jets) m_cleaner->JetEnergyResolutionShifter();
  //apply loose jet cleaning for 2D cut
  if(bcc->jets) m_cleaner->JetCleaner(25,double_infinity(),true);

  if(!first_selection->passSelection())  throw SError( SError::SkipEvent );

  //apply tighter jet cleaning for further cuts and analysis steps
  if(bcc->jets) m_cleaner->JetCleaner(50,2.4,true);

  //remove all taus from collection for HTlep calculation
  if(bcc->taus) m_cleaner->TauCleaner(double_infinity(),0.0);

  if(!second_selection->passSelection())  throw SError( SError::SkipEvent );


  WriteOutputTree();

  return;

  
}

