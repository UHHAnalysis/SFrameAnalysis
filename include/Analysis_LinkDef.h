// Dear emacs, this is -*- c++ -*-
// $Id: Analysis_LinkDef.h,v 1.17 2012/09/28 09:45:53 peiffer Exp $
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
//#pragma link C++ class LeptoquarkPreSelectionCycle+;
#pragma link C++ class TauEffiCycle+;
#pragma link C++ class ExampleCycle+;

#pragma link C++ class TopJetHists;
#pragma link C++ class JetHists;
#pragma link C++ class EventHists;
#pragma link C++ class ElectronHists;
#pragma link C++ class MuonHists;

#pragma link C++ class JetLeptonCleanerCycle+;
#pragma link C++ class ZprimePreSelectionCycle+;
#pragma link C++ class ZprimeSelectionCycle+;
#pragma link C++ class TriggerEffiCycle+;


#endif // __CINT__
