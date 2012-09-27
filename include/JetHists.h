#ifndef JetHists_H
#define JetHists_H

// ROOT include(s):
#include <TObject.h>
#include <TString.h>

// Local include(s):
#include "include/BaseHists.h"
#include "include/BaseCycleContainer.h"

/**
 *   Class for booking and filling jet histograms
 *
 *   
 *   @version $Revision: 1.1 $
 */

class JetHists : public BaseHists {

public:
   /// Named constructor
   JetHists(const char* name);

   /// Default destructor
   ~JetHists();

   void Init();

   void Fill();

   void Finish();

private:

}; // class JetHists


#endif // JetHists_H


