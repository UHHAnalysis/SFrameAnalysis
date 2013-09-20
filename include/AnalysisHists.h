#ifndef AnalysisHists_H
#define AnalysisHists_H

// SFrame include(s):
#include "SFrameTools/include/BaseHists.h"
#include "SFrameTools/include/HypothesisDiscriminator.h"

#include <iostream>

/**
 * @short histogram class to draw histograms for the ttbar reconstruction
 *
 * histograms are plotted for the best hypothesis determined by the HypothesisDiscriminator given to the constructor
 */

class AnalysisHists : public BaseHists {

public:
   /// Named constructor
  AnalysisHists(const char* name, HypothesisDiscriminator* discr);

   /// Default destructor
   ~AnalysisHists();

   void Init();

   void Fill();


private:

   HypothesisDiscriminator* m_discr;

}; // class AnalysisHists


#endif // AnalysisHists_H
