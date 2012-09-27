#ifndef EventHists_H
#define EventHists_H

// ROOT include(s):
#include <TObject.h>
#include <TString.h>

// Local include(s):
#include "include/BaseHists.h"
#include "include/BaseCycleContainer.h"

/**
 *   Class for booking and filling Event histograms
 *
 *   
 *   @version $Revision: 1.1 $
 */

class EventHists : public BaseHists {

public:
   /// Named constructor
   EventHists(const char* name);

   /// Default destructor
   ~EventHists();

   void Init();

   void Fill();

   void Finish();

private:

}; // class EventHists


#endif // EventHists_H


