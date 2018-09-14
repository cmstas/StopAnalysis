/*
 * ResolvedTopMVA.h
 *
 *  Created on: Oct 6, 2016
 *  Original author: hqu
 *  Modified on: Oct 23, 2017
 */

#ifndef STOPBABYMAKER_TOPTAGGER_RESOLVEDTOPMVA_H
#define STOPBABYMAKER_TOPTAGGER_RESOLVEDTOPMVA_H

#include "TopCand.h"
#include "CMS3.h"
#include "TMVAReader.h"

class ResolvedTopMVA {
 public:
  ResolvedTopMVA(TString weightfile, TString mvaname);
  virtual ~ResolvedTopMVA();

  // Working points for the MVA output
  static constexpr double WP_ALL    = -1.0; // used for candidate studies
  static constexpr double WP_LOOSE  = 0.83;
  static constexpr double WP_MEDIUM = 0.98;
  static constexpr double WP_TIGHT  = 0.99;

  std::map<TString,float> calcTopCandVars(const TopCand& topjet);
  std::vector<TopCand> getTopCandidates(const double WP = WP_TIGHT, const size_t max_ntopcand = 2);

  void setJetVecPtrs(std::vector<LorentzVector>* p4vec, std::vector<float>* csvvec, std::vector<float>* cvslvec,
                     std::vector<float>* ptDvec, std::vector<float>* axis1vec, std::vector<int>* multvec);

  const std::vector<LorentzVector>* p4vec;
  const std::vector<float>* csvvec;
  const std::vector<float>* cvslvec;
  const std::vector<float>* ptDvec;
  const std::vector<float>* axis1vec;
  const std::vector<int>* multvec;

 private:
  void initTopMVA();
  std::vector<TopCand> removeOverlap(std::vector<TopCand> &cands, double threshold);

  TMVAReader mvaReader;
  std::vector<TString> vars;

};

#endif /* STOPBABYMAKER_TOPTAGGER_RESOLVEDTOPMVA_H */
