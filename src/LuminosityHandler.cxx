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
#include <cstdlib>

// Local include(s):
#include "include/LuminosityHandler.h"
//#include "GoodRunsLists/include/TGoodRunsListReader.h"

ClassImp(LuminosityHandler)

// constructors
LuminosityHandler::LuminosityHandler( string name ) :
   m_logger ( name.c_str() )
{
   TNamed::SetName( name.c_str() );
   
   //m_grlName = "";
   m_grlPath = "";
   m_lumiFileName = "";
   m_triggerName = "";
   
   m_intLumiPerBin = 0;
   m_targetLumi = 0;

   m_NBins = 0;

   m_isLumiCalc    = false;
   m_lumiTxtFirst  = true;
   m_targetLumi    = 0;
   m_mapLbNr2IntLumi.clear();
   m_mapLbNr2InstLumi.clear();
   m_mapLbNr2LumiBin.clear();
   m_mapLumiBin2Info.clear();
   m_mapIntLumiRunNr.clear();
   m_lumiHist = NULL;
}

// destructor
LuminosityHandler::~LuminosityHandler()
{
   m_mapLbNr2IntLumi.clear();
   m_mapLbNr2InstLumi.clear();
   m_mapLbNr2LumiBin.clear();
   m_mapLumiBin2Info.clear();
   m_mapIntLumiRunNr.clear();
}

bool LuminosityHandler::Initialise()
{
   // retrieve sframe main directory
  TString grl_dir = /*(TString)std::getenv( "SFRAME_DIR" ) + "/" +*/ m_grlPath + "/";
 
   // retrieve lumi and GoodRunsList information
//    if( !m_grlName.empty() ) {
//       m_logger << INFO << "Using GoodRunsList \"" << m_grlName << "\"!" << SLogger::endmsg;
//       Root::TGoodRunsListReader foo;
//       foo.Reset();
//       TString list = m_grlName.c_str();
//       while( list != "" ){
//          int pos = list.Last(',');
//          TString grl = grl_dir;
//          for( int i = pos+1; i < list.Length(); i++){
//             if( list[i] != ' ')
//                grl.Append( list[i] );
//          }
//          list.Remove( (pos==-1 ? 0 : pos), list.Length()-pos );
//          m_logger << INFO << "Reading good-runs-list \"" << grl << "\"!" << SLogger::endmsg;
//          foo.AddXMLFile( grl );
//       }
//       if( !foo.Interpret() ){
//          m_logger << FATAL << "Problems in interpreting good-runs-lists. Please check if all files are available!" << SLogger::endmsg;
//          exit(-1);
//       }
//       m_grl = foo.GetMergedGoodRunsList();
//    } 

   if( !m_lumiFileName.empty() ){

//      TFile *testfile =  new TFile( "/afs/naf.desy.de/user/p/peiffer/CMSSW_5_2_3_patch4/src/UHHAnalysis/NtupleWriter/test.root", "READ");

//      TTree *testTr = (TTree*)testfile->Get( "AnalysisTree" );

//      TString* testString;
//      testTr->SetBranchAddress( "HLTpath", &testString);

//       for( Int_t ientry = 0; ientry<testTr->GetEntries(); ientry++){
//          testTr->GetEntry( ientry );
// 	 std::cout << testString->Data() << std::endl;
//       }
//       testfile->Close();

      TFile *lumifile = new TFile( grl_dir + m_lumiFileName.c_str(), "READ");
      if( lumifile == NULL ){
         m_logger << FATAL << "Lumi File \"" << m_lumiFileName << "\" doesn't exist or cannot be opened!"
                  << SLogger::endmsg;
         exit(-1);
      }
      
      // retrieve Trigger Tree from file
      TTree *tree = (TTree*)lumifile->Get( "AnalysisTree" );
      if( tree == NULL){
         m_logger << FATAL << "Canot find AnalysisTree in Lumi File \"" << m_lumiFileName << "\"!" << SLogger::endmsg;
         exit(-1);
      }
         
      m_logger << INFO << "Using LumiFile \"" << m_lumiFileName 
               << "\", luminosity calculation with Trigger \"" << m_triggerName  << "\"!" << SLogger::endmsg;

      // retrieve branches
      Double_t intLumi, instLumi, l1ps, hltps;
      UInt_t lbNr, runNr;
      UInt_t runNr_help = 0;
      TString hltpath;
      TString* hltpath_pointer = new TString("");
      tree->SetBranchAddress( "luminosityBlock", &lbNr);
      tree->SetBranchAddress( "run", &runNr);
      tree->SetBranchAddress( "intgRecLumi", &intLumi ); 
      //tree->SetBranchAddress( "InstLumi", &instLumi );
      instLumi=0;//fehlt noch
      tree->SetBranchAddress( "L1presc", &l1ps );
      tree->SetBranchAddress( "HLTpresc", &hltps );
      tree->SetBranchAddress( "HLTpath", &hltpath_pointer);

      // compute target lumi and map it to RunNr_LbNr
      for( Int_t ientry = 0; ientry<tree->GetEntries(); ientry++){
         tree->GetEntry( ientry );
	 hltpath = hltpath_pointer->Data();
         //if ( m_grl.IsEmpty() || m_grl.HasRunLumiBlock( runNr, lbNr)) {
	 if(hltpath.BeginsWith(m_triggerName)){
	   // std::cout << runNr << " " << lbNr <<" " << intLumi << "  " << l1ps*hltps << "  trigger : " << hltpath << std::endl;
	    if( runNr_help != runNr && m_targetLumi != 0 ){
               m_mapIntLumiRunNr.insert( make_pair( m_targetLumi, runNr_help) );
            }
            runNr_help = runNr;
            RunNr_LbNr runlb( runNr, lbNr );
            // intLumi/1000: micro barn -> pico barn
            m_mapLbNr2IntLumi.insert( make_pair( runlb, intLumi*l1ps*hltps/1000000. ) );  
            m_mapLbNr2InstLumi.insert( make_pair( runlb, instLumi*l1ps*hltps ) ); // in 10e31 cm-2 s-1 = ub-1 s-1  
            if( l1ps != 1 ) cout << runlb << " --> " << l1ps << endl; 
            m_mapLbNr2L1PS.insert( make_pair( runlb, l1ps ) );
            m_mapLbNr2HLTPS.insert( make_pair( runlb, hltps ) );
            m_targetLumi += intLumi/1000000.;
            if( l1ps*hltps != 1 )
               m_logger <<  WARNING << hltpath << Form(" is prescaled (ps=%i) in ", int(l1ps*hltps) ) 
                        << runlb << "!" << SLogger::endmsg;
	 }
	 else{
	   //m_logger <<  FATAL << "Trigger not found: " <<  m_triggerName << " in LumiFile!" << SLogger::endmsg;
	 }
      }

      m_mapIntLumiRunNr.insert( make_pair( m_targetLumi, runNr_help) );
      lumifile->Close();
     
      m_logger << INFO << m_targetLumi << " (pb-1) found in luminosity file: " << m_lumiFileName << SLogger::endmsg;
      
      // divide target lumi in equally large lumi bins
      // store LumiBinInfo for every LumiBin
      Int_t nbin = 1;
      RunNr_LbNr start, end;
      Double_t lumiPerBin = 0;
      bool first = true;
      std::map<RunNr_LbNr,double>::iterator it; 
      for( it = m_mapLbNr2IntLumi.begin(); it !=  m_mapLbNr2IntLumi.end(); ++it ){
         if( first ) { start = it->first; first = false; }
         // map nbin to RunNr_LbNr
         m_mapLbNr2LumiBin.insert( make_pair( it->first, nbin ) );
         lumiPerBin += it->second;
         if( lumiPerBin >= m_intLumiPerBin ) {
            LumiBinInfo *info = new LumiBinInfo();
            info->start_runNrLbNr = start;
            info->end_runNrLbNr   = it->first;
            info->lumiInBin = lumiPerBin;
            m_mapLumiBin2Info[nbin] = info;
            lumiPerBin = 0; nbin++;
            first = true;
         }
         end = it->first;
      }
      // fill the remaining lumi in last bin
      LumiBinInfo *info = new LumiBinInfo();
      info->start_runNrLbNr = start;
      info->end_runNrLbNr = end;
      info->lumiInBin = lumiPerBin;
      m_mapLumiBin2Info[nbin] = info;
      m_NBins = nbin;

      // store general luminosity plot
      if( !m_lumiHist ){
         Int_t nbins = this->GetNLumiBins();
         m_lumiHist = new TH1F( "IntLumiPerLumiBin", "Integrated lumi per LumiBin", nbins, 0.5, nbins+0.5 );
         for( int i = 1; i <= nbins; i++ ){
            m_lumiHist->SetBinContent( i, this->GetIntLumiInBin( i ) );
            m_lumiHist->SetBinError( i, 0 );
         }
         m_logger << INFO << " <LuminosityHandler> IntLumiPerLumiBin histogram was filled!" << SLogger::endmsg;
         m_logger << INFO << "                    " << m_lumiHist->GetNbinsX() << " Lumi Bins in histogram!" << SLogger::endmsg;
         m_logger << INFO << "                    " << m_lumiHist->Integral() 
                  << " pb-1 integrated luminosity in histogram!" << SLogger::endmsg;
      }

      m_isLumiCalc = true;
   } 
   else if( m_lumiFileName.empty() /*&& !m_grl.IsEmpty()*/ ) {
      if( !m_lumiHist ){
         m_lumiHist = new TH1F( "IntLumiPerLumiBin", "Integrated lumi per LumiBin", 1, 0.5, 1.5 );
         m_logger << INFO << " <GetLumiPerLumiBinHist> IntLumiPerLumiBin histogram is empty!" << SLogger::endmsg;
      }
      m_logger << WARNING << "No Lumi File specified ==> No lumi calculation" << SLogger::endmsg;
      m_isLumiCalc = false;
   }
//    else if( m_lumiFileName.empty() && m_grl.IsEmpty() ) {
//       if( !m_lumiHist ){
//          m_lumiHist = new TH1F( "IntLumiPerLumiBin", "Integrated lumi per LumiBin", 1, 0.5, 1.5 );
//          m_logger << INFO << " <GetLumiPerLumiBinHist> IntLumiPerLumiBin histogram is empty!" << SLogger::endmsg;
//       }
//       m_logger << WARNING << "Neither Lumi nor GoodRunsList File specified ==> No lumi calculation and no GoodRunsList!" 
//                << SLogger::endmsg;
//       m_isLumiCalc = false;
//    } 
   
   return m_isLumiCalc;
}

void LuminosityHandler::PrintUsedSetup()
{
//    if( !m_grlName.empty() ) {
//       m_logger << INFO << "Used GoodRunsList \"" << m_grlName << "\"!" << SLogger::endmsg;
//    }

   if( !m_lumiFileName.empty() ) {
      m_logger << INFO << "Used LumiFile \"" << m_lumiFileName 
               << "\", luminosity calculation with Trigger \"" << m_triggerName  << "\"!" << SLogger::endmsg;
   }
   //if( m_lumiFileName.empty() && m_grl.IsEmpty() ) {
   else{
      m_logger << WARNING << "No Lumi File specified ==> No lumi calculation and no GoodRunsList!" 
               << SLogger::endmsg;
   }
}

// ---------------------------- implementation of accessor methods ----------------------------

bool LuminosityHandler::PassGoodRunsList( UInt_t runNr, UInt_t lbNr )
{
   bool ret = false;
   
   if( !m_mapLbNr2IntLumi.empty() ){
      RunNr_LbNr res( runNr, lbNr );
      std::map<RunNr_LbNr,double>::iterator it = m_mapLbNr2IntLumi.find(res);
      if( it !=  m_mapLbNr2IntLumi.end() )
         ret = true;
   }
//    else if( !m_grl.IsEmpty() )
//       ret = m_grl.HasRunLumiBlock(runNr, lbNr); 
//    else if( m_grl.IsEmpty() )
//       ret = true;
   
   return ret;
}

int LuminosityHandler::GetLumiBin( UInt_t runNr, UInt_t lbNr )
{   
  //m_logger << FATAL << "<GetLumiBin> " <<  SLogger::endmsg;
   
   int ret = -1;
   RunNr_LbNr res( runNr, lbNr );
   std::map<RunNr_LbNr,int>::iterator it = m_mapLbNr2LumiBin.find( res );
   if( it !=  m_mapLbNr2LumiBin.end() ){
      ret = it->second;
   }
   else{
      m_logger << DEBUG << "<GetLumiBin> " << res
               << "\" not associated with LumiBin!" << SLogger::endmsg;
   }
   return ret;
}

double LuminosityHandler::GetIntLumiInBin( int bin )
{
   double ret = -1;
   std::map<int,LumiBinInfo*>::iterator it = m_mapLumiBin2Info.find( bin );
   if( it != m_mapLumiBin2Info.end() ){
      ret = (it->second)->lumiInBin;
   }
   else{
      m_logger << FATAL << "<GetIntLumiInBin> LumiBin \"" << bin << "\" out of range "
               << Form("[0,%i]!", m_NBins) << SLogger::endmsg;
   }
   return ret;
}

RunNr_LbNr LuminosityHandler::GetStartRunNrLbNr( int bin )
{
   RunNr_LbNr ret(0,0);
   std::map<int,LumiBinInfo*>::iterator it = m_mapLumiBin2Info.find( bin );
   if( it != m_mapLumiBin2Info.end() ){
      ret = (it->second)->start_runNrLbNr;
   }
   else{
      m_logger << FATAL << "<GetStartRunNrLbNr> LumiBin \"" << bin << "\" out of range "
               << Form("[0,%i]!", m_NBins) << SLogger::endmsg;
   }
   return ret;
}

RunNr_LbNr LuminosityHandler::GetEndRunNrLbNr( int bin )
{
   RunNr_LbNr ret(0,0);
   std::map<int,LumiBinInfo*>::iterator it = m_mapLumiBin2Info.find( bin );
   if( it != m_mapLumiBin2Info.end() ){
      ret = (it->second)->end_runNrLbNr;
   }
   else{
      m_logger << FATAL << "<GetEndRunNrLbNr> LumiBin \"" << bin << "\" out of range "
               << Form("[0,%i]!", m_NBins) << SLogger::endmsg;
   }
   return ret;
}

double LuminosityHandler::GetIntLumiInLb( UInt_t runNr, UInt_t lbNr )
{
   double ret = -1;
   
   RunNr_LbNr res( runNr, lbNr );
   std::map<RunNr_LbNr,double>::iterator it = m_mapLbNr2IntLumi.find( res );
   if( it !=  m_mapLbNr2IntLumi.end() ){
      ret = it->second;
   }
   else{
      m_logger << FATAL << "<GetIntLumiInLb> " << res
               << "\" are not found!" << SLogger::endmsg;
   }
      
   return ret;
}

double LuminosityHandler::GetInstLumiPerLb( UInt_t runNr, UInt_t lbNr )
{
   double ret = -1;
   
   RunNr_LbNr res( runNr, lbNr );
   std::map<RunNr_LbNr,double>::iterator it = m_mapLbNr2InstLumi.find( res );
   if( it !=  m_mapLbNr2InstLumi.end() ){
      ret = it->second;
   }
   else{
      m_logger << FATAL << "<GetInstLumiPerLb> " << res
               << "\" are not found!" << SLogger::endmsg;
   }
   
   return ret;
}

double LuminosityHandler::GetL1Prescale( UInt_t runNr, UInt_t lbNr )
{
   double ret = -1;
   
   RunNr_LbNr res( runNr, lbNr );
   std::map<RunNr_LbNr,double>::iterator it = m_mapLbNr2InstLumi.find( res );
   if( it !=  m_mapLbNr2L1PS.end() ){
      ret = it->second;
   }
   else{
      m_logger << FATAL << "<GetL1Prescale> " << res
               << "\" are not found!" << SLogger::endmsg;
   }
   
   return ret;
}

double LuminosityHandler::GetHLTPrescale( UInt_t runNr, UInt_t lbNr )
{
   double ret = -1;
   
   RunNr_LbNr res( runNr, lbNr );
   std::map<RunNr_LbNr,double>::iterator it = m_mapLbNr2InstLumi.find( res );
   if( it !=  m_mapLbNr2HLTPS.end() ){
      ret = it->second;
   }
   else{
      m_logger << FATAL << "<GetEFPrescale> " << res
               << "\" are not found!" << SLogger::endmsg;
   }
   
   return ret;
}


double LuminosityHandler::GetIntLumiInRange( UInt_t runNr1, UInt_t runNr2 )
{
   double ret = 0;
   
   if( runNr2 < runNr1 ){
      m_logger << FATAL << "<GetIntLumiInRange> First runNr \"" << runNr1 
               << "\" must be larger than (or equal) second runNr \"" << runNr2 << "\"!" << SLogger::endmsg; 
   }
   std::map<RunNr_LbNr,double>::iterator it; 
   for( it = m_mapLbNr2IntLumi.begin(); it !=  m_mapLbNr2IntLumi.end(); ++it ){
      if( (it->first).runNr >= runNr1 && (it->first).runNr <= runNr2 ) 
         ret += it->second;
   }

   return ret;
}

// get histogram: integrated luminosity vs. Nr. of Luminosity Bin
TH1F* LuminosityHandler::GetLumiPerLumiBinHist()
{
   return m_lumiHist;
}

void LuminosityHandler::DumpLumiInfoIntoTxtFile()
{
   // plot general Luminosity plot only once
   if( this->IsLumiCalc() && m_lumiTxtFirst ){
      std::ofstream txtFile( "LuminosityDump.txt" );
      txtFile << std::setw(20) << " LumiBin Nr. |" << setw(30) << " Start Run / LB Nr.|" << setw(30) 
              << " End Run / LB Nr. |" << setw(20) << " int Lumi [pb-1] |" << endl; 
      Int_t nbins = this->GetNLumiBins(); 
      for( int i = 1; i <= nbins; i++ ){
         txtFile << setw(20) << Form( " %i |", i) 
                 << setw(30) << Form( " %li / %li |", GetStartRunNrLbNr(i).runNr, GetStartRunNrLbNr(i).lbNr) 
                 << setw(30) << Form( " %li / %li |", GetEndRunNrLbNr(i).runNr, GetEndRunNrLbNr(i).lbNr) 
                 << setw(20) << Form( " %3.8f | ", GetIntLumiInBin( i ) ) << endl; 
      }

      m_lumiTxtFirst = false; 
      txtFile.close();  
   }
}

UInt_t LuminosityHandler::GetRandomRunNr()
{
   Double_t lumi = gRandom->Rndm()*m_targetLumi;
   UInt_t runNr = 0;

   std::map< double, UInt_t>::iterator it; 
   for( it = m_mapIntLumiRunNr.begin(); it != m_mapIntLumiRunNr.end(); ++it ){
      if( it->first <= lumi )
         runNr = it->second;
      else
         return runNr;
   }
   return runNr;
}

TTree *LuminosityHandler::GetTreeLuminosityPerRun()
{
   TTree *tree = new TTree( "LumiPerRun", "luminosity per run" );

   long   runNr = 0;
   double lumi  = 0; 
   double total = 0; 

   tree->Branch( "run", &runNr, "runNr/l" );
   tree->Branch( "intgRecLumi", &lumi, "lumi/D" );
   
   std::map< double, UInt_t>::iterator it; 
   for( it = m_mapIntLumiRunNr.begin(); it != m_mapIntLumiRunNr.end(); ++it ){
      runNr = it->second;
      lumi  = it->first - total;
      tree->Fill();
      total = it->first;
   }
   return tree;
}

