#include <iostream>
#include <vector>
#include "TH1F.h"
#include "TF1.h"
#include "TString.h"
#include "/home/users/sicheng/tas/Software/dataMCplotMaker/dataMCplotMaker.h"

void compareControlRegions();

void compare_data_17vs16()
{
  cout << "Executing macro compareControlRegions()...\n";
  compareControlRegions();

  return;
}

void compareControlRegions() {

  // auto ifile_new = new TFile("../StopLooper/output/temp/data.root");
  // auto ifile_old = new TFile("../StopLooper/output/data2016/data.root");

  // auto ifile_new = new TFile("../StopLooper/output/temp4/data_2017.root");   // 3, 4 with Nvtx reweighting
  // auto ifile_old = new TFile("../StopLooper/output/temp3/data_2016.root");

  // auto ifile_new = new TFile("../StopLooper/output/temp2/data_2017.root");   // 1, 2 without nvtx reweighting
  // auto ifile_old = new TFile("../StopLooper/output/temp1/data_2016.root");

  // auto ifile_new = new TFile("../StopLooper/output/temp6/data_2017.root");   // 5, 6 with Nvtx reweighting
  // auto ifile_old = new TFile("../StopLooper/output/temp5/data_2016.root");

  // auto ifile_new = new TFile("../StopLooper/output/temp6/data_2017dilep.root");
  // auto ifile_old = new TFile("../StopLooper/output/temp5/data_2016dilep.root");

  // auto ifile_new = new TFile("../StopLooper/output/temp4/data_2017dilep.root");
  // auto ifile_old = new TFile("../StopLooper/output/temp3/data_2016dilep.root");

  // auto ifile_new = new TFile("../StopLooper/output/data2017_rwgtd/data_2017F.root");
  auto ifile_new = new TFile("../StopLooper/output/data2017_rwgtd/all_data_2017.root");
  auto ifile_old = new TFile("../StopLooper/output/data2016/data_2016all.root");


  // // For nvtx reweighting purpose
  // vector<string> Dirs = {"testCutflow"};
  // vector<string> Hists = {"h_nvtxs"};

  // vector<string> Hists = {"h_njets", "h_nbjets", "h_mll", "h_met"};
  // vector<string> Hists = {"h_tmod_0b", "h_tmod_wb", "h_nbjets", "h_nbjets_t", "h_njets_t", "h_nbjets_twb"};

  // vector<string> Hists = {"h_njets", "h_rlmet", "h_mt2w", "h_tmod", "h_nbjets", "h_dphijmet"};
  // vector<string> Hists = {"h_mll", "h_finemet", "h_met", "h_njets", "h_nbjets", "h_metphi", "h_lep1pt"};
  // vector<string> Hists = {"h_met", "h_njets_zpeak", "h_njets_noz", "h_mll_2j", "h_mll_g2j"};
  // vector<string> Hists = {"h_mll", "h_rlmet", "h_rlmt", "h_met", "h_nvtxs", "h_mt", "h_njets", "h_nbjets", "h_tmod", "h_mlepb", "h_lep1pt", "h_dphijmet"};
  // vector<string> Hists = {"h_mll", "h_zpt", "h_njets", "h_njets_zpeak", "h_nbjets_zpeak", "h_nbjets_mzpt", "h_nbjets_hzpt"};

  // vector<string> Dirs = {"cr0btest1", "cr0btest2", "cr0btest3", "cr0bbase"};
  // vector<string> Hists = {"h_metbins", "h_mt", "h_njets", "h_nbjets", "h_tmod", "h_mlepb"};
  // vector<string> Hists = {"h_met", "h_njets", "h_mt", "h_tmod", "h_nvtxs", "h_mlepb", "h_lep1pt", "h_dphijmet"};

  vector<string> Dirs = {"cr0bbase"};
  vector<string> Hists = {"h_met"};

  // vector<string> Dirs = {"cr2ltest3", "cr2ltest1", "cr2ltest4", "cr2lbase"};
  // // vector<string> Hists = {"h_metbins", "h_rlmetbins", "h_njets", "h_nbjets", "h_tmod", "h_mlepb"};
  // vector<string> Hists = {"h_mll", "h_rlmet", "h_rlmt", "h_met", "h_nvtxs", "h_mt", "h_njets", "h_nbjets", "h_tmod", "h_mlepb", "h_lep1pt", "h_dphijmet"};

  // vector<string> Dirs = {"cr2lbase"};
  // vector<string> Hists = {"h_rlmet", "h_rlmt", "h_rlmet_u", "h_rlmt_u", "h_met", "h_mt", "h_met_u", "h_mt_u", "h_njets", "h_nbjets", "h_tmod", "h_mlepb",};
  // vector<string> Dirs = {"cr0bbase"};
  // vector<string> Hists = {"h_met", "h_mt", "h_met_u", "h_mt_u", "h_njets", "h_tmod", "h_mlepb",};

  for (auto dirstr : Dirs) {
    for (auto hn : Hists) {
      // for (string suf : {"", "_e", "_mu", "_barrele", "_endcape"}) {
      for (string suf : {"", "_e", "_mu"}) {
      // for (string suf : {"", "_ee", "_mumu", "_hltmet", "_hltmu", "_hltel"}) {
      // for (string suf : {"", "_ee", "_mumu", "_emu"}) {
      // for (string suf : {""}) {
      // for (string suf : {"_passHLT"}) {
        string hstr = hn + suf;
        vector<Color_t> vcolor;
        if (suf == "") vcolor.push_back(kAzure+7);
        else if (suf == "_mu" || suf == "_mumu") vcolor.push_back(kOrange+2);
        else if (suf == "_e" || suf == "_ee") vcolor.push_back(kCyan-7);
        else if (suf == "_barrele") vcolor.push_back(kCyan-3);
        else if (suf == "_endcape") vcolor.push_back(kRed-9);

        auto hnew = (TH1F*) ifile_new->Get((dirstr+"/"+hstr).c_str());
        if (!hnew) { cout << "Can't find " << dirstr+"/"+hstr << endl; continue; }
        auto hold = (TH1F*) ifile_old->Get((dirstr+"/"+hstr).c_str());
        if (!hold) { cout << "Can't find " << dirstr+"/"+hstr << endl; continue; }

        // hnew->Scale(35.87/41.96);
        // hold->Scale(41.96/35.87);
        hold->Scale(hnew->Integral(0,-1) / hold->Integral(0,-1));

        float scale = hnew->Integral(0,-1)/hold->Integral(0,-1);

        // Temporary
        // if (hn == "h_njets")
        //   for (int i = 1; i < 13; ++i)
        //     cout << "ratio for " << i-1 << " jets is: " << hnew->GetBinContent(i) / hold->GetBinContent(i) << endl;

        // Resetting the plot scale
        hnew = (TH1F*) hnew->Clone();
        hold = (TH1F*) hold->Clone();
        if (hn == "h_njets") {
          hnew->GetXaxis()->SetRangeUser(2, 11);
          hold->GetXaxis()->SetRangeUser(2, 11);
        } else if (hn == "h_met") {
          hnew->GetXaxis()->SetRangeUser(150, 650);
          hold->GetXaxis()->SetRangeUser(150, 650);
        } else if (hn == "h_mt") {
          hnew->GetXaxis()->SetRangeUser(150, 600);
          hold->GetXaxis()->SetRangeUser(150, 600);
        }

        // Try to use the dataMCplotMaker
        string optstr = "--darkColorLines --topYaxisTitle Entries --type Preliminary --outOfFrame";
        string xlabel = " --xAxisOverride " + string(hnew->GetXaxis()->GetTitle());
        string oname = " --outputName plots/" + dirstr + "_" + hstr + "_compare.png";
        if (hn.find("h_n") != string::npos || hn.find("phi") != string::npos || !TString(hnew->GetXaxis()->GetTitle()).Contains("[GeV]"))
          optstr += " --noDivisionLabel";
        optstr += " --topYaxisTitle Ratio  --dataName 2017 data (42.0 fb^{-1}, scaled to 35.9 fb^{-1})";
        optstr += " --legendRight -0.5 --legendUp 0.12 --overrideLumi 13 TeV";
        dataMCplotMaker(hnew, {hold}, {"2016 data (35.9 fb^{-1})"}, "", "", optstr+xlabel+oname, {}, {}, vcolor);

        // // Produce nvtx reweighting files, only do it once with the loosest Dir and only h_nvtxs in Hists
        // hnew->Scale(1./hnew->Integral());
        // hold->Scale(1./hold->Integral());
        // auto h_16to17 = (TH1F*) hnew->Clone();
        // auto h_17to16 = (TH1F*) hold->Clone();
        // h_16to17->Divide(hold);
        // h_17to16->Divide(hnew);
        // TFile temp("nvtx_reweighting_alldata.root", "RECREATE");
        // h_16to17->Write("h_nvtxscale_16to17");
        // h_17to16->Write("h_nvtxscale_17to16");
        // temp.Close();

      }
    }
  }
}
