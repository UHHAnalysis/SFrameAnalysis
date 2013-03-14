#include "include/HypothesisHists.h"

using namespace std;

HypothesisHists::HypothesisHists(const char* name, HypothesisDiscriminator *discr) : BaseHists(name)
{
  // named default constructor
  m_discr = discr;
}

HypothesisHists::~HypothesisHists()
{
  // default destructor, does nothing
}

void HypothesisHists::Init()
{

  Book( TH1F( "M_ttbar_rec", "M_{t#bar{t}}^{rec} [GeV/c^{2}]", 100, 0, 5000 ) );
  Book( TH1F( "M_ttbar_rec_ly", "M_{t#bar{t}}^{rec} [GeV/c^{2}]", 100, 0, 5000 ) );

  Book( TH1F( "M_ttbar_gen", "M_{t#bar{t}}^{gen} [GeV/c^{2}]", 100, 0, 5000 ) );
  Book( TH1F( "M_ttbar_gen_ly", "M_{t#bar{t}}^{gen} [GeV/c^{2}]", 100, 0, 5000 ) );


  Book( TH1F( "M_toplep_rec", "M^{top,lep} [GeV/c^{2}]", 70, 0, 700 ) );
  Book( TH1F( "M_tophad_rec", "M^{top,had} [GeV/c^{2}]", 50, 0, 500 ) );

  Book( TH1F( "Pt_toplep_rec", "P_{T}^{top,lep} [GeV/c]", 60, 0, 1200 ) );
  Book( TH1F( "Pt_tophad_rec", "P_{T}^{top,had} [GeV/c]", 60, 0, 1200 ) );

  Book( TH1F( "Pt_ttbar_rec", "P_{T,t#bar{t}}^{rec} [GeV/c]", 60, 0, 600 ) );
  Book( TH1F( "Pt_ttbar_gen", "P_{T,t#bar{t}}^{gen} [GeV/c]", 60, 0, 600 ) );

  Book( TH2F( "Pt_ttbar_rec_vs_Pt_ttbar_gen", "P_{T,t#bar{t}}^{rec} [GeV/c] vs P_{T,t#bar{t}}^{gen} [GeV/c]", 60, 0, 600 ,60, 0, 600));

  TString name = m_discr->GetLabel();
  name += " discriminator";
  double min=0;
  double max=500;
  if( m_discr->GetLabel()=="BestPossible"){
    min=0;
    max=6;
  }
  if( m_discr->GetLabel()=="CorrectMatch"){
    min=0;
    max=2;
  }
  Book( TH1F("Discriminator", name , 50, min,max) );
  Book( TH1F("Discriminator_2", name , 50, 0,10) );
  Book( TH1F("Discriminator_3", name , 300, 0,30) );  

  Book( TH2F("Discriminator_vs_M_ttbar", name+" vs M_{t#bar{t}}^{rec}" , 50, min,max, 100,0,5000) );

  Book( TH2F("M_ttbar_rec_vs_M_ttbar_gen","M_{t#bar{t}}^{rec} [GeV/c^{2}] vs M_{t#bar{t}}^{gen} [GeV/c^{2}]",100,0,3000,100,0,3000));
  Book( TH1F("M_ttbar_resolution", "(M_{t#bar{t}}^{gen} - M_{t#bar{t}}^{rec})/M_{t#bar{t}}^{rec}", 100, -5,5) );

  Book( TH2F("M_ttbar_rec_vs_pt_lepton","M_{t#bar{t}}^{rec} [GeV/c^{2}] vs p_{T}^{lep} [GeV/c]",100,0,3000,100,0,500));
  Book( TH2F("M_ttbar_rec_vs_reliso_lepton","M_{t#bar{t}}^{rec} [GeV/c^{2}] vs reliso lep",100,0,3000,100,0,0.5));
  Book( TH2F("M_ttbar_rec_vs_deltarmin_lepton","M_{t#bar{t}}^{rec} [GeV/c^{2}] vs #Delta R_{min}",100,0,3000,100,0,3.0));
  Book( TH2F("M_ttbar_rec_vs_ptrel_lepton","M_{t#bar{t}}^{rec} [GeV/c^{2}] vs p_{T}^{rel} [GeV/c]",100,0,3000,100,0,200));

}

void HypothesisHists::Fill()
{
  // fill the histograms

  EventCalc* calc = EventCalc::Instance();

  // important: get the event weight
  double weight = calc->GetWeight();

  ReconstructionHypothesis* hyp = m_discr->GetBestHypothesis();
  
  double mttbar_rec = 0;
  if( (hyp->top_v4()+hyp->antitop_v4()).isTimelike() ) 
    mttbar_rec = (hyp->top_v4()+hyp->antitop_v4()).M();
  else{
    mttbar_rec = sqrt( -(hyp->top_v4()+hyp->antitop_v4()).mass2());
  }
  double ptttbar_rec = (hyp->top_v4()+hyp->antitop_v4()).Pt();
  double mttbar_gen = 0;
  double ptttbar_gen = 0;
  if(calc->GetGenParticles() ){
    mttbar_gen = ( calc->GetTTbarGen()->Top().v4() + calc->GetTTbarGen()->Antitop().v4()).M();
    ptttbar_gen = ( calc->GetTTbarGen()->Top().v4() + calc->GetTTbarGen()->Antitop().v4()).Pt();
  }

  Hist("M_ttbar_rec")->Fill(mttbar_rec, weight);
  Hist("M_ttbar_rec_ly")->Fill(mttbar_rec, weight);
  Hist("M_ttbar_gen")->Fill(mttbar_gen, weight);
  Hist("M_ttbar_gen_ly")->Fill(mttbar_gen, weight);
  Hist("M_ttbar_rec_vs_M_ttbar_gen")->Fill(mttbar_rec, mttbar_gen);
  Hist("M_ttbar_resolution")->Fill( (mttbar_gen-mttbar_rec)/mttbar_gen, weight);
  Hist("Discriminator")->Fill(hyp->discriminator(m_discr->GetLabel()), weight);
  Hist("Discriminator_2")->Fill(hyp->discriminator(m_discr->GetLabel()), weight);
  Hist("Discriminator_3")->Fill(hyp->discriminator(m_discr->GetLabel()), weight);

  TH2F* h = (TH2F*)Hist("Discriminator_vs_M_ttbar");
  h->Fill(hyp->discriminator(m_discr->GetLabel()), mttbar_rec, weight);

  double mtoplep=0;
  double mtophad=0;
  if(hyp->toplep_v4().isTimelike()) mtoplep = hyp->toplep_v4().M();
  if(hyp->tophad_v4().isTimelike()) mtophad = hyp->tophad_v4().M();
  Hist("M_toplep_rec")->Fill(mtoplep,weight);
  Hist("M_tophad_rec")->Fill(mtophad,weight); 

  Hist("Pt_toplep_rec")->Fill( hyp->toplep_v4().Pt(),weight );
  Hist("Pt_tophad_rec")->Fill( hyp->tophad_v4().Pt(),weight );

  Hist("Pt_ttbar_rec")->Fill ( ptttbar_rec, weight);
  Hist("Pt_ttbar_gen")->Fill ( ptttbar_gen, weight); 
  Hist("Pt_ttbar_rec_vs_Pt_ttbar_gen")->Fill(ptttbar_rec, ptttbar_gen);

  std::vector< Muon >* muons = calc->GetMuons();
  std::vector< Electron >* electrons = calc->GetElectrons();

  double lep_pt=0;
  double lep_reliso=0;
  double lep_ptrel=0;
  double lep_deltar=0;
  if(muons->size()==1 && electrons->size()==0){
    lep_pt = muons->at(0).pt();
    lep_reliso = muons->at(0).relIso();
    lep_ptrel= pTrel(&muons->at(0), calc->GetJets());
    lep_deltar = deltaRmin(&muons->at(0), calc->GetJets());
  }
  if(muons->size()==0 && electrons->size()==1){
    lep_pt = electrons->at(0).pt();
    lep_reliso = electrons->at(0).relIso();
    lep_ptrel= pTrel(&electrons->at(0), calc->GetJets());
    lep_deltar = deltaRmin(&electrons->at(0), calc->GetJets());
  }
  Hist("M_ttbar_rec_vs_pt_lepton")->Fill(mttbar_rec, lep_pt);
  Hist("M_ttbar_rec_vs_reliso_lepton")->Fill(mttbar_rec, lep_reliso);
  Hist("M_ttbar_rec_vs_ptrel_lepton")->Fill(mttbar_rec, lep_ptrel);
  Hist("M_ttbar_rec_vs_deltarmin_lepton")->Fill(mttbar_rec, lep_deltar);
  
}

void HypothesisHists::Finish()
{
  // final calculations, like division and addition of certain histograms


}

