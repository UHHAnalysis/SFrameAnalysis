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

  Book( TH2F("M_ttbar_rec_vs_M_ttbar_gen","M_{t#bar{t}}^{rec} [GeV/c^{2}] vs M_{t#bar{t}}^{gen} [GeV/c^{2}]",100,0,3000,100,0,3000));
  Book( TH1F("M_ttbar_resolution", "(M_{t#bar{t}}^{gen} - M_{t#bar{t}}^{rec})/M_{t#bar{t}}^{rec}", 100, -5,5) );
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
  else
    mttbar_rec = -sqrt( (hyp->top_v4()+hyp->antitop_v4()).mass2());
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

}

void HypothesisHists::Finish()
{
  // final calculations, like division and addition of certain histograms


}

