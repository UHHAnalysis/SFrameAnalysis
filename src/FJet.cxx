#include "../include/FJet.h"
#include "../include/Objects.h"

#include <vector>
#include "Math/LorentzVector.h"
#include "Math/PtEtaPhiE4D.h"
#include "TString.h"
#include "TObject.h"
#include "TMath.h"

typedef ROOT::Math::LorentzVector< ROOT::Math::PtEtaPhiE4D< Double32_t > > LorentzVector;
using namespace std;

FJet::FJet()
{
  // standard constructor, use the following as defaults:
  // jet algorithm = Cam/Aachen
  // radius = 0.8
  // Pt_min = 3.0 GeV
  // recombination scheme = E-scheme (four-vector addition)
  // clustering strategy = best 

  fJetAlgo = fastjet::cambridge_algorithm;
  //fJetAlgo = fastjet::kt_algorithm;
  fR = 0.8;
  fPtMin = 10.0;
  fRecombScheme = fastjet::E_scheme; 
  fStrategy = fastjet::Best;
  fJetDef = new fastjet::JetDefinition(fJetAlgo, fR, fRecombScheme, fStrategy);
   
  // the jet finder, initialise it to NULL
  fJetFinder = NULL;
  
  // default settings for the area treatment
  bUseArea = false;
  fAreaDef = NULL;
  fGhostSpec = new fastjet::GhostedAreaSpec(4.1, 1, 0.02, 1.0, 0.1, 1e-100);
  fVoronoiSpec = new fastjet::VoronoiAreaSpec();
  fGhostAreaType = fastjet::active_area;
  bVoronoiArea = false;

}

FJet::~FJet()
{
  // destructor, clean up
  if (fJetDef) delete fJetDef;
  if (fAreaDef) delete fAreaDef;
  if (fGhostSpec) delete fGhostSpec;
  if (fVoronoiSpec) delete fVoronoiSpec;
  fJetDef=NULL;
  fAreaDef=NULL;
  fGhostSpec=NULL;
  fVoronoiSpec=NULL;

}

void FJet::PrepareInput(vector<GenParticle> genparts, vector<Particle*>& out)
{
  // fill the input array for the jetfinder with 
  // pointers to objects of type Particle, a memcopy is used

  if (out.size()!=0){
    out.clear();
  }

  for (unsigned int i=0; i<genparts.size(); ++i){
    GenParticle* gen = new GenParticle();
    memcpy(gen, &genparts[i], sizeof(genparts[i]));
    Particle* part = (Particle*)gen;
    out.push_back(part);
  }
  return;

}

void FJet::FindJets(const vector<Particle*> ParticlesIn, vector<Jet*>& JetsOut)
{

  // the main routine:
  // jet finding using FastJet

  cout << "Main finding routine..." << endl;

  // clear the list if it is filled
  if (JetsOut.size()!=0){
    JetsOut.clear();
  }
    
  unsigned int NumParticles = ParticlesIn.size();
  if (NumParticles == 0) return;  

  // this is the input list for the fastjet finder
  vector<fastjet::PseudoJet> InputParticles;

  LorentzVector v;
  // loop over the given particles and fill the input list
  for (unsigned int ipart=0; ipart<NumParticles; ++ipart){
    Particle* part = ParticlesIn[ipart];
    v = part->v4();
    InputParticles.push_back(fastjet::PseudoJet(v.Px(),v.Py(),v.Pz(),v.E()));     
  }

  // !!!!!!!!  run the jet-finder !!!!!!!!!!!!!
  // delete the old one, if it exists
  if (fJetFinder){
    if (!bUseArea) delete fJetFinder;
    else delete (fastjet::ClusterSequenceArea*) fJetFinder; 
  }
  if (!bUseArea){
    fJetFinder = new fastjet::ClusterSequence(InputParticles, *fJetDef);
  } else {
    fJetFinder = (fastjet::ClusterSequence*) new fastjet::ClusterSequenceArea(InputParticles, *fJetDef, *fAreaDef);
  }

  // get the jets
  vector<fastjet::PseudoJet> Jets = fJetFinder->inclusive_jets(fPtMin);

  // return if no jets were found
  if (Jets.size() == 0){
    return; 
  }

  // sort jets by increasing pt
  vector<fastjet::PseudoJet> SortedJets = sorted_by_pt(Jets);

  // fill the jets, more sophisticated variables could be calculated (to come)
  for (unsigned int ijet = 0; ijet < SortedJets.size(); ijet++) { 
    Jet* jet = new Jet();
    JetsOut.push_back(jet);
    jet->pt = SortedJets[ijet].pt();
    jet->eta = SortedJets[ijet].eta();
    jet->phi = SortedJets[ijet].phi();
    jet->energy = SortedJets[ijet].E();
  }

  // get the association of jetparticles to jets
  fpart_jet_assoc.clear();
  fpart_jet_assoc = fJetFinder->particle_jet_indices(SortedJets);

  // loop over the input particles and calculate wanted quantities (charged hadron fraction...)
  // not used at the moment
  //for (unsigned int ipart=0; ipart<NumParticles; ++ipart){
  //     // particle not inside any jet
  //     if (fpart_jet_assoc[ipart]==-1){
  //       continue;
  //     }    
  //     // get the according jet number and store this particle there
  //     int jetnum = fpart_jet_assoc[ipart];

  //  if (fpart_jet_assoc[ipart]==IJet){
    // do something with it
  //  }
  //}
  
  return;
}

vector<int> FJet::GetPartJetAssoc()
{
  // returns the jet indices for each particle. 
  // if particle k is in jet j, then the returned vector 
  // has value j at position k

  return fpart_jet_assoc;  
}


void FJet::Init()
{
  // initialize the jet definition 
  // always performed when a parameter changes
  if (fJetDef) delete fJetDef;
  fJetDef = new fastjet::JetDefinition(fJetAlgo, fR, fRecombScheme, fStrategy);
}

void FJet::InitArea()
{
  // initialize the area definition
  // always performed when an area parameter changed
  
  if (fAreaDef) delete fAreaDef;

  if (bVoronoiArea){
    fAreaDef = new fastjet::AreaDefinition(*fVoronoiSpec);
  } else {
    fAreaDef = new fastjet::AreaDefinition(fGhostAreaType, *fGhostSpec);
  }

}

void FJet::SetJetAlgorithm(fastjet::JetAlgorithm algo)
{
  // set the jet algorithm, 
  // possibilities: kt, anti-kt, cambridge-aachen
  
  fJetAlgo = algo;
  Init();   
}


void FJet::SetRadius(double radius)
{
  // set the used 'radius' 
  // this is the 1/R term for the kt, anti-kt and Cam/Aachen

  fR = radius; 
  Init(); 

}


void FJet::SetRecombScheme(fastjet::RecombinationScheme recom)
{
  // set the recombination scheme, 
  // possibilities: pt, pt2, Et, Et2, E

  fRecombScheme = recom; 
  Init(); 

}

//_________________  getters _______________________

fastjet::JetAlgorithm FJet::GetJetAlgorithm()
{
  // get the jet algorithm
  return fJetAlgo;
}


fastjet::RecombinationScheme FJet::GetRecombScheme()
{
  // get the recombination scheme, returns unknown if SISCone was used
  return fRecombScheme;
}

double FJet::GetExclusiveDmerge(int Njet)
{
  // Return the distance parameter dmin from the jet finding algorithm 
  // corresponding to the recombination that went from n+1 to n jets (=Njet)
  // If the number of particles in the event is <= njets, the function returns 0. 
  // Only call this function after the jet finder has been run

  if (!fJetFinder) return 0;
  return fJetFinder->exclusive_dmerge(Njet);

}


//_________________  print information _______________________

void FJet::Print(const char* usertitle)
{
  // print the settings to the default output

  cout << endl;
  cout << "+---------------------------------------------------------------" << endl;
  cout << "| Settings of the FastJet Interface (" << usertitle << ")" << endl;
  cout << "+---------------------------------------------------------------" << endl; 
  cout << "|" << endl << "| Jet-finder: " << endl;
  TString des(fJetDef->description());
  des.ReplaceAll(",", ",\n| ");
  cout << "|  " << des << endl;
  cout << "|  Minimal Pt cut for inclusive jets is set to " << fPtMin << " GeV" << endl;

  cout << "|  Strategy used by FastJet is ";
  switch(fJetDef->strategy()){
  case(-4): cout << "N^2 Minimum Heap Tiled (fastest for 500 < N < 10^4)" << endl; break;
  case(-3): cout << "N^2 Tiled (fastest for 50 < N < 500)" << endl; break;
  case(-2): cout << "N^2 Poor Tiled (legacy)" << endl; break;
  case(-1): cout << "N^2 Plain (fastest for N < 50)" << endl; break;
  case(0): cout << "N^3 Dumb (slowest variant)" << endl; break;
  case(1): cout << "automatic selection depending on N" << endl; break;
  case(2): cout << "N ln(N) (fastest for N > 10^4)" << endl; break;
  case(3): cout << "N ln(N)3pi (legacy)" << endl; break;
  case(4): cout << "N ln(N)4pi (legacy)" << endl; break;
  case(12): cout << "N ln(N) Cambridge (exclusively used for cambridge algorithm)" << endl; break;
  case(13): cout << "N ln(N) Cambridge 2pi2R (exclusively used for cambridge algorithm)" << endl; break;
  case(14): cout << "N ln(N) Cambridge 4pi (exclusively used for cambridge algorithm)" << endl; break;
  case(999): cout << "SISCone, which uses its own strategy" << endl; break;
  default: cout << "unknown strategy" << endl; break;
  }
  cout << "|" << endl;
  if (!bUseArea){
    cout << "| No area calculation was performed." << endl; 
  } else {
    if (bVoronoiArea){
      cout << "| Area calculation was performed with the Voronoi area definition." << endl; 
      cout << "|  "<< fVoronoiSpec->description() << endl;
    } else {
      cout << "| Area calculation was performed with Ghost particles:" << endl;
      if (fGhostAreaType == fastjet::active_area) cout << "|  Active ghost area was used. " << endl;
      if (fGhostAreaType == fastjet::active_area_explicit_ghosts) cout << "|  Active ghost area was used, ghost particles were included in output jets." << endl;
      if (fGhostAreaType == fastjet::one_ghost_passive_area) cout << "|  Passive ghost area was used, clustering the event with one ghost at a time." << endl;
      if (fGhostAreaType == fastjet::passive_area) cout << "|  Passive ghost area was used, sped up with information specific to the jet-finder algorithm." << endl;
      TString des(fGhostSpec->description());
      des.Prepend("|  ");
      des.ReplaceAll(",", ",\n| ");
      cout << des << endl;
    }
  }
  cout << "|" << endl;
  cout << "+---------------------------------------------------------------" << endl; 

}
