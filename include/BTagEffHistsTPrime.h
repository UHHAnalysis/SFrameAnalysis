#ifndef BTagEffHistsTPrime_H
#define BTagEffHistsTPrime_H

// ROOT include(s):
#include <TObject.h>
#include <TString.h>

// Local include(s):
#include "SFrameTools/include/BaseHists.h"

/**
 *   Class for booking and filling btag histograms
 *
 *   
 *   @version $Revision: 1.1 $
 */

class BTagEffHistsTPrime : public BaseHists {

public:
   /// Named constructor
  BTagEffHistsTPrime(const char* name);

   /// Default destructor
   ~BTagEffHistsTPrime();

   void Init();

   void Fill();

   void Finish();

private:

}; // class BTagEffHistsTPrime

#endif // BTagEffHistsTPrime_H
