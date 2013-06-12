// Dear emacs, this is -*- c++ -*-
#ifndef ZprimePreSelectionCycle_H
#define ZprimePreSelectionCycle_H

// SFrame include(s):
#include "include/AnalysisCycle.h"
#include "Cleaner.h"

/**
 *  @short Selection cycle to perform 
 *         pre-selection for Z'->ttbar analysis
 *  @author Thomas Peiffer
 */


class ZprimePreSelectionCycle : public AnalysisCycle {

public:
  /// Default constructor
  ZprimePreSelectionCycle();
  /// Default destructor
  ~ZprimePreSelectionCycle();

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
  
  // Flg use to reverse electron selection
  bool m_reversed_electron_selection;

  std::string m_Electron_Or_Muon_Selection;

  // Macro adding the functions for dictionary generation
  ClassDef( ZprimePreSelectionCycle, 0 );

}; // class ZprimePreSelectionCycle

#endif // ZprimePreSelectionCycle_H

