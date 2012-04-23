// $Id: PileUpHistoCycle.cxx,v 1.2 2012/04/10 13:18:29 peiffer Exp $

// Local include(s):
#include "../include/LeptoquarkCycle.h"

ClassImp( LeptoquarkCycle );

using namespace std;

LeptoquarkCycle::LeptoquarkCycle()
   : SCycleBase() {

   SetLogName( GetName() );
}

LeptoquarkCycle::~LeptoquarkCycle() {

}

void LeptoquarkCycle::BeginCycle() throw( SError ) {
  
  elec =0;
  muon =0;
  had=0;

  return;

}

void LeptoquarkCycle::EndCycle() throw( SError ) {

  std::cout <<"in electrons: "<< elec << " in muons: "<< muon << " hadronic: " << had <<std::endl; 
      return;

}

void LeptoquarkCycle::BeginInputData( const SInputData& ) throw( SError ) {
  

  //
  // Declare the output histograms:
  //
  Book( TH1F( "Leptoquark_pT", "Leptoquark p_T", 100,0,2000 ) );
  Book( TH1F( "Leptoquark_eta", "Leptoquark eta", 100,-5,5 ) );
  Book( TH1F( "Top_pT", "Top p_T", 100,0,2000 ) );
  Book( TH1F( "Top_eta", "Top eta", 100,-5,5 ) );
  Book( TH1F( "WTopdecay_pT", "", 100,0,1200 ) );
  Book( TH1F( "WTopdecay_eta", "", 100,-5,5 ) );
  Book( TH1F( "BTopdecay_pT", "", 100,0,1000 ) );
  Book( TH1F( "BTopdecay_eta", "", 100,-5,5 ) );
  Book( TH1F( "Tau_pT", "Tau p_T", 100,0,2000 ) );
  Book( TH1F( "Wdecay_pT", "p_T W decay", 100,0,1000 ) );
  Book( TH1F( "Wdecay_eta", "eta W decay", 100,-5,5 ) );
  Book( TH2F( "R_pT_top", "R_pT(top)", 50, 0, 2000, 50, 0, 5) );
  Book( TH2F( "R_pT_W", "R_pT(W)", 50, 0, 1000, 50, 0, 5) );
  Book( TH1F( "P_TMiss","P_TMiss", 100,0,600 ));


  
  
  

  return;

}

void LeptoquarkCycle::EndInputData( const SInputData& ) throw( SError ) {

   return;

}

void LeptoquarkCycle::BeginInputFile( const SInputData& ) throw( SError ) {

  // ConnectVariable( "AnalysisTree", "genInfo" , bcc.genInfo);
  ConnectVariable( "AnalysisTree", "GenParticles" , bcc.genparticles);
  return;

}

void LeptoquarkCycle::ExecuteEvent( const SInputData&, Double_t weight) throw( SError ) 
{
 
  //double npu = bcc.genInfo->pileup_TrueNumInteractions;
  //if(npu>25) npu=24.9999;
  //Hist( "N_pileup_hist" )->Fill( npu, weight );
   
  
  
  double R = 0;
  double deltaphi = 0;
  double deltaeta = 0;
  TLorentzVector pTMiss;
  pTMiss.SetPtEtaPhiE( 0, 0, 0, 0);

  for(unsigned int i=0; i<bcc.genparticles->size(); ++i)
    {
      GenParticle genp = bcc.genparticles->at(i);
      // std::cout << genp.index <<"  pdgId = " << genp.pdgId << "  status = " << genp.status << "  mo1 = " << genp.mother1 << "  mo2 = " << genp.mother2 <<"  da1 = " << genp.daughter1 << "  da2 = " << genp.daughter2 <<std::endl;
      

      
      if (genp.pdgId == 42 || genp.pdgId == -42 ) 
	{
	  Hist ("Leptoquark_pT") -> Fill(genp.pt);
	  Hist ("Leptoquark_eta") -> Fill(genp.eta);
	}
      

     
      if (genp.pdgId == 6 || genp.pdgId == -6 )
	{
	  Hist ( "Top_pT") -> Fill(genp.pt);
	  Hist ( "Top_eta") -> Fill(genp.eta);
	  if (genp.daughter(bcc.genparticles,1) && (genp.daughter(bcc.genparticles,2)))
	    {
	      if (genp.daughter(bcc.genparticles,1)->pdgId == 24 || genp.daughter(bcc.genparticles,1)->pdgId == -24)
		{
		  Hist("WTopdecay_pT") -> Fill(genp.daughter(bcc.genparticles,1)->pt);
		  Hist("WTopdecay_eta") -> Fill(genp.daughter(bcc.genparticles,1)->eta);
		  //std::cout << genp.index <<"  pdgId = " << genp.pdgId << "  mo1 = " << genp.mother1 << "  mo2 = " << genp.mother2 <<"  da1 = " << genp.daughter1 << "  da2 = " << genp.daughter2 <<std::endl;  
		}
	      if (genp.daughter(bcc.genparticles,1)->pdgId == 5 || genp.daughter(bcc.genparticles,1)->pdgId == -5)
		{
		  Hist("BTopdecay_pT") -> Fill(genp.daughter(bcc.genparticles,1)->pt);
		  Hist("BTopdecay_eta") -> Fill(genp.daughter(bcc.genparticles,1)->eta);
		}
	      
	      if (genp.daughter(bcc.genparticles,2)->pdgId == 24 || genp.daughter(bcc.genparticles,2)->pdgId == -24)
		{
		  Hist("WTopdecay_pT") -> Fill(genp.daughter(bcc.genparticles,2)->pt);
		  Hist("WTopdecay_eta") -> Fill(genp.daughter(bcc.genparticles,2)->eta);
		}
	      if (genp.daughter(bcc.genparticles,2)->pdgId == 5 || genp.daughter(bcc.genparticles,2)->pdgId == -5)
		{
		  Hist("BTopdecay_pT") -> Fill(genp.daughter(bcc.genparticles,2)->pt);
		  Hist("BTopdecay_eta") -> Fill(genp.daughter(bcc.genparticles,2)->eta);
		}
	    
	    
	    
	      if (genp.daughter(bcc.genparticles,1)->pdgId == 24 && (genp.daughter(bcc.genparticles,2)->pdgId == 5))
		{
		  deltaphi = fabs(genp.daughter(bcc.genparticles,1)->phi - genp.daughter(bcc.genparticles,2)->phi);
		  if(deltaphi > PI) deltaphi = 2 *PI - deltaphi;
		
		  deltaeta = genp.daughter(bcc.genparticles,1)->eta - genp.daughter(bcc.genparticles,2)->eta ;
		  
		  R = sqrt(deltaeta*deltaeta+deltaphi*deltaphi);
                    
		  Hist("R_pT_top") -> Fill(genp.pt,R);

		}
	      if (genp.daughter(bcc.genparticles,2)->pdgId == 24 && (genp.daughter(bcc.genparticles,1)->pdgId == 5))
		{
		  deltaphi = fabs(genp.daughter(bcc.genparticles,1)->phi - genp.daughter(bcc.genparticles,2)->phi);
		  if(deltaphi > PI) deltaphi = 2 *PI - deltaphi;
		  
		  deltaeta = genp.daughter(bcc.genparticles,1)->eta - genp.daughter(bcc.genparticles,2)->eta ;
                
		  R = sqrt(deltaeta*deltaeta+deltaphi*deltaphi);
                
		  Hist("R_pT_top") -> Fill(genp.pt,R);
		}
	      if (genp.daughter(bcc.genparticles,1)->pdgId == -24 && (genp.daughter(bcc.genparticles,2)->pdgId == -5))
		{
		  deltaphi = fabs(genp.daughter(bcc.genparticles,1)->phi - genp.daughter(bcc.genparticles,2)->phi);
		  if(deltaphi > PI) deltaphi = 2 *PI - deltaphi;
		  
		  deltaeta = genp.daughter(bcc.genparticles,1)->eta - genp.daughter(bcc.genparticles,2)->eta ;
		    
		  R = sqrt(deltaeta*deltaeta+deltaphi*deltaphi);
                    
		  Hist("R_pT_top") -> Fill(genp.pt,R);

		}
	      if (genp.daughter(bcc.genparticles,2)->pdgId == -24 && (genp.daughter(bcc.genparticles,1)->pdgId == -5))
		{
		  deltaphi = fabs(genp.daughter(bcc.genparticles,1)->phi - genp.daughter(bcc.genparticles,2)->phi);
		  if(deltaphi > PI) deltaphi = 2 *PI - deltaphi;
		
		  deltaeta = genp.daughter(bcc.genparticles,1)->eta - genp.daughter(bcc.genparticles,2)->eta ;
		  
		  R = sqrt(deltaeta*deltaeta+deltaphi*deltaphi);
                
		  Hist("R_pT_top") -> Fill(genp.pt,R);
		    
		}
	      
	    }
	}

      //if ((genp.pdgId == 5 && (genp.mother(bcc.genparticles,1)->pdgId == 6 )) || (genp.pdgId == -5 && (genp.mother(bcc.genparticles,1)->pdgId == -6 )) && genp.status ==3 )
      //	{
      //  Hist("BTopzerfall_pT") -> Fill(genp.pt);
      //  Hist("BTopzerfall_eta") -> Fill(genp.eta);
      //	}



      if (genp.pdgId == 24 && (genp.mother(bcc.genparticles,1)->pdgId == 6 ))
	{
	  if (genp.daughter(bcc.genparticles,1) && genp.daughter(bcc.genparticles,2) )
	    {
	      Hist("Wdecay_pT") -> Fill(genp.daughter(bcc.genparticles,1)->pt);
	      Hist("Wdecay_eta") -> Fill(genp.daughter(bcc.genparticles,1)->eta);
            
	      Hist("Wdecay_pT") -> Fill(genp.daughter(bcc.genparticles,2)->pt); 
	      Hist("Wdecay_eta") -> Fill(genp.daughter(bcc.genparticles,2)->eta);
	       

	      deltaphi = fabs(genp.daughter(bcc.genparticles,1)->phi - genp.daughter(bcc.genparticles,2)->phi);
	      if(deltaphi > PI) deltaphi = 2 *PI - deltaphi;
	      
	      deltaeta = genp.daughter(bcc.genparticles,1)->eta - genp.daughter(bcc.genparticles,2)->eta ;
	      
	      R = sqrt(deltaeta*deltaeta+deltaphi*deltaphi);
                    
	      Hist("R_pT_W") -> Fill(genp.pt,R);
	    }
	}
      
      if (genp.pdgId == -24 && (genp.mother(bcc.genparticles,1)->pdgId == -6 ))
	{
	  if (genp.daughter(bcc.genparticles,1) && genp.daughter(bcc.genparticles,2) )
	    {
	      Hist("Wdecay_pT") -> Fill(genp.daughter(bcc.genparticles,1)->pt);
	      Hist("Wdecay_eta") -> Fill(genp.daughter(bcc.genparticles,1)->eta);
	      
	      Hist("Wdecay_pT") -> Fill(genp.daughter(bcc.genparticles,2)->pt); 
	      Hist("Wdecay_eta") -> Fill(genp.daughter(bcc.genparticles,2)->eta); 
            
	      deltaphi = fabs(genp.daughter(bcc.genparticles,1)->phi - genp.daughter(bcc.genparticles,2)->phi);
	      if(deltaphi > PI) deltaphi = 2 *PI - deltaphi;
	    
	      deltaeta = genp.daughter(bcc.genparticles,1)->eta - genp.daughter(bcc.genparticles,2)->eta ;
	    
	      R = sqrt(deltaeta*deltaeta+deltaphi*deltaphi);
                    
	      Hist("R_pT_W") -> Fill(genp.pt,R);

	    }
	}
    
     



      
      if (genp.mother(bcc.genparticles,1))
	{
	  if (genp.pdgId == 15 && (genp.mother(bcc.genparticles,1)->pdgId == 42 ))
	    {
	      Hist ( "Tau_pT") -> Fill(genp.pt);
	      //std::cout << genp.index <<"  pdgId = " << genp.pdgId << "  mo1 = " << genp.mother1 << "  mo2 = " << genp.mother2 <<"  da1 = " << genp.daughter1 << "  da2 = " << genp.daughter2 <<std::endl;
	    }
	  if (genp.pdgId == 15 && (genp.mother(bcc.genparticles,1)->pdgId == 42 || genp.mother(bcc.genparticles,1)->pdgId == 15  ))
	    {
	      if (genp.daughter(bcc.genparticles,1) && genp.daughter(bcc.genparticles,2) )
		{
		  if (genp.daughter(bcc.genparticles,1)->pdgId == 11 || genp.daughter(bcc.genparticles,1)->pdgId == 13)
		    {
		      if (genp.daughter(bcc.genparticles,1)->pdgId == 11) elec = elec +1;  
		      if (genp.daughter(bcc.genparticles,1)->pdgId == 13) muon = muon +1; 
		      
		    }
		  else
		    {
		      had = had +1;
		    }
		    
		
		  if (genp.daughter(bcc.genparticles,2)->pdgId == 11 || genp.daughter(bcc.genparticles,2)->pdgId == 13 )
		    {
		      if (genp.daughter(bcc.genparticles,2)->pdgId == 11) elec = elec +1;  
		      if (genp.daughter(bcc.genparticles,2)->pdgId == 13) muon = muon +1;  
		    }
		  else 
		    {
		      had = had +1;
		    }
		}
	    }
	
	  if (genp.pdgId == -15 && (genp.mother(bcc.genparticles,1)->pdgId == -42 ))
	    {
	      Hist ( "Tau_pT") -> Fill(genp.pt);
	    }
	  if (genp.pdgId == -15 && (genp.mother(bcc.genparticles,1)->pdgId == -42 || genp.mother(bcc.genparticles,1)->pdgId == -15  ))
	    {
	      if (genp.daughter(bcc.genparticles,1) && genp.daughter(bcc.genparticles,2))
		{
		  if (genp.daughter(bcc.genparticles,1)->pdgId == -11 || genp.daughter(bcc.genparticles,1)->pdgId == -13)
		    {
		      if (genp.daughter(bcc.genparticles,1)->pdgId == -11) elec = elec +1;  
		      if (genp.daughter(bcc.genparticles,1)->pdgId == -13) muon = muon +1;  
		    }
		  else
		    {
		      had = had +1;
		    }
		
		  if (genp.daughter(bcc.genparticles,2)->pdgId == -11 || genp.daughter(bcc.genparticles,2)->pdgId == -13 )
		    {
		      if (genp.daughter(bcc.genparticles,2)->pdgId == -11) elec = elec +1;  
		      if (genp.daughter(bcc.genparticles,2)->pdgId == -13) muon = muon +1;  
		    }
		  else 
		    {
		    had = had +1;
		    }
		}
	    }
	  
	}

      // p_TMiss
      if ((genp.pdgId == 12 || genp.pdgId == -12 || genp.pdgId == 14 || genp.pdgId == -14 || genp.pdgId == 16 || genp.pdgId == -16) && genp.status == 3)
	{
	  TLorentzVector neutrino;
	  neutrino.SetPtEtaPhiE( genp.pt, genp.eta, genp.phi, genp.energy);
	  pTMiss = neutrino +pTMiss;
	}



    }
  //std::cout << "lep" << lep << std::endl; 
  Hist("P_TMiss")-> Fill(pTMiss.Pt());
  
  return;
  
}

