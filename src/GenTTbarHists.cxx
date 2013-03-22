#include "include/GenTTbarHists.h"
#include "include/ObjectHandler.h"
#include "include/EventCalc.h"
#include <iostream>

using namespace std;

GenTTbarHists::GenTTbarHists(const char* name) : BaseHists(name)
{
  // named default constructor
   
}

GenTTbarHists::~GenTTbarHists()
{
  // default destructor, does nothing
}

void GenTTbarHists::Init()
{
  // book all histograms here
  Book( TH1F( "M_ttbar_gen", "M_{t#bar{t}} [GeV/c^{2}]", 1000, 0, 5000 ) );
  Book( TH1F( "Pt_ttbar_gen", "P_{T,t#bar{t}} [GeV/c]", 600, 0, 600 ) );

  Book( TH1F("M_top", "M_{t} [GeV/c^{2}]", 100, 150, 200) );
  Book( TH1F("M_antitop", "M_{#bar{t}} [GeV/c^{2}]", 100, 150, 200) );  

  Book( TH1F( "Pt_top", "P_{T,t} [GeV/c]", 1000, 0, 1000 ) );
  Book( TH1F( "Pt_antitop", "P_{T,#bar{t}} [GeV/c]", 1000, 0, 1000 ) );

  Book( TH1F( "eta_top", "#eta_{t}", 1000, -5, 5 ) );
  Book( TH1F( "eta_antitop", "#eta_{#bar{t}}", 1000, -5, 5 ) );
  Book( TH1F( "y_top", "y_{t}", 1000, -5, 5 ) );
  Book( TH1F( "y_antitop", "y_{#bar{t}}", 1000, -5, 5 ) );

  Book (TH1F( "diffabseta", "|#eta_{t}|-|#eta_{#bar{t}}|",100,-5,5));
  Book (TH1F( "diffabsy", "|y_{t}|-|y_{#bar{t}}|",100,-5,5));

  Book (TH1F( "deltaR_top_decays", "#DeltaR(t decay prod.)",1000,0,5));
  Book (TH1F( "deltaR_antitop_decays", "#DeltaR(#bar{t} decay prod.)",1000,0,5));  
  Book (TH2F( "M_ttbar_vs_deltaR_top", "M_{t#bar{t}} [GeV/c^{2}] vs #DeltaR(t decay prod.)",1000,0,5000,1000,0,5));
  Book (TH2F( "M_ttbar_vs_deltaR_antitop", "M_{t#bar{t}} [GeV/c^{2}] vs #DeltaR(#bar{t} decay prod.)",1000,0,5000,1000,0,5)); 


  Book (TH1F( "deltaR_Wplus_decays", "#DeltaR(W^{+} decay prod.)",1000,0,5));
  Book (TH1F( "deltaR_Wminus_decays", "#DeltaR(W{-} decay prod.)",1000,0,5));  
  Book (TH2F( "M_ttbar_vs_deltaR_Wplus", "M_{t#bar{t}} [GeV/c^{2}] vs #DeltaR(W^{+} decay prod.)",1000,0,5000,1000,0,5));
  Book (TH2F( "M_ttbar_vs_deltaR_Wminus", "M_{t#bar{t}} [GeV/c^{2}] vs #DeltaR(W^{-} decay prod.)",1000,0,5000,1000,0,5)); 

}

void GenTTbarHists::Fill()
{
  // fill the histograms

  EventCalc* calc = EventCalc::Instance();
  double weight = calc->GetWeight();

  double  mttbar_gen = ( calc->GetTTbarGen()->Top().v4() + calc->GetTTbarGen()->Antitop().v4()).M();
  double  ptttbar_gen = ( calc->GetTTbarGen()->Top().v4() + calc->GetTTbarGen()->Antitop().v4()).Pt();

  Hist("M_ttbar_gen")->Fill(mttbar_gen, weight);
  Hist("Pt_ttbar_gen")->Fill ( ptttbar_gen, weight); 

  Hist("M_top")->Fill( calc->GetTTbarGen()->Top().v4().M(),weight);
  Hist("M_antitop")->Fill( calc->GetTTbarGen()->Antitop().v4().M(),weight);

  Hist("Pt_top")->Fill( calc->GetTTbarGen()->Top().v4().Pt(),weight);
  Hist("Pt_antitop")->Fill( calc->GetTTbarGen()->Antitop().v4().Pt(),weight);
  Hist("eta_top")->Fill( calc->GetTTbarGen()->Top().v4().eta(),weight);
  Hist("eta_antitop")->Fill( calc->GetTTbarGen()->Antitop().v4().eta(),weight);
  Hist("y_top")->Fill( calc->GetTTbarGen()->Top().v4().Rapidity(),weight);
  Hist("y_antitop")->Fill( calc->GetTTbarGen()->Antitop().v4().Rapidity(),weight);

  double diffabseta = fabs(  calc->GetTTbarGen()->Top().v4().eta()) - fabs( calc->GetTTbarGen()->Antitop().v4().eta());
  double diffabsy = fabs(  calc->GetTTbarGen()->Top().v4().Rapidity()) - fabs( calc->GetTTbarGen()->Antitop().v4().Rapidity());

  Hist("diffabseta")->Fill(diffabseta,weight);
  Hist("diffabsy")->Fill(diffabsy,weight);

  double deltaR_top = max (max( calc->GetTTbarGen()->bTop().deltaR( calc->GetTTbarGen()->Wdecay1() ), 
			   calc->GetTTbarGen()->bTop().deltaR( calc->GetTTbarGen()->Wdecay2() ) )
			   , calc->GetTTbarGen()->Wdecay1().deltaR( calc->GetTTbarGen()->Wdecay2() ) );

  double deltaR_antitop = max (max( calc->GetTTbarGen()->bAntitop().deltaR( calc->GetTTbarGen()->WMinusdecay1() ), 
			   calc->GetTTbarGen()->bAntitop().deltaR( calc->GetTTbarGen()->WMinusdecay2() ) )
			   , calc->GetTTbarGen()->WMinusdecay1().deltaR( calc->GetTTbarGen()->WMinusdecay2() ) );
  Hist("deltaR_top_decays")->Fill(deltaR_top,weight);
  Hist("deltaR_antitop_decays")->Fill(deltaR_antitop,weight);

  TH2F* h2D_1 = (TH2F*) Hist("M_ttbar_vs_deltaR_top");
  TH2F* h2D_2 = (TH2F*) Hist("M_ttbar_vs_deltaR_antitop");  
  h2D_1->Fill(mttbar_gen, deltaR_top, weight);
  h2D_2->Fill(mttbar_gen, deltaR_antitop, weight);

  double deltaR_Wplus = calc->GetTTbarGen()->Wdecay1().deltaR( calc->GetTTbarGen()->Wdecay2());
  double deltaR_Wminus = calc->GetTTbarGen()->WMinusdecay1().deltaR( calc->GetTTbarGen()->WMinusdecay2());
  Hist("deltaR_Wplus_decays")->Fill(deltaR_Wplus,weight);
  Hist("deltaR_Wminus_decays")->Fill(deltaR_Wminus,weight);

  TH2F* h2D_3 = (TH2F*) Hist("M_ttbar_vs_deltaR_Wplus");
  TH2F* h2D_4 = (TH2F*) Hist("M_ttbar_vs_deltaR_Wminus");  
  h2D_3->Fill(mttbar_gen, deltaR_Wplus, weight);
  h2D_4->Fill(mttbar_gen, deltaR_Wminus, weight);
  
}

void GenTTbarHists::Finish()
{


}

