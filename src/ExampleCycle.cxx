// $Id: CycleCreators.py 159 2010-04-13 09:44:22Z krasznaa $

#include <iostream>

using namespace std;

// Local include(s):
#include "include/ExampleCycle.h"
#include "include/SelectionModules.h"
#include "include/ExampleHists.h"
#include "include/ObjectHandler.h"

ClassImp( ExampleCycle );

ExampleCycle::ExampleCycle()
   : AnalysisCycle() {

  // constructor, declare additional variables that should be 
  // obtained from the steering-xml file
  
  // set the integrated luminosity per bin for the lumi-yield control plots
  SetIntLumiPerBin(25.);

}

ExampleCycle::~ExampleCycle() 
{
  // destructor
}

void ExampleCycle::BeginCycle() throw( SError ) 
{
  // Start of the job, general set-up and definition of 
  // objects are done here

  // Important: first call BeginCycle of base class
  AnalysisCycle::BeginCycle();

  return;

}

void ExampleCycle::EndCycle() throw( SError ) 
{
  // clean-up, info messages and final calculations after the analysis

  
  // call the base cycle class for all standard methods
  // and a summary of the made selections
  AnalysisCycle::EndCycle();

  return;

}

void ExampleCycle::BeginInputData( const SInputData& id ) throw( SError ) 
{
  // declaration of histograms and selections

  // Important: first call BeginInputData of base class
  AnalysisCycle::BeginInputData( id );

  // -------------------- set up the selections ---------------------------

  Selection* TopSel = new Selection("TopSelection");
  //DO NOT use trigger selection in PROOF mode at the moment
  //TopSel->addSelectionModule(new TriggerSelection("HLT_PFJet320_v"));
  TopSel->addSelectionModule(new NTopJetSelection(2,999,350,2.5));
  TopSel->addSelectionModule(new NTopTagSelection(2,999));
  RegisterSelection(TopSel);


  // ---------------- set up the histogram collections --------------------

  // histograms without any cuts
  RegisterHistCollection( new ExampleHists("NoCuts") );
  // histograms after the top selection
  RegisterHistCollection( new ExampleHists("TopSel") );


  // important: initialise histogram collections after their definition
  InitHistos();

  return;

}

void ExampleCycle::EndInputData( const SInputData& id ) throw( SError ) 
{

   return;

}

void ExampleCycle::BeginInputFile( const SInputData& id ) throw( SError ) 
{
  // Connect all variables from the Ntuple file with the ones needed for the analysis
  // The variables are commonly stored in the BaseCycleContaincer

  // important: call to base function to connect all variables to Ntuples from the input tree
  AnalysisCycle::BeginInputFile( id );

  return;

}

void ExampleCycle::ExecuteEvent( const SInputData& id, Double_t weight) throw( SError ) 
{
  // this is the most important part: here the full analysis happens
  // user should implement selections, filling of histograms and results

  // first step: call Execute event of base class to perform basic consistency checks
  // also, the good-run selection is performed there and the calculator is reset
  AnalysisCycle::ExecuteEvent( id, weight);

  // get the selections
  static Selection* TopSel = GetSelection("TopSelection");

  // get the histogram collections
  static BaseHists* HistsNoCuts = GetHistCollection("NoCuts");
  static BaseHists* HistsTopSel = GetHistCollection("TopSel");


  // start the analysis
  HistsNoCuts->Fill();
  
  ObjectHandler* objs = ObjectHandler::Instance();
  BaseCycleContainer* bcc = objs->GetBaseCycleContainer();
  
  if(!TopSel->passSelection(bcc))  throw SError( SError::SkipEvent );

  HistsTopSel->Fill();
  
  return;
  
}

