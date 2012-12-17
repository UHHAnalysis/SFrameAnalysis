#include <iostream>

using namespace std;

// Local include(s):
#include "include/ZprimePreSelectionCycle.h"
#include "include/SelectionModules.h"
#include "include/ExampleHists.h"
#include "include/ObjectHandler.h"
#include "JetCorrectorParameters.h"

ClassImp( ZprimePreSelectionCycle );

ZprimePreSelectionCycle::ZprimePreSelectionCycle()
    : AnalysisCycle()
{

    // constructor, declare additional variables that should be
    // obtained from the steering-xml file

    // set the integrated luminosity per bin for the lumi-yield control plots
    SetIntLumiPerBin(25.);

    m_corrector = NULL;

    DeclareProperty( "Electron_Or_Muon_Selection", m_Electron_Or_Muon_Selection );
}

ZprimePreSelectionCycle::~ZprimePreSelectionCycle()
{
    // destructor
    if (m_corrector) delete m_corrector;
}

void ZprimePreSelectionCycle::BeginCycle() throw( SError )
{
    // Start of the job, general set-up and definition of
    // objects are done here

    // Important: first call BeginCycle of base class
    AnalysisCycle::BeginCycle();

    return;

}

void ZprimePreSelectionCycle::EndCycle() throw( SError )
{
    // clean-up, info messages and final calculations after the analysis


    // call the base cycle class for all standard methods
    // and a summary of the made selections
    AnalysisCycle::EndCycle();

    return;

}

void ZprimePreSelectionCycle::BeginInputData( const SInputData& id ) throw( SError )
{
    // declaration of histograms and selections

    // Important: first call BeginInputData of base class
    AnalysisCycle::BeginInputData( id );

    // -------------------- set up the selections ---------------------------

    //Selection* preselection = new Selection("pre-selection");
    Selection* preselection = new Selection("pre-selection");

    if(m_Electron_Or_Muon_Selection=="Electrons" || m_Electron_Or_Muon_Selection=="Electron" || m_Electron_Or_Muon_Selection=="Ele" || m_Electron_Or_Muon_Selection=="ELE") {
        preselection->addSelectionModule(new NElectronSelection(1,int_infinity(),0.,double_infinity(),false));//at least one electron
        preselection->addSelectionModule(new NMuonSelection(0,0));//no muons
    } else if(m_Electron_Or_Muon_Selection=="Muon" || m_Electron_Or_Muon_Selection=="Muons" || m_Electron_Or_Muon_Selection=="Mu" || m_Electron_Or_Muon_Selection=="MU") {
        preselection->addSelectionModule(new NElectronSelection(0,0));//no electron
        preselection->addSelectionModule(new NMuonSelection(1,int_infinity()));//at least one muon
    } else {
        m_logger << ERROR << "Electron_Or_Muon_Selection is not defined in your xml config file --- should be either `ELE` or `MU`" << SLogger::endmsg;
    }

    preselection->addSelectionModule(new NJetSelection(2));//at least two jets

    RegisterSelection(preselection);

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

    return;

}

void ZprimePreSelectionCycle::EndInputData( const SInputData& id ) throw( SError )
{
    AnalysisCycle::EndInputData( id );

    return;

}

void ZprimePreSelectionCycle::BeginInputFile( const SInputData& id ) throw( SError )
{
    // Connect all variables from the Ntuple file with the ones needed for the analysis
    // The variables are commonly stored in the BaseCycleContaincer

    // important: call to base function to connect all variables to Ntuples from the input tree
    AnalysisCycle::BeginInputFile( id );

    return;

}

void ZprimePreSelectionCycle::ExecuteEvent( const SInputData& id, Double_t weight) throw( SError )
{
    // this is the most important part: here the full analysis happens
    // user should implement selections, filling of histograms and results

    // first step: call Execute event of base class to perform basic consistency checks
    // also, the good-run selection is performed there and the calculator is reset
    AnalysisCycle::ExecuteEvent( id, weight);

    Cleaner cleaner;
    static Selection* preselection = GetSelection("pre-selection");

    ObjectHandler* objs = ObjectHandler::Instance();
    BaseCycleContainer* bcc = objs->GetBaseCycleContainer();

    //save uncleaned jet collection and MET to be stored in output
    std::vector<Jet> uncleaned_jets;
    for(unsigned int i=0; i<bcc->jets->size(); ++i) {
        uncleaned_jets.push_back(bcc->jets->at(i));
    }
    MET uncleaned_met = *bcc->met;

    //clean collections here

    if(bcc->muons) cleaner.MuonCleaner_noIso(45,2.1);
    if(bcc->electrons) cleaner.ElectronCleaner_noIso(35,2.5);
    if(bcc->jets) cleaner.JetLeptonSubtractor(m_corrector,false);
    if(!bcc->isRealData && bcc->jets) cleaner.JetEnergyResolutionShifter();
    if(bcc->jets) cleaner.JetCleaner(30,2.5,true);

    // get the selections

    if(!preselection->passSelection())  throw SError( SError::SkipEvent );

    //fill the uncleaned collections back to bcc to store them in output tree
    bcc->met->set_pt (uncleaned_met.pt());
    bcc->met->set_phi (uncleaned_met.phi());
    bcc->jets->clear();
    for(unsigned int i=0; i<uncleaned_jets.size(); ++i) {
        bcc->jets->push_back(uncleaned_jets.at(i));
    }

    WriteOutputTree();

    return;


}

