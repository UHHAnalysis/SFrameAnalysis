#ifndef ElectronHists_H
#define ElectronHists_H

// Local include(s):
#include "SFrameTools/include/BaseHists.h"

/**
 *   Class for booking and filling electron histograms
 *
 *
 *   @version $Revision: 1.1 $
 */

class ElectronHists : public BaseHists {

public:
   /// Named constructor
   ElectronHists(const char* name);

   /// Default destructor
   ~ElectronHists();

   void Init();

   void Scale(double scale);

   void Fill();

}; // class ElectronHists


#endif // ElectronHists_H


