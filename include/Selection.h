#ifndef Selection_H
#define Selection_H

#include "Objects.h"
#include "BaseCycleContainer.h"
#include <algorithm>


class Selection{

 public:
  Selection(){};
  Selection(BaseCycleContainer*);

  ~Selection(){};

  bool HBHENoiseFilter();

  bool TriggerSelection(std::string name);

  bool NJetSelection(int min_nparticle,  double ptmin=0., double etamax=9999.);
  bool NTopJetSelection(int min_nparticle,  double ptmin=0., double etamax=9999.);

  bool NTopTagSelection(int min_ntoptag);

  bool TopTag(TopJet topjet, double& mjet, int& nsubjets, double& mmin);

  struct HigherPt {
    bool operator() (const Particle& j1, const Particle& j2) const {
      return j1.pt > j2.pt;
    };
  };

 private:

  BaseCycleContainer* bcc;



};


#endif
