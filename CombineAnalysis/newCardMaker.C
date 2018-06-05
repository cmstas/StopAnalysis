// -*- C++ -*-
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <stdexcept>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <algorithm>

// ROOT
#include "TDirectory.h"
#include "TTreeCache.h"
#include "Math/VectorUtil.h"
#include "TVector2.h"
#include "TBenchmark.h"
#include "TMath.h"
#include "TString.h"
#include "TLorentzVector.h"
#include "TF1.h"


// defined steps in lsp/stop mass - 25 GeV is fine for everything
const int step = 25;
// define maximum number of processes global as it is used by correlation testing functions outside main function
const int maxnprocess = 5;

// do correlated syst for bkg errors? default: true
const bool correlated = true;
// do subtract signal contamination from CR? default: true
const bool dosigcont = true;
// do average with yields using genmet for signal? default: true
const bool dogenmet = true;
// test without real data?
const bool fakedata = false;
// test without background systematics?
const bool nobgsyst = false;
// test without signal systematics?
const bool nosigsyst = false;

bool verbose = true;  // automatic turned off if is signal scan

// global file pointers
TFile *fsig;
TFile *f2l;
TFile *f1l;
TFile *f1ltop;
TFile *fznunu;
TFile *fdata;
TFile *fsig_genmet;

// generic function to get yield from histograms
double getYield(TFile* file, TString hname, int metbin, int m1 = 0, int m2 = 0, int endbin = 0) {
  TH1* hist = (TH1*) file->Get(hname);
  if (!hist) return 0;
  if (metbin > hist->GetNbinsX()) {
    cout << "[getYield] Request bin " << metbin << " exceed the maximum " << hist->GetNbinsX() << " for " << hname << " in file " << file->GetName() << " !!\n";
    return 0;
  }
  if (hname.Contains("hSMS")) {
    int biny = hist->GetYaxis()->FindBin(m1);
    int binz = hist->GetZaxis()->FindBin(m2);
    if (endbin != 0)
      return ((TH3*) hist)->Integral(metbin, endbin, biny, biny, binz, binz);
    else
      return hist->GetBinContent(metbin, biny, binz);
  }

  if (endbin != 0)
    return hist->Integral(metbin, endbin);

  return hist->GetBinContent(metbin);
}

// generic function to get yield from histograms, and error in the card format 1 + relerr
double getYieldAndError(double& error, TFile* file, TString hname, int metbin, int m1 = 0, int m2 = 0) {
  TH1* hist = (TH1*) file->Get(hname);
  if (!hist) {
    if (verbose) cout << "[getYieldAndError] Cannot find yield hist " << hname << "  in file " << file->GetName() << " !! returning 0!\n";
    return 0;
  }
  if (metbin > hist->GetNbinsX()) {
    cout << "[getYieldAndError] Request bin " << metbin << " exceed the maximum " << hist->GetNbinsX() << " for " << hname << " in file " << file->GetName() << " !!\n";
    return 0;
  }

  double yield = 0;
  if (hname.Contains("hSMS")) {
    int biny = hist->GetYaxis()->FindBin(m1);
    int binz = hist->GetZaxis()->FindBin(m2);
    yield = hist->GetBinContent(metbin, biny, binz);
    error = (yield > 0)? (1 + hist->GetBinError(metbin, biny, binz) / yield) : 1;
  } else {
    yield = hist->GetBinContent(metbin);
    error = (yield > 0)? (1 + hist->GetBinError(metbin) / yield) : 1;
  }

  return yield;
}

// up and down variations given - safe both, don't need to worry which one is bigger
// note: don't allow for >100% uncertainties for each source
int getUncertainties(double& errup, double& errdn, double origyield, TFile* file, TString hname, int metbin, int m1 = 0, int m2 = 0) {
  // upshape   = origyield + upvariation   --> want 1 + upvariation   / origyield
  // downshape = origyield + downvariation --> want 1 + downvariation / origyield, where downvariation < 0
  errup = 1; errdn = 1;
  if (origyield <= 0) return 0;

  TH1* histUp = (TH1*) file->Get(hname+"Up");
  TH1* histDn = (TH1*) file->Get(hname+"Dn");
  if (!histUp || !histDn) {
    if (verbose) cout << "[getUncertainties] Cannot find the up or down syst for: " << hname << " in file: " << file->GetName() << " !! skipping!\n";
    return 0;
  }

  if (hname.Contains("hSMS")) {
    int biny = histUp->GetYaxis()->FindBin(m1);
    int binz = histUp->GetZaxis()->FindBin(m2);
    errup = histUp->GetBinContent(metbin, biny, binz) / origyield;
    errdn = histDn->GetBinContent(metbin, biny, binz) / origyield;
  } else {
    errup = histUp->GetBinContent(metbin) / origyield;
    errdn = histDn->GetBinContent(metbin) / origyield;
  }

  if (errdn > 0 && errup <= 0) {
    cout << "Switched " << file->GetName() << ":" << hname << (m1? Form("Up for (%d_%d)", m1, m2) : "Up") << " from " << errup << " to " << -1 << endl;
    errup = -1;
  }
  bool onereturn = false;
  if (std::isnan(errup)) { errup = -1; onereturn = true; }
  if (std::isnan(errdn)) { errdn = -1; onereturn = true; }
  if (onereturn) return -1;
  if (errup <= 0 && errup != -1) errup = 0.001;
  else if (errup > 2) errup = 2;
  if (errdn <= 0 && errdn != -1) errdn = 0.001;
  else if (errdn>2) errdn = 2;
  if (errup == 1 && errdn == 1) return 0;
  return 1;
}

int addCorrelatedUnc(std::ostringstream *fLogStream, string name, double* d, double* u, int bin, string unctype){
  bool isgmN = (unctype.find("gmN") != string::npos);
  bool allneg = true;
  for (unsigned int i = 0; i<maxnprocess; ++i) {
    if (d[i] >= 0 && d[i] != 1)
      allneg = false; // check if there is at least one good uncertainty
    if (isgmN && u[i] < 0)
      return 0; // all defined u's have to be >=0
  }
  if (allneg) return 0;

  if (bin > 0) name += to_string(bin);
  *fLogStream << " " << left << setw(20) << name;
  *fLogStream << " "  << setw(6) << unctype << " ";
  if (isgmN) *fLogStream << int(d[0]) << " ";

  for (int i = 0; i < maxnprocess; ++i) {
    if (isgmN)                 *fLogStream << u[i] << " ";
    else if (d[i]>=0&&u[i] <0) *fLogStream << d[i] << " ";
    else if (d[i]==0&&u[i]==0) *fLogStream << "- ";
    else if (d[i]==1&&u[i]==1) *fLogStream << "- ";
    else if (d[i]>=0&&u[i]>=0) *fLogStream << d[i] << "/" << u[i] << " ";
    else                       *fLogStream << "- ";
  }
  *fLogStream << endl;

  for (unsigned int i = 0; i<maxnprocess; ++i) {
    u[i] = -1;
    d[i] = -1;
  }
  return 1;
}

int addOneUnc(std::ostringstream *fLogStream, string name, double d, double u, int process, int bin, string unctype){
  if (d<0) return 0;
  if (d>0&&u==0) cout << name << " " << d << " " << u << endl;//return;
  if (d==0&&u==0) return 0;
  if (d==1&&u==1) return 0;
  if (d==1&&u<0) return 0;
  if (unctype.find(string("gmN")) != string::npos && u<0) return 0;

  if (bin > 0) name += to_string(bin);
  *fLogStream << " " << left << setw(20) << name;
  *fLogStream << " " << setw(6) << unctype << " ";
  if (unctype.find(string("gmN")) != string::npos) {
    *fLogStream << int(d) << " ";
    d = -1;
  }
  for (int i = 0; i<process; ++i) *fLogStream << "- ";
  if (d==0) { d = 0.001; }
  if (d<0)      *fLogStream << u << " ";//gmN
  else if (u<0) *fLogStream << d << " ";//only one uncertainty
  else         *fLogStream << d << "/" << u << " ";
  for (int i = process + 1; i<maxnprocess; ++i) *fLogStream << "- ";
  *fLogStream << endl;
  return 1;
}

int makeCardForOneBin(TString dir, int mstop, int mlsp, int metbin, int bin, TString cardname) {

  TString hname = dir + "/h_metbins";
  TString hname_2l = hname + "_"; // + "_2lep_";
  TString hname_1l = hname + "_"; // + "_1lepW_";
  TString hname_Z  = hname + "_"; // + "_Znunu_";
  // TString hname_sig = hname + signame(4, 12);  // signame format: "T2tt_1200_900"
  TString hname_sig = dir + "/hSMS_metbins";

  // constants uncertainties
  // TODO: numbers to be updated for new analysis
  double triggerrDn = 0.98;
  if (bin == 1 || bin == 4 || bin == 8 || bin == 11)  // TODO: To use better scheme for these
    triggerrDn = 0.95;
  double triggerrUp  = 1.01;
  double lumierr     = 1.025;
  double lepvetoerr  = 1.03;

  // these are the yields and statistical uncertainties
  double sig(0.), sigerr(1.);
  double sig1(0.), sig2(1.);
  double osig(0.); // keep original without signal contamination correction
  double data(0.), dataerr(0);
  double bg2l(0.), bg2lerr(1.), bg1l(0.), bg1lerr(1.), bg1ltop(0.), bg1ltoperr(1.), bgznunu(0.), bgznunuerr(1.);

  // start reading yields
  sig = getYieldAndError(sigerr, fsig, hname_sig, metbin, mstop, mlsp);
  osig = sig; sig1 = sig;

  if (dosigcont) {
    auto getSigContainmination = [&](TFile* fsig, TFile* fbkg, TString crname) {
      double sigcont(0.);
      TString hnameCR_sig = TString(hname_sig).ReplaceAll("sr", crname);
      TString hname_alpha = TString(hname).ReplaceAll("metbins", "alphaHist");
      int extr_start_bin = getYield(fbkg, "h_extrstart", 1);
      if (extr_start_bin > 0 && extr_start_bin <= metbin) {
        // doing met extrapolation for this bin
        sigcont = getYield(fsig, hnameCR_sig, extr_start_bin, mstop, mlsp, -1);
        sigcont *= getYield(fbkg, hname_alpha, metbin);
      } else {
        sigcont = getYield(fsig, hnameCR_sig, metbin, mstop, mlsp);
        sigcont *= getYield(fbkg, hname_alpha, metbin);
      }
      return sigcont;
    };

    double sigcont_cr2l = getSigContainmination(fsig, f2l, "cr2l");
    double sigcont_cr0b = getSigContainmination(fsig, f1l, "cr0b");
    sig1 = sig - sigcont_cr2l - sigcont_cr0b;
    sig1 = TMath::Max(0., sig1);
    sig = sig1;

    // do the average of yield between the normal and genmet yields
    if (dogenmet && fsig_genmet) {
      sig2 = getYield(fsig_genmet, hname_sig, metbin, mstop, mlsp);
      double sigcont_cr2l = getSigContainmination(fsig_genmet, f2l, "cr2l");
      double sigcont_cr0b = getSigContainmination(fsig_genmet, f1l, "cr0b");
      sig2 = sig2 - sigcont_cr2l - sigcont_cr0b;
      sig2 = TMath::Max(0., sig2);
      sig = (sig1 + sig2) / 2;
    }
  }

  data = getYieldAndError(dataerr, fdata, hname, metbin);
  bg2l = getYieldAndError(bg2lerr, f2l, hname + "_dataStats", metbin);  // <-- look up for the errors
  bg1l = getYieldAndError(bg1lerr, f1l, hname + "_dataStats", metbin);
  bg1ltop = getYieldAndError(bg1ltoperr, f1ltop, hname, metbin);
  bgznunu = getYieldAndError(bgznunuerr, fznunu, hname, metbin);

  bg1ltoperr = 2;  // simply put 100% uncertainty on 1ltop, as is estimate from MC only

  // now the important part - get the datacard
  std::ostringstream* fLogStream = new std::ostringstream;
  std::ostringstream* fLogStreamHeader = new std::ostringstream;

  int numnuis = 0;
  if (fakedata) data = bg2l + bg1l + bg1ltop + bgznunu;

  *fLogStream << "------------" << endl << "# observations " << endl;
  *fLogStream << "bin         " << bin << endl;
  *fLogStream << "observation " << data << endl; // use float for data as to check against exp for fake data
  *fLogStream << "------------" << endl;
  *fLogStream << "#now we list all expected number of events" << endl;
  *fLogStream << "bin          " << bin << "       " << bin << "       " << bin << "       " << bin << "       " << bin << "       " << endl;
  *fLogStream << "process     sig       2l      1lW       1ltop       znunu" << endl;
  *fLogStream << "process      0        1        2          3           4  " << endl;
  *fLogStream << "rate        " << sig << "  " << bg2l << "  " << bg1l << "  " << bg1ltop << "  " << bgznunu << endl;
  *fLogStream << "------------" << endl;

  // next ALL control region statistical uncertainties
  if (dosigcont) {
    double genmetunc = 1. + fabs(sig1-sig) / (2.*sig); // <-- careful for sig == 0.
    if (fsig_genmet && (fabs(fabs(sig1-sig)-fabs(sig2-sig)) > 0.001)) cout << "This should not happen " << fabs(sig1-sig) << " " << fabs(sig2-sig) << endl;
    if (genmetunc != 1) {
      numnuis += addOneUnc(fLogStream, "SigGenMETunc", genmetunc, -1, 0,  bin, "lnU");
    }
  }

  // signal systematic uncertainties (or general - therefore add no "Sig" prefix)
  numnuis += addOneUnc(fLogStream, "SigStat",     sigerr,     -1, 0, bin, "lnN"); // <-- revisit the Stats later

  if (!nosigsyst && sig > 0) {
    // vector<string> systNamesCard = {"PUSyst", "BLFSyst", "BHFSyst", "JESSyst", "ISRSyst", "LEffSyst", "LEffFSSyst", "BFSSyst", "MuRFSyst"};
    vector<string> systNames_sig = {"pileup", "bTagEffLF", "bTagEffHF", "jes", "ISR", "lepSF", "lepFSSF", "bTagFSEff"}; //  "q2" <-- fix this
    for (string syst : systNames_sig) {
      double systUp, systDn;
      getUncertainties(systUp, systDn, osig, fsig, hname_sig+"_"+syst.c_str(), metbin, mstop, mlsp);
      numnuis += addOneUnc(fLogStream, syst+"SystSig", systDn, systUp, 0,  -1, "lnN");
    }
    numnuis += addOneUnc(fLogStream, "lepVetoSyst", lepvetoerr, -1, 0, bin, "lnN");
  }

  // Statistical uncertainties
  if (bg2l > 0) {
    numnuis += addOneUnc(fLogStream, "Bg2lDataStat", bg2lerr, -1, 1, bin, "lnN");
    getYieldAndError(bg2lerr, f2l, hname + "_MCStats", metbin);
    numnuis += addOneUnc(fLogStream, "Bg2lMCStat",   bg2lerr, -1, 1, bin, "lnN");
  }
  if (bg1l > 0) {
    numnuis += addOneUnc(fLogStream, "Bg1lDataStat", bg1lerr, -1, 2, bin, "lnN");
    getYieldAndError(bg1lerr, f1l, hname + "_MCStats", metbin);
    numnuis += addOneUnc(fLogStream, "Bg1lMCStat",   bg1lerr, -1, 2, bin, "lnN");
  }
  if (bg1ltop > 0) numnuis += addOneUnc(fLogStream, "Bg1lTopSyst", bg1ltoperr, -1, 3, bin, "lnN");
  if (bgznunu > 0) numnuis += addOneUnc(fLogStream, "BgZnunuStat", bgznunuerr, -1, 4, bin, "lnN");

  if (!nobgsyst) {
    // common systematic uncertainties
    vector<string> systNames_corr = {"bTagEffHF", "bTagEffLF", "lepSF", "pdf", "q2", "jes", "ISR", "pileup", "alphas"};
    // individual systematic uncertainties for different backgrounds
    vector<string> systNames_bg2l = {"metRes", "ttbarSysPt", "tauSF"};
    vector<string> systNames_bg1l = {"metRes", "CRpurity", "WbXsec"};
    vector<string> systNames_bgZnunu;

    double dnerr[] = {triggerrDn, -1, -1, triggerrDn, triggerrDn }; // obviously(?) need to swap CR2l sf
    double uperr[] = {triggerrUp, -1, -1, triggerrUp, triggerrUp }; // why 1l[2] has been left out?
    getUncertainties(uperr[1], dnerr[1], bg2l, f2l, hname_2l + "cr2lTriggerSF", metbin);
    numnuis += addCorrelatedUnc(fLogStream, "TrigSyst", dnerr, uperr, -1, "lnN"); // err array are reset after addCorrelatedUnc()

    dnerr[0] = lumierr; dnerr[3] = lumierr; dnerr[4] = lumierr;
    numnuis += addCorrelatedUnc(fLogStream, "LumiSyst", dnerr, uperr, -1, "lnN");

    if (correlated) {
      for (auto syst : systNames_corr) {
        getUncertainties(uperr[1], dnerr[1], bg2l, f2l, hname_2l + syst.c_str(), metbin); // doing pdf?
        if (syst != "lepSF")  // bg1l
          getUncertainties(uperr[2], dnerr[2], bg1l, f1l, hname_1l + syst.c_str(), metbin);
        // bgZnunu <-- uses both up and dn err for Znunu bkg until otherwise instructed
        getUncertainties(uperr[4], dnerr[4], bgznunu, fznunu, hname_Z + syst.c_str(), metbin);
        numnuis += addCorrelatedUnc(fLogStream, syst+"SystBG", dnerr, uperr, -1, "lnN");
      }
    } else {
      for (auto syst : systNames_corr) {
        if (syst != "pdf") systNames_bg2l.push_back(syst);    // bg2l
        if (syst != "lepSF" && syst != "ISR") systNames_bg1l.push_back(syst);  // bg1l
        systNames_bgZnunu.push_back(syst);                    // bgZnunu
      }
    }
    // bg2l
    for (string syst : systNames_bg2l) {
      double systUp, systDn;
      getUncertainties(systUp, systDn, bg2l, f2l, hname_2l+syst.c_str(), metbin);
      numnuis += addOneUnc(fLogStream, syst+"Syst2l", systDn, systUp, 0,  -1, "lnN");
    }
    // bg1l
    for (string syst : systNames_bg1l) {
      double systUp, systDn;
      getUncertainties(systUp, systDn, bg1l, f1l, hname_1l+syst.c_str(), metbin);
      numnuis += addOneUnc(fLogStream, syst+"Syst1l", systDn, systUp, 2,  -1, "lnN");
    }
    // bgZnunu
    for (string syst : systNames_bgZnunu) {
      double systUp, systDn;  // <-- use both up and down error here until otherwise instructed
      getUncertainties(systUp, systDn, bgznunu, fznunu, hname_Z+syst.c_str(), metbin);
      numnuis += addOneUnc(fLogStream, syst+"SystZ", systDn, systUp, 4,  -1, "lnN");
    }
  }
  *fLogStream << endl;

  // now correctly counted nuisances
  *fLogStreamHeader << "# Datacard for Stop-1l Analys at signal region " << dir << " and metbin " << metbin << endl;
  *fLogStreamHeader << "imax  " << 1  << "  number of channels" << endl; // single bin
  *fLogStreamHeader << "jmax  " << 4  << "  number of backgrounds" << endl;
  *fLogStreamHeader << "kmax "  << numnuis << "  number of nuisance parameters" << endl;

  ofstream f_log(cardname.Data(), ios::trunc);
  f_log << fLogStreamHeader->str();
  f_log << fLogStream->str();
  if (verbose) cout << "[cardMaker] Wrote results into  " << cardname <<  " (old file replaced if exists)" << endl;
  delete fLogStream;
  delete fLogStreamHeader;

  return 1;
}


void makeCardsForPoint(TString signal, int mstop, int mlsp, TString outdir) {

  system(Form("mkdir -p %s", outdir.Data()));
  int nbintot = 1;
  // Loop through list of every directory in the signal file.
  // if directory begins with "sr", excluding "srbase", make cards for it.
  TList* listOfDirs = fsig->GetListOfKeys();
  string keep = "sr";
  string skip = "srbase";
  for (auto k : *listOfDirs) {
    TString dir = k->GetName();
    if (dir == "srJ") break;
    if (strncmp (dir, skip.c_str(), skip.length()) == 0) continue;
    if (strncmp (dir, keep.c_str(), keep.length()) == 0) { // it is a signal region
      TString hname = dir + "/h_metbins"; // for met binning information, empty hist for signal output
      // cout << "Looking at hname  " << hname << endl;
      TH1D* hist = (TH1D*) fdata->Get(hname);
      if (!hist) { cout << "Cannot find yield hist " << hname << " in " << fdata->GetName() << endl; continue; }
      int n_metbins = hist->GetNbinsX();
      for (int ibin = 1; ibin <= n_metbins; ++ibin, ++nbintot) {
        // Make a separate card for each met bin.
        TString anName = (dir.Contains("srI"))? "compressed" : "std";
        TString cardname = outdir + Form("/datacard_%s_%s_%d_%d_bin%d.txt", anName.Data(), signal.Data(), mstop, mlsp, nbintot);
        makeCardForOneBin(dir, mstop, mlsp, ibin, nbintot, cardname);
      }
    }
  }
}

// -------------------------------------------------------------------------------------------------------------------
// Make cards for a single mass point
void newCardMaker(string signal = "T2tt", int mStop = 800, int mLSP = 400, string input_dir="../StopLooper/output/temp", string output_dir="datacards/temp") {
  cout << "Making cards for single mass point: " << signal << endl;
  system(Form("mkdir -p %s", output_dir.c_str()));

  // set input files (global pointers)
  // fsig = new TFile(Form("%s/SMS_%s.root",input_dir.c_str(), signal.substr(0,4).c_str()));
  fsig = new TFile(Form("%s/Signal_%s.root",input_dir.c_str(), signal.substr(0,4).c_str()));
  f2l = new TFile(Form("%s/lostlepton.root",input_dir.c_str()));
  f1l = new TFile(Form("%s/1lepFromW.root",input_dir.c_str()));
  f1ltop = new TFile(Form("%s/1lepFromTop.root",input_dir.c_str()));
  fznunu = new TFile(Form("%s/ZToNuNu.root",input_dir.c_str()));
  // fsig_genmet = nullptr; // don't use switched genmet signal for now
  fsig_genmet = new TFile(Form("%s/Signal_%s_gen.root",input_dir.c_str(), signal.substr(0,4).c_str()));

  if (!fakedata) fdata = new TFile(Form("%s/allData_25ns.root",input_dir.c_str()));
  else fdata = new TFile(Form("%s/allBkg_25ns.root",input_dir.c_str()));

  if ( f2l->IsZombie() || f1l->IsZombie() ||  f1ltop->IsZombie() || fznunu->IsZombie() || fsig->IsZombie() || fdata->IsZombie() ) {
    std::cout << "Input file does not exist" << std::endl;
    return;
  }

  makeCardsForPoint(signal.c_str(), mStop, mLSP, output_dir.c_str());
}


// -------------------------------------------------------------------------------------------------------------------
// Make cards for all signal mass points through scan
int newCardMaker(string signal, string input_dir="../StopLooper/output/temp", string output_dir="datacards/scan_temp") {
  cout << "Making cards for " << signal  << " scan!" << endl;
  system(Form("mkdir -p %s", output_dir.c_str()));

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

  // set input files (global pointers)
  // fsig = new TFile(Form("%s/SMS_%s.root",input_dir.c_str(), signal.c_str()));
  fsig = new TFile(Form("%s/Signal_%s.root",input_dir.c_str(), signal.c_str()));
  f2l = new TFile(Form("%s/lostlepton.root",input_dir.c_str()));
  f1l = new TFile(Form("%s/1lepFromW.root",input_dir.c_str()));
  f1ltop = new TFile(Form("%s/1lepFromTop.root",input_dir.c_str()));
  fznunu = new TFile(Form("%s/ZToNuNu.root",input_dir.c_str()));
  // fsig_genmet = nullptr; // don't use switched genmet signal for now
  fsig_genmet = new TFile(Form("%s/Signal_%s_gen.root",input_dir.c_str(), signal.c_str()));

  if (!fakedata) fdata = new TFile(Form("%s/allData_25ns.root",input_dir.c_str()));
  else fdata = new TFile(Form("%s/allBkg_25ns.root",input_dir.c_str()));

  if ( f2l->IsZombie() || f1l->IsZombie() ||  f1ltop->IsZombie() || fznunu->IsZombie() || fsig->IsZombie() || fdata->IsZombie() ) {
    std::cout << "Input file does not exist" << std::endl;
    return -1;
  }

  verbose = false;  // make sure no exccessive printing
  set<pair<int, int> > signal_points;

  TH2D* hpoints = (TH2D*) fsig->Get("srbase/h2d_signal_masspts");
  if (!hpoints) {
    cout << "Cannot find signal mass points hist in " << fsig->GetName() << endl;
    return -1;
  }

  for (int im1 = 150; im1 <= 1250; im1 += 25) {
    for (int im2 = 0; im2 <= 750; im2 += 25) {
      // if (im1 < 900 && im2 < 400) continue;
      // if (im1 - im2 < 400) continue;
      if (hpoints->GetBinContent(hpoints->FindBin(im1, im2)) == 0) continue;
      if (im2 == 0) im2 = 1;
      cout << "Making cards for point: " << Form("%s_%d_%d", signal.c_str(), im1, im2) << endl;
      makeCardsForPoint(signal.c_str(), im1, im2, output_dir.c_str());
      signal_points.insert( make_pair(im1,im2) );
      if (im2 == 1) im2 = 0;
    } // scanM2 loop
  } // scanM1 loop

  TString filename = Form("%s/points_%s.txt", output_dir.c_str(), signal.c_str());
  ofstream ofile;
  ofile.open(filename);
  cout << "--------------------------------------------" << endl;
  // << "- saw nonzero signal entries for the following points: " << endl;
  for (set<pair<int,int> >::iterator it=signal_points.begin(); it!=signal_points.end(); ++it) {
    ofile << signal << "_" << (*it).first << "_" << (*it).second << endl;
  }
  cout << "Wrote point list to file  " << filename << endl;
  ofile.close();

  bmark->Stop("benchmark");
  cout << endl;
  cout << "------------------------------" << endl;
  cout << "CPU  Time:   " << Form( "%.01f s", bmark->GetCpuTime("benchmark")  ) << endl;
  cout << "Real Time:   " << Form( "%.01f s", bmark->GetRealTime("benchmark") ) << endl;
  cout << endl;
  delete bmark;

  return 0;
}
