#ifndef MuonHists_H
#define MuonHists_H

// ROOT include(s):
#include <TObject.h>
#include <TString.h>
#include "TH2.h"

// Local include(s):
#include "include/BaseHists.h"
#include "include/BaseCycleContainer.h"

/**
 *   Class for booking and filling muon histograms
 *
 *   
 *   @version $Revision: 1.1 $
 */

class MuonHists : public BaseHists {

public:
   /// Named constructor
   MuonHists(const char* name);

   /// Default destructor
   ~MuonHists();

   void Init();

   void Fill();

   void Finish();

private:

}; // class MuonHists


#endif // MuonHists_H


