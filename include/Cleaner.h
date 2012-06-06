#ifndef Cleaner_H
#define Cleaner_H

#include "Objects.h"
#include "BaseCycleContainer.h"
#include <algorithm>
#include "Utils.h"
#include "FactorizedJetCorrector.h"
#include "ObjectHandler.h"
#include "EventCalc.h"

class Cleaner{

 public:
  Cleaner();
  Cleaner(BaseCycleContainer*);

  ~Cleaner(){};

  //use syst_shift<0 or >0 for systematic uncertainties of the JER shifts
  void JetEnergyResolutionShifter(int syst_shift=0);
  void JetLeptonSubtractor(FactorizedJetCorrector *corrector);

  void ElectronCleaner(double ptmin=0, double etamax=9999, double relisomax=0.1);
  void MuonCleaner(double ptmin=0, double etamax=9999, double relisomax=0.125); 
  void ElectronCleaner_noIso(double ptmin=0, double etamax=9999);
  void MuonCleaner_noIso(double ptmin=0, double etamax=9999);
  void TauCleaner(double ptmin=0, double etamax=9999); 
  void JetCleaner(double ptmin=0, double etamax=9999, bool doPFID=true);
  void TopJetCleaner(double ptmin=0, double etamax=9999, bool doPFID=true); 

  void PrimaryVertexCleaner(int ndofmax=4, double zmax=24.0, double rhomax=2.0);

 private:

  BaseCycleContainer* bcc;

  bool eleID(Electron);
  bool pfID(Jet);

  //call this routine at the end of each cleaner to force re-calculation of basic variables in EventCalc
  void resetEventCalc();

};


#endif
