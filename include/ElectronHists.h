#ifndef ElectronHists_H
#define ElectronHists_H

// ROOT include(s):
#include <TObject.h>
#include <TString.h>

// Local include(s):
#include "include/BaseHists.h"
#include "include/BaseCycleContainer.h"

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

   void Fill();

   void Finish();

private:

}; // class ElectronHists


#endif // ElectronHists_H


