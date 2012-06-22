#ifndef JetLeptonCleanerHists_H
#define JetLeptonCleanerHists_H

// ROOT include(s):
#include <TObject.h>
#include <TString.h>

// Local include(s):
#include "include/BaseHists.h"
#include "include/BaseCycleContainer.h"
#include "include/Cleaner.h"
#include "FactorizedJetCorrector.h"
#include "JetCorrectorParameters.h"
#include "TH2.h"

/**
 *   @short histogram class for the JetLeptonCleanerCycle
 */

class JetLeptonCleanerHists : public BaseHists {

public:
   /// Named constructor
   JetLeptonCleanerHists(const char* name);

   /// Default destructor
   ~JetLeptonCleanerHists();

   void Init();

   void Fill();

   void Finish();

private:

   FactorizedJetCorrector* m_corrector;


}; // class JetLeptonCleanerHists


#endif // JetLeptonCleanerHists_H
