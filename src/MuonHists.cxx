#include "include/MuonHists.h"
#include "include/SelectionModules.h"
#include <iostream>

#include "TH1F.h"
#include "TH2F.h"

using namespace std;

MuonHists::MuonHists(const char* name) : BaseHists(name)
{
    // named default constructor
}

MuonHists::~MuonHists()
{
    // default destructor, does nothing
}

void MuonHists::Init()
{
    // book all histograms here

  Book( TH1F( "number","number of muons",5,-0.5,4.5));
  Book( TH1F( "number_ly","number of muons",5,-0.5,4.5));
  Book( TH1F( "pT","p_{T} muon [GeV]",100,0,500));
  Book( TH1F( "pT_ly","p_{T} muon [GeV]",100,0,500));
  Book( TH1F( "eta","#eta muon",100,-3,3));
  Book( TH1F( "eta_ly","#eta muon",100,-3,3));
  Book( TH1F( "phi","#phi muon",100,-M_PI,M_PI));
  Book( TH1F( "phi_ly","#phi muon",100,-M_PI,M_PI));
  Book( TH1F( "isolation","relIso muon",100,0,0.5));
  Book( TH1F( "isolation_ly","relIso muon",100,0,0.5));
  Book( TH1F( "pT_1"," p_{T} leading muon [GeV]",160,0,800));
  Book( TH1F( "pT_1_ly"," p_{T} leading muon [GeV]",160,0,800));
  Book( TH1F( "pT_2","p_{T} 2nd muon [GeV]",100,0,500));
  Book( TH1F( "pT_2_ly","p_{T} 2nd muon [GeV]",100,0,500));
  Book( TH1F( "eta_1","#eta leading muon",100,-3,3));
  Book( TH1F( "eta_1_ly","#eta leading muon",100,-3,3));
  Book( TH1F( "eta_2","#eta 2nd muon",100,-3,3));
  Book( TH1F( "eta_2_ly","#eta 2nd muon",100,-3,3));
  Book( TH1F( "phi_1","#phi leading muon",100,-M_PI,M_PI));
  Book( TH1F( "phi_1_ly","#phi leading muon",100,-M_PI,M_PI));
  Book( TH1F( "phi_2","#phi 2nd muon",100,-M_PI,M_PI));
  Book( TH1F( "phi_2_ly","#phi 2nd muon",100,-M_PI,M_PI));
  Book( TH1F( "isolation_1","relIso leading  muon",100,0,0.5));
  Book( TH1F( "isolation_1_ly","relIso leading muon",100,0,0.5));
  Book( TH1F( "isolation_2","relIso 2nd muon",100,0,0.5));
  Book( TH1F( "isolation_2_ly","relIso 2nd muon",100,0,0.5));
  Book( TH1F( "deltar_Muon_Jet", "#Deltar muons, jets  ", 50, 0,5));
  Book( TH1F( "deltar_Muon_Jet_ly", "#Deltar muons, jets  ", 50, 0,5));
  Book( TH1F( "deltar_Muon_nextJet", "#Deltar muon, next jet  ", 50, 0,5));
  Book( TH1F( "deltar_Muon_nextJet_ly", "#Deltar muon, next jet  ", 50, 0,5));
  Book( TH1F( "deltar_Muon1_Jet2", "#Deltar 1.Muon, 2.Jet ", 50, 0,5));
  Book( TH1F( "deltar_Muon1_Jet1", "#Deltar 1.Muon, 1.Jet ",50, 0,5));
  Book( TH1F( "deltar_Muon1_Jet2_ly", "#Deltar 1.Muon, 2.Jet ", 50, 0,5));
  Book( TH1F( "deltar_Muon1_Jet1_ly", "#Deltar 1.Muon, 1.Jet ",50, 0,5));
  Book( TH1F( "deltar_Muon1_Muon1", "#Delta r 1.Muon, 1.Muon ",50, 0,5));
  Book( TH1F( "deltar_Muon1_Muon1_ly", "#Delta r 1.Muon, 1.Muon ",50, 0,5));
  Book( TH1F( "DeltaPhi_Muon1_Jet1", "#Delta#Phi(first tau, first jet)", 40, 0, 7) );
  Book( TH1F( "DeltaPhi_Muons_Jets", "#Delta#Phi(taus, jets)", 40, 0, 7) );
  Book( TH1F( "ptrel", "p_{T}^{rel}(#mu,jet)", 40, 0, 200.) );
  Book( TH1F( "deltaRmin", "#Delta R_{min}(#mu,jet)", 40, 0, 2.0) );
  Book( TH1F( "ptrel_ly", "p_{T}^{rel}(#mu,jet)", 40, 0, 200.) );
  Book( TH1F( "deltaRmin_ly", "#Delta R_{min}(#mu,jet)", 40, 0, 2.0) );
  Book( TH2F( "deltaRmin_vs_ptrel", "#Delta R_{min}(#mu,jet) vs p_{T}^{rel}(#mu,jet)", 40, 0, 2.0, 40, 0, 200.) );
  Book( TH1F( "ptrel_1", "p_{T}^{rel}(leading #mu,jet)", 40, 0, 200.) );
  Book( TH1F( "deltaRmin_1", "#Delta R_{min}(leading #mu,jet)", 40, 0, 2.0) );
  Book( TH1F( "ptrel_1_ly", "p_{T}^{rel}(leading #mu,jet)", 40, 0, 200.) );
  Book( TH1F( "deltaRmin_1_ly", "#Delta R_{min}(leading #mu,jet)", 40, 0, 2.0) );
  Book( TH2F( "deltaRmin_vs_ptrel_1", "#Delta R_{min}(leading #mu,jet) vs p_{T}^{rel}(leading #mu,jet)", 40, 0, 2.0, 40, 0, 200.) );
  Book( TH1F( "ptrel_2", "p_{T}^{rel}(2nd #mu,jet)", 40, 0, 200.) );
  Book( TH1F( "deltaRmin_2", "#Delta R_{min}(2nd #mu,jet)", 40, 0, 2.0) );
  Book( TH1F( "ptrel_2_ly", "p_{T}^{rel}(2nd #mu,jet)", 40, 0, 200.) );
  Book( TH1F( "deltaRmin_2_ly", "#Delta R_{min}(2nd #mu,jet)", 40, 0, 2.0) );
  Book( TH2F( "deltaRmin_vs_ptrel_2", "#Delta R_{min}(2nd #mu,jet) vs p_{T}^{rel}(2nd #mu,jet)", 40, 0, 2.0, 40, 0, 200.) );
  Book( TH1F( "MaxInvMass", "maximum M(#mu #mu)", 100, 0, 500) );
  Book( TH1F( "MaxInvMass_ly", "maximun M(#mu #mu)", 100, 0, 500) );
  Book( TH1F( "InvMass", "M(#mu #mu)", 100, 0, 500) );
  Book( TH1F( "InvMass_ly", "M(#mu #mu)", 100, 0, 500) );
  Book( TH1F( "InvMassMuJet1", "M(#mu first jet)", 100, 0, 500) );
  Book( TH1F( "InvMassMuJet1_ly", "M(#mu first jet)", 100, 0, 500) );
  Book( TH1F( "InvMassMuJet2", "M(#mu second jet)", 100, 0, 500) );
  Book( TH1F( "InvMassMuJet2_ly", "M(#mu second jet)", 100, 0, 500) );
  Book( TH1F( "charge","charge muon",3,-1.5,1.5));
  Book( TH1F( "charge_1","charge leading muon",3,-1.5,1.5));
  Book( TH1F( "charge_2","charge 2nd muon",3,-1.5,1.5));
}

void MuonHists::Scale(double scale)
{
    Hist("number")->Scale(scale);
    Hist("number_ly")->Scale(scale);
    Hist("pT")->Scale(scale);
    Hist("pT_ly")->Scale(scale);
    Hist("eta")->Scale(scale);
    Hist("eta_ly")->Scale(scale);
    Hist("phi")->Scale(scale);
    Hist("phi_ly")->Scale(scale);
    Hist("isolation")->Scale(scale);
    Hist("isolation_ly")->Scale(scale);
    Hist("pT_1")->Scale(scale);
    Hist("pT_1_ly")->Scale(scale);
    Hist("pT_2")->Scale(scale);
    Hist("pT_2_ly")->Scale(scale);
    Hist("eta_1")->Scale(scale);
    Hist("eta_1_ly")->Scale(scale);
    Hist("eta_2")->Scale(scale);
    Hist("eta_2_ly")->Scale(scale);
    Hist("phi_1")->Scale(scale);
    Hist("phi_1_ly")->Scale(scale);
    Hist("phi_2")->Scale(scale);
    Hist("phi_2_ly")->Scale(scale);
    Hist("isolation_1")->Scale(scale);
    Hist("isolation_1_ly")->Scale(scale);
    Hist("isolation_2")->Scale(scale);
    Hist("isolation_2_ly")->Scale(scale);
    Hist("deltar_Muon_Jet")->Scale(scale);
    Hist("deltar_Muon_Jet_ly")->Scale(scale);
    Hist("deltar_Muon_nextJet")->Scale(scale);
    Hist("deltar_Muon_nextJet_ly")->Scale(scale);
    Hist("deltar_Muon1_Jet2")->Scale(scale);
    Hist("deltar_Muon1_Jet1")->Scale(scale);
    Hist("deltar_Muon1_Jet2_ly")->Scale(scale);
    Hist("deltar_Muon1_Jet1_ly")->Scale(scale);
    Hist("deltar_Muon1_Muon1")->Scale(scale);
    Hist("deltar_Muon1_Muon1_ly")->Scale(scale);
    Hist("DeltaPhi_Muon1_Jet1")->Scale(scale);
    Hist("DeltaPhi_Muons_Jets")->Scale(scale);
    Hist("ptrel")->Scale(scale);
    Hist("deltaRmin")->Scale(scale);
    Hist("ptrel_ly")->Scale(scale);
    Hist("deltaRmin_ly")->Scale(scale);
    Hist("deltaRmin_vs_ptrel")->Scale(scale);
    Hist("ptrel_1")->Scale(scale);
    Hist("deltaRmin_1")->Scale(scale);
    Hist("ptrel_1_ly")->Scale(scale);
    Hist("deltaRmin_1_ly")->Scale(scale);
    Hist("deltaRmin_vs_ptrel_1")->Scale(scale);
    Hist("ptrel_2")->Scale(scale);
    Hist("deltaRmin_2")->Scale(scale);
    Hist("ptrel_2_ly")->Scale(scale);
    Hist("deltaRmin_2_ly")->Scale(scale);
    Hist("deltaRmin_vs_ptrel_2")->Scale(scale);
    Hist("MaxInvMass")->Scale(scale);
    Hist("MaxInvMass_ly")->Scale(scale);
    Hist("InvMass")->Scale(scale);
    Hist("InvMass_ly")->Scale(scale);
    Hist("InvMassMuJet1")->Scale(scale);
    Hist("InvMassMuJet1_ly")->Scale(scale);
    Hist("InvMassMuJet2")->Scale(scale);
    Hist("InvMassMuJet2_ly")->Scale(scale);
    Hist("charge")->Scale(scale);
    Hist("charge_1")->Scale(scale);
    Hist("charge_2")->Scale(scale);
}
void MuonHists::Fill()
{
    // important: get the event weight
    EventCalc* calc = EventCalc::Instance();
    double weight = calc -> GetWeight();

    BaseCycleContainer* bcc = calc->GetBaseCycleContainer();

    int NMuons = bcc->muons->size();
    Hist("number")-> Fill(NMuons,weight);
    Hist("number_ly")-> Fill(NMuons,weight);

    for(unsigned int i=0; i< bcc->muons->size(); ++i) {
        Muon muon  = bcc->muons->at(i);
        Hist("pT")-> Fill(muon.pt(),weight);
        Hist("pT_ly")-> Fill(muon.pt(),weight);
        Hist("eta") -> Fill(muon.eta(),weight);
        Hist("eta_ly") -> Fill(muon.eta(),weight);
        Hist("phi") -> Fill(muon.phi(),weight);
        Hist("phi_ly") -> Fill(muon.phi(),weight);
        Hist("isolation")->Fill(muon.relIso(),weight);
        Hist("isolation_ly")->Fill(muon.relIso(),weight);
        Hist("charge")->Fill(muon.charge(),weight);

        std::vector<Jet>* jets = calc->GetJets();
        if(jets) {
            Hist("ptrel")->Fill( pTrel(&muon, jets), weight);
            Hist("deltaRmin")->Fill( deltaRmin(&muon, jets), weight);
            Hist("ptrel_ly")->Fill( pTrel(&muon, jets), weight);
            Hist("deltaRmin_ly")->Fill( deltaRmin(&muon, jets), weight);

            TH2F* h = (TH2F*) Hist("deltaRmin_vs_ptrel");
            h->Fill(deltaRmin(&muon, jets), pTrel(&muon, jets), weight);
        }

    }
    sort(bcc->muons->begin(), bcc->muons->end(), HigherPt());
    for (unsigned int i =0; i<=1; ++i) {
        if (bcc->muons->size()> i) {
            Muon muon =  bcc->muons->at(i);
            TString hname = TString::Format("pT_%d", i+1);
            Hist(hname)->Fill(muon.pt(),weight);
            TString hname_ly = TString::Format("pT_%d_ly", i+1);
            Hist(hname_ly)->Fill(muon.pt(),weight);
            TString hname_eta = TString::Format("eta_%d", i+1);
            Hist(hname_eta)->Fill(muon.eta(),weight);
            TString hname_eta_ly = TString::Format("eta_%d_ly", i+1);
            Hist(hname_eta_ly)->Fill(muon.eta(),weight);
            TString hname_phi = TString::Format("phi_%d", i+1);
            Hist(hname_phi)->Fill(muon.phi(),weight);
            TString hname_phi_ly = TString::Format("phi_%d_ly", i+1);
            Hist(hname_phi_ly)->Fill(muon.phi(),weight);
            TString hname_iso = TString::Format("isolation_%d", i+1);
            Hist(hname_iso)->Fill(muon.relIso(),weight);
            TString hname_iso_ly = TString::Format("isolation_%d_ly", i+1);
            Hist(hname_iso_ly)->Fill(muon.relIso(),weight);
            TString hname_charge = TString::Format("charge_%d", i+1);
            Hist(hname_charge)->Fill(muon.charge(),weight);

            std::vector<Jet>* jets = calc->GetJets();
            if(jets) {
                TString hname_ptrel = TString::Format("ptrel_%d", i+1);
                Hist(hname_ptrel)->Fill( pTrel(&muon, jets), weight);
                TString hname_deltaRmin = TString::Format("deltaRmin_%d", i+1);
                Hist(hname_deltaRmin)->Fill( deltaRmin(&muon, jets), weight);
                TString hname_ptrel_ly = TString::Format("ptrel_%d_ly", i+1);
                Hist(hname_ptrel_ly)->Fill( pTrel(&muon, jets), weight);
                TString hname_deltaRmin_ly = TString::Format("deltaRmin_%d_ly", i+1);
                Hist(hname_deltaRmin_ly)->Fill( deltaRmin(&muon, jets), weight);

                TString hname_deltaRmin_vs_ptrel = TString::Format("deltaRmin_vs_ptrel_%d", i+1);
                TH2F* h = (TH2F*) Hist(hname_deltaRmin_vs_ptrel);
                h->Fill(deltaRmin(&muon, jets), pTrel(&muon, jets), weight);
            }
        }
    }



if (bcc->muons->size() > 1)
    {
      double max_InvMass =0;
      for(unsigned int i=0; i< bcc->muons->size(); ++i)
	{
	  Muon muon1 = bcc->muons->at(i);
	  TLorentzVector Mu1;
	  Mu1.SetPtEtaPhiE(muon1.pt() ,muon1.eta() ,muon1.phi() ,muon1.energy() );
	  for(unsigned int j=0; j< bcc->muons->size(); ++j)
	    {
	      if (i!=j)
		{
		  Muon muon2 = bcc->muons->at(j);
		  TLorentzVector Mu2;
		  Mu2.SetPtEtaPhiE(muon2.pt() ,muon2.eta() ,muon2.phi() ,muon2.energy() );
		  TLorentzVector DiLepton = Mu1 +Mu2;
		  double InvMass = DiLepton.M();
		  if (InvMass > max_InvMass)  max_InvMass = InvMass;
		}
	    }
	}
      Hist("MaxInvMass")->Fill(max_InvMass, weight);
      Hist("MaxInvMass_ly")->Fill(max_InvMass, weight);
    }

if (bcc->muons->size() > 1)
    {
      for(unsigned int i=0; i< bcc->muons->size(); ++i)
	{
	  Muon muon1 = bcc->muons->at(i);
	  TLorentzVector Mu1;
	  Mu1.SetPtEtaPhiE(muon1.pt() ,muon1.eta() ,muon1.phi() ,muon1.energy() );
	  for(unsigned int j=0; j< bcc->muons->size(); ++j)
	    {
	      if (i!=j)
		{
		  Muon muon2 = bcc->muons->at(j);
		  TLorentzVector Mu2;
		  Mu2.SetPtEtaPhiE(muon2.pt() ,muon2.eta() ,muon2.phi() ,muon2.energy() );
		  TLorentzVector DiLepton = Mu1 +Mu2;
		  double InvMass = DiLepton.M();
		  Hist("InvMass")->Fill(InvMass, weight);
		  Hist("InvMass_ly")->Fill(InvMass, weight);
		}
	    }
	}
    }

for(unsigned int i=0; i< bcc->muons->size(); ++i)
    {
      Muon muon =  bcc->muons->at(i);
      std::vector<Jet> *jets = calc->GetJets();
      double deltaR = deltaRmin(&muon, jets);
      Hist("deltar_Muon_nextJet")-> Fill(deltaR,weight);
      Hist("deltar_Muon_nextJet_ly")-> Fill(deltaR,weight);

      for(unsigned int j=0; j< bcc->jets->size(); ++j)
	{
	  Jet jet = bcc->jets->at(j);
	  double deltar_Muon_Jet = jet.deltaR(muon);
	  Hist ("deltar_Muon_Jet") -> Fill(deltar_Muon_Jet,weight);
	  Hist ("deltar_Muon_Jet_ly") -> Fill(deltar_Muon_Jet,weight);
	  double deltaPhi = abs(muon.phi() - jet.phi());
	  Hist("DeltaPhi_Muons_Jets") -> Fill(deltaPhi, weight);
	}
    }

  sort(bcc->jets->begin(), bcc->jets->end(), HigherPt());
  sort(bcc->muons->begin(), bcc->muons->end(), HigherPt());
  if (bcc->muons->size() > 0 && bcc->jets->size() > 0 )
    {
      Muon muon = bcc->muons->at(0);
      Jet jet = bcc->jets->at(0);
      double deltar_Muon1_Jet1 = jet.deltaR(muon);
      Hist ("deltar_Muon1_Jet1") -> Fill(deltar_Muon1_Jet1,weight);
      Hist ("deltar_Muon1_Jet1_ly") -> Fill(deltar_Muon1_Jet1,weight);
      double deltaPhi = abs(muon.phi() - jet.phi());
      Hist("DeltaPhi_Muon1_Jet1") -> Fill(deltaPhi, weight);
      if (bcc->jets->size() > 1 )
	{
	  Muon muon = bcc->muons->at(0);
	  Jet jet = bcc->jets->at(1);
	  double deltar_Muon1_Jet2 = jet.deltaR(muon);
	  Hist ("deltar_Muon1_Jet2") -> Fill(deltar_Muon1_Jet2,weight);
	  Hist ("deltar_Muon1_Jet2_ly") -> Fill(deltar_Muon1_Jet2,weight);
	}
    }

  if (bcc->muons->size() > 0 && bcc->jets->size() > 0)
    {
      Muon muon = bcc->muons->at(0);
      TLorentzVector Mu;
      Mu.SetPtEtaPhiE(muon.pt() ,muon.eta() ,muon.phi() ,muon.energy() );

      Jet jet1 = bcc->jets->at(0);
      TLorentzVector Jet1;
      Jet1.SetPtEtaPhiE(jet1.pt() ,jet1.eta() ,jet1.phi() ,jet1.energy() );
      TLorentzVector Jet1Mu = Jet1 +Mu;
      double InvMass = Jet1Mu.M();

      Hist("InvMassMuJet1")->Fill(InvMass, weight);
      Hist("InvMassMuJet1_ly")->Fill(InvMass, weight);

      if (bcc->jets->size() > 1)
	{
	  Jet jet2 = bcc->jets->at(1);
	  TLorentzVector Jet2;
	  Jet2.SetPtEtaPhiE(jet2.pt() ,jet2.eta() ,jet2.phi() ,jet2.energy() );
	  TLorentzVector Jet2Mu = Jet2 +Mu;
	  double InvMass = Jet2Mu.M();

	  Hist("InvMassMuJet2")->Fill(InvMass, weight);
	  Hist("InvMassMuJet2_ly")->Fill(InvMass, weight);
	}

    }


}

