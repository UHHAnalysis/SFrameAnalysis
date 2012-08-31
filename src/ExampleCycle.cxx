// $Id: ExampleCycle.cxx,v 1.6 2012/08/22 15:31:45 peiffer Exp $

#include <iostream>

using namespace std;

// Local include(s):
#include "include/ExampleCycle.h"
#include "include/SelectionModules.h"
#include "include/ExampleHists.h"
#include "include/ObjectHandler.h"
#include "include/HypothesisHists.h"

ClassImp( ExampleCycle );

ExampleCycle::ExampleCycle()
   : AnalysisCycle() {

  // constructor, declare additional variables that should be 
  // obtained from the steering-xml file
  
  // set the integrated luminosity per bin for the lumi-yield control plots
  SetIntLumiPerBin(250.);

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

  static Chi2Discriminator* m_chi2discr = new Chi2Discriminator();
  RegisterHistCollection( new HypothesisHists("Chi2_NoCuts", m_chi2discr ) );

  //histograms with and without b tagging
  RegisterHistCollection( new ExampleHists("BTag") );
  RegisterHistCollection( new ExampleHists("NoBTag") );
  RegisterHistCollection( new HypothesisHists("Chi2_BTag", m_chi2discr ) );
  RegisterHistCollection( new HypothesisHists("Chi2_NoBTag", m_chi2discr ) );

  // histograms after the top selection
  RegisterHistCollection( new ExampleHists("TopSel") );
  RegisterHistCollection( new HypothesisHists("Chi2_TopSel", m_chi2discr ) );

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
  
  BaseHists* Chi2_HistsNoCuts = GetHistCollection("Chi2_NoCuts");
  BaseHists* Chi2_HistsBTag = GetHistCollection("Chi2_BTag");
  BaseHists* Chi2_HistsNoBTag = GetHistCollection("Chi2_NoBTag");
  BaseHists* Chi2_HistsTopSel = GetHistCollection("Chi2_TopSel");


  // start the analysis
  HistsNoCuts->Fill();
  Chi2_HistsNoCuts->Fill();

  if(BSel->passSelection()){
    HistsBTag->Fill();
    Chi2_HistsBTag->Fill();
  }
  if(NoBSel->passSelection()){
    HistsNoBTag->Fill();  
    Chi2_HistsNoBTag->Fill();
  }

  ObjectHandler* objs = ObjectHandler::Instance();
  
  if(!TopSel->passSelection())  throw SError( SError::SkipEvent );

  HistsTopSel->Fill();
  Chi2_HistsTopSel->Fill();
  
  return;
  
}

