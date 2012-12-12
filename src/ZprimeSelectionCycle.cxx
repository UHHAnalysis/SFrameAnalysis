#include <iostream>

using namespace std;

// Local include(s):
#include "include/ZprimeSelectionCycle.h"

ClassImp( ZprimeSelectionCycle );

ZprimeSelectionCycle::ZprimeSelectionCycle()
    : AnalysisCycle()
{

    // constructor, declare additional variables that should be
    // obtained from the steering-xml file

    // set the integrated luminosity per bin for the lumi-yield control plots
    SetIntLumiPerBin(25.);

    m_corrector = NULL;

    DeclareProperty( "Electron_Or_Muon_Selection", m_Electron_Or_Muon_Selection );

    //default: no btagging cuts applied, other cuts can be defined in config file
    m_Nbtags_min=0;
    m_Nbtags_max=int_infinity();
    DeclareProperty( "Nbtags_min", m_Nbtags_min);
    DeclareProperty( "Nbtags_max", m_Nbtags_max);
}

ZprimeSelectionCycle::~ZprimeSelectionCycle()
{
    // destructor
    if (m_corrector) delete m_corrector;
}

void ZprimeSelectionCycle::BeginCycle() throw( SError )
{
    // Start of the job, general set-up and definition of
    // objects are done here

    // Important: first call BeginCycle of base class
    AnalysisCycle::BeginCycle();

    return;
}

void ZprimeSelectionCycle::EndCycle() throw( SError )
{
    // clean-up, info messages and final calculations after the analysis


    // call the base cycle class for all standard methods
    // and a summary of the made selections
    AnalysisCycle::EndCycle();

    return;
}

void ZprimeSelectionCycle::BeginInputData( const SInputData& id ) throw( SError )
{
    // declaration of histograms and selections

    // Important: first call BeginInputData of base class
    AnalysisCycle::BeginInputData( id );

    // -------------------- set up the selections ---------------------------

    //Set-Up Selection

    Selection* first_selection= new Selection("first_selection");

    bool doEle=false;
    bool doMu=false;

    if(m_Electron_Or_Muon_Selection=="Electrons" || m_Electron_Or_Muon_Selection=="Electron" || m_Electron_Or_Muon_Selection=="Ele" || m_Electron_Or_Muon_Selection=="ELE") {
        doEle=true;
    } else if(m_Electron_Or_Muon_Selection=="Muon" || m_Electron_Or_Muon_Selection=="Muons" || m_Electron_Or_Muon_Selection=="Mu" || m_Electron_Or_Muon_Selection=="MU") {
        doMu=true;
    } else {
        m_logger << ERROR << "Electron_Or_Muon_Selection is not defined in your xml config file --- should be either `ELE` or `MU`" << SLogger::endmsg;
    }

    if(doEle)
        first_selection->addSelectionModule(new TriggerSelection(m_lumi_trigger));
    if(doMu)
        first_selection->addSelectionModule(new TriggerSelection(m_lumi_trigger));

    first_selection->addSelectionModule(new NPrimaryVertexSelection(1)); //at least one good PV
    first_selection->addSelectionModule(new NJetSelection(2,int_infinity(),50,2.4));//at least two jets


    if(doEle) {
        first_selection->addSelectionModule(new NElectronSelection(1,int_infinity()));//at least one electron
        first_selection->addSelectionModule(new NElectronSelection(1,1));//exactly one electron
        first_selection->addSelectionModule(new NMuonSelection(0,0));//no muons
    }
    if(doMu) {
        first_selection->addSelectionModule(new NMuonSelection(1,int_infinity()));//at least one muon
        first_selection->addSelectionModule(new NMuonSelection(1,1));//exactly one muon
        first_selection->addSelectionModule(new NElectronSelection(0,0));//no electrons
    }

    first_selection->addSelectionModule(new TwoDCut());


    Selection* second_selection= new Selection("second_selection");

    second_selection->addSelectionModule(new NJetSelection(1,int_infinity(),150,2.5));//leading jet with pt>150 GeV
    second_selection->addSelectionModule(new NBTagSelection(m_Nbtags_min,m_Nbtags_max)); //b tags from config file
    second_selection->addSelectionModule(new HTlepCut(150));
    if(doEle) {
        //if(!m_reversed_electron_selection)
            second_selection->addSelectionModule(new TriangularCut());
        //else
            // second_selection->addSelectionModule(new TriangularCut_reverse());
    }
    second_selection->addSelectionModule(new METCut(20));

    Selection* chi2_selection= new Selection("chi2_selection");
    m_chi2discr = new Chi2Discriminator();
    chi2_selection->addSelectionModule(new HypothesisDiscriminatorCut( m_chi2discr, -1*double_infinity(), 10));

    m_bpdiscr = new BestPossibleDiscriminator();
    m_sumdrdiscr = new SumDeltaRDiscriminator();
    m_cmdiscr = new CorrectMatchDiscriminator();

    Selection* matchable_selection = new Selection("matchable_selection");
    matchable_selection->addSelectionModule(new HypothesisDiscriminatorCut( m_cmdiscr, -1*double_infinity(), 999));

    RegisterSelection(first_selection);
    RegisterSelection(second_selection);
    RegisterSelection(chi2_selection);
    RegisterSelection(matchable_selection);

    std::vector<JetCorrectorParameters> pars;

    //see https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookJetEnergyCorrections#GetTxtFiles how to get the txt files with jet energy corrections from the database
    if(!addGenInfo()) {
        pars.push_back(JetCorrectorParameters(m_JECFileLocation + "/" + m_JECDataGlobalTag + "_L1FastJet_" + m_JECJetCollection + ".txt"));
        pars.push_back(JetCorrectorParameters(m_JECFileLocation + "/" + m_JECDataGlobalTag + "_L2Relative_" + m_JECJetCollection + ".txt"));
        pars.push_back(JetCorrectorParameters(m_JECFileLocation + "/" + m_JECDataGlobalTag + "_L3Absolute_" + m_JECJetCollection + ".txt"));
        pars.push_back(JetCorrectorParameters(m_JECFileLocation + "/" + m_JECDataGlobalTag + "_L2L3Residual_" + m_JECJetCollection + ".txt"));
    } else {
        pars.push_back(JetCorrectorParameters(m_JECFileLocation + "/" + m_JECMCGlobalTag + "_L1FastJet_" + m_JECJetCollection + ".txt"));
        pars.push_back(JetCorrectorParameters(m_JECFileLocation + "/" + m_JECMCGlobalTag + "_L2Relative_" + m_JECJetCollection + ".txt"));
        pars.push_back(JetCorrectorParameters(m_JECFileLocation + "/" + m_JECMCGlobalTag + "_L3Absolute_" + m_JECJetCollection + ".txt"));
    }

    m_corrector = new FactorizedJetCorrector(pars);

    // ---------------- set up the histogram collections --------------------

    // histograms without any cuts
    RegisterHistCollection( new HypothesisHists("Chi2", m_chi2discr) );
    RegisterHistCollection( new HypothesisHists("BestPossible", m_bpdiscr) );
    RegisterHistCollection( new HypothesisHists("SumDeltaR", m_sumdrdiscr) );
    RegisterHistCollection( new HypothesisHists("CorrectMatch", m_cmdiscr) );

    // control histogras
    RegisterHistCollection( new EventHists("Event_Presel") );
    RegisterHistCollection( new JetHists("Jets_Presel") );
    RegisterHistCollection( new ElectronHists("Electron_Presel") );
    RegisterHistCollection( new MuonHists("Muon_Presel") );
    RegisterHistCollection( new TauHists("Tau_Presel") );
    RegisterHistCollection( new TopJetHists("TopJets_Presel") );
    
    RegisterHistCollection( new EventHists("Event_Cleaned") );
    RegisterHistCollection( new JetHists("Jets_Cleaned") );
    RegisterHistCollection( new ElectronHists("Electron_Cleaned") );
    RegisterHistCollection( new MuonHists("Muon_Cleaned") );
    RegisterHistCollection( new TauHists("Tau_Cleaned") );
    RegisterHistCollection( new TopJetHists("TopJets_Cleaned") );

    RegisterHistCollection( new EventHists("Event_Postsel") );
    RegisterHistCollection( new JetHists("Jets_Postsel") );
    RegisterHistCollection( new ElectronHists("Electron_Postsel") );
    RegisterHistCollection( new MuonHists("Muon_Postsel") );
    RegisterHistCollection( new TauHists("Tau_Postsel") );
    RegisterHistCollection( new TopJetHists("TopJets_Postsel") );

    // important: initialise histogram collections after their definition
    InitHistos();

    m_bp_chi2 = new HypothesisStatistics("b.p. vs. Chi2");
    m_bp_sumdr = new HypothesisStatistics("b.p. vs. SumDR");
    m_cm_chi2 = new HypothesisStatistics("matched vs. Chi2");
    m_cm_sumdr = new HypothesisStatistics("matched vs. SumDR");
    m_cm_bp = new HypothesisStatistics("matched vs. b.p.");

    return;

}

void ZprimeSelectionCycle::EndInputData( const SInputData& id ) throw( SError )
{

    AnalysisCycle::EndInputData( id );

    m_bp_chi2->PrintStatistics();
    m_bp_sumdr->PrintStatistics();
    m_cm_chi2->PrintStatistics();
    m_cm_sumdr->PrintStatistics();
    m_cm_bp->PrintStatistics();

    return;
}

void ZprimeSelectionCycle::BeginInputFile( const SInputData& id ) throw( SError )
{
    // Connect all variables from the Ntuple file with the ones needed for the analysis
    // The variables are commonly stored in the BaseCycleContaincer

    // important: call to base function to connect all variables to Ntuples from the input tree
    AnalysisCycle::BeginInputFile( id );

    return;
}

void ZprimeSelectionCycle::ExecuteEvent( const SInputData& id, Double_t weight) throw( SError )
{
    // this is the most important part: here the full analysis happens
    // user should implement selections, filling of histograms and results

    // first step: call Execute event of base class to perform basic consistency checks
    // also, the good-run selection is performed there and the calculator is reset
    AnalysisCycle::ExecuteEvent( id, weight);

    // control histograms
    FillControlHists("_Presel");

    static Selection* first_selection = GetSelection("first_selection");
    static Selection* second_selection = GetSelection("second_selection");
    static Selection* chi2_selection = GetSelection("chi2_selection");
    static Selection* matchable_selection = GetSelection("matchable_selection");

    m_cleaner = new Cleaner();

    ObjectHandler* objs = ObjectHandler::Instance();
    BaseCycleContainer* bcc = objs->GetBaseCycleContainer();
    EventCalc* calc = EventCalc::Instance();


    if(bcc->pvs)  m_cleaner->PrimaryVertexCleaner(4, 24., 2.);
    if(bcc->electrons) {
        if(!m_reversed_electron_selection) m_cleaner->ElectronCleaner_noIso(35,2.5);
        else m_cleaner->ElectronCleaner_noIso_reverse(35,2.5);
    }
    if(bcc->muons) m_cleaner->MuonCleaner_noIso(45,2.1);
    if(bcc->jets) m_cleaner->JetLeptonSubtractor(m_corrector,false);

    if(!bcc->isRealData && bcc->jets) m_cleaner->JetEnergyResolutionShifter();
    //apply loose jet cleaning for 2D cut
    if(bcc->jets) m_cleaner->JetCleaner(25,double_infinity(),true);

    // control histograms
    FillControlHists("_Cleaned");

    if(!first_selection->passSelection())  throw SError( SError::SkipEvent );

    //apply tighter jet cleaning for further cuts and analysis steps
    if(bcc->jets) m_cleaner->JetCleaner(50,2.5,true);

    //remove all taus from collection for HTlep calculation
    if(bcc->taus) m_cleaner->TauCleaner(double_infinity(),0.0);

    if(!second_selection->passSelection())  throw SError( SError::SkipEvent );

    //do reconstruction here

    calc->FillHighMassTTbarHypotheses();

    m_chi2discr->FillDiscriminatorValues();
    m_bpdiscr->FillDiscriminatorValues();
    m_sumdrdiscr->FillDiscriminatorValues();
    m_cmdiscr->FillDiscriminatorValues();

    //if(!chi2_selection->passSelection())  throw SError( SError::SkipEvent );
    //if(!matchable_selection->passSelection())  throw SError( SError::SkipEvent );

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

    ReconstructionHypothesis *bp_hyp = m_bpdiscr->GetBestHypothesis();
    //double bp_mttbar =  (bp_hyp->toplep_v4()+bp_hyp->tophad_v4()).M();
    //std::cout << "               bp mttbar : " << bp_mttbar << "   bp deltaR : " << bp_hyp->discriminator("BestPossible") << std::endl;

    ReconstructionHypothesis *sdr_hyp = m_sumdrdiscr->GetBestHypothesis();
    ReconstructionHypothesis *cm_hyp = m_cmdiscr->GetBestHypothesis();

    // control histograms
    FillControlHists("_Postsel");

    // get the histogram collections
    BaseHists* Chi2Hists = GetHistCollection("Chi2");
    BaseHists* BPHists = GetHistCollection("BestPossible");
    BaseHists* SumDRHists = GetHistCollection("SumDeltaR");
    BaseHists* CorrectMatchHists = GetHistCollection("CorrectMatch");

    Chi2Hists->Fill();
    BPHists->Fill();
    SumDRHists->Fill();
    if(cm_hyp) CorrectMatchHists->Fill();

    m_bp_chi2->FillHyps(bp_hyp,hyp);
    m_bp_sumdr->FillHyps(bp_hyp,sdr_hyp);


    if(cm_hyp) {
        m_cm_chi2->FillHyps(cm_hyp,hyp);
        m_cm_sumdr->FillHyps(cm_hyp,sdr_hyp);
        m_cm_bp->FillHyps(cm_hyp, bp_hyp);
    }

    //calc->PrintEventContent();

    WriteOutputTree();

    return;


}

void ZprimeSelectionCycle::FillControlHists(TString postfix)
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
