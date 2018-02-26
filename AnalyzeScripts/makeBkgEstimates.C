// -*- C++ -*-

void dataDrivenFromCR(TFile* fdata, TFile* fmc, TFile* fout, TString ddtype, TString gentype) {
  // Additional hists to consider: dataStats, MCstats, impurity
  
  TList* listOfDirs = fmc->GetListOfKeys();
  for (auto k : *listOfDirs) {
    TString srname = k->GetName();
    if (!srname.Contains("sr")) continue;
    if (srname.Contains("base")) continue;
    TString crname = srname;
    crname.ReplaceAll("sr", ddtype);

    TString hname_data_CR = crname + "/h_metbins";
    TString hname_MC_SR = srname + "/h_metbins" + gentype;
    TString hname_MC_CR = crname + "/h_metbins";

    auto outdir = (TDirectory*) fout->mkdir(srname);

    auto hist_data_CR = (TH1D*) fdata->Get(hname_data_CR);
    auto hist_MC_CR = (TH1D*) fmc->Get(hname_MC_CR);
    auto hist_MC_SR = (TH1D*) fmc->Get(hname_MC_SR);

    if (!fmc->Get(hname_MC_CR)) {
      cout << "Couldn't find yield hist for " << hist_MC_CR << " in " << fmc->GetName() << "!!" << endl;
      cout << "This should not happend! Can not use data driven on this region! Use MC yields directly use TF from other SR!" << endl;
      continue;  // <-- actions to add
    }
    if (!fmc->Get(hname_MC_SR)) {
      cout << "Couldn't find yield hist for " << hist_MC_SR << " in " << fmc->GetName() << ". Cannot define TF!" << endl;
      continue;  // <-- actions to add
    }
    if (!fdata->Get(hname_data_CR)) {
      cout << "Couldn't find yield hist for " << hist_data_CR << " in " << fmc->GetName() << ". Use yield from MC!" << endl;
      continue;  // <-- actions to add
    }

    TH1D* centralHist;

    auto crdir = (TDirectoryFile*) fmc->Get(crname);
    for (auto h : *(crdir->GetListOfKeys())) {
      TString hname = h->GetName();
      if (!hname.BeginsWith("h_metbins")) continue;
      // hardcode genclass skipping for now
      if (hname.Contains("_2lep") || hname.Contains("_1lep") || hname.Contains("_Znunu") || hname.Contains("_unclass")) continue;

      TString hnameSR = hname;
      hnameSR.ReplaceAll("h_metbins", "h_metbins" + gentype);

      // Not using fraction
      auto hist_MC_CR = (TH1D*) crdir->Get(hname);
      auto hist_MC_SR = (TH1D*) fmc->Get(srname + "/" + hnameSR);

      if (!hist_MC_SR) {
        if (!hnameSR.Contains("cr2lTriggerSF"))
          cout << "Couldn't find yield hist for " << (srname + "/" + hnameSR) << " in " << fmc->GetName() << ". Use centralHist!" << endl;
        hist_MC_SR = (TH1D*) fmc->Get(hname_MC_SR)->Clone(hnameSR);
      }

      auto alphaHist = (TH1D*) hist_MC_SR->Clone(hname+"_alpha");
      alphaHist->Divide(hist_MC_CR);

      for (int i = 1; i <= alphaHist->GetNbinsX(); ++i) {
        // zero out negative yields
        if (alphaHist->GetBinContent(i) < 0) {
          alphaHist->SetBinContent(i, 0);
          alphaHist->SetBinError(i, 0);
        }
      }

      outdir->cd();
      TH1D* hout = (TH1D*) alphaHist->Clone(hname);
      hout->Multiply(hist_data_CR);
      hout->Write();

      if (hname.EndsWith("h_metbins")) centralHist = hout;
    }

    // Create alphaHist for dataStats
    auto h_dataStats = (TH1D*) centralHist->Clone("h_metbins_dataStats");
    auto h_MCStats = (TH1D*) centralHist->Clone("h_metbins_MCStats");
    for (int ibin = 1; ibin <= hist_data_CR->GetNbinsX(); ++ibin) {
      double data_error_thisbin = (hist_data_CR->GetBinContent(ibin) < 0.01)? 0 : hist_data_CR->GetBinError(ibin) / hist_data_CR->GetBinContent(ibin);
      h_dataStats->SetBinError(ibin, data_error_thisbin * h_dataStats->GetBinContent(ibin));

      double MC_SR_error_thisbin = (hist_MC_SR->GetBinContent(ibin) < 1e-5)? 0 : hist_MC_SR->GetBinError(ibin) / hist_MC_SR->GetBinContent(ibin);
      double MC_CR_error_thisbin = (hist_MC_CR->GetBinContent(ibin) < 1e-5)? 0 : hist_MC_CR->GetBinError(ibin) / hist_MC_CR->GetBinContent(ibin);
      double MC_error_thisbin = sqrt(MC_SR_error_thisbin*MC_SR_error_thisbin + MC_CR_error_thisbin*MC_CR_error_thisbin);

      h_MCStats->SetBinError(ibin, MC_error_thisbin * h_MCStats->GetBinContent(ibin));
    }
    h_dataStats->Write();
    h_MCStats->Write();

  }
}


void takeDirectlyFromMC(TFile* fin, TFile* fout, TString gentype) {

  TList* listOfDirs = fin->GetListOfKeys();
  for (auto k : *listOfDirs) {
    TString srname = k->GetName();
    if (!srname.Contains("sr")) continue;
    if (srname.Contains("base")) continue;

    auto indir = (TDirectoryFile*) fin->Get(srname);
    auto outdir = (TDirectory*) fout->mkdir(srname);
    auto hlist = indir->GetListOfKeys();

    for (auto h : *hlist) {
      TString hname = h->GetName();
      if (!hname.BeginsWith("h_metbins" + gentype)) continue;
      TH1D* hin = (TH1D*) indir->Get(hname);
      outdir->cd();
      TH1D* hout = (TH1D*) hin->Clone(hname.ReplaceAll(gentype, ""));
      for (int i = 1; i <= hout->GetNbinsX(); ++i) {
        // zero out negative yields
        if (hout->GetBinContent(i) < 0) {
          hout->SetBinContent(i, 0);
          hout->SetBinError(i, 0);
        }
      }
      hout->Write();
    }
    if (!outdir->Get("h_metbins")) {
      cout << "Didn't find yield hist for " << gentype << " in " << fin->GetName() << ":" << srname << "/. Faking a 0 one!" << endl;
      outdir->cd();
      // Get the MET binning from h_metbins, which shall always exist, and then set all bins to 0
      TH1D* hout = (TH1D*) fin->Get(srname + "/h_metbins")->Clone("h_metbins");
      for (int i = 1; i <= hout->GetNbinsX(); ++i) {
        hout->SetBinContent(i, 0);
        hout->SetBinError(i, 0);
      }
      hout->Write();
    }
  }
}

int makeBkgEstimates(string input_dir="../StopLooper/output/temp14", string output_dir="../StopLooper/output/temp14") {

  // Set input files (global pointers)
  TFile* fbkg = new TFile(Form("%s/allBkg_25ns.root",input_dir.c_str()));
  TFile* fdata = new TFile(Form("%s/allData_25ns.root",input_dir.c_str()));

  if (!fdata) {
    cout << "Couldn't find fdata!! Can't procceed, exiting!\n"; return -1;
  }

  // Create output files
  TFile* f2l = new TFile(Form("%s/lostlepton.root",output_dir.c_str()), "RECREATE");
  TFile* f1l = new TFile(Form("%s/1lepFromW.root",output_dir.c_str()), "RECREATE");
  TFile* f1ltop = new TFile(Form("%s/1lepFromTop.root",output_dir.c_str()), "RECREATE");
  TFile* fznunu = new TFile(Form("%s/ZToNuNu.root",output_dir.c_str()), "RECREATE");

  dataDrivenFromCR(fdata, fbkg, f2l, "cr2l", "_2lep");
  dataDrivenFromCR(fdata, fbkg, f1l, "cr0b", "_1lepW");

  takeDirectlyFromMC(fbkg, f1ltop, "_1lepTop");
  takeDirectlyFromMC(fbkg, fznunu, "_Znunu");

  return 0;
}
