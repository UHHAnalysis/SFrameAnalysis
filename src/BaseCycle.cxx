// $Id: BaseCycle.cxx,v 1.5 2012/04/10 15:36:44 peiffer Exp $

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

  if(pu_filename_mc.size()>0 && pu_filename_data.size()>0 && pu_histname_mc.size()>0 && pu_histname_data.size()>0){
    puwp = new PUWeightProducer(pu_filename_mc, pu_filename_data, pu_histname_mc, pu_histname_data);
  }

  //
  // Declare the output histograms:
  //
  Book( TH1F( "N_lep_hist", "N^{lep}", 10,0,10 ) );
  Book( TH1F( "pt_lep_hist", "p_{T}^{lep}", 100,0,500 ) );
  Book( TH1F( "Mjet_hist", "m_{jet}", 100,0,500 ) );
  Book( TH1F( "Mmin_hist", "m_{min}", 100,0,200 ) );
  Book( TH1F( "Nsubjet_hist", "N^{subjet}", 10,0,10 ) );
  Book( TH1F( "N_pileup_hist", "N^{PU}", 1000,0,50 ) );
  Book( TH1F( "DR_jj_hist", "#Delta R(jj)", 100,0,6 ) );
  Book( TH1F( "DR_tau_nextjet", "#Delta R(#tau j)",100,0,6));
  Book( TH1F( "DR_tau_nexttau", "#Delta R(#tau #tau)",100,0,6)); 
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
 
//   for(unsigned int i=0; i<bcc.triggerNames->size();++i){
//     std::cout << bcc.triggerNames->at(i) << "   " << bcc.triggerResults->at(i) << std::endl;
//   }
  
  if(bcc.isRealData && addGenInfo){
    m_logger << WARNING<< "this seems to be real data but addGenInfo=True in config file" << SLogger::endmsg;
  }

  //fill list of trigger names
  if(bcc.triggerNames->size()!=0){
    bcc.triggerNames_actualrun = *bcc.triggerNames;
    newrun=true;
  }


  if(bcc.genInfo){
    if(puwp){
      weight *= puwp->produceWeight(bcc.genInfo);
      //std::cout << bcc.genInfo->pileup_TrueNumInteractions << "   " << puwp->produceWeight(bcc.genInfo) <<std::endl;
    }
    double npu = bcc.genInfo->pileup_TrueNumInteractions;
    if(npu>50) npu=49.9999;
    Hist( "N_pileup_hist" )->Fill( npu, weight );
  }

  //clean collections here

  Cleaner cleaner(&bcc);

  bcc.electrons = cleaner.ElectronCleaner();
  bcc.muons = cleaner.MuonCleaner();

  for(unsigned int i=0; i<bcc.jets->size(); ++i){
    //std::cout << jets->at(i).v4().pt() << "   " << jets->at(i).pt << std::endl;
    if(bcc.jets->at(i).v4().pt()<30){
      bcc.jets->erase(bcc.jets->begin()+i);
      i--;
    }
  }
  for(unsigned int i=0; i<bcc.taus->size(); ++i){
    if(bcc.taus->at(i).v4().pt()<0 || !bcc.taus->at(i).decayModeFinding /*|| !bcc.taus->at(i).byMediumCombinedIsolationDeltaBetaCorr ||  !bcc.taus->at(i).againstElectronTight ||  !bcc.taus->at(i).againstMuonTight*/){
      bcc.taus->erase(bcc.taus->begin()+i);
      i--;
    }
  }

  for(unsigned int i=0;i<bcc.taus->size(); ++i){
    //std::cout << i << ":  " <<  bcc.taus->at(i).pt << "  " << bcc.taus->at(i).eta << "  " << bcc.taus->at(i).phi << std::endl;
    double mindrjet=9999;
    double mindrtau=9999;

    for(unsigned int j=0; j<bcc.jets->size(); ++j){
      if(bcc.taus->at(i).deltaR(bcc.jets->at(j))<mindrjet)
	mindrjet= bcc.taus->at(i).deltaR(bcc.jets->at(j));
    }
    for(unsigned int j=0;j<bcc.taus->size(); ++j){
      if(i==j) continue;
      if(bcc.taus->at(i).deltaR(bcc.taus->at(j))<mindrtau)
 	mindrtau= bcc.taus->at(i).deltaR(bcc.taus->at(j));
    }

    Hist( "DR_tau_nextjet" )->Fill( mindrjet, weight );
    Hist( "DR_tau_nexttau" )->Fill( mindrtau, weight );
  }



//   for(unsigned int i=0; i<bcc.genparticles->size(); ++i){
//     GenParticle genp = bcc.genparticles->at(i);
//     std::cout << genp.index <<"  pdgId = " << genp.pdgId << "  mo1 = " << genp.mother1 << "  mo2 = " << genp.mother2 <<"  da1 = " << genp.daughter1 << "  da2 = " << genp.daughter2 <<std::endl;
//     if(genp.mother(bcc.genparticles,1)){
//       std::cout << "  Mother1: " << genp.mother(bcc.genparticles,1)->pdgId << "  " << genp.mother(bcc.genparticles,1)->pt <<std::endl;
//     }
//     if(genp.mother(bcc.genparticles,2)){
//       std::cout << "  Mother2: " << genp.mother(bcc.genparticles,2)->pdgId << "  " << genp.mother(bcc.genparticles,2)->pt <<std::endl;
//     }
//     if(genp.daughter(bcc.genparticles,1)){
//       std::cout << "  Daughter1: " << genp.daughter(bcc.genparticles,1)->pdgId << "  " << genp.daughter(bcc.genparticles,1)->pt <<std::endl;
//     }
//     if(genp.daughter(bcc.genparticles,2)){
//       std::cout << "  Daughter2: " << genp.daughter(bcc.genparticles,2)->pdgId << "  " << genp.daughter(bcc.genparticles,2)->pt <<std::endl;
//     }
//   }

  //selection
  
  /*
  Selection selection(&bcc);

  //HBHE noise filter only for data
  if(bcc.isRealData)
    if(!selection.HBHENoiseFilter()) throw SError( SError::SkipEvent );

  //trigger

  //DO NOT use trigger selection in PROOF mode for the moment
  //if(!selection.TriggerSelection("HLT_Jet300_v5"))  throw SError( SError::SkipEvent );

  //at least two CA 0.8 fat jets
  if(!selection.NTopJetSelection(2,350,2.5)) throw SError( SError::SkipEvent );
 
  for(unsigned int i=0; i< bcc.topjets->size(); ++i){
    TopJet topjet =  bcc.topjets->at(i);
    double mmin=0;
    double mjet=0;
    int nsubjets=0;
    selection.TopTag(topjet,mjet,nsubjets,mmin);
    Hist( "Mjet_hist" )->Fill( mjet, weight );
    if(nsubjets>=3) Hist( "Mmin_hist" )->Fill( mmin, weight );
    Hist( "Nsubjet_hist" )->Fill( nsubjets, weight ); 
  }

  //at least 2 top tags
  int min_toptag=2;
  if(!selection.NTopTagSelection(min_toptag)) throw SError( SError::SkipEvent );
  */

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
  if(bcc.topjets->size()>=2){
    Hist( "DR_jj_hist" )->Fill( bcc.topjets->at(0).deltaR(bcc.topjets->at(1)), weight ); 
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
  o_triggerNames.clear();
  o_triggerResults.clear();
  o_L1_prescale.clear();
  o_HLT_prescale.clear();

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
  o_L1_prescale = *bcc.L1_prescale;
  o_HLT_prescale = *bcc.HLT_prescale;

  return;

}

