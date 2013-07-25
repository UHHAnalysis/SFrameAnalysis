#ifndef BTagEffHists_H
#define BTagEffHists_H

// Local include(s):
#include "SFrameTools/include/BaseHists.h"
#include "SFrameTools/include/Utils.h"

/**
 *   Class for booking and filling btag histograms
 *
 *   
 *   @version $Revision: 1.1 $
 */

class BTagEffHists : public BaseHists {

public:
   /// Named constructor
   BTagEffHists(const char*, E_BtagType);

   /// Default destructor
   ~BTagEffHists();

   void Init();

   void Fill();

   void Finish();

private:

  E_BtagType m_type;

}; // class BTagEffHists

#endif // BTagEffHists_H

