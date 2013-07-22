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
  Book( TH1F( "pT", "p_{T} tau [GeV]", 100,0,600 ) );
  Book( TH1F( "pT_ly", "p_{T} tau [GeV]", 100,0,600 ) );
  Book( TH1F( "eta", "#eta tau", 100,-3,3 ) );
  Book( TH1F( "eta_ly", "#eta tau", 100,-3,3 ) );
  Book( TH1F( "phi", "#phi tau", 100,-PI,PI ) );
  Book( TH1F( "phi_ly", "#phi tau", 100,-PI,PI ) );
  Book( TH1F( "charge", "charge tau", 3,-1.5,1.5 ) );
  Book( TH1F( "energy", "E tau [GeV]", 100,0,1100 ) );
  Book( TH1F( "energy_ly", "E tau [GeV]", 100,0,1100 ) );
  Book( TH1F( "decayMode", "decay mode finding tau", 2,0,2 ) );
  Book( TH1F( "isolation", "by medium combined isolation DeltaBetaCorr tau", 2,0,2 ) );
  Book( TH1F( "againstElectron", "against electron loose", 2,0,2 ) );
  Book( TH1F( "againstMuon", "against muon loose", 2,0,2 ) );
  Book( TH1F( "invMass", "invariant mass taus [GeV]",100, 0,1200));
  Book( TH1F( "invMass_ly", "invariant mass taus [GeV]",100, 0,1200));
  Book( TH1F( "pT_1", "p_{T} leading tau [GeV]", 100,0,600 ) );
  Book( TH1F( "pT_1_ly", "p_{T} leading tau [GeV]", 160,0,800 ) );
  Double_t bins[5] = {20, 60, 120, 200, 800};
  Book( TH1F( "pT_1_binned", "p_{T} leading tau [GeV]",4, bins));
  Book( TH1F( "pT_1_binned_ly", "p_{T} leading tau [GeV]", 4,bins ) );
  Book( TH1F( "pT_1_binned_lx", "p_{T} leading tau [GeV]", 4,bins ) );
  Book( TH1F( "eta_1", "#eta leading tau", 100,-3,3 ) );
  Book( TH1F( "eta_1_ly", "#eta leading tau", 100,-3,3 ) );
  Book( TH1F( "phi_1", "#phi leading tau", 100,-PI,PI ) );
  Book( TH1F( "phi_1_ly", "#phi leading tau", 100,-PI,PI ) );
  Book( TH1F( "decayMode_1", "decay mode finding leading tau", 2,0,2 ) );
  Book( TH1F( "isolation_1", "by medium combined isolation DeltaBetaCorr leading tau", 2,0,2 ) );
  Book( TH1F( "againstElectron_1", "against electron loose", 2,0,2 ) );
  Book( TH1F( "againstMuon_1", "against muon loose", 2,0,2 ) );
  Book( TH1F( "pT_2", "p_{T} 2nd tau [GeV]", 100,0,500 ) );
  Book( TH1F( "pT_2_ly", "p_{T} 2nd tau [GeV]", 100,0,500 ) );
  Book( TH1F( "eta_2", "#eta 2nd tau", 100,-3,3 ) );
  Book( TH1F( "eta_2_ly", "#eta 2nd tau ", 100,-3,3 ) );
  Book( TH1F( "phi_2", "#phi 2nd tau", 100,-PI,PI ) );
  Book( TH1F( "phi_2_ly", "#phi 2nd tau", 100,-PI,PI ) );
  Book( TH1F( "decayMode_2", "decay mode finding 2nd tau", 2,0,2 ) );
  Book( TH1F( "isolation_2", "by medium combined isolation DeltaBetaCorr 2nd tau", 2,0,2 ) );
  Book( TH1F( "againstElectron_2", "against electron loose", 2,0,2 ) );
  Book( TH1F( "againstMuon_2", "against muon loose", 2,0,2 ) );
  Book( TH2F( "Jet1_Tau1_pT","1.Jet, 1.Tau",100,0,2000,100,0,2000));
  Book( TH2F( "Jet1_Tau2_pT","1.Jet, 2.Tau",100,0,2000,100,0,2000));	
  Book( TH2F( "Jet2_Tau1_pT","2.Jet, 1.Tau",100,0,2000,100,0,2000));
  Book( TH2F( "Jet2_Tau2_pT","2.Jet, 2.Tau",100,0,2000,100,0,2000));	 
  Book( TH2F( "Tau1_Tau2_pT","1.Jet, 2.Tau",100,0,2000,100,0,2000));
  Book( TH1F( "deltar_Tau_Jet", "#Deltar taus, jets  ", 50, 0,5));
  Book( TH1F( "deltar_Tau_Jet_ly", "#Deltar taus, jets  ", 50, 0,5));
  Book( TH1F( "deltar_Tau_nextJet", "#Deltar tau, next jet  ", 50, 0,5));
  Book( TH1F( "deltar_Tau_nextJet_ly", "#Deltar tau, next jet  ", 50, 0,5));
  Book( TH1F( "deltar_Tau1_Jet2", "#Deltar 1.Tau, 2.Jet ", 50, 0,5));
  Book( TH1F( "deltar_Tau1_Jet1", "#Deltar 1.Tau, 1.Jet ",50, 0,5));
  Book( TH1F( "deltar_Tau1_Jet2_ly", "#Deltar 1.Tau, 2.Jet ", 50, 0,5));
  Book( TH1F( "deltar_Tau1_Jet1_ly", "#Deltar 1.Tau, 1.Jet ",50, 0,5));
  Book( TH1F( "deltar_Tau1_Muon1", "#Delta r 1.Tau, 1.Muon ",50, 0,5));
  Book( TH1F( "deltar_Tau1_Muon1_ly", "#Delta r 1.Tau, 1.Muon ",50, 0,5));
  Book( TH1F( "DeltaPhi_Tau_Muon", "#Delta#Phi(first muon, first tau)", 40, 0, 7) );
  Book( TH1F( "DeltaPhi_Tau1_Jet1", "#Delta#Phi(first tau, first jet)", 40, 0, 7) );
  Book( TH1F( "DeltaPhi_Taus_Jets", "#Delta#Phi(taus, jets)", 40, 0, 7) );
  Double_t bins2[5] = {20, 80, 200, 400,800};
  Book( TH1F( "MuonpT1_TaupT1", "p_{T} leading tau + p_{T} leading muon",50, 0,800));
  Book( TH1F( "MuonpT1_TaupT1_ly", "p_{T} leading tau + p_{T} leading muon",50, 0,800));
  Book( TH1F( "MuonpT1_TaupT1_binned", "p_{T} leading tau + p_{T} leading muon [GeV]",4, bins2));
  Book( TH1F( "MuonpT1_TaupT1_binned_ly", "p_{T} leading tau + p_{T} leading muon [GeV]",4, bins2));
  Book( TH1F( "MuonpT1_TaupT1_binned_lx", "p_{T} leading tau + p_{T} leading muon [GeV]",4, bins2));
  Book( TH1F( "MuonpT_TaupT", "p_{T} tau + p_{T} muon [GeV]",50, 0,800));
  Book( TH1F( "MuonpT_TaupT_ly", "p_{T} tau + p_{T} muon [GeV]",50, 0,800));
  Book( TH1F( "MuonpT_TaupT_binned", "p_{T} tau + p_{T} muon [GeV]",4, bins2));
  Book( TH1F( "MuonpT_TaupT_binned_ly", "p_{T} tau + p_{T} muon [GeV]",4, bins2));
  Book( TH1F( "MuonpT_TaupT_binned_lx", "p_{T} tau + p_{T} muon [GeV]",4, bins2));
  Book( TH1F( "max_InvMass", "maximum M(#mu #tau)", 100, 0, 500) );
  Book( TH1F( "max_InvMass_ly", "maximum M(#mu #tau)", 100, 0, 500) );
  Book( TH1F( "max_InvMass_binned", "maximum M(#mu #tau)", 4, bins2) );
  Book( TH1F( "max_InvMass_binned_lx", "maximum M(#mu #tau)", 4, bins2) );
  Book( TH1F( "max_InvMass_binned_ly", "maximum M(#mu #tau)", 4, bins2) );
  Book( TH1F( "max_InvMassTau", "maximum M(#tau #tau)", 100, 0, 500) );
  Book( TH1F( "max_InvMassTau_ly", "maximum M(#tau #tau)", 100, 0, 500) );
  Book( TH1F( "max_InvMassTau_binned", "maximum M(#tau #tau)", 4, bins2) );
  Book( TH1F( "max_InvMassTau_binned_lx", "maximum M(#tau #tau)", 4, bins2) );
  Book( TH1F( "max_InvMassTau_binned_ly", "maximum M(#tau #tau)", 4, bins2) );
  Book( TH1F( "tau_mass", "mass tau", 100, 0, 10) );
  Book( TH1F( "InvMassMuTau", "M(#mu #tau) if N_{#mu} = 1 & N_{#tau} = 1", 100, 0, 500) );
  Book( TH1F( "InvMassMuTau_ly", "M(#mu #tau) if N_{#mu} = 1 & N_{#tau} = 1", 100, 0, 500) );
  Book( TH1F( "InvMassMuTau_binned", "M(#mu #tau) if N_{#mu} = 1 & N_{#tau} = 1", 4, bins2) );
  Book( TH1F( "InvMassMuTau_binned_lx", "M(#mu #tau) if N_{#mu} = 1 & N_{#tau} = 1", 4, bins2) );
  Book( TH1F( "InvMassMuTau_binned_ly", "M(#mu #tau) if N_{#mu} = 1 & N_{#tau} = 1", 4, bins2) ); 
  Book( TH1F( "InvMassTauJet1", "M(#tau first jet)", 100, 0, 500) );
  Book( TH1F( "InvMassTauJet1_ly", "M(#tau first jet)", 100, 0, 500) );
  Book( TH1F( "InvMassTauJet2", "M(#tau second jet)", 100, 0, 500) );
  Book( TH1F( "InvMassTauJet2_ly", "M(#tau second jet)", 100, 0, 500) );
  

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
       Hist("isolation") -> Fill(tau.byMediumCombinedIsolationDeltaBetaCorr(), weight);
      Hist("againstElectron") -> Fill(tau.againstElectronLooseMVA3(), weight);
      Hist("againstMuon") -> Fill(tau.againstMuonLoose2(), weight);
    }
    if (bcc->taus->size() > 0)
    {
      Tau tau = bcc->taus->at(0);
      Hist("pT_1") -> Fill(tau.pt(), weight);
      Hist("pT_1_ly") -> Fill(tau.pt(), weight);
      Hist("pT_1_binned") -> Fill(tau.pt(), weight);
      Hist("pT_1_binned_ly") -> Fill(tau.pt(), weight);
      Hist("pT_1_binned_lx") -> Fill(tau.pt(), weight);
      Hist("eta_1") -> Fill(tau.eta(), weight);
      Hist("eta_1_ly") -> Fill(tau.eta(), weight);
      Hist("phi_1") -> Fill(tau.phi(), weight);
      Hist("phi_1_ly") -> Fill(tau.phi(), weight);
      Hist("decayMode_1") -> Fill(tau.decayModeFinding(), weight);
      Hist("isolation_1") -> Fill(tau.byMediumCombinedIsolationDeltaBetaCorr(), weight);
      Hist("againstElectron_1") -> Fill(tau.againstElectronLooseMVA3(), weight);
      Hist("againstMuon_1") -> Fill(tau.againstMuonLoose2(), weight);

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
	  Hist("isolation_2") -> Fill(tau1.byMediumCombinedIsolationDeltaBetaCorr(), weight);
	  Hist("againstElectron_2") -> Fill(tau.againstElectronLooseMVA3(), weight);
	  Hist("againstMuon_2") -> Fill(tau.againstMuonLoose2(), weight);
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


  for(unsigned int i=0; i< bcc->taus->size(); ++i)
    {
      Tau tau =  bcc->taus->at(i);
      std::vector<Jet> *jets = calc->GetJets();
      double deltaR = deltaRmin(&tau, jets);
      Hist("deltar_Tau_nextJet")-> Fill(deltaR,weight);
      Hist("deltar_Tau_nextJet_ly")-> Fill(deltaR,weight);
      
      for(unsigned int j=0; j< bcc->jets->size(); ++j)
	{
	  Jet jet = bcc->jets->at(j);
	  double deltar_Tau_Jet = jet.deltaR(tau);
	  Hist ("deltar_Tau_Jet") -> Fill(deltar_Tau_Jet,weight);
	  Hist ("deltar_Tau_Jet_ly") -> Fill(deltar_Tau_Jet,weight);
	  double deltaPhi = abs(tau.phi() - jet.phi());
	  Hist("DeltaPhi_Taus_Jets") -> Fill(deltaPhi, weight);
	}
    }

  sort(bcc->jets->begin(), bcc->jets->end(), HigherPt()); 
  sort(bcc->taus->begin(), bcc->taus->end(), HigherPt());
  if (bcc->taus->size() > 0 && bcc->jets->size() > 0 )
    {
      Tau tau = bcc->taus->at(0);
      Jet jet = bcc->jets->at(0);
      TH2F* h2 = (TH2F*)Hist("Jet1_Tau1_pT"); 
      h2->Fill(jet.pt(),tau.pt(),weight);
      double deltar_Tau1_Jet1 = jet.deltaR(tau);
      Hist ("deltar_Tau1_Jet1") -> Fill(deltar_Tau1_Jet1,weight);
      Hist ("deltar_Tau1_Jet1_ly") -> Fill(deltar_Tau1_Jet1,weight);
      double deltaPhi = abs(tau.phi() - jet.phi());
      Hist("DeltaPhi_Tau1_Jet1") -> Fill(deltaPhi, weight);
      if (bcc->taus->size() > 1 )
	{
	  Tau tau1 = bcc->taus->at(1);
	  TH2F* h3 = (TH2F*)Hist("Jet1_Tau2_pT"); 
	  h3->Fill(jet.pt(),tau1.pt(),weight);
	}
    }
  
  if (bcc->taus->size() > 0 && bcc->jets->size() > 0 )
    {
      if (bcc->jets->size() > 1 )
	{
	  Tau tau = bcc->taus->at(0);
	  Jet jet = bcc->jets->at(1);
	  TH2F* h4 = (TH2F*)Hist("Jet2_Tau1_pT"); 
	  h4->Fill(jet.pt(),tau.pt(),weight);
	  double deltar_Tau1_Jet2 = jet.deltaR(tau);
	  Hist ("deltar_Tau1_Jet2") -> Fill(deltar_Tau1_Jet2,weight);
	  Hist ("deltar_Tau1_Jet2_ly") -> Fill(deltar_Tau1_Jet2,weight);
	  if (bcc->taus->size() > 1 )
	    {
	      Tau tau1 = bcc->taus->at(1);
	      TH2F* h5 = (TH2F*)Hist("Jet2_Tau2_pT"); 
	      h5->Fill(jet.pt(),tau1.pt(),weight);
	    }
	}
    }

  

  if (bcc->taus->size() > 0 && bcc->muons->size() > 0 )
    {
	  Tau tau = bcc->taus->at(0);
	  Muon muon = bcc->muons->at(0);
	  double deltar_Tau1_Muon1 = muon.deltaR(tau);
	  Hist ("deltar_Tau1_Muon1") -> Fill(deltar_Tau1_Muon1,weight);
	  Hist ("deltar_Tau1_Muon1_ly") -> Fill(deltar_Tau1_Muon1,weight);
	  Hist("MuonpT1_TaupT1") -> Fill(muon.pt()+tau.pt(),weight);
	  Hist("MuonpT1_TaupT1_ly") -> Fill(muon.pt()+tau.pt(),weight);
	  Hist("MuonpT1_TaupT1_binned") -> Fill(muon.pt()+tau.pt(),weight);
	  Hist("MuonpT1_TaupT1_binned_lx") -> Fill(muon.pt()+tau.pt(),weight);
	  Hist("MuonpT1_TaupT1_binned_ly") -> Fill(muon.pt()+tau.pt(),weight);
	  double deltaPhi = abs(tau.phi() - muon.phi());
	  Hist("DeltaPhi_Tau_Muon") -> Fill(deltaPhi, weight);
    }
  
  for(unsigned int i=0; i< bcc->taus->size(); ++i)
    {
      Tau tau = bcc->taus->at(i);
      for(unsigned int j=0; j< bcc->muons->size(); ++j)
	{
	  Muon muon = bcc->muons->at(j);
	  Hist("MuonpT_TaupT") -> Fill(muon.pt()+tau.pt(),weight);
	  Hist("MuonpT_TaupT_ly") -> Fill(muon.pt()+tau.pt(),weight);
	  Hist("MuonpT_TaupT_binned") -> Fill(muon.pt()+tau.pt(),weight);
	  Hist("MuonpT_TaupT_binned_lx") -> Fill(muon.pt()+tau.pt(),weight);
	  Hist("MuonpT_TaupT_binned_ly") -> Fill(muon.pt()+tau.pt(),weight);
	}
    }

  if (bcc->muons->size() > 0 && bcc->taus->size() > 0)
    {
      double max_InvMass =0;
      for(unsigned int i=0; i< bcc->muons->size(); ++i)
	{
	  Muon muon = bcc->muons->at(i);
	  TLorentzVector Mu;
	  Mu.SetPtEtaPhiE(muon.pt() ,muon.eta() ,muon.phi() ,muon.energy() );
	  for(unsigned int j=0; j< bcc->taus->size(); ++j)
	    {
	      Tau tau = bcc->taus->at(j);
	      TLorentzVector Tau;
	      Tau.SetPtEtaPhiE(tau.pt() ,tau.eta() ,tau.phi() ,tau.energy() );
	      TLorentzVector DiLepton = Tau +Mu;
	      double InvMass = DiLepton.M();
	      if (InvMass > max_InvMass)  max_InvMass = InvMass;
	    }
	}
      Hist("max_InvMass")->Fill(max_InvMass, weight);
      Hist("max_InvMass_ly")->Fill(max_InvMass, weight);
      Hist("max_InvMass_binned")->Fill(max_InvMass, weight);
      Hist("max_InvMass_binned_lx")->Fill(max_InvMass, weight);
      Hist("max_InvMass_binned_ly")->Fill(max_InvMass, weight);
    }
  
   if (bcc->taus->size() > 1)
    {
      double max_InvMass =0;
      for(unsigned int i=0; i< bcc->taus->size(); ++i)
	{
	  Tau tau1 = bcc->taus->at(i);
	  TLorentzVector Tau1;
	  Tau1.SetPtEtaPhiE(tau1.pt() ,tau1.eta() ,tau1.phi() ,tau1.energy() );
	  for(unsigned int j=0; j< bcc->taus->size(); ++j)
	    {
	      Tau tau2 = bcc->taus->at(j);
	      TLorentzVector Tau2;
	      Tau2.SetPtEtaPhiE(tau2.pt() ,tau2.eta() ,tau2.phi() ,tau2.energy() );
	      TLorentzVector DiLepton = Tau1 +Tau2;
	      double InvMass = DiLepton.M();
	      if (InvMass > max_InvMass)  max_InvMass = InvMass;
	    }
	}
      Hist("max_InvMassTau")->Fill(max_InvMass, weight);
      Hist("max_InvMassTau_ly")->Fill(max_InvMass, weight);
      Hist("max_InvMassTau_binned")->Fill(max_InvMass, weight);
      Hist("max_InvMassTau_binned_lx")->Fill(max_InvMass, weight);
      Hist("max_InvMassTau_binned_ly")->Fill(max_InvMass, weight);
    }


  if (bcc->muons->size() == 1 && bcc->taus->size() == 1)
    {
      Muon muon = bcc->muons->at(0);
      TLorentzVector Mu;
      Mu.SetPtEtaPhiE(muon.pt() ,muon.eta() ,muon.phi() ,muon.energy() );
	  
      Tau tau = bcc->taus->at(0);
      TLorentzVector Tau;
      Tau.SetPtEtaPhiE(tau.pt() ,tau.eta() ,tau.phi() ,tau.energy() );
      TLorentzVector DiLepton = Tau +Mu;
      double InvMass = DiLepton.M();
	      
      Hist("InvMassMuTau")->Fill(InvMass, weight);
      Hist("InvMassMuTau_ly")->Fill(InvMass, weight);
      Hist("InvMassMuTau_binned")->Fill(InvMass, weight);
      Hist("InvMassMuTau_binned_lx")->Fill(InvMass, weight);
      Hist("InvMassMuTau_binned_ly")->Fill(InvMass, weight);
    }
  
  if (bcc->taus->size() > 0 && bcc->jets->size() > 0)
    {
      Tau tau = bcc->taus->at(0);
      TLorentzVector Tau;
      Tau.SetPtEtaPhiE(tau.pt() ,tau.eta() ,tau.phi() ,tau.energy() );
      Hist("tau_mass") ->Fill(Tau.M(), weight);
      Jet jet1 = bcc->jets->at(0);
      TLorentzVector Jet1;
      Jet1.SetPtEtaPhiE(jet1.pt() ,jet1.eta() ,jet1.phi() ,jet1.energy() );
      TLorentzVector Jet1Tau = Jet1 +Tau;
      double InvMass = Jet1Tau.M();
	      
      Hist("InvMassTauJet1")->Fill(InvMass, weight);
      Hist("InvMassTauJet1_ly")->Fill(InvMass, weight);

      if (bcc->jets->size() > 1)
	{
	  Jet jet2 = bcc->jets->at(1);
	  TLorentzVector Jet2;
	  Jet2.SetPtEtaPhiE(jet2.pt() ,jet2.eta() ,jet2.phi() ,jet2.energy() );
	  TLorentzVector Jet2Tau = Jet2 +Tau;
	  double InvMass = Jet2Tau.M();
	      
	  Hist("InvMassTauJet2")->Fill(InvMass, weight);
	  Hist("InvMassTauJet2_ly")->Fill(InvMass, weight);
	} 
    }
}

void TauHists::Finish()
{
  // final calculations, like division and addition of certain histograms

 
}



