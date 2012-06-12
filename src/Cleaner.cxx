#include "../include/Cleaner.h"


Cleaner::Cleaner( BaseCycleContainer* input){

  bcc = input;

}

Cleaner::Cleaner(){

  ObjectHandler* objs = ObjectHandler::Instance();
  bcc = objs->GetBaseCycleContainer();

}

void Cleaner::resetEventCalc(){

  EventCalc* evc = EventCalc::Instance();
  evc->Reset();

}

void Cleaner::JetEnergyResolutionShifter(int syst_shift){

  double met = 0;
  if(bcc->met) met=bcc->met->pt();

  for(unsigned int i=0; i<bcc->jets->size(); ++i){
    Jet jet = bcc->jets->at(i);
    float gen_pt = jet.genjet_pt();
    //ignore unmatched jets (which have zero vector) or jets with very low pt:
    if(gen_pt < 15.0) continue;

    met += jet.pt()*jet.JEC_factor_raw();
    
    float recopt = jet.pt();
    double factor = -10;
    double abseta = fabs(jet.eta());

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
    jet.set_pt(jet.pt() * ptscale);
    bcc->jets->at(i).set_pt( jet.pt());

    //propagate JER shifts to MET
    met -= jet.pt()*jet.JEC_factor_raw();
  }
  
  //store changed MET, flip phi if new MET is negative
  if(bcc->met){
    if(met>=0){
      bcc->met->set_pt( met);
    }
    else{
      bcc->met->set_pt( -1*met);
      if(bcc->met->phi()<=0)
	bcc->met->set_phi (bcc->met->phi()+PI);
      else
	bcc->met->set_phi (bcc->met->phi()-PI);
    }
  }

  sort(bcc->jets->begin(), bcc->jets->end(), HigherPt());
  resetEventCalc();
}


void Cleaner::JetLeptonSubtractor(FactorizedJetCorrector *corrector){

  double met = 0;
  if(bcc->met) met=bcc->met->pt();

  for(unsigned int i=0; i<bcc->jets->size(); ++i){

    LorentzVector jet_v4_raw = bcc->jets->at(i).v4()*bcc->jets->at(i).JEC_factor_raw();

    met-=bcc->jets->at(i).pt();
  
    //subtract lepton momenta from raw jet momentum
    
    double ele_energy =  bcc->jets->at(i).chargedEmEnergyFraction()*jet_v4_raw.E();
    double mu_energy = bcc->jets->at(i).muonEnergyFraction()*jet_v4_raw.E();

    if(bcc->electrons){
      for(unsigned int j=0; j<bcc->electrons->size(); ++j){
	if(bcc->jets->at(i).deltaR(bcc->electrons->at(j))<0.5){
	  if(jet_v4_raw.pt() >= bcc->electrons->at(j).pt()){
	    jet_v4_raw -= bcc->electrons->at(j).v4();
	    bcc->jets->at(i).set_electronMultiplicity(bcc->jets->at(i).electronMultiplicity()-1);
	    ele_energy -= bcc->electrons->at(j).energy();
	  }
	  else{
	    //std::cout << "Subtracting electron with pt="<< bcc->electrons->at(j).pt() <<" from jet with pt(corrected)= " << bcc->jets->at(i).pt() << ", pt(raw)=" << jet_v4_raw.pt()  << std::endl;
	    jet_v4_raw -= jet_v4_raw;
	  }
	}
      }
    }
    if(bcc->muons){
      for(unsigned int j=0; j<bcc->muons->size(); ++j){
	if(bcc->jets->at(i).deltaR(bcc->muons->at(j))<0.5){
	  jet_v4_raw -= bcc->muons->at(j).v4();
	  if(jet_v4_raw.pt()>= bcc->muons->at(j).pt()){
	    jet_v4_raw -= bcc->muons->at(j).v4();
	    bcc->jets->at(i).set_muonMultiplicity(bcc->jets->at(i).muonMultiplicity()-1);
	    mu_energy -= bcc->muons->at(j).energy();
	  }
	  else{
	    //std::cout << "Subtracting muon with pt="<< bcc->muons->at(j).pt() <<" from jet with pt(corrected)= " << bcc->jets->at(i).pt() << ", pt(raw)=" << jet_v4_raw.pt()  << std::endl;
	    jet_v4_raw -= jet_v4_raw;	
	  }
	}
      }
    }
    if(ele_energy<=jet_v4_raw.E())
      bcc->jets->at(i).set_chargedEmEnergyFraction(ele_energy/jet_v4_raw.E());
    if(mu_energy<=jet_v4_raw.E())
      bcc->jets->at(i).set_muonEnergyFraction(mu_energy/jet_v4_raw.E());
    

    //apply jet energy corrections to modified raw momentum
    corrector->setJetPt(jet_v4_raw.Pt());
    corrector->setJetEta(jet_v4_raw.Eta());
    corrector->setJetE(jet_v4_raw.E());  
    corrector->setJetA(bcc->jets->at(i).jetArea());
    corrector->setRho(bcc->rho);
    //corrector->setNPV(bcc->pvs->size());	

    float correctionfactor = corrector->getCorrection();
    //std::cout << correctionfactor << "   " << 1./bcc->jets->at(i).JEC_factor_raw() << std::endl;
    LorentzVector jet_v4_corrected = jet_v4_raw *correctionfactor;

    bcc->jets->at(i).set_v4(jet_v4_corrected);
    bcc->jets->at(i).set_JEC_factor_raw(1./correctionfactor);

    met+=bcc->jets->at(i).pt();

  }

  //store changed MET, flip phi if new MET is negative
  if(bcc->met){
    if(met>=0){
      bcc->met->set_pt( met);
    }
    else{
      bcc->met->set_pt( -1*met);
      if(bcc->met->phi()<=0)
	bcc->met->set_phi (bcc->met->phi()+PI);
      else
	bcc->met->set_phi (bcc->met->phi()-PI);
    }
  }

  sort(bcc->jets->begin(), bcc->jets->end(), HigherPt());
  resetEventCalc();
}

//tight ele ID from https://twiki.cern.ch/twiki/bin/view/CMS/EgammaCutBasedIdentification
bool Cleaner::eleID(Electron ele){

  bool pass=false;
  
  if(fabs(ele.supercluster_eta())<1.4442){
    if(ele.dEtaIn()<0.004 && ele.dPhiIn()<0.03 && ele.sigmaIEtaIEta()<0.01 && ele.HoverE()<0.12 && fabs(1./ele.EcalEnergy()-1./ele.v4().P())<0.05) pass=true; 
  }
  else if( fabs(ele.supercluster_eta())>1.5660){
    if(ele.dEtaIn()<0.005 && ele.dPhiIn()<0.02 && ele.sigmaIEtaIEta()<0.03 && ele.HoverE()<0.10 && fabs(1./ele.EcalEnergy()-1./ele.v4().P())<0.05) pass=true; 
  }

  return pass;

}

//pf ID has already been applied when using goodPatJets
bool Cleaner::pfID(Jet jet){

  if(jet.numberOfDaughters()>1 
     && jet.neutralHadronEnergyFraction()<0.99
     && jet.neutralEmEnergyFraction()<0.99){
    
    if(fabs(jet.eta())>=2.4)
      return true;
    
    if(jet.chargedEmEnergyFraction()<0.99
       && jet.chargedHadronEnergyFraction()>0
       && jet.chargedMultiplicity()>0)
      return true;

  }
  return false;
     
}


void Cleaner::ElectronCleaner_noIso(double ptmin, double etamax){

  std::vector<Electron> good_eles;
  for(unsigned int i=0; i<bcc->electrons->size(); ++i){
    Electron ele = bcc->electrons->at(i);
    if(ele.pt()>ptmin){
      if(fabs(ele.eta())<etamax){
  	if(fabs(ele.supercluster_eta())<1.4442 || fabs(ele.supercluster_eta())>1.5660){
  	  if(bcc->pvs->size()>0){
  	    if(ele.gsfTrack_dxy_vertex(bcc->pvs->at(0).x(), bcc->pvs->at(0).y())<0.02){
 	      if(ele.passconversionveto()){
  		if(ele.mvaTrigV0()>0.0){
  		  if(eleID(ele)){
		    good_eles.push_back(ele);
  		  }
  		}
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
  sort(bcc->electrons->begin(), bcc->electrons->end(), HigherPt());
}

void Cleaner::ElectronCleaner(double ptmin, double etamax, double relisomax){

  ElectronCleaner_noIso(ptmin, etamax);
  std::vector<Electron> good_eles;
  for(unsigned int i=0; i<bcc->electrons->size(); ++i){
    Electron ele = bcc->electrons->at(i);
    if(ele.relIso()<relisomax){
      good_eles.push_back(ele);
    }
  }
  bcc->electrons->clear();

  for(unsigned int i=0; i<good_eles.size(); ++i){
    bcc->electrons->push_back(good_eles[i]);
  }
  sort(bcc->electrons->begin(), bcc->electrons->end(), HigherPt());
  resetEventCalc();
}


void Cleaner::MuonCleaner_noIso(double ptmin, double etamax){

  std::vector<Muon> good_mus;
  for(unsigned int i=0; i<bcc->muons->size(); ++i){
    Muon mu = bcc->muons->at(i);
    if(mu.pt()>ptmin){
      if(fabs(mu.eta())<etamax){
	if(mu.isGlobalMuon()){
	  if(mu.globalTrack_chi2()/mu.globalTrack_ndof()<10){
	    if(mu.innerTrack_trackerLayersWithMeasurement()>5){
	      if(mu.dB()<0.02){
		if(fabs(mu.vertex_z()-bcc->pvs->at(0).z())<1){
		  if(mu.innerTrack_numberOfValidPixelHits()>0){
		    if(mu.numberOfMatchedStations()>1){
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
  bcc->muons->clear();

  for(unsigned int i=0; i<good_mus.size(); ++i){
    bcc->muons->push_back(good_mus[i]);
  }
  sort(bcc->muons->begin(), bcc->muons->end(), HigherPt());
  resetEventCalc();
}

void Cleaner::MuonCleaner(double ptmin, double etamax, double relisomax){

  MuonCleaner_noIso(ptmin, etamax);

  std::vector<Muon> good_mus;
  for(unsigned int i=0; i<bcc->muons->size(); ++i){
    Muon mu = bcc->muons->at(i);
    if(mu.relIso()<relisomax){
      good_mus.push_back(mu);
    }
  }
  bcc->muons->clear();

  for(unsigned int i=0; i<good_mus.size(); ++i){
    bcc->muons->push_back(good_mus[i]);
  }
  sort(bcc->muons->begin(), bcc->muons->end(), HigherPt());
}

void Cleaner::TauCleaner(double ptmin, double etamax){

  std::vector<Tau> good_taus;
  for(unsigned int i=0; i<bcc->taus->size(); ++i){
    Tau tau = bcc->taus->at(i);
    if(tau.pt()>ptmin){
      if(fabs(tau.eta())<etamax){
	if(bcc->taus->at(i).decayModeFinding()){
	  if(bcc->taus->at(i).byMediumCombinedIsolationDeltaBetaCorr()){
	    if(bcc->taus->at(i).againstElectronTight()){
	      if(bcc->taus->at(i).againstMuonTight()){
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
  sort(bcc->taus->begin(), bcc->taus->end(), HigherPt());
  resetEventCalc();
}


void Cleaner::JetCleaner(double ptmin, double etamax, bool doPFID){

  std::vector<Jet> good_jets;
  for(unsigned int i=0; i<bcc->jets->size(); ++i){
    Jet jet = bcc->jets->at(i);
    if(jet.pt()>ptmin){
      if(fabs(jet.eta())<etamax){
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
  sort(bcc->jets->begin(), bcc->jets->end(), HigherPt());
  resetEventCalc();
}

void Cleaner::TopJetCleaner(double ptmin, double etamax, bool doPFID){

  std::vector<TopJet> good_topjets;
  for(unsigned int i=0; i<bcc->topjets->size(); ++i){
    TopJet topjet = bcc->topjets->at(i);
    if(topjet.pt()>ptmin){
      if(fabs(topjet.eta())<etamax){
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
  sort(bcc->topjets->begin(), bcc->topjets->end(), HigherPt());
  resetEventCalc();
}


void Cleaner::PrimaryVertexCleaner(int ndofmax, double zmax, double rhomax){

  std::vector<PrimaryVertex> good_pvs;
  for(unsigned int i=0; i<bcc->pvs->size(); ++i){
    PrimaryVertex pv = bcc->pvs->at(i);
    if(pv.ndof()>=ndofmax && fabs(pv.z())<zmax && pv.rho()<=rhomax){
      good_pvs.push_back(pv);
    } 
  }
  bcc->pvs->clear();

  for(unsigned int i=0; i<good_pvs.size(); ++i){
    bcc->pvs->push_back(good_pvs[i]);
  }
  resetEventCalc();
}
