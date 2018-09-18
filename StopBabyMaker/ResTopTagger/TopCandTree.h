#ifndef TOPCANDTREE_H
#define TOPCANDTREE_H

#include <vector>
#include <string>
#include "TopCand.h"
#include "TTree.h"
#include "TRandom3.h"

class TopCandTree {
 public:
  // flatTree(TString fileName, TString treeName, TString outfileName, size randomSeed, bool isMCTree, cfgSet::ConfigSet *pars);
  TopCandTree();
  TopCandTree(std::string treeName, std::string outputName, std::string sampletype);
  ~TopCandTree();
  void AddEventInfo(int evt, float wgt, float pfmet, int nPV, int nLeps, int nJets, int nBJets, int nLBJets);
  void AddGenTopInfo(int topidx);
  void AddTopCandInfo(const TopCand *topcand, int isGenMatched);
  bool IsGenTopMatched(const TopCand* topcand);
  int  IsGenTopMatchedSloppy(const TopCand* topcand);
  void FillTree();
  void SetJetVectors(const std::vector<LorentzVector>* p4, const std::vector<float>* csv, const std::vector<float>* cvsl,
                     const std::vector<float>* ptD, const std::vector<float>* axis1, const std::vector<float>* axis2, const std::vector<int>* mult,
                     const std::vector<float>* deepcsvb, const std::vector<float>* deepcsvc, const std::vector<float>* deepcsvl, const std::vector<float>* deepcsvbb);
  void SetGenParticleVectors(const std::vector<LorentzVector>* p4, const std::vector<int>* id, const std::vector<bool>* isLastCopy,
                             const std::vector<int>* motherid, const std::vector<int>* motheridx);
  void SetBranches();
  void Setup(std::string treeName, std::string outputName, std::string sampletype);
  void ResetAll();
  void Write();

  TFile* outfile;
  TTree* tree;
  TRandom3* randGen;

  // Configurations
  size_t max_nbcand;

  // Pointers to vectors to jet collections to be passed in for each event
  const std::vector<LorentzVector>* jets_p4;
  const std::vector<float>* jets_csv;
  const std::vector<float>* jets_cvsl;
  const std::vector<float>* jets_ptD;
  const std::vector<float>* jets_axis1;
  const std::vector<float>* jets_axis2;
  const std::vector<int>*   jets_mult;
  const std::vector<float>* jets_deepcsvb;
  const std::vector<float>* jets_deepcsvc;
  const std::vector<float>* jets_deepcsvl;
  const std::vector<float>* jets_deepcsvbb;

  // Pointers to vectors to genparticle collections to be passed in for each event
  const std::vector<LorentzVector>* genps_p4;
  const std::vector<int>*  genps_id;
  const std::vector<bool>* genps_isLastCopy;
  const std::vector<int>*  genps_motherid;
  const std::vector<int>*  genps_motheridx;

 public:
  // Variables to be stored int the tree
  int   event;
  float weight;
  short npv;
  short nlep;
  short ntau;
  short ntrk;
  short njets;
  short nbjets;
  short nlbjets;
  short flag_signal;
  bool  flag_shuffle;
  float met;
  float ht;

  float gen_top_pt;
  float gen_w_pt;
  float gen_b_diag;
  float gen_w_diag;
  int   gen_top_resolved;

  float var_b_pt;
  float var_b_mass;
  float var_b_csv;
  float var_b_cvsb;
  float var_b_dcsv;
  float var_b_dcvsb;
  float var_b_dcvsl;
  float var_b_qgl;
  float var_b_ptD;
  float var_b_axis1;
  float var_b_axis2;
  int   var_b_mult;

  float var_j2_pt;
  float var_j2_csv;
  float var_j2_cvsl;
  float var_j2_cvsb;
  float var_j2_dcsv;
  float var_j2_dcvsb;
  float var_j2_dcvsl;
  float var_j2_qgl;
  float var_j2_ptD;
  float var_j2_axis1;
  float var_j2_axis2;
  int   var_j2_mult;

  float var_j3_pt;
  float var_j3_csv;
  float var_j3_cvsl;
  float var_j3_cvsb;
  float var_j3_dcsv;
  float var_j3_dcvsb;
  float var_j3_dcvsl;
  float var_j3_qgl;
  float var_j3_ptD;
  float var_j3_axis1;
  float var_j3_axis2;
  int   var_j3_mult;

  float var_topcand_pt;
  float var_topcand_mass;
  float var_topcand_ptDR;
  float var_b_wcand_deltaR;
  float var_wcand_mass;
  float var_wcand_ptDR;
  float var_wcand_pt;
  float var_wcand_deltaR;
  float var_chi2;
  float var_sd_0;
  float var_sd_0p5;
  float var_sd_1;
  float var_sd_1p5;
  float var_sd_n0p5;
  float var_sd_n1;
  float var_sd_n1p5;
  float var_sd_n2;
  float var_b_j2_mass;
  float var_b_j3_mass;
  float var_top_radius;
  int   var_n_extra_jets;

};

#endif
