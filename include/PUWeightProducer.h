#ifndef PUWeightProducer_H
#define PUWeightProducer_H

#include "Objects.h"
#include "BaseCycleContainer.h"
#include <algorithm>
#include "TH1.h"
#include "TFile.h"

class PUWeightProducer{

 public:
  PUWeightProducer(){};

  PUWeightProducer(TString filename_mc, TString filename_data, TString histname_mc, TString histname_data);

  ~PUWeightProducer(){};

  double produceWeight(GenInfo* genInfo);
  

 private:

  TH1F* h_npu_mc;
  TH1F* h_npu_data;

};


#endif
