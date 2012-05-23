// $Id: TauEffiCycle.cxx,v 1.1 2012/04/23 14:53:22 peiffer Exp $

// Local include(s):
#include "../include/TauEffiCycle.h"

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
  ConnectVariable( "AnalysisTree", "selectedPatTausPFlow" , bcc.taus); 

  return;

}

void TauEffiCycle::ExecuteEvent( const SInputData&, Double_t weight) throw( SError ) {
 
  //produce clean tau collection
  for(unsigned int i=0; i<bcc.taus->size(); ++i){
    if(!bcc.taus->at(i).decayModeFinding() || !bcc.taus->at(i).byLooseCombinedIsolationDeltaBetaCorr() /*||  !bcc.taus->at(i).againstElectronTight() ||  !bcc.taus->at(i).againstMuonTight()*/){
      bcc.taus->erase(bcc.taus->begin()+i);
      i--;
    }
  }
  
  for(unsigned int i=0; i<bcc.genparticles->size(); ++i){
    GenParticle genp = bcc.genparticles->at(i);
    if(abs(genp.pdgId())!=15) continue;
    if(fabs(genp.eta())>2.5) continue;

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

