#ifndef EventHists_H
#define EventHists_H

// Local include(s):
#include "SFrameTools/include/BaseHists.h"

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

}; // class EventHists


#endif // EventHists_H


