#ifndef MuonHists_H
#define MuonHists_H

// Local include(s):
#include "SFrameTools/include/BaseHists.h"

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


