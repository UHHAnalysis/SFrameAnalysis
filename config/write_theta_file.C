void merge_processes()
{
  gSystem->Exec("mkdir not_used");
  gSystem->Exec("mv *QCD* not_used");
  gSystem->Exec("mv *RSG* not_used");
  gSystem->Exec("mv *DATA* not_used");
  gSystem->Exec("mv *NoShift* not_used");

  // ttbar
  gSystem->Exec("hadd -f ZprimePostSelectionCycle.MC.TTbarJECdown.root *.MC.TTbar_*JECdown*.root");
  gSystem->Exec("hadd -f ZprimePostSelectionCycle.MC.TTbarJERdown.root *.MC.TTbar_*JERdown*.root");
  gSystem->Exec("hadd -f ZprimePostSelectionCycle.MC.TTbarJECup.root *.MC.TTbar_*JECup*.root");
  gSystem->Exec("hadd -f ZprimePostSelectionCycle.MC.TTbarJERup.root *.MC.TTbar_*JERup*.root");
  gSystem->Exec("mv *.MC.TTbar_*.root not_used/.");

  // single top
  gSystem->Exec("hadd -f ZprimePostSelectionCycle.MC.SingleTopJECdown.root *.MC.SingleTop_*JECdown*.root");
  gSystem->Exec("hadd -f ZprimePostSelectionCycle.MC.SingleTopJERdown.root *.MC.SingleTop_*JERdown*.root");
  gSystem->Exec("hadd -f ZprimePostSelectionCycle.MC.SingleTopJECup.root *.MC.SingleTop_*JECup*.root");
  gSystem->Exec("hadd -f ZprimePostSelectionCycle.MC.SingleTopJERup.root *.MC.SingleTop_*JERup*.root");
  gSystem->Exec("mv *.MC.SingleTop_*.root not_used/.");

  // DY
  gSystem->Exec("hadd -f ZprimePostSelectionCycle.MC.DYJetsJECdown.root *.MC.DY?Jets_*JECdown*.root");
  gSystem->Exec("hadd -f ZprimePostSelectionCycle.MC.DYJetsJERdown.root *.MC.DY?Jets_*JERdown*.root");
  gSystem->Exec("hadd -f ZprimePostSelectionCycle.MC.DYJetsJECup.root *.MC.DY?Jets_*JECup*.root");
  gSystem->Exec("hadd -f ZprimePostSelectionCycle.MC.DYJetsJERup.root *.MC.DY?Jets_*JERup*.root");
  gSystem->Exec("mv *.MC.DY?Jets_*.root not_used/.");

  // W+jets
  gSystem->Exec("hadd -f ZprimePostSelectionCycle.MC.WbJECdown.root *.MC.W?Jets_bflavorJECdown*.root");
  gSystem->Exec("hadd -f ZprimePostSelectionCycle.MC.WbJERdown.root *.MC.W?Jets_bflavorJERdown*.root");
  gSystem->Exec("hadd -f ZprimePostSelectionCycle.MC.WbJECup.root *.MC.W?Jets_bflavorJECup*.root");
  gSystem->Exec("hadd -f ZprimePostSelectionCycle.MC.WbJERup.root *.MC.W?Jets_bflavorJERup*.root");

  gSystem->Exec("hadd -f ZprimePostSelectionCycle.MC.WcJECdown.root *.MC.W?Jets_cflavorJECdown*.root");
  gSystem->Exec("hadd -f ZprimePostSelectionCycle.MC.WcJERdown.root *.MC.W?Jets_cflavorJERdown*.root");
  gSystem->Exec("hadd -f ZprimePostSelectionCycle.MC.WcJECup.root *.MC.W?Jets_cflavorJECup*.root");
  gSystem->Exec("hadd -f ZprimePostSelectionCycle.MC.WcJERup.root *.MC.W?Jets_cflavorJERup*.root");

  gSystem->Exec("hadd -f ZprimePostSelectionCycle.MC.WlightJECdown.root *.MC.W?Jets_lflavorJECdown*.root");
  gSystem->Exec("hadd -f ZprimePostSelectionCycle.MC.WlightJERdown.root *.MC.W?Jets_lflavorJERdown*.root");
  gSystem->Exec("hadd -f ZprimePostSelectionCycle.MC.WlightJECup.root *.MC.W?Jets_lflavorJECup*.root");
  gSystem->Exec("hadd -f ZprimePostSelectionCycle.MC.WlightJERup.root *.MC.W?Jets_lflavorJERup*.root");

  gSystem->Exec("mv *.MC.W?Jets_*.root not_used/.");
  
  // diboson
  gSystem->Exec("hadd -f ZprimePostSelectionCycle.MC.DibosonJECdown.root *.MC.WW*JECdown.root *.MC.ZZ*JECdown.root");
  gSystem->Exec("hadd -f ZprimePostSelectionCycle.MC.DibosonJERdown.root *.MC.WW*JERdown.root *.MC.ZZ*JERdown.root");
  gSystem->Exec("hadd -f ZprimePostSelectionCycle.MC.DibosonJECup.root *.MC.WW*JECup.root *.MC.ZZ*JECup.root");
  gSystem->Exec("hadd -f ZprimePostSelectionCycle.MC.DibosonJERup.root *.MC.WW*JERup.root *.MC.ZZ*JERup.root");

  gSystem->Exec("mv *.MC.WW*.root not_used/.");
  gSystem->Exec("mv *.MC.ZZ*.root not_used/.");



}

void get_filenames(vector<TString> &names, const char *dirname=".", const char *ext=".root", const char *prefix = "Zprime")
{
  TSystemDirectory dir(dirname, dirname);
  TList *files = dir.GetListOfFiles();
  if (files) {
    TSystemFile *file;
    TString fname;
    TIter next(files);
    while ((file=(TSystemFile*)next())) {
      fname = file->GetName();
      if (!file->IsDirectory() && fname.EndsWith(ext) && fname.BeginsWith(prefix)) {
	names.push_back(TString(fname.Data()));
      }
    }
  }
}

TString get_sample_name(TString name)
{
  TObjArray* arr = name.Tokenize(".");
  
  TString out = ((TObjString*)arr->At(arr->GetEntries()-2))->GetString();
  out.ReplaceAll("JERup", "");
  out.ReplaceAll("JERdown", "");
  out.ReplaceAll("JECup", "");
  out.ReplaceAll("JECdown", "");
  out.ToLower();

  out.ReplaceAll("dyjets", "zlight");

  if (out.Contains("zp")){
    TString temp = out;
    temp.ReplaceAll("zp","");
    temp.ReplaceAll("p", ".");
    TObjArray* arr2 = temp.Tokenize("w");
    TString nom = ((TObjString*)arr2->At(1))->GetString();
    TString denom = ((TObjString*)arr2->At(0))->GetString();
    cout << "nom = " << nom << " denom = " << denom << endl;
    Int_t p = (100*nom.Atof())/denom.Atof();
    cout << "p = " << p << endl;
    out = TString::Format("zp %d w %d p",denom.Atoi(), p);
    out.ReplaceAll(" ", "");
  }

  return out;
}

TString get_sys_name(TString name)
{
  TObjArray* arr = name.Tokenize(".");
  
  TString out = ((TObjString*)arr->At(arr->GetEntries()-2))->GetString();
  TString res;
  if (out.Contains("JER")) res = "jer";
  if (out.Contains("JEC")) res = "jec";
  
  return res;
}

TString get_var_name(TString name)
{
  TObjArray* arr = name.Tokenize(".");
  
  TString out = ((TObjString*)arr->At(arr->GetEntries()-2))->GetString();
  TString res;
  if (out.Contains("up")) res = "plus";
  if (out.Contains("down")) res = "minus";
  
  return res;
}

TString get_full_name(TString name, TString prefix)
{
  TString sample = get_sample_name(name);
  TString sys = get_sys_name(name);
  TString var = get_var_name(name);
  TString res(prefix.Data());
  res.Append("__" + sample + "__" + sys + "__" + var);
  cout << "sample name = " << sample << " sys = " << sys << " var = " << var << endl;
  return res;
}

void write_theta_file()
{
  // first: merge processes to have the right input files
  //merge_processes();

  TFile* of = new TFile("theta_mtt_muon_jet_unc.root", "RECREATE");

  TString dir = string(".");
  vector<TString> fnames = vector<TString>();
  get_filenames(fnames);

  for (unsigned int i = 0;i < fnames.size();i++) {
    cout << "opening file " << fnames[i] << endl;
    TFile* f = new TFile(fnames[i], "READ");

    // 1 b-tag
    TH1D* h = f->Get("Chi2_BTag/M_ttbar_rec");
    of->cd();
    TH1D* wh = (TH1D*) h->Clone();
    TString name = get_full_name(fnames[i], "mu_1btag_mttbar");
    wh->SetName(name);

    // 0 b-tag
    TH1D* h2 = f->Get("Chi2_NoBTag/M_ttbar_rec");
    of->cd();
    TH1D* wh2 = (TH1D*) h2->Clone();
    TString name = get_full_name(fnames[i], "mu_0btag_mttbar");
    wh2->SetName(name);
    
    f->Close();
    delete f;
  }

  cout << "\nwrote file " << of->GetName() << endl;

  of->Write();
  of->Close();
  delete of;
  

}
