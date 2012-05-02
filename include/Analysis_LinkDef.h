// Dear emacs, this is -*- c++ -*-
// $Id: Analysis_LinkDef.h,v 1.5 2012/04/23 15:04:40 mmeyer Exp $
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
#pragma link C++ class LeptoquarkCycle+;
#pragma link C++ class TauEffiCycle;


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

#pragma link C++ class LuminosityHandler+;
#pragma link C++ struct BaseCycleContainer+;

//PROOF braucht das (warum auch immer):
#pragma link C++ class pair<const string,string*>;
#pragma link C++ class pair<const string,int*>;
#pragma link C++ class pair<const string,double*>;
#pragma link C++ class pair<const string,bool*>;
#pragma link C++ class pair<const string,vector<string>*>;
#pragma link C++ class pair<const string,vector<int>*> ;
#pragma link C++ class pair<const string,vector<double>*>;
#pragma link C++ class pair<const string,vector<bool>*>;
#pragma link C++ class pair<pair<string,string>,TH1*>;
#pragma link C++ class TObjLink;

#endif // __CINT__
