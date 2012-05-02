/**********************************************************************************
 * @Project: SFrame - ROOT-based analysis framework
 * @Package: SFrame
 * @Class  : LuminosityHandler
 *           
 * @brief  : This class contains all luminosity information and 
 *           good-runs-list evaluations. The luminosity from data are 
 *           computed via an input luminosity file and xml good-runs-list.
 *
 * @author : Martin Goebel (martin.goebel@desy.de)
 *
**********************************************************************************/

#ifndef SFRAME_LuminosityHandler_H
#define SFRAME_LuminosityHandler_H

// STL include(s):
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <map>

// ROOT include(s):
#include "TNamed.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TRandom2.h"

// Local include(s)
#include "core/include/SLogger.h"
#include "LuminosityUtils.h"
//#include "GoodRunsLists/include/TGoodRunsList.h"

using namespace LuminosityUtils;

using std::string;
using std::map;
using std::make_pair;
using std::cout;
using std::endl;
using std::setw;

class LuminosityHandler : public TNamed {

public:
   
   LuminosityHandler( string name = "LuminosityHandler" );
   ~LuminosityHandler();

   // Initialise
   bool Initialise();

   // Setter
   //void SetGoodRunsList  ( string grlName )      { m_grlName       = grlName; }
   void SetGRLPath       ( string grlPath )      { m_grlPath       = grlPath; }
   void SetLumiFileName  ( string lumiFileName ) { m_lumiFileName  = lumiFileName; }
   void SetTrigger       ( string trigger )      { m_triggerName   = trigger; }
   void SetIntLumiPerBin ( float intLumiPerBin ) { m_intLumiPerBin = intLumiPerBin; } // in (pb)^-1

   // Getter
   //string     GetGoodRunsLists ( ) { return m_grlName; }
   //Root::TGoodRunsList*   GetTGoodRunsLists ( ) { return &m_grl; }
   string     GetLumiFile      ( ) { return m_lumiFileName; }
   double     GetTargetLumi    ( ) { return m_targetLumi; } // in (pb)^-1
   int        GetNLumiBins     ( ) { return m_NBins; } 
   int        GetLumiBin       ( UInt_t runNr, UInt_t lbNr );
   double     GetIntLumiInBin  ( int bin );
   RunNr_LbNr GetStartRunNrLbNr( int bin );
   RunNr_LbNr GetEndRunNrLbNr  ( int bin );
   double     GetIntLumiInLb   ( UInt_t runNr, UInt_t lbNr ); 
   double     GetInstLumiPerLb ( UInt_t runNr, UInt_t lbNr ); // in 1e31 cm-2 s-1 = ub-1 s-1  
   double     GetIntLumiInRange( UInt_t runNr1, UInt_t runNr2 );
   double     GetL1Prescale( UInt_t runNr, UInt_t lbNr );
   double     GetHLTPrescale( UInt_t runNr, UInt_t lbNr );

   TH1F* GetLumiPerLumiBinHist();
   void DumpLumiInfoIntoTxtFile();
   TTree* GetTreeLuminosityPerRun();
   UInt_t GetRandomRunNr();

   // does lumiblock pass good runs list?
   bool PassGoodRunsList( UInt_t runNr, UInt_t lbNr );
   
   bool IsLumiCalc() { return m_isLumiCalc; }

   void PrintUsedSetup();

private:

   // logger 
   mutable SLogger m_logger;

   // is lumi information available
   bool m_isLumiCalc;
  
   // general Luminosity plot
   TH1F *m_lumiHist;

   // dump to txt file only once
   bool m_lumiTxtFirst;

   // member variables
   map< RunNr_LbNr, double >  m_mapLbNr2IntLumi;
   map< RunNr_LbNr, double >  m_mapLbNr2InstLumi;
   map< RunNr_LbNr, int >     m_mapLbNr2LumiBin;
   map< int, LumiBinInfo* >   m_mapLumiBin2Info;
   map< double, UInt_t >      m_mapIntLumiRunNr;
   map< RunNr_LbNr, double >  m_mapLbNr2L1PS;
   map< RunNr_LbNr, double >  m_mapLbNr2HLTPS;
   
   //string m_grlName;
   string m_grlPath;
   string m_lumiFileName;
   string m_triggerName;

   double m_intLumiPerBin;
   double m_targetLumi;

   int m_NBins;

   // pointer to GoodRunsList
   //Root::TGoodRunsList m_grl;

   ClassDef(LuminosityHandler,1) 
}; 
      

#endif // SFRAME_LuminosityHandler_H

