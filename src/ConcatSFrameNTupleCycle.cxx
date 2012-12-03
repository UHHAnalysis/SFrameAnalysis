#include <iostream>

using namespace std;

// Local include(s):
#include "include/ConcatSFrameNTupleCycle.h"

ClassImp( ConcatSFrameNTupleCycle );

ConcatSFrameNTupleCycle::ConcatSFrameNTupleCycle()
   : AnalysisCycle() {

  // constructor, declare additional variables that should be
  // obtained from the steering-xml file


}

ConcatSFrameNTupleCycle::~ConcatSFrameNTupleCycle()
{
  // destructor
}

void ConcatSFrameNTupleCycle::BeginCycle() throw( SError )
{
  // Start of the job, general set-up and definition of
  // objects are done here

  // Important: first call BeginCycle of base class
  AnalysisCycle::BeginCycle();

  return;

}

void ConcatSFrameNTupleCycle::EndCycle() throw( SError )
{
  // clean-up, info messages and final calculations after the analysis


  // call the base cycle class for all standard methods
  // and a summary of the made selections
  AnalysisCycle::EndCycle();

  return;

}

void ConcatSFrameNTupleCycle::BeginInputData( const SInputData& id ) throw( SError )
{
  // declaration of histograms and selections

  // Important: first call BeginInputData of base class
  AnalysisCycle::BeginInputData( id );

  // -------------------- set up the selections ---------------------------

  //Selection* preselection = new Selection("pre-selection");

  return;

}

void ConcatSFrameNTupleCycle::EndInputData( const SInputData& id ) throw( SError )
{
  AnalysisCycle::EndInputData( id );

  return;

}

void ConcatSFrameNTupleCycle::BeginInputFile( const SInputData& id ) throw( SError )
{
  // Connect all variables from the Ntuple file with the ones needed for the analysis
  // The variables are commonly stored in the BaseCycleContaincer

  // important: call to base function to connect all variables to Ntuples from the input tree
  AnalysisCycle::BeginInputFile( id );

  return;

}

void ConcatSFrameNTupleCycle::ExecuteEvent( const SInputData& id, Double_t weight) throw( SError )
{
  // this is the most important part: here the full analysis happens
  // user should implement selections, filling of histograms and results

  // first step: call Execute event of base class to perform basic consistency checks
  // also, the good-run selection is performed there and the calculator is reset
  AnalysisCycle::ExecuteEvent( id, weight);

  WriteOutputTree();

  return;


}

