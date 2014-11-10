//--sframe new--
#ifndef CMSTopTagSelectionMods_H
#define CMSTopTagSelectionMods_H

#include "SFrameTools/include/Objects.h"
#include "SFrameTools/include/Utils.h"
#include "SFrameTools/include/BaseCycleContainer.h"
#include "SFrameTools/include/Selection.h"
#include "SFrameTools/include/EventCalc.h"
#include "SFrameTools/include/HypothesisDiscriminator.h"

//#include "SFrameTools/include/SubJetTagger.h"

#include "SFrameAnalysis/include/EventFilterFromListStandAlone.h"


#include <algorithm>
#include <memory>


class CMSTopTagOverlapSelection: public SelectionModule {
 public:
  CMSTopTagOverlapSelection(double delR_Lep_TopTag = 0.8 , double delR_Jet_TopTag = 1.3, double tau32Cut=0.7);
  ~CMSTopTagOverlapSelection(){};

  virtual bool pass(BaseCycleContainer*);
  virtual std::string description();


 private:
  double m_delR_Lep_TopTag;
  double m_delR_Jet_TopTag;
  double m_tau32Cut;
};


class CMSTopTagAntiktJetSelection : public SelectionModule{
 public:
  CMSTopTagAntiktJetSelection(unsigned int min_TopTag, unsigned int min_Jets, double min_distance,unsigned int max_TopTag=int_infinity(),unsigned int max_Jets=int_infinity(), double tau32Cut=0.7);

  virtual bool pass(BaseCycleContainer*);
  virtual std::string description();

 private:
  unsigned int m_min_TopTag; 		   
  unsigned int m_max_TopTag;
  unsigned int m_min_Jets;		   
  unsigned int m_max_Jets;   
  double m_min_distance;  
  double m_tau32Cut;       
};


class NCMSTopTagSelection: public SelectionModule {
public:
  NCMSTopTagSelection(int min_ntoptag, int max_ntoptag=int_infinity(), double tau32Cut=0.7);
    ~NCMSTopTagSelection() {};


    virtual bool pass(BaseCycleContainer*);
    virtual std::string description();

private:
    int m_min_ntoptag;
    int m_max_ntoptag;
    double m_tau32Cut;
    //CMSTopTagger CMSTopTag;
};


#endif //CMSTopTagSelectionMods_H


//__attribute__ ((deprecated))
