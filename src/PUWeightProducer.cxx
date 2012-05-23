#include "../include/PUWeightProducer.h"


PUWeightProducer::PUWeightProducer(TString filename_mc, TString filename_data, TString histname_mc, TString histname_data){

  TFile *file_mc = new TFile(filename_mc);
  TFile *file_data =  new TFile(filename_data);

  h_npu_mc=(TH1F*) file_mc->Get(histname_mc);
  h_npu_data=(TH1F*) file_data->Get(histname_data);


  if(h_npu_mc->GetNbinsX() != h_npu_data->GetNbinsX()){
    std::cerr << "ERROR: pile-up histograms for data and MC have different numbers of bins" <<std::endl;
    exit(-1);
  }
  if( (h_npu_mc->GetXaxis()->GetXmax() != h_npu_data->GetXaxis()->GetXmax()) || (h_npu_mc->GetXaxis()->GetXmin() != h_npu_data->GetXaxis()->GetXmin())){
    std::cerr << "ERROR: pile-up histograms for data and MC have different axis ranges" <<std::endl;
    exit(-1);
  }

  h_npu_mc->Scale(1./h_npu_mc->Integral());
  h_npu_data->Scale(1./h_npu_data->Integral());


}

double PUWeightProducer::produceWeight(GenInfo* genInfo){

  double weight =0;

  int binnumber = h_npu_mc->GetXaxis()->FindBin(genInfo->pileup_TrueNumInteractions());
  
  if(h_npu_data->GetBinContent(binnumber)!=0){

    weight = h_npu_mc->GetBinContent(binnumber)/h_npu_data->GetBinContent(binnumber);

  }

  return weight;
}
