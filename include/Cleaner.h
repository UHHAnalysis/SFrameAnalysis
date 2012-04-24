#ifndef Cleaner_H
#define Cleaner_H

#include "Objects.h"
#include "BaseCycleContainer.h"
#include <algorithm>


class Cleaner{

 public:
  Cleaner(){};
  Cleaner(BaseCycleContainer*);

  ~Cleaner(){};

  //use syst_shift<0 or >0 for systematic uncertainties of the JER shifts
  void JetEnergyResolutionShifter(int syst_shift=0);

  std::vector<Electron>* ElectronCleaner(double ptmin=0, double etamax=9999);
  std::vector<Muon>* MuonCleaner(double ptmin=0, double etamax=9999); 
  std::vector<Jet>* JetCleaner(double ptmin=0, double etamax=9999, bool doPFID=true);
  std::vector<TopJet>* TopJetCleaner(double ptmin=0, double etamax=9999, bool doPFID=true); 

  struct HigherPt {
    bool operator() (const Particle& j1, const Particle& j2) const {
      return j1.pt > j2.pt;
    };
  };

 private:

  BaseCycleContainer* bcc;

  bool eleID(Electron);
  bool pfID(Jet);



};


#endif
