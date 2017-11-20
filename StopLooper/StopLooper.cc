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
const bool doNvtxReweight = false;
// turn on to apply nTrueInt reweighting to MC
const bool doNTrueIntReweight = true;
// turn on top tagging studies, off for 2016 data/mc
const bool doTopTagging = false;
// turn on to apply json file to data
const bool applyjson = true;
// ignore scale1fb to run over test samples
const bool ignoreScale1fb = false;
// to test synchronization with the standard Analysis
const bool synchronizing = false;
// not running the standard regions to speed up
const bool fasterRuntime = true;
// set bool def here for member function usage
bool is2016data = false;

const float fInf = std::numeric_limits<float>::max();

void StopLooper::SetSignalRegions() {

  // SRVec = getStopSignalRegions();
  CR0bVec = getStopControlRegionsNoBTags();
  CR2lVec = getStopControlRegionsDilepton();
  CRemuVec = getStopCrosscheckRegionsEMu();

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

void StopLooper::looper(TChain* chain, string samplestr, string output_dir) {

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

  // gROOT->cd();
  TString output_name = Form("%s/%s.root",output_dir.c_str(),samplestr.c_str());
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
  // const float kLumi = 35.87;

  // Setup pileup re-weighting
  if (doNvtxReweight) {
    TFile f_purw("/home/users/sicheng/working/StopAnalysis/AnalyzeScripts/pu_reweighting_hists/nvtx_reweighting_alldata.root");
    TH1F* h_nvtxscale = (TH1F*) f_purw.Get("h_nvtxscale_16to17");
    if (!h_nvtxscale) throw invalid_argument("???");
    cout << "Doing nvtx reweighting! The scale factors are:" << endl;
    for (int i = 1; i < 70; ++i) {
      nvtxscale_[i] = h_nvtxscale->GetBinContent(i);
      cout << i << "  " << nvtxscale_[i] << endl;
    }
  }

  if (applyjson) {
    cout << "Loading json file: " << json_file << endl;
    set_goodrun_file(json_file);
  }

  TFile dummy( (output_dir+"/dummy.root").c_str(), "RECREATE" );
  SetSignalRegions();
  // GenerateAllSRptrSets();

  is2016data = (samplestr.find("data_2016") == 0);

  sampleInfo::sampleUtil sample(sampleInfo::ID::k_ttbar);
  // Setup event weight calculation
  sysInfo::evtWgtInfo wgtInfo;
  wgtInfo.setUp( sample.id );
  // wgtInfo.setUp( sample.id, useBTagSFs_fromFiles_, useLepSFs_fromFiles_, add2ndLepToMet_ );
  // wgtInfo.apply_cr2lTrigger_sf  = (apply_cr2lTrigger_sf_ && add2ndLepToMet_);
  // wgtInfo.apply_bTag_sf         = apply_bTag_sf_;
  // wgtInfo.apply_lep_sf          = apply_lep_sf_;
  // wgtInfo.apply_vetoLep_sf      = apply_vetoLep_sf_;
  // wgtInfo.apply_tau_sf          = apply_tau_sf_;
  // wgtInfo.apply_lepFS_sf        = apply_lepFS_sf_;
  // wgtInfo.apply_topPt_sf        = apply_topPt_sf_;
  // wgtInfo.apply_metRes_sf       = apply_metRes_sf_;
  // wgtInfo.apply_metTTbar_sf     = apply_metTTbar_sf_;
  // wgtInfo.apply_ttbarSysPt_sf   = apply_ttbarSysPt_sf_;
  // wgtInfo.apply_ISR_sf          = apply_ISR_sf_;
  // wgtInfo.apply_pu_sf           = apply_pu_sf_;
  // wgtInfo.apply_sample_sf       = apply_sample_sf_;

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

    // Get weight histogram from baby
    wgtInfo.getWeightHistogramFromBaby(&file);

    is_signal_ = fname.Contains("SMS") || fname.Contains("Signal");

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

      if( sample.id == sampleInfo::k_W1JetsToLNu_madgraph_pythia8 ||
          sample.id == sampleInfo::k_W2JetsToLNu_madgraph_pythia8 ||
          sample.id == sampleInfo::k_W3JetsToLNu_madgraph_pythia8 ||
          sample.id == sampleInfo::k_W4JetsToLNu_madgraph_pythia8    ) {
        if( nupt()>200.0 ) continue;
      }

      // Apply met filters
      // if (!filt_globalTightHalo2016()) continue; // problematic for 2017
      if (!is_signal_) {
        if ( !filt_globalsupertighthalo2016() ) continue;
        if ( !filt_hbhenoise() ) continue;
        if ( !filt_hbheisonoise() )   continue;
        if ( !filt_ecaltp() ) continue;
        if ( !filt_eebadsc() ) continue;
        if ( !filt_goodvtx() ) continue;
        if ( firstGoodVtxIdx() == -1 ) continue;
        if ( !filt_badMuonFilter() ) continue;
        if ( !filt_badChargedCandidateFilter() ) continue;
      }

      TString dsname = dataset();
      if (dsname.Contains("T2tt")) {
        auto checkMassPt = [&](double mstop, double mlsp) { return (mass_stop() == mstop) && (mass_lsp() == mlsp); };
        if (!checkMassPt(600, 450) && !checkMassPt(800, 200) && !checkMassPt(800, 600) && !checkMassPt(1200, 200))
          continue;
        plot2D("h_T2tt_masspts", mass_stop(), mass_lsp() , evtweight_, testVec[1].histMap, ";M(stop) [GeV]; M(lsp) [GeV]", 100, 300, 1300, 80, 0, 800);
      }

      ++nPassedTotal;

      // Calculate event weight
      // wgtInfo.getEventWeights(); // what does this do?    // <-- breaks on signal samples

      if (!is_data() && !is_signal_) {
        evtweight_ = scale1fb();
      }

      // bool isCorridor = true;   // for testing purpose
      // sysInfo::ID sysID = sysInfo::ID::k_nominal; // temporary for testing
      // if (isCorridor)
      //   evtweight_ = sysInfo::GetEventWeight_corridor( sysID );
      // // else if ( useTightTagHighMlb && useMetTTbarWgts )
      // //   evtweight_ = sysInfo::GetEventWeight_SRbulk( sysID );
      // // else if ( useTightTagHighMlb && !useMetTTbarWgts )
      // //   evtweight_ = sysInfo::GetEventWeight_CR2lbulk( sysID );
      // else
      //   evtweight_ = sysInfo::GetEventWeight( sysID );

      if (doNvtxReweight) {
        if (nvtxs() > 69) continue;
        if (is2016data)
          evtweight_ *= nvtxscale_[nvtxs()];
      }

      // Temporary test for
      testTopTaggingEffficiency(testVec[0]);


      // Fill the variables
      values_.clear();

      values_["mt"] = mt_met_lep();
      values_["met"] = pfmet();
      values_["mt2w"] = MT2W();
      values_["mlb"] = Mlb_closestb();
      values_["tmod"] = topnessMod();
      values_["nlep"] = ngoodleps();
      values_["nvlep"] = nvetoleps();
      values_["njet"] = ngoodjets();
      values_["nbjet"] = ngoodbtags();  // nbtag30();
      values_["ntbtag"] = ntightbtags();
      values_["dphijmet"] = mindphi_met_j1_j2();
      values_["passvetos"] = PassTrackVeto() && PassTauVeto();

      values_["lep1pt"] = lep1_p4().pt();
      values_["dphilmet"] = fabs(lep1_p4().phi() - pfmet_phi());

      values_["lep1eta"] = lep1_p4().eta();
      values_["metphi"] = pfmet_phi();
      values_["nbtag"]  = nanalysisbtags();
      values_["chi2"]    = hadronic_top_chi2();
      values_["leadbpt"] = ak4pfjets_leadbtag_p4().pt();
      values_["mlb_0b"]   = (ak4pfjets_leadbtag_p4() + lep1_p4()).M();
      values_["ht"]      = ak4_HT();
      values_["htratio"] = ak4_htratiom();

      values_["dphilmet"] = fabs(lep1_p4().phi() - pfmet_phi());
      values_["passlep1pt"] = (abs(lep1_pdgid()) == 13 && lep1_p4().pt() > 40) || (abs(lep1_pdgid()) == 11 && lep1_p4().pt() > 45);
      // values_["j1csv"] = ak4pfjets_deepCSV().at(0);  // j1 csv for the compressed region
      values_["j1csv"] = (ak4pfjets_CSV().size())? ak4pfjets_CSV().at(0) : -1;

      // Filling histograms for SR
      fillHistosForSR();

      // testCutFlowHistos(testVec[1]);

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

      // fillHistosForCR0b();

      values_["nlep_rl"] = (ngoodleps() == 1 && nvetoleps() >= 2 && lep2_p4().Pt() > 10)? 2 : ngoodleps();
      values_["mt_rl"] = mt_met_lep_rl();
      values_["mt2w_rl"] = MT2W_rl();
      values_["met_rl"] = pfmet_rl();
      values_["dphijmet_rl"]= mindphi_met_j1_j2_rl();
      values_["tmod_rl"] = topnessMod_rl();
      // values_["mlb_rl"] = Mlb_closestb();
      values_["mll"] = (lep1_p4() + lep2_p4()).M();
      values_["osdilep"] = lep1_pdgid() == -lep2_pdgid();

      // fillHistosForCR2l();
      // fillHistosForCRemu();

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

  writeHistsToFile(testVec);
  writeHistsToFile(SRVec);
  writeHistsToFile(CR0bVec);
  writeHistsToFile(CR2lVec);
  writeHistsToFile(CRemuVec);

  wgtInfo.cleanUp();            // why do we need this?

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
        plot1D("h_metbins"+s,   values_["met"]    , evtweight_, sr.histMap, ";E^{miss}_{T} [GeV]"   , 6, met_bins);

        if (is_signal_) {
          string masspt_suf = "_" + to_string((int) mass_stop()) + "_" + to_string((int) mass_lsp());
          plot1D("h_metbins"+masspt_suf+s, values_["met"], evtweight_, sr.histMap, ";E^{miss}_{T} [GeV]" , 6, met_bins);
        }
        if (doTopTagging && topcands_disc().size() > 0)
          plot1D("h_leadtopcand_disc", topcands_disc()[0], evtweight_, sr.histMap, ";top discriminator", 110, -1.1, 1.1);
      };
      fillhists(suf);
      if ( abs(lep1_pdgid()) == 11 )
        fillhists(suf+"_e");
      else if ( abs(lep1_pdgid()) == 13 )
        fillhists(suf+"_mu");

      if (doTopTagging && topcands_disc().size() > 0 && topcands_disc()[0] > 0.98)
        fillhists("_wtc"+suf);
    }
  }
  // SRVec[0].PassesSelectionPrintFirstFail(values_);
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
        plot1D("h_nbjets"+s,   values_["nbjet"]   , evtweight_, cr.histMap, ";nbtags"               , 6,   0, 6);
        plot1D("h_tmod"+s,     values_["tmod"]    , evtweight_, cr.histMap, ";modified topness"     , 30, -15, 15);
        plot1D("h_mlepb"+s,    values_["mlb_0b"]  , evtweight_, cr.histMap, ";M(l,b) [GeV]"         , 24,  0, 600);
        plot1D("h_dphijmet"+s, values_["dphijmet"], evtweight_, cr.histMap, ";#Delta #phi (j, met)" , 24,  0, 4);
        plot1D("h_mll"+s,      values_["mll"]     , evtweight_, cr.histMap, ";M(ll) [GeV]"          , 100, 0, 500 );

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
  if ( !(HLT_SingleEl() || HLT_SingleMu() || HLT_MET_MHT()) ) return;

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
            plot1D("h_zpt"+s, (lep1_p4() + lep2_p4()).pt(), evtweight_, cr.histMap, ";p_{T}(Z) [GeV]"       , 200, 0, 200);
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

  // Trigger requirements
  if ( !(HLT_SingleEl() || HLT_SingleMu() || HLT_MET_MHT()) ) return;

  for (auto& cr : CR0bVec) {
    if ( cr.PassesSelection(values_) ) {

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

void StopLooper::testTopTaggingEffficiency(SR& sr) {
  // Function to test the top tagging efficiencies and miss-tag rate
  // The current tagger only works for hadronically decay tops
  if (!doTopTagging) return;

  // First need to determine how many gen tops does hadronic decay
  int nHadDecayTops = 2 - gen_nfromtleps_();  // 2 gentops for sure <-- checked

  int ntopcands = topcands_disc().size();
  // Divide the events into tt->1l and tt->2l
  // For tt->1l, test:
  // 1. Raw nTopCand / all evts 2. Raw nTopCand / calculable <-- how many get overlap / rejected
  // 3. has TopCand.disc > 0 / all events 4. has TopCand.disc > 0 / raw TopCand > 0
  // 5. count 2D vecTopCand.size(), TopCand.disc

  // int calculable = (ngoodbtags() > 0 && ngoodjets() > 2);
  // calculable += (ngoodjets() > 5);
  // int n4jets = (ngoodbtags() > 0 && ngoodjets() > 3);

  int calculable = (ngoodbtags() > 0 && ngoodjets() > 2);
  calculable += (ngoodjets() > 5);
  int n4jets = (ngoodbtags() > 0 && ngoodjets() > 3);

  if (nHadDecayTops == 1) {
    plot1D("h_calculable", calculable, evtweight_, sr.histMap, ";N(events)", 4, 0, 4);
    plot1D("h_n4jets", n4jets, evtweight_, sr.histMap, ";N(4j events)", 4, 0, 4);
    if (n4jets == 0) return;

    float ratio_ntcandvscalable;
    if (calculable != 0) ratio_ntcandvscalable = (float) topcands_disc().size() / calculable;
    else ratio_ntcandvscalable = (ntopcands == 0)? -1/12 : 2.5;
    plot1D("h_ratio_ntcandvscalable", ratio_ntcandvscalable, evtweight_, sr.histMap, ";N(topcand)", 7, -0.5, 3);

    plot1D("h_ntopcand_raw", ntopcands, evtweight_, sr.histMap, ";N(topcand)", 4, 0, 4);
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
          int leptonictopidx = -1;
          for (size_t l = 0; l < genleps_id().size(); ++l) {
            if (genleps_isfromt().at(l)) {
              leptonictopidx = genleps_gmotheridx().at(l);
              break;
            }
          }
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

      plot1D("hden_disc", topcands_disc().at(0), evtweight_, sr.histMap, ";discriminator", 1100, -1.1, 1.1);
      if (ntopcandp98 >= 1) {
        plot1D("hden_pt", topcands_p4().at(0).pt(), evtweight_, sr.histMap, ";discriminator", 110, 0, 1100);
        plot1D("hden_gentoppt", gentoppt, evtweight_, sr.histMap, ";p_{T}(gen top)", 110, 0, 1100);
      }
      if (isActualTopJet) {
        plot1D("hnom_disc", topcands_disc().at(0), evtweight_, sr.histMap, ";topcand discriminator", 1100, -1.1, 1.1);
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
    plot1D("h_nfakecand_raw", ntopcands, evtweight_, sr.histMap, ";N(topcand)", 4, 0, 4);

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


void StopLooper::temporaryAnalysisDump(string suffix) {

  // // Find the gen pT of the ttbar system
  // double ttbarPt = -99.9;
  // LorentzVector genTTbar_LV;
  // int nFoundGenTop=0;
  // if (sampleIsTTbar) {
  //   for (int iGen=0; iGen<(int)genqs_p4().size(); iGen++) {
  //     if ( abs(genqs_id().at(iGen))==6 && genqs_isLastCopy().at(iGen) ) {
  //       genTTbar_LV += genqs_p4().at(iGen);
  //       nFoundGenTop++;
  //     } // end if last copy of top
  //   } // end loop over gen quarks

  //   if ( nFoundGenTop == 2 ) ttbarPt = genTTbar_LV.Pt();
  // } // end if sample is ttbar


  // // Calculate p4 of (lep1 lep2 b b) system
  // LorentzVector lep1lep2bb_TLV(0.0,0.0,0.0,0.0);
  // lep1lep2bb_TLV += lep1_p4();
  // if (nvetoleps() > 1) lep1lep2bb_TLV += lep2_p4();

  // int jet1_idx = -1;
  // double max_csv = -99.9;
  // for (int iJet=0; iJet<(int)ak4pfjets_p4().size(); iJet++) {
  //   if (ak4pfjets_CSV().at(iJet) > max_csv) {
  //     jet1_idx = iJet;
  //     max_csv  = ak4pfjets_CSV().at(iJet);
  //   }
  // }
  // if (jet1_idx >= 0) lep1lep2bb_TLV += ak4pfjets_p4().at(jet1_idx);

  // int jet2_idx = -1;
  // max_csv = -99.9;
  // for (int iJet=0; iJet<(int)ak4pfjets_p4().size(); iJet++) {
  //   if (iJet == jet1_idx) continue;
  //   if (ak4pfjets_CSV().at(iJet) > max_csv) {
  //     jet2_idx = iJet;
  //     max_csv = ak4pfjets_CSV().at(iJet);
  //   }
  // }
  // if(jet2_idx>=0) lep1lep2bb_TLV += ak4pfjets_p4().at(jet2_idx);

  // // Calculate p4 of (lep1 lep2 b b MET) system
  // double lep1lep2bbMet_pt = -99.9;
  // LorentzVector lep1lep2bbMet_TLV = lep1lep2bb_TLV;
  // LorentzVector met_TLV( pfmet()*cos(pfmet_phi()), pfmet()*sin(pfmet_phi()), 0.0, pfmet() );
  // lep1lep2bbMet_TLV += met_TLV;
  // lep1lep2bbMet_pt = lep1lep2bbMet_TLV.Pt();

  // // Calculate mlb using lep2 instead of lep1
  // LorentzVector lep2b_TLV(0.0,0.0,0.0,0.0);
  // double minDr = 99.9;
  // int minBJetIdx = -99;
  // if(nvetoleps()>1) {
  //   lep2b_TLV += lep2_p4();
  //   for(int iJet=0; iJet<(int)ak4pfjets_p4().size(); iJet++) {
  //     if(!ak4pfjets_passMEDbtag().at(iJet)) continue;
  //     if(ROOT::Math::VectorUtil::DeltaR(ak4pfjets_p4().at(iJet),lep2_p4())<minDr) {
  //       minDr = ROOT::Math::VectorUtil::DeltaR(ak4pfjets_p4().at(iJet),lep2_p4());
  //       minBJetIdx = iJet;
  //     }
  //   } // end loop over jets
  //   if(minBJetIdx>=0) lep2b_TLV += ak4pfjets_p4().at(minBJetIdx);
  // } // end if nvetoleps>1

}
