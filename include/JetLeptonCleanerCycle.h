// Dear emacs, this is -*- c++ -*-
#ifndef JetLeptonCleanerCycle_H
#define JetLeptonCleanerCycle_H

// SFrame include(s):
#include "include/AnalysisCycle.h"
#include "FactorizedJetCorrector.h"
#include "Cleaner.h"
#include "include/SelectionModules.h"
#include "include/JetLeptonCleanerHists.h"
#include "include/EventCalc.h"
#include "JetCorrectorParameters.h"

/**
 *  @short Cycle to study the jet-lepton cleaner
 *    
 *  @author Thomas Peiffer
 */


class JetLeptonCleanerCycle : public AnalysisCycle {

public:
  /// Default constructor
  JetLeptonCleanerCycle();
  /// Default destructor
  ~JetLeptonCleanerCycle();

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

  // Macro adding the functions for dictionary generation
  ClassDef( JetLeptonCleanerCycle, 0 );

}; // class JetLeptonCleanerCycle

#endif // JetLeptonCleanerCycle_H

