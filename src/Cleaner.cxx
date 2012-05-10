#include "../include/Cleaner.h"


Cleaner::Cleaner( BaseCycleContainer* input_){

  bcc = input_;

}

void Cleaner::JetEnergyResolutionShifter(int syst_shift){

  double met = bcc->met->pt;

  for(unsigned int i=0; i<bcc->jets->size(); ++i){
    Jet jet = bcc->jets->at(i);
    float gen_pt = jet.genjet_pt;
    //ignore unmatched jets (which have zero vector) or jets with very low pt:
    if(gen_pt < 15.0) continue;

    met += jet.pt*jet.JEC_factor_raw;
    
    float recopt = jet.pt;
    double factor = -10;
    double abseta = fabs(jet.eta);

    //numbers taken from https://twiki.cern.ch/twiki/bin/view/CMS/JetResolution
    if(syst_shift==0){
      if(abseta < 0.5)
	factor = 0.052;
      else if(abseta >= 0.5 && abseta <1.1)
	factor = 0.057;
      else if(abseta >= 1.1 && abseta <1.7)
	factor = 0.096;
      else if(abseta >= 1.7 && abseta <2.3)
	factor = 0.134;
      else if(abseta >= 2.3)
	factor = 0.288;
    }
    else if(syst_shift>0){
      if(abseta < 0.5)
	factor = 0.11;
      else if(abseta >= 0.5 && abseta <1.1)
	factor = 0.12;
      else if(abseta >= 1.1 && abseta <1.7)
	factor = 0.16;
      else if(abseta >= 1.7 && abseta <2.3)
	factor = 0.23;
      else if(abseta >= 2.3)
	factor = 0.49;
    }
    else{
      if(abseta < 0.5)
	factor = -0.01;
      else if(abseta >= 0.5 && abseta <1.1)
	factor = 0.00;
      else if(abseta >= 1.1 && abseta <1.7)
	factor = 0.04;
      else if(abseta >= 1.7 && abseta <2.3)
	factor = 0.03;
      else if(abseta >= 2.3)
	factor = 0.09;
    }

    float deltapt = (recopt - gen_pt) * factor;
    float ptscale = std::max(0.0f, (recopt + deltapt) / recopt);
    jet.pt *= ptscale;
    bcc->jets->at(i).pt = jet.pt;

    //propagate JER shifts to MET
    met -= jet.pt*jet.JEC_factor_raw;
  }
  
  //store changed MET, flip phi if new MET is negative
  if(met>=0){
    bcc->met->pt = met;
  }
  else{
    bcc->met->pt = -1*met;
    if(bcc->met->phi<=0)
      bcc->met->phi = bcc->met->phi+PI;
    else
      bcc->met->phi = bcc->met->phi-PI;
  }
 
}


//tight ele ID from https://twiki.cern.ch/twiki/bin/view/CMS/EgammaCutBasedIdentification
bool Cleaner::eleID(Electron ele){

  bool pass=false;
  
  if(fabs(ele.supercluster_eta)<1.4442){
    if(ele.dEtaIn<0.004 && ele.dPhiIn<0.03 && ele.sigmaIEtaIEta<0.01 && ele.HoverE<0.12 && fabs(1./ele.EcalEnergy-1./ele.v4().P())<0.05) pass=true; 
  }
  else if( fabs(ele.supercluster_eta)>1.5660){
    if(ele.dEtaIn<0.005 && ele.dPhiIn<0.02 && ele.sigmaIEtaIEta<0.03 && ele.HoverE<0.10 && fabs(1./ele.EcalEnergy-1./ele.v4().P())<0.05) pass=true; 
  }

  return pass;

}

//pf ID has already been applied when using goodPatJets
bool Cleaner::pfID(Jet jet){

  if(jet.numberOfDaughters>1 
     && jet.neutralHadronEnergyFraction<0.99
     && jet.neutralEmEnergyFraction<0.99){
    
    if(fabs(jet.eta)>=2.4)
      return true;
    
    if(jet.chargedEmEnergyFraction<0.99
       && jet.chargedHadronEnergyFraction>0
       && jet.chargedMultiplicity>0)
      return true;

  }
  return false;
     
}


void Cleaner::ElectronCleaner(double ptmin, double etamax){

  std::vector<Electron> good_eles;
  for(unsigned int i=0; i<bcc->electrons->size(); ++i){
    Electron ele = bcc->electrons->at(i);
    if(ele.pt>ptmin){
      if(fabs(ele.eta)<etamax){
 	if(fabs(ele.supercluster_eta)<1.4442 || fabs(ele.supercluster_eta)>1.5660){
 	  if(bcc->pvs->size()>0){
 	    if(ele.gsfTrack_dxy_vertex(bcc->pvs->at(0).x, bcc->pvs->at(0).y)<0.02){
	      if(ele.passconversionveto){
		//if(ele.mvaTrigV0>0.0){
		if(eleID(ele)){
		  if(ele.relIso()<0.1){
		    good_eles.push_back(ele);
		  }
		}
		//}
	      }
 	    }
 	  }
 	}
      }
    }
  }

  bcc->electrons->clear();

  for(unsigned int i=0; i<good_eles.size(); ++i){
    bcc->electrons->push_back(good_eles[i]);
  }

}


void Cleaner::MuonCleaner(double ptmin, double etamax){

  std::vector<Muon> good_mus;
  for(unsigned int i=0; i<bcc->muons->size(); ++i){
    Muon mu = bcc->muons->at(i);
    if(mu.pt>ptmin){
      if(fabs(mu.eta)<etamax){
	if(mu.isGlobalMuon){
	  if(mu.globalTrack_chi2/mu.globalTrack_ndof<10){
	    if(mu.innerTrack_trackerLayersWithMeasurement>8){
	      if(mu.dB<0.02){
		if(mu.relIso()<0.125){
		  if(fabs(mu.vertex_z-bcc->pvs->at(0).z)<1){
		    if(mu.innerTrack_numberOfValidPixelHits>0){
		      if(mu.numberOfMatchedStations>1){
			good_mus.push_back(mu);
		      }
		    }
		  }
		}
	      }
	    }
	  }
	}
      }
    }
  }
  bcc->muons->clear();

  for(unsigned int i=0; i<good_mus.size(); ++i){
    bcc->muons->push_back(good_mus[i]);
  }

}

void Cleaner::TauCleaner(double ptmin, double etamax){

  std::vector<Tau> good_taus;
  for(unsigned int i=0; i<bcc->taus->size(); ++i){
    Tau tau = bcc->taus->at(i);
    if(tau.pt>ptmin){
      if(fabs(tau.eta)<etamax){
	if(bcc->taus->at(i).decayModeFinding){
	  if(bcc->taus->at(i).byMediumCombinedIsolationDeltaBetaCorr){
	    if(bcc->taus->at(i).againstElectronTight){
	      if(bcc->taus->at(i).againstMuonTight){
		good_taus.push_back(tau);
	      }
	    }
	  }
	}
      }
    }
  }
  
  bcc->taus->clear();

  for(unsigned int i=0; i<good_taus.size(); ++i){
    bcc->taus->push_back(good_taus[i]);
  }

}


void Cleaner::JetCleaner(double ptmin, double etamax, bool doPFID){

  std::vector<Jet> good_jets;
  for(unsigned int i=0; i<bcc->jets->size(); ++i){
    Jet jet = bcc->jets->at(i);
    if(jet.pt>ptmin){
      if(fabs(jet.eta)<etamax){
	if(!doPFID || pfID(jet)){
	  good_jets.push_back(jet);
	}
      }
    }
  }

  bcc->jets->clear();

  for(unsigned int i=0; i<good_jets.size(); ++i){
    bcc->jets->push_back(good_jets[i]);
  }

}

void Cleaner::TopJetCleaner(double ptmin, double etamax, bool doPFID){

  std::vector<TopJet> good_topjets;
  for(unsigned int i=0; i<bcc->topjets->size(); ++i){
    TopJet topjet = bcc->topjets->at(i);
    if(topjet.pt>ptmin){
      if(fabs(topjet.eta)<etamax){
	if(!doPFID || pfID(topjet)){
	  good_topjets.push_back(topjet);
	}
      }
    }
  }
  bcc->topjets->clear();

  for(unsigned int i=0; i<good_topjets.size(); ++i){
    bcc->topjets->push_back(good_topjets[i]);
  }

}
