#ifndef TMVATreeFiller_H
#define TMVATreeFiller_H

// ROOT include(s):
#include <TObject.h>
#include <TString.h>
#include <TLorentzVector.h>

// Local include(s):
#include "include/BaseHists.h"
#include "include/BaseCycleContainer.h"

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


}; // class TMVATreeFiller


#endif // TMVATreeFiller_H
