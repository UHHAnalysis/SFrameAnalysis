// $Id: TauEffiCycle.cxx,v 1.1 2012/04/23 14:53:22 peiffer Exp $

#include "include/TauEffiCycle.h"
#include "include/Cleaner.h"
#include "TGraphAsymmErrors.h"

ClassImp( TauEffiCycle );


TauEffiCycle::TauEffiCycle()
   : SCycleBase() {

   SetLogName( GetName() );
}

TauEffiCycle::~TauEffiCycle() {

}

void TauEffiCycle::BeginCycle() throw( SError ) {
  
  return;

}

void TauEffiCycle::EndCycle() throw( SError ) {

  return;

}

void TauEffiCycle::BeginInputData( const SInputData& ) throw( SError ) {
  

  //
  // Declare the output histograms:
  //
  Book( TH1F( "pt_gentau_hist", "p_{T}^{gen}", 100,0,250 ) );
  Book( TH1F( "pt_rectau_hist", "p_{T}^{rec}", 100,0,250 ) );  

  return;

}

void TauEffiCycle::EndInputData( const SInputData& ) throw( SError ) {

  TGraphAsymmErrors *eff = new TGraphAsymmErrors(Hist("pt_rectau_hist"), Hist("pt_gentau_hist"), "cp");
  eff->SetName("pt_tau_effi");
  eff->SetTitle("pt_tau_effi");

  Book( *eff );
   return;

}

void TauEffiCycle::BeginInputFile( const SInputData& ) throw( SError ) {

  ConnectVariable( "AnalysisTree", "GenParticles" , bcc.genparticles);
  ConnectVariable( "AnalysisTree", "slimmedTaus" , bcc.taus); 

  return;

}

void TauEffiCycle::ExecuteEvent( const SInputData&, Double_t weight) throw( SError ) {
 
  //produce clean tau collection
  Cleaner cleaner(&bcc);
  cleaner.TauCleaner(30,2.5);
  
  for(unsigned int i=0; i<bcc.genparticles->size(); ++i){
    GenParticle genp = bcc.genparticles->at(i);
    if(abs(genp.pdgId())!=15) continue;
    if(fabs(genp.eta())>2.5) continue;
    //take only taus from matrix element
    if(genp.status()!=23) continue;

    Hist("pt_gentau_hist")->Fill(genp.pt(),weight);

    bool match=false;
    for(unsigned int j=0; j<bcc.taus->size(); ++j){
      Tau tau = bcc.taus->at(j);
      if(tau.deltaR(genp)<0.3){
	if(match){
	  std::cout << "double match found" << std::endl;
	}
	else{
	  Hist("pt_rectau_hist")->Fill(genp.pt(),weight);
	  match=true;
	}
      }
    }
  }
  
  return;

}

