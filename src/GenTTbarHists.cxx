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

  Book( TH1F( "Pt_top", "P_{T,t} [GeV/c]", 1000, 0, 2000 ) );
  Book( TH1F( "Pt_antitop", "P_{T,#bar{t}} [GeV/c]", 1000, 0, 2000 ) );

  Book( TH1F( "eta_top", "#eta_{t}", 1000, -5, 5 ) );
  Book( TH1F( "eta_antitop", "#eta_{#bar{t}}", 1000, -5, 5 ) );
  Book( TH1F( "y_top", "y_{t}", 1000, -5, 5 ) );
  Book( TH1F( "y_antitop", "y_{#bar{t}}", 1000, -5, 5 ) );

  Book (TH1F( "diffabseta", "|#eta_{t}|-|#eta_{#bar{t}}|",1000,-5,5));
  Book (TH1F( "diffabsy", "|y_{t}|-|y_{#bar{t}}|",1000,-5,5));

  Book (TH1F( "deltaR_top_decays", "#DeltaR(t decay prod.)",1000,0,5));
  Book (TH1F( "deltaR_antitop_decays", "#DeltaR(#bar{t} decay prod.)",1000,0,5));  
  Book (TH2F( "M_ttbar_vs_deltaR_top", "M_{t#bar{t}} [GeV/c^{2}] vs #DeltaR(t decay prod.)",500,0,5000,500,0,5));
  Book (TH2F( "M_ttbar_vs_deltaR_antitop", "M_{t#bar{t}} [GeV/c^{2}] vs #DeltaR(#bar{t} decay prod.)",500,0,5000,500,0,5)); 

  Book (TH2F( "Pt_top_vs_deltaR_top", "P_{T,t} [GeV/c] vs #DeltaR(t decay prod.)",500,0,2000,500,0,5));
  Book (TH2F( "Pt_antitop_vs_deltaR_antitop", "P_{T,#bar{t}} [GeV/c] vs #DeltaR(#bar{t} decay prod.)",500,0,2000,500,0,5));


  Book (TH1F( "deltaR_Wplus_decays", "#DeltaR(W^{+} decay prod.)",1000,0,5));
  Book (TH1F( "deltaR_Wminus_decays", "#DeltaR(W{-} decay prod.)",1000,0,5));  
  Book (TH2F( "M_ttbar_vs_deltaR_Wplus", "M_{t#bar{t}} [GeV/c^{2}] vs #DeltaR(W^{+} decay prod.)",500,0,5000,500,0,5));
  Book (TH2F( "M_ttbar_vs_deltaR_Wminus", "M_{t#bar{t}} [GeV/c^{2}] vs #DeltaR(W^{-} decay prod.)",500,0,5000,500,0,5)); 

  Book (TH1F( "Pt_b","P_{T,b} [GeV/c]",1000,0,1000));
  Book (TH1F( "Pt_antib","P_{T,#bar{b}} [GeV/c]",1000,0,1000));
  Book (TH1F( "eta_b","#eta_{b}",1000,-5,5));
  Book (TH1F( "eta_antib","#eta_{#bar{b}}",1000,-5,5));
  Book (TH1F( "y_b","y_{b}",1000,-5,5));
  Book (TH1F( "y_antib","y_{#bar{b}}",1000,-5,5));
  Book (TH1F( "M_b","M_{b} [GeV/c^{2}]",1000,0,10));
  Book (TH1F( "M_antib","M_{#bar{b}} [GeV/c^{2}]",1000,0,10));

  Book (TH1F( "Pt_Wplus","P_{T,W^{+}} [GeV/c]",1000,0,1000));
  Book (TH1F( "Pt_Wminus","P_{T,W^{-}} [GeV/c]",1000,0,1000));
  Book (TH1F( "eta_Wplus","#eta_{W^{+}}",1000,-5,5));
  Book (TH1F( "eta_Wminus","#eta_{W^{-}}",1000,-5,5));
  Book (TH1F( "y_Wplus","y_{W^{+}}",1000,-5,5));
  Book (TH1F( "y_Wminus","y_{W^{-}}",1000,-5,5));
  Book (TH1F( "M_Wplus","M_{W^{+}} [GeV/c^{2}]",1000,50,100));
  Book (TH1F( "M_Wminus","M_{W^{-}} [GeV/c^{2}]",1000,50,100));
  

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

  TH2F* h2D_5 = (TH2F*) Hist("Pt_top_vs_deltaR_top");
  TH2F* h2D_6 = (TH2F*) Hist("Pt_antitop_vs_deltaR_antitop");  
  h2D_5->Fill(calc->GetTTbarGen()->Top().v4().Pt(), deltaR_top, weight);
  h2D_6->Fill(calc->GetTTbarGen()->Antitop().v4().Pt(), deltaR_antitop, weight);

  Hist("Pt_b")->Fill( calc->GetTTbarGen()->bTop().pt(), weight);
  Hist("Pt_antib")->Fill( calc->GetTTbarGen()->bAntitop().pt(), weight);  
  Hist("eta_b")->Fill( calc->GetTTbarGen()->bTop().eta(), weight);
  Hist("eta_antib")->Fill( calc->GetTTbarGen()->bAntitop().eta(), weight); 
  Hist("y_b")->Fill( calc->GetTTbarGen()->bTop().v4().Rapidity(), weight);
  Hist("y_antib")->Fill( calc->GetTTbarGen()->bAntitop().v4().Rapidity(), weight); 
  Hist("M_b")->Fill( calc->GetTTbarGen()->bTop().v4().M(), weight);
  Hist("M_antib")->Fill( calc->GetTTbarGen()->bAntitop().v4().M(), weight); 

  Hist("Pt_Wplus")->Fill( calc->GetTTbarGen()->WTop().pt(), weight);
  Hist("Pt_Wminus")->Fill( calc->GetTTbarGen()->WAntitop().pt(), weight);  
  Hist("eta_Wplus")->Fill( calc->GetTTbarGen()->WTop().eta(), weight);
  Hist("eta_Wminus")->Fill( calc->GetTTbarGen()->WAntitop().eta(), weight); 
  Hist("y_Wplus")->Fill( calc->GetTTbarGen()->WTop().v4().Rapidity(), weight);
  Hist("y_Wminus")->Fill( calc->GetTTbarGen()->WAntitop().v4().Rapidity(), weight); 
  Hist("M_Wplus")->Fill( calc->GetTTbarGen()->WTop().v4().M(), weight);
  Hist("M_Wminus")->Fill( calc->GetTTbarGen()->WAntitop().v4().M(), weight); 

}

void GenTTbarHists::Finish()
{


}

