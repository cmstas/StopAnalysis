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
#include "TString.h"
#include "TLorentzVector.h"
#include "TF1.h"

// CORE
#include "../CORE/Tools/utils.h"
#include "../CORE/Tools/goodrun.h"
#include "../CORE/Tools/dorky/dorky.h"
#include "../CORE/Tools/badEventFilter.h"

// Stop baby class
#include "../StopCORE/stop_1l_babyAnalyzer.h"
// #include "../StopCORE/sampleInfo.h"
// #include "../StopCORE/genClassyInfo.h"
// #include "../StopCORE/categoryInfo.h"
// #include "../StopCORE/selectionInfo.h"
// #include "../StopCORE/sysInfo.h"

#include "SR.h"
#include "StopRegions.h"
#include "StopLooper.h"
#include "Utilities.h"

using namespace std;
using namespace stop_1l;
// using namespace selectionInfo;

class SR;

const bool verbose = false;
// turn on to apply weights to central value
const bool applyWeights = false;
// turn on to apply btag sf - take from files defined in eventWeight_btagSF.cc
const bool applyBtagSFfromFiles = false; // default false
// turn on to apply lepton sf to central value - reread from files
const bool applyLeptonSFfromFiles = false; // default false
// turn on to enable plots of metbins with systematic variations applied. will only do variations for applied weights
const bool doSystVariations = true;
// turn on to enable plots of metbins with different gen classifications
const bool doGenClassification = true;
// turn on to apply Nvtx reweighting to MC / data2016
const bool doNvtxReweight = false;
// turn on to apply nTrueInt reweighting to MC
const bool doNTrueIntReweight = true;
// turn on top tagging studies, off for 2016 data/mc
const bool doTopTagging = true;
// turn on to apply json file to data
const bool applyjson = true;
// ignore scale1fb to run over test samples
const bool ignoreScale1fb = false;
// not running the standard regions to speed up
const bool runYieldsOnly = false;
// debug symbol, for printing exact event kinematics that passes
const bool printPassedEvents = false;

// set bool def here for member function usage
bool is2016data = false;
bool printOnce1 = false;

const float fInf = std::numeric_limits<float>::max();

std::ofstream ofile;

void StopLooper::SetSignalRegions() {

  // SRVec = getStopSignalRegions();
  // CR0bVec = getStopControlRegionsNoBTags();
  // CR2lVec = getStopControlRegionsDilepton();
  CRemuVec = getStopCrosscheckRegionsEMu();

  SRVec = getStopSignalRegionsTopological();
  CR0bVec = getStopControlRegionsNoBTagsTopological();
  CR2lVec = getStopControlRegionsDileptonTopological();

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
        plot1D("h_metbins", -1, 0, sr.histMap, sr.GetName()+"_"+sr.GetDetailName()+";E^{miss}_{T} [GeV]", sr.GetNMETBins(), sr.GetMETBinsPtr());
      }
    }
  };

  createRangesHists(SRVec);
  createRangesHists(CR0bVec);
  createRangesHists(CR2lVec);
  createRangesHists(CRemuVec);

  testVec.emplace_back("testTopTagging");
  testVec.emplace_back("testCutflow");
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

void StopLooper::looper(TChain* chain, string samplestr, string output_dir, int jes_type) {

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

  TString output_name = Form("%s/%s.root",output_dir.c_str(),samplestr.c_str());
  cout << "[StopLooper::looper] creating output file: " << output_name << endl;  outfile_ = new TFile(output_name.Data(),"RECREATE") ;

  if (printPassedEvents) ofile.open("passEventList.txt");

  outfile_ = new TFile(output_name.Data(), "RECREATE") ;

  // // full 2016 dataset json, 35.87/fb:
  // const char* json_file = "../StopCORE/inputs/json_files/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON_snt.txt";
  // // 2017 dataset json, 10.09/fb (10.07 until 300780)
  // const char* json_file = "../StopCORE/inputs/json_files/Cert_294927-301141_13TeV_PromptReco_Collisions17_JSON_snt.txt";
  // // 2017 dataset json, 8.32/fb
  // const char* json_file = "../StopCORE/inputs/json_files/Cert_294927-300575_13TeV_PromptReco_Collisions17_JSON_snt.txt";

  // Full 2017 dataset json, 41.96/fb
  // const char* json_file = "../StopBabyMaker/json_files/Cert_294927-306462_13TeV_PromptReco_Collisions17_JSON_snt.txt";
  const float kLumi = 120;

  // Combined 2016 and 2017 json,
  // const char* json_file = "../StopCORE/inputs/json_files/Cert_271036-301141_13TeV_Combined1617_JSON_snt.txt";
  // const char* json_file = "../StopCORE/inputs/json_files/Cert_271036-300575_13TeV_Combined1617_JSON_snt.txt";
  const char* json_file = "../StopCORE/inputs/json_files/Cert_271036-306462_13TeV_Combined1617_JSON_snt.txt";

  // Setup pileup re-weighting
  if (doNvtxReweight) {
    TFile f_purw("/home/users/sicheng/working/StopAnalysis/AnalyzeScripts/pu_reweighting_hists/nvtx_reweighting_alldata.root");
    TString scaletype = "17to16";
    TH1F* h_nvtxscale = (TH1F*) f_purw.Get("h_nvtxscale_"+scaletype);
    if (!h_nvtxscale) throw invalid_argument("???");
    if (verbose) cout << "Doing nvtx reweighting! Scaling " << scaletype << ". The scale factors are:" << endl;
    for (int i = 1; i < 85; ++i) {
      nvtxscale_[i] = h_nvtxscale->GetBinContent(i);
      if (verbose) cout << i << "  " << nvtxscale_[i] << endl;
    }
  }

  if (applyjson) {
    cout << "Loading json file: " << json_file << endl;
    set_goodrun_file(json_file);
  }

  TFile dummy( (output_dir+"/dummy.root").c_str(), "RECREATE" );
  SetSignalRegions();
  // GenerateAllSRptrSets();

  // Setup the event weight calculator

  evtWgt.Setup(samplestr, applyBtagSFfromFiles, applyLeptonSFfromFiles);
  evtWgt.setDefaultSystematics(0);

  if (samplestr.find("data_2016") || samplestr == "data_single_lepton_met") is2016data = true;

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

    TString fname = currentFile->GetTitle();
    TFile file( fname, "READ" );
    TTree *tree = (TTree*) file.Get("t");
    TTreeCache::SetLearnEntries(10);
    tree->SetCacheSize(128*1024*1024);
    babyAnalyzer.Init(tree);

    is_signal_ = fname.Contains("SMS") || fname.Contains("Signal");

    // Get event weight histogram from baby
    TH3D* h_sig_counter = nullptr;
    TH2D* h_sig_counter_nEvents = nullptr;
    if ( is_signal_ ) {
      h_sig_counter = (TH3D*) file.Get("h_counterSMS");
      h_sig_counter_nEvents = (TH2D*) file.Get("histNEvts");
    }
    evtWgt.getCounterHistogramFromBaby(&file);
    // Extra file weight for extension dataset, should move these code to other places
    if (fname.Contains("ttbar_diLept_madgraph_pythia8_ext1"))
      evtWgt.setExtraFileWeight(23198554.0 / (23198554.0+5689986.0));
    else if (fname.Contains("ttbar_diLept_madgraph_pythia8"))
      evtWgt.setExtraFileWeight(5689986.0 / (23198554.0+5689986.0));
    else if (fname.Contains("t_tW_5f_powheg_pythia8_noHadDecays_ext1"))
      evtWgt.setExtraFileWeight((3145334.0)/(4473156.0+3145334.0));
    else if (fname.Contains("t_tW_5f_powheg_pythia8_noHadDecays"))
      evtWgt.setExtraFileWeight((4473156.0)/(4473156.0+3145334.0));
    else if (fname.Contains("t_tbarW_5f_powheg_pythia8_noHadDecays_ext1"))
      evtWgt.setExtraFileWeight((3146940.0)/(5029568.0+3146940.0));
    else if (fname.Contains("t_tbarW_5f_powheg_pythia8_noHadDecays"))
      evtWgt.setExtraFileWeight((5029568.0)/(5029568.0+3146940.0));

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
      if (is_data()) {
        if (is2016data) {
          if ( !filt_met() ) continue;  // contains globalTightHalo2016 and other things in the else clause
          if ( filt_duplicatemuons() ) continue;
          if ( filt_badmuons() ) continue;
          if ( !filt_nobadmuons() ) continue;
        } else {
          if ( !filt_globalsupertighthalo2016() ) continue;
          if ( !filt_hbhenoise() ) continue;
          if ( !filt_hbheisonoise() )   continue;
          if ( !filt_ecaltp() ) continue;
          if ( !filt_eebadsc() ) continue;
        }
        if ( !filt_goodvtx() ) continue;
        if ( firstGoodVtxIdx() == -1 ) continue;
        if ( !filt_badMuonFilter() ) continue;
        if ( !filt_badChargedCandidateFilter() ) continue;
        if ( !filt_jetWithBadMuon() ) continue;
        if ( !filt_pfovercalomet() ) continue;
      }

      // Require at least 1 good vertex
      if (nvtxs() < 1) continue;

      // For testing on only subset of mass points
      TString dsname = dataset();
      if (dsname.Contains("T2tt")) {
        auto checkMassPt = [&](double mstop, double mlsp) { return (mass_stop() == mstop) && (mass_lsp() == mlsp); };
        if (!checkMassPt(600, 400) && !checkMassPt(650, 400) && !checkMassPt(800, 400) && !checkMassPt(800, 600) && !checkMassPt(800, 500) &&
            !checkMassPt(700, 400) && !checkMassPt(700, 450) && !checkMassPt(700, 500) && !checkMassPt(900, 700) && !checkMassPt(900, 600))
          continue;
        // float massdiff = mass_stop() - mass_lsp();
        // if (massdiff < 200 || massdiff > 300 || mass_lsp() < 400 || mass_stop() > 900) continue;
        // plot2D("h_T2tt_masspts", mass_stop(), mass_lsp() , evtweight_, testVec[1].histMap, ";M(stop) [GeV]; M(lsp) [GeV]", 100, 300, 1300, 80, 0, 800);
      }

      // Only events nupt < 200 for WNJetsToLNu samples
      if (dsname.Contains("JetsToLNu") && !dsname.Contains("NuPt-200") && nupt() > 200) continue;

      ++nPassedTotal;

      // Calculate event weight
      // wgtInfo.getEventWeights(); // what does this do?    // <-- breaks on signal samples
      /// weights need re-calculation for every event, but only do if event get selected
      is_bkg_ = (!is_data() && !is_signal_);
      evtWgt.resetWeights();

      int nEventsSample;
      if (!is_data()) {
        if (is_signal_) {
          nEventsSample = h_sig_counter_nEvents->GetBinContent(h_sig_counter->FindBin(mass_stop(), mass_lsp()));
          evtweight_ = kLumi * xsec() * 1000 / nEventsSample;
        } else {
          evtweight_ = kLumi * scale1fb();
        }
      }

      if (doNvtxReweight && is_data()) {
        if (nvtxs() < 85 && !is2016data)
          evtweight_ = nvtxscale_[nvtxs()];  // only scale for data
      }

      // // Plot nvtxs on the base selection of stopbaby for reweighting purpose
      // plot1D("h_nvtxs", nvtxs(), evtweight_, testVec[1].histMap, ";Number of vertices" , 100,  1, 101);

      // Temporary test for top tagging efficiency
      testTopTaggingEffficiency(testVec[0]);

      // nbtag for CSV valued btags -- for comparison between the 2016 analysis
      int nbtagsCSV = 0;
      for (float csv : ak4pfjets_CSV())
        if (csv > 0.8484) nbtagsCSV++;

      // Fill the variables
      values_.clear();

      /// Common variables for all JES type
      values_["nlep"] = ngoodleps();
      values_["nvlep"] = nvetoleps();
      values_["lep1pt"] = lep1_p4().pt();
      values_["passvetos"] = PassTrackVeto() && PassTauVeto();

      /// Values only for hist filling or testing
      values_["chi2"] = hadronic_top_chi2();
      values_["lep1eta"] = lep1_p4().eta();
      values_["passlep1pt"] = (abs(lep1_pdgid()) == 13 && lep1_p4().pt() > 40) || (abs(lep1_pdgid()) == 11 && lep1_p4().pt() > 45);

      for (int jestype = 0; jestype < ((doSystVariations && !is_data())? 3 : 1); ++jestype) {
        if (doSystVariations) jestype_ = jestype;
        string suffix = "";

        /// JES type dependent variables
        if (jestype_ == 0) {
          values_["mt"] = mt_met_lep();
          values_["met"] = pfmet();
          values_["mt2w"] = MT2W();
          values_["mlb"] = Mlb_closestb();
          values_["mlb_0b"] = (lep1_p4() + ak4pfjets_leadbtag_p4()).M();
          values_["tmod"] = topnessMod();
          values_["njet"] = ngoodjets();
          values_["nbjet"] = ngoodbtags();
          values_["nbtag"]  = nanalysisbtags();
          values_["dphijmet"] = mindphi_met_j1_j2();
          values_["dphilmet"] = lep1_dphiMET();
          values_["j1passbtag"] = (ngoodjets() > 0)? ak4pfjets_passMEDbtag().at(0) : 0;

          values_["ht"] = ak4_HT();
          values_["metphi"] = pfmet_phi();
          values_["ntbtag"] = ntightbtags();
          values_["leadbpt"] = ak4pfjets_leadbtag_p4().pt();
          values_["mlb_0b"] = (ak4pfjets_leadbtag_p4() + lep1_p4()).M();
          // values_["htratio"] = ak4_htratiom();

          // suffix = "_nominal";
        } else if (jestype_ == 1) {
          values_["mt"] = mt_met_lep_jup();
          values_["met"] = pfmet_jup();
          values_["mt2w"] = MT2W_jup();
          values_["mlb"] = Mlb_closestb_jup();
          values_["mlb_0b"] = (lep1_p4() + jup_ak4pfjets_leadbtag_p4()).M();
          values_["tmod"] = topnessMod_jup();
          values_["njet"] = jup_ngoodjets();
          values_["nbjet"] = jup_ngoodbtags();  // nbtag30();
          values_["nbtag"]  = jup_nanalysisbtags();
          values_["dphijmet"] = mindphi_met_j1_j2_jup();
          values_["dphilmet"] = fabs(lep1_p4().phi() - pfmet_phi_jup());
          values_["j1passbtag"] = (jup_ngoodjets() > 0)? jup_ak4pfjets_passMEDbtag().at(0) : 0;

          values_["ht"] = jup_ak4_HT();
          values_["metphi"] = pfmet_phi_jup();
          values_["ntbtag"] = jup_ntightbtags();
          values_["leadbpt"] = jup_ak4pfjets_leadbtag_p4().pt();
          values_["mlb_0b"] = (jup_ak4pfjets_leadbtag_p4() + lep1_p4()).M();
          // values_["htratio"] = jup_ak4_htratiom();

          suffix = "_jesUp";
        } else if (jestype_ == 2) {
          values_["mt"] = mt_met_lep_jdown();
          values_["met"] = pfmet_jdown();
          values_["mt2w"] = MT2W_jdown();
          values_["mlb"] = Mlb_closestb_jdown();
          values_["mlb_0b"] = (lep1_p4() + jdown_ak4pfjets_leadbtag_p4()).M();
          values_["tmod"] = topnessMod_jdown();
          values_["njet"] = jdown_ngoodjets();
          values_["nbjet"] = jdown_ngoodbtags();  // nbtag30();
          values_["ntbtag"] = jdown_ntightbtags();
          values_["dphijmet"] = mindphi_met_j1_j2_jdown();
          values_["dphilmet"] = fabs(lep1_p4().phi() - pfmet_phi_jdown());
          values_["j1passbtag"] = (jdown_ngoodjets() > 0)? jdown_ak4pfjets_passMEDbtag().at(0) : 0;

          values_["ht"] = jdown_ak4_HT();
          values_["metphi"] = pfmet_phi_jdown();
          values_["nbtag"]  = jdown_nanalysisbtags();
          values_["leadbpt"] = jdown_ak4pfjets_leadbtag_p4().pt();
          values_["mlb_0b"] = (jdown_ak4pfjets_leadbtag_p4() + lep1_p4()).M();
          // values_["htratio"] = jdown_ak4_htratiom();

          suffix = "_jesDn";
        }

        // Filling histograms for SR
        fillHistosForSR(suffix);

        // testCutFlowHistos(testVec[1]);

        // Temporary variable for CR0b before better solution to express this
        values_["is0b"] = (values_["nbjet"] == 0 || (values_["nbjet"] >= 1 && values_["ntbtag"] == 0 && values_["mlb"] > 175.0));
        fillHistosForCR0b(suffix);

        values_["nlep_rl"] = (ngoodleps() == 1 && nvetoleps() >= 2 && lep2_p4().Pt() > 10)? 2 : ngoodleps();
        values_["osdilep"] = lep1_pdgid() == -lep2_pdgid();
        values_["mll"] = (lep1_p4() + lep2_p4()).M();
        // values_["mlb_rl"] = Mlb_closestb();

        if (jestype_ == 0) {
          values_["mt_rl"] = mt_met_lep_rl();
          values_["mt2w_rl"] = MT2W_rl();
          values_["met_rl"] = pfmet_rl();
          values_["dphijmet_rl"]= mindphi_met_j1_j2_rl();
          values_["dphilmet_rl"] = lep1_dphiMET_rl();
          values_["tmod_rl"] = topnessMod_rl();
        } else if (jestype_ == 1) {
          values_["mt_rl"] = mt_met_lep_rl_jup();
          values_["mt2w_rl"] = MT2W_rl_jup();
          values_["met_rl"] = pfmet_rl_jup();
          values_["dphijmet_rl"]= mindphi_met_j1_j2_rl_jup();
          values_["dphilmet_rl"] = lep1_dphiMET_rl_jup();
          values_["tmod_rl"] = topnessMod_rl_jup();
        } else if (jestype_ == 2) {
          values_["mt_rl"] = mt_met_lep_rl_jdown();
          values_["mt2w_rl"] = MT2W_rl_jdown();
          values_["met_rl"] = pfmet_rl_jdown();
          values_["dphijmet_rl"]= mindphi_met_j1_j2_rl_jdown();
          values_["dphilmet_rl"] = lep1_dphiMET_rl_jdown();
          values_["tmod_rl"] = topnessMod_rl_jdown();
        }
        fillHistosForCR2l(suffix);
        fillHistosForCRemu(suffix);
      }

      // if (event > 10) break;  // for debugging purpose
    } // end of event loop


    delete tree;
    file.Close();
  } // end of file loop

  cout << "[StopLooper::looper] processed  " << nEventsTotal << " events" << endl;
  if ( nEventsChain != nEventsTotal )
    cout << "WARNING: Number of events from files is not equal to total number of events" << endl;

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

  writeHistsToFile(testVec);
  writeHistsToFile(SRVec);
  writeHistsToFile(CR0bVec);
  writeHistsToFile(CR2lVec);
  writeHistsToFile(CRemuVec);

  auto writeRatioHists = [&] (const SR& sr) {
    for (const auto& h : sr.histMap) {
      if (h.first.find("hnom") != 0) continue;
      string hname = h.first;
      hname.erase(0, 4);
      dummy.cd();
      TH1F* h_ratio = (TH1F*) h.second->Clone(("ratio"+hname).c_str());
      h_ratio->Divide(h_ratio, sr.histMap.at("hden"+hname), 1, 1, "B");

      TDirectory * dir = (TDirectory*) outfile_->Get(sr.GetName().c_str());
      if (dir == 0) dir = outfile_->mkdir(sr.GetName().c_str()); // shouldn't happen
      dir->cd();
      h_ratio->Write();
    }
  };
  writeRatioHists(testVec[0]);

  outfile_->Write();
  outfile_->Close();
  if (printPassedEvents) ofile.close();

  bmark->Stop("benchmark");
  cout << endl;
  cout << nEventsTotal << " Events Processed, where " << nDuplicates << " duplicates were skipped, and ";
  cout << nPassedTotal << " Events passed all selections." << endl;
  cout << "------------------------------" << endl;
  cout << "CPU  Time:   " << Form( "%.01f s", bmark->GetCpuTime("benchmark")  ) << endl;
  cout << "Real Time:   " << Form( "%.01f s", bmark->GetRealTime("benchmark") ) << endl;
  cout << endl;
  delete bmark;

  return;
}

void StopLooper::fillYieldHistos(SR& sr, float met, string suf, bool is_cr2l) {

  evtweight_ = evtWgt.getWeight(evtWgtInfo::systID(jestype_), is_cr2l);

  auto fillhists = [&](string s) {
    plot1D("h_metbins"+s+suf, met, evtweight_, sr.histMap, ";E^{miss}_{T} [GeV]" , sr.GetNMETBins(), sr.GetMETBinsPtr());
    if (is_signal_) {
      string masspt_hn = "h_metbins_" + to_string((int) mass_stop()) + "_" + to_string((int) mass_lsp()) + s + suf;
      plot1D(masspt_hn, met, evtweight_, sr.histMap, ";E^{miss}_{T} [GeV]" , sr.GetNMETBins(), sr.GetMETBinsPtr());
    }
    if (doSystVariations && is_bkg_ && jestype_ == 0) {
      // Only run once when JES type == 0. JES variation dealt with above. No need for signals?
      for (int isyst = 3; isyst < evtWgtInfo::k_nSyst; ++isyst) {
        auto syst = (evtWgtInfo::systID) isyst;
        if (evtWgt.doingSystematic(syst))
          plot1D("h_metbins"+s+"_"+evtWgt.getLabel(syst), met, evtWgt.getWeight(syst, is_cr2l), sr.histMap, ";E^{miss}_{T} [GeV]", sr.GetNMETBins(), sr.GetMETBinsPtr());
      }
    }
  };
  fillhists("");
  if (doGenClassification && is_bkg_) {
    // Only fill gen classification for background events, used for background estimation
    // fillhists("_allclass");
    if (isZtoNuNu()) fillhists("_Znunu");
    else if (is2lep()) fillhists("_2lep");
    else if (is1lepFromW()) fillhists("_1lepW");
    else if (is1lepFromTop()) fillhists("_1lepTop");
    else fillhists("_unclass");  // either unclassified 1lep or 0lep, or something else unknown, should have (m)any
  }

  // Debugging
  if (printPassedEvents && sr.GetName() == "srC" && suf == "" && pfmet_rl() < 350) {
    // if (!printOnce1) { for (auto& v : values_) ofile << ' ' << v.first; ofile << endl; printOnce1 = true; }
    ofile << run() << ' ' << ls() << ' ' << evt() << ' ' << sr.GetName();
    // if (run() == 277168) for (auto& v : values_) ofile << ' ' << v.second;
    ofile << ' ' << evtweight_ << ' ' << evtWgt.getWeight(evtWgtInfo::systID::k_bTagEffHFUp) << ' ' << evtWgt.getLabel(evtWgtInfo::systID::k_bTagEffHFUp);
    // ofile << ' ' << evtweight_ << ' ' << pfmet() << ' ' << mt_met_lep() << ' ' << Mlb_closestb() << ' ' << topnessMod() << ' ' << ngoodjets() << ' ' << ngoodbtags() << ' ' << mindphi_met_j1_j2() << ' ' << pfmet_rl();
    ofile << endl;
  }
}

void StopLooper::fillHistosForSR(string suf) {

  // Trigger requirements
  if (doTopTagging) { // 2017 Triggers
    if (not ( (abs(lep1_pdgid()) == 11 && HLT_SingleEl()) || (abs(lep1_pdgid()) == 13 && HLT_SingleMu()) || HLT_MET_MHT() )) return;
  } else if (is_data()) { // 2016 MET Triggers
    if (not ( (abs(lep1_pdgid()) == 11 && HLT_SingleEl()) || (abs(lep1_pdgid()) == 13 && HLT_SingleMu()) ||(HLT_MET() || HLT_MET110_MHT110() || HLT_MET120_MHT120()) )) return;
  }

  for (auto& sr : SRVec) {
    if (!sr.PassesSelection(values_)) continue;
    fillYieldHistos(sr, values_["met"], suf);

    if (runYieldsOnly) continue;

    // Plot kinematics histograms
    auto fillKineHists = [&](string s) {
      // Simple plot function plot1D to add extra plots anywhere in the code, is great for quick checks
      plot1D("h_mt"+s,       values_["mt"]      , evtweight_, sr.histMap, ";M_{T} [GeV]"             , 12,  0, 600);
      plot1D("h_mt_h"+s,     values_["mt"]      , evtweight_, sr.histMap, ";M_{T} [GeV]"             , 12, 150, 650);
      plot1D("h_mt2w"+s,     values_["mt2w"]    , evtweight_, sr.histMap, ";MT2W [GeV]"              , 18,  50, 500);
      plot1D("h_met"+s,      values_["met"]     , evtweight_, sr.histMap, ";#slash{E}_{T} [GeV]"      , 24,  50, 650);
      plot1D("h_met_h"+s,    values_["met"]     , evtweight_, sr.histMap, ";#slash{E}_{T} [GeV]"      , 20, 250, 650);
      plot1D("h_metphi"+s,   values_["metphi"]  , evtweight_, sr.histMap, ";#phi(#slash{E}_{T})"      , 24,  50, 650);
      plot1D("h_lep1pt"+s,   values_["lep1pt"]  , evtweight_, sr.histMap, ";p_{T}(lepton) [GeV]"     , 30,  0, 300);
      plot1D("h_lep1eta"+s,  values_["lep1eta"] , evtweight_, sr.histMap, ";#eta(leppton)"           , 20, -5, 5);
      plot1D("h_nleps"+s,    values_["nlep"]    , evtweight_, sr.histMap, ";Number of leptons"       ,  5,  0, 5);
      plot1D("h_njets"+s,    values_["njet"]    , evtweight_, sr.histMap, ";Number of jets"          ,  8,  2, 10);
      plot1D("h_nbjets"+s,   values_["nbjet"]   , evtweight_, sr.histMap, ";Number of b-tagged jets" ,  4,  1, 5);
      plot1D("h_mlepb"+s,    values_["mlb"]     , evtweight_, sr.histMap, ";M_{#it{l}b} [GeV]"  , 24,  0, 600);
      plot1D("h_dphijmet"+s, values_["dphijmet"], evtweight_, sr.histMap, ";#Delta#phi(jet,#slash{E}_{T})" , 24,  0, 4);
      plot1D("h_tmod"+s,     values_["tmod"]    , evtweight_, sr.histMap, ";Modified topness"        , 25, -10, 15);
      if (doTopTagging && values_["njet"] >= 4) {
        if (topcands_disc().size() > 0) {
          plot1D("h_leadtopcand_disc"+s, topcands_disc()[0], evtweight_, sr.histMap, ";top discriminator", 110, -1.1, 1.1);
          plot1D("h_leadtopcand_finedisc"+s, topcands_disc()[0], evtweight_, sr.histMap, ";top discriminator", 550, -1.1, 1.1);
        } else {
          plot1D("h_leadtopcand_disc"+s, -1.1, evtweight_, sr.histMap, ";top discriminator", 110, -1.1, 1.1);
          plot1D("h_leadtopcand_finedisc"+s, -1.1, evtweight_, sr.histMap, ";top discriminator", 550, -1.1, 1.1);
        }
        float chi2_disc = -log(hadronic_top_chi2()) / 8;
        if (fabs(chi2_disc) >= 1.0) chi2_disc = std::copysign(0.99999, chi2_disc);
        plot1D("h_chi2_disc"+s, chi2_disc, evtweight_, sr.histMap, ";hadronic #chi^2 discriminator", 110, -1.1, 1.1);
        plot1D("h_chi2_finedisc"+s, chi2_disc, evtweight_, sr.histMap, ";hadronic #chi^2 discriminator", 550, -1.1, 1.1);
      }
    };
    // if (sr.GetName().find("base") != string::npos) // only plot for base regions
    fillKineHists(suf);
    if (is_signal_) fillKineHists("_"+to_string((int)mass_stop())+"_"+to_string((int)mass_lsp()) + suf);

    // Re-using fillKineHists with different suffix for extra/checking categories
    // if ( abs(lep1_pdgid()) == 11 )
    //   fillKineHists(suf+"_e");
    // else if ( abs(lep1_pdgid()) == 13 )
    //   fillKineHists(suf+"_mu");

    if (doTopTagging && topcands_disc().size() > 0 && topcands_disc()[0] > 0.9) {
      fillYieldHistos(sr, values_["met"], "_wtc"+suf);
      // fillKineHists("_wtc"+suf);
    }
  }
  // SRVec[0].PassesSelectionPrintFirstFail(values_);
}

void StopLooper::fillHistosForCR2l(string suf) {

  // Trigger requirements
  if (doTopTagging) { // 2017 Triggers
    if (not ( (abs(lep1_pdgid()) == 11 && HLT_SingleEl()) || (abs(lep1_pdgid()) == 13 && HLT_SingleMu()) || HLT_MET_MHT() )) return;
  } else if (is_data()) { // 2016 CR2l Triggers
    // if (not ( (abs(lep1_pdgid()) == 11 && HLT_SingleEl()) || (abs(lep1_pdgid()) == 13 && HLT_SingleMu()) || (HLT_MET() || HLT_MET110_MHT110() || HLT_MET120_MHT120()) )) return;
    if (not ( ( HLT_SingleEl() && (abs(lep1_pdgid())==11 || abs(lep2_pdgid())==11) ) || ( HLT_SingleMu() && (abs(lep1_pdgid())==13 || abs(lep2_pdgid())==13) ) ||
              ( HLT_MET()) || ( HLT_MET110_MHT110()) || ( HLT_MET120_MHT120()) )) return;
  }

  // // For getting into full trigger efficiency in 2017 data
  // if ( (abs(lep1_pdgid()) == 11 && values_["lep1pt"] < 40) || (abs(lep1_pdgid()) == 13 && values_["lep1pt"] < 30) ) return;

  for (auto& cr : CR2lVec) {
    if (!cr.PassesSelection(values_)) continue;
    fillYieldHistos(cr, values_["met_rl"], suf, true);

    if (runYieldsOnly) continue;

    auto fillKineHists = [&] (string s) {
      plot1D("h_finemet"+s,  values_["met"]         , evtweight_, cr.histMap, ";#slash{E}_{T} [GeV]"           , 80,  0, 800);
      plot1D("h_met"+s,      values_["met"]         , evtweight_, cr.histMap, ";#slash{E}_{T} [GeV]"           , 24,  50, 650);
      plot1D("h_met_u"+s,    values_["met"]         , evtweight_, cr.histMap, ";#slash{E}_{T} [GeV]"           , 20, 250, 650);
      plot1D("h_metphi"+s,   values_["metphi"]      , evtweight_, cr.histMap, ";#phi(#slash{E}_{T})"           , 40,  -4, 4);
      plot1D("h_mt"+s,       values_["mt"]          , evtweight_, cr.histMap, ";M_{T} [GeV]"                   , 12,  0, 600);
      plot1D("h_mt_u"+s,     values_["mt"]          , evtweight_, cr.histMap, ";M_{T} [GeV]"                   , 12, 150, 650);
      plot1D("h_rlmet"+s,    values_["met_rl"]      , evtweight_, cr.histMap, ";#slash{E}_{T} (with removed lepton) [GeV]" , 24,  50, 650);
      plot1D("h_rlmt"+s,     values_["mt_rl"]       , evtweight_, cr.histMap, ";M_{T} (with removed lepton) [GeV]" , 12,  0, 600);
      plot1D("h_rlmet_u"+s,  values_["met_rl"]      , evtweight_, cr.histMap, ";#slash{E}_{T} (with removed lepton) [GeV]" , 20, 250, 650);
      plot1D("h_rlmt_u"+s,   values_["mt_rl"]       , evtweight_, cr.histMap, ";M_{T} (with removed lepton) [GeV]" , 12, 150, 600);
      plot1D("h_mt2w"+s,     values_["mt2w_rl"]     , evtweight_, cr.histMap, ";MT2W"                          , 18,  50, 500);
      plot1D("h_tmod"+s,     values_["tmod_rl"]     , evtweight_, cr.histMap, ";Modified topness"              , 20, -10, 15);
      plot1D("h_njets"+s,    values_["njet"]        , evtweight_, cr.histMap, ";Number of jets"                ,  8,  2, 10);
      plot1D("h_nbjets"+s,   values_["nbjet"]       , evtweight_, cr.histMap, ";Number of b-tagged jets"       ,  4,  1, 5);
      plot1D("h_nleps"+s,    values_["nlep_rl"]     , evtweight_, cr.histMap, ";nleps (dilep)"                 ,  5,  0, 5);
      plot1D("h_lep1pt"+s,   values_["lep1pt"]      , evtweight_, cr.histMap, ";p_{T}(lepton) [GeV]"           , 20,  0, 200);
      plot1D("h_lep1eta"+s,  values_["lep1eta"]     , evtweight_, cr.histMap, ";#eta(leppton)"                 , 20, -5, 5);
      plot1D("h_mlepb"+s,    values_["mlb"]         , evtweight_, cr.histMap, ";M_{#it{l}b} [GeV]"             , 24,  0, 600);
      plot1D("h_dphijmet"+s, values_["dphijmet_rl"] , evtweight_, cr.histMap, ";#Delta#phi(jet,#slash{E}_{T})" , 24,  0, 4);
      plot1D("h_nvtxs"+s,          nvtxs()          , evtweight_, cr.histMap, ";Number of vertices"            , 70,  1, 71);

      // Luminosity test at Z peak
      if (lep1_pdgid() == -lep2_pdgid()) {
        plot1D("h_mll"+s,   values_["mll"], evtweight_, cr.histMap, ";M_{#it{ll}} [GeV]" , 120, 0, 240 );
        if (82 < values_["mll"] && values_["mll"] < 100) {
          plot1D("h_zpt"+s, (lep1_p4() + lep2_p4()).pt(), evtweight_, cr.histMap, ";p_{T}(Z) [GeV]"          , 200, 0, 200);
          plot1D("h_njets_zpeak"+s,  values_["njet"]    , evtweight_, cr.histMap, ";Number of jets"          , 12,  0, 12);
          plot1D("h_nbjets_zpeak"+s, values_["nbjet"]   , evtweight_, cr.histMap, ";Number of b-tagged jets" ,  6,  0, 6);
        } else {
          plot1D("h_njets_noz"+s,    values_["njet"]    , evtweight_, cr.histMap, ";Number of jets"          , 12,  0, 12);
          plot1D("h_nbjets_noz"+s,   values_["nbjet"]   , evtweight_, cr.histMap, ";Number of b-tagged jets" ,  6,  0, 6);
        }
      }
    };
    fillKineHists(suf);

    // if ( HLT_SingleMu() ) fillKineHists(suf+"_hltmu");
    // if ( HLT_SingleEl() ) fillKineHists(suf+"_hltel");
    // if ( HLT_MET() )      fillKineHists(suf+"_hltmet");

    // if ( abs(lep1_pdgid()*lep2_pdgid()) == 121 )
    //   fillKineHists(suf+"_ee");
    // else if ( abs(lep1_pdgid()*lep2_pdgid()) == 143 )
    //   fillKineHists(suf+"_emu");
    // else if ( abs(lep1_pdgid()*lep2_pdgid()) == 169 )
    //   fillKineHists(suf+"_mumu");

  }
}

void StopLooper::fillHistosForCR0b(string suf) {

  // Trigger requirements
  if (doTopTagging) { // 2017 Triggers
    if (not ( (abs(lep1_pdgid()) == 11 && HLT_SingleEl()) || (abs(lep1_pdgid()) == 13 && HLT_SingleMu()) || HLT_MET_MHT() )) return;
  } else if (is_data()) { // 2016 MET Triggers
    if (not ( (abs(lep1_pdgid()) == 11 && HLT_SingleEl()) || (abs(lep1_pdgid()) == 13 && HLT_SingleMu()) ||(HLT_MET() || HLT_MET110_MHT110() || HLT_MET120_MHT120()) )) return;
  }

  for (auto& cr : CR0bVec) {
    if (!cr.PassesSelection(values_)) continue;
    fillYieldHistos(cr, values_["met"], suf);

    if (runYieldsOnly) continue;

    auto fillKineHists = [&] (string s) {
      const float met_bins[] = {0, 250, 350, 450, 550, 650, 800};
      plot1D("h_metbins"+s,   values_["met"]    , evtweight_, cr.histMap, ";E^{miss}_{T} [GeV]"   , 6, met_bins);
      if (runYieldsOnly) return;

      plot1D("h_mt"+s,       values_["mt"]      , evtweight_, cr.histMap, ";M_{T} [GeV]"          , 12,  0, 600);
      plot1D("h_mt_u"+s,     values_["mt"]      , evtweight_, cr.histMap, ";M_{T} [GeV]"          , 12, 150, 650);
      plot1D("h_mt2w"+s,     values_["mt2w"]    , evtweight_, cr.histMap, ";MT2W [GeV]"           , 18,  50, 500);
      plot1D("h_met"+s,      values_["met"]     , evtweight_, cr.histMap, ";#slash{E}_{T} [GeV]"  , 24,  50, 650);
      plot1D("h_met_u"+s,    values_["met"]     , evtweight_, cr.histMap, ";#slash{E}_{T} [GeV]"  , 20, 250, 650);
      plot1D("h_metphi"+s,   values_["metphi"]  , evtweight_, cr.histMap, ";#phi(#slash{E}_{T})"  , 24,  50, 650);
      plot1D("h_lep1pt"+s,   values_["lep1pt"]  , evtweight_, cr.histMap, ";p_{T}(lepton) [GeV]"  , 20,  0, 200);
      plot1D("h_lep1eta"+s,  values_["lep1eta"] , evtweight_, cr.histMap, ";#eta(leppton)"        , 20, -5, 5);
      plot1D("h_nleps"+s,    values_["nlep"]    , evtweight_, cr.histMap, ";Number of leptons"    ,  5,  0, 5);
      plot1D("h_njets"+s,    values_["njet"]    , evtweight_, cr.histMap, ";Number of jets"       ,  8,  2, 10);
      plot1D("h_nbjets"+s,   values_["nbjet"]   , evtweight_, cr.histMap, ";Number of b-tagged jets" ,  6,  0, 6);
      plot1D("h_mlepb"+s,    values_["mlb_0b"]  , evtweight_, cr.histMap, ";M_{#it{l}b} [GeV]"  , 24,  0, 600);
      plot1D("h_dphijmet"+s, values_["dphijmet"], evtweight_, cr.histMap, ";#Delta#phi(jet,#slash{E}_{T})" , 24,  0, 4);
      plot1D("h_tmod"+s,     values_["tmod"]    , evtweight_, cr.histMap, ";Modified topness"     , 25, -10, 15);
      plot1D("h_nvtxs"+s,        nvtxs()        , evtweight_, cr.histMap, ";Number of vertices"   , 70,  1, 71);
    };
    fillKineHists(suf);
    // if ( abs(lep1_pdgid()) == 11 ) {
    //   fillKineHists(suf+"_e");
    // }
    // else if ( abs(lep1_pdgid()) == 13 ) {
    //   fillKineHists(suf+"_mu");
    // }
    // if (HLT_SingleMu() || HLT_SingleEl())
    //   fillKineHists(suf+"_hltsl");
    // if (HLT_MET_MHT())
    //   fillKineHists(suf+"_hltmht");
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
        plot1D("h_mt_h"+s,     values_["mt"]      , evtweight_, cr.histMap, ";M_{T} [GeV]"          , 12, 150, 650);
        plot1D("h_mt2w"+s,     values_["mt2w"]    , evtweight_, cr.histMap, ";MT2W [GeV]"           , 18,  50, 500);
        plot1D("h_met"+s,      values_["met"]     , evtweight_, cr.histMap, ";#slash{E}_{T} [GeV]"   , 24,  50, 650);
        plot1D("h_met_h"+s,    values_["met"]     , evtweight_, cr.histMap, ";#slash{E}_{T} [GeV]"   , 20, 250, 650);
        plot1D("h_metphi"+s,   values_["metphi"]  , evtweight_, cr.histMap, ";#phi(#slash{E}_{T})"   , 24,  50, 650);
        plot1D("h_lep1pt"+s,   values_["lep1pt"]  , evtweight_, cr.histMap, ";p_{T}(lepton) [GeV]"  , 30,  0, 300);
        plot1D("h_lep2pt"+s,   values_["lep2pt"]  , evtweight_, cr.histMap, ";p_{T}(lep2) [GeV]"    , 30,  0, 300);
        plot1D("h_lep1eta"+s,  values_["lep1eta"] , evtweight_, cr.histMap, ";#eta(leppton)"        , 20, -5, 5);
        plot1D("h_lep2eta"+s,  values_["lep2eta"] , evtweight_, cr.histMap, ";#eta (lep2)"          , 20, -5, 5);
        plot1D("h_nleps"+s,    values_["nlep"]    , evtweight_, cr.histMap, ";Number of leptons"    ,  5,  0, 5);
        plot1D("h_njets"+s,    values_["njet"]    , evtweight_, cr.histMap, ";Number of jets"       ,  8,  2, 10);
        plot1D("h_nbjets"+s,   values_["nbjet"]   , evtweight_, cr.histMap, ";nbtags"               , 6,   0, 6);
        plot1D("h_tmod"+s,     values_["tmod"]    , evtweight_, cr.histMap, ";Modified topness"     , 30, -15, 15);
        plot1D("h_mlepb"+s,    values_["mlb_0b"]  , evtweight_, cr.histMap, ";M_{#it{l}b} [GeV]" , 24,  0, 600);
        plot1D("h_dphijmet"+s, values_["dphijmet"], evtweight_, cr.histMap, ";#Delta#phi(jet,#slash{E}_{T})" , 24,  0, 4);

        const float leppt_bins[] = {0, 30, 40, 50, 75, 100, 125, 200};
        plot1D("h_lep1ptbins"+s, values_["lep1pt"], evtweight_, cr.histMap, ";p_{T}(lepton) [GeV]", 7, leppt_bins);
        plot1D("h_lep2ptbins"+s, values_["lep2pt"], evtweight_, cr.histMap, ";p_{T}(lep2) [GeV]"  , 7, leppt_bins);
      };
      fillhists(suf);
      if (HLT_MuE())
        fillhists(suf+"_passHLT");
    }
  }
}

void StopLooper::testTopTaggingEffficiency(SR& sr) {
  // Function to test the top tagging efficiencies and miss-tag rate
  // The current tagger only works for hadronically decay tops
  if (!doTopTagging) return;
  // Temporary for consistency accross samples
  if (pfmet() < 100 && pfmet_rl() < 100) return;

  // First need to determine how many gen tops does hadronic decay
  int nHadDecayTops = 2 - gen_nfromtleps_();  // 2 gentops for sure <-- checked

  int ntopcands = topcands_disc().size();
  // Divide the events into tt->1l and tt->2l

  int calculable = (ngoodbtags() > 0 && ngoodjets() > 2);
  calculable += (ngoodjets() > 5);
  int n4jets = (ngoodbtags() > 0 && ngoodjets() > 3);

  if (nHadDecayTops == 1) {
    plot1D("h_calculable", calculable, evtweight_, sr.histMap, ";N(events)", 4, 0, 4);
    plot1D("h_n4jets", n4jets, evtweight_, sr.histMap, ";N(4j events)", 4, 0, 4);
    if (n4jets == 0) return;

    // Hadronic chi2 for comparison
    /// Define a disc variable that look similar
    float chi2_disc = -log(hadronic_top_chi2()) / 8;
    if (fabs(chi2_disc) >= 1.0) chi2_disc = std::copysign(0.99999, chi2_disc);
    plot1D("h_chi2_disc1", chi2_disc, evtweight_, sr.histMap, ";discriminator", 220, -1.1, 1.1);
    chi2_disc = std::copysign(pow(fabs(chi2_disc), 0.1), chi2_disc);
    plot1D("h_chi2_disc2", chi2_disc, evtweight_, sr.histMap, ";discriminator", 220, -1.1, 1.1);

    float ratio_ntcandvscalable;
    if (calculable != 0) ratio_ntcandvscalable = (float) topcands_disc().size() / calculable;
    else ratio_ntcandvscalable = (ntopcands == 0)? -1/12 : 2.5;
    plot1D("h_ratio_ntcandvscalable", ratio_ntcandvscalable, evtweight_, sr.histMap, ";N(topcand)", 7, -0.5, 3);

    plot1D("h_ntopcand_raw", ntopcands, evtweight_, sr.histMap, ";N(topcand)", 4, 0, 4);
    if (ntopcands > 0) {
      plot1D("h_lead_topcand_disc", topcands_disc().at(0), evtweight_, sr.histMap, ";lead topcand discriminator", 110, -1.1, 1.1);
      plot1D("h_lead_topcand_finedisc", topcands_disc().at(0), evtweight_, sr.histMap, ";lead topcand discriminator", 1100, -1.1, 1.1);
    } else {
      plot1D("h_lead_topcand_disc", -1.1, evtweight_, sr.histMap, ";lead topcand discriminator", 110, -1.1, 1.1);
      plot1D("h_lead_topcand_finedisc", -1.1, evtweight_, sr.histMap, ";lead topcand discriminator", 1100, -1.1, 1.1);
    }
    int ntopcand0 = 0;
    int ntopcandp5 = 0;
    int ntopcandp9 = 0;
    int ntopcandp98 = 0;
    for (float disc : topcands_disc()) {
      plot1D("h_topcand_disc", disc, evtweight_, sr.histMap, ";discriminator", 110, -1.1, 1.1);
      plot1D("h_topcand_finedisc", disc, evtweight_, sr.histMap, ";discriminator", 1100, -1.1, 1.1);
      if (disc > 0) ntopcand0++;
      if (disc > 0.5) ntopcandp5++;
      if (disc > 0.9) ntopcandp9++;
      if (disc > 0.98) ntopcandp98++;
    }
    plot1D("h_ntopcandp9", ntopcandp9, evtweight_, sr.histMap, ";N(topcand)", 4, 0, 4);
    plot2D("h2d_ntopcand0", ntopcands, ntopcand0, evtweight_, sr.histMap, ";N(all topcand);N(disc > 0)", 4, 0, 4, 4, 0, 4);
    plot2D("h2d_ntopcandp5", ntopcands, ntopcandp5, evtweight_, sr.histMap, ";N(all topcand);N(disc > 0.5)", 4, 0, 4, 4, 0, 4);
    plot2D("h2d_ntopcandp9", ntopcands, ntopcandp9, evtweight_, sr.histMap, ";N(all topcand);N(disc > 0.9)", 4, 0, 4, 4, 0, 4);
    plot2D("h2d_ntopcandp98", ntopcands, ntopcandp98, evtweight_, sr.histMap, ";N(all topcand);N(disc > 0.98)", 4, 0, 4, 4, 0, 4);

    // Find the daughters of the hadronically decayed top
    /// First find the leptonic decayed top
    int leptonictopidx = -1;
    for (size_t l = 0; l < genleps_id().size(); ++l) {
      if (genleps_isfromt().at(l)) {
        leptonictopidx = genleps_gmotheridx().at(l);
        break;
      }
    }
    vector<int> jets_fromhadtop;
    int bjetidx = -1;
    for (size_t q = 0; q < genqs_id().size(); ++q) {
      if (genqs_status().at(q) != 23 && genqs_status().at(q) != 1) continue;
      if (genqs_isfromt().at(q) && genqs_motheridx().at(q) != leptonictopidx) {
        int jetidx = -1;
        float minDR = 0.4;
        for (size_t j = 0; j < ak4pfjets_p4().size(); ++j) {
          float dr = ROOT::Math::VectorUtil::DeltaR(ak4pfjets_p4().at(j), genqs_p4().at(q));
          if (dr < minDR) {
            minDR = dr;
            jetidx = j;
          }
        }
        if (minDR < 0.4) {
          jets_fromhadtop.push_back(jetidx);
          if (abs(genqs_id().at(q)) == 5) bjetidx = jetidx;
        }
      }
    }

    if (jets_fromhadtop.size() < 3) {
      // Check all jets from top decay are within the list
      if (ntopcandp98 < 1)
        plot1D("h_category", 0, evtweight_, sr.histMap, ";category for toptagging", 8, 0, 8);
      else
        plot1D("h_category", 1, evtweight_, sr.histMap, ";category for toptagging", 8, 0, 8);
    } else if (ntopcands < 1) {
      plot1D("h_category", 7, evtweight_, sr.histMap, ";category for toptagging", 8, 0, 8);
    } else {
      if (bjetidx >= 0) {
        plot1D("h_bfromt_csv", ak4pfjets_CSV().at(bjetidx), evtweight_, sr.histMap, ";CSVv2 (gen b)", 100, 0, 1);
        plot1D("h_bfromt_deepcsv", ak4pfjets_deepCSV().at(bjetidx), evtweight_, sr.histMap, ";CSVv2 (gen b)", 100, 0, 1);
      }
      plot1D("h_allaccepted_disc", topcands_disc().at(0), evtweight_, sr.histMap, ";lead topcand discriminator", 110, -1.1, 1.1);

      vector<int> jidxs = topcands_ak4idx().at(0);
      bool isRealTop = std::is_permutation(jidxs.begin(), jidxs.end(), jets_fromhadtop.begin());
      if (ntopcandp98 < 1) {
        if (jidxs.at(0) != bjetidx)
          plot1D("h_category", 2, evtweight_, sr.histMap, ";category for toptagging", 8, 0, 8);
        else if (isRealTop)
          plot1D("h_category", 3, evtweight_, sr.histMap, ";category for toptagging", 8, 0, 8);
        else
          plot1D("h_category", 4, evtweight_, sr.histMap, ";category for toptagging", 8, 0, 8);
      } else {
        if (!isRealTop)
          plot1D("h_category", 5, evtweight_, sr.histMap, ";category for toptagging", 8, 0, 8);
        else
          plot1D("h_category", 6, evtweight_, sr.histMap, ";category for toptagging", 8, 0, 8);
      }
    }

    if (ntopcands >= 1) {
      bool isActualTopJet = true;
      float gentoppt = 0.;
      vector<int> jidxs = topcands_ak4idx().at(0);
      vector<int> midxs;
      for (int j = 0; j < 3; ++j) {
        int nMatchedGenqs = 0;
        int genqidx = -1;
        float minDR = 0.4;
        for (size_t i = 0; i < genqs_id().size(); ++i) {
          if (genqs_status().at(i) != 23 && genqs_status().at(i) != 1) continue;
          float dr = ROOT::Math::VectorUtil::DeltaR(ak4pfjets_p4().at(jidxs[j]), genqs_p4().at(i));
          if (dr > minDR) continue;
          nMatchedGenqs++;
          genqidx = i;
          minDR = dr;
        }
        if (ntopcandp98 >= 1)
          plot1D("h_nMatchedGenqs", nMatchedGenqs, evtweight_, sr.histMap, ";nMatchedGenqs", 4, 0, 4);
        if (nMatchedGenqs < 1) {
          isActualTopJet = false;
          break;
        }
        if (ntopcandp98 >= 1)
          plot1D("h_jqminDR", minDR, evtweight_, sr.histMap, ";min(#Delta R)", 41, 0, 0.41);
        if (!genqs_isfromt().at(genqidx)) {
          isActualTopJet = false;
          break;
        }
        if (abs(genqs_id().at(genqidx)) == 5 && abs(genqs_motherid().at(genqidx)) == 6) {
          gentoppt = genqs_motherp4().at(genqidx).pt();
          if (leptonictopidx > 0 && leptonictopidx != genqs_motheridx().at(genqidx)) {
            midxs.push_back(genqidx);
          } else {
            isActualTopJet = false;
            break;
          }
        }
        else if (abs(genqs_motherid().at(genqidx)) == 24) {
          midxs.push_back(genqidx);
        }
      }
      if (midxs.size() != 3) isActualTopJet = false;

      plot1D("hden_disc", topcands_disc().at(0), evtweight_, sr.histMap, ";discriminator", 110, -1.1, 1.1);
      if (ntopcandp98 >= 1) {
        plot1D("hden_pt", topcands_p4().at(0).pt(), evtweight_, sr.histMap, ";discriminator", 110, 0, 1100);
        plot1D("hden_gentoppt", gentoppt, evtweight_, sr.histMap, ";p_{T}(gen top)", 110, 0, 1100);
      }
      if (isActualTopJet) {
        plot1D("hnom_disc", topcands_disc().at(0), evtweight_, sr.histMap, ";topcand discriminator", 110, -1.1, 1.1);
        if (ntopcandp98 >= 1) {
          plot1D("hnom_pt", topcands_p4().at(0).pt(), evtweight_, sr.histMap, ";topcand pt", 110, 0, 1100);
          plot1D("hnom_gentoppt", gentoppt, evtweight_, sr.histMap, ";p_{T}(gen top)", 110, 0, 1100);
        }
      } else if (ntopcandp98 > 0) {

      }
    }
  }
  else if (nHadDecayTops == 0) {
    plot1D("h_fakable", calculable, evtweight_, sr.histMap, ";N(events)", 4, 0, 4);
    plot1D("h_fak4j", n4jets, evtweight_, sr.histMap, ";N(events)", 4, 0, 4);
    if (n4jets == 0) return;

    float chi2_disc = -log(hadronic_top_chi2()) / 8;
    if (fabs(chi2_disc) >= 1.0) chi2_disc = std::copysign(0.99999, chi2_disc);
    plot1D("h_chi2fake_disc1", chi2_disc, evtweight_, sr.histMap, ";discriminator", 220, -1.1, 1.1);
    chi2_disc = std::copysign(pow(fabs(chi2_disc), 0.1), chi2_disc);
    plot1D("h_chi2fake_disc2", chi2_disc, evtweight_, sr.histMap, ";discriminator", 220, -1.1, 1.1);

    plot1D("h_nfakecand_raw", ntopcands, evtweight_, sr.histMap, ";N(topcand)", 4, 0, 4);

    if (ntopcands > 0) {
      plot1D("h_lead_fakecand_disc", topcands_disc().at(0), evtweight_, sr.histMap, ";lead topcand discriminator", 110, -1.1, 1.1);
      plot1D("h_lead_fakecand_finedisc", topcands_disc().at(0), evtweight_, sr.histMap, ";lead topcand discriminator", 1100, -1.1, 1.1);
    } else {
      plot1D("h_lead_fakecand_disc", -1.1, evtweight_, sr.histMap, ";lead topcand discriminator", 110, -1.1, 1.1);
      plot1D("h_lead_fakecand_finedisc", -1.1, evtweight_, sr.histMap, ";lead topcand discriminator", 1100, -1.1, 1.1);
    }

    if (ntopcands > 0) {
      plot1D("hden_fakep5_pt", topcands_p4().at(0).pt(), evtweight_, sr.histMap, ";fakecand pt", 110, 0, 1100);
      plot1D("hden_fakep9_pt", topcands_p4().at(0).pt(), evtweight_, sr.histMap, ";fakecand pt", 110, 0, 1100);
      plot1D("hden_fakep98_pt", topcands_p4().at(0).pt(), evtweight_, sr.histMap, ";fakecand pt", 110, 0, 1100);
      for (float disc : topcands_disc()) {
        plot1D("h_fakecand_disc", disc, evtweight_, sr.histMap, ";discriminator", 110, -1.1, 1.1);
        plot1D("h_fakecand_finedisc", disc, evtweight_, sr.histMap, ";discriminator", 1100, -1.1, 1.1);
      }
      if (topcands_disc().at(0) > 0.98)
        plot1D("hnom_fakep98_pt", topcands_p4().at(0).pt(), evtweight_, sr.histMap, ";fakecand pt", 110, 0, 1100);
      if (topcands_disc().at(0) > 0.9)
        plot1D("hnom_fakep9_pt", topcands_p4().at(0).pt(), evtweight_, sr.histMap, ";fakecand pt", 110, 0, 1100);
      if (topcands_disc().at(0) > 0.5)
        plot1D("hnom_fakep5_pt", topcands_p4().at(0).pt(), evtweight_, sr.histMap, ";fakecand pt", 110, 0, 1100);
    }
  }
}

void StopLooper::testCutFlowHistos(SR& sr) {

  auto fillCFhist = [&](string&& hname, auto& cutflow, auto& cfvallow, auto& cfvalupp, auto& cfnames) {
    int ncuts = cutflow.size();
    if (sr.histMap.count(hname) == 0) {
      plot1D(hname, 0, 1, sr.histMap, "cutflow;icut", ncuts+1, 0, ncuts+1);
      sr.histMap[hname]->GetXaxis()->SetBinLabel(1, "base");
      for (int icf = 0; icf < ncuts; ++icf)
        sr.histMap[hname]->GetXaxis()->SetBinLabel(icf+2, cfnames[icf].c_str());
    } else {
      plot1D(hname, 0, 1, sr.histMap, "cutflow;icut", ncuts+1, 0, ncuts+1);
    }
    for (int icf = 0; icf < ncuts; ++icf) {
      string cut = cutflow[icf];
      if (values_[cut] >= cfvallow[icf] && values_[cut] < cfvalupp[icf]) {
        plot1D(hname, icf+1, 1, sr.histMap, "cutflow;icut", ncuts+1, 0, ncuts+1);
      } else {
        break;
      }
    }
  };

  string masspt_suf = (is_signal_)? "_" + to_string((int) mass_stop()) + "_" + to_string((int) mass_lsp()) : "";

  const vector<string> cfnames1 = {"met",  "mt",};
  const vector<string> cutflow1 = {"met",  "mt",};
  const vector<float> cfvallow1 = {  250,  100 ,};
  const vector<float> cfvalupp1 = { fInf, fInf ,};
  fillCFhist("h_cutflow1_org", cutflow1, cfvallow1, cfvalupp1, cfnames1);
  if (is_signal_) fillCFhist("h_cutflow1"+masspt_suf+"_org", cutflow1, cfvallow1, cfvalupp1, cfnames1);
  if (doTopTagging && topcands_disc().size() > 0 && topcands_disc()[0] > 0.98) {
    fillCFhist("h_cutflow1_wtc", cutflow1, cfvallow1, cfvalupp1, cfnames1);
    if (is_signal_) fillCFhist("h_cutflow1"+masspt_suf+"c", cutflow1, cfvallow1, cfvalupp1, cfnames1);
  }

  const vector<string> cfnames2 = {  "mt",  "mlb",};
  const vector<string> cutflow2 = {  "mt",  "mlb",};
  const vector<float> cfvallow2 = {  150 ,   100 ,};
  const vector<float> cfvalupp2 = { fInf ,  fInf ,};
  fillCFhist("h_cutflow2_org", cutflow2, cfvallow2, cfvalupp2, cfnames2);
  if (is_signal_) fillCFhist("h_cutflow2"+masspt_suf+"_org", cutflow2, cfvallow2, cfvalupp2, cfnames2);
  if (doTopTagging && topcands_disc().size() > 0 && topcands_disc()[0] > 0.98) {
    fillCFhist("h_cutflow2_wtc", cutflow2, cfvallow2, cfvalupp2, cfnames2);
    if (is_signal_) fillCFhist("h_cutflow2"+masspt_suf+"_wtc", cutflow2, cfvallow2, cfvalupp2, cfnames2);
  }

  const vector<string> cfnames3 = { "njet4", "njet5",};
  const vector<string> cutflow3 = {  "njet",  "njet",};
  const vector<float> cfvallow3 = {      4 ,      5 ,};
  const vector<float> cfvalupp3 = {   fInf ,   fInf ,};
  fillCFhist("h_cutflow3_org", cutflow3, cfvallow3, cfvalupp3, cfnames3);
  if (is_signal_) fillCFhist("h_cutflow3"+masspt_suf+"_org", cutflow3, cfvallow3, cfvalupp3, cfnames3);
  if (doTopTagging && topcands_disc().size() > 0 && topcands_disc()[0] > 0.98) {
    fillCFhist("h_cutflow3_wtc", cutflow3, cfvallow3, cfvalupp3, cfnames3);
    if (is_signal_) fillCFhist("h_cutflow3"+masspt_suf+"_wtc", cutflow3, cfvallow3, cfvalupp3, cfnames3);
  }

}
