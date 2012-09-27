#include "include/JetHists.h"
#include "include/ObjectHandler.h"
#include "include/SelectionModules.h"
#include <iostream>

using namespace std;

JetHists::JetHists(const char* name) : BaseHists(name)
{
  // named default constructor
  
}

JetHists::~JetHists()
{
  // default destructor, does nothing
}

void JetHists::Init()
{
  // book all histograms here
  Book( TH1F( "NJets", "number of jets", 13, -0.5, 12.5 ) );
  Book( TH1F( "NJets_ly", "number of jets", 13, -0.5, 12.5 ) );
  Book( TH1F( "pT"," p_{T} jets",100,0,2000));
  Book( TH1F( "pT_ly"," p_{T} jets",100,0,2000));
  Book( TH1F( "eta","#eta jets",100,-3,3));
  Book( TH1F( "eta_ly","#eta jets",100,-3,3));
  Book( TH1F( "phi","#phi jets",100,-PI,PI));
  Book( TH1F( "phi_ly","#phi jets",100,-PI,PI));
  Book( TH1F( "pT_1"," p_{T} leading jet",100,0,2000));
  Book( TH1F( "pT_1_ly"," p_{T} leading jet",100,0,2000));
  Book( TH1F( "pT_2","p_{T} 2nd jet",100,0,2000));
  Book( TH1F( "pT_2_ly","p_{T} 2nd jet",100,0,2000));
  Book( TH1F( "pT_3","p_{T} 3rd jet",100,0,1000));
  Book( TH1F( "pT_3_ly","p_{T} 3rd jet",100,0,1000));
  Book( TH1F( "pT_4","p_{T} 4th jet",100,0,1000));
  Book( TH1F( "pT_4_ly","p_{T} 4th jet",100,0,1000));
  Book( TH1F( "eta_1","#eta leading jet",100,-3,3));
  Book( TH1F( "eta_1_ly","#eta leading jet",100,-3,3));
  Book( TH1F( "eta_2","#eta 2nd jet",100,-3,3));
  Book( TH1F( "eta_2_ly","#eta 2nd jet",100,-3,3));
  Book( TH1F( "eta_3","#eta 3rd jet",100,-3,3));
  Book( TH1F( "eta_3_ly","#eta 3rd jet",100,-3,3));
  Book( TH1F( "eta_4","#eta 4th jet",100,-3,3));
  Book( TH1F( "eta_4_ly","#eta 4th jet",100,-3,3));
  Book( TH1F( "phi_1","#phi leading jet",100,-PI,PI));
  Book( TH1F( "phi_1_ly","#phi leading jet",100,-PI,PI));
  Book( TH1F( "phi_2","#phi 2nd jet",100,-PI,PI));
  Book( TH1F( "phi_2_ly","#phi 2nd jet",100,-PI,PI));
  Book( TH1F( "phi_3","#phi 3rd jet",100,-PI,PI));
  Book( TH1F( "phi_3_ly","#phi 3rd jet",100,-PI,PI));
  Book( TH1F( "phi_4","#phi 4th jet",100,-PI,PI));
  Book( TH1F( "phi_4_ly","#phi 4th jet",100,-PI,PI));
  Book( TH1F( "NbJets", "number of bJets", 8, -0.5, 7.5 ) );
  Book( TH1F( "NbJets_ly", "number of bJets", 8, -0.5, 7.5 ) );
  Book( TH1F( "pT_bJet_1"," p_{T} leading bJet",100,0,1300));
  Book( TH1F( "pT_bJet_1_ly"," p_{T} leading bJet",100,0,1300));
  Book( TH1F( "pT_bJet_2"," p_{T} 2nd bJet",100,0,1300));
  Book( TH1F( "pT_bJet_2_ly"," p_{T} 2nd bJet",100,0,1300));
  Book( TH1F( "eta_bJet_1"," #eta leading bJet",100,-3,3));
  Book( TH1F( "eta_bJet_1_ly"," #eta leading bJet",100,-3,3));
  Book( TH1F( "eta_bJet_2"," #eta 2nd bJet",100,-3,3));
  Book( TH1F( "eta_bJet_2_ly"," #eta 2nd bJet",100,-3,3));
  Book( TH1F( "phi_bJet_1"," #phi leading bJet",100,-PI,PI));
  Book( TH1F( "phi_bJet_1_ly"," #phi leading bJet",100,-PI,PI));
  Book( TH1F( "phi_bJet_2"," #phi 2nd bJet",100,-PI,PI));
  Book( TH1F( "phi_bJet_2_ly"," #phi 2nd bJet",100,-PI,PI));
  Book( TH1F( "bjet_tag","b jet",4,0.5,4.5));
}

void JetHists::Fill()
{
   // important: get the event weight
  EventCalc* calc = EventCalc::Instance();
  double weight = calc -> GetWeight();
  
  ObjectHandler* objs = ObjectHandler::Instance();
  BaseCycleContainer* bcc = objs->GetBaseCycleContainer();
  
  int NJets = bcc-> jets -> size();
  Hist("NJets")->Fill(NJets, weight);
  Hist("NJets_ly")->Fill(NJets, weight);
  
  for (unsigned int i =0; i<bcc->jets->size(); ++i)
    {
      Jet jet =  bcc->jets->at(i); 
      Hist("pT") -> Fill(jet.pt(),weight );
      Hist("pT_ly") -> Fill(jet.pt(),weight);
      Hist("eta") -> Fill(jet.eta(),weight);
      Hist("eta_ly") -> Fill(jet.eta(),weight);
      Hist("phi") -> Fill(jet.phi(),weight);
      Hist("phi_ly") -> Fill(jet.phi(),weight);
    }
  
  sort(bcc->jets->begin(), bcc->jets->end(), HigherPt());
  for (unsigned int i =0; i<=3; ++i)
    {
      if (bcc->jets->size()> i)
	{
	  Jet jet =  bcc->jets->at(i); 
	  TString hname = TString::Format("pT_%d", i+1);
	  Hist(hname)->Fill(jet.pt(),weight);
	  TString hname_ly = TString::Format("pT_%d_ly", i+1);
	  Hist(hname_ly)->Fill(jet.pt(),weight);
	  TString hname_eta = TString::Format("eta_%d", i+1);
	  Hist(hname_eta)->Fill(jet.eta(),weight);
	  TString hname_eta_ly = TString::Format("eta_%d_ly", i+1);
	  Hist(hname_eta_ly)->Fill(jet.eta(),weight);
	  TString hname_phi_ly = TString::Format("phi_%d_ly", i+1);
	  Hist(hname_phi_ly)->Fill(jet.phi(),weight);
	  if (jet.btag_combinedSecondaryVertex()>0.244)
	    {
	      Hist("bjet_tag")-> Fill(i+1,weight);
	    }
	}
    } 
    
  vector<Jet> bjets; 
  for (unsigned int i =0; i<bcc->jets->size(); ++i)
    {
      if(bcc->jets->at(i).btag_combinedSecondaryVertex()>0.244) 
	{
	  bjets.push_back(bcc->jets->at(i));
	}
    }
  
  int NbJets = bjets.size();
  Hist("NbJets")-> Fill(NbJets,weight);
  Hist("NbJets_ly")-> Fill(NbJets,weight);

  sort(bjets.begin(), bjets.end(), HigherPt());
  for (unsigned int i =0; i<=1; ++i)
    {
      if (bjets.size()> i)
	{
	  Jet bjet = bjets[i];
	  TString hname = TString::Format("pT_bJet_%d", i+1);
	  Hist(hname)->Fill(bjet.pt(),weight);
	  TString hname_ly = TString::Format("pT_bJet_%d_ly", i+1);
	  Hist(hname_ly)->Fill(bjet.pt(),weight);
	  TString hname_eta = TString::Format("eta_bJet_%d", i+1);
	  Hist(hname_eta)->Fill(bjet.eta(),weight);
          TString hname_eta_ly = TString::Format("eta_bJet_%d_ly", i+1);
	  Hist(hname_eta_ly)->Fill(bjet.eta(),weight);
	  TString hname_phi = TString::Format("phi_bJet_%d", i+1);
	  Hist(hname_phi)->Fill(bjet.phi(),weight);
          TString hname_phi_ly = TString::Format("phi_bJet_%d_ly", i+1);
	  Hist(hname_phi_ly)->Fill(bjet.phi(),weight);
	}
    } 
}



void JetHists::Finish()
{
  // final calculations, like division and addition of certain histograms

 
}

