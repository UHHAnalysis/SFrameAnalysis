#include "include/GenTTbarHists.h"
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
  Book( TH1F( "shat", "#hat{s} [GeV]", 1000, 0, 5000 ) );

  Book( TH1F("M_top", "M_{t} [GeV/c^{2}]", 1000, 150, 200) );
  Book( TH1F("M_antitop", "M_{#bar{t}} [GeV/c^{2}]", 1000, 150, 200) );  

  Book( TH1F( "Pt_top", "P_{T,t} [GeV/c]", 1000, 0, 2000 ) );
  Book( TH1F( "Pt_antitop", "P_{T,#bar{t}} [GeV/c]", 1000, 0, 2000 ) );

  Book( TH1F( "Pt_top_over_shat", "P_{T,t}/#hat{s}", 1000, 0, 1 ) );
  Book( TH1F( "Pt_antitop_over_shat", "P_{T,#bar{t}}/#hat{s}", 1000, 0, 1 ) );
  Book( TH1F( "Pt_top_over_M_ttbar", "P_{T,t}/M_{t#bar{t}}", 1000, 0, 1 ) );
  Book( TH1F( "Pt_antitop_over_M_ttbar", "P_{T,#bar{t}}/M_{t#bar{t}}", 1000, 0, 1 ) );

  Book( TH2F( "M_ttbar_vs_shat","M_{t#bar{t}} [GeV/c^{2}] vs #hat{s} [GeV]", 500,0,5000,500,0,5000));
  Book( TH2F( "M_ttbar_vs_M_ttbar_decay","M_{t#bar{t}} [GeV/c^{2}] vs M_{t#bar{t}}^{dec.} [GeV/c^{2}]", 500,0,5000,500,0,5000));

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

  Book (TH2F( "shat_vs_deltaR_top", "#hat{s} [GeV] vs #DeltaR(t decay prod.)",500,0,5000,500,0,5));
  Book (TH2F( "shat_vs_deltaR_antitop", "#hat{s} [GeV] vs #DeltaR(#bar{t} decay prod.)",500,0,5000,500,0,5)); 

  Book (TH2F( "Pt_top_vs_deltaR_top", "P_{T,t} [GeV/c] vs #DeltaR(t decay prod.)",500,0,2000,500,0,5));
  Book (TH2F( "Pt_antitop_vs_deltaR_antitop", "P_{T,#bar{t}} [GeV/c] vs #DeltaR(#bar{t} decay prod.)",500,0,2000,500,0,5));


  Book (TH1F( "deltaR_Wplus_decays", "#DeltaR(W^{+} decay prod.)",1000,0,5));
  Book (TH1F( "deltaR_Wminus_decays", "#DeltaR(W^{-} decay prod.)",1000,0,5));  
  Book (TH2F( "M_ttbar_vs_deltaR_Wplus", "M_{t#bar{t}} [GeV/c^{2}] vs #DeltaR(W^{+} decay prod.)",500,0,5000,500,0,5));
  Book (TH2F( "M_ttbar_vs_deltaR_Wminus", "M_{t#bar{t}} [GeV/c^{2}] vs #DeltaR(W^{-} decay prod.)",500,0,5000,500,0,5)); 

  Book (TH2F( "M_ttbar_vs_Pt_top", "M_{t#bar{t}} [GeV/c^{2}] vs P_{T,t} [GeV/c]",500,0,5000,500,0,2000)); 
  Book (TH2F( "M_ttbar_vs_Pt_antitop", "M_{t#bar{t}} [GeV/c^{2}] vs P_{T,#bar{t}} [GeV/c]",500,0,5000,500,0,2000));
  Book (TH2F( "shat_vs_Pt_top", "#hat{s} [GeV] vs P_{T,t} [GeV/c]",500,0,5000,500,0,2000)); 
  Book (TH2F( "shat_vs_Pt_antitop", "#hat{s} [GeV] vs P_{T,#bar{t}} [GeV/c]",500,0,5000,500,0,2000));
  Book (TH2F( "Pt_top_vs_Pt_antitop", "P_{T,t} [GeV/c] vs P_{T,#bar{t}} [GeV/c]",500,0,2000,500,0,2000));

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
  
  Book( TH1F( "cosThetastar_top_ttframe", "cos(#Theta*)_{t}",1000,-1,1));
  Book( TH1F( "cosThetastar_antitop_ttframe", "cos(#Theta*)_{#bar{t}}",1000,-1,1));
  Book( TH1F( "Pt_top_ttframe", "P_{T,t}* [GeV/c]",1000,0,2000));
  Book( TH1F( "Pt_antitop_ttframe", "P_{T,#bar{t}}* [GeV/c]",1000,0,2000));
  Book (TH2F( "M_ttbar_vs_Pt_top_ttframe", "M_{t#bar{t}} [GeV/c^{2}] vs P_{T,t}* [GeV/c]",500,0,5000,500,0,2000)); 
  Book (TH2F( "M_ttbar_vs_Pt_antitop_ttframe", "M_{t#bar{t}} [GeV/c^{2}] vs P_{T,#bar{t}}* [GeV/c]",500,0,5000,500,0,2000));

  Book (TH2F( "M_ttbar_vs_eta_top", "M_{t#bar{t}} [GeV/c^{2}] vs #eta_{t}",500,0,5000,500,-5,5)); 
  Book (TH2F( "M_ttbar_vs_eta_antitop", "M_{t#bar{t}} [GeV/c^{2}] vs #eta_{#bar{t}}",500,0,5000,500,-5,5));
}

void GenTTbarHists::Fill()
{
  // fill the histograms


  EventCalc* calc = EventCalc::Instance();
  double weight = calc->GetWeight();

  LorentzVector top =  calc->GetTTbarGen()->Top().v4();
  LorentzVector antitop = calc->GetTTbarGen()->Antitop().v4();

  double  mttbar_gen = ( top + antitop ).M();
  double  ptttbar_gen = ( top + antitop ).Pt();

  //mttbar from decay products of tops
  int topindex=-1;
  int antitopindex=-1;
  int ntop=0;
  int nantitop=0;
  LorentzVector v4_top_dec(0,0,0,0);
  LorentzVector v4_antitop_dec(0,0,0,0);

  for(unsigned int i=0; i<calc->GetGenParticles()->size(); ++i){
    if(calc->GetGenParticles()->at(i).pdgId()==6) ntop++;
    if(calc->GetGenParticles()->at(i).pdgId()==-6) nantitop++;
    //get first tops in list
    if(calc->GetGenParticles()->at(i).pdgId()==6 && topindex<0) topindex=i;
    if(calc->GetGenParticles()->at(i).pdgId()==-6 && antitopindex<0) antitopindex=i;
  }
  //std::cout << ntop << "   " << nantitop << std::endl;
  if(ntop>0 && nantitop>0){
    for(unsigned int i=0; i<calc->GetGenParticles()->size(); ++i){
      if(calc->GetGenParticles()->at(i).mother1()==topindex || calc->GetGenParticles()->at(i).mother2()==topindex)
	v4_top_dec += calc->GetGenParticles()->at(i).v4();
      if(calc->GetGenParticles()->at(i).mother1()==antitopindex || calc->GetGenParticles()->at(i).mother2()==antitopindex)
	v4_antitop_dec += calc->GetGenParticles()->at(i).v4();
    }
  }
  double mttbar_gen_decay= (v4_top_dec+v4_antitop_dec).M();
  double shat = (calc->GetGenParticles()->at(0).v4()+ calc->GetGenParticles()->at(1).v4()).M();

  Hist("M_ttbar_gen")->Fill(mttbar_gen, weight);
  Hist("Pt_ttbar_gen")->Fill ( ptttbar_gen, weight); 
  Hist("shat")->Fill(shat,weight);

  Hist("M_top")->Fill( calc->GetTTbarGen()->Top().v4().M(),weight);
  Hist("M_antitop")->Fill( calc->GetTTbarGen()->Antitop().v4().M(),weight);

  Hist("Pt_top")->Fill( calc->GetTTbarGen()->Top().v4().Pt(),weight);
  Hist("Pt_antitop")->Fill( calc->GetTTbarGen()->Antitop().v4().Pt(),weight);

  Hist("Pt_top_over_shat")->Fill( calc->GetTTbarGen()->Top().v4().Pt()/shat,weight);
  Hist("Pt_antitop_over_shat")->Fill( calc->GetTTbarGen()->Antitop().v4().Pt()/shat,weight);
  Hist("Pt_top_over_M_ttbar")->Fill( calc->GetTTbarGen()->Top().v4().Pt()/mttbar_gen,weight);
  Hist("Pt_antitop_over_M_ttbar")->Fill( calc->GetTTbarGen()->Antitop().v4().Pt()/mttbar_gen,weight);

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

  TH2F* h2D_0 = (TH2F*) Hist("M_ttbar_vs_M_ttbar_decay");
  h2D_0->Fill(mttbar_gen, mttbar_gen_decay, weight);

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

  TH2F* h2D_7 = (TH2F*) Hist("M_ttbar_vs_Pt_top");
  TH2F* h2D_8 = (TH2F*) Hist("M_ttbar_vs_Pt_antitop");  
  h2D_7->Fill(mttbar_gen, calc->GetTTbarGen()->Top().v4().Pt(),  weight);
  h2D_8->Fill(mttbar_gen, calc->GetTTbarGen()->Antitop().v4().Pt(), weight);
  TH2F* h2D_9 = (TH2F*) Hist("Pt_top_vs_Pt_antitop"); 
  h2D_9->Fill(calc->GetTTbarGen()->Top().v4().Pt(),calc->GetTTbarGen()->Antitop().v4().Pt(), weight);
  TH2F* h2D_10 = (TH2F*) Hist("M_ttbar_vs_shat");
  h2D_10->Fill(mttbar_gen,shat,weight);

  TH2F* h2D_11 = (TH2F*) Hist("shat_vs_deltaR_top");
  TH2F* h2D_12 = (TH2F*) Hist("shat_vs_deltaR_antitop");  
  h2D_11->Fill(shat, deltaR_top, weight);
  h2D_12->Fill(shat, deltaR_antitop, weight);
  TH2F* h2D_13 = (TH2F*) Hist("shat_vs_Pt_top");
  TH2F* h2D_14 = (TH2F*) Hist("shat_vs_Pt_antitop");  
  h2D_13->Fill(shat, calc->GetTTbarGen()->Top().v4().Pt(),  weight);
  h2D_14->Fill(shat, calc->GetTTbarGen()->Antitop().v4().Pt(), weight);
  

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
  if(calc->GetTTbarGen()->WTop().v4().isTimelike())
    Hist("M_Wplus")->Fill( calc->GetTTbarGen()->WTop().v4().M(), weight);
  if(calc->GetTTbarGen()->WAntitop().v4().isTimelike())
    Hist("M_Wminus")->Fill( calc->GetTTbarGen()->WAntitop().v4().M(), weight); 


  TLorentzVector top_ttframe(0,0,0,0);
  top_ttframe.SetPtEtaPhiE(top.pt(), top.eta(), top.phi(), top.E());
  TLorentzVector antitop_ttframe(0,0,0,0);
  antitop_ttframe.SetPtEtaPhiE(antitop.pt(), antitop.eta(), antitop.phi(), antitop.E());
  TLorentzVector ttbar(0,0,0,0);
  ttbar.SetPtEtaPhiE((top+antitop).pt(), (top+antitop).eta(), (top+antitop).phi(), (top+antitop).E()); 

  top_ttframe.Boost(-ttbar.BoostVector());
  antitop_ttframe.Boost(-ttbar.BoostVector());

  Hist("cosThetastar_top_ttframe")->Fill(cos(top_ttframe.Theta()) ,weight);
  Hist("cosThetastar_antitop_ttframe")->Fill(cos(antitop_ttframe.Theta()) ,weight);
  Hist("Pt_top_ttframe")->Fill(top_ttframe.Pt(),weight);
  Hist("Pt_antitop_ttframe")->Fill(antitop_ttframe.Pt(),weight);  

  TH2F* h2D_15 = (TH2F*) Hist("M_ttbar_vs_Pt_top_ttframe");
  TH2F* h2D_16 = (TH2F*) Hist("M_ttbar_vs_Pt_antitop_ttframe");
  h2D_15->Fill(mttbar_gen, top_ttframe.Pt(),weight );
  h2D_16->Fill(mttbar_gen, antitop_ttframe.Pt(),weight ); 

  TH2F* h2D_17 = (TH2F*) Hist("M_ttbar_vs_eta_top");
  TH2F* h2D_18 = (TH2F*) Hist("M_ttbar_vs_eta_antitop");  
  h2D_17->Fill(mttbar_gen, calc->GetTTbarGen()->Top().v4().eta(),  weight);
  h2D_18->Fill(mttbar_gen, calc->GetTTbarGen()->Antitop().v4().eta(), weight);

}

void GenTTbarHists::Finish()
{


}

