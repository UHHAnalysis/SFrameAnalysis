#ifndef AnalysisHists_H
#define AnalysisHists_H

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

class AnalysisHists : public BaseHists {

public:
   /// Named constructor
  AnalysisHists(const char* name, HypothesisDiscriminator* discr);

   /// Default destructor
   ~AnalysisHists();

   void Init();

   void Fill();

   void Finish();

private:

   HypothesisDiscriminator* m_discr;

}; // class AnalysisHists


#endif // AnalysisHists_H
