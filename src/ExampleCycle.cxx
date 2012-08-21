// $Id: ExampleCycle.cxx,v 1.4 2012/06/22 07:46:17 peiffer Exp $

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
  AnalysisCycle::EndCycle();

  return;

}

void ExampleCycle::BeginInputData( const SInputData& id ) throw( SError ) 
{
  // declaration of histograms and selections

  // Important: first call BeginInputData of base class
  AnalysisCycle::BeginInputData( id );

  // -------------------- set up the selections ---------------------------

  Selection* BSel = new Selection( "BSelection");
  BSel->addSelectionModule(new NBTagSelection(1)); //at least one b tag

  Selection* NoBSel = new Selection( "NoBSelection");
  NoBSel->addSelectionModule(new NBTagSelection(0,0)); //no b tags

  Selection* TopSel = new Selection("TopSelection");
  //DO NOT use trigger selection in PROOF mode at the moment
  //TopSel->addSelectionModule(new TriggerSelection("HLT_PFJet320_v"));
  TopSel->addSelectionModule(new NTopJetSelection(1,int_infinity(),350,2.5));
  TopSel->addSelectionModule(new NTopTagSelection(1,int_infinity()));

  RegisterSelection(BSel);
  RegisterSelection(NoBSel);
  RegisterSelection(TopSel);

  // ---------------- set up the histogram collections --------------------

  // histograms without any cuts
  RegisterHistCollection( new ExampleHists("NoCuts") );

  //histograms with and without b tagging
  RegisterHistCollection( new ExampleHists("BTag") );
  RegisterHistCollection( new ExampleHists("NoBTag") );

  // histograms after the top selection
  RegisterHistCollection( new ExampleHists("TopSel") );

  // important: initialise histogram collections after their definition
  InitHistos();

  return;

}

void ExampleCycle::EndInputData( const SInputData& id ) throw( SError ) 
{
  AnalysisCycle::EndInputData( id );
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
  static Selection* BSel = GetSelection("BSelection");
  static Selection* NoBSel = GetSelection("NoBSelection");
  static Selection* TopSel = GetSelection("TopSelection");

  // get the histogram collections
  BaseHists* HistsNoCuts = GetHistCollection("NoCuts");
  BaseHists* HistsBTag = GetHistCollection("BTag");
  BaseHists* HistsNoBTag = GetHistCollection("NoBTag");
  BaseHists* HistsTopSel = GetHistCollection("TopSel");

  // start the analysis
  HistsNoCuts->Fill();
  EventCalc* calc = EventCalc::Instance();
  if(BSel->passSelection()){
    HistsBTag->Fill();
  }
  if(NoBSel->passSelection()){
    HistsNoBTag->Fill();  
  }

  ObjectHandler* objs = ObjectHandler::Instance();
  
  if(!TopSel->passSelection())  throw SError( SError::SkipEvent );

  HistsTopSel->Fill();
  
  return;
  
}

