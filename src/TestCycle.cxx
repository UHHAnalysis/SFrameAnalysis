// $Id: TestCycle.cxx,v 1.2 2012/05/23 13:21:35 peiffer Exp $

// Local include(s):
#include "../include/TestCycle.h"

ClassImp( TestCycle );


TestCycle::TestCycle()
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

TestCycle::~TestCycle() {

}

void TestCycle::BeginCycle() throw( SError ) {
  
  // Set-Up LumiHandler
  LuminosityHandler *lumiHandler = new LuminosityHandler();
  // Declared Properties readable not before BeginCycle
  
  lumiHandler->SetGRLPath( "/afs/naf.desy.de/user/p/peiffer/CMSSW_5_2_5/src/UHHAnalysis/NtupleWriter/" );
  lumiHandler->SetLumiFileName( "GoodRun.root" );
  lumiHandler->SetTrigger( "HLT_PFJet320_v" );
  lumiHandler->SetIntLumiPerBin( 25 );
  
  // Initialise, checks also if LumiFile is specified and readable
  lumiHandler->Initialise();
  // dump lumi information in text file
  // lumiHandler->DumpLumiInfoIntoTxtFile();
  
  // adding luminosity handler to gloabl config
  AddConfigObject( lumiHandler );

  //Set-Up Selection
  selection = new Selection();
  //DO NOT use trigger selection in PROOF mode for the moment
  selection->addSelectionModule(new TriggerSelection("HLT_PFJet320_v"));
  selection->addSelectionModule(new NTopJetSelection(2,350,2.5));
  selection->addSelectionModule(new NTopTagSelection(1));

  return;

}

void TestCycle::EndCycle() throw( SError ) {

   return;

}

void TestCycle::BeginInputData( const SInputData& ) throw( SError ) {


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

  
//   DeclareVariable( bcc.run, "run" );
//   DeclareVariable( bcc.luminosityBlock, "luminosityBlock" );
//   DeclareVariable( bcc.event, "event" );
//   DeclareVariable( bcc.isRealData, "isRealData" );
//   DeclareVariable( bcc.HBHENoiseFilterResult, "HBHENoiseFilterResult" );
//   DeclareVariable( bcc.beamspot_x0, "beamspot_x0");
//   DeclareVariable( bcc.beamspot_y0, "beamspot_y0");
//   DeclareVariable( bcc.beamspot_z0, "beamspot_z0");

//   if(ElectronCollection.size()>0) DeclareVariable( o_electrons, ElectronCollection.c_str() );
//   if(MuonCollection.size()>0) DeclareVariable( o_muons, MuonCollection.c_str() ); 
//   if(TauCollection.size()>0) DeclareVariable( o_taus, TauCollection.c_str() );
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
  

  if(pu_filename_mc.size()>0 && pu_filename_data.size()>0 && pu_histname_mc.size()>0 && pu_histname_data.size()>0){
    puwp = new PUWeightProducer(pu_filename_mc, pu_filename_data, pu_histname_mc, pu_histname_data);
  }

  //
  // Declare the output histograms:
  //

  Book( TH1F( "Mjet_hist", "m_{jet}", 100,0,500 ) );
  Book( TH1F( "Mmin_hist", "m_{min}", 100,0,200 ) );
  Book( TH1F( "Nsubjet_hist", "N^{subjet}", 10,0,10 ) );
  Book( TH1F( "N_pileup_hist", "N^{PU}", 1000,0,50 ) );
  Book( TH1F( "DR_jj_hist", "#Delta R(jj)", 100,0,6 ) );
  Book( TH1F( "DR_tau_nextjet", "#Delta R(#tau j)",100,0,6));
  Book( TH1F( "DR_tau_nexttau", "#Delta R(#tau #tau)",100,0,6)); 

  Book( TH1F( "N_events_perLumiBin_hist", "N^{evt}", 20,1,21 ) );
  Book( TH1F( "N_pv_perLumiBin_hist", "N^{PV}", 20,1,21 ) );

  Book( TH1F( "N_pv", "N^{PV}", 50,0,50 ) );

  Book( TH1F( "N_lep_hist", "N^{lep}", 10,0,10 ) );
  Book( TH1F( "pt_lep_hist", "p_{T}^{lep}", 100,0,500 ) );

  Book( TH1F( "N_jet_hist", "N^{jet}", 10,0,10 ) );
  Book( TH1F( "N_bjet_hist", "N^{b-jet}", 10,0,10 ) );
  Book( TH1F( "m_j1j2_hist", "m_{j1,j2}", 100,0,500 ) ); 
  Book( TH1F( "m_j1j3_hist", "m_{j1,j3}", 100,0,400 ) );
  Book( TH1F( "m_j2j3_hist", "m_{j2,j3}", 100,0,300 ) );
  Book( TH1F( "dR_j1j2_hist", "#Delta R_{j1,j2}", 100,0,6 ) ); 
  Book( TH1F( "dR_j1j3_hist", "#Delta R_{j1,j3}", 100,0,6 ) );
  Book( TH1F( "dR_j2j3_hist", "#Delta R_{j2,j3}", 100,0,6 ) );
  Book( TH1F( "pt_lep1_hist", "p_{T}^{1st lep}", 100,0,500 ) );
  Book( TH1F( "pt_jet1_hist", "p_{T}^{1st jet}", 100,0,500 ) );
  Book( TH1F( "pt_jet2_hist", "p_{T}^{2nd jet}", 100,0,400 ) );
  Book( TH1F( "pt_jet3_hist", "p_{T}^{3rd jet}", 100,0,300 ) );
  Book( TH1F( "pt_jet4_hist", "p_{T}^{4th jet}", 100,0,200 ) );
  Book( TH1F( "eta_lep1_hist", "#eta^{1st lep}", 100,-2.5,2.5 ) );
  Book( TH1F( "eta_jet1_hist", "#eta^{1st jet}", 100,-2.5,2.5 ) );
  Book( TH1F( "eta_jet2_hist", "#eta^{2nd jet}", 100,-2.5,2.5 ) );
  Book( TH1F( "eta_jet3_hist", "#eta^{3rd jet}", 100,-2.5,2.5 ) );
  Book( TH1F( "eta_jet4_hist", "#eta^{4th jet}", 100,-2.5,2.5 ) );
  Book( TH1F( "pt_bjet_hist", "p_{T}^{1st b-jet}", 100,0,400 ) );
  Book( TH1F( "eta_bjet_hist", "#eta^{1st b-jet}", 100,-2.5,2.5 ) );

  Book( TH1F( "eta_bjet_20_35_hist", "#eta^{b-jet} 20<p_{T}<35", 100,-2.5,2.5 ) );
  Book( TH1F( "eta_bjet_35_50_hist", "#eta^{b-jet} 35<p_{T}<50", 100,-2.5,2.5 ) );
  Book( TH1F( "eta_bjet_50_75_hist", "#eta^{b-jet} 50<p_{T}<75", 100,-2.5,2.5 ) );
  Book( TH1F( "eta_bjet_75_100_hist", "#eta^{b-jet} 75<p_{T}<100", 100,-2.5,2.5 ) );
  Book( TH1F( "eta_bjet_100_inf_hist", "#eta^{b-jet} 100<p_{T}", 100,-2.5,2.5 ) );

  Book( TH1F( "eta_bquark_0_20_hist", "#eta^{b-quark} 0<p_{T}<20", 100,-2.5,2.5 ) );
  Book( TH1F( "eta_bquark_20_35_hist", "#eta^{b-quark} 20<p_{T}<35", 100,-2.5,2.5 ) );
  Book( TH1F( "eta_bquark_35_50_hist", "#eta^{b-quark} 35<p_{T}<50", 100,-2.5,2.5 ) );
  Book( TH1F( "eta_bquark_50_75_hist", "#eta^{b-quark} 50<p_{T}<75", 100,-2.5,2.5 ) );
  Book( TH1F( "eta_bquark_75_100_hist", "#eta^{b-quark} 75<p_{T}<100", 100,-2.5,2.5 ) );
  Book( TH1F( "eta_bquark_100_inf_hist", "#eta^{b-quark} 100<p_{T}", 100,-2.5,2.5 ) );
  Book( TH1F( "eta_bquark_hist", "#eta^{b-quark}", 100,-2.5,2.5 ) );

  Book( TH1F( "dR_bj1j_hist", "#Delta R_{1st b-jet,jet}", 100,0,6 ) ); 

  Book( TH1F( "b_quark_mass", "m_{b}", 200,4.4,4.9 ) ); 
  
  Book( TH1F( "m_ttbar" , "m^{tt}",100,0,1200) );
  Book( TH1F( "y_ttbar" , "y^{tt}",100,-4,4) );
  Book( TH1F( "pt_ttbar" , "p_{T}^{tt}",100,0,300) );
  Book( TH1F( "y_t" , "y^{t}",100,-4,4) );
  Book( TH1F( "pt_t" , "p_{t}^{t}",100,0,800) );

  Book( TH1F( "Eta_B_jet_CSVL", "#eta^{b-jet} CSVL",100,-2.5,2.5)); 
  Book( TH1F( "Eta_B_jet_CSVM", "#eta^{b-jet} CSVM",100,-2.5,2.5)); 
  Book( TH1F( "Eta_B_jet_CSVT", "#eta^{b-jet} CSVT",100,-2.5,2.5)); 
  Book( TH1F( "Eta_B_jet_JPL", "#eta^{b-jet} JPL",100,-2.5,2.5)); 
  Book( TH1F( "Eta_B_jet_JPM", "#eta^{b-jet} JPM",100,-2.5,2.5)); 
  Book( TH1F( "Eta_B_jet_JPT", "#eta^{b-jet} JPT",100,-2.5,2.5)); 
  Book( TH1F( "Pt_B_jet_CSVL", "p_{T}^{b-jet} CSVL",100,0,400)); 
  Book( TH1F( "Pt_B_jet_CSVM", "p_{T}^{b-jet} CSVM",100,0,400)); 
  Book( TH1F( "Pt_B_jet_CSVT", "p_{T}^{b-jet} CSVT",100,0,400)); 
  Book( TH1F( "Pt_B_jet_JPL", "p_{T}^{b-jet} JPL",100,0,400)); 
  Book( TH1F( "Pt_B_jet_JPM", "p_{T}^{b-jet} JPM",100,0,400)); 
  Book( TH1F( "Pt_B_jet_JPT", "p_{T}^{b-jet} JPT",100,0,400)); 
  return;

}

void TestCycle::EndInputData( const SInputData& ) throw( SError ) {

  
  Hist( "N_pv_perLumiBin_hist")->Divide(   Hist( "N_pv_perLumiBin_hist") ,Hist( "N_events_perLumiBin_hist"));


  /*LumiHandler()->PrintUsedSetup();
  
  // LumiBins vs. integrated luminosity for that bin
  if( LumiHandler()->IsLumiCalc() ){
    m_logger << INFO << "Target Lumi in data files and used for weighting: " 
	     << LumiHandler()->GetTargetLumi() << " (pb^-1)!" << SLogger::endmsg;
    
    // store lumi hist to file
    WriteObj( *(LumiHandler()->GetLumiPerLumiBinHist()) ); 
    
    // store target luminosity (luminosity in data)
    TTree *tree = new TTree( "LuminosityTree", "luminosity tree" );
    Double_t lumi = LumiHandler()->GetTargetLumi();
    tree->Branch( "targetLuminosity", &lumi );
    tree->Fill();
    WriteObj( *tree );
    
    // store the luminosity collected in each run
    WriteObj( *(LumiHandler()->GetTreeLuminosityPerRun()) ); 
  }*/
  
  return;
  
}

void TestCycle::BeginInputFile( const SInputData& ) throw( SError ) {
  //ConnectVariable( "JetTree", "pt_jet_1", pt_jet_1 );
  //ConnectVariable( "JetTree", "pt_jet_2", pt_jet_2 );

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
  ConnectVariable( "AnalysisTree", "run" , bcc.run);
  ConnectVariable( "AnalysisTree", "luminosityBlock" , bcc.luminosityBlock);
  ConnectVariable( "AnalysisTree" ,"event" ,bcc.event);
  ConnectVariable( "AnalysisTree" ,"isRealData", bcc.isRealData);
  //ConnectVariable( "AnalysisTree" ,"HBHENoiseFilterResult", bcc.HBHENoiseFilterResult);
  ConnectVariable( "AnalysisTree" ,"beamspot_x0", bcc.beamspot_x0);
  ConnectVariable( "AnalysisTree" ,"beamspot_y0", bcc.beamspot_y0);
  ConnectVariable( "AnalysisTree" ,"beamspot_z0", bcc.beamspot_z0);

  return;

}

void TestCycle::ExecuteEvent( const SInputData&, Double_t weight) throw( SError ) {

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
      //std::cout << bcc.genInfo->pileup_TrueNumInteractions() << "   " << puwp->produceWeight(bcc.genInfo) <<std::endl;
    }
    double npu = bcc.genInfo->pileup_TrueNumInteractions();
    if(npu>50) npu=49.9999;
    Hist( "N_pileup_hist" )->Fill( npu, weight );
  }



  //clean collections here

  Cleaner cleaner(&bcc);
  if(!bcc.isRealData && bcc.jets && bcc.met) cleaner.JetEnergyResolutionShifter();
  if(bcc.electrons) cleaner.ElectronCleaner(20,2.5);
  if(bcc.muons) cleaner.MuonCleaner(20,2.1);
  if(bcc.jets) cleaner.JetCleaner(20,2.5,true);
  if(bcc.topjets) cleaner.TopJetCleaner(350,2.5,false);
  if(bcc.taus) cleaner.TauCleaner(20,2.1);

  /*
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
  */

//   for(unsigned int i=0; i<bcc.genparticles->size(); ++i){
//     GenParticle genp = bcc.genparticles->at(i);
//     //std::cout << genp.index() <<"  pdgId = " << genp.pdgId() << "  mo1 = " << genp.mother1 << "  mo2 = " << genp.mother2 <<"  da1 = " << genp.daughter1 << "  da2 = " << genp.daughter2 <<std::endl;
//     if(genp.mother(bcc.genparticles,1)){
//       std::cout << "  Mother1: " << genp.mother(bcc.genparticles,1)->pdgId() << "  " << genp.mother(bcc.genparticles,1)->pt() <<std::endl;
//     }
//     if(genp.mother(bcc.genparticles,2)){
//       std::cout << "  Mother2: " << genp.mother(bcc.genparticles,2)->pdgId() << "  " << genp.mother(bcc.genparticles,2)->pt() <<std::endl;
//     }
//     if(genp.daughter(bcc.genparticles,1)){
//       std::cout << "  Daughter1: " << genp.daughter(bcc.genparticles,1)->pdgId() << "  " << genp.daughter(bcc.genparticles,1)->pt() <<std::endl;
//     }
//     if(genp.daughter(bcc.genparticles,2)){
//       std::cout << "  Daughter2: " << genp.daughter(bcc.genparticles,2)->pdgId() << "  " << genp.daughter(bcc.genparticles,2)->pt() <<std::endl;
//     }
//   }


  GenParticle top;
  GenParticle antitop;
  if(bcc.genparticles){
    for(unsigned int i=0; i<bcc.genparticles->size(); ++i){
      GenParticle genp = bcc.genparticles->at(i);
      if( genp.pdgId()==6 && genp.status()==3) top=genp;
      if( genp.pdgId()==-6 && genp.status()==3) antitop=genp;  
      if(abs(genp.pdgId())==5 && genp.status()==3 ){
	Hist ("b_quark_mass")->Fill( sqrt(genp.v4().mass2()));
	Hist ("eta_bquark_hist") ->Fill(genp.eta());
	if( genp.pt()<=20)
	  Hist( "eta_bquark_0_20_hist")->Fill( genp.eta());
	if( genp.pt()>20 && genp.pt()<=35)
	  Hist( "eta_bquark_20_35_hist")->Fill( genp.eta());
	if( genp.pt()>35 && genp.pt()<=50)
	  Hist( "eta_bquark_35_50_hist")->Fill( genp.eta());	
	if( genp.pt()>50 && genp.pt()<=75)
	  Hist( "eta_bquark_50_75_hist")->Fill( genp.eta());
	if( genp.pt()>75 && genp.pt()<=100)
	  Hist( "eta_bquark_75_100_hist")->Fill( genp.eta());
	if( genp.pt()>100)
	  Hist( "eta_bquark_100_inf_hist")->Fill( genp.eta());
      }
      //     if(abs(genp.pdgId)==5 && genp.status==3 &&  genp.v4().mass2()<4.78*4.78)
      //       throw SError( SError::SkipEvent );
    }
  }

  //selection

  //select only good runs
  if(bcc.isRealData && LumiHandler()->IsLumiCalc() ){
    if( !LumiHandler()->PassGoodRunsList( bcc.run, bcc.luminosityBlock )) throw SError( SError::SkipEvent );
  }

  if(!selection->passSelection(&bcc))  throw SError( SError::SkipEvent );

  //analysis code

  for(unsigned int i=0; i< bcc.topjets->size(); ++i){
    TopJet topjet =  bcc.topjets->at(i);
    double mmin=0;
    double mjet=0;
    int nsubjets=0;
    TopTag(topjet,mjet,nsubjets,mmin);
    Hist( "Mjet_hist" )->Fill( mjet, weight );
    if(nsubjets>=3) Hist( "Mmin_hist" )->Fill( mmin, weight );
    Hist( "Nsubjet_hist" )->Fill( nsubjets, weight ); 
  }

  Hist( "N_pv")->Fill(bcc.pvs->size(),weight);

  if(bcc.isRealData){  
    Hist( "N_pv_perLumiBin_hist")->Fill( LumiHandler()->GetLumiBin(bcc.run, bcc.luminosityBlock), bcc.pvs->size()*weight);
    Hist( "N_events_perLumiBin_hist")->Fill( LumiHandler()->GetLumiBin(bcc.run, bcc.luminosityBlock),weight);
  }

 
  Hist ("m_ttbar")->Fill( sqrt((top.v4()+antitop.v4()).mass2()),weight);
  Hist ("y_ttbar")->Fill( (top.v4()+antitop.v4()).Rapidity(),weight);  
  Hist ("pt_ttbar")->Fill( (top.v4()+antitop.v4()).pt(),weight); 
 
  Hist ("pt_t")->Fill( top.pt(),weight); 
  Hist ("pt_t")->Fill( antitop.pt(),weight); 
  Hist ("y_t")->Fill( top.v4().Rapidity(),weight); 
  Hist ("y_t")->Fill( antitop.v4().Rapidity(),weight);  
  

  int nbjets=0;
  Jet PrimBJet;
  for(unsigned int i=0; i<bcc.jets->size(); ++i){
    if(bcc.jets->at(i).btag_combinedSecondaryVertex()>0.244)
      Hist ("Eta_B_jet_CSVL")->Fill ( bcc.jets->at(i).eta(), weight);
    if(bcc.jets->at(i).btag_combinedSecondaryVertex()>0.679)
      Hist ("Eta_B_jet_CSVM")->Fill ( bcc.jets->at(i).eta(), weight);
    if(bcc.jets->at(i).btag_combinedSecondaryVertex()>0.898)
      Hist ("Eta_B_jet_CSVT")->Fill ( bcc.jets->at(i).eta(), weight);
    if(bcc.jets->at(i).btag_jetProbability()>0.275)
      Hist ("Eta_B_jet_JPL")->Fill ( bcc.jets->at(i).eta(), weight);
    if(bcc.jets->at(i).btag_jetProbability()>0.545)
      Hist ("Eta_B_jet_JPM")->Fill ( bcc.jets->at(i).eta(), weight);
    if(bcc.jets->at(i).btag_jetProbability()>0.790)
      Hist ("Eta_B_jet_JPT")->Fill ( bcc.jets->at(i).eta(), weight);
    if(bcc.jets->at(i).btag_combinedSecondaryVertex()>0.244)
      Hist ("Pt_B_jet_CSVL")->Fill ( bcc.jets->at(i).pt(), weight);
    if(bcc.jets->at(i).btag_combinedSecondaryVertex()>0.679)
      Hist ("Pt_B_jet_CSVM")->Fill ( bcc.jets->at(i).pt(), weight);
    if(bcc.jets->at(i).btag_combinedSecondaryVertex()>0.898)
      Hist ("Pt_B_jet_CSVT")->Fill ( bcc.jets->at(i).pt(), weight);
    if(bcc.jets->at(i).btag_jetProbability()>0.275)
      Hist ("Pt_B_jet_JPL")->Fill ( bcc.jets->at(i).pt(), weight);
    if(bcc.jets->at(i).btag_jetProbability()>0.545)
      Hist ("Pt_B_jet_JPM")->Fill ( bcc.jets->at(i).pt(), weight);
    if(bcc.jets->at(i).btag_jetProbability()>0.790)
      Hist ("Pt_B_jet_JPT")->Fill ( bcc.jets->at(i).pt(), weight);
    if(bcc.jets->at(i).btag_combinedSecondaryVertex()>0.679){
      nbjets++;
      if(bcc.jets->at(i).pt()> PrimBJet.pt())
	PrimBJet = bcc.jets->at(i);

      if(bcc.jets->at(i).pt()>20 && bcc.jets->at(i).pt()<=35)
	Hist("eta_bjet_20_35_hist")->Fill( bcc.jets->at(i).eta(),weight);
      if(bcc.jets->at(i).pt()>35 && bcc.jets->at(i).pt()<=50)
	Hist("eta_bjet_35_50_hist")->Fill( bcc.jets->at(i).eta(),weight);
      if(bcc.jets->at(i).pt()>50 && bcc.jets->at(i).pt()<=75)
	Hist("eta_bjet_50_75_hist")->Fill( bcc.jets->at(i).eta(),weight);
      if(bcc.jets->at(i).pt()>75 && bcc.jets->at(i).pt()<=100)
	Hist("eta_bjet_75_100_hist")->Fill( bcc.jets->at(i).eta(),weight);
      if(bcc.jets->at(i).pt()>100)
	Hist("eta_bjet_100_inf_hist")->Fill( bcc.jets->at(i).eta(),weight);
    }
  }
  if(PrimBJet.pt()>0){
    Hist("pt_bjet_hist")->Fill(PrimBJet.pt(),weight);
    Hist("eta_bjet_hist")->Fill(PrimBJet.eta(),weight);
    for(unsigned int i=0; i<bcc.jets->size(); ++i){
      if(bcc.jets->at(i).pt()!=PrimBJet.pt()){
	Hist("dR_bj1j_hist")->Fill(PrimBJet.deltaR(bcc.jets->at(i)));
	break;
      }
    }
  }

  Hist( "N_jet_hist" )->Fill( bcc.jets->size(), weight );
  Hist( "N_bjet_hist" )->Fill( nbjets, weight );

  if(bcc.jets->size()>=2){
    Hist("m_j1j2_hist")->Fill ( sqrt((bcc.jets->at(0).v4()+bcc.jets->at(1).v4()).mass2()) );
    Hist("dR_j1j2_hist")->Fill ( bcc.jets->at(0).deltaR(bcc.jets->at(1)) );
  }
  if(bcc.jets->size()>=3){
    Hist("m_j1j3_hist")->Fill ( sqrt((bcc.jets->at(0).v4()+bcc.jets->at(2).v4()).mass2()) );
    Hist("dR_j1j3_hist")->Fill ( bcc.jets->at(0).deltaR(bcc.jets->at(2)) );
    Hist("m_j2j3_hist")->Fill ( sqrt((bcc.jets->at(1).v4()+bcc.jets->at(2).v4()).mass2()) );
    Hist("dR_j2j3_hist")->Fill ( bcc.jets->at(1).deltaR(bcc.jets->at(2)) );
  }
  return;
  std::vector<Particle> leptons;
  Particle PrimLep;
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
    Hist( "pt_lep_hist" )->Fill( leptons[i].pt(), weight );
    if(leptons[i].pt()>PrimLep.pt())
      PrimLep = leptons[i];
  }
  if(PrimLep.pt()>0){
    Hist("pt_lep1_hist")->Fill(PrimLep.pt(),weight);
    Hist("eta_lep1_hist")->Fill(PrimLep.eta(),weight);
  }
  if(bcc.jets->size()>=1){
    Hist("pt_jet1_hist")->Fill(bcc.jets->at(0).pt(),weight);
    Hist("eta_jet1_hist")->Fill(bcc.jets->at(0).eta(),weight);
  }
  if(bcc.jets->size()>=2){
    Hist("pt_jet2_hist")->Fill(bcc.jets->at(1).pt(),weight);
    Hist("eta_jet2_hist")->Fill(bcc.jets->at(1).eta(),weight);
  }
  if(bcc.jets->size()>=3){
    Hist("pt_jet3_hist")->Fill(bcc.jets->at(2).pt(),weight);
    Hist("eta_jet3_hist")->Fill(bcc.jets->at(2).eta(),weight);
  }
  if(bcc.jets->size()>=4){
    Hist("pt_jet4_hist")->Fill(bcc.jets->at(3).pt(),weight);
    Hist("eta_jet4_hist")->Fill(bcc.jets->at(3).eta(),weight);
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

