// -*- C++ -*-
#include <iostream>
#include <vector>
#include "TH1F.h"
#include "TF1.h"
#include "TString.h"
#include "/home/users/sicheng/tas/Software/dataMCplotMaker/dataMCplotMaker.h"

void compareControlRegions();
void compareData161718();
void compareDataMC_CRemu();

void compareDistributions()
{
  cout << "Executing macro compareControlRegions()...\n";
  // compareControlRegions();
  // compareData161718();
  compareDataMC_CRemu();

  return;
}

void compareDataMC_CRemu() {
  auto fdata  = new TFile("../StopLooper/output/samp17_v30_t1/allData_17.root");
  auto fttbar = new TFile("../StopLooper/output/samp17_v30_t1/ttbar_17.root");

  auto h_nvtx_data  = (TH1F*) fdata->Get("cremuA0/h_nvtxs");
  auto h_nvtx_ttbar = (TH1F*) fttbar->Get("cremuA0/h_nvtxs");

  // h_nvtx_data->Scale(1./h_nvtx_data->Integral());
  // h_nvtx_ttbar->Scale(1./h_nvtx_ttbar->Integral());
  // auto h_rwgt = (TH1F*) h_nvtx_data->Clone();
  // h_rwgt->Divide(h_nvtx_ttbar);
  // TFile temp("nvtx_reweighting_all.root", "UPDATE");
  // h_rwgt->Write("h_nvtxscale_cremu0_ttbar");
  // temp.Close();

  auto h_njetisr_data  = (TH1F*) fdata ->Get("cremuA0/h_njet_200nonb");
  auto h_njetisr_ttbar = (TH1F*) fttbar->Get("cremuA0/h_njet_200nonb");

  h_njetisr_data ->Scale(1./h_njetisr_data ->Integral());
  h_njetisr_ttbar->Scale(1./h_njetisr_ttbar->Integral());
  auto h_rwgt = (TH1F*) h_njetisr_data->Clone();
  h_rwgt->Divide(h_njetisr_ttbar);
  for (int i = 1; i < h_rwgt->GetNbinsX()+1; ++i) {
    cout << __LINE__ << ": i= " << i << ", h_rwgt->GetBinContent(i)= " << h_rwgt->GetBinContent(i) << endl;
  }

  // TFile temp("nvtx_reweighting_all.root", "UPDATE");
  // h_rwgt->Write("h_nvtxscale_cremu0_ttbar");
  // temp.Close();
}

void compareData161718() {

  // tempdata5 -- uses 14.4/fb, use 2017C JEC for 2018 data, v27_5 for 17 & 18
  // tempdata6 -- uses deepCSV, mean for comparison between 17 & 18 only
  // tempdata8 -- folder for separated datasets
  // tempdata9 -- using CSV btag, do Nvtx reweighting, uses 14.4/fb should be used for comparisons v27_6 for 17 & 18
  // tempdataA -- temporary for no Nvtx reweighting,
  // tempdata3 -- uses 14.4/fb, deepCSV btag, v27_6 for both 2017 and 2018 data
  // tempdata4 -- 2017 prompt reco data (old header, deepCSV) also ran the same config for 2016 data just in case (CSVv2)
  // tempdata1 -- Going back to v27_5 for
  // auto ifile_new = new TFile("../StopLooper/output/temp2018data1/data_2018_all.root");
  // auto ifile_new = new TFile("../StopLooper/output/temp2017data5/data_2017_all.root");
  // auto ifile_ref = new TFile("../StopLooper/output/temp2017data4/data_2017_all.root");
  // auto ifile_ref = new TFile("../StopLooper/output/temp2017data1/data_2017_all.root");
  // auto ifile_ref = new TFile("../StopLooper/output/temp2018data5/data_2018_all.root");
  auto ifile_old = new TFile("../StopLooper/output/temp2016data4/data_2016_all.root");
  // auto ifile_old = new TFile("../StopLooper/output/temp2017data1/data_2017_all.root");
  // auto ifile_old2 = new TFile("../StopLooper/output/temp2017data9/data_2017C.root");
  auto ifile_new = new TFile("../StopLooper/output/tempHEcheck/ttbar_RelValHEmiss.root");
  auto ifile_ref = new TFile("../StopLooper/output/tempHEcheck/ttbar_RelValHEgood.root");

  // // For nvtx reweighting purpose
  // vector<string> Dirs = {"testGeneral", "cr2ltest0"};
  // vector<string> Hists = {"h_nvtxs", "h_nvtxs_rwtd", "h_mll"};
  // // vector<string> Dirs = {"testGeneral"};
  // // vector<string> Hists = {"h_nvtxs"};

  // vector<string> Dirs = {"cr0bbase", "cr2lbase"};
  // vector<string> Dirs = {"srtest2", "cr0btest2", "cr2ltest2"};
  // vector<string> Dirs = {"cr2ltest5"};
  // vector<string> Dirs = {"cr0bI", "cr2lI"};
  // vector<string> Hists = {"h_met", "h_njets", "h_nbjets", "h_mt", "h_tmod", "h_nvtxs", "h_mlepb", "h_lep1pt", "h_lep1eta", "h_dphijmet", "h_jet1pt", "h_jet1eta", "h_jet2pt", "h_jet2eta", "h_nleps", "h_lep2pt", "h_lep2eta"};
  // vector<string> Hists = {"h_lep1eta", "h_jet2eta",};

  // vector<string> Dirs = {"cr0btest5"};
  // vector<string> Hists = { "h_dphijmet", "h_met_h"};

  vector<string> Dirs = {"srtest0", "cr2ltest0", "cr0btest0"};
  vector<string> Hists = { "h_met_h", "h_mt_h"};

  // vector<string> Dirs = {"cr0btest2", "cr2ltest2"};
  // vector<string> Hists = {"h_dphijmet", "h_dphijmet_notrl"};

  // vector<string> Dirs = {"cr0btestMET", "cr2ltestMET"};
  // vector<string> Hists = {"h_met", "h_met_h"};

  // vector<string> Dirs = {"cremu1"};
  // vector<string> Hists = {"h_met", "h_njets", "h_nbjets", "h_mt", "h_tmod", "h_mlepb", "h_lep1pt", "h_lep1eta", "h_dphijmet", "h_jet1pt", "h_jet1eta"};

  // vector<string> Dirs = {"cr2lbase"};
  // vector<string> Dirs = {"cr2ltest3", "cr2ltest1", "cr2ltest4", "cr2lbase"};
  // // vector<string> Hists = {"h_metbins", "h_rlmetbins", "h_njets", "h_nbjets", "h_tmod", "h_mlepb"};
  // vector<string> Hists = {"h_mll", "h_rlmet", "h_rlmet_h" "h_rlmt", "h_rlmt_h", "h_njets", "h_nbjets", "h_tmod", "h_mlepb", "h_lep1pt", "h_dphijmet"};

  // vector<string> Dirs = {"cr2ltest0"};
  // vector<string> Hists = {"h_mll"};
  // vector<string> Hists = {"h_rlmet", "h_rlmt", "h_rlmet_u", "h_rlmt_u", "h_met", "h_mt", "h_met_u", "h_mt_u", "h_njets", "h_nbjets", "h_tmod", "h_mlepb",};
  // vector<string> Dirs = {"cr0bbase"};
  // vector<string> Hists = {"h_met", "h_mt", "h_met_u", "h_mt_u", "h_njets", "h_tmod", "h_mlepb",};

  for (auto dirstr : Dirs) {
    for (auto hn : Hists) {
      // for (string suf : {"", "_e", "_mu"}) {
      // for (string suf : {"", "_ee", "_mumu", "_emu"}) {
      for (string suf : {""}) {
        string hstr = hn + suf;
        vector<Color_t> vcolor;
        if (suf == "") {
          if (hn.find("cr2l") != string::npos) vcolor.push_back(kTeal);
          else vcolor.push_back(kAzure+7);
        }
        else if (suf == "_mu" || suf == "_mumu") vcolor.push_back(kOrange+2);
        else if (suf == "_e" || suf == "_ee") vcolor.push_back(kCyan-7);
        else if (suf == "_hltmet" || suf == "_ee") vcolor.push_back(kGray);
        else if (suf == "_hltmu") vcolor.push_back(kOrange-2);
        else if (suf == "_hltel") vcolor.push_back(kCyan+2);
        else if (suf == "_hltsl") vcolor.push_back(kGreen+2);
        else if (suf == "_barrele") vcolor.push_back(kCyan-3);
        else if (suf == "_endcape") vcolor.push_back(kRed-9);

        auto hnew = (TH1F*) ifile_new->Get((dirstr+"/"+hstr).c_str());
        if (!hnew) { cout << "Can't find " << dirstr+"/"+hstr << " in hnew!" << endl; continue; }
        auto hold = (TH1F*) ifile_old->Get((dirstr+"/"+hstr).c_str());
        // if (!hold) { cout << "Can't find " << dirstr+"/"+hstr << " in hold!" << endl; continue; }
        auto href = (TH1F*) ifile_ref->Get((dirstr+"/"+hstr).c_str());
        if (!href && hstr == "h_nvtxs_rwtd") href = (TH1F*) ifile_ref->Get("testGeneral/h_nvtxs");
        if (!href) { cout << "Can't find " << dirstr+"/"+hstr << " in href!" << endl; continue; }

        // Resetting the plot scale
        hnew = (TH1F*) hnew->Clone();
        // hold = (TH1F*) hold->Clone();
        href = (TH1F*) href->Clone();

        // hnew->Scale(41.96/19.26);
        hold->Scale(41.96/35.87);
        // hnew->Scale(41.96/16.59);
        // hnew->Scale(41.96/14.38);
        // hold->Scale(14.38/35.87);

        float scale1 = hnew->Integral(0,-1)/href->Integral(0,-1);
        float scale0 = hold->Integral(0,-1)/href->Integral(0,-1);
        cout << "  hstr= " << hstr << ", scale0= " << scale0 << ", scale1= " << scale1 << endl;

        if (hstr == "h_mll") {
          float scale_zpeak = hnew->GetBinContent(hnew->FindBin(91))/href->GetBinContent(href->FindBin(91));
          cout << "  scale_zpeak= " << scale_zpeak << endl;
        }

        // Try to use the dataMCplotMaker
        string optstr = "--darkColorLines --topYaxisTitle Entries --type Preliminary --outOfFrame";
        // optstr += " --noOverflow";
        string xlabel = " --xAxisOverride " + string(hnew->GetXaxis()->GetTitle());
        string oname = " --outputName plots/" + dirstr + "_" + hstr + "_compare.png";
        if (hn.find("h_n") != string::npos || hn.find("phi") != string::npos || !TString(hnew->GetXaxis()->GetTitle()).Contains("[GeV]"))
          optstr += " --noDivisionLabel";
        // optstr += " --topYaxisTitle Ratio --dataName 2018 data (14.4 fb^{-1}, scaled to 42.0 fb^{-1})";
        // optstr += " --topYaxisTitle Ratio --dataName 2017 data PromptReco (42.0 fb^{-1})";
        optstr += " --topYaxisTitle Ratio --dataName 2018 data (16.6 fb^{-1}, scaled to 42.0 fb^{-1})";
        optstr += " --legendRight -0.5 --legendUp 0.12 --overrideLumi 13 TeV";
        string dataname16 = "2016 data (35.9 fb^{-1}, scaled to 42.0 fb^{-1})";
        // dataMCplotMaker(hnew, {href}, {"2017 data 31Mar2018 ReReco (42.0 fb^{-1})"}, "", "", optstr+xlabel+oname, {hold}, {dataname16}, vcolor);
        dataMCplotMaker(hnew, {href}, {"2017 data (42.0 fb^{-1})"}, "", "", optstr+xlabel+oname, {}, {}, vcolor);

        // // Produce nvtx reweighting files, only do it once with the loosest Dir and only h_nvtxs in Hists
        // hnew->Scale(1./hnew->Integral());
        // href->Scale(1./href->Integral());
        // auto h_16to17 = (TH1F*) hnew->Clone();
        // auto h_17to16 = (TH1F*) href->Clone();
        // h_16to17->Divide(hold);
        // h_17to16->Divide(hnew);
        // TFile temp("nvtx_reweighting_alldata.root", "UPDATE");
        // h_16to17->Write("h_nvtxscale_17to18");
        // h_17to16->Write("h_nvtxscale_18to17");
        // temp.Close();

      }
    }
  }
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
