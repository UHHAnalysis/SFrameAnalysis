#include "include/TopPtReweight.h"

TopPtReweight::TopPtReweight() {
    m_AverageWeight = 0.0;
    m_NumWeights = 0;
    m_UpdateWeight = true;
    m_SmoothTurnOff = false;
    m_Function = new TF1("topptreweight","expo(0)");
    m_Function->SetParameter(0,0.156);
    m_Function->SetParameter(1,-0.00137);
}

TopPtReweight::TopPtReweight(double average) {
    TopPtReweight();
    m_AverageWeight = average;
    m_NumWeights = 1;
    m_UpdateWeight = false;
}

TopPtReweight::~TopPtReweight() {
    std::cout << "TopPtReweight Destructor()" << std::endl;
    std::cout << " TopPtReweight :: Average Weight = " << m_AverageWeight << std::endl;
    std::cout << " TopPtReweight :: NumWeights = " << m_NumWeights << std::endl;
    delete m_Function;
}

bool TopPtReweight::EnableSmoothTurnOff()
{
    if(m_Function) delete m_Function;
    m_Function = new TF1("topptreweight_smooth","(0.5+0.5*TMath::Erf((x-[0])/([1]*TMath::Sqrt(2))))+expo(2)*(0.5-0.5*TMath::Erf((x-[0])/([1]*TMath::Sqrt(2))))");
    m_Function->SetParameter(0,400);
    m_Function->SetParameter(1,25);
    m_Function->SetParameter(2,0.156);
    m_Function->SetParameter(3,-0.00137);
    m_SmoothTurnOff = true;
    return (m_Function != NULL);
}

bool TopPtReweight::EnableTurnOff()
{
    m_TurnOff = true;
    return m_TurnOff;
}

double TopPtReweight::ReturnWeight(double scale_factor)
{
    if(m_UpdateWeight) {
        m_AverageWeight = ((m_AverageWeight * m_NumWeights) + scale_factor) / (m_NumWeights + 1);
        m_NumWeights += 1;
    }
    return scale_factor;
}

double TopPtReweight::GetAverageWeight()
{
    return m_AverageWeight;
}

double TopPtReweight::GetScaleWeight() { return ReturnWeight(scale()); }

double TopPtReweight::GetScalePlus()
{
    double retvalue = scale();
    return ReturnWeight(retvalue*retvalue);
}

double TopPtReweight::GetScaleMinus() { return ReturnWeight(1.0); }

double TopPtReweight::scale()
{
    EventCalc* calc = EventCalc::Instance();

    double scale_factor = 1.;
    if(!calc->IsRealData())
    {
      TTbarGen* Decay = calc->GetTTbarGen();

      double toppt1 = Decay->Top().pt();
      double toppt2 = Decay->Antitop().pt();

      double wtop1 = m_Function->Eval(toppt1);
      double wtop2 = m_Function->Eval(toppt2);
      if(toppt1 > 400) {
          if(m_TurnOff)
              wtop1 = 1.0;
          else if(!m_SmoothTurnOff)
              wtop1 = m_Function->Eval(400.0);
      }
      if(toppt2 > 400) {
          if(m_TurnOff)
              wtop2 = 1.0;
          else if(!m_SmoothTurnOff)
              wtop2 = m_Function->Eval(400.0);
      }

      scale_factor = sqrt(wtop1*wtop2);
    }
    //std::cout<<"sf from TopPtReweight: "<<scale_factor<<std::endl;
    return scale_factor;
}


