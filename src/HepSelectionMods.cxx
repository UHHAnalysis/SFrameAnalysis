//---sframe new---- 
#include "include/HepSelectionMods.h"






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



//Inverted top-tag selection for QCD estimation from data in TPrime analysis (Region C)

InvertedHEPTopTagRegularBTagRegularHiggsTag::InvertedHEPTopTagRegularBTagRegularHiggsTag(E_BtagType type1, E_BtagType type2, E_BtagType type3, TString mode, TString filename, double HiggsMassCut){
   m_type1 = type1; //to be used in b-tag
   m_type2 = type2; //to be used in higgs-tag
   m_type3 = type3; //to be used in higgs-tag
   m_mode = mode;
   m_filename = filename;
   m_HiggsMassCut = HiggsMassCut;
}

bool InvertedHEPTopTagRegularBTagRegularHiggsTag::pass(BaseCycleContainer *bcc){
  int nheptoptag = 0;
  int nheptoptagInverted = 0;
  int nheptoptagInvertedPlusBTag = 0;
  int nhiggstag = 0;
  std::vector<int> topTaggedJetsInverted;
  std::vector<int> HiggsTaggedJets;
  double ptcut=150;
  for(unsigned int i=0; i< bcc->topjets->size(); ++i){
    if(bcc->topjets->at(i).pt()<ptcut) continue;
    TopJet topjet =  bcc->topjets->at(i);
    if(HepTopTagWithMatch(topjet)){
	nheptoptag++;
    }
    if (HepTopTagInverted(topjet) && subJetBTagTop(topjet, m_type1, m_mode, m_filename)>=1){
	topTaggedJetsInverted.push_back(i);
	nheptoptagInvertedPlusBTag++;
    }
    if (HiggsTag(topjet, m_type2, m_type3, m_mode, m_filename)){
      double HiggsMass = HiggsMassFromBTaggedSubjets(topjet, m_type2, m_mode, m_filename);
      if (HiggsMass > m_HiggsMassCut){
	nhiggstag++;
	HiggsTaggedJets.push_back(i);
      }
    }
  }
  if (nheptoptag != 0) return false;
  if (nheptoptagInvertedPlusBTag == 0) return false;
  if (nhiggstag == 0) return false;
  if (nheptoptagInvertedPlusBTag ==1 && nhiggstag == 1 && topTaggedJetsInverted[0] == HiggsTaggedJets[0]) return false;

return true;
}

std::string InvertedHEPTopTagRegularBTagRegularHiggsTag::description(){
  char s[100];
  sprintf(s, "1 toptag with b-tagged sub jet and 1 higgs tag");
  return s;
}


RegularHEPTopTagRegularBTagFullyInvertedHiggsTag::RegularHEPTopTagRegularBTagFullyInvertedHiggsTag(E_BtagType type1, E_BtagType type2, E_BtagType type3, TString mode, TString filename){
   m_type1 = type1; //to be used in top-tag
   m_type2 = type2; //to be used in higgs-tag
   m_type3 = type3; //to be used in higgs-tag
 m_mode = mode;
  m_filename = filename;
}

bool RegularHEPTopTagRegularBTagFullyInvertedHiggsTag::pass(BaseCycleContainer *bcc){
  int nheptoptag=0;
  int nhiggstag=0;
  std::vector<int> topTaggedJets;
  std::vector<int> HiggsTaggedJets;
  double ptcut=150;
  for(unsigned int i=0; i< bcc->topjets->size(); ++i){
    if(bcc->topjets->at(i).pt()<ptcut) continue;
    TopJet topjet =  bcc->topjets->at(i);
    if(HepTopTagWithMatch(topjet) && subJetBTagTop(topjet, m_type1, m_mode, m_filename)>=1){
      nheptoptag++;
      topTaggedJets.push_back(i);
    }
    if (HiggsTag(topjet, m_type2, m_type3, m_mode, m_filename)){
      nhiggstag++;
      HiggsTaggedJets.push_back(i);
    }
  }


  if(nhiggstag == 1){
    if(nheptoptag == 1 && topTaggedJets[0] == HiggsTaggedJets[0]) return true;// there is only one top-tagged jet and this jet can also be higgs-tagged, which is ok
    else return false;
  }
if (nhiggstag > 1) return false;
if (nheptoptag == 0) return false;
return true;

}

std::string RegularHEPTopTagRegularBTagFullyInvertedHiggsTag::description(){
  char s[100];
  sprintf(s, "1 toptag with b-tagged sub jet and 1 inverted higgs tag");
  return s;
}


//Inverted Higgs- and top-tag selection for QCD estimation from data in TPrime analysis(Region A)

InvertedHEPTopTagRegularBTagFullyInvertedHiggsTag::InvertedHEPTopTagRegularBTagFullyInvertedHiggsTag(E_BtagType type1, E_BtagType type2, E_BtagType type3, TString mode, TString filename){
   m_type1 = type1; //to be used in top-tag
   m_type2 = type2; //to be used in higgs-tag
   m_type3 = type3; //to be used in higgs-tag
   m_mode = mode;
  m_filename = filename;
}

bool InvertedHEPTopTagRegularBTagFullyInvertedHiggsTag::pass(BaseCycleContainer *bcc){
  int nheptoptag=0;
  int nhiggstag=0;
  std::vector<int> topTaggedJets;
  std::vector<int> HiggsTaggedJets;
  double ptcut=150;
  for(unsigned int i=0; i< bcc->topjets->size(); ++i){
    if(bcc->topjets->at(i).pt()<ptcut) continue;
    TopJet topjet =  bcc->topjets->at(i);
    if(HepTopTagInverted(topjet) && subJetBTagTop(topjet, m_type1, m_mode, m_filename)>=1){
      nheptoptag++;
      topTaggedJets.push_back(i);
    }
    if (HiggsTag(topjet, m_type2, m_type3, m_mode, m_filename)){
      nhiggstag++;
      HiggsTaggedJets.push_back(i);
    }
  }



 if(nhiggstag == 1){
   if(nheptoptag == 1 && topTaggedJets[0] == HiggsTaggedJets[0]) return true;// there is only one top-tagged jet and this jet can also be higgs-tagged, which is ok
    else return false;
  }
 
if (nhiggstag > 1) return false;
if (nheptoptag == 0) return false;
return true;

}

std::string InvertedHEPTopTagRegularBTagFullyInvertedHiggsTag::description(){
  char s[100];
  sprintf(s, "1 toptag with b-tagged sub jet and 1 inverted higgs tag");
  return s;
}
