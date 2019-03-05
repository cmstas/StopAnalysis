// -*- C++ -*-
const double scale = 1;

void copyYieldHist(TFile* fin, TFile* fout, TString gentype) {

  TList* listOfDirs = fin->GetListOfKeys();
  string keep = "sr";
  string skip = "srbase";
  for (auto k : *listOfDirs) {
    TString srname = k->GetName();
    if (srname == "srJ") break;
    if (strncmp (srname, skip.c_str(), skip.length()) == 0) continue;
    if (strncmp (srname, keep.c_str(), keep.length()) != 0) continue;

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
      if (scale != 1) hout->Scale(scale);
      hout->Write();
      if (hname.EndsWith("h_metbins")) hout->Clone("h_metbins_dataStats")->Write();
    }
    if (!outdir->Get("h_metbins")) {
      cout << "Didn't find yield hist for " << gentype << " in " << fin->GetName() << ":" << srname << "/. Faking a 0 one!" << endl;
      outdir->cd();
      TH1D* hout = (TH1D*) fin->Get(srname + "/h_metbins")->Clone("h_metbins");
      for (int i = 1; i <= hout->GetNbinsX(); ++i) {
        hout->SetBinContent(i, 0);
        hout->SetBinError(i, 0);
      }
      hout->Write();
    }
    // if (gentype == "_2lep") {
    //   outdir->cd();
    //   TH1D* hout = (TH1D*) fin->Get(TString(srname).ReplaceAll("sr", "cr2l") + "/h_metbins_cr2lTriggerUp")->Clone("h_metbins_cr2lTriggerUp");
    //   hout->Write();
    //   hout = (TH1D*) fin->Get(TString(srname).ReplaceAll("sr", "cr2l") + "/h_metbins_cr2lTriggerDn")->Clone("h_metbins_cr2lTriggerDn");
    //   hout->Write();
    // }
  }
}

int fakeBkgEstimates(string input_dir="../StopLooper/output/temp_4bins/", string output_dir="", string ysuf="25ns") {
  if (output_dir == "") output_dir = input_dir;

  // set input files (global pointers)
  TFile* fallbkg = new TFile(Form("%s/allBkg_%s.root", input_dir.c_str(), ysuf.c_str()));
  if (fallbkg->IsZombie()) return -1;

  TFile* f2l = new TFile(Form("%s/lostlepton_%s.root",output_dir.c_str(), ysuf.c_str()), "RECREATE");
  TFile* f1l = new TFile(Form("%s/1lepFromW_%s.root",output_dir.c_str(), ysuf.c_str()), "RECREATE");
  TFile* f1ltop = new TFile(Form("%s/1lepFromTop_%s.root",output_dir.c_str(), ysuf.c_str()), "RECREATE");
  TFile* fznunu = new TFile(Form("%s/ZToNuNu_%s.root",output_dir.c_str(), ysuf.c_str()), "RECREATE");

  copyYieldHist(fallbkg, f2l, "_2lep");
  copyYieldHist(fallbkg, f1l, "_1lepW");
  copyYieldHist(fallbkg, f1ltop, "_1lepTop");
  copyYieldHist(fallbkg, fznunu, "_Znunu");

  return 0;
}
