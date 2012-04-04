// $Id: SETUP.C,v 1.1 2012/04/02 15:28:00 peiffer Exp $

int SETUP() {

   /// Add all the additional libraries here that this package
   /// depends on. (With the same command that loads this package's library...)
   if( gSystem->Load( "libTree" ) == -1 ) return -1;
   if( gSystem->Load( "libHist" ) == -1 ) return -1;
   if( gSystem->Load( "libGraf" ) == -1 ) return -1;
   if( gSystem->Load( "libAnalysis" ) == -1 ) return -1;
   return 0;
}
