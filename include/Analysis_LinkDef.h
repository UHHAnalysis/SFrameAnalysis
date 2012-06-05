// Dear emacs, this is -*- c++ -*-
// $Id: Analysis_LinkDef.h,v 1.10 2012/05/31 09:01:42 peiffer Exp $
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
#pragma link C++ class BaseCycle+;
#pragma link C++ class PileUpHistoCycle+;
#pragma link C++ class LeptoquarkCycle+;
#pragma link C++ class TauEffiCycle+;
#pragma link C++ class TestCycle+;
#pragma link C++ class PreSelectionCycle+;
#pragma link C++ class SelectionCycle+;
#pragma link C++ class ExampleCycle;


#endif // __CINT__
