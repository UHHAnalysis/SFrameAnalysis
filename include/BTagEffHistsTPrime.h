#ifndef BTagEffHistsTPrime_H
#define BTagEffHistsTPrime_H

#include "SFrameTools/include/BaseHists.h"
#include "SFrameTools/include/Utils.h"

/**
 *   Class for booking and filling btag histograms
 *
 *   
 *   @version $Revision: 1.1 $
 */

class BTagEffHistsTPrime : public BaseHists {

public:
   /// Named constructor
   BTagEffHistsTPrime(const char*);

   /// Default destructor
   ~BTagEffHistsTPrime();

   void Init();

   void Fill();

   void Finish();

private:

  E_BtagType m_type;

}; // class BTagEffHistsTPrime

#endif // BTagEffHistsTPrime_H
