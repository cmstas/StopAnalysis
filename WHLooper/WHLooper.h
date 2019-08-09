#ifndef WHLOOPER_h
#define WHLOOPER_h

#include "TFile.h"
#include "TChain.h"
//#include "../StopLooper/SR.h"
#include "../StopCORE/eventWeight.h"
//#include "../StopLooper/eventWeight.cc"


class WHLooper {
 public:
  WHLooper() : evtweight_(1.), jestype_(0) {}
  ~WHLooper() {}

  void SetJetEnergyScaleType(int jestype) { jestype_ = jestype; }
  void looper(TChain* chain, std::string sample, std::string outputdir, int jestype = 0);

  void temporaryAnalysisDump(std::string suffix = "");

 protected:
 

  // Helper functions
  bool PassingHLTriggers(const int type = 1);

  evtWgtInfo evtWgt; 

 private:
  // Global variable
  TFile* outfile_;

  // Event specific variables
  bool is_fastsim_;
  bool is_bkg_;
  double evtweight_;
  int jestype_;
  int year_;
  std::map<std::string,float> values_;

  // For nvtx reweighting
  float nvtxscale_[100];

};

#endif
