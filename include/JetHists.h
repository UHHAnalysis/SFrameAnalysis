#ifndef JetHists_H
#define JetHists_H

// Local include(s):
#include "SFrameTools/include/BaseHists.h"

/**
 *   Class for booking and filling jet histograms
 *
 *   
 *   @version $Revision: 1.1 $
 */

class JetHists : public BaseHists {

public:
   /// Named constructor
   JetHists(const char* name);

   /// Default destructor
   ~JetHists();

   void Init();

   void Fill();
}; // class JetHists


#endif // JetHists_H


