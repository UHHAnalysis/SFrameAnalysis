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

  bool TopTagging(int);

 private:
  BaseCycleContainer* bcc;


  struct HigherPt {
    bool operator() (const Particle& j1, const Particle& j2) const {
      return j1.pt > j2.pt;
    };
  };


};


#endif
