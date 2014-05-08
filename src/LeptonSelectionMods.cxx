//---sframe new---- 
#include "include/LeptonSelectionMods.h"



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


NAntiMuonTopJetsSelection::NAntiMuonTopJetsSelection(int min_ntops, int max_ntops, double ptmin, double etamax )
{
  m_min_ntops=min_ntops;
  m_max_ntops=max_ntops;
  m_ptmin=ptmin;
  m_etamax=etamax;
}

bool NAntiMuonTopJetsSelection::pass(BaseCycleContainer *bcc)
{
    int ntops=0;

    //Assumes to have only one muon
    if(bcc->muons->size() != 1){
      std::cout << "ATTENTION!!! muon size " << bcc->muons->size() << std::endl;
    }
    
    for(unsigned int i=0; i<bcc->topjets->size(); ++i) {

      double deltaphi=bcc->topjets->at(i).deltaPhi(bcc->muons->at(0));
      
      if((deltaphi>(2*PI/3))&&(bcc->topjets->at(i).pt()>m_ptmin)&&(fabs(bcc->topjets->at(i).eta())<m_etamax)){
	
	ntops++;
	
      }
      
    }
    
    if(ntops<m_min_ntops) return false;
    if(ntops>m_max_ntops) return false;
    return true;
}

std::string NAntiMuonTopJetsSelection::description()
{
    char s[100];
    sprintf(s, "%d <= number of top-jets in the muon anti-hemisphere <= %d",m_min_ntops,m_max_ntops);
    return s;
}

NAntiMuonHEPBTagSelection::NAntiMuonHEPBTagSelection(int min_nbtag, int max_nbtag, E_BtagType type, double ptmin, double etamax )
{
  m_min_nbtag=min_nbtag;
  m_max_nbtag=max_nbtag;
  m_type=type;
  m_ptmin=ptmin;
  m_etamax=etamax;
}

bool NAntiMuonHEPBTagSelection::pass(BaseCycleContainer *bcc)
{
    int nbtag=0;

    //Assumes to have only one muon

    for(unsigned int i=0; i<bcc->topjets->size(); ++i) {

      if(!(HepTopTag(bcc->topjets->at(i)))) continue;

      int jettagged=0;

      std::vector<Particle> subjets_top;
      std::vector<float> btagsub_combinedSecondaryVertex_top;
      subjets_top=bcc->topjets->at(i).subjets();
      btagsub_combinedSecondaryVertex_top=bcc->topjets->at(i).btagsub_combinedSecondaryVertex();

      float test=btagsub_combinedSecondaryVertex_top[2];
      
      if(m_type==e_CSVL && test>0.244) jettagged=1;
      if(m_type==e_CSVM && test>0.679) jettagged=1;
      if(m_type==e_CSVT && test>0.898) jettagged=1;
      
      test=btagsub_combinedSecondaryVertex_top[0];
      
      if(m_type==e_CSVL && test>0.244) jettagged=1;
      if(m_type==e_CSVM && test>0.679) jettagged=1;
      if(m_type==e_CSVT && test>0.898) jettagged=1;

      test=btagsub_combinedSecondaryVertex_top[1];
      
      if(m_type==e_CSVL && test>0.244) jettagged=1;
      if(m_type==e_CSVM && test>0.679) jettagged=1;
      if(m_type==e_CSVT && test>0.898) jettagged=1;

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

std::string NAntiMuonHEPBTagSelection::description()
{
    char s[100];
    sprintf(s, "%d <= number of hep-top-tagged jets with b-tagged b-hep-subjet in the muon anti-hemisphere <= %d",m_min_nbtag,m_max_nbtag);
    return s;
}

NAntiMuonSubBTagSelection::NAntiMuonSubBTagSelection(int min_nbtag, int max_nbtag, E_BtagType type, double ptmin, double etamax , TString filename)
{
    m_min_nbtag=min_nbtag;
    m_max_nbtag=max_nbtag;
    m_type=type;
    m_ptmin=ptmin;
    m_etamax=etamax;
    m_filename=filename;
}

bool NAntiMuonSubBTagSelection::pass(BaseCycleContainer *bcc)
{
    int nbtag=0;

    //Assumes to have only one muon

    for(unsigned int i=0; i<bcc->topjets->size(); ++i) {
      int jettagged=0;

      std::vector<Particle> subjets_top;
      std::vector<float> btagsub_combinedSecondaryVertex_top;
      subjets_top=bcc->topjets->at(i).subjets();
      btagsub_combinedSecondaryVertex_top=bcc->topjets->at(i).btagsub_combinedSecondaryVertex();

      for(unsigned int j=0; j < btagsub_combinedSecondaryVertex_top.size(); ++j){

	float test=btagsub_combinedSecondaryVertex_top[j];

	if(m_type==e_CSVL && test>0.244) jettagged=1;
	if(m_type==e_CSVM && test>0.679) jettagged=1;
	if(m_type==e_CSVT && test>0.898) jettagged=1;
	
	if(m_filename!=""){

	  jettagged=0;
	  
	  if(subJetBTagOne(bcc->topjets->at(i),m_type, "mean", m_filename, j)) jettagged=1;
	}

      }

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

std::string NAntiMuonSubBTagSelection::description()
{
    char s[100];
    sprintf(s, "%d <= number of sub-b-tagged top jets in the muon anti-hemisphere <= %d",m_min_nbtag,m_max_nbtag);
    return s;
}

NAntiMuonSubBTagSelectionOne::NAntiMuonSubBTagSelectionOne(int min_nbtag, int max_nbtag, E_BtagType type, double ptmin, double etamax , TString filename)
{
    m_min_nbtag=min_nbtag;
    m_max_nbtag=max_nbtag;
    m_type=type;
    m_ptmin=ptmin;
    m_etamax=etamax;
    m_filename=filename;
}

bool NAntiMuonSubBTagSelectionOne::pass(BaseCycleContainer *bcc)
{
    int nbtag=0;

    //Assumes to have only one muon

    for(unsigned int i=0; i<bcc->topjets->size(); ++i) {
      int jettagged=0;

      std::vector<Particle> subjets_top;
      std::vector<float> btagsub_combinedSecondaryVertex_top;
      subjets_top=bcc->topjets->at(i).subjets();
      btagsub_combinedSecondaryVertex_top=bcc->topjets->at(i).btagsub_combinedSecondaryVertex();

      for(unsigned int j=0; j < btagsub_combinedSecondaryVertex_top.size(); ++j){

	float test=btagsub_combinedSecondaryVertex_top[j];
	
	if(m_filename!=""){
	  
	  if(subJetBTagOne(bcc->topjets->at(i),m_type, "mean", m_filename, j)){
	    jettagged+=1;
	  }

	}
	else{
	  if(m_type==e_CSVL && test>0.244) jettagged+=1;
	  if(m_type==e_CSVM && test>0.679) jettagged+=1;
	  if(m_type==e_CSVT && test>0.898) jettagged+=1;
	}

      }

      if(bcc->muons->size() != 1){
	std::cout << "ATTENTION!!! muon size " << bcc->muons->size() << std::endl;
      }

      double deltaphi=bcc->topjets->at(i).deltaPhi(bcc->muons->at(0));
      
      if((jettagged==1)&&(deltaphi>(2*PI/3))&&(bcc->topjets->at(i).pt()>m_ptmin)&&(fabs(bcc->topjets->at(i).eta())<m_etamax)){

	nbtag++;

      }

    }

    if(nbtag<m_min_nbtag) return false;
    if(nbtag>m_max_nbtag) return false;
    return true;
}

std::string NAntiMuonSubBTagSelectionOne::description()
{
    char s[100];
    sprintf(s, "%d <= number of sub-b-tagged top jets in the muon anti-hemisphere <= %d",m_min_nbtag,m_max_nbtag);
    return s;
}

NMuonBTagSelection::NMuonBTagSelection(int min_nbtag, int max_nbtag, E_BtagType type, double ptmin, double etamax )
{
    m_min_nbtag=min_nbtag;
    m_max_nbtag=max_nbtag;
    m_type=type;
    m_ptmin=ptmin;
    m_etamax=etamax;
}

bool NMuonBTagSelection::pass(BaseCycleContainer *bcc)
{
    int nbtag=0;

    //Assumes to have only one muon

    for(unsigned int i=0; i<bcc->jets->size(); ++i) {
      int jettagged=0;
      if(m_type==e_CSVL && bcc->jets->at(i).btag_combinedSecondaryVertex()>0.244) jettagged=1;
      if(m_type==e_CSVM && bcc->jets->at(i).btag_combinedSecondaryVertex()>0.679) jettagged=1;
      if(m_type==e_CSVT && bcc->jets->at(i).btag_combinedSecondaryVertex()>0.898) jettagged=1;

      if(bcc->muons->size() != 1){
	std::cout << "ATTENTION!!! muon size " << bcc->muons->size() << std::endl;
      }

      double deltaphi=bcc->jets->at(i).deltaPhi(bcc->muons->at(0));
      
      if(jettagged&&(deltaphi<(2*PI/3))&&(bcc->jets->at(i).pt()>m_ptmin)&&(fabs(bcc->jets->at(i).eta())<m_etamax)){

	nbtag++;

      }

    }

    if(nbtag<m_min_nbtag) return false;
    if(nbtag>m_max_nbtag) return false;
    return true;
}

std::string NMuonBTagSelection::description()
{
    char s[100];
    sprintf(s, "%d <= number of b-tags in the muon hemisphere <= %d",m_min_nbtag,m_max_nbtag);
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

TauMuonInvMassCut::TauMuonInvMassCut(double min_InvMass, double max_InvMass){
 m_min_InvMass = min_InvMass;
 m_max_InvMass = max_InvMass;
}

bool TauMuonInvMassCut::pass(BaseCycleContainer *bcc)
{
  if (bcc->muons->size() == 1)
    {
      Muon muon = bcc->muons->at(0);
      TLorentzVector Mu;
      Mu.SetPtEtaPhiE(muon.pt() ,muon.eta() ,muon.phi() ,muon.energy() );
      for(unsigned int i=0; i<bcc->taus->size(); ++i) 
	{
	  Tau tau = bcc->taus->at(i);
	  TLorentzVector Tau;
	  Tau.SetPtEtaPhiE(tau.pt() ,tau.eta() ,tau.phi() ,tau.energy() );
   	  TLorentzVector MuTau = Tau +Mu;
	  double InvMass = MuTau.M();    
          if (InvMass > m_min_InvMass && InvMass < m_max_InvMass) return false;
	}
      return true;
    }
  else return true;
}

std::string TauMuonInvMassCut::description()
{
  char s[100];
  sprintf(s, "%.1f < invariant mass leading muon and leading tau  < %.1f",m_min_InvMass, m_max_InvMass);
  return s;
}

MuonInvMassCut::MuonInvMassCut(double min_InvMass, double max_InvMass){
 m_min_InvMass = min_InvMass;
 m_max_InvMass = max_InvMass;
}

bool MuonInvMassCut::pass(BaseCycleContainer *bcc)
{
  for(unsigned int i=0; i<bcc->muons->size(); ++i) 
    {
      Muon muon1 = bcc->muons->at(i);
      TLorentzVector Mu1;
      Mu1.SetPtEtaPhiE(muon1.pt() ,muon1.eta() ,muon1.phi() ,muon1.energy() );
      for(unsigned int j=0; j<bcc->muons->size(); ++j) 
	{
	  Muon muon2 = bcc->muons->at(j);
	  TLorentzVector Mu2;
	  Mu2.SetPtEtaPhiE(muon2.pt() ,muon2.eta() ,muon2.phi() ,muon2.energy() );
   	  TLorentzVector Vec =  Mu1+Mu2;
	  double InvMass = Vec.M();    
          if (InvMass > m_min_InvMass && InvMass < m_max_InvMass) return true;
	}
      return false;
    }
  return false;
}

std::string MuonInvMassCut::description()
{
  char s[100];
  sprintf(s, "%.1f < invariant mass muons  < %.1f",m_min_InvMass, m_max_InvMass);
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
  sprintf(s, "%.1f < invariant mass muons and taus  < %.1f",m_massmin, m_massmax);
  return s;
}

GenTauSelection::GenTauSelection()
{ 
}

bool GenTauSelection::pass(BaseCycleContainer* bcc)
{
  for (unsigned int i=0; i<bcc->taus->size(); ++i) 
    {
      Tau tau = bcc->taus->at(i);
      for(unsigned int j=0; j<bcc->genparticles->size(); ++j)
	{
	  GenParticle genp = bcc->genparticles->at(j);
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

OneProngTauSelection::OneProngTauSelection()
{ 
}

bool OneProngTauSelection::pass(BaseCycleContainer* bcc)
{
  Tau tau = bcc->taus->at(0);
  if (tau.decayMode() == 0 || tau.decayMode() == 1 || tau.decayMode() == 2 || tau.decayMode() == 3 || tau.decayMode() == 4 ) return true;
  else return false;
}

std::string OneProngTauSelection::description()
{
  char s[500];
  sprintf(s, "found a one prong tau decay");
  return s;
}


FakeTauSelectionElectron::FakeTauSelectionElectron()
{ 
}

bool FakeTauSelectionElectron::pass(BaseCycleContainer* bcc)
{
  for(unsigned int i=0; i<bcc->taus->size(); ++i)
    {
      Tau tau = bcc->taus->at(i);
      for(unsigned int j=0; j<bcc->genparticles->size(); ++j)
	{
	  GenParticle genp = bcc->genparticles->at(j);
	  double deltaR = genp.deltaR(tau);
	  if (deltaR < 0.5 && abs(genp.pdgId())== 11 && genp.status()==3) return true;
	}
    }
  return false;
}

std::string FakeTauSelectionElectron::description()
{
  char s[500];
  sprintf(s, "found an electron faking a tau");
  return s;
}

SameSignCutHalil::SameSignCutHalil(){ 
}

 bool SameSignCutHalil::pass(BaseCycleContainer *bcc)
 {
    for(unsigned int i=0; i< bcc->muons->size(); ++i)
       {
          Muon muon = bcc->muons->at(i);
          for(unsigned int j=0; j< bcc->taus->size(); ++j)
             {
                Tau tau = bcc->taus->at(j);
                if (muon.charge() == tau.charge()) return true;
             }
       
       }
    return false;
 }

  std::string SameSignCutHalil::description(){
    char s[100];
    sprintf(s, "same-sign lepton pair requirement");
    return s;
  }



SameSignCut::SameSignCut(){ 
}


bool SameSignCut::pass(BaseCycleContainer *bcc)
{
   if (bcc->muons->size()>0 && bcc->taus->size()>0)
      {
         Muon muon = bcc->muons->at(0);
         Tau tau = bcc->taus->at(0);
         if (muon.charge() == tau.charge()) return true;
      }
   return false;
}

 std::string SameSignCut::description(){
   char s[100];
   sprintf(s, "same-sign lepton pair requirement");
   return s;
 }





