#ifndef SelectionModules_H
#define SelectionModules_H

#include "Objects.h"
#include "Utils.h"
#include "BaseCycleContainer.h"
#include <algorithm>
#include "Selection.h"


class TriggerSelection: public SelectionModule{
 public:
  TriggerSelection(std::string);
  ~TriggerSelection(){};

  virtual bool pass(BaseCycleContainer*);
  virtual std::string description();

 private:
  std::string m_name;

};

class NMuonSelection: public SelectionModule{
 public:
  NMuonSelection(int min_nparticle, int max_nparticle=int_infinity(), double ptmin=0., double etamax=double_infinity() );
  ~NMuonSelection(){};

  virtual bool pass(BaseCycleContainer*);
  virtual std::string description();

 private:
  int m_min_nparticle;
  int m_max_nparticle;
  double m_ptmin;
  double m_etamax;
};

class NElectronSelection: public SelectionModule{
 public:
  NElectronSelection(int min_nparticle, int max_nparticle=int_infinity(),  double ptmin=0., double etamax=double_infinity());
  ~NElectronSelection(){};

  virtual bool pass(BaseCycleContainer*);
  virtual std::string description();

 private:
  int m_min_nparticle;
  int m_max_nparticle;
  double m_ptmin;
  double m_etamax;
};

class NTauSelection: public SelectionModule{
 public:
  NTauSelection(int min_nparticle, int max_nparticle=int_infinity(),  double ptmin=0., double etamax=double_infinity());
  ~NTauSelection(){};

  virtual bool pass(BaseCycleContainer*);
  virtual std::string description();

 private:
  int m_min_nparticle;
  int m_max_nparticle;
  double m_ptmin;
  double m_etamax;
};


class NJetSelection: public SelectionModule{
 public:
  NJetSelection(int min_nparticle, int max_nparticle=int_infinity(),  double ptmin=0., double etamax=double_infinity());
  ~NJetSelection(){};

  virtual bool pass(BaseCycleContainer*);
  virtual std::string description();

 private:
  int m_min_nparticle;
  int m_max_nparticle;
  double m_ptmin;
  double m_etamax;
};

class NTopJetSelection: public SelectionModule{
 public:
  NTopJetSelection(int min_nparticle, int max_nparticle=int_infinity(),  double ptmin=0., double etamax=double_infinity());
  ~NTopJetSelection(){};

  virtual bool pass(BaseCycleContainer*);
  virtual std::string description();

 private:
  int m_min_nparticle;
  int m_max_nparticle;
  double m_ptmin;
  double m_etamax;
};

class NTopTagSelection: public SelectionModule{
 public:
  NTopTagSelection(int min_ntoptag, int max_ntoptag=int_infinity());
  ~NTopTagSelection(){};

  virtual bool pass(BaseCycleContainer*);
  virtual std::string description();

 private:
  int m_min_ntoptag;
  int m_max_ntoptag;
};



class NBTagSelection: public SelectionModule{
 public:

  NBTagSelection(int min_nbtag, int max_nbtag=int_infinity(), E_BtagType type=e_CSVT);
  ~NBTagSelection(){};

  virtual bool pass(BaseCycleContainer*);
  virtual std::string description();

 private:
  int m_min_nbtag;
  int m_max_nbtag;
  E_BtagType m_type;
};



class NPrimaryVertexSelection: public SelectionModule{
 public:
  NPrimaryVertexSelection(int min_npv=1, int max_npv=int_infinity());
  ~NPrimaryVertexSelection(){};

  virtual bool pass(BaseCycleContainer*);
  virtual std::string description();

 private:
  int m_min_npv;
  int m_max_npv;
};


class HTlepCut: public SelectionModule{
 public:
  HTlepCut(double min_htlep, double max_htlep=double_infinity());
  ~HTlepCut(){};

  virtual bool pass(BaseCycleContainer*);
  virtual std::string description();

 private:
  double m_min_htlep;
  double m_max_htlep;
};

class METCut: public SelectionModule{
 public:
  METCut(double min_met, double max_met=double_infinity());
  ~METCut(){};

  virtual bool pass(BaseCycleContainer*);
  virtual std::string description();

 private:
  double m_min_met;
  double m_max_met;
};

class TwoDCut: public SelectionModule{
 public:
  TwoDCut(){};
  ~TwoDCut(){};

  virtual bool pass(BaseCycleContainer*);
  virtual std::string description();

};

class TriangularCut: public SelectionModule{
 public:
  TriangularCut(){};
  ~TriangularCut(){};

  virtual bool pass(BaseCycleContainer*);
  virtual std::string description();

};

#endif
