// Dear emacs, this is -*- c++ -*-
// $Id: Analysis_LinkDef.h,v 1.8 2012/05/25 12:47:48 peiffer Exp $
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
#pragma link C++ class TauEffiCycle+;
#pragma link C++ class TestCycle+;
#pragma link C++ class PreSelectionCycle+;

//PROOF braucht das (warum auch immer):
// #pragma link C++ class pair<const string,string*>;
// #pragma link C++ class pair<const string,int*>;
// #pragma link C++ class pair<const string,double*>;
// #pragma link C++ class pair<const string,bool*>;
// #pragma link C++ class pair<const string,vector<string>*>;
// #pragma link C++ class pair<const string,vector<int>*> ;
// #pragma link C++ class pair<const string,vector<double>*>;
// #pragma link C++ class pair<const string,vector<bool>*>;
// #pragma link C++ class pair<pair<string,string>,TH1*>;
// #pragma link C++ class TObjLink;

#endif // __CINT__
