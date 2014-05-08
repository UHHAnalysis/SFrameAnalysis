// $Id: PDFWeightsCycle.cxx,v 1.1 2013/02/01 12:39:01 peiffer Exp $

// Local include(s):
#include "include/PDFWeightsCycle.h"

ClassImp( PDFWeightsCycle );


PDFWeightsCycle::PDFWeightsCycle()
   : AnalysisCycle() {

  //SetLogName( GetName() );
  SetIntLumiPerBin(500.); 

  //DeclareProperty( "PDFName", m_pdfname );
}

PDFWeightsCycle::~PDFWeightsCycle() {

}

void PDFWeightsCycle::BeginCycle() throw( SError ) {
  AnalysisCycle::BeginCycle();
  return;

}

void PDFWeightsCycle::EndCycle() throw( SError ) {
  AnalysisCycle::EndCycle();
   return;

}

void PDFWeightsCycle::BeginInputData( const SInputData& id ) throw( SError ) {
  AnalysisCycle::BeginInputData(id);
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
  AnalysisCycle::EndInputData(inputData);

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

void PDFWeightsCycle::BeginInputFile( const SInputData& sd) throw( SError ) {
  AnalysisCycle::BeginInputFile(sd);
//   bcc.reset();
//   ConnectVariable( "AnalysisTree", "genInfo" , bcc.genInfo);

//   EventCalc* calc = EventCalc::Instance();
//   calc->SetBaseCycleContainer(&bcc);
//   LuminosityHandler *lumiHandler = new LuminosityHandler();
//   calc->SetLumiHandler( lumiHandler );
//   return;

}

void PDFWeightsCycle::ExecuteEvent( const SInputData& sd, Double_t weight) throw( SError ) {
 
  AnalysisCycle::ExecuteEvent(sd, weight);
  EventCalc* calc = EventCalc::Instance();
  calc->Reset();

  int id1 = calc->GetGenInfo()->pdf_id1();
  int id2 = calc->GetGenInfo()->pdf_id2();

  m_Ntotal++;

  std::vector<double> weights = m_pdfweights->GetWeightList();

  for(unsigned int i=0; i< m_pdfweights->GetNWeights(); ++i){
    m_sumofweights[i]+=weights[i];
  }

  return;

}

