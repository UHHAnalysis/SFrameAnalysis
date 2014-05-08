#include "include/HTSelectionMods.h" 


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
  //double htlep = calc->GetHTlep();
  double htall = calc->GetHT();
  double ht = htall;//-htlep;
  if( ht < m_min_ht) return false;
  if( ht > m_max_ht) return false;
  return true;

}

std::string HTCut::description(){
  char s[100];
  sprintf(s, "%.1f GeV < HT < %.1f GeV",m_min_ht,m_max_ht);
  return s;
}



//for T' analysis
HTSubJetsCut::HTSubJetsCut(double min_ht, double max_ht){
  m_min_ht = min_ht;
  m_max_ht = max_ht;
}

bool HTSubJetsCut::pass(BaseCycleContainer *bcc){
  double ht = 0;//-htlep;
  std::vector<Particle> subjets_top;

  for(unsigned int i=0; i< bcc->topjets->size(); ++i){
    TopJet topjet =  bcc->topjets->at(i);
    subjets_top=topjet.subjets();
    for (unsigned int subj = 0; subj < subjets_top.size(); subj++){
      ht += subjets_top.at(subj).pt();
    }
  }
  if( ht < m_min_ht) return false;
  if( ht > m_max_ht) return false;
  return true;

}

std::string HTSubJetsCut::description(){
  char s[100];
  sprintf(s, "%.1f GeV < HTSubJets < %.1f GeV",m_min_ht,m_max_ht);
  return s;
}


HThadCut::HThadCut(double ptmin_jet, double etamax_jet, double min_ht, double max_ht){
  m_ptmin_jet = ptmin_jet;
  m_etamax_jet = etamax_jet;
  m_min_ht = min_ht;
  m_max_ht = max_ht;
}

bool HThadCut::pass(BaseCycleContainer *bcc){
  EventCalc* calc = EventCalc::Instance();
  double hthad = calc->GetHThad(m_ptmin_jet, m_etamax_jet);
  if( hthad < m_min_ht) return false;
  if( hthad > m_max_ht) return false;
  return true;

}

std::string HThadCut::description(){
  char s[100];
  sprintf(s, "%.1f GeV < HThad < %.1f GeV for jets with pt > %.1f GeV, abs(eta) < %.1f",m_min_ht,m_max_ht,m_ptmin_jet, m_etamax_jet);
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
