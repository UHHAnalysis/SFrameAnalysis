#include "include/BTagSelectionMods.h"

NBTagAntiktJetSelection::NBTagAntiktJetSelection(int min_nbtag, int max_nbtag=int_infinity(), E_BtagType type, double delR_Jet_TopTag){
  m_min_nbtag=min_nbtag; 
  m_max_nbtag=max_nbtag;  
  m_type=type;
  m_delR_Jet_TopTag=delR_Jet_TopTag;
}

bool NBTagAntiktJetSelection::pass(BaseCycleContainer *bcc){

  int nbtag=0;
    for(unsigned int m = 0; m< bcc->topjets->size();++m){
      TopJet topjet =  bcc->topjets->at(m);
	
      for(unsigned int i=0; i<bcc->jets->size(); ++i) {
 	if(m_type==e_CSVL && bcc->jets->at(i).btag_combinedSecondaryVertex()>0.244 && topjet.deltaR(bcc->jets->at(i))>m_delR_Jet_TopTag) nbtag++;
        if(m_type==e_CSVM && bcc->jets->at(i).btag_combinedSecondaryVertex()>0.679 && topjet.deltaR(bcc->jets->at(i))>m_delR_Jet_TopTag) nbtag++;
        if(m_type==e_CSVT && bcc->jets->at(i).btag_combinedSecondaryVertex()>0.898 && topjet.deltaR(bcc->jets->at(i))>m_delR_Jet_TopTag) nbtag++;
    } 
    }

  if(nbtag<m_min_nbtag) return false;
  if(nbtag>m_max_nbtag) return false;
  return true;
}

std::string NBTagAntiktJetSelection::description()
{
    char s[100];
       sprintf(s, "%d <= number of b-tags <= %d, with  deltaR >= %.1f",m_min_nbtag,m_max_nbtag,m_delR_Jet_TopTag);
    return s;
}  


NSumBTagsSelection::NSumBTagsSelection(int min_nsumbtags, int max_nsumbtags=int_infinity(), E_BtagType type){
  m_min_nsumbtags=min_nsumbtags;
  m_max_nsumbtags=max_nsumbtags;
  m_type=type;
}

bool NSumBTagsSelection::pass(BaseCycleContainer *bcc)
{
  EventCalc* calc = EventCalc::Instance();
  double nsumbtags = calc->GetNSumBTags();
  if(nsumbtags  < m_min_nsumbtags) return false;
  if(nsumbtags  > m_max_nsumbtags) return false;
  return true;
}
std::string NSumBTagsSelection::description(){
char s[100];
sprintf(s, "%d <= N(Sum B-Tags) <= %d", m_min_nsumbtags, m_max_nsumbtags);
return s;
}

NCMSSubBTagSelection::NCMSSubBTagSelection(int min_ntoptag, int max_ntoptag=int_infinity(), int min_nbtag, int max_nbtag, E_BtagType type, double nsubjettiness): toptagger(50., 140., 250., nsubjettiness){
  m_min_ntoptag=min_ntoptag;
  m_max_ntoptag=max_ntoptag;
  m_min_nsubjetbtag = min_nbtag;
  m_max_nsubjetbtag = max_nbtag;
  m_type = type;
}

bool NCMSSubBTagSelection::pass(BaseCycleContainer *bcc){

  int nsubjetbtag=0; 
  double mmin=0;
  double mjet=0;
  int nsubjets=0;
  for(unsigned int i=0; i< bcc->topjets->size(); ++i){
    const TopJet & topjet =  bcc->topjets->at(i);
    if(!toptagger.Tag(topjet)) continue; // note: nsubjettiness cut part of toptagger
    if(subJetBTag(topjet, m_type)>=1) nsubjetbtag++;  
  }
  if(nsubjetbtag<m_min_nsubjetbtag) return false;
  if(nsubjetbtag>m_max_nsubjetbtag) return false;
  return true;
}

std::string NCMSSubBTagSelection::description(){
  char s[100];
  sprintf(s, "%d <= N(top-tags) <= %d, with %d <= N(subjetBtags) <= %d", m_min_ntoptag, m_max_ntoptag, m_min_nsubjetbtag, m_max_nsubjetbtag);
  return s;
}


NBTagSelection::NBTagSelection(int min_nbtag, int max_nbtag, E_BtagType type)
{
    m_min_nbtag=min_nbtag;
    m_max_nbtag=max_nbtag;
    m_type=type;
}

bool NBTagSelection::pass(BaseCycleContainer *bcc)
{
    int nbtag=0;
    for(unsigned int i=0; i<bcc->jets->size(); ++i) {
        if(m_type==e_CSVL && bcc->jets->at(i).btag_combinedSecondaryVertex()>0.244) nbtag++;
        if(m_type==e_CSVM && bcc->jets->at(i).btag_combinedSecondaryVertex()>0.679) nbtag++;
        if(m_type==e_CSVT && bcc->jets->at(i).btag_combinedSecondaryVertex()>0.898) nbtag++;
        if(m_type==e_JPL && bcc->jets->at(i).btag_jetProbability()>0.275) nbtag++;
        if(m_type==e_JPM && bcc->jets->at(i).btag_jetProbability()>0.545) nbtag++;
        if(m_type==e_JPT && bcc->jets->at(i).btag_jetProbability()>0.790) nbtag++;
    }

    if(nbtag<m_min_nbtag) return false;
    if(nbtag>m_max_nbtag) return false;
    return true;
}

std::string NBTagSelection::description()
{
    char s[100];
    sprintf(s, "%d <= number of b-tags <= %d",m_min_nbtag,m_max_nbtag);
    return s;
}

