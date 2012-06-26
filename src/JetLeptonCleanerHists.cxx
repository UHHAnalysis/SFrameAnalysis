#include "include/JetLeptonCleanerHists.h"
#include "include/ObjectHandler.h"
#include "include/EventCalc.h"
#include <iostream>

using namespace std;

JetLeptonCleanerHists::JetLeptonCleanerHists(const char* name) : BaseHists(name)
{
  // named default constructor
   
}

JetLeptonCleanerHists::~JetLeptonCleanerHists()
{
  // default destructor, does nothing
}

void JetLeptonCleanerHists::Init()
{
  // book all histograms here

  Book( TH1F( "MET_noJLS", "E_{T}^{miss} [GeV]", 200, 0, 400 ) );
  Book( TH1F( "METdiff_noJLS", "#Sigma p_{T}^{#nu} - E_{T}^{miss} [GeV]", 200, -100, 100 ) );
  Book( TH1F( "METresolution_noJLS", "(#Sigma p_{T}^{#nu} - E_{T}^{miss})/#Sigma p_{T}^{#nu}", 200, -5, 5 ) );
  
  Book( TH1F( "MET_withJLS", "E_{T}^{miss} [GeV]", 200, 0, 400 ) );
  Book( TH1F( "METdiff_withJLS", "#Sigma p_{T}^{#nu} - E_{T}^{miss} [GeV]", 200, -100, 100 ) );
  Book( TH1F( "METresolution_withJLS", "(#Sigma p_{T}^{#nu} - E_{T}^{miss})/#Sigma p_{T}^{#nu}", 200, -5, 5 ) );

  Book( TH1F( "delta_pt", "#Delta p_{T} [GeV/c]", 200, -100,100));
  Book( TH1F( "delta_eta", "|#Delta #eta|", 200, 0,6)); 
  Book( TH1F( "delta_phi", "#Delta #phi", 200, 0,3.2)); 
  Book( TH1F( "delta_R", "#Delta R", 200, 0,6));  

  Book( TH2F( "delta_R_delta_pt", "#Delta R vs #Delta p_{T} [GeV/c]", 100, 0,6, 100,-100,100));  

  Book( TH1F( "pt_resolution_noJLS", "(p_{T}^{gen} - p_{T})/p_{T}^{gen}", 200,-15,5));
  Book( TH1F( "pt_resolution_withJLS", "(p_{T}^{gen} - p_{T})/p_{T}^{gen}", 200,-15,5));

  Book( TH2F( "ptrec_ptgen_noJLS", "p_{T} vs p_{T}^{gen}", 100,0,500,100,0,500));
  Book( TH2F( "ptrec_ptgen_withJLS", "p_{T} vs p_{T}^{gen}", 100,0,500,100,0,500));
  
  Book( TH2F( "ptresolution_ptgenlep_noJLS", "p_{T} vs p_{T}^{gen}", 100,-15,5,100,0,200));
  Book( TH2F( "ptresolution_ptgenlep_withJLS", "p_{T} vs p_{T}^{gen}", 100,-15,5,100,0,200));

  Book( TH1F( "pt_resolution_genJLS_noJLS", "(p_{T}^{gen} - p_{T})/p_{T}^{gen}", 200,-15,5));
  Book( TH1F( "pt_resolution_genJLS_withJLS", "(p_{T}^{gen} - p_{T})/p_{T}^{gen}", 200,-15,5));

  Book( TH2F( "ptrec_ptgen_genJLS_noJLS", "p_{T} vs p_{T}^{gen}", 100,0,500,100,0,500));
  Book( TH2F( "ptrec_ptgen_genJLS_withJLS", "p_{T} vs p_{T}^{gen}", 100,0,500,100,0,500));

  std::vector<JetCorrectorParameters> pars;

  pars.push_back(JetCorrectorParameters("/scratch/hh/lustre/cms/user/peiffer/JECFiles/START52_V11_L1FastJet_AK5PFchs.txt"));
  pars.push_back(JetCorrectorParameters("/scratch/hh/lustre/cms/user/peiffer/JECFiles/START52_V11_L2Relative_AK5PFchs.txt"));  
  pars.push_back(JetCorrectorParameters("/scratch/hh/lustre/cms/user/peiffer/JECFiles/START52_V11_L3Absolute_AK5PFchs.txt")); 


  m_corrector = new FactorizedJetCorrector(pars);

}

void JetLeptonCleanerHists::Fill()
{
  // fill the histograms

  // important: get the event weight

  EventCalc* calc = EventCalc::Instance();

  ObjectHandler* objs = ObjectHandler::Instance();

  BaseCycleContainer* bcc = objs->GetBaseCycleContainer();
  

  Cleaner* cleaner = new Cleaner(bcc);

  std::vector<Jet> original_jets;
  for(unsigned int i=0; i< bcc->jets->size();++i){
    original_jets.push_back(bcc->jets->at(i));
  }

  cleaner->JetRecorrector(m_corrector,false);
  cleaner->JetEnergyResolutionShifter(e_Default,false);

  std::vector<Jet> uncleaned_jets;
  for(unsigned int i=0; i< bcc->jets->size();++i){
    uncleaned_jets.push_back(bcc->jets->at(i));
  }

  float uncleanedmet = bcc->met->pt();

  //fill back the original jets and perform lepton subtraction
  bcc->jets->clear();
  for(unsigned int i=0; i<original_jets.size();++i){
     bcc->jets->push_back(original_jets.at(i));
  }

  cleaner->JetLeptonSubtractor(m_corrector,false);
  cleaner->JetEnergyResolutionShifter(e_Default,false);
  float met = bcc->met->pt();

  //search for changed jets by comparing uncleaned and new jet collection
  bool changed=false;
  for(unsigned int i=0; i< bcc->jets->size();++i){
    if(fabs(uncleaned_jets.at(i).pt()-bcc->jets->at(i).pt())>0.0001){
      //std::cout << "vorher: " << uncleaned_jets.at(i).pt() << "   nachher: " << bcc->jets->at(i).pt() <<std::endl;
      changed=true;
      Hist( "delta_pt")->Fill( uncleaned_jets.at(i).pt()-bcc->jets->at(i).pt());
      Hist( "delta_eta")->Fill( fabs(uncleaned_jets.at(i).eta()-bcc->jets->at(i).eta())); 
      Hist( "delta_phi")->Fill( bcc->jets->at(i).deltaPhi(uncleaned_jets.at(i)));
      Hist( "delta_R")->Fill( bcc->jets->at(i).deltaR(uncleaned_jets.at(i)));
      Hist( "delta_R_delta_pt")->Fill( bcc->jets->at(i).deltaR(uncleaned_jets.at(i)), uncleaned_jets.at(i).pt()-bcc->jets->at(i).pt());
      if(  bcc->jets->at(i).has_genjet()){
	Hist("pt_resolution_noJLS")->Fill( (bcc->jets->at(i).genjet_pt() -  uncleaned_jets.at(i).pt() )/ bcc->jets->at(i).genjet_pt() );	
	Hist("pt_resolution_withJLS")->Fill( (bcc->jets->at(i).genjet_pt() -  bcc->jets->at(i).pt() )/ bcc->jets->at(i).genjet_pt() );
	Hist("ptrec_ptgen_noJLS")->Fill( uncleaned_jets.at(i).pt(), bcc->jets->at(i).genjet_pt());
	Hist("ptrec_ptgen_withJLS")->Fill( bcc->jets->at(i).pt(), bcc->jets->at(i).genjet_pt());

	//lepton removal from jets on generator level
	LorentzVector genlep_v4(0,0,0,0);
	for(unsigned int j=0; j<bcc->jets->at(i).genparticles_indices().size(); ++j){
	  unsigned int index = bcc->jets->at(i).genparticles_indices().at(j);
	  int pdgId = abs(bcc->genparticles->at(index).pdgId());
	  int status = abs(bcc->genparticles->at(index).status());
	  if(status==1 &&(  pdgId==11 || pdgId==13)){
	    genlep_v4+= bcc->genparticles->at(index).v4();
	  }
	}


	Hist("ptresolution_ptgenlep_noJLS") -> Fill( (bcc->jets->at(i).genjet_pt() -  uncleaned_jets.at(i).pt() )/ bcc->jets->at(i).genjet_pt() , genlep_v4.pt());
	Hist("ptresolution_ptgenlep_withJLS") -> Fill ( (bcc->jets->at(i).genjet_pt() -  bcc->jets->at(i).pt() )/ bcc->jets->at(i).genjet_pt() , genlep_v4.pt());

	LorentzVector genjet_v4 =  bcc->jets->at(i).genjet_v4();
	genjet_v4 -= genlep_v4;
	Hist("pt_resolution_genJLS_noJLS")->Fill( (genjet_v4.pt() -  uncleaned_jets.at(i).pt() )/ genjet_v4.pt() );	
	Hist("pt_resolution_genJLS_withJLS")->Fill( (genjet_v4.pt() -  bcc->jets->at(i).pt() )/ genjet_v4.pt() );
	Hist("ptrec_ptgen_genJLS_noJLS")->Fill( uncleaned_jets.at(i).pt(), genjet_v4.pt());
	Hist("ptrec_ptgen_genJLS_withJLS")->Fill( bcc->jets->at(i).pt(), genjet_v4.pt());

      }
    }
  }
  if(changed){

    LorentzVector p4nu(0,0,0,0);

    for(unsigned int i=0; i<bcc->genparticles->size();++i){
      if(bcc->genparticles->at(i).status()!=3) continue;
      int pdgId= abs(bcc->genparticles->at(i).pdgId());
      if(pdgId==12 || pdgId==14 ||pdgId==16){
	p4nu+=bcc->genparticles->at(i).v4();
      }
    }

    float genmet=p4nu.pt();

    Hist("MET_noJLS")->Fill(uncleanedmet);
    Hist("METdiff_noJLS")->Fill(genmet-uncleanedmet);
    if(genmet!=0) Hist("METresolution_noJLS")->Fill((genmet-uncleanedmet)/genmet);
    Hist("MET_withJLS")->Fill(met);
    Hist("METdiff_withJLS")->Fill(genmet-met);
    if(genmet!=0)Hist("METresolution_withJLS")->Fill((genmet-met)/genmet);
  }

}

void JetLeptonCleanerHists::Finish()
{


}

