// Dear emacs, this is -*- c++ -*-
#ifndef ZprimeSelectionCycle_H
#define ZprimeSelectionCycle_H

// SFrame include(s):
#include "include/AnalysisCycle.h"
#include "FactorizedJetCorrector.h"
#include "Cleaner.h"
#include "HypothesisDiscriminator.h"
#include "HypothesisHists.h"
#include "include/SelectionModules.h"
#include "include/ObjectHandler.h"
#include "HypothesisStatistics.h"
#include "JetCorrectorParameters.h"

#include "EventHists.h"
#include "JetHists.h"
#include "ElectronHists.h"
#include "MuonHists.h"
#include "TauHists.h"
#include "TopJetHists.h"

/**
 *  @short Selection cycle to perform 
 *         full selection for Z'->ttbar analysis
 *  @author Thomas Peiffer
 */

class ZprimeSelectionCycle : public AnalysisCycle {

public:
  /// Default constructor
  ZprimeSelectionCycle();
  /// Default destructor
  ~ZprimeSelectionCycle();

  /// Function called at the beginning of the cycle
  void BeginCycle() throw( SError );
  /// Function called at the end of the cycle
  void EndCycle() throw( SError );

  /// Function called at the beginning of a new input data
  void BeginInputData( const SInputData& ) throw( SError );
  /// Function called after finishing to process an input data
  void EndInputData  ( const SInputData& ) throw( SError );

  /// Function called after opening each new input file
  void BeginInputFile( const SInputData& ) throw( SError );

  /// Function called for every event
  void ExecuteEvent( const SInputData&, Double_t ) throw( SError );

  /// Fill control histograms
  void FillControlHists(TString postfix="");

private:
  //
  // Put all your private variables here
  //
  
  std::string m_Electron_Or_Muon_Selection;
 
  int m_Nbtags_max;
  int m_Nbtags_min;  

  FactorizedJetCorrector* m_corrector;
  Cleaner* m_cleaner;
  Chi2Discriminator* m_chi2discr;
  BestPossibleDiscriminator* m_bpdiscr;
  SumDeltaRDiscriminator* m_sumdrdiscr;
  CorrectMatchDiscriminator* m_cmdiscr;

  HypothesisStatistics* m_bp_chi2;
  HypothesisStatistics* m_bp_sumdr;
  HypothesisStatistics* m_cm_chi2;
  HypothesisStatistics* m_cm_sumdr; 
  HypothesisStatistics* m_cm_bp; 

  // Macro adding the functions for dictionary generation
  ClassDef( ZprimeSelectionCycle, 0 );

}; // class ZprimeSelectionCycle

#endif // ZprimeSelectionCycle_H

