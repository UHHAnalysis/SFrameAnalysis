#ifndef Selection_H
#define Selection_H

#include "Objects.h"
#include "BaseCycleContainer.h"
#include "core/include/SLogger.h"

class SelectionModule{
 public:
  SelectionModule(){};
  virtual ~SelectionModule(){};
   
  virtual bool pass(BaseCycleContainer*)=0;
  virtual std::string description()=0;
};


class Selection{
 public:
  
  //Selection(){};
  Selection(std::string name = "Selection");
  ~Selection(){};
  
  void addSelectionModule(SelectionModule*);
  void clearSelectionModulesList();
  bool passSelection(BaseCycleContainer *bcc);
  bool passInvertedSelection(BaseCycleContainer *bcc);

  void printCutFlow();

 private:
  mutable SLogger m_logger;
  std::vector<SelectionModule*> m_cuts;
  std::vector<int> m_cutflow;
};



#endif
