#ifndef PhysicsTools_Utilities_EventFilterFromList
#define PhysicsTools_Utilities_EventFilterFromList

// -*- C++ -*-
//
// Package:    PhysicsTools
// Class:      EventFilterFromListStandAlone
//
// $Id: EventFilterFromListStandAlone.h,v 1.4 2013/01/16 17:19:24 vadler Exp $
//
/**
  \class    EventFilterFromListStandAlone EventFilterFromListStandAlone.h "physicsTools/utilities/interface/EventFilterFromListStandAlone.h"
  \brief    Stand-alone class to flag events, based on an event list in a gzipped tex file

   EventFilterFromListStandAlone provides a boolean flag, which marks events as "bad", if they appear in a given gzipped text file.
   The boolean returns
   - 'false', if the event is in the list ("bad event") and
   - 'true' otherwise ("good event").

   The class is designed as stand-alone utility, so that it can be used outside CMSSW, too.
   It is instatiated as follows:

     #include "[PATH/]EventFilterFromListStandAlone.h"
     [...]
     EventFilterFromListStandAlone myFilter("[GZIPPED_TEXT_FILE]");

   !!! --->>> There might be OFFICIAL releases of such EVENT LISTS, provided by the PdmV group <<<--- !!!
   An important example is the list of HCAL laser polluted events in

   EventFilter/HcalRawToDigi/data/HCALLaser2012AllDatasets.txt.gz

   The path to the gzipped input file needs to be the real path. CMSSW-like "FileInPath" is not supported.

   The boolean is then determined with

     bool myFlag = myFilter.filter(run_number, lumi_sec_number, event_number);

   where the parameters are all of type 'int'.

   Compilation:
   ============

   EventFilterFromListStandAlone uses 'zlib', which requires varying compilation settings, depending on the environment:

   LXPLUS, no CMSSW:
   -----------------
   - Files needed in current directory:
     EventFilterFromListStandAlone.h # a copy of this file
     test.cc                         # the actual code, using this class
     events.txt.gz                   # gzipped input file
   - In test.cc, you have
       #include "EventFilterFromListStandAlone.h"
       [...]
       EventFilterFromListStandAlone myFilter("./events.txt.gz");
       [...]
       bool myFlag = myFilter.filter(run_number, lumi_sec_number, event_number)
   - To compile:
       source /afs/cern.ch/sw/lcg/contrib/gcc/4.3/x86_64-slc5/setup.[c]sh
       source /afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.00/x86_64-slc5-gcc43-opt/root/bin/thisroot.[c]sh
       g++ -I$ROOTSYS/include -L$ROOTSYS/lib -lCore test.cc
     which results in the default executable './a.out'.

   LXPLUS, CMSSW environment
   -------------------------
   - Files needed in current directory:
     test.cc                         # the actual code, using this class
     events.txt.gz                   # gzipped input file
                                     # e.g. by
                                     # cp $CMSSW_RELEASE_BASE/src/EventFilter/HcalRawToDigi/data/HCALLaser2012AllDatasets.txt.gz ./events.txt.gz
   - In test.cc, you have
       #include "PhysicsTools/Utilities/interface/EventFilterFromListStandAlone.h"
       [...]
       EventFilterFromListStandAlone myFilter("./events.txt.gz");
       [...]
       bool myFlag = myFilter.filter(run_number, lumi_sec_number, event_number)
   - To compile:
       g++ -I$CMS_PATH/$SCRAM_ARCH/external/zlib/include -L$CMS_PATH/$SCRAM_ARCH/external/zlib/lib -lz test.cc
     which results in the default executable './a.out'.

   LXPLUS, CMSSW environment, compilation with SCRAM
   -------------------------------------------------
   - Files needed in code directory (e.g. $CMSSW_BASE/src/[SUBSYSTEM]/[PACKAGE]/bin/):
     test.cc                         # the actual code, using this class
     BuildFile.xml                   #
   - Files needed in current directory:
     events.txt.gz                   # gzipped input file
                                     # e.g. by
                                     # cp $CMSSW_RELEASE_BASE/src/EventFilter/HcalRawToDigi/data/HCALLaser2012AllDatasets.txt.gz ./events.txt.gz
   - In test.cc, you have
       #include "PhysicsTools/Utilities/interface/EventFilterFromListStandAlone.h"
       [...]
       EventFilterFromListStandAlone myFilter("./events.txt.gz");
       [...]
       bool myFlag = myFilter.filter(run_number, lumi_sec_number, event_number)
   - In BuildFile.xml, you have:
       <use name="zlib"/>
       <use name="PhysicsTools/Utilities"/>
       [...]
       <environment>
        [...]
        <bin file="test.cc"></bin>
        [...]
       </environment>
   - To compile:
       scram b # evtl. followed by 'rehash' (for csh) to make the executable available
     which results in the executable '$CMSSW_BASE/bin/$SCRAM_ARCH/test'.

  \author   Thomas Speer
  \version  $Id: EventFilterFromListStandAlone.h,v 1.4 2013/01/16 17:19:24 vadler Exp $
*/

#include <vector>
#include <string.h>
#include <iostream>
#include <algorithm>
#include <sstream>
#include "zlib.h"
#include <stdio.h>

class EventFilterFromListStandAlone  {
public:
  /**
   * Constructor
   * eventFileName: The gzipped file with the list of events
   */

  EventFilterFromListStandAlone(const std::string & eventFileName);

  ~EventFilterFromListStandAlone() {}

  /**
   * The filter, returns true for good events, bad for events which are
   * to be filtered out, i.e. events that are in the list
   */
  bool filter(int run, int lumiSection, int event);

private:

  void readEventListFile(const std::string & eventFileName);
  void addEventString(const std::string & eventString);

  typedef std::vector< std::string > strVec;
  typedef std::vector< std::string >::iterator strVecI;

  std::vector< std::string > EventList_;  // vector of strings representing bad events, with each string in "run:LS:event" format
  bool verbose_;  // if set to true, then the run:LS:event for any event failing the cut will be printed out

  // Set run range of events in the BAD event LIST.
  // The purpose of these values is to shorten the length of the EventList_ vector when running on only a subset of data
  int minrun_;
  int maxrun_;  // if specified (i.e., values > -1), then only events in the given range will be filtered
  int minRunInFile, maxRunInFile;

};

#endif
