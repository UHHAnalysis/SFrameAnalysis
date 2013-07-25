#ifndef TopJetHists_H
#define TopJetHists_H

#include "SFrameTools/include/BaseHists.h"

/**
 *   Class for booking and filling TopJet histograms
 *
 *   
 *   @version $Revision: 1.1 $
 */

class TopJetHists : public BaseHists {

public:
   /// Named constructor
   TopJetHists(const char* name);

   /// Default destructor
   ~TopJetHists();

   void Init();

   void Fill();

   void Finish();

private:

}; // class TopJetHists


#endif // TopJetHists_H


