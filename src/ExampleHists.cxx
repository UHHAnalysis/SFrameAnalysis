#include "include/ExampleHists.h"
#include "include/ObjectHandler.h"
#include "include/EventCalc.h"
#include <iostream>

using namespace std;

ExampleHists::ExampleHists(const char* name) : BaseHists(name)
{
  // named default constructor
   
}

ExampleHists::~ExampleHists()
{
  // default destructor, does nothing
}

void ExampleHists::Init()
{
  // book all histograms here

  // missing ET and HT
  double* logMET_bins = MakeLogBinning(100, 50, 1200);
  Book( TH1F( "MET_lx", "missing E_{T} [GeV]", 100, logMET_bins ) );
  double* logHT_bins = MakeLogBinning(100, 150, 3000);
  Book( TH1F( "HT_lx", "H_{T} [GeV]", 100, logHT_bins ) );
  double* logHTlep_bins = MakeLogBinning(100, 150, 1200);
  Book( TH1F( "HTlep_lx", "H_{T}^{lep} [GeV]", 100, logHTlep_bins ) );

  // primary vertices
  Book( TH1F( "N_events_perLumiBin", "N^{evt}", 20, 1, 21 ) );
  Book( TH1F( "N_pv_perLumiBin", "N^{PV}", 20, 1, 21 ) );
  Book( TH1F( "N_pv", "N^{PV}", 50, 0, 50 ) );

  // jets
  Book( TH1F( "N_jets", "N^{jets}", 20, 0, 20 ) );
  double* logPtjet1_bins = MakeLogBinning(100, 150, 1500);
  double* logPtjet2_bins = MakeLogBinning(100, 50, 1000);
  double* logPtjet3_bins = MakeLogBinning(100, 50, 500);
  double* logPtjet4_bins = MakeLogBinning(100, 50, 250);
  Book( TH1F( "pt_jet1_lx", "p_{T}^{jet 1} [GeV]", 100, logPtjet1_bins ) );
  Book( TH1F( "pt_jet2_lx", "p_{T}^{jet 2} [GeV]", 100, logPtjet2_bins ) ); 
  Book( TH1F( "pt_jet3_lx", "p_{T}^{jet 3} [GeV]", 100, logPtjet3_bins ) );
  Book( TH1F( "pt_jet4_lx", "p_{T}^{jet 4} [GeV]", 100, logPtjet4_bins ) );

  // leptons
  Book( TH1F( "N_mu", "N^{#mu}", 10, 0, 10 ) );
  double* logPtlep_bins = MakeLogBinning(50, 45, 500);
  Book( TH1F( "pt_mu_lx", "p_{T}^{#mu}", 50, logPtlep_bins ) );


}

void ExampleHists::Fill()
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
  if(IsRealData){  
    Hist( "N_pv_perLumiBin")->Fill( lumih->GetLumiBin(run, lumiblock), Npvs*weight);
    Hist( "N_events_perLumiBin")->Fill( lumih->GetLumiBin(run, lumiblock), weight);
  }

  //double npu = bcc.genInfo->pileup_TrueNumInteractions();
  //if(npu>50) npu=49.9999;
  //Hist( "N_pileup_hist" )->Fill( npu, weight );

  MET* met = calc->GetMET();
  Hist("MET_lx")->Fill(met->pt(), weight);

  double HT = calc->GetHT();
  Hist("HT_lx")->Fill(HT, weight);

  double HTlep = calc->GetHTlep();
  Hist("HTlep_lx")->Fill(HTlep, weight);

  std::vector<Jet>* jets = calc->GetJets();
  int Njets = jets->size();
  Hist("N_jets")->Fill(Njets, weight);

  if(Njets>=1)
    Hist("pt_jet1_lx")->Fill(jets->at(0).pt(), weight);
  if(Njets>=2)
    Hist("pt_jet2_lx")->Fill(jets->at(1).pt(), weight);
  if(Njets>=3)
    Hist("pt_jet3_lx")->Fill(jets->at(2).pt(), weight);
  if(Njets>=4)
    Hist("pt_jet4_lx")->Fill(jets->at(3).pt(), weight);

  std::vector<Muon>* muons = calc->GetMuons();
  int Nmuons = muons->size();
  Hist("N_mu")->Fill(Nmuons, weight);
  for (int i=0; i<Nmuons; ++i){
    Muon thismu = muons->at(i);
    Hist("pt_mu_lx")->Fill(thismu.pt(), weight);
  }


}

void ExampleHists::Finish()
{
  // final calculations, like division and addition of certain histograms
  EventCalc* calc = EventCalc::Instance();
  bool IsRealData = calc->IsRealData();
  if (IsRealData){
    Hist("N_pv_perLumiBin")->Divide( Hist("N_pv_perLumiBin"), Hist("N_events_perLumiBin"));
    Hist( "N_pv_perLumiBin")->GetYaxis()->SetTitle("Events/Lumi");
  }

}

