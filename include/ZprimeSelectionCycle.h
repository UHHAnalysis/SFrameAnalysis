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
#include "JetCorrectorParameters.h"

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

private:
  //
  // Put all your private variables here
  //
  
  FactorizedJetCorrector* m_corrector;
  Cleaner* m_cleaner;
  Chi2Discriminator* m_chi2discr;
  BestPossibleDiscriminator* m_bpdiscr;

  // Macro adding the functions for dictionary generation
  ClassDef( ZprimeSelectionCycle, 0 );

}; // class ZprimeSelectionCycle

#endif // ZprimeSelectionCycle_H

