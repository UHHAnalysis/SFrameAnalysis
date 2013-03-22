// Dear emacs, this is -*- c++ -*-
// $Id: GenTTbarCycle.h,v 1.3 2012/12/07 14:21:52 peiffer Exp $
#ifndef GenTTbarCycle_H
#define GenTTbarCycle_H

// SFrame include(s):
#include "include/AnalysisCycle.h"
#include "include/SelectionModules.h"
#include "include/GenTTbarHists.h"
#include "include/ObjectHandler.h"
#include "include/HypothesisHists.h"

class GenTTbarCycle : public AnalysisCycle {

public:
  /// Default constructor
  GenTTbarCycle();
  /// Default destructor
  ~GenTTbarCycle();

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
  ClassDef( GenTTbarCycle, 0 );

}; // class GenTTbarCycle

#endif // GenTTbarCycle_H

