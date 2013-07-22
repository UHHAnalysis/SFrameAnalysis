#include "include/SelectionModules.h"
#include "TLorentzVector.h"

TriggerSelection::TriggerSelection(std::string triggername)
{
  m_name=triggername;
}

bool TriggerSelection::pass(BaseCycleContainer *bcc)
{
    for(unsigned int i=0; i<bcc->triggerNames_actualrun.size(); ++i) {
        if(!bcc->triggerNames_actualrun.at(i).find(m_name)) {
            return bcc->triggerResults->at(i);
        }
    }
    std::cout<< "WARNING: trigger not found: " << m_name <<"; reject this event." << std::endl;
    for(unsigned int i=0; i<bcc->triggerNames_actualrun.size(); ++i) {
        std::cout << "   "  << bcc->triggerNames_actualrun.at(i) << std::endl;
    }
    return false;
}

std::string TriggerSelection::description()
{
    return "Trigger: "+m_name;
}



TopTagOverlapSelection::TopTagOverlapSelection(double delR_Lep_TopTag, double delR_Jet_TopTag)
{
  m_delR_Lep_TopTag=delR_Lep_TopTag;
  m_delR_Jet_TopTag=delR_Jet_TopTag;
}


bool TopTagOverlapSelection::pass(BaseCycleContainer* bcc)
{

  double mjet = 0;
  int nsubjets = 0;
  double mmin = 0;

  for(unsigned int i = 0; i< bcc->topjets->size();++i){
    TopJet topjet = bcc->topjets->at(i);
    if(TopTag(topjet,mjet,nsubjets,mmin) &&  m_delR_Lep_TopTag < topjet.deltaR(bcc->muons->at(0))){
      for(unsigned int m =0; m< bcc->jets->size(); ++m){
	if(topjet.deltaR(bcc->jets->at(m))>m_delR_Jet_TopTag)return true;
      }
    }
  }

  return false;

}

std::string TopTagOverlapSelection::description()
{
    char s[100];
    sprintf(s, "delR(Muon,TopTag)> %.1f  &&at least one Jet with  delR(Jet,TopTag) > %.1f ",m_delR_Lep_TopTag,m_delR_Jet_TopTag);

    return s;
}



RazorSelection::RazorSelection(HypothesisDiscriminator *discr, double mrazor, double mrazorT)
{
  m_mrazor=mrazor;
  m_mrazorT=mrazorT;
  m_discr=discr;
 
}

bool RazorSelection::pass(BaseCycleContainer* bcc)
{
  ReconstructionHypothesis* hyp = m_discr->GetBestHypothesis();
 
  LorentzVector toplep;
  LorentzVector tophad; 
  TVector3 phadT;
  TVector3 pwlepT; 
  Particle wlep;

  wlep.set_v4(hyp->wlep_v4());

  toplep = hyp->toplep_v4();
  tophad = hyp->tophad_v4();

  phadT.SetPtEtaPhi(tophad.pt(),0,tophad.phi());
  pwlepT.SetPtEtaPhi(wlep.pt(),0,wlep.phi());



  TVector3 met3Vec; 
  met3Vec.SetPtEtaPhi(bcc->met->pt(),0,bcc->met->phi());

  double mrT = sqrt(bcc->met->pt()*(wlep.pt()+tophad.pt())- met3Vec*(pwlepT+phadT));
  double mr = sqrt(pow(wlep.v4().E()+tophad.E(),2)-pow(wlep.v4().Pz()+tophad.Pz(),2));

  return mr > m_mrazor || mrT/mr >  m_mrazorT;

}


std::string RazorSelection::description()
{
    char s[100];
    sprintf(s, " mrazor <= %.1f  && mrazorT/mrazor <= %.1f ",m_mrazor,m_mrazorT);

    return s;
}


NMuonSelection::NMuonSelection(int min_nparticle, int max_nparticle, double ptmin, double etamax)
{
    m_min_nparticle=min_nparticle;
    m_max_nparticle=max_nparticle;
    m_ptmin=ptmin;
    m_etamax=etamax;
}

bool NMuonSelection::pass(BaseCycleContainer *bcc)
{
    int nparticle=0;
    for(unsigned int i=0; i<bcc->muons->size(); ++i) {
        if(bcc->muons->at(i).pt()>m_ptmin && fabs(bcc->muons->at(i).eta())<m_etamax) nparticle++;
    }
    return nparticle>=m_min_nparticle && nparticle<=m_max_nparticle;
}

std::string NMuonSelection::description()
{
    char s[100];
    sprintf(s, "%d <= number of muons <= %d, with pt>%.1f GeV, abs(eta)<%.1f",m_min_nparticle,m_max_nparticle,m_ptmin,m_etamax);

    return s;
}

NElectronSelection::NElectronSelection(int min_nparticle, int max_nparticle, double ptmin, double etamax)
{
    m_min_nparticle=min_nparticle;
    m_max_nparticle=max_nparticle;
    m_ptmin=ptmin;
    m_etamax=etamax;
}


bool NElectronSelection::pass(BaseCycleContainer *bcc)
{
    int nparticle=0;
    for(unsigned int i=0; i<bcc->electrons->size(); ++i) {
        if(bcc->electrons->at(i).pt()>m_ptmin && fabs(bcc->electrons->at(i).eta())<m_etamax) nparticle++; 
    }
    return nparticle>=m_min_nparticle && nparticle<=m_max_nparticle;
}


std::string NElectronSelection::description()
{
    char s[100];
    sprintf(s, "%d <= number of electrons <= %d, with pt>%.1f GeV, abs(eta)<%.1f",m_min_nparticle,m_max_nparticle,m_ptmin,m_etamax);

    return s;
}


NTauSelection::NTauSelection(int min_nparticle, int max_nparticle, double ptmin, double etamax)
{
    m_min_nparticle=min_nparticle;
    m_max_nparticle=max_nparticle;
    m_ptmin=ptmin;
    m_etamax=etamax;
}

bool NTauSelection::pass(BaseCycleContainer *bcc)
{
    int nparticle=0;
    for(unsigned int i=0; i<bcc->taus->size(); ++i) {
        if(bcc->taus->at(i).pt()>m_ptmin && fabs(bcc->taus->at(i).eta())<m_etamax) nparticle++;
    }
    return nparticle>=m_min_nparticle && nparticle<=m_max_nparticle;
}

std::string NTauSelection::description()
{
    char s[100];
    sprintf(s, "%d <= number of taus <= %d, with pt>%.1f GeV, abs(eta)<%.1f",m_min_nparticle,m_max_nparticle,m_ptmin,m_etamax);

    return s;
}


NJetSelection::NJetSelection(int min_nparticle, int max_nparticle, double ptmin, double etamax)
{
    m_min_nparticle=min_nparticle;
    m_max_nparticle=max_nparticle;
    m_ptmin=ptmin;
    m_etamax=etamax;
}

bool NJetSelection::pass(BaseCycleContainer *bcc)
{
    int nparticle=0;
    for(unsigned int i=0; i<bcc->jets->size(); ++i) {
        if(bcc->jets->at(i).pt()>m_ptmin && fabs(bcc->jets->at(i).eta())<m_etamax) nparticle++;
    }
    return nparticle>=m_min_nparticle && nparticle<=m_max_nparticle;
}

std::string NJetSelection::description()
{
    char s[100];
    sprintf(s, "%d <= number of jets <= %d, with pt>%.1f GeV, abs(eta)<%.1f",m_min_nparticle,m_max_nparticle,m_ptmin,m_etamax);

    return s;
}


NTopJetSelection::NTopJetSelection(int min_nparticle, int max_nparticle, double ptmin, double etamax)
{
    m_min_nparticle=min_nparticle;
    m_max_nparticle=max_nparticle;
    m_ptmin=ptmin;
    m_etamax=etamax;
}


CAAntiktJetSelection::CAAntiktJetSelection(unsigned int min_Topjets, unsigned  int min_Jets, double min_distance, unsigned int max_Topjets, unsigned int max_Jets){
  m_min_Topjets=min_Topjets; 
  m_max_Topjets=max_Topjets;
  m_min_Jets=min_Jets;
  m_max_Jets=max_Jets;
  m_min_distance=min_distance;             
}

bool CAAntiktJetSelection::pass(BaseCycleContainer *bcc){

  if(bcc->topjets->size()< m_min_Topjets || bcc->topjets->size()> m_max_Topjets ) return false;
  if(bcc->jets->size() < m_min_Jets || bcc->jets->size()> m_max_Jets ) return false;

  //for(unsigned int p=0; p< bcc->jets->size(); ++p){
  //  Jet antiktjet = bcc->jets->at(p);
    for(unsigned int i=0; i< bcc->topjets->size(); ++i){
      TopJet topjet =  bcc->topjets->at(i);
      //if(topjet.deltaR(antiktjet)> m_min_distance)return true;
      if(topjet.deltaR(bcc->muons->at(0))> m_min_distance)return true;
    }
    //}
  return false;
}

std::string CAAntiktJetSelection::description(){
  char s[100];
  sprintf(s, "%d <= N(CA-Jets) <= %d , %d <= N(Jets)<= %d ,with minimal delR <%.1f",m_min_Topjets,m_max_Topjets,m_min_Jets,m_max_Jets,m_min_distance);
  return s;
}

TopTagAntiktJetSelection::TopTagAntiktJetSelection(unsigned int min_TopTag, unsigned  int min_Jets, double min_distance, unsigned int max_TopTag, unsigned int max_Jets){
  m_min_TopTag=min_TopTag; 
  m_max_TopTag=max_TopTag;
  m_min_Jets=min_Jets;
  m_max_Jets=max_Jets;
  m_min_distance=min_distance;             
}

bool TopTagAntiktJetSelection::pass(BaseCycleContainer *bcc){

  if(bcc->jets->size() < m_min_Jets || bcc->jets->size()> m_max_Jets ) return false;
  double mjet = 0;
  int nsubjets = 0;
  double mmin = 0;
  unsigned int nTopTags = 0;

  double deltaR_Lep_Tophad = -1;
  TopJet top_had;
  Particle lepton;
  lepton.set_v4(bcc->muons->at(0).v4());

  for(unsigned int i = 0; i< bcc->topjets->size();++i){
    TopJet topjet = bcc->topjets->at(i);
    if(TopTag(topjet,mjet,nsubjets,mmin) && deltaR_Lep_Tophad < topjet.deltaR(lepton)){
      top_had = topjet;
      deltaR_Lep_Tophad = topjet.deltaR(lepton);
    }
    if(TopTag(topjet,mjet,nsubjets,mmin)) nTopTags++;
  }
   
  if(nTopTags < m_min_TopTag || nTopTags > m_max_TopTag) return false;

  
  for(unsigned int p=0; p< bcc->jets->size(); ++p){
    Jet antiktjet = bcc->jets->at(p);
    if(top_had.deltaR(antiktjet)> m_min_distance) return true; 
  }

  return false;
}

std::string TopTagAntiktJetSelection::description(){
  char s[100];
  sprintf(s, "%d <= N(TopTags) <= %d , %d <= N(Jets)<= %d ,with minimal delR <%.1f",m_min_TopTag,m_max_TopTag,m_min_Jets,m_max_Jets,m_min_distance);
  return s;
}

bool NTopJetSelection::pass(BaseCycleContainer *bcc)
{
    int nparticle=0;
    for(unsigned int i=0; i<bcc->topjets->size(); ++i) {
        if(bcc->topjets->at(i).pt()>m_ptmin && fabs(bcc->topjets->at(i).eta())<m_etamax) nparticle++;
    }
    return nparticle>=m_min_nparticle && nparticle<=m_max_nparticle;
}


std::string NTopJetSelection::description()
{
    char s[100];
    sprintf(s, "%d <= N(CA-jets) <= %d, with pt>%.1f GeV, |eta|<%.1f",m_min_nparticle,m_max_nparticle,m_ptmin,m_etamax);

    return s;
}

NPrunedJetSelection::NPrunedJetSelection(int min_nparticle, int max_nparticle, double ptmin, double etamax)
{
    m_min_nparticle=min_nparticle;
    m_max_nparticle=max_nparticle;
    m_ptmin=ptmin;
    m_etamax=etamax;
}

bool NPrunedJetSelection::pass(BaseCycleContainer *bcc)
{
    int nparticle=0;
    for(unsigned int i=0; i<bcc->prunedjets->size(); ++i) {
        if(bcc->prunedjets->at(i).pt()>m_ptmin && fabs(bcc->prunedjets->at(i).eta())<m_etamax) nparticle++;
    }
    return nparticle>=m_min_nparticle && nparticle<=m_max_nparticle;
}

std::string NPrunedJetSelection::description()
{
    char s[100];
    sprintf(s, "%d <= N(pruned jets) <= %d, with pt>%.1f GeV, |eta|<%.1f",m_min_nparticle,m_max_nparticle,m_ptmin,m_etamax);

    return s;
}



NTopTagSelection::NTopTagSelection(int min_ntoptag, int max_ntoptag)
{
    m_min_ntoptag=min_ntoptag;
    m_max_ntoptag=max_ntoptag;
}

bool NTopTagSelection::pass(BaseCycleContainer *bcc)
{

    int ntoptag=0;

    for(unsigned int i=0; i< bcc->topjets->size(); ++i) {
        TopJet topjet =  bcc->topjets->at(i);
        double mmin=0;
        double mjet=0;
        int nsubjets=0;
        if(TopTag(topjet,mjet,nsubjets,mmin)) ntoptag++;
    }
    if(ntoptag<m_min_ntoptag) return false;
    if(ntoptag>m_max_ntoptag) return false;
    return true;

}

std::string NTopTagSelection::description()
{
    char s[100];
    sprintf(s, "%d <= N(top-tags) <= %d",m_min_ntoptag,m_max_ntoptag);
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
    if(HepTopTagFull(topjet)) nheptoptag++;
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

NHEPTopAndSubBTagSelection::NHEPTopAndSubBTagSelection(int min_nheptoptag, int max_nheptoptag, E_BtagType type){
  m_min_nheptoptag = min_nheptoptag;
  m_max_nheptoptag = max_nheptoptag; 
  m_type = type;
  cout << m_type << " step4" << endl;
}

bool NHEPTopAndSubBTagSelection::pass(BaseCycleContainer *bcc){

  int nheptoptag=0;  
  double ptcut=150;
  for(unsigned int i=0; i< bcc->topjets->size(); ++i){
    if(bcc->topjets->at(i).pt()<ptcut) continue;
    TopJet topjet =  bcc->topjets->at(i);
    if(HepTopTagFull(topjet) && subJetBTag(topjet, m_type)>=1) nheptoptag++;
  }
  if(nheptoptag<m_min_nheptoptag) return false;
  if(nheptoptag>m_max_nheptoptag) return false;
  return true;

}

std::string NHEPTopAndSubBTagSelection::description(){
  char s[100];
   cout << m_type << " step4" << endl;
  sprintf(s, "%d <= N(top-tags) <= %d",m_min_nheptoptag,m_max_nheptoptag);
  return s;
}


//selects events with one jet with a heptoptag and a b-tagged sub-jet and another jet that is higgs-tagged
HEPTopAndSubBTagPlusOtherHiggsTag::HEPTopAndSubBTagPlusOtherHiggsTag(E_BtagType type1, E_BtagType type2, E_BtagType type3){
   m_type1 = type1; //to be used in top-tag
   m_type2 = type2; //to be used in higgs-tag
   m_type3 = type3; //to be used in higgs-tag
}

bool HEPTopAndSubBTagPlusOtherHiggsTag::pass(BaseCycleContainer *bcc){
  int nheptoptag=0;
  int nhiggstag=0;
  std::vector<int> topTaggedJets;
  std::vector<int> HiggsTaggedJets;
  double ptcut=150;
  for(unsigned int i=0; i< bcc->topjets->size(); ++i){
    if(bcc->topjets->at(i).pt()<ptcut) continue;
    TopJet topjet =  bcc->topjets->at(i);
    if(HepTopTagFull(topjet) && subJetBTag(topjet, m_type1)>=1){
      nheptoptag++;
      topTaggedJets.push_back(i);
    }
    if (HiggsTag(topjet, m_type2, m_type3)){
      nhiggstag++;
      HiggsTaggedJets.push_back(i);
    }
  }

if (nheptoptag ==0) return false;
if (nhiggstag ==0) return false; 
if(nheptoptag == 1 && nhiggstag == 1 && topTaggedJets[0] == HiggsTaggedJets[0]) return false;
return true;

}

std::string HEPTopAndSubBTagPlusOtherHiggsTag::description(){
  char s[100];
  sprintf(s, "1 toptag with b-tagged sub jet and 1 higgs tag");
  return s;
}

STCut::STCut(double min_st, double max_st){
  m_min_st = min_st;
  m_max_st = max_st;
}

bool STCut::pass(BaseCycleContainer *bcc){
  
  EventCalc* calc = EventCalc::Instance();
  double st = calc->GetHT();
  if( st < m_min_st) return false;
  if( st > m_max_st) return false;
  return true;

}

std::string STCut::description(){
  char s[100];
  sprintf(s, "%.1f GeV < ST < %.1f GeV",m_min_st,m_max_st);
  return s;
}

HTCut::HTCut(double min_ht, double max_ht){
 m_min_ht = min_ht;
  m_max_ht = max_ht;
}

bool HTCut::pass(BaseCycleContainer *bcc){
  //double htlep = HTlep(bcc);
  EventCalc* calc = EventCalc::Instance();
  double htlep = calc->GetHTlep();
  double htall = calc->GetHT();
  double ht = htall-htlep;
  if( ht < m_min_ht) return false;
  if( ht > m_max_ht) return false;
  return true;

}

std::string HTCut::description(){
  char s[100];
  sprintf(s, "%.1f GeV < HT < %.1f GeV",m_min_ht,m_max_ht);
  return s;
}


NWTagSelection::NWTagSelection(int min_nwtag, int max_nwtag)
{
    m_min_nwtag=min_nwtag;
    m_max_nwtag=max_nwtag;
}

bool NWTagSelection::pass(BaseCycleContainer *bcc)
{

    int nwtag=0;

    for(unsigned int i=0; i< bcc->prunedjets->size(); ++i) {
        TopJet prunedjet =  bcc->prunedjets->at(i);
        double mmin=0;
        double mjet=0;
        int nsubjets=0;
        if(WTag(prunedjet,mjet,nsubjets,mmin)) nwtag++;
    }
    if(nwtag<m_min_nwtag) return false;
    if(nwtag>m_max_nwtag) return false;
    return true;

}

std::string NWTagSelection::description()
{
    char s[100];
    sprintf(s, "%d <= N(W-tags) <= %d",m_min_nwtag,m_max_nwtag);
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



NPrimaryVertexSelection::NPrimaryVertexSelection(int min_npv, int max_npv)
{
    m_min_npv=min_npv;
    m_max_npv=max_npv;
}

bool NPrimaryVertexSelection::pass(BaseCycleContainer *bcc)
{

    int npv=bcc->pvs->size();

    if(npv<m_min_npv) return false;
    if(npv>m_max_npv) return false;
    return true;

}

std::string NPrimaryVertexSelection::description()
{
    char s[100];
    sprintf(s, "%d <= number of primary vertices <= %d",m_min_npv,m_max_npv);
    return s;
}



HTlepCut::HTlepCut(double min_htlep, double max_htlep)
{
    m_min_htlep = min_htlep;
    m_max_htlep = max_htlep;
}

bool HTlepCut::pass(BaseCycleContainer *bcc)
{
    //double htlep = HTlep(bcc);
    EventCalc* calc = EventCalc::Instance();
    double htlep = calc->GetHTlep();
    if( htlep < m_min_htlep) return false;
    if( htlep > m_max_htlep) return false;
    return true;

}

std::string HTlepCut::description()
{
    char s[100];
    sprintf(s, "%.1f GeV < HTlep < %.1f GeV",m_min_htlep,m_max_htlep);
    return s;
}

HTmuonCut::HTmuonCut(double min_htmuon, double max_htmuon)
{
    m_min_htmuon = min_htmuon;
    m_max_htmuon = max_htmuon;
}

bool HTmuonCut::pass(BaseCycleContainer *bcc)
{
    //double htmuon = HTmuon(bcc);
    EventCalc* calc = EventCalc::Instance();
    double htmuon = 0;

    for(unsigned int i=0; i<calc->GetMuons()->size(); ++i){
      htmuon += calc->GetMuons()->at(i).pt();
    }
    if(bcc->met) htmuon += bcc->met->pt(); 

    if( htmuon < m_min_htmuon) return false;
    if( htmuon > m_max_htmuon) return false;
    return true;

}

std::string HTmuonCut::description()
{
    char s[100];
    sprintf(s, "%.1f GeV < HTmuon < %.1f GeV",m_min_htmuon,m_max_htmuon);
    return s;
}



METCut::METCut(double min_met, double max_met)
{
    m_min_met = min_met;
    m_max_met = max_met;
}

bool METCut::pass(BaseCycleContainer *bcc)
{
    double MET = bcc->met->pt();
    if( MET < m_min_met) return false;
    if( MET > m_max_met) return false;
    return true;

}

std::string METCut::description()
{
    char s[100];
    sprintf(s, "%.1f GeV < MET < %.1f GeV",m_min_met,m_max_met);
    return s;
}


bool TwoDCut::pass(BaseCycleContainer *bcc)
{

    //make sure that you have cleaned the electron and muon collections: bcc should content only one charged lepton
    //clean jets for pt>25 GeV before calling this routine
    for(unsigned int i=0; i<bcc->electrons->size(); ++i) {
        //std::cout<< deltaRmin(&(bcc->electrons->at(i)), bcc->jets) << "   " << pTrel(&(bcc->electrons->at(i)), bcc->jets) << std::endl;
        if(deltaRmin(&(bcc->electrons->at(i)), bcc->jets)<0.5 && pTrel(&(bcc->electrons->at(i)), bcc->jets)<25)
            return false;
    }
    for(unsigned int i=0; i<bcc->muons->size(); ++i) {
        if(deltaRmin(&(bcc->muons->at(i)), bcc->jets)<0.5 && pTrel(&(bcc->muons->at(i)), bcc->jets)<25)
            return false;
    }

    return true;
}

std::string TwoDCut::description()
{
    char s[100];
    sprintf(s, "2D cut");
    return s;
}


TwoDCutMuon::TwoDCutMuon(double delRmin , double pTrelmin)
{
  m_delRmin = delRmin;
  m_pTrelmin = pTrelmin;
}

bool TwoDCutMuon::pass(BaseCycleContainer *bcc)
{
    // same as above, but only for muons
    for(unsigned int i=0; i<bcc->muons->size(); ++i) {
        if(deltaRmin(&(bcc->muons->at(i)), bcc->jets)< m_delRmin && pTrel(&(bcc->muons->at(i)), bcc->jets)< m_pTrelmin)
            return false;
    }

    return true;
}

std::string TwoDCutMuon::description()
{
    char s[100];
    sprintf(s, "2D cut");
    return s;
}

bool MuonElectronOSCut::pass(BaseCycleContainer *bcc)
{

    // make sure that you have cleaned the electron and muon collections: bcc should contain at least one muon and one electron
    // this cut looks only at the leading muon and leading electron
    if ( bcc->electrons->size()==0) return false;
    if ( bcc->muons->size()==0) return false;
    
    if (bcc->electrons->at(0).charge() == bcc->muons->at(0).charge()) return false;

    return true;
}

std::string MuonElectronOSCut::description()
{
    char s[100];
    sprintf(s, "Opposite sign of leading muon and leading electron");
    return s;
}

bool MuonTauOSCut::pass(BaseCycleContainer *bcc)
{

    // make sure that you have cleaned the tau and muon collections: bcc should contain at least one tau and one muon
    // this cut looks only at the leading muon and leading tau
    if ( bcc->taus->size()==0) return false;
    if ( bcc->muons->size()==0) return false;
    
    if (bcc->taus->at(0).charge() == bcc->muons->at(0).charge()) return false;

    return true;
}

std::string MuonTauOSCut::description()
{
    char s[100];
    sprintf(s, "Opposite sign of leading muon and leading tau");
    return s;
}



bool TriangularCut::pass(BaseCycleContainer *bcc)
{
    if(bcc->electrons->size()!=1) {
        std::cout << "WARNING: called triangular cut but electron collection contains " << bcc->electrons->size()<< " !=1 entries. Cut is not applied" <<std::endl;
        return true;
    }
    if(bcc->jets->size()<1) {
        std::cout << "WARNING: called triangular cut but jet collection is empty. Cut is not applied" <<std::endl;
        return true;
    }

    double k=1.5/75.;

    Particle METp;
    METp.set_pt(bcc->met->pt());
    METp.set_phi(bcc->met->phi());
    METp.set_eta(0);
    METp.set_energy(0);

    if(METp.deltaPhi(bcc->electrons->at(0)) > k* METp.pt()+1.5) return false;
    if(METp.deltaPhi(bcc->electrons->at(0)) < -1*k* METp.pt()+1.5) return false;
    if(METp.deltaPhi(bcc->jets->at(0)) > k* METp.pt()+1.5) return false;
    if(METp.deltaPhi(bcc->jets->at(0)) < -1*k* METp.pt()+1.5) return false;

    return true;
}

std::string TriangularCut::description()
{
    char s[100];
    sprintf(s, "triangular cut");
    return s;
}

bool TriangularCutMuon::pass(BaseCycleContainer *bcc)
{
    if(bcc->muons->size()!=1) {
        std::cout << "WARNING: called triangular cut but muon collection contains " << bcc->muons->size()<< " !=1 entries. Cut is not applied" <<std::endl;
        return true;
    }
    if(bcc->jets->size()<1) {
        std::cout << "WARNING: called triangular cut but jet collection is empty. Cut is not applied" <<std::endl;
        return true;
    }

    double k=1.5/75.;

    Particle METp;
    METp.set_pt(bcc->met->pt());
    METp.set_phi(bcc->met->phi());
    METp.set_eta(0);
    METp.set_energy(0);

    if(METp.deltaPhi(bcc->muons->at(0)) > k* METp.pt()+1.5) return false;
    if(METp.deltaPhi(bcc->muons->at(0)) < -1*k* METp.pt()+1.5) return false;
    if(METp.deltaPhi(bcc->jets->at(0)) > k* METp.pt()+1.5) return false;
    if(METp.deltaPhi(bcc->jets->at(0)) < -1*k* METp.pt()+1.5) return false;

    return true;
}

std::string TriangularCutMuon::description()
{
    char s[100];
    sprintf(s, "triangular cut for muons");
    return s;
}

HypothesisDiscriminatorCut::HypothesisDiscriminatorCut(HypothesisDiscriminator* discr, double min_discr, double max_discr)
{
    m_discr=discr;
    m_min_discr = min_discr;
    m_max_discr = max_discr;
}

bool HypothesisDiscriminatorCut::pass(BaseCycleContainer *bcc)
{

    ReconstructionHypothesis *hyp = m_discr->GetBestHypothesis();

    if(!hyp) {
        // std::cout << "WARNING: no hypothesis " << m_discr->GetLabel() << " found, event is rejected." <<std::endl;
        return false;
    }

    double discr_value = hyp->discriminator(m_discr->GetLabel());

    if(discr_value<m_min_discr || discr_value>m_max_discr) return false;

    return true;
}



std::string HypothesisDiscriminatorCut::description()
{
    char s[100];
    sprintf(s, "%.1f < %s discriminator < %.1f",m_min_discr,m_discr->GetLabel().c_str(),m_max_discr);
    return s;
}

HypothesisLeptopPtCut::HypothesisLeptopPtCut(HypothesisDiscriminator* discr, double min_discr, double max_discr)
{
    m_discr=discr;
    m_min_discr = min_discr;
    m_max_discr = max_discr;
}

bool HypothesisLeptopPtCut::pass(BaseCycleContainer *bcc)
{
    ReconstructionHypothesis *hyp = m_discr->GetBestHypothesis();

    if(!hyp) {
        std::cout << "WARNING: no hypothesis " << m_discr->GetLabel() << " found, event is rejected." <<std::endl;
        return false;
    }

    double discr_value = hyp->toplep_v4().Pt();

    if(discr_value<m_min_discr || discr_value>m_max_discr) return false;

    return true;
}

std::string HypothesisLeptopPtCut::description()
{
    char s[100];
    sprintf(s, "%.1f < %s leptop m < %.1f",m_min_discr,m_discr->GetLabel().c_str(),m_max_discr);
    return s;
}

MttbarGenCut::MttbarGenCut(double mttbar_min, double mttbar_max)
{
    m_mttbar_min = mttbar_min;
    m_mttbar_max = mttbar_max;
}

bool MttbarGenCut::pass(BaseCycleContainer *bcc)
{
    EventCalc* calc = EventCalc::Instance();
    if(!calc->GetGenParticles() ) {
        if (!calc->GetTTbarGen()) {
            return true;
        }
    }

    double mttbar_gen = ( calc->GetTTbarGen()->Top().v4() + calc->GetTTbarGen()->Antitop().v4()).M();
    if( mttbar_gen < m_mttbar_min) return false;
    if( mttbar_gen > m_mttbar_max) return false;
    return true;
}

std::string MttbarGenCut::description()
{
    char s[100];
    sprintf(s, "%.1f < MttbarGen < %.1f",m_mttbar_min, m_mttbar_max);
    return s;
}


ZGenCut::ZGenCut(int pdgid)
{
  m_pdgid = abs(pdgid);
}

bool ZGenCut::pass(BaseCycleContainer *bcc)
{
    EventCalc* calc = EventCalc::Instance();
    if(!calc->GetGenParticles() ) {
      return false;
    }

    for(unsigned int i=0; i< calc->GetGenParticles()->size(); ++i){

      GenParticle genp = calc->GetGenParticles()->at(i);
      if(genp.pdgId()==23){
	if(genp.daughter(calc->GetGenParticles(),1) && genp.daughter(calc->GetGenParticles(),2)){
	  if( abs(genp.daughter(calc->GetGenParticles(),1)->pdgId()) == m_pdgid && abs(genp.daughter(calc->GetGenParticles(),2)->pdgId()) == m_pdgid){
	    return true;
	  }
	}
      }
    }

    return false;
}

std::string ZGenCut::description()
{
    char s[100];
    sprintf(s, "generated Z decaying to pdgid = %.1d",m_pdgid);
    return s;
}



EventFlavorSelection::EventFlavorSelection(E_EventFlavor flavor)
{
    m_flavor = flavor;
}

bool EventFlavorSelection::pass(BaseCycleContainer *bcc)
{
    std::map<int,int> counter;
    for(unsigned int i=0; i<bcc->jets->size(); ++i) {
        Jet jet = bcc->jets->at(i);
        int flavor = abs(jet.flavor());
        // std::cout << flavor << " ";
        if (counter.find(flavor) == counter.end())
            counter[flavor] = 1;
        else
            counter[flavor] = counter[flavor] + 1;
    }
    
    // std::cout << std::endl;

    if (m_flavor == e_BFlavor && 
        counter.find(5) != counter.end()) {
        // std::cout << "It is a BFlavor event." << std::endl;
        return true;
    }
    if (m_flavor == e_CFlavor && 
        counter.find(5) == counter.end() &&
        counter.find(4) != counter.end()) {
        // std::cout << "It is a CFlavor event." << std::endl;
        return true;
    }
    if (m_flavor == e_LFlavor && 
        counter.find(5) == counter.end() &&
        counter.find(4) == counter.end()) {
        // std::cout << "It is a LFlavor event." << std::endl;
        return true;
    }

    return false;
}

std::string EventFlavorSelection::description()
{
    char s[100], flavor;
    if (m_flavor == e_BFlavor)
        flavor = 'B';
    else if (m_flavor == e_CFlavor)
        flavor = 'C';
    else
        flavor = 'L';
    sprintf(s, "Filtering by event %c flavor" , flavor);
    return s;
}

EventFilterSelection::EventFilterSelection(const std::string eventfile)
{
  m_evfilter = new EventFilterFromListStandAlone(eventfile);
  m_filename = eventfile;
}

EventFilterSelection::~EventFilterSelection()
{
  delete m_evfilter;
}

bool EventFilterSelection::pass(BaseCycleContainer* bcc)
{
  return m_evfilter->filter(bcc->run, bcc->luminosityBlock, bcc->event);
}

std::string EventFilterSelection::description()
{
  char s[500];
  sprintf(s, "selection of events based on file: %s", m_filename.c_str());
  return s;

}


TauMuonInvMassCut::TauMuonInvMassCut(double min_InvMass, double max_InvMass){
 m_min_InvMass = min_InvMass;
 m_max_InvMass = max_InvMass;
}

bool TauMuonInvMassCut::pass(BaseCycleContainer *bcc)
{
  if (bcc->taus->size() == 1 && bcc->muons->size() == 1)
    {
      Tau tau = bcc->taus->at(0);
      TLorentzVector Tau;
      Tau.SetPtEtaPhiE(tau.pt() ,tau.eta() ,tau.phi() ,tau.energy() );
   
      Muon muon = bcc->muons->at(0);
      TLorentzVector Mu;
      Mu.SetPtEtaPhiE(muon.pt() ,muon.eta() ,muon.phi() ,muon.energy() );
      TLorentzVector MuTau = Tau +Mu;
      double InvMass = MuTau.M();    
    
      if (InvMass < m_min_InvMass || InvMass > m_max_InvMass) return true;
      return false;
    }
  else return true;
}


SameSignCut::SameSignCut(){ 
}

bool SameSignCut::pass(BaseCycleContainer *bcc)
{
  for(unsigned int i=0; i< bcc->muons->size(); ++i)
    {
      Muon muon1 = bcc->muons->at(i);
      for(unsigned int j=0; j< bcc->muons->size(); ++j)
	{
	  if (i!=j)
	    {
	      Muon muon2 = bcc->muons->at(j); 
	      if (muon1.charge() == muon2.charge()) return true;
	    }
	}
      for(unsigned int j=0; j< bcc->taus->size(); ++j)
	{
	  Tau tau = bcc->taus->at(j);
	  if (muon1.charge() == tau.charge()) return true;
	}
      
    }
  for(unsigned int i=0; i< bcc->taus->size(); ++i)
    {
      Tau tau1 = bcc->taus->at(i);
      for(unsigned int j=0; j< bcc->taus->size(); ++j)
	{
	  if (i!=j)
	    {
	      Tau tau2 = bcc->taus->at(j); 
	      if (tau1.charge() == tau2.charge()) return true;
	    }
	}
    }
  return false;
}

 std::string SameSignCut::description(){
   char s[100];
   sprintf(s, "same-sign lepton pair requirement");
   return s;
 }


bool TauMuonMassCut::pass(BaseCycleContainer *bcc)
{ 
  if (bcc->muons->size() > 0 && bcc->taus->size() > 0)
    {
      double max_InvMass =0;
      for(unsigned int i=0; i< bcc->muons->size(); ++i)
	{
	  Muon muon = bcc->muons->at(i);
	  TLorentzVector Mu;
	  Mu.SetPtEtaPhiE(muon.pt() ,muon.eta() ,muon.phi() ,muon.energy() );
	  for(unsigned int j=0; j< bcc->taus->size(); ++j)
	    {
	      Tau tau = bcc->taus->at(j);
	      TLorentzVector Tau;
	      Tau.SetPtEtaPhiE(tau.pt() ,tau.eta() ,tau.phi() ,tau.energy() );
	      TLorentzVector DiLepton = Tau +Mu;
	      double InvMass = DiLepton.M();
	      if (InvMass > max_InvMass)  max_InvMass = InvMass;
	    }
	}
      if (max_InvMass > m_massmin && max_InvMass < m_massmax) return true;
    }
  return false;
}

std::string TauMuonMassCut::description()
{
  char s[100];
  sprintf(s, "%.1f < invariant mass leading muon and leading tau  < %.1f",m_massmin, m_massmax);
  return s;
}

GenTauSelection::GenTauSelection()
{ 
}

bool GenTauSelection::pass(BaseCycleContainer* bcc)
{
  if (bcc->taus->size()>0)
    {
      Tau tau = bcc->taus->at(0);
      for(unsigned int i=0; i<bcc->genparticles->size(); ++i)
	{
	  GenParticle genp = bcc->genparticles->at(i);
	  double deltaR = genp.deltaR(tau);
	  if (deltaR < 0.5 && abs(genp.pdgId())==15) return true;
	}
    }
  return false;
}

std::string GenTauSelection::description()
{
  char s[500];
  sprintf(s, "found a real tau");
  return s;
}



