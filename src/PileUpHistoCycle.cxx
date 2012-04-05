// $Id: PileUpHistoCycle.cxx,v 1.3 2012/04/04 13:08:56 peiffer Exp $

// Local include(s):
#include "../include/PileUpHistoCycle.h"

ClassImp( PileUpHistoCycle );


PileUpHistoCycle::PileUpHistoCycle()
   : SCycleBase() {

   SetLogName( GetName() );
}

PileUpHistoCycle::~PileUpHistoCycle() {

}

void PileUpHistoCycle::BeginCycle() throw( SError ) {

  return;

}

void PileUpHistoCycle::EndCycle() throw( SError ) {

   return;

}

void PileUpHistoCycle::BeginInputData( const SInputData& ) throw( SError ) {
  

  //
  // Declare the output histograms:
  //
  Book( TH1F( "N_pileup_hist", "N^{PU}", 1000,0,50 ) );

  return;

}

void PileUpHistoCycle::EndInputData( const SInputData& ) throw( SError ) {

   return;

}

void PileUpHistoCycle::BeginInputFile( const SInputData& ) throw( SError ) {

  ConnectVariable( "AnalysisTree", "genInfo" , bcc.genInfo);

  return;

}

void PileUpHistoCycle::ExecuteEvent( const SInputData&, Double_t weight) throw( SError ) {
 
  Hist( "N_pileup_hist" )->Fill( bcc.genInfo->pileup_TrueNumInteractions, weight );

  return;

}

