
// $Id: cutflowCycle.cxx,v 1.1 2013/03/26 12:50:16 hoeing Exp $

#include <iostream>

using namespace std;

// Local include(s):
#include "include/cutflowCycle.h"
#include "include/SelectionModules.h"
#include "include/cutflowHists.h"

ClassImp( cutflowCycle );

cutflowCycle::cutflowCycle()
  : AnalysisCycle() {

  // constructor, declare additional variables that should be 
  // obtained from the steering-xml file

  // set the integrated luminosity per bin for the lumi-yield control plots
  SetIntLumiPerBin(25.);
 
}

cutflowCycle::~cutflowCycle() 
{
  // destructor
}

void cutflowCycle::BeginCycle() throw( SError ) 
{
  // Start of the job, general set-up and definition of 
  // objects are done here

  // Important: first call BeginCycle of base class
  AnalysisCycle::BeginCycle();

  return;




}

void cutflowCycle::EndCycle() throw( SError ) 
{
  // clean-up, info messages and final calculations after the analysis

  
  // call the base cycle class for all standard methods
  AnalysisCycle::EndCycle();

  return;

}

void cutflowCycle::BeginInputData( const SInputData& id ) throw( SError ) 
{
  // declaration of histograms and selections

  // Important: first call BeginInputData of base class
  AnalysisCycle::BeginInputData( id );
  
 


  // -------------------- set up the selections ---------------------------


 
  Selection* trigger = new Selection("HLT_HT750");
  Selection* nJets = new Selection("nJets");
  Selection* topTag = new Selection("topTag");
  Selection* topAndSubBTag = new Selection("topAndSubBTag");
 Selection* topAndSubBTagPlusHiggsTag = new Selection("topAndSubBTagPlusHiggsTag");
  Selection* HT1000 = new Selection("HT1000");


  //DO NOT use trigger selection in PROOF mode at the moment
  trigger->addSelectionModule(new TriggerSelection("HLT_HT750"));
  nJets->addSelectionModule(new NTopJetSelection(2, 99, 0., 99.));
  topTag->addSelectionModule(new NHEPTopTagSelection(1, 99));
  topAndSubBTag->addSelectionModule(new NHEPTopAndSubBTagSelection(1, 99, e_CSVM));
  topAndSubBTagPlusHiggsTag->addSelectionModule(new HEPTopAndSubBTagPlusOtherHiggsTag(e_CSVM, e_CSVM, e_CSVM));
  HT1000->addSelectionModule(new HTCut(1000.));
  
  RegisterSelection(trigger);
  RegisterSelection(nJets);
  RegisterSelection(topTag);
  RegisterSelection(topAndSubBTag);
  RegisterSelection(topAndSubBTagPlusHiggsTag);
  RegisterSelection(HT1000);
  RegisterHistCollection( new BTagEffHistsTPrime("BTagEff") );

  // ---------------- set up the histogram collections --------------------

  // histograms without any cuts
  RegisterHistCollection( new cutflowHists("NoCuts") );
  // histograms after the top selection
  RegisterHistCollection( new cutflowHists("cutflow1") );
  RegisterHistCollection( new cutflowHists("cutflow2") );
  RegisterHistCollection( new cutflowHists("cutflow3") );
  RegisterHistCollection( new cutflowHists("cutflow4") );
  RegisterHistCollection( new cutflowHists("cutflow5") );
  RegisterHistCollection( new cutflowHists("cutflow6") );
  // RegisterHistCollection( new BTagEffHistsTPrime("BTagEff") );

  
  // important: initialise histogram collections after their definition
  InitHistos();

  return;

}

void cutflowCycle::EndInputData( const SInputData& id ) throw( SError ) 
{
  AnalysisCycle::EndInputData( id );
  return;

}

void cutflowCycle::BeginInputFile( const SInputData& id ) throw( SError ) 
{
  // Connect all variables from the Ntuple file with the ones needed for the analysis
  // The variables are commonly stored in the BaseCycleContaincer

  // important: call to base function to connect all variables to Ntuples from the input tree
  AnalysisCycle::BeginInputFile( id );

  return;

}

void cutflowCycle::ExecuteEvent( const SInputData& id, Double_t weight) throw( SError ) 
{
  // this is the most important part: here the full analysis happens
  // user should implement selections, filling of histograms and results

  // first step: call Execute event of base class to perform basic consistency checks
  // also, the good-run selection is performed there and the calculator is reset
  AnalysisCycle::ExecuteEvent( id, weight);
   


  // get the selections
  static Selection* trigger = GetSelection("HLT_HT750");
  static Selection* nJets = GetSelection("nJets");
  static Selection* topTag = GetSelection("topTag");
  static Selection* topAndSubBTag = GetSelection("topAndSubBTag");
  static Selection* topAndSubBTagPlusHiggsTag = GetSelection("topAndSubBTagPlusHiggsTag");
  static Selection* HT1000 = GetSelection("HT1000");
  
  // get the histogram collections
  BaseHists* HistsNoCuts = GetHistCollection("NoCuts");
  BaseHists* Histscutflow1 = GetHistCollection("cutflow1");
  BaseHists* Histscutflow2 = GetHistCollection("cutflow2");
  BaseHists* Histscutflow3 = GetHistCollection("cutflow3");
  BaseHists* Histscutflow4 = GetHistCollection("cutflow4");
  BaseHists* Histscutflow5 = GetHistCollection("cutflow5");
  BaseHists* Histscutflow6 = GetHistCollection("cutflow6");
  BaseHists* BTagEff_Hists = GetHistCollection("BTagEff");


  // start the analysis
  HistsNoCuts->Fill();



  EventCalc* calc = EventCalc::Instance();
  BaseCycleContainer* bcc = calc->GetBaseCycleContainer();
   bool IsRealData = calc->IsRealData();





   if(trigger->passSelection(bcc))Histscutflow1->Fill();
   else throw SError( SError::SkipEvent );
   if(nJets->passSelection(bcc)) Histscutflow2->Fill();
   else throw SError( SError::SkipEvent );
   if(HT1000->passSelection(bcc))Histscutflow3->Fill();
   else throw SError( SError::SkipEvent );
   if (IsRealData == false)BTagEff_Hists->Fill(); 
   if(topTag->passSelection(bcc)) Histscutflow4->Fill();
   else throw SError( SError::SkipEvent );
   if(topAndSubBTag->passSelection(bcc))Histscutflow5->Fill();
   else throw SError( SError::SkipEvent );
   if(topAndSubBTagPlusHiggsTag->passSelection(bcc))Histscutflow6->Fill();
   else throw SError( SError::SkipEvent );
   if (IsRealData == false)BTagEff_Hists->Fill();





  return;
  
}

