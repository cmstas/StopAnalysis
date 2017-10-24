/*
 * ResolvedTopMVA.cc
 *
 *  Created on: Oct 6, 2016
 *      Author: hqu
 *  Modified on: Oct 23, 2017
 */

#include "ResolvedTopMVA.h"
#include "Math/VectorUtil.h"

using namespace tas;

std::map<TString, float> TopCand::calcTopCandVars() {
  std::map<TString, float> vars;

  vars["var_b_pt"]         = pfjets_p4().at(ib_).pt();
  vars["var_b_mass"]       = pfjets_p4().at(ib_).mass();
  vars["var_b_csv"]        = pfjets_pfCombinedInclusiveSecondaryVertexV2BJetTag().at(ib_);
  vars["var_b_ptD"]        = pfjets_ptDistribution().at(ib_);
  vars["var_b_axis1"]      = pfjets_axis1().at(ib_);
  vars["var_b_mult"]       = pfjets_totalMultiplicity().at(ib_);

  vars["var_j2_pt"]        = pfjets_p4().at(ij2_).pt();
  vars["var_j2_mass"]      = pfjets_p4().at(ij2_).mass();
  vars["var_j2_csv"]       = pfjets_pfCombinedInclusiveSecondaryVertexV2BJetTag().at(ij2_);
  vars["var_j2_cvsl"]      = getbtagvalue("pfCombinedCvsLJetTags", ij2_);
  vars["var_j2_ptD"]       = pfjets_ptDistribution().at(ij2_);
  vars["var_j2_axis1"]     = pfjets_axis1().at(ij2_);
  vars["var_j2_mult"]      = pfjets_totalMultiplicity().at(ij2_);

  vars["var_j3_pt"]        = pfjets_p4().at(ij3_).pt();
  vars["var_j3_mass"]      = pfjets_p4().at(ij3_).mass();
  vars["var_j3_csv"]       = pfjets_pfCombinedInclusiveSecondaryVertexV2BJetTag().at(ij3_);
  vars["var_j3_cvsl"]      = getbtagvalue("pfCombinedCvsLJetTags", ij3_);
  vars["var_j3_ptD"]       = pfjets_ptDistribution().at(ij3_);
  vars["var_j3_axis1"]     = pfjets_axis1().at(ij3_);
  vars["var_j3_mult"]      = pfjets_totalMultiplicity().at(ij3_);

  vars["var_topcand_mass"] = topcand.mass();
  vars["var_topcand_ptDR"] = topcand.pt() * ROOT::Math::VectorUtil::DeltaR(pfjets_p4().at(ib_), wcand);

  vars["var_wcand_mass"]   = wcand.mass();
  vars["var_wcand_ptDR"]   = wcand.pt() * ROOT::Math::VectorUtil::DeltaR(pfjets_p4().at(ij2_), pfjets_p4().at(ij3_));

  vars["var_sd_0"]         = pfjets_p4().at(ij3_).pt() / (pfjets_p4().at(ij2_).pt() + pfjets_p4().at(ij3_).pt());
  vars["var_sd_n2"]        = vars.at("var_sd_0") / std::pow(vars.at("var_wcand_deltaR"), -2);

  vars["var_b_j2_mass"]    = (pfjets_p4().at(ib_) + pfjets_p4().at(ij2_)).mass();
  vars["var_b_j2_mass"]    = (pfjets_p4().at(ib_) + pfjets_p4().at(ij3_)).mass();

  // vars["var_b_qgl"]        = topcand->b->qgl();
  // vars["var_b_cvsb"]       = topcand->b->cvsb();
  // vars["var_b_axis2"]      = topcand->b->axis2();
  // vars["var_j2_cvsb"]      = topcand->j2->cvsb();
  // vars["var_j2_qgl"]       = topcand->j2->qgl();
  // vars["var_j2_axis2"]     = topcand->j2->axis2();
  // vars["var_j3_cvsb"]      = topcand->j3->cvsb();
  // vars["var_j3_qgl"]       = topcand->j3->qgl();
  // vars["var_j3_axis2"]     = topcand->j3->axis2();
  // vars["var_sd_n1p5"]      = vars.at("var_sd_0")/std::pow(vars.at("var_wcand_deltaR"), -1.5);
  // vars["var_topcand_pt"]   = topcand.pt();
  // vars["var_b_wcand_deltaR"] = DeltaR(pfjets_p4().at(ib_), wcand);
  // vars["var_wcand_pt"]     = topcand->wcand.pt();
  // vars["var_wcand_deltaR"] = PhysicsUtilities::deltaR(*topcand->j2, *topcand->j3);

  return vars;
}

ResolvedTopMVA::ResolvedTopMVA(TString weightfile, TString mvaname)
    : mvaReader(weightfile, mvaname)
{
  initTopMVA();
}

ResolvedTopMVA::~ResolvedTopMVA() {}

void ResolvedTopMVA::initTopMVA() {
  // variable order must match the order in XML file

  vars = {
    "var_b_mass",
    "var_b_csv",
    "var_j2_csv",
    "var_j2_cvsl",
    "var_j2_ptD",
    "var_j2_axis1",
    "var_j3_csv",
    "var_j3_cvsl",
    "var_j3_ptD",
    "var_j3_axis1",
    "var_topcand_mass",
    "var_topcand_ptDR",
    "var_wcand_mass",
    "var_wcand_ptDR",
    "var_b_j2_mass",
    "var_b_j3_mass",
    "var_sd_n2",

    // "var_j2_pt",
    // "var_j2_axis2",
    // "var_j3_pt",
    // "var_j3_axis2",
    // "var_b_pt",
    // "var_topcand_pt",
    // "var_b_wcand_deltaR",
    // "var_wcand_pt",
    // "var_wcand_deltaR",

    "var_j2_mult",
    "var_j3_mult",
  };

  mvaReader.addVariables(vars);

}

std::vector<TopCand> ResolvedTopMVA::getTopCandidates(const double WP) {
  // need at least three jets to form a top
  if (pfjets_p4().size() < 3) return std::vector<TopCand>();

  // sort jets by CSV
  std::vector<size_t> jetidx( pfjets_p4().size() );
  std::iota(jetidx.begin(), jetidx.end(), 0);
  std::sort(jetidx.begin(), jetidx.end(), [&](size_t i, size_t j) { 
      return pfjets_pfCombinedInclusiveSecondaryVertexV2BJetTag().at(i) > pfjets_pfCombinedInclusiveSecondaryVertexV2BJetTag().at(j);
    });

  std::vector<TopCand> allCands;
  for (size_t ib = 0; ib < 2; ++ib) {
    // if(csvJets.at(iB)->csv() < defaults::CSV_LOOSE) break; // b must pass CSVL
    for (size_t ij2 = 0; ij2 < jetidx.size()-1; ++ij2) {
      if (ij2 == ib) continue;
      for (size_t ij3 = ij2+1; ij3 < jetidx.size(); ++ij3) {
        if (ij3 == ib) continue;
        TopCand tmpCand(jetidx[ib], jetidx[ij2], jetidx[ij3]);
        if (tmpCand.passMassW() && tmpCand.passMassTop()) {
          auto varMap = tmpCand.calcTopCandVars();
          for (const auto &v: vars) {
            mvaReader.setValue(v, varMap.at(v));
          }
          tmpCand.disc = mvaReader.eval();
          allCands.push_back(tmpCand);
        }
      }
    }
  }

  auto cands = removeOverlap(allCands, WP);
  return cands;
}

std::vector<TopCand> ResolvedTopMVA::removeOverlap(std::vector<TopCand>& cands, double threshold) {
  std::sort(cands.begin(), cands.end(), [](const TopCand &a, const TopCand &b) { return a.disc > b.disc; });

  std::vector<TopCand> cleanedCands;
  for (const auto &c : cands) {
    if (c.disc < threshold) break;
    bool isOverlap = false;
    for (const auto &cleaned : cleanedCands) {
      if (c.overlaps(cleaned)) {
        isOverlap = true; break;
      }
    }
    if(!isOverlap) cleanedCands.push_back(c);
  }

  return cleanedCands;
}
