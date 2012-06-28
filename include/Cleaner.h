#ifndef Cleaner_H
#define Cleaner_H

#include "TVector2.h"
#include "Objects.h"
#include "BaseCycleContainer.h"
#include <algorithm>
#include "Utils.h"
#include "FactorizedJetCorrector.h"
#include "ObjectHandler.h"
#include "EventCalc.h"

/**
 *  @short Modules to clean event content
 *
 *  Collection of functions that clean lepton and jet collections of the actual BaseCycleContainer. 
 *
 *  @author Thomas Peiffer
 */

class Cleaner{

 public:
  Cleaner();
  Cleaner(BaseCycleContainer*);

  ~Cleaner(){};

  /**
   * Function that corrects the jet momentum to account for differences in jet energy resolution between data and MC. 
   * Changes are propagated to missing transverse energy. To be applied on MC only. 
   * Use the options syst_shift=e_Up and syst_shift=e_Down to shift the jet momenta within their systematic uncertainties.
   * Use sort=true if you want to re-order the jets according to corrected pt after the applied shifts.
  */
  void JetEnergyResolutionShifter(E_SystShift syst_shift=e_Default, bool sort=true);
  /**
   * Function to subtract lepton momenta from jets if the distance between jet and lepton axis is less than 0.5 in the eta-phi plane. 
   * All jets and leptons in the actual BaseCycleContainer are considered.
   * Use sort=false or sort=true if you want to re-order the jets according to corrected pt after the applied shifts.
  */
  void JetLeptonSubtractor(FactorizedJetCorrector *corrector, bool sort=true);

  /**
   * Function to apply new jet corrections to all jets of the BaseCycleContainer.
   * Use sort=false or sort=true if you want to re-order the jets according to corrected pt after the applied shifts.
  */
  void JetRecorrector(FactorizedJetCorrector *corrector, bool sort=true);

  void ElectronCleaner(double ptmin=0, double etamax=9999, double relisomax=0.1);
  void MuonCleaner(double ptmin=0, double etamax=9999, double relisomax=0.125); 
  void ElectronCleaner_noIso(double ptmin=0, double etamax=9999);
  void MuonCleaner_noIso(double ptmin=0, double etamax=9999);
  void ElectronCleaner_noID_noIso(double ptmin=0, double etamax=9999);
  void MuonCleaner_noID_noIso(double ptmin=0, double etamax=9999);
  void TauCleaner(double ptmin=0, double etamax=9999); 
  void JetCleaner(double ptmin=0, double etamax=9999, bool doPFID=true);
  void TopJetCleaner(double ptmin=0, double etamax=9999, bool doPFID=true); 

  void PrimaryVertexCleaner(int ndofmax=4, double zmax=24.0, double rhomax=2.0);

 private:

  BaseCycleContainer* bcc;

  /// call this routine at the end of each cleaner to force re-calculation of basic variables in EventCalc
  void resetEventCalc();

};


#endif
