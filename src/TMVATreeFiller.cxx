#include "include/TMVATreeFiller.h"
#include "include/EventCalc.h"
#include "include/JetProps.h"
#include <iostream>

using namespace std;

TMVATreeFiller::TMVATreeFiller(const char* name, TTree * t) : BaseHists(name), m_tree(t)
{
  // named default constructor
}

TMVATreeFiller::~TMVATreeFiller()
{
  // default destructor, does nothing
}

void TMVATreeFiller::Init()
{  
  m_tree->Branch("TopJet_px", &m_px, "TopJet_px/D");
  m_tree->Branch("TopJet_py", &m_py, "TopJet_py/D");
  m_tree->Branch("TopJet_pz", &m_pz, "TopJet_pz/D");
  m_tree->Branch("TopJet_mass", &m_mass, "TopJet_mass/D");

  m_tree->Branch("TopQuark_px", &m_genpx, "TopQuark_px/D");
  m_tree->Branch("TopQuark_py", &m_genpy, "TopQuark_py/D");
  m_tree->Branch("TopQuark_pz", &m_genpz, "TopQuark_pz/D");

  m_tree->Branch("Event_NPV", &m_npv, "N_PV/I");
  m_tree->Branch("Event_Weight", &m_weight, "Event_Weight/D");

  m_tree->Branch("TopJet_tau1", &m_tau1, "TopJet_tau1/D");
  m_tree->Branch("TopJet_tau2", &m_tau2, "TopJet_tau2/D");  
  m_tree->Branch("TopJet_tau3", &m_tau3, "TopJet_tau3/D");
  m_tree->Branch("TopJet_tau4", &m_tau4, "TopJet_tau4/D");

  m_tree->Branch("TopJet_pruned_tau1", &m_pruned_tau1, "TopJet_pruned_tau1/D");
  m_tree->Branch("TopJet_pruned_tau2", &m_pruned_tau2, "TopJet_pruned_tau2/D");  
  m_tree->Branch("TopJet_pruned_tau3", &m_pruned_tau3, "TopJet_pruned_tau3/D");
  m_tree->Branch("TopJet_pruned_tau4", &m_pruned_tau4, "TopJet_pruned_tau4/D");

  m_tree->Branch("TopJet_Qjets_volatility", &m_qjets, "TopJet_Qjets_volatility/D");

  m_tree->Branch("TopJet_Nsubjets", &m_nsubs, "TopJet_Nsubjets/I");

  m_tree->Branch("Subjet1_px", &m_sub1_px, "Subjet1_px/D");
  m_tree->Branch("Subjet1_py", &m_sub1_py, "Subjet1_py/D");
  m_tree->Branch("Subjet1_pz", &m_sub1_pz, "Subjet1_pz/D");
  m_tree->Branch("Subjet1_mass", &m_sub1_mass, "Subjet1_mass/D");

  m_tree->Branch("Subjet2_px", &m_sub2_px, "Subjet2_px/D");
  m_tree->Branch("Subjet2_py", &m_sub2_py, "Subjet2_py/D");
  m_tree->Branch("Subjet2_pz", &m_sub2_pz, "Subjet2_pz/D");
  m_tree->Branch("Subjet2_mass", &m_sub2_mass, "Subjet2_mass/D");

  m_tree->Branch("Subjet3_px", &m_sub3_px, "Subjet3_px/D");
  m_tree->Branch("Subjet3_py", &m_sub3_py, "Subjet3_py/D");
  m_tree->Branch("Subjet3_pz", &m_sub3_pz, "Subjet3_pz/D");
  m_tree->Branch("Subjet3_mass", &m_sub3_mass, "Subjet3_mass/D");

  m_tree->Branch("Subjet4_px", &m_sub4_px, "Subjet4_px/D");
  m_tree->Branch("Subjet4_py", &m_sub4_py, "Subjet4_py/D");
  m_tree->Branch("Subjet4_pz", &m_sub4_pz, "Subjet4_pz/D");
  m_tree->Branch("Subjet4_mass", &m_sub4_mass, "Subjet4_mass/D");

  m_tree->Branch("Subjets12_mass", &m_m12, "Subjet_mass12/D");
  m_tree->Branch("Subjets13_mass", &m_m13, "Subjet_mass13/D");
  m_tree->Branch("Subjets23_mass", &m_m23, "Subjet_mass23/D");

  m_tree->Branch("TopJet_pruned_mass", &m_pruned_mass, "TopJet_pruned_mass/D");
  m_tree->Branch("Subjets12_pruned_mass", &m_pruned_m12, "Subjet_pruned_mass12/D");
  m_tree->Branch("Subjets13_pruned_mass", &m_pruned_m13, "Subjet_pruned_mass13/D");
  m_tree->Branch("Subjets23_pruned_mass", &m_pruned_m23, "Subjet_pruned_mass23/D");
  
  m_tree->Branch("TopJet_HEPTopTag", &m_HEPTopTag, "HEPTopTag/O");
  m_tree->Branch("TopJet_CMSTopTag", &m_CMSTopTag, "CMSTopTag/O");

  Book( TH1F( "DR_top","#DeltaR_{jet,top}",100,0,3.5));  
  Book( TH1F( "DR_atop","#DeltaR_{jet,anti-top}",100,0,3.5));

  Book( TH1F( "TopGen_pt", "gen top P_{T} [GeV]", 100, 0, 1000));
  Book( TH1F( "MatchedJet_pt", "matched jet: gen top P_{T} [GeV]", 100, 0, 1000));

  Book( TH1F( "TopGen_eta", "gen top #eta (P_{T} > 200 GeV)", 50, -3, 3));
  Book( TH1F( "MatchedJet_eta", "matched jet: gen top #eta  (P_{T} > 200 GeV)", 50, -3, 3));

  Book( TH1F( "TopGen_NPV", "gen top N_{PV} (P_{T} > 200 GeV)", 51, 0, 50));
  Book( TH1F( "MatchedJet_NPV", "matched jet N_{PV} (P_{T} > 200 GeV)", 51, 0, 50));


}

void TMVATreeFiller::Fill()
{
  // fill the histograms
  
  EventCalc* calc = EventCalc::Instance();
  bool IsRealData = calc->IsRealData();
  LuminosityHandler* lumih = calc->GetLumiHandler();

  // important: get the event weight
  double weight = calc->GetWeight();

  int run = calc->GetRunNum();
  int lumiblock = calc->GetLumiBlock();
  int Npvs = calc->GetPrimaryVertices()->size();
  
  std::vector< TopJet >* tjets = calc->GetCAJets();

  // check if we got a ttbar event on generator level

  TTbarGen geninfo(calc->GetBaseCycleContainer());
  GenParticle top = geninfo.Top();
  GenParticle atop = geninfo.Antitop();

  // match the CA-jet with a top-jet only for a sample where ttbar quarks have been generated
  bool backgroundMC = false;
  if (top.pt()<0.001 && atop.pt()<0.001) backgroundMC = true;  

  if (!backgroundMC){
    double npv = calc->GetPrimaryVertices()->size();
    Hist("TopGen_pt")-> Fill(top.v4().pt(),weight);
    if (top.v4().pt()>250){
      Hist("TopGen_eta")-> Fill(top.v4().eta(),weight);
      Hist("TopGen_NPV")-> Fill(npv,weight);
    }    

    Hist("TopGen_pt")-> Fill(atop.v4().pt(),weight);
    if (atop.v4().pt()>250){
      Hist("TopGen_eta")-> Fill(atop.v4().eta(),weight);
      Hist("TopGen_NPV")-> Fill(npv,weight);
    }
  }

  for (UInt_t i=0; i<tjets->size(); ++i){

    ClearVariables();

    TopJet tj = tjets->at(i);

    bool is_signal = false;

    Double_t DR1 = RecGenMatch(tj, top);
    Double_t DR2 = RecGenMatch(tj, atop);

    GenParticle topquark;

    m_weight = weight;
    Hist("DR_top")-> Fill(DR1,weight);
    Hist("DR_atop")-> Fill(DR2,weight);

    if (DR1<0.7){
      if (geninfo.IsTopHadronicDecay()){
	is_signal = true;
	topquark = top;
      }
    }
    if (DR2<0.7){
      if (geninfo.IsAntiTopHadronicDecay()){
	is_signal = true;
	topquark = atop;
      }
    }

    if ((!backgroundMC) && (!is_signal)){
      continue;
    }

    // check if this jet fulfills the cuts
    if (tj.pt()<200) continue;
    if (fabs(tj.eta())>2.5) continue;
    
    FillTopJetProperties(tj, topquark);

    m_tree->Fill();
    
  }

}

void TMVATreeFiller::Finish()
{
  // final calculations, like division and addition of certain histograms

}

Double_t TMVATreeFiller::RecGenMatch(TopJet tj, GenParticle gen)
{
  
  // protection against non-existant gen particle
  if (gen.pt()<0.0001) return 9999.;

  double eta1 = tj.eta();
  double eta2 = gen.eta();
  double DeltaEta = eta1 - eta2;

  double phi1 = tj.phi();
  double phi2 = gen.phi();

  // fold difference in phi into [0,pi]
  static const double pi = 3.14159265358979323846;
  double DeltaPhi = phi1 - phi2;
  if (abs(DeltaPhi) > pi){
    if (DeltaPhi>0) DeltaPhi-=2*pi;
    else DeltaPhi+=2*pi;
  }
  DeltaPhi = abs(DeltaPhi); // projection into [0,pi]

  double DR = sqrt(DeltaEta*DeltaEta + DeltaPhi*DeltaPhi);

  return DR;

}

void TMVATreeFiller::ClearVariables()
{
  m_npv = 0;
  m_weight = 0;

  m_px = 0;
  m_py = 0;
  m_pz = 0;
  m_mass = 0;

  m_genpx = 0;
  m_genpy = 0;
  m_genpz = 0;

  m_tau1 = 0;
  m_tau2 = 0;
  m_tau3 = 0;
  m_tau4 = 0;

  m_pruned_tau1 = 0;
  m_pruned_tau2 = 0;
  m_pruned_tau3 = 0;
  m_pruned_tau4 = 0;

  m_qjets = 0;

  m_sub1_px = 0;
  m_sub1_py = 0;
  m_sub1_pz = 0;
  m_sub1_mass = 0;

  m_sub2_px = 0;
  m_sub2_py = 0;
  m_sub2_pz = 0;
  m_sub2_mass = 0;

  m_sub3_px = 0;
  m_sub3_py = 0;
  m_sub3_pz = 0;
  m_sub3_mass = 0;

  m_sub4_px = 0;
  m_sub4_py = 0;
  m_sub4_pz = 0;
  m_sub4_mass = 0;

  m_m12 = 0;	
  m_m13 = 0;	
  m_m23 = 0;	

  m_pruned_mass = 0;
  m_pruned_m12 = 0;	
  m_pruned_m13 = 0;	
  m_pruned_m23 = 0;

  m_HEPTopTag = false;
  m_CMSTopTag = false;

}


void TMVATreeFiller::FillTopJetProperties(TopJet topjet, GenParticle topquark)
{

  EventCalc* calc = EventCalc::Instance();
  m_npv = calc->GetPrimaryVertices()->size();
  double weight = calc->GetWeight();

  m_px = topjet.v4().px();
  m_py = topjet.v4().py();
  m_pz = topjet.v4().pz();

  m_genpx = topquark.v4().px();
  m_genpy = topquark.v4().py();
  m_genpz = topquark.v4().pz();

  Hist("MatchedJet_pt")-> Fill(topquark.v4().pt(),weight);
  if (topquark.v4().pt()>250){
    Hist("MatchedJet_eta")-> Fill(topquark.v4().eta(),weight);
    Hist("MatchedJet_NPV")-> Fill(m_npv,weight);
  }

  m_nsubs=topjet.numberOfDaughters();

  // some jet substructure variables
  // N-subjettiness
  JetProps jp(&topjet);
  m_tau1 = jp.GetNsubjettiness(1, Njettiness::onepass_kt_axes, 1., 0.8);
  m_tau2 = jp.GetNsubjettiness(2, Njettiness::onepass_kt_axes, 1., 0.8);
  m_tau3 = jp.GetNsubjettiness(3, Njettiness::onepass_kt_axes, 1., 0.8);
  m_tau4 = jp.GetNsubjettiness(4, Njettiness::onepass_kt_axes, 1., 0.8);

  m_pruned_tau1 = jp.GetPrunedNsubjettiness(1, Njettiness::onepass_kt_axes, 1., 0.8);
  m_pruned_tau2 = jp.GetPrunedNsubjettiness(2, Njettiness::onepass_kt_axes, 1., 0.8);
  m_pruned_tau3 = jp.GetPrunedNsubjettiness(3, Njettiness::onepass_kt_axes, 1., 0.8);
  m_pruned_tau4 = jp.GetPrunedNsubjettiness(4, Njettiness::onepass_kt_axes, 1., 0.8);

  // Q-jets
  m_qjets = jp.GetQjetVolatility(calc->GetEventNum(), 0.8);

  // HEP TopTag
  m_HEPTopTag = HepTopTag(topjet);

  // CMS TopTag
  double dum1, dum2;
  int idum;
  m_CMSTopTag = TopTag(topjet, dum1, idum, dum2);
  

  LorentzVector allsubjets(0,0,0,0);
  
  for(int j=0; j<topjet.numberOfDaughters(); ++j){
    allsubjets += topjet.subjets()[j].v4();
  }

  if(!allsubjets.isTimelike()){
    m_mass=0;
  } else {
    m_mass = allsubjets.M();
  }
  
  std::vector<Particle> subjets = topjet.subjets();
  sort(subjets.begin(), subjets.end(), HigherPt());

  if (m_nsubs>=1) {
    m_sub1_px = subjets[0].v4().px();
    m_sub1_py = subjets[0].v4().py();
    m_sub1_pz = subjets[0].v4().pz();
    if (subjets[0].v4().isTimelike()){
      m_sub1_mass = subjets[0].v4().mass();
    }
  }  

  if (m_nsubs>=2) {
    m_sub2_px = subjets[1].v4().px();
    m_sub2_py = subjets[1].v4().py();
    m_sub2_pz = subjets[1].v4().pz();
    if (subjets[1].v4().isTimelike()){
      m_sub2_mass = subjets[1].v4().mass();
    }
    if ( (subjets[0].v4()+subjets[1].v4()).isTimelike() ){
      m_m12=(subjets[0].v4()+subjets[1].v4()).M();
    }
  }

  if (m_nsubs>=3) {
    m_sub3_px = subjets[2].v4().px();
    m_sub3_py = subjets[2].v4().py();
    m_sub3_pz = subjets[2].v4().pz();
    if (subjets[2].v4().isTimelike()){
      m_sub3_mass = subjets[2].v4().mass();
    }
    if( (subjets[0].v4()+subjets[2].v4()).isTimelike() ){
      m_m13=(subjets[0].v4()+subjets[2].v4()).M();
    }
    
    if( (subjets[1].v4()+subjets[2].v4()).isTimelike() ){
      m_m23 = (subjets[1].v4()+subjets[2].v4()).M();   
    } 
  }

  if (m_nsubs>=4) {
    m_sub4_px = subjets[3].v4().px();
    m_sub4_py = subjets[3].v4().py();
    m_sub4_pz = subjets[3].v4().pz();
    if (subjets[3].v4().isTimelike()){
      m_sub4_mass = subjets[3].v4().mass();
    }
  }


  // calculate pruned masses
  std::vector<fastjet::PseudoJet> jets = jp.GetFastJet(2.0);   // something large to make sure jet is inside radius
  if(jets.empty()){
      m_logger << WARNING << "TMVATreeFiller::FillTopJetProperties: no jet found!" << SLogger::endmsg; 
  }
  else{
    fastjet::PseudoJet pjet = jp.GetPrunedJet(jets[0]);
    std::vector<fastjet::PseudoJet> prunedsubjets;
    if (pjet.constituents().size()>=2){
        prunedsubjets = pjet.exclusive_subjets(2);
    }
    if (pjet.constituents().size()>=3){
        prunedsubjets = pjet.exclusive_subjets(3);
    }

    unsigned int pnsubs = prunedsubjets.size();

    fastjet::PseudoJet psubjets(0,0,0,0);
    
    for(unsigned int j=0; j<pnsubs; ++j){
        psubjets += pjet.pieces()[j];
    }

    m_pruned_mass = psubjets.m();

    if (pnsubs>=2) {
        m_pruned_m12 = (prunedsubjets[0]+prunedsubjets[1]).m();
    }

    if (pnsubs>=3) {
        m_pruned_m13 = (prunedsubjets[0]+prunedsubjets[2]).m();
        m_pruned_m23 = (prunedsubjets[1]+prunedsubjets[2]).m();
    }
  }
}
