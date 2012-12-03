// Dear emacs, this is -*- c++ -*-
#ifndef ConcatSFrameNTupleCycle_H
#define ConcatSFrameNTupleCycle_H

// SFrame include(s):
#include "include/AnalysisCycle.h"

/**
 *  @short Selection cycle to perform 
 *         NTuple concatenation for Z'->ttbar analysis
 *  @author Paul Turner
 */


class ConcatSFrameNTupleCycle : public AnalysisCycle {

public:
  /// Default constructor
  ConcatSFrameNTupleCycle();
  /// Default destructor
  ~ConcatSFrameNTupleCycle();

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
  ClassDef( ConcatSFrameNTupleCycle, 0 );

}; // class ConcatSFrameNTupleCycle

#endif // ConcatSFrameNTupleCycle_H

