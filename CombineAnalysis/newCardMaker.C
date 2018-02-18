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

// switches
bool verbose = true;
// do correlated syst for bkg errors
const bool correlated = true;
const bool fakedata = true;
const bool nobgsyst = true;
const bool nosigsyst = true;
const bool dropsigcont = true;
const bool dogenmet = false;

// global file pointers
TFile *fsig;
TFile *f2l;
TFile *f1l;
TFile *f1ltop;
TFile *fznunu;
TFile *fdata;
TFile *fsig_genmet;

// generic function to get yield from histograms, and error in the card format 1 + relerr
double getYieldAndError(double& error, TFile* file, TString hname, int metbin) {
  TH1D* hist = (TH1D*) file->Get(hname);
  if (!hist) {
    if (verbose) cout << "[getYieldAndError] Cannot find yield hist " << hname << "  in file " << file->GetName() << " !! returning 0!\n";
    return 0;
  }
  double yield = hist->GetBinContent(metbin);
  error = (yield > 0)? (1 + hist->GetBinError(metbin) / yield) : 1;

  return yield;
}

// up and down variations given - safe both, don't need to worry which one is bigger
// note: don't allow for >100% uncertainties for each source
int getUncertainties(double& errup, double& errdn, double origyield, TFile* file, TString hname, int metbin) {
  // upshape   = origyield + upvariation   --> want 1 + upvariation   / origyield
  // downshape = origyield + downvariation --> want 1 + downvariation / origyield, where downvariation < 0
  errup = 1; errdn = 1;
  if (origyield <= 0) return 0;

  TH1D* histUp = (TH1D*) file->Get(hname+"Up");
  TH1D* histDn = (TH1D*) file->Get(hname+"Dn");
  if (!histUp || !histDn) {
    if (verbose) cout << "[getUncertainties] Cannot find the up or down syst for: " << hname << " in file: " << file->GetName() << " !! skipping!\n";
    return 0;
  }
  errup = histUp->GetBinContent(metbin) / origyield;
  errdn = histDn->GetBinContent(metbin) / origyield;

  if (errdn > 0 && errup <= 0) {
    cout << "switched for " << hname << "Up uncertainties " << errup << " to " << -1 << endl;
    errup = -1;
  }
  bool onereturn = false;
  if (std::isnan(errup)) { errup = -1; onereturn = true; }
  if (std::isnan(errdn)) { errdn = -1; onereturn = true; }
  if (onereturn) return -1;
  if (errup<0&&errup!=-1) errup = 0.0001;
  else if (errup>2) errup = 2;
  if (errdn<0&&errdn!=-1) errdn = 0.0001;
  else if (errdn>2) errdn = 2;
  if (errup==1&&errdn==1) return 0;
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
  if (d==0){ d = 0.001; }
  if (d<0)      *fLogStream << u << " ";//gmN
  else if (u<0) *fLogStream << d << " ";//only one uncertainty
  else         *fLogStream << d << "/" << u << " ";
  for (int i = process + 1; i<maxnprocess; ++i) *fLogStream << "- ";
  *fLogStream << endl;
  return 1;
}

int makeCardForOneBin(TString dir, int metbin, TString signame, int bin, TString outputdir) {

  TString hname = dir + "/h_metbins";
  TString hname_2l = hname + "_"; // + "_2lep_";
  TString hname_1l = hname + "_"; // + "_1lepW_";
  TString hname_Z  = hname + "_"; // + "_Znunu_";
  TString hname_sig = hname + signame(4, 12);  // signame format: "T2tt_1200_900"

  // some constants
  double triggerrDn = 0.98;
  if (bin == 1 || bin == 4 || bin == 8 || bin == 11)
    triggerrDn = 0.95;
  double triggerrUp  = 1.01; //also on any other simulation
  double lumierr     = 1.062; //also on any other simulation
  double lepvetoerr  = 1.03;

  // these are the yields and statistical uncertainties
  double sig(0.), sigerr(1.);
  double sig1(0.), sig2(1.), sig1err(1.), sig2err(1.);
  double osig(0.); // keep original without signal contamination correction
  double data(0.), dataerr(0);
  double bg2l(0.), bg2lerr(1.), bg1l(0.), bg1lerr(1.), bg1ltop(0.), bg1ltoperr(1.), bgznunu(0.), bgznunuerr(1.);

  // start reading yields
  sig = getYieldAndError(sigerr, fsig, hname_sig, metbin);
  osig = sig; sig1 = sig;

  if (!dropsigcont) {
    TString hname_cr2l_sig = TString(hname_sig).ReplaceAll("sr", "cr2l");
    TString hname_cr0b_sig = TString(hname_sig).ReplaceAll("sr", "cr0b");
    double sig_cr2l = getYieldAndError(sig1err, fsig, hname_cr2l_sig, metbin);
    double sig_cr0b = getYieldAndError(sig1err, fsig, hname_cr0b_sig, metbin);
    sig1 = sig - sig_cr2l - sig_cr0b;
    sig1 = TMath::Max(0., sig1);
    sig = sig1;

    // this is the yield deal with the genmet switched samples? I guess
    if (dogenmet && fsig_genmet) {
      sig2 = getYieldAndError(sig2err, fsig_genmet, hname_sig, metbin);
      double sig2_cr2l = getYieldAndError(sig1err, fsig_genmet, hname_cr2l_sig, metbin);
      double sig2_cr0b = getYieldAndError(sig1err, fsig_genmet, hname_cr0b_sig, metbin);
      sig2 = sig2 - sig2_cr2l - sig2_cr0b;
      sig2 = TMath::Max(0., sig2);
      sig = (sig1 + sig2) / 2;
    }
  }

  data = getYieldAndError(dataerr, fdata, hname, metbin);
  bg2l = getYieldAndError(bg2lerr, f2l, hname, metbin);  // <-- look up for the errors
  bg1l = getYieldAndError(bg1lerr, f1l, hname, metbin);
  bg1ltop = getYieldAndError(bg1ltoperr, f1ltop, hname, metbin);
  bgznunu = getYieldAndError(bgznunuerr, fznunu, hname, metbin);

  bg1ltoperr = 2;  // - no statistical uncertainty on 1ltop! <-- Why??

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
  if (!dropsigcont) {
    double genmetunc = 1. + fabs(sig1-sig) / (2.*sig); // <-- careful for sig == 0.
    if (fsig_genmet && (fabs(fabs(sig1-sig)-fabs(sig2-sig)) > 0.001)) cout << "This should not happen " << fabs(sig1-sig) << " " << fabs(sig2-sig) << endl;
    if (genmetunc != 1) {
      *fLogStream << "SigGenMETunc" << bin << "   lnU " << genmetunc << " -  -  -  -" << endl;
      ++numnuis;
    }
  }

  // signal systematic uncertainties (or general - therefore add no "Sig" prefix)
  numnuis += addOneUnc(fLogStream, "SigStat",     sigerr,     -1, 0, bin, "lnN"); // <-- revisit the Stats later

  if (!nosigsyst && sig > 0) {
    // vector<string> systNamesCard = {"PUSyst", "BLFSyst",   "BHFSyst",   "JESSyst", "ISRSyst", "LEffSyst", "LEffFSSyst",}; // "BFSSyst", "MuRFSyst",
    vector<string> systNames_sig = {"pileup", "bTagEffLF", "bTagEffHF", "jes",     "ISR",     "lepSF",    "lepFSSF",   }; // "BFSSyst", "MuRFSyst",
    for (string syst : systNames_sig) {
      double systUp, systDn;
      getUncertainties(systUp, systDn, osig, fsig, hname_sig+"_"+syst.c_str(), metbin);
      numnuis += addOneUnc(fLogStream, syst+"SystSig", systDn, systUp, 0,  -1, "lnN");
    }
    numnuis += addOneUnc(fLogStream, "lepVetoSyst", lepvetoerr, -1, 0, bin, "lnN");
  }

  if (bg2l > 0)    numnuis += addOneUnc(fLogStream, "Bg2lStat",    bg2lerr,    -1, 1, bin, "lnN");
  if (bg1l > 0)    numnuis += addOneUnc(fLogStream, "Bg1lStat",    bg1lerr,    -1, 2, bin, "lnN");
  if (bg1ltop > 0) numnuis += addOneUnc(fLogStream, "Bg1lTopStat", bg1ltoperr, -1, 3, bin, "lnN");
  if (bgznunu > 0) numnuis += addOneUnc(fLogStream, "BgZnunuStat", bgznunuerr, -1, 4, bin, "lnN");

  if (!nobgsyst) {
    // common systematic uncertainties
    vector<string> systNames_corr = {"bTagEffHF", "bTagEffLF", "lepSF", "pdf", "q2", "jes",};
    // individual systematic uncertainties for different backgrounds
    vector<string> systNames_bg2l = {"ttbarSysPt", "alphas"};  // + MCStats
    vector<string> systNames_bg1l = {"metRes"};                // + MCStats + Wb + Cont
    vector<string> systNames_bgZnunu = {"topPtSF", "pileup"};

    double dnerr[] = {triggerrDn, -1, -1, triggerrDn, triggerrDn }; // obviously(?) need to swap CR2l sf
    double uperr[] = {triggerrUp, -1, -1, triggerrUp, triggerrUp }; // why 1l[2] has been left out?
    getUncertainties(uperr[1], dnerr[1], bg2l, f2l, hname_2l + "cr2lTriggerSF", metbin);
    numnuis += addCorrelatedUnc(fLogStream, "TrigSyst2l", dnerr, uperr, -1, "lnN"); // err array are reset after addCorrelatedUnc()

    dnerr[0] = lumierr; dnerr[3] = lumierr; dnerr[4] = lumierr;
    numnuis += addCorrelatedUnc(fLogStream, "LumiSyst", dnerr, uperr, -1, "lnN");

    if (correlated) {
      for (auto syst : systNames_corr) {
        if (syst != "pdf")  // bg2l
          getUncertainties(uperr[1], dnerr[1], bg2l, f2l, hname_2l + syst.c_str(), metbin);
        if (syst != "lepSF")  // bg1l
          getUncertainties(uperr[2], dnerr[2], bg1l, f1l, hname_1l + syst.c_str(), metbin);
        // bgZnunu <-- uses both up and dn err for Znunu bkg until otherwise instructed
        getUncertainties(uperr[4], dnerr[4], bgznunu, fznunu, hname_Z + syst.c_str(), metbin);
        numnuis += addCorrelatedUnc(fLogStream, syst+"SystBG", dnerr, uperr, -1, "lnN");
      }
    } else {
      for (auto syst : systNames_corr) {
        if (syst != "pdf") systNames_bg2l.push_back(syst);    // bg2l
        if (syst != "lepSF") systNames_bg1l.push_back(syst);  // bg1l
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
  *fLogStreamHeader << "# My significance experiment" << endl;
  *fLogStreamHeader << "imax  " << 1  << "  number of channels" << endl; // single bin
  *fLogStreamHeader << "jmax  " << 4  << "  number of backgrounds" << endl;
  *fLogStreamHeader << "kmax "  << numnuis << "  number of nuisance parameters" << endl;

  // TString outputdir = "datacards/";
  // if (correlated)          outputdir += "correlated";
  // if (nosigsyst&&nobgsyst) outputdir += "_nounc";
  // else if (nosigsyst)      outputdir += "_nosigsyst";
  // else if (nobgsyst)       outputdir += "_nobgsyst";
  // if (dropsigcont)         outputdir += "_dropsigcont";
  // if (fakedata)            outputdir += "/fakedata/";
  system(Form("mkdir -p %s", outputdir.Data()));

  TString anName = (dir.Contains("srI"))? "compressed" : "std";
  TString logname = outputdir + "/datacard_" + anName + "_" + signame + "_bin" + to_string(bin) + ".txt";
  ofstream f_log(logname.Data(), ios::trunc);
  f_log << fLogStreamHeader->str();
  f_log << fLogStream->str();
  if (verbose) cout << "[cardMaker] Wrote results into  " << logname <<  " (old file replaced if exists)" << endl;
  delete fLogStream;
  delete fLogStreamHeader;

  return 1;
}


void makeCardsForPoint(TString sigpoint, TString outdir) {

  int nbintot = 0;
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
      TString hname = dir + "/h_metbins";
      // cout << "Looking at hname  " << hname << endl;
      TH1D* hist = (TH1D*) fsig->Get(hname);
      if (!hist) { cout << "Cannot find yield hist " << hname << " in " << fsig->GetName() << endl; continue; }
      int n_metbins = hist->GetNbinsX();
      for (int ibin = 1; ibin <= n_metbins; ++ibin) { 
        // Make a separate card for each met bin.
        makeCardForOneBin(dir, ibin, sigpoint, ++nbintot, outdir);
      }
    }
  }
}

// Make cards for a single mass point
void newCardMaker(int single_point, string signal="T2tt_800_400", string input_dir="../StopLooper/output/temp12", string output_dir="datacards/temp12") {
  cout << "Making cards for single mass point: " << signal << endl;
  system(Form("mkdir -p %s", output_dir.c_str()));

  // set input files (global pointers)
  fsig = new TFile(Form("%s/SMS_T2tt.root",input_dir.c_str()));
  f2l = new TFile(Form("%s/lostlepton.root",input_dir.c_str()));
  f1l = new TFile(Form("%s/1lepFromW.root",input_dir.c_str()));
  f1ltop = new TFile(Form("%s/1lepFromTop.root",input_dir.c_str()));
  fznunu = new TFile(Form("%s/ZToNuNu.root",input_dir.c_str()));
  fsig_genmet = nullptr; // don't use switched genmet signal for now

  if (!fakedata) fdata = new TFile(Form("%s/allData.root",input_dir.c_str()));
  else fdata = new TFile(Form("%s/allBkg_25ns.root",input_dir.c_str()));

  if ( f2l->IsZombie() || f1l->IsZombie() ||  f1ltop->IsZombie() || fznunu->IsZombie() || fsig->IsZombie() || fdata->IsZombie() ) {
    std::cout << "Input file does not exist" << std::endl;
    return;
  }

  makeCardsForPoint(signal.c_str(), output_dir.c_str());
}


// Make cards for all scan
void newCardMaker(string signal="T2tt", string input_dir="../StopLooper/output/temp13", string output_dir="datacards/scan_temp13") {
  cout << "Making cards for " << signal  << " scan!" << endl;
  system(Form("mkdir -p %s", output_dir.c_str()));

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

  // set input files (global pointers)
  fsig = new TFile(Form("%s/SMS_%s.root",input_dir.c_str(), signal.c_str()));
  f2l = new TFile(Form("%s/lostlepton.root",input_dir.c_str()));
  f1l = new TFile(Form("%s/1lepFromW.root",input_dir.c_str()));
  f1ltop = new TFile(Form("%s/1lepFromTop.root",input_dir.c_str()));
  fznunu = new TFile(Form("%s/ZToNuNu.root",input_dir.c_str()));
  fsig_genmet = nullptr; // don't use switched genmet signal for now
  // fsig_genmet = new TFile(Form("%s/Signal_%s_gen.root",input_dir.c_str(), signal.c_str()));

  if (!fakedata) fdata = new TFile(Form("%s/allData.root",input_dir.c_str()));
  else fdata = new TFile(Form("%s/allBkg_25ns.root",input_dir.c_str()));

  if ( f2l->IsZombie() || f1l->IsZombie() ||  f1ltop->IsZombie() || fznunu->IsZombie() || fsig->IsZombie() || fdata->IsZombie() ) {
    std::cout << "Input file does not exist" << std::endl;
    return;
  }

  bool is_scan = true;
  set<pair<int, int> > signal_points;

  if (is_scan) {
    verbose = false;
    for (int im1 = 600; im1 <= 1250; im1 += 25) {
      for (int im2 = 0; im2 <= 750; im2 += 25) {
        if (im1 < 900 && im2 < 400) continue;
        if (im1 - im2 < 400) continue;
        TString hname_sig = Form("srbase/h_metbins_%d_%d", im1, ((im2 == 0)? 1 : im2));
        TH1D* hpoint = (TH1D*) fsig->Get(hname_sig);
        // if (!hpoint) { cout << "Cannot find yield hist " << hname_sig << " in " << fsig->GetName() << endl; continue; }
        if (!hpoint) continue; 
        if (im2 == 0) im2 = 1;
        cout << "Making cards for point: " << Form("%s_%d_%d", signal.c_str(), im1, im2) << endl;
        makeCardsForPoint(Form("%s_%d_%d", signal.c_str(), im1, im2), output_dir.c_str());
        signal_points.insert( make_pair(im1,im2) );
        if (im2 == 1) im2 = 0;
      } // scanM2 loop
    } // scanM1 loop
  }

  if (is_scan) {
    TString filename = Form("%s/points_%s.txt", output_dir.c_str(), signal.c_str());
    ofstream ofile;
    ofile.open(filename);
    cout << "--------------------------------------------" << endl;
         // << "- saw nonzero signal entries for the following points: " << endl;
    for (set<pair<int,int> >::iterator it=signal_points.begin(); it!=signal_points.end(); ++it) {
      // cout << signal << "_" << (*it).first << "_" << (*it).second << endl;
      ofile << signal << "_" << (*it).first << "_" << (*it).second << endl;
    }
    cout << "Wrote point list to file  " << filename << endl;
    ofile.close();
  }

  bmark->Stop("benchmark");
  cout << endl;
  cout << "------------------------------" << endl;
  cout << "CPU  Time:   " << Form( "%.01f s", bmark->GetCpuTime("benchmark")  ) << endl;
  cout << "Real Time:   " << Form( "%.01f s", bmark->GetRealTime("benchmark") ) << endl;
  cout << endl;
  delete bmark;

  return 0;
}
