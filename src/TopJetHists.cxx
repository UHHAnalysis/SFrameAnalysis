#include "include/TopJetHists.h"
#include "include/SelectionModules.h"
#include <iostream>

using namespace std;

TopJetHists::TopJetHists(const char* name) : BaseHists(name)
{
  // named default constructor
  
}

TopJetHists::~TopJetHists()
{
  // default destructor, does nothing
}

void TopJetHists::Init()
{
  // book all histograms here
  Book( TH1F( "NTopJets", "number of topjets", 8, -0.5, 7.5 ) );
  Book( TH1F( "NTopJets_ly", "number of topjets", 8, -0.5, 7.5 ) );
  Book( TH1F( "pT"," p_{T} topjets",100,0,2000));
  Book( TH1F( "pT_ly"," p_{T} topjets",100,0,2000));
  Book( TH1F( "eta"," #eta topjets",100,-3,3));
  Book( TH1F( "eta_ly"," #eta topjets",100,-3,3));
  Book( TH1F( "phi"," #phi topjets",100,-PI,PI));
  Book( TH1F( "phi_ly"," #phi topjets",100,-PI,PI));
  Book( TH1F( "MJet", "m_{jet}", 100,0,400 ) );
  Book( TH1F( "MJet_ly", "m_{jet}", 100,0,400 ) );
  Book( TH1F( "Mmin", "m_{min}", 100,0,160 ) );
  Book( TH1F( "Mmin_ly", "m_{min}", 100,0,160 ) );
  Book( TH1F( "NSubjets", "number of subjets", 6,-0.5,5.5) );
  Book( TH1F( "NSubjets_ly", "number of subjets", 6,-0.5,5.5 ) );
  Book( TH1F( "pT_1"," p_{T} leading topjet",100,0,2000));
  Book( TH1F( "pT_1_ly"," p_{T} leading topjet",100,0,2000));
  Book( TH1F( "pT_2","p_{T} 2nd topjet",100,0,2000));
  Book( TH1F( "pT_2_ly","p_{T} 2nd topjet",100,0,2000));
  Book( TH1F( "pT_3","p_{T} 3rd topjet",100,0,1000));
  Book( TH1F( "pT_3_ly","p_{T} 3rd topjet",100,0,1000));
  Book( TH1F( "pT_4","p_{T} 4th topjet",100,0,800));
  Book( TH1F( "pT_4_ly","p_{T} 4th topjet",100,0,800));
  Book( TH1F( "eta_1","#eta leading topjet",100,-3,3));
  Book( TH1F( "eta_1_ly","#eta leading topjet",100,-3,3));
  Book( TH1F( "eta_2","#eta 2nd topjet",100,-3,3));
  Book( TH1F( "eta_2_ly","#eta 2nd topjet",100,-3,3));
  Book( TH1F( "eta_3","#eta 3rd topjet",100,-3,3));
  Book( TH1F( "eta_3_ly","#eta 3rd topjet",100,-3,3));
  Book( TH1F( "eta_4","#eta 4th topjet",100,-3,3));
  Book( TH1F( "eta_4_ly","#eta 4th topjet",100,-3,3));
  Book( TH1F( "phi_1","#phi leading topjet",100,-PI,PI));
  Book( TH1F( "phi_1_ly","#phi leading topjet",100,-PI,PI));
  Book( TH1F( "phi_2","#phi 2nd topjet",100,-PI,PI));
  Book( TH1F( "phi_2_ly","#phi 2nd topjet",100,-PI,PI));
  Book( TH1F( "phi_3","#phi 3rd topjet",100,-PI,PI));
  Book( TH1F( "phi_3_ly","#phi 3rd topjet",100,-PI,PI));
  Book( TH1F( "phi_4","#phi 4th topjet",100,-PI,PI));
  Book( TH1F( "phi_4_ly","#phi 4th topjet",100,-PI,PI));
  Book( TH1F( "MJet_1", "m_{jet} leading topjet", 100,0,400 ) );
  Book( TH1F( "MJet_1_ly", "m_{jet} leading topjet", 100,0,400 ) );
  Book( TH1F( "MJet_2", "m_{jet} 2nd topjet", 100,0,400 ) );
  Book( TH1F( "MJet_2_ly", "m_{jet} 2nd topjet", 100,0,400 ) );
  Book( TH1F( "MJet_3", "m_{jet} 3rd topjet", 100,0,400 ) );
  Book( TH1F( "MJet_3_ly", "m_{jet} 3rd topjet", 100,0,400 ) );
  Book( TH1F( "MJet_4", "m_{jet} 4th topjet", 100,0,400 ) );
  Book( TH1F( "MJet_4_ly", "m_{jet} 4th topjet", 100,0,400 ) );
  Book( TH1F( "Mmin_1", "m_{min} leading topjet", 100,0,160 ) );
  Book( TH1F( "Mmin_1_ly", "m_{min}, leading topjet", 100,0,160 ) );
  Book( TH1F( "Mmin_2", "m_{min} 2nd topjet", 100,0,160 ) );
  Book( TH1F( "Mmin_2_ly", "m_{min} 2nd topjet", 100,0,160 ) );
  Book( TH1F( "Mmin_3", "m_{min} 3rd topjet", 100,0,160 ) );
  Book( TH1F( "Mmin_3_ly", "m_{min} 3rd topjet", 100,0,160 ) );
  Book( TH1F( "Mmin_4", "m_{min} 4th topjet", 100,0,160 ) );
  Book( TH1F( "Mmin_4_ly", "m_{min} 4th topjet", 100,0,160 ) );
  Book( TH1F( "NSubjets_1", "number of subjets leading topjet", 6,-0.5,5.5) );
  Book( TH1F( "NSubjets_1_ly", "number of subjets leading topjet", 6,-0.5,5.5 ) );
  Book( TH1F( "NSubjets_2", "number of subjets 2nd topjet", 6,-0.5,5.5 ) );
  Book( TH1F( "NSubjets_2_ly", "number of subjets 2nd topjet", 6,-0.5,5.5 ) );
  Book( TH1F( "NSubjets_3", "number of subjets 3rd topjet", 6,-0.5,5.5 ) );
  Book( TH1F( "NSubjets_3_ly", "number of subjets 3rd topjet", 6,-0.5,5.5 ) );
  Book( TH1F( "NSubjets_4", "number of subjets 4th topjet", 6,-0.5,5.5 ) ); 
  Book( TH1F( "NSubjets_4_ly", "number of subjets 4th topjet", 6,-0.5,5.5 ) ); 
}


void TopJetHists::Fill()
{
   // important: get the event weight
  EventCalc* calc = EventCalc::Instance();
  double weight = calc -> GetWeight();

  BaseCycleContainer* bcc = calc->GetBaseCycleContainer();
  
  int NTopJets = bcc-> topjets -> size();
  Hist("NTopJets")->Fill(NTopJets, weight);
  Hist("NTopJets_ly")->Fill(NTopJets, weight);
  
  for (unsigned int i =0; i<bcc->topjets->size(); ++i)
    {
      TopJet topjet =  bcc->topjets->at(i);
      
      Hist("pT") -> Fill(topjet.pt(), weight);
      Hist("pT_ly") -> Fill(topjet.pt(), weight);
     
      Hist("eta") -> Fill(topjet.eta(), weight);
      Hist("eta_ly") -> Fill(topjet.eta(), weight);
      Hist("phi") -> Fill(topjet.phi(), weight);
      Hist("phi_ly") -> Fill(topjet.phi(), weight);
      
      double mmin=0;
      double mjet=0;
      int nsubjets=0;
      TopTag(topjet,mjet,nsubjets,mmin);
     
      Hist( "MJet" )->Fill( mjet, weight );
      Hist( "MJet_ly" )->Fill( mjet, weight );
      
      if(nsubjets>=3) 
	{
	  Hist( "Mmin" )->Fill( mmin, weight );
	  Hist( "Mmin_ly" )->Fill( mmin, weight );
	  
	}
      Hist( "NSubjets" )->Fill( nsubjets, weight ); 
      Hist( "NSubjets_ly" )->Fill( nsubjets, weight ); 
      
     
    }
  
  


  sort(bcc->topjets->begin(), bcc->topjets->end(), HigherPt());
  for (unsigned int i =0; i<=3; ++i)
    {
      if (bcc->topjets->size()> i)
	{
	  
	  TopJet topjet =  bcc->topjets->at(i); 
	  TString hname = TString::Format("pT_%d", i+1);
	  Hist(hname)->Fill(topjet.pt(),weight);
	  TString hname_ly = TString::Format("pT_%d_ly", i+1);
	  Hist(hname_ly)->Fill(topjet.pt(),weight);
	  TString hname_eta = TString::Format("eta_%d", i+1);
	  Hist(hname_eta)->Fill(topjet.eta(),weight);
	  TString hname_eta_ly = TString::Format("eta_%d_ly", i+1);
	  Hist(hname_eta_ly)->Fill(topjet.eta(),weight);
	  TString hname_phi = TString::Format("phi_%d", i+1);
	  Hist(hname_phi)->Fill(topjet.phi(),weight);
	  TString hname_phi_ly = TString::Format("phi_%d_ly", i+1);
	  Hist(hname_phi_ly)->Fill(topjet.phi(),weight);
	  
	  double mmin=0;
	  double mjet=0;
	  int nsubjets=0;
	  TopTag(topjet,mjet,nsubjets,mmin);
	  TString hname_MJet = TString::Format("MJet_%d", i+1);
	  Hist(hname_MJet )->Fill( mjet, weight );
	  TString hname_MJet_ly = TString::Format("MJet_%d_ly", i+1);
	  Hist(hname_MJet_ly )->Fill( mjet, weight );
	  if(nsubjets>=3) 
	    {
	      TString hname_Mmin = TString::Format("Mmin_%d", i+1);
	      Hist(hname_Mmin  )->Fill( mmin, weight );
	      TString hname_Mmin_ly = TString::Format("Mmin_%d_ly", i+1);
	      Hist( hname_Mmin_ly )->Fill( mmin, weight );
	    }
	  TString hname_NSubjets = TString::Format("NSubjets_%d", i+1);
	  Hist( hname_NSubjets )->Fill( nsubjets, weight ); 
	  TString hname_NSubjets_ly = TString::Format("NSubjets_%d_ly", i+1);
	  Hist( hname_NSubjets_ly )->Fill( nsubjets, weight ); 
	}
    } 
}
 

void TopJetHists::Finish()
{
  // final calculations, like division and addition of certain histograms 
}

