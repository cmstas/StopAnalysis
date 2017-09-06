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

  auto ifile_new = new TFile("../StopLooper/output/temp4/data.root");  // 3, 4 with Nvtx reweighting
  auto ifile_old = new TFile("../StopLooper/output/temp3/data.root");

  // auto ifile_new = new TFile("../StopLooper/output/temp2/data.root");  // 1, 2 without nvtx reweighting
  // auto ifile_old = new TFile("../StopLooper/output/temp1/data.root");

  // auto ifile_new = new TFile("../StopLooper/output/temp2/data_singlemu.root");
  // auto ifile_old = new TFile("../StopLooper/output/temp1/data_single_muon.root");

  // auto ifile_new = new TFile("../StopLooper/output/temp/data_singlemu.root");
  // auto ifile_old = new TFile("../StopLooper/output/data2016/data_single_muon.root");

  // auto ifile_new = new TFile("../StopLooper/output/temp/data_singleel.root");
  // auto ifile_old = new TFile("../StopLooper/output/data2016/data_single_electron.root");

  // vector<string> Dirs = {"cr0btest2", "cr0btest3", "cr0bbase"};
  // vector<string> Hists = {"h_met", "h_njets", "h_mt", "h_tmod", "h_nvtxs", "h_mlepb", "h_lep1pt"};

  // vector<string> Dirs = {"cremu1"};
  // vector<string> Hists = {"h_njets", "h_njets_sp", "h_nbjets", "h_nbjets_sp", "h_mll", "h_met"};

  // vector<string> Dirs = {"cr2ltest", "cr2ltest1", "cr2ltest2", "cr2lbase"};
  vector<string> Dirs = {"cr0btest2"};
  // vector<string> Dirs = {"cr2ltest3", "cr2ltest4"};
  // vector<string> Dirs = {"cr2ltest4", "cr2ltest5", "cr2ltest6", "cr2lbase"};
  // vector<string> Dirs = {"cr2lbase", "cr2lincl1", "cr2lincl2"};
  // vector<string> Hists = {"h_metbins", "h_rlmetbins", "h_njets", "h_nbjets", "h_tmod", "h_mlepb"};
  // vector<string> Hists = {"h_njets", "h_rlmet", "h_mt2w", "h_tmod", "h_nbjets", "h_dphijmet"};
  // vector<string> Hists = {"h_mll", "h_finemet", "h_met", "h_njets", "h_nbjets", "h_metphi", "h_lep1pt"};
  // vector<string> Hists = {"h_met", "h_njets_zpeak", "h_njets_noz", "h_mll_2j", "h_mll_g2j"};
  // vector<string> Hists = {"h_mll", "h_rlmet", "h_rlmt", "h_met", "h_nvtxs", "h_mt", "h_njets", "h_nbjets", "h_tmod", "h_mlepb", "h_lep1pt"};
  vector<string> Hists = {"h_nvtxs", "h_mt", "h_met"};

  // vector<string> Dirs = {"cr0bbase"};
  // vector<string> Hists = {"h_metbins", "h_mt", "h_njets", "h_nbjets", "h_tmod", "h_mlepb"};
  // vector<string> Hists = {"h_metbins", "h_mt", "h_njets", "h_nbjets", "h_tmod", "h_mlepb"};

  for (auto dirstr : Dirs) {
    for (auto hn : Hists) {
      // for (string suf : {"", "_e", "_mu", "_barrele", "_endcape"}) {
      // for (string suf : {"", "_e", "_mu", "_hltsl", "_hltmht"}) {
      // for (string suf : {"", "_ee", "_mumu", "_hltmet", "_hltmu", "_hltel"}) {
      for (string suf : {"", "_ee", "_mumu", "_emu"}) {
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

        hold->Scale(8.32/35.87);

        float scale = hnew->Integral(0,-1)/hold->Integral(0,-1);

        // Try to use the dataMCplotMaker
        string optstr = "--darkColorLines --lumi 8.3 --topYaxisTitle Entries --type Preliminary --dataName data2017 --outOfFrame";
        string xlabel = " --xAxisOverride " + string(hnew->GetXaxis()->GetTitle());
        string oname = " --outputName plots/" + dirstr + "_" + hstr + "_compare.png";
        dataMCplotMaker(hnew, {hold}, {"data2016"}, "", "scale: "+to_string(scale), optstr+xlabel+oname, {}, {}, vcolor);

        // // Produce nvtx reweighting files
        // hnew->Scale(1./hnew->Integral());
        // hold->Scale(1./hold->Integral());
        // hnew->Divide(hold);
        // TFile temp("nvtx_reweighting_alldata.root", "RECREATE");
        // hnew->Write("h_nvtxscale_16to17");
        // temp.Close();

      }
    }
  }
}
