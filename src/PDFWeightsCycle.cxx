// $Id: PDFWeightsCycle.cxx,v 1.1 2013/02/01 12:39:01 peiffer Exp $

// Local include(s):
#include "include/PDFWeightsCycle.h"

ClassImp( PDFWeightsCycle );


PDFWeightsCycle::PDFWeightsCycle()
   : SCycleBase() {

   SetLogName( GetName() );

   DeclareProperty( "PDFName", m_pdfname );
}

PDFWeightsCycle::~PDFWeightsCycle() {

}

void PDFWeightsCycle::BeginCycle() throw( SError ) {

  return;

}

void PDFWeightsCycle::EndCycle() throw( SError ) {

   return;

}

void PDFWeightsCycle::BeginInputData( const SInputData& ) throw( SError ) {
  

  //
  // Declare the output histograms:
  //

  //systeamtic shift e_Up not relevant here
  m_pdfweights = new PDFWeights(e_Up,m_pdfname);


  //set all counters to 0
  m_Ntotal=0;

  m_sumofweights.clear();
  for(unsigned int i=0; i< m_pdfweights->GetNWeights(); ++i){
    m_sumofweights.push_back(0);
  }

  return;

}

void PDFWeightsCycle::EndInputData( const SInputData& inputData ) throw( SError ) {

  TString outfilename = GetConfig().GetOutputDirectory();
  outfilename += "/";
  outfilename += inputData.GetVersion();
  outfilename +="_";
  outfilename += m_pdfname;
  outfilename += "_weights.txt";

  ofstream outfile;
  outfile.open (((string)outfilename).c_str());
  outfile << m_Ntotal <<std::endl;
  for(unsigned int i=0; i< m_pdfweights->GetNWeights(); ++i){
    outfile<< m_sumofweights[i] << " ";
  }
  outfile.close();
  return;

}

void PDFWeightsCycle::BeginInputFile( const SInputData& ) throw( SError ) {

  ConnectVariable( "AnalysisTree", "genInfo" , bcc.genInfo);

  EventCalc* calc = EventCalc::Instance();
  calc->SetBaseCycleContainer(&bcc);
  LuminosityHandler *lumiHandler = new LuminosityHandler();
  calc->SetLumiHandler( lumiHandler );
  return;

}

void PDFWeightsCycle::ExecuteEvent( const SInputData&, Double_t weight) throw( SError ) {
 
  EventCalc* calc = EventCalc::Instance();
  calc->Reset();
  
  m_Ntotal++;

  std::vector<double> weights = m_pdfweights->GetWeightList();

  for(unsigned int i=0; i< m_pdfweights->GetNWeights(); ++i){
    m_sumofweights[i]+=weights[i];
  }

  return;

}

