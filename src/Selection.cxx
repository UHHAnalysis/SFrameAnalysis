#include "../include/Selection.h"


Selection::Selection( BaseCycleContainer* input_){

  bcc = input_;

}

bool Selection::HBHENoiseFilter(){
  return bcc->HBHENoiseFilterResult;
}

bool Selection::TriggerSelection(std::string name){

  for(unsigned int i=0; i<bcc->triggerNames_actualrun.size(); ++i){
    if(bcc->triggerNames_actualrun.at(i)==name)
      return bcc->triggerResults->at(i);
  }
  std::cout<< "WARNING: trigger not found: " << name <<"; reject this event." << std::endl;
  return false;
}


bool Selection::NJetSelection(int min_nparticle, double ptmin, double etamax){

  int nparticle=0;
   for(unsigned int i=0;i<bcc->jets->size(); ++i){
     if(bcc->jets->at(i).pt>ptmin && fabs(bcc->jets->at(i).eta)<etamax) nparticle++;
   }
  return nparticle>=min_nparticle;
}

bool Selection::NTopJetSelection(int min_nparticle, double ptmin, double etamax){

  int nparticle=0;
   for(unsigned int i=0;i<bcc->topjets->size(); ++i){
     if(bcc->topjets->at(i).pt>ptmin && fabs(bcc->topjets->at(i).eta)<etamax) nparticle++;
   }
  return nparticle>=min_nparticle;
}



bool Selection::NTopTagSelection(int min_ntoptag){

  int ntoptag=0;
  
  for(unsigned int i=0; i< bcc->topjets->size(); ++i){
    
    TopJet topjet =  bcc->topjets->at(i);
    double mmin=0;
    double mjet=0;
    int nsubjets=0;
    if(TopTag(topjet,mjet,nsubjets,mmin)) ntoptag++;
  }

  if(ntoptag<min_ntoptag) return false;
  return true;

}

bool Selection::TopTag(TopJet topjet,  double &mjet, int &nsubjets, double &mmin){
 
  nsubjets=topjet.numberOfDaughters;

  LorentzVector allsubjets(0,0,0,0);
  
  for(int j=0; j<topjet.numberOfDaughters; ++j){
    allsubjets += topjet.subjets[j].v4();
  }
  if(!allsubjets.isTimelike()){
    mjet=0;
    mmin=0;
    return false;
  }

  mjet = allsubjets.M();
  
  if(nsubjets>=3) {
    sort(topjet.subjets.begin(), topjet.subjets.end(), HigherPt());
    
    double m01 = 0;
    if( (topjet.subjets[0].v4()+topjet.subjets[1].v4()).isTimelike()) 
      m01=(topjet.subjets[0].v4()+topjet.subjets[1].v4()).M();
    double m02 = 0;
    if( (topjet.subjets[0].v4()+topjet.subjets[2].v4()).isTimelike() ) 
      m02=(topjet.subjets[0].v4()+topjet.subjets[2].v4()).M();
    double m12 = 0;
    if( (topjet.subjets[1].v4()+topjet.subjets[2].v4()).isTimelike()  )
      m12 = (topjet.subjets[1].v4()+topjet.subjets[2].v4()).M();
    
    //minimum pairwise mass > 50 GeV/c^2
    mmin = std::min(m01,std::min(m02,m12));
  }

  //at least 3 sub-jets
  if(nsubjets<3) return false;
  //minimum pairwise mass > 50 GeV/c^2
  if(mmin<50) return false;
  //jet mass between 140 and 250 GeV/c^2
  if(mjet<140 || mjet>250) return false;
  
  return true;
}
