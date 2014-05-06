// $Id: AnalysisCycle.cxx,v 1.42 2013/06/19 14:04:46 jott Exp $
#include <iostream>
#include <stdint.h>

#include "include/AnalysisCycle.h"
#include "include/SelectionModules.h"

#include "SFrameTools/include/EventCalc.h"
#include "SFrameAnalysis/include/Cleaner.h"

#include "core/include/STreeType.h"
#include "core/include/SCycleStatistics.h"

using namespace std;

ClassImp( AnalysisCycle );

AnalysisCycle::AnalysisCycle()
    : SCycleBase()
{

    // constructor, declare variables that should be obtained
    // from the steering-xml file

    SetLogName( GetName() );

    m_puwp = NULL;
    m_trig = NULL;
    m_tpr = NULL;
    m_hepsf = NULL;
    m_newrun = false;
    m_lsf = NULL;
    m_jsf =  NULL;
    m_pdfweights=NULL;
    m_pdf_index=0;
    m_corrector = NULL;
    m_correctortop = NULL;
    m_correctortoptag = NULL;
    m_correctorhiggstag = NULL;
    m_correctorsubjet = NULL;
    m_jes_unc = NULL;
    m_jes_unc_top = NULL;
    m_jes_unc_sub = NULL;

    m_sys_unc = e_None;
    m_sys_var = e_Default;
    m_actual_run=-99999;

    // set some default values
    m_readTTbarReco = false;
    m_writeTTbarReco = false;
    m_readCommonInfo = true;
    m_channel = "inclusive_LQ";

    // declare variables for lumi file
    DeclareProperty( "LumiFilePath" , m_lumifile_path);
    DeclareProperty( "LumiFileName" , m_lumifile_name);
    DeclareProperty( "LumiTrigger" ,  m_lumi_trigger);

    //trigger
    DeclareProperty( "TriggerMode" , m_triggermode);
    DeclareProperty( "TriggerDir" , m_triggerdir);

    // steerable properties of the Ntuple files
    DeclareProperty( "JetCollection", m_JetCollection );
    DeclareProperty( "GenJetCollection", m_GenJetCollection );
    DeclareProperty( "ElectronCollection", m_ElectronCollection );
    DeclareProperty( "MuonCollection", m_MuonCollection );
    DeclareProperty( "TauCollection", m_TauCollection );
    DeclareProperty( "PhotonCollection", m_PhotonCollection );
    DeclareProperty( "PrimaryVertexCollection", m_PrimaryVertexCollection );
    DeclareProperty( "METName", m_METName );
    DeclareProperty( "TopJetCollection", m_TopJetCollection );
    DeclareProperty( "TopJetCollectionGen", m_TopJetCollectionGen );
    DeclareProperty( "PrunedJetCollection", m_PrunedJetCollection );
    DeclareProperty( "TopTagJetCollection", m_TopTagJetCollection );
    DeclareProperty( "HiggsTagJetCollection", m_HiggsTagJetCollection );
    //DeclareProperty( "addGenInfo", m_addGenInfo);
    DeclareProperty( "GenParticleCollection", m_GenParticleCollection);
    DeclareProperty( "PFParticleCollection", m_PFParticleCollection);
    DeclareProperty( "readTTbarReco", m_readTTbarReco);
    DeclareProperty( "writeTTbarReco", m_writeTTbarReco);
    DeclareProperty( "readCommonInfo", m_readCommonInfo);

    // steerable properties for the jec
    DeclareProperty( "JECFileLocation" , m_JECFileLocation);
    DeclareProperty( "JECDataGlobalTag" , m_JECDataGlobalTag);
    DeclareProperty( "JECMCGlobalTag" , m_JECMCGlobalTag);
    DeclareProperty( "JECJetCollection" , m_JECJetCollection);
    DeclareProperty( "JECTopJetCollection" , m_JECTopJetCollection);
    DeclareProperty( "JECTopTagJetCollection" , m_JECTopTagJetCollection);
    DeclareProperty( "JECHiggsTagJetCollection" , m_JECHiggsTagJetCollection);
    DeclareProperty( "JECSubJetCollection" , m_JECSubJetCollection);
    DeclareProperty( "ExtraTopJEC" , m_extra_topJEC);
    DeclareProperty( "ExtraSubjetJEC" , m_extra_subjetJEC);
    DeclareProperty( "OnlyUNCSubjetJEC" , m_onlyUNC_subjetJEC);

    //top pag pt reweighting mode
    DeclareProperty( "toppagptweight", m_toppagptweight);

    //top-tagging sf reweighting mode
    DeclareProperty( "TopTaggingSFMode", m_TopTaggingSFMode);

    // steerable properties for the Pile-up reweighting
    DeclareProperty( "PU_Filename_MC" , m_PUFilenameMC);
    DeclareProperty( "PU_Filename_Data" , m_PUFilenameData);
    DeclareProperty( "PU_Histname_MC" , m_PUHistnameMC);
    DeclareProperty( "PU_Histname_Data" , m_PUHistnameData);

    // lepton weights
    DeclareProperty( "LeptonScaleFactors", m_leptonweights);

    // uncertainties
    DeclareProperty("SystematicUncertainty", m_sys_unc_name);
    DeclareProperty("SystematicVariation", m_sys_var_name);

    //special configuration for PDF uncertainty
    DeclareProperty( "PDFName", m_pdfname );
    DeclareProperty( "PDFWeightFilesDirectory", m_pdfdir );
    DeclareProperty( "PDFIndex", m_pdf_index);

    //find out the LQ channel
    DeclareProperty( "LQChannel", m_channel);
}

AnalysisCycle::~AnalysisCycle()
{

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
}

void AnalysisCycle::EndCycle() throw( SError )
{

}

void AnalysisCycle::EndMasterInputData(const SInputData & d) throw (SError){
    // check that all events have actually been processed:
    // 1. get the number of input events, NEventsMax and NEventsSkip:
    uint64_t ntotal_input = d.GetEventsTotal();
    uint64_t nmax = d.GetNEventsMax();
    uint64_t nskip = d.GetNEventsSkip();
    // the expected propcessed number of events is the minimum of 
    // ntotal_input - nskip and nmax:
    uint64_t nprocessed_expected = min(ntotal_input - nskip, nmax);
    
    m_logger << INFO << "Checking how many events have actually been processed (ntot: " << ntotal_input
             << "; NEventsMax: " << nmax << "; nskip: " << nskip << "; expecting " << nprocessed_expected << ")" << SLogger::endmsg;
    
    // 2. get the (merged) histogram of processed events. Note that a double can 
    // accurately represent an integer up to 52 bits, i.e. 2^52, which is always enough
    // for our purposes.
    TList * l = GetHistOutput();
    SCycleOutput * out = dynamic_cast<SCycleOutput*>(l->FindObject("nprocessed"));
    assert(out);        
    TH1D * hprocessed = dynamic_cast<TH1D*>(out->GetObject());
    assert(hprocessed!=0);
    uint64_t nprocessed = hprocessed->GetBinContent(1);
    if(nprocessed != nprocessed_expected){
        m_logger << FATAL << "Consistency check failed: processed " << nprocessed << " events, but from input dataset info expected " << nprocessed_expected << " events (jobs crashed?)" << SLogger::endmsg;
        throw SError(SError::StopExecution) << "Failed nprocessed consistency check (see log for details)";
    }
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
	if(m_JECTopJetCollection.size()>0) {
	  m_logger << INFO << "Using JEC for topjet collection " << m_JECTopJetCollection << SLogger::endmsg;
	}
	if(m_JECTopTagJetCollection.size()>0) {
	  m_logger << INFO << "Using JEC for toptagjet collection " << m_JECTopTagJetCollection << SLogger::endmsg;
	}
	if(m_JECHiggsTagJetCollection.size()>0) {
	  m_logger << INFO << "Using JEC for higgstagjet collection " << m_JECHiggsTagJetCollection << SLogger::endmsg;
	}
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

    //trigger reweighting
    if(m_triggermode.size()>0 && m_triggerdir.size()>0 && m_addGenInfo) {
        m_triggerdir += ".";
        m_triggerdir += inputData.GetType();
        m_triggerdir += ".";
        m_triggerdir += inputData.GetVersion();
        m_triggerdir += ".root";
        m_logger << INFO << "Trigger Reweighting will be performed. File = " << m_triggerdir << SLogger::endmsg;
        m_trig = new TriggerWeight(m_triggerdir, m_triggermode);
    } else {
        m_trig = NULL;
    }
    

    //toppag pt re-weighting
    TString InputSampleName(inputData.GetVersion());
    if((m_toppagptweight.size()>0)&&(InputSampleName.Contains("ttbar",TString::kIgnoreCase))&&(m_addGenInfo==true)){
      m_logger << INFO << "Top PAG pt re-weighting will be performed" << SLogger::endmsg;
      m_tpr = new TopPtReweight();
    }

    //top-tagging sf re-weighting
    if((m_TopTaggingSFMode.size()>0)&&((InputSampleName.Contains("ttbar",TString::kIgnoreCase))||(InputSampleName.Contains("tp",TString::kIgnoreCase)))&&(m_addGenInfo==true)){
      m_logger << INFO << "HepTopTagger scale factors re-weighting will be performed" << SLogger::endmsg;
      m_hepsf = new HEPTopTaggerReweightTPrime();
    }
 
    // check if the settings for the systematic uncertainty make sense
    if(m_sys_unc_name.size()>0){
      bool isok = false;
      if (m_sys_unc_name=="NONE" || m_sys_unc_name=="none" || m_sys_unc_name=="None"){
	m_sys_unc = e_None;
	isok = true;
      }
      if (m_sys_unc_name=="JEC" || m_sys_unc_name=="jec"){
	m_sys_unc = e_JEC;
	isok = true;
      }
      if (m_sys_unc_name=="SUBJEC" || m_sys_unc_name=="subjec"){
	m_sys_unc = e_subJEC;
	isok = true;
      }
      if (m_sys_unc_name=="JER" || m_sys_unc_name=="jer"){
	m_sys_unc = e_JER; 
	isok = true;
      }
       if (m_sys_unc_name=="TER" || m_sys_unc_name=="ter"){
	m_sys_unc = e_TER; 
	isok = true;
      }
       if (m_sys_unc_name=="SUBJER" || m_sys_unc_name=="subjer"){
	m_sys_unc = e_subJER; 
	isok = true;
       }
       if (m_sys_unc_name=="MuonSF"){       
          m_sys_unc = e_MuonSF;
	isok = true;
      }
      if (m_sys_unc_name=="EleSF"){
	m_sys_unc = e_EleSF;
	isok = true;
      }
      if (m_sys_unc_name=="TauSF"){
	m_sys_unc = e_TauSF;
	isok = true;
      }
      if (m_sys_unc_name=="TauEleSF"){
	m_sys_unc = e_TauEleSF;
	isok = true;
      }
      if (m_sys_unc_name=="TauEffSF"){
	m_sys_unc = e_TauEffSF;
	isok = true;
      }
      if (m_sys_unc_name=="TauEnergy"){
	m_sys_unc = e_TauEnergy;
	isok = true;
      }
      if (m_sys_unc_name=="PDF" || m_sys_unc_name=="pdf"){
	m_sys_unc = e_PDF;
	isok = true;
      }
 
      if (m_sys_unc != e_None){
	if (GetSysShiftName()=="UP" || GetSysShiftName()=="up" || GetSysShiftName()=="Up") m_sys_var = e_Up; 
	if (GetSysShiftName()=="DOWN" || GetSysShiftName()=="down" || GetSysShiftName()=="Down") m_sys_var = e_Down; 
      } 

      if (isok){

	if (m_sys_unc == e_None){
	  m_logger << INFO << "Running without systematic uncertainty" << SLogger::endmsg;

	} else {
	  m_logger << WARNING << "Running with systematic uncertainty: " << m_sys_unc_name << "  (this is more an info than a warning)" << SLogger::endmsg;

	  if (m_sys_var_name=="UP" || m_sys_var_name=="up" || m_sys_var_name=="Up" 
	      || m_sys_var_name=="DOWN" || m_sys_var_name=="down" || m_sys_var_name=="Down"){
	    m_logger << WARNING << "Applying shift: " << m_sys_var_name << SLogger::endmsg;

	  } else {
	    m_logger << FATAL << "Requested shift: " << m_sys_var_name 
		     << " is not supported. Please choose \"up\" or \"down\" for SystematicVariation" << SLogger::endmsg;	  
	    exit(EXIT_FAILURE);
	  }
	}
	
      } else {
	m_logger << FATAL << "Systematic uncertainty: " << m_sys_unc_name << " is not known." << SLogger::endmsg;
	exit(EXIT_FAILURE);
      }
      
    }
    
    // if (m_LQannel_OS == "True" || m_LQChannel_OS == "true" ||m_LQChannel_OS == "TRUE") m_channel_OS = true; 
    
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
        if(m_addGenInfo && m_GenJetCollection.size()>0) DeclareVariable(m_output_genjets, m_GenJetCollection.c_str() );	
        if(m_PhotonCollection.size()>0) DeclareVariable(m_output_photons, m_PhotonCollection.c_str() );
        if(m_METName.size()>0) DeclareVariable(m_output_met, m_METName.c_str() );
        if(m_PrimaryVertexCollection.size()>0) DeclareVariable(m_output_pvs, m_PrimaryVertexCollection.c_str());
        if(m_TopJetCollection.size()>0) DeclareVariable(m_output_topjets, m_TopJetCollection.c_str());
	if(m_TopTagJetCollection.size()>0) DeclareVariable(m_output_toptagjets, m_TopTagJetCollection.c_str());
	if(m_HiggsTagJetCollection.size()>0) DeclareVariable(m_output_higgstagjets, m_HiggsTagJetCollection.c_str());
        if(m_addGenInfo && m_TopJetCollectionGen.size()>0) DeclareVariable(m_output_topjetsgen, m_TopJetCollectionGen.c_str());
        if(m_PrunedJetCollection.size()>0) DeclareVariable(m_output_prunedjets, m_PrunedJetCollection.c_str());
        if(m_addGenInfo && m_GenParticleCollection.size()>0) DeclareVariable(m_output_genparticles, m_GenParticleCollection.c_str());
        if(m_PFParticleCollection.size()>0) DeclareVariable(m_output_pfparticles, m_PFParticleCollection.c_str());
        if(m_addGenInfo && m_readCommonInfo) DeclareVariable(m_output_genInfo, "genInfo" );
        if(m_writeTTbarReco) DeclareVariable(m_output_recoHyps, "recoHyps");
        DeclareVariable(m_output_triggerNames, "triggerNames");
        DeclareVariable(m_output_triggerResults, "triggerResults");
    }

    m_lsf = new LeptonScaleFactors(m_leptonweights, m_channel);	
    m_jsf = new JetpTReweightingInWJets();
        
    if (m_sys_unc == e_MuonSF){
      if(m_sys_var == e_Up){
	cout << "apply muon up var" << endl;
	m_lsf->DoUpVarMuonSF();
      } else {
	cout << "apply muon down var" << endl;
	m_lsf->DoDownVarMuonSF();
      } 
    }
    if (m_sys_unc == e_EleSF){
      if(m_sys_var == e_Up){
	m_lsf->DoUpVarEleSF();
      } else {
	m_lsf->DoDownVarEleSF();
      } 
    }
    if (m_sys_unc == e_TauSF){
      if(m_sys_var == e_Up){
	m_lsf->DoUpVarTauSF();
      } else {
	m_lsf->DoDownVarTauSF();
      } 
    }
    if (m_sys_unc == e_TauEleSF){
      if(m_sys_var == e_Up){
	m_lsf->DoUpVarTauEleSF();
      } else {
	m_lsf->DoDownVarTauEleSF();
      } 
    }
    if (m_sys_unc == e_TauEffSF){
      if(m_sys_var == e_Up){
	m_lsf->DoUpVarTauEffSF();
      } else {
	m_lsf->DoDownVarTauEffSF();
      } 
    }

 
    if(m_sys_unc == e_PDF){

      TString dirname = m_pdfdir;
      if(m_pdfdir.size()>0){
	dirname += "/";
	dirname += inputData.GetVersion();
      }
      

      if(m_sys_var == e_Up){
	m_pdfweights = new PDFWeights(e_Up,m_pdfname,dirname);	
      } else {
	m_pdfweights = new PDFWeights(e_Down,m_pdfname,dirname);
      }   
    }

    // ------------- jet energy correction ----------------

    if(m_JECFileLocation.size()>0 && m_JECJetCollection.size()>0 )
    {

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

      // jec uncertainty
      TString unc_file = m_JECFileLocation + "/" + m_JECDataGlobalTag + "_Uncertainty_" + m_JECJetCollection + ".txt";
      m_jes_unc = new JetCorrectionUncertainty(unc_file.Data());
    }

    // ------------- subjetjet energy correction ----------------

    if(m_JECFileLocation.size()>0 && m_JECSubJetCollection.size()>0 )
    {

      std::vector<JetCorrectorParameters> subpars;
      
      //see https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookJetEnergyCorrections#GetTxtFiles how to get the txt files with jet energy corrections from the database
      if(!addGenInfo()) {
        subpars.push_back(JetCorrectorParameters(m_JECFileLocation + "/" + m_JECDataGlobalTag + "_L1FastJet_" + m_JECSubJetCollection + ".txt"));
        subpars.push_back(JetCorrectorParameters(m_JECFileLocation + "/" + m_JECDataGlobalTag + "_L2Relative_" + m_JECSubJetCollection + ".txt"));
        subpars.push_back(JetCorrectorParameters(m_JECFileLocation + "/" + m_JECDataGlobalTag + "_L3Absolute_" + m_JECSubJetCollection + ".txt"));
        subpars.push_back(JetCorrectorParameters(m_JECFileLocation + "/" + m_JECDataGlobalTag + "_L2L3Residual_" + m_JECSubJetCollection + ".txt"));
      } else {
        subpars.push_back(JetCorrectorParameters(m_JECFileLocation + "/" + m_JECMCGlobalTag + "_L1FastJet_" + m_JECSubJetCollection + ".txt"));
        subpars.push_back(JetCorrectorParameters(m_JECFileLocation + "/" + m_JECMCGlobalTag + "_L2Relative_" + m_JECSubJetCollection + ".txt"));
        subpars.push_back(JetCorrectorParameters(m_JECFileLocation + "/" + m_JECMCGlobalTag + "_L3Absolute_" + m_JECSubJetCollection + ".txt"));
      }

      m_correctorsubjet = new FactorizedJetCorrector(subpars);

      // jec uncertainty
      TString unc_file_sub = m_JECFileLocation + "/" + m_JECDataGlobalTag + "_Uncertainty_" + m_JECSubJetCollection + ".txt";
      m_jes_unc_sub = new JetCorrectionUncertainty(unc_file_sub.Data());
    }

    // ------------- top jet energy correction ----------------

    if(m_JECFileLocation.size()>0 && m_JECTopJetCollection.size()>0 )
    {

      std::vector<JetCorrectorParameters> toppars;
      
      //see https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookJetEnergyCorrections#GetTxtFiles how to get the txt files with jet energy corrections from the database
      if(!addGenInfo()) {
        toppars.push_back(JetCorrectorParameters(m_JECFileLocation + "/" + m_JECDataGlobalTag + "_L1FastJet_" + m_JECTopJetCollection + ".txt"));
        toppars.push_back(JetCorrectorParameters(m_JECFileLocation + "/" + m_JECDataGlobalTag + "_L2Relative_" + m_JECTopJetCollection + ".txt"));
        toppars.push_back(JetCorrectorParameters(m_JECFileLocation + "/" + m_JECDataGlobalTag + "_L3Absolute_" + m_JECTopJetCollection + ".txt"));
        toppars.push_back(JetCorrectorParameters(m_JECFileLocation + "/" + m_JECDataGlobalTag + "_L2L3Residual_" + m_JECTopJetCollection + ".txt"));
      } else {
        toppars.push_back(JetCorrectorParameters(m_JECFileLocation + "/" + m_JECMCGlobalTag + "_L1FastJet_" + m_JECTopJetCollection + ".txt"));
        toppars.push_back(JetCorrectorParameters(m_JECFileLocation + "/" + m_JECMCGlobalTag + "_L2Relative_" + m_JECTopJetCollection + ".txt"));
        toppars.push_back(JetCorrectorParameters(m_JECFileLocation + "/" + m_JECMCGlobalTag + "_L3Absolute_" + m_JECTopJetCollection + ".txt"));
      }

      m_correctortop = new FactorizedJetCorrector(toppars);

      // jec uncertainty
      TString unc_file_top = m_JECFileLocation + "/" + m_JECDataGlobalTag + "_Uncertainty_" + m_JECTopJetCollection + ".txt";
      m_jes_unc_top = new JetCorrectionUncertainty(unc_file_top.Data());

    }

    // ------------- toptag jet energy correction ----------------

    if(m_JECFileLocation.size()>0 && m_JECTopTagJetCollection.size()>0 )
    {

      std::vector<JetCorrectorParameters> toptagpars;
      
      //see https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookJetEnergyCorrections#GetTxtFiles how to get the txt files with jet energy corrections from the database
      if(!addGenInfo()) {
        toptagpars.push_back(JetCorrectorParameters(m_JECFileLocation + "/" + m_JECDataGlobalTag + "_L1FastJet_" + m_JECTopTagJetCollection + ".txt"));
        toptagpars.push_back(JetCorrectorParameters(m_JECFileLocation + "/" + m_JECDataGlobalTag + "_L2Relative_" + m_JECTopTagJetCollection + ".txt"));
        toptagpars.push_back(JetCorrectorParameters(m_JECFileLocation + "/" + m_JECDataGlobalTag + "_L3Absolute_" + m_JECTopTagJetCollection + ".txt"));
        toptagpars.push_back(JetCorrectorParameters(m_JECFileLocation + "/" + m_JECDataGlobalTag + "_L2L3Residual_" + m_JECTopTagJetCollection + ".txt"));
      } else {
        toptagpars.push_back(JetCorrectorParameters(m_JECFileLocation + "/" + m_JECMCGlobalTag + "_L1FastJet_" + m_JECTopTagJetCollection + ".txt"));
        toptagpars.push_back(JetCorrectorParameters(m_JECFileLocation + "/" + m_JECMCGlobalTag + "_L2Relative_" + m_JECTopTagJetCollection + ".txt"));
        toptagpars.push_back(JetCorrectorParameters(m_JECFileLocation + "/" + m_JECMCGlobalTag + "_L3Absolute_" + m_JECTopTagJetCollection + ".txt"));
      }

      m_correctortoptag = new FactorizedJetCorrector(toptagpars);

    }

    // ------------- higgstag jet energy correction ----------------

    if(m_JECFileLocation.size()>0 && m_JECHiggsTagJetCollection.size()>0 )
    {

      std::vector<JetCorrectorParameters> higgstagpars;
      
      //see https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookJetEnergyCorrections#GetTxtFiles how to get the txt files with jet energy corrections from the database
      if(!addGenInfo()) {
        higgstagpars.push_back(JetCorrectorParameters(m_JECFileLocation + "/" + m_JECDataGlobalTag + "_L1FastJet_" + m_JECHiggsTagJetCollection + ".txt"));
        higgstagpars.push_back(JetCorrectorParameters(m_JECFileLocation + "/" + m_JECDataGlobalTag + "_L2Relative_" + m_JECHiggsTagJetCollection + ".txt"));
        higgstagpars.push_back(JetCorrectorParameters(m_JECFileLocation + "/" + m_JECDataGlobalTag + "_L3Absolute_" + m_JECHiggsTagJetCollection + ".txt"));
        higgstagpars.push_back(JetCorrectorParameters(m_JECFileLocation + "/" + m_JECDataGlobalTag + "_L2L3Residual_" + m_JECHiggsTagJetCollection + ".txt"));
      } else {
        higgstagpars.push_back(JetCorrectorParameters(m_JECFileLocation + "/" + m_JECMCGlobalTag + "_L1FastJet_" + m_JECHiggsTagJetCollection + ".txt"));
        higgstagpars.push_back(JetCorrectorParameters(m_JECFileLocation + "/" + m_JECMCGlobalTag + "_L2Relative_" + m_JECHiggsTagJetCollection + ".txt"));
        higgstagpars.push_back(JetCorrectorParameters(m_JECFileLocation + "/" + m_JECMCGlobalTag + "_L3Absolute_" + m_JECHiggsTagJetCollection + ".txt"));
      }

      m_correctorhiggstag = new FactorizedJetCorrector(higgstagpars);

    }

    // -- nprocessed consistency check --
    
    nprocessed = Book(TH1D("nprocessed", "nprocessed", 1, 0, 1));
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
}


Selection* AnalysisCycle::GetSelection(const std::string & name)
{
    // get a selection from the list of selections
    for (unsigned int i=0; i<m_selections.size(); ++i) {
        if (name == m_selections[i]->GetName()) {
            return m_selections[i];
        }
    }
    m_logger << WARNING << "Could not find selection with name " << name << "." << SLogger::endmsg;
    return NULL;
}


BaseHists* AnalysisCycle::GetHistCollection(const std::string & name)
{
    // get a histogram collection from the list
    for (unsigned int i=0; i<m_histcollections.size(); ++i) {
        if (name == m_histcollections[i]->GetName()) {
            return m_histcollections[i];
        }
    }
    m_logger << WARNING << "Could not find hist collection with name " << name << "." << SLogger::endmsg;
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
	delete m_histcollections[i];
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
    
    delete m_lsf;
    delete m_pdfweights;
    delete m_puwp;
    delete m_trig;
    delete m_tpr;
    delete m_hepsf;
    delete m_corrector;
    delete m_correctortop;
    delete m_correctortoptag;
    delete m_correctorhiggstag;
    delete m_correctorsubjet; 
    delete m_jes_unc;
    delete m_jes_unc_top;
    delete m_jes_unc_sub;

    m_tpr = NULL;
    m_hepsf = NULL;

    return;


}

void AnalysisCycle::BeginInputFile( const SInputData& ) throw( SError )
{
   // Connect all variables from the Ntuple file with the ones needed for the analysis.
    // The different collections that should be loaded are steerable through the XML file.
    // The variables are commonly stored in the BaseCycleContaincer and can be
    // accessed afterwards through EventCalc
    m_bcc.reset();

    if(m_ElectronCollection.size()>0) ConnectVariable( "AnalysisTree", m_ElectronCollection.c_str(), m_bcc.electrons);
    if(m_MuonCollection.size()>0) ConnectVariable( "AnalysisTree", m_MuonCollection.c_str(), m_bcc.muons);
    if(m_TauCollection.size()>0) ConnectVariable( "AnalysisTree", m_TauCollection.c_str(), m_bcc.taus);
    if(m_JetCollection.size()>0) ConnectVariable( "AnalysisTree", m_JetCollection.c_str(), m_bcc.jets);
    if(m_addGenInfo && m_GenJetCollection.size()>0) ConnectVariable( "AnalysisTree", m_GenJetCollection.c_str(), m_bcc.genjets);
    if(m_PhotonCollection.size()>0) ConnectVariable( "AnalysisTree", m_PhotonCollection.c_str(), m_bcc.photons);
    if(m_METName.size()>0) ConnectVariable( "AnalysisTree", m_METName.c_str(), m_bcc.met);
    if(m_PrimaryVertexCollection.size()>0) ConnectVariable( "AnalysisTree", m_PrimaryVertexCollection.c_str() , m_bcc.pvs);
    if(m_TopJetCollection.size()>0) ConnectVariable( "AnalysisTree", m_TopJetCollection.c_str(), m_bcc.topjets);
    if(m_addGenInfo && m_TopJetCollectionGen.size()>0) ConnectVariable( "AnalysisTree", m_TopJetCollectionGen.c_str() , m_bcc.topjetsgen);
    if(m_PrunedJetCollection.size()>0) ConnectVariable( "AnalysisTree", m_PrunedJetCollection.c_str() , m_bcc.prunedjets);
    if(m_TopTagJetCollection.size()>0) ConnectVariable( "AnalysisTree", m_TopTagJetCollection.c_str(), m_bcc.toptagjets);
    if(m_HiggsTagJetCollection.size()>0) ConnectVariable( "AnalysisTree", m_HiggsTagJetCollection.c_str(), m_bcc.higgstagjets);
    if(m_addGenInfo && m_GenParticleCollection.size()>0) ConnectVariable( "AnalysisTree", m_GenParticleCollection.c_str() , m_bcc.genparticles);
    if(m_PFParticleCollection.size()>0) ConnectVariable( "AnalysisTree", m_PFParticleCollection.c_str() , m_bcc.pfparticles);
    if(m_addGenInfo && m_readCommonInfo) ConnectVariable( "AnalysisTree", "genInfo" , m_bcc.genInfo);
    if(m_readTTbarReco) ConnectVariable( "AnalysisTree", "recoHyps", m_bcc.recoHyps);

    ConnectVariable( "AnalysisTree", "run" , m_bcc.run);
    ConnectVariable( "AnalysisTree", "rho" , m_bcc.rho);
    ConnectVariable( "AnalysisTree", "luminosityBlock" , m_bcc.luminosityBlock);
    ConnectVariable( "AnalysisTree" ,"event" ,m_bcc.event);
    ConnectVariable( "AnalysisTree" ,"isRealData", m_bcc.isRealData);

    if(m_readCommonInfo){
        ConnectVariable( "AnalysisTree", "triggerResults" , m_bcc.triggerResults);
        ConnectVariable( "AnalysisTree", "triggerNames" , m_bcc.triggerNames);
        //ConnectVariable( "AnalysisTree" ,"HBHENoiseFilterResult", m_bcc.HBHENoiseFilterResult);
        ConnectVariable( "AnalysisTree" ,"beamspot_x0", m_bcc.beamspot_x0);
        ConnectVariable( "AnalysisTree" ,"beamspot_y0", m_bcc.beamspot_y0);
        ConnectVariable( "AnalysisTree" ,"beamspot_z0", m_bcc.beamspot_z0);
    }

    //if(m_caTopTagGen.size()>0) ConnectVariable("AnalysisTree", m_caTopTagGen.c_str(), m_bcc.topjets);

    EventCalc* calc = EventCalc::Instance();
    calc->SetBaseCycleContainer(&m_bcc);
    calc->SetLumiHandler(LumiHandler());

    TTree * t = GetInputTree("AnalysisTree");
    m_logger << INFO << "Current file: " << t->GetCurrentFile()->GetName() << SLogger::endmsg;
}

void AnalysisCycle::ExecuteEvent( const SInputData&, Double_t weight) throw( SError )
{
    // This method performs basic consistency checks, resets the event calculator,
    // calculates the pile-up weight and performs the good-run selection.
    // It should always be the first thing to be called in each user analysis.
   nprocessed->Fill(0.5);

    // first thing to do: call reset of event calc
    EventCalc* calc = EventCalc::Instance();
    calc->Reset();

    if(m_bcc.isRealData && m_addGenInfo) {
        m_logger << WARNING<< "Running over real data, but addGenInfo=True?!" << SLogger::endmsg;
    }

    //fill list of trigger names

    FillTriggerNames();

    // generate random run Nr for MC samples (consider luminosity of each run)
    // e.g. for proper OTX cut in MC, and needs to be done only once per event
    if( !m_bcc.isRealData && LumiHandler()->IsLumiCalc() ) {
      m_bcc.run = LumiHandler()->GetRandomRunNr();
    }

    //set the lumiweight to 1 for data
    if(m_bcc.isRealData) weight = 1;

    // store the weight (lumiweight) in the eventcalc class and use it
    calc -> ProduceWeight(weight);

    
    // apply energy shift of tau candidates for uncertainty
    if (m_sys_unc == e_TauEnergy){
      if(m_sys_var == e_Up){
	calc->ApplyTauEnergySmearing(1.03);
      } else {
	calc->ApplyTauEnergySmearing(0.97);
      } 
    }


    if(!m_bcc.isRealData && m_bcc.genInfo) {

        std::vector<float> genweights =  m_bcc.genInfo->weights();
        for(unsigned int i=0; i< genweights.size(); ++i) {
            calc -> ProduceWeight(genweights[i]);
        }

        if(m_puwp) {
            double pu_weight = m_puwp->produceWeight(m_bcc.genInfo);
            // set the weight in the eventcalc
            calc -> ProduceWeight(pu_weight);
        }
	if(m_trig) {
            double trig_weight = m_trig->produceWeight(&m_bcc);
            // set the weight in the eventcalc
            calc -> ProduceWeight(trig_weight);
        }

	if(m_tpr){
	  double tpr_weight=m_tpr->GetScaleWeight();
	  if(m_toppagptweight=="mean"||m_toppagptweight=="Mean"||m_toppagptweight=="MEAN"){
	    //cout <<" wird angewandt" << endl;
	    calc -> ProduceWeight(tpr_weight);
	  }
	  else if(m_toppagptweight=="up"||m_toppagptweight=="Up"||m_toppagptweight=="UP"){
	    calc -> ProduceWeight(tpr_weight*tpr_weight);
	  }
	  else if(m_toppagptweight=="down"||m_toppagptweight=="Down"||m_toppagptweight=="DOWN"){
	    calc -> ProduceWeight(1.);
	  }
	  else{
	    m_logger << ERROR << "Wrong identifier for Top PAG pt re-weighting!!! Accepted only: mean, up, down!" << SLogger::endmsg;
	  }
	}

	if(m_hepsf){
	  if(m_TopTaggingSFMode=="mean"||m_TopTaggingSFMode=="Mean"||m_TopTaggingSFMode=="MEAN"){
	    calc -> ProduceWeight(m_hepsf->GetScaleWeight(0));
	  }
	  else if(m_TopTaggingSFMode=="down"||m_TopTaggingSFMode=="Down"||m_TopTaggingSFMode=="DOWN"){
	    calc -> ProduceWeight(m_hepsf->GetScaleWeight(-1));
	  }
	  else if(m_TopTaggingSFMode=="up"||m_TopTaggingSFMode=="Up"||m_TopTaggingSFMode=="UP"){
	    calc -> ProduceWeight(m_hepsf->GetScaleWeight(+1));
	  }
	  else{
	    m_logger << ERROR << "Wrong identifier for Top-tag SF re-weighting!!! Accepted only: mean, up, down!" << SLogger::endmsg;
	  }
	}

        //lepton scale factor
        if(m_lsf) {
	  calc->ProduceWeight(m_lsf->GetWeight());
        }
	//pdf re-weighting for systematics
	if(m_pdfweights){
	  calc->ProduceWeight(m_pdfweights->GetWeight(m_pdf_index));
	}

	//fill pointers to genjet
	if(calc->GetJets() && calc->GetGenJets()){
	  for(unsigned int i=0; i<calc->GetJets()->size(); ++i){
	    calc->GetJets()->at(i).set_genjet(calc->GetGenJets());
	  }
	}
    }

    //select only good runs
    if(m_bcc.isRealData && LumiHandler()->IsLumiCalc() ) {
        if( !LumiHandler()->PassGoodRunsList( m_bcc.run, m_bcc.luminosityBlock )) throw SError( SError::SkipEvent );
    }

    //create new pointer to recoHyps if no recoHyps were read in
    //note: list of recoHyps is still empty, has to be filled in the user cycle

    if(!m_readTTbarReco && m_writeTTbarReco){  
      delete m_bcc.recoHyps; // would be 0 the first time, but that's Ok
      m_bcc.recoHyps = new std::vector<ReconstructionHypothesis>;
    }

    //apply jet energy corrections
    if(calc->GetJets() && m_corrector){
      Cleaner cleaner;

      cleaner.SetJECUncertainty(m_jes_unc);

      // settings for jet correction uncertainties
      if (m_sys_unc==e_JEC){
	if (m_sys_var==e_Up) cleaner.ApplyJECVariationUp();
	if (m_sys_var==e_Down) cleaner.ApplyJECVariationDown();
      }
      cleaner.JetRecorrector(m_corrector);
    }

    //apply subjet energy corrections to topjet subjets
    if(m_TopJetCollection.size()>0 && m_correctorsubjet){
      Cleaner cleanersub;

      cleanersub.SetJECUncertainty(m_jes_unc_sub);

      // settings for jet correction uncertainties
      if (m_sys_unc==e_subJEC){
	if (m_sys_var==e_Up) cleanersub.ApplyJECVariationUp();
	if (m_sys_var==e_Down) cleanersub.ApplyJECVariationDown();
      }
      
      if(m_extra_subjetJEC.size()>0&&(m_sys_unc==e_subJEC)){
	cleanersub.SubjetRecorrector(m_correctorsubjet,atof(m_extra_subjetJEC.c_str()),atoi(m_onlyUNC_subjetJEC.c_str()));
      }
      else{
	cleanersub.SubjetRecorrector(m_correctorsubjet,1.0,atoi(m_onlyUNC_subjetJEC.c_str()));
      }
    }


    //apply top jet energy corrections
    if(m_TopJetCollection.size()>0 && m_correctortop){
      Cleaner cleanertop;

      cleanertop.SetJECUncertainty(m_jes_unc_top);

      // settings for jet correction uncertainties
      if (m_sys_unc==e_JEC){
	if (m_sys_var==e_Up) cleanertop.ApplyJECVariationUp();
	if (m_sys_var==e_Down) cleanertop.ApplyJECVariationDown();
      }
      
      if(m_extra_topJEC.size()>0&&(m_sys_unc==e_JEC)){
	cleanertop.JetRecorrector(m_correctortop,true,true,false,false,false,atof(m_extra_topJEC.c_str()));
      }
      else{
	cleanertop.JetRecorrector(m_correctortop,true,true,false,false,false,1.0);
      }
    }

    //apply toptag jet energy corrections
    if(m_TopTagJetCollection.size()>0 && m_correctortoptag){
      Cleaner cleanertoptag;
      cleanertoptag.JetRecorrector(m_correctortoptag,true,false,false,true);
    }

    //apply higgstag jet energy corrections
    if(m_HiggsTagJetCollection.size()>0&& m_correctorhiggstag){
      Cleaner cleanerhiggstag;
      cleanerhiggstag.JetRecorrector(m_correctorhiggstag,true,false,false,false,true);
    }

}


void AnalysisCycle::ClearEvent() throw( SError )
{
  throw SError( SError::SkipEvent );
}

void AnalysisCycle::FillTriggerNames()
{

    if(!m_bcc.triggerNames) return;

    //remove trigger list when starting a new run
    if( m_bcc.run != m_actual_run && m_bcc.isRealData){
      m_bcc.triggerNames_actualrun.clear();
      m_newrun=true;
    }
    m_actual_run = m_bcc.run;

    //fill list of trigger names
    if(m_bcc.triggerNames->size()!=0) {
        m_bcc.triggerNames_actualrun = *m_bcc.triggerNames;
        m_newrun=true;
    }
    
    if(m_bcc.triggerNames_actualrun.size()==0){

      m_logger << WARNING<< "No trigger table found for this event -> start trigger search on following events" << SLogger::endmsg;
 
      int tmp_run= m_bcc.run;

      TTree* tmp_tree = GetInputTree("AnalysisTree");

      int processed_events = tmp_tree->GetReadEvent();

      tmp_tree->SetBranchStatus("*",0);
      tmp_tree->SetBranchStatus("run",1); 
      tmp_tree->SetBranchStatus("triggerNames",1);

      int N_ent= tmp_tree->GetEntriesFast();

      for(int i=0; i<N_ent; ++i){

	//if(i%10000==0) std::cout << "Search trigger " << i << std::endl;
	tmp_tree->GetEntry(i);

	//search for next event in tree with trigger table filled, check for same run number in case of real data
	if(m_bcc.triggerNames->size()!=0 && (!m_bcc.isRealData || m_bcc.run==tmp_run)){
	  m_bcc.triggerNames_actualrun = *m_bcc.triggerNames;
	  m_newrun=true;
	  m_logger << WARNING<< "Trigger search was succesful" << SLogger::endmsg;
	  break;
	}
      }

      //go back to original event
      tmp_tree->SetBranchStatus("*",1);
      tmp_tree->GetEntry(processed_events);

    }
    if(m_bcc.triggerNames_actualrun.size()==0){
      m_logger << ERROR << "Trigger search was NOT succesful!!!" << SLogger::endmsg;
    }

}

void AnalysisCycle::WriteOutputTree() throw( SError)
{
    //write out all objects
    m_output_photons.clear();
    m_output_jets.clear();
    m_output_genjets.clear();
    m_output_electrons.clear();
    m_output_muons.clear();
    m_output_taus.clear();
    m_output_pvs.clear();
    m_output_topjets.clear();
    m_output_topjetsgen.clear();
    m_output_prunedjets.clear();
    m_output_toptagjets.clear();
    m_output_higgstagjets.clear();
    m_output_genparticles.clear();
    m_output_triggerNames.clear();
    m_output_triggerResults.clear();
    m_output_recoHyps.clear();

    if(m_PhotonCollection.size()>0) m_output_photons=*m_bcc.photons;
    if(m_JetCollection.size()>0) m_output_jets=*m_bcc.jets;
    if(m_addGenInfo && m_GenJetCollection.size()>0) m_output_genjets=*m_bcc.genjets;
    if(m_ElectronCollection.size()>0) m_output_electrons=*m_bcc.electrons;
    if(m_MuonCollection.size()>0) m_output_muons=*m_bcc.muons;
    if(m_TauCollection.size()>0) m_output_taus=*m_bcc.taus;
    if(m_PrimaryVertexCollection.size()>0) m_output_pvs=*m_bcc.pvs;
    if(m_METName.size()>0) m_output_met = *m_bcc.met;
    if(m_addGenInfo) m_output_genInfo = *m_bcc.genInfo;
    if(m_TopJetCollection.size()>0) m_output_topjets=*m_bcc.topjets;
    if(m_TopTagJetCollection.size()>0) m_output_toptagjets=*m_bcc.toptagjets;
    if(m_HiggsTagJetCollection.size()>0) m_output_higgstagjets=*m_bcc.higgstagjets;
    if(m_addGenInfo && m_TopJetCollectionGen.size()>0) m_output_topjetsgen=*m_bcc.topjetsgen;
    if(m_PrunedJetCollection.size()>0) m_output_prunedjets=*m_bcc.prunedjets;
    if(m_addGenInfo && m_GenParticleCollection.size()>0) m_output_genparticles=*m_bcc.genparticles;
    if(m_PFParticleCollection.size()>0) m_output_pfparticles=*m_bcc.pfparticles;
    if(m_writeTTbarReco) m_output_recoHyps=*m_bcc.recoHyps;

    if(m_newrun) m_output_triggerNames = m_bcc.triggerNames_actualrun;//store trigger names only for new runs
    m_newrun=false;

    m_output_triggerResults = *m_bcc.triggerResults;
}

