// Dear emacs, this is -*- c++ -*-
// $Id: TauEffiCycle.h,v 1.1 2012/04/23 12:51:30 peiffer Exp $
#ifndef TauEffiCycle_H
#define TauEffiCycle_H

// SFrame include(s):
#include "core/include/SCycleBase.h"
#include "SFrameTools/include/BaseCycleContainer.h"


/**
 *   @short Put short description of class here
 *
 *          Put a longer description over here...
 *
 *  @author Put your name here
 * @version $Revision: 1.1 $
 */


class TauEffiCycle : public SCycleBase {

public:
   /// Default constructor
   TauEffiCycle();
   /// Default destructor
   ~TauEffiCycle();

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
  ClassDef( TauEffiCycle, 0 );

}; // class TauEffiCycle

#endif // TauEffiCycle_H

