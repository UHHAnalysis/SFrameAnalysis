// Dear emacs, this is -*- c++ -*-
// $Id: Analysis_LinkDef.h,v 1.1 2012/04/02 15:28:00 peiffer Exp $
#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ nestedclass;

// Add the declarations of your cycles, and any other classes for which you
// want to generate a dictionary, here. The usual format is:
//
// #pragma link C++ class MySuperClass+;

#pragma link C++ class BaseCycle+;
#pragma link C++ class PileUpHistoCycle+;

#pragma link C++ class Particle+;
#pragma link C++ class std::vector<Particle>+;
#pragma link C++ class Jet+;
#pragma link C++ class std::vector<Jet>+;
#pragma link C++ class TopJet+;
#pragma link C++ class std::vector<TopJet>+;
#pragma link C++ class Electron+;
#pragma link C++ class std::vector<Electron>+;
#pragma link C++ class Muon+;
#pragma link C++ class std::vector<Muon>+;
#pragma link C++ class Tau+;
#pragma link C++ class std::vector<Tau>+;
#pragma link C++ class Photon+;
#pragma link C++ class std::vector<Photon>+;
#pragma link C++ class PrimaryVertex+;
#pragma link C++ class std::vector<PrimaryVertex>+;
#pragma link C++ class MET+;
#pragma link C++ class GenInfo+;
#pragma link C++ class GenParticle+;
#pragma link C++ class std::vector<GenParticle>+;

#endif // __CINT__
