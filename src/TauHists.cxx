#include "include/TauHists.h"
#include "include/SelectionModules.h"
#include "TH2.h"
#include "TLorentzVector.h"
#include <iostream>

using namespace std;

TauHists::TauHists(const char* name) : BaseHists(name)
{
  // named default constructor
  
}

TauHists::~TauHists()
{
  // default destructor, does nothing
}

void TauHists::Init()
{
  Book( TH1F( "number", "number of taus",6,-0.5,5.5 ) );
  Book( TH1F( "number_ly", "number of taus",6,-0.5,5.5 ) );
  Book( TH1F( "pT", "p_{T} tau", 100,0,600 ) );
  Book( TH1F( "pT_ly", "p_{T} tau", 100,0,600 ) );
  Book( TH1F( "eta", "#eta tau", 100,-3,3 ) );
  Book( TH1F( "eta_ly", "#eta tau", 100,-3,3 ) );
  Book( TH1F( "phi", "#phi tau", 100,-PI,PI ) );
  Book( TH1F( "phi_ly", "#phi tau", 100,-PI,PI ) );
  Book( TH1F( "charge", "charge tau", 3,-1,1 ) );
  Book( TH1F( "energy", "E tau", 100,0,1100 ) );
  Book( TH1F( "energy_ly", "E tau", 100,0,1100 ) );
  Book( TH1F( "decayMode", "decay mode finding tau", 2,0,2 ) );
  Book( TH1F( "isolation", "by loose combined isolation DeltaBetaCorr tau", 2,0,2 ) );
  Book( TH1F( "invMass", "invariant mass taus",100, 0,1200));
  Book( TH1F( "invMass_ly", "invariant mass taus",100, 0,1200));
  Book( TH1F( "pT_1", "p_{T} leading tau", 100,0,600 ) );
  Book( TH1F( "pT_1_ly", "p_{T} leading tau", 100,0,600 ) );
  Book( TH1F( "eta_1", "#eta leading tau", 100,-3,3 ) );
  Book( TH1F( "eta_1_ly", "#eta leading tau", 100,-3,3 ) );
  Book( TH1F( "phi_1", "#phi leading tau", 100,-PI,PI ) );
  Book( TH1F( "phi_1_ly", "#phi leading tau", 100,-PI,PI ) );
  Book( TH1F( "decayMode_1", "decay mode finding leading tau", 2,0,2 ) );
  Book( TH1F( "isolation_1", "by loose combined isolation DeltaBetaCorr leading tau", 2,0,2 ) );
  Book( TH1F( "pT_2", "p_{T} 2nd tau", 100,0,500 ) );
  Book( TH1F( "pT_2_ly", "p_{T} 2nd tau", 100,0,500 ) );
  Book( TH1F( "eta_2", "#eta 2nd tau", 100,-3,3 ) );
  Book( TH1F( "eta_2_ly", "#eta 2nd tau", 100,-3,3 ) );
  Book( TH1F( "phi_2", "#phi 2nd tau", 100,-PI,PI ) );
  Book( TH1F( "phi_2_ly", "#phi 2nd tau", 100,-PI,PI ) );
  Book( TH1F( "decayMode_2", "decay mode finding 2nd tau", 2,0,2 ) );
  Book( TH1F( "isolation_2", "by loose combined isolation DeltaBetaCorr 2nd tau", 2,0,2 ) );
  Book( TH2F( "Jet1_Tau1_pT","1.Jet, 1.Tau",100,0,2000,100,0,2000));
  Book( TH2F( "Jet1_Tau2_pT","1.Jet, 2.Tau",100,0,2000,100,0,2000));	
  Book( TH2F( "Jet2_Tau1_pT","2.Jet, 1.Tau",100,0,2000,100,0,2000));
  Book( TH2F( "Jet2_Tau2_pT","2.Jet, 2.Tau",100,0,2000,100,0,2000));	 
  Book( TH2F( "Tau1_Tau2_pT","1.Jet, 2.Tau",100,0,2000,100,0,2000));
  Book( TH1F( "deltar_Tau1_Jet2", "#Deltar 1.Tau, 2.Jet ", 100, 0,5));
  Book( TH1F( "deltar_Tau1_Jet1", "#Deltar 1.Tau, 1.Jet ",100, 0,5));
  Book( TH1F( "deltar_Tau1_Jet2_ly", "#Deltar 1.Tau, 2.Jet ", 100, 0,5));
  Book( TH1F( "deltar_Tau1_Jet1_ly", "#Deltar 1.Tau, 1.Jet ",100, 0,5));

}

void TauHists::Fill()
{
   // important: get the event weight
  EventCalc* calc = EventCalc::Instance();
  double weight = calc -> GetWeight();
  
  BaseCycleContainer* bcc = calc->GetBaseCycleContainer();
  
  int NTaus = bcc-> taus -> size();
  Hist("number")->Fill(NTaus, weight);
  Hist("number_ly")->Fill(NTaus, weight);


  for(unsigned int i=0; i< bcc->taus->size(); ++i)
    {
      
      Tau tau = bcc->taus->at(i);
      Hist("pT") -> Fill(tau.pt(), weight);
      Hist("pT_ly") -> Fill(tau.pt(), weight);
      Hist("eta") -> Fill(tau.eta(), weight);
      Hist("eta_ly") -> Fill(tau.eta(), weight);
      Hist("phi") -> Fill(tau.phi(), weight);
      Hist("phi_ly") -> Fill(tau.phi(), weight);
      Hist("charge") -> Fill(tau.charge(), weight);
      Hist("energy") -> Fill(tau.energy(), weight);
      Hist("energy_ly") -> Fill(tau.energy(), weight);
      Hist("decayMode") -> Fill(tau.decayModeFinding(), weight);
      Hist("isolation") -> Fill(tau.byLooseCombinedIsolationDeltaBetaCorr(), weight);
    }
  
  
  if (bcc->taus->size() > 0)
    {
      Tau tau = bcc->taus->at(0);
      Hist("pT_1") -> Fill(tau.pt(), weight);
      Hist("pT_1_ly") -> Fill(tau.pt(), weight);
      Hist("eta_1") -> Fill(tau.eta(), weight);
      Hist("eta_1_ly") -> Fill(tau.eta(), weight);
      Hist("phi_1") -> Fill(tau.phi(), weight);
      Hist("phi_1_ly") -> Fill(tau.phi(), weight);
      Hist("decayMode_1") -> Fill(tau.decayModeFinding(), weight);
      Hist("isolation_1") -> Fill(tau.byLooseCombinedIsolationDeltaBetaCorr(), weight);
      if (bcc->taus->size() > 1 )
	{
	  Tau tau1 = bcc->taus->at(1);
	  Hist("pT_2") -> Fill(tau1.pt(), weight);
	  Hist("pT_2_ly") -> Fill(tau1.pt(), weight);
	  Hist("eta_2") -> Fill(tau1.eta(), weight);
	  Hist("eta_2_ly") -> Fill(tau1.eta(), weight);
	  Hist("phi_2") -> Fill(tau1.phi(), weight);
	  Hist("phi_2_ly") -> Fill(tau1.phi(), weight);
	  Hist("decayMode_2") -> Fill(tau1.decayModeFinding(), weight);
	  Hist("isolation_2") -> Fill(tau1.byLooseCombinedIsolationDeltaBetaCorr(), weight);
	  TH2F* h6 = (TH2F*)Hist("Tau1_Tau2_pT"); 
	  h6->Fill(tau.pt(),tau1.pt(),weight);
	}
    }
  
  sort(bcc->taus->begin(), bcc->taus->end(), HigherPt()); 
  if (bcc->taus->size() >= 2)
    {
      Tau tau1 = bcc->taus->at(0);
      Tau tau2 = bcc->taus->at(1);
      TLorentzVector Tau1;
      TLorentzVector Tau2;
      TLorentzVector Tau;
      Tau1.SetPtEtaPhiE(tau1.pt() ,tau1.eta() ,tau1.phi() ,tau1.energy() );
      Tau2.SetPtEtaPhiE(tau2.pt() ,tau2.eta() ,tau2.phi() ,tau2.energy() );
      Tau = Tau1 + Tau2;
      Hist("invMass")->Fill(Tau.M(),weight);
      Hist("invMass_ly")->Fill(Tau.M(),weight);
    }


  sort(bcc->topjets->begin(), bcc->topjets->end(), HigherPt()); 
  sort(bcc->taus->begin(), bcc->taus->end(), HigherPt());
  if (bcc->taus->size() > 0 && bcc->topjets->size() > 0 )
    {
      Tau tau = bcc->taus->at(0);
      TopJet topjet = bcc->topjets->at(0);
      TH2F* h2 = (TH2F*)Hist("Jet1_Tau1_pT"); 
      h2->Fill(topjet.pt(),tau.pt(),weight);
      double deltar_Tau1_Jet1 = topjet.deltaR(tau);
      Hist ("deltar_Tau1_Jet1") -> Fill(deltar_Tau1_Jet1,weight);
      Hist ("deltar_Tau1_Jet1_ly") -> Fill(deltar_Tau1_Jet1,weight);
      if (bcc->taus->size() > 1 )
	{
	  Tau tau1 = bcc->taus->at(1);
	  TH2F* h3 = (TH2F*)Hist("Jet1_Tau2_pT"); 
	  h3->Fill(topjet.pt(),tau1.pt(),weight);
	}
    }
  
  if (bcc->taus->size() > 0 && bcc->topjets->size() > 0 )
    {
      if (bcc->topjets->size() > 1 )
	{
	  Tau tau = bcc->taus->at(0);
	  TopJet topjet = bcc->topjets->at(1);
	  TH2F* h4 = (TH2F*)Hist("Jet2_Tau1_pT"); 
	  h4->Fill(topjet.pt(),tau.pt(),weight);
	  double deltar_Tau1_Jet2 = topjet.deltaR(tau);
	  Hist ("deltar_Tau1_Jet2") -> Fill(deltar_Tau1_Jet2,weight);
	  Hist ("deltar_Tau1_Jet2_ly") -> Fill(deltar_Tau1_Jet2,weight);
	  if (bcc->taus->size() > 1 )
	    {
	      Tau tau1 = bcc->taus->at(1);
	      TH2F* h5 = (TH2F*)Hist("Jet2_Tau2_pT"); 
	      h5->Fill(topjet.pt(),tau1.pt(),weight);
	    }
	}
    }

}

void TauHists::Finish()
{
  // final calculations, like division and addition of certain histograms

 
}



