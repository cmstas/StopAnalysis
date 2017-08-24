#include <iostream>
#include <vector>
#include "TH1F.h"
#include "TString.h"

using namespace std;

void make_CRemu_efficiency_plots(TString input_dir = "../StopLooper/output/data2016") {
  
  auto file = new TFile(input_dir+"/all_data_Run2016.root");

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
      auto hden = (TH1F*) file->Get(dirstr+"/"+hstr);
      if (!hden) { cout << "Can't find " << dirstr+"/"+hstr << endl; continue; }
      auto hnum = (TH1F*) file->Get(dirstr+"/"+hstr+hltsuf);
      if (!hnum) { cout << "Can't find " << dirstr+"/"+hstr+hltsuf << endl; continue; }
      
      auto heff = (TH1F*) hnum->Clone(hstr+"_eff");
      heff->Divide(hnum, hden, 1, 1, "B");

      heff->GetYaxis()->SetRangeUser(0, 1.1);
      heff->GetYaxis()->SetTitle("HLT_MuE eff");
      heff->SetTitle("HLT_MuE efficiency in" + dirstr);
      heff->Draw();

      // gPad->Update();
      // cms.Draw();
      // prelim.Draw();
      // data.Draw();
      // lumi.Draw();
      c1->SaveAs("plots/"+dirstr+"_"+hstr+"_HLT_MuE_eff.pdf");
    }
  }

}
