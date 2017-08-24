// C++
#include <iostream>
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
#include "StopSelections.h"
#include "StopLooper.h"
#include "PlotUtilities.h"

using namespace std;
using namespace stop_1l;
using namespace selectionInfo;

class SR;
extern stop_1l_babyAnalyzer babyAnalyzer;

const bool verbose = false;
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
// turn on to apply Nvtx reweighting to MC
const bool doNvtxReweight = false;
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

  CR2lVec = getStopControlRegionsDilepton();
  CR0bVec = getStopControlRegionsNoBTags();
  CRemuVec = getStopControlRegionsEMu();

  if (verbose) {
    cout << "CR2lVec.size = " << CR2lVec.size() << ", including the following:" << endl;
    for (auto it = CR2lVec.begin(); it != CR2lVec.end(); ++it)
      cout << it-CR2lVec.begin() << "  "<< it->GetName() << endl;
  }

  for (auto& cr : CR2lVec) {
    vector<string> vars = cr.GetListOfVariables();
    TDirectory * dir = (TDirectory*) outfile_->Get((cr.GetName() + "/ranges").c_str());
    if (dir == 0) dir = outfile_->mkdir((cr.GetName() + "/ranges").c_str());
    dir->cd("ranges");
    for (auto& var : vars) {
      plot1D("h_"+var+"_"+"LOW",  1, cr.GetLowerBound(var), cr.histMap, "", 1, 0, 2);
      plot1D("h_"+var+"_"+"HI",   1, cr.GetUpperBound(var), cr.histMap, "", 1, 0, 2);
    }
  }

  for (auto& cr : CRemuVec) {
    vector<string> vars = cr.GetListOfVariables();
    TDirectory * dir = (TDirectory*) outfile_->Get((cr.GetName() + "/ranges").c_str());
    if (dir == 0) dir = outfile_->mkdir((cr.GetName() + "/ranges").c_str());
    dir->cd("ranges");
    for (auto& var : vars) {
      plot1D("h_"+var+"_"+"LOW",  1, cr.GetLowerBound(var), cr.histMap, "", 1, 0, 2);
      plot1D("h_"+var+"_"+"HI",   1, cr.GetUpperBound(var), cr.histMap, "", 1, 0, 2);
    }
  }

}


void StopLooper::GenerateAllSRptrSets() {
  allSRptrSets.clear();

  vector<SR*> all_SRptrs;
  // all_SRptrs.reserve(SRVec.size() + CRVec.size());
  for (auto& sr : SRVec) all_SRptrs.push_back(&sr);
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

  // // full 2016 dataset json, 36.26/fb:
  // const char* json_file = "../StopCORE/inputs/json_files/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON_snt.txt";
  // // 2017 dataset json
  // const char* json_file = "../StopCORE/inputs/json_files/Cert_294927-300575_13TeV_PromptReco_Collisions17_JSON_snt.txt";

  // Combined 2016 and 2017 json,
  const char* json_file = "../StopCORE/inputs/json_files/Cert_271036-300575_13TeV_Combined1617_JSON_snt.txt";
  if (applyjson) {
    cout << "Loading json file: " << json_file << endl;
    set_goodrun_file(json_file);
  }

  // sampleInfo::sampleUtil sample( sampleInfo::ID::k_single_mu );
  TFile dummy( (output_dir+"/dummy.root").c_str(), "RECREATE" );
  SetSignalRegions();
  GenerateAllSRptrSets();

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
	if ( is_duplicate(id) ) continue;
      }

      // Apply filters
      // if (!filt_globalTightHalo2016()) continue; // problematic
      if ( !filt_globalsupertighthalo2016() ) continue;
      if ( !filt_hbhenoise() ) continue;
      if ( !filt_hbheisonoise() )   continue;
      if ( !filt_ecaltp() ) continue;
      if ( !filt_eebadsc() ) continue;
      if ( !filt_goodvtx() ) continue;
      if ( firstGoodVtxIdx() == -1 ) continue;
      if ( !filt_badMuonFilter() ) continue;
      if ( !filt_badChargedCandidateFilter() ) continue;

      // Place for some vetos
      // if ( therecolepveto && nvetoleps()!=1 ) continue;
      // if ( theisotrackveto && !PassTrackVeto() ) continue;
      // if ( thetauveto && !PassTauVeto() ) continue;


      ++nPassedTotal;

      evtweight_ = 1.0; // just here for demonstration purpose for now.

      // Fill the variables

      values_["mt"] = mt_met_lep();
      values_["met"] = pfmet();
      values_["mt2w"] = MT2W();
      // values_["mlb"] = Mlb_closestb(); // currently all -9999
      values_["mlb"] = 200;          // for temporary check purpose
      values_["tmod"] = topnessMod();
      values_["nlep"] = ngoodleps();
      values_["njet"] = ngoodjets();
      values_["nbjet"] = ngoodbtags();
      values_["dphijmet"] = mindphi_met_j1_j2();
      values_["dphilmet"] = fabs(lep1_p4().phi() - pfmet_phi());

      values_["lep1pt"] = lep1_p4().pt();
      values_["lep1eta"] = lep1_p4().eta();

      values_["chi2"]    = hadronic_top_chi2();
      values_["leadbpt"] = ak4pfjets_leadbtag_p4().pt();
      values_["mlepb"]   = (ak4pfjets_leadbtag_p4() + lep1_p4()).M();
      values_["htratio"] = ak4_htratiom();
      values_["ht"]      = ak4_HT();

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

      if (ngoodleps() == 1 && nvetoleps() >= 2 && lep2_p4().Pt()>10)
        values_["nlep"] = 2;
      values_["mt_rl"] = mt_met_lep_rl();
      values_["mt2w_rl"] = MT2W_rl();
      values_["met_rl"] = pfmet_rl();
      values_["dphijmet_rl"]= mindphi_met_j1_j2_rl();
      values_["tmod_rl"] = topnessMod_rl();

      // fillHistosForCR2l();

      fillHistosForCRemu();

      // if (event > 10) break;  // for debug purpose
    } // end of event loop

    delete tree;
    file.Close();
  } // end of file loop

  cout << "[StopLooper::looper] processed  " << nEventsTotal << " events" << endl;
  if ( nEventsChain != nEventsTotal )
    std::cout << "WARNING: number of events from files is not equal to total number of events" << std::endl;

  outfile_->cd();

  for (auto& cr : CR2lVec) {
    TDirectory * dir = (TDirectory*) outfile_->Get(cr.GetName().c_str());
    if (dir == 0) dir = outfile_->mkdir(cr.GetName().c_str()); // shouldn't happen
    dir->cd();
    for (auto& h : cr.histMap) {
      if (h.first.find("HI") != string::npos || h.first.find("LOW") != string::npos) continue;
      h.second->Write();
    }
  }

  for (auto& cr : CRemuVec) {
    TDirectory * dir = (TDirectory*) outfile_->Get(cr.GetName().c_str());
    if (dir == 0) dir = outfile_->mkdir(cr.GetName().c_str()); // shouldn't happen
    dir->cd();
    for (auto& h : cr.histMap) {
      if (h.first.find("HI") != string::npos || h.first.find("LOW") != string::npos) continue;
      h.second->Write();
    }
  }

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

void StopLooper::fillHistosForCRemu(string suf) {

  // Trigger requirements
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
    if (cr.PassesSelection(values_)) {

      auto fillhists = [&] (string s) {
        plot1D("h_mt"+suf+s,       values_["mt"]      , evtweight_, cr.histMap, ";MT [GeV]"             , 12,  0, 600);
        plot1D("h_mt2w"+suf+s,     values_["mt2w"]    , evtweight_, cr.histMap, ";MT2W [GeV]"           , 18,  50, 500);
        plot1D("h_met"+suf+s,      values_["met"]     , evtweight_, cr.histMap, ";E_{T}^{miss} [GeV]"   , 24,  50, 650);
        plot1D("h_lep1pt"+suf+s,   values_["lep1pt"]  , evtweight_, cr.histMap, ";p_{T}(lep1) [GeV]"    , 20,  0, 200);
        plot1D("h_lep2pt"+suf+s,   values_["lep2pt"]  , evtweight_, cr.histMap, ";p_{T}(lep2) [GeV]"    , 20,  0, 200);
        plot1D("h_lep1eta"+suf+s,  values_["lep1eta"] , evtweight_, cr.histMap, ";#eta (lep1)"          , 20, -5, 5);
        plot1D("h_lep2eta"+suf+s,  values_["lep2eta"] , evtweight_, cr.histMap, ";#eta (lep2)"          , 20, -5, 5);
        plot1D("h_nleps"+suf+s,    values_["nlep"]    , evtweight_, cr.histMap, ";njets"                , 12,  0, 12);
        plot1D("h_njets"+suf+s,    values_["njet"]    , evtweight_, cr.histMap, ";njets"                , 12,  0, 12);
        plot1D("h_nbjets"+suf+s,   values_["nbjet"]   , evtweight_, cr.histMap, ";nbjets"               , 6,   0, 6);
        plot1D("h_mlepb"+suf+s,    values_["mlb"]     , evtweight_, cr.histMap, ";M(l,b) [GeV]"         , 24,  0, 600);
        plot1D("h_dphijmet"+suf+s, values_["dphijmet"], evtweight_, cr.histMap, ";#Delta #phi (j, met)" , 24,  0, 4);

        const float leppt_bins[] = {0, 30, 40, 50, 75, 100, 125, 200};
        plot1D("h_lep1ptbins"+suf+s, values_["lep1pt"], evtweight_, cr.histMap, ";p_{T}(lep1) [GeV]"  , 7, leppt_bins);
        plot1D("h_lep2ptbins"+suf+s, values_["lep2pt"], evtweight_, cr.histMap, ";p_{T}(lep2) [GeV]"  , 7, leppt_bins);
      };
      fillhists("");
      if (HLT_MuE())
        fillhists("_passHLT");
    }
  }
}


void StopLooper::fillHistosForSR(string suf) {

  for (auto& sr : SRVec) {
    if ( sr.PassesSelection(values_) ) {
      // This plot function port from MT2 demonstrate the simpleness of adding extra plots anywhere in the code
      // and this functionality is great for quick checks
      plot1D("h_met"+suf, pfmet(), evtweight_, sr.histMap, ";E_{T}^{miss} [GeV]", 150, 0, 1500);
      plot1D("h_njet"+suf, ngoodjets(), evtweight_, sr.histMap, ";n jets", 20, 0, 20);

      // fillBasic1DHistos(sr.histMap);

      // The following skim is under development, can be used to fill standard set of plots
      // for (string v : {"njet", "nbjet", "mt2w"})
      //   plot1d("h_"+v +suf, values_[v], evtweight_, sr.histMap);
    }
  }
}

void StopLooper::fillHistosForCR2l(string suf) {

  for (auto& cr : CR2lVec) {
    if ( cr.PassesSelection(values_) ) {
      // This plot function port from MT2 demonstrate the simpleness of adding extra plots anywhere in the code
      // and this functionality is great for quick checks
      // if (cr.GetName() != "cr2l_test" && cr.GetName() != "cr2l_base")
      //   cout << __LINE__ << ": Passes Selections! " << suf << endl;

      plot1D("h_MET"+suf, pfmet_rl(), evtweight_, cr.histMap, ";E_{T}^{miss} [GeV]", 150, 0, 1500);

      plot1D("h_met"+suf,      values_["met_rl"]   , evtweight_, cr.histMap, ";E_{T}^{miss} [GeV]"   , 24,  50, 650);
      plot1D("h_mt"+suf,       values_["mt_rl"]    , evtweight_, cr.histMap, ";MT"                   , 12,  0, 600);
      plot1D("h_mt2w"+suf,     values_["mt2w_rl"]  , evtweight_, cr.histMap, ";MT2W"                 , 18,  50, 500);
      plot1D("h_tmod"+suf,     values_["tmod"]     , evtweight_, cr.histMap, ";t_{mod}"              , 20, -5, 15);
      plot1D("h_njets"+suf,    values_["njets"]    , evtweight_, cr.histMap, ";njets"                , 12,  0, 12);
      plot1D("h_nbjets"+suf,   values_["nbjets"]   , evtweight_, cr.histMap, ";nbjets"               , 6,   0, 6);
      plot1D("h_mlepb"+suf,    values_["mlepb"]    , evtweight_, cr.histMap, ";M(l,b)"               , 24,  0, 600);
      plot1D("h_dphijmet"+suf, values_["dphijmet"] , evtweight_, cr.histMap, ";#Delta #phi (j, met)" , 24,  0, 4);

      plot1D("h_nleps"+suf,  values_["nlep"]   , evtweight_, cr.histMap, ";njets"                , 12,  0, 12);
      // plot1D("h_mlb"+suf,       Mlb_closestb() , evtweight_, cr.histMap, ";M(l,b)"               , 24,  0, 600);


      // plot1D("h_wpt"+suf,          wpt_        , evtweight_, cr.histMap, ";p_{T}(W)"             , 13,  0, 650);

      // The following skim is under development, can be used to fill standard set of plots
      // for (string v : {"njet", "nbjet", "mt2w"})
      //   plot1d("h_"+v +suf, values_[v], evtweigh  t_, cr.histMap);
    }
  }
}

void StopLooper::fillHistosForCR0b(string suf) {
  for (auto& cr : CR0bVec) {
    if ( cr.PassesSelection(values_) ) {
      // This plot function port from MT2 demonstrate the simpleness of adding extra plots anywhere in the code
      // and this functionality is great for quick checks
      plot1D("h_met"+suf, pfmet(), evtweight_, cr.histMap, ";E_{T}^{miss} [GeV]", 150, 0, 1500);
      plot1D("h_njet"+suf, ngoodjets(), evtweight_, cr.histMap, ";n jets", 20, 0, 20);

      // The following skim is under development, can be used to fill standard set of plots
      // for (string v : {"njet", "nbjet", "mt2w"})
      //   plot1d("h_"+v +suf, values_[v], evtweigh  t_, cr.histMap);
    }
  }
}


/*
  void StopLooper::fillBasic1DHistos(map<string,TH1*>& h1d, string suf) {

  plot1D("h_mt"+suf,           mt_         , evtweight_, h1d, ";MT"                   , 12,  0, 600);
  plot1D("h_mt2w"+suf,         mt2w_       , evtweight_, h1d, ";MT2W"                 , 18,  50, 500);
  plot1D("h_tmod"+suf,         tmod_       , evtweight_, h1d, ";t_{mod}"              , 20, -5, 15);
  plot1D("h_chi2"+suf,         chi2_       , evtweight_, h1d, ";#chi^{2}"             , 20,  0, 20);
  plot1D("h_mindphi"+suf,      mindphi_    , evtweight_, h1d, ";min(#Delta Phi)"      , 20,  0, 4);
  plot1D("h_htratio"+suf,      htratio_    , evtweight_, h1d, ";HTratio"              , 20,  0, 1);
  plot1D("h_drlepb"+suf,       drlepb_     , evtweight_, h1d, ";DR(l,b)"              , 20,  0, 5);
  plot1D("h_leadbpt"+suf,      leadbpt_    , evtweight_, h1d, ";leadbpt"              , 18,  30, 390);
  plot1D("h_met"+suf,          met_        , evtweight_, h1d, ";E_{T}^{miss}"         , 24,  50, 650);
  plot1D("h_leppt"+suf,        leppt_      , evtweight_, h1d, ";p_{T} (lep)"          , 26,  30, 680);
  plot1D("h_lepeta"+suf,       lepeta_     , evtweight_, h1d, ";#eta (lep)"           , 30, -3, 3);
  plot1D("h_lepphi"+suf,       lepphi_     , evtweight_, h1d, ";#phi (lep)"           , 20, -1, 1);
  plot1D("h_ht"+suf,           ht_         , evtweight_, h1d, ";HT"                   , 30,  0, 1500);
  plot1D("h_njets"+suf,        njets_      , evtweight_, h1d, ";njets"                , 12,  0, 12);
  plot1D("h_nbjets"+suf,       nbjets_     , evtweight_, h1d, ";nbjets"               , 6,   0, 6);
  plot1D("h_mlepb"+suf,        mlepb_      , evtweight_, h1d, ";M(l,b)"               , 24,  0, 600);
  plot1D("h_wpt"+suf,          wpt_        , evtweight_, h1d, ";p_{T}(W)"             , 13,  0, 650);

  }

  void StopLooper::fillMore1DHistos(map<string,TH1*>& h1d, string suf) {

  plot1D("h_topness"+suf,                   0, evtweight_, h1d, ";Topness"              , 18,  50, 500);
  plot1D("h_njets_pt200_nobtags"+suf,       0, evtweight_, h1d, ";njets"                , 5,   0, 5);
  plot1D("h_leadisrjet_pt200_nobtags"+suf,  0, evtweight_, h1d, ";p_{T}"                , 24,  0, 600);
  plot1D("h_genmlepb"+suf,                  0, evtweight_, h1d, ";gen M(l,b)"           , 24,  0, 600);
  plot1D("h_m3b"+suf,                       0, evtweight_, h1d, ";M3b"                  , 10,  0, 1);
  plot1D("h_ak4_nef"+suf,                   0, evtweight_, h1d, ";ak4_nef"              , 34,  30, 300);
  plot1D("h_ak4_cef"+suf,                   0, evtweight_, h1d, ";ak4_cef"              , 22,  30, 200);
  plot1D("h_ak4_eff"+suf,                   0, evtweight_, h1d, ";ak4_EF"               , 10,  0, 1.);
  plot1D("h_mhtsig"+suf,                    0, evtweight_, h1d, ";MET/#sqrt{HT}"        , 20,  0, 40);

  plot1D("h_jetpt"+suf,                     0, evtweight_, h1d, ";p_{T}(jet)"           , 32,  30, 480);
  plot1D("h_dphilepjet"+suf,                0, evtweight_, h1d, ";#Delta #phi (l, jet)" , 20,  0, 4);
  plot1D("h_leadjetpt"+suf,                 0, evtweight_, h1d, ";pt(leadjet)"          , 32,  30, 480);
  plot1D("h_dphiwlep"+suf,                  0, evtweight_, h1d, ";DPhiWLep"             , 25,  0, 5);
  plot1D("h_pzeta_trklep"+suf,              0, evtweight_, h1d, ";PZeta_TrkLep"         , 60, -600, 600);
  plot1D("h_pzetavis_trklep"+suf,           0, evtweight_, h1d, ";PZetaVis_TrkLep"      , 12,  0, 600);

  }
*/
