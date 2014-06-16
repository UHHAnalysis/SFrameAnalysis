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
    TopPtReweight();
    ///Default destructor
    ~TopPtReweight();

    ///return the weighted correction factor
    double GetScaleWeight();

    //return the average of the weights produced
    double GetAverageWeight();

private:
    int m_NumWeights;
    double m_AverageWeight;
};

#endif
