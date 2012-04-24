// Dear emacs, this is -*- c++ -*-
// $Id: LeptoquarkCycle.h,v 1.1 2012/04/23 12:21:06 mmeyer Exp $
#ifndef LeptoquarkCycle_H
#define LeptoquarkCycle_H

// SFrame include(s):
#include "core/include/SCycleBase.h"

#include "Objects.h"
#include "BaseCycleContainer.h"
#include "Selection.h"
#include "TH2.h"
#include "TLorentzVector.h"
#include "Cleaner.h"
#include "FJet.h"

/**
 *   @short Put short description of class here
 *
 *          Put a longer description over here...
 *
 *  @author Put your name here
 * @version $Revision: 1.1 $
 */


class LeptoquarkCycle : public SCycleBase {

public:
   /// Default constructor
   LeptoquarkCycle();
   /// Default destructor
   ~LeptoquarkCycle();

   /// Function called at the beginning of the cycle
   virtual void BeginCycle() throw( SError );
   /// Function called at the end of the cycle
   virtual void EndCycle() throw( SError );

   /// Function called at the beginning of a new input data
   virtual void BeginInputData( const SInputData& ) throw( SError );
   /// Function called after finishing to process an input data
   virtual void EndInputData  ( const SInputData& ) throw( SError );

   /// Function called after opening each new input file
   virtual void BeginInputFile( const SInputData& ) throw( SError );

   /// Function called for every event
   virtual void ExecuteEvent( const SInputData&, Double_t ) throw( SError );

private:
   //
   // Put all your private variables here
   //
  BaseCycleContainer bcc;
  int elec;
  int muon;
  int had;
  
  
  // Macro adding the functions for dictionary generation
  ClassDef( LeptoquarkCycle, 0 );

}; // class PileUpHistoCycle

#endif // PileUpHistoCycle_H

