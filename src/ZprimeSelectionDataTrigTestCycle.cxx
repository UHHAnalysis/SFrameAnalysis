#include <iostream>

using namespace std;

// Local include(s):
#include "include/ZprimeSelectionDataTrigTestCycle.h"

ClassImp( ZprimeSelectionDataTrigTestCycle );

ZprimeSelectionDataTrigTestCycle::ZprimeSelectionDataTrigTestCycle()
   : AnalysisCycle() {

  // constructor, declare additional variables that should be 
  // obtained from the steering-xml file
  
  // set the integrated luminosity per bin for the lumi-yield control plots
  SetIntLumiPerBin(25.);

  DeclareProperty( "Electron_Or_Muon_Selection", m_Electron_Or_Muon_Selection );

  //default: no btagging cuts applied, other cuts can be defined in config file
  m_Nbtags_min=0;
  m_Nbtags_max=int_infinity();
  DeclareProperty( "Nbtags_min", m_Nbtags_min);
  DeclareProperty( "Nbtags_max", m_Nbtags_max);  

}

ZprimeSelectionDataTrigTestCycle::~ZprimeSelectionDataTrigTestCycle() 
{
  // destructor
}

void ZprimeSelectionDataTrigTestCycle::BeginCycle() throw( SError ) 
{
  // Start of the job, general set-up and definition of 
  // objects are done here

  // Important: first call BeginCycle of base class
  AnalysisCycle::BeginCycle();

  return;

}

void ZprimeSelectionDataTrigTestCycle::EndCycle() throw( SError ) 
{
  // clean-up, info messages and final calculations after the analysis
  
  // call the base cycle class for all standard methods
  // and a summary of the made selections
  AnalysisCycle::EndCycle();

  return;

}

void ZprimeSelectionDataTrigTestCycle::BeginInputData( const SInputData& id ) throw( SError ) 
{
  // declaration of histograms and selections

  // Important: first call BeginInputData of base class
  AnalysisCycle::BeginInputData( id );

  // -------------------- set up the selections ---------------------------

  //Set-Up Selection

  Selection* first_selection= new Selection("first_selection");
  Selection* trig_selection = new Selection("trig_selection");
  Selection* reftrig_selection = new Selection("reftrig_selection");

  doEle=false;
  doMu=false;

  if(m_Electron_Or_Muon_Selection=="Electrons" || m_Electron_Or_Muon_Selection=="Electron" || m_Electron_Or_Muon_Selection=="Ele" || m_Electron_Or_Muon_Selection=="ELE"){
    doEle=true;
  }
  else if(m_Electron_Or_Muon_Selection=="Muon" || m_Electron_Or_Muon_Selection=="Muons" || m_Electron_Or_Muon_Selection=="Mu" || m_Electron_Or_Muon_Selection=="MU"){
    doMu=true;
  }
  else{
    m_logger << ERROR << "Electron_Or_Muon_Selection is not defined in your xml config file --- should be either `ELE` or `MU`" << SLogger::endmsg;
  }
  
  if(doEle)
    trig_selection->addSelectionModule(new TriggerSelection("HLT_Ele30_CaloIdVT_TrkIdT_PFNoPUJet100_PFNoPUJet25_"));
  if(doMu)
    trig_selection->addSelectionModule(new TriggerSelection("HLT_Mu40_eta2p1_v"));

  //reftrig_selection->addSelectionModule(new TriggerSelection("HLT_L1SingleEG12_v6"));
  reftrig_selection->addSelectionModule(new TriggerSelection("HLT_Photon150_"));

  first_selection->addSelectionModule(new NPrimaryVertexSelection(1)); //at least one good PV
  first_selection->addSelectionModule(new NJetSelection(2,int_infinity(),50,2.4));//at least two jets


  if(doEle){
    first_selection->addSelectionModule(new NElectronSelection(1,int_infinity(),155));//at least one electron with pT > 140GeV
    first_selection->addSelectionModule(new NElectronSelection(1,1,155));//exactly one electron
    first_selection->addSelectionModule(new NMuonSelection(0,0));//no muons
  }
  if(doMu){
    first_selection->addSelectionModule(new NMuonSelection(1,int_infinity()));//at least one muon
    first_selection->addSelectionModule(new NMuonSelection(1,1));//exactly one muon
    first_selection->addSelectionModule(new NElectronSelection(0,0));//no electrons
  }

  first_selection->addSelectionModule(new TwoDCut());
  

  Selection* second_selection= new Selection("second_selection");

  second_selection->addSelectionModule(new NJetSelection(1,int_infinity(),150,2.5));//leading jet with pt>150 GeV
  second_selection->addSelectionModule(new NBTagSelection(m_Nbtags_min,m_Nbtags_max)); //b tags from config file
  second_selection->addSelectionModule(new HTlepCut(150));
  if(doEle)
    second_selection->addSelectionModule(new TriangularCut());
  second_selection->addSelectionModule(new METCut(50));

  Selection* chi2_selection= new Selection("chi2_selection");
  m_chi2discr = new Chi2Discriminator();
  chi2_selection->addSelectionModule(new HypothesisDiscriminatorCut( m_chi2discr, -1*double_infinity(), 10));

  m_bpdiscr = new BestPossibleDiscriminator();

  RegisterSelection(first_selection);
  RegisterSelection(trig_selection);
  RegisterSelection(reftrig_selection);
  RegisterSelection(second_selection);
  RegisterSelection(chi2_selection); 

  // ---------------- set up the histogram collections --------------------

  // histograms without any cuts
  //RegisterHistCollection( new HypothesisHists("Chi2", m_chi2discr) );
  //RegisterHistCollection( new HypothesisHists("BestPossible", m_bpdiscr) );
  if(doEle) {
    RegisterHistCollection( new ElectronHists("TestTriggerHistosChi2") );
    RegisterHistCollection( new ElectronHists("TestTriggerHistosNoChi2") );
    RegisterHistCollection( new ElectronHists("RefTriggerHistosChi2") );
    RegisterHistCollection( new ElectronHists("RefTriggerHistosNoChi2") );
  } else {
    RegisterHistCollection( new MuonHists("TestTriggerHistosChi2") );
    RegisterHistCollection( new MuonHists("TestTriggerHistosNoChi2") );
    RegisterHistCollection( new MuonHists("StandardSelectionChi2") );
    RegisterHistCollection( new MuonHists("StandardSelectionNoChi2") );
  }
  // important: initialise histogram collections after their definition
  InitHistos();

  return;

}

void ZprimeSelectionDataTrigTestCycle::EndInputData( const SInputData& id ) throw( SError ) 
{
/*
  if(doEle) {
    ElectronEffHists* eff_histos = (ElectronEffHists*)GetHistCollection("Efficiency");
    ElectronEffHists* eff_histos_chi2 = (ElectronEffHists*)GetHistCollection("Efficiency_chi2");
    ElectronHists* test_histos_chi2 = (ElectronHists*)GetHistCollection("TestTriggerHistosChi2");
    ElectronHists* test_histos = (ElectronHists*)GetHistCollection("TestTriggerHistosNoChi2");
    ElectronHists* standard_histos = (ElectronHists*)GetHistCollection("StandardSelectionNoChi2");
    ElectronHists* standard_histos_chi2 = (ElectronHists*)GetHistCollection("StandardSelectionChi2");
    TH1* eff_pt = eff_histos->getHist("pT");
    TH1* eff_pt_chi2 = eff_histos_chi2->getHist("pT");
    TH1* standard = standard_histos->getHist("pT");
    TH1* standard_chi2 = standard_histos_chi2->getHist("pT");
    TH1* test = test_histos->getHist("pT");
    TH1* test_chi2 = test_histos_chi2->getHist("pT");

    eff_pt->Divide(test,standard);
    eff_pt_chi2->Divide(test_chi2,standard_chi2);
  } else if(doMu) {
    MuonEffHists* eff_histos = (MuonEffHists*)GetHistCollection("Efficiency");
    MuonEffHists* eff_histos_chi2 = (MuonEffHists*)GetHistCollection("Efficiency_chi2");
    MuonHists* test_histos_chi2 = (MuonHists*)GetHistCollection("TestTriggerHistosChi2");
    MuonHists* test_histos = (MuonHists*)GetHistCollection("TestTriggerHistosNoChi2");
    MuonHists* standard_histos = (MuonHists*)GetHistCollection("StandardSelectionNoChi2");
    MuonHists* standard_histos_chi2 = (MuonHists*)GetHistCollection("StandardSelectionChi2");
    TH1* eff_pt = eff_histos->getHist("pT");
    TH1* eff_pt_chi2 = eff_histos_chi2->getHist("pT");
    TH1* standard = standard_histos->getHist("pT");
    TH1* standard_chi2 = standard_histos_chi2->getHist("pT");
    TH1* test = test_histos->getHist("pT");
    TH1* test_chi2 = test_histos_chi2->getHist("pT");

    eff_pt->Divide(test,standard,1,1,"B");
    eff_pt_chi2->Divide(test_chi2,standard_chi2,1,1,"B");  
    //eff_pt->SetOption("HIST");
    //eff_pt_chi2->SetOption("HIST");
*/
    /*int nbins = eff_pt->GetNbinsX();
    for(int i=0;i<=nbins+1;i++) {
      eff_pt->SetBinContent(i,(test->GetBinContent(i) / standard->GetBinContent(i)));
      eff_pt_chi2->SetBinContent(i,(test_chi2->GetBinContent(i) / standard_chi2->GetBinContent(i)));
    }
  }*/

  AnalysisCycle::EndInputData( id );
  return;

}

void ZprimeSelectionDataTrigTestCycle::BeginInputFile( const SInputData& id ) throw( SError ) 
{
  // Connect all variables from the Ntuple file with the ones needed for the analysis
  // The variables are commonly stored in the BaseCycleContaincer

  // important: call to base function to connect all variables to Ntuples from the input tree
  AnalysisCycle::BeginInputFile( id );

  return;

}

void ZprimeSelectionDataTrigTestCycle::ExecuteEvent( const SInputData& id, Double_t weight) throw( SError ) 
{
  // this is the most important part: here the full analysis happens
  // user should implement selections, filling of histograms and results

  // first step: call Execute event of base class to perform basic consistency checks
  // also, the good-run selection is performed there and the calculator is reset
  AnalysisCycle::ExecuteEvent( id, weight);
  
  static Selection* trig_selection = GetSelection("trig_selection");
  static Selection* reftrig_selection = GetSelection("reftrig_selection");
  static Selection* first_selection = GetSelection("first_selection");
  static Selection* second_selection = GetSelection("second_selection");
  static Selection* chi2_selection = GetSelection("chi2_selection");
 
  m_cleaner = new Cleaner();

  EventCalc* calc = EventCalc::Instance();
  BaseCycleContainer* bcc = calc->GetBaseCycleContainer();

  if(bcc->pvs)  m_cleaner->PrimaryVertexCleaner(4, 24., 2.);
  if(bcc->electrons) m_cleaner->ElectronCleaner_noIso(155,2.5);
  if(bcc->muons) m_cleaner->MuonCleaner_noIso(45,2.1);  
  if(bcc->jets) m_cleaner->JetLeptonSubtractor(m_corrector,false);

  if(!bcc->isRealData && bcc->jets) m_cleaner->JetEnergyResolutionShifter();
  //apply loose jet cleaning for 2D cut
  if(bcc->jets) m_cleaner->JetCleaner(25,double_infinity(),true);


  if(!first_selection->passSelection())  throw SError( SError::SkipEvent );

  //apply tighter jet cleaning for further cuts and analysis steps
  if(bcc->jets) m_cleaner->JetCleaner(50,2.5,true);

  //remove all taus from collection for HTlep calculation
  if(bcc->taus) m_cleaner->TauCleaner(double_infinity(),0.0);

  if(!second_selection->passSelection())  throw SError( SError::SkipEvent );


  //do reconstruction here



  calc->FillHighMassTTbarHypotheses();

  m_chi2discr->FillDiscriminatorValues();

  //m_bpdiscr->FillDiscriminatorValues();

  ReconstructionHypothesis *hyp = m_chi2discr->GetBestHypothesis();

  //double mttbar= (hyp->toplep_v4()+hyp->tophad_v4()).M();
  //double nu_pz = hyp->neutrino_v4().pz();
  //std::cout << "event: " << bcc->event << "   mttbar : " << mttbar << "   chi2 : " << hyp->discriminator("Chi2") << "   chi2 tlep: " << hyp->discriminator("Chi2_tlep") <<"   chi2 thad: " << hyp->discriminator("Chi2_thad") <<std::endl;

  //std::cout << "  leptonic side:  top pt : " << hyp->toplep_v4().Pt() << "   top m : " << hyp->toplep_v4().mass() << "   lepton pt : " << hyp->lepton().pt()  << "   nu pz : " << nu_pz << "   jet pt : " << bcc->jets->at( hyp->blep_index() ).pt() <<"   njets : " << hyp->toplep_jets_indices().size() << std::endl;
  //std::cout << "  hadronic side:  top pt : " << hyp->tophad_v4().Pt() << "   top m : " << hyp->tophad_v4().mass() << "   njets : " << hyp->tophad_jets_indices().size();
//   for(unsigned int i=0; i< hyp->tophad_jets_indices().size(); ++i){
//     std::cout << "   jet " << i<< " pt : " << bcc->jets->at( hyp->tophad_jets_indices().at(i) ).pt() ;
//   }
//   std::cout << std::endl;

  //ReconstructionHypothesis *bp_hyp = m_bpdiscr->GetBestHypothesis();
  //double bp_mttbar =  (bp_hyp->toplep_v4()+bp_hyp->tophad_v4()).M();
  //std::cout << "               bp mttbar : " << bp_mttbar << "   bp deltaR : " << bp_hyp->discriminator("BestPossible") << std::endl;

  // get the histogram collections
  //BaseHists* Chi2Hists = GetHistCollection("Chi2");
  //BaseHists* BPHists = GetHistCollection("BestPossible");

  //if(!chi2_selection->passSelection())  throw SError( SError::SkipEvent );

  //Chi2Hists->Fill();
  //BPHists->Fill();

  //calc->PrintEventContent();

  BaseHists* test_histos_chi2 = GetHistCollection("TestTriggerHistosChi2");
  BaseHists* test_histos = GetHistCollection("TestTriggerHistosNoChi2");
  BaseHists* reftrig_histos = GetHistCollection("RefTriggerHistosNoChi2");
  BaseHists* reftrig_histos_chi2 = GetHistCollection("RefTriggerHistosChi2");
  
  bool trigpass = false;
  bool reftrigpass = false;
  if(trig_selection->passSelection())
    trigpass = true;
  if(reftrig_selection->passSelection())
	  reftrigpass = true;
  bool chi2pass = false;
  if(chi2_selection->passSelection())
    chi2pass = true;
  if(trigpass && reftrigpass){
  	test_histos->Fill();
  }
  if(reftrigpass){
	reftrig_histos->Fill();
  }
  if(chi2pass) {
    if(trigpass){
      test_histos_chi2->Fill();
    }
    if(reftrigpass){
	    reftrig_histos_chi2->Fill();
    }
  }



  WriteOutputTree();

  return;

  
}

