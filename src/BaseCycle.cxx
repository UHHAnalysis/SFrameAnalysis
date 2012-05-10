// $Id: BaseCycle.cxx,v 1.9 2012/05/07 14:25:56 peiffer Exp $

// Local include(s):
#include "../include/BaseCycle.h"

ClassImp( BaseCycle );


BaseCycle::BaseCycle()
   : SCycleBase() {

  SetLogName( GetName() );
  newrun=true;
  
  DeclareProperty( "JetCollection", JetCollection );
  DeclareProperty( "ElectronCollection", ElectronCollection );
  DeclareProperty( "MuonCollection", MuonCollection );
  DeclareProperty( "TauCollection", TauCollection );
  DeclareProperty( "PhotonCollection", PhotonCollection );
  DeclareProperty( "PrimaryVertexCollection", PrimaryVertexCollection );
  DeclareProperty( "METName", METName );
  DeclareProperty( "TopJetCollection", TopJetCollection );
  DeclareProperty( "PrunedJetCollection", PrunedJetCollection );
  DeclareProperty( "addGenInfo", addGenInfo);
  DeclareProperty( "GenParticleCollection", GenParticleCollection);
  
  DeclareProperty( "pu_filename_mc" ,pu_filename_mc);
  DeclareProperty( "pu_filename_data" ,pu_filename_data);
  DeclareProperty( "pu_histname_mc" ,pu_histname_mc);
  DeclareProperty( "pu_histname_data" ,pu_histname_data);

}

BaseCycle::~BaseCycle() {

}

void BaseCycle::BeginCycle() throw( SError ) {
  
  // Set-Up LumiHandler
  LuminosityHandler *lumiHandler = new LuminosityHandler();
  // Declared Properties readable not before BeginCycle
  
  lumiHandler->SetGRLPath( "/afs/naf.desy.de/user/p/peiffer/CMSSW_5_2_3_patch4/src/UHHAnalysis/NtupleWriter/" );
  lumiHandler->SetLumiFileName( "GoodRun.root" );
  lumiHandler->SetTrigger( "HLT_PFJet320_v" );
  lumiHandler->SetIntLumiPerBin( 25 );
  
  // Initialise, checks also if LumiFile is specified and readable
  lumiHandler->Initialise();
  // dump lumi information in text file
  // lumiHandler->DumpLumiInfoIntoTxtFile();
  
  // adding luminosity handler to gloabl config
  AddConfigObject( lumiHandler );
  return;

}

void BaseCycle::EndCycle() throw( SError ) {

   return;

}

void BaseCycle::BeginInputData( const SInputData& ) throw( SError ) {


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

  
  DeclareVariable( bcc.run, "run" );
  DeclareVariable( bcc.luminosityBlock, "luminosityBlock" );
  DeclareVariable( bcc.event, "event" );
  DeclareVariable( bcc.isRealData, "isRealData" );
  DeclareVariable( bcc.HBHENoiseFilterResult, "HBHENoiseFilterResult" );
  DeclareVariable( bcc.beamspot_x0, "beamspot_x0");
  DeclareVariable( bcc.beamspot_y0, "beamspot_y0");
  DeclareVariable( bcc.beamspot_z0, "beamspot_z0");

  if(ElectronCollection.size()>0) DeclareVariable( o_electrons, ElectronCollection.c_str() );
  if(MuonCollection.size()>0) DeclareVariable( o_muons, MuonCollection.c_str() ); 
  if(TauCollection.size()>0) DeclareVariable( o_taus, TauCollection.c_str() );
  if(JetCollection.size()>0) DeclareVariable( o_jets, JetCollection.c_str() );
  if(PhotonCollection.size()>0) DeclareVariable( o_photons, PhotonCollection.c_str() );
  if(METName.size()>0) DeclareVariable( o_met, METName.c_str() );
  if(PrimaryVertexCollection.size()>0) DeclareVariable( o_pvs, PrimaryVertexCollection.c_str());
  if(TopJetCollection.size()>0) DeclareVariable( o_topjets, TopJetCollection.c_str());
  if(PrunedJetCollection.size()>0) DeclareVariable( o_prunedjets, PrunedJetCollection.c_str());
  if(GenParticleCollection.size()>0) DeclareVariable( o_genparticles, GenParticleCollection.c_str());
  if(addGenInfo) DeclareVariable( o_genInfo, "genInfo" );
  DeclareVariable( o_triggerNames, "triggerNames");
  DeclareVariable( o_triggerResults, "triggerResults");  

  if(pu_filename_mc.size()>0 && pu_filename_data.size()>0 && pu_histname_mc.size()>0 && pu_histname_data.size()>0){
    puwp = new PUWeightProducer(pu_filename_mc, pu_filename_data, pu_histname_mc, pu_histname_data);
  }

  return;

}

void BaseCycle::EndInputData( const SInputData& ) throw( SError ) {

  // LumiHandler()->PrintUsedSetup();
  
  // LumiBins vs. integrated luminosity for that bin
//   if( LumiHandler()->IsLumiCalc() ){
//     m_logger << INFO << "Target Lumi in data files and used for weighting: " 
// 	     << LumiHandler()->GetTargetLumi() << " (pb^-1)!" << SLogger::endmsg;
    
//     // store lumi hist to file
//     WriteObj( *(LumiHandler()->GetLumiPerLumiBinHist()) ); 
    
//     // store target luminosity (luminosity in data)
//     TTree *tree = new TTree( "LuminosityTree", "luminosity tree" );
//     Double_t lumi = LumiHandler()->GetTargetLumi();
//     tree->Branch( "targetLuminosity", &lumi );
//     tree->Fill();
//     WriteObj( *tree );
    
//     // store the luminosity collected in each run
//     WriteObj( *(LumiHandler()->GetTreeLuminosityPerRun()) ); 
//   }
  return;
  
}

void BaseCycle::BeginInputFile( const SInputData& ) throw( SError ) {

  ConnectVariable( "AnalysisTree", "triggerResults" , bcc.triggerResults);
  ConnectVariable( "AnalysisTree", "triggerNames" , bcc.triggerNames);
  if(ElectronCollection.size()>0) ConnectVariable( "AnalysisTree", ElectronCollection.c_str() ,bcc. electrons);
  if(MuonCollection.size()>0) ConnectVariable( "AnalysisTree", MuonCollection.c_str() , bcc.muons); 
  if(TauCollection.size()>0) ConnectVariable( "AnalysisTree", TauCollection.c_str() , bcc.taus);
  if(JetCollection.size()>0) ConnectVariable( "AnalysisTree", JetCollection.c_str() , bcc.jets);
  if(PhotonCollection.size()>0) ConnectVariable( "AnalysisTree", PhotonCollection.c_str() , bcc.photons);
  if(METName.size()>0) ConnectVariable( "AnalysisTree", METName.c_str() , bcc.met);
  if(PrimaryVertexCollection.size()>0) ConnectVariable( "AnalysisTree", PrimaryVertexCollection.c_str() , bcc.pvs);
  if(TopJetCollection.size()>0) ConnectVariable( "AnalysisTree", TopJetCollection.c_str() , bcc.topjets);
  if(PrunedJetCollection.size()>0) ConnectVariable( "AnalysisTree", PrunedJetCollection.c_str() , bcc.prunedjets);
  if(GenParticleCollection.size()>0) ConnectVariable( "AnalysisTree", GenParticleCollection.c_str() , bcc.genparticles);
  if(addGenInfo) ConnectVariable( "AnalysisTree", "genInfo" , bcc.genInfo);
  
//     unsigned int myrun=0;
//     ConnectVariable( "AnalysisTree", "run" , myrun);
//     bcc.run = (int)myrun;
//     unsigned int mylb=0;
//     ConnectVariable( "AnalysisTree", "luminosityBlock" , mylb);
//     bcc.luminosityBlock = (int) mylb;
  
  
  
  ConnectVariable( "AnalysisTree", "run" , bcc.run);
  ConnectVariable( "AnalysisTree", "luminosityBlock" , bcc.luminosityBlock);
  
  ConnectVariable( "AnalysisTree" ,"event" ,bcc.event);
  ConnectVariable( "AnalysisTree" ,"isRealData", bcc.isRealData);
  ConnectVariable( "AnalysisTree" ,"HBHENoiseFilterResult", bcc.HBHENoiseFilterResult);
  ConnectVariable( "AnalysisTree" ,"beamspot_x0", bcc.beamspot_x0);
  ConnectVariable( "AnalysisTree" ,"beamspot_y0", bcc.beamspot_y0);
  ConnectVariable( "AnalysisTree" ,"beamspot_z0", bcc.beamspot_z0);

  return;

}

void BaseCycle::ExecuteEvent( const SInputData&, Double_t weight) throw( SError ) {

  if(bcc.isRealData && addGenInfo){
    m_logger << WARNING<< "this seems to be real data but addGenInfo=True in config file" << SLogger::endmsg;
  }

  //fill list of trigger names
  if(bcc.triggerNames->size()!=0){
    bcc.triggerNames_actualrun = *bcc.triggerNames;
    newrun=true;
  }

  // generate random run Nr for MC samples (consider luminosity of each run)
  // e.g. for proper OTX cut in MC, and needs to be done only once per event
  if( !bcc.isRealData && LumiHandler()->IsLumiCalc() )
    bcc.run = LumiHandler()->GetRandomRunNr() ;


  if(bcc.genInfo){
    if(puwp){
      weight *= puwp->produceWeight(bcc.genInfo);
      //std::cout << bcc.genInfo->pileup_TrueNumInteractions << "   " << puwp->produceWeight(bcc.genInfo) <<std::endl;
    }
  }

  //clean collections here

  Cleaner cleaner(&bcc);

  if(!bcc.isRealData && bcc.jets) cleaner.JetEnergyResolutionShifter();
  //if(bcc.electrons) cleaner.ElectronCleaner(20,2.5);
  //if(bcc.muons) cleaner.MuonCleaner(20,2.1);
  //if(bcc.jets) cleaner.JetCleaner(35,2.5,true);
  //if(bcc.topjets) cleaner.TopJetCleaner(350,2.5,false);
  //if(bcc.taus) cleaner.TauCleaner(20,2.1);

  //selection

  Selection selection(&bcc);

  //select only good runs
  if(bcc.isRealData && LumiHandler()->IsLumiCalc() ){
    if( !LumiHandler()->PassGoodRunsList( bcc.run, bcc.luminosityBlock )) throw SError( SError::SkipEvent );
  }

  //HBHE noise filter only for data
  if(bcc.isRealData)
    if(!selection.HBHENoiseFilter()) throw SError( SError::SkipEvent );

  //trigger

  //DO NOT use trigger selection in PROOF mode for the moment
  //if(!selection.TriggerSelection("HLT_PFJet320_v"))  throw SError( SError::SkipEvent );

  //write out all objects
  
  o_photons.clear();
  o_jets.clear();
  o_electrons.clear();
  o_muons.clear();
  o_taus.clear();
  o_pvs.clear();
  o_topjets.clear();
  o_prunedjets.clear();
  o_genparticles.clear();
  o_triggerNames.clear();
  o_triggerResults.clear();
  
  if(PhotonCollection.size()>0) o_photons=*bcc.photons;
  if(JetCollection.size()>0) o_jets=*bcc.jets;
  if(ElectronCollection.size()>0) o_electrons=*bcc.electrons;
  if(MuonCollection.size()>0) o_muons=*bcc.muons;
  if(TauCollection.size()>0) o_taus=*bcc.taus;
  if(PrimaryVertexCollection.size()>0) o_pvs=*bcc.pvs;
  if(METName.size()>0) o_met = *bcc.met;
  if(addGenInfo) o_genInfo = *bcc.genInfo;
  if(TopJetCollection.size()>0) o_topjets=*bcc.topjets;
  if(PrunedJetCollection.size()>0) o_prunedjets=*bcc.prunedjets;
  if(GenParticleCollection.size()>0) o_genparticles=*bcc.genparticles;
  
  if(newrun) o_triggerNames = bcc.triggerNames_actualrun;//store trigger names only for new runs
  newrun=false;
  o_triggerResults = *bcc.triggerResults;

  return;

}

