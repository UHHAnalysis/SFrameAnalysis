//--sframe new--
#ifndef HepSelectionMods_H
#define HepSelectionMods_H

#include "SFrameTools/include/Objects.h"
#include "SFrameTools/include/Utils.h"
#include "SFrameTools/include/BaseCycleContainer.h"
#include "SFrameTools/include/Selection.h"
#include "SFrameTools/include/EventCalc.h"
#include "SFrameTools/include/HypothesisDiscriminator.h"

#include "SFrameAnalysis/include/EventFilterFromListStandAlone.h"


#include <algorithm>
#include <memory>


namespace HepSelectionMods{


class NAntiMuonHEPTopSelection: public SelectionModule {
public:

  NAntiMuonHEPTopSelection(int min_nbtag, int max_nbtag=int_infinity(), double ptmin=0., double etamax=double_infinity() );
    ~NAntiMuonHEPTopSelection() {};

    virtual bool pass(BaseCycleContainer*);
    virtual std::string description();

private:
    int m_min_nbtag;
    int m_max_nbtag;
    double m_ptmin;
    double m_etamax;
};

class NAntiMuonHEPTopSelectionMatch: public SelectionModule {
public:

  NAntiMuonHEPTopSelectionMatch(int min_nbtag, int max_nbtag=int_infinity(), double ptmin=0., double etamax=double_infinity() );
    ~NAntiMuonHEPTopSelectionMatch() {};

    virtual bool pass(BaseCycleContainer*);
    virtual std::string description();

private:
    int m_min_nbtag;
    int m_max_nbtag;
    double m_ptmin;
    double m_etamax;
};

class NHEPTopTagSelection: public SelectionModule{
 public:
  NHEPTopTagSelection(int min_nheptoptag, int max_nheptoptag=int_infinity());
  ~NHEPTopTagSelection(){};

  virtual bool pass(BaseCycleContainer*);
  virtual std::string description();

 private:
  int m_min_nheptoptag;
  int m_max_nheptoptag;
};


}


#endif // HepSelectionMods_H
