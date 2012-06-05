// Dear emacs, this is -*- c++ -*-
// $Id: CycleCreators.py 159 2010-04-13 09:44:22Z krasznaa $
#ifndef ExampleCycle_H
#define ExampleCycle_H

// SFrame include(s):
#include "include/AnalysisCycle.h"

/**
 *   @short Example of an analysis cycle
 *
 *          This is an example of an analysis cycle. It can be used
 *          as a template for writing your own analysis. Also should
 *          be used for quick cross checks of the system setup.
 *
 *  @author Roman Kogler
 *  @version $Revision: 159 $
 */

class ExampleCycle : public AnalysisCycle {

public:
  /// Default constructor
  ExampleCycle();
  /// Default destructor
  ~ExampleCycle();

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
  ClassDef( ExampleCycle, 0 );

}; // class ExampleCycle

#endif // ExampleCycle_H

