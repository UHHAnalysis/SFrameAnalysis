SFrameAnalysis
==============

SFrameAnalysis is a library for sframe that defines the cycle/output types needed for an analysis. In order to utilize
  any cycles/classes from this library it must first be included in your cycle configuration file. Examples can be found
  in the 'config' folder.

* config/
  * Folder containing sample (good) configuration xml files. Note the proper imported librarys/packages and the
  user configuration settings.
  
* include/
  * Folder containing class definition files for the cycles as well as Analysis_LinkDef.h, which must be updated
  whenever a new cycle/class is defined/added
  
* proof/
  * Folder containing implementation files for ROOT/proof
  
* src/
  * Folder containing class implementation files for the cycles/classes in this library
  
* Makefile
  * Makefile for this library
  
* MakeAnalysis.sh
  * Shell script to help with quick creation of custom cycles based on src/AnalysisCycle

* ChangeLog
  * Logfile for updates/changes

* README.md
  * This file.
