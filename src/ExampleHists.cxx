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
  double* logMET_bins = MakeLogBinning(40, 50, 1200);
  Book( TH1F( "MET_lx", "missing E_{T} [GeV]", 40, logMET_bins ) );
  double* logHT_bins = MakeLogBinning(40, 150, 3000);
  Book( TH1F( "HT_lx", "H_{T} [GeV]", 40, logHT_bins ) );
  double* logHTlep_bins = MakeLogBinning(40, 150, 1200);
  Book( TH1F( "HTlep_lx", "H_{T}^{lep} [GeV]", 40, logHTlep_bins ) );

  // jets
  Book( TH1F( "N_jets_ly", "N^{jets}", 20, 0, 20 ) );
  double* logPtjet1_bins = MakeLogBinning(40, 150, 1500);
  double* logPtjet2_bins = MakeLogBinning(40, 50, 1000);
  double* logPtjet3_bins = MakeLogBinning(40, 50, 500);
  double* logPtjet4_bins = MakeLogBinning(40, 50, 250);
  Book( TH1F( "pt_jet1_lx", "p_{T}^{jet 1} [GeV/c]", 40, logPtjet1_bins ) );
  Book( TH1F( "pt_jet2_lx", "p_{T}^{jet 2} [GeV/c]", 40, logPtjet2_bins ) ); 
  Book( TH1F( "pt_jet3_lx", "p_{T}^{jet 3} [GeV/c]", 40, logPtjet3_bins ) );
  Book( TH1F( "pt_jet4_lx", "p_{T}^{jet 4} [GeV/c]", 40, logPtjet4_bins ) );
  Book( TH1F( "eta_jet1", "#eta^{jet 1}", 40, -2.5, 2.5) );
  Book( TH1F( "eta_jet2", "#eta^{jet 2}", 40, -2.5, 2.5) );
  Book( TH1F( "eta_jet3", "#eta^{jet 3}", 40, -2.5, 2.5) );
  Book( TH1F( "eta_jet4", "#eta^{jet 4}", 40, -2.5, 2.5) );

  // leptons
  Book( TH1F( "N_mu", "N^{#mu}", 10, 0, 10 ) );
  double* logPtlep_bins = MakeLogBinning(40, 45, 500);
  Book( TH1F( "pt_mu_lx", "p_{T}^{#mu} [GeV/c]", 40, logPtlep_bins ) );
  Book( TH1F( "eta_mu", "#eta^{#mu}", 40, -2.1, 2.1) );
  Book( TH1F( "reliso_mu", "#mu rel. Iso", 40, 0, 0.5) );
  Book( TH1F( "ptrel_mu", "p_{T}^{rel}(#mu,jet)", 40, 0, 200.) );
  Book( TH1F( "deltaRmin_mu", "#Delta R_{min}(#mu,jet)", 40, 0, 2.0) );

  // primary vertices
  Book( TH1F( "N_pv", "N^{PV}", 50, 0, 50 ) );
//   Book( TH1F( "N_events_perLumiBin", "N^{evt}", 20, 1, 21 ) );
//   Book( TH1F( "N_pv_perLumiBin", "N^{PV}", 20, 1, 21 ) );

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
//   if(IsRealData){  
//     Hist( "N_pv_perLumiBin")->Fill( lumih->GetLumiBin(run, lumiblock), Npvs*weight);
//     Hist( "N_events_perLumiBin")->Fill( lumih->GetLumiBin(run, lumiblock), weight);
//   }

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
  Hist("N_jets_ly")->Fill(Njets, weight);

  if(Njets>=1){
    Hist("pt_jet1_lx")->Fill(jets->at(0).pt(), weight);
    Hist("eta_jet1")->Fill(jets->at(0).eta(), weight);
  }
  if(Njets>=2){
    Hist("pt_jet2_lx")->Fill(jets->at(1).pt(), weight);
    Hist("eta_jet2")->Fill(jets->at(1).eta(), weight);
  }
  if(Njets>=3){
    Hist("pt_jet3_lx")->Fill(jets->at(2).pt(), weight);
    Hist("eta_jet3")->Fill(jets->at(2).eta(), weight);
  }
  if(Njets>=4){
    Hist("pt_jet4_lx")->Fill(jets->at(3).pt(), weight);
    Hist("eta_jet4")->Fill(jets->at(3).eta(), weight);
  }

  std::vector<Muon>* muons = calc->GetMuons();
  int Nmuons = muons->size();
  Hist("N_mu")->Fill(Nmuons, weight);
  for (int i=0; i<Nmuons; ++i){
    Muon thismu = muons->at(i);
    Hist("pt_mu_lx")->Fill(thismu.pt(), weight);
    Hist("eta_mu")->Fill(thismu.eta(), weight);

    Hist("reliso_mu")->Fill(thismu.relIso(), weight);
    Hist("ptrel_mu")->Fill( pTrel(&thismu, jets), weight);
    Hist("deltaRmin_mu")->Fill( deltaRmin(&thismu, jets), weight);

  }


}

void ExampleHists::Finish()
{
  // final calculations, like division and addition of certain histograms
  EventCalc* calc = EventCalc::Instance();
  bool IsRealData = calc->IsRealData();
  if (IsRealData){
//     Hist("N_pv_perLumiBin")->Divide( Hist("N_pv_perLumiBin"), Hist("N_events_perLumiBin"));
//     Hist( "N_pv_perLumiBin")->GetYaxis()->SetTitle("Events/Lumi");
  }

}

