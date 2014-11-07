#ifndef BTagSelectionMods_H
#define BTagSelectionMods_H


#include "SFrameTools/include/Objects.h"
#include "SFrameTools/include/Utils.h"
#include "SFrameTools/include/SubJetTagger.h"
#include "SFrameTools/include/BaseCycleContainer.h"
#include "SFrameTools/include/Selection.h"
#include "SFrameTools/include/EventCalc.h"
#include "SFrameTools/include/HypothesisDiscriminator.h"

#include "SFrameAnalysis/include/EventFilterFromListStandAlone.h"



class NBTagAntiktJetSelection: public SelectionModule{
public:
  NBTagAntiktJetSelection(int min_nbtag,int max_nbtag, E_BtagType type=e_CSVT, double delR_Jet_TopTag=1.3 );
  ~NBTagAntiktJetSelection(){}
  
   virtual bool pass(BaseCycleContainer*);
  virtual std::string description();
  
private:
 int m_min_nbtag;
 int m_max_nbtag;
 E_BtagType m_type;
 double m_delR_Jet_TopTag;
};

class NSumBTagsSelection: public SelectionModule{
public:
  NSumBTagsSelection(int min_nsumbtags, int max_nsumbtags, E_BtagType type=e_CSVL);
~NSumBTagsSelection(){}

virtual bool pass(BaseCycleContainer*);
virtual std::string description();

private:
int m_min_nsumbtags;
int m_max_nsumbtags;
E_BtagType m_type;
};


class NBTagSelection: public SelectionModule {
public:

    NBTagSelection(int min_nbtag, int max_nbtag=int_infinity(), E_BtagType type=e_CSVT);
    ~NBTagSelection() {};

    virtual bool pass(BaseCycleContainer*);
    virtual std::string description();

private:
    int m_min_nbtag;
    int m_max_nbtag;
    E_BtagType m_type;
};


//-- Selects events with b-tagged subjet in the CMSTopTagged jets
class NCMSSubBTagSelection: public SelectionModule{
 public:
  NCMSSubBTagSelection(int min_ntoptag, int max_ntoptag, int min_nbtag=0, int max_nbtag=int_infinity(), E_BtagType type=e_CSVM, double nsubjettiness=0.7);
  ~NCMSSubBTagSelection(){};

  virtual bool pass(BaseCycleContainer*);
  virtual std::string description();

 private:
  int m_min_ntoptag;
  int m_max_ntoptag;
  int m_min_nsubjetbtag;
  int m_max_nsubjetbtag;
  E_BtagType m_type;
  CMSTopTagger toptagger;
};

#endif //BTagSelectionMods_H
