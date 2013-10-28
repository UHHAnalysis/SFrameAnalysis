#ifndef TopPtReweight_H
#define TopPtReweight_H

#include "SFrameTools/include/Utils.h"
#include "SFrameTools/include/EventCalc.h"

/**
 *  @module to apply data-MC scale factors for b tagging
 *
 *
 */
class TopPtReweight {
public:
    /**
     * constructor
     *
     */
    TopPtReweight() {};
    ///Default destructor
    ~TopPtReweight() {};

    ///return the weighted correction factor
    double GetScaleWeight();

private:
                 
};

#endif
