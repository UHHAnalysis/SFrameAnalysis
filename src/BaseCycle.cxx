// $Id: BaseCycle.cxx,v 1.1 2012/04/02 15:28:01 peiffer Exp $

// Local include(s):
#include "../include/BaseCycle.h"

ClassImp( BaseCycle );


BaseCycle::BaseCycle()
   : SCycleBase() {

   SetLogName( GetName() );
}

BaseCycle::~BaseCycle() {

}

void BaseCycle::BeginCycle() throw( SError ) {
  
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

  return;

}

void BaseCycle::EndCycle() throw( SError ) {

   return;

}

void BaseCycle::BeginInputData( const SInputData& ) throw( SError ) {
  
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
  DeclareVariable( o_L1_prescale, "L1_prescale");  
  DeclareVariable( o_HLT_prescale, "HLT_prescale");  

  //
  // Declare the output histograms:
  //
  Book( TH1F( "N_lep_hist", "N^{lep}", 10,0,10 ) );
  Book( TH1F( "pt_lep_hist", "p_{T}^{lep}", 100,0,500 ) );
   
  return;

}

void BaseCycle::EndInputData( const SInputData& ) throw( SError ) {

   return;

}

void BaseCycle::BeginInputFile( const SInputData& ) throw( SError ) {
  //ConnectVariable( "JetTree", "pt_jet_1", pt_jet_1 );
  //ConnectVariable( "JetTree", "pt_jet_2", pt_jet_2 );

  ConnectVariable( "AnalysisTree", "triggerResults" , bcc.triggerResults);
  ConnectVariable( "AnalysisTree", "triggerNames" , bcc.triggerNames);
  ConnectVariable( "AnalysisTree", "L1_prescale" , bcc.L1_prescale);
  ConnectVariable( "AnalysisTree", "HLT_prescale" , bcc.HLT_prescale);
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
 
  // for(unsigned int i=0; i<triggerNames->size();++i){
  //   std::cout << triggerNames->at(i) << "   " << triggerResults->at(i) << std::endl;
  // }
 
  if(bcc.isRealData && addGenInfo){
    std::cout << "WARNING : this seems to be real data but addGenInfo=True in config file" << std::endl;
  }


  //clean collections here
  
  for(unsigned int i=0; i<bcc.jets->size(); ++i){
    //std::cout << jets->at(i).v4().pt() << "   " << jets->at(i).pt << std::endl;
    if(bcc.jets->at(i).v4().pt()<30){
      bcc.jets->erase(bcc.jets->begin()+i);
      i--;
    }
  }

  //selection

  Selection selection(&bcc);
 
  int min_toptag=1;
  if(!selection.TopTagging(min_toptag)) throw SError( SError::SkipEvent );
  
  //analysis code

  std::vector<Particle> leptons;
  for(unsigned int i=0; i<bcc.electrons->size(); ++i){
    leptons.push_back(bcc.electrons->at(i));
  }
  for(unsigned int i=0; i<bcc.muons->size(); ++i){
    leptons.push_back(bcc.muons->at(i));
  }
  for(unsigned int i=0; i<bcc.taus->size(); ++i){
    leptons.push_back(bcc.taus->at(i));
  }
  Hist( "N_lep_hist" )->Fill( leptons.size(), weight );
  for(unsigned int i=0; i<leptons.size(); ++i){
    Hist( "pt_lep_hist" )->Fill( leptons[i].pt, weight );
  }

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

  o_triggerNames = *bcc.triggerNames;
  o_triggerResults = *bcc.triggerResults;
  o_L1_prescale = *bcc.L1_prescale;
  o_HLT_prescale = *bcc.HLT_prescale;

  return;

}

