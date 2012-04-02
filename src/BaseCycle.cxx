// $Id: CycleCreators.py 159 2010-04-13 09:44:22Z krasznaa $

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
  
  DeclareVariable( run, "run" );
  DeclareVariable( luminosityBlock, "luminosityBlock" );
  DeclareVariable( event, "event" );
  DeclareVariable( isRealData, "isRealData" );
  DeclareVariable( HBHENoiseFilterResult, "HBHENoiseFilterResult" );
  DeclareVariable( beamspot_x0, "beamspot_x0");
  DeclareVariable( beamspot_y0, "beamspot_y0");
  DeclareVariable( beamspot_z0, "beamspot_z0");

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

  ConnectVariable( "AnalysisTree", "triggerResults" , triggerResults);
  ConnectVariable( "AnalysisTree", "triggerNames" , triggerNames);
  ConnectVariable( "AnalysisTree", "L1_prescale" , L1_prescale);
  ConnectVariable( "AnalysisTree", "HLT_prescale" , HLT_prescale);
  if(ElectronCollection.size()>0) ConnectVariable( "AnalysisTree", ElectronCollection.c_str() , electrons);
  if(MuonCollection.size()>0) ConnectVariable( "AnalysisTree", MuonCollection.c_str() , muons); 
  if(TauCollection.size()>0) ConnectVariable( "AnalysisTree", TauCollection.c_str() , taus);
  if(JetCollection.size()>0) ConnectVariable( "AnalysisTree", JetCollection.c_str() , jets);
  if(PhotonCollection.size()>0) ConnectVariable( "AnalysisTree", PhotonCollection.c_str() , photons);
  if(METName.size()>0) ConnectVariable( "AnalysisTree", METName.c_str() , met);
  if(PrimaryVertexCollection.size()>0) ConnectVariable( "AnalysisTree", PrimaryVertexCollection.c_str() , pvs);
  if(TopJetCollection.size()>0) ConnectVariable( "AnalysisTree", TopJetCollection.c_str() , topjets);
  if(PrunedJetCollection.size()>0) ConnectVariable( "AnalysisTree", PrunedJetCollection.c_str() , prunedjets);
  if(GenParticleCollection.size()>0) ConnectVariable( "AnalysisTree", GenParticleCollection.c_str() , genparticles);
  if(addGenInfo) ConnectVariable( "AnalysisTree", "genInfo" , genInfo);
  ConnectVariable( "AnalysisTree", "run" , run);
  ConnectVariable( "AnalysisTree", "luminosityBlock" , luminosityBlock);
  ConnectVariable( "AnalysisTree" ,"event" ,event);
  ConnectVariable( "AnalysisTree" ,"isRealData", isRealData);
  ConnectVariable( "AnalysisTree" ,"HBHENoiseFilterResult", HBHENoiseFilterResult);
  ConnectVariable( "AnalysisTree" ,"beamspot_x0", beamspot_x0);
  ConnectVariable( "AnalysisTree" ,"beamspot_y0", beamspot_y0);
  ConnectVariable( "AnalysisTree" ,"beamspot_z0", beamspot_z0);

  return;

}

void BaseCycle::ExecuteEvent( const SInputData&, Double_t weight) throw( SError ) {
 
  // for(unsigned int i=0; i<triggerNames->size();++i){
  //   std::cout << triggerNames->at(i) << "   " << triggerResults->at(i) << std::endl;
  // }
 
  if(isRealData && addGenInfo){
    std::cout << "WARNING : this seems to be real data but addGenInfo=True in config file" << std::endl;
  }


  //clean collections here
  
  for(unsigned int i=0; i<jets->size(); ++i){
    //std::cout << jets->at(i).v4().pt() << "   " << jets->at(i).pt << std::endl;
    if(jets->at(i).v4().pt()<30){
      jets->erase(jets->begin()+i);
      i--;
    }
  }

  //selection

  if(jets->size()<2)  throw SError( SError::SkipEvent );
  
  //analysis code

  std::vector<Particle> leptons;
  for(unsigned int i=0; i<electrons->size(); ++i){
    leptons.push_back(electrons->at(i));
  }
  for(unsigned int i=0; i<muons->size(); ++i){
    leptons.push_back(muons->at(i));
  }
  for(unsigned int i=0; i<taus->size(); ++i){
    leptons.push_back(taus->at(i));
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

  if(PhotonCollection.size()>0) o_photons=*photons;
  if(JetCollection.size()>0) o_jets=*jets;
  if(ElectronCollection.size()>0) o_electrons=*electrons;
  if(MuonCollection.size()>0) o_muons=*muons;
  if(TauCollection.size()>0) o_taus=*taus;
  if(PrimaryVertexCollection.size()>0) o_pvs=*pvs;
  if(METName.size()>0) o_met = *met;
  if(addGenInfo) o_genInfo = *genInfo;
  if(TopJetCollection.size()>0) o_topjets=*topjets;
  if(PrunedJetCollection.size()>0) o_prunedjets=*prunedjets;
  if(GenParticleCollection.size()>0) o_genparticles=*genparticles;

  o_triggerNames = *triggerNames;
  o_triggerResults = *triggerResults;
  o_L1_prescale = *L1_prescale;
  o_HLT_prescale = *HLT_prescale;
  //std::cout<< met->pt << "   "  << o_met.pt << std::endl;

  return;

}

