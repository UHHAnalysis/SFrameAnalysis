#ifndef HypothesisHists_H
#define HypothesisHists_H

// ROOT include(s):
#include <TObject.h>
#include <TString.h>

// Local include(s):
#include "include/BaseHists.h"
#include "include/BaseCycleContainer.h"
#include "HypothesisDiscriminator.h"
#include "EventCalc.h"
#include <iostream>
#include "TH2.h"

/**
 * @short histogram class to draw histograms for the ttbar reconstruction
 *
 * histograms are plotted for the best hypothesis determined by the HypothesisDiscriminator given to the constructor
 */

class HypothesisHists : public BaseHists {

public:
   /// Named constructor
  HypothesisHists(const char* name, HypothesisDiscriminator* discr);

   /// Default destructor
   ~HypothesisHists();

   void Init();

   void Fill();

   void Finish();

private:

   HypothesisDiscriminator* m_discr;

}; // class HypothesisHists


#endif // HypothesisHists_H
