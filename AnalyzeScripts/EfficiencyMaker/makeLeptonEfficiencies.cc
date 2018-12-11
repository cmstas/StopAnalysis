// -*- C++ -*-
// Usage:
// > root -b -q doAll.C

#include <iostream>
#include <vector>

// ROOT
#include "TBenchmark.h"
#include "TChain.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TROOT.h"
#include "TTreeCache.h"
#include "TVector2.h"
#include "TF1.h"
#include "TH2.h"

// CORE
#include "../../CORE/CMS3.h"
#include "../../CORE/Tools/utils.cc"
// #include "../../CORE/Tools/goodrun.cc"
// #include "../../CORE/Tools/dorky/dorky.cc"
// #include "../../CORE/MetSelections.cc"
#include "../../CORE/Config.h"
#include "../../CORE/MuonSelections.h"
#include "../../CORE/ElectronSelections.h"
// #include "../../CORE/IsolationTools.cc"
// #include "../../CORE/TriggerSelections.cc"

// Stop Selections

using namespace std;
using namespace tas;

const string inputdir = "/hadoop/cms/store/group/snt/run2_mc2017/TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_MINIAODSIM_CMS4_V09-04-19";
const bool plotEfficiency = true;

template<typename... Ts>
inline void plot1d(string name, float xval, double weight, map<string, TH1*> &allhistos, Ts... args)
{
  auto iter = allhistos.find(name);
  if (iter == allhistos.end()) {
    TH1D* currentHisto= new TH1D(name.c_str(), args...);
    currentHisto->Sumw2();
    currentHisto->Fill(xval, weight);
    allhistos.insert( pair<string, TH1D*>(name, currentHisto) );
  } else {
    iter->second->Fill(xval, weight);
  }
}

template<typename... Ts>
inline void plot2d(string name, float xval, float yval, double weight, map<string, TH1*> &allhistos, Ts... args)
{
  auto iter = allhistos.find(name);
  if (iter == allhistos.end()) {
    TH2D* currentHisto= new TH2D(name.c_str(), args...);
    currentHisto->Sumw2();
    currentHisto->Fill(xval, yval, weight);
    allhistos.insert( pair<string, TH2D*>(name, currentHisto) );
  } else {
    ((TH2D*) iter->second)->Fill(xval, yval, weight);
  }
}

int ScanChain(TChain* chain, TString outname = "lepEff.root", int nEvents = -1, bool fast = true) {

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

  // Histograms Definition

  if (!outname.EndsWith(".root")) outname += ".root";
  TFile* outfile = new TFile(outname, "RECREATE");

  // Loop over events to Analyze
  unsigned int nDuplicates = 0;
  unsigned int nEventsTotal = 0;
  unsigned int nEventsSelected = 0;
  unsigned int nEventsChain = chain->GetEntries();
  if (nEvents >= 0) nEventsChain = nEvents;
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;

  // const char* json_file = "../../StopCORE/inputs/json_files/Cert_294927-302343_13TeV_PromptReco_Collisions17_JSON_snt.txt";
  // if (false) {
  //   cout << "Loading json file: " << json_file << endl;
  //   set_goodrun_file(json_file);
  // }

  enum EffType { lepeff_veto_el, lepeff_veto_mu, lepeff_veto_tau, nEffType};

  vector<TString> hname(nEffType);
  vector<TString> htitle(nEffType);

  hname[lepeff_veto_el]  = "lepeff_veto_el";
  hname[lepeff_veto_mu]  = "lepeff_veto_mu";
  hname[lepeff_veto_tau] = "lepeff_veto_tau";

  htitle[lepeff_veto_el]  = ";p_{T} (gen e); #eta (gen e)";
  htitle[lepeff_veto_mu]  = ";p_{T} (gen #mu); #eta (gen #mu)";
  htitle[lepeff_veto_tau] = ";p_{T} (gen #tau); #eta (gen #tau)";

  vector<TH1*> hnum(nEffType);
  vector<TH1*> hden(nEffType);
  
  const vector<float> bins_pt = {0, 10, 20, 30, 50, 100, 200};
  const vector<float> bins_eta = {0, 0.5, 1.0, 1.5, 2.5};

  gconf.GetConfigsFromDatasetName(inputdir.c_str());
  int year = gconf.year - 2000;

  for(int ihist = 0; ihist < nEffType; ++ihist){
    hden[ihist] = new TH2D(Form("hden%d_", year)+hname[ihist], htitle[ihist], bins_pt.size()-1, bins_pt.data(), bins_eta.size()-1, bins_eta.data());
    hnum[ihist] = new TH2D(Form("hnum%d_", year)+hname[ihist], htitle[ihist], bins_pt.size()-1, bins_pt.data(), bins_eta.size()-1, bins_eta.data());
  }

  // File Loop
  while ( (currentFile = (TFile*)fileIter.Next()) ) {

    // Get File Content
    TFile file(currentFile->GetTitle());
    TTree *tree = (TTree*)file.Get("Events");
    if (fast) TTreeCache::SetLearnEntries(10);
    if (fast) tree->SetCacheSize(128*1024*1024);
    cms3.Init(tree);

    outfile->cd();
    // Loop over Events in current file
    if (nEventsTotal >= nEventsChain) continue;
    unsigned int nEventsTree = tree->GetEntriesFast();
    for (unsigned int event = 0; event < nEventsTree; ++event) {
      // if (event > 10) break;                 // testing
      // Get Event Content
      if (nEventsTotal >= nEventsChain) continue;
      if (fast) tree->LoadTree(event);
      cms3.GetEntry(event);
      ++nEventsTotal;

      // Progress
      CMS3::progress( nEventsTotal, nEventsChain );

      // Analysis Code
      // if (!goodrun(cms3.evt_run(), cms3.evt_lumiBlock())) continue;
      // duplicate_removal::DorkyEventIdentifier id(cms3.evt_run(), cms3.evt_event(), cms3.evt_lumiBlock());
      // if (is_duplicate(id)) { ++nDuplicates; continue; }

      float met = evt_pfmet();
      if (met < 150) continue;

      // float run = evt_run();

      if (pfjets_p4().size() < 2) continue;
      if ((els_p4().size() + mus_p4().size()) < 1) continue;
      int njet30 = 0;
      for (auto jet : pfjets_p4()) {
        if (jet.pt() > 30 && fabs(jet.eta()) < 2.4) njet30++;
      }
      if (njet30 < 2) continue;

      int nleps = 0;
      int nvleps = 0;
      for (auto el : els_p4()) {
        if (fabs(el.eta()) > 2.4) continue;
        if (el.pt() > 20) nleps++;
        if (el.pt() > 10) nvleps++;
      }
      for (auto mu : mus_p4()) {
        if (fabs(mu.eta()) > 2.4) continue;
        if (mu.pt() > 20) nleps++;
        if (mu.pt() > 10) nvleps++;
      }
      if (nleps < 1) continue;

      nEventsSelected++;

      for (auto ibegin = els_p4().begin(), iend = els_p4().end(), el = ibegin; el != iend; ++el) {
        if (el->pt() < 10 || fabs(el->eta()) > 2.4) continue;  // veto electron kinematic selection
        for (size_t igen = 0; igen < genps_p4().size(); ++igen) {
          if (!genps_isLastCopy().at(igen)) continue;
          if (abs(genps_id().at(igen)) != 11) continue;
          if (!genps_fromHardProcessFinalState().at(igen)) continue;

          float pt = genps_p4()[igen].pt();
          if (pt > bins_pt.back()) pt = bins_pt.back()-0.001;
          float aeta = fabs(genps_p4()[igen].eta());
          if (pt < 5 || aeta > 2.4) continue;
          if (!utils::isCloseObject(*el, genps_p4()[igen], 0.1)) continue;

          // Found gen matching, fill in denominator now
          hden[lepeff_veto_el]->Fill(pt, aeta);
          if (electronID(el-ibegin, STOP_veto_v4))
            hnum[lepeff_veto_el]->Fill(pt, aeta);

          break;
        }
      }

      for (auto ibegin = mus_p4().begin(), iend = mus_p4().end(), mu = ibegin; mu != iend; ++mu) {
        if (mu->pt() < 10 || fabs(mu->eta()) > 2.4) continue;  // veto muon kinematic selection
        for (size_t igen = 0; igen < genps_p4().size(); ++igen) {
          if (!genps_isLastCopy().at(igen)) continue;
          if (abs(genps_id().at(igen)) != 13) continue;
          if (!genps_fromHardProcessFinalState().at(igen)) continue;

          float pt = genps_p4()[igen].pt();
          if (pt > bins_pt.back()) pt = bins_pt.back()-0.001;
          float aeta = fabs(genps_p4()[igen].eta());
          if (pt < 5 || aeta > 2.4) continue;
          if (!utils::isCloseObject(*mu, genps_p4()[igen], 0.1)) continue;

          // Found gen matching, fill in denominator now
          hden[lepeff_veto_mu]->Fill(pt, aeta);
          if (muonID(mu-ibegin, STOP_loose_v4))
            hnum[lepeff_veto_mu]->Fill(pt, aeta);

          break;
        }
      }

      for (size_t itau = 0; itau < taus_pf_p4().size(); ++itau) {
        if (taus_pf_p4().at(itau).Pt() < 20) continue;
        if (fabs(taus_pf_p4().at(itau).Eta()) > 2.3) continue;

        for (size_t igen = 0; igen < genps_p4().size(); ++igen) {
          if (!genps_isLastCopy().at(igen)) continue;
          if (abs(genps_id().at(igen)) != 15) continue;
          if (!genps_fromHardProcessFinalState().at(igen) && !genps_fromHardProcessDecayed().at(igen)) continue;
          if (!utils::isCloseObject(taus_pf_p4()[itau], genps_p4()[igen], 0.3)) continue;
          // TODO: make sure it is decaying hadronically <-- add in later

          // Check if already veto'ed by the isotrack veto
          bool isVetoTrack = false;
          for (unsigned int itrk = 0; itrk < isotracks_p4().size(); ++itrk) {
            if (!isotracks_isPFCand().at(itrk)) continue;  // only consider pfcandidates
            if (isotracks_p4().at(itrk).pt() < 10) continue;
            if (fabs(isotracks_p4().at(itrk).eta()) > 2.4 ) continue;
            if (isotracks_charge().at(itrk) == 0) continue;
            if (fabs(isotracks_dz().at(itrk)) > 0.1) continue;
            if (isotracks_lepOverlap().at(itrk)) continue;  // should remove all lep overlap, but it didn't, so we need the line below
            if (!utils::isCloseObject(taus_pf_p4()[itau], isotracks_p4()[itrk], 0.3)) continue;

            float pfiso = isotracks_pfIso_ch().at(itrk);

            if (isotracks_p4().at(itrk).pt() > 60.) {
              if (pfiso < 6.0 ) isVetoTrack = true;
            } else {
              if (pfiso/isotracks_p4().at(itrk).pt() < 0.1) isVetoTrack = true;
            }
          }
          if (isVetoTrack) continue;

          // Check if the decay mode is nprong == 3 or nprong == 5, as is checked when applying the veto SF
          int nprongs = 0;
          bool leptonic = false;
          for (size_t genx = 0; genx < genps_p4().size(); genx++) {
            if (abs(genps_id().at(genps_idx_mother().at(genx))) == 24) {
              if (genps_idx_mother().at(genps_idx_mother().at(genx)) != (int)igen) continue;
            } else if (genps_idx_mother().at(genx) != (int)igen) {
              continue;
            }
            if (genps_status().at(genx) != 1 && genps_status().at(genx) != 2) continue;
            int pdgid = abs(genps_id().at(genx));
            if (pdgid == 11 || pdgid == 13) {
              leptonic = true;
              break;
            }
            if (pdgid != 24 && genps_charge().at(genx) != 0) {
              if (pdgid == 20213) {
                for (unsigned int genx2 = 0; genx2 < genps_p4().size(); genx2++) {
                  if (genps_idx_mother().at(genx2) != (int)genx) continue;
                  if (genps_charge().at(genx2) != 0) nprongs++;
                }
              } else {
                nprongs++;
              }
            }
          }
          if (leptonic || (nprongs != 1 && nprongs != 3)) continue;


          // Found gen matching, fill in denominator now
          float pt = genps_p4()[igen].pt();
          if (pt > bins_pt.back()) pt = bins_pt.back()-0.001;
          float aeta = fabs(genps_p4()[igen].eta());
          // if (pt < 15 || aeta > 2.3) continue;

          hden[lepeff_veto_tau]->Fill(pt, aeta);

          if (passTauID("decayModeFinding", itau) < 1) break;
          if (passTauID("byMediumIsolationMVArun2v1DBoldDMwLT", itau) < 1) break;

          // Passed veto tau selection, fill in the numerator
          hnum[lepeff_veto_tau]->Fill(pt, aeta);

          break;
        }
      }

    }
  
    // Clean Up
    delete tree;
    file.Close();
  }
  if (nEventsChain != nEventsTotal) {
    cout << Form( "ERROR: number of events from files (%d) is not equal to total number of events (%d)", nEventsChain, nEventsTotal ) << endl;
  }
  
  // Write Histograms
  outfile->cd();

  for (int i = 0; i < nEffType; ++i) {
    hnum[i]->Write();
  }
  for (int i = 0; i < nEffType; ++i) {
    hden[i]->Write();
  }

  if (plotEfficiency) {
    for (int i = 0; i < nEffType; ++i) {
      TH2D* heff = (TH2D*) hnum[i]->Clone(Form("heff%d_", year)+hname[i]);
      heff->Divide(hnum[i], hden[i], 1, 1, "B");
      heff->Write();
    }
  }

  // return
  bmark->Stop("benchmark");
  cout << endl;
  cout << nEventsTotal << " Events Processed, where " << nDuplicates << " duplicates were skipped" << endl;
  cout << nEventsSelected << " Events total pass all the selections" << endl;
  cout << "------------------------------" << endl;
  cout << "CPU  Time: " << Form( "%.01f", bmark->GetCpuTime("benchmark")  ) << endl;
  cout << "Real Time: " << Form( "%.01f", bmark->GetRealTime("benchmark") ) << endl;
  cout << endl;
  delete bmark;
  return 0;
}

int makeLeptonEfficiencies() {
  
  TChain* ch = new TChain("Events");
  ch->Add(Form("%s/merged_ntuple_*.root", inputdir.c_str()));

  ScanChain(ch);

  return 0;
}

int main(int argc, char** argv)
{
  return makeLeptonEfficiencies();
}
