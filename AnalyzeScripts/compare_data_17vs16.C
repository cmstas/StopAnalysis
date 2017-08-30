#include <iostream>
#include <vector>
#include "TH1F.h"
#include "TF1.h"
#include "TString.h"
#include "/home/users/sicheng/tas/Software/dataMCplotMaker/dataMCplotMaker.h"

void compareCR2lBase();

void compare_data_17vs16()
{
  cout << "Executing macro compareCR2lBase()...\n";
  compareCR2lBase();

  return;
}

void compareCR2lBase() {

  auto ifile_new = new TFile("../StopLooper/output/temp/emumet_data_Run2017.root");
  auto ifile_old = new TFile("../StopLooper/output/data2016/emumet_data_Run2016.root");

  // auto ifile_new = new TFile("../StopLooper/output/temp/all_data_Run2017.root");
  // auto ifile_old = new TFile("../StopLooper/output/data2016/all_data_Run2016.root");

  // auto ifile_new = new TFile("../StopLooper/output/temp/data_met.root");
  // auto ifile_old = new TFile("../StopLooper/output/data2016/data_met.root");

  // auto ifile_new = new TFile("../StopLooper/output/temp/data_singlemu.root");
  // auto ifile_old = new TFile("../StopLooper/output/data2016/data_single_muon.root");

  // auto ifile_new = new TFile("../StopLooper/output/temp/data_singleel.root");
  // auto ifile_old = new TFile("../StopLooper/output/data2016/data_single_electron.root");

  // vector<string> Dirs = {"cr2lbase", "cr2lincl1", "cr2lincl2"};
  // vector<string> Hists = {"h_metbins", "h_rlmetbins", "h_njets", "h_nbjets", "h_tmod", "h_mlepb"};

  // vector<string> Dirs = {"cr2ltest", "cr2ltest2", "cr2lbase"};
  // vector<string> Hists = {"h_njets", "h_rlmetbins", "h_tmod", "h_mlepb"};

  // vector<string> Dirs = {"crtest", "crtest1", "crtest2", "crtest3", "crtest4"};
  // vector<string> Dirs = {"cr2ltest", "cr2ltest1", "cr2ltest2", "cr2ltest3", "cr2ltest4"};
  // vector<string> Dirs = {"cr2ltest4", "cr2ltest5", "cr2ltest6", "cr2lbase"};
  // vector<string> Hists = {"h_njets", "h_rlmet", "h_mt2w", "h_tmod", "h_nbjets", "h_dphijmet"};
  // vector<string> Hists = {"h_njets", "h_rlmet", "h_mt2w", "h_tmod", "h_nbjets", "h_dphijmet"};

  vector<string> Dirs = {"cr0bbase", "cr0bA", "cr0bC"};
  vector<string> Hists = {"h_metbins", "h_mt", "h_njets", "h_nbjets", "h_tmod", "h_mlepb"};

  for (auto dirstr : Dirs) {
    for (auto hn : Hists) {
      for (string suf : {"", "_e", "_mu"}) {
        string hstr = hn + suf;
        vector<Color_t> vcolor;
        if (suf == "") vcolor.push_back(kAzure+7);
        if (suf == "_mu") vcolor.push_back(kOrange+2);

        auto hnew = (TH1F*) ifile_new->Get((dirstr+"/"+hstr).c_str());
        if (!hnew) { cout << "Can't find " << dirstr+"/"+hstr << endl; continue; }
        auto hold = (TH1F*) ifile_old->Get((dirstr+"/"+hstr).c_str());
        if (!hold) { cout << "Can't find " << dirstr+"/"+hstr << endl; continue; }

        // hold->Scale(1.0*hnew->Integral()/hold->Integral());
        hold->Scale(10.7/35.87);

        float scale = hnew->Integral(0,-1)/hold->Integral(0,-1);

        // Try to use the dataMCplotMaker
        string optstr = "--darkColorLines --lumi 10.7 --topYaxisTitle Entries --type Preliminary --dataName data2017 --outOfFrame";
        string xlabel = " --xAxisOverride " + string(hnew->GetXaxis()->GetTitle());
        string oname = " --outputName plots/" + dirstr + "_" + hstr + "_compare.png";
        dataMCplotMaker(hnew, {hold}, {"data2016"}, "", "scale: "+to_string(scale), optstr+xlabel+oname, {}, {}, vcolor);
      }
    }
  }
}
