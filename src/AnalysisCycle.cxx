// $Id: AnalysisCycle.cxx,v 1.19 2012/12/05 13:06:05 bazterra Exp $

#include <iostream>

using namespace std;

// Local include(s):
#include "include/AnalysisCycle.h"
#include "include/SelectionModules.h"
#include "include/ObjectHandler.h"
#include "include/EventCalc.h"
#include "STreeType.h"

ClassImp( AnalysisCycle );

AnalysisCycle::AnalysisCycle()
    : SCycleBase()
{

    // constructor, declare variables that should be obtained
    // from the steering-xml file

    SetLogName( GetName() );

    m_puwp = NULL;
    m_newrun = false;
    m_lsf = NULL;
    m_bsf = NULL;

    // set some default values
    m_readTTbarReco = false;
    m_writeTTbarReco = false;
    m_dobsf=false;

    // declare variables for lumi file
    DeclareProperty( "LumiFilePath" , m_lumifile_path);
    DeclareProperty( "LumiFileName" , m_lumifile_name);
    DeclareProperty( "LumiTrigger" ,  m_lumi_trigger);

    // steerable properties of the Ntuple files
    DeclareProperty( "JetCollection", m_JetCollection );
    DeclareProperty( "ElectronCollection", m_ElectronCollection );
    DeclareProperty( "MuonCollection", m_MuonCollection );
    DeclareProperty( "TauCollection", m_TauCollection );
    DeclareProperty( "PhotonCollection", m_PhotonCollection );
    DeclareProperty( "PrimaryVertexCollection", m_PrimaryVertexCollection );
    DeclareProperty( "METName", m_METName );
    DeclareProperty( "TopJetCollection", m_TopJetCollection );
    DeclareProperty( "TopJetCollectionGen", m_TopJetCollectionGen );
    DeclareProperty( "PrunedJetCollection", m_PrunedJetCollection );
    //DeclareProperty( "addGenInfo", m_addGenInfo);
    DeclareProperty( "GenParticleCollection", m_GenParticleCollection);
    DeclareProperty( "readTTbarReco", m_readTTbarReco);
    DeclareProperty( "writeTTbarReco", m_writeTTbarReco);

    // steerable properties for the jec
    DeclareProperty( "JECFileLocation" , m_JECFileLocation);
    DeclareProperty( "JECDataGlobalTag" , m_JECDataGlobalTag);
    DeclareProperty( "JECMCGlobalTag" , m_JECMCGlobalTag);
    DeclareProperty( "JECJetCollection" , m_JECJetCollection);

    // steerable properties for the Pile-up reweighting
    DeclareProperty( "PU_Filename_MC" , m_PUFilenameMC);
    DeclareProperty( "PU_Filename_Data" , m_PUFilenameData);
    DeclareProperty( "PU_Histname_MC" , m_PUHistnameMC);
    DeclareProperty( "PU_Histname_Data" , m_PUHistnameData);

    DeclareProperty( "LeptonScaleFactors", m_leptonweights);
    DeclareProperty( "BTaggingScaleFactors", m_dobsf);

    // steerable properties for making qcd (pre) selection
    DeclareProperty( "ReversedElectronSelection", m_reversed_electron_selection);
}

AnalysisCycle::~AnalysisCycle()
{
    // destructor

    if (m_puwp) {
        delete m_puwp;
    }

}

void AnalysisCycle::BeginCycle() throw( SError )
{
    // Start of the job, general set-up and definition of
    // objects are done here

    // Set-Up LumiHandler
    LuminosityHandler *lumiHandler = new LuminosityHandler();

    lumiHandler->SetGRLPath( m_lumifile_path );
    lumiHandler->SetLumiFileName( m_lumifile_name );
    lumiHandler->SetTrigger( m_lumi_trigger );
    lumiHandler->SetIntLumiPerBin( m_int_lumi_per_bin );

    // Initialise, checks also if LumiFile is specified and readable
    lumiHandler->Initialise();
    // dump lumi information in text file
    //lumiHandler->DumpLumiInfoIntoTxtFile();


    // adding luminosity handler to gloabl config
    AddConfigObject( lumiHandler );

    return;

}

void AnalysisCycle::EndCycle() throw( SError )
{

    return;

}

void AnalysisCycle::BeginInputData( const SInputData& inputData) throw( SError )
{
    // declaration of variables and histograms

    // check if LumiHandler is set up correctly
    if( LumiHandler() == NULL ) {
        m_logger << FATAL << "Luminosity Handler not properly added to Configuration!" << SLogger::endmsg;
        exit(-1);
    }

    //determine whether running on MC or data
    m_addGenInfo=true;
    if(inputData.GetType()=="DATA" || inputData.GetType()=="Data" || inputData.GetType()=="data") m_addGenInfo=false;

    // Overwrite target Luminosity
    // has to be done for every input data
    if( LumiHandler()->IsLumiCalc() ) {
        m_logger << WARNING << "--- Not a WARNING! --- Overwrite Target Lumi with Lumi from LumiFile: "
                 << LumiHandler()->GetTargetLumi() << " (pb^-1)!" << SLogger::endmsg;
        GetConfig().SetTargetLumi( LumiHandler()->GetTargetLumi() );
    }

    if(m_JECFileLocation.size()>0) {
        m_logger << INFO << "Using JEC files from " << m_JECFileLocation << SLogger::endmsg;
        m_logger << INFO << "Using JEC global tags for data " << m_JECDataGlobalTag << " and for MC " << m_JECMCGlobalTag << SLogger::endmsg;
        m_logger << INFO << "Using JEC for jet collection " << m_JECJetCollection << SLogger::endmsg;
    } else
        m_logger << WARNING << "No location for JEC files is provided" << SLogger::endmsg;

    // pile-up reweighting
    if(m_PUFilenameMC.size()>0 && m_PUFilenameData.size()>0 && m_PUHistnameMC.size()>0 && m_PUHistnameData.size()>0 && m_addGenInfo) {
        m_PUFilenameMC += ".";
        m_PUFilenameMC += inputData.GetType();
        m_PUFilenameMC += ".";
        m_PUFilenameMC += inputData.GetVersion();
        m_PUFilenameMC += ".root";
        m_logger << INFO << "PU Reweighting will be performed. File(data) = " << m_PUFilenameData
                 << " File(MC) = " << m_PUFilenameMC << SLogger::endmsg;
        m_logger << INFO << "PU, histograms: Hist(data) = " << m_PUHistnameData
                 << " Hist(MC) = " << m_PUHistnameMC << SLogger::endmsg;
        m_puwp = new PUWeightProducer(m_PUFilenameMC, m_PUFilenameData, m_PUHistnameMC, m_PUHistnameData);
    } else {
        m_puwp = NULL;
    }

    if(m_reversed_electron_selection)
        m_logger << INFO << "Applying reversed electron selection (data-driven qcd) !!!!" << SLogger::endmsg;

    // output Ntuple
    if (inputData.GetTrees(STreeType::OutputSimpleTree)) {
        m_logger << INFO << "adding output tree" << SLogger::endmsg;
        DeclareVariable( m_bcc.run, "run" );
        DeclareVariable( m_bcc.rho, "rho" );
        DeclareVariable( m_bcc.luminosityBlock, "luminosityBlock" );
        DeclareVariable( m_bcc.event, "event" );
        DeclareVariable( m_bcc.isRealData, "isRealData" );
        //DeclareVariable( m_bcc.HBHENoiseFilterResult, "HBHENoiseFilterResult" );
        DeclareVariable( m_bcc.beamspot_x0, "beamspot_x0");
        DeclareVariable( m_bcc.beamspot_y0, "beamspot_y0");
        DeclareVariable( m_bcc.beamspot_z0, "beamspot_z0");
        if(m_ElectronCollection.size()>0) DeclareVariable(m_output_electrons, m_ElectronCollection.c_str() );
        if(m_MuonCollection.size()>0) DeclareVariable(m_output_muons, m_MuonCollection.c_str() );
        if(m_TauCollection.size()>0) DeclareVariable(m_output_taus, m_TauCollection.c_str() );
        if(m_JetCollection.size()>0) DeclareVariable(m_output_jets, m_JetCollection.c_str() );
        if(m_PhotonCollection.size()>0) DeclareVariable(m_output_photons, m_PhotonCollection.c_str() );
        if(m_METName.size()>0) DeclareVariable(m_output_met, m_METName.c_str() );
        if(m_PrimaryVertexCollection.size()>0) DeclareVariable(m_output_pvs, m_PrimaryVertexCollection.c_str());
        if(m_TopJetCollection.size()>0) DeclareVariable(m_output_topjets, m_TopJetCollection.c_str());
        if(m_addGenInfo && m_TopJetCollectionGen.size()>0) DeclareVariable(m_output_topjetsgen, m_TopJetCollectionGen.c_str());
        if(m_PrunedJetCollection.size()>0) DeclareVariable(m_output_prunedjets, m_PrunedJetCollection.c_str());
        if(m_addGenInfo && m_GenParticleCollection.size()>0) DeclareVariable(m_output_genparticles, m_GenParticleCollection.c_str());
        if(m_addGenInfo) DeclareVariable(m_output_genInfo, "genInfo" );
        if(m_writeTTbarReco) DeclareVariable(m_output_recoHyps, "recoHyps");
        DeclareVariable(m_output_triggerNames, "triggerNames");
        DeclareVariable(m_output_triggerResults, "triggerResults");
    }

    if(m_leptonweights.size()>0)
        m_lsf = new LeptonScaleFactors(m_leptonweights);

    if(m_dobsf)
      m_bsf = new BTaggingScaleFactors();

    return;

}


void AnalysisCycle::RegisterSelection(Selection* sel)
{
    // register a selection in the list of selections

    if (!sel) {
        m_logger << WARNING << "Got NULL pointer, can not register selection." << SLogger::endmsg;
        return;
    }

    // check if selection already exists, only register it if not
    TString InName(sel->GetName());
    bool exists = false;
    for (unsigned int i=0; i<m_selections.size(); ++i) {
        TString SelName = m_selections[i]->GetName();
        if (InName == SelName) {
            exists = true;
        }
    }
    if (!exists) {
        m_selections.push_back(sel);
    }

    return;
}


void AnalysisCycle::RegisterHistCollection(BaseHists* hists)
{
    // register a collection of histograms in the list

    if (!hists) {
        m_logger << WARNING << "Got NULL pointer, can not register histogram collection." << SLogger::endmsg;
        return;
    }


    m_histcollections.push_back(hists);


    return;
}


Selection* AnalysisCycle::GetSelection(const std::string name)
{
    // get a selection from the list of selections

    TString InName(name.c_str());
    for (unsigned int i=0; i<m_selections.size(); ++i) {
        TString SelName = m_selections[i]->GetName();
        if (InName == SelName) {
            return m_selections[i];
        }
    }
    m_logger << DEBUG << "Could not find selection with name " << InName << "." << SLogger::endmsg;
    return NULL;
}


BaseHists* AnalysisCycle::GetHistCollection(const std::string name)
{
    // get a histogram collection from the list

    TString InName(name.c_str());
    for (unsigned int i=0; i<m_histcollections.size(); ++i) {
        TString HistName = m_histcollections[i]->GetName();
        if (InName == HistName) {
            return m_histcollections[i];
        }
    }
    m_logger << WARNING << "Could not find hist collection with name " << InName << "." << SLogger::endmsg;
    return NULL;
}



void AnalysisCycle::InitHistos()
{
    // initialise all registered histograms and set the
    // correct output path

    for (unsigned int i=0; i<m_histcollections.size(); ++i) {
        m_histcollections[i]->SetHistOutput(this->GetHistOutput());
        m_histcollections[i]->Init();
    }

    return;

}

void AnalysisCycle::FinaliseHistos()
{
    // finalise all registered histograms

    for (unsigned int i=0; i<m_histcollections.size(); ++i) {
        m_histcollections[i]->Finish();
    }

    m_histcollections.clear();

    return;

}

void AnalysisCycle::PrintSelectionSummary()
{
    // print a summary of all selections

    for (unsigned int i=0; i<m_selections.size(); ++i) {
        m_selections[i]->printCutFlow();
    }

}

void AnalysisCycle::ResetSelectionStats()
{
    // set all selection statistics to zero

    for (unsigned int i=0; i<m_selections.size(); ++i) {
        m_selections[i]->resetCutFlow();
    }

}


void AnalysisCycle::EndInputData( const SInputData& ) throw( SError )
{

    // clean-up, info messages and final calculations after the analysis

    PrintSelectionSummary();

    FinaliseHistos();

    ResetSelectionStats();

    return;


}

void AnalysisCycle::BeginInputFile( const SInputData& ) throw( SError )
{
    // Connect all variables from the Ntuple file with the ones needed for the analysis.
    // The different collections that should be loaded are steerable through the XML file.
    // The variables are commonly stored in the BaseCycleContaincer and can be
    // accessed afterwards through the ObjectHandler

    ConnectVariable( "AnalysisTree", "triggerResults" , m_bcc.triggerResults);
    ConnectVariable( "AnalysisTree", "triggerNames" , m_bcc.triggerNames);
    if(m_ElectronCollection.size()>0) ConnectVariable( "AnalysisTree", m_ElectronCollection.c_str() ,m_bcc. electrons);
    if(m_MuonCollection.size()>0) ConnectVariable( "AnalysisTree", m_MuonCollection.c_str() , m_bcc.muons);
    if(m_TauCollection.size()>0) ConnectVariable( "AnalysisTree", m_TauCollection.c_str() , m_bcc.taus);
    if(m_JetCollection.size()>0) ConnectVariable( "AnalysisTree", m_JetCollection.c_str() , m_bcc.jets);
    if(m_PhotonCollection.size()>0) ConnectVariable( "AnalysisTree", m_PhotonCollection.c_str() , m_bcc.photons);
    if(m_METName.size()>0) ConnectVariable( "AnalysisTree", m_METName.c_str() , m_bcc.met);
    if(m_PrimaryVertexCollection.size()>0) ConnectVariable( "AnalysisTree", m_PrimaryVertexCollection.c_str() , m_bcc.pvs);
    if(m_TopJetCollection.size()>0) ConnectVariable( "AnalysisTree", m_TopJetCollection.c_str() , m_bcc.topjets);
    if(m_addGenInfo && m_TopJetCollectionGen.size()>0) ConnectVariable( "AnalysisTree", m_TopJetCollectionGen.c_str() , m_bcc.topjetsgen);
    if(m_PrunedJetCollection.size()>0) ConnectVariable( "AnalysisTree", m_PrunedJetCollection.c_str() , m_bcc.prunedjets);
    if(m_addGenInfo && m_GenParticleCollection.size()>0) ConnectVariable( "AnalysisTree", m_GenParticleCollection.c_str() , m_bcc.genparticles);
    if(m_addGenInfo) ConnectVariable( "AnalysisTree", "genInfo" , m_bcc.genInfo);
    if(m_readTTbarReco) ConnectVariable( "AnalysisTree", "recoHyps", m_bcc.recoHyps);
    ConnectVariable( "AnalysisTree", "run" , m_bcc.run);
    ConnectVariable( "AnalysisTree", "rho" , m_bcc.rho);
    ConnectVariable( "AnalysisTree", "luminosityBlock" , m_bcc.luminosityBlock);
    ConnectVariable( "AnalysisTree" ,"event" ,m_bcc.event);
    ConnectVariable( "AnalysisTree" ,"isRealData", m_bcc.isRealData);
    //ConnectVariable( "AnalysisTree" ,"HBHENoiseFilterResult", m_bcc.HBHENoiseFilterResult);
    ConnectVariable( "AnalysisTree" ,"beamspot_x0", m_bcc.beamspot_x0);
    ConnectVariable( "AnalysisTree" ,"beamspot_y0", m_bcc.beamspot_y0);
    ConnectVariable( "AnalysisTree" ,"beamspot_z0", m_bcc.beamspot_z0);

    //if(m_caTopTagGen.size()>0) ConnectVariable("AnalysisTree", m_caTopTagGen.c_str(), m_bcc.topjets);


    ObjectHandler* objs = ObjectHandler::Instance();
    objs->SetBaseCycleContainer(&m_bcc);
    objs->SetLumiHandler( LumiHandler() );

    return;

}

void AnalysisCycle::ExecuteEvent( const SInputData&, Double_t weight) throw( SError )
{
    // This method performs basic consistency checks, resets the event calculator,
    // calculates the pile-up weight and performs the good-run selection.
    // It should always be the first thing to be called in each user analysis.

    // first thing to do: call reset of event calc
    EventCalc* calc = EventCalc::Instance();
    calc->Reset();

    if(m_bcc.isRealData && m_addGenInfo) {
        m_logger << WARNING<< "Running over real data, but addGenInfo=True?!" << SLogger::endmsg;
    }

    //fill list of trigger names
    if(m_bcc.triggerNames->size()!=0) {
        m_bcc.triggerNames_actualrun = *m_bcc.triggerNames;
        m_newrun=true;
    }

    // generate random run Nr for MC samples (consider luminosity of each run)
    // e.g. for proper OTX cut in MC, and needs to be done only once per event
    if( !m_bcc.isRealData && LumiHandler()->IsLumiCalc() ) {
        m_bcc.run = LumiHandler()->GetRandomRunNr();
    }


    //set the lumiweight to 1 for data
    if(m_bcc.isRealData) weight = 1;

    // store the weight (lumiweight) in the eventcalc class and use it
    calc -> ProduceWeight(weight);

    if(m_bcc.genInfo) {

        std::vector<float> genweights =  m_bcc.genInfo->weights();
        for(unsigned int i=0; i< genweights.size(); ++i) {
            calc -> ProduceWeight(genweights[i]);
        }

        if(m_puwp) {
            double pu_weight = m_puwp->produceWeight(m_bcc.genInfo);
            // set the weight in the eventcalc
            calc -> ProduceWeight(pu_weight);
        }

        //lepton scale factor
        if(m_lsf) {
            calc->ProduceWeight(m_lsf->GetWeight());
        }
        //b tagging scale factor
        if(m_bsf) {
            calc->ProduceWeight(m_bsf->GetWeight());
        }

    }

    //select only good runs
    if(m_bcc.isRealData && LumiHandler()->IsLumiCalc() ) {
        if( !LumiHandler()->PassGoodRunsList( m_bcc.run, m_bcc.luminosityBlock )) throw SError( SError::SkipEvent );
    }

    //clear reco hyp pointers from previous events
    if( m_bcc.recoHyps && !m_readTTbarReco && m_writeTTbarReco) {
        delete m_bcc.recoHyps ;
        //m_bcc.recoHyps->clear();
        //m_bcc.recoHyps = NULL;
    }

    //create new pointer to recoHyps if no recoHyps were read in
    //note: list of recoHyps is still empty, has to be filled in the user cycle

    if(!m_readTTbarReco && m_writeTTbarReco)  m_bcc.recoHyps = new std::vector<ReconstructionHypothesis>;

    return;

}

void AnalysisCycle::WriteOutputTree() throw( SError)
{
    //write out all objects

    m_output_photons.clear();
    m_output_jets.clear();
    m_output_electrons.clear();
    m_output_muons.clear();
    m_output_taus.clear();
    m_output_pvs.clear();
    m_output_topjets.clear();
    m_output_topjetsgen.clear();
    m_output_prunedjets.clear();
    m_output_genparticles.clear();
    m_output_triggerNames.clear();
    m_output_triggerResults.clear();
    m_output_recoHyps.clear();

    if(m_PhotonCollection.size()>0) m_output_photons=*m_bcc.photons;
    if(m_JetCollection.size()>0) m_output_jets=*m_bcc.jets;
    if(m_ElectronCollection.size()>0) m_output_electrons=*m_bcc.electrons;
    if(m_MuonCollection.size()>0) m_output_muons=*m_bcc.muons;
    if(m_TauCollection.size()>0) m_output_taus=*m_bcc.taus;
    if(m_PrimaryVertexCollection.size()>0) m_output_pvs=*m_bcc.pvs;
    if(m_METName.size()>0) m_output_met = *m_bcc.met;
    if(m_addGenInfo) m_output_genInfo = *m_bcc.genInfo;
    if(m_TopJetCollection.size()>0) m_output_topjets=*m_bcc.topjets;
    if(m_addGenInfo && m_TopJetCollectionGen.size()>0) m_output_topjetsgen=*m_bcc.topjetsgen;
    if(m_PrunedJetCollection.size()>0) m_output_prunedjets=*m_bcc.prunedjets;
    if(m_addGenInfo && m_GenParticleCollection.size()>0) m_output_genparticles=*m_bcc.genparticles;
    if(m_writeTTbarReco) m_output_recoHyps=*m_bcc.recoHyps;

    if(m_newrun) m_output_triggerNames = m_bcc.triggerNames_actualrun;//store trigger names only for new runs
    m_newrun=false;

    m_output_triggerResults = *m_bcc.triggerResults;

}

