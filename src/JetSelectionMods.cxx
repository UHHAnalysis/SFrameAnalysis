#include "include/JetSelectionMods.h"




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

NJetdeltaySelection::NJetdeltaySelection(double deltay)
{
 
 
    m_ptmin=deltay;
 
}

bool NJetdeltaySelection::pass(BaseCycleContainer *bcc)
{
    int nparticle=0;
    sort(bcc->jets->begin(), bcc->jets->end(), HigherPt());
    Jet jet = bcc->jets->at(0);
    Jet jet2 = bcc->jets->at(1);
    return abs(jet.v4().Rapidity()-jet2.v4().Rapidity())<m_ptmin;
}

std::string NJetdeltaySelection::description()
{
    char s[100];
    sprintf(s, "delta rapidity leading jets <%.1f ",m_ptmin);

    return s;
}


NJetdeltaphiSelection::NJetdeltaphiSelection(double deltaphi)
{
 
 
    m_ptmin=deltaphi;
 
}

bool NJetdeltaphiSelection::pass(BaseCycleContainer *bcc)
{
    int nparticle=0;
    sort(bcc->jets->begin(), bcc->jets->end(), HigherPt());
    Jet jet = bcc->jets->at(0);
    Jet jet2 = bcc->jets->at(1);
    return abs(jet.v4().Phi()-jet2.v4().Phi())>m_ptmin;
}

std::string NJetdeltaphiSelection::description()
{
    char s[100];
    sprintf(s, "delta phi leading jets >%.1f ",m_ptmin);

    return s;
}

NTopJetdeltaphiSelection::NTopJetdeltaphiSelection(double deltaphi)
{
 
 
    m_ptmin=deltaphi;
 
}

bool NTopJetdeltaphiSelection::pass(BaseCycleContainer *bcc)
{
    int nparticle=0;
    sort(bcc->topjets->begin(), bcc->topjets->end(), HigherPt());
    TopJet topjet = bcc->topjets->at(0);
    TopJet topjet2 = bcc->topjets->at(1);
    return abs(topjet.v4().Phi()-topjet2.v4().Phi())>m_ptmin;
}

std::string NTopJetdeltaphiSelection::description()
{
    char s[100];
    sprintf(s, "delta phi leading topjets >%.1f ",m_ptmin);

    return s;
}










