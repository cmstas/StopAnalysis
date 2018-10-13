#include <stdexcept>
#include <cmath>
#include <numeric>
#include <algorithm>

#include "TopCandTree.h"
#include "TH1.h"
#include "TFile.h"
#include "TMath.h"
#include "Math/VectorUtil.h"

using namespace std;

namespace {
// Helper function for fast deltaR matching
const float PI = TMath::Pi();

bool isCloseObject(const LorentzVector p1, const LorentzVector p2, const float conesize, float* deltaR = nullptr) {
  float deltaEta = fabs(p1.eta() - p2.eta());
  if (deltaEta > conesize) return false;
  float deltaPhi = fabs(p1.phi() - p2.phi());
  if (deltaPhi > PI) deltaPhi = 2*PI - deltaPhi;
  if (deltaPhi > conesize) return false;
  float deltaR2 = deltaEta*deltaEta + deltaPhi*deltaPhi;
  if (deltaR2 > conesize*conesize) return false;

  if (deltaR) *deltaR = sqrt(deltaR2);
  return true;
}

template<typename... TArgs>
void plot1d(std::string name, double xval, double weight, std::map<std::string, TH1*> &allhistos, TArgs... args)
{
  auto iter = allhistos.find(name);
  if (iter == allhistos.end()) {
    TH1D* currentHisto= new TH1D(name.c_str(), args...);
    currentHisto->SetDirectory(0);
    currentHisto->Sumw2();
    currentHisto->Fill(xval, weight);
    allhistos.insert( std::pair<std::string, TH1*>(name, currentHisto) );
  } else {
    iter->second->Fill(xval, weight);
  }
}

}

TopCandTree::TopCandTree() :
    randGen(nullptr),
    max_nbcand(4),
    doSloppyMatch(true)
{}

TopCandTree::TopCandTree(string treeName, string outputName, string sampletype) : TopCandTree()
{
  Setup(treeName, outputName, sampletype);
}

TopCandTree::~TopCandTree() {
  delete tree;
  if (outfile) outfile->Close();
  delete outfile;
  delete randGen;
}

void TopCandTree::AddEventInfo(int evt, float wgt, float pfmet, int nPV, int nLeps, int nJets, int nBJets, int nLBJets, float MT, float tMod, float Mlb)
{
  event = evt;
  weight = wgt;
  met = pfmet;
  npv = nPV;
  nlep = nLeps;
  njets = nJets;
  nbjets = nBJets;
  nlbjets = nLBJets;
  mt = MT;
  mlb = Mlb;
  tmod = tMod;
}

void TopCandTree::AddTopCandInfo(const TopCand* topcand, int genmatch) {
  flag_signal = genmatch;

  int ib  = topcand->getIdxForb();
  int ij2 = topcand->getIdxForj2();
  int ij3 = topcand->getIdxForj3();

  var_b_pt      = jets_p4->at(ib).pt();
  var_b_mass    = jets_p4->at(ib).mass();
  var_b_csv     = jets_csv->at(ib);
  var_b_ptD     = jets_ptD->at(ib);
  var_b_axis1   = jets_axis1->at(ib);
  var_b_axis2   = jets_axis2->at(ib);
  var_b_mult    = jets_mult->at(ib);
  var_b_dcsv    = jets_deepcsvb->at(ib) + jets_deepcsvbb->at(ib);
  var_b_dcvsb   = jets_deepcsvc->at(ib) / (jets_deepcsvc->at(ib) + var_b_dcsv);
  var_b_dcvsl   = jets_deepcsvc->at(ib) / (jets_deepcsvc->at(ib) + jets_deepcsvl->at(ib));

  var_j2_pt     = jets_p4->at(ij2).pt();
  var_j2_csv    = jets_csv->at(ij2);
  var_j2_cvsl   = jets_cvsl->at(ij2);
  var_j2_ptD    = jets_ptD->at(ij2);
  var_j2_axis1  = jets_axis1->at(ij2);
  var_j2_axis2  = jets_axis2->at(ij2);
  var_j2_mult   = jets_mult->at(ij2);
  var_j2_dcsv   = jets_deepcsvb->at(ij2) + jets_deepcsvbb->at(ij2);
  var_j2_dcvsb  = jets_deepcsvc->at(ij2) / (jets_deepcsvc->at(ij2) + var_j2_dcsv);
  var_j2_dcvsl  = jets_deepcsvc->at(ij2) / (jets_deepcsvc->at(ij2) + jets_deepcsvl->at(ij2));

  var_j3_pt     = jets_p4->at(ij3).pt();
  var_j3_csv    = jets_csv->at(ij3);
  var_j3_cvsl   = jets_cvsl->at(ij3);
  var_j3_ptD    = jets_ptD->at(ij3);
  var_j3_axis1  = jets_axis1->at(ij3);
  var_j3_axis2  = jets_axis2->at(ij3);
  var_j3_mult   = jets_mult->at(ij3);
  var_j3_dcsv   = jets_deepcsvb->at(ij3) + jets_deepcsvbb->at(ij3);
  var_j3_dcvsb  = jets_deepcsvc->at(ij3) / (jets_deepcsvc->at(ij3) + var_j3_dcsv);
  var_j3_dcvsl  = jets_deepcsvc->at(ij3) / (jets_deepcsvc->at(ij3) + jets_deepcsvl->at(ij3));

  // var_b_cvsb   = jets_cvsb->at(ib);
  // var_b_qgl    = jets_qgl->at(ib);
  // var_j2_cvsb  = jets_cvsb->at(ij2);
  // var_j2_qgl   = jets_qgl->at(ij2);
  // var_j3_cvsb  = jets_cvsb->at(ij3);
  // var_j3_qgl   = jets_qgl->at(ij3);

  var_b_wcand_deltaR = ROOT::Math::VectorUtil::DeltaR(jets_p4->at(ib), topcand->wcand);
  var_topcand_pt    = topcand->topcand.pt();
  var_topcand_ptDR  = topcand->topcand.pt()*var_b_wcand_deltaR;
  var_topcand_mass  = topcand->topcand.mass();

  var_wcand_deltaR = ROOT::Math::VectorUtil::DeltaR(jets_p4->at(ij2), jets_p4->at(ij3));
  var_wcand_pt      = topcand->wcand.pt();
  var_wcand_ptDR    = topcand->wcand.pt()*var_wcand_deltaR;
  var_wcand_mass    = topcand->wcand.mass();

  auto calcChi2 = [](const TopCand &c) {
      double m_top, m_w, sigma_top, sigma_w;
      if (c.topcand.pt()<250)       { m_top=161; sigma_top=18; m_w=79; sigma_w=11; }
      else if (c.topcand.pt()<350)  { m_top=167; sigma_top=16; m_w=81; sigma_w=10; }
      else                          { m_top=172; sigma_top=16; m_w=84; sigma_w=10; }
      return std::pow((c.topcand.mass()-m_top)/sigma_top, 2)+std::pow((c.wcand.mass()-m_w)/sigma_w, 2);
    };
  var_chi2 = calcChi2(*topcand);

  var_sd_0    = jets_p4->at(ij3).pt() / (jets_p4->at(ij2).pt()+jets_p4->at(ij3).pt());
  // var_sd_0p5  = var_sd_0/std::pow(var_wcand_deltaR, 0.5);
  // var_sd_1    = var_sd_0/var_wcand_deltaR;
  // var_sd_1p5  = var_sd_0/std::pow(var_wcand_deltaR, 1.5);
  // var_sd_n0p5 = var_sd_0/std::pow(var_wcand_deltaR, -0.5);
  var_sd_n1   = var_sd_0/std::pow(var_wcand_deltaR, -1);
  // var_sd_n1p5 = var_sd_0/std::pow(var_wcand_deltaR, -1.5);
  var_sd_n2   = var_sd_0/std::pow(var_wcand_deltaR, -2);

  var_b_j2_mass = (jets_p4->at(ib) + jets_p4->at(ij2)).mass();
  var_b_j3_mass = (jets_p4->at(ib) + jets_p4->at(ij3)).mass();

  double top_radius = -1;
  for (const auto j : {ib, ij2, ij3}) {
    double dR = ROOT::Math::VectorUtil::DeltaR(jets_p4->at(j), topcand->topcand);
    if (dR > top_radius) top_radius = dR;
  }
  var_top_radius = top_radius;

  int n_extra_jets = 0;
  for (int j = 0; j < (int) jets_p4->size(); ++j) {
    if (j == ib || j == ij2 || j == ij3) continue;
    if (ROOT::Math::VectorUtil::DeltaR(topcand->topcand, jets_p4->at(j)) < top_radius)
      ++n_extra_jets;
  }
  var_n_extra_jets = n_extra_jets;
}

void TopCandTree::AddGenTopInfo(int itop) {
  if (!genps_p4) throw std::logic_error("[TopCandTree::AddGenTopInfo]: The genps branches hasn't get setup!! Cannot fill gen information!");

  gen_top_pt = genps_p4->at(itop).pt();
  // gen_w_pt,   topDecay->W->pt() ;
  // gen_b_diag, topDecay->b->diag ;
  // gen_w_diag, topDecay->W_decay->diag ;
  // gen_top_resolved, true ;
}

int TopCandTree::FillTreeFromGenTop() {

  if (njets < 4) return -1;
  if (met < 100) return -1;
  if (nbjets < 1) return -1;
  // if (nlbjets < 2) return -1;

  const bool verbose = true;
  const bool testplots = true;
  jetset_fromtop.clear();

  vector<int> gentopidx;
  for (size_t q = 0; q < genps_id->size(); ++q) {
    if (!genps_isLastCopy->at(q)) continue;
    if (abs(genps_id->at(q)) != 6) continue;
    gentopidx.push_back(q);
  }
  if (testplots) plot1d("h_sanity_ngentop", gentopidx.size(), 1, hvec, ";N gen top", 5, 0, 5);

  // Find all genps that decay from top
  for (int itop : gentopidx) {  // in case there are 2 tops (or more!)
    vector<int> genq_fromtop;
    for (size_t q = 0; q < genps_id->size(); ++q) {
      if (!genps_isLastCopy->at(q)) continue;
      if (abs(genps_id->at(q)) > 5 ) continue;
      if ((abs(genps_id->at(q)) == 5 && abs(genps_motheridx->at(q)) == itop) ||
          (abs(genps_motherid->at(q)) == 24 && abs(genps_motheridx->at(genps_motheridx->at(q))) == itop))
        genq_fromtop.push_back(q);

      // // Sanity checks
      // if (verbose) {
      //   if (abs(genps_motheridx->at(q)) == itop && abs(genps_id->at(q)) != 5)
      //     cout << __FILE__ << ":" << __LINE__ << ": Mother is top but itself is not b!! genps_id->at(q)= " << genps_id->at(q) << endl;
      //   if (abs(genps_motheridx->at(genps_motheridx->at(q))) == itop && abs(genps_motherid->at(q)) == 24 && abs(genps_id->at(q)) == 5)
      //     // cout << __FILE__ << ":" << __LINE__ << ": Gandmother is top but mother is not W!! genps_id->at(q)= " << genps_id->at(q) << ", genps_motherid->at(q)= " << genps_motherid->at(q) << endl;
      //     cout << __FILE__ << ":" << __LINE__ << ": A b whose mother is W!! genps_id->at(q)= " << genps_id->at(q) << ", genps_motherid->at(q)= " << genps_motherid->at(q) << endl;
      // }
    }

    if (testplots) plot1d("h_ngenq_fromtop", genq_fromtop.size(), 1, hvec, ";N gen q from top", 7, 0, 7); // should be 1 or 3
    if (genq_fromtop.size() == 1) continue;      // this is a leptonic decaying top, skip
    else if (genq_fromtop.size() < 3) return -1; // this is a rare case of lost gen particle, skip the event

    vector<int> jets_fromtop;
    vector<pair<int,int>> jet_quark_pair;  // jet-quark pair
    long matchedjetidx = 0;                // for fast duplicate check
    // int bjetidx = -1;
    if (verbose && alljets_p4->size() > 63) cout << __FILE__ << ":" << __LINE__ << ": We have a super large jet vector!! jets_p4->size()= " << jets_p4->size() << endl;
    // Sort genq_fromtop first to put the b quark from top at first
    std::sort(genq_fromtop.begin(), genq_fromtop.end(), [&](int q1, int q2) { return abs(genps_motherid->at(q1)) == 6; });
    for (int q : genq_fromtop) {
      int genqid = genps_id->at(q);
      int jetidx = -1;
      float minDR = 0.6;
      for (size_t j = 0; j < alljets_p4->size(); ++j) {
        if (alljets_partonid->at(j) != genqid) continue;  // use genjet info to narrow down
        float dr = 1;
        if (isCloseObject(alljets_p4->at(j), genps_p4->at(q), 0.6, &dr) && dr < minDR) {
          minDR = dr;
          jetidx = j;
        }
      }
      if (minDR < 0.6 && !(matchedjetidx & 1<<jetidx)) {
        matchedjetidx |= 1<<jetidx;
        jets_fromtop.push_back(jetidx);
        if (abs(genqid) == 5 && abs(genps_motherid->at(q)) == 6) {
          // bjetidx = jetidx;
          if (testplots) {
            plot1d("h_genbfromtop_pt", genps_p4->at(q).pt(), 1, hvec, ";p_{T}(gen b from top)", 100, 0, 500);
            plot1d("h_genbfromtop_eta", genps_p4->at(q).eta(), 1, hvec, ";#eta(gen b from top)", 100, -5, 5);
            plot1d("h_bjetfromtop_pt", alljets_p4->at(jetidx).pt(), 1, hvec, ";p_{T}(gen b from top)", 100, 0, 500);
            plot1d("h_bjetfromtop_eta", alljets_p4->at(jetidx).eta(), 1, hvec, ";#eta(gen b from top)", 100, -5, 5);
          }
        }
        jet_quark_pair.emplace_back(jetidx, q);
      }
    }

    if (jets_fromtop.size() > 2)
      std::sort(jets_fromtop.begin()+1, jets_fromtop.end(), [&](int j1, int j2) { return alljets_p4->at(j1).pt() > alljets_p4->at(j2).pt(); });

    vector<int> jet10_fromtop;
    vector<int> jet15_fromtop;
    vector<int> jet20_fromtop;
    vector<int> jet30_fromtop;
    for (int j : jets_fromtop) {
      if (fabs(alljets_p4->at(j).eta()) > 2.4) continue;
      if (alljets_p4->at(j).pt() < 10) continue;
      jet10_fromtop.push_back(j);
      if (alljets_p4->at(j).pt() < 15) continue;
      jet15_fromtop.push_back(j);
      if (alljets_p4->at(j).pt() < 20) continue;
      jet20_fromtop.push_back(j);
      if (alljets_p4->at(j).pt() < 30) continue;
      jet30_fromtop.push_back(j);
    }

    auto fillLostJetPlot = [&](vector<int>& jetidxs, string suf) {
      plot1d("h_njets"+suf+"_fromtop", jetidxs.size(), 1, hvec, ";N jets fromtop", 7, 0, 7);
      if (mt > 150) plot1d("h_njets"+suf+"_fromtop_mt150", jetidxs.size(), 1, hvec, ";N jets fromtop", 7, 0, 7);

      // Find properties of the gen_q that didn't have jet-match
      if (jetidxs.size() < 3) {
        for (auto q : genq_fromtop) {
          bool matched = false;
          for (auto jq : jet_quark_pair) {
            if (q == jq.second) {
              matched = true; break;
            }
          }
          if (!matched) {
            plot1d("h_lostjet"+suf+"_genq_id", abs(genps_id->at(q)), 1, hvec, ";pdgID", 7, 0, 7);
            plot1d("h_lostjet"+suf+"_genq_pt", genps_p4->at(q).pt(), 1, hvec, ";p_{T}(gen q from top)", 50, 0, 200);
            plot1d("h_lostjet"+suf+"_genq_eta", genps_p4->at(q).eta(), 1, hvec, ";#eta(gen q from top)", 50, -5, 5);
            if (fabs(genps_p4->at(q).eta()) < 2.4)
              plot1d("h_lostjet"+suf+"_ineta_genq_pt", genps_p4->at(q).pt(), 1, hvec, ";p_{T}(gen q from top)", 50, 0, 200);
            if (genps_p4->at(q).pt() > 30)
              plot1d("h_lostjet"+suf+"_inpt_genq_eta", genps_p4->at(q).eta(), 1, hvec, ";#eta(gen q from top)", 50, -5, 5);

            float minDR_genqs = 7;
            for (auto q2 : genq_fromtop) {
              float dr = 7;
              if (q != q2 && isCloseObject(genps_p4->at(q), genps_p4->at(q2), 6, &dr) && dr < minDR_genqs)
                minDR_genqs = dr;
            }
            plot1d("h_lostjet"+suf+"_genq_minDR", minDR_genqs, 1, hvec, ";#DeltaR(gen quarks)", 50, 0, 5);
          } else {
            plot1d("h_acptjet"+suf+"_genq_pt", genps_p4->at(q).pt(), 1, hvec, ";p_{T}(gen q from top)", 50, 0, 200);
            plot1d("h_acptjet"+suf+"_genq_eta", genps_p4->at(q).eta(), 1, hvec, ";#eta(gen q from top)", 50, -5, 5);
          }
        }
      }

      // To test if the masses are passed
      else if (jetidxs.size() == 3) {
        TopCand tc(jetidxs[0], jetidxs[1], jetidxs[2], alljets_p4);
        plot1d("h_truecand"+suf+"_topcandpt", tc.topcand.pt(), 1, hvec, ";p_{T}(true topcand)", 60, 0, 600);
        plot1d("h_truecand"+suf+"_topmass", tc.topcand.mass(), 1, hvec, ";Mass(true topcand)", 60, 0, 300);
        plot1d("h_truecand"+suf+"_Wmass", tc.wcand.mass(), 1, hvec, ";Mass(true Wcand)", 60, 0, 300);
        int cat = tc.passMassW() + 2*tc.passMassTop();
        plot1d("h_truecand"+suf+"_cat", cat, 1, hvec, ";category", 5, 0, 5);
      }
    };

    if (testplots) {
      fillLostJetPlot(jets_fromtop, "_allpf");
      fillLostJetPlot(jet10_fromtop, "10");
      fillLostJetPlot(jet15_fromtop, "15");
      fillLostJetPlot(jet20_fromtop, "20");
      fillLostJetPlot(jet30_fromtop, "30");
    }

    if (testplots) {
      // General for DR between gen_qs
      float minDR_genqs = 7;
      for (auto q1 : genq_fromtop) {
        for (auto q2 : genq_fromtop) {
          float dr = 7;
          if (q1 != q2 && isCloseObject(genps_p4->at(q1), genps_p4->at(q2), 6, &dr) && dr < minDR_genqs)
            minDR_genqs = dr;
        }
      }
      plot1d("h_minDR_genqs", minDR_genqs, 1, hvec, ";#DeltaR(gen quarks)", 50, 0, 5);
    }

    if (jet10_fromtop.size() >= 3) {
      TopCand topcand(jets_fromtop[0], jets_fromtop[1], jets_fromtop[2], alljets_p4);
      AddTopCandInfo(&topcand, true);
      AddGenTopInfo(itop);
      flag_shuffle = false;
      min_jetpt = std::min(alljets_p4->at(topcand.ib_).pt(), alljets_p4->at(topcand.ij3_).pt());
      max_jeteta = std::max(fabs(alljets_p4->at(topcand.ib_).eta()), std::max(fabs(alljets_p4->at(topcand.ij2_).eta()), fabs(alljets_p4->at(topcand.ij3_).eta())));
      tree->Fill();
      jetset_fromtop.push_back(jets_fromtop);
      truecands.push_back(topcand);
      if (testplots) {
        plot1d("h_truecand_min_jetpt", min_jetpt, 1, hvec, ";min(jet pt)", 60, 0, 150);
        plot1d("h_truecand_max_jeteta", max_jeteta, 1, hvec, ";max(jet eta)", 50, 0, 5);
      }
    }
  }

  // return jetset_fromtop.size();
  return jetset_fromtop.size();
}

int TopCandTree::IsGenTopMatched(const TopCand* topcand) {

  if (!processed_gen) {
    // GetJetsFromGenTop();
    processed_gen = true;
  }

  return 0;
}

int TopCandTree::IsGenTopMatchedSloppy(const TopCand* topcand) {
  // Sloppy version that check if the 3 jet combination match to gen quarks that decay from a top
  //
  // also doesn't care about if jet not matching to any gen part (return -1)

  if (!genps_id) throw std::logic_error("[TopCandTree::IsGenTopMatched]: The genps branches hasn't get setup!! Cannot perform gen-matching!");


  vector<int> gentopidx;
  for (size_t q = 0; q < genps_id->size(); ++q) {
    if (!genps_isLastCopy->at(q)) continue;
    if (abs(genps_id->at(q)) != 6) continue;
    gentopidx.push_back(q);
  }

  // Find all genps that decay from top
  // int result = 0;
  vector<int> genqidx_fromtop;
  for (int itop : gentopidx) {  // in case there are 2 tops
    for (size_t q = 0; q < genps_id->size(); ++q) {
      if (!genps_isLastCopy->at(q)) continue;
      if (abs(genps_id->at(q)) > 5 ) continue;
      if ((abs(genps_id->at(q)) == 5 && abs(genps_motheridx->at(q)) == itop) ||
          (abs(genps_motherid->at(q)) == 24 && abs(genps_motheridx->at(genps_motheridx->at(q))) == itop))
        genqidx_fromtop.push_back(q);
    }
    if (genqidx_fromtop.size() != 3) { // This is a leptonic decaying top, skip
      genqidx_fromtop.clear();
      continue;
    }
    // if (genqidx_fromtop.size() != 3) return -2;  // but shouldn't happen

    // Check that the genqs from top match with the selection jets with deltaR < 0.3
    bool b_matched = false;
    for (size_t ijet : {topcand->ib_, topcand->ij2_, topcand->ij3_}) {
      float minDR = 0.6;
      auto matchq = genqidx_fromtop.end();
      for (auto itq = genqidx_fromtop.begin(); itq != genqidx_fromtop.end(); ++itq) {
        float thisDR = 0.6;
        if (!isCloseObject(jets_p4->at(ijet), genps_p4->at(*itq), 0.3, &thisDR)) continue;
        if (thisDR > minDR) continue;
        minDR = thisDR;
        matchq = itq;
      }
      if (minDR > 0.3) break;
      if (ijet == topcand->ib_ && abs(genps_id->at(*matchq)) == 5 && abs(genps_motherid->at(*matchq)) == 6)
        b_matched = true;
      genqidx_fromtop.erase(matchq);
    }
    if (genqidx_fromtop.size() == 0) {
      AddGenTopInfo(itop);
      return true + !b_matched;  // return 2 if is a permutation of the top cand
    }
  }

  return false;
}

void TopCandTree::FillTree() {

  // Need to check that event variables are filled
  if (event < 0) cout << "[TopCandTree::FillTree]>> Need to fill in event variables first!!\n";

  // if (!goodvtx()) return;
  if (njets < 4) return;
  if (met < 100) return;
  if (nbjets < 1) return;
  if (nlbjets < 2) return;

  // jets sorted by CSV, the jet vector size should be at least 3
  std::vector<size_t> jetidx( jets_p4->size() );
  std::iota(jetidx.begin(), jetidx.end(), 0);
  std::sort(jetidx.begin(), jetidx.end(), [&](size_t i, size_t j) { return jets_csv->at(i) > jets_csv->at(j); });

  // ------------------------------
  // form all possible candidates
  vector<TopCand> allCands;
  for (size_t ib = 0; ib < max_nbcand; ++ib) {
    // if(csvJets.at(iB)->csv() < defaults::CSV_LOOSE) break; // b must pass CSVL
    for (size_t ij2 = 0; ij2 < jetidx.size()-1; ++ij2) {
      if (ij2 == ib) continue;
      for (size_t ij3 = ij2+1; ij3 < jetidx.size(); ++ij3) {
        if (ij3 == ib) continue;
        TopCand tmpCand(jetidx[ib], jetidx[ij2], jetidx[ij3], jets_p4);
        if (tmpCand.passMassW() && tmpCand.passMassTop())
          allCands.push_back(tmpCand);
      }
    }
  }

  if (allCands.empty()) return;

  auto addOverlapInfo = [&](const TopCand& topcand) {
      flag_shuffle = false;
      for (const auto &c : allCands) {
        if (&c != &topcand && topcand.similarAs(c)) {
          flag_shuffle = true;
          break;
        }
      }
    };

  // ------------------------------
  // treat signal and bkg differently
  if (!randGen) {
    vector<const TopCand*> fakeCands;
    int ntruecand_filled = (doSloppyMatch)? 0 : truecands.size();
    for (const auto &topcand : allCands) {
      int type = (doSloppyMatch)? IsGenTopMatchedSloppy(&topcand) : 0;
      if (type > 0) {
        AddTopCandInfo(&topcand, type);
        addOverlapInfo(topcand);
        min_jetpt = std::min(jets_p4->at(topcand.ib_).pt(), jets_p4->at(topcand.ij3_).pt());
        max_jeteta = std::max(fabs(jets_p4->at(topcand.ib_).eta()), std::max(fabs(jets_p4->at(topcand.ij2_).eta()), fabs(jets_p4->at(topcand.ij3_).eta())));
        tree->Fill();
        ntruecand_filled++;
      } else {
        fakeCands.push_back(&topcand);
      }
    }
    std::random_shuffle(fakeCands.begin(), fakeCands.end());
    int nfakecand_filled = 0;
    for (size_t i = 0; i < fakeCands.size(); ++i) {
      if (ntruecand_filled + nfakecand_filled > 2) break;
      min_jetpt = std::min(jets_p4->at(fakeCands.at(i)->ib_).pt(), jets_p4->at(fakeCands.at(i)->ij3_).pt());
      max_jeteta = std::max(fabs(jets_p4->at(fakeCands.at(i)->ib_).eta()), std::max(fabs(jets_p4->at(fakeCands.at(i)->ij2_).eta()), fabs(jets_p4->at(fakeCands.at(i)->ij3_).eta())));
      if ((min_jetpt < 20 || max_jeteta > 2.4) && event %3 != 0) continue;
      if ((min_jetpt < 15) && event %5 != 0) continue;
      int type = false;
      if (!doSloppyMatch) {
        for (auto tc : truecands) {
          if (tc.sameAs(*fakeCands.at(i))) type = 1;
          else if (tc.similarAs(*fakeCands.at(i))) type = 2;
        }
        if (type == 1) continue;
      }
      AddTopCandInfo(fakeCands.at(i), type);
      addOverlapInfo(*(fakeCands.at(i)));
      tree->Fill();
      nfakecand_filled++;
    }
  } else {
    // Fill at most 2 candiates from each event
    for (int i = 0; i < 2 && allCands.size() > 0; ++i) {
      size_t rndidx = randGen->Uniform(0, allCands.size());
      AddTopCandInfo(&allCands.at(rndidx), false);
      tree->Fill();
      allCands.erase(allCands.begin()+rndidx);
    }
  }

}

void TopCandTree::Setup(string treeName, string outputName, string sampletype)
{
  outfile = new TFile(outputName.c_str(), "RECREATE");
  tree = new TTree("tree", "Flat ntuple for top tagger training");
  if (sampletype.find("tt") == string::npos)
    randGen = new TRandom3();

  SetBranches();
}

void TopCandTree::ResetAll() {

  // Reset these branches every event, just to be save
  jets_p4 = nullptr;
  jets_csv = nullptr;
  jets_cvsl = nullptr;
  jets_ptD = nullptr;
  jets_axis1 = nullptr;
  jets_axis2 = nullptr;
  jets_mult = nullptr;
  jets_deepcsvb = nullptr;
  jets_deepcsvc = nullptr;
  jets_deepcsvl = nullptr;
  jets_deepcsvbb = nullptr;

  genps_p4 = nullptr;
  genps_id = nullptr;
  genps_isLastCopy = nullptr;
  genps_motherid = nullptr;
  genps_motheridx = nullptr;

  event = -1;
  weight = -1;
  // npv = -1;
  nlep = -1;
  // ntau = -1;
  // ntrk = -1;
  njets = -1;
  nbjets = -1;
  nlbjets = -1;
  flag_signal = -1;
  met = -1;
  // ht = -1;
  mt = -1;
  mlb = -1;
  tmod = -1;
  min_jetpt = -1;
  max_jeteta = -1;

  processed_gen = false;
  jetset_fromtop.clear();
  truecands.clear();

  gen_top_pt = -1;
  gen_w_pt = -1;
  gen_b_diag = -1;
  gen_w_diag = -1;
  gen_top_resolved = -1;

  var_b_pt = -1;
  var_b_mass = -1;
  var_b_csv = -1;
  var_b_cvsb = -1;
  var_b_dcsv = -1;
  var_b_dcvsb = -1;
  var_b_dcvsl = -1;
  var_b_qgl = -1;
  var_b_ptD = -1;
  var_b_axis1 = -1;
  var_b_axis2 = -1;
  var_b_mult = -1;

  var_j2_pt = -1;
  var_j2_csv = -1;
  var_j2_cvsl = -1;
  var_j2_cvsb = -1;
  var_j2_dcsv = -1;
  var_j2_dcvsb = -1;
  var_j2_dcvsl = -1;
  var_j2_qgl = -1;
  var_j2_ptD = -1;
  var_j2_axis1 = -1;
  var_j2_axis2 = -1;
  var_j2_mult = -1;

  var_j3_pt = -1;
  var_j3_csv = -1;
  var_j3_cvsl = -1;
  var_j3_cvsb = -1;
  var_j3_dcsv = -1;
  var_j3_dcvsb = -1;
  var_j3_dcvsl = -1;
  var_j3_qgl = -1;
  var_j3_ptD = -1;
  var_j3_axis1 = -1;
  var_j3_axis2 = -1;
  var_j3_mult = -1;

  var_topcand_pt = -1;
  var_topcand_mass = -1;
  var_topcand_ptDR = -1;
  var_b_wcand_deltaR = -1;
  var_wcand_mass = -1;
  var_wcand_ptDR = -1;
  var_wcand_pt = -1;
  var_wcand_deltaR = -1;
  var_chi2 = -1;
  var_sd_0 = -1;
  var_sd_0p5 = -1;
  var_sd_1 = -1;
  var_sd_1p5 = -1;
  var_sd_n0p5 = -1;
  var_sd_n1 = -1;
  var_sd_n1p5 = -1;
  var_sd_n2 = -1;
  var_b_j2_mass = -1;
  var_b_j3_mass = -1;
  var_top_radius = -1;
  var_n_extra_jets = -1;
}

void TopCandTree::SetBranches() {
  tree->Branch("event", &event);
  tree->Branch("weight", &weight);
  tree->Branch("npv", &npv);
  tree->Branch("nlep", &nlep);
  tree->Branch("njets", &njets);
  tree->Branch("nbjets", &nbjets);
  tree->Branch("nlbjets", &nlbjets);
  tree->Branch("flag_signal", &flag_signal);
  tree->Branch("met", &met);
  tree->Branch("mt", &mt);
  tree->Branch("mlb", &mlb);
  tree->Branch("tmod", &tmod);
  tree->Branch("min_jetpt", &min_jetpt);
  tree->Branch("max_jeteta", &max_jeteta);
  // tree->Branch("ntau", &ntau);
  // tree->Branch("ntrk", &ntrk);
  // tree->Branch("ht", &ht);

  tree->Branch("gen_top_pt", &gen_top_pt);
  tree->Branch("gen_w_pt", &gen_w_pt);
  tree->Branch("gen_b_diag", &gen_b_diag);
  tree->Branch("gen_w_diag", &gen_w_diag);
  tree->Branch("gen_top_resolved", &gen_top_resolved);

  tree->Branch("var_b_pt", &var_b_pt);
  tree->Branch("var_b_mass", &var_b_mass);
  tree->Branch("var_b_csv", &var_b_csv);
  // tree->Branch("var_b_cvsb", &var_b_cvsb);
  tree->Branch("var_b_dcsv", &var_b_dcsv);
  tree->Branch("var_b_dcvsb", &var_b_dcvsb);
  tree->Branch("var_b_dcvsl", &var_b_dcvsl);
  // tree->Branch("var_b_qgl", &var_b_qgl);
  tree->Branch("var_b_ptD", &var_b_ptD);
  tree->Branch("var_b_axis1", &var_b_axis1);
  tree->Branch("var_b_axis2", &var_b_axis2);
  tree->Branch("var_b_mult", &var_b_mult);

  tree->Branch("var_j2_pt", &var_j2_pt);
  tree->Branch("var_j2_csv", &var_j2_csv);
  tree->Branch("var_j2_cvsl", &var_j2_cvsl);
  // tree->Branch("var_j2_cvsb", &var_j2_cvsb);
  tree->Branch("var_j2_dcsv", &var_j2_dcsv);
  tree->Branch("var_j2_dcvsb", &var_j2_dcvsb);
  tree->Branch("var_j2_dcvsl", &var_j2_dcvsl);
  // tree->Branch("var_j2_qgl", &var_j2_qgl);
  tree->Branch("var_j2_ptD", &var_j2_ptD);
  tree->Branch("var_j2_axis1", &var_j2_axis1);
  tree->Branch("var_j2_axis2", &var_j2_axis2);
  tree->Branch("var_j2_mult", &var_j2_mult);

  tree->Branch("var_j3_pt", &var_j3_pt);
  tree->Branch("var_j3_csv", &var_j3_csv);
  tree->Branch("var_j3_cvsl", &var_j3_cvsl);
  // tree->Branch("var_j3_cvsb", &var_j3_cvsb);
  tree->Branch("var_j3_dcsv", &var_j3_dcsv);
  tree->Branch("var_j3_dcvsb", &var_j3_dcvsb);
  tree->Branch("var_j3_dcvsl", &var_j3_dcvsl);
  // tree->Branch("var_j3_qgl", &var_j3_qgl);
  tree->Branch("var_j3_ptD", &var_j3_ptD);
  tree->Branch("var_j3_axis1", &var_j3_axis1);
  tree->Branch("var_j3_axis2", &var_j3_axis2);
  tree->Branch("var_j3_mult", &var_j3_mult);

  tree->Branch("var_topcand_mass", &var_topcand_mass);
  tree->Branch("var_topcand_pt", &var_topcand_pt);
  tree->Branch("var_topcand_ptDR", &var_topcand_ptDR);
  tree->Branch("var_b_wcand_deltaR", &var_b_wcand_deltaR);
  tree->Branch("var_wcand_mass", &var_wcand_mass);
  tree->Branch("var_wcand_ptDR", &var_wcand_ptDR);
  tree->Branch("var_wcand_pt", &var_wcand_pt);
  tree->Branch("var_wcand_deltaR", &var_wcand_deltaR);
  tree->Branch("var_chi2", &var_chi2);
  tree->Branch("var_sd_0", &var_sd_0);
  // tree->Branch("var_sd_0p5", &var_sd_0p5);
  // tree->Branch("var_sd_1", &var_sd_1);
  // tree->Branch("var_sd_1p5", &var_sd_1p5);
  // tree->Branch("var_sd_n0p5", &var_sd_n0p5);
  tree->Branch("var_sd_n1", &var_sd_n1);
  // tree->Branch("var_sd_n1p5", &var_sd_n1p5);
  tree->Branch("var_sd_n2", &var_sd_n2);
  tree->Branch("var_b_j2_mass", &var_b_j2_mass);
  tree->Branch("var_b_j3_mass", &var_b_j3_mass);
  tree->Branch("var_top_radius", &var_top_radius);
  // tree->Branch("var_n_extra_jets", &var_n_extra_jets);

}

void TopCandTree::SetJetVectors(const vector<LorentzVector>* p4, const vector<float>* csv, const vector<float>* cvsl,
                   const vector<float>* ptD, const vector<float>* axis1, const vector<float>* axis2, const vector<int>* mult,
                   const vector<float>* deepcsvb, const vector<float>* deepcsvc, const vector<float>* deepcsvl, const vector<float>* deepcsvbb)
{
  jets_p4 = p4;
  jets_csv = csv;
  jets_cvsl = cvsl;
  jets_ptD = ptD;
  jets_axis1 = axis1;
  jets_axis2 = axis2;
  jets_mult = mult;

  jets_deepcsvb = deepcsvb;
  jets_deepcsvc = deepcsvc;
  jets_deepcsvl = deepcsvl;
  jets_deepcsvbb = deepcsvbb;
}

void TopCandTree::SetGenParticleVectors(const vector<LorentzVector>* p4, const vector<int>* id, const vector<int>* motherid, const vector<int>* motheridx,
                                        const vector<bool>* isLastCopy, const vector<LorentzVector>* alljetp4, const vector<int>* partonid)
{
  genps_p4 = p4;
  genps_id = id;
  genps_motherid = motherid;
  genps_motheridx = motheridx;
  genps_isLastCopy = isLastCopy;
  alljets_p4 = alljetp4;
  alljets_partonid = partonid;
}

void TopCandTree::Write()
{
  outfile->cd();
  tree->Write();

  if (hvec.size() > 0) {
    TDirectory* dir = outfile->mkdir("TestPlots");
    dir->cd();
    for (auto& h : hvec) {
      // Move overflows of the yield hist to the last bin of histograms
      int nbin = h.second->GetNbinsX();
      if (h.second->GetBinContent(nbin+1) > 0) {
        double err = 0;
        h.second->SetBinContent(nbin, h.second->IntegralAndError(nbin, -1, err));
        h.second->SetBinError(nbin, err);
        h.second->SetBinContent(nbin+1, 0);
        h.second->SetBinError(nbin+1, 0);
      }

      h.second->Write();
    }
  }

}
