// Dear emacs, this is -*- c++ -*-
// $Id: cutflowCycle.h,v 1.1 2012/06/05 14:45:24 rkogler Exp $
#ifndef cutflowCycle_H
#define cutflowCycle_H

#include "SFrameAnalysis/include/AnalysisCycle.h"
#include "SFrameAnalysis/include/BTagEffHistsTPrime.h"

/**
 *   @short Example of an analysis cycle
 *
 *          This is an example of an analysis cycle. It can be used
 *          as a template for writing your own analysis. Also should
 *          be used for quick cross checks of the system setup.
 *
 *  @author Roman Kogler
 *  @version $Revision: 1.1 $
 */

class cutflowCycle : public AnalysisCycle {

public:
  /// Default constructor
  cutflowCycle();
  /// Default destructor
  ~cutflowCycle();

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
  // std::string m_BTagEffiFilenameMC;
//   std::string m_BTaggingMode;
//   BTaggingRoutine* m_bsf;
  
  // Macro adding the functions for dictionary generation
  ClassDef( cutflowCycle, 0 );

}; // class cutflowCycle

#endif // cutflowCycle_H

