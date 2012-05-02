/**********************************************************************************
 * @Project: SFrame - ROOT-based analysis framework
 * @Package: SFrame
 * @Class  : LuminosityUtils
 *           
 * @brief  : This is a collection of classes which are used 
 *           by the LuminosityHandler.
 *
 * @author : Martin Goebel (martin.goebel@desy.de)
 *
**********************************************************************************/

#ifndef SFRAME_LuminosityUtils_H
#define SFRAME_LuminosityUtils_H

// STL include(s):
#include <iostream>

namespace LuminosityUtils {

   // class merged run Nr and LB Nr
   class RunNr_LbNr{
   public:
      RunNr_LbNr() {
         runNr = 0; 
         lbNr  = 0;
      }
      RunNr_LbNr( const RunNr_LbNr& obj ) {
         runNr = obj.runNr;
         lbNr  = obj.lbNr;
      }
      RunNr_LbNr(long rNr, long lNr) {
         runNr = rNr; 
         lbNr  = lNr;
      }
      ~RunNr_LbNr(){}
      
      long runNr;
      long lbNr;
     
      // overloading operators (inline implementation)
      RunNr_LbNr& operator = ( const RunNr_LbNr& obj ) {
         runNr = obj.runNr;
         lbNr = obj.lbNr;
         return *this;
      }
      friend bool operator== ( const RunNr_LbNr lhs, const RunNr_LbNr rhs ) { 
         return ( lhs.runNr == rhs.runNr && lhs.lbNr == rhs.lbNr );
      }
      friend bool operator< ( const RunNr_LbNr lhs, const RunNr_LbNr rhs ) { 
         return ( lhs.runNr < rhs.runNr ) || ( lhs.runNr == rhs.runNr&& lhs.lbNr < rhs.lbNr );
      }
      friend bool operator<= ( const RunNr_LbNr lhs, const RunNr_LbNr rhs ) { 
         return ( lhs.runNr < rhs.runNr ) || ( lhs.runNr == rhs.runNr&& lhs.lbNr <= rhs.lbNr );
      }
      friend bool operator> ( const RunNr_LbNr lhs, const RunNr_LbNr rhs ) { 
         return ( lhs.runNr > rhs.runNr ) || ( lhs.runNr == rhs.runNr&& lhs.lbNr > rhs.lbNr );
      }
      friend bool operator>= ( const RunNr_LbNr lhs, const RunNr_LbNr rhs ) { 
         return ( lhs.runNr > rhs.runNr ) || ( lhs.runNr == rhs.runNr&& lhs.lbNr >= rhs.lbNr );
      }
      friend bool operator!= ( const RunNr_LbNr lhs, const RunNr_LbNr rhs ) { 
         return !(lhs == rhs);
      }
      friend ostream& operator<< (ostream& os, RunNr_LbNr& obj){
         os << "(Run Nr, LB Nr) = (" << obj.runNr << ", " << obj.lbNr << ")";
         return os;
      }
   }; // end RunNr_LbNr class


   // class containing LumiBin infos
   class LumiBinInfo{
   public:
      LumiBinInfo(){}
      ~LumiBinInfo(){}
      
      RunNr_LbNr start_runNrLbNr;
      RunNr_LbNr end_runNrLbNr;
      Double_t lumiInBin;
   }; // end LumiBinInfo class

};
      

#endif // SFRAME_LuminosityUtils_H

