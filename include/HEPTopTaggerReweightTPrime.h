#ifndef HEPTopTaggerReweightTPrime_H
#define HEPTopTaggerReweightTPrime_H

#include "SFrameTools/include/Utils.h"
#include "SFrameTools/include/EventCalc.h"
/**
 *  @module to apply data-MC scale factors for top tagging
 *
 *
 */
class HEPTopTaggerReweightTPrime {
public:
    /**
     * constructor
     *
     */
    HEPTopTaggerReweightTPrime() {};
    ///Default destructor
    ~HEPTopTaggerReweightTPrime() {};

    ///return the weighted correction factor
    double GetScaleWeight(int scale=0);

private:
                 
};

#endif
