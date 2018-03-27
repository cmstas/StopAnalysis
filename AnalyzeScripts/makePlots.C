// -*- C++ -*-
#include <iostream>
#include <vector>
#include <typeinfo>
#include "TH1F.h"
#include "TF1.h"
#include "TString.h"
#include "/home/users/sicheng/tas/Software/dataMCplotMaker/dataMCplotMaker.h"

using namespace std;

void compareControlRegions();
void makePlotsForSR(TString indir = "3binsMp4Rp9_120ifb");

template<class T> inline bool checkNull(vector<T*> vec, string vecname= "") {
  for (size_t i = 0; i < vec.size(); ++i) {
    if (!vec[i]) {
      cout << "Can't find the " << i << "th object in vector " << vecname << endl;
      return true;
    }
  }
  return false;
}

void makePlots()
{
  cout << "Executing macro compareControlRegions()...\n";
  // compareControlRegions();
  makePlotsForSR();

  return;
}

void makePlotsForSR(TString indir) {

  auto f_ttbar = new TFile("../StopLooper/output/"+indir+"/ttbar_25ns.root");
  auto f_Wjets = new TFile("../StopLooper/output/"+indir+"/Vjets_25ns.root");
  auto f_1ltop = new TFile("../StopLooper/output/"+indir+"/singleT_25ns.root");
  auto f_Znunu = new TFile("../StopLooper/output/"+indir+"/rare_25ns.root");

  // vector<TFile*> vf_bkgs = {f_ttbar, f_Wjets, f_1ltop, f_Znunu, };
  // vector<string> bkgNames = {"ttbar", "Wjets", "1ltop", "Znunu", };
  vector<TFile*> vf_bkgs = {f_1ltop, f_Znunu, f_Wjets, f_ttbar, };
  vector<string> bkgNames = {"1ltop", "Znunu", "Wjets", "ttbar", };

  auto f_T2tt = new TFile("../StopLooper/output/"+indir+"/SMS_T2tt.root");
  auto f_T2bW = new TFile("../StopLooper/output/"+indir+"/SMS_T2bW.root");
  auto f_T2bt = new TFile("../StopLooper/output/"+indir+"/SMS_T2bt.root");

  vector<TFile*> vf_sigs = {f_T2tt, f_T2bW, f_T2bt, };
  vector<string> sigNames = {"T2tt", "T2bW", "T2bt", };

  vector<TString> Dirs = {"srA", "srB", "srC", "srD", "srE", "srF", "srG", "srH", };
  vector<TString> ntagDirs = {"srA_ntt", "srB_ntt", "srC_ntt", "srD_ntt", "srE_ntt", "srF_ntt", "srG_ntt", "srH_ntt", };
  vector<TString> mtagDirs = {"srA_wdt", "srB_wdt", "srC_wdt", "srD_wdt", "srE_wdt", "srF_wdt", "srG_wdt", "srH_wdt", };
  vector<TString> rtagDirs = {"srC_wrt", "srD_wrt", "srE_wrt", "srF_wrt", "srG_wrt", "srH_wrt", };

  vector<TString> Hists = {"met_h"};
  
  TH1F* dummy = new TH1F("", "", 1, 0, 1);

  if (checkNull(vf_bkgs, "vf_bkgs")) return;
  if (checkNull(vf_sigs, "vf_sigs")) return;

  vector<TString> allDirs = mtagDirs; 
  allDirs.insert(allDirs.end(), rtagDirs.begin(), rtagDirs.end());
  allDirs.insert(allDirs.end(), ntagDirs.begin(), ntagDirs.end());

  for (auto dir : allDirs) {
    for (auto hn : Hists) {
      for (TString suf : {""}) {

        vector<TH1F*> bkgVec;
        TString hstr = "h_" + hn + suf;
        for (TFile* f : vf_bkgs) {
          auto hist = (TH1F*) f->Get(dir+"/"+hstr);
          bkgVec.push_back(hist);
        }
        // if (checkNull(bkgVec, Form("bkgVec by %s/%s", dir.Data(), hstr.Data()))) continue;

        float scaleSig = 1;
        TString sigsuf = "_1200_50";
        auto sigTitles = sigNames;
        if (sigsuf != "") for_each(sigTitles.begin(), sigTitles.end(), [&](string& sn) { sn += sigsuf; });
        if (scaleSig != 1) for_each(sigTitles.begin(), sigTitles.end(), [&](string& sn) { sn += " #times " + to_string(int(scaleSig)); });

        vector<TH1F*> sigVec;
        for (TFile* f : vf_sigs) {
          auto hist = (TH1F*) f->Get(dir+"/h_" +hn+sigsuf+suf);
          if (scaleSig != 1) hist->Scale(scaleSig);
          sigVec.push_back(hist);
        }
        if (checkNull(sigVec, "sigVec")) continue;

        TH1F* nonNullHist = nullptr;
        for (auto h : bkgVec) {
          if (h) { nonNullHist = h; break; }
        }
        if (!nonNullHist) { cout << __LINE__ << ": This should not happen!!\n"; continue; }
        for (auto& h : bkgVec) {
          if (!h) {
            h = (TH1F*) nonNullHist->Clone("h_empty");
            h->Reset();
          }
        }

        if (checkNull(bkgVec, Form("bkgVec by %s/%s", dir.Data(), hstr.Data()))) continue;

        // for (auto h : bkgVec) {
        //   h->GetXaxis()->SetRangeUser(250, 650);
        // }
        // for (auto h : sigVec) {
        //   h->GetXaxis()->SetRangeUser(250, 650);
        // }

        // Try to use the dataMCplotMaker
        string optstr = "--darkColorLines --topYaxisTitle Entries --type Preliminary --outOfFrame";
        string xlabel = " --xAxisOverride " + string(bkgVec[0]->GetXaxis()->GetTitle());
        string oname = " --outputName plots/" + string(dir) + "_" + string(hstr) + ".pdf";
        if (!TString(bkgVec[0]->GetXaxis()->GetTitle()).Contains("[GeV]"))
          optstr += " --noDivisionLabel ";
        optstr += " --topYaxisTitle Ratio  ";
        // optstr += "--dataName 2017 data (42.0 fb^{-1}, scaled to 35.9 fb^{-1})";
        // optstr += " --legendRight -0.5 --legendUp 0.12 --overrideLumi 13 TeV";
        optstr += "--legendRight -0.12  --legendUp 0.05  --energy 13  --lumiPrec 1  ";
        optstr += Form("--lumi %.1f  ", 120.0);
        optstr += "--preserveBackgroundOrder  --preserveSignalOrder  --errHistAtBottom  ";
        optstr += "--yTitleOffset -0.3  ";
        // optstr += "--isLinear ";

        vector<Color_t> vcolors = { kRed-7, kOrange-2, kSpring-5, kAzure+10, kCyan-7, kBlack, kMagenta, kOrange+6, };
        dataMCplotMaker(dummy, bkgVec, bkgNames, "", "", optstr+oname+xlabel, sigVec, sigTitles, vcolors);

      }
    }
  }

}

