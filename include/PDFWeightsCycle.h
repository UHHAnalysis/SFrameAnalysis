// Dear emacs, this is -*- c++ -*-
// $Id: PDFWeightsCycle.h,v 1.2 2012/05/25 09:32:26 peiffer Exp $
#ifndef PDFWeightsCycle_H
#define PDFWeightsCycle_H

#include <iostream>
#include <fstream>

// SFrame include(s):
#include "core/include/SCycleBase.h"

#include "Objects.h"
#include "BaseCycleContainer.h"
#include "EventCalc.h"
#include "PDFWeights.h"

/**
 *   @short PDFWeights cycle
 *
 *          This cycle runs on un-selected signal events and writes out the total normalization of the sample with
 *          different pdf sets. To be used to study the acceptance change of PDF uncertainties.
 *
 */


class PDFWeightsCycle : public SCycleBase {

public:
   /// Default constructor
   PDFWeightsCycle();
   /// Default destructor
   ~PDFWeightsCycle();

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

  std::string m_pdfname;

  PDFWeights* m_pdfweights;

  std::vector<double> m_sumofweights;
  int m_Ntotal;
  
  // Macro adding the functions for dictionary generation
  ClassDef( PDFWeightsCycle, 0 );

}; // class PDFWeightsCycle

#endif // PDFWeightsCycle_H

