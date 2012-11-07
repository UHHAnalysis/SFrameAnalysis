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


  double* logPtjet1_bins = MakeLogBinning(50, 100, 1500);
  double* logPtjet2_bins = MakeLogBinning(50, 30, 1000);
  double* logPtjet3_bins = MakeLogBinning(50, 30, 500);
  double* logPtjet4_bins = MakeLogBinning(50, 30, 250);

  Book( TH1F( "pt_lx"," p_{T} all jets", 50, logPtjet1_bins));
  Book( TH1F( "pt_lxy"," p_{T} all jets", 50, logPtjet1_bins));

  Book( TH1F( "eta","#eta all jets", 50,-3,3));
  Book( TH1F( "phi","#phi all jets", 50, -PI, PI));
  
  // jet pts
  Book( TH1F( "pt_jet1_lx", "p_{T}^{jet 1} [GeV/c]", 50, logPtjet1_bins ) );
  Book( TH1F( "pt_jet1_lxy", "p_{T}^{jet 1} [GeV/c]", 50, logPtjet1_bins ) );
  Book( TH1F( "pt_jet2_lx", "p_{T}^{jet 2} [GeV/c]", 50, logPtjet2_bins ) ); 
  Book( TH1F( "pt_jet2_lxy", "p_{T}^{jet 2} [GeV/c]", 50, logPtjet2_bins ) ); 
  Book( TH1F( "pt_jet3_lx", "p_{T}^{jet 3} [GeV/c]", 50, logPtjet3_bins ) );
  Book( TH1F( "pt_jet3_lxy", "p_{T}^{jet 3} [GeV/c]", 50, logPtjet3_bins ) );
  Book( TH1F( "pt_jet4_lx", "p_{T}^{jet 4} [GeV/c]", 50, logPtjet4_bins ) );
  Book( TH1F( "pt_jet4_lxy", "p_{T}^{jet 4} [GeV/c]", 50, logPtjet4_bins ) );

  // jet etas
  Book( TH1F( "eta_1","#eta leading jet",100,-3,3));
  Book( TH1F( "eta_2","#eta 2nd jet",100,-3,3));
  Book( TH1F( "eta_3","#eta 3rd jet",100,-3,3));
  Book( TH1F( "eta_4","#eta 4th jet",100,-3,3));

  // jet phis
  Book( TH1F( "phi_1","#phi leading jet",100,-PI,PI));
  Book( TH1F( "phi_2","#phi 2nd jet",100,-PI,PI));
  Book( TH1F( "phi_3","#phi 3rd jet",100,-PI,PI));
  Book( TH1F( "phi_4","#phi 4th jet",100,-PI,PI));

  // b-jets
  Book( TH1F( "NbJets", "number of bJets", 8, -0.5, 7.5 ) );
  Book( TH1F( "NbJets_ly", "number of bJets", 8, -0.5, 7.5 ) );

  Book( TH1F( "pt_bJet_1_lx"," p_{T} leading b-jet", 50, logPtjet1_bins));
  Book( TH1F( "pt_bJet_1_lxy"," p_{T} leading b-jet", 50, logPtjet1_bins));
  Book( TH1F( "pt_bJet_2_lx"," p_{T} 2nd bJet", 50, logPtjet1_bins));
  Book( TH1F( "pt_bJet_2_lxy"," p_{T} 2nd bJet", 50, logPtjet1_bins));

  Book( TH1F( "eta_bJet_1"," #eta leading bJet",100,-3,3));
  Book( TH1F( "eta_bJet_2"," #eta 2nd bJet",100,-3,3));
  Book( TH1F( "phi_bJet_1"," #phi leading bJet",100,-PI,PI));
  Book( TH1F( "phi_bJet_2"," #phi 2nd bJet",100,-PI,PI));
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
      Hist("pt_lx") -> Fill(jet.pt(),weight );
      Hist("pt_lxy") -> Fill(jet.pt(),weight);
      Hist("eta") -> Fill(jet.eta(),weight);
      Hist("phi") -> Fill(jet.phi(),weight);
    }
  
  sort(bcc->jets->begin(), bcc->jets->end(), HigherPt());
  for (unsigned int i =0; i<=3; ++i)
    {
      if (bcc->jets->size()> i)
	{
	  Jet jet =  bcc->jets->at(i); 
	  TString hname = TString::Format("pt_jet%d_lx", i+1);
	  Hist(hname)->Fill(jet.pt(),weight);
	  TString hname_ly = TString::Format("pt_jet%d_lxy", i+1);
	  Hist(hname_ly)->Fill(jet.pt(),weight);
	  TString hname_eta = TString::Format("eta_%d", i+1);
	  Hist(hname_eta)->Fill(jet.eta(),weight);
	  TString hname_phi = TString::Format("phi_%d", i+1);
	  Hist(hname_phi)->Fill(jet.phi(),weight);
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
	  TString hname = TString::Format("pt_bJet_%d_lx", i+1);
	  Hist(hname)->Fill(bjet.pt(),weight);
	  TString hname_ly = TString::Format("pt_bJet_%d_lxy", i+1);
	  Hist(hname_ly)->Fill(bjet.pt(),weight);
	  TString hname_eta = TString::Format("eta_bJet_%d", i+1);
	  Hist(hname_eta)->Fill(bjet.eta(),weight);
	  TString hname_phi = TString::Format("phi_bJet_%d", i+1);
	  Hist(hname_phi)->Fill(bjet.phi(),weight);
	}
    } 
}



void JetHists::Finish()
{
  // final calculations, like division and addition of certain histograms

 
}

