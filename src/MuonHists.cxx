#include "include/MuonHists.h"
#include "include/ObjectHandler.h"
#include "include/SelectionModules.h"
#include <iostream>

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
    Book( TH1F( "pT","p_{T} muon",100,0,500));
    Book( TH1F( "pT_ly","p_{T} muon",100,0,500));
    Book( TH1F( "eta","#eta muon",100,-3,3));
    Book( TH1F( "eta_ly","#eta muon",100,-3,3));
    Book( TH1F( "phi","#phi muon",100,-PI,PI));
    Book( TH1F( "phi_ly","#phi muon",100,-PI,PI));
    Book( TH1F( "isolation","relIso muon",100,0,0.5));
    Book( TH1F( "isolation_ly","relIso muon",100,0,0.5));
    Book( TH1F( "pT_1"," p_{T} leading muon",100,0,500));
    Book( TH1F( "pT_1_ly"," p_{T} leading muon",100,0,500));
    Book( TH1F( "pT_2","p_{T} 2nd muon",100,0,500));
    Book( TH1F( "pT_2_ly","p_{T} 2nd muon",100,0,500));
    Book( TH1F( "eta_1","#eta leading muon",100,-3,3));
    Book( TH1F( "eta_1_ly","#eta leading muon",100,-3,3));
    Book( TH1F( "eta_2","#eta 2nd muon",100,-3,3));
    Book( TH1F( "eta_2_ly","#eta 2nd muon",100,-3,3));
    Book( TH1F( "phi_1","#phi leading muon",100,-PI,PI));
    Book( TH1F( "phi_1_ly","#phi leading muon",100,-PI,PI));
    Book( TH1F( "phi_2","#phi 2nd muon",100,-PI,PI));
    Book( TH1F( "phi_2_ly","#phi 2nd muon",100,-PI,PI));
    Book( TH1F( "isolation_1","relIso leading  muon",100,0,0.5));
    Book( TH1F( "isolation_1_ly","relIso leading muon",100,0,0.5));
    Book( TH1F( "isolation_2","relIso 2nd muon",100,0,0.5));
    Book( TH1F( "isolation_2_ly","relIso 2nd muon",100,0,0.5));

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

    Book( TH1F( "charge","charge muon",3,-1.5,1.5));
    Book( TH1F( "charge_1","charge leading muon",3,-1.5,1.5));
    Book( TH1F( "charge_2","charge 2nd muon",3,-1.5,1.5));
}


void MuonHists::Fill()
{
    // important: get the event weight
    EventCalc* calc = EventCalc::Instance();
    double weight = calc -> GetWeight();

    ObjectHandler* objs = ObjectHandler::Instance();
    BaseCycleContainer* bcc = objs->GetBaseCycleContainer();

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
}


void MuonHists::Finish()
{
    // final calculations, like division and addition of certain histograms
}

