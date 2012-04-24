#ifndef FJet_H
#define FJet_H

#include <vector>
#include "TObjArray.h"
#include "TClonesArray.h"
#include "../include/Objects.h"
#include "fastjet/JetDefinition.hh"
#include "fastjet/AreaDefinition.hh"
#include "fastjet/ClusterSequenceArea.hh"

using namespace std;

class FJet{

 public:

  enum EStrategy{eN2Plain = 0, eN2Tiled, eN2MinHeapTiled, eNlnN, eNlnNCam, eBest};
  enum EAreaType{eActive = 0, eActiveGhostsInJets, ePassiveOneGhost, ePassive, eVoronoi};

  FJet();

  ~FJet();

  void FindJets(const vector<Particle*> parts, vector<Jet*>& jets);
  void PrepareInput(vector<GenParticle> genparts, vector<Particle*>& out);
  vector<int> GetPartJetAssoc();
  
  void Init();
  void Print(const char* usertitle);
  
  // setters
  void SetJetAlgorithm(fastjet::JetAlgorithm algo);
  void SetRecombScheme(fastjet::RecombinationScheme recom);
  void SetRadius(double radius);
  void SetPtCut(double ptmin){fPtMin = ptmin;}

  void CalculateArea(bool flag = true){bUseArea = flag; InitArea();}
  void InitArea();
  void SetAreaType(EAreaType type);
  void SetGhostMaxRap(double max_rap){fGhostSpec->set_ghost_maxrap(max_rap); InitArea();}
  void SetGhostRepeat(int repeat){fGhostSpec->set_repeat(repeat); InitArea();}
  void SetGhostArea(double area){fGhostSpec->set_ghost_area(area); InitArea();}
  void SetGhostGridScatter(double scatter){fGhostSpec->set_grid_scatter(scatter); InitArea();}
  void SetGhostKtScatter(double scatter){fGhostSpec->set_kt_scatter(scatter); InitArea();}
  void SetGhostMeanKt(double meankt){fGhostSpec->set_mean_ghost_kt(meankt); InitArea();}
  void SetVoronoiEffR(double factor){if (fVoronoiSpec){delete fVoronoiSpec;} fVoronoiSpec = new fastjet::VoronoiAreaSpec(factor); InitArea();}

  // getters
  fastjet::JetAlgorithm GetJetAlgorithm();
  fastjet::RecombinationScheme GetRecombScheme();
  double GetExclusiveDmerge(int Njet);
  double GetRadius(){return fR;}
  double GetPtCut(){return fPtMin;}

 private:

  // store the jetfinder
  fastjet::ClusterSequence* fJetFinder;      //! new object every time fastjet is called

  // general fastjet settings
  fastjet::JetAlgorithm fJetAlgo;             //! the required jet algorithm
  fastjet::RecombinationScheme fRecombScheme; //! the required recombination scheme
  fastjet::Strategy fStrategy;                //! the strategy FastJet uses
  double fR;                                  //! jet finder radius
  double fPtMin;                              //! Pt_min of clustered jets

  fastjet::JetDefinition* fJetDef;            //! the jet definition 

  vector<int> fpart_jet_assoc;                //! indices of jet constituents

  // area utilities:
  bool bUseArea;                              //! calculate area? (default: false)
  fastjet::AreaDefinition* fAreaDef;          //! area definition
  fastjet::GhostedAreaSpec* fGhostSpec;       //! ghosted area specifications
  fastjet::VoronoiAreaSpec* fVoronoiSpec;     //! voronoi area specifications
  fastjet::AreaType fGhostAreaType;           //! the requested area type
  bool bVoronoiArea;                          //! calculate voronoi area? (default: false)

};


#endif
