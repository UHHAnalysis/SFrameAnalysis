#include "include/ElectronHists.h"
#include "include/ObjectHandler.h"
#include "include/SelectionModules.h"
#include <iostream>

using namespace std;

ElectronHists::ElectronHists(const char* name) : BaseHists(name)
{
    // named default constructor

}

ElectronHists::~ElectronHists()
{
    // default destructor, does nothing
}

void ElectronHists::Init()
{
    // book all histograms here
    Book( TH1F( "number","number of electrons",7,-0.5,6.5));
    Book( TH1F( "number_ly","number of electrons",7,-0.5,6.5));
    Book( TH1F( "pT","p_{T} electron",100,0,500));
    Book( TH1F( "pT_ly","p_{T} electron",100,0,500));
    Book( TH1F( "eta","#eta electron",100,-3,3));
    Book( TH1F( "eta_ly","#eta electron",100,-3,3));
    Book( TH1F( "phi","#phi electron",100,-PI,PI));
    Book( TH1F( "phi_ly","#phi electron",100,-PI,PI));
    Book( TH1F( "isolation","relIso electron",100,0,0.5));
    Book( TH1F( "isolation_ly","relIso electron",100,0,0.5));
    Book( TH1F( "pT_1"," p_{T} leading electron",100,0,500));
    Book( TH1F( "pT_1_ly"," p_{T} leading electron",100,0,500));
    Book( TH1F( "pT_2","p_{T} 2nd electron",100,0,500));
    Book( TH1F( "pT_2_ly","p_{T} 2nd electron",100,0,500));
    Book( TH1F( "eta_1","#eta leading electron",100,-3,3));
    Book( TH1F( "eta_1_ly","#eta leading electron",100,-3,3));
    Book( TH1F( "eta_2","#eta 2nd electron",100,-3,3));
    Book( TH1F( "eta_2_ly","#eta 2nd electron",100,-3,3));
    Book( TH1F( "phi_1","#phi leading electron",100,-PI,PI));
    Book( TH1F( "phi_1_ly","#phi leading electron",100,-PI,PI));
    Book( TH1F( "phi_2","#phi 2nd electron",100,-PI,PI));
    Book( TH1F( "phi_2_ly","#phi 2nd electron",100,-PI,PI));
    Book( TH1F( "isolation_1","relIso leading electron",100,0,0.5));
    Book( TH1F( "isolation_1_ly","relIso leading electron",100,0,0.5));
    Book( TH1F( "isolation_2","relIso 2nd electron",100,0,0.5));
    Book( TH1F( "isolation_2_ly","relIso 2nd electron",100,0,0.5));

    Book( TH1F( "ptrel", "p_{T}^{rel}(e,jet)", 40, 0, 200.) );
    Book( TH1F( "deltaRmin", "#Delta R_{min}(e,jet)", 40, 0, 2.0) );
    Book( TH1F( "ptrel_ly", "p_{T}^{rel}(e,jet)", 40, 0, 200.) );
    Book( TH1F( "deltaRmin_ly", "#Delta R_{min}(e,jet)", 40, 0, 2.0) );
    Book( TH2F( "deltaRmin_vs_ptrel", "#Delta R_{min}(e,jet) vs p_{T}^{rel}(e,jet)", 40, 0, 2.0, 40, 0, 200.) );
    Book( TH1F( "ptrel_1", "p_{T}^{rel}(leading e,jet)", 40, 0, 200.) );
    Book( TH1F( "deltaRmin_1", "#Delta R_{min}(leading e,jet)", 40, 0, 2.0) );
    Book( TH1F( "ptrel_1_ly", "p_{T}^{rel}(leading e,jet)", 40, 0, 200.) );
    Book( TH1F( "deltaRmin_1_ly", "#Delta R_{min}(leading e,jet)", 40, 0, 2.0) );
    Book( TH2F( "deltaRmin_vs_ptrel_1", "#Delta R_{min}(leading e,jet) vs p_{T}^{rel}(leading e,jet)", 40, 0, 2.0, 40, 0, 200.) );
    Book( TH1F( "ptrel_2", "p_{T}^{rel}(2nd e,jet)", 40, 0, 200.) );
    Book( TH1F( "deltaRmin_2", "#Delta R_{min}(2nd e,jet)", 40, 0, 2.0) );
    Book( TH1F( "ptrel_2_ly", "p_{T}^{rel}(2nd e,jet)", 40, 0, 200.) );
    Book( TH1F( "deltaRmin_2_ly", "#Delta R_{min}(2nd e,jet)", 40, 0, 2.0) );
    Book( TH2F( "deltaRmin_vs_ptrel_2", "#Delta R_{min}(2nd e,jet) vs p_{T}^{rel}(2nd e,jet)", 40, 0, 2.0, 40, 0, 200.) );

    Book( TH1F( "charge","charge electron",3,-1.5,1.5));
    Book( TH1F( "charge_1","charge leading electron",3,-1.5,1.5));
    Book( TH1F( "charge_2","charge 2nd electron",3,-1.5,1.5));
}


void ElectronHists::Fill()
{
    // important: get the event weight
    EventCalc* calc = EventCalc::Instance();
    double weight = calc -> GetWeight();


    ObjectHandler* objs = ObjectHandler::Instance();
    BaseCycleContainer* bcc = objs->GetBaseCycleContainer();
    int NElectrons = bcc->electrons->size();
    Hist("number")-> Fill(NElectrons,weight);
    Hist("number_ly")-> Fill(NElectrons,weight);
    for(unsigned int i=0; i< bcc->electrons->size(); ++i) {
        Electron electron  = bcc->electrons->at(i);
        Hist("pT")-> Fill(electron.pt(),weight);
        Hist("pT_ly")-> Fill(electron.pt(),weight);
        Hist("eta") -> Fill(electron.eta(),weight);
        Hist("eta_ly") -> Fill(electron.eta(),weight);
        Hist("phi") -> Fill(electron.phi(),weight);
        Hist("phi_ly") -> Fill(electron.phi(),weight);
        Hist("isolation")->Fill(electron.relIso(),weight);
        Hist("isolation_ly")->Fill(electron.relIso(),weight);
        Hist("charge")->Fill(electron.charge(),weight);

        std::vector<Jet>* jets = calc->GetJets();
        if(jets) {
            Hist("ptrel")->Fill( pTrel(&electron, jets), weight);
            Hist("deltaRmin")->Fill( deltaRmin(&electron, jets), weight);
            Hist("ptrel_ly")->Fill( pTrel(&electron, jets), weight);
            Hist("deltaRmin_ly")->Fill( deltaRmin(&electron, jets), weight);
            TH2F* h = (TH2F*) Hist("deltaRmin_vs_ptrel");
            h->Fill(deltaRmin(&electron, jets), pTrel(&electron, jets), weight);
        }
    }
    sort(bcc->electrons->begin(), bcc->electrons->end(), HigherPt());
    for (unsigned int i =0; i<=1; ++i) {
        if (bcc->electrons->size()> i) {
            Electron electron =  bcc->electrons->at(i);
            TString hname = TString::Format("pT_%d", i+1);
            Hist(hname)->Fill(electron.pt(),weight);
            TString hname_ly = TString::Format("pT_%d_ly", i+1);
            Hist(hname_ly)->Fill(electron.pt(),weight);
            TString hname_eta = TString::Format("eta_%d", i+1);
            Hist(hname_eta)->Fill(electron.eta(),weight);
            TString hname_eta_ly = TString::Format("eta_%d_ly", i+1);
            Hist(hname_eta_ly)->Fill(electron.eta(),weight);
            TString hname_phi = TString::Format("phi_%d", i+1);
            Hist(hname_phi)->Fill(electron.phi(),weight);
            TString hname_phi_ly = TString::Format("phi_%d_ly", i+1);
            Hist(hname_phi_ly)->Fill(electron.phi(),weight);
            TString hname_iso = TString::Format("isolation_%d", i+1);
            Hist(hname_iso)->Fill(electron.relIso(),weight);
            TString hname_iso_ly = TString::Format("isolation_%d_ly", i+1);
            Hist(hname_iso_ly)->Fill(electron.relIso(),weight);
            std::vector<Jet>* jets = calc->GetJets();
            if(jets) {
                TString hname_ptrel = TString::Format("ptrel_%d", i+1);
                Hist(hname_ptrel)->Fill( pTrel(&electron, jets), weight);
                TString hname_deltaRmin = TString::Format("deltaRmin_%d", i+1);
                Hist(hname_deltaRmin)->Fill( deltaRmin(&electron, jets), weight);
                TString hname_ptrel_ly = TString::Format("ptrel_%d_ly", i+1);
                Hist(hname_ptrel_ly)->Fill( pTrel(&electron, jets), weight);
                TString hname_deltaRmin_ly = TString::Format("deltaRmin_%d_ly", i+1);
                Hist(hname_deltaRmin_ly)->Fill( deltaRmin(&electron, jets), weight);
                TString hname_deltaRmin_vs_ptrel = TString::Format("deltaRmin_vs_ptrel_%d", i+1);
                TH2F* h = (TH2F*) Hist(hname_deltaRmin_vs_ptrel);
                h->Fill(deltaRmin(&electron, jets), pTrel(&electron, jets), weight);
            }
        }
    }
}


void ElectronHists::Finish()
{
    // final calculations, like division and addition of certain histograms
}

