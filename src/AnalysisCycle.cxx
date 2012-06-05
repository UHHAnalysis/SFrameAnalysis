// $Id: CycleCreators.py 159 2010-04-13 09:44:22Z krasznaa $

#include <iostream>

using namespace std;

// Local include(s):
#include "include/AnalysisCycle.h"
#include "include/SelectionModules.h"
#include "include/ExampleHists.h"
#include "include/ObjectHandler.h"
#include "include/EventCalc.h"

ClassImp( AnalysisCycle );

AnalysisCycle::AnalysisCycle()
   : SCycleBase() {

  // constructor, declare variables that should be obtained 
  // from the steering-xml file
  
  SetLogName( GetName() );

  m_puwp = NULL;
  m_newrun = false;
  
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
  DeclareProperty( "PrunedJetCollection", m_PrunedJetCollection );
  DeclareProperty( "addGenInfo", m_addGenInfo);
  DeclareProperty( "GenParticleCollection", m_GenParticleCollection);
  
  // steerable properties for the Pile-up reweighting
  DeclareProperty( "PU_Filename_MC" , m_PUFilenameMC);
  DeclareProperty( "PU_Filename_Data" , m_PUFilenameData);
  DeclareProperty( "PU_Histname_MC" , m_PUHistnameMC);
  DeclareProperty( "PU_Histname_Data" , m_PUHistnameData);

}

AnalysisCycle::~AnalysisCycle() 
{
  // destructor
  
  if (m_puwp){
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
  // clean-up, info messages and final calculations after the analysis

  PrintSelectionSummary();
  
  FinaliseHistos();

  return;

}

void AnalysisCycle::BeginInputData( const SInputData& ) throw( SError ) 
{
  // declaration of variables and histograms

  // check if LumiHandler is set up correctly
  if( LumiHandler() == NULL ){
    m_logger << FATAL << "Luminosity Handler not properly added to Configuration!" << SLogger::endmsg;
    exit(-1);
  }

  // Overwrite target Luminosity
  // has to be done for every input data 
  if( LumiHandler()->IsLumiCalc() ){
    m_logger << WARNING << "--- Not a WARNING! --- Overwrite Target Lumi with Lumi from LumiFile: " 
	     << LumiHandler()->GetTargetLumi() << " (pb^-1)!" << SLogger::endmsg;
    GetConfig().SetTargetLumi( LumiHandler()->GetTargetLumi() );
  } 

  // pile-up reweighting
  if(m_PUFilenameMC.size()>0 && m_PUFilenameData.size()>0 && m_PUHistnameMC.size()>0 && m_PUHistnameData.size()>0){
    m_logger << INFO << "PU Reweighting will be performed. File(data) = " << m_PUFilenameData
	     << " File(MC) = " << m_PUFilenameMC << SLogger::endmsg;
    m_logger << INFO << "PU, histograms: Hist(data) = " << m_PUHistnameData
	     << " Hist(MC) = " << m_PUHistnameMC << SLogger::endmsg;
    m_puwp = new PUWeightProducer(m_PUFilenameMC, m_PUFilenameData, m_PUHistnameMC, m_PUHistnameData);
  }

  // output Ntuple for preselection
  //  if (OutputTree){ // defined in XML file?
//   DeclareVariable( m_bcc.run, "run" );
//   DeclareVariable( m_bcc.luminosityBlock, "luminosityBlock" );
//   DeclareVariable( m_bcc.event, "event" );
//   DeclareVariable( m_bcc.isRealData, "isRealData" );
//   DeclareVariable( m_bcc.HBHENoiseFilterResult, "HBHENoiseFilterResult" );
//   DeclareVariable( m_bcc.beamspot_x0, "beamspot_x0");
//   DeclareVariable( m_bcc.beamspot_y0, "beamspot_y0");
//   DeclareVariable( m_bcc.beamspot_z0, "beamspot_z0");
//   if(m_ElectronCollection.size()>0) DeclareVariable( o_electrons, ElectronCollection.c_str() );
//   if(m_MuonCollection.size()>0) DeclareVariable( o_muons, MuonCollection.c_str() ); 
//   if(m_TauCollection.size()>0) DeclareVariable( o_taus, TauCollection.c_str() );
//   if(JetCollection.size()>0) DeclareVariable( o_jets, JetCollection.c_str() );
//   if(PhotonCollection.size()>0) DeclareVariable( o_photons, PhotonCollection.c_str() );
//   if(METName.size()>0) DeclareVariable( o_met, METName.c_str() );
//   if(PrimaryVertexCollection.size()>0) DeclareVariable( o_pvs, PrimaryVertexCollection.c_str());
//   if(TopJetCollection.size()>0) DeclareVariable( o_topjets, TopJetCollection.c_str());
//   if(PrunedJetCollection.size()>0) DeclareVariable( o_prunedjets, PrunedJetCollection.c_str());
//   if(GenParticleCollection.size()>0) DeclareVariable( o_genparticles, GenParticleCollection.c_str());
//   if(addGenInfo) DeclareVariable( o_genInfo, "genInfo" );
//   DeclareVariable( o_triggerNames, "triggerNames");
//   DeclareVariable( o_triggerResults, "triggerResults"); 
//  }
  
  return;

}


void AnalysisCycle::RegisterSelection(Selection* sel)
{
  // register a selection in the list of selections

  m_selections.push_back(sel);
  return;
}


void AnalysisCycle::RegisterHistCollection(BaseHists* hists)
{
  // register a collection of histograms in the list

  m_histcollections.push_back(hists);
  return;
}


Selection* AnalysisCycle::GetSelection(const std::string name)
{
  // get a selection from the list of selections

  TString InName(name.c_str());
  for (unsigned int i=0; i<m_selections.size(); ++i){
    TString SelName = m_selections[i]->GetName();
    if (InName == SelName){
      return m_selections[i];
    }
  }
  m_logger << WARNING << "Could not find selection with name " << InName << "." << SLogger::endmsg;
  return NULL;
}


BaseHists* AnalysisCycle::GetHistCollection(const std::string name)
{
  // get a histogram collection from the list
  
  TString InName(name.c_str());
  for (unsigned int i=0; i<m_histcollections.size(); ++i){
    TString HistName = m_histcollections[i]->GetName();
    if (InName == HistName){
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

  for (unsigned int i=0; i<m_histcollections.size(); ++i){
    m_histcollections[i]->SetHistOutput(this->GetHistOutput());
    m_histcollections[i]->Init();
  }

  return;

}

void AnalysisCycle::FinaliseHistos()
{
  // finalise all registered histograms

  for (unsigned int i=0; i<m_histcollections.size(); ++i){
    m_histcollections[i]->Finish();
  }

  return;

}

void AnalysisCycle::PrintSelectionSummary()
{
  // print a summary of all selections

  for (unsigned int i=0; i<m_selections.size(); ++i){
    m_selections[i]->printCutFlow();
  }

}


void AnalysisCycle::EndInputData( const SInputData& ) throw( SError ) 
{

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
  if(m_PrunedJetCollection.size()>0) ConnectVariable( "AnalysisTree", m_PrunedJetCollection.c_str() , m_bcc.prunedjets);
  if(m_GenParticleCollection.size()>0) ConnectVariable( "AnalysisTree", m_GenParticleCollection.c_str() , m_bcc.genparticles);
  if(m_addGenInfo) ConnectVariable( "AnalysisTree", "genInfo" , m_bcc.genInfo);
  ConnectVariable( "AnalysisTree", "run" , m_bcc.run);
  ConnectVariable( "AnalysisTree", "luminosityBlock" , m_bcc.luminosityBlock);
  ConnectVariable( "AnalysisTree" ,"event" ,m_bcc.event);
  ConnectVariable( "AnalysisTree" ,"isRealData", m_bcc.isRealData);
  //ConnectVariable( "AnalysisTree" ,"HBHENoiseFilterResult", m_bcc.HBHENoiseFilterResult);
  ConnectVariable( "AnalysisTree" ,"beamspot_x0", m_bcc.beamspot_x0);
  ConnectVariable( "AnalysisTree" ,"beamspot_y0", m_bcc.beamspot_y0);
  ConnectVariable( "AnalysisTree" ,"beamspot_z0", m_bcc.beamspot_z0);

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

  if(m_bcc.isRealData && m_addGenInfo){
    m_logger << WARNING<< "Running over real data, but addGenInfo=True in config file?!" << SLogger::endmsg;
  }

  //fill list of trigger names
  if(m_bcc.triggerNames->size()!=0){
    m_bcc.triggerNames_actualrun = *m_bcc.triggerNames;
    m_newrun=true;
  }

  // generate random run Nr for MC samples (consider luminosity of each run)
  // e.g. for proper OTX cut in MC, and needs to be done only once per event
  if( !m_bcc.isRealData && LumiHandler()->IsLumiCalc() ){
    m_bcc.run = LumiHandler()->GetRandomRunNr();
  }


  // store the weight (lumiweight) in the eventcalc class and use it 

  if(m_bcc.genInfo){
    if(m_puwp){
      double pu_weight = m_puwp->produceWeight(m_bcc.genInfo);
      // set the weight in the eventcalc
    }
  }

  //select only good runs
  if(m_bcc.isRealData && LumiHandler()->IsLumiCalc() ){
    if( !LumiHandler()->PassGoodRunsList( m_bcc.run, m_bcc.luminosityBlock )) throw SError( SError::SkipEvent );
  }


  return;
  
}

