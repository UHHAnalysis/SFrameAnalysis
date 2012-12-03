#ifndef MuonEffHists_H
#define MuonEffHists_H

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

class MuonEffHists : public BaseHists {

public:
   /// Named constructor
   MuonEffHists(const char* name);

   /// Default destructor
   ~MuonEffHists();

   void Init();

   void Fill();

   TH1* getHist(const char* name);

   void Finish();

private:

}; // class MuonEffHists


#endif // MuonEffHists_H


