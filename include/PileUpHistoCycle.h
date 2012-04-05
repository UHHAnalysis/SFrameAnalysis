// Dear emacs, this is -*- c++ -*-
// $Id: PileUpHistoCycle.h,v 1.3 2012/04/04 13:08:57 peiffer Exp $
#ifndef PileUpHistoCycle_H
#define PileUpHistoCycle_H

// SFrame include(s):
#include "core/include/SCycleBase.h"

#include "Objects.h"
#include "BaseCycleContainer.h"
#include "Selection.h"

/**
 *   @short Put short description of class here
 *
 *          Put a longer description over here...
 *
 *  @author Put your name here
 * @version $Revision: 1.3 $
 */


class PileUpHistoCycle : public SCycleBase {

public:
   /// Default constructor
   PileUpHistoCycle();
   /// Default destructor
   ~PileUpHistoCycle();

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
  
  // Macro adding the functions for dictionary generation
  ClassDef( PileUpHistoCycle, 0 );

}; // class PileUpHistoCycle

#endif // PileUpHistoCycle_H

