#include "TTbarGen.h"
#include "include/cutflowHists.h"
#include "include/EventCalc.h"
#include <iostream>
#include "../include/Utils.h"
#include <TH2.h>
#include <TGraph.h> 
#include <TVectorT.h>
#include <TLorentzVector.h>

using namespace std;


cutflowHists::cutflowHists(const char* name) : BaseHists(name){
  // named default constructor
 
  topJetConeSize = 1.5;
  runWith4Jets = true;
 
}

cutflowHists::~cutflowHists(){
  // default destructor, does nothing
}

void cutflowHists::Init()
{
  
  
 

  Book( TH1F( "effNominator", "p#_{T}(Jet) (GeV)", 100, 0, 2000));
  Book( TH1F( "effNumerator", "p#_{T}(Jet) (GeV)", 100, 0, 2000));
  Book( TH1F( "numberEvents", "", 1, 0, 1));
  Book( TH1F( "nTopJets", "n jets", 10, 0, 10));
  Book( TH1F( "nJets", "n jets", 20, 0, 20));
  Book( TH1F( "nTopTags", "n top-tags", 10, 0, 10));
  Book( TH1F( "nSubJetBTagsL", "n subjet b-tags", 10, 0, 10));
  Book( TH1F( "nSubJetBTagsM", "n subjet b-tags", 10, 0, 10));
  Book( TH1F( "nSubJetBTagsT", "n subjet b-tags", 10, 0, 10));
  Book( TH1F( "nTopTagsWithSubBTagL", "n top-tags + b-tagged subjet", 10, 0, 10));
  Book( TH1F( "nTopTagsWithSubBTagM", "n top-tags + b-tagged subjet", 10, 0, 10));
  Book( TH1F( "nTopTagsWithSubBTagT", "n top-tags + b-tagged subjet", 10, 0, 10));
  Book( TH1F( "nHiggsTagsLL", "n Higgs-tags", 10, 0, 10));
  Book( TH1F( "nHiggsTagsLM", "n Higgs-tags", 10, 0, 10));
  Book( TH1F( "nHiggsTagsMM", "n Higgs-tags", 10, 0, 10));
  Book( TH1F( "nHiggsTagsMT", "n Higgs-tags", 10, 0, 10));
  Book( TH1F( "nHiggsTagsTT", "n Higgs-tags", 10, 0, 10));
  Book( TH1F( "HTJets", "HT (GeV)", 50, 0, 3000));
  Book( TH1F( "HTJetsHighPt", "HT (GeV)", 50, 0, 3000));
  Book( TH1F( "HTTopJets", "HT (GeV)", 50, 0, 3000));
  Book( TH1F( "ptJet1", "#p_{T}(Jet1) (GeV)", 50, 0, 2000));
  Book( TH1F( "ptJet2", "#p_{T}(Jet2) (GeV)", 50, 0, 2000));
  Book( TH1F( "ptJet3", "#p_{T}(Jet3) (GeV)", 50, 0, 2000));
  Book( TH1F( "ptJet4", "#p_{T}(Jet4) (GeV)", 50, 0, 2000)); 
  Book( TH1F( "InvMass11", "", 50, 0, 2000));
  Book( TH1F( "InvMass21", "", 50, 0, 2000));
  Book( TH1F( "InvMass22", "", 50, 0, 2000));
  Book( TH1F( "InvMassAll", "", 50, 0, 2000));
  Book( TH1F( "InvMassBackToBack", "", 50, 0, 2000));
  Book( TH1F( "bestInvMass1TP1000", "", 100, 0, 2000));
  Book( TH1F( "bestInvMass2TP1000", "", 100, 0, 2000));
  Book( TH1F( "bestInvMass1TP700", "", 100, 0, 2000));
  Book( TH1F( "bestInvMass2TP700", "", 100, 0, 2000));
  Book( TH1F( "bestInvMass1TP500", "", 100, 0, 2000));
  Book( TH1F( "bestInvMass2TP500", "", 100, 0, 2000));
  Book( TH1F( "test", "", 10, 0, 10));
  Book( TH1F( "deltaRDaughters", "", 100, 0, 10));
  Book( TH1F( "deltaPhiDaughters", "", 20, 0, 3.5));
  Book( TH1F( "deltaPhiMothers", "", 20, 0, 3.5));
  Book( TH1F( "NPrimaryVertices", "n prim. vertices", 20, 0, 40));
}


void cutflowHists::Fill()
{ 
  // fill the histograms

  //for topjet copllection
  double ptcut=150;
  //for jet collection
  double ptcutjets=10;

  EventCalc* calc = EventCalc::Instance();
  bool IsRealData = calc->IsRealData();
  LuminosityHandler* lumih = calc->GetLumiHandler();
  // important: get the event weight
  double weight = calc->GetWeight();
  int run = calc->GetRunNum();
  int lumiblock = calc->GetLumiBlock();
  int Npvs = calc->GetPrimaryVertices()->size();
  std::vector<Jet>* jets = calc->GetJets();
  int Njets = 0;
  BaseCycleContainer* bcc = calc->GetBaseCycleContainer();
  for (unsigned int itj=0;itj<bcc->jets->size();itj++){
    if(bcc->jets->at(itj).pt()<ptcutjets) continue;
    Njets=Njets+1;
  }
 
  if (IsRealData == false)TTbarGen* Decay = calc->GetTTbarGen();

  Hist("NPrimaryVertices")-> Fill(Npvs, weight);

  Hist("test") -> Fill(0., weight);
 
  if (bcc->topjets->size() > 0.){
    Hist("effNominator") -> Fill( bcc->topjets->at(0).pt(), weight);
    Hist("effNumerator") -> Fill( bcc->topjets->at(0).pt(), weight);  
  }

  Hist("numberEvents") -> Fill(.5, weight);
  Hist("nJets") -> Fill(Njets, weight);
  int Ntopjets=0;

  for (unsigned int itj=0;itj<bcc->topjets->size();itj++){
    if(bcc->topjets->at(itj).pt()<ptcut) continue;
    Ntopjets=Ntopjets+1;
    if(Ntopjets==1){
      Hist("ptJet1") -> Fill( bcc->topjets->at(itj).pt(), weight);
    }

    if(Ntopjets==2){
      Hist("ptJet2") -> Fill( bcc->topjets->at(itj).pt(), weight);
    }
    if(Ntopjets==3){
      Hist("ptJet3") -> Fill( bcc->topjets->at(itj).pt(), weight);
    }
    if(Ntopjets==4){
      Hist("ptJet4") -> Fill( bcc->topjets->at(itj).pt(), weight);
    }
    if(Ntopjets>4) break;
  }
    


  Hist("nTopJets") -> Fill(Ntopjets, weight);

  int nTopTags = 0;
  int indexFirstTopJet = -99;
  int indexTopJet1 = -99;
  int indexTopJet2 = -99;
  int indexHiggs1= -99;
  int indexHiggs2= -99;
  int nSubTagsFirstTopTaggedJet = 0;
  int nSubTagsL = 0;
  int nSubTagsM = 0;
  int nSubTagsT = 0;
  int countTopTagPlusSubBTagL =0;
  int countTopTagPlusSubBTagM =0;
  int countTopTagPlusSubBTagT =0;
  int countHiggsTagLL = 0;
  int countHiggsTagLM = 0;
  int countHiggsTagMM = 0;
  int countHiggsTagMT = 0;
  int countHiggsTagTT = 0;

  double HTJets = 0.;
  double HTJetsHighPt = 0.;
  for (unsigned int itj=0;itj<bcc->jets->size();itj++){
    if(bcc->jets->at(itj).pt()<ptcutjets) continue;
    HTJets += jets->at(itj).pt();
  }
  Hist("HTJets")-> Fill(HTJets, weight);
  int HTTopJets = 0.;
  for(int i =0; i<bcc->topjets->size(); i++ ){
    if(bcc->topjets->at(i).pt()<ptcut) continue;
    TopJet myJet = bcc->topjets->at(i);
    HTTopJets += myJet.pt();
    if (HepTopTagFull(myJet)){
      nTopTags+= 1;
      if( nTopTags == 1 && indexTopJet1 == -99) indexTopJet1 = i;
      if( nTopTags == 2 && indexTopJet2 == -99) indexTopJet2 = i;
      nSubTagsL = subJetBTag(myJet, e_CSVL);
      nSubTagsM = subJetBTag(myJet, e_CSVM);
      nSubTagsT = subJetBTag(myJet, e_CSVT);
      Hist("nSubJetBTagsL") -> Fill(nSubTagsL, weight);
      Hist("nSubJetBTagsM") -> Fill(nSubTagsM, weight);
      Hist("nSubJetBTagsT") -> Fill(nSubTagsT, weight);
      if (nSubTagsL > 0) countTopTagPlusSubBTagL++;
      if (nSubTagsM > 0) countTopTagPlusSubBTagM++;  
      if (nSubTagsT > 0) countTopTagPlusSubBTagT++;
      if (countTopTagPlusSubBTagM == 1 && indexFirstTopJet == -99)indexFirstTopJet = i;
    }
  }
  for(int i =0; i<bcc->topjets->size(); i++ ){
    if(bcc->topjets->at(i).pt()<ptcut) continue;
    TopJet myJet = bcc->topjets->at(i);
    if(i != indexFirstTopJet){
      if (HiggsTag(myJet, e_CSVL, e_CSVL)) countHiggsTagLL++;  
      if (HiggsTag(myJet, e_CSVL, e_CSVM)) countHiggsTagLM++; 
      if (HiggsTag(myJet, e_CSVM, e_CSVM)) countHiggsTagMM++; 
      if (HiggsTag(myJet, e_CSVM, e_CSVT)) countHiggsTagMT++; 
      if (HiggsTag(myJet, e_CSVT, e_CSVT)) countHiggsTagTT++;
      if (countHiggsTagLL == 1 && indexHiggs1 == -99) indexHiggs1 = i;
      if (countHiggsTagLL == 2 && indexHiggs2 == -99) indexHiggs2 = i;
    }
  }

  Hist("HTTopJets")-> Fill(HTTopJets, weight);
  Hist("nTopTags") -> Fill(nTopTags, weight);
  Hist("nTopTagsWithSubBTagL") -> Fill(countTopTagPlusSubBTagL, weight);
  Hist("nTopTagsWithSubBTagM") -> Fill(countTopTagPlusSubBTagM, weight);
  Hist("nTopTagsWithSubBTagT") -> Fill(countTopTagPlusSubBTagT, weight);
  Hist("nHiggsTagsLL") -> Fill(countHiggsTagLL, weight);
  Hist("nHiggsTagsLM") -> Fill(countHiggsTagLM, weight);
  Hist("nHiggsTagsMM") -> Fill(countHiggsTagMM, weight);
  Hist("nHiggsTagsMT") -> Fill(countHiggsTagMT, weight);
  Hist("nHiggsTagsTT") -> Fill(countHiggsTagTT, weight);

  TopJet combinationJet1;
  TLorentzVector jet1Vec;
  TopJet combinationJet2;
  TLorentzVector jet2Vec;
  TLorentzVector newVector;
  double InvMass11 = -99;
  double InvMass21 = -99;
  double InvMass22 = -99;
  double InvMassBackToBack = -99;

    if (indexTopJet1 != -99 &&indexHiggs1 != -99){
   combinationJet1 = bcc->topjets->at(indexTopJet1);
    jet1Vec.SetPtEtaPhiE(combinationJet1.pt(), combinationJet1.eta(), combinationJet1.phi(), combinationJet1.energy());
    combinationJet2 =  bcc->topjets->at(indexHiggs1);  
    jet2Vec.SetPtEtaPhiE(combinationJet2.pt(), combinationJet2.eta(), combinationJet2.phi(), combinationJet2.energy());
    newVector = jet1Vec + jet2Vec;
    InvMass11 =newVector.M();
    if (indexTopJet2 != -99){
      if(indexHiggs2 != -99){
	combinationJet1 = bcc->topjets->at(indexTopJet2);
	jet1Vec.SetPtEtaPhiE(combinationJet1.pt(), combinationJet1.eta(), combinationJet1.phi(), combinationJet1.energy());
	combinationJet2 =  bcc->topjets->at(indexHiggs2);
	jet2Vec.SetPtEtaPhiE(combinationJet2.pt(), combinationJet2.eta(), combinationJet2.phi(), combinationJet2.energy());
	newVector = jet1Vec + jet2Vec;
	InvMass21 =newVector.M();
      }
      else{
	combinationJet1 = bcc->topjets->at(indexTopJet2);
	jet1Vec.SetPtEtaPhiE(combinationJet1.pt(), combinationJet1.eta(), combinationJet1.phi(), combinationJet1.energy());
	combinationJet2 =  bcc->topjets->at(indexHiggs1);
	jet2Vec.SetPtEtaPhiE(combinationJet2.pt(), combinationJet2.eta(), combinationJet2.phi(), combinationJet2.energy());
	newVector = jet1Vec + jet2Vec;
	InvMass22 =newVector.M();
      }

      combinationJet1 = bcc->topjets->at(indexHiggs1);
      if (combinationJet1.deltaPhi(bcc->topjets->at(indexTopJet1)) > combinationJet1.deltaPhi(bcc->topjets->at(indexTopJet2))){
	combinationJet2 = bcc->topjets->at(indexTopJet1);
      }
      else combinationJet2 = bcc->topjets->at(indexTopJet2);
      jet2Vec.SetPtEtaPhiE(combinationJet2.pt(), combinationJet2.eta(), combinationJet2.phi(), combinationJet2.energy());
      newVector = jet1Vec + jet2Vec;
      InvMassBackToBack =newVector.M();
    }
    if (indexTopJet2 == -99)InvMassBackToBack = InvMass11;
    
    }
    Hist("InvMass11")-> Fill(InvMass11, weight);
    Hist("InvMass21")-> Fill(InvMass21, weight);
    Hist("InvMass22")-> Fill(InvMass22, weight);
    Hist("InvMassAll")-> Fill(InvMass11, weight);
    Hist("InvMassAll")-> Fill(InvMass21, weight);
    Hist("InvMassAll")-> Fill(InvMass22, weight);
    Hist("InvMassBackToBack")-> Fill(InvMassBackToBack, weight);


     
    if(IsRealData == false){
      bool secondTPrime = false;
      GenParticle TPrime1;
      GenParticle TPrime2;

      for( int i=0; i<bcc->genparticles->size(); ++i){
 
	GenParticle genp = bcc->genparticles->at(i); 
	if (abs(genp.pdgId()) > 15000){//selects the Tprime
	  if(secondTPrime == false) TPrime1 = genp;
	  if(secondTPrime == true)  TPrime2 = genp;
	  int indexDaughter1 = genp.daughter(bcc->genparticles,1)->index();
	  int indexDaughter2 = genp.daughter(bcc->genparticles,2)->index();
	  double deltaRDaughters = bcc->genparticles->at(indexDaughter1).deltaR(bcc->genparticles->at(indexDaughter2));
	  double deltaPhiDaughters = bcc->genparticles->at(indexDaughter1).deltaPhi(bcc->genparticles->at(indexDaughter2));
	  Hist("deltaRDaughters") ->  Fill(deltaRDaughters);
	  Hist("deltaPhiDaughters") ->  Fill(deltaPhiDaughters);
	  // if (secondTPrime = false){
	  // 	GenParticle TPrime1Daughter1 = bcc->genparticles->at(indexDaughter1);
	  // 	GenParticle TPrime1Daughter2 = bcc->genparticles->at(indexDaughter2);
	  //       }
	  //       else {
	  // 	GenParticle TPrime2Daughter1 = bcc->genparticles->at(indexDaughter1);
	  // 	GenParticle TPrime2Daughter2 = bcc->genparticles->at(indexDaughter2);
	  //       }
	  secondTPrime = true;
	}
      }
  
      double deltaPhiMothers = TPrime1.deltaPhi(TPrime2);
      Hist("deltaPhiMothers") -> Fill(deltaPhiMothers);
    }
 


  
}
void cutflowHists::Finish()
{
  // final calculations, like division and addition of certain histograms
  EventCalc* calc = EventCalc::Instance(); 
}


