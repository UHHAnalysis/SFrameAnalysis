#include "include/AnalysisHists.h"
#include "include/ObjectHandler.h"
#include "include/EventCalc.h"
#include <iostream>

using namespace std;

AnalysisHists::AnalysisHists(const char* name, HypothesisDiscriminator *discr) : BaseHists(name)
{
  // named default constructor
  m_discr = discr;
}

AnalysisHists::~AnalysisHists()
{
  // default destructor, does nothing
}

void AnalysisHists::Init()
{
  // missing ET and HT
  double* logMET_bins = MakeLogBinning(50, 0, 1500);
  Book( TH1F( "MET", "missing E_{T} [GeV]", 1500, 0, 1500) );//50, logMET_bins ) );
  double* logHT_bins = MakeLogBinning(100, 0, 3000);
  Book( TH1F( "HT", "H_{T} [GeV]", 3000, 0, 3000) );//100, logHT_bins ) );
  double* logHTlep_bins = MakeLogBinning(50, 0, 1500);
  Book( TH1F( "HTlep", "H_{T}^{lep} [GeV]", 1500, 0, 1500) );//50, logHTlep_bins ) );

  // jets
  Book( TH1F( "N_jets", "N^{jets}", 20, 0, 20 ) );
 /* double* logPtjet1_bins = MakeLogBinning(50, 0, 1500);
  double* logPtjet2_bins = MakeLogBinning(50, 0, 1500);
  double* logPtjet3_bins = MakeLogBinning(50, 0, 1500);
  double* logPtjet4_bins = MakeLogBinning(50, 0, 1500);
  double* logMjet1_bins = MakeLogBinning(50, 0, 1500);
  double* logMjet2_bins = MakeLogBinning(50, 0, 1500);
  double* logMjet3_bins = MakeLogBinning(50, 0, 1500);
  double* logMjet4_bins = MakeLogBinning(50, 0, 1500);*/
  Book( TH1F( "pt_jet1", "p_{T}^{jet 1} [GeV/c]", 1500, 0, 1500) );// logPtjet1_bins ) );
  Book( TH1F( "pt_jet2", "p_{T}^{jet 2} [GeV/c]", 1500, 0, 1500) );//, logPtjet2_bins ) );
  Book( TH1F( "pt_jet3", "p_{T}^{jet 3} [GeV/c]", 1500, 0, 1500) );//50, logPtjet3_bins ) );
  Book( TH1F( "pt_jet4", "p_{T}^{jet 4} [GeV/c]", 1500, 0, 1500) );//50, logPtjet4_bins ) );
  Book( TH1F( "m_jet1", "M^{jet 1} [GeV/c]", 1500, 0, 1500) );//50, logMjet1_bins ) );
  Book( TH1F( "m_jet2", "M^{jet 2} [GeV/c]", 1500, 0, 1500) );//50, logMjet2_bins ) );
  Book( TH1F( "m_jet3", "M^{jet 3} [GeV/c]", 1500, 0, 1500) );//50, logMjet3_bins ) );
  Book( TH1F( "m_jet4", "M^{jet 4} [GeV/c]", 1500, 0, 1500) );//50, logMjet4_bins ) );
  Book( TH1F( "eta_jet1", "#eta^{jet 1}", 50, -2.5, 2.5) );
  Book( TH1F( "eta_jet2", "#eta^{jet 2}", 50, -2.5, 2.5) );
  Book( TH1F( "eta_jet3", "#eta^{jet 3}", 50, -2.5, 2.5) );
  Book( TH1F( "eta_jet4", "#eta^{jet 4}", 50, -2.5, 2.5) );

  // leptons
  Book( TH1F( "N_ele", "N^{e}", 10, 0, 10 ) );
  double* logPtlep_bins = MakeLogBinning(50, 45, 500);
  Book( TH1F( "pt_ele", "p_{T}^{e} [GeV/c]", 50, logPtlep_bins ) );
  Book( TH1F( "eta_ele", "#eta^{e}", 50, -2.1, 2.1) );

  // primary vertices
  Book( TH1F( "N_pv", "N^{PV}", 50, 0, 50 ) );

  Book( TH1F( "M_ttbar_rec", "M_{t#bar{t}}^{rec} [GeV/c^{2}]", 150, 0, 3000 ) );

  Book( TH1F( "M_toplep_rec", "M^{top,lep} [GeV/c^{2}]", 70, 0, 700 ) );
  Book( TH1F( "M_tophad_rec", "M^{top,had} [GeV/c^{2}]", 50, 0, 500 ) );

  Book( TH1F( "Pt_toplep_rec", "P_{T}^{top,lep} [GeV/c]", 60, 0, 1200 ) );
  Book( TH1F( "Pt_tophad_rec", "P_{T}^{top,had} [GeV/c]", 60, 0, 1200 ) );

  TString name = m_discr->GetLabel();
  name += " discriminator";
  double min=0;
  double max=500;
  if( m_discr->GetLabel()=="BestPossible"){
    min=0;
    max=6;
  }
  Book( TH1F("Discriminator", name , 100, min,max) );

//   Book( TH2F("M_ttbar_rec_vs_M_ttbar_gen","M_{t#bar{t}}^{rec} [GeV/c^{2}] vs M_{t#bar{t}}^{gen} [GeV/c^{2}]",100,0,3000,100,0,3000));
//   Book( TH1F("M_ttbar_resolution", "(M_{t#bar{t}}^{gen} - M_{t#bar{t}}^{rec})/M_{t#bar{t}}^{rec}", 100, -5,5) );
}

void AnalysisHists::Fill()
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

  Hist("N_pv")->Fill(Npvs, weight);
//   if(IsRealData){
//     Hist( "N_pv_perLumiBin")->Fill( lumih->GetLumiBin(run, lumiblock), Npvs*weight);
//     Hist( "N_events_perLumiBin")->Fill( lumih->GetLumiBin(run, lumiblock), weight);
//   }

  //double npu = bcc.genInfo->pileup_TrueNumInteractions();
  //if(npu>50) npu=49.9999;
  //Hist( "N_pileup_hist" )->Fill( npu, weight );

  MET* met = calc->GetMET();
  Hist("MET")->Fill(met->pt(), weight);

  double HT = calc->GetHT();
  Hist("HT")->Fill(HT, weight);

  double HTlep = calc->GetHTlep();
  Hist("HTlep")->Fill(HTlep, weight);

  std::vector<Jet>* jets = calc->GetJets();
  int Njets = jets->size();
  Hist("N_jets")->Fill(Njets, weight);

  if(Njets>=1){
    Hist("pt_jet1")->Fill(jets->at(0).pt(), weight);
    Hist("eta_jet1")->Fill(jets->at(0).eta(), weight);
    if(jets->at(0).v4().isTimelike())
        Hist("m_jet1")->Fill(jets->at(0).v4().M(), weight);
  }
  if(Njets>=2){
    Hist("pt_jet2")->Fill(jets->at(1).pt(), weight);
    Hist("eta_jet2")->Fill(jets->at(1).eta(), weight);
    if(jets->at(1).v4().isTimelike())
        Hist("m_jet2")->Fill(jets->at(1).v4().M(), weight);
  }
  if(Njets>=3){
    Hist("pt_jet3")->Fill(jets->at(2).pt(), weight);
    Hist("eta_jet3")->Fill(jets->at(2).eta(), weight);
    if(jets->at(2).v4().isTimelike())
        Hist("m_jet3")->Fill(jets->at(2).v4().M(), weight);
  }
  if(Njets>=4){
    Hist("pt_jet4")->Fill(jets->at(3).pt(), weight);
    Hist("eta_jet4")->Fill(jets->at(3).eta(), weight);
    if(jets->at(3).v4().isTimelike())
        Hist("m_jet4")->Fill(jets->at(3).v4().M(), weight);
  }


   std::vector<Electron>* electrons = calc->GetElectrons();
  int Nelectrons = electrons->size();
  Hist("N_ele")->Fill(Nelectrons, weight);
  for (int i=0; i<Nelectrons; ++i){
    Electron thisele = electrons->at(i);
    Hist("pt_ele")->Fill(thisele.pt(), weight);
    Hist("eta_ele")->Fill(thisele.eta(), weight);
  }



  ReconstructionHypothesis* hyp = m_discr->GetBestHypothesis();

  double mttbar_rec = 0;
  if( (hyp->top_v4()+hyp->antitop_v4()).isTimelike() )
    mttbar_rec = (hyp->top_v4()+hyp->antitop_v4()).M();
  else
    mttbar_rec = -sqrt( (hyp->top_v4()+hyp->antitop_v4()).mass2());
  double mttbar_gen = 0;
  if(calc->GetGenParticles() )
    mttbar_gen = ( calc->GetTTbarGen()->Top().v4() + calc->GetTTbarGen()->Antitop().v4()).M();

  Hist("M_ttbar_rec")->Fill(mttbar_rec, weight);
//   Hist("M_ttbar_rec_vs_M_ttbar_gen")->Fill(mttbar_rec, mttbar_gen);
//   Hist("M_ttbar_resolution")->Fill( (mttbar_gen-mttbar_rec)/mttbar_gen, weight);
  Hist("Discriminator")->Fill(hyp->discriminator(m_discr->GetLabel()), weight);

  double mtoplep=0;
  double mtophad=0;
  if(hyp->toplep_v4().isTimelike()) mtoplep = hyp->toplep_v4().M();
  if(hyp->tophad_v4().isTimelike()) mtophad = hyp->tophad_v4().M();
  Hist("M_toplep_rec")->Fill(mtoplep,weight);
  Hist("M_tophad_rec")->Fill(mtophad,weight);

  Hist("Pt_toplep_rec")->Fill( hyp->toplep_v4().Pt(),weight );
  Hist("Pt_tophad_rec")->Fill( hyp->tophad_v4().Pt(),weight );

}

void AnalysisHists::Finish()
{
  // final calculations, like division and addition of certain histograms


}

