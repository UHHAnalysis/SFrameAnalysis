// $Id: ZprimePostSelectionCycle.cxx,v 1.1 2012/12/07 14:21:51 peiffer Exp $

#include <iostream>

using namespace std;

// Local include(s):
#include "include/ZprimePostSelectionCycle.h"

ClassImp( ZprimePostSelectionCycle );

ZprimePostSelectionCycle::ZprimePostSelectionCycle()
    : AnalysisCycle()
{

    // constructor, declare additional variables that should be
    // obtained from the steering-xml file

    // set the integrated luminosity per bin for the lumi-yield control plots
    SetIntLumiPerBin(500.);

}

ZprimePostSelectionCycle::~ZprimePostSelectionCycle()
{
    // destructor
}

void ZprimePostSelectionCycle::BeginCycle() throw( SError )
{
    // Start of the job, general set-up and definition of
    // objects are done here

    // Important: first call BeginCycle of base class
    AnalysisCycle::BeginCycle();

    return;

}

void ZprimePostSelectionCycle::EndCycle() throw( SError )
{
    // clean-up, info messages and final calculations after the analysis


    // call the base cycle class for all standard methods
    AnalysisCycle::EndCycle();

    return;

}

void ZprimePostSelectionCycle::BeginInputData( const SInputData& id ) throw( SError )
{
    // declaration of histograms and selections

    // Important: first call BeginInputData of base class
    AnalysisCycle::BeginInputData( id );

    // -------------------- set up the selections ---------------------------

    // Chi2 reconstruction and discriminant 
    static Chi2Discriminator* m_chi2discr = new Chi2Discriminator();

    // Leading jet selection
    Selection* LeadingJetSelection = new Selection("LeadingJetSelection");
    LeadingJetSelection->addSelectionModule(new NJetSelection(1,int_infinity(),250,2.5));

    // Kinematic selection
    Selection* KinematicSelection = new Selection("KinematicSelection");
    KinematicSelection->addSelectionModule(new METCut(50));
    KinematicSelection->addSelectionModule(new HypothesisLeptopPtCut( m_chi2discr, 140.0, double_infinity()));

    Selection* Chi2Seletion = new Selection("Chi2Selection");
    Chi2Seletion->addSelectionModule(new HypothesisDiscriminatorCut( m_chi2discr, -1*double_infinity(), 10));

    Selection* BTagSelection = new Selection("BTagSelection");
    BTagSelection->addSelectionModule(new NBTagSelection(1)); //at least one b tag

    Selection* NoBTagSelection = new Selection("NoBTagSelection");
    NoBTagSelection->addSelectionModule(new NBTagSelection(0,0)); //no b tags

    //chi2_selection->addSelectionModule(new MttbarGenCut(0,700));

    Selection* TopTagSelection = new Selection("TopTagSelection");
    //DO NOT use trigger selection in PROOF mode at the moment
    //TopSel->addSelectionModule(new TriggerSelection("HLT_PFJet320_v"));
    TopTagSelection->addSelectionModule(new NTopJetSelection(1,int_infinity(),350,2.5));
    TopTagSelection->addSelectionModule(new NTopTagSelection(1,int_infinity()));

    RegisterSelection(LeadingJetSelection);
    RegisterSelection(KinematicSelection);
    RegisterSelection(TopTagSelection);
    RegisterSelection(Chi2Seletion);
    RegisterSelection(BTagSelection);
    RegisterSelection(NoBTagSelection);

    // ---------------- set up the histogram collections --------------------

    // histograms without any cuts
    RegisterHistCollection( new HypothesisHists("Chi2_Presel", m_chi2discr ) );

    RegisterHistCollection( new EventHists("Event_Presel") );
    RegisterHistCollection( new JetHists("Jets_Presel") );
    RegisterHistCollection( new ElectronHists("Electron_Presel") );
    RegisterHistCollection( new MuonHists("Muon_Presel") );
    RegisterHistCollection( new TauHists("Tau_Presel") );
    RegisterHistCollection( new TopJetHists("TopJets_Presel") );

    // histograms with leading jet, ....
    RegisterHistCollection( new HypothesisHists("Chi2_LJetsel", m_chi2discr ) );

    RegisterHistCollection( new EventHists("Event_LJetsel") );
    RegisterHistCollection( new JetHists("Jets_LJetsel") );
    RegisterHistCollection( new ElectronHists("Electron_LJetsel") );
    RegisterHistCollection( new MuonHists("Muon_LJetsel") );
    RegisterHistCollection( new TauHists("Tau_LJetsel") );
    RegisterHistCollection( new TopJetHists("TopJets_LJetsel") );

    // histogram with kinematic cuts
    RegisterHistCollection( new HypothesisHists("Chi2_Kinesel", m_chi2discr ) );

    RegisterHistCollection( new EventHists("Event_Kinesel") );
    RegisterHistCollection( new JetHists("Jets_Kinesel") );
    RegisterHistCollection( new ElectronHists("Electron_Kinesel") );
    RegisterHistCollection( new MuonHists("Muon_Kinesel") );
    RegisterHistCollection( new TauHists("Tau_Kinesel") );
    RegisterHistCollection( new TopJetHists("TopJets_Kinesel") );

    // histograms with chi2 cut
    RegisterHistCollection( new HypothesisHists("Chi2_Chi2sel", m_chi2discr ) );

    RegisterHistCollection( new EventHists("Event_Chi2sel") );
    RegisterHistCollection( new JetHists("Jets_Chi2sel") );
    RegisterHistCollection( new ElectronHists("Electron_Chi2sel") );
    RegisterHistCollection( new MuonHists("Muon_Chi2sel") );
    RegisterHistCollection( new TauHists("Tau_Chi2sel") );
    RegisterHistCollection( new TopJetHists("TopJets_Chi2sel") );

    // histograms with Btag and NoBtag and Chi2
    RegisterHistCollection( new HypothesisHists("Chi2_BTag", m_chi2discr ) );
    RegisterHistCollection( new HypothesisHists("Chi2_NoBTag", m_chi2discr ) );

    RegisterHistCollection( new EventHists("Event_BTag") );
    RegisterHistCollection( new JetHists("Jets_BTag") );
    RegisterHistCollection( new ElectronHists("Electron_BTag") );
    RegisterHistCollection( new MuonHists("Muon_BTag") );
    RegisterHistCollection( new TauHists("Tau_BTag") );
    RegisterHistCollection( new TopJetHists("TopJets_BTag") );

    RegisterHistCollection( new EventHists("Event_NoBTag") );
    RegisterHistCollection( new JetHists("Jets_NoBTag") );
    RegisterHistCollection( new ElectronHists("Electron_NoBTag") );
    RegisterHistCollection( new MuonHists("Muon_NoBTag") );
    RegisterHistCollection( new TauHists("Tau_NoBTag") );
    RegisterHistCollection( new TopJetHists("TopJets_NoBTag") );

    // histograms after the top selection
    RegisterHistCollection( new HypothesisHists("Chi2_TopTag", m_chi2discr ) );

    RegisterHistCollection( new EventHists("Event_TopTag") );
    RegisterHistCollection( new JetHists("Jets_TopTag") );
    RegisterHistCollection( new ElectronHists("Electron_TopTag") );
    RegisterHistCollection( new MuonHists("Muon_TopTag") );
    RegisterHistCollection( new TauHists("Tau_TopTag") );
    RegisterHistCollection( new TopJetHists("TopJets_TopTag") );

    // important: initialise histogram collections after their definition
    InitHistos();

    return;

}

void ZprimePostSelectionCycle::EndInputData( const SInputData& id ) throw( SError )
{
    AnalysisCycle::EndInputData( id );
    return;
}

void ZprimePostSelectionCycle::BeginInputFile( const SInputData& id ) throw( SError )
{
    // Connect all variables from the Ntuple file with the ones needed for the analysis
    // The variables are commonly stored in the BaseCycleContaincer

    // important: call to base function to connect all variables to Ntuples from the input tree
    AnalysisCycle::BeginInputFile( id );

    return;
}

void ZprimePostSelectionCycle::ExecuteEvent( const SInputData& id, Double_t weight) throw( SError )
{
    // this is the most important part: here the full analysis happens
    // user should implement selections, filling of histograms and results

    // first step: call Execute event of base class to perform basic consistency checks
    // also, the good-run selection is performed there and the calculator is reset
    AnalysisCycle::ExecuteEvent( id, weight );

    // get the selections
    static Selection* LeadingJetSelection = GetSelection("LeadingJetSelection");
    static Selection* KinematicSelection = GetSelection("KinematicSelection");
    static Selection* TopTagSelection = GetSelection("TopTagSelection");
    static Selection* Chi2Selection = GetSelection("Chi2Selection");
    static Selection* BTagSelection = GetSelection("BTagSelection");
    static Selection* NoBTagSelection = GetSelection("NoBTagSelection");

    // get the histogram collections
    BaseHists* Chi2_HistsPresel = GetHistCollection("Chi2_Presel");
    BaseHists* Chi2_HistsLJetsel = GetHistCollection("Chi2_LJetsel");
    BaseHists* Chi2_HistsKinesel = GetHistCollection("Chi2_Kinesel");
    BaseHists* Chi2_HistsChi2sel = GetHistCollection("Chi2_Chi2sel");
    BaseHists* Chi2_HistsBTag = GetHistCollection("Chi2_BTag");
    BaseHists* Chi2_HistsNoBTag = GetHistCollection("Chi2_NoBTag");
    BaseHists* Chi2_HistsTopTag = GetHistCollection("Chi2_TopTag");

    EventCalc* calc = EventCalc::Instance();
    if(calc->GetJets()->size()>=12) {
        std::cout << "run: " << calc->GetRunNum() << "   lb: " << calc->GetLumiBlock() << "  event: " << calc->GetEventNum() << "   N(jets): " << calc->GetJets()->size() << std::endl;
    }

    Chi2_HistsPresel->Fill();
    FillControlHistos("_Presel");

    if(!LeadingJetSelection->passSelection())  throw SError( SError::SkipEvent );

    Chi2_HistsLJetsel->Fill();
    FillControlHistos("_LJetsel");

    if(!KinematicSelection->passSelection())  throw SError( SError::SkipEvent );

    Chi2_HistsKinesel->Fill();
    FillControlHistos("_Kinesel");

    if(TopTagSelection->passSelection()) {
        Chi2_HistsTopTag->Fill();
        FillControlHistos("_TopTag");
    }

    if(!Chi2Selection->passSelection()) throw SError( SError::SkipEvent ); 

    Chi2_HistsChi2sel->Fill();
    FillControlHistos("_Chi2sel");

    if(BTagSelection->passSelection()) {
        Chi2_HistsBTag->Fill();
        FillControlHistos("_BTag");
    }
    if(NoBTagSelection->passSelection()) {
        Chi2_HistsNoBTag->Fill();
        FillControlHistos("_NoBTag");
    }

    return;
}


void ZprimePostSelectionCycle::FillControlHistos(TString postfix)
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
