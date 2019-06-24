// -*- C++ -*-
#include "TList.h"
#include "TMath.h"
#include "TString.h"
#include "TObject.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TH2.h"
#include "TH3.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TH3F.h"
#include "TH2F.h"
#include "TH1F.h"
#include "TH3D.h"
#include "TH2D.h"
#include "TH1D.h"
#include "TSystem.h"
#include "TMap.h"
#include "TGraph.h"
#include "TGraph2D.h"
#include "TStopwatch.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <string>
#include <cmath>

using namespace std;

// void Make2DLimitHistos(TString signaltype, bool prefit, bool fakedata, bool nosigunc, bool nobkgunc, int xsecupdown, int compressed, bool dropsigcont, bool correlated);
TH2F* InterpolateThisHistogram(TH2F *hold);
TH2F* InterpolateThisHistogramNew(TH2F *hold);
TGraph2D* GetInterpolatingGraph(TH2F *hold);
TGraph2D* GetInterpolatingGraphNew(TH2F *hold);
TH2F* PassThisHistogram(TH2F *hold);
TH2F* XsecThisHistogram(TH2F *hold, TH1D *hxsec);
TGraph* GetContour(TGraph2D *g, TString name, TGraph *gempty);
TGraph* GetContour2(TGraph2D *g, TString name, TGraph *gempty);
inline TString MakeOutputDir(TString dir);

inline TString MakeOutputDir(TString dir){
  if(!dir.EndsWith("/")) dir += "/";
  // Create directory if needed
  char cmd[100];
  sprintf(cmd,"mkdir -p %s", dir.Data());
  system(cmd);
  return dir;
}

TString sigtype;

void Make2DLimitHistos(TString signaltype="std_T2tt", TString indir="limits", bool prefit=false){

  sigtype = signaltype;

  TH1D *hxsec;
  // TFile *fxsec = new TFile("xsec_stop_13TeV.root","READ");
  TFile *fxsec = new TFile("xsec_susy_13TeV_run2.root","READ");
  if(fxsec->IsZombie()) {
    std::cout << "Somehow xsec_susy_13tev_run2 is corrupted. Exit..." << std::endl;
    exit(0);
  }
  hxsec = (TH1D*)fxsec->Get("h_xsec_stop");
  
  int mStopLow  = 150;
  int mStopHigh = 2000;
  int mStopStep = 25;
  int mLSPLow   = 0;
  int mLSPHigh  = 1150;
  int mLSPStep  = 25;
  int binsize = 5;
  int nbinsx = (mStopHigh - mStopLow) / binsize + 1;
  int nbinsy = (mLSPHigh - mLSPLow) / binsize + 1;
  
  if (!indir.EndsWith("/")) indir += "/";
  cout << "inputs from " << indir << endl;
  TString outputdir = indir;
  outputdir += "histo/";
  cout << "make directory " << outputdir << endl;
  MakeOutputDir(outputdir);

  TString outfilename = outputdir + "Limits2DHistograms_"+signaltype+"_prefit.root";
  if(!prefit) outfilename = outputdir + "Limits2DHistograms_"+signaltype+"_postfit.root";
  TFile *file = new TFile(outfilename, "recreate");
  file->cd();
  double xlow  = mStopLow  - binsize / 2.0;
  double xhigh = mStopHigh + binsize / 2.0;
  double ylow  = mLSPLow  - binsize / 2.0;
  double yhigh = mLSPHigh + binsize / 2.0;
  TH2F *hExpOrg   = new TH2F("hExpOrg",   "hExp"  , nbinsx, mStopLow, mStopHigh, nbinsy, mLSPLow, mLSPHigh);
  TH2F *hObsOrg   = new TH2F("hObsOrg",   "hObs"  , nbinsx, mStopLow, mStopHigh, nbinsy, mLSPLow, mLSPHigh);
  TH2F *hObs1pOrg = new TH2F("hObs1pOrg", "hObs1p", nbinsx, mStopLow, mStopHigh, nbinsy, mLSPLow, mLSPHigh);
  TH2F *hObs1mOrg = new TH2F("hObs1mOrg", "hObs1m", nbinsx, mStopLow, mStopHigh, nbinsy, mLSPLow, mLSPHigh);
  TH2F *hExp1mOrg = new TH2F("hExp1mOrg", "hExp1m", nbinsx, mStopLow, mStopHigh, nbinsy, mLSPLow, mLSPHigh);
  TH2F *hExp2mOrg = new TH2F("hExp2mOrg", "hExp2m", nbinsx, mStopLow, mStopHigh, nbinsy, mLSPLow, mLSPHigh);
  TH2F *hExp1pOrg = new TH2F("hExp1pOrg", "hExp1p", nbinsx, mStopLow, mStopHigh, nbinsy, mLSPLow, mLSPHigh);
  TH2F *hExp2pOrg = new TH2F("hExp2pOrg", "hExp2p", nbinsx, mStopLow, mStopHigh, nbinsy, mLSPLow, mLSPHigh);
  TH2F *hExpXsecOrg   = new TH2F("hExpXsecOrg",   "hExp"  , nbinsx, mStopLow, mStopHigh, nbinsy, mLSPLow, mLSPHigh);
  TH2F *hObsXsecOrg   = new TH2F("hObsXsecOrg",   "hObs"  , nbinsx, mStopLow, mStopHigh, nbinsy, mLSPLow, mLSPHigh);
  TH2F *hObs1pXsecOrg = new TH2F("hObs1pXsecOrg", "hObs1p", nbinsx, mStopLow, mStopHigh, nbinsy, mLSPLow, mLSPHigh);
  TH2F *hObs1mXsecOrg = new TH2F("hObs1mXsecOrg", "hObs1m", nbinsx, mStopLow, mStopHigh, nbinsy, mLSPLow, mLSPHigh);
  TH2F *hExp1mXsecOrg = new TH2F("hExp1mXsecOrg", "hExp1m", nbinsx, mStopLow, mStopHigh, nbinsy, mLSPLow, mLSPHigh);
  TH2F *hExp2mXsecOrg = new TH2F("hExp2mXsecOrg", "hExp2m", nbinsx, mStopLow, mStopHigh, nbinsy, mLSPLow, mLSPHigh);
  TH2F *hExp1pXsecOrg = new TH2F("hExp1pXsecOrg", "hExp1p", nbinsx, mStopLow, mStopHigh, nbinsy, mLSPLow, mLSPHigh);
  TH2F *hExp2pXsecOrg = new TH2F("hExp2pXsecOrg", "hExp2p", nbinsx, mStopLow, mStopHigh, nbinsy, mLSPLow, mLSPHigh);
  TH2F *hExpPassOrg   = new TH2F("hExpPassOrg",   "hExp"  , nbinsx, mStopLow, mStopHigh, nbinsy, mLSPLow, mLSPHigh);
  TH2F *hObsPassOrg   = new TH2F("hObsPassOrg",   "hObs"  , nbinsx, mStopLow, mStopHigh, nbinsy, mLSPLow, mLSPHigh);
  TH2F *hObs1pPassOrg = new TH2F("hObs1pPassOrg", "hObs1p", nbinsx, mStopLow, mStopHigh, nbinsy, mLSPLow, mLSPHigh);
  TH2F *hObs1mPassOrg = new TH2F("hObs1mPassOrg", "hObs1m", nbinsx, mStopLow, mStopHigh, nbinsy, mLSPLow, mLSPHigh);
  TH2F *hExp1mPassOrg = new TH2F("hExp1mPassOrg", "hExp1m", nbinsx, mStopLow, mStopHigh, nbinsy, mLSPLow, mLSPHigh);
  TH2F *hExp2mPassOrg = new TH2F("hExp2mPassOrg", "hExp2m", nbinsx, mStopLow, mStopHigh, nbinsy, mLSPLow, mLSPHigh);
  TH2F *hExp1pPassOrg = new TH2F("hExp1pPassOrg", "hExp1p", nbinsx, mStopLow, mStopHigh, nbinsy, mLSPLow, mLSPHigh);
  TH2F *hExp2pPassOrg = new TH2F("hExp2pPassOrg", "hExp2p", nbinsx, mStopLow, mStopHigh, nbinsy, mLSPLow, mLSPHigh);
  TH2F *hExp   = new TH2F("hExp",   "hExp"  , nbinsx, mStopLow, mStopHigh, nbinsy, mLSPLow, mLSPHigh);
  TH2F *hObs   = new TH2F("hObs",   "hObs"  , nbinsx, mStopLow, mStopHigh, nbinsy, mLSPLow, mLSPHigh);
  TH2F *hObs1p = new TH2F("hObs1p", "hObs1p", nbinsx, mStopLow, mStopHigh, nbinsy, mLSPLow, mLSPHigh);
  TH2F *hObs1m = new TH2F("hObs1m", "hObs1m", nbinsx, mStopLow, mStopHigh, nbinsy, mLSPLow, mLSPHigh);
  TH2F *hExp1m = new TH2F("hExp1m", "hExp1m", nbinsx, mStopLow, mStopHigh, nbinsy, mLSPLow, mLSPHigh);
  TH2F *hExp2m = new TH2F("hExp2m", "hExp2m", nbinsx, mStopLow, mStopHigh, nbinsy, mLSPLow, mLSPHigh);
  TH2F *hExp1p = new TH2F("hExp1p", "hExp1p", nbinsx, mStopLow, mStopHigh, nbinsy, mLSPLow, mLSPHigh);
  TH2F *hExp2p = new TH2F("hExp2p", "hExp2p", nbinsx, mStopLow, mStopHigh, nbinsy, mLSPLow, mLSPHigh);
  TH2F *hExpXsec   = new TH2F("hExpXsec",   "hExp"  , nbinsx, mStopLow, mStopHigh, nbinsy, mLSPLow, mLSPHigh);
  TH2F *hObsXsec   = new TH2F("hObsXsec",   "hObs"  , nbinsx, mStopLow, mStopHigh, nbinsy, mLSPLow, mLSPHigh);
  TH2F *hObs1pXsec = new TH2F("hObs1pXsec", "hObs1p", nbinsx, mStopLow, mStopHigh, nbinsy, mLSPLow, mLSPHigh);
  TH2F *hObs1mXsec = new TH2F("hObs1mXsec", "hObs1m", nbinsx, mStopLow, mStopHigh, nbinsy, mLSPLow, mLSPHigh);
  TH2F *hExp1mXsec = new TH2F("hExp1mXsec", "hExp1m", nbinsx, mStopLow, mStopHigh, nbinsy, mLSPLow, mLSPHigh);
  TH2F *hExp2mXsec = new TH2F("hExp2mXsec", "hExp2m", nbinsx, mStopLow, mStopHigh, nbinsy, mLSPLow, mLSPHigh);
  TH2F *hExp1pXsec = new TH2F("hExp1pXsec", "hExp1p", nbinsx, mStopLow, mStopHigh, nbinsy, mLSPLow, mLSPHigh);
  TH2F *hExp2pXsec = new TH2F("hExp2pXsec", "hExp2p", nbinsx, mStopLow, mStopHigh, nbinsy, mLSPLow, mLSPHigh);
  TH2F *hExpPass   = new TH2F("hExpPass",   "hExp"  , nbinsx, mStopLow, mStopHigh, nbinsy, mLSPLow, mLSPHigh);
  TH2F *hObsPass   = new TH2F("hObsPass",   "hObs"  , nbinsx, mStopLow, mStopHigh, nbinsy, mLSPLow, mLSPHigh);
  TH2F *hObs1pPass = new TH2F("hObs1pPass", "hObs1p", nbinsx, mStopLow, mStopHigh, nbinsy, mLSPLow, mLSPHigh);
  TH2F *hObs1mPass = new TH2F("hObs1mPass", "hObs1m", nbinsx, mStopLow, mStopHigh, nbinsy, mLSPLow, mLSPHigh);
  TH2F *hExp1mPass = new TH2F("hExp1mPass", "hExp1m", nbinsx, mStopLow, mStopHigh, nbinsy, mLSPLow, mLSPHigh);
  TH2F *hExp2mPass = new TH2F("hExp2mPass", "hExp2m", nbinsx, mStopLow, mStopHigh, nbinsy, mLSPLow, mLSPHigh);
  TH2F *hExp1pPass = new TH2F("hExp1pPass", "hExp1p", nbinsx, mStopLow, mStopHigh, nbinsy, mLSPLow, mLSPHigh);
  TH2F *hExp2pPass = new TH2F("hExp2pPass", "hExp2p", nbinsx, mStopLow, mStopHigh, nbinsy, mLSPLow, mLSPHigh);
  

  cout << "First create 2d limit histograms" << endl;
  for(int im1 = mStopLow; im1<=mStopHigh; im1 += mStopStep){
    // string tarfile = "tar_Limits_Asymptotic_PostFit" + (string)signaltype.Data() + "_" + to_string(stop) + ".tar.gz";
    // if(prefit) tarfile = "tar_Limits_Asymptotic_PreFit" + (string)signaltype.Data() + "_" + to_string(stop) + ".tar.gz";
    // string cpcommand = "cp " + (string)myinputdir.Data() + tarfile + " temp/.";
    // string tarcommand = "tar --directory temp -xzvf temp/" + tarfile;
    // string rmcommand1 = "rm temp/" + tarfile;
    // string rmcommand2 = "rm temp/*.root";
    // system(cpcommand.c_str());
    // //system("ls");
    // system(tarcommand.c_str());
    // //system ("cd ..");
    // system(rmcommand1.c_str());
    int tcorpt = 0;
    for(int im2 = mLSPLow; im2<=mLSPHigh; im2 += mLSPStep){
      // if(signaltype.Contains("T2bW")&&stop==350&&lsp==100) continue;
      int stop = im1;
      int lsp = im2;
      if (lsp == 0) lsp = 1;
      if (stop - lsp < 0) continue;  // temporary
      if (stop-lsp == 75) lsp -= 12;
      if (stop-lsp == 175) {
        if (tcorpt == 1) stop -= 8;
        if (tcorpt == 2) stop += 8;
        tcorpt++;
      }
      TString limitfilebase = "Limits_Asymptotic_";
      if(prefit) limitfilebase = limitfilebase + "PreFit_";
      else       limitfilebase = limitfilebase + "PostFit_";
      TString signalname = signaltype + "_" + std::to_string(stop) + "_" + ((lsp == 0)? "1" : std::to_string(lsp));
      if(signalname.Contains("comb")){
        signalname.ReplaceAll("comb", "std");
        if (stop - lsp < 225) signalname.ReplaceAll("std", "tcor");
        if (stop - lsp <= 150) signalname.ReplaceAll("tcor", "Wcor");
        // if (stop - lsp <= 150) signalname.ReplaceAll("tcor", "srJ3");
        // if (stop - lsp < 151) continue;  // temporary
      }
      TString limitfile = indir + limitfilebase + signalname + ".root";
      ifstream infile(limitfile.Data());
      bool exists = infile.good();
      if(!exists) {
	// cout << "No limit name for " << signalname << " - exit" << endl;
        tcorpt = 0;
	continue;
      }
      cout << "Limit file exists for " << signalname << endl;
      double obs    = -1.0; //observed limit
      double obsm1s  = -1.0; //observed limit - 1 sigma theory
      double obsp1s = -1.0; //observed limit + 1 sigma theory
      double expm2s = -1.0; //expected - 2 sigma
      double expm1s = -1.0; //expected - 1 sigma
      double exp    = -1.0; //expected limit
      double expp1s = -1.0; //expected + 1 sigma
      double expp2s = -1.0; //expected + 2 sigma
      
      double xsec = hxsec->GetBinContent(hxsec->FindBin(stop));
      double xsecunc = hxsec->GetBinError(hxsec->FindBin(stop));
      TFile *flimit;
      flimit = new TFile(limitfile,"READ");
      if(flimit->IsZombie()) {
	flimit->Close();
	delete flimit;
	cout << "No limit file for " << signalname << " - exit" << endl;
	continue;
      }
      //so flimit exists
      flimit->cd();
      TTree *tlimit = (TTree*)flimit->Get("limit");
      if (!tlimit) {
        cout << "Limit tree is broken in " << signalname << " -- skipping!\n";
        continue;
      }
      double value = -1.0;
      tlimit->SetBranchAddress("limit", &value);
      
      for(int i=0; i< 6; i++){
	tlimit->GetEntry(i);
	//cout << "stop " << stop << " lsp " << lsp << " i " << i << " value " << value << endl;
	if(i==0)      expm2s = value;
	else if(i==1) expm1s = value;
	else if(i==2) exp    = value;
	else if(i==3) expp1s = value;
	else if(i==4) expp2s = value;
	else if(i==5) obs    = value;
      }
      delete tlimit;
      flimit->Close();
      delete flimit;

      file->cd();
      if (stop-lsp == 175-8) stop += 4;
      hExpOrg  ->Fill(stop,lsp,exp   );
      hExp2mOrg->Fill(stop,lsp,expm2s);
      hExp1mOrg->Fill(stop,lsp,expm1s);
      hExp1pOrg->Fill(stop,lsp,expp1s);
      hExp2pOrg->Fill(stop,lsp,expp2s);
      hObsOrg  ->Fill(stop,lsp,obs   );
      //xsecupdown
      hObs1pOrg->Fill(stop,lsp,obs*xsec/(xsec+xsecunc)   );
      hObs1mOrg->Fill(stop,lsp,obs*xsec/(xsec-xsecunc)   );
      if (tcorpt > 0 && tcorpt < 3) im2 -= mLSPStep;
    }//lsp
    // system(rmcommand2.c_str());
  }//stop
  cout << "Now interpolate these histograms" << endl;  
  hExp   = (TH2F*)InterpolateThisHistogramNew(hExpOrg);
  hExp2m = (TH2F*)InterpolateThisHistogramNew(hExp2mOrg);
  hExp1m = (TH2F*)InterpolateThisHistogramNew(hExp1mOrg);
  hExp1p = (TH2F*)InterpolateThisHistogramNew(hExp1pOrg);
  hExp2m = (TH2F*)InterpolateThisHistogramNew(hExp2mOrg);
  hObs   = (TH2F*)InterpolateThisHistogramNew(hObsOrg);
  hObs1m = (TH2F*)InterpolateThisHistogramNew(hObs1mOrg);
  hObs1p = (TH2F*)InterpolateThisHistogramNew(hObs1pOrg);

  TGraph2D *g2Exp   = (TGraph2D*)GetInterpolatingGraph(hExpOrg);
  TGraph2D *g2Exp2m = (TGraph2D*)GetInterpolatingGraph(hExp2mOrg);
  TGraph2D *g2Exp1m = (TGraph2D*)GetInterpolatingGraph(hExp1mOrg);
  TGraph2D *g2Exp1p = (TGraph2D*)GetInterpolatingGraph(hExp1pOrg);
  TGraph2D *g2Exp2p = (TGraph2D*)GetInterpolatingGraph(hExp2pOrg);
  TGraph2D *g2Obs   = (TGraph2D*)GetInterpolatingGraph(hObsOrg);
  TGraph2D *g2Obs1m = (TGraph2D*)GetInterpolatingGraph(hObs1mOrg);
  TGraph2D *g2Obs1p = (TGraph2D*)GetInterpolatingGraph(hObs1pOrg);
  
  cout << "Now get pass/fail histograms" << endl;
  hExpPass   = (TH2F*)PassThisHistogram(hExp);
  hExp2mPass = (TH2F*)PassThisHistogram(hExp2m);
  hExp1mPass = (TH2F*)PassThisHistogram(hExp1m);
  hExp1pPass = (TH2F*)PassThisHistogram(hExp1p);
  hExp2pPass = (TH2F*)PassThisHistogram(hExp2p);
  hObsPass   = (TH2F*)PassThisHistogram(hObs);
  hObs1mPass = (TH2F*)PassThisHistogram(hObs1m);
  hObs1pPass = (TH2F*)PassThisHistogram(hObs1p);
  hExpPassOrg   = (TH2F*)PassThisHistogram(hExpOrg);
  hExp2mPassOrg = (TH2F*)PassThisHistogram(hExp2mOrg);
  hExp1mPassOrg = (TH2F*)PassThisHistogram(hExp1mOrg);
  hExp1pPassOrg = (TH2F*)PassThisHistogram(hExp1pOrg);
  hExp2pPassOrg = (TH2F*)PassThisHistogram(hExp2pOrg);
  hObsPassOrg   = (TH2F*)PassThisHistogram(hObsOrg);
  hObs1mPassOrg = (TH2F*)PassThisHistogram(hObs1mOrg);
  hObs1pPassOrg = (TH2F*)PassThisHistogram(hObs1pOrg);
  cout << "Now get cross section histograms" << endl;
  hExpXsec   = (TH2F*)XsecThisHistogram(hExp, hxsec);
  hExp2mXsec = (TH2F*)XsecThisHistogram(hExp2m, hxsec);
  hExp1mXsec = (TH2F*)XsecThisHistogram(hExp1m, hxsec);
  hExp1pXsec = (TH2F*)XsecThisHistogram(hExp1p, hxsec);
  hExp2pXsec = (TH2F*)XsecThisHistogram(hExp2p, hxsec);
  hObsXsec   = (TH2F*)XsecThisHistogram(hObs, hxsec);
  hObs1mXsec = (TH2F*)XsecThisHistogram(hObs1m, hxsec);
  hObs1pXsec = (TH2F*)XsecThisHistogram(hObs1p, hxsec);
  hExpXsecOrg   = (TH2F*)XsecThisHistogram(hExpOrg, hxsec);
  hExp2mXsecOrg = (TH2F*)XsecThisHistogram(hExp2mOrg, hxsec);
  hExp1mXsecOrg = (TH2F*)XsecThisHistogram(hExp1mOrg, hxsec);
  hExp1pXsecOrg = (TH2F*)XsecThisHistogram(hExp1pOrg, hxsec);
  hExp2pXsecOrg = (TH2F*)XsecThisHistogram(hExp2pOrg, hxsec);
  hObsXsecOrg   = (TH2F*)XsecThisHistogram(hObsOrg, hxsec);
  hObs1mXsecOrg = (TH2F*)XsecThisHistogram(hObs1mOrg, hxsec);
  hObs1pXsecOrg = (TH2F*)XsecThisHistogram(hObs1pOrg, hxsec);
  //this is stupid, but I don't want empty 0 axis
  for(unsigned int i = 1; i<=hExpXsec->GetNbinsX(); ++i){
    if(hExpXsec->GetBinContent(i,1)==0 && hExpXsec->GetBinContent(i,2)>0) hExpXsec->SetBinContent(i,1,hExpXsec->GetBinContent(i,2));
  }
  for(unsigned int i = 1; i<=hExpXsec->GetNbinsY(); ++i){
    if(hExpXsec->GetBinContent(1,i)==0 && hExpXsec->GetBinContent(2,i)>0) hExpXsec->SetBinContent(1,i,hExpXsec->GetBinContent(2,i));
  }
  TGraph *gEmpty = new TGraph();//empty graph for suppressing obs in limit plots
  gEmpty->SetName("gEmpty");
  gEmpty->SetPoint(0,-100,-100);
  gEmpty->SetPoint(1,-200,-200);
  
  cout << "Get contours" << endl;
  TGraph *gExp_c   = (TGraph*)GetContour(g2Exp, "gExp", gEmpty);
  TGraph *gExp2m_c = (TGraph*)GetContour(g2Exp2m, "gExp2m", gEmpty);
  TGraph *gExp1m_c = (TGraph*)GetContour(g2Exp1m, "gExp1m", gEmpty);
  TGraph *gExp1p_c = (TGraph*)GetContour(g2Exp1p, "gExp1p", gEmpty);
  TGraph *gExp2p_c = (TGraph*)GetContour(g2Exp2p, "gExp2p", gEmpty);
  TGraph *gObs_c   = (TGraph*)GetContour(g2Obs, "gObs", gEmpty);
  TGraph *gObs1m_c = (TGraph*)GetContour(g2Obs1m, "gObs1m", gEmpty);
  TGraph *gObs1p_c = (TGraph*)GetContour(g2Obs1p, "gObs1p", gEmpty);
  TGraph *gExp_c2   = (TGraph*)GetContour2(g2Exp, "gExp_2", gEmpty);
  TGraph *gExp2m_c2 = (TGraph*)GetContour2(g2Exp2m, "gExp2m_2", gEmpty);
  TGraph *gExp1m_c2 = (TGraph*)GetContour2(g2Exp1m, "gExp1m_2", gEmpty);
  TGraph *gExp1p_c2 = (TGraph*)GetContour2(g2Exp1p, "gExp1p_2", gEmpty);
  TGraph *gExp2p_c2 = (TGraph*)GetContour2(g2Exp2p, "gExp2p_2", gEmpty);
  TGraph *gObs_c2   = (TGraph*)GetContour2(g2Obs, "gObs_2", gEmpty);
  TGraph *gObs1m_c2 = (TGraph*)GetContour2(g2Obs1m, "gObs1m_2", gEmpty);
  TGraph *gObs1p_c2 = (TGraph*)GetContour2(g2Obs1p, "gObs1p_2", gEmpty);

  
  file->cd();
  hExpOrg   ->Write();
  hObsOrg   ->Write();
  hObs1mOrg ->Write();
  hObs1pOrg ->Write();
  hExp1mOrg ->Write();
  hExp2mOrg ->Write();
  hExp1pOrg ->Write();
  hExp2pOrg ->Write();
  hExpXsecOrg   ->Write();
  hObsXsecOrg   ->Write();
  hObs1mXsecOrg ->Write();
  hObs1pXsecOrg ->Write();
  hExp1mXsecOrg ->Write();
  hExp2mXsecOrg ->Write();
  hExp1pXsecOrg ->Write();
  hExp2pXsecOrg ->Write();
  hExpPassOrg   ->Write();
  hObsPassOrg   ->Write();
  hObs1mPassOrg ->Write();
  hObs1pPassOrg ->Write();
  hExp1mPassOrg ->Write();
  hExp2mPassOrg ->Write();
  hExp1pPassOrg ->Write();
  hExp2pPassOrg ->Write();
  hExp   ->Write();
  hObs   ->Write();
  hObs1m ->Write();
  hObs1p ->Write();
  hExp1m ->Write();
  hExp2m ->Write();
  hExp1p ->Write();
  hExp2p ->Write();
  hExpXsec   ->Write();
  hObsXsec   ->Write();
  hObs1mXsec ->Write();
  hObs1pXsec ->Write();
  hExp1mXsec ->Write();
  hExp2mXsec ->Write();
  hExp1pXsec ->Write();
  hExp2pXsec ->Write();
  hExpPass   ->Write();
  hObsPass   ->Write();
  hObs1mPass ->Write();
  hObs1pPass ->Write();
  hExp1mPass ->Write();
  hExp2mPass ->Write();
  hExp1pPass ->Write();
  hExp2pPass ->Write();

  g2Exp   ->Write();
  g2Obs   ->Write();
  g2Obs1m ->Write();
  g2Obs1p ->Write();
  g2Exp1m ->Write();
  g2Exp2m ->Write();
  g2Exp1p ->Write();
  g2Exp2p ->Write();
  gExp_c   ->Write();
  gObs_c   ->Write();
  gObs1m_c ->Write();
  gObs1p_c ->Write();
  gExp1m_c ->Write();
  gExp2m_c ->Write();
  gExp1p_c ->Write();
  gExp2p_c ->Write();
  gExp_c2   ->Write();
  gObs_c2   ->Write();
  gObs1m_c2 ->Write();
  gObs1p_c2 ->Write();
  gExp1m_c2 ->Write();
  gExp2m_c2 ->Write();
  gExp1p_c2 ->Write();
  gExp2p_c2 ->Write();
  gEmpty->Write();
  file->Close();
  cout << "Saved all histos in  " << file->GetName() << endl;

  fxsec->Close();
  delete fxsec;
  
}


TH2F* InterpolateThisHistogram(TH2F *hold/*, TH2F* hnew*/){
  float binsize = hold->GetXaxis()->GetBinWidth(1)/2.;
  TString name = hold->GetName();
  name.ReplaceAll("Org","");
  TGraph2D *g = new TGraph2D(hold);
  g->SetNpx(int(g->GetXmax()-g->GetXmin())/binsize);
  g->SetNpy(int(g->GetYmax()-g->GetYmin())/binsize);
  TH2F *hnew = (TH2F*)g->GetHistogram();
  TH2F *h = new TH2F(name.Data(),hold->GetTitle(),hnew->GetNbinsX(),g->GetXmin()-binsize,g->GetXmax()-binsize,hnew->GetNbinsY(),g->GetYmin()-binsize,g->GetYmax()-binsize);
  for(unsigned int x = 1; x<=hnew->GetNbinsX(); ++x){
    for(unsigned int y = 1; y<=hnew->GetNbinsY(); ++y){
      h->SetBinContent(x,y,hnew->GetBinContent(x,y));
    }
  }
  delete g;
  return h;
}

TH2F* InterpolateThisHistogramNew(TH2F *hold/*, TH2F* hnew*/){
  float binsize = hold->GetXaxis()->GetBinWidth(1)/2.;
  TString name = hold->GetName();
  name.ReplaceAll("Org","");
  TGraph2D *g = new TGraph2D(hold);
  g->SetNpx(int(g->GetXmax()-g->GetXmin())/binsize);
  g->SetNpy(int(g->GetYmax()-g->GetYmin())/binsize);
  TH2F *hnew = (TH2F*)g->GetHistogram();
  TH2F *h = new TH2F(name.Data(),hold->GetTitle(),hnew->GetNbinsX(),g->GetXmin()-binsize,g->GetXmax()-binsize,hnew->GetNbinsY(),g->GetYmin()-binsize,g->GetYmax()-binsize);
  for(unsigned int x = 1; x<=hnew->GetNbinsX(); ++x){
    for(unsigned int y = 1; y<=hnew->GetNbinsY(); ++y){
      float xval = hnew->GetXaxis()->GetBinCenter(x);
      float yval = hnew->GetYaxis()->GetBinCenter(y);
      float rold = hold->GetBinContent(hold->FindBin(xval, yval));
      float rnew = hnew->GetBinContent(x,y);
      // if (fabs(xval-yval-175) < 10 && yval < 450) {
      if (fabs(xval-yval-175) < 15 && yval < 450 && rold > 1 && rnew < 1) {
        rnew = rold;
      }
      h->SetBinContent(x,y,rnew);
    }
  }
  delete g;
  return h;
}

TGraph2D* GetInterpolatingGraph(TH2F *hold){
  float binsize = hold->GetXaxis()->GetBinWidth(1)/2.;
  TString name = hold->GetName();
  name.ReplaceAll("Org","");
  TGraph2D *g = new TGraph2D(hold);
  g->SetNpx(int(g->GetXmax()-g->GetXmin())/binsize);
  g->SetNpy(int(g->GetYmax()-g->GetYmin())/binsize);
  return g;
}

TGraph2D* GetInterpolatingGraphNew(TH2F *hold){
  float binsize = hold->GetXaxis()->GetBinWidth(1)/2.;
  TString name = hold->GetName();
  name.ReplaceAll("Org","");
  TGraph2D *g = new TGraph2D(hold);
  g->SetNpx(int(g->GetXmax()-g->GetXmin())/binsize);
  g->SetNpy(int(g->GetYmax()-g->GetYmin())/binsize);
  TH2F* hnew = (TH2F*)g->GetHistogram();
  for(unsigned int x = 1; x<=hnew->GetNbinsX(); ++x){
    for(unsigned int y = 1; y<=hnew->GetNbinsY(); ++y){
      float xval = hnew->GetXaxis()->GetBinCenter(x);
      float yval = hnew->GetYaxis()->GetBinCenter(y);
      if (fabs(xval - yval -175) < 15 && yval < 450) {
        float rold = hold->GetBinContent(hold->FindBin(xval, yval));
        float rnew = hnew->GetBinContent(x,y);
        if (rold > 1 && rnew < 1 && yval < 450) {
          hnew->SetBinContent(hnew->FindBin(x,y), rold);
        }
      }
    }
  }
  g = new TGraph2D(hnew);
  return g;
}

TH2F* PassThisHistogram(TH2F *hold){
  TString name = hold->GetName() + (TString)"Pass";
  TH2F *hnew = (TH2F*)hold->Clone(name);
  for(int x = 1; x<=hnew->GetNbinsX(); ++x){
    for(int y = 1; y<=hnew->GetNbinsY(); ++y){
      float val = hnew->GetBinContent(x,y);
      float pass = 0;
      if(val>1) pass = 0.001;
      else if(val>0) pass = 1;
      hnew->SetBinContent(x,y,pass);
    }
  }
  return hnew;
}

TH2F* XsecThisHistogram(TH2F *hold, TH1D *hxsec){
  TString name = hold->GetName() + (TString)"Xsec";
  TH2F *hnew = (TH2F*)hold->Clone(name);
  for(int x = 1; x<=hnew->GetNbinsX(); ++x){
    for(int y = 1; y<=hnew->GetNbinsY(); ++y){
      float val = hnew->GetBinContent(x,y);
      float mstop = hnew->GetXaxis()->GetBinLowEdge(x);
      float xsec = hxsec->GetBinContent(hxsec->FindBin(mstop));
      hnew->SetBinContent(x,y,val*xsec);
    }
  }
  return hnew;
}

TGraph* GetContour(TGraph2D *g, TString name, TGraph *gempty){
  TGraph *gnew;
  TH2D *temp = (TH2D*)g->GetHistogram();//need this for list to work?
  // if (name == "gExp"){
  //   TCanvas c1("c1","c1",800,400);
  //   TH2F* temp2 = PassThisHistogram((TH2F*) temp);
  //   temp2->GetZaxis()->SetRangeUser(0,2);
  //   temp2->Draw("colz");
  //   c1.SaveAs("temp2dcont.pdf");
  // }
  //g->Draw("alp");//need this for list to work?
  TList *glist = (TList*)g->GetContourList(1.0);
  if(glist == nullptr) {gnew = (TGraph*)gempty->Clone(name); return gnew; }
  if(glist->GetSize()==0) {gnew = (TGraph*)gempty->Clone(name); return gnew; }
  int max_points = -1;
  int nn = glist->GetSize();
  for(int i = 0; i<glist->GetSize(); ++i){
    TGraph *gtemp = (TGraph*)glist->At(i);
    int Npoints = gtemp->GetN();
    if(Npoints>max_points){
      gnew = (TGraph*)gtemp->Clone(name);
      max_points = Npoints;
      if(gnew->GetN()>0){
	double x,y;
	gnew->GetPoint(0,x,y);
	if(y<13.) gnew->SetPoint(0,x,0);
	gnew->GetPoint(gnew->GetN()-1,x,y);
	if(y<13.) { gnew->SetPoint(gnew->GetN(),x,0); }//they way ROOT does the interpolation the LSP = 0 strip becomes empty
      }
    }
  }

  double x,y;
  double px,py;
  for (int i = 0; i < gnew->GetN(); ++i) {
    gnew->GetPoint(i,x,y);
    double dm = x-y;
    auto apprpt = [&](double a, double b) {
      return fabs(x-a) < 2 && fabs(y-b) < 2;
    };
    const bool smooth = true;
    if (smooth && sigtype == "comb_T2tt") {
      /*
      if (name == "gObs") {
        if (apprpt(608, 508)) gnew->RemovePoint(i--);
        if (apprpt(593, 481)) gnew->RemovePoint(i--);
        if (apprpt(651, 473)) gnew->RemovePoint(i--);
        if (apprpt(687, 460)) gnew->SetPoint(i,670,465);
        if (apprpt(689, 462)) gnew->SetPoint(i,685,472);
        if (apprpt(688, 486)) gnew->SetPoint(i,700,475);
        if (apprpt(612, 418)) {
          // gnew->InsertPointBefore(i++,450+5,300-5);
          gnew->InsertPointBefore(i++,450-4,275+6);
          gnew->InsertPointBefore(i++,450-5,275-5);
          gnew->InsertPointBefore(i++,450+6,275-4);
          // gnew->InsertPointBefore(i++,475+5,275+5);
        }
        if (dm < 250 && dm > 210 && x > 640 && x < 780) {
          if (dm < 225 || dm > 235) gnew->RemovePoint(i--);
        }
      } else if (name== "gObs1m") {
        if (apprpt(596, 496)) gnew->RemovePoint(i--);
        if (apprpt(645, 480)) gnew->RemovePoint(i--);
        if (x> 657 && x< 672) gnew->RemovePoint(i--);
        if (apprpt(612, 418)) {

          gnew->InsertPointBefore(i++,412-4,237+6);
          gnew->InsertPointBefore(i++,412-5,237-5);
          gnew->InsertPointBefore(i++,412+6,237-4);
        }
        if (dm < 250 && dm > 210 && x > 640 && x < 780) {
          if (dm < 225 || dm > 235) gnew->RemovePoint(i--);
        }
      } else if (name== "gObs1p") {
        if (apprpt(596, 496)) gnew->RemovePoint(i--);
        if (apprpt(645, 480)) gnew->RemovePoint(i--);
        if (apprpt(1163, 645)) gnew->RemovePoint(i--);
        if (x> 657 && x< 672) gnew->RemovePoint(i--);
        if (apprpt(612, 418)) {
          gnew->InsertPointBefore(i++,450-4,275+6);
          gnew->InsertPointBefore(i++,450-5,275-5);
          gnew->InsertPointBefore(i++,450+6,275-4);
        }
        if (dm < 250 && dm > 210 && x > 640 && x < 780) {
          if (dm < 225 || dm > 235) gnew->RemovePoint(i--);
        }
      } else if (name == "gExp") {
        if (apprpt(665, 565)) gnew->RemovePoint(i--);
        if (apprpt(640, 510)) gnew->SetPoint(i,640,500);
        if (apprpt(685, 465)) gnew->SetPoint(i,678,470);
        if (apprpt(662, 463)) {
          gnew->InsertPointBefore(i++,625+5,475-5);
          gnew->InsertPointBefore(i++,600+5,450-5);
          gnew->InsertPointBefore(i++,575+5,425-5);
          gnew->InsertPointBefore(i++,425-4,250+6);
          gnew->InsertPointBefore(i++,425-5,250-5);
          gnew->InsertPointBefore(i++,425+6,250-4);
        }
      } else if (name == "gExp1m") {
        // if (apprpt(689, 534)) gnew->RemovePoint(i--);
        if (apprpt(696, 528)) gnew->RemovePoint(i--);
        if (apprpt(700, 525)) gnew->RemovePoint(i--);
        if (apprpt(712, 592)) gnew->RemovePoint(i--);
        if (apprpt(662, 468)) {
          gnew->InsertPointBefore(i++,625-3,465+3);
          gnew->InsertPointBefore(i++,600+3,440+3);
          gnew->InsertPointBefore(i++,475-4,300+6);
          gnew->InsertPointBefore(i++,475-5,300-5);
          gnew->InsertPointBefore(i++,475+6,300-4);
          gnew->InsertPointBefore(i++,650+5,450+5);
        }
      } else if (name == "gExp1p") {
        if (apprpt(597, 497)) gnew->RemovePoint(i--);
        if (apprpt(605, 419)) gnew->RemovePoint(i--);
        if (apprpt(587, 485)) gnew->RemovePoint(i--);
        if (apprpt(542, 408)) gnew->RemovePoint(i--);
        if (apprpt(562, 365)) {
          gnew->InsertPointBefore(i++,525-3,365+3);
          gnew->InsertPointBefore(i++,475+3,312+3);
          gnew->InsertPointBefore(i++,400-4,225+6);
          gnew->InsertPointBefore(i++,400-0,225-0);
          gnew->InsertPointBefore(i++,400+6,225-4);
          gnew->InsertPointBefore(i++,487+5,287+5);
          gnew->InsertPointBefore(i++,550+5,350+5);
        }
      }
      */
      if (name == "gObs") {
        if (apprpt(715, 509)) gnew->RemovePoint(i--);
        // if (apprpt(593, 481)) gnew->RemovePoint(i--);
        // if (apprpt(651, 473)) gnew->RemovePoint(i--);
      }
    }
    if (smooth && sigtype == "std_T2bW") {

    }
    if (smooth && sigtype == "std_T2bt") {
      if (name == "gObs") {
        // if (apprpt(1013, 478)) gnew->SetPoint(i,1013,469);
        if (apprpt(570, 370)) gnew->RemovePoint(i--);
     } else if (name == "gObs1p") {
       if (apprpt(1000, 523)) gnew->RemovePoint(i--);
       if (apprpt(587, 387)) gnew->RemovePoint(i--);
     }
    }

    px = x; py = y;
  }

  return gnew;
}

TGraph* GetContour2(TGraph2D *g, TString name, TGraph *gempty){
  TGraph *gnew;
  TH2D *temp = (TH2D*)g->GetHistogram();//need this for list to work?
  //g->Draw("alp");//need this for list to work?
  TList *glist = (TList*)g->GetContourList(1.0);
  if(glist == nullptr) {gnew = (TGraph*)gempty->Clone(name); return gnew; }
  if(glist->GetSize()==0) {gnew = (TGraph*)gempty->Clone(name); return gnew; }
  int max_points = -1;
  int nn = glist->GetSize();
  int noti = -1;
  //cout << "number of entries in list " << nn << endl;
  for(int i = 0; i<glist->GetSize(); ++i){
    TGraph *gtemp = (TGraph*)glist->At(i);
    int Npoints = gtemp->GetN();
    if(Npoints>max_points){
      gnew = (TGraph*)gtemp->Clone(name);
      max_points = Npoints;
      noti = i;
      if(gnew->GetN()>0){
	double x,y;
	gnew->GetPoint(0,x,y);
	if(y<13.) gnew->SetPoint(0,x,0);
	gnew->GetPoint(gnew->GetN()-1,x,y);
	if(y<13.) { gnew->SetPoint(gnew->GetN(),x,0);}//they way ROOT does the interpolation the LSP = 0 strip becomes empty
      }
    }
  }
  max_points = -1;
  for(int i = 0; i<glist->GetSize(); ++i){
    if(i==noti&&glist->GetSize()>1) continue;
    TGraph *gtemp = (TGraph*)glist->At(i);
    int Npoints = gtemp->GetN();
    if(Npoints>max_points){
      gnew = (TGraph*)gtemp->Clone(name);
      max_points = Npoints;
      if(gnew->GetN()>0){
	double x,y;
	gnew->GetPoint(0,x,y);
	if(y<13.) gnew->SetPoint(0,x,0);
	gnew->GetPoint(gnew->GetN()-1,x,y);
	if(y<13.) { gnew->SetPoint(gnew->GetN(),x,0);}//they way ROOT does the interpolation the LSP = 0 strip becomes empty
      }
    }
  }
  return gnew;
}
