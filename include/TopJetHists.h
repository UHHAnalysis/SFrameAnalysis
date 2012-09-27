#ifndef TopJetHists_H
#define TopJetHists_H

// ROOT include(s):
#include <TObject.h>
#include <TString.h>

// Local include(s):
#include "include/BaseHists.h"
#include "include/BaseCycleContainer.h"

/**
 *   Class for booking and filling TopJet histograms
 *
 *   
 *   @version $Revision: 1.1 $
 */

class TopJetHists : public BaseHists {

public:
   /// Named constructor
   TopJetHists(const char* name);

   /// Default destructor
   ~TopJetHists();

   void Init();

   void Fill();

   void Finish();

private:

}; // class TopJetHists


#endif // TopJetHists_H


