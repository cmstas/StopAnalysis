// -*- C++ -*-
bool useMetExtrapolation = true;
double extr_threshold = 5;  // minimum number of events in a bin to not need an MET extrapolation
bool doCRPurityError = true;
double extr_TFcap = 3;  // maximum value for the transfer factor to not do MET extrapolation
double maxFractionForMC = 0.10;  // minimum value for bkg fraction in SR to do extrapolation, else take from MC
bool extrShapeFromCR = true;
bool yearSeparateSyst = true;

TFile* fbkgs[4];

void dataDrivenFromCR(TFile* fdata, TFile* fmc, TFile* fout, TString ddtype, TString gentype) {
  // Additional hists to consider: dataStats, MCstats, impurity

  TList* listOfDirs = fmc->GetListOfKeys();
  for (auto k : *listOfDirs) {
    TString srname = k->GetName();
    if (!srname.Contains("sr")) continue;
    if (srname.Contains("base") || srname.Contains("incl") || srname.Contains("sb")) continue;
    if (ddtype == "cr0b" && (srname.EndsWith("2") || srname.EndsWith("3"))) continue;

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
      continue;  // <-- actions to be added
    }
    if (!fmc->Get(hname_MC_SR)) {
      cout << "Couldn't find yield hist for " << hist_MC_SR << " in " << fmc->GetName() << ". Cannot define TF!" << endl;
      continue;  // <-- actions to be added
    }
    if (!fdata->Get(hname_data_CR)) {
      cout << "Couldn't find yield hist for " << hist_data_CR << " in " << fdata->GetName() << ". Please use yield from MC!" << endl;
      continue;  // <-- actions to be added
    }

    int lastbin = hist_data_CR->GetNbinsX();
    int extr_start_bin = lastbin; // the bin to start extrapolation, if == lastbin means no MET extrapolation is needed

    auto combineYieldsInExtrBins = [&](TH1D* hist) {
      double err = 0;
      double ylds = hist->IntegralAndError(extr_start_bin, -1, err);
      for (int ibin = extr_start_bin; ibin <= lastbin; ++ibin) {
        hist->SetBinContent(ibin, ylds);
        hist->SetBinError(ibin, err);
      }
    };

    if (useMetExtrapolation) {
      double yldCR(0.0), err(0.0);
      for (; extr_start_bin > 1; --extr_start_bin) {
        yldCR = hist_MC_CR->IntegralAndError(extr_start_bin, -1, err);
        double TFval = hist_MC_SR->GetBinContent(extr_start_bin) / yldCR;
        if (yldCR > extr_threshold && (TFval < extr_TFcap)) break;
      }
      // if (gentype == "_2lep" && (srname == "srE2" || srname == "srG2")) extr_start_bin = lastbin; // temporary hack for 2016
      // if (gentype == "_2lep" && (srname == "srH")) extr_start_bin = 1; // temporary hack for 2016
      if (extr_start_bin != lastbin) {
        cout << "Doing MET extrapolation for  " << crname << "  from bin " << lastbin << " (last bin) to bin " << extr_start_bin << "!" << endl;
        hist_data_CR->Clone("h_datayields_CR_raw")->Write();
        combineYieldsInExtrBins(hist_data_CR);
      }
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
      auto hist_MC_CR = (TH1D*) crdir->Get(hname)->Clone(hname+"_cr");
      auto hist_MC_SR = (TH1D*) fmc->Get(srname + "/" + hnameSR);

      if (!hist_MC_SR) {
        if (!hnameSR.Contains("cr2lTriggerSF"))
          cout << "Couldn't find yield hist for " << (srname + "/" + hnameSR) << " in " << fmc->GetName() << ". Use centralHist!" << endl;
        hist_MC_SR = (TH1D*) fmc->Get(hname_MC_SR)->Clone(hnameSR);
      }

      auto alphaHist = (TH1D*) hist_MC_SR->Clone(hname+"_alpha");
      if (useMetExtrapolation && extrShapeFromCR && extr_start_bin != lastbin) {
        // To take the shape of the MET distribution from the CR
        double cerr_SR(0.), cerr_CR(0.);
        double cyld_SR = alphaHist->IntegralAndError(extr_start_bin, -1, cerr_SR);
        double cyld_CR = hist_MC_CR->IntegralAndError(extr_start_bin, -1, cerr_CR);
        for (int ibin = extr_start_bin; ibin <= lastbin; ++ibin) {
          double metfrac = hist_MC_CR->GetBinContent(ibin) / cyld_CR;
          double relerr = sqrt(pow(cerr_SR/cyld_SR, 2) + pow(hist_MC_CR->GetBinError(ibin)/hist_MC_CR->GetBinContent(ibin), 2));
          alphaHist->SetBinContent(ibin, metfrac * cyld_SR);
          alphaHist->SetBinError(ibin, metfrac * cyld_SR * relerr);
          hist_MC_CR->SetBinContent(ibin, cyld_CR);
          hist_MC_CR->SetBinError(ibin, 0);
        }
        // combineYieldsInExtrBins(hist_MC_CR);
      }
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

      if (yearSeparateSyst && (hname.EndsWith("Up") || hname.EndsWith("Dn"))) {
        for (int i = 1; i < 4; ++i) {

          auto hcen_MC_CR = (TH1D*) fbkgs[i]->Get(crname+"/h_metbins");
          auto hcen_MC_SR = (TH1D*) fbkgs[i]->Get(srname+"/h_metbins"+gentype);
          auto hsys_MC_CR = (TH1D*) fbkgs[i]->Get(crname+"/"+hname);
          auto hsys_MC_SR = (TH1D*) fbkgs[i]->Get(srname+"/"+hnameSR);

          if (!hist_MC_SR) {
            if (!hnameSR.Contains("cr2lTriggerSF"))
              cout << "Couldn't find yield hist for " << (srname + "/" + hnameSR) << " in " << fmc->GetName() << ". Use centralHist!" << endl;
            hist_MC_SR = (TH1D*) fmc->Get(hname_MC_SR)->Clone(hnameSR);
          }

          auto alphaHist_yi = (TH1D*) fmc->Get(hname_MC_SR)->Clone(TString(hname).Insert(hname.Length()-2, Form("%d", 15+i)));
          auto h_MC_CR_yi = (TH1D*) fmc->Get(hname_MC_CR)->Clone(Form("%s_den_%d", hname.Data(), 15+i));
          if (hcen_MC_SR) alphaHist_yi->Add(hcen_MC_SR, -1);
          if (hsys_MC_SR) alphaHist_yi->Add(hsys_MC_SR);
          if (hcen_MC_CR) h_MC_CR_yi->Add(hcen_MC_CR, -1);
          if (hsys_MC_CR) h_MC_CR_yi->Add(hsys_MC_CR);

          if (useMetExtrapolation && extr_start_bin != lastbin) {
            // To take the MET distribution from the CR
            double cerr_SR = 0;
            double cyld_SR = alphaHist_yi->IntegralAndError(extr_start_bin, -1, cerr_SR);
            double cyld_CR = h_MC_CR_yi->Integral(extr_start_bin, -1);
            for (int ibin = extr_start_bin; ibin <= lastbin; ++ibin) {
              double metfrac = h_MC_CR_yi->GetBinContent(ibin) / cyld_CR;
              alphaHist_yi->SetBinContent(ibin, metfrac * cyld_SR);
              alphaHist_yi->SetBinError(ibin, metfrac * cerr_SR);
            }
            combineYieldsInExtrBins(h_MC_CR_yi);
          }
          alphaHist_yi->Divide(h_MC_CR_yi);

          for (int i = 1; i <= alphaHist_yi->GetNbinsX(); ++i) {
            // zero out negative yields
            if (alphaHist_yi->GetBinContent(i) < 0) {
              alphaHist_yi->SetBinContent(i, 0);
              alphaHist_yi->SetBinError(i, 0);
            }
          }
          outdir->cd();
          alphaHist_yi->Multiply(hist_data_CR);
          alphaHist_yi->Write();
        }
      }

      if (hname.EndsWith("h_metbins")) {
        centralHist = hout;
        // Store the central alpha hist and extr_start_bin for signal contamination
        alphaHist->Write("h_alphaHist");
        if (useMetExtrapolation && extr_start_bin < lastbin) {
          TH1D* h_extrstart = new TH1D("h_extrstart", "MET extrapolation start bin", 1, 0, 1);
          h_extrstart->SetBinContent(1, extr_start_bin);
          h_extrstart->Write();
        }
        if (doCRPurityError) {
          auto hist_MC_CR_pure = (TH1D*) fmc->Get(hname_MC_CR + gentype);
          auto hout_purityUp = (TH1D*) hout->Clone(hname+"_CRpurityUp");
          auto hout_purityDn = (TH1D*) hout->Clone(hname+"_CRpurityDn");
          if (useMetExtrapolation && extr_start_bin < lastbin)
            combineYieldsInExtrBins(hist_MC_CR_pure);
          for (int ibin = 1; ibin <= lastbin; ++ibin) {
            double crpurityerr = 0.5 * (hist_MC_CR->GetBinContent(ibin) - hist_MC_CR_pure->GetBinContent(ibin)) / hist_MC_CR->GetBinContent(ibin);
            hout_purityUp->SetBinContent(ibin, hout->GetBinContent(ibin) / ( 1 - crpurityerr));
            hout_purityDn->SetBinContent(ibin, hout->GetBinContent(ibin) / ( 1 + crpurityerr));
          }
          hout_purityUp->Write();
          hout_purityDn->Write();

          auto purityHist = (TH1D*) hist_MC_CR_pure->Clone("h_CRpurity");
          purityHist->Divide(hist_MC_CR_pure, hist_MC_CR, 1, 1, "B");
          purityHist->Write();
        }
      }
    }

    // Create alphaHist for dataStats
    auto h_dataStats = (TH1D*) centralHist->Clone("h_metbins_dataStats");
    auto h_MCStats = (TH1D*) centralHist->Clone("h_metbins_MCStats");
    for (int ibin = 1; ibin <= extr_start_bin; ++ibin) {
      // If not doing met extrapolation, extr_start_bin will equal to lastbin
      double data_error_thisbin = (hist_data_CR->GetBinContent(ibin) < 0.01)? 0 : hist_data_CR->GetBinError(ibin) / hist_data_CR->GetBinContent(ibin);
      h_dataStats->SetBinError(ibin, data_error_thisbin * h_dataStats->GetBinContent(ibin));

      double MC_SR_error_thisbin = (hist_MC_SR->GetBinContent(ibin) < 1e-5)? 0 : hist_MC_SR->GetBinError(ibin) / hist_MC_SR->GetBinContent(ibin);
      double MC_CR_error_thisbin = (hist_MC_CR->GetBinContent(ibin) < 1e-5)? 0 : hist_MC_CR->GetBinError(ibin) / hist_MC_CR->GetBinContent(ibin);
      double MC_error_thisbin = sqrt(MC_SR_error_thisbin*MC_SR_error_thisbin + MC_CR_error_thisbin*MC_CR_error_thisbin);

      h_MCStats->SetBinError(ibin, MC_error_thisbin * h_MCStats->GetBinContent(ibin));
    }
    for (int ibin = extr_start_bin+1; ibin <= lastbin; ++ibin) {
      // If doing met extrapolation, the bins following extr_start_bin for data and MC CR will be set to have 0 stat error
      // TODO: verify that this is the right thing to do
      h_dataStats->SetBinError(ibin, 0);
      double MC_SR_error_thisbin = (hist_MC_SR->GetBinContent(ibin) < 1e-5)? 0 : hist_MC_SR->GetBinError(ibin) / hist_MC_SR->GetBinContent(ibin);
      h_MCStats->SetBinError(ibin, MC_SR_error_thisbin * h_MCStats->GetBinContent(ibin));
    }

    h_dataStats->Write();
    h_MCStats->Write();

    hist_data_CR->Clone("h_datayields_CR")->Write();
    hist_MC_CR->Clone("h_MCyields_CR")->Write();
    hist_MC_SR->Clone("h_MCyields_SR")->Write();
  }
}


void takeDirectlyFromMC(TFile* fin, TFile* fout, TString gentype) {

  TList* listOfDirs = fin->GetListOfKeys();
  for (auto k : *listOfDirs) {
    TString srname = k->GetName();
    if (!srname.Contains("sr")) continue;
    if (srname.Contains("base") || srname.Contains("incl") || srname.Contains("sb")) continue;
    if (gentype == "_1lepW" && !srname.EndsWith("2") && !srname.EndsWith("3")) continue;

    auto indir = (TDirectoryFile*) fin->Get(srname);
    auto outdir = (TDirectory*) fout->mkdir(srname);
    auto hlist = indir->GetListOfKeys();

    for (auto h : *hlist) {
      TString hname = h->GetName();
      if (!hname.BeginsWith("h_metbins" + gentype)) continue;
      TH1D* hin = (TH1D*) indir->Get(hname);
      outdir->cd();
      TH1D* hout = (TH1D*) hin->Clone(TString(hname).ReplaceAll(gentype, ""));
      for (int i = 1; i <= hout->GetNbinsX(); ++i) {
        // zero out negative yields
        if (hout->GetBinContent(i) < 0) {
          hout->SetBinContent(i, 0);
          hout->SetBinError(i, 0);
        }
      }
      hout->Write();

      if (yearSeparateSyst && (hname.EndsWith("Up") || hname.EndsWith("Dn"))) {
        for (int i = 1; i < 4; ++i) {
          TH1D* hcen_yi = (TH1D*) fbkgs[i]->Get(srname+"/h_metbins"+gentype);
          TH1D* hsys_yi = (TH1D*) fbkgs[i]->Get(srname+"/"+hname);
          if (hsys_yi && !hcen_yi) {
            cout << "Find " << srname+"/"+hname << " from " << fbkgs[i]->GetName() << " but not hcen " << srname+"/h_metbins"+gentype << " Should not happen?" << endl;
          }
          TH1D* hout_yi = (TH1D*) fin->Get(srname+"/h_metbins"+gentype)->Clone(TString(hname).Insert(hname.Length()-2, Form("%d", 15+i)).ReplaceAll(gentype, ""));
          if (hcen_yi) hout_yi->Add(hcen_yi, -1);
          if (hsys_yi) hout_yi->Add(hsys_yi);
          hout_yi->Write();
        }
      }
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

// Helper functions to set argument
inline TString parseArg(const TString& input, TString arg, const TString dfval="") {
  if (!arg.EndsWith("=")) arg += "=";
  if (input.Contains(arg)) {
    int sidx = input.Index(arg) + arg.Sizeof() - 1;
    int eidx = input.Index(",", sidx);
    if (eidx < 0) eidx = input.Sizeof();
    return input(sidx, eidx-sidx);
  } else {
    return dfval;
  }
}

inline void parseAndSet_d(const TString& input, TString arg, double& value) {
  TString newskim_arg = parseArg(input, arg);
  if (newskim_arg.IsFloat()) {
    value = newskim_arg.Atof();
  }
}

inline void parseAndSet_b(const TString& input, TString arg, bool& value) {
  TString newval = parseArg(input, arg);
  if (!newval.IsNull()) {
    if (newval.IsBin()) value = newval.Atoi();
    else if (newval == "true") value = true;
    else if (newval == "false") value = false;
  }
}

int makeBkgEstimates(string input_dir="../StopLooper/output/temp14", string output_dir="../StopLooper/output/temp14", string suffix="17", TString extrargs="") {
  // First parse extra argument
  bool MetExtrFor2l = true;
  bool MetExtrFor0b = true;

  parseAndSet_b(extrargs, "useMetExtrapolation" , useMetExtrapolation);
  // parseAndSet_b(extrargs, "MetExtrFor2l"        , MetExtrFor2l);
  // parseAndSet_b(extrargs, "MetExtrFor0b"        , MetExtrFor0b);
  parseAndSet_b(extrargs, "doCRPurityError"     , doCRPurityError);
  parseAndSet_b(extrargs, "extrShapeFromCR"     , extrShapeFromCR);
  parseAndSet_b(extrargs, "yearSeparateSyst"    , yearSeparateSyst);
  parseAndSet_d(extrargs, "extr_threshold"      , extr_threshold);
  parseAndSet_d(extrargs, "extr_TFcap"          , extr_TFcap);
  parseAndSet_d(extrargs, "maxFractionForMC"    , maxFractionForMC);

  // Set input files (global pointers)
  TFile* fbkg = new TFile(Form("%s/allBkg_%s.root",input_dir.c_str(),suffix.c_str()));
  TFile* fdata = new TFile(Form("%s/allData_%s.root",input_dir.c_str(),suffix.c_str()));

  if (!fdata) {
    cout << "Couldn't find fdata!! Can't procceed, exiting!\n"; return -1;
  }

  if (yearSeparateSyst) {
    fbkgs[0] = fbkg;
    fbkgs[1] = new TFile(Form("%s/allBkg_16.root",input_dir.c_str()));
    fbkgs[2] = new TFile(Form("%s/allBkg_17.root",input_dir.c_str()));
    fbkgs[3] = new TFile(Form("%s/allBkg_18.root",input_dir.c_str()));
  }

  // Create output files
  TFile* f2l = new TFile(Form("%s/lostlepton_%s.root",output_dir.c_str(), suffix.c_str()), "RECREATE");
  TFile* f1l = new TFile(Form("%s/1lepFromW_%s.root",output_dir.c_str(), suffix.c_str()), "RECREATE");
  TFile* f1ltop = new TFile(Form("%s/1lepFromTop_%s.root",output_dir.c_str(), suffix.c_str()), "RECREATE");
  TFile* fznunu = new TFile(Form("%s/ZToNuNu_%s.root",output_dir.c_str(), suffix.c_str()), "RECREATE");

  dataDrivenFromCR(fdata, fbkg, f2l, "cr2l", "_2lep");
  dataDrivenFromCR(fdata, fbkg, f1l, "cr0b", "_1lepW");

  takeDirectlyFromMC(fbkg, f1l,    "_1lepW");  // only for top-tagged bins
  takeDirectlyFromMC(fbkg, f1ltop, "_1lepTop");
  takeDirectlyFromMC(fbkg, fznunu, "_Znunu");

  return 0;
}

int postProcessOnly(string sample="ttHtoInv", string input_dir="../StopLooper/output/sampttH_v32_s7", string output_dir="../StopLooper/output/sampttH_v32_s7", string suffix="run2") {

  // Set input files (global pointers)
  TFile* fbkg = new TFile(Form("%s/%s_%s.root", input_dir.c_str(), sample.c_str(), suffix.c_str()));

  yearSeparateSyst = (suffix == "run2");
  if (yearSeparateSyst) {
    fbkgs[0] = fbkg;
    fbkgs[1] = new TFile(Form("%s/%s_s16v3.root",input_dir.c_str(), sample.c_str()));
    fbkgs[2] = new TFile(Form("%s/%s_f17v2.root",input_dir.c_str(), sample.c_str()));
    fbkgs[3] = new TFile(Form("%s/%s_a18v1.root",input_dir.c_str(), sample.c_str()));
  }

  // Create output files
  TFile* fout = new TFile(Form("%s/sig_%s_%s.root",output_dir.c_str(), sample.c_str(), suffix.c_str()), "RECREATE");

  takeDirectlyFromMC(fbkg, fout, "");

  fbkg->Close();
  fout->Close();
  return 0;
}
