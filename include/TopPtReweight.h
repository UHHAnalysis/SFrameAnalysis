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
    ///Default destructor
    TopPtReweight();
    //Constructor with predefined average weight
    TopPtReweight(double average);
    ~TopPtReweight();

    ///return the weighted correction factor
    double ReturnWeight(double weight);
    double GetScaleWeight();
    double GetScalePlus();
    double GetScaleMinus();

    //return the average of the weights produced
    double GetAverageWeight();

    //enables the smooth turn off
    bool EnableSmoothTurnOff();

    //disables SF above pT=400
    bool EnableTurnOff();

private:
    bool m_SmoothTurnOff;
    bool m_TurnOff;
    bool m_UpdateWeight;
    int m_NumWeights;
    double m_AverageWeight;
    TF1* m_Function;
    double scale();
};

#endif
