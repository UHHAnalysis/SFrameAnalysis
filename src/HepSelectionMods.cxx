//---sframe new---- 
#include "include/HepSelectionMods.h"


namespace HepSelectionMods{



NAntiMuonHEPTopSelection::NAntiMuonHEPTopSelection(int min_nbtag, int max_nbtag, double ptmin, double etamax )
{
    m_min_nbtag=min_nbtag;
    m_max_nbtag=max_nbtag;
    m_ptmin=ptmin;
    m_etamax=etamax;
}

bool NAntiMuonHEPTopSelection::pass(BaseCycleContainer *bcc)
{
    int nbtag=0;

    //Assumes to have only one muon

    for(unsigned int i=0; i<bcc->topjets->size(); ++i) {
      int jettagged=0;

      if(HepTopTag(bcc->topjets->at(i))) jettagged=1;

      if(bcc->muons->size() != 1){
	std::cout << "ATTENTION!!! muon size " << bcc->muons->size() << std::endl;
      }

      double deltaphi=bcc->topjets->at(i).deltaPhi(bcc->muons->at(0));
      
      if(jettagged&&(deltaphi>(2*PI/3))&&(bcc->topjets->at(i).pt()>m_ptmin)&&(fabs(bcc->topjets->at(i).eta())<m_etamax)){

	nbtag++;

      }

    }

    if(nbtag<m_min_nbtag) return false;
    if(nbtag>m_max_nbtag) return false;
    return true;
}

std::string NAntiMuonHEPTopSelection::description()
{
    char s[100];
    sprintf(s, "%d <= number of hep-top-tagged jets in the muon anti-hemisphere <= %d",m_min_nbtag,m_max_nbtag);
    return s;
}

NAntiMuonHEPTopSelectionMatch::NAntiMuonHEPTopSelectionMatch(int min_nbtag, int max_nbtag, double ptmin, double etamax )
{
    m_min_nbtag=min_nbtag;
    m_max_nbtag=max_nbtag;
    m_ptmin=ptmin;
    m_etamax=etamax;
}


bool NAntiMuonHEPTopSelectionMatch::pass(BaseCycleContainer *bcc)
{
    int nbtag=0;

    //Assumes to have only one muon

    for(unsigned int i=0; i<bcc->topjets->size(); ++i) {
      int jettagged=0;

      if(HepTopTagMatch(bcc->topjets->at(i))) jettagged=1;

      if(bcc->muons->size() != 1){
	std::cout << "ATTENTION!!! muon size " << bcc->muons->size() << std::endl;
      }

      double deltaphi=bcc->topjets->at(i).deltaPhi(bcc->muons->at(0));
      
      if(jettagged&&(deltaphi>(2*PI/3))&&(bcc->topjets->at(i).pt()>m_ptmin)&&(fabs(bcc->topjets->at(i).eta())<m_etamax)){

	nbtag++;

      }

    }

    if(nbtag<m_min_nbtag) return false;
    if(nbtag>m_max_nbtag) return false;
    return true;
}

std::string NAntiMuonHEPTopSelectionMatch::description()
{
    char s[100];
    sprintf(s, "%d <= number of hep-top-tagged jets with matching in the muon anti-hemisphere <= %d",m_min_nbtag,m_max_nbtag);
    return s;
}


NHEPTopTagSelection::NHEPTopTagSelection(int min_nheptoptag, int max_nheptoptag){
  m_min_nheptoptag=min_nheptoptag;
  m_max_nheptoptag=max_nheptoptag;
}

bool NHEPTopTagSelection::pass(BaseCycleContainer *bcc){

  int nheptoptag=0;
  
  double ptcut=150;
  
  for(unsigned int i=0; i< bcc->topjets->size(); ++i){
    if(bcc->topjets->at(i).pt()<ptcut) continue;
    TopJet topjet =  bcc->topjets->at(i);
    double mmin=0;
    double mjet=0;
    int nsubjets=0;
    if(HepTopTagWithMatch(topjet)) nheptoptag++;
  }
  if(nheptoptag<m_min_nheptoptag) return false;
  if(nheptoptag>m_max_nheptoptag) return false;
  return true;

}

std::string NHEPTopTagSelection::description(){
  char s[100];
  sprintf(s, "%d <= N(top-tags) <= %d",m_min_nheptoptag,m_max_nheptoptag);
  return s;
}


}
