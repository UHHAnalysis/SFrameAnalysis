#include "../include/Selection.h"


Selection::Selection(std::string name):
  m_logger ( name.c_str() ){
  clearSelectionModulesList();
}

void Selection::addSelectionModule(SelectionModule* sel){
  m_cuts.push_back(sel);
  m_cutflow.push_back(0);
}

void Selection::clearSelectionModulesList(){
  m_cuts.clear();
  m_cutflow.clear();
}

bool Selection::passSelection(BaseCycleContainer *bcc){
  if(m_cuts.size()!=m_cutflow.size()){
    m_logger << WARNING << "size of cut list != number of entries in cut flow table "<< SLogger::endmsg;
  }
  for(unsigned int i=0; i<m_cuts.size(); ++i){
    if(!m_cuts[i]->pass(bcc)) return false;
    m_cutflow[i]++;
  }
  return true;
}

bool Selection::passInvertedSelection(BaseCycleContainer *bcc){
  
  for(unsigned int i=0; i<m_cuts.size(); ++i){
    if(!m_cuts[i]->pass(bcc)) return true;
  }
  return false;
}

void Selection::printCutFlow(){
  m_logger << INFO << "--------------- Cut Flow Table ---------------"<< SLogger::endmsg;
  if(m_cuts.size()!=m_cutflow.size()){
    m_logger << WARNING << "size of cut list != number of entries in cut flow table "<< SLogger::endmsg;
  }
  else{
    for(unsigned int i=0; i<m_cuts.size(); ++i){
      m_logger << INFO << m_cutflow[i] << "    " << m_cuts[i]->description() << SLogger::endmsg;
    }
  }
  m_logger << INFO << "----------------------------------------------"<< SLogger::endmsg;

}
