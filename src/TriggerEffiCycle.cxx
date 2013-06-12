
#include <iostream>

using namespace std;

// Local include(s):
#include "include/TriggerEffiCycle.h"
#include "include/SelectionModules.h"
#include "include/ExampleHists.h"

ClassImp( TriggerEffiCycle );

TriggerEffiCycle::TriggerEffiCycle()
   : AnalysisCycle() {

  // constructor, declare additional variables that should be 
  // obtained from the steering-xml file
  
}

TriggerEffiCycle::~TriggerEffiCycle() 
{
  // destructor
}

void TriggerEffiCycle::BeginCycle() throw( SError ) 
{
  // Start of the job, general set-up and definition of 
  // objects are done here

  // Important: first call BeginCycle of base class
  AnalysisCycle::BeginCycle();

  return;

}

void TriggerEffiCycle::EndCycle() throw( SError ) 
{
  // clean-up, info messages and final calculations after the analysis

  
  // call the base cycle class for all standard methods
  AnalysisCycle::EndCycle();

  return;

}

void TriggerEffiCycle::BeginInputData( const SInputData& id ) throw( SError ) 
{
  // declaration of histograms and selections

  // Important: first call BeginInputData of base class
  AnalysisCycle::BeginInputData( id );

  // -------------------- set up the selections ---------------------------

  Selection* reference_trigger= new Selection("ReferenceTriggerSelection");
  //DO NOT use trigger selection in PROOF mode at the moment
  reference_trigger->addSelectionModule(new TriggerSelection("HLT_Photon135_v"));
  RegisterSelection(reference_trigger);

  Selection* test_trigger= new Selection("TestTriggerSelection");
  test_trigger->addSelectionModule(new TriggerSelection("HLT_Ele30_CaloIdVT_TrkIdT_PFNoPUJet150_PFNoPUJet25_v"));
  RegisterSelection(test_trigger);

  // ---------------- set up the histogram collections --------------------

  // histograms after the top selection
  RegisterHistCollection( new ExampleHists("ReferenceTriggerHistos") );
  RegisterHistCollection( new ExampleHists("TestTriggerHistos") );
  

  // important: initialise histogram collections after their definition
  InitHistos();

  return;

}

void TriggerEffiCycle::EndInputData( const SInputData& id ) throw( SError ) 
{
  AnalysisCycle::EndInputData( id );
  return;

}

void TriggerEffiCycle::BeginInputFile( const SInputData& id ) throw( SError ) 
{
  // Connect all variables from the Ntuple file with the ones needed for the analysis
  // The variables are commonly stored in the BaseCycleContaincer

  // important: call to base function to connect all variables to Ntuples from the input tree
  AnalysisCycle::BeginInputFile( id );

  return;

}

void TriggerEffiCycle::ExecuteEvent( const SInputData& id, Double_t weight) throw( SError ) 
{
  // this is the most important part: here the full analysis happens
  // user should implement selections, filling of histograms and results

  // first step: call Execute event of base class to perform basic consistency checks
  // also, the good-run selection is performed there and the calculator is reset
  AnalysisCycle::ExecuteEvent( id, weight);

  // get the selections
  static Selection* reference_trigger = GetSelection("ReferenceTriggerSelection");
  static Selection* test_trigger = GetSelection("TestTriggerSelection");

  // get the histogram collections
  BaseHists* reference_histos = GetHistCollection("ReferenceTriggerHistos");
  BaseHists* test_histos = GetHistCollection("TestTriggerHistos");

  // start the analysis
  if(reference_trigger->passSelection()) 
    reference_histos->Fill();

  if(test_trigger->passSelection()) 
    test_histos->Fill();
  
  return;
  
}

