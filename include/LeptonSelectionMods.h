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


namespace LeptonSelectionMods{


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









}


#endif // LeptonSelectionMods_H
