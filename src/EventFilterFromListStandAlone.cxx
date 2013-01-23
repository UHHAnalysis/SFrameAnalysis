#include "include/EventFilterFromListStandAlone.h"

using namespace std;

EventFilterFromListStandAlone::EventFilterFromListStandAlone(const std::string & eventFileName)
{
  verbose_=false;
  minRunInFile=999999; maxRunInFile=1;
  minrun_ = 0;  maxrun_ = 999999;
  if (verbose_) cout << "Event list from file "<<eventFileName<<endl;
  readEventListFile(eventFileName);
  std::sort(EventList_.begin(), EventList_.end());
  if (verbose_) cout<<" A total of "<<EventList_.size()<<" listed events found in given run range"<<endl;
  minrun_=minRunInFile;
  maxrun_=maxRunInFile;
}

void EventFilterFromListStandAlone::addEventString(const string & eventString)
{
  int run=0;
  unsigned int ls=0;
  unsigned int event=0;
  // Check that event list object is in correct form
  size_t found = eventString.find(":");  // find first colon
  if (found!=std::string::npos)
    run=atoi((eventString.substr(0,found)).c_str());  // convert to run
  else
    {
      cout<<"  Unable to parse Event list input '"<<eventString<<"' for run number!\n";
      return;
    }
  size_t found2 = eventString.find(":",found+1);  // find second colon
  if (found2!=std::string::npos)
    {
      /// Some event numbers are less than 0?  \JetHT\Run2012C-v1\RAW:201278:2145:-2130281065  -- due to events being dumped out as ints, not uints!
      ls=atoi((eventString.substr(found+1,(found2-found-1))).c_str());  // convert to ls
      event=atoi((eventString.substr(found2+1)).c_str()); // convert to event
      /// Some event numbers are less than 0?  \JetHT\Run2012C-v1\RAW:201278:2145:-2130281065
      if (ls==0 || event==0) cout<<"  Strange lumi, event numbers for input '"<<eventString<<"'";
    }
  else
    {
      cout<<"Unable to parse Event list input '"<<eventString<<"' for run number!\n";
      return;
    }
  // If necessary, check that run is within allowed range
  if (minrun_>-1 && run<minrun_)
    {
      if (verbose_)  cout <<"Skipping Event list input '"<<eventString<<"' because it is less than minimum run # "<<minrun_<<endl;
      return;
    }
  if (maxrun_>-1 && run>maxrun_)
    {
      if (verbose_) cout <<"Skipping Event list input '"<<eventString<<"' because it is greater than maximum run # "<<maxrun_<<endl;
      return;
    }
  if (minRunInFile>run) minRunInFile=run;
  if (maxRunInFile<run) maxRunInFile=run;
  // Now add event to Event List
  EventList_.push_back(eventString);
}

#define LENGTH 0x2000

void EventFilterFromListStandAlone::readEventListFile(const string & eventFileName)
{
  gzFile  file = gzopen (eventFileName.c_str(), "r");
  if (! file) {
    cout<<"  Unable to open event list file "<<eventFileName<<endl;
    return;
  }
  string b2;
  int err;
  int bytes_read;
  char buffer[LENGTH];
  unsigned int i;
  char * pch;

  while (1) {
    bytes_read = gzread (file, buffer, LENGTH - 1);
    buffer[bytes_read] = '\0';
    i=0;
    pch = strtok (buffer,"\n");
    if (buffer[0] == '\n' ) {
      addEventString(b2);
      ++i;
    } else b2+=pch;

    while (pch != NULL)
    {
      i+=strlen(pch)+1;
      if (i>b2.size()) b2= pch;
      if (i==(LENGTH-1)) {
	 if ((buffer[LENGTH-2] == '\n' )|| (buffer[LENGTH-2] == '\0' )){
          addEventString(b2);
          b2="";
	}
      } else if (i<LENGTH) {
	addEventString(b2);
      }
      pch = strtok (NULL, "\n");
    }
    if (bytes_read < LENGTH - 1) {
      if (gzeof (file)) break;
        else {
          const char * error_string;
          error_string = gzerror (file, & err);
          if (err) {
	    cout<<"Error while reading gzipped file:  "<<error_string<<endl;
            return;
          }
        }
    }
  }
  gzclose (file);
  return;
}

bool EventFilterFromListStandAlone::filter(int run, int lumiSection, int event)
{
  // if run is outside filter range, then always return true
  if (minrun_>-1 && run<minrun_) return true;
  if (maxrun_>-1 && run>maxrun_) return true;

  // Okay, now create a string object for this run:ls:event
  std::stringstream thisevent;
  thisevent<<run<<":"<<lumiSection<<":"<<event;

  // Event not found in bad list; it is a good event
  strVecI it = std::lower_bound(EventList_.begin(), EventList_.end(), thisevent.str());
  if (it == EventList_.end() || thisevent.str() < *it) return true;
  // Otherwise, this is a bad event
  // if verbose, dump out event info
  // Dump out via cout, or via LogInfo?  For now, use cout
  if (verbose_) std::cout <<"EventFilterFromListStandAlone removed "<<thisevent.str()<<std::endl;

  return false;
}
