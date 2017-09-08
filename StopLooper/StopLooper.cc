// C++
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <cmath>
#include <sstream>
#include <stdexcept>

// ROOT
#include "TDirectory.h"
#include "TTreeCache.h"
#include "Math/VectorUtil.h"
#include "TVector2.h"
#include "TBenchmark.h"
#include "TMath.h"
#include "TLorentzVector.h"
#include "TF1.h"

// CORE
#include "../CORE/Tools/utils.h"
#include "../CORE/Tools/goodrun.h"
#include "../CORE/Tools/dorky/dorky.h"
#include "../CORE/Tools/badEventFilter.h"

// Stop CORE
#include "../StopCORE/stop_1l_babyAnalyzer.h"
#include "../StopCORE/sampleInfo.h"
#include "../StopCORE/genClassyInfo.h"
#include "../StopCORE/categoryInfo.h"
#include "../StopCORE/selectionInfo.h"
#include "../StopCORE/sysInfo.h"

#include "SR.h"
#include "StopRegions.h"
#include "StopLooper.h"
#include "PlotUtilities.h"

using namespace std;
using namespace stop_1l;
using namespace selectionInfo;

class SR;
extern stop_1l_babyAnalyzer babyAnalyzer;

const bool verbose = true;
// turn on to apply weights to central value
const bool applyWeights = false;
// turn on to apply btag sf to central value
const bool applyBtagSF = true; // default true
// turn on to apply lepton sf to central value - take from babies
const bool applyLeptonSFfromBabies = false;
// turn on to apply lepton sf to central value - reread from files
const bool applyLeptonSFfromFiles = true; // default true
// turn on to apply lepton sf to central value also for 0L SR. values chosen by options above
const bool applyLeptonSFtoSR = false;
// turn on to apply reweighting to ttbar based on top pt
const bool applyTopPtReweight = false;
// add weights to correct for photon trigger efficiencies
const bool applyPhotonTriggerWeights = false; //not needed since we apply trigger safe H/E cut
// add weights to correct for dilepton trigger efficiencies
const bool applyDileptonTriggerWeights = true;
// use 2016 ICHEP ISR weights based on nisrMatch, signal and ttbar only
const bool applyISRWeights = true;
// turn on to enable plots of MT2 with systematic variations applied. will only do variations for applied weights
const bool doSystVariationPlots = true;
// turn on to apply Nvtx reweighting to MC / data2016
const bool doNvtxReweight = true;
// turn on to apply nTrueInt reweighting to MC
const bool doNTrueIntReweight = true;
// turn on to apply json file to data
const bool applyjson = true;
// ignore scale1fb to run over test samples
const bool ignoreScale1fb = false;
// to test synchronization with the standard Analysis
const bool synchronizing = false;
// not running the standard regions to speed up
const bool fasterRuntime = true;


void StopLooper::SetSignalRegions() {

  // SRVec = getStopSignalRegions();
  CR0bVec = getStopControlRegionsNoBTags();
  CR2lVec = getStopControlRegionsDilepton();
  CRemuVec = getStopControlRegionsEMu();

  SRVec = getStopSignalRegionsTopological();
  // CR0bVec = getStopControlRegionsNoBTagsTopological();

  if (verbose) {
    cout << "SRVec.size = " << SRVec.size() << ", including the following:" << endl;
    for (auto it = SRVec.begin(); it != SRVec.end(); ++it) {
      cout << it-SRVec.begin() << "  " << it->GetName() << ":  " << it->GetDetailName() << endl;
    }
  }

  auto createRangesHists = [&] (vector<SR>& srvec) {
    for (auto& sr : srvec) {
      vector<string> vars = sr.GetListOfVariables();
      TDirectory * dir = (TDirectory*) outfile_->Get((sr.GetName() + "/ranges").c_str());
      if (dir == 0) dir = outfile_->mkdir((sr.GetName() + "/ranges").c_str());
      dir->cd("ranges");
      for (auto& var : vars) {
        plot1D("h_"+var+"_"+"LOW",  1, sr.GetLowerBound(var), sr.histMap, "", 1, 0, 2);
        plot1D("h_"+var+"_"+"HI",   1, sr.GetUpperBound(var), sr.histMap, "", 1, 0, 2);
      }
      if (sr.GetNMETBins() > 0) {
        plot1D("h_metbins", -1, 0, sr.histMap, ";E^{miss}_{T} [GeV]", sr.GetNMETBins(), sr.GetMETBinsPtr());
      }
    }
  };

  createRangesHists(SRVec);
  createRangesHists(CR0bVec);
  createRangesHists(CR2lVec);
  createRangesHists(CRemuVec);

}


void StopLooper::GenerateAllSRptrSets() {
  allSRptrSets.clear();

  vector<SR*> all_SRptrs;
  // all_SRptrs.reserve(SRVec.size() + CRVec.size());
  for (auto& sr : SRVec)   all_SRptrs.push_back(&sr);
  for (auto& cr : CR2lVec) all_SRptrs.push_back(&cr);
  for (auto& cr : CR0bVec) all_SRptrs.push_back(&cr);

  // cout << __FILE__ << __LINE__ << ": all_SRptrs.size(): " << all_SRptrs.size() << endl;
  // for (auto a : all_SRptrs) cout << __FILE__ << ':' << __LINE__ << ": a= " << a << endl;
  // allSRptrSets = generateSRptrSet(all_SRptrs);
}

void StopLooper::looper(TChain* chain, string sample, string output_dir) {

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

  // gROOT->cd();
  TString output_name = Form("%s/%s.root",output_dir.c_str(),sample.c_str());
  cout << "[StopLooper::looper] creating output file: " << output_name << endl;  outfile_ = new TFile(output_name.Data(),"RECREATE") ;

  outfile_ = new TFile(output_name.Data(), "RECREATE") ;

  // // full 2016 dataset json, 35.87/fb:
  // const char* json_file = "../StopCORE/inputs/json_files/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON_snt.txt";
  // // 2017 dataset json, 10.09/fb (10.07 until 300780)
  // const char* json_file = "../StopCORE/inputs/json_files/Cert_294927-301141_13TeV_PromptReco_Collisions17_JSON_snt.txt";
  // // 2017 dataset json, 8.32/fb
  // const char* json_file = "../StopCORE/inputs/json_files/Cert_294927-300575_13TeV_PromptReco_Collisions17_JSON_snt.txt";

  // Combined 2016 and 2017 json,
  // const char* json_file = "../StopCORE/inputs/json_files/Cert_271036-301141_13TeV_Combined1617_JSON_snt.txt";
  const char* json_file = "../StopCORE/inputs/json_files/Cert_271036-300575_13TeV_Combined1617_JSON_snt.txt";
  const float kLumi = 35.87;

  // Setup pileup re-weighting
  if (doNvtxReweight) {
    TFile f_purw("/home/users/sicheng/working/StopAnalysis/AnalyzeScripts/pu_reweighting_hists/nvtx_reweighting_alldata.root");
    TH1F* h_nvtxscale = (TH1F*) f_purw.Get("h_nvtxscale_16to17");
    if (!h_nvtxscale) throw invalid_argument("???");
    for (int i = 1; i < 70; ++i) {
      nvtxscale[i] = h_nvtxscale->GetBinContent(i);
      cout << i << "  " << nvtxscale[i] << endl;
    }
  }

  if (applyjson) {
    cout << "Loading json file: " << json_file << endl;
    set_goodrun_file(json_file);
  }

  TFile dummy( (output_dir+"/dummy.root").c_str(), "RECREATE" );
  SetSignalRegions();
  // GenerateAllSRptrSets();

  // Setup event weight calculation
  sampleInfo::sampleUtil samp_info( sampleInfo::ID::k_single_mu ); // test
  sysInfo::evtWgtInfo wgtInfo;
  wgtInfo.setUp( samp_info.id );

  int nDuplicates = 0;
  int nEvents = chain->GetEntries();
  unsigned int nEventsChain = nEvents;
  cout << "[StopLooper::looper] running on " << nEventsChain << " events" << endl;
  unsigned int nEventsTotal = 0;
  unsigned int nPassedTotal = 0;

  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;
  while ( (currentFile = (TFile*)fileIter.Next()) ) {

    TFile file( currentFile->GetTitle(), "READ" );
    TTree *tree = (TTree*) file.Get("t");
    TTreeCache::SetLearnEntries(10);
    tree->SetCacheSize(128*1024*1024);
    babyAnalyzer.Init(tree);

    bool is2016data = TString(currentFile->GetTitle()).Contains("Run2016");
    dummy.cd();
    // Loop over Events in current file
    if (nEventsTotal >= nEventsChain) continue;
    unsigned int nEventsTree = tree->GetEntriesFast();
    for (unsigned int event = 0; event < nEventsTree; ++event) {
      // Read Tree
      if (nEventsTotal >= nEventsChain) continue;
      tree->LoadTree(event);
      babyAnalyzer.GetEntry(event);
      ++nEventsTotal;

      if ( is_data() ) {
        if ( applyjson && !goodrun(run(), ls()) ) continue;
	duplicate_removal::DorkyEventIdentifier id(run(), evt(), ls());
	if ( is_duplicate(id) ) {
          ++nDuplicates;
          continue;
        }
      }

      // Apply met filters
      // if (!filt_globalTightHalo2016()) continue; // problematic for 2017
      if ( !filt_globalsupertighthalo2016() ) continue;
      if ( !filt_hbhenoise() ) continue;
      if ( !filt_hbheisonoise() )   continue;
      if ( !filt_ecaltp() ) continue;
      if ( !filt_eebadsc() ) continue;
      if ( !filt_goodvtx() ) continue;
      if ( firstGoodVtxIdx() == -1 ) continue;
      if ( !filt_badMuonFilter() ) continue;
      if ( !filt_badChargedCandidateFilter() ) continue;

      ++nPassedTotal;

      // Calculate event weight

      // float scale_vary = 1.;
      float MCscale_HF = 1.;
      // if(apply_HFweight_up) MCscale_HF = GetHFWeight(1.5);
      // if(apply_HFweight_down) MCscale_HF = GetHFWeight(0.5);

      // float MCscale_met = 1.;
      float MCscaling = weight_PU(); // just for now, will put in SFs later


      if (sample.find("WJets") != string::npos)
        evtweight_ = kLumi * scale1fb() * MCscaling * MCscale_HF;
      else if (sample.find("data") != string::npos)
        evtweight_ = 1;
      else
        evtweight_ = kLumi * scale1fb() * MCscaling;

      if (doNvtxReweight) {
        if (nvtxs() > 69) continue;
        if (is2016data)
          evtweight_ *= nvtxscale[nvtxs()];
      }

      // Fill the variables
      values_.clear();

      values_["mt"] = mt_met_lep();
      values_["met"] = pfmet();
      values_["metphi"] = pfmet_phi();
      values_["mt2w"] = MT2W();
      values_["mlb"] = Mlb_closestb();
      values_["tmod"] = topnessMod();
      values_["nlep"] = ngoodleps();
      values_["nvlep"] = nvetoleps();
      values_["njet"] = ngoodjets();
      values_["nbjet"] = ngoodbtags();
      values_["ntbtag"] = ntightbtags();
      values_["dphijmet"] = mindphi_met_j1_j2();
      values_["dphilmet"] = fabs(lep1_p4().phi() - pfmet_phi());
      values_["passvetos"] = PassTrackVeto() && PassTauVeto();

      values_["lep1pt"] = lep1_p4().pt();
      values_["lep1eta"] = lep1_p4().eta();

      values_["chi2"]    = hadronic_top_chi2();
      values_["leadbpt"] = ak4pfjets_leadbtag_p4().pt();
      values_["mlb_0b"]   = (ak4pfjets_leadbtag_p4() + lep1_p4()).M();
      values_["htratio"] = ak4_htratiom();
      values_["ht"]      = ak4_HT();

      values_["passlep1pt"] = (abs(lep1_pdgid()) == 13 && lep1_p4().pt() > 40) || (abs(lep1_pdgid()) == 11 && lep1_p4().pt() > 45);
      // drlepb_  = dR_lep_leadb();
      // mhtsig_  = getMHTSig();

      // Some stuff to do with MC
      if ( !is_data() ) {
        vector<float> jet_pt;
        vector<float> jet_eta;
        vector<float> jet_CSV;
        vector<int>   jet_flavour;
        for (unsigned iJet=0; iJet < ak4pfjets_p4().size(); iJet++) {
          jet_pt.push_back( ak4pfjets_p4().at(iJet).Pt() );
          jet_eta.push_back( ak4pfjets_p4().at(iJet).Eta() );
          jet_CSV.push_back( ak4pfjets_CSV().at(iJet) );
          jet_flavour.push_back( abs(ak4pfjets_hadron_flavor().at(iJet)) );
        }
      }

      // Filling histograms
      // fillHistosForSR();

      fillHistosForCR0b();

      values_["nlep_rl"] = (ngoodleps() == 1 && nvetoleps() >= 2 && lep2_p4().Pt() > 10)? 2 : ngoodleps();
      values_["mt_rl"] = mt_met_lep_rl();
      values_["mt2w_rl"] = MT2W_rl();
      values_["met_rl"] = pfmet_rl();
      values_["dphijmet_rl"]= mindphi_met_j1_j2_rl();
      values_["tmod_rl"] = topnessMod_rl();
      // values_["mlb_rl"] = Mlb_closestb();
      values_["mll"] = (lep1_p4() + lep2_p4()).M();
      values_["osdilep"] = lep1_pdgid() == -lep2_pdgid();

      fillHistosForCR2l();
      fillHistosForCRemu();

      // if (event > 10) break;  // for debugging purpose
    } // end of event loop

    delete tree;
    file.Close();
  } // end of file loop

  cout << "[StopLooper::looper] processed  " << nEventsTotal << " events" << endl;
  if ( nEventsChain != nEventsTotal )
    cout << "WARNING: number of events from files is not equal to total number of events" << endl;

  outfile_->cd();

  auto writeHistsToFile = [&] (vector<SR>& srvec) {
    for (auto& sr : srvec) {
      TDirectory * dir = (TDirectory*) outfile_->Get(sr.GetName().c_str());
      if (dir == 0) dir = outfile_->mkdir(sr.GetName().c_str()); // shouldn't happen
      dir->cd();
      for (auto& h : sr.histMap) {
        if (h.first.find("HI") != string::npos || h.first.find("LOW") != string::npos) continue;
        h.second->Write();
      }
    }
  };

  writeHistsToFile(SRVec);
  writeHistsToFile(CR0bVec);
  writeHistsToFile(CR2lVec);
  writeHistsToFile(CRemuVec);

  outfile_->Write();
  outfile_->Close();

  bmark->Stop("benchmark");
  cout << endl;
  cout << nEventsTotal << " Events Processed, where " << nDuplicates << " duplicates were skipped, and ";
  cout << nPassedTotal << " Events passed all selections." << endl;
  cout << "------------------------------" << endl;
  cout << "CPU  Time:	" << Form( "%.01f s", bmark->GetCpuTime("benchmark")  ) << endl;
  cout << "Real Time:	" << Form( "%.01f s", bmark->GetRealTime("benchmark") ) << endl;
  cout << endl;
  delete bmark;

  return;
}


void StopLooper::fillHistosForSR(string suf) {

  for (auto& sr : SRVec) {
    if ( sr.PassesSelection(values_) ) {
      // This plot function port from MT2 demonstrate the simpleness of adding extra plots anywhere in the code
      // and this functionality is great for quick checks
      auto fillhists = [&] (string s) {
        plot1D("h_mt"+s,       values_["mt"]      , evtweight_, sr.histMap, ";M_{T} [GeV]"          , 12,  0, 600);
        plot1D("h_mt2w"+s,     values_["mt2w"]    , evtweight_, sr.histMap, ";MT2W [GeV]"           , 18,  50, 500);
        plot1D("h_met"+s,      values_["met"]     , evtweight_, sr.histMap, ";E_{T}^{miss} [GeV]"   , 24,  50, 650);
        plot1D("h_metphi"+s,   values_["metphi"]  , evtweight_, sr.histMap, ";#phi(E_{T}^{miss})"   , 24,  50, 650);
        plot1D("h_lep1pt"+s,   values_["lep1pt"]  , evtweight_, sr.histMap, ";p_{T}(lep1) [GeV]"    , 30,  0, 300);
        plot1D("h_lep1eta"+s,  values_["lep1eta"] , evtweight_, sr.histMap, ";#eta (lep1)"          , 20, -5, 5);
        plot1D("h_nleps"+s,    values_["nlep"]    , evtweight_, sr.histMap, ";nleps"                ,  5,  0, 5);
        plot1D("h_njets"+s,    values_["njet"]    , evtweight_, sr.histMap, ";njets"                , 12,  0, 12);
        plot1D("h_nbjets"+s,   values_["nbjet"]   , evtweight_, sr.histMap, ";nbjets"               ,  6,  0, 6);
        plot1D("h_mlepb"+s,    values_["mlb"]     , evtweight_, sr.histMap, ";M(l,b) [GeV]"         , 24,  0, 600);
        plot1D("h_dphijmet"+s, values_["dphijmet"], evtweight_, sr.histMap, ";#Delta #phi (j, met)" , 24,  0, 4);
        plot1D("h_tmod"+s,     values_["tmod"]    , evtweight_, sr.histMap, ";modified topness"     , 25, -10, 15);

        const float met_bins[] = {0, 250, 350, 450, 550, 650, 800};
        plot1D("h_metbins"+s,   values_["met"]    , evtweight_, sr.histMap, ";E^{miss}_{T} [GeV]"        , 6, met_bins);
      };
      fillhists(suf);
      if ( abs(lep1_pdgid()) == 11 )
        fillhists(suf+"_e");
      else if ( abs(lep1_pdgid()) == 13 )
        fillhists(suf+"_mu");

    }
  }
}

void StopLooper::fillHistosForCRemu(string suf) {

  // Trigger requirements
  if ( !HLT_MET_MHT() ) return;
  if ( !(lep1_pdgid() * lep2_pdgid() == -143) ) return;

  // Basic cuts that are not supposed to change frequently
  if ( lep1_p4().pt() < 30 || fabs(lep1_p4().eta()) > 2.1 ||
       (abs(lep1_pdgid()) == 13 && !lep1_passTightID()) ||
       (abs(lep1_pdgid()) == 11 && !lep1_passMediumID()) ||
       lep1_MiniIso() > 0.1 ) return;

  if ( lep2_p4().pt() < 15 || fabs(lep2_p4().eta()) > 2.1 ||
       (abs(lep2_pdgid()) == 13 && !lep2_passTightID()) ||
       (abs(lep2_pdgid()) == 11 && !lep2_passMediumID()) ||
       lep2_MiniIso() > 0.1 ) return;

  if ( (lep1_p4() + lep2_p4()).M() < 20 ) return;

  values_["lep2pt"] = lep2_p4().pt();
  values_["lep2eta"] = lep2_p4().eta();

  for (auto& cr : CRemuVec) {
    if ( cr.PassesSelection(values_) ) {

      auto fillhists = [&] (string s) {
        plot1D("h_mt"+s,       values_["mt"]      , evtweight_, cr.histMap, ";M_{T} [GeV]"          , 12,  0, 600);
        plot1D("h_mt2w"+s,     values_["mt2w"]    , evtweight_, cr.histMap, ";MT2W [GeV]"           , 18,  50, 500);
        plot1D("h_met"+s,      values_["met"]     , evtweight_, cr.histMap, ";E_{T}^{miss} [GeV]"   , 24,  50, 650);
        plot1D("h_metphi"+s,   values_["metphi"]  , evtweight_, cr.histMap, ";#phi(E_{T}^{miss})"   , 24,  50, 650);
        plot1D("h_lep1pt"+s,   values_["lep1pt"]  , evtweight_, cr.histMap, ";p_{T}(lep1) [GeV]"    , 30,  0, 300);
        plot1D("h_lep2pt"+s,   values_["lep2pt"]  , evtweight_, cr.histMap, ";p_{T}(lep2) [GeV]"    , 30,  0, 300);
        plot1D("h_lep1eta"+s,  values_["lep1eta"] , evtweight_, cr.histMap, ";#eta (lep1)"          , 20, -5, 5);
        plot1D("h_lep2eta"+s,  values_["lep2eta"] , evtweight_, cr.histMap, ";#eta (lep2)"          , 20, -5, 5);
        plot1D("h_nleps"+s,    values_["nlep"]    , evtweight_, cr.histMap, ";nleps"                ,  5,  0, 5);
        plot1D("h_njets"+s,    values_["njet"]    , evtweight_, cr.histMap, ";njets"                , 12,  0, 12);
        plot1D("h_nbjets"+s,   values_["nbjet"]   , evtweight_, cr.histMap, ";nbjets"               , 6,   0, 6);
        plot1D("h_mlepb"+s,    values_["mlb_0b"]  , evtweight_, cr.histMap, ";M(l,b) [GeV]"         , 24,  0, 600);
        plot1D("h_dphijmet"+s, values_["dphijmet"], evtweight_, cr.histMap, ";#Delta #phi (j, met)" , 24,  0, 4);
        plot1D("h_mll"+s,      values_["mll"]     , evtweight_, cr.histMap, ";M(ll) [GeV]"          , 100, 0, 500 );

        // Temporary
        if (60 < values_["mll"] && values_["mll"] < 120) {
          plot1D("h_njets_sp"+s,  values_["njet"] , evtweight_, cr.histMap, ";njets"                , 12,  0, 12);
          plot1D("h_nbjets_sp"+s, values_["nbjet"], evtweight_, cr.histMap, ";njets"                , 12,  0, 12);
        }

        const float leppt_bins[] = {0, 30, 40, 50, 75, 100, 125, 200};
        plot1D("h_lep1ptbins"+s, values_["lep1pt"], evtweight_, cr.histMap, ";p_{T}(lep1) [GeV]"  , 7, leppt_bins);
        plot1D("h_lep2ptbins"+s, values_["lep2pt"], evtweight_, cr.histMap, ";p_{T}(lep2) [GeV]"  , 7, leppt_bins);
      };
      fillhists(suf);
      if (HLT_MuE())
        fillhists(suf+"_passHLT");
    }
  }
}

void StopLooper::fillHistosForCR2l(string suf) {

  // Trigger requirements
  if ( !HLT_SingleEl() && !HLT_SingleMu() ) return;

  // if ( (abs(lep1_pdgid()) == 11 && values_["lep1pt"] < 40) || (abs(lep1_pdgid()) == 13 && values_["lep1pt"] < 30) ) return;
  // if (lep1_pdgid() != -lep2_pdgid()) return; // temporary for zpeak check

  for (auto& cr : CR2lVec) {
    if ( cr.PassesSelection(values_) ) {

      auto fillhists = [&] (string s) {
        plot1D("h_finemet"+s,  values_["met"]         , evtweight_, cr.histMap, ";E_{T}^{miss} [GeV]"   , 80,  0, 800);
        plot1D("h_met"+s,      values_["met"]         , evtweight_, cr.histMap, ";E_{T}^{miss} [GeV]"   , 24,  50, 650);
        plot1D("h_metphi"+s,   values_["metphi"]      , evtweight_, cr.histMap, ";#phi(E_{T}^{miss})"   , 40,  -4, 4);
        plot1D("h_rlmet"+s,    values_["met_rl"]      , evtweight_, cr.histMap, ";(E^{miss}+lep2)_{T} [GeV]" , 24,  50, 650);
        plot1D("h_mt"+s,       values_["mt"]          , evtweight_, cr.histMap, ";M_{T} [GeV]"          , 12,  0, 600);
        plot1D("h_rlmt"+s,     values_["mt_rl"]       , evtweight_, cr.histMap, ";M_{T} (rl) [GeV]"     , 12,  0, 600);
        plot1D("h_mt2w"+s,     values_["mt2w_rl"]     , evtweight_, cr.histMap, ";MT2W"                 , 18,  50, 500);
        plot1D("h_tmod"+s,     values_["tmod_rl"]     , evtweight_, cr.histMap, ";modified topness"     , 20, -10, 15);
        plot1D("h_njets"+s,    values_["njet"]        , evtweight_, cr.histMap, ";njets"                , 12,  0, 12);
        plot1D("h_nbjets"+s,   values_["nbjet"]       , evtweight_, cr.histMap, ";nbjets"               ,  6,  0, 6);
        plot1D("h_nleps"+s,    values_["nlep_rl"]     , evtweight_, cr.histMap, ";nleps (dilep)"        ,  5,  0, 5);
        plot1D("h_lep1pt"+s,   values_["lep1pt"]      , evtweight_, cr.histMap, ";p_{T}(lep1) [GeV]"    , 20,  0, 200);
        plot1D("h_lep1eta"+s,  values_["lep1eta"]     , evtweight_, cr.histMap, ";#eta (lep1)"          , 20, -5, 5);
        plot1D("h_mlepb"+s,    values_["mlb"]         , evtweight_, cr.histMap, ";M(l,b) [GeV]"         , 24,  0, 600);
        plot1D("h_dphijmet"+s, values_["dphijmet_rl"] , evtweight_, cr.histMap, ";#Delta #phi (j, met)" , 24,  0, 4);
        plot1D("h_nvtxs"+s,          nvtxs()          , evtweight_, cr.histMap, ";N Vertexes"           , 70,  1, 71);

        const float met_bins[] = {0, 250, 350, 450, 550, 650, 800};
        plot1D("h_metbins"+s,   values_["met"]    , evtweight_, cr.histMap, ";E^{miss}_{T} [GeV]"        , 6, met_bins);
        plot1D("h_rlmetbins"+s, values_["met_rl"] , evtweight_, cr.histMap, ";(E^{miss}+lep2)_{T} [GeV]" , 6, met_bins);

        // Luminosity test at Z peak
        if (lep1_pdgid() == -lep2_pdgid()) {
          plot1D("h_mll"+s,   values_["mll"], evtweight_, cr.histMap, ";M(ll) [GeV]", 120, 0, 240 );
          if (82 < values_["mll"] && values_["mll"] < 100) {
            plot1D("h_zpt"+s, (lep1_p4() + lep2_p4()).pt(), evtweight_, cr.histMap, ";M(ll) [GeV]", 200, 0, 200 );
            plot1D("h_njets_zpeak"+s,  values_["njet"]    , evtweight_, cr.histMap, ";njets"                , 12,  0, 12);
            plot1D("h_nbjets_zpeak"+s, values_["nbjet"]   , evtweight_, cr.histMap, ";nbjets"               ,  6,  0, 6);
          } else {
            plot1D("h_njets_noz"+s,    values_["njet"]    , evtweight_, cr.histMap, ";njets"                , 12,  0, 12);
            plot1D("h_nbjets_noz"+s,   values_["nbjet"]   , evtweight_, cr.histMap, ";nbjets"               ,  6,  0, 6);
          }
        }
      };
      fillhists(suf);

      // if ( HLT_SingleMu() ) fillhists(suf+"_hltmu");
      // if ( HLT_SingleEl() ) fillhists(suf+"_hltel");
      // if ( HLT_MET() )      fillhists(suf+"_hltmet");

      if ( abs(lep1_pdgid()*lep2_pdgid()) == 121 )
        fillhists(suf+"_ee");
      else if ( abs(lep1_pdgid()*lep2_pdgid()) == 143 )
        fillhists(suf+"_emu");
      else if ( abs(lep1_pdgid()*lep2_pdgid()) == 169 )
        fillhists(suf+"_mumu");

    }
  }
}

void StopLooper::fillHistosForCR0b(string suf) {

  if ( !(HLT_SingleEl() || HLT_SingleMu() || HLT_MET_MHT()) ) return;

  for (auto& cr : CR0bVec) {
    if ( cr.PassesSelection(values_) ) {
      // if ( cr.GetLowerBound("met") < 200 && !(HLT_SingleEl() || HLT_SingleMu()) ) continue;
      auto fillhists = [&] (string s) {
        plot1D("h_mt"+s,       values_["mt"]      , evtweight_, cr.histMap, ";M_{T} [GeV]"          , 12,  0, 600);
        plot1D("h_mt2w"+s,     values_["mt2w"]    , evtweight_, cr.histMap, ";MT2W [GeV]"           , 18,  50, 500);
        plot1D("h_met"+s,      values_["met"]     , evtweight_, cr.histMap, ";E_{T}^{miss} [GeV]"   , 24,  50, 650);
        plot1D("h_metphi"+s,   values_["metphi"]  , evtweight_, cr.histMap, ";#phi(E_{T}^{miss})"   , 24,  50, 650);
        plot1D("h_lep1pt"+s,   values_["lep1pt"]  , evtweight_, cr.histMap, ";p_{T}(lep1) [GeV]"    , 20,  0, 200);
        plot1D("h_lep1eta"+s,  values_["lep1eta"] , evtweight_, cr.histMap, ";#eta (lep1)"          , 20, -5, 5);
        plot1D("h_nleps"+s,    values_["nlep"]    , evtweight_, cr.histMap, ";nleps"                ,  5,  0, 5);
        plot1D("h_njets"+s,    values_["njet"]    , evtweight_, cr.histMap, ";njets"                , 12,  0, 12);
        plot1D("h_nbjets"+s,   values_["nbjet"]   , evtweight_, cr.histMap, ";nbjets"               ,  6,  0, 6);
        plot1D("h_mlepb"+s,    values_["mlb_0b"]  , evtweight_, cr.histMap, ";M(l,b) [GeV]"         , 24,  0, 600);
        plot1D("h_dphijmet"+s, values_["dphijmet"], evtweight_, cr.histMap, ";#Delta #phi (j, met)" , 24,  0, 4);
        plot1D("h_tmod"+s,     values_["tmod"]    , evtweight_, cr.histMap, ";modified topness"     , 25, -10, 15);
        plot1D("h_nvtxs"+s,        nvtxs()        , evtweight_, cr.histMap, ";N Vertexes"           , 70,  1, 71);

        const float met_bins[] = {0, 250, 350, 450, 550, 650, 800};
        plot1D("h_metbins"+s,   values_["met"]    , evtweight_, cr.histMap, ";E^{miss}_{T} [GeV]"   , 6, met_bins);
      };
      fillhists(suf);
      if ( abs(lep1_pdgid()) == 11 ) {
        fillhists(suf+"_e");
        // if ( fabs(lep1_p4().eta()) < 1.4 )
        //   fillhists(suf+"_barrele");
        // else
        //   fillhists(suf+"_endcape");
      }
      else if ( abs(lep1_pdgid()) == 13 ) {
        fillhists(suf+"_mu");
      }
      if (HLT_SingleMu() || HLT_SingleEl())
        fillhists(suf+"_hltsl");
      if (HLT_MET())
        fillhists(suf+"_hltmet");
      if (HLT_MET_MHT())
        fillhists(suf+"_hltmht");

    }
  }
}
