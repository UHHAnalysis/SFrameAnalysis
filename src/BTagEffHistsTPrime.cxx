#include "include/BTagEffHistsTPrime.h"
#include "include/ObjectHandler.h"
#include "include/SelectionModules.h"
#include <iostream>

using namespace std;

BTagEffHistsTPrime::BTagEffHistsTPrime(const char* name) : BaseHists(name)
{
    // named default constructor
}

BTagEffHistsTPrime::~BTagEffHistsTPrime()
{
    // default destructor, does nothing
}

void BTagEffHistsTPrime::Init()
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
    Book( TH1F( "phi_bJet","#phi b-jets", 50, -PI, PI));

    Book( TH1F( "pt_cJet"," p_{T} c-jets", 14, jetpt_cbins));
    Book( TH1F( "eta_cJet","#eta c-jets", 50,-3,3));
    Book( TH1F( "phi_cJet","#phi c-jets", 50, -PI, PI));

    Book( TH1F( "pt_lJet"," p_{T} l-jets", 16, jetpt_lbins));
    Book( TH1F( "pt_lJet_e1"," p_{T} l-jets", 16, jetpt_lbins));
    Book( TH1F( "pt_lJet_e2"," p_{T} l-jets", 16, jetpt_lbins));
    Book( TH1F( "pt_lJet_e3"," p_{T} l-jets", 16, jetpt_lbins));
    Book( TH1F( "eta_lJet","#eta l-jets", 50,-3,3));
    Book( TH1F( "phi_lJet","#phi l-jets", 50, -PI, PI));

    Book( TH1F( "pt_bJet_bTag"," p_{T} tagged b-jets", 17, jetpt_bbins));
    Book( TH1F( "eta_bJet_bTag","#eta tagged b-jets", 50,-3,3));
    Book( TH1F( "phi_bJet_bTag","#phi tagged b-jets", 50, -PI, PI));

    Book( TH1F( "pt_cJet_bTag"," p_{T} tagged c-jets", 14, jetpt_cbins));
    Book( TH1F( "eta_cJet_bTag","#eta tagged c-jets", 50,-3,3));
    Book( TH1F( "phi_cJet_bTag","#phi tagged c-jets", 50, -PI, PI));

    Book( TH1F( "pt_lJet_bTag"," p_{T} tagged l-jets", 16, jetpt_lbins));
    Book( TH1F( "pt_lJet_e1_bTag"," p_{T} tagged l-jets", 16, jetpt_lbins));
    Book( TH1F( "pt_lJet_e2_bTag"," p_{T} tagged l-jets", 16, jetpt_lbins));
    Book( TH1F( "pt_lJet_e3_bTag"," p_{T} tagged l-jets", 16, jetpt_lbins));
    Book( TH1F( "eta_lJet_bTag","#eta tagged l-jets", 50,-3,3));
    Book( TH1F( "phi_lJet_bTag","#phi tagged l-jets", 50, -PI, PI));
}

void BTagEffHistsTPrime::Fill()
{
    EventCalc* calc = EventCalc::Instance();
    double weight = calc -> GetWeight();

    ObjectHandler* objs = ObjectHandler::Instance();
    BaseCycleContainer* bcc = objs->GetBaseCycleContainer();

    std::vector<Particle> subjets_top;
    std::vector<float> btagsub_combinedSecondaryVertex_top;
    std::vector<int> flavorsub_top;

    bool checkbtag=0;
    
    for (unsigned int i =0; i<bcc->topjets->size(); ++i) {
      
      TopJet topjet=bcc->topjets->at(i);
      
      subjets_top=topjet.subjets();
      btagsub_combinedSecondaryVertex_top=topjet.btagsub_combinedSecondaryVertex();
      flavorsub_top=topjet.flavorsub();
      
      float csvcut=0.679;
      
      //Looping over subjets and checking if they are b-tagged
      for(unsigned int j=0; j < btagsub_combinedSecondaryVertex_top.size(); ++j){
	
	float test=btagsub_combinedSecondaryVertex_top[j];
	
	Particle subjet=subjets_top[j];
	
	checkbtag=0;
	
	if(test>csvcut){
	  checkbtag=1;
	}
	
        unsigned int flavor = abs(flavorsub_top[j]);
        bool btag = checkbtag;
	
        switch(flavor) {
        case 5: // b-jets
	  Hist("pt_bJet") -> Fill(subjet.pt(),weight);
	  Hist("eta_bJet") -> Fill(subjet.eta(),weight);
	  Hist("phi_bJet") -> Fill(subjet.phi(),weight);
	  if (btag) {
	    Hist("pt_bJet_bTag") -> Fill(subjet.pt(),weight);
	    Hist("eta_bJet_bTag") -> Fill(subjet.eta(),weight);
	    Hist("phi_bJet_bTag") -> Fill(subjet.phi(),weight);
	  }
	  break;
        case 4: // c-jets
	  Hist("pt_cJet") -> Fill(subjet.pt(),weight);
	  Hist("eta_cJet") -> Fill(subjet.eta(),weight);
	  Hist("phi_cJet") -> Fill(subjet.phi(),weight);
	  if (btag) {
	    Hist("pt_cJet_bTag") -> Fill(subjet.pt(),weight);
	    Hist("eta_cJet_bTag") -> Fill(subjet.eta(),weight);
	    Hist("phi_cJet_bTag") -> Fill(subjet.phi(),weight);
	  }
	  break;
        case 3: // s-quark
        case 2: // d-quark
        case 1: // u-quark
        case 21: // gluon
	  Hist("pt_lJet") -> Fill(subjet.pt(),weight);
	  if(fabs(subjet.eta())>=0&&fabs(subjet.eta())<=0.8){
	    Hist("pt_lJet_e1") -> Fill(subjet.pt(),weight);
	  }
	  if(fabs(subjet.eta())>=0.8&&fabs(subjet.eta())<=1.6){
	    Hist("pt_lJet_e2") -> Fill(subjet.pt(),weight);
	  }
	  if(fabs(subjet.eta())>=1.6&&fabs(subjet.eta())<=2.4){
	    Hist("pt_lJet_e3") -> Fill(subjet.pt(),weight);
	  }
	  Hist("eta_lJet") -> Fill(subjet.eta(),weight);
	  Hist("phi_lJet") -> Fill(subjet.phi(),weight);
	  if (btag) {
	    Hist("pt_lJet_bTag") -> Fill(subjet.pt(),weight);
	    if(fabs(subjet.eta())>=0&&fabs(subjet.eta())<=0.8){
	      Hist("pt_lJet_e1_bTag") -> Fill(subjet.pt(),weight);
	    }
	    if(fabs(subjet.eta())>=0.8&&fabs(subjet.eta())<=1.6){
	      Hist("pt_lJet_e2_bTag") -> Fill(subjet.pt(),weight);
	    }
	    if(fabs(subjet.eta())>=1.6&&fabs(subjet.eta())<=2.4){
	      Hist("pt_lJet_e3_bTag") -> Fill(subjet.pt(),weight);
	    }
	    Hist("eta_lJet_bTag") -> Fill(subjet.eta(),weight);
	    Hist("phi_lJet_bTag") -> Fill(subjet.phi(),weight);
	  }
	  break;
        default:
	  break;
        }
      }
    }
}


void BTagEffHistsTPrime::Finish()
{
  // final calculations, like division and addition of certain histograms
}

