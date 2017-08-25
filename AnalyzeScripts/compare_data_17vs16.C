#include <iostream>
#include <vector>
#include "TH1F.h"
#include "TF1.h"
#include "TString.h"
#include "/home/users/sicheng/tas/Software/dataMCplotMaker/dataMCplotMaker.h"

void compareCR2lBase();

void compare_data_17vs16()
{
  cout << "Loading dataMCplotMaker...\n";
  // gROOT->ProcessLine(".L ~/tas/Software/dataMCplotMaker/dataMCplotMaker.cc+");
  gSystem->Load("~/tas/Software/dataMCplotMaker/dataMCplotMaker_cc.so");
  
  cout << "Executing macro compareCR2lBase()...\n";
  compareCR2lBase();

  return;
}

void compareCR2lBase() {

  // cout << "Loading dataMCplotMaker...\n";
  // gSystem->Load("/home/users/sicheng/tas/Software/dataMCplotMaker/dataMCplotMaker_cc.so");
  // gROOT->ProcessLine(".L ~/tas/Software/dataMCplotMaker/dataMCplotMaker.cc+");

  auto ifile_new = new TFile("../StopLooper/output/temp/all_data_Run2017.root");
  auto ifile_old = new TFile("../StopLooper/output/data2016/all_data_Run2016.root");

  // auto ofile = new TFile("plots/HLT_MuE_data_compare.root", "RECREATE");

  vector<TString> Dirs = {"cr2lbase", "cr2lincl1", "cr2lincl2"};
  vector<TString> Hists = {"h_metbins", "h_rlmetbins", "h_njets", "h_nbjets", "h_tmod", "h_mlepb"};

  auto c1 = new TCanvas("c1", "c1", 600, 400);
  gStyle->SetOptStat("");

  TLatex cms(0.17, 0.9, "CMS");
  cms.SetNDC();
  cms.SetTextFont(61);
  cms.SetTextSize(0.06);

  TLatex prelim(0.2, 0.9, "Preliminary");
  prelim.SetNDC();
  prelim.SetTextAlign(13);
  prelim.SetTextFont(52);
  prelim.SetTextSize(0.0456);

  TLatex data(0.17, 0.76, "Data 2017");
  data.SetNDC();
  data.SetTextAlign(13);
  data.SetTextFont(52);
  data.SetTextSize(0.0456);

  TLatex lumi(0.85, 0.82, "2.89 fb^{-1} (13 TeV)");
  lumi.SetNDC();
  lumi.SetTextAlign(31);
  lumi.SetTextFont(42);
  lumi.SetTextSize(0.052);

  for (auto dirstr : Dirs) {
    for (auto hstr : Hists) {

      auto hnew = (TH1F*) ifile_new->Get(dirstr+"/"+hstr);
      if (!hnew) { cout << "Can't find " << dirstr+"/"+hstr << endl; continue; }
      auto hold = (TH1F*) ifile_old->Get(dirstr+"/"+hstr);
      if (!hold) { cout << "Can't find " << dirstr+"/"+hstr << endl; continue; }

      hold->Scale(1.0*hnew->Integral()/hold->Integral());

      // Try to use the dataMCplotMaker
      // string optstr = "--darkColorLines --lumi ??? --topYaxisTitle Entries --type Preliminary --dataName data2017 --outOfFrame";
      // dataMCplotMaker(hnew, {hold}, vector<string>{"data2016"}, "", "", optstr);

      hnew->GetYaxis()->SetTitle("Entries");
      hnew->SetMarkerStyle(20);
      hnew->Draw();

      hold->SetFillColor(kAzure+7);
      hold->Draw("histsame");
      hnew->Draw("same");

      gPad->Update();
      TLegend leg(0.7, 0.7, 0.88, 0.83);
      leg.AddEntry(hold, "data 2016");
      leg.AddEntry(hnew, "data 2017");
      leg.Draw();

      c1->SaveAs("plots/"+dirstr+"_"+hstr+"_compare.pdf");
    }
  }

}
