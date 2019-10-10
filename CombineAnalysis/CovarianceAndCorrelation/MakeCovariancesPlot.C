#include "TList.h"
#include "TMath.h"
#include "TString.h"
#include "TObject.h"
#include "TFile.h"
#include "TPad.h"
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
#include "TStopwatch.h"
#include "TColor.h"
#include "TLegend.h"
#include "THStack.h"
#include "TLatex.h"
#include "TLine.h"
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

void MakeCovariancesPlot(){
    
   gStyle->SetOptFit(1);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   gStyle->SetPaintTextFormat(".0f");


    int NRGBs = 5;
    int NCont = 255;
    double stops[5] {0.00, 0.34, 0.61, 0.84, 1.00};
    double red[5] = {0.50, 0.50, 1.00, 1.00, 1.00};
    double green[5] = {0.50, 1.00, 1.00, 0.60, 0.50};
    double blue[5] = {1.00, 1.00, 0.50, 0.40, 0.50};
    TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    gStyle->SetNumberContours(NCont);
    //gStyle->SetPalette(kLightTemperature);
    //gStyle->SetPalette(kBird);

    
    TFile *f = new TFile("matrices_std.root");
    f->cd();
    TH2D *std = (TH2D*)f->Get("covarianceMatrix");
    std->SetName("std");
    TFile *f2 = new TFile("matrices_tcor.root");
    f2->cd();
    TH2D *tcor = (TH2D*)f2->Get("covarianceMatrix");
    tcor->SetName("tcor");
    
    TFile *f3 = new TFile("matrices_Wcor.root");
    f3->cd();
    TH2D *Wcor = (TH2D*)f3->Get("covarianceMatrix");
    Wcor->SetName("Wcor");


    // This is the Covariances matrix for the standard analysis
    TH2D *correlation = new TH2D("correlationMatrix","",39,0,39,39,0,39);
    TH2D *correlationtext = new TH2D("correlationMatrix","",39,0,39,39,0,39);
    TH2D *correlationtext2 = new TH2D("correlationMatrix","",39,0,39,39,0,39); // for different digit
    for(int x = 1; x<=39;++x){
        for(int y = 1; y<=39; ++y){
	  correlation->SetBinContent(x,y,std->GetBinContent(x,y));
          float corval = std->GetBinContent(x,y);
          if (corval < 100)
            correlationtext->SetBinContent(x,y, corval);
          else
            correlationtext2->SetBinContent(x,y, corval);
        }
    }

    // This is the correlation matrix for the compressed analysis
    TH2D *correlationComp = new TH2D("correlationMatrixComp","",10,0,10,10,0,10);
    TH2D *correlationtextComp = new TH2D("correlationMatrixComp","",10,0,10,10,0,10);
    for(int x = 1; x<=5;++x){
        for(int y = 1; y<=5; ++y){
	  correlationComp->SetBinContent(x,y,tcor->GetBinContent(x,y));
	  correlationtextComp->SetBinContent(x,y, tcor->GetBinContent(x,y));
        }
    }

    for(int x = 6; x<=10;++x){
      for(int y = 6; y<=10; ++y){
        correlationComp->SetBinContent(x,y,Wcor->GetBinContent(x-5,y-5));
        correlationtextComp->SetBinContent(x,y, Wcor->GetBinContent(x-5,y-5));
      }
    }

    correlationComp->GetXaxis()->SetBinLabel( 1,"I:250");
    correlationComp->GetXaxis()->SetBinLabel( 2,"I:350");
    correlationComp->GetXaxis()->SetBinLabel( 3,"I:450");
    correlationComp->GetXaxis()->SetBinLabel( 4,"I:550");
    correlationComp->GetXaxis()->SetBinLabel( 5,"I:750");
    correlationComp->GetXaxis()->SetBinLabel( 6,"J:250");
    correlationComp->GetXaxis()->SetBinLabel( 7,"J:350");
    correlationComp->GetXaxis()->SetBinLabel( 8,"J:450");
    correlationComp->GetXaxis()->SetBinLabel( 9,"J:550");
    correlationComp->GetXaxis()->SetBinLabel(10,"J:750");
    correlationComp->GetYaxis()->SetBinLabel( 1,"I:250");
    correlationComp->GetYaxis()->SetBinLabel( 2,"I:350");
    correlationComp->GetYaxis()->SetBinLabel( 3,"I:450");
    correlationComp->GetYaxis()->SetBinLabel( 4,"I:550");
    correlationComp->GetYaxis()->SetBinLabel( 5,"I:750");
    correlationComp->GetYaxis()->SetBinLabel( 6,"J:250");
    correlationComp->GetYaxis()->SetBinLabel( 7,"J:350");
    correlationComp->GetYaxis()->SetBinLabel( 8,"J:450");
    correlationComp->GetYaxis()->SetBinLabel( 9,"J:550");
    correlationComp->GetYaxis()->SetBinLabel(10,"J:750");
    correlationComp->GetXaxis()->SetLabelSize(0.054);
    correlationComp->GetYaxis()->SetLabelSize(0.05);
    correlationComp->GetXaxis()->SetTitle("signal regions");
    correlationComp->GetXaxis()->SetTitleOffset(1.0);
    correlationComp->GetYaxis()->SetTitleOffset(1.0);
    correlationComp->GetXaxis()->SetTitleSize(0.05);
    correlationComp->GetYaxis()->SetTitleSize(0.05);
    correlationComp->GetYaxis()->SetTitle("signal regions");
    correlationComp->GetZaxis()->SetTitle("covariance");
    correlationComp->GetZaxis()->SetTitleSize(0.05);
    correlationComp->GetZaxis()->SetLabelSize(0.05);
    correlationComp->GetZaxis()->SetNdivisions(505);
    correlationComp->GetZaxis()->SetRangeUser(0.1,8000);


    correlation->GetXaxis()->SetBinLabel( 1,"A0:600");
    correlation->GetXaxis()->SetBinLabel( 2,"A0:750");
    correlation->GetXaxis()->SetBinLabel( 3,"A1:350");
    correlation->GetXaxis()->SetBinLabel( 4,"A1:450");
    correlation->GetXaxis()->SetBinLabel( 5,"A2:250");
    correlation->GetXaxis()->SetBinLabel( 6,"B:250");
    correlation->GetXaxis()->SetBinLabel( 7,"B:450");
    correlation->GetXaxis()->SetBinLabel( 8,"B:700");
    correlation->GetXaxis()->SetBinLabel( 9,"C:350");
    correlation->GetXaxis()->SetBinLabel(10,"C:450");
    correlation->GetXaxis()->SetBinLabel(11,"C:550");
    correlation->GetXaxis()->SetBinLabel(12,"C:650");
    correlation->GetXaxis()->SetBinLabel(13,"C:800");
    correlation->GetXaxis()->SetBinLabel(14,"D:250");
    correlation->GetXaxis()->SetBinLabel(15,"D:350");
    correlation->GetXaxis()->SetBinLabel(16,"D:450");
    correlation->GetXaxis()->SetBinLabel(17,"D:600");
    correlation->GetXaxis()->SetBinLabel(18,"E0:450");
    correlation->GetXaxis()->SetBinLabel(19,"E0:600");
    correlation->GetXaxis()->SetBinLabel(20,"E1:250");
    correlation->GetXaxis()->SetBinLabel(21,"E1:350");
    correlation->GetXaxis()->SetBinLabel(22,"E2:250");
    correlation->GetXaxis()->SetBinLabel(23,"E2:350");
    correlation->GetXaxis()->SetBinLabel(24,"E3:250");
    correlation->GetXaxis()->SetBinLabel(25,"E3:350");
    correlation->GetXaxis()->SetBinLabel(26,"F:250");
    correlation->GetXaxis()->SetBinLabel(27,"F:350");
    correlation->GetXaxis()->SetBinLabel(28,"F:450");
    correlation->GetXaxis()->SetBinLabel(29,"G0:450");
    correlation->GetXaxis()->SetBinLabel(30,"G0:550");
    correlation->GetXaxis()->SetBinLabel(31,"G0:750");
    correlation->GetXaxis()->SetBinLabel(32,"G1:250");
    correlation->GetXaxis()->SetBinLabel(33,"G1:350");
    correlation->GetXaxis()->SetBinLabel(34,"G2:250");
    correlation->GetXaxis()->SetBinLabel(35,"G2:350");
    correlation->GetXaxis()->SetBinLabel(36,"G3:250");
    correlation->GetXaxis()->SetBinLabel(37,"G3:350");
    correlation->GetXaxis()->SetBinLabel(38,"H:250");
    correlation->GetXaxis()->SetBinLabel(39,"H:500");
    correlation->GetXaxis()->SetBit(TAxis::kLabelsVert);
    correlation->GetXaxis()->SetLabelSize(0.034);

    correlation->GetYaxis()->SetBinLabel( 1,"A0:600");
    correlation->GetYaxis()->SetBinLabel( 2,"A0:750");
    correlation->GetYaxis()->SetBinLabel( 3,"A1:350");
    correlation->GetYaxis()->SetBinLabel( 4,"A1:450");
    correlation->GetYaxis()->SetBinLabel( 5,"A2:250");
    correlation->GetYaxis()->SetBinLabel( 6,"B:250");
    correlation->GetYaxis()->SetBinLabel( 7,"B:450");
    correlation->GetYaxis()->SetBinLabel( 8,"B:700");
    correlation->GetYaxis()->SetBinLabel( 9,"C:350");
    correlation->GetYaxis()->SetBinLabel(10,"C:450");
    correlation->GetYaxis()->SetBinLabel(11,"C:550");
    correlation->GetYaxis()->SetBinLabel(12,"C:650");
    correlation->GetYaxis()->SetBinLabel(13,"C:800");
    correlation->GetYaxis()->SetBinLabel(14,"D:250");
    correlation->GetYaxis()->SetBinLabel(15,"D:350");
    correlation->GetYaxis()->SetBinLabel(16,"D:450");
    correlation->GetYaxis()->SetBinLabel(17,"D:600");
    correlation->GetYaxis()->SetBinLabel(18,"E0:450");
    correlation->GetYaxis()->SetBinLabel(19,"E0:600");
    correlation->GetYaxis()->SetBinLabel(20,"E1:250");
    correlation->GetYaxis()->SetBinLabel(21,"E1:350");
    correlation->GetYaxis()->SetBinLabel(22,"E2:250");
    correlation->GetYaxis()->SetBinLabel(23,"E2:350");
    correlation->GetYaxis()->SetBinLabel(24,"E3:250");
    correlation->GetYaxis()->SetBinLabel(25,"E3:350");
    correlation->GetYaxis()->SetBinLabel(26,"F:250");
    correlation->GetYaxis()->SetBinLabel(27,"F:350");
    correlation->GetYaxis()->SetBinLabel(28,"F:450");
    correlation->GetYaxis()->SetBinLabel(29,"G0:450");
    correlation->GetYaxis()->SetBinLabel(30,"G0:550");
    correlation->GetYaxis()->SetBinLabel(31,"G0:750");
    correlation->GetYaxis()->SetBinLabel(32,"G1:250");
    correlation->GetYaxis()->SetBinLabel(33,"G1:350");
    correlation->GetYaxis()->SetBinLabel(34,"G2:250");
    correlation->GetYaxis()->SetBinLabel(35,"G2:350");
    correlation->GetYaxis()->SetBinLabel(36,"G3:250");
    correlation->GetYaxis()->SetBinLabel(37,"G3:350");
    correlation->GetYaxis()->SetBinLabel(38,"H:250");
    correlation->GetYaxis()->SetBinLabel(39,"H:500");
    correlation->GetYaxis()->SetLabelSize(0.03);

    correlation->GetXaxis()->SetTitle("signal regions");
    correlation->GetYaxis()->SetTitle("signal regions");
    correlation->GetXaxis()->SetTitleOffset(1.5);
    correlation->GetYaxis()->SetTitleOffset(1.0);
    correlation->GetXaxis()->SetTitleSize(0.045);
    correlation->GetYaxis()->SetTitleSize(0.045);
    correlation->GetZaxis()->SetTitle("covariance");
    correlation->GetZaxis()->SetTitleSize(0.05);
    correlation->GetZaxis()->SetLabelSize(0.05);
    correlation->GetZaxis()->SetRangeUser(0.01,1000);

    TLatex *tLumi = new TLatex(0.894,0.942,"137 fb^{-1} (13 TeV)");
    tLumi->SetNDC();
    tLumi->SetTextAlign(31);
    tLumi->SetTextFont(42);
    tLumi->SetTextSize(0.042);
    tLumi->SetLineWidth(2);
    TLatex *tCMS = new TLatex(0.095,0.942,"CMS");
    tCMS->SetNDC();
    tCMS->SetTextAlign(11);
    tCMS->SetTextFont(61);
    tCMS->SetTextSize(0.0525);
    tCMS->SetLineWidth(2);
    TLatex *tPrel = new TLatex(0.172,0.942,"Supplementary");
    // TLatex *tPrel = new TLatex(0.172,0.942,"Preliminary");
    tPrel->SetNDC();
    tPrel->SetTextAlign(11);
    tPrel->SetTextFont(52);
    tPrel->SetTextSize(0.042);
    tPrel->SetLineWidth(2);


    gStyle->SetTextFont(42);
    TCanvas * c1 = new TCanvas("c1","",100,100,750,500);
    c1->SetLogz();
    //c1->SetLeftMargin(0.15);
    c1->SetRightMargin(0.15);
    c1->SetTopMargin(0.07);
    //c1->SetBottomMargin(0.15);
    c1->SetBottomMargin(0.15);
    // correlation->GetZaxis()->SetRangeUser(std::min(0.,correlation->GetMinimum()),100.);
    correlation->Draw("colz");
    gStyle->SetPaintTextFormat("2.1f");
    correlationtext->SetMarkerSize(0.7);
    correlationtext->Draw("textsame");
    correlationtext2->SetMarkerSize(0.53);
    correlationtext2->Draw("textsame");
    tLumi->Draw();
    tCMS->Draw();
    tPrel->Draw();
    c1->Print("CovariancePlotStandard.pdf","pdf");

    TCanvas * c2 = new TCanvas("c2","",100,100,750,500);
    c2->SetLogz();
    // c2->SetLeftMargin(0.12);
    c2->SetRightMargin(0.15);
    c2->SetTopMargin(0.07);
    c2->SetBottomMargin(0.15);
    // c2->SetBottomMargin(0.125);
    // correlationComp->GetZaxis()->SetRangeUser(std::min(0.,correlationComp->GetMinimum()),100.);
    correlationComp->Draw("colz");
    correlationtextComp->SetMarkerSize(1.7);
    gStyle->SetPaintTextFormat("3.1f");
    correlationtextComp->Draw("textsame");
    tLumi->Draw();
    tCMS->Draw();
    tPrel->Draw();
    c2->Print("CovariancePlotCompressed.pdf","pdf");

}
