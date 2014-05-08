//--sframe new--
#ifndef SelectionModules_H
#define SelectionModules_H

#include "SFrameTools/include/Objects.h"
#include "SFrameTools/include/Utils.h"
#include "SFrameTools/include/BaseCycleContainer.h"
#include "SFrameTools/include/Selection.h"
#include "SFrameTools/include/EventCalc.h"
#include "SFrameTools/include/HypothesisDiscriminator.h"

#include "SFrameAnalysis/include/EventFilterFromListStandAlone.h"


#include "SFrameAnalysis/include/CMSTopTagSelectionMods.h"
#include "SFrameAnalysis/include/JetSelectionMods.h"
#include "SFrameAnalysis/include/LeptonSelectionMods.h"
#include "SFrameAnalysis/include/HepSelectionMods.h"
#include "SFrameAnalysis/include/BTagSelectionMods.h"
#include "SFrameAnalysis/include/HTSelectionMods.h"

#include <algorithm>
#include <memory>


//------------------------------------
// The first part of the name of the selection decides in which file it is stored eg:
// MuonBTagSelection --> LeptonSelectionMods
// HEPMuonSelection  --> HepSelectionMods
//-----------------------------------------


class TriggerSelection: public SelectionModule {
public:
    TriggerSelection(std::string);
    ~TriggerSelection() {};

    virtual bool pass(BaseCycleContainer*);
    virtual std::string description();

private:
    std::string m_name;
};


class TopTagOverlapSelection: public SelectionModule {
 public:
  TopTagOverlapSelection(double delR_Lep_TopTag = 0.8 , double delR_Jet_TopTag = 1.3);
  ~TopTagOverlapSelection(){};

  virtual bool pass(BaseCycleContainer*);
  virtual std::string description();


 private:
  double m_delR_Lep_TopTag;
  double m_delR_Jet_TopTag;
}__attribute__ ((deprecated)); // moved to CMSTopTagSelectionMods.h and renamed 


class RazorSelection: public SelectionModule {
 public:
  RazorSelection(HypothesisDiscriminator *discr, double mrazor = 1050, double mrazorT = 0.54);
  ~RazorSelection(){}

  virtual bool pass(BaseCycleContainer*);
  virtual std::string description();


 private:
  double m_mrazor;
  double m_mrazorT;
  HypothesisDiscriminator *m_discr;
};



class NTopJetSelection: public SelectionModule {
public:
    NTopJetSelection(int min_nparticle, int max_nparticle=int_infinity(),  double ptmin=0., double etamax=double_infinity());
    ~NTopJetSelection() {};

    virtual bool pass(BaseCycleContainer*);
    virtual std::string description();

private:
    int m_min_nparticle;
    int m_max_nparticle;
    double m_ptmin;
    double m_etamax;
};

class NPrunedJetSelection: public SelectionModule {
public:
    NPrunedJetSelection(int min_nparticle, int max_nparticle=int_infinity(),  double ptmin=0., double etamax=double_infinity());
    ~NPrunedJetSelection() {};

     virtual bool pass(BaseCycleContainer*);
    virtual std::string description();

private:
    int m_min_nparticle;
    int m_max_nparticle;
    double m_ptmin;
    double m_etamax;
};

class CAAntiktJetSelection : public SelectionModule{
 public:
  CAAntiktJetSelection(unsigned int min_Topjets, unsigned int min_Jets, double min_distance,unsigned int max_Topjets=int_infinity(),unsigned int max_Jets=int_infinity());

  virtual bool pass(BaseCycleContainer*);
  virtual std::string description();

 private:
  unsigned int m_min_Topjets; 		   
  unsigned int m_max_Topjets;
  unsigned int m_min_Jets;		   
  unsigned int m_max_Jets;   
  double m_min_distance;           
};

class TopTagAntiktJetSelection : public SelectionModule{
 public:
  TopTagAntiktJetSelection(unsigned int min_TopTag, unsigned int min_Jets, double min_distance,unsigned int max_TopTag=int_infinity(),unsigned int max_Jets=int_infinity());

  virtual bool pass(BaseCycleContainer*);
  virtual std::string description();

 private:
  unsigned int m_min_TopTag; 		   
  unsigned int m_max_TopTag;
  unsigned int m_min_Jets;		   
  unsigned int m_max_Jets;   
  double m_min_distance;           
}__attribute__ ((deprecated)); //moved to CMSTopTagSelectionMods.h and renamed 


class NTopTagSelection: public SelectionModule {
public:
    NTopTagSelection(int min_ntoptag, int max_ntoptag=int_infinity());
    ~NTopTagSelection() {};


    virtual bool pass(BaseCycleContainer*);
    virtual std::string description();

private:
    int m_min_ntoptag;
    int m_max_ntoptag;
}__attribute__ ((deprecated)); //moved to CMSTopTagSelectionMods.h and renamed 


// Selects events with b-tagged subjet in the HEPTopTagged jets
class NHEPTopAndSubBTagSelection: public SelectionModule{
 public:
  NHEPTopAndSubBTagSelection(int min_nheptoptag, int max_nheptoptag, E_BtagType type, TString mode="default", TString filename="");
  ~NHEPTopAndSubBTagSelection(){};

  virtual bool pass(BaseCycleContainer*);
  virtual std::string description();

 private:
  int m_min_nheptoptag;
  int m_max_nheptoptag;
  E_BtagType m_type;
   TString m_mode;
  TString m_filename;
};

class HEPTopAndSubBTagPlusOtherHiggsTag: public SelectionModule{
 public:
  HEPTopAndSubBTagPlusOtherHiggsTag(E_BtagType type1, E_BtagType type2, E_BtagType type3, TString mode="default", TString filename="", double HiggsMassCut=0.);
  ~HEPTopAndSubBTagPlusOtherHiggsTag(){};

  virtual bool pass(BaseCycleContainer*);
  virtual std::string description();

 private:
  E_BtagType m_type1;
  E_BtagType m_type2;
  E_BtagType m_type3;
 TString m_mode;
  TString m_filename;
  double m_HiggsMassCut;
};


class InvertedTopTagRegularBTagRegularHiggsTag: public SelectionModule{
 public:
  InvertedTopTagRegularBTagRegularHiggsTag(E_BtagType type1, E_BtagType type2, E_BtagType type3, TString mode="default", TString filename="", double HiggsMassCut=0.);
  ~InvertedTopTagRegularBTagRegularHiggsTag(){};

  virtual bool pass(BaseCycleContainer*);
  virtual std::string description();

 private:
  E_BtagType m_type1;
  E_BtagType m_type2;
  E_BtagType m_type3;
  /* bool m_doBTag; */
/*   bool m_doHiggsTag; */
 TString m_mode;
  TString m_filename;
double m_HiggsMassCut;
}__attribute__ ((deprecated)); //moved to HepSelectionMods.h and renamed ;



class RegularTopTagRegularBTagFullyInvertedHiggsTag: public SelectionModule{
 public:
  RegularTopTagRegularBTagFullyInvertedHiggsTag(E_BtagType type1, E_BtagType type2, E_BtagType type3, TString mode="default", TString filename="");
  ~RegularTopTagRegularBTagFullyInvertedHiggsTag(){};

  virtual bool pass(BaseCycleContainer*);
  virtual std::string description();

 private:
  E_BtagType m_type1;
  E_BtagType m_type2;
  E_BtagType m_type3;
 TString m_mode;
  TString m_filename;
}__attribute__ ((deprecated)); //moved to HepSelectionMods.h and renamed;


class InvertedTopTagRegularBTagFullyInvertedHiggsTag: public SelectionModule{
 public:
  InvertedTopTagRegularBTagFullyInvertedHiggsTag(E_BtagType type1, E_BtagType type2, E_BtagType type3, TString mode="default", TString filename="");
  ~InvertedTopTagRegularBTagFullyInvertedHiggsTag(){};

  virtual bool pass(BaseCycleContainer*);
  virtual std::string description();

 private:
  E_BtagType m_type1;
  E_BtagType m_type2;
  E_BtagType m_type3;
 TString m_mode;
  TString m_filename;
}__attribute__ ((deprecated)); //moved to HepSelectionMods.h and renamed;


class NWTagSelection: public SelectionModule {
public:
    NWTagSelection(int min_nwtag, int max_nwtag=int_infinity());
    ~NWTagSelection() {};

    virtual bool pass(BaseCycleContainer*);
    virtual std::string description();

private:
    int m_min_nwtag;
    int m_max_nwtag;
};



class NPrimaryVertexSelection: public SelectionModule {
public:
    NPrimaryVertexSelection(int min_npv=1, int max_npv=int_infinity());
    ~NPrimaryVertexSelection() {};

    virtual bool pass(BaseCycleContainer*);
    virtual std::string description();

private:
    int m_min_npv;
    int m_max_npv;
};

class METCut: public SelectionModule {
public:
    METCut(double min_met, double max_met=double_infinity());
    ~METCut() {};

    virtual bool pass(BaseCycleContainer*);
    virtual std::string description();

private:
    double m_min_met;
    double m_max_met;
};

class IsoConeSelection: public SelectionModule {
public:
	IsoConeSelection(TString type="mu", double a=29.1356, double b=164.383, double c=0.023111, double iso_num=0.2);
	~IsoConeSelection() {};

    virtual bool pass(BaseCycleContainer*);
    virtual std::string description();

private:
TString m_type;
double m_a;
double m_b;
double m_c;
double m_iso;
};


class TwoDCut: public SelectionModule {
public:
    TwoDCut() {};
    ~TwoDCut() {};

    virtual bool pass(BaseCycleContainer*);
    virtual std::string description();

};

class TwoDCutMuon: public SelectionModule {
public:
    TwoDCutMuon(double delRmin= 0.5 , double pTrelmin = 25);
    ~TwoDCutMuon() {};

    virtual bool pass(BaseCycleContainer*);
    virtual std::string description();

private:
    double m_delRmin;
    double m_pTrelmin;

};



class TriangularCut: public SelectionModule {
public:
    TriangularCut() {};
    ~TriangularCut() {};

    virtual bool pass(BaseCycleContainer*);
    virtual std::string description();

};

class TriangularCutMuon: public SelectionModule {
public:
    TriangularCutMuon() {};
    ~TriangularCutMuon() {};

    virtual bool pass(BaseCycleContainer*);
    virtual std::string description();

};


class HypothesisDiscriminatorCut: public SelectionModule {
public:
    HypothesisDiscriminatorCut(HypothesisDiscriminator* discr, double min_discr, double max_discr);
    ~HypothesisDiscriminatorCut() {};

    virtual bool pass(BaseCycleContainer*);
    virtual std::string description();

private:
    double m_max_discr;
    double m_min_discr;
    HypothesisDiscriminator* m_discr;

};

class HypothesisLeptopPtCut: public SelectionModule {
public:
    HypothesisLeptopPtCut(HypothesisDiscriminator* discr, double min_discr, double max_discr);
    ~HypothesisLeptopPtCut() {};

    virtual bool pass(BaseCycleContainer*);
    virtual std::string description();

private:
    double m_max_discr;
    double m_min_discr;
    HypothesisDiscriminator* m_discr;

};



class MttbarGenCut: public SelectionModule {
public:
    MttbarGenCut(double mttbar_min=0, double mttbar_max=double_infinity());
    ~MttbarGenCut() {};

    virtual bool pass(BaseCycleContainer*);
    virtual std::string description();

private:
    double m_mttbar_min;
    double m_mttbar_max;

};

class ZGenCut: public SelectionModule {
public:
    ZGenCut(int pdgid);
    ~ZGenCut() {};

    virtual bool pass(BaseCycleContainer*);
    virtual std::string description();

private:
    int m_pdgid;

};



class EventFlavorSelection: public SelectionModule {
public:
    explicit EventFlavorSelection(E_EventFlavor flavor);
    ~EventFlavorSelection() {};

    virtual bool pass(BaseCycleContainer*);
    virtual std::string description();

private:

    E_EventFlavor m_flavor;
};

class EventFilterSelection: public SelectionModule {
public:
    explicit EventFilterSelection(const std::string & eventfile);
    ~EventFilterSelection(){}

    virtual bool pass(BaseCycleContainer*);
    virtual std::string description();

private:
    std::string m_filename;
    std::auto_ptr<EventFilterFromListStandAlone> m_evfilter;
};


class NoSelection: public SelectionModule {
 public:
  explicit NoSelection(bool no_sel=true);
  ~NoSelection(){};

  virtual bool pass(EventCalc & );
  virtual std::string description();

 private:
  bool m_no_sel;

};

class HadronicEventSelection: public SelectionModule {
public:
   HadronicEventSelection();
   ~HadronicEventSelection(){};

   virtual bool pass(BaseCycleContainer*);
   virtual std::string description();

private:  

};


#endif
