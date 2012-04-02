#ifndef Objects_H
#define Objects_H

#include <vector>
#include "Math/LorentzVector.h"
#include "Math/PtEtaPhiE4D.h"
#include "TObject.h"

typedef ROOT::Math::LorentzVector< ROOT::Math::PtEtaPhiE4D< Double32_t > > LorentzVector;

class Particle{
 public:
  Particle(){
    charge=0;
    pt=0; 
    eta=0; 
    phi=0; 
    energy=0; 
  };

  ~Particle(){
  };

  LorentzVector v4(){
    LorentzVector v4;
    v4.SetPt(pt);
    v4.SetEta(eta);
    v4.SetPhi(phi);
    v4.SetE(energy);
    return v4;
  };

  float charge;
  float pt; 
  float eta; 
  float phi; 
  float energy; 

};

class Jet : public Particle{

 public:

  Jet(){
     nTracks=0;
     jetArea=0;
     pileup=0;
     numberOfDaughters=0; 
     neutralEmEnergyFraction=0;
     neutralHadronEnergyFraction=0;
     chargedEmEnergyFraction=0;
     chargedHadronEnergyFraction=0;
     muonEnergyFraction=0;
     photonEnergyFraction=0;
     chargedMultiplicity=0;
     neutralMultiplicity=0;
     muonMultiplicity=0; 
     electronMultiplicity=0;
     photonMultiplicity=0;
     btag_simpleSecondaryVertexHighEff=0;
     btag_simpleSecondaryVertexHighPur=0;
     btag_combinedSecondaryVertex=0;
     btag_combinedSecondaryVertexMVA=0;
     btag_jetBProbability=0;
     btag_jetProbability=0;
  };

  ~Jet(){
  };
  
  int nTracks;
  float jetArea;
  float pileup;
  int numberOfDaughters; 
  float neutralEmEnergyFraction;
  float neutralHadronEnergyFraction;
  float chargedEmEnergyFraction;
  float chargedHadronEnergyFraction;
  float muonEnergyFraction;
  float photonEnergyFraction;
  int chargedMultiplicity;
  int neutralMultiplicity;
  int muonMultiplicity; 
  int electronMultiplicity;
  int photonMultiplicity;
  float btag_simpleSecondaryVertexHighEff;
  float btag_simpleSecondaryVertexHighPur;
  float btag_combinedSecondaryVertex;
  float btag_combinedSecondaryVertexMVA;
  float btag_jetBProbability;
  float btag_jetProbability;
};


class Electron : public Particle{

 public:
  Electron(){
    vertex_x=0; 
    vertex_y=0; 
    vertex_z=0; 
    supercluster_eta=0; 
    supercluster_phi=0; 
    dB=0; 
    // particleIso=0; 
    neutralHadronIso=0; 
    chargedHadronIso=0; 
    trackIso=0; 
    puChargedHadronIso=0;
  };

  ~Electron(){
  };

  float vertex_x; 
  float vertex_y; 
  float vertex_z; 
  float supercluster_eta; 
  float supercluster_phi; 
  float dB; 
  //float particleIso; 
  float neutralHadronIso; 
  float chargedHadronIso; 
  float trackIso; 
  float puChargedHadronIso;

};

class Muon : public Particle{

 public:
  Muon(){
    vertex_x=0; 
    vertex_y=0; 
    vertex_z=0; 
    dB=0; 
    // particleIso=0; 
    neutralHadronIso=0; 
    chargedHadronIso=0; 
    trackIso=0; 
    puChargedHadronIso=0;
    isGlobalMuon=false;
    isStandAloneMuon=false;
    isTrackerMuon=false;
    numberOfMatchedStations=0;
    globalTrack_chi2=0;
    globalTrack_ndof=0;
    globalTrack_d0=0;
    globalTrack_d0Error=0; 
    globalTrack_numberOfValidHits=0;  
    globalTrack_numberOfLostHits=0;  
    innerTrack_chi2=0;
    innerTrack_ndof=0;
    innerTrack_d0=0;
    innerTrack_d0Error=0; 
    innerTrack_numberOfValidHits=0;  
    innerTrack_numberOfLostHits=0;  
    outerTrack_chi2=0;
    outerTrack_ndof=0;
    outerTrack_d0=0;
    outerTrack_d0Error=0; 
    outerTrack_numberOfValidHits=0;  
    outerTrack_numberOfLostHits=0; 
  };

  ~Muon(){
  };

  float vertex_x; 
  float vertex_y; 
  float vertex_z; 
  float dB; 
  //float particleIso; 
  float neutralHadronIso; 
  float chargedHadronIso; 
  float trackIso; 
  float puChargedHadronIso;
  bool isGlobalMuon;
  bool isStandAloneMuon;
  bool isTrackerMuon;
  int numberOfMatchedStations;
  float globalTrack_chi2;
  float globalTrack_ndof;
  float globalTrack_d0;
  float globalTrack_d0Error; 
  unsigned short globalTrack_numberOfValidHits;  
  unsigned short globalTrack_numberOfLostHits;  
  float innerTrack_chi2;
  float innerTrack_ndof;
  float innerTrack_d0;
  float innerTrack_d0Error; 
  unsigned short innerTrack_numberOfValidHits;  
  unsigned short innerTrack_numberOfLostHits;  
  float outerTrack_chi2;
  float outerTrack_ndof;
  float outerTrack_d0;
  float outerTrack_d0Error; 
  unsigned short outerTrack_numberOfValidHits;  
  unsigned short outerTrack_numberOfLostHits;  
};


class Tau : public Particle{

 public:
  Tau(){

  };

  ~Tau(){
  };

};

class Photon : public Particle{

 public:
  Photon(){
 
    vertex_x=0; 
    vertex_y=0; 
    vertex_z=0; 
    supercluster_eta=0; 
    supercluster_phi=0;
/*     neutralHadronIso=0;  */
/*     chargedHadronIso=0;  */
    trackIso=0; 
  };

  ~Photon(){
  };
 
  float vertex_x; 
  float vertex_y; 
  float vertex_z; 
  float supercluster_eta; 
  float supercluster_phi; 
/*   float neutralHadronIso;  */
/*   float chargedHadronIso;  */
  float trackIso; 
  
};

class MET{
   
 public:
  MET(){
    pt=0; 
    phi=0; 
    mEtSig=0;

  };

  ~MET(){
  };

  float pt; 
  float phi; 
  float mEtSig; 

};

class PrimaryVertex{
   
 public:
  PrimaryVertex(){
    x=0; 
    y=0; 
    z=0; 
    nTracks=0; 
    //isValid=false; 
    chi2=0; 
    ndof=0;

  };

  ~PrimaryVertex(){
  };

  float x; 
  float y; 
  float z; 
  unsigned int nTracks; 
  //bool isValid; 
  float chi2; 
  float ndof;
};

class GenInfo{

 public:
  GenInfo(){
    binningValues.clear();
    weights.clear();
    alphaQCD=0;
    alphaQED=0;
    qScale=0;
    pdf_id1=0;
    pdf_id2=0;
    pdf_x1=0;
    pdf_x2=0;
    pdf_xPDF1=0;
    pdf_xPDF2=0;
    pdf_scalePDF=0;
    pileup_NumInteractions_intime=0; 
    pileup_NumInteractions_ootbefore=0; 
    pileup_NumInteractions_ootafter=0;
    pileup_TrueNumInteractions=0; 
  }

  ~GenInfo(){
  };
  std::vector<double> binningValues;
  std::vector<double> weights;
  float alphaQCD;
  float alphaQED;
  float qScale;
  int pdf_id1;
  int pdf_id2;
  float pdf_x1;
  float pdf_x2;
  float pdf_xPDF1;
  float pdf_xPDF2;
  float pdf_scalePDF;
  int pileup_NumInteractions_intime; 
  int pileup_NumInteractions_ootbefore; 
  int pileup_NumInteractions_ootafter;
  float pileup_TrueNumInteractions;  //poisson mean
  
};


class TopJet : public Jet{
 public:
  TopJet(){
    subjets.clear();
  };
  ~TopJet(){
  };

  std::vector<Particle> subjets;

};

class GenParticle : public Particle{
 public:
  GenParticle(){
    pdgId=0;
    status=0;
    index=0;
    mother1=0;
    mother2=0;
    daughter1=0;
    daughter2=0;
  };
  ~GenParticle(){
  };
  int pdgId;
  int status;
  int index;
  int mother1;
  int mother2;
  int daughter1;
  int daughter2;
};

#endif 
