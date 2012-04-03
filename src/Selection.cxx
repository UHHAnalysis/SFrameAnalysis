#include "../include/Selection.h"


Selection::Selection( BaseCycleContainer* input_){

  bcc = input_;

}


bool Selection::TopTagging(int min_toptag){

  int ntoptag=0;
  
  for(unsigned int i=0; i< bcc->topjets->size(); ++i){
    
    TopJet topjet =  bcc->topjets->at(i);

    LorentzVector allsubjets(0,0,0,0);
    for(int j=0; j<topjet.numberOfDaughters; ++j){
      allsubjets += topjet.subjets[j].v4();
    }
    double mjet = allsubjets.M();
    
    //jet mass between 140 and 250 GeV/c^2
    if(mjet<140 || mjet>250) continue;

    //at least 3 sub-jets
    if(topjet.numberOfDaughters<3) continue;

    sort(topjet.subjets.begin(), topjet.subjets.end(), HigherPt());

    double m01 = (topjet.subjets[0].v4()+topjet.subjets[1].v4()).M();
    double m02 = (topjet.subjets[0].v4()+topjet.subjets[2].v4()).M();
    double m12 = (topjet.subjets[1].v4()+topjet.subjets[2].v4()).M();
      
    //minimum pairwise mass > 50 GeV/c^2
    double mmin = std::min(m01,std::min(m02,m12));

    if(mmin<50) continue;

    ntoptag++;
  }

  if(ntoptag<min_toptag) return false;
  return true;

}
