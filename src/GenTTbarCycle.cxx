#include <iostream>

using namespace std;

// Local include(s):
#include "include/GenTTbarCycle.h"

ClassImp( GenTTbarCycle );

GenTTbarCycle::GenTTbarCycle()
   : AnalysisCycle() {

  // constructor, declare additional variables that should be 
  // obtained from the steering-xml file
  
  // set the integrated luminosity per bin for the lumi-yield control plots
  SetIntLumiPerBin(500.);

}

GenTTbarCycle::~GenTTbarCycle() 
{
  // destructor
}

void GenTTbarCycle::BeginCycle() throw( SError ) 
{
  // Start of the job, general set-up and definition of 
  // objects are done here

  // Important: first call BeginCycle of base class
  AnalysisCycle::BeginCycle();

  return;

}

void GenTTbarCycle::EndCycle() throw( SError ) 
{
  // clean-up, info messages and final calculations after the analysis

  
  // call the base cycle class for all standard methods
  AnalysisCycle::EndCycle();

  return;

}

void GenTTbarCycle::BeginInputData( const SInputData& id ) throw( SError ) 
{
  // declaration of histograms and selections

  // Important: first call BeginInputData of base class
  AnalysisCycle::BeginInputData( id );

  // -------------------- set up the selections ---------------------------


  Selection* mtt_selection= new Selection("mtt_selection");
  mtt_selection->addSelectionModule(new MttbarGenCut(0,700));

  RegisterSelection(mtt_selection);

  // ---------------- set up the histogram collections --------------------

  // histograms without any cuts
  RegisterHistCollection( new GenTTbarHists("NoCuts") );

  // important: initialise histogram collections after their definition
  InitHistos();

  return;

}

void GenTTbarCycle::EndInputData( const SInputData& id ) throw( SError ) 
{
  AnalysisCycle::EndInputData( id );
  return;

}

void GenTTbarCycle::BeginInputFile( const SInputData& id ) throw( SError ) 
{
  // Connect all variables from the Ntuple file with the ones needed for the analysis
  // The variables are commonly stored in the BaseCycleContaincer

  // important: call to base function to connect all variables to Ntuples from the input tree
  AnalysisCycle::BeginInputFile( id );

  return;

}

void GenTTbarCycle::ExecuteEvent( const SInputData& id, Double_t weight) throw( SError ) 
{
  // this is the most important part: here the full analysis happens
  // user should implement selections, filling of histograms and results

  // first step: call Execute event of base class to perform basic consistency checks
  // also, the good-run selection is performed there and the calculator is reset
  AnalysisCycle::ExecuteEvent( id, weight );

  // get the selections
//   static Selection* mtt_selection = GetSelection("mtt_selection");
//   if(!mtt_selection->passSelection())  throw SError( SError::SkipEvent );

  // get the histogram collections
  BaseHists* HistsNoCuts = GetHistCollection("NoCuts");



  HistsNoCuts->Fill();
  
  return;
  
}


