#ifndef Cleaner_H
#define Cleaner_H

#include "Objects.h"
#include "BaseCycleContainer.h"
#include <algorithm>
#include "Utils.h"

class Cleaner{

 public:
  Cleaner(){};
  Cleaner(BaseCycleContainer*);

  ~Cleaner(){};

  //use syst_shift<0 or >0 for systematic uncertainties of the JER shifts
  void JetEnergyResolutionShifter(int syst_shift=0);

  void ElectronCleaner(double ptmin=0, double etamax=9999);
  void MuonCleaner(double ptmin=0, double etamax=9999); 
  void TauCleaner(double ptmin=0, double etamax=9999); 
  void JetCleaner(double ptmin=0, double etamax=9999, bool doPFID=true);
  void TopJetCleaner(double ptmin=0, double etamax=9999, bool doPFID=true); 

 private:

  BaseCycleContainer* bcc;

  bool eleID(Electron);
  bool pfID(Jet);



};


#endif
