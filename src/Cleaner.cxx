#include "../include/Cleaner.h"


Cleaner::Cleaner( BaseCycleContainer* input_){

  bcc = input_;

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


std::vector<Electron>* Cleaner::ElectronCleaner(double ptmin, double etamax){

  std::vector<Electron>* good_eles = new std::vector<Electron>;
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
		    good_eles->push_back(ele);
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

  return good_eles;

}

std::vector<Muon>* Cleaner::MuonCleaner(double ptmin, double etamax){

  std::vector<Muon>* good_mus = new std::vector<Muon>;
  for(unsigned int i=0; i<bcc->muons->size(); ++i){
    Muon mu = bcc->muons->at(i);
    if(mu.pt>ptmin){
      if(fabs(mu.eta)>etamax){
	if(mu.isGlobalMuon){
	  if(mu.globalTrack_chi2/mu.globalTrack_ndof<10){
	    if(mu.innerTrack_trackerLayersWithMeasurement>8){
	      if(mu.dB<0.02){
		if(mu.relIso()<0.0125){
		  if(fabs(mu.vertex_z-bcc->pvs->at(0).z)<1){
		    if(mu.innerTrack_numberOfValidPixelHits>0){
		      if(mu.numberOfMatchedStations>1){
			good_mus->push_back(mu);
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
  
  return good_mus;

}
