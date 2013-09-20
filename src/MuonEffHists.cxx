#include "include/MuonEffHists.h"
#include "include/SelectionModules.h"
#include <iostream>

using namespace std;

MuonEffHists::MuonEffHists(const char* name) : BaseHists(name)
{
  // named default constructor
  
}

MuonEffHists::~MuonEffHists()
{
  // default destructor, does nothing
}

void MuonEffHists::Init()
{
  // book all histograms here
  Book( TH1F( "pT","Trig Efficiency vs p_{T} muon",400,0,2000));
}


void MuonEffHists::Fill()
{
   // important: get the event weight
  EventCalc* calc = EventCalc::Instance();
  double weight = calc -> GetWeight();

  BaseCycleContainer* bcc = calc->GetBaseCycleContainer();
  for(unsigned int i=0; i< bcc->muons->size(); ++i)
    {
      Muon muon  = bcc->muons->at(i); 
      Hist("pT")-> Fill(muon.pt(),weight);
    }


}
 