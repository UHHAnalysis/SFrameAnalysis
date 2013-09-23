#ifndef HypothesisHists_H
#define HypothesisHists_H

// Local include(s):
#include "SFrameTools/include/BaseHists.h"
#include "SFrameTools/include/HypothesisDiscriminator.h"


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

private:

   HypothesisDiscriminator* m_discr;

}; // class HypothesisHists


#endif // HypothesisHists_H
