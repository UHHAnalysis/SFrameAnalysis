#include <iostream>

using namespace std;

// Local include(s):
#include "include/ZprimeEleTrigCycle.h"

ClassImp( ZprimeEleTrigCycle );

ZprimeEleTrigCycle::ZprimeEleTrigCycle()
    : AnalysisCycle()
{

    // constructor, declare additional variables that should be
    // obtained from the steering-xml file

    // set the integrated luminosity per bin for the lumi-yield control plots
    SetIntLumiPerBin(25.);

    m_sys_var = e_Default;
    m_sys_unc = e_None;

    m_mttgencut = false;

    DeclareProperty( "Electron_Or_Muon_Selection", m_Electron_Or_Muon_Selection );

    //default: no btagging cuts applied, other cuts can be defined in config file
    m_Nbtags_min=0;
    m_Nbtags_max=int_infinity();
    DeclareProperty( "Nbtags_min", m_Nbtags_min);
    DeclareProperty( "Nbtags_max", m_Nbtags_max);
    DeclareProperty( "ApplyMttbarGenCut", m_mttgencut );

    // steering property for data-driven qcd in electron channel
    m_reversed_electron_selection = false;
    DeclareProperty( "ReversedElectronSelection", m_reversed_electron_selection);
}

ZprimeEleTrigCycle::~ZprimeEleTrigCycle()
{
    // destructor
}

void ZprimeEleTrigCycle::BeginCycle() throw( SError )
{
    // Start of the job, general set-up and definition of
    // objects are done here

    // Important: first call BeginCycle of base class
    AnalysisCycle::BeginCycle();

    return;
}

void ZprimeEleTrigCycle::EndCycle() throw( SError )
{
    // clean-up, info messages and final calculations after the analysis


    // call the base cycle class for all standard methods
    // and a summary of the made selections
    AnalysisCycle::EndCycle();

    return;
}

void ZprimeEleTrigCycle::BeginInputData( const SInputData& id ) throw( SError )
{
    // declaration of histograms and selections

    // Important: first call BeginInputData of base class
    AnalysisCycle::BeginInputData( id );

    // -------------------- set up the selections ---------------------------

    //Set-Up Selection

    static Selection* mttbar_gen_selection = new Selection("Mttbar_Gen_Selection");
    if ( m_mttgencut && ((id.GetVersion() == "TTbar_0to700") || (id.GetVersion() == "TTbar") )  ) {
      m_logger << INFO << "Applying mttbar generator cut from 0 to 700 GeV." << SLogger::endmsg;
      mttbar_gen_selection->addSelectionModule(new MttbarGenCut(0,700));
      mttbar_gen_selection->EnableSelection();
    } else {
      m_logger << INFO << "Disabling mttbar generator cut." << SLogger::endmsg;
      mttbar_gen_selection->DisableSelection();
    }

    // muon or electron?
    bool doMu=false;
    if(m_Electron_Or_Muon_Selection=="Muon" || m_Electron_Or_Muon_Selection=="Muons" || m_Electron_Or_Muon_Selection=="Mu" || m_Electron_Or_Muon_Selection=="MU") {
        doMu=true;
    } else {
        m_logger << ERROR << "Electron Trigger Cycle needs to be run with muon selection!" << SLogger::endmsg;
    }      
    Selection* std_mu_selection= new Selection("std_mu_selection");

    if(doMu) {
      std_mu_selection->addSelectionModule(new TriggerSelection(m_lumi_trigger));
    }

    std_mu_selection->addSelectionModule(new NPrimaryVertexSelection(1)); //at least one good PV
    std_mu_selection->addSelectionModule(new NJetSelection(2,int_infinity(),50,2.4));//at least two jets

    // standard muon selection
    std_mu_selection->addSelectionModule(new NMuonSelection(1,int_infinity()));//at least one muon
    std_mu_selection->addSelectionModule(new NMuonSelection(1,1));//exactly one muon
    std_mu_selection->addSelectionModule(new TwoDCutMuon());

    // this is different now: don't veto on electron 
    std_mu_selection->addSelectionModule(new NElectronSelection(0,1)); // 0 or 1 electron

    // second part of standard cuts 
    Selection* std_mu_selection2 = new Selection("second_std_mu_selection");
    std_mu_selection2->addSelectionModule(new NJetSelection(1,int_infinity(),150,2.5));//leading jet with pt>150 GeV
    std_mu_selection2->addSelectionModule(new NJetSelection(2,int_infinity(), 50,2.5));//second leading jet with pt>50 GeV
    std_mu_selection2->addSelectionModule(new NBTagSelection(m_Nbtags_min,m_Nbtags_max)); //b tags from config file
    std_mu_selection2->addSelectionModule(new HTlepCut(150));
    std_mu_selection2->addSelectionModule(new METCut(20));

    // triangular cuts for electron selection
    Selection* triangularcut_selection= new Selection("triangularcut_selection");
    triangularcut_selection->addSelectionModule(new NElectronSelection(1,1)); // exactly one electron
    triangularcut_selection->addSelectionModule(new TriangularCut()); // triangular cuts
    triangularcut_selection->addSelectionModule(new MuonElectronOSCut()); // additional cut: OS for ele+muon

    Selection* ele_trig_selection = new Selection("electron_trigger");
    ele_trig_selection->addSelectionModule(new TriggerSelection("HLT_Ele30_CaloIdVT_TrkIdT_PFNoPUJet100_PFNoPUJet25_v"));
                                                                
    // chi2 selection
    //Selection* chi2_selection= new Selection("chi2_selection");
    //m_chi2discr = new Chi2Discriminator();
    //chi2_selection->addSelectionModule(new HypothesisDiscriminatorCut( m_chi2discr, -1*double_infinity(), 10));

    RegisterSelection(mttbar_gen_selection);
    RegisterSelection(std_mu_selection);
    RegisterSelection(std_mu_selection2);
    RegisterSelection(triangularcut_selection);
    RegisterSelection(ele_trig_selection);

    if (GetSysUncName()=="JEC" || GetSysUncName()=="jec") m_sys_unc = e_JEC; 
    if (GetSysUncName()=="JER" || GetSysUncName()=="jer") m_sys_unc = e_JER;
    if (m_sys_unc != e_None){
      if (GetSysShiftName()=="UP" || GetSysShiftName()=="up" || GetSysShiftName()=="Up") m_sys_var = e_Up; 
      if (GetSysShiftName()=="DOWN" || GetSysShiftName()=="down" || GetSysShiftName()=="Down") m_sys_var = e_Down; 
    }    

    // ---------------- set up the histogram collections --------------------

    // histograms without any cuts
    //RegisterHistCollection( new HypothesisHists("Chi2", m_chi2discr) );

    // control histogras
    RegisterHistCollection( new EventHists("Event_Presel") );
    RegisterHistCollection( new JetHists("Jets_Presel") );
    RegisterHistCollection( new ElectronHists("Electron_Presel") );
    RegisterHistCollection( new MuonHists("Muon_Presel") );
    RegisterHistCollection( new TauHists("Tau_Presel") );
    RegisterHistCollection( new TopJetHists("TopJets_Presel") );
    
    RegisterHistCollection( new EventHists("Event_Muonsel") );
    RegisterHistCollection( new JetHists("Jets_Muonsel") );
    RegisterHistCollection( new ElectronHists("Electron_Muonsel") );
    RegisterHistCollection( new MuonHists("Muon_Muonsel") );
    RegisterHistCollection( new TauHists("Tau_Muonsel") );
    RegisterHistCollection( new TopJetHists("TopJets_Muonsel") );

    RegisterHistCollection( new EventHists("Event_Electronsel") );
    RegisterHistCollection( new JetHists("Jets_Electronsel") );
    RegisterHistCollection( new ElectronHists("Electron_Electronsel") );
    RegisterHistCollection( new MuonHists("Muon_Electronsel") );
    RegisterHistCollection( new TauHists("Tau_Electronsel") );
    RegisterHistCollection( new TopJetHists("TopJets_Electronsel") );

    RegisterHistCollection( new EventHists("Event_Electrontrig") );
    RegisterHistCollection( new JetHists("Jets_Electrontrig") );
    RegisterHistCollection( new ElectronHists("Electron_Electrontrig") );
    RegisterHistCollection( new MuonHists("Muon_Electrontrig") );
    RegisterHistCollection( new TauHists("Tau_Electrontrig") );
    RegisterHistCollection( new TopJetHists("TopJets_Electrontrig") );

    RegisterHistCollection( new EventHists("Event_ElectrontrigWithWeight") );
    RegisterHistCollection( new JetHists("Jets_ElectrontrigWithWeight") );
    RegisterHistCollection( new ElectronHists("Electron_ElectrontrigWithWeight") );
    RegisterHistCollection( new MuonHists("Muon_ElectrontrigWithWeight") );
    RegisterHistCollection( new TauHists("Tau_ElectrontrigWithWeight") );
    RegisterHistCollection( new TopJetHists("TopJets_ElectrontrigWithWeight") );

    // important: initialise histogram collections after their definition
    InitHistos();

    return;

}

void ZprimeEleTrigCycle::EndInputData( const SInputData& id ) throw( SError )
{

    AnalysisCycle::EndInputData( id );

    return;
}

void ZprimeEleTrigCycle::BeginInputFile( const SInputData& id ) throw( SError )
{
    // Connect all variables from the Ntuple file with the ones needed for the analysis
    // The variables are commonly stored in the BaseCycleContaincer

    // important: call to base function to connect all variables to Ntuples from the input tree
    AnalysisCycle::BeginInputFile( id );

    return;
}

void ZprimeEleTrigCycle::ExecuteEvent( const SInputData& id, Double_t weight) throw( SError )
{
    // this is the most important part: here the full analysis happens
    // user should implement selections, filling of histograms and results

    // first step: call Execute event of base class to perform basic consistency checks
    // also, the good-run selection is performed there and the calculator is reset


    // instead of calling ExecuteEvent from the analysis base cycle, do the individual steps here
    // such that the trigger weights can be applied only later in the intermediate steps
    // -> implement the needed functionality of AnalysisCycle::ExecuteEvent( id, weight); in the following:

    // first thing to do: call reset of event calc
    EventCalc* calc = EventCalc::Instance();
    BaseCycleContainer* bcc = calc->GetBaseCycleContainer();
    calc->Reset();

    // look at period A+B only
    if (calc->GetRunNum()<196600) return; 

    if(bcc->isRealData && m_addGenInfo) {
        m_logger << WARNING<< "Running over real data, but addGenInfo=True?!" << SLogger::endmsg;
    }
    
    FillTriggerNames();

    // generate random run Nr for MC samples (consider luminosity of each run)
    // e.g. for proper OTX cut in MC, and needs to be done only once per event
    if( !bcc->isRealData && LumiHandler()->IsLumiCalc() ) {
        bcc->run = LumiHandler()->GetRandomRunNr();
    }
    //set the lumiweight to 1 for data
    if(bcc->isRealData) weight = 1;
    // store the weight (lumiweight) in the eventcalc class and use it
    calc -> ProduceWeight(weight);

    if(bcc->genInfo) {
      std::vector<float> genweights =  bcc->genInfo->weights();
      for(unsigned int i=0; i< genweights.size(); ++i) {
	calc -> ProduceWeight(genweights[i]);
      }
      if(m_puwp) {
	double pu_weight = m_puwp->produceWeight(bcc->genInfo);
	// set the weight in the eventcalc
	calc -> ProduceWeight(pu_weight);
      }
    }

    //select only good runs
    if(bcc->isRealData && LumiHandler()->IsLumiCalc() ) {
        if( !LumiHandler()->PassGoodRunsList( bcc->run, bcc->luminosityBlock )) throw SError( SError::SkipEvent );
    }
    
    // now start the actual implementation of the electron trigger efficiency
    static Selection* mttbar_gen_selection = GetSelection("Mttbar_Gen_Selection");
    if(!mttbar_gen_selection->passSelection())  throw SError( SError::SkipEvent );

    // muon efficiency weights 
    if (!calc->IsRealData()){
      calc->ProduceWeight(m_lsf->GetMuonWeight());
    }

    // control histograms
    FillControlHists("_Presel");

    static Selection* std_mu_selection = GetSelection("std_mu_selection");
    static Selection* std_mu_selection2 = GetSelection("second_std_mu_selection");
    static Selection* triangularcut_selection = GetSelection("triangularcut_selection");
    static Selection* ele_trig_selection = GetSelection("electron_trigger");

    m_cleaner = new Cleaner();
    m_cleaner->SetJECUncertainty(m_jes_unc);

    // settings for jet correction uncertainties
    if (m_sys_unc==e_JEC){
      if (m_sys_var==e_Up) m_cleaner->ApplyJECVariationUp();
      if (m_sys_var==e_Down) m_cleaner->ApplyJECVariationDown();
    }
    if (m_sys_unc==e_JER){
      if (m_sys_var==e_Up) m_cleaner->ApplyJERVariationUp();
      if (m_sys_var==e_Down) m_cleaner->ApplyJERVariationDown();
    }

    if(bcc->pvs)  m_cleaner->PrimaryVertexCleaner(4, 24., 2.);
    if(bcc->electrons) m_cleaner->ElectronCleaner_noIso(35,2.5, m_reversed_electron_selection);
    if(bcc->muons) m_cleaner->MuonCleaner_noIso(45,2.1);
    if(bcc->jets) m_cleaner->JetLeptonSubtractor(m_corrector,false);
    if(!bcc->isRealData && bcc->jets) m_cleaner->JetEnergyResolutionShifter();

    // apply loose jet cleaning for 2D cut
    if(bcc->jets) m_cleaner->JetCleaner(25,double_infinity(),true);
    if(!std_mu_selection->passSelection())  throw SError( SError::SkipEvent );

    //apply tight jet cleaning
    if(bcc->jets) m_cleaner->JetCleaner(30,2.5,true);

    //remove all taus from collection for HTlep calculation
    if(bcc->taus) m_cleaner->TauCleaner(double_infinity(),0.0);


    //
    // --------- standard muon selection with electrons allowed ------------
    // 
    if(!std_mu_selection2->passSelection())  throw SError( SError::SkipEvent );
    FillControlHists("_Muonsel");     // control histograms for standard muon selection with 0 or 1 electrons


    //
    // --------- one electron with triangular cut ------------
    // 
    if(!triangularcut_selection->passSelection())  throw SError( SError::SkipEvent );
    FillControlHists("_Electronsel");

    //
    // --------- electron trigger selection ------------
    // 
    if(!ele_trig_selection->passSelection())  throw SError( SError::SkipEvent );
    FillControlHists("_Electrontrig");

    // electron efficiency weights
    if (!calc->IsRealData()){
      calc->ProduceWeight(m_lsf->GetElectronWeight());
    }
    FillControlHists("_ElectrontrigWithWeight");


    //do reconstruction here

    //calc->FillHighMassTTbarHypotheses();
    //if(!chi2_selection->passSelection())  throw SError( SError::SkipEvent );
    // ReconstructionHypothesis *hyp = m_chi2discr->GetBestHypothesis();
    // BaseHists* Chi2Hists = GetHistCollection("Chi2");
    // Chi2Hists->Fill();

    WriteOutputTree();

    return;
}

void ZprimeEleTrigCycle::FillControlHists(TString postfix)
{
    // fill some control histograms, need to be defined in BeginInputData

    BaseHists* eventhists = GetHistCollection((std::string)("Event"+postfix));
    BaseHists* jethists = GetHistCollection((std::string)("Jets"+postfix));
    BaseHists* elehists = GetHistCollection((std::string)("Electron"+postfix));
    BaseHists* muonhists = GetHistCollection((std::string)("Muon"+postfix));
    BaseHists* tauhists = GetHistCollection((std::string)("Tau"+postfix));
    BaseHists* topjethists = GetHistCollection((std::string)("TopJets"+postfix));

    eventhists->Fill();
    jethists->Fill();
    elehists->Fill();
    muonhists->Fill();
    tauhists->Fill();
    topjethists->Fill();

}

