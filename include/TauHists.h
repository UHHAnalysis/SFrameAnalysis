#ifndef TauHists_H
#define TauHists_H

#include "SFrameTools/include/BaseHists.h"

/**
 *   Class for booking and filling Top Tagging efficiency histograms
 *
 *   
 *   @version $Revision: 1.1 $
 */

class TauHists : public BaseHists {

public:
   /// Named constructor
   TauHists(const char* name);

   /// Default destructor
   ~TauHists();

   void Init();
   
   void Fill();

}; // class TauHists


#endif // TauHists_H


