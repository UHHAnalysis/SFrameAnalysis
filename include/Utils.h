#ifndef Utils_H
#define Utils_H

#include "Objects.h"

struct HigherPt {
  bool operator() (const Particle& j1, const Particle& j2) const {
    return j1.pt() > j2.pt();
  };
};


bool TopTag(TopJet topjet, double& mjet, int& nsubjets, double& mmin);

#endif
