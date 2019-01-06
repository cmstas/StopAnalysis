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
#include "../StopCORE/StopTree.h"
// #include "../StopCORE/stop_variables/metratio.cc"

//#include "../StopLooper/SR.h"
//#include "StopRegions.h"
#include "WHLooper.h"
//#include "../StopLooper/Utilities.h"

using namespace std;
using namespace stop1l;

class SR;

const bool verbose = false;
// turn on to apply btag sf - take from files defined in eventWeight_btagSF.cc
const bool applyBtagSFfromFiles = false; // default false
// turn on to apply lepton sf to central value - reread from files
const bool applyLeptonSFfromFiles = false; // default false
// turn on to enable plots of metbins with systematic variations applied. will only do variations for applied weights
const bool doSystVariations = false;
// turn on to enable plots of metbins with different gen classifications
const bool doGenClassification = true;
// turn on to apply Nvtx reweighting to MC / data2016
const bool doNvtxReweight = false;
// turn on top tagging studies, off for baby ver < 25
const bool doTopTagging = true;
// turn on to apply json file to data
const bool applyGoodRunList = true;
// re-run resolved top MVA locally
const bool runResTopMVA = false;
// only produce yield histos
const bool runYieldsOnly = false;
// only running selected signal points to speed up
const bool runFullSignalScan = false;
// debug symbol, for printing exact event kinematics that passes
const bool printPassedEvents = false;

// some global helper variables to be used in member functions
int datayear = -1;
string samplever;

const float fInf = std::numeric_limits<float>::max();

const float kSMSMassStep = 25;
const vector<float> mStopBins = []() { vector<float> bins; for (float i = 150; i < 1350; i += kSMSMassStep) bins.push_back(i); return bins; } ();
const vector<float> mLSPBins  = []() { vector<float> bins; for (float i =   0; i <  750; i += kSMSMassStep) bins.push_back(i); return bins; } ();

std::ofstream ofile;

inline bool sortIndexbyCSV( pair<int, float> &vec1, pair<int, float> &vec2 ) {
    return vec1.second > vec2.second;
}

float getdphi( float phi1 , float phi2 ){                                                                                                                                      
  float dphi = fabs( phi1 - phi2 );
  if( dphi > TMath::Pi() ) dphi = TMath::TwoPi() - dphi;
  return dphi;
}

bool WHLooper::PassingHLTriggers(const int type) {
  if (type == 1) {
    switch (year_) {
      case 2016:
        return ( (HLT_MET110_MHT110() || HLT_MET120_MHT120() || HLT_MET()) ||
                 (abs(lep1_pdgid()) == 11 && HLT_SingleEl()) || (abs(lep1_pdgid()) == 13 && HLT_SingleMu()) );
      case 2017:
      case 2018:
      default:
        return ( HLT_MET_MHT() || (abs(lep1_pdgid()) == 11 && HLT_SingleEl()) || (abs(lep1_pdgid()) == 13 && HLT_SingleMu()) );
    }
  } else if (type == 2) {
    switch (year_) {
      case 2016:
        return ( (HLT_MET() || HLT_MET110_MHT110() || HLT_MET120_MHT120()) || (HLT_SingleEl() && (abs(lep1_pdgid())==11 || abs(lep2_pdgid())==11)) ||
                 (HLT_SingleMu() && (abs(lep1_pdgid())==13 || abs(lep2_pdgid())==13)) );
      case 2017:
      case 2018:
      default:
        return ( HLT_MET_MHT() || (HLT_SingleEl() && (abs(lep1_pdgid())==11 || abs(lep2_pdgid())==11)) ||
                  (HLT_SingleMu() && (abs(lep1_pdgid())==13 || abs(lep2_pdgid())==13)) );
    }
  } else if (type == 3) {
    int dilepid = abs(lep1_pdgid()*lep2_pdgid());
    switch (year_) {
      case 2016:
        return ( (HLT_MET() || HLT_MET110_MHT110() || HLT_MET120_MHT120()) ||
                 (dilepid == 121 && HLT_DiEl()) || (dilepid == 169 && HLT_DiMu()) || (dilepid == 143 && HLT_MuE()) ||
                 (HLT_SingleEl() && (abs(lep1_pdgid())==11 || abs(lep2_pdgid())==11)) ||
                 (HLT_SingleMu() && (abs(lep1_pdgid())==13 || abs(lep2_pdgid())==13)) );
      case 2017:
      case 2018:
      default:
        return ( HLT_MET_MHT() || (dilepid == 121 && HLT_DiEl()) || (dilepid == 169 && HLT_DiMu()) || (dilepid == 143 && HLT_MuE()) ||
                 (HLT_SingleEl() && (abs(lep1_pdgid())==11 || abs(lep2_pdgid())==11)) ||
                 (HLT_SingleMu() && (abs(lep1_pdgid())==13 || abs(lep2_pdgid())==13)) );
    }
  }
  return false;
}

void WHLooper::looper(TChain* chain, string samplestr, string output_dir, int jes_type) {

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

  TString output_name = Form("%s/%s.root",output_dir.c_str(),samplestr.c_str());
  cout << "[WHLooper::looper] creating output file: " << output_name << endl;  //outfile_ = new TFile(output_name.Data(),"RECREATE") ;
  cout << "Complied with C++ standard: " << __cplusplus << endl;

  if (printPassedEvents) ofile.open("passEventList.txt");


//  outfile_ = new TFile(output_name.Data(), "RECREATE") ;

  float kLumi = 133.53;
  // Combined 2016 (35.922/fb), 2017 (41.529/fb) and 2018 (56.077/fb) json,
  const char* json_file = "../StopCORE/inputs/json_files/Cert_271036-325175_13TeV_Combined161718_JSON_snt.txt";

  // Determine datayear from the sample name
  if (samplestr == "data_single_lepton_met") datayear = 16;
  else if (samplestr.find("data_2016") == 0) datayear = 2016;
  else if (samplestr.find("data_2017") == 0) datayear = 2017;
  else if (samplestr.find("data_2018") == 0) datayear = 2018;
  else datayear = -1;

  // Setup pileup re-weighting for comparing data of different years
  if (doNvtxReweight) {
    TFile f_purw("../AnalyzeScripts/pu_reweighting_hists/nvtx_reweighting_alldata.root");
    TString scaletype = "18to17";
    if (datayear == 2018) scaletype = "18to17";
    else if (datayear == 2016) scaletype = "16to17";
    TH1F* h_nvtxscale = (TH1F*) f_purw.Get("h_nvtxscale_"+scaletype);
    if (!h_nvtxscale) throw invalid_argument("No nvtx reweighting hist found for " + scaletype);
    if (verbose) cout << "Doing nvtx reweighting! Scaling " << scaletype << ". The scale factors are:" << endl;
    for (int i = 1; i < 100; ++i) {
      nvtxscale_[i] = h_nvtxscale->GetBinContent(i);
      if (verbose) cout << i << "  " << nvtxscale_[i] << endl;
    }
  }

  if (applyGoodRunList) {
    cout << "Loading json file: " << json_file << endl;
    set_goodrun_file(json_file);
  }

 // TFile dummy( (output_dir+"/dummy.root").c_str(), "RECREATE" );



  int nDuplicates = 0;
  int nEvents = chain->GetEntries();
  unsigned int nEventsChain = nEvents;
  cout << "[WHLooper::looper] running on " << nEventsChain << " events" << endl;
  unsigned int nEventsTotal = 0;
  unsigned int nPassedTotal = 0;

  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;
  while ( (currentFile = (TFile*)fileIter.Next()) ) {

    TString fname = currentFile->GetTitle();
    TFile file( fname, "READ" );
    TTree *tree = (TTree*) file.Get("t");

    TString outname = ((TObjString*)fname.Tokenize("/")->Last())->String().Data();
    outname.ReplaceAll(".root","_wh.root");
    outname = output_dir+"/"+outname;
    outfile_ = new TFile(outname, "RECREATE") ;
    TTree *extraTree = new TTree("t_wh","t_wh");


    //Weight info
    float weight=0;
    float w_lumi=0;
    float w_lumi_scale1fbs=0;
    float w_lumi_getWeight=0;
    float w_noBtagSF=0;

    TBranch * b_weight = extraTree->Branch("weight",&weight,"weight/F");
    TBranch * b_w_lumi = extraTree->Branch("w_lumi",&w_lumi,"w_lumi/F");
    TBranch * b_w_lumi_scale1fbs = extraTree->Branch("w_lumi_scale1fbs",&w_lumi_scale1fbs,"w_lumi_scale1fbs/F");
    TBranch * b_w_lumi_getWeight = extraTree->Branch("w_lumi_getWeight",&w_lumi_getWeight,"w_lumi_getWeight/F");
    TBranch * b_w_noBtagSF = extraTree->Branch("w_noBtagSF",&w_noBtagSF,"w_noBtagSF/F");

    extraTree->SetBranchAddress("weight",&weight,&b_weight);
    extraTree->SetBranchAddress("w_lumi",&w_lumi,&b_w_lumi);
    extraTree->SetBranchAddress("w_lumi_scale1fbs",&w_lumi_scale1fbs,&b_w_lumi_scale1fbs);
    extraTree->SetBranchAddress("w_lumi_getWeight",&w_lumi_getWeight,&b_w_lumi_getWeight);
    extraTree->SetBranchAddress("w_noBtagSF",&w_noBtagSF,&b_w_noBtagSF);
    
    //Jet info (flatten LorentzVectors) 

    //To add: mass
    int njets=0;
    int nbtagCSV=0;
    vector<float> * v_ak4pt = new vector<float>();
    vector<float> * v_ak4eta = new vector<float>();
    vector<float> * v_ak4phi = new vector<float>();
    vector<float> * v_ak4mass = new vector<float>();
  
    TBranch * b_njets = extraTree->Branch("njets",&njets,"njets/I");
    TBranch * b_nbtagCSV = extraTree->Branch("nbtagCSV",&nbtagCSV,"nbtagCSV/I");
    TBranch * b_ak4pt = extraTree->Branch("ak4pfjets_pt",&v_ak4pt);
    TBranch * b_ak4eta = extraTree->Branch("ak4pfjets_eta",&v_ak4eta);
    TBranch * b_ak4phi = extraTree->Branch("ak4pfjets_phi",&v_ak4phi);
    TBranch * b_ak4mass = extraTree->Branch("ak4pfjets_m",&v_ak4mass);
    
    extraTree->SetBranchAddress("njets",&njets,&b_njets);
    extraTree->SetBranchAddress("nbtagCSV",&nbtagCSV,&b_nbtagCSV);
    extraTree->SetBranchAddress("ak4pfjets_pt",&v_ak4pt,&b_ak4pt);
    extraTree->SetBranchAddress("ak4pfjets_eta",&v_ak4eta,&b_ak4eta);
    extraTree->SetBranchAddress("ak4pfjets_phi",&v_ak4phi,&b_ak4phi);
    extraTree->SetBranchAddress("ak4pfjets_m",&v_ak4mass,&b_ak4mass);

    int nfatjets=0;
    vector<float> * v_ak8pt = new vector<float>();
    vector<float> * v_ak8eta = new vector<float>();
    vector<float> * v_ak8phi = new vector<float>();
    vector<float> * v_ak8mass = new vector<float>();
    
    TBranch * b_nfatjets = extraTree->Branch("nfatjets",&njets,"nfatjets/I");
    TBranch * b_ak8pt = extraTree->Branch("ak8pfjets_pt",&v_ak8pt);
    TBranch * b_ak8eta = extraTree->Branch("ak8pfjets_eta",&v_ak8eta);
    TBranch * b_ak8phi = extraTree->Branch("ak8pfjets_phi",&v_ak8phi);
    TBranch * b_ak8mass = extraTree->Branch("ak8pfjets_m",&v_ak8mass);
    
    extraTree->SetBranchAddress("nfatjets",&nfatjets,&b_nfatjets);
    extraTree->SetBranchAddress("ak8pfjets_pt",&v_ak8pt,&b_ak8pt);
    extraTree->SetBranchAddress("ak8pfjets_eta",&v_ak8eta,&b_ak8eta);
    extraTree->SetBranchAddress("ak8pfjets_phi",&v_ak8phi,&b_ak8phi);
    extraTree->SetBranchAddress("ak8pfjets_m",&v_ak8mass,&b_ak8mass);

    //Lepton info
    int nleps_tm=0;
    vector<float> * leps_pt = new vector<float>(); 
    vector<float> * leps_eta = new vector<float>();
    vector<float> * leps_phi = new vector<float>();
    vector<int> * leps_fromW = new vector<int>();
    vector<int> * leps_pdgid = new vector<int>();
    TBranch * b_nleps_tm = extraTree->Branch("nleps_tm",&nleps_tm,"nleps_tm/I");
    TBranch * b_leps_pt = extraTree->Branch("leps_pt",&leps_pt);
    TBranch * b_leps_eta = extraTree->Branch("leps_eta",&leps_eta);
    TBranch * b_leps_phi = extraTree->Branch("leps_phi",&leps_phi);
    TBranch * b_leps_fromW = extraTree->Branch("leps_fromW",&leps_fromW);
    TBranch * b_leps_pdgid = extraTree->Branch("leps_pdgid",&leps_pdgid);
    extraTree->SetBranchAddress("nleps_tm",&nleps_tm,&b_nleps_tm);
    extraTree->SetBranchAddress("leps_pt",&leps_pt,&b_leps_pt);
    extraTree->SetBranchAddress("leps_eta",&leps_eta,&b_leps_eta);
    extraTree->SetBranchAddress("leps_phi",&leps_phi,&b_leps_phi);
    extraTree->SetBranchAddress("leps_fromW",&leps_fromW,&b_leps_fromW);
    extraTree->SetBranchAddress("leps_pdgid",&leps_pdgid,&b_leps_pdgid);

    int nels_tm=0;
    vector<float> * els_pt = new vector<float>(); 
    vector<float> * els_eta = new vector<float>();
    vector<float> * els_phi = new vector<float>();
    vector<int> * els_fromW = new vector<int>();
    vector<int> * els_pdgid = new vector<int>();
    TBranch * b_nels_tm = extraTree->Branch("nels_tm",&nels_tm,"nels_tm/I");
    TBranch * b_els_pt = extraTree->Branch("els_pt",&els_pt);
    TBranch * b_els_eta = extraTree->Branch("els_eta",&els_eta);
    TBranch * b_els_phi = extraTree->Branch("els_phi",&els_phi);
    TBranch * b_els_fromW = extraTree->Branch("els_fromW",&els_fromW);
    TBranch * b_els_pdgid = extraTree->Branch("els_pdgid",&els_pdgid);
    extraTree->SetBranchAddress("nels_tm",&nels_tm,&b_nels_tm);
    extraTree->SetBranchAddress("els_pt",&els_pt,&b_els_pt);
    extraTree->SetBranchAddress("els_eta",&els_eta,&b_els_eta);
    extraTree->SetBranchAddress("els_phi",&els_phi,&b_els_phi);
    extraTree->SetBranchAddress("els_fromW",&els_fromW,&b_els_fromW);
    extraTree->SetBranchAddress("els_pdgid",&els_pdgid,&b_els_pdgid);

    int nmus_tm=0;
    vector<float> * mus_pt = new vector<float>(); 
    vector<float> * mus_eta = new vector<float>();
    vector<float> * mus_phi = new vector<float>();
    vector<int> * mus_fromW = new vector<int>();
    vector<int> * mus_pdgid = new vector<int>();
    TBranch * b_nmus_tm = extraTree->Branch("nmus_tm",&nmus_tm,"nmus_tm/I");
    TBranch * b_mus_pt = extraTree->Branch("mus_pt",&mus_pt);
    TBranch * b_mus_eta = extraTree->Branch("mus_eta",&mus_eta);
    TBranch * b_mus_phi = extraTree->Branch("mus_phi",&mus_phi);
    TBranch * b_mus_fromW = extraTree->Branch("mus_fromW",&mus_fromW);
    TBranch * b_mus_pdgid = extraTree->Branch("mus_pdgid",&mus_pdgid);
    extraTree->SetBranchAddress("nmus_tm",&nmus_tm,&b_nmus_tm);
    extraTree->SetBranchAddress("mus_pt",&mus_pt,&b_mus_pt);
    extraTree->SetBranchAddress("mus_eta",&mus_eta,&b_mus_eta);
    extraTree->SetBranchAddress("mus_phi",&mus_phi,&b_mus_phi);
    extraTree->SetBranchAddress("mus_fromW",&mus_fromW,&b_mus_fromW);
    extraTree->SetBranchAddress("mus_pdgid",&mus_pdgid,&b_mus_pdgid);

    /// other kinematic variables ///
    float mbb=0; //sorted by CSV
    float ptbb=0;
    float mct=0;
    TBranch * b_mbb = extraTree->Branch("mbb",&mbb,"mbb/F");
    TBranch * b_ptbb = extraTree->Branch("ptbb",&ptbb,"ptbb/F");
    TBranch * b_mct = extraTree->Branch("mct",&mct,"mct/F");
    extraTree->SetBranchAddress("mbb",&mbb,&b_mbb);
    extraTree->SetBranchAddress("ptbb",&ptbb,&b_ptbb);
    extraTree->SetBranchAddress("mct",&mct,&b_mct);


    TTreeCache::SetLearnEntries(10);
    tree->SetCacheSize(128*1024*1024);
    babyAnalyzer.Init(tree);

    // Use the first event to get dsname
    tree->LoadTree(0);
    babyAnalyzer.GetEntry(0);

    // Setup configs for sample dependent processes
    //year_ = (doTopTagging)? year() : 2016;
    
    //Hack for broken v25 sample
    year_=2016;
    TString dsname = dataset();

    // Find the stopbaby versions automatically from file path
    if (int i = fname.Index("_v"); i >= 0) samplever = fname(i+1, 3); // ignore subversions
    else if (fname.Contains("v24")) samplever = "v24";
    else cout << "[looper] >> Cannot find the sample version!" << endl;

    // Attach the MiniAOD version from dsname
    if (dsname.Contains("RunIIFall17MiniAODv2")) samplever += ":Fall17v2";
    else if (dsname.Contains("RunIISummer16MiniAODv2")) samplever += ":Summer16v2";
    else if (dsname.Contains("RunIISummer16MiniAODv3")) samplever += ":Summer16v3";
    else if (dsname.Contains("RunIISpring16MiniAODv2")) samplever += ":Spring16v2";

    cout << "[looper] >> Running on sample: " << dsname << endl;
    cout << "[looper] >> Sample detected with year = " << year_ << " and version = " << samplever << endl;

    is_fastsim_ = dsname.Contains("SMS") || fname.Contains("SMS") || fname.Contains("Signal");
    //if(dsname.Contains("TChi")) is_fastsim_=false;

    // Get event weight histogram from baby
    TH3D* h_sig_counter = nullptr;
    TH2D* h_sig_counter_nEvents = nullptr;
    if (is_fastsim_) {
      h_sig_counter = (TH3D*) file.Get("h_counterSMS");
      h_sig_counter_nEvents = (TH2D*) file.Get("histNEvts");
    }

    TH1D *h_bkg_counter = (TH1D*) file.Get("h_counter");

    // Setup the event weight calculator
    if (year_ == 2016)
      evtWgt.setDefaultSystematics(0);  // systematic set for Moriond17 analysis
    else if (year_ >= 2017)
      evtWgt.setDefaultSystematics(1);  // systematic set for 94X
    evtWgt.Setup(samplestr, year_, doSystVariations, applyBtagSFfromFiles, applyLeptonSFfromFiles);

    evtWgt.getCounterHistogramFromBaby(&file);
    // Extra file weight for extension dataset, should move these code to other places
    if (year_ == 2016 && samplever.find("v22") == 0)
      evtWgt.getExtSampleWeightSummer16v2(fname);

    evtWgt.getZSampleWeightFromCR3l(fname);

    if (year_ == 2016) kLumi = 35.867;
    else if (year_ == 2017) kLumi = 41.96;
    else if (year_ == 2018) kLumi = 70;

    //dummy.cd();
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
        if ( applyGoodRunList && !goodrun(run(), ls()) ) continue;
        duplicate_removal::DorkyEventIdentifier id(run(), evt(), ls());
        if ( is_duplicate(id) ) {
          ++nDuplicates;
          continue;
        }
      }

      int nEvts;
      if ( is_fastsim_ ) {
        nEvts = (int) h_sig_counter_nEvents->GetBinContent(h_sig_counter->FindBin(mass_stop(), mass_lsp()));
      } else {
        nEvts = h_bkg_counter->GetBinContent(22);
      }  

      /// Get and fill Weight info
      evtWgt.resetEvent();
      if (year_ == 2016)
        evtWgt.setDefaultSystematics(0);  // systematic set for Moriond17 analysis
      else if (year_ >= 2017)
        evtWgt.setDefaultSystematics(1);  // systematic set for 94X
      weight = evtWgt.getWeight(evtWgtInfo::systID(0), false);
      
      w_lumi = kLumi * xsec() * 1000 / nEvts;
      w_lumi_scale1fbs = evtweight_ = kLumi * scale1fb();

      evtWgt.resetEvent();
      evtWgt.setDefaultSystematics(2);
      w_noBtagSF = evtWgt.getWeight(evtWgtInfo::systID(0), false);

      evtWgt.resetEvent();
      evtWgt.setDefaultSystematics(3);
      w_lumi_getWeight = evtWgt.getWeight(evtWgtInfo::systID(0), false);


      //Get and fill jet info
      v_ak4pt->clear();
      v_ak4eta->clear();
      v_ak4phi->clear();
      v_ak4mass->clear();
      for(uint ijet=0;ijet<ak4pfjets_p4().size();ijet++){
        v_ak4pt->push_back(ak4pfjets_p4().at(ijet).pt());
       // cout<<"jet pt"<<ak4pfjets_p4().at(ijet).pt()<<endl;
        v_ak4eta->push_back(ak4pfjets_p4().at(ijet).eta());
        v_ak4phi->push_back(ak4pfjets_p4().at(ijet).phi());
        v_ak4mass->push_back(ak4pfjets_p4().at(ijet).mass());
      }
      njets=v_ak4pt->size();


      // nbtag for CSV valued btags -- for comparison with previous version
      int nbtagCSV = 0;
      for (float csv : ak4pfjets_CSV()) {
        if (year_ == 2016) {
          if (csv > 0.8484) nbtagCSV++;  // 80X Moriond17
        } else if (year_ == 2017) {
          if (csv > 0.8838) nbtagCSV++;  // 94X
        }
      }

      v_ak8pt->clear();
      v_ak8eta->clear();
      v_ak8phi->clear();
      v_ak8mass->clear();
      for(uint ijet=0;ijet<ak8pfjets_p4().size();ijet++){
        v_ak8pt->push_back(ak8pfjets_p4().at(ijet).pt());
        v_ak8eta->push_back(ak8pfjets_p4().at(ijet).eta());
        v_ak8phi->push_back(ak8pfjets_p4().at(ijet).phi());
        v_ak8mass->push_back(ak8pfjets_p4().at(ijet).mass());
      }
      nfatjets=v_ak8pt->size();
      nleps_tm=0;
      leps_pt->clear();
      leps_eta->clear();
      leps_phi->clear();
      leps_pdgid->clear();
      leps_fromW->clear();
      nels_tm=0;
      els_pt->clear();
      els_eta->clear();
      els_phi->clear();
      els_pdgid->clear();
      els_fromW->clear();
      nmus_tm=0;
      mus_pt->clear();
      mus_eta->clear();
      mus_phi->clear();
      mus_pdgid->clear();
      mus_fromW->clear();

      //// Lepton Info ////
      if(ngoodleps()>0){
        leps_pt->push_back(lep1_p4().pt());
        leps_phi->push_back(lep1_p4().phi());
        leps_eta->push_back(lep1_p4().eta());
        leps_pdgid->push_back(lep1_pdgid());
        if(lep1_production_type() == 0){
          leps_fromW->push_back(1);
          nleps_tm++;
        }
        else
          leps_fromW->push_back(0);

        if(abs(lep1_pdgid())==11){
          els_pt->push_back(lep1_p4().pt());
          els_phi->push_back(lep1_p4().phi());
          els_eta->push_back(lep1_p4().eta());
          els_pdgid->push_back(lep1_pdgid());
          if(lep1_production_type() == 0){
            els_fromW->push_back(1);
            nels_tm++;
          }
          else
            els_fromW->push_back(0);
        }

        else if(abs(lep1_pdgid())==13){
          mus_pt->push_back(lep1_p4().pt());
          mus_phi->push_back(lep1_p4().phi());
          mus_eta->push_back(lep1_p4().eta());
          mus_pdgid->push_back(lep1_pdgid());
          if(lep1_production_type() == 0){
            mus_fromW->push_back(1);
            nmus_tm++;
          }
          else
            mus_fromW->push_back(0);
        }

      }
      if(ngoodleps()>1){
        leps_pt->push_back(lep2_p4().pt());
        leps_phi->push_back(lep2_p4().phi());
        leps_eta->push_back(lep2_p4().eta());
        leps_pdgid->push_back(lep2_pdgid());
        if(lep2_production_type() == 0){
          leps_fromW->push_back(1);
          nleps_tm++;
        }
        else
          leps_fromW->push_back(0);

        if(abs(lep2_pdgid())==11){
          els_pt->push_back(lep2_p4().pt());
          els_phi->push_back(lep2_p4().phi());
          els_eta->push_back(lep2_p4().eta());
          els_pdgid->push_back(lep2_pdgid());
          if(lep2_production_type() == 0){
            els_fromW->push_back(1);
            nels_tm++;
          }
          else
            els_fromW->push_back(0);
        }

        else if(abs(lep2_pdgid())==13){
          mus_pt->push_back(lep2_p4().pt());
          mus_phi->push_back(lep2_p4().phi());
          mus_eta->push_back(lep2_p4().eta());
          mus_pdgid->push_back(lep2_pdgid());
          if(lep2_production_type() == 0){
            mus_fromW->push_back(1);
            nmus_tm++;
          }
          else
            mus_fromW->push_back(0);
        }
      }

      ///Kinematic variables
      // Use CSV sorted for mbb
      vector <pair<int, float>> jet_csv_pairs;
      //Only 30 GeV jets passing ID, inside eta 2.4 should survive to babies.
      for(uint ijet=0;ijet<ak4pfjets_deepCSVb().size();ijet++){
        float btagvalue = ak4pfjets_deepCSVb().at(ijet) + ak4pfjets_deepCSVbb().at(ijet);
        jet_csv_pairs.push_back(make_pair(ijet,btagvalue));
      }
      sort( jet_csv_pairs.begin(), jet_csv_pairs.end(), sortIndexbyCSV);

      mbb=-999;
      mct=-999;
      ptbb=-999;
      if(ngoodjets()>1){
        mbb = (ak4pfjets_p4().at(jet_csv_pairs.at(0).first) + ak4pfjets_p4().at(jet_csv_pairs.at(1).first)).mass()  ; 
        ptbb = (ak4pfjets_p4().at(jet_csv_pairs.at(0).first) + ak4pfjets_p4().at(jet_csv_pairs.at(1).first)).pt()  ; 
        
        float ptb1,ptb2,dPhibb;
        ptb1 = ak4pfjets_p4().at(jet_csv_pairs.at(0).first).pt();
        ptb2 = ak4pfjets_p4().at(jet_csv_pairs.at(1).first).pt();
        dPhibb = getdphi(ak4pfjets_p4().at(jet_csv_pairs.at(0).first).phi(),ak4pfjets_p4().at(jet_csv_pairs.at(1).first).phi());
        mct = sqrt(2*ptb1*ptb2*(1+cos(dPhibb)));  
       
      }

      extraTree->Fill();
      // fillEfficiencyHistos(testVec[0], "filters");

      // Apply met filters
      if (doTopTagging) {
        // Recommended filters for the legacy analysis
        switch (year_) {
          case 2018:  // 2017 and 2018 uses the same set of filters
          case 2017:
            if ( !filt_ecalbadcalib() ) continue;
          case 2016:
            if ( !filt_goodvtx() ) continue;
            if ( !is_fastsim_ && !filt_globalsupertighthalo2016() ) continue;
            if ( !filt_hbhenoise() ) continue;
            if ( !filt_hbheisonoise() )   continue;
            if ( !filt_ecaltp() ) continue;
            if ( !filt_badMuonFilter() ) continue;
            if ( !filt_badChargedCandidateFilter() ) continue;
            if ( is_data() && !filt_eebadsc() ) continue;
        }
      } else if (samplever.find("v24") == 0) {
        // Filters used in Moriond17 study, keep for sync check
        if ( !filt_met() ) continue;
        if ( !filt_goodvtx() ) continue;
        if ( firstGoodVtxIdx() == -1 ) continue;
        if ( !filt_badMuonFilter() ) continue;
        if ( !filt_badChargedCandidateFilter() ) continue;
        if ( !filt_jetWithBadMuon() ) continue;
        if ( !filt_pfovercalomet() ) continue;
        if ( filt_duplicatemuons() ) continue; // Temporary, breaks for old 16 babies
        if ( filt_badmuons() ) continue;
        if ( !filt_nobadmuons() ) continue;
      }

      // stop defined filters
      if (is_fastsim_) {
        if ( !filt_fastsimjets() ) continue;
      } else {
        if ( !filt_pfovercalomet() ) continue;  // reject event if pfmet/calomet > 5
        if ( !filt_jetWithBadMuon() ) continue; // there's a jup/jdown of this filter, but this alone should be enough
      }

      // Require at least 1 good vertex
      if (nvtxs() < 1) continue;

      // For testing on only subset of mass points
      if (!runFullSignalScan && is_fastsim_) {
        // auto checkMassPt = [&](double mstop, double mlsp) { return (mass_stop() == mstop) && (mass_lsp() == mlsp); };
        // if (!checkMassPt(800,400)  && !checkMassPt(1200,50)  && !checkMassPt(400,100) &&
        //     !checkMassPt(1100,300) && !checkMassPt(1100,500) && !checkMassPt(900,600)) continue;
        if (dsname.Contains("T2tt")) {
          float massdiff = mass_stop() - mass_lsp();
          // if (mass_lsp() < 400 && mass_stop() < 1000) continue;
          // if (massdiff < 200 || massdiff > 400) continue;
          // if (massdiff < 200 || massdiff > 400 || mass_lsp() < 400) continue;
          if (massdiff < 600 || mass_stop() < 1000) continue;
         // plot2d("h2d_T2tt_masspts", mass_stop(), mass_lsp(), 1, SRVec.at(0).histMap, ";M_{stop} [GeV]; M_{lsp} [GeV]", 100, 300, 1300, 80, 0, 800);
        } else if (dsname.Contains("T2bW")) {
          float massdiff = mass_stop() - mass_lsp();
          if (mass_lsp() < 300 && mass_stop() < 800) continue;
          if (massdiff < 300) continue;
          // if (massdiff < 900 || mass_stop() < 1000) continue;
          //plot2d("h2d_T2bW_masspts", mass_stop(), mass_lsp(), 1, SRVec.at(0).histMap, ";M_{stop} [GeV]; M_{lsp} [GeV]", 100, 300, 1300, 80, 0, 800);
        } else if (dsname.Contains("T2bt")) {
          float massdiff = mass_stop() - mass_lsp();
          if (mass_lsp() < 300 && mass_stop() < 800) continue;
          if (massdiff < 300) continue;
          // if (massdiff < 900 || mass_stop() < 1000) continue;
          // if (!checkMassPt(800, 400) && !checkMassPt(800, 600) && !checkMassPt(1000, 50) && !checkMassPt(1000, 200)) continue;
         // plot2d("h2d_T2bt_masspts", mass_stop(), mass_lsp(), 1, SRVec.at(0).histMap, ";M_{stop} [GeV]; M_{LSP} [GeV]", 100, 300, 1300, 64, 0, 800);
        }
      }

      // Only events nupt < 200 for WNJetsToLNu samples -- 80X only since NuPt-200 sample not available in 94X
      //if (dsname.BeginsWith("/W") && dsname.Contains("JetsToLNu") && dsname.Contains("80X") && !dsname.Contains("NuPt-200") && nupt() > 200) continue;

      if (is_fastsim_) {
        if (fmod(mass_stop(), kSMSMassStep) > 2 || fmod(mass_lsp(), kSMSMassStep) > 2) continue;  // skip points in between the binning
        //plot2d("h2d_signal_masspts", mass_stop(), mass_lsp() , evtweight_, SRVec.at(0).histMap, ";M_{stop} [GeV]; M_{lsp} [GeV]", 96, 100, 1300, 64, 0, 800);
      }

      ++nPassedTotal;

      is_bkg_ = (!is_data() && !is_fastsim_);

      // Calculate event weight
      //evtWgt.resetEvent(); // full event weights only get calculated if the event get selected for a SR

      // Simple weight with scale1fb only
      if (!is_data()) {
        if (is_fastsim_) {
          int nEventsSample = h_sig_counter_nEvents->GetBinContent(h_sig_counter->FindBin(mass_stop(), mass_lsp()));
          evtweight_ = kLumi * xsec() * 1000 / nEventsSample;
        } else {
          evtweight_ = kLumi * scale1fb();
        }
      }



      if (doNvtxReweight && (datayear == 2016 || datayear == 2018)) {
        if (nvtxs() < 100) evtweight_ = nvtxscale_[nvtxs()];  // only scale for data
      }



      // Calculate leading top tagger variables for the event
      // float lead_restopdisc = -1.1;
      // float lead_tftopdisc = -0.1;
      // float lead_deepdisc_top = -0.1;
      // float lead_bindisc_top = -0.1;
      // if (doTopTagging) {
      //   lead_restopdisc = (topcands_disc().size())? topcands_disc()[0] : -1.1;
      //   for (size_t iak8 = 0; iak8 < ak8pfjets_deepdisc_top().size(); ++iak8) {
      //     float disc = ak8pfjets_deepdisc_top()[iak8];
      //     if (disc > lead_deepdisc_top) lead_deepdisc_top = disc;
      //     float bindisc = disc / (disc + ak8pfjets_deepdisc_qcd().at(iak8));
      //     if (bindisc > lead_bindisc_top) lead_bindisc_top = bindisc;
      //   }
      //   for (auto disc : tftops_disc()) {
      //     if (disc > lead_tftopdisc) lead_tftopdisc = disc;
      //   }
      // }

      // Filling the variables for analysis
      values_.clear();

      // /// Common variables for all JES type
      // values_["nlep"] = ngoodleps();
      // values_["nvlep"] = nvetoleps();
      // values_["lep1pt"] = lep1_p4().pt();
      // values_["passvetos"] = PassTrackVeto() && PassTauVeto();

      // // For toptagging, add correct switch later
      // // values_["resttag"] = lead_restopdisc;
      // values_["resttag"] = lead_tftopdisc;
      // values_["deepttag"] = lead_deepdisc_top;
      // values_["tfttag"] = lead_tftopdisc;
      // values_["bdtttag"] = lead_restopdisc;
      // values_["binttag"] = lead_bindisc_top;

      // if (runResTopMVA) {
      //   // Prepare deep_cvsl vector
      //   vector<float> ak4pfjets_dcvsl;
      //   for (size_t j = 0; j < ak4pfjets_deepCSV().size(); ++j) {
      //     ak4pfjets_dcvsl.push_back(ak4pfjets_deepCSVc().at(j) / (ak4pfjets_deepCSVc().at(j) + ak4pfjets_deepCSVl().at(j)));
      //   }
      //   resTopMVA->setJetVecPtrs(&ak4pfjets_p4(), &ak4pfjets_deepCSV(), &ak4pfjets_dcvsl, &ak4pfjets_ptD(), &ak4pfjets_axis1(), &ak4pfjets_mult());
      //   std::vector<TopCand> topcands = resTopMVA->getTopCandidates(-1);
      //   values_["resttag"] = (topcands.size() > 0)? topcands[0].disc : -1.1;
      // }

      // Temporary test for top tagging efficiency
      // testTopTaggingEffficiency(testVec[1]);

      /// Values only for hist filling or testing
      // values_["chi2"] = hadronic_top_chi2();
      // values_["lep1eta"] = lep1_p4().eta();
      // values_["passlep1pt"] = (abs(lep1_pdgid()) == 13 && lep1_p4().pt() > 40) || (abs(lep1_pdgid()) == 11 && lep1_p4().pt() > 45);

      for (int jestype = 0; jestype < ((doSystVariations && !is_data())? 3 : 1); ++jestype) {
        if (doSystVariations) jestype_ = jestype;
        string suffix = "";

        /// JES type dependent variables
        if (jestype_ == 0) {
          values_["mt"] = mt_met_lep();
          values_["met"] = pfmet();
          values_["mlb"] = Mlb_closestb();
          values_["tmod"] = topnessMod();
          values_["njet"] = ngoodjets();
          values_["nbjet"] = ngoodbtags();
          values_["nbtag"]  = nanalysisbtags();
          values_["dphijmet"] = mindphi_met_j1_j2();
          values_["dphilmet"] = lep1_dphiMET();
          values_["j1passbtag"] = (ngoodjets() > 0)? ak4pfjets_passMEDbtag().at(0) : 0;

          values_["jet1pt"] = (ngoodjets() > 0)? ak4pfjets_p4().at(0).pt() : 0;
          values_["jet2pt"] = (ngoodjets() > 1)? ak4pfjets_p4().at(1).pt() : 0;
          values_["jet1eta"] = (ngoodjets() > 0)? ak4pfjets_p4().at(0).eta() : -9;
          values_["jet2eta"] = (ngoodjets() > 1)? ak4pfjets_p4().at(1).eta() : -9;

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
          values_["mlb"] = Mlb_closestb_jup();
          values_["tmod"] = topnessMod_jup();
          values_["njet"] = jup_ngoodjets();
          values_["nbjet"] = jup_ngoodbtags();  // nbtag30();
          values_["nbtag"]  = jup_nanalysisbtags();
          values_["dphijmet"] = mindphi_met_j1_j2_jup();
          values_["dphilmet"] = fabs(lep1_p4().phi() - pfmet_phi_jup());
          values_["j1passbtag"] = (jup_ngoodjets() > 0)? jup_ak4pfjets_passMEDbtag().at(0) : 0;

          values_["jet1pt"] = (jup_ngoodjets() > 0)? jup_ak4pfjets_p4().at(0).pt() : 0;
          values_["jet2pt"] = (jup_ngoodjets() > 1)? jup_ak4pfjets_p4().at(1).pt() : 0;
          values_["jet1eta"] = (jup_ngoodjets() > 0)? jup_ak4pfjets_p4().at(0).eta() : -9;
          values_["jet2eta"] = (jup_ngoodjets() > 1)? jup_ak4pfjets_p4().at(1).eta() : -9;

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
          values_["mlb"] = Mlb_closestb_jdown();
          values_["tmod"] = topnessMod_jdown();
          values_["njet"] = jdown_ngoodjets();
          values_["nbjet"] = jdown_ngoodbtags();  // nbtag30();
          values_["ntbtag"] = jdown_ntightbtags();
          values_["dphijmet"] = mindphi_met_j1_j2_jdown();
          values_["dphilmet"] = fabs(lep1_p4().phi() - pfmet_phi_jdown());
          values_["j1passbtag"] = (jdown_ngoodjets() > 0)? jdown_ak4pfjets_passMEDbtag().at(0) : 0;

          values_["jet1pt"] = (jdown_ngoodjets() > 0)? jdown_ak4pfjets_p4().at(0).pt() : 0;
          values_["jet2pt"] = (jdown_ngoodjets() > 1)? jdown_ak4pfjets_p4().at(1).pt() : 0;
          values_["jet1eta"] = (jdown_ngoodjets() > 0)? jdown_ak4pfjets_p4().at(0).eta() : -9;
          values_["jet2eta"] = (jdown_ngoodjets() > 1)? jdown_ak4pfjets_p4().at(1).eta() : -9;

          values_["ht"] = jdown_ak4_HT();
          values_["metphi"] = pfmet_phi_jdown();
          values_["nbtag"]  = jdown_nanalysisbtags();
          values_["leadbpt"] = jdown_ak4pfjets_leadbtag_p4().pt();
          values_["mlb_0b"] = (jdown_ak4pfjets_leadbtag_p4() + lep1_p4()).M();
          // values_["htratio"] = jdown_ak4_htratiom();

          suffix = "_jesDn";
        }
        /// should do the same job as nanalysisbtags
        values_["nbtag"] = (values_["mlb"] > 175)? values_["ntbtag"] : values_["nbjet"];

        // // Uncomment following lines if want to use CSV instead
        // values_["nbtag"] = (values_["mlb"] > 175)? ntbtagCSV : nbtagCSV;
        // values_["nbjet"] = nbtagCSV;
        // values_["ntbtag"] = ntbtagCSV;

        // Filling histograms for SR
        // fillHistosForSR(suffix);

        // fillHistosForCR0b(suffix);

        // Filling analysis variables with removed leptons, for CR2l
        values_["nlep_rl"] = (ngoodleps() == 1 && nvetoleps() >= 2 && lep2_p4().Pt() > 10)? 2 : ngoodleps();
        values_["mll"] = (lep1_p4() + lep2_p4()).M();

        if (jestype_ == 0) {
          values_["mt_rl"] = mt_met_lep_rl();
          values_["mt2_ll"] = (doTopTagging)? MT2_ll() : 90;
          values_["met_rl"] = pfmet_rl();
          values_["dphijmet_rl"]= mindphi_met_j1_j2_rl();
          values_["dphilmet_rl"] = lep1_dphiMET_rl();
          values_["tmod_rl"] = topnessMod_rl();
        } else if (jestype_ == 1) {
          values_["mt_rl"] = mt_met_lep_rl_jup();
          values_["mt2_ll"] = (doTopTagging)? MT2_ll_jup() : 90;
          values_["met_rl"] = pfmet_rl_jup();
          values_["dphijmet_rl"]= mindphi_met_j1_j2_rl_jup();
          values_["dphilmet_rl"] = lep1_dphiMET_rl_jup();
          values_["tmod_rl"] = topnessMod_rl_jup();
        } else if (jestype_ == 2) {
          values_["mt_rl"] = mt_met_lep_rl_jdown();
          values_["mt2_ll"] = (doTopTagging)? MT2_ll_jdown() : 90;
          values_["met_rl"] = pfmet_rl_jdown();
          values_["dphijmet_rl"]= mindphi_met_j1_j2_rl_jdown();
          values_["dphilmet_rl"] = lep1_dphiMET_rl_jdown();
          values_["tmod_rl"] = topnessMod_rl_jdown();
        }
        // fillHistosForCR2l(suffix);
        // fillHistosForCRemu(suffix);

        // // testCutFlowHistos(testVec[2]);
        // fillTopTaggingHistos(suffix);

        // // Also do yield using genmet for fastsim samples <-- under development
        // if (is_fastsim_ && jestype_ == 0) {
        //   values_["met"] = genmet();
        //   values_["mt"] = mt_genmet_lep();
        //   values_["tmod"] = topnessMod_genmet();
        //   values_["dphijmet"] = mindphi_genmet_j1_j2();
        //   values_["dphilmet"] = mindphi_genmet_lep1();

        //   fillHistosForSR("_genmet");
        // }

      }  // end of jes variation

     // if (event > 100) break;  // for debugging purpose
    } // end of event loop


    delete tree;
    file.Close();
    outfile_->Write();
    outfile_->Close();
  } // end of file loop

  cout << "[WHLooper::looper] processed  " << nEventsTotal << " events" << endl;
  if ( nEventsChain != nEventsTotal )
    cout << "WARNING: Number of events from files is not equal to total number of events" << endl;

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








