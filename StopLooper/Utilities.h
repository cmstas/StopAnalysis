#ifndef STOPLOOPER_UTILITIES_H
#define STOPLOOPER_UTILITIES_H

#include "TMath.h"
#include "TH1.h" 
#include "TH2.h" 
#include "TH3.h" 
#include "TGraph.h"
#include "TCanvas.h" 
#include "TFile.h"

#include <map>
#include <string>

// Histogram manipulation
inline void moveOverFlowToLastBin1D(TH1* hist) {
  int nbin = hist->GetNbinsX();
  if (hist->GetBinContent(nbin+1) > 0) {
    double err = 0;
    hist->SetBinContent(nbin, hist->IntegralAndError(nbin, -1, err));
    hist->SetBinError(nbin, err);
    hist->SetBinContent(nbin+1, 0);
    hist->SetBinError(nbin+1, 0);
  }
}

inline void moveXOverFlowToLastBin3D(TH3* hist) {
  int nbinx = hist->GetNbinsX();
  for (int ibiny = 0; ibiny < hist->GetNbinsY(); ++ibiny) {
    for (int ibinz = 0; ibinz < hist->GetNbinsZ(); ++ibinz) {
      if (hist->GetBinContent(nbinx+1, ibiny, ibinz) <= 0) continue;
      double err = 0;
      double yield = hist->IntegralAndError(nbinx, -1, ibiny, ibiny, ibinz, ibinz, err);
      hist->SetBinContent(nbinx, ibiny, ibinz, yield);
      hist->SetBinError(nbinx, ibiny, ibinz, err);
      hist->SetBinContent(nbinx+1, ibiny, ibinz, 0);
      hist->SetBinError(nbinx+1, ibiny, ibinz, 0);
    }
  }
}

// Templated function
template<class LorentzVectorType>
bool isCloseObject(const LorentzVectorType p1, const LorentzVectorType p2, const float conesize)
{
  const float PI = TMath::Pi();
  float deltaEta = fabs(p1.eta() - p2.eta());
  if (deltaEta > conesize) return false;
  float deltaPhi = fabs(p1.phi() - p2.phi());
  if (deltaPhi > PI) deltaPhi = 2*PI - deltaPhi;
  if (deltaPhi > conesize) return false;
  float deltaR2 = deltaEta*deltaEta + deltaPhi*deltaPhi;
  if (deltaR2 > conesize*conesize) return false;

  return true;
}

template<typename... TArgs>
void plot1d(std::string name, float xval, double weight, std::map<std::string, TH1*> &allhistos, TArgs... args)
{
  auto iter = allhistos.find(name);
  if (iter == allhistos.end()) {
    TH1D* currentHisto= new TH1D(name.c_str(), args...);
    currentHisto->Sumw2();
    currentHisto->Fill(xval, weight);
    allhistos.insert( std::pair<std::string, TH1*>(name, currentHisto) );
  } else {
    iter->second->Fill(xval, weight);
  }
}

template<typename... TArgs>
void plot2d(std::string name, float xval, float yval, double weight, std::map<std::string, TH1*> &allhistos, TArgs... args)
{
  auto iter = allhistos.find(name);
  if (iter == allhistos.end()) {
    TH2D* currentHisto= new TH2D(name.c_str(), args...);
    currentHisto->Sumw2();
    currentHisto->Fill(xval, yval, weight);
    allhistos.insert( std::pair<std::string, TH1*>(name, currentHisto) );
  } else {
    ((TH2D*) iter->second)->Fill(xval, yval, weight);
  }
}

template<typename... TArgs>
void plot3d(std::string name, float xval, float yval, float zval, double weight, std::map<std::string, TH1*> &allhistos, TArgs... args)
{
  auto iter = allhistos.find(name);
  if (iter == allhistos.end()) {
    TH3D* currentHisto= new TH3D(name.c_str(), args...);
    currentHisto->Sumw2();
    currentHisto->Fill(xval, yval, zval, weight);
    allhistos.insert( std::pair<std::string, TH1*>(name, currentHisto) );
  } else {
    ((TH3D*) iter->second)->Fill(xval, yval, zval, weight);
  }
}

#endif  // STOPLOOPER_UTILITIES_H
