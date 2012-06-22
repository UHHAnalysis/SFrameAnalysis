// Dear emacs, this is -*- c++ -*-
// $Id: Analysis_LinkDef.h,v 1.13 2012/06/12 13:50:24 rkogler Exp $
#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ nestedclass;

// Add the declarations of your cycles, and any other classes for which you
// want to generate a dictionary, here. The usual format is:
//
// #pragma link C++ class MySuperClass+;

#pragma link C++ class AnalysisCycle+;
#pragma link C++ class PileUpHistoCycle+;
//#pragma link C++ class LeptoquarkCycle+;
#pragma link C++ class TauEffiCycle+;
#pragma link C++ class ExampleCycle+;
#pragma link C++ class JetLeptonCleanerCycle+;
#pragma link C++ class ZprimePreSelectionCycle+;
#pragma link C++ class ZprimeSelectionCycle+;


#endif // __CINT__
