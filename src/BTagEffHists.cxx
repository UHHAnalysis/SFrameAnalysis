#include "include/BTagEffHists.h"
#include "include/SelectionModules.h"
#include <iostream>

using namespace std;

BTagEffHists::BTagEffHists(const char* name, E_BtagType type) : BaseHists(name)
{
    // named default constructor
    m_type = type;
}

BTagEffHists::~BTagEffHists()
{
    // default destructor, does nothing
}

void BTagEffHists::Init()
{
    // book all histograms here
    double jetpt_bbins[] = {
        20, 30, 40, 50, 60, 70, 80, 100, 120, 160, 210, 260, 320, 400, 500, 600, 800, 1600
    };

    double jetpt_cbins[] = {
        20, 30, 40, 50, 60, 70, 80, 100, 120, 160, 210, 260, 320, 400, 1600
    };

    double jetpt_lbins[] = {
        20, 30, 40, 50, 60, 70, 80, 100, 120, 160, 210, 260, 320, 400, 500, 600, 1600
    };

    // basic jet kinematics
    Book( TH1F( "pt_bJet"," p_{T} b-jets", 17, jetpt_bbins));
    Book( TH1F( "eta_bJet","#eta b-jets", 50,-3,3));
    Book( TH1F( "phi_bJet","#phi b-jets", 50, -M_PI, M_PI));

    Book( TH1F( "pt_cJet"," p_{T} c-jets", 14, jetpt_cbins));
    Book( TH1F( "eta_cJet","#eta c-jets", 50,-3,3));
    Book( TH1F( "phi_cJet","#phi c-jets", 50, -M_PI, M_PI));

    Book( TH1F( "pt_lJet"," p_{T} l-jets", 16, jetpt_lbins));
    Book( TH1F( "eta_lJet","#eta l-jets", 50,-3,3));
    Book( TH1F( "phi_lJet","#phi l-jets", 50, -M_PI, M_PI));

    Book( TH1F( "pt_bJet_bTag"," p_{T} tagged b-jets", 17, jetpt_bbins));
    Book( TH1F( "eta_bJet_bTag","#eta tagged b-jets", 50,-3,3));
    Book( TH1F( "phi_bJet_bTag","#phi tagged b-jets", 50, -M_PI, M_PI));

    Book( TH1F( "pt_cJet_bTag"," p_{T} tagged c-jets", 14, jetpt_cbins));
    Book( TH1F( "eta_cJet_bTag","#eta tagged c-jets", 50,-3,3));
    Book( TH1F( "phi_cJet_bTag","#phi tagged c-jets", 50, -M_PI, M_PI));

    Book( TH1F( "pt_lJet_bTag"," p_{T} tagged l-jets", 16, jetpt_lbins));
    Book( TH1F( "eta_lJet_bTag","#eta tagged l-jets", 50,-3,3));
    Book( TH1F( "phi_lJet_bTag","#phi tagged l-jets", 50, -M_PI, M_PI));

    Book( TH1F( "pt_bJet_SubJet"," p_{T} sub-jet b-jets", 17, jetpt_bbins));
    Book( TH1F( "pt_bJet_SubJet_bTag"," p_{T} tagged sub-jet b-jets ", 17, jetpt_bbins));

    Book( TH1F( "pt_cJet_SubJet"," p_{T} sub-jet c-jets", 14, jetpt_cbins));
    Book( TH1F( "pt_cJet_SubJet_bTag"," p_{T} tagged sub-jet c-jets ", 14, jetpt_cbins));

    Book( TH1F( "pt_lJet_SubJet"," p_{T} sub-jet l-jets", 16, jetpt_lbins));
    Book( TH1F( "pt_lJet_SubJet_bTag"," p_{T} tagged sub-jet l-jets ", 16, jetpt_lbins));

    Book( TH1F( "pt_bJet_TopJet"," p_{T} top-jet b-jets", 17, jetpt_bbins));
    Book( TH1F( "pt_bJet_TopJet_bTag"," p_{T} tagged top-jet b-jets ", 17, jetpt_bbins));

    Book( TH1F( "pt_cJet_TopJet"," p_{T} top-jet c-jets", 14, jetpt_cbins));
    Book( TH1F( "pt_cJet_TopJet_bTag"," p_{T} tagged top-jet c-jets ", 14, jetpt_cbins));

    Book( TH1F( "pt_lJet_TopJet"," p_{T} top-jet l-jets", 16, jetpt_lbins));
    Book( TH1F( "pt_lJet_TopJet_bTag"," p_{T} tagged top-jet l-jets ", 16, jetpt_lbins));

}

void BTagEffHists::Fill()
{
    EventCalc* calc = EventCalc::Instance();
    double weight = calc -> GetWeight();

    BaseCycleContainer* bcc = calc->GetBaseCycleContainer();

    for (unsigned int i =0; i<bcc->jets->size(); ++i) {

        Jet jet =  bcc->jets->at(i);
        unsigned int flavor = abs(jet.flavor());
        bool btag = IsTagged(jet, m_type);

        switch(flavor) {
        case 5: // b-jets
            Hist("pt_bJet") -> Fill(jet.pt(),weight);
            Hist("eta_bJet") -> Fill(jet.eta(),weight);
            Hist("phi_bJet") -> Fill(jet.phi(),weight);
            if (btag) {
                Hist("pt_bJet_bTag") -> Fill(jet.pt(),weight);
                Hist("eta_bJet_bTag") -> Fill(jet.eta(),weight);
                Hist("phi_bJet_bTag") -> Fill(jet.phi(),weight);
            }
            break;
        case 4: // c-jets
            Hist("pt_cJet") -> Fill(jet.pt(),weight);
            Hist("eta_cJet") -> Fill(jet.eta(),weight);
            Hist("phi_cJet") -> Fill(jet.phi(),weight);
            if (btag) {
                Hist("pt_cJet_bTag") -> Fill(jet.pt(),weight);
                Hist("eta_cJet_bTag") -> Fill(jet.eta(),weight);
                Hist("phi_cJet_bTag") -> Fill(jet.phi(),weight);
            }
            break;
        case 3: // s-quark
        case 2: // d-quark
        case 1: // u-quark
        case 21: // gluon
            Hist("pt_lJet") -> Fill(jet.pt(),weight);
            Hist("eta_lJet") -> Fill(jet.eta(),weight);
            Hist("phi_lJet") -> Fill(jet.phi(),weight);
            if (btag) {
                Hist("pt_lJet_bTag") -> Fill(jet.pt(),weight);
                Hist("eta_lJet_bTag") -> Fill(jet.eta(),weight);
                Hist("phi_lJet_bTag") -> Fill(jet.phi(),weight);
            }
            break;
        default:
            break;
        }
    }

    for (unsigned int i =0; i<bcc->topjets->size(); ++i) {

       TopJet topjet =  bcc->topjets->at(i);
       //kinematic cut on top-jets
       if(topjet.pt() < 250.) continue;

       unsigned int nsubjets = topjet.subjets().size();
       double min_dr = 5.;
       if(nsubjets > 1){
	 // loop over subjets, find minimum DeltaR between them
	 for(unsigned int g = 0; g<nsubjets-1; ++g){
	   
	   Particle subjetg = topjet.subjets().at(g);
	   for(unsigned int k = g+1; k<nsubjets; ++k){
	     
	     double dr = subjetg.deltaR(topjet.subjets().at(k));
	     if(dr < min_dr) min_dr = dr;
	   }
	 }
       }

       //fill efficiency histograms for subjets in case dR>0.4
       if(min_dr>0.4){

	 double discriminator_cut= 0.0;
	 if(m_type==e_CSVL) discriminator_cut = 0.244;
	 if(m_type==e_CSVM) discriminator_cut = 0.679;
	 if(m_type==e_CSVT) discriminator_cut = 0.898;

	 std::vector<float> btagsub_combinedSecondaryVertex_top;
	 std::vector<int> flavorsub_top;
	 
	 btagsub_combinedSecondaryVertex_top=topjet.btagsub_combinedSecondaryVertex();
	 flavorsub_top=topjet.flavorsub();
	 
	 for(unsigned int j = 0; j<nsubjets; ++j){
	   bool btag = btagsub_combinedSecondaryVertex_top[j]>discriminator_cut;
	  
	   float jet_pt = topjet.subjets().at(j).pt();

	   unsigned int flavor = abs(flavorsub_top[j]);

	   switch(flavor) {
	   case 5: // b-jets
	     Hist("pt_bJet_SubJet") -> Fill(jet_pt,weight);
	     if(btag) Hist("pt_bJet_SubJet_bTag") -> Fill(jet_pt,weight);
	     break;
	   case 4: // c-jets
	     Hist("pt_cJet_SubJet") -> Fill(jet_pt,weight);
	     if(btag) Hist("pt_cJet_SubJet_bTag") -> Fill(jet_pt,weight);
	     break;
	   case 3: // s-quark
	   case 2: // d-quark
	   case 1: // u-quark
	   case 21: // gluon
	     Hist("pt_lJet_SubJet") -> Fill(jet_pt,weight);
	     if(btag) Hist("pt_lJet_SubJet_bTag") -> Fill(jet_pt,weight);
	     break;

	   default:
	     break;
	   }
	 }
       }
       //fill efficiency histograms for entire top-jets
       else{
	 bool btag = IsTagged(topjet, m_type);
	  
	 float jet_pt = topjet.pt();
	 
	 unsigned int flavor = abs(topjet.flavor());

	 switch(flavor) {
	 case 5: // b-jets
	   Hist("pt_bJet_TopJet") -> Fill(jet_pt,weight);
	   if(btag) Hist("pt_bJet_TopJet_bTag") -> Fill(jet_pt,weight);
	   break;
	 case 4: // c-jets
	   Hist("pt_cJet_TopJet") -> Fill(jet_pt,weight);
	   if(btag) Hist("pt_cJet_TopJet_bTag") -> Fill(jet_pt,weight);
	   break;
	 case 3: // s-quark
	 case 2: // d-quark
	 case 1: // u-quark
	 case 21: // gluon
	   Hist("pt_lJet_TopJet") -> Fill(jet_pt,weight);
	   if(btag) Hist("pt_lJet_TopJet_bTag") -> Fill(jet_pt,weight);
	   break;
	   
	 default:
	   break;
	 }
       }
       
      
    }
    
    
}

