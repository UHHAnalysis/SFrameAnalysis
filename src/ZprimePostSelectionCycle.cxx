// $Id: ZprimePostSelectionCycle.cxx,v 1.9 2012/12/06 08:24:48 peiffer Exp $

#include <iostream>

using namespace std;

// Local include(s):
#include "include/ZprimePostSelectionCycle.h"

ClassImp( ZprimePostSelectionCycle );

ZprimePostSelectionCycle::ZprimePostSelectionCycle()
   : AnalysisCycle() {

  // constructor, declare additional variables that should be 
  // obtained from the steering-xml file
  
  // set the integrated luminosity per bin for the lumi-yield control plots
  SetIntLumiPerBin(500.);

}

ZprimePostSelectionCycle::~ZprimePostSelectionCycle() 
{
  // destructor
}

void ZprimePostSelectionCycle::BeginCycle() throw( SError ) 
{
  // Start of the job, general set-up and definition of 
  // objects are done here

  // Important: first call BeginCycle of base class
  AnalysisCycle::BeginCycle();

  return;

}

void ZprimePostSelectionCycle::EndCycle() throw( SError ) 
{
  // clean-up, info messages and final calculations after the analysis

  
  // call the base cycle class for all standard methods
  AnalysisCycle::EndCycle();

  return;

}

void ZprimePostSelectionCycle::BeginInputData( const SInputData& id ) throw( SError ) 
{
  // declaration of histograms and selections

  // Important: first call BeginInputData of base class
  AnalysisCycle::BeginInputData( id );

  // -------------------- set up the selections ---------------------------

  Selection* BSel = new Selection( "BSelection");
  BSel->addSelectionModule(new NBTagSelection(1)); //at least one b tag

  Selection* NoBSel = new Selection( "NoBSelection");
  NoBSel->addSelectionModule(new NBTagSelection(0,0)); //no b tags

  Selection* chi2_selection= new Selection("chi2_selection");
  static Chi2Discriminator* m_chi2discr = new Chi2Discriminator();
  chi2_selection->addSelectionModule(new HypothesisDiscriminatorCut( m_chi2discr, -1*double_infinity(), 10));
  //chi2_selection->addSelectionModule(new MttbarGenCut(0,700));

  Selection* TopSel = new Selection("TopSelection");
  //DO NOT use trigger selection in PROOF mode at the moment
  //TopSel->addSelectionModule(new TriggerSelection("HLT_PFJet320_v"));
  TopSel->addSelectionModule(new NTopJetSelection(1,int_infinity(),350,2.5));
  TopSel->addSelectionModule(new NTopTagSelection(1,int_infinity()));


  RegisterSelection(BSel);
  RegisterSelection(NoBSel);
  RegisterSelection(TopSel);
  RegisterSelection(chi2_selection);

  // ---------------- set up the histogram collections --------------------

  // histograms without any cuts

  RegisterHistCollection( new HypothesisHists("Chi2_NoCuts", m_chi2discr ) );
  
  RegisterHistCollection( new EventHists("Event") );
  RegisterHistCollection( new JetHists("Jets") );
  RegisterHistCollection( new ElectronHists("Electron") );
  RegisterHistCollection( new MuonHists("Muon") );
  RegisterHistCollection( new TauHists("Tau") );
  RegisterHistCollection( new TopJetHists("TopJets") );

  //histograms with and without b tagging
  RegisterHistCollection( new HypothesisHists("Chi2_BTag", m_chi2discr ) );
  RegisterHistCollection( new HypothesisHists("Chi2_NoBTag", m_chi2discr ) );

  RegisterHistCollection( new EventHists("Event_BTag") );
  RegisterHistCollection( new JetHists("Jets_BTag") );
  RegisterHistCollection( new ElectronHists("Electron_BTag") );
  RegisterHistCollection( new MuonHists("Muon_BTag") );
  RegisterHistCollection( new TauHists("Tau_BTag") );
  RegisterHistCollection( new TopJetHists("TopJets_BTag") );

  RegisterHistCollection( new EventHists("Event_NoBTag") );
  RegisterHistCollection( new JetHists("Jets_NoBTag") );
  RegisterHistCollection( new ElectronHists("Electron_NoBTag") );
  RegisterHistCollection( new MuonHists("Muon_NoBTag") );
  RegisterHistCollection( new TauHists("Tau_NoBTag") );
  RegisterHistCollection( new TopJetHists("TopJets_NoBTag") );

  // histograms after the top selection
  RegisterHistCollection( new HypothesisHists("Chi2_TopSel", m_chi2discr ) );

  RegisterHistCollection( new EventHists("Event_TopSel") );
  RegisterHistCollection( new JetHists("Jets_TopSel") );
  RegisterHistCollection( new ElectronHists("Electron_TopSel") );
  RegisterHistCollection( new MuonHists("Muon_TopSel") );
  RegisterHistCollection( new TauHists("Tau_TopSel") );
  RegisterHistCollection( new TopJetHists("TopJets_TopSel") );
  
  // important: initialise histogram collections after their definition
  InitHistos();

  return;

}

void ZprimePostSelectionCycle::EndInputData( const SInputData& id ) throw( SError ) 
{
  AnalysisCycle::EndInputData( id );
  return;

}

void ZprimePostSelectionCycle::BeginInputFile( const SInputData& id ) throw( SError ) 
{
  // Connect all variables from the Ntuple file with the ones needed for the analysis
  // The variables are commonly stored in the BaseCycleContaincer

  // important: call to base function to connect all variables to Ntuples from the input tree
  AnalysisCycle::BeginInputFile( id );

  return;

}

void ZprimePostSelectionCycle::ExecuteEvent( const SInputData& id, Double_t weight) throw( SError ) 
{
  // this is the most important part: here the full analysis happens
  // user should implement selections, filling of histograms and results

  // first step: call Execute event of base class to perform basic consistency checks
  // also, the good-run selection is performed there and the calculator is reset
  AnalysisCycle::ExecuteEvent( id, weight );

  // get the selections
  static Selection* BSel = GetSelection("BSelection");
  static Selection* NoBSel = GetSelection("NoBSelection");
  static Selection* TopSel = GetSelection("TopSelection");
  static Selection* chi2_selection = GetSelection("chi2_selection");

  // get the histogram collections
  BaseHists* Chi2_HistsNoCuts = GetHistCollection("Chi2_NoCuts");
  BaseHists* Chi2_HistsBTag = GetHistCollection("Chi2_BTag");
  BaseHists* Chi2_HistsNoBTag = GetHistCollection("Chi2_NoBTag");
  BaseHists* Chi2_HistsTopSel = GetHistCollection("Chi2_TopSel");

  //if(!chi2_selection->passSelection())  throw SError( SError::SkipEvent );

  EventCalc* calc = EventCalc::Instance();
  if(calc->GetJets()->size()>=12){
    std::cout << "run: " << calc->GetRunNum() << "   lb: " << calc->GetLumiBlock() << "  event: " << calc->GetEventNum() << "   N(jets): " << calc->GetJets()->size() << std::endl;
  }

  // start the analysis

  Chi2_HistsNoCuts->Fill();
  FillControlHistos("");


  if(BSel->passSelection()){
    Chi2_HistsBTag->Fill();
    FillControlHistos("_BTag");
  }
  if(NoBSel->passSelection()){
    Chi2_HistsNoBTag->Fill();
    FillControlHistos("_NoBTag");
  }
  
  if(!TopSel->passSelection())  throw SError( SError::SkipEvent );
  Chi2_HistsTopSel->Fill();
  FillControlHistos("_TopSel");
  
  return;
  
}


void ZprimePostSelectionCycle::FillControlHistos(TString postfix)
{
  // fill some control histograms, need to be defined in BeginInputData

  BaseHists* eventhists = GetHistCollection((std::string)("Event"+postfix));
  BaseHists* jethists = GetHistCollection((std::string)("Jets"+postfix));
  BaseHists* elehists = GetHistCollection((std::string)("Electron"+postfix));
  BaseHists* muonhists = GetHistCollection((std::string)("Muon"+postfix));
  BaseHists* tauhists = GetHistCollection((std::string)("Tau"+postfix));
  BaseHists* topjethists = GetHistCollection((std::string)("TopJets"+postfix));    

  eventhists->Fill();
  jethists->Fill();
  elehists->Fill();
  muonhists->Fill();  
  tauhists->Fill();
  topjethists->Fill();

}
