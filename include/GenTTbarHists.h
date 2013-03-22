#ifndef GenTTbarHists_H
#define GenTTbarHists_H

// ROOT include(s):
#include <TObject.h>
#include <TString.h>
#include "TH2.h"

// Local include(s):
#include "include/BaseHists.h"
#include "include/BaseCycleContainer.h"

class GenTTbarHists : public BaseHists {

public:
   /// Named constructor
   GenTTbarHists(const char* name);

   /// Default destructor
   ~GenTTbarHists();

   void Init();

   void Fill();

   void Finish();

private:

}; // class GenTTbarHists


#endif // GenTTbarHists_H
