#ifndef ElectronEffHists_H
#define ElectronEffHists_H

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

class ElectronEffHists : public BaseHists {

public:
   /// Named constructor
   ElectronEffHists(const char* name);

   /// Default destructor
   ~ElectronEffHists();

   void Init();

   void Fill();

   TH1* getHist(const char* name);

   void Finish();

private:

}; // class ElectronEffHists


#endif // ElectronEffHists_H


