#include "include/JetHists.h"
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
  
  Book( TH1F( "NJets30", "number of jets with p_{T} > 30 GeV", 15, -0.5, 14.5 ) );
  Book( TH1F( "NJets30_ly", "number of jets with p_{T} > 30 GeV", 15, -0.5, 14.5 ) );
  
  Book( TH1F( "NJets50", "number of jets with p_{T} > 50 GeV", 10, -0.5, 9.5 ) );
  Book( TH1F( "NJets50_ly", "number of jets with p_{T} > 50 GeV", 10, -0.5, 9.5 ) );
  
  Book( TH1F( "NJets100", "number of jets with p_{T} > 100 GeV", 8, -0.5, 7.5 ) );
  Book( TH1F( "NJets100_ly", "number of jets with p_{T} > 100 GeV", 8, -0.5, 7.5 ) );
  
  double* logPtjet_bins  = MakeLogBinning(50, 30, 1500);
  double* logPtjet1_bins = MakeLogBinning(50, 100, 1500);
  double* logPtjet2_bins = MakeLogBinning(50, 30, 1000);
  double* logPtjet3_bins = MakeLogBinning(50, 20, 500);
  double* logPtjet4_bins = MakeLogBinning(50, 20, 250); 
  
  double* logMjet1_bins = MakeLogBinning(50, 0, 300);
  double* logMjet2_bins = MakeLogBinning(50, 0, 300);
  double* logMjet3_bins = MakeLogBinning(50, 0, 300);
  double* logMjet4_bins = MakeLogBinning(50, 0, 300);
  
  Book( TH1F( "pt_lx"," p_{T} all jets", 50, logPtjet_bins));
  Book( TH1F( "pt_lxy"," p_{T} all jets", 50, logPtjet_bins));

  Book( TH1F( "eta","#eta all jets", 50,-3,3));
  Book( TH1F( "phi","#phi all jets", 50, -M_PI, M_PI));

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
  Book( TH1F( "phi_1","#phi leading jet",100,-M_PI,M_PI));
  Book( TH1F( "phi_2","#phi 2nd jet",100,-M_PI,M_PI));
  Book( TH1F( "phi_3","#phi 3rd jet",100,-M_PI,M_PI));
  Book( TH1F( "phi_4","#phi 4th jet",100,-M_PI,M_PI));
  
  // jet mass
  Book( TH1F( "m_1", "M^{jet 1} [GeV/c^{2}]", 100, 0, 300) );
  Book( TH1F( "m_2", "M^{jet 2} [GeV/c^{2}]", 100, 0, 300) );
  Book( TH1F( "m_3", "M^{jet 3} [GeV/c^{2}]", 100, 0, 300) );
  Book( TH1F( "m_4", "M^{jet 4} [GeV/c^{2}]", 100, 0, 300) );
  
    // Delta_R
  Book( TH1F( "deltaRmin_1", "#Delta R_{min}(first jet, nearest jet)", 40, 0, 2.0) );
  Book( TH1F( "deltaRmin_1_ly", "#Delta R_{min}(first jet, nearest jet)", 40, 0, 2.0) );
  Book( TH1F( "deltaRmin_2", "#Delta R_{min}(2nd jet, nearest jet)", 40, 0, 2.0) );
  Book( TH1F( "deltaRmin_2_ly", "#Delta R_{min}(2nd jet, nearest jet)", 40, 0, 2.0) );
  
 // Delta_Phi
  Book( TH1F( "DeltaPhi_Jet1_Jet2", "#Delta#Phi(first jet, second jet)", 40, 0, 7) );

  // b-jets
  Book( TH1F( "NbJets", "number of bJets", 10, -0.5, 9.5 ) );
  Book( TH1F( "NbJets_ly", "number of bJets", 8, -0.5, 7.5 ) );
  
  Book( TH1F( "pt_bJet_1_lx"," p_{T} leading b-jet", 50, logPtjet1_bins));
  Book( TH1F( "pt_bJet_1_lxy"," p_{T} leading b-jet", 50, logPtjet1_bins));
  Book( TH1F( "pt_bJet_2_lx"," p_{T} 2nd bJet", 50, logPtjet1_bins));
  Book( TH1F( "pt_bJet_2_lxy"," p_{T} 2nd bJet", 50, logPtjet1_bins));
  
  Book( TH1F( "eta_bJet_1"," #eta leading bJet",100,-3,3));
  Book( TH1F( "eta_bJet_2"," #eta 2nd bJet",100,-3,3));
  Book( TH1F( "phi_bJet_1"," #phi leading bJet",100,-M_PI,M_PI));
  Book( TH1F( "phi_bJet_2"," #phi 2nd bJet",100,-M_PI,M_PI));
  Book( TH1F( "bjet_tag","index of b jet",4,0.5,4.5));

  Book( TH1F( "InvMassJet1Jet2", "M(first jet second jet)", 100, 0, 500) );
  Book( TH1F( "InvMassJet1Jet2_ly", "M(first jet second jet)", 100, 0, 500) );
}

void JetHists::Fill()
{
    // important: get the event weight
    EventCalc* calc = EventCalc::Instance();
    double weight = calc -> GetWeight();

    BaseCycleContainer* bcc = calc->GetBaseCycleContainer();

    int NJets = bcc-> jets -> size();
    Hist("NJets")->Fill(NJets, weight);
    Hist("NJets_ly")->Fill(NJets, weight);

    int NJets30 = 0;
    int NJets50 = 0;
    int NJets100 = 0;
    
    for (unsigned int i =0; i<bcc->jets->size(); ++i) {
        Jet jet =  bcc->jets->at(i);
        Hist("pt_lx") -> Fill(jet.pt(),weight );
        Hist("pt_lxy") -> Fill(jet.pt(),weight);
        Hist("eta") -> Fill(jet.eta(),weight);
        Hist("phi") -> Fill(jet.phi(),weight);
        if (jet.pt()>30) ++NJets30;
        if (jet.pt()>50) ++NJets50;
        if (jet.pt()>100) ++NJets100;
    }
    
    Hist("NJets30")->Fill(NJets30, weight);
    Hist("NJets30_ly")->Fill(NJets30, weight);
    Hist("NJets50")->Fill(NJets50, weight);
    Hist("NJets50_ly")->Fill(NJets50, weight);
    Hist("NJets100")->Fill(NJets100, weight);
    Hist("NJets100_ly")->Fill(NJets100, weight);

    sort(bcc->jets->begin(), bcc->jets->end(), HigherPt());
    for (unsigned int i =0; i<=3; ++i) {
        if (bcc->jets->size()> i) {
            Jet jet =  bcc->jets->at(i);
            TString hname = TString::Format("pt_jet%d_lx", i+1);
            Hist(hname)->Fill(jet.pt(),weight);
            TString hname_ly = TString::Format("pt_jet%d_lxy", i+1);
            Hist(hname_ly)->Fill(jet.pt(),weight);
            TString hname_eta = TString::Format("eta_%d", i+1);
            Hist(hname_eta)->Fill(jet.eta(),weight);
            TString hname_phi = TString::Format("phi_%d", i+1);
            Hist(hname_phi)->Fill(jet.phi(),weight);
            TString hname_m = TString::Format("m_%d", i+1);
            if(jet.v4().isTimelike())
                Hist(hname_m)->Fill(jet.v4().M(), weight);
            if (jet.btag_combinedSecondaryVertex()>0.244) {
                Hist("bjet_tag")-> Fill(i+1,weight);
            }
            if (i<2) {
                TString hname_deltaRmin = TString::Format("deltaRmin_%d", i+1);
                Hist(hname_deltaRmin)->Fill( deltaRmin(&jet, bcc->jets), weight);
                TString hname_deltaRmin_ly = TString::Format("deltaRmin_%d_ly", i+1);
                Hist(hname_deltaRmin_ly)->Fill( deltaRmin(&jet, bcc->jets), weight);
            }
        }
    }

  if (bcc->jets->size()> 1)
     {
      Jet jet1 =  bcc->jets->at(0);  
      Jet jet2 =  bcc->jets->at(1);
      double deltaPhi = abs(jet1.phi() - jet2.phi());
      Hist("DeltaPhi_Jet1_Jet2") -> Fill(deltaPhi, weight);
     }

    vector<Jet> bjets;
    for (unsigned int i =0; i<bcc->jets->size(); ++i) {
        if(bcc->jets->at(i).btag_combinedSecondaryVertex()>0.244) {
            bjets.push_back(bcc->jets->at(i));
        }
    }

    int NbJets = bjets.size();
    Hist("NbJets")-> Fill(NbJets,weight);
    Hist("NbJets_ly")-> Fill(NbJets,weight);

    sort(bjets.begin(), bjets.end(), HigherPt());
    for (unsigned int i =0; i<=1; ++i) {
        if (bjets.size()> i) {
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
    if (bcc->jets->size() > 0 )
    {
      Jet jet1 = bcc->jets->at(0);
      TLorentzVector Jet1;
      Jet1.SetPtEtaPhiE(jet1.pt() ,jet1.eta() ,jet1.phi() ,jet1.energy() );
     
      Jet jet2 = bcc->jets->at(0);
      TLorentzVector Jet2;
      Jet2.SetPtEtaPhiE(jet2.pt() ,jet2.eta() ,jet2.phi() ,jet2.energy() );
      TLorentzVector Jet = Jet2 +Jet1;
      double InvMass = Jet.M();
	      
      Hist("InvMassJet1Jet2")->Fill(InvMass, weight);
      Hist("InvMassJet1Jet2_ly")->Fill(InvMass, weight);
    }

}


