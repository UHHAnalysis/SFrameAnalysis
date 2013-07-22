#ifndef Cleaner_H
#define Cleaner_H

#include "TVector2.h"
#include "Objects.h"
#include "BaseCycleContainer.h"
#include <algorithm>
#include "Utils.h"
#include "FactorizedJetCorrector.h"
#include "JetCorrectionUncertainty.h"
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
   * Use sort=true if you want to re-order the jets according to corrected pt after the applied shifts.
  */
  void JetEnergyResolutionShifter(bool sort=true);
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

  /**
   * Function to pass a jet energy uncertainty object to the cleaner.
   * To apply a variation of the jet energy uncertainty, use the ApplyJECVariationUp or ApplyJECVariationDown.
  */
  void SetJECUncertainty(JetCorrectionUncertainty* jes_unc){m_jec_unc = jes_unc;}
  JetCorrectionUncertainty* GetJECUncertainty(){return m_jec_unc;}

  /**
   * Apply an up variation of the jet energy scale uncertainty.
  */
  void ApplyJECVariationUp(){m_jecvar=e_Up;}
  /**
   * Apply a down variation of the jet energy scale uncertainty.
  */
  void ApplyJECVariationDown(){m_jecvar=e_Down;}
  /**
   * Do not apply jet energy scale variation.
   */
  void NoJECVariation(){m_jecvar=e_Default;}

  /**
   * Apply an up variation of the jet energy resolution uncertainty.
  */
  void ApplyJERVariationUp(){m_jervar=e_Up;}
  /**
   * Apply a down variation of the jet energy resolution uncertainty.
  */
  void ApplyJERVariationDown(){m_jervar=e_Down;}
  /**
   * Do not apply jet energy resolution variation.
   */
  void NoJERVariation(){m_jervar=e_Default;}

  E_SystShift GetJECVariation(){return m_jecvar;}

  void ElectronCleaner(double ptmin=0, double etamax=9999, double relisomax=0.1, bool reverseID = false, bool reverseIso = false);
  void MuonCleaner(double ptmin=0, double etamax=9999, double relisomax=0.125);
  void ElectronCleaner_noIso(double ptmin=0, double etamax=9999, bool reverseID = false);
  void MuonCleaner_noIso(double ptmin=0, double etamax=9999);
  void MuonCleaner_Loose(double ptmin=0, double etamax=9999);
  void ElectronCleaner_noID_noIso(double ptmin=0, double etamax=9999);
  void MuonCleaner_noID_noIso(double ptmin=0, double etamax=9999);
  void TauCleaner(double ptmin=0, double etamax=9999);
  void TauCleanerDecayModeFinding(double ptmin=0, double etamax=9999);
  void JetCleaner(double ptmin=0, double etamax=9999, bool doPFID=true);
  void TopJetCleaner(double ptmin=0, double etamax=9999, bool doPFID=true);
  void JetLeptonOverlapRemoval();
  void PrimaryVertexCleaner(int ndofmax=4, double zmax=24.0, double rhomax=2.0);
  
  /**
   * Function to correct phi modulation of MET in data
  */
  void METPhiCorrector();

 private:

  BaseCycleContainer* bcc;

  JetCorrectionUncertainty* m_jec_unc;

  E_SystShift m_jecvar;  
  E_SystShift m_jervar;

  // helper function to define electron id criteria.
  bool passElectronId(BaseCycleContainer*, unsigned int);

  /// call this routine at the end of each cleaner to force re-calculation of basic variables in EventCalc
  void resetEventCalc();

};


#endif
