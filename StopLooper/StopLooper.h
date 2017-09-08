#ifndef STOPLOOPER_h
#define STOPLOOPER_h

#include "TFile.h"
#include "TChain.h"
#include "SR.h"

class StopLooper {
public:
  StopLooper() : evtweight_(1.) {}
  ~StopLooper() {}

  void SetSignalRegions();
  void GenerateAllSRptrSets();

  void looper(TChain* chain, std::string sample, std::string outputdir);

  void fillHistosForSR(std::string suffix = "");
  void fillHistosForCR2l(std::string suffix = "");
  void fillHistosForCR0b(std::string suffix = "");
  void fillHistosForCRemu(std::string suffix = "");

  // Under development
  std::vector<SR*> FindSR(std::map<std::string,float>& values);
  void newFillHistosForCR(std::string suffix = "");

protected:
  std::vector<SR> SRVec;
  std::vector<SR> CR2lVec;
  std::vector<SR> CR0bVec;
  std::vector<SR> CRemuVec;

  // Under development
  std::map<std::string,SRptrSet> allSRptrSets;

private:
  // Global variable
  TFile* outfile_;

  // Event specific variables
  float evtweight_;
  std::map<std::string,float> values_;

  // For nvtx reweighting
  float nvtxscale[70];

};

#endif
