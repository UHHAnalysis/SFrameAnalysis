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
  double* logMET_bins = MakeLogBinning(100, 10, 2000);
  Book( TH1F( "MET_lx", "MET [GeV]", 100, logMET_bins ) );
  double* logHT_bins = MakeLogBinning(100, 100, 4000);
  Book( TH1F( "HT_lx", "H_{T} [GeV]", 100, logHT_bins ) );

  // primary vertices
  Book( TH1F( "N_events_perLumiBin", "N^{evt}", 20, 1, 21 ) );
  Book( TH1F( "N_pv_perLumiBin", "N^{PV}", 20, 1, 21 ) );
  Book( TH1F( "N_pv", "N^{PV}", 50, 0, 50 ) );

  // jets
  Book( TH1F( "N_jets", "N^{jets}", 20, 0, 20 ) );
  double* logPtjet_bins = MakeLogBinning(100, 15, 2000);
  Book( TH1F( "pt_jet", "p_{T}^{jet} [GeV]", 100, logPtjet_bins ) );

  // leptons
  Book( TH1F( "N_mu", "N^{#mu}", 10, 0, 10 ) );
  double* logPtlep_bins = MakeLogBinning(50, 10, 500);
  Book( TH1F( "pt_mu", "p_{T}^{#mu}", 50, logPtlep_bins ) );


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

  std::vector<Jet>* jets = calc->GetJets();
  int Njets = jets->size();
  Hist("N_jets")->Fill(Njets, weight);
  for (int i=0; i<Njets; ++i){
    Jet thisjet = jets->at(i);
    Hist("pt_jet")->Fill(thisjet.pt(), weight);
  }

  std::vector<Muon>* muons = calc->GetMuons();
  int Nmuons = muons->size();
  Hist("N_mu")->Fill(Nmuons, weight);
  for (int i=0; i<Nmuons; ++i){
    Muon thismu = muons->at(i);
    Hist("pt_mu")->Fill(thismu.pt(), weight);
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

