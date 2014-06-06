#include "include/TopPtReweight.h"

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
    } 
    //std::cout<<"sf from TopPtReweight: "<<scale_factor<<std::endl;
    return scale_factor;
}
