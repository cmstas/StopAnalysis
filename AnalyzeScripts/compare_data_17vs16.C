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

  auto ifile_new = new TFile("../StopLooper/output/temp/all_data_Run2017.root");
  auto ifile_old = new TFile("../StopLooper/output/data2016/all_data_Run2016.root");

  vector<string> Dirs = {"cr2lbase", "cr2lincl1", "cr2lincl2"};
  vector<string> Hists = {"h_metbins", "h_rlmetbins", "h_njets", "h_nbjets", "h_tmod", "h_mlepb"};

  for (auto dirstr : Dirs) {
    for (auto hstr : Hists) {

      auto hnew = (TH1F*) ifile_new->Get((dirstr+"/"+hstr).c_str());
      if (!hnew) { cout << "Can't find " << dirstr+"/"+hstr << endl; continue; }
      auto hold = (TH1F*) ifile_old->Get((dirstr+"/"+hstr).c_str());
      if (!hold) { cout << "Can't find " << dirstr+"/"+hstr << endl; continue; }

      hold->Scale(1.0*hnew->Integral()/hold->Integral());

      // Try to use the dataMCplotMaker
      string optstr = "--darkColorLines --lumi 999 --topYaxisTitle Entries --type Preliminary --dataName data2017 --outOfFrame";
      string oname = " --outputName plots/" + dirstr + "_" + hstr + "_compare.pdf";
      dataMCplotMaker(hnew, {hold}, {"data2016"}, "", "", optstr+oname, {}, {}, {kAzure+7});

    }
  }
}
