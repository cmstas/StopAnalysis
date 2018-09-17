#include <stdexcept>
#include <cmath>
#include <numeric>
#include <algorithm>

#include "TopCandTree.h"
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

}

TopCandTree::TopCandTree() :
    randGen(nullptr),
    max_ntopcand(4)
{}

TopCandTree::TopCandTree(string treeName, string outputName, string sampletype) :
    randGen(nullptr),
    max_ntopcand(4)
{
  Setup(treeName, outputName, sampletype);
}

TopCandTree::~TopCandTree() {}

void TopCandTree::AddEventInfo(int evt, float wgt, float pfmet, int nPV, int nleps, int ntaus, int ntrks, float HT, int nJets, int nBJets, int nLBJets)
{
  event = evt;
  weight = wgt;
  met = pfmet;
  npv = nPV;
  nvetolep = nleps;
  nvetotau = ntaus;
  nvetotrk = ntrks;
  ht = HT;
  njets = nJets;
  nbjets = nBJets;
  nlbjets = nLBJets;
}

void TopCandTree::AddTopCandInfo(const TopCand* topcand, bool isSignal) {
  flag_signal = isSignal;

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
  var_b_dcsvb   = jets_deepcsvb->at(ib);
  var_b_dcsvc   = jets_deepcsvc->at(ib);
  var_b_dcsvl   = jets_deepcsvl->at(ib);
  var_b_dcsvbb  = jets_deepcsvbb->at(ib);
  var_b_dcsv    = var_b_dcsvb + var_b_dcsvbb;

  var_j2_pt     = jets_p4->at(ij2).pt();
  var_j2_csv    = jets_csv->at(ij2);
  var_j2_cvsl   = jets_cvsl->at(ij2);
  var_j2_ptD    = jets_ptD->at(ij2);
  var_j2_axis1  = jets_axis1->at(ij2);
  var_j2_axis2  = jets_axis2->at(ij2);
  var_j2_mult   = jets_mult->at(ij2);
  var_j2_dcsvb  = jets_deepcsvb->at(ij2);
  var_j2_dcsvc  = jets_deepcsvc->at(ij2);
  var_j2_dcsvl  = jets_deepcsvl->at(ij2);
  var_j2_dcsvbb = jets_deepcsvbb->at(ij2);
  var_j2_dcsv   = var_j2_dcsvb + var_j2_dcsvbb;

  var_j3_pt     = jets_p4->at(ij3).pt();
  var_j3_csv    = jets_csv->at(ij3);
  var_j3_cvsl   = jets_cvsl->at(ij3);
  var_j3_ptD    = jets_ptD->at(ij3);
  var_j3_axis1  = jets_axis1->at(ij3);
  var_j3_axis2  = jets_axis2->at(ij3);
  var_j3_mult   = jets_mult->at(ij3);
  var_j3_dcsvb  = jets_deepcsvb->at(ij3);
  var_j3_dcsvc  = jets_deepcsvc->at(ij3);
  var_j3_dcsvl  = jets_deepcsvl->at(ij3);
  var_j3_dcsvbb = jets_deepcsvbb->at(ij3);
  var_j3_dcsv   = var_j3_dcsvb + var_j3_dcsvbb;

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

  var_sd_0 = jets_p4->at(ij3).pt() / (jets_p4->at(ij2).pt()+jets_p4->at(ij3).pt());
  var_sd_0    = var_sd_0;
  var_sd_0p5  = var_sd_0/std::pow(var_wcand_deltaR, 0.5);
  var_sd_1    = var_sd_0/var_wcand_deltaR;
  var_sd_1p5  = var_sd_0/std::pow(var_wcand_deltaR, 1.5);
  var_sd_n0p5 = var_sd_0/std::pow(var_wcand_deltaR, -0.5);
  var_sd_n1   = var_sd_0/std::pow(var_wcand_deltaR, -1);
  var_sd_n1p5 = var_sd_0/std::pow(var_wcand_deltaR, -1.5);
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

// void TopCandTree::AddGenTopInfo() {
//   gen_top_pt, topDecay->top->pt() ;
//   gen_w_pt,   topDecay->W->pt() ;
//   gen_b_diag, topDecay->b->diag ;
//   gen_w_diag, topDecay->W_decay->diag ;
//   gen_top_resolved, true ;
// }

bool TopCandTree::IsGenTopMatched(const TopCand* topcand) {
  // Place holder for future development
  return false;
}

int TopCandTree::IsGenTopMatchedSloppy(const TopCand* topcand) {
  // Sloppy version that check if the 3 jet combination match to gen quarks that decay from a top
  //
  // also doesn't care about if jet not matching to any gen part (return -1)

  if (!genps_id) throw std::logic_error("The genps branches hasn't get setup!! Cannot perform gen-matching!");


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
      if (abs(genps_motheridx->at(q)) == itop || abs(genps_motheridx->at(genps_motheridx->at(q))) == itop)
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
    if (genqidx_fromtop.size() == 0)
      return true + !b_matched;  // return 2 if is a permutation of the top cand
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

  // jets sorted by CSV
  std::vector<size_t> jetidx( jets_p4->size() );
  std::iota(jetidx.begin(), jetidx.end(), 0);
  std::sort(jetidx.begin(), jetidx.end(), [&](size_t i, size_t j) { return jets_csv->at(i) > jets_csv->at(j); });

  // ------------------------------
  // form all possible candidates
  vector<TopCand> allCands;
  for (size_t ib = 0; ib < max_ntopcand; ++ib) {
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

  // ------------------------------
  // treat signal and bkg differently
  if (!randGen) {
    vector<const TopCand*> fakeCands;
    for (const auto &topcand : allCands){
      if (IsGenTopMatchedSloppy(&topcand) == 1) {
        AddTopCandInfo(&topcand, true);
        tree->Fill();
      } else {
        fakeCands.push_back(&topcand);
      }
    }
    std::random_shuffle(fakeCands.begin(), fakeCands.end());
    for (size_t i = 0; i < 2 && i < fakeCands.size(); ++i) {
      AddTopCandInfo(fakeCands.at(i), false);
      tree->Fill();
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

void TopCandTree::Reset() {

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
  npv = -1;
  nvetolep = -1;
  nvetotau = -1;
  nvetotrk = -1;
  njets = -1;
  nbjets = -1;
  nlbjets = -1;
  flag_signal = -1;
  met = -1;
  ht = -1;

  gen_top_pt = -1;
  gen_w_pt = -1;
  gen_b_diag = -1;
  gen_w_diag = -1;
  gen_top_resolved = -1;

  var_b_pt = -1;
  var_b_mass = -1;
  var_b_csv = -1;
  var_b_dcsv = -1;
  var_b_dcsvb = -1;
  var_b_dcsvc = -1;
  var_b_dcsvl = -1;
  var_b_dcsvbb = -1;
  var_b_cvsb = -1;
  var_b_qgl = -1;
  var_b_ptD = -1;
  var_b_axis1 = -1;
  var_b_axis2 = -1;
  var_b_mult = -1;
  var_j2_pt = -1;
  var_j2_csv = -1;
  var_j2_dcsv = -1;
  var_j2_dcsvb = -1;
  var_j2_dcsvc = -1;
  var_j2_dcsvl = -1;
  var_j2_dcsvbb = -1;
  var_j2_cvsl = -1;
  var_j2_cvsb = -1;
  var_j2_qgl = -1;
  var_j2_ptD = -1;
  var_j2_axis1 = -1;
  var_j2_axis2 = -1;
  var_j2_mult = -1;
  var_j3_pt = -1;
  var_j3_csv = -1;
  var_j3_dcsv = -1;
  var_j3_dcsvb = -1;
  var_j3_dcsvc = -1;
  var_j3_dcsvl = -1;
  var_j3_dcsvbb = -1;
  var_j3_cvsl = -1;
  var_j3_cvsb = -1;
  var_j3_qgl = -1;
  var_j3_ptD = -1;
  var_j3_axis1 = -1;
  var_j3_axis2 = -1;
  var_j3_mult = -1;

  var_topcand_mass = -1;
  var_topcand_pt = -1;
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
  tree->Branch("nvetolep", &nvetolep);
  tree->Branch("nvetotau", &nvetotau);
  tree->Branch("nvetotrk", &nvetotrk);
  tree->Branch("njets", &njets);
  tree->Branch("nbjets", &nbjets);
  tree->Branch("nlbjets", &nlbjets);
  tree->Branch("flag_signal", &flag_signal);
  tree->Branch("met", &met);
  tree->Branch("ht", &ht);

  tree->Branch("gen_top_pt", &gen_top_pt);
  tree->Branch("gen_w_pt", &gen_w_pt);
  tree->Branch("gen_b_diag", &gen_b_diag);
  tree->Branch("gen_w_diag", &gen_w_diag);
  tree->Branch("gen_top_resolved", &gen_top_resolved);

  tree->Branch("var_b_pt", &var_b_pt);
  tree->Branch("var_b_mass", &var_b_mass);
  tree->Branch("var_b_csv", &var_b_csv);
  tree->Branch("var_b_dcsv", &var_b_dcsv);
  tree->Branch("var_b_dcsvb", &var_b_dcsvb);
  tree->Branch("var_b_dcsvc", &var_b_dcsvc);
  tree->Branch("var_b_dcsvl", &var_b_dcsvl);
  tree->Branch("var_b_dcsvbb", &var_b_dcsvbb);
  tree->Branch("var_b_cvsb", &var_b_cvsb);
  tree->Branch("var_b_qgl", &var_b_qgl);
  tree->Branch("var_b_ptD", &var_b_ptD);
  tree->Branch("var_b_axis1", &var_b_axis1);
  tree->Branch("var_b_axis2", &var_b_axis2);
  tree->Branch("var_b_mult", &var_b_mult);
  tree->Branch("var_j2_pt", &var_j2_pt);
  tree->Branch("var_j2_csv", &var_j2_csv);
  tree->Branch("var_j2_dcsv", &var_j2_dcsv);
  tree->Branch("var_j2_dcsvb", &var_j2_dcsvb);
  tree->Branch("var_j2_dcsvc", &var_j2_dcsvc);
  tree->Branch("var_j2_dcsvl", &var_j2_dcsvl);
  tree->Branch("var_j2_dcsvbb", &var_j2_dcsvbb);
  tree->Branch("var_j2_cvsl", &var_j2_cvsl);
  tree->Branch("var_j2_cvsb", &var_j2_cvsb);
  tree->Branch("var_j2_qgl", &var_j2_qgl);
  tree->Branch("var_j2_ptD", &var_j2_ptD);
  tree->Branch("var_j2_axis1", &var_j2_axis1);
  tree->Branch("var_j2_axis2", &var_j2_axis2);
  tree->Branch("var_j2_mult", &var_j2_mult);
  tree->Branch("var_j3_pt", &var_j3_pt);
  tree->Branch("var_j3_csv", &var_j3_csv);
  tree->Branch("var_j3_dcsv", &var_j3_dcsv);
  tree->Branch("var_j3_dcsvb", &var_j3_dcsvb);
  tree->Branch("var_j3_dcsvc", &var_j3_dcsvc);
  tree->Branch("var_j3_dcsvl", &var_j3_dcsvl);
  tree->Branch("var_j3_dcsvbb", &var_j3_dcsvbb);
  tree->Branch("var_j3_cvsl", &var_j3_cvsl);
  tree->Branch("var_j3_cvsb", &var_j3_cvsb);
  tree->Branch("var_j3_qgl", &var_j3_qgl);
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
  tree->Branch("var_sd_0p5", &var_sd_0p5);
  tree->Branch("var_sd_1", &var_sd_1);
  tree->Branch("var_sd_1p5", &var_sd_1p5);
  tree->Branch("var_sd_n0p5", &var_sd_n0p5);
  tree->Branch("var_sd_n1", &var_sd_n1);
  tree->Branch("var_sd_n1p5", &var_sd_n1p5);
  tree->Branch("var_sd_n2", &var_sd_n2);
  tree->Branch("var_b_j2_mass", &var_b_j2_mass);
  tree->Branch("var_b_j3_mass", &var_b_j3_mass);
  tree->Branch("var_top_radius", &var_top_radius);
  tree->Branch("var_n_extra_jets", &var_n_extra_jets);

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

void TopCandTree::SetGenParticleVectors(const vector<LorentzVector>* p4, const vector<int>* id, const vector<bool>* isLastCopy,
                                        const vector<int>* motherid, const vector<int>* motheridx)
{
  genps_p4 = p4;
  genps_id = id;
  genps_isLastCopy = isLastCopy;
  genps_motherid = motherid;
  genps_motheridx = motheridx;
}

void TopCandTree::Write()
{
  outfile->cd();
  tree->Write();
  outfile->Close();
}
