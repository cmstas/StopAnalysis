#ifndef STOPLOOPER_h
#define STOPLOOPER_h

#include "TFile.h"
#include "TChain.h"
#include "SR.h"
#include "eventWeight.h"
#include "../StopCORE/TopTagger/ResolvedTopMVA.h"

class StopLooper {
 public:
  StopLooper() : evtweight_(1.) , jestype_(0) {}
  ~StopLooper() {}

  void SetSignalRegions();
  void SetJetEnergyScaleType(int jestype) { jestype_ = jestype; }
  void GenerateAllSRptrSets();

  void looper(TChain* chain, std::string sample, std::string outputdir, int jestype = 0);

  void temporaryAnalysisDump(std::string suffix = "");

 protected:
  std::vector<SR> SRVec;
  std::vector<SR> CR2lVec;
  std::vector<SR> CR0bVec;
  std::vector<SR> CRemuVec;

  std::vector<SR> testVec;

  // Analysis
  void fillYieldHistos(SR& sr, float met, std::string suffix = "", bool is_cr2l = false);
  void fillHistosForSR(std::string suffix = "");
  void fillHistosForCR2l(std::string suffix = "");
  void fillHistosForCR0b(std::string suffix = "");
  void fillHistosForCRemu(std::string suffix = "");

  // Testing
  void fillTopTaggingHistos(std::string suffix = "");
  void fillEfficiencyHistos(SR& sr, std::string type = "", std::string suffix = "");
  void testTopTaggingEffficiency(SR& sr);
  void testCutFlowHistos(SR& sr);

  // Under development
  std::map<std::string,SRptrSet> allSRptrSets;
  std::vector<SR*> FindSR(std::map<std::string,float>& values);
  void newFillHistosForCR(std::string suffix = "");

  evtWgtInfo evtWgt;
  ResolvedTopMVA* resTopMVA;

 private:
  // Global variable
  TFile* outfile_;

  // Event specific variables
  bool is_fastsim_;
  bool is_bkg_;
  float evtweight_;
  int jestype_;
  std::map<std::string,float> values_;

  // For nvtx reweighting
  float nvtxscale_[100];

};

#endif
