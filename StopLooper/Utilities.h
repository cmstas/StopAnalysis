#ifndef PLOTUTILITIES_H
#define PLOTUTILITIES_H

#include "TH1.h" 
#include "TH2.h" 
#include "TH3.h" 
#include "TGraph.h"
#include "TCanvas.h" 
#include "TFile.h"

#include "RooRealVar.h"
#include "RooDataSet.h"

#include <map>
#include <string>

TH1D cumulate (const TH1D &in, bool increasing);
TGraph eff_rej (const TH1D &signal, TH1D &background, bool normalize, bool increasing);
void saveHist(const char* filename, const char* pat="*");
void deleteHistos();
TCanvas *ComparePlots(TFile *f, const char *hist1, const char *hist2, const char *label1, const char *label2, 
		      unsigned int rebin, bool norm, bool log, unsigned int opt);
TGraph GetROC(TFile *f, const char *hist1, const char *hist2, bool increasing);
TGraph GetEff(TFile *f, const char *hist1, bool increasing);

void plotRooDataSet(std::string name, RooRealVar* x_, RooRealVar* w_, double weight, std::map<std::string, RooDataSet*> &allRooDatasets, std::string title);
void plot1D(std::string name, float xval, double weight, std::map<std::string, TH1*> &allhistos, 
	    std::string title, int numbinsx, float xmin, float xmax);
void plot1D(std::string name, float xval, double weight, std::map<std::string, TH1*> &allhistos, 
	    std::string title, int numbinsx, const float * xbins);
TH1D* getHist1D(std::string title, std::map<std::string, TH1*> &allhistos, 
                int numbinsx, float xmin, float xmax);
void insertHist1D(TH1D* hist, std::map<std::string, TH1*> &allhistos);
void plot2D(std::string name, float xval, float yval, double weight, std::map<std::string, TH1*> &allhistos, 
	    std::string title, int numbinsx, float xmin, float xmax, int numbinsy, float ymin, float ymax);
void plot2D(std::string name, float xval, float yval, double weight, std::map<std::string, TH1*> &allhistos, 
	    std::string title, int numbinsx, const float * xbins, int numbinsy, const float * ybins);
TH2D* getHist2D(std::string title, std::map<std::string, TH1*> &allhistos, 
	    int numbinsx, float xmin, float xmax, int numbinsy, float ymin, float ymax);
void insertHist2D(TH2D* hist, std::map<std::string, TH1*> &allhistos);
// Old plot2D, with TH2D map
void plot2D(std::string name, float xval, float yval, double weight, std::map<std::string, TH2D*> &allhistos, 
	    std::string title, int numbinsx, float xmin, float xmax, int numbinsy, float ymin, float ymax);
void plot2D(std::string name, float xval, float yval, double weight, std::map<std::string, TH2D*> &allhistos, 
	    std::string title, int numbinsx, const float * xbins, int numbinsy, const float * ybins);
void plot3D(std::string name, float xval, float yval, float zval, double weight, std::map<std::string, TH1*> &allhistos, 
	    std::string title, int numbinsx, float xmin, float xmax, int numbinsy, float ymin, float ymax,
	    int numbinsz, float zmin, float zmax);
void plot3D(std::string name, float xval, float yval, float zval, double weight, std::map<std::string, TH1*> &allhistos, 
	    std::string title, int numbinsx, const float * xbins, int numbinsy, const float * ybins,
	    int numbinsz, const float * zbins);

void plot1DUnderOverFlow(std::string title, double xval, double weight, std::map<std::string, TH1*> &allhistos, 
	    int numbinsx, double xmin, double xmax);
void plot2DUnderOverFlow(std::string name, double xval, double yval, double weight, std::map<std::string, TH1*> &allhistos, 
	    std::string title, int numbinsx, double xmin, double xmax, int numbinsy, double ymin, double ymax);

void savePlots(std::map<std::string, TH1*>&, const char* );
void savePlots2(std::map<std::string, TH2D*>&, const char* );
void saveRooDataSetsDir(std::map<std::string, RooDataSet*>& datasets, TFile* outfile, const char* outdir = "");
void savePlotsDir(std::map<std::string, TH1*>& h_1d, TFile* outfile, const char* outdir = "");
void savePlots2Dir(std::map<std::string, TH2D*>& h_2d, TFile* outfile, const char* outdir = "");
void savePlots12(std::map<std::string, TH1D*>&, std::map<std::string, TH2D*>&, const char* );

// New functions
template<typename... TArgs>
void plot1d(std::string name, float xval, double weight, std::map<std::string, TH1*> &allhistos, TArgs... args);

template<typename... TArgs>
void plot2d(std::string name, float xval, float yval, double weight, std::map<std::string, TH1*> &allhistos, TArgs... args);

#endif

