#include "include/TopPtReweight.h"

TopPtReweight::TopPtReweight() {
    m_AverageWeight = 0.0;
    m_NumWeights = 0;
    m_UpdateWeight = true;
}

TopPtReweight::TopPtReweight(double average) {
    m_AverageWeight = average;
    m_UpdateWeight = false;
}

TopPtReweight::~TopPtReweight() {
    std::cout << "TopPtReweight Destructor()" << std::endl;
    std::cout << " TopPtReweight :: Average Weight = " << m_AverageWeight << std::endl;
}

double TopPtReweight::GetScaleWeight()
{
    EventCalc* calc = EventCalc::Instance();

    double scale_factor = 1.;
    if(!calc->IsRealData())
    {
      TTbarGen* Decay = calc->GetTTbarGen();

      double toppt1 = Decay->Top().pt();
      double toppt2 = Decay->Antitop().pt();

      if(toppt1 > 400) toppt1 = 400.;
      if(toppt2 > 400) toppt2 = 400.;

      double wtop1 = exp(0.156-0.00137*toppt1);
      double wtop2 = exp(0.156-0.00137*toppt2);

      scale_factor = sqrt(wtop1*wtop2);
      if(m_UpdateWeight) {
        m_AverageWeight = ((m_AverageWeight * m_NumWeights) + scale_factor) / (m_NumWeights + 1);
        m_NumWeights += 1;
      }
    }
    //std::cout<<"sf from TopPtReweight: "<<scale_factor<<std::endl;
    return scale_factor;
}

double TopPtReweight::GetScalePlus()
{
    EventCalc* calc = EventCalc::Instance();

    double scale_factor = 1.;
    if(!calc->IsRealData())
    {
      TTbarGen* Decay = calc->GetTTbarGen();

      double toppt1 = Decay->Top().pt();
      double toppt2 = Decay->Antitop().pt();

      if(toppt1 > 400) toppt1 = 400.;
      if(toppt2 > 400) toppt2 = 400.;

      double wtop1 = exp(0.156-0.00137*toppt1);
      double wtop2 = exp(0.156-0.00137*toppt2);

      scale_factor = wtop1*wtop2;
      if(m_UpdateWeight) {
        m_AverageWeight = ((m_AverageWeight * m_NumWeights) + scale_factor) / (m_NumWeights + 1);
        m_NumWeights += 1;
      }
    }
    //std::cout<<"sf from TopPtReweight: "<<scale_factor<<std::endl;
    return scale_factor;
}

double TopPtReweight::GetScaleMinus()
{
    EventCalc* calc = EventCalc::Instance();

    double scale_factor = 1.;
    if(!calc->IsRealData() && m_UpdateWeight)
    {
      m_AverageWeight = ((m_AverageWeight * m_NumWeights) + scale_factor) / (m_NumWeights + 1);
      m_NumWeights += 1;
    }
    return scale_factor;
}


double TopPtReweight::GetAverageWeight()
{
    return m_AverageWeight;
}

