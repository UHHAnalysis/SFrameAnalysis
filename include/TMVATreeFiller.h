#ifndef TMVATreeFiller_H
#define TMVATreeFiller_H

#include "SFrameTools/include/BaseHists.h"
#include "NtupleWriter/include/TopJet.h"
#include "NtupleWriter/include/GenParticle.h"

/**
 *   Example class for booking and filling histograms
 *
 *   This class books and fills a collection of histograms.
 *   It should have a unique name, such that the histograms
 *   of multiple instances of this class are ordered in the
 *   output file. 
 *   Always sort your histograms and used methods topically.
 *   This example collection can be used for data and reconstructed
 *   MC events.
 *   
 *   @version $Revision: 1.1 $
 */

class TMVATreeFiller : public BaseHists {

public:
   /// Named constructor
   TMVATreeFiller(const char* name, TTree* tree);

   /// Default destructor
   ~TMVATreeFiller();

   void Init();

   void Fill();

   void Finish();

   void FillTopJetProperties(TopJet topjet, GenParticle topquark);
   
   void ClearVariables();

   Double_t CalcDR(GenParticle tj, GenParticle gen);

   Double_t RecGenMatch(TopJet tj, GenParticle gen);

private:
   
   TTree* m_tree;
   
   // variables for the TMVA training   
   Double_t m_px;       // px of the CA-fat jet	 
   Double_t m_py;       // py of the CA-fat jet	 
   Double_t m_pz;       // pz of the CA-fat jet	 
   Double_t m_mass;     // mass of the CA-fat jet

   Double_t m_genpx;    // px of the top quark (gen)
   Double_t m_genpy;    // py of the top quark (gen)
   Double_t m_genpz;    // pz of the top quark (gen)

   Double_t m_b_genpx;    // px of the b quark (gen)
   Double_t m_b_genpy;    // py of the b quark (gen)
   Double_t m_b_genpz;    // pz of the b quark (gen)

   Double_t m_wq1_genpx;    // px of the W decay quark 1 (gen)
   Double_t m_wq1_genpy;    // py of the W decay quark 1 (gen)
   Double_t m_wq1_genpz;    // pz of the W decay quark 1 (gen)

   Double_t m_wq2_genpx;    // px of the W decay quark 1 (gen)
   Double_t m_wq2_genpy;    // py of the W decay quark 1 (gen)
   Double_t m_wq2_genpz;    // pz of the W decay quark 1 (gen)

   Int_t m_npv;         // number of reconstructed primary vertices
   Double_t m_weight;   // event weight

   Double_t m_tau1;     // N-subjettiness, tau = 1
   Double_t m_tau2;     // N-subjettiness, tau = 2
   Double_t m_tau3;     // N-subjettiness, tau = 3
   Double_t m_tau4;     // N-subjettiness, tau = 4

   Double_t m_pruned_tau1;     // N-subjettiness for pruned jet, tau = 1
   Double_t m_pruned_tau2;     // N-subjettiness for pruned jet, tau = 2
   Double_t m_pruned_tau3;     // N-subjettiness for pruned jet, tau = 3
   Double_t m_pruned_tau4;     // N-subjettiness for pruned jet, tau = 4

   Double_t m_qjets;    // Q-jets volatility

   Int_t m_nsubs;       // number of sub-jets
   Bool_t m_HEPTopTag;  // tag from HEP Top Tagger
   Bool_t m_CMSTopTag;  // tag from CMS Top Tagger

   Double_t m_sub1_px;       // px of subjet 1
   Double_t m_sub1_py;       // py of subjet 1
   Double_t m_sub1_pz;       // pz of subjet 1
   Double_t m_sub1_mass;     // mass of subjet 1

   Double_t m_sub2_px;       // px of subjet 2
   Double_t m_sub2_py;       // py of subjet 2
   Double_t m_sub2_pz;       // pz of subjet 2
   Double_t m_sub2_mass;     // mass of subjet 2

   Double_t m_sub3_px;       // px of subjet 3
   Double_t m_sub3_py;       // py of subjet 3
   Double_t m_sub3_pz;       // pz of subjet 3
   Double_t m_sub3_mass;     // mass of subjet 3

   Double_t m_sub4_px;       // px of subjet 4
   Double_t m_sub4_py;       // py of subjet 4
   Double_t m_sub4_pz;       // pz of subjet 4
   Double_t m_sub4_mass;     // mass of subjet 4

   Double_t m_m12;      // mass of subjets 1+2
   Double_t m_m13;      // mass of subjets 1+3
   Double_t m_m23;      // mass of subjets 2+3

   Double_t m_pruned_mass;     // mass of the pruned CA-fat jet
   Double_t m_pruned_m12;      // mass of pruned subjets 1+2
   Double_t m_pruned_m13;      // mass of pruned subjets 1+3
   Double_t m_pruned_m23;      // mass of pruned subjets 2+3

   Double_t m_psi_02;       // jet shape with rmax=0.2
   Double_t m_psi_04;       // jet shape with rmax=0.4
   Double_t m_psi_06;       // jet shape with rmax=0.6

   Int_t m_number_of_constituents;
   Int_t m_number_of_charged_constituents;
   Double_t m_jet_charge;
   Double_t m_weighted_jet_charge;
   Double_t m_weighted_jet_charge_02;
   Double_t m_weighted_jet_charge_04;
   Double_t m_weighted_jet_charge_06;
   Double_t m_weighted_jet_charge_08;
   Double_t m_first_jet_moment;
   Double_t m_second_jet_moment;  

}; // class TMVATreeFiller


#endif // TMVATreeFiller_H
