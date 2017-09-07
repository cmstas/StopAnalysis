#include <iostream>
#include <vector>
#include "TH1F.h"
#include "TF1.h"
#include "TString.h"

using namespace std;

void compare_CRemu_efficiency_plots() {

  auto ifile_new = new TFile("../StopLooper/output/temp4/data_met.root");
  auto ifile_old = new TFile("../StopLooper/output/temp3/data_met.root");

  // auto ifile_new = new TFile("../StopLooper/output/temp6/data_muon_eg.root");
  // auto ifile_old = new TFile("../StopLooper/output/temp3/data_muon_eg.root");

  auto ofile = new TFile("plots/HLT_MuE_data_compare.root", "RECREATE");

  vector<TString> Dirs = {"cremu1", "cremu2", "cremu3"};
  vector<TString> Hists = {"h_lep1ptbins", "h_lep2ptbins", "h_njets", "h_nbjets"};
  TString hltsuf = "_passHLT";

  auto c1 = new TCanvas("c1", "c1", 600, 400);
  gStyle->SetOptStat("");
  c1->SetGrid();

  TLatex cms(0.17, 0.83, "CMS");
  cms.SetNDC();
  cms.SetTextFont(61);
  cms.SetTextSize(0.06);

  TLatex prelim(0.17, 0.81, "Preliminary");
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
      // eff new
      auto hden = (TH1F*) ifile_new->Get(dirstr+"/"+hstr);
      if (!hden) { cout << "Can't find " << dirstr+"/"+hstr << endl; continue; }
      auto hnum = (TH1F*) ifile_new->Get(dirstr+"/"+hstr+hltsuf);
      if (!hnum) { cout << "Can't find " << dirstr+"/"+hstr+hltsuf << endl; continue; }

      auto heff_new = (TH1F*) hnum->Clone(hstr+"_eff");
      heff_new->Divide(hnum, hden, 1, 1, "B");

      heff_new->GetYaxis()->SetRangeUser(0, 1.1);
      heff_new->GetYaxis()->SetTitle("HLT_MuE eff");
      heff_new->SetTitle("HLT_MuE efficiency in " + dirstr);
      heff_new->Draw("");
      heff_new->Write(dirstr+"/"+hstr+"_eff_new");

      // eff old
      hden = (TH1F*) ifile_old->Get(dirstr+"/"+hstr);
      if (!hden) { cout << "Can't find " << dirstr+"/"+hstr << endl; continue; }
      hnum = (TH1F*) ifile_old->Get(dirstr+"/"+hstr+hltsuf);
      if (!hnum) { cout << "Can't find " << dirstr+"/"+hstr+hltsuf << endl; continue; }

      auto heff_old = (TH1F*) hnum->Clone(hstr+"_eff");
      heff_old->Divide(hnum, hden, 1, 1, "B");

      heff_old->GetYaxis()->SetRangeUser(0, 1.1);
      heff_old->GetYaxis()->SetTitle("HLT_MuE eff");
      heff_old->SetTitle("HLT_MuE efficiency in " + dirstr);
      heff_old->SetLineColor(kRed);
      heff_old->Draw("same");
      heff_old->Write(dirstr+"/"+hstr+"_eff_old");

      TF1 line1("line1", "1", heff_new->GetXaxis()->GetXmin(), heff_new->GetXaxis()->GetXmax());
      line1.SetLineColor(13);
      line1.SetLineStyle(2);
      line1.Draw("same");

      gPad->Update();
      TLegend leg(0.7, 0.3, 0.88, 0.43);
      leg.AddEntry(heff_old, "data 2016");
      leg.AddEntry(heff_new, "data 2017");
      leg.Draw();


      // cms.Draw();
      // prelim.Draw();
      // data.Draw();
      // lumi.Draw();
      c1->SaveAs("plots/"+dirstr+"_"+hstr+"_HLT_MuE_eff_compare.pdf");
    }
  }

}

void make_CRemu_efficiency_plots(TString ifdir = "../StopLooper/output/temp/", TString ifname = "all_data_Run2017.root") {

  compare_CRemu_efficiency_plots();
  return;

  auto ifile = new TFile(ifdir + ifname);

  auto ofile = new TFile("plots/HLT_MuE_"+ifname, "RECREATE");

  vector<TString> Dirs = {"cremu_0b_met50toInf", "cremu_1b_met50toInf", "cremu_2b_met50toInf"};
  vector<TString> Hists = {"h_lep1ptbins", "h_njets", "h_nbjets"};
  TString hltsuf = "_passHLT";

  auto c1 = new TCanvas("c1", "c1", 600, 400);
  gStyle->SetOptStat("");

  TLatex cms(0.17, 0.83, "CMS");
  cms.SetNDC();
  cms.SetTextFont(61);
  cms.SetTextSize(0.06);

  TLatex prelim(0.17, 0.81, "Preliminary");
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
      auto hden = (TH1F*) ifile->Get(dirstr+"/"+hstr);
      if (!hden) { cout << "Can't find " << dirstr+"/"+hstr << endl; continue; }
      auto hnum = (TH1F*) ifile->Get(dirstr+"/"+hstr+hltsuf);
      if (!hnum) { cout << "Can't find " << dirstr+"/"+hstr+hltsuf << endl; continue; }

      auto heff = (TH1F*) hnum->Clone(hstr+"_eff");
      heff->Divide(hnum, hden, 1, 1, "B");

      heff->GetYaxis()->SetRangeUser(0, 1.1);
      heff->GetYaxis()->SetTitle("HLT_MuE eff");
      heff->SetTitle("HLT_MuE efficiency in " + dirstr);
      heff->Draw();
      heff->Write(dirstr+"/"+hstr+"_eff");

      // gPad->Update();
      // cms.Draw();
      // prelim.Draw();
      // data.Draw();
      // lumi.Draw();
      c1->SaveAs("plots/"+dirstr+"_"+hstr+"_HLT_MuE_eff.pdf");
    }
  }

}
