#include "../include/Cleaner.h"
#include "../../SFrameTools/include/Utils.h"

Cleaner::Cleaner( BaseCycleContainer* input)
{

    bcc = input;
    m_jec_unc = NULL;
    m_jecvar = e_Default;
    m_jervar = e_Default;

}

Cleaner::Cleaner()
{

    EventCalc* calc = EventCalc::Instance();
    bcc = calc->GetBaseCycleContainer();
    m_jec_unc = NULL;
    m_jecvar = e_Default;
    m_jervar = e_Default;

}

void Cleaner::resetEventCalc()
{

    //reset everything in EventCalc except the event weight
    EventCalc* evc = EventCalc::Instance();
    double weight = evc->GetWeight();
    evc->Reset();
    evc->ProduceWeight(weight);

}

void Cleaner::JetEnergyResolutionShifter(bool sort)
{

    LorentzVector met(0,0,0,0);
    if(bcc->met) {
        met = bcc->met->v4();
    }

    for(unsigned int i=0; i<bcc->jets->size(); ++i) {

      //std::cout << "ResolutionShifter start: Jet " << i << ", pt = " << bcc->jets->at(i).pt() << " correction factor = " << 1./bcc->jets->at(i).JEC_factor_raw() <<  std::endl;

        float genpt = bcc->jets->at(i).genjet_pt();
        //ignore unmatched jets (which have zero vector) or jets with very low pt:
        if(genpt < 15.0) {
            // std::cout << "1.0 | " <<  bcc->jets->at(i).pt()  << " | " << bcc->jets->at(i).eta() << " | " << genpt << std::endl;
            continue;
        }

        LorentzVector jet_v4 =  bcc->jets->at(i).v4();

        LorentzVector jet_v4_raw = jet_v4*bcc->jets->at(i).JEC_factor_raw();

        double recopt = jet_v4.pt();
        double factor = 0.0;
	double uperr = 0.0;
	double downerr = 0.0;
        double abseta = fabs(jet_v4.eta());

        //numbers taken from https://twiki.cern.ch/twiki/bin/view/CMS/JetResolution
        if(m_jervar==e_Default) {
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
            else
                factor = 0.288;
        } else if(m_jervar==e_Up) {
            if(abseta < 0.5)
                factor = 0.115;
            else if(abseta >= 0.5 && abseta <1.1)
                factor = 0.114;
            else if(abseta >= 1.1 && abseta <1.7)
                factor = 0.161;
            else if(abseta >= 1.7 && abseta <2.3)
                factor = 0.228;
            else if(abseta >= 2.3)
                factor = 0.488;
        } else if(m_jervar==e_Down) {
            if(abseta < 0.5)
                factor = -0.01;
            else if(abseta >= 0.5 && abseta <1.1)
                factor = 0.00;
            else if(abseta >= 1.1 && abseta <1.7)
                factor = 0.032;
            else if(abseta >= 1.7 && abseta <2.3)
                factor = 0.042;
            else if(abseta >= 2.3)
                factor = 0.089;
        }

        double ptscale = std::max(0.0, 1 + factor * (recopt - genpt) / recopt);

        jet_v4*=ptscale;

        bcc->jets->at(i).set_v4(jet_v4);
	
        //propagate JER shifts to MET
        met += jet_v4_raw;
        jet_v4_raw*=ptscale;
        met -= jet_v4_raw;
    }

    //store changed MET
    if(bcc->met) {
        bcc->met->set_pt(met.Pt());
        bcc->met->set_phi(met.Phi());
    }

    if(sort) std::sort(bcc->jets->begin(), bcc->jets->end(), HigherPt());
    resetEventCalc();
}


void Cleaner::JetLeptonSubtractor(FactorizedJetCorrector *corrector, bool sort)
{

    //std::cout<< "event: " <<bcc->event <<std::endl;
    //std::cout<< "ID| pt raw  | PAT pt | area     | rho     | corr    | old cor| JER ptscale | pt new | eta new | ptgen " <<std::endl;

    if (!corrector){
      std::cerr << "JetLeptonSubtractor: called without a valid JetCorrector! Please correct the error." << std::endl; 
      std::cerr << "Hint: have you supplied correct JEC files in your xml steering?" << std::endl;
      exit(EXIT_FAILURE);
    }
  

    for(unsigned int i=0; i<bcc->jets->size(); ++i) {
      
        LorentzVector jet_v4_raw = bcc->jets->at(i).v4()*bcc->jets->at(i).JEC_factor_raw();

        //subtract lepton momenta from raw jet momentum

        //std::cout << i+1 << " | " << jet_v4_raw.Pt() << std::endl;
        double ele_energy =  bcc->jets->at(i).chargedEmEnergyFraction()*jet_v4_raw.E();
        double mu_energy = bcc->jets->at(i).muonEnergyFraction()*jet_v4_raw.E();

        if(bcc->electrons) {
            for(unsigned int j=0; j<bcc->electrons->size(); ++j) {
                if(bcc->jets->at(i).deltaR(bcc->electrons->at(j))<0.5) {
//       if(jet_v4_raw.pt() >= bcc->electrons->at(j).pt()){
                    jet_v4_raw -= bcc->electrons->at(j).v4();
//  	  bcc->jets->at(i).set_electronMultiplicity(bcc->jets->at(i).electronMultiplicity()-1);
//  	  ele_energy -= bcc->electrons->at(j).energy();
                    //}
                    //else{
                    //std::cout  << bcc->jets->at(i).pt()<< " -ele pt:"<< bcc->electrons->at(j).pt() << std::endl;
                    //jet_v4_raw -= jet_v4_raw;
                    //}
                }
            }
        }
        if(bcc->muons) {
            for(unsigned int j=0; j<bcc->muons->size(); ++j) {
                if(bcc->jets->at(i).deltaR(bcc->muons->at(j))<0.5) {
                    //if(jet_v4_raw.pt()>= bcc->muons->at(j).pt()){
                    jet_v4_raw -= bcc->muons->at(j).v4();
//  	  bcc->jets->at(i).set_muonMultiplicity(bcc->jets->at(i).muonMultiplicity()-1);
//  	  mu_energy -= bcc->muons->at(j).energy();
                    //}
                    //else{
                    //std::cout << bcc->jets->at(i).pt() << " -mu pt:"<< bcc->muons->at(j).pt()  << std::endl;
                    //jet_v4_raw -= jet_v4_raw;
                    //}
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
        corrector->setNPV(bcc->pvs->size());

        double correctionfactor = corrector->getCorrection();

        LorentzVector jet_v4_corrected = jet_v4_raw *correctionfactor;

	if (m_jecvar != e_Default){
	  if (m_jec_unc==NULL){
	    std::cerr << "JEC variation should be applied, but JEC uncertainty object is NULL! Abort." << std::endl;
	    exit(EXIT_FAILURE);
	  }
	  // ignore jets with very low pt or high eta, avoiding a crash from the JESUncertainty tool
	  double pt = jet_v4_corrected.Pt();
	  double eta = jet_v4_corrected.Eta();
	  if (pt<5. || fabs(eta)>5.) continue;
	  	  
	  m_jec_unc->setJetEta(eta);
	  m_jec_unc->setJetPt(pt);

	  double unc = 0.;	  
	  if (m_jecvar == e_Up){
	    unc = m_jec_unc->getUncertainty(1);
	    correctionfactor *= (1 + fabs(unc));
	  } else if (m_jecvar == e_Down){
	    unc = m_jec_unc->getUncertainty(-1);
	    correctionfactor *= (1 - fabs(unc));
	  }
	  jet_v4_corrected = jet_v4_raw * correctionfactor;
	}

        //std::cout << i+1 << " | " << jet_v4_raw.Pt() << " | " <<  bcc->jets->at(i).pt() << " | " << bcc->jets->at(i).jetArea()<< " | " << bcc->rho << " | " << correctionfactor  << " | " << 1./bcc->jets->at(i).JEC_factor_raw() << " | " << std::endl;

        bcc->jets->at(i).set_v4(jet_v4_corrected);
        bcc->jets->at(i).set_JEC_factor_raw(1./correctionfactor);
    }

    if(sort) std::sort(bcc->jets->begin(), bcc->jets->end(), HigherPt());
    resetEventCalc();
}

void Cleaner::JetRecorrector( FactorizedJetCorrector *corrector, bool sort, bool useTopJets, bool propagate_to_met, bool useTopTagJets, bool useHiggsTagJets)
{
    
  if(useTopJets) propagate_to_met = false;
  if(useTopTagJets) propagate_to_met = false;
  if(useHiggsTagJets) propagate_to_met = false;

  std::vector<Jet*> jets;
  if(useTopJets){
    for(unsigned int i=0; i<bcc->topjets->size(); ++i) {
      Jet* jet = &bcc->topjets->at(i);
      jets.push_back( jet );
    }
  }
  else if(useTopTagJets){
    for(unsigned int i=0; i<bcc->toptagjets->size(); ++i) {
      Jet* jet = &bcc->toptagjets->at(i);
      jets.push_back( jet );
    }
  }
  else if(useHiggsTagJets){
    for(unsigned int i=0; i<bcc->higgstagjets->size(); ++i) {
      Jet* jet = &bcc->higgstagjets->at(i);
      jets.push_back( jet );
    }
  }
  else{
    for(unsigned int i=0; i<bcc->jets->size(); ++i) {
      Jet* jet = &bcc->jets->at(i);
      jets.push_back( jet );
    }
  }
  
    for(unsigned int i=0; i<jets.size(); ++i) {
        LorentzVector jet_v4_raw = jets.at(i)->v4()*jets.at(i)->JEC_factor_raw();
        corrector->setJetPt(jet_v4_raw.Pt());
        corrector->setJetEta(jet_v4_raw.Eta());
        corrector->setJetE(jet_v4_raw.E());
        corrector->setJetA(jets.at(i)->jetArea());
        corrector->setRho(bcc->rho);
        corrector->setNPV(bcc->pvs->size());

        float correctionfactor = corrector->getCorrection();

        LorentzVector jet_v4_corrected = jet_v4_raw *correctionfactor;

	if (m_jecvar != e_Default){
	  if (m_jec_unc==NULL){
	    std::cerr << "JEC variation should be applied, but JEC uncertainty object is NULL! Abort." << std::endl;
	    exit(EXIT_FAILURE);
	  }
	  m_jec_unc->setJetEta(jet_v4_corrected.Eta());
	  m_jec_unc->setJetPt(jet_v4_corrected.Pt());
	  double unc = 0.;	  
	  if (m_jecvar == e_Up){
	    unc = m_jec_unc->getUncertainty(1);
	    correctionfactor *= (1 + fabs(unc));
	  } else if (m_jecvar == e_Down){
	    unc = m_jec_unc->getUncertainty(-1);
	    correctionfactor *= (1 - fabs(unc));
	  }
	  jet_v4_corrected = jet_v4_raw * correctionfactor;
	}
	
	
	// see https://twiki.cern.ch/twiki/bin/view/CMS/METType1Type2Formulae : we add the old corrected jet and 
        // subtract the new corrected (smeared) jet v4 from/to MET, if the corrected pt is > 10GeV.
        // Note that this implementation does not do exactly the same as re-applying typeI corrections of the new corrected jets to raw met as it does not
        // consider the cases in which the new corrections makes some jets flip-flop over the pt = 10GeV threshold used in the typeI-met correction.
        // To consider that, we would need the pure L1 corrected jet here as well ...
	if(propagate_to_met){
            if(jets.at(i)->v4().Pt() > 10){
                LorentzVector metv4 = bcc->met->v4();
                metv4 += jets.at(i)->v4();
                metv4 -= jet_v4_corrected;
                bcc->met->set_pt(metv4.Pt());
                bcc->met->set_phi(metv4.Phi());
            }
        }

        jets.at(i)->set_v4(jet_v4_corrected);
        jets.at(i)->set_JEC_factor_raw(1./correctionfactor);
    }

    if(sort){
      if(useTopJets) std::sort(bcc->topjets->begin(), bcc->topjets->end(), HigherPt()); 
      else if(useTopTagJets) std::sort(bcc->toptagjets->begin(), bcc->toptagjets->end(), HigherPt()); 
      else if(useHiggsTagJets) std::sort(bcc->higgstagjets->begin(), bcc->higgstagjets->end(), HigherPt()); 
      else std::sort(bcc->jets->begin(), bcc->jets->end(), HigherPt());
    }
    resetEventCalc();
}

// Old electron id
/*
bool Cleaner::passElectronId(BaseCycleContainer * bcc, unsigned int index)
{
    Electron ele = bcc->electrons->at(index);
    if(fabs(ele.supercluster_eta())<1.4442 || fabs(ele.supercluster_eta())>1.5660) {
        if(bcc->pvs->size()>0) {
            if(fabs(ele.gsfTrack_dxy_vertex(bcc->pvs->at(0).x(), bcc->pvs->at(0).y()))<0.02) {
                if(fabs(ele.gsfTrack_dz_vertex(bcc->pvs->at(0).x(), bcc->pvs->at(0).y(), bcc->pvs->at(0).z()))<0.1) {
                    if(ele.passconversionveto()) {
                        if(ele.mvaTrigV0()>0.0) {
                            if(ele.eleID(Electron::e_Tight)) {
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }   
    return false;
}*/

// Base on the new top simple cut recomendation for 53x
 /*
bool Cleaner::passElectronId(BaseCycleContainer * bcc, unsigned int index)
{
    Electron ele = bcc->electrons->at(index);
    if(fabs(ele.supercluster_eta())<1.4442 || fabs(ele.supercluster_eta())>1.5660) {
        if(bcc->pvs->size()>0) {
            if(fabs(ele.gsfTrack_dxy_vertex(bcc->pvs->at(0).x(), bcc->pvs->at(0).y()))<0.02) {
                if(fabs(ele.gsfTrack_dz_vertex(bcc->pvs->at(0).x(), bcc->pvs->at(0).y(), bcc->pvs->at(0).z()))<0.1) {
		  if(ele.passconversionveto()) {
		      if(ele.eleID(Electron::e_Tight)) {
			return true;
		      }
		  }
                }
            }
        }
    }
    return false;
}
 */
// Base on the new top mva recomendation for 53x 


bool Cleaner::passElectronId(BaseCycleContainer * bcc, unsigned int index)
{
    Electron ele = bcc->electrons->at(index);
    if(fabs(ele.supercluster_eta())<1.4442 || fabs(ele.supercluster_eta())>1.5660) {
        if(bcc->pvs->size()>0) {
            if(fabs(ele.gsfTrack_dxy_vertex(bcc->pvs->at(0).x(), bcc->pvs->at(0).y()))<0.02) {
                if(ele.passconversionveto()) {
                    if(ele.mvaTrigV0()>0.9) {
                        if (ele.gsfTrack_trackerExpectedHitsInner_numberOfLostHits() <= 0) {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}


void Cleaner::ElectronCleaner_noID_noIso(double ptmin, double etamax)
{
    std::vector<Electron> good_eles;
    for(unsigned int i=0; i<bcc->electrons->size(); ++i) {
        Electron ele = bcc->electrons->at(i);
        if(ele.pt()>ptmin) {
            if(fabs(ele.eta())<etamax) {
                good_eles.push_back(ele);
            }
        }
    }

    bcc->electrons->clear();

    for(unsigned int i=0; i<good_eles.size(); ++i) {
        bcc->electrons->push_back(good_eles[i]);
    }
    sort(bcc->electrons->begin(), bcc->electrons->end(), HigherPt());
    resetEventCalc();
}

void Cleaner::ElectronCleaner_noIso(double ptmin, double etamax, bool reverseID)
{
    ElectronCleaner_noID_noIso(ptmin, etamax);
    std::vector<Electron> good_eles;
    for(unsigned int i=0; i<bcc->electrons->size(); ++i) {
        Electron ele = bcc->electrons->at(i);
        if(!reverseID && passElectronId(bcc, i))
            good_eles.push_back(ele);
        else if (reverseID && !passElectronId(bcc, i))
            good_eles.push_back(ele);
    }
    bcc->electrons->clear();

    for(unsigned int i=0; i<good_eles.size(); ++i) {

        bcc->electrons->push_back(good_eles[i]);
    }
    sort(bcc->electrons->begin(), bcc->electrons->end(), HigherPt());
    resetEventCalc();
}


void Cleaner::ElectronCleaner(double ptmin, double etamax, double relisomax, bool reverseID, bool reverseIso)
{
    ElectronCleaner_noIso(ptmin, etamax, reverseID);
    std::vector<Electron> good_eles;
    for(unsigned int i=0; i<bcc->electrons->size(); ++i) {
        Electron ele = bcc->electrons->at(i);
        if(!reverseIso && ele.relIsorho(bcc->rho)<relisomax)
            good_eles.push_back(ele);
        else if (reverseIso && ele.relIsorho(bcc->rho)>=relisomax)
            good_eles.push_back(ele);
    }
    bcc->electrons->clear();

    for(unsigned int i=0; i<good_eles.size(); ++i) {
        bcc->electrons->push_back(good_eles[i]);
    }
    sort(bcc->electrons->begin(), bcc->electrons->end(), HigherPt());
    resetEventCalc();
}


void Cleaner::MuonCleaner_noID_noIso(double ptmin, double etamax)
{
    std::vector<Muon> good_mus;
    for(unsigned int i=0; i<bcc->muons->size(); ++i) {
        Muon mu = bcc->muons->at(i);
        if(mu.pt()>ptmin) {
            if(fabs(mu.eta())<etamax) {
                good_mus.push_back(mu);
            }
        }
    }
    bcc->muons->clear();

    for(unsigned int i=0; i<good_mus.size(); ++i) {
        bcc->muons->push_back(good_mus[i]);
    }
    sort(bcc->muons->begin(), bcc->muons->end(), HigherPt());
    resetEventCalc();
}

void Cleaner::MuonCleaner_noIso(double ptmin, double etamax)
{

    MuonCleaner_noID_noIso(ptmin, etamax);
    std::vector<Muon> good_mus;
    for(unsigned int i=0; i<bcc->muons->size(); ++i) {
        Muon mu = bcc->muons->at(i);
        if(mu.isGlobalMuon()) {
            if(mu.isPFMuon()) {
                if(mu.globalTrack_chi2()/mu.globalTrack_ndof()<10) {
                    if(mu.globalTrack_numberOfValidMuonHits()>0) {
                        if(mu.innerTrack_trackerLayersWithMeasurement()>5) {
                            if(mu.dB()<0.2) { 
                                if(fabs(mu.vertex_z()-bcc->pvs->at(0).z())<0.5) {
                                    if(mu.innerTrack_numberOfValidPixelHits()>0) {
                                        if(mu.numberOfMatchedStations()>1) {
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

    for(unsigned int i=0; i<good_mus.size(); ++i) {
        bcc->muons->push_back(good_mus[i]);
    }
    sort(bcc->muons->begin(), bcc->muons->end(), HigherPt());
    resetEventCalc();
}

void Cleaner::MuonCleaner(double ptmin, double etamax, double relisomax)
{

    MuonCleaner_noIso(ptmin, etamax);
    std::vector<Muon> good_mus;
    for(unsigned int i=0; i<bcc->muons->size(); ++i) {
        Muon mu = bcc->muons->at(i);
        if(mu.relIso()<relisomax) {
            good_mus.push_back(mu);
        }
    }
    bcc->muons->clear();

    for(unsigned int i=0; i<good_mus.size(); ++i) {
        bcc->muons->push_back(good_mus[i]);
    }
    sort(bcc->muons->begin(), bcc->muons->end(), HigherPt());
    resetEventCalc();
}

void Cleaner::MuonCleaner_Loose(double ptmin, double etamax)
{

    MuonCleaner_noID_noIso(ptmin, etamax);
    std::vector<Muon> good_mus;
    for(unsigned int i=0; i<bcc->muons->size(); ++i) 
      {
        Muon mu = bcc->muons->at(i);
        if(mu.isPFMuon()) 
	  {
	    if(mu.isGlobalMuon() || mu.isTrackerMuon())
	      {
		good_mus.push_back(mu);	
	      }
	  }
      }
    
    bcc->muons->clear();

    for(unsigned int i=0; i<good_mus.size(); ++i) {
        bcc->muons->push_back(good_mus[i]);
    }
    sort(bcc->muons->begin(), bcc->muons->end(), HigherPt());
    resetEventCalc();
}

void Cleaner::TauCleaner_noIso(double ptmin, double etamax)
{
  std::vector<Tau> good_taus;
  for(unsigned int i=0; i<bcc->taus->size(); ++i) {
    Tau tau = bcc->taus->at(i);
    if(tau.pt()>ptmin) {
      if(fabs(tau.eta())<etamax) {
	if(bcc->taus->at(i).decayModeFinding()) {
	  if(bcc->taus->at(i).againstElectronTightMVA3()) {
	    if(bcc->taus->at(i).againstMuonTight2()) {
	      double deltaRmin = 100;
	      for(unsigned int k=0; k<bcc->muons->size(); ++k) 
		{
		  Muon muon = bcc->muons->at(k);
		  double deltaR = muon.deltaR(tau);
		  if (deltaR < deltaRmin) deltaRmin = deltaR;
		}
	      if (deltaRmin > 0.5)
		{
		  good_taus.push_back(tau);
		}
	    }
	  }
	}
      }
    }
  }
  
  bcc->taus->clear();
  
  for(unsigned int i=0; i<good_taus.size(); ++i) {
    bcc->taus->push_back(good_taus[i]);
  }
  sort(bcc->taus->begin(), bcc->taus->end(), HigherPt());
  resetEventCalc();
}



void Cleaner::TauCleaner(double ptmin, double etamax)
{
  TauCleaner_noIso(ptmin, etamax);
  std::vector<Tau> good_taus;
  for(unsigned int i=0; i<bcc->taus->size(); ++i) {
    Tau tau = bcc->taus->at(i);
    if(bcc->taus->at(i).byMediumCombinedIsolationDeltaBetaCorr()) {
      good_taus.push_back(tau);
    }
  }
  
  bcc->taus->clear();
  
  for(unsigned int i=0; i<good_taus.size(); ++i) {
    bcc->taus->push_back(good_taus[i]);
  }
  sort(bcc->taus->begin(), bcc->taus->end(), HigherPt());
  resetEventCalc();
}


void Cleaner::JetCleaner(double ptmin, double etamax, bool doPFID)
{

    std::vector<Jet> good_jets;
    for(unsigned int i=0; i<bcc->jets->size(); ++i) {
        Jet jet = bcc->jets->at(i);
        if(jet.pt()>ptmin) {
            if(fabs(jet.eta())<etamax) {
                if(!doPFID || jet.pfID()) {
                    good_jets.push_back(jet);
                }
            }
        }
    }

    bcc->jets->clear();

    for(unsigned int i=0; i<good_jets.size(); ++i) {
        bcc->jets->push_back(good_jets[i]);
    }
    sort(bcc->jets->begin(), bcc->jets->end(), HigherPt());
    resetEventCalc();
}

void Cleaner::JetLeptonOverlapRemoval()
{
  
  std::vector<Jet> good_jets;
  for(unsigned int i=0; i<bcc->jets->size(); ++i) 
    {
      Jet jet = bcc->jets->at(i);
      double deltaRmin = 100;
      for(unsigned int j=0; j<bcc->taus->size(); ++j) 
	{
	  Tau tau = bcc->taus->at(j);
	  double deltaR = tau.deltaR(jet);
	  if (deltaR < deltaRmin) deltaRmin = deltaR;
	}
      if (deltaRmin > 0.5)
	{
	  deltaRmin = 100;
	  for(unsigned int k=0; k<bcc->muons->size(); ++k) 
	    {
	      Muon muon = bcc->muons->at(k);
	      double deltaR = muon.deltaR(jet);
	      if (deltaR < deltaRmin) deltaRmin = deltaR;
	    }
	  if (deltaRmin > 0.5)
	    {
	      good_jets.push_back(jet);
	    }
	}  
      
    }	     
  
  bcc->jets->clear();
  
  for(unsigned int i=0; i<good_jets.size(); ++i) {
    bcc->jets->push_back(good_jets[i]);
  }
  sort(bcc->jets->begin(), bcc->jets->end(), HigherPt());
  resetEventCalc();
}



void Cleaner::TauCleanerDecayModeFinding(double ptmin, double etamax)
{
  std::vector<Tau> good_taus;
  for(unsigned int i=0; i<bcc->taus->size(); ++i) 
    {
      Tau tau = bcc->taus->at(i);
      if(tau.pt()>ptmin) 
	{
	  if(fabs(tau.eta())<etamax) 
	    {
	      if(bcc->taus->at(i).decayModeFinding()) 
		{
		  good_taus.push_back(tau);
		}
	    }
	}
    }
  
  bcc->taus->clear();
  
  for(unsigned int i=0; i<good_taus.size(); ++i) {
    bcc->taus->push_back(good_taus[i]);
  }
  sort(bcc->taus->begin(), bcc->taus->end(), HigherPt());
  resetEventCalc();
  
}


void Cleaner::TopJetCleaner(double ptmin, double etamax, bool doPFID)
{

    std::vector<TopJet> good_topjets;
    for(unsigned int i=0; i<bcc->topjets->size(); ++i) {
        TopJet topjet = bcc->topjets->at(i);
        if(topjet.pt()>ptmin) {
            if(fabs(topjet.eta())<etamax) {
                if(!doPFID || topjet.pfID()) {
                    good_topjets.push_back(topjet);
                }
            }
        }
    }
    bcc->topjets->clear();

    for(unsigned int i=0; i<good_topjets.size(); ++i) {
        bcc->topjets->push_back(good_topjets[i]);
    }
    sort(bcc->topjets->begin(), bcc->topjets->end(), HigherPt());
    resetEventCalc();
}


void Cleaner::PrimaryVertexCleaner(int ndofmax, double zmax, double rhomax)
{

    std::vector<PrimaryVertex> good_pvs;
    for(unsigned int i=0; i<bcc->pvs->size(); ++i) {
        PrimaryVertex pv = bcc->pvs->at(i);
        if(pv.ndof()>=ndofmax && fabs(pv.z())<zmax && pv.rho()<=rhomax) {
            good_pvs.push_back(pv);
        }
    }
    bcc->pvs->clear();

    for(unsigned int i=0; i<good_pvs.size(); ++i) {
        bcc->pvs->push_back(good_pvs[i]);
    }
    resetEventCalc();
}

void Cleaner::METPhiCorrector()
{
  
  float met_x = bcc->met->v4().x();
  float met_y = bcc->met->v4().y();

  met_x += 0.2661+0.3217*bcc->pvs->size();
  met_y += -0.2252-0.1747*bcc->pvs->size();

  float met = sqrt(met_x*met_x + met_y*met_y);
  float phi = atan(met_y/met_x);

  bcc->met->set_phi(phi);
  bcc->met->set_pt(met);
  
  resetEventCalc();
}
