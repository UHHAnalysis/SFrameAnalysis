#include "include/CMSTopTagSelectionMods.h"
#include "SFrameTools/include/SubJetTagger.h"

CMSTopTagOverlapSelection::CMSTopTagOverlapSelection(double delR_Lep_TopTag, double delR_Jet_TopTag, double tau32Cut)
{
  m_delR_Lep_TopTag=delR_Lep_TopTag;
  m_delR_Jet_TopTag=delR_Jet_TopTag;
  m_tau32Cut = tau32Cut;
}


bool CMSTopTagOverlapSelection::pass(BaseCycleContainer* bcc)
{

   //find primary charged lepton
  EventCalc* calc = EventCalc::Instance();
  Particle* lepton = calc->GetPrimaryLepton();

  CMSTopTagger toptag;
  toptag.SetTau32Cut(m_tau32Cut);
  
  for(unsigned int i = 0; i< bcc->topjets->size();++i){
    TopJet topjet = bcc->topjets->at(i);
    if(toptag.Tag(topjet) &&  m_delR_Lep_TopTag < topjet.deltaR(*lepton)){
      for(unsigned int m =0; m< bcc->jets->size(); ++m){
	if(topjet.deltaR(bcc->jets->at(m))>m_delR_Jet_TopTag)return true;
      }
    }
  }

  return false;

}

std::string CMSTopTagOverlapSelection::description()
{
    char s[100];
    sprintf(s, "delR(Lepton,TopTag) > %.1f  and at least one ak5-jet with delR(Jet,TopTag) > %.1f ",m_delR_Lep_TopTag,m_delR_Jet_TopTag);

    return s;
}

CMSTopTagAntiktJetSelection::CMSTopTagAntiktJetSelection(unsigned int min_TopTag, unsigned  int min_Jets, double min_distance, unsigned int max_TopTag, unsigned int max_Jets, double tau32Cut){
  m_min_TopTag=min_TopTag; 
  m_max_TopTag=max_TopTag;
  m_min_Jets=min_Jets;
  m_max_Jets=max_Jets;
  m_min_distance=min_distance;  
  m_tau32Cut = tau32Cut;
}

bool CMSTopTagAntiktJetSelection::pass(BaseCycleContainer *bcc){

  if(bcc->jets->size() < m_min_Jets || bcc->jets->size()> m_max_Jets ) return false;
  unsigned int nTopTags = 0;

  double deltaR_Lep_Tophad = -1;
  TopJet top_had;
  Particle lepton;
  lepton.set_v4(bcc->muons->at(0).v4());

  CMSTopTagger toptag;
  toptag.SetTau32Cut(m_tau32Cut);

  for(unsigned int i = 0; i< bcc->topjets->size();++i){
    TopJet topjet = bcc->topjets->at(i);
    if(toptag.Tag(topjet) && deltaR_Lep_Tophad < topjet.deltaR(lepton)){
      top_had = topjet;
      deltaR_Lep_Tophad = topjet.deltaR(lepton);
    }
    if(toptag.Tag(topjet)) nTopTags++;
  }
   
  if(nTopTags < m_min_TopTag || nTopTags > m_max_TopTag) return false;

  
  for(unsigned int p=0; p< bcc->jets->size(); ++p){
    Jet antiktjet = bcc->jets->at(p);
    if(top_had.deltaR(antiktjet)> m_min_distance) return true; 
  }

  return false;
}

std::string CMSTopTagAntiktJetSelection::description(){
  char s[100];
  sprintf(s, "%d <= N(TopTags) <= %d , %d <= N(Jets)<= %d ,with minimal delR <%.1f",m_min_TopTag,m_max_TopTag,m_min_Jets,m_max_Jets,m_min_distance);
  return s;
}


NCMSTopTagSelection::NCMSTopTagSelection(int min_ntoptag, int max_ntoptag, double tau32Cut)
{
    m_min_ntoptag=min_ntoptag;
    m_max_ntoptag=max_ntoptag;
    m_tau32Cut = tau32Cut;
}

bool NCMSTopTagSelection::pass(BaseCycleContainer *bcc)
{

    int ntoptag=0;

    CMSTopTagger toptag;
    toptag.SetTau32Cut(m_tau32Cut);

    for(unsigned int i=0; i< bcc->topjets->size(); ++i) {
        TopJet & topjet =  bcc->topjets->at(i);
        if(toptag.Tag(topjet)) ntoptag++;
	
    }
    if(ntoptag<m_min_ntoptag) return false;
    if(ntoptag>m_max_ntoptag) return false;
    return true;

}

std::string NCMSTopTagSelection::description()
{
    char s[100];
    sprintf(s, "%d <= N(top-tags) <= %d",m_min_ntoptag,m_max_ntoptag);
    return s;
}
