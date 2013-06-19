// $Id: TopTagTMVACycle.cxx,v 1.1 2013/05/21 14:40:03 rkogler Exp $

#include <iostream>

using namespace std;

// Local include(s):
#include "include/TopTagTMVACycle.h"
#include "include/TMVATreeFiller.h"
#include "include/SelectionModules.h"
#include "include/HypothesisHists.h"

ClassImp( TopTagTMVACycle );

TopTagTMVACycle::TopTagTMVACycle()
   : AnalysisCycle() {

  // constructor, declare additional variables that should be 
  // obtained from the steering-xml file
  
  // set the integrated luminosity per bin for the lumi-yield control plots
  SetIntLumiPerBin(250.);

}

TopTagTMVACycle::~TopTagTMVACycle() 
{
  // destructor
}

void TopTagTMVACycle::BeginCycle() throw( SError ) 
{
  // Start of the job, general set-up and definition of 
  // objects are done here

  // Important: first call BeginCycle of base class
  AnalysisCycle::BeginCycle();

  return;

}

void TopTagTMVACycle::EndCycle() throw( SError ) 
{
  // clean-up, info messages and final calculations after the analysis

  
  // call the base cycle class for all standard methods
  AnalysisCycle::EndCycle();

  return;

}

void TopTagTMVACycle::BeginInputData( const SInputData& id ) throw( SError ) 
{
  // declaration of histograms and selections

  // Important: first call BeginInputData of base class
  AnalysisCycle::BeginInputData( id );

  // create one selection: select CA-jets
  Selection* CAJetSel = new Selection("CAJetSelection");
  CAJetSel->addSelectionModule(new NTopJetSelection(1,int_infinity(),200,2.5));
  RegisterSelection(CAJetSel);
  
  // this class fills the tree for the TMVA analysis
  TTree * toptagtree = GetOutputMetadataTree("TopTagTree");
  RegisterHistCollection( new TMVATreeFiller("TopTagFiller", toptagtree) );

  // important: initialise histogram collections after their definition
  InitHistos();

  return;

}

void TopTagTMVACycle::EndInputData( const SInputData& id ) throw( SError ) 
{
  AnalysisCycle::EndInputData( id );

  return;

}

void TopTagTMVACycle::BeginInputFile( const SInputData& id ) throw( SError ) 
{
  // Connect all variables from the Ntuple file with the ones needed for the analysis
  // The variables are commonly stored in the BaseCycleContaincer

  // important: call to base function to connect all variables to Ntuples from the input tree
  AnalysisCycle::BeginInputFile( id );

  return;

}

void TopTagTMVACycle::ExecuteEvent( const SInputData& id, Double_t weight) throw( SError ) 
{
  // this is the most important part: here the full analysis happens
  // user should implement selections, filling of histograms and results

  // first step: call Execute event of base class to perform basic consistency checks
  // also, the good-run selection is performed there and the calculator is reset
  AnalysisCycle::ExecuteEvent( id, weight );

  static Selection* CAJetSel = GetSelection("CAJetSelection");

  BaseHists* TreeFiller = GetHistCollection("TopTagFiller");

  if(!CAJetSel->passSelection())  throw SError( SError::SkipEvent );

  TreeFiller->Fill();

  return;
  
}

