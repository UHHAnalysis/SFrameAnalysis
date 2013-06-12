#include <iostream>

using namespace std;

// Local include(s):
#include "include/JetLeptonCleanerCycle.h"


ClassImp( JetLeptonCleanerCycle );

JetLeptonCleanerCycle::JetLeptonCleanerCycle()
   : AnalysisCycle() {

  // constructor, declare additional variables that should be 
  // obtained from the steering-xml file
  
  // set the integrated luminosity per bin for the lumi-yield control plots
  SetIntLumiPerBin(25.);

}

JetLeptonCleanerCycle::~JetLeptonCleanerCycle() 
{
  // destructor
}

void JetLeptonCleanerCycle::BeginCycle() throw( SError ) 
{
  // Start of the job, general set-up and definition of 
  // objects are done here

  // Important: first call BeginCycle of base class
  AnalysisCycle::BeginCycle();

  return;

}

void JetLeptonCleanerCycle::EndCycle() throw( SError ) 
{
  // clean-up, info messages and final calculations after the analysis

  
  // call the base cycle class for all standard methods
  // and a summary of the made selections
  AnalysisCycle::EndCycle();

  return;

}

void JetLeptonCleanerCycle::BeginInputData( const SInputData& id ) throw( SError ) 
{
  // declaration of histograms and selections

  // Important: first call BeginInputData of base class
  AnalysisCycle::BeginInputData( id );

  // ---------------- set up the histogram collections --------------------

  // histograms without any cuts
  RegisterHistCollection( new JetLeptonCleanerHists("AllLeptons") );
  // histograms after the top selection
  RegisterHistCollection( new JetLeptonCleanerHists("CleanedLeptons") );

  // important: initialise histogram collections after their definition
  InitHistos();

  Selection* selection= new Selection("selection");

  selection->addSelectionModule(new NElectronSelection(1,int_infinity()));//at least one electron
  selection->addSelectionModule(new NMuonSelection(0,0));//no muons
  RegisterSelection(selection);

  return;

}

void JetLeptonCleanerCycle::EndInputData( const SInputData& id ) throw( SError ) 
{
  AnalysisCycle::EndInputData( id );


  return;

}

void JetLeptonCleanerCycle::BeginInputFile( const SInputData& id ) throw( SError ) 
{
  // Connect all variables from the Ntuple file with the ones needed for the analysis
  // The variables are commonly stored in the BaseCycleContaincer

  // important: call to base function to connect all variables to Ntuples from the input tree
  AnalysisCycle::BeginInputFile( id );

  return;

}

void JetLeptonCleanerCycle::ExecuteEvent( const SInputData& id, Double_t weight) throw( SError ) 
{
  // this is the most important part: here the full analysis happens
  // user should implement selections, filling of histograms and results

  // first step: call Execute event of base class to perform basic consistency checks
  // also, the good-run selection is performed there and the calculator is reset
  AnalysisCycle::ExecuteEvent( id, weight);

  EventCalc* calc = EventCalc::Instance();
  BaseCycleContainer* bcc = calc->GetBaseCycleContainer();


  // get the histogram collections
  BaseHists* HistsNoCleaning = GetHistCollection("AllLeptons");
  BaseHists* HistsLeptonCleaning = GetHistCollection("CleanedLeptons");

  //save uncleaned jet collection and MET for second step with lepton cuts
  std::vector<Jet> uncleaned_jets;
  for(unsigned int i=0; i<bcc->jets->size();++i){
    uncleaned_jets.push_back(bcc->jets->at(i));
  }
  MET uncleaned_met = *bcc->met;

  HistsNoCleaning->Fill();

  //fill the uncleaned collections back to bcc
  bcc->met->set_pt (uncleaned_met.pt());
  bcc->met->set_phi (uncleaned_met.phi());
  bcc->jets->clear();
  for(unsigned int i=0; i<uncleaned_jets.size();++i){
     bcc->jets->push_back(uncleaned_jets.at(i));
  }

  Cleaner cleaner;
  if(bcc->muons) cleaner.MuonCleaner_noIso(35,2.1);
  if(bcc->electrons) cleaner.ElectronCleaner_noIso(35,2.5);

  //static Selection* selection = GetSelection("selection");
  //if(!selection->passSelection())  throw SError( SError::SkipEvent );

  HistsLeptonCleaning->Fill();

  return;

  
}

