//--sframe new--
#ifndef JetSelectionMods_H
#define JetSelectionMods_H

#include "SFrameTools/include/Objects.h"
#include "SFrameTools/include/Utils.h"
#include "SFrameTools/include/BaseCycleContainer.h"
#include "SFrameTools/include/Selection.h"
#include "SFrameTools/include/EventCalc.h"
#include "SFrameTools/include/HypothesisDiscriminator.h"

#include "SFrameAnalysis/include/EventFilterFromListStandAlone.h"


#include <algorithm>
#include <memory>



class NJetSelection: public SelectionModule {
public:
    NJetSelection(int min_nparticle, int max_nparticle=int_infinity(),  double ptmin=0., double etamax=double_infinity());
    ~NJetSelection() {};

    virtual bool pass(BaseCycleContainer*);
    virtual std::string description();

private:
    int m_min_nparticle;
    int m_max_nparticle;
    double m_ptmin;
    double m_etamax;
};

class NJetdeltaySelection: public SelectionModule {
public:
    NJetdeltaySelection(double ptmin=0.);
    ~NJetdeltaySelection() {};

    virtual bool pass(BaseCycleContainer*);
    virtual std::string description();

private:
    double m_ptmin;
};


class NJetdeltaphiSelection: public SelectionModule {
public:
    NJetdeltaphiSelection(double ptmin=0.);
    ~NJetdeltaphiSelection() {};

    virtual bool pass(BaseCycleContainer*);
    virtual std::string description();

private:
    double m_ptmin;
};

class NTopJetdeltaphiSelection: public SelectionModule {
public:
    NTopJetdeltaphiSelection(double ptmin=0.);
    ~NTopJetdeltaphiSelection() {};

    virtual bool pass(BaseCycleContainer*);
    virtual std::string description();

private:
    double m_ptmin;
};







#endif // JetSelectionMods_H 
