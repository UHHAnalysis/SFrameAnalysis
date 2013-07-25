#ifndef ElectronEffHists_H
#define ElectronEffHists_H

// Local include(s):
#include "SFrameTools/include/BaseHists.h"

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


