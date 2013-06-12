#include "include/ElectronEffHists.h"
#include "include/SelectionModules.h"
#include <iostream>

using namespace std;

ElectronEffHists::ElectronEffHists(const char* name) : BaseHists(name)
{
  // named default constructor
  
}

ElectronEffHists::~ElectronEffHists()
{
  // default destructor, does nothing
}

void ElectronEffHists::Init()
{
  // book all histograms here
  Book( TH1F( "pT","Trig Efficiency vs p_{T} electron",400,0,2000));
}


void ElectronEffHists::Fill()
{
   // important: get the event weight
  EventCalc* calc = EventCalc::Instance();
  double weight = calc -> GetWeight();

  
  BaseCycleContainer* bcc = calc->GetBaseCycleContainer();
  for(unsigned int i=0; i< bcc->electrons->size(); ++i)
    {
      Electron electron  = bcc->electrons->at(i); 
      Hist("pT")-> Fill(electron.pt(),weight);
    }


}
 

TH1* ElectronEffHists::getHist(const char *name) {
  return Hist(name);
}

void ElectronEffHists::Finish()
{
  // final calculations, like division and addition of certain histograms 
}

