#include "include/HEPTopTaggerReweightTPrime.h"

double HEPTopTaggerReweightTPrime::GetScaleWeight(int scale)
{
    EventCalc* calc = EventCalc::Instance();
    BaseCycleContainer* bcc = calc->GetBaseCycleContainer();
    bool IsRealData = calc->IsRealData();
    double scale_factor = 1.;
    double localweight=1.;
    if(!IsRealData)
      {

	double sf[3]={0.959548632146, 0.980234133823, 1.0461755891};
	double esf[3]={0.0286794912614, 0.0218470546224, 0.0565194212748};

	sf[0]+=scale*esf[0];
	sf[1]+=scale*esf[1];
	sf[2]+=scale*esf[2];

	TTbarGen* Decay = calc->GetTTbarGen();

	GenParticle topgen=Decay->Top();
	GenParticle antitopgen=Decay->Antitop();
	int decaytop=abs(Decay->Wdecay1().pdgId());
	int decayantitop=abs(Decay->WMinusdecay1().pdgId());
	int hadtop=0;
	int hadantitop=0;

	int indextopjet=-999;
	int indexantitopjet=-999;

	double mindeltart=9999999;    
	double mindeltara=9999999;   

	if(decaytop>0&&decaytop<=5){
	  hadtop=1;
	}

	if(decayantitop>0&&decayantitop<=5){
	  hadantitop=1;
	}
 
	if((hadtop==1)||(hadantitop==1)){

	  for(unsigned int itj=0; itj<bcc->topjets->size();++itj){
	    if(bcc->topjets->at(itj).pt()<200.) continue;
	    if((bcc->topjets->at(itj).deltaR(topgen)) < mindeltart){
	      mindeltart=bcc->topjets->at(itj).deltaR(topgen);
	      indextopjet=itj;
	    }
	    if((bcc->topjets->at(itj).deltaR(antitopgen)) < mindeltara){
	      mindeltara=bcc->topjets->at(itj).deltaR(antitopgen);
	      indexantitopjet=itj;
	    }
	  }

	}
	else{
	  return localweight;
	}

	if((hadtop==1)&&(mindeltart<1.0)){
	  double jetpt=bcc->topjets->at(indextopjet).pt();
	  if(jetpt>200. && jetpt<=250.) scale_factor *= (1. - sf[0]);
	  if(jetpt>250. && jetpt<=400.) scale_factor *= (1. - sf[1]);
	  if(jetpt>400.) scale_factor *= (1. - sf[2]);
	}
	
	if((hadantitop==1)&&(mindeltara<1.0)){
	  double jetpt=bcc->topjets->at(indexantitopjet).pt();
	  if(jetpt>200. && jetpt<=250.) scale_factor *= (1. - sf[0]);
	  if(jetpt>250. && jetpt<=400.) scale_factor *= (1. - sf[1]);
	  if(jetpt>400.) scale_factor *= (1. - sf[2]);
	}


	
      }

    if(scale_factor>1.||scale_factor<1.)
    localweight -= scale_factor;

    return localweight;
}
