#include "SFrameTools/include/EventCalc.h"
#include "include/LikelihoodHists.h"
#include <iostream>

using namespace std;

LikelihoodHists::LikelihoodHists(const char* name, TString filename, TString HypoMass,  TString mode,TString btagfilename) : BaseHists(name)
{
  // named default constructor
  m_HypoMass=HypoMass;
  m_filename=filename;

  m_BTaggingMode = mode;
  m_BTagEffiFilenameMC=btagfilename;

  //cout << "Opeinig file " << m_filename << endl;

  file_mc = new TFile(m_filename);
  HTSignalsingle=(TH1F*) file_mc->Get("HTSubJetsSingleHiggsTagBin__TPTHTH"+HypoMass);
  mHSignalsingle=(TH1F*) file_mc->Get("mHiggsSingleHiggsTagBin__TPTHTH"+HypoMass);
  HTSignalmulti=(TH1F*) file_mc->Get("HTSubJetsMultiHiggsTagBin__TPTHTH"+HypoMass);
  mHSignalmulti=(TH1F*) file_mc->Get("mHiggsMultiHiggsTagBin__TPTHTH"+HypoMass);
  HTQCDmulti=(TH1F*) file_mc->Get("HTSubJetsMultiHiggsTagBin__QCD");
  mHQCDmulti=(TH1F*) file_mc->Get("mHiggsMultiHiggsTagBin__QCD");
  HTttbarmulti=(TH1F*) file_mc->Get("HTSubJetsMultiHiggsTagBin__TTbar");
  mHttbarmulti=(TH1F*) file_mc->Get("mHiggsMultiHiggsTagBin__TTbar");
  HTQCDsingle=(TH1F*) file_mc->Get("HTSubJetsSingleHiggsTagBin__QCD");
  mHQCDsingle=(TH1F*) file_mc->Get("mHiggsSingleHiggsTagBin__QCD");
  HTttbarsingle=(TH1F*) file_mc->Get("HTSubJetsSingleHiggsTagBin__TTbar");
  mHttbarsingle=(TH1F*) file_mc->Get("mHiggsSingleHiggsTagBin__TTbar");
  HTbacksingle=(TH1F*)HTQCDsingle->Clone();
  HTbacksingle->Add(HTttbarsingle);
  mHbacksingle=(TH1F*)mHQCDsingle->Clone();
  mHbacksingle->Add(mHttbarsingle);
  HTbackmulti=(TH1F*)HTQCDmulti->Clone();
  HTbackmulti->Add(HTttbarmulti);
  mHbackmulti=(TH1F*)mHQCDmulti->Clone();
  mHbackmulti->Add(mHttbarmulti);
  HTbacksingle->Scale((1./(HTbacksingle->Integral())));
  mHbacksingle->Scale((1./(mHbacksingle->Integral())));
  HTbackmulti->Scale((1./(HTbackmulti->Integral())));
  mHbackmulti->Scale((1./(mHbackmulti->Integral())));
  HTSignalsingle->Scale((1./(HTSignalsingle->Integral())));
  mHSignalsingle->Scale((1./(mHSignalsingle->Integral())));
  HTSignalmulti->Scale((1./(HTSignalmulti->Integral())));
  mHSignalmulti->Scale((1./(mHSignalmulti->Integral())));

  //cout << "Test " <<  HTttbarmulti->Integral() << endl;

}

void LikelihoodHists::Init()
{
  // book all histograms here
  Book( TH1F( "hL_single", "hL_single", 100, 0, 100));
  Book( TH1F( "hL_multi", "hL_multi", 100, 0, 100));
 
}


void LikelihoodHists::Fill()
{
   // important: get the event weight
  EventCalc* calc = EventCalc::Instance();
  double weight = calc -> GetWeight();

  BaseCycleContainer* bcc = calc->GetBaseCycleContainer();

  double HTSubjets = 0.;
  std::vector<Particle> subjets_top;

  for (unsigned int itj = 0; itj < bcc->topjets->size(); itj++){
    TopJet topjet = bcc->topjets->at(itj);
    subjets_top=topjet.subjets();
    for (unsigned int subj = 0; subj < subjets_top.size(); subj++){
      HTSubjets += subjets_top.at(subj).pt();
    }
  }
  
  //For Rebekka to change
  std::vector<int> topTaggedJets;
  std::vector<int> HiggsTaggedJets;
  int finallySelected = 1;
  int indexTopCandidate = -99;
  int indexHiggsCandidate = -99;
  int nheptoptag=0;
  int nhiggstag=0;
  int nhiggstagWithCut=0;

  for(unsigned int i=0; i< bcc->topjets->size(); ++i){
    TopJet topjet =  bcc->topjets->at(i);

    if(HepTopTagWithMatch(topjet) && subJetBTagTop(topjet, e_CSVM, m_BTaggingMode, m_BTagEffiFilenameMC)>=1){
      nheptoptag++;
      topTaggedJets.push_back(i);
    }
    if (HiggsTag(topjet, e_CSVM, e_CSVM, m_BTaggingMode, m_BTagEffiFilenameMC)){
      nhiggstag++;
      if (HiggsMassFromBTaggedSubjets(topjet, e_CSVM, m_BTaggingMode, m_BTagEffiFilenameMC)>60.){
	nhiggstagWithCut ++;
	HiggsTaggedJets.push_back(i);
      }
    }
  }
  if (nheptoptag == 0){
    finallySelected = 0;
    if (nhiggstagWithCut != 0){
      indexHiggsCandidate = HiggsTaggedJets[0];
    }
  }
  if (nhiggstagWithCut == 0){
    finallySelected = 0; 
    if(nheptoptag != 0){
      indexTopCandidate = topTaggedJets[0];
    }
  }
  if(nheptoptag == 1 && nhiggstagWithCut == 1 && topTaggedJets[0] == HiggsTaggedJets[0]){
    finallySelected = 0;
    indexTopCandidate = topTaggedJets[0];
  } 
  if (finallySelected == 1){
    if (topTaggedJets[0] != HiggsTaggedJets[0]){
      indexTopCandidate = topTaggedJets[0];
      indexHiggsCandidate = HiggsTaggedJets[0];
    }
    if (topTaggedJets[0] == HiggsTaggedJets[0]){
      if (HiggsTaggedJets.size()>1){
	indexTopCandidate = topTaggedJets[0];
	indexHiggsCandidate = HiggsTaggedJets[1];
      }
      if(topTaggedJets.size()>1){
	indexTopCandidate = topTaggedJets[1];
	indexHiggsCandidate = HiggsTaggedJets[0];
      }     
    }
    }





  TopJet higgsCandidateJet=bcc->topjets->at(0);

  double higgsmass=HiggsMassFromBTaggedSubjets(higgsCandidateJet, e_CSVM, m_BTaggingMode, m_BTagEffiFilenameMC);
  
  int iBin_HTsig_single =  HTSignalsingle->FindFixBin(HTSubjets);
  int iBin_HTback_single =  HTbacksingle->FindFixBin(HTSubjets);
  int iBin_mHsig_single =  mHSignalsingle->FindFixBin(higgsmass);
  int iBin_mHback_single =  mHbacksingle->FindFixBin(higgsmass);
  
  int iBin_HTsig_multi =  HTSignalmulti->FindFixBin(HTSubjets);
  int iBin_HTback_multi =  HTbackmulti->FindFixBin(HTSubjets);
  int iBin_mHsig_multi =  mHSignalmulti->FindFixBin(higgsmass);
  int iBin_mHback_multi =  mHbackmulti->FindFixBin(higgsmass);
  
  // get bin content
  double LHT_sig_single =  HTSignalsingle->GetBinContent(iBin_HTsig_single);
  double LHT_back_single = HTbacksingle->GetBinContent(iBin_HTback_single);
  double LMH_sig_single =  mHSignalsingle->GetBinContent(iBin_mHsig_single);
  double LMH_back_single = mHbacksingle->GetBinContent(iBin_mHback_single);

  double LHT_sig_multi =  HTSignalmulti->GetBinContent(iBin_HTsig_multi);
  double LHT_back_multi = HTbackmulti->GetBinContent(iBin_HTback_multi);
  double LMH_sig_multi =  mHSignalmulti->GetBinContent(iBin_mHsig_multi);
  double LMH_back_multi = mHbackmulti->GetBinContent(iBin_mHback_multi);
 
  // cout << "HT signal single " << LHT_sig_multi << endl;
//   cout << "HT signal multi " << LHT_sig_multi << endl;
//   cout << "mH signal single " << LMH_sig_multi << endl;
//   cout << "mH signal multi " << LMH_sig_multi << endl;

  double L_single = (LHT_sig_single / LHT_back_single ) * (LMH_sig_single / LMH_back_single);
  double L_multi = (LHT_sig_multi / LHT_back_multi ) * (LMH_sig_multi / LMH_back_multi); 

  Hist("hL_single")->Fill(L_single,weight);
  Hist("hL_multi")->Fill(L_multi,weight);
 
 //  cout  << "Likelihhod single " << L_single << endl;
//   cout << "Likelihood multi " << L_multi << endl;
 
}
