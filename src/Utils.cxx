#include "../include/Utils.h"


bool TopTag(TopJet topjet,  double &mjet, int &nsubjets, double &mmin){
 
  nsubjets=topjet.numberOfDaughters();

  LorentzVector allsubjets(0,0,0,0);
  
  for(int j=0; j<topjet.numberOfDaughters(); ++j){
    allsubjets += topjet.subjets()[j].v4();
  }
  if(!allsubjets.isTimelike()){
    mjet=0;
    mmin=0;
    return false;
  }

  mjet = allsubjets.M();
  
  if(nsubjets>=3) {

    std::vector<Particle> subjets = topjet.subjets();
    sort(subjets.begin(), subjets.end(), HigherPt());

    double m01 = 0;
    if( (subjets[0].v4()+subjets[1].v4()).isTimelike()) 
      m01=(subjets[0].v4()+subjets[1].v4()).M();
    double m02 = 0;
    if( (subjets[0].v4()+subjets[2].v4()).isTimelike() ) 
      m02=(subjets[0].v4()+subjets[2].v4()).M();
    double m12 = 0;
    if( (subjets[1].v4()+subjets[2].v4()).isTimelike()  )
      m12 = (subjets[1].v4()+subjets[2].v4()).M();
    
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
