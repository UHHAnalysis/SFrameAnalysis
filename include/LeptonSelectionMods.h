#ifndef LeptonSelectionMods_H
#define LeptonSelectionMods_H

#include "SFrameTools/include/Objects.h"
#include "SFrameTools/include/Utils.h"
#include "SFrameTools/include/BaseCycleContainer.h"
#include "SFrameTools/include/Selection.h"
#include "SFrameTools/include/EventCalc.h"
#include "SFrameTools/include/HypothesisDiscriminator.h"

#include "SFrameAnalysis/include/EventFilterFromListStandAlone.h"


#include <algorithm>
#include <memory>


class NMuonSelection: public SelectionModule {
public:
    NMuonSelection(int min_nparticle, int max_nparticle=int_infinity(), double ptmin=0., double etamax=double_infinity() );
    ~NMuonSelection() {};

    virtual bool pass(BaseCycleContainer*);
    virtual std::string description();

private:
    int m_min_nparticle;
    int m_max_nparticle;
    double m_ptmin;
    double m_etamax;
};


class NElectronSelection: public SelectionModule {
public:
    NElectronSelection(
        int min_nparticle, int max_nparticle=int_infinity(),
        double ptmin=0., double etamax=double_infinity() 
    );
    ~NElectronSelection() {};

    virtual bool pass(BaseCycleContainer*);
    virtual std::string description();

private:
    int m_min_nparticle;
    int m_max_nparticle;
    double m_ptmin;
    double m_etamax;
};

class NTauSelection: public SelectionModule {
public:
    NTauSelection(int min_nparticle, int max_nparticle=int_infinity(),  double ptmin=0., double etamax=double_infinity());
    ~NTauSelection() {};

    virtual bool pass(BaseCycleContainer*);
    virtual std::string description();

private:
    int m_min_nparticle;
    int m_max_nparticle;
    double m_ptmin;
    double m_etamax;
};



class NAntiMuonTopJetsSelection: public SelectionModule {
public:

  NAntiMuonTopJetsSelection(int min_ntops, int max_ntops=int_infinity(), double ptmin=0., double etamax=double_infinity());
    ~NAntiMuonTopJetsSelection() {};

    virtual bool pass(BaseCycleContainer*);
    virtual std::string description();

private:
    int m_min_ntops;
    int m_max_ntops;
    double m_ptmin;
    double m_etamax;

};

class NAntiMuonSubBTagSelection: public SelectionModule {
public:

  NAntiMuonSubBTagSelection(int min_nbtag, int max_nbtag=int_infinity(), E_BtagType type=e_CSVM, double ptmin=0., double etamax=double_infinity() , TString filename="");
    ~NAntiMuonSubBTagSelection() {};

    virtual bool pass(BaseCycleContainer*);
    virtual std::string description();

private:
    int m_min_nbtag;
    int m_max_nbtag;
    E_BtagType m_type;
    double m_ptmin;
    double m_etamax;
    std::string m_filename;
};

class NAntiMuonSubBTagSelectionOne: public SelectionModule {
public:

  NAntiMuonSubBTagSelectionOne(int min_nbtag, int max_nbtag=int_infinity(), E_BtagType type=e_CSVM, double ptmin=0., double etamax=double_infinity() , TString filename="");
    ~NAntiMuonSubBTagSelectionOne() {};

    virtual bool pass(BaseCycleContainer*);
    virtual std::string description();

private:
    int m_min_nbtag;
    int m_max_nbtag;
    E_BtagType m_type;
    double m_ptmin;
    double m_etamax;
    std::string m_filename;
};

class NAntiMuonHEPBTagSelection: public SelectionModule {
public:

  NAntiMuonHEPBTagSelection(int min_nbtag, int max_nbtag=int_infinity(), E_BtagType type=e_CSVM, double ptmin=0., double etamax=double_infinity() );
    ~NAntiMuonHEPBTagSelection() {};

    virtual bool pass(BaseCycleContainer*);
    virtual std::string description();

private:
    int m_min_nbtag;
    int m_max_nbtag;
    E_BtagType m_type;
    double m_ptmin;
    double m_etamax;
};


class NMuonBTagSelection: public SelectionModule {
public:

  NMuonBTagSelection(int min_nbtag, int max_nbtag=int_infinity(), E_BtagType type=e_CSVM, double ptmin=0., double etamax=double_infinity() );
    ~NMuonBTagSelection() {};

    virtual bool pass(BaseCycleContainer*);
    virtual std::string description();

private:
    int m_min_nbtag;
    int m_max_nbtag;
    E_BtagType m_type;
    double m_ptmin;
    double m_etamax;
};

class MuonElectronOSCut: public SelectionModule {
public:
    MuonElectronOSCut() {};
    ~MuonElectronOSCut() {};

    virtual bool pass(BaseCycleContainer*);
    virtual std::string description();

};


class MuonTauOSCut: public SelectionModule {
public:
    MuonTauOSCut() {};
    ~MuonTauOSCut() {};

    virtual bool pass(BaseCycleContainer*);
    virtual std::string description();

};


class MuonInvMassCut: public SelectionModule{
 public:
  MuonInvMassCut(double min_InvMass, double max_InvMass);
  ~MuonInvMassCut(){};

  virtual bool pass(BaseCycleContainer*);
  virtual std::string description();

 private:
  double m_min_InvMass;
  double m_max_InvMass;
};

class TauMuonMassCut: public SelectionModule{
 public:
  explicit TauMuonMassCut(double massmin =0, double massmax = double_infinity());
  ~TauMuonMassCut(){};

  virtual bool pass(BaseCycleContainer*);
  virtual std::string description();

 private:
  double m_massmin;
  double m_massmax;

};

class GenTauSelection: public SelectionModule {
public:
    GenTauSelection();
    ~GenTauSelection(){};

    virtual bool pass(BaseCycleContainer*);
    virtual std::string description();

private:   

};


class OneProngTauSelection: public SelectionModule {
public:
    OneProngTauSelection();
    ~OneProngTauSelection(){};

    virtual bool pass(BaseCycleContainer*);
    virtual std::string description();

private:   

};

class TauMuonInvMassCut: public SelectionModule{
 public:
  TauMuonInvMassCut(double min_InvMass, double max_InvMass);
  ~TauMuonInvMassCut(){};

  virtual bool pass(BaseCycleContainer*);
  virtual std::string description();

 private:
  double m_min_InvMass;
  double m_max_InvMass;
};

class SameSignCut: public SelectionModule{
 public:
  SameSignCut();
  ~SameSignCut(){};

  virtual bool pass(BaseCycleContainer*);
  virtual std::string description();

 private:
 
};

class SameSignCutHalil: public SelectionModule{
 public:
   SameSignCutHalil();
  ~SameSignCutHalil(){};

  virtual bool pass(BaseCycleContainer*);
  virtual std::string description();

 private:
 
};



class FakeTauSelectionElectron: public SelectionModule {

public:
    FakeTauSelectionElectron();
    ~FakeTauSelectionElectron(){};

    virtual bool pass(BaseCycleContainer*);
    virtual std::string description();

private:   

};


#endif // LeptonSelectionMods_H
