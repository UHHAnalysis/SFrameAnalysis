//--sframe new--
#ifndef HTSelectionMods_H
#define HTSelectionMods_H

#include "SFrameTools/include/Objects.h"
#include "SFrameTools/include/Utils.h"
#include "SFrameTools/include/BaseCycleContainer.h"
#include "SFrameTools/include/Selection.h"
#include "SFrameTools/include/EventCalc.h"
#include "SFrameTools/include/HypothesisDiscriminator.h"

#include "SFrameAnalysis/include/EventFilterFromListStandAlone.h"

#include <algorithm>
#include <memory>



class STCut: public SelectionModule{
 public:
  STCut(double min_st, double max_st=double_infinity());
  ~STCut(){};

  virtual bool pass(BaseCycleContainer*);
  virtual std::string description();

 private:
  double m_min_st;
  double m_max_st;
};

class HTCut: public SelectionModule{
 public:
  HTCut(double min_ht, double max_ht=double_infinity());
  ~HTCut(){};

  virtual bool pass(BaseCycleContainer*);
  virtual std::string description();

 private:
  double m_min_ht;
  double m_max_ht;
};


class HTSubJetsCut: public SelectionModule{
 public:
  HTSubJetsCut(double min_ht, double max_ht=double_infinity());
  ~HTSubJetsCut(){};

  virtual bool pass(BaseCycleContainer*);
  virtual std::string description();

 private:
  double m_min_ht;
  double m_max_ht;
};

class HThadCut: public SelectionModule{
 public:
   HThadCut(double ptmin_jet,double etamax_jet, double min_ht, double max_ht=double_infinity());
  ~HThadCut(){};

  virtual bool pass(BaseCycleContainer*);
  virtual std::string description();

 private:
   double m_ptmin_jet;
   double m_etamax_jet;
   double m_min_ht;
   double m_max_ht;
};

class HTlepCut: public SelectionModule {
public:
    HTlepCut(double min_htlep, double max_htlep=double_infinity());
    ~HTlepCut() {};

    virtual bool pass(BaseCycleContainer*);
    virtual std::string description();

private:
    double m_min_htlep;
    double m_max_htlep;
};

class HTmuonCut: public SelectionModule {
public:
    HTmuonCut(double min_htmuon, double max_htmuon=double_infinity());
    ~HTmuonCut() {};

    virtual bool pass(BaseCycleContainer*);
    virtual std::string description();

private:
    double m_min_htmuon;
    double m_max_htmuon;
};




#endif
