#ifndef TauHists_H
#define TauHists_H

// ROOT include(s):
#include <TObject.h>
#include <TString.h>

// Local include(s):
#include "include/BaseHists.h"
#include "include/BaseCycleContainer.h"

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

   void Finish();

private:

}; // class TauHists


#endif // TauHists_H


