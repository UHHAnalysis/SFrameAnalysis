#include "include/TopPtReweight.h"

double TopPtReweight::GetScaleWeight()
{
    EventCalc* calc = EventCalc::Instance();
    BaseCycleContainer* bcc = calc->GetBaseCycleContainer();
    bool IsRealData = calc->IsRealData();
    double scale_factor = 1.;
    if(!IsRealData)
    {
      TTbarGen* Decay = calc->GetTTbarGen();
      GenParticle topgen=Decay->Top();
      GenParticle antitopgen=Decay->Antitop();
      double wtop=exp(0.156-0.00137*topgen.pt());
      double wantitop=exp(0.156-0.00137*antitopgen.pt());
      double SF = sqrt(wtop*wantitop);
      scale_factor *= SF;
    } 
    //std::cout<<"sf from TopPtReweight: "<<scale_factor<<std::endl;
    return scale_factor;
}
