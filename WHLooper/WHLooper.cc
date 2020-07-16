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

// JER
#include "../CORE/Tools/JetResolution/JetResolution.h"

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

const bool verbose = true;
// turn on to apply btag sf - take from files defined in eventWeight_btagSF.cc
const bool applyBtagSFfromFiles = true; // default false
// turn on to apply lepton sf to central value - reread from files
const bool applyLeptonSFfromFiles = false; // default false
// turn on to enable plots of metbins with systematic variations applied. will only do variations for applied weights
const bool doSystVariations = true;
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
// veto 2018 events with an electron land in the HEM region
const bool doHEMElectronVeto = true;
// veto 2018 events with an AK4 jet land in the HEM region
const bool doHEMJetVeto = true;
// only produce yield histos
const bool runYieldsOnly = false;
// only running selected signal points to speed up
const bool runFullSignalScan = false;
// debug symbol, for printing exact event kinematics that passes
const bool printPassedEvents = false;

// some global helper variables to be used in member functions
int datayear = -1;
string samplever;
float babyver;

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

float getdR(float phi1, float phi2, float eta1, float eta2){
  float dR = sqrt( pow(getdphi(phi1,phi2),2) + pow((eta1-eta2),2));
  return dR;
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

  float kLumi = 133.53;//Overwritten later.
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
  float nEventsNegativeWeight = chain->GetEntries("scale1fb<0");
  if (nEventsNegativeWeight / nEventsChain < 0.01){
    evtWgt.use_hcounter_instead_of_scale1b = true;
     cout << "[WHLooper::looper] Normalizing weights based on h_counter." << endl;
  }
  else{
    evtWgt.use_hcounter_instead_of_scale1b = false;
    cout << "[WHLooper::looper] Fraction of negative weights: " <<nEventsNegativeWeight / nEventsChain<<", relying on scale1fb to normalize weights."<< endl;
  }


  cout << "[WHLooper::looper] running on " << nEventsChain << " events" << endl;
  unsigned int nEventsTotal = 0;


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
    bool pass=true;
    bool pass_old=true;
    bool stitch=true;
    bool isgoodrun=true;
    bool duplicate=false;

    float weight=0;
    float full_weight=0;
    float w_lumi=0;
    float w_lumi_scale1fb=0;
    float w_noBtagSF=0;
    float w_BtagSF_medmed=0;
    float w_BtagSF_medloose=0;
    float w_BtagSF_tightloose=0;
    float w_80X=0;
    float w_pu=0;
    float w_L1=0;
    float w_ISRnjets=0;

    //Trig eff
    float trig_eff =1.0;

    //Systematic weight variations
    float w_btagHFUp=0;
    float w_btagHFDown=0;
    float w_btagLFUp=0;
    float w_btagLFDown=0;
    float w_btagFSUp=0;
    float w_btagFSDown=0;
    float w_lepSFUp=0;
    float w_lepSFDown=0;
    float w_lepFSSFUp=0;
    float w_lepFSSFDown=0;
    float w_topPtSFUp=0;
    float w_topPtSFDown=0;
    // float w_ttbarSysPtUp=0; //this is stop 1l ad hoc weight
    // float w_ttbarSysPtDown=0;
    float w_pdfUp=0;
    float w_pdfDown=0;
    float w_alphasUp=0;
    float w_alphasDown=0;
    float w_q2Up=0;
    float w_q2Down=0;
    float w_ISRUp=0; //this is SUSY prescription for 2016 fullsim ttbar, and njets reweighting for strong susy production. for EWK signal, probably easier to use namedFunc.
    float w_ISRDown=0;
    float w_xsecUp=0;
    float w_xsecDown=0;
    float w_puUp=0;
    float w_puDown=0;
    float w_tauSFUp=0;
    float w_tauSFDown=0;
    float w_L1prefireUp=0;
    float w_L1prefireDown=0;

    TBranch * b_pass = extraTree->Branch("pass",&pass,"pass/O");
    TBranch * b_pass_old = extraTree->Branch("pass_old",&pass_old,"pass_old/O");
    TBranch * b_stitch = extraTree->Branch("stitch",&stitch,"stitch/O");
    TBranch * b_isgoodrun = extraTree->Branch("goodrun",&isgoodrun,"isgoodrun/O");
    TBranch * b_duplicate = extraTree->Branch("duplicate",&duplicate,"duplicate/O");

    TBranch * b_weight = extraTree->Branch("weight",&weight,"weight/F");
    TBranch * b_full_weight = extraTree->Branch("full_weight",&full_weight,"full_weight/F");
    TBranch * b_w_lumi = extraTree->Branch("w_lumi",&w_lumi,"w_lumi/F");
    TBranch * b_w_lumi_scale1fb = extraTree->Branch("w_lumi_scale1fb",&w_lumi_scale1fb,"w_lumi_scale1fb/F");
    TBranch * b_w_noBtagSF = extraTree->Branch("w_noBtagSF",&w_noBtagSF,"w_noBtagSF/F");
    TBranch * b_w_BtagSF_medmed = extraTree->Branch("w_BtagSF_medmed",&w_BtagSF_medmed,"w_BtagSF_medmed/F");
    TBranch * b_w_BtagSF_medloose = extraTree->Branch("w_BtagSF_medloose",&w_BtagSF_medloose,"w_BtagSF_medloose/F");
    TBranch * b_w_BtagSF_tightloose = extraTree->Branch("w_BtagSF_tightloose",&w_BtagSF_tightloose,"w_BtagSF_tightloose/F");
    TBranch * b_w_80X = extraTree->Branch("w_80X",&w_80X,"w_80X/F");
    TBranch * b_w_pu = extraTree->Branch("w_pu",&w_pu,"w_pu/F");
    TBranch * b_w_L1 = extraTree->Branch("w_L1",&w_L1,"w_L1/F");
    TBranch * b_w_ISRnjets = extraTree->Branch("w_ISRnjets",&w_ISRnjets,"w_ISRnjets/F");

    TBranch * b_trig_eff = extraTree->Branch("trig_eff",&trig_eff,"trig_eff/F");


    //Systematic weight variations
    TBranch * b_w_btagHFUp = extraTree->Branch("w_btagHFUp",&w_btagHFUp,"w_btagHFUp/F");
    TBranch * b_w_btagHFDown = extraTree->Branch("w_btagHFDown",&w_btagHFDown,"w_btagHFDown/F");
    TBranch * b_w_btagLFUp = extraTree->Branch("w_btagLFUp",&w_btagLFUp,"w_btagLFUp/F");
    TBranch * b_w_btagLFDown = extraTree->Branch("w_btagLFDown",&w_btagLFDown,"w_btagLFDown/F");
    TBranch * b_w_btagFSUp = extraTree->Branch("w_btagFSUp",&w_btagFSUp,"w_btagFSUp/F");
    TBranch * b_w_btagFSDown = extraTree->Branch("w_btagFSDown",&w_btagFSDown,"w_btagFSDown/F");
    TBranch * b_w_lepSFUp = extraTree->Branch("w_lepSFUp",&w_lepSFUp,"w_lepSFUp/F");
    TBranch * b_w_lepSFDown = extraTree->Branch("w_lepSFDown",&w_lepSFDown,"w_lepSFDown/F");
    TBranch * b_w_lepFSSFUp = extraTree->Branch("w_lepFSSFUp",&w_lepFSSFUp,"w_lepFSSFUp/F");
    TBranch * b_w_lepFSSFDown = extraTree->Branch("w_lepFSSFDown",&w_lepFSSFDown,"w_lepFSSFDown/F");
    TBranch * b_w_topPtSFUp = extraTree->Branch("w_topPtSFUp",&w_topPtSFUp,"w_topPtSFUp/F");
    TBranch * b_w_topPtSFDown = extraTree->Branch("w_topPtSFDown",&w_topPtSFDown,"w_topPtSFDown/F");
    TBranch * b_w_pdfUp = extraTree->Branch("w_pdfUp",&w_pdfUp,"w_pdfUp/F");
    TBranch * b_w_pdfDown = extraTree->Branch("w_pdfDown",&w_pdfDown,"w_pdfDown/F");
    TBranch * b_w_alphasUp = extraTree->Branch("w_alphasUp",&w_alphasUp,"w_alphasUp/F");
    TBranch * b_w_alphasDown = extraTree->Branch("w_alphasDown",&w_alphasDown,"w_alphasDown/F");
    TBranch * b_w_q2Up = extraTree->Branch("w_q2Up",&w_q2Up,"w_q2Up/F");
    TBranch * b_w_q2Down = extraTree->Branch("w_q2Down",&w_q2Down,"w_q2Down/F");
    TBranch * b_w_ISRUp = extraTree->Branch("w_ISRUp",&w_ISRUp,"w_ISRUp/F");
    TBranch * b_w_ISRDown = extraTree->Branch("w_ISRDown",&w_ISRDown,"w_ISRDown/F");
    TBranch * b_w_xsecUp = extraTree->Branch("w_xsecUp",&w_xsecUp,"w_xsecUp/F");
    TBranch * b_w_xsecDown = extraTree->Branch("w_xsecDown",&w_xsecDown,"w_xsecDown/F");
    TBranch * b_w_puUp = extraTree->Branch("w_puUp",&w_puUp,"w_puUp/F");
    TBranch * b_w_puDown = extraTree->Branch("w_puDown",&w_puDown,"w_puDown/F");
    TBranch * b_w_tauSFUp = extraTree->Branch("w_tauSFUp",&w_tauSFUp,"w_tauSFUp/F");
    TBranch * b_w_tauSFDown = extraTree->Branch("w_tauSFDown",&w_tauSFDown,"w_tauSFDown/F");
    TBranch * b_w_L1prefireUp = extraTree->Branch("w_L1prefireUp",&w_L1prefireUp,"w_L1prefireUp/F");
    TBranch * b_w_L1prefireDown = extraTree->Branch("w_L1prefireDown",&w_L1prefireDown,"w_L1prefireDown/F");

    extraTree->SetBranchAddress("pass",&pass,&b_pass);
    extraTree->SetBranchAddress("pass_old",&pass_old,&b_pass_old);
    extraTree->SetBranchAddress("stitch",&stitch,&b_stitch);
    extraTree->SetBranchAddress("goodrun",&isgoodrun,&b_isgoodrun);
    extraTree->SetBranchAddress("duplicate",&duplicate,&b_duplicate);

    extraTree->SetBranchAddress("weight",&weight,&b_weight);
    extraTree->SetBranchAddress("full_weight",&full_weight,&b_full_weight);
    extraTree->SetBranchAddress("w_lumi",&w_lumi,&b_w_lumi);
    extraTree->SetBranchAddress("w_lumi_scale1fb",&w_lumi_scale1fb,&b_w_lumi_scale1fb);
    extraTree->SetBranchAddress("w_noBtagSF",&w_noBtagSF,&b_w_noBtagSF);
    extraTree->SetBranchAddress("w_BtagSF_medmed",&w_BtagSF_medmed,&b_w_BtagSF_medmed);
    extraTree->SetBranchAddress("w_BtagSF_medloose",&w_BtagSF_medloose,&b_w_BtagSF_medloose);
    extraTree->SetBranchAddress("w_BtagSF_tightloose",&w_BtagSF_tightloose,&b_w_BtagSF_tightloose);
    extraTree->SetBranchAddress("w_80X",&w_80X,&b_w_80X);
    extraTree->SetBranchAddress("w_pu",&w_pu,&b_w_pu);
    extraTree->SetBranchAddress("w_L1",&w_L1,&b_w_L1);
    extraTree->SetBranchAddress("w_ISRnjets",&w_ISRnjets,&b_w_ISRnjets);

    extraTree->SetBranchAddress("trig_eff",&trig_eff,&b_trig_eff);

    //Systematic weight variations
    extraTree->SetBranchAddress("w_btagHFUp",&w_btagHFUp,&b_w_btagHFUp);
    extraTree->SetBranchAddress("w_btagHFDown",&w_btagHFDown,&b_w_btagHFDown);
    extraTree->SetBranchAddress("w_btagLFUp",&w_btagLFUp,&b_w_btagLFUp);
    extraTree->SetBranchAddress("w_btagLFDown",&w_btagLFDown,&b_w_btagLFDown);
    extraTree->SetBranchAddress("w_btagFSUp",&w_btagFSUp,&b_w_btagFSUp);
    extraTree->SetBranchAddress("w_btagFSDown",&w_btagFSDown,&b_w_btagFSDown);
    extraTree->SetBranchAddress("w_lepSFUp",&w_lepSFUp,&b_w_lepSFUp);
    extraTree->SetBranchAddress("w_lepSFDown",&w_lepSFDown,&b_w_lepSFDown);
    extraTree->SetBranchAddress("w_lepFSSFUp",&w_lepFSSFUp,&b_w_lepFSSFUp);
    extraTree->SetBranchAddress("w_lepFSSFDown",&w_lepFSSFDown,&b_w_lepFSSFDown);
    extraTree->SetBranchAddress("w_topPtSFUp",&w_topPtSFUp,&b_w_topPtSFUp);
    extraTree->SetBranchAddress("w_topPtSFDown",&w_topPtSFDown,&b_w_topPtSFDown);
    extraTree->SetBranchAddress("w_pdfUp",&w_pdfUp,&b_w_pdfUp);
    extraTree->SetBranchAddress("w_pdfDown",&w_pdfDown,&b_w_pdfDown);
    extraTree->SetBranchAddress("w_alphasUp",&w_alphasUp,&b_w_alphasUp);
    extraTree->SetBranchAddress("w_alphasDown",&w_alphasDown,&b_w_alphasDown);
    extraTree->SetBranchAddress("w_q2Up",&w_q2Up,&b_w_q2Up);
    extraTree->SetBranchAddress("w_q2Down",&w_q2Down,&b_w_q2Down);
    extraTree->SetBranchAddress("w_ISRUp",&w_ISRUp,&b_w_ISRUp);
    extraTree->SetBranchAddress("w_ISRDown",&w_ISRDown,&b_w_ISRDown);
    extraTree->SetBranchAddress("w_xsecUp",&w_xsecUp,&b_w_xsecUp);
    extraTree->SetBranchAddress("w_xsecDown",&w_xsecDown,&b_w_xsecDown);
    extraTree->SetBranchAddress("w_puUp",&w_puUp,&b_w_puUp);
    extraTree->SetBranchAddress("w_puDown",&w_puDown,&b_w_puDown);
    extraTree->SetBranchAddress("w_tauSFUp",&w_tauSFUp,&b_w_tauSFUp);
    extraTree->SetBranchAddress("w_tauSFDown",&w_tauSFDown,&b_w_tauSFDown);
    extraTree->SetBranchAddress("w_L1prefireUp",&w_L1prefireUp,&b_w_L1prefireUp);
    extraTree->SetBranchAddress("w_L1prefireDown",&w_L1prefireDown,&b_w_L1prefireDown);

    
    //Jet info (flatten LorentzVectors) 

    //To add: mass
    int njets=0;
    int nbtagCSV=0;
    vector<float> * v_ak4pt = new vector<float>();
    vector<float> * v_ak4pt_jup = new vector<float>();
    vector<float> * v_ak4pt_jdown = new vector<float>();
    vector<float> * v_ak4eta = new vector<float>();
    vector<float> * v_ak4phi = new vector<float>();
    vector<float> * v_ak4mass = new vector<float>();
    vector<float> * v_ak4genpt = new vector<float>();
  
    TBranch * b_njets = extraTree->Branch("njets",&njets,"njets/I");
    TBranch * b_nbtagCSV = extraTree->Branch("nbtagCSV",&nbtagCSV,"nbtagCSV/I");
    TBranch * b_ak4pt = extraTree->Branch("ak4pfjets_pt",&v_ak4pt);
    TBranch * b_ak4pt_jup = extraTree->Branch("ak4pfjets_pt_jup",&v_ak4pt_jup);
    TBranch * b_ak4pt_jdown = extraTree->Branch("ak4pfjets_pt_jdown",&v_ak4pt_jdown);
    TBranch * b_ak4eta = extraTree->Branch("ak4pfjets_eta",&v_ak4eta);
    TBranch * b_ak4phi = extraTree->Branch("ak4pfjets_phi",&v_ak4phi);
    TBranch * b_ak4mass = extraTree->Branch("ak4pfjets_m",&v_ak4mass);
    TBranch * b_ak4genpt = extraTree->Branch("ak4pfjets_genpt",&v_ak4mass);
    
    extraTree->SetBranchAddress("njets",&njets,&b_njets);
    extraTree->SetBranchAddress("nbtagCSV",&nbtagCSV,&b_nbtagCSV);
    extraTree->SetBranchAddress("ak4pfjets_pt",&v_ak4pt,&b_ak4pt);
    extraTree->SetBranchAddress("ak4pfjets_pt_jup",&v_ak4pt_jup,&b_ak4pt_jup);
    extraTree->SetBranchAddress("ak4pfjets_pt_jdown",&v_ak4pt_jdown,&b_ak4pt_jdown);
    extraTree->SetBranchAddress("ak4pfjets_eta",&v_ak4eta,&b_ak4eta);
    extraTree->SetBranchAddress("ak4pfjets_phi",&v_ak4phi,&b_ak4phi);
    extraTree->SetBranchAddress("ak4pfjets_m",&v_ak4mass,&b_ak4mass);
    extraTree->SetBranchAddress("ak4pfjets_genpt",&v_ak4genpt,&b_ak4genpt);

    int nfatjets=0;
    vector<float> * v_ak8pt = new vector<float>();
    vector<float> * v_ak8pt_jup = new vector<float>();
    vector<float> * v_ak8pt_jdown = new vector<float>();
    vector<float> * v_ak8eta = new vector<float>();
    vector<float> * v_ak8phi = new vector<float>();
    vector<float> * v_ak8mass = new vector<float>();
    // vector<float> * v_ak8genpt = new vector<float>();
    
    TBranch * b_nfatjets = extraTree->Branch("nfatjets",&njets,"nfatjets/I");
    TBranch * b_ak8pt = extraTree->Branch("ak8pfjets_pt",&v_ak8pt);
    TBranch * b_ak8pt_jup = extraTree->Branch("ak8pfjets_pt_jup",&v_ak8pt_jup);
    TBranch * b_ak8pt_jdown = extraTree->Branch("ak8pfjets_pt_jdown",&v_ak8pt_jdown);
    TBranch * b_ak8eta = extraTree->Branch("ak8pfjets_eta",&v_ak8eta);
    TBranch * b_ak8phi = extraTree->Branch("ak8pfjets_phi",&v_ak8phi);
    TBranch * b_ak8mass = extraTree->Branch("ak8pfjets_m",&v_ak8mass);
    // TBranch * b_ak8genpt = extraTree->Branch("ak8pfjets_genpt",&v_ak8genpt);
    
    extraTree->SetBranchAddress("nfatjets",&nfatjets,&b_nfatjets);
    extraTree->SetBranchAddress("ak8pfjets_pt",&v_ak8pt,&b_ak8pt);
    extraTree->SetBranchAddress("ak8pfjets_pt_jup",&v_ak8pt_jup,&b_ak8pt_jup);
    extraTree->SetBranchAddress("ak8pfjets_pt_jdown",&v_ak8pt_jdown,&b_ak8pt_jdown);
    extraTree->SetBranchAddress("ak8pfjets_eta",&v_ak8eta,&b_ak8eta);
    extraTree->SetBranchAddress("ak8pfjets_phi",&v_ak8phi,&b_ak8phi);
    extraTree->SetBranchAddress("ak8pfjets_m",&v_ak8mass,&b_ak8mass);
    // extraTree->SetBranchAddress("ak8pfjets_genpt",&v_ak8genpt,&b_ak8genpt);

    //Lepton info
    int nWHLeptons=0;
    int nleps_tm=0;
    vector<float> * leps_pt = new vector<float>(); 
    vector<float> * leps_eta = new vector<float>();
    vector<float> * leps_phi = new vector<float>();
    vector<int> * leps_fromW = new vector<int>();
    vector<int> * leps_pdgid = new vector<int>();
    vector<float> * vetoleps_pt = new vector<float>(); 
    vector<float> * vetoleps_eta = new vector<float>();
    vector<float> * vetoleps_phi = new vector<float>();
    vector<int> * vetoleps_pdgid = new vector<int>();
    TBranch * b_nWHLeptons = extraTree->Branch("nWHLeptons",&nWHLeptons,"nWHLeptons/I");
    TBranch * b_nleps_tm = extraTree->Branch("nleps_tm",&nleps_tm,"nleps_tm/I");
    TBranch * b_leps_pt = extraTree->Branch("leps_pt",&leps_pt);
    TBranch * b_leps_eta = extraTree->Branch("leps_eta",&leps_eta);
    TBranch * b_leps_phi = extraTree->Branch("leps_phi",&leps_phi);
    TBranch * b_leps_fromW = extraTree->Branch("leps_fromW",&leps_fromW);
    TBranch * b_leps_pdgid = extraTree->Branch("leps_pdgid",&leps_pdgid);
    TBranch * b_vetoleps_pt  = extraTree->Branch("vetoleps_pt",&leps_pt);
    TBranch * b_vetoleps_eta = extraTree->Branch("vetoleps_eta",&leps_eta);
    TBranch * b_vetoleps_phi = extraTree->Branch("vetoleps_phi",&leps_phi);
    TBranch * b_vetoleps_pdgid = extraTree->Branch("vetoleps_pdgid",&vetoleps_pdgid);
    extraTree->SetBranchAddress("nWHLeptons",&nWHLeptons,&b_nWHLeptons);
    extraTree->SetBranchAddress("nleps_tm",&nleps_tm,&b_nleps_tm);
    extraTree->SetBranchAddress("leps_pt",&leps_pt,&b_leps_pt);
    extraTree->SetBranchAddress("leps_eta",&leps_eta,&b_leps_eta);
    extraTree->SetBranchAddress("leps_phi",&leps_phi,&b_leps_phi);
    extraTree->SetBranchAddress("leps_fromW",&leps_fromW,&b_leps_fromW);
    extraTree->SetBranchAddress("leps_pdgid",&leps_pdgid,&b_leps_pdgid);
    extraTree->SetBranchAddress("vetoleps_pt", &vetoleps_pt, &b_vetoleps_pt);
    extraTree->SetBranchAddress("vetoleps_eta",&vetoleps_eta,&b_vetoleps_eta);
    extraTree->SetBranchAddress("vetoleps_phi",&vetoleps_phi,&b_vetoleps_phi);
    extraTree->SetBranchAddress("vetoleps_pdgid",&vetoleps_pdgid,&b_vetoleps_pdgid);

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
    
    /// other kinematic variables /// central, up, down
    float mbb=0; //sorted by CSV
    float ptbb=0;
    float mct=0;
    TBranch * b_mbb = extraTree->Branch("mbb",&mbb,"mbb/F");
    TBranch * b_ptbb = extraTree->Branch("ptbb",&ptbb,"ptbb/F");
    TBranch * b_mct = extraTree->Branch("mct",&mct,"mct/F");
    extraTree->SetBranchAddress("mbb",&mbb,&b_mbb);
    extraTree->SetBranchAddress("ptbb",&ptbb,&b_ptbb);
    extraTree->SetBranchAddress("mct",&mct,&b_mct);
    float jer_mbb  = 0;
    float jer_ptbb = 0;
    float jer_mct  = 0;
    TBranch * jer_b_mbb  = extraTree->Branch("jer_mbb" , &jer_mbb ,"jer_mbb/F" );
    TBranch * jer_b_ptbb = extraTree->Branch("jer_ptbb", &jer_ptbb,"jer_ptbb/F");   
    TBranch * jer_b_mct  = extraTree->Branch("jer_mct" , &jer_mct ,"jer_mct/F" );
    extraTree->SetBranchAddress("jer_mbb" , &jer_mbb , &jer_b_mbb );
    extraTree->SetBranchAddress("jer_ptbb", &jer_ptbb, &jer_b_ptbb);
    extraTree->SetBranchAddress("jer_mct" , &jer_mct , &jer_b_mct );
    float jerup_mbb  = 0;
    float jerup_ptbb = 0;
    float jerup_mct  = 0;
    TBranch * jerup_b_mbb  = extraTree->Branch("jerup_mbb" , &jerup_mbb ,"jerup_mbb/F" );
    TBranch * jerup_b_ptbb = extraTree->Branch("jerup_ptbb", &jerup_ptbb,"jerup_ptbb/F");   
    TBranch * jerup_b_mct  = extraTree->Branch("jerup_mct" , &jerup_mct ,"jerup_mct/F" );
    extraTree->SetBranchAddress("jerup_mbb" , &jerup_mbb , &jerup_b_mbb );
    extraTree->SetBranchAddress("jerup_ptbb", &jerup_ptbb, &jerup_b_ptbb);
    extraTree->SetBranchAddress("jerup_mct" , &jerup_mct , &jerup_b_mct );

    float jerdown_mbb  = 0;
    float jerdown_ptbb = 0;
    float jerdown_mct  = 0;
    TBranch * jerdown_b_mbb  = extraTree->Branch("jerdown_mbb" , &jerdown_mbb ,"jerdown_mbb/F" );
    TBranch * jerdown_b_ptbb = extraTree->Branch("jerdown_ptbb", &jerdown_ptbb,"jerdown_ptbb/F");   
    TBranch * jerdown_b_mct  = extraTree->Branch("jerdown_mct" , &jerdown_mct ,"jerdown_mct/F" );
    extraTree->SetBranchAddress("jerdown_mbb" , &jerdown_mbb , &jerdown_b_mbb );
    extraTree->SetBranchAddress("jerdown_ptbb", &jerdown_ptbb, &jerdown_b_ptbb );
    extraTree->SetBranchAddress("jerdown_mct" , &jerdown_mct , &jerdown_b_mct );

    float jercdown_mbb  = 0;
    float jercdown_ptbb = 0;
    float jercdown_mct  = 0;
    TBranch * jercdown_b_mbb  = extraTree->Branch("jercdown_mbb" , &jercdown_mbb ,"jercdown_mbb/F" );
    TBranch * jercdown_b_ptbb = extraTree->Branch("jercdown_ptbb", &jercdown_ptbb,"jercdown_ptbb/F");   
    TBranch * jercdown_b_mct  = extraTree->Branch("jercdown_mct" , &jercdown_mct ,"jercdown_mct/F" );
    extraTree->SetBranchAddress("jercdown_mbb" , &jercdown_mbb , &jercdown_b_mbb );
    extraTree->SetBranchAddress("jercdown_ptbb", &jercdown_ptbb, &jercdown_b_ptbb);
    extraTree->SetBranchAddress("jercdown_mct" , &jercdown_mct , &jercdown_b_mct );

    float jup_mbb  = 0;
    float jup_ptbb = 0;
    float jup_mct  = 0;
    TBranch * jup_b_mbb  = extraTree->Branch("jup_mbb" , &jup_mbb ,"jup_mbb/F" );
    TBranch * jup_b_ptbb = extraTree->Branch("jup_ptbb", &jup_ptbb,"jup_ptbb/F");   
    TBranch * jup_b_mct  = extraTree->Branch("jup_mct" , &jup_mct ,"jup_mct/F" );
    extraTree->SetBranchAddress("jup_mbb" , &jup_mbb , &jup_b_mbb );
    extraTree->SetBranchAddress("jup_ptbb", &jup_ptbb, &jup_b_ptbb);
    extraTree->SetBranchAddress("jup_mct" , &jup_mct , &jup_b_mct );
    float jdown_mbb  = 0;
    float jdown_ptbb = 0;
    float jdown_mct  = 0;
    TBranch * jdown_b_mbb  = extraTree->Branch("jdown_mbb" , &jdown_mbb ,"jdown_mbb/F" );
    TBranch * jdown_b_ptbb = extraTree->Branch("jdown_ptbb", &jdown_ptbb,"jdown_ptbb/F");   
    TBranch * jdown_b_mct  = extraTree->Branch("jdown_mct" , &jdown_mct ,"jdown_mct/F" );
    extraTree->SetBranchAddress("jdown_mbb" , &jdown_mbb , &jdown_b_mbb );
    extraTree->SetBranchAddress("jdown_ptbb", &jdown_ptbb, &jdown_b_ptbb);
    extraTree->SetBranchAddress("jdown_mct" , &jdown_mct , &jdown_b_mct );
        

    //Generator information
    vector<float> * gen_pt = new vector<float>(); 
    vector<float> * gen_eta = new vector<float>(); 
    vector<float> * gen_phi = new vector<float>(); 
    vector<float> * gen_m = new vector<float>(); 
    vector<int> * gen_id = new vector<int>(); 
    vector<int> * gen_idx = new vector<int>(); 
    vector<int> * gen_motherid = new vector<int>(); 
    vector<int> * gen_motheridx = new vector<int>(); 
    vector<int> * gen_gmotherid = new vector<int>(); 
    vector<int> * gen_gmotheridx = new vector<int>(); 

    TBranch * b_gen_pt = extraTree->Branch("gen_pt",&gen_pt);
    TBranch * b_gen_eta = extraTree->Branch("gen_eta",&gen_eta);
    TBranch * b_gen_phi = extraTree->Branch("gen_phi",&gen_phi);
    TBranch * b_gen_m = extraTree->Branch("gen_m",&gen_m);
    TBranch * b_gen_id = extraTree->Branch("gen_id",&gen_id);
    TBranch * b_gen_idx = extraTree->Branch("gen_idx",&gen_idx);
    TBranch * b_gen_motherid = extraTree->Branch("gen_motherid",&gen_motherid);
    TBranch * b_gen_motheridx = extraTree->Branch("gen_motheridx",&gen_motheridx);
    TBranch * b_gen_gmotherid = extraTree->Branch("gen_gmotherid",&gen_gmotherid);
    TBranch * b_gen_gmotheridx = extraTree->Branch("gen_gmotheridx",&gen_gmotheridx);

    extraTree->SetBranchAddress("gen_pt",&gen_pt,&b_gen_pt);
    extraTree->SetBranchAddress("gen_eta",&gen_eta,&b_gen_eta);
    extraTree->SetBranchAddress("gen_phi",&gen_phi,&b_gen_phi);
    extraTree->SetBranchAddress("gen_m",&gen_m,&b_gen_m);
    extraTree->SetBranchAddress("gen_id",&gen_id,&b_gen_id);
    extraTree->SetBranchAddress("gen_idx",&gen_idx,&b_gen_idx);
    extraTree->SetBranchAddress("gen_motherid",&gen_motherid,&b_gen_motherid);
    extraTree->SetBranchAddress("gen_motheridx",&gen_motheridx,&b_gen_motheridx);
    extraTree->SetBranchAddress("gen_gmotherid",&gen_gmotherid,&b_gen_gmotherid);
    extraTree->SetBranchAddress("gen_gmotheridx",&gen_gmotheridx,&b_gen_gmotheridx);

    TTreeCache::SetLearnEntries(10);
    tree->SetCacheSize(128*1024*1024);
    babyAnalyzer.Init(tree);

    // Use the first event to get dsname
    tree->LoadTree(0);
    babyAnalyzer.GetEntry(0);

    // Setup configs for sample dependent processes
    //year_ = (doTopTagging)? year() : 2016;
    
    year_= year();// 2016;
    TString dsname = dataset();

    // Temporary setup to use 2017 MC in place for 2018
    if (output_dir.find("samp18") != string::npos) year_ = 2018;

    // Find the stopbaby versions automatically from file path
    if (int i = fname.Index("_v"); i >= 0) samplever = fname(i+1, 3); // ignore subversions
    else if (fname.Contains("v24")) samplever = "v24";
    else cout << "[looper] >> Cannot find the sample version!" << endl;

 // Attach the MiniAOD version from dsname
    if      (dsname.Contains("RunIIAutumn18MiniAOD"))   samplever += ":Autumn18v1";
    else if (dsname.Contains("RunIIFall17MiniAODv2"))   samplever += ":Fall17v2";
    else if (dsname.Contains("RunIISummer16MiniAODv2")) samplever += ":Summer16v2";
    else if (dsname.Contains("RunIISummer16MiniAODv3")) samplever += ":Summer16v3";
    else if (dsname.Contains("RunIISpring16MiniAODv2")) samplever += ":Spring16v2";

    cout << "[looper] >> Running on sample: " << dsname << endl;
    cout << "[looper] >> Sample detected with year = " << year_ << " and version = " << samplever << endl;

    is_fastsim_ = dsname.Contains("SMS") || fname.Contains("SMS") || fname.Contains("Signal");
    if(dsname.Contains("mChargino")) is_fastsim_=false; //This is a fullsim point
    babyver = TString(samplever).ReplaceAll("v","").ReplaceAll("_",".").Atof();

    cout<<"WHLooper: is_fastsim_: "<<is_fastsim_<<endl;
    // Get event weight histogram from baby
    TH3D* h_sig_counter = nullptr;
    TH2D* h_sig_counter_nEvents = nullptr;
    if (is_fastsim_) {
      h_sig_counter = (TH3D*) file.Get("h_counterSMS");
      h_sig_counter_nEvents = (TH2D*) file.Get("histNEvts");
    }

    TH1D *h_bkg_counter = (TH1D*) file.Get("h_counter");

    // Setup the event weight calculator
    evtWgt.verbose = true;
    evtWgt.susy_xsec_fromfile = true;
    evtWgt.setDefaultSystematics(evtWgtInfo::stop_Run2, is_fastsim_);

    // evtWgt.apply_L1prefire_sf = false;
    evtWgt.apply_HEMveto_el_sf = doHEMElectronVeto;
    evtWgt.apply_HEMveto_jet_sf = doHEMJetVeto;

    evtWgt.Setup(samplestr, year_, doSystVariations, applyBtagSFfromFiles, applyLeptonSFfromFiles, true, dsname);

    evtWgt.getCounterHistogramFromBaby(&file);

    // Extra file weight for extension dataset, should move these code to other places
    if (year_ == 2016 && samplever.find("v22") == 0)
      evtWgt.getExtSampleWeightSummer16v2(fname);
    else if (year_ == 2016 && samplever.find("Summer16v3") != string::npos)
      evtWgt.getExtSampleWeightSummer16v3(fname);
    else if (year_ == 2017)
      evtWgt.getExtSampleWeightFall17v2(fname);

    evtWgt.getWNJetsNuPt200SampleWeight(fname);

    evtWgt.getZSampleWeightFromCR3l(fname);

    // if (year_ == 2016) kLumi = 35.867;
    // else if (year_ == 2017) kLumi = 41.96;
    // else if (year_ == 2018) kLumi = 70;

    // JER
    JetResolution res;
    if (year_ == 2016){
        std::cout << "Loading 2016 JER files" << std::endl;
        res.loadResolutionFile("../CORE/Tools/JetResolution/data/Summer16_25nsV1_MC_PtResolution_AK4PFchs.txt");
        res.loadScaleFactorFile("../CORE/Tools/JetResolution/data/Summer16_25nsV1_MC_SF_AK4PFchs.txt");
    }
    else if (year_ == 2017){
        std::cout << "Loading 2017 JER files" << std::endl;
        res.loadResolutionFile("../CORE/Tools/JetResolution/data/Fall17_V3_MC_PtResolution_AK4PFchs.txt");
        res.loadScaleFactorFile("../CORE/Tools/JetResolution/data/Fall17_V3_MC_SF_AK4PFchs.txt");
    }
    else if (year_ == 2018){
        std::cout << "Loading 2018 JER files" << std::endl;
        res.loadResolutionFile("../CORE/Tools/JetResolution/data/Autumn18_V6_MC_PtResolution_AK4PFchs.txt");
        res.loadScaleFactorFile("../CORE/Tools/JetResolution/data/Autumn18_V6_MC_SF_AK4PFchs.txt");
    }
    

    //WH framework: normalize weights always to 1 fb-1
    kLumi=1.0;

    //dummy.cd();
    // Loop over Events in current file

    if (nEventsTotal >= nEventsChain) continue;
    unsigned int nEventsTree = tree->GetEntriesFast();

     // nEventsTree = 2000;
    //cout<<"apply good run list "<<applyGoodRunList<<endl;
    for (unsigned int event = 0; event < nEventsTree; ++event) {
      // Read Tree
      if(event%100000==0) cout<<"Event "<<event<<endl;
      if (nEventsTotal >= nEventsChain) continue;
      tree->LoadTree(event);
      babyAnalyzer.GetEntry(event);
      ++nEventsTotal;

      isgoodrun=true;
      duplicate=false;
      if ( is_data() ) {
        // cout<<"run, ls"<<run()<<" "<<ls()<<endl;
        // cout<<"On good run? "<< goodrun(run(), ls())<<endl;
        if ( applyGoodRunList && !goodrun(run(), ls()) ) isgoodrun=false;//continue;
        duplicate_removal::DorkyEventIdentifier id(run(), evt(), ls());
        if ( is_duplicate(id) ) {
          ++nDuplicates;
          duplicate=true;
         // continue;
        }
      }

      int nEvts;
      if ( is_fastsim_ ) {
        nEvts = (int) h_sig_counter_nEvents->GetBinContent(h_sig_counter->FindBin(mass_stop(), mass_lsp()));
      } else {
        nEvts = h_bkg_counter->GetBinContent(22);
      }  
 
      //Various event weights
      evtWgt.resetEvent();
      evtWgt.setDefaultSystematics(evtWgtInfo::stop_Moriond17, is_fastsim_);
      w_80X = evtWgt.getWeight(evtWgtInfo::systID(0), false);

      evtWgt.resetEvent();
      evtWgt.setDefaultSystematics(evtWgtInfo::WH_Run2, is_fastsim_);
      weight = evtWgt.getWeight(evtWgtInfo::systID(0), false);
      
      evtWgt.resetEvent();
      evtWgt.setDefaultSystematics(evtWgtInfo::WH_Run2_nobtag, is_fastsim_);
      w_noBtagSF = evtWgt.getWeight(evtWgtInfo::systID(0), false);
      
      evtWgt.resetEvent();
      evtWgt.setDefaultSystematics(evtWgtInfo::puOnly, is_fastsim_);
      w_pu = evtWgt.getWeight(evtWgtInfo::systID(0), false);
      w_puUp = evtWgt.getWeight(evtWgtInfo::systID::k_puUp,false);
      w_puDown = evtWgt.getWeight(evtWgtInfo::systID::k_puDown,false);

      evtWgt.resetEvent();
      evtWgt.setDefaultSystematics(evtWgtInfo::L1Only, is_fastsim_);
      w_L1 = evtWgt.getWeight(evtWgtInfo::systID(0), false);
      
      evtWgt.resetEvent();
      evtWgt.setDefaultSystematics(evtWgtInfo::ISROnly, is_fastsim_);
      w_ISRnjets = evtWgt.getWeight(evtWgtInfo::systID(0), false);
      w_ISRUp = evtWgt.getWeight(evtWgtInfo::systID::k_ISRUp,false); 
      w_ISRDown = evtWgt.getWeight(evtWgtInfo::systID::k_ISRDown,false);

      evtWgt.resetEvent();
      evtWgt.setDefaultSystematics(evtWgtInfo::WH_Run2, is_fastsim_);
      evtWgt.setbTagSF_only();
      w_BtagSF_medmed = evtWgt.getWeight(evtWgtInfo::systID(0), false);

      evtWgt.resetEvent();
      evtWgt.setbTagSF_medloose();
      w_BtagSF_medloose = evtWgt.getWeight(evtWgtInfo::systID(0), false);

      evtWgt.resetEvent();
      evtWgt.setbTagSF_tightloose();
      w_BtagSF_tightloose = evtWgt.getWeight(evtWgtInfo::systID(0), false);

      w_lumi = kLumi * xsec() * 1000 / nEvts; //This uses Stop1l Baby xsec, not actual xsec for TChiWH
      w_lumi_scale1fb = evtweight_ = kLumi * scale1fb();
      if(event==0){
        cout<<"h_counter and scale1b weights: "<<w_lumi<<", "<<w_lumi_scale1fb<<endl;
        if( abs(w_lumi - w_lumi_scale1fb)/w_lumi_scale1fb > 0.15 ) cout<<"WARNING: incompatible lumi weights."<<endl;
      }


      //year weight for 3-year plotting simultaneously
      float year_weight=1.;
      float totalLumi = 35.9+41.6+59.7;
      if(!is_data()){
        if(year() == 2016 ) year_weight = 35.9 / totalLumi;
        else if(year() == 2017)  year_weight = 41.6 / totalLumi;
        else if(year() == 2018)  year_weight = 59.7 / totalLumi;
      }

      //Full weight
      full_weight = weight;
      if(!is_data()) full_weight = weight * w_pu * w_ISRnjets * year_weight; //ISR weight only != 1 for 2016 ttbar.

      //systematic weight variations:
      evtWgt.resetEvent();
      evtWgt.setDefaultSystematics(evtWgtInfo::WH_Run2, is_fastsim_);
      float trig_eff_up,trig_eff_dn;
      if(!is_data()) evtWgt.getWH_trig_eff_fromFile(trig_eff,trig_eff_up,trig_eff_dn);
      else {trig_eff=1.;trig_eff_up=1.;trig_eff_dn=1.;}
      w_btagHFUp = evtWgt.getWeight(evtWgtInfo::systID::k_bTagEffHFUp,false);
      w_btagHFDown = evtWgt.getWeight(evtWgtInfo::systID::k_bTagEffHFDown,false);
      w_btagLFUp = evtWgt.getWeight(evtWgtInfo::systID::k_bTagEffLFUp,false);
      w_btagLFDown = evtWgt.getWeight(evtWgtInfo::systID::k_bTagEffLFDown,false);
      w_btagFSUp = evtWgt.getWeight(evtWgtInfo::systID::k_bTagFSEffUp,false);
      w_btagFSDown = evtWgt.getWeight(evtWgtInfo::systID::k_bTagFSEffDown,false);
      w_lepSFUp = evtWgt.getWeight(evtWgtInfo::systID::k_lepSFUp,false);
      w_lepSFDown = evtWgt.getWeight(evtWgtInfo::systID::k_lepSFDown,false);
      w_lepFSSFUp = evtWgt.getWeight(evtWgtInfo::systID::k_lepFSSFUp,false);
      w_lepFSSFDown = evtWgt.getWeight(evtWgtInfo::systID::k_lepFSSFDown,false);
      w_topPtSFUp = evtWgt.getWeight(evtWgtInfo::systID::k_topPtSFUp,false);
      w_topPtSFDown = evtWgt.getWeight(evtWgtInfo::systID::k_topPtSFDown,false);
      w_pdfUp = evtWgt.getWeight(evtWgtInfo::systID::k_pdfUp,false);
      w_pdfDown = evtWgt.getWeight(evtWgtInfo::systID::k_pdfDown,false);
      w_alphasUp = evtWgt.getWeight(evtWgtInfo::systID::k_alphasUp,false);
      w_alphasDown = evtWgt.getWeight(evtWgtInfo::systID::k_alphasDown,false);
      w_q2Up = evtWgt.getWeight(evtWgtInfo::systID::k_q2Up,false);
      w_q2Down = evtWgt.getWeight(evtWgtInfo::systID::k_q2Down,false);
     
      w_xsecUp = evtWgt.getWeight(evtWgtInfo::systID::k_xsecUp,false);
      w_xsecDown = evtWgt.getWeight(evtWgtInfo::systID::k_xsecDown,false);
      w_tauSFUp = evtWgt.getWeight(evtWgtInfo::systID::k_tauSFUp,false);
      w_tauSFDown = evtWgt.getWeight(evtWgtInfo::systID::k_tauSFDown,false);
      w_L1prefireUp = evtWgt.getWeight(evtWgtInfo::systID::k_L1prefireUp,false);
      w_L1prefireDown = evtWgt.getWeight(evtWgtInfo::systID::k_L1prefireDown,false);

      //Get and fill jet info
      v_ak4pt->clear();
      v_ak4pt_jdown->clear();
      v_ak4pt_jup->clear();
      v_ak4eta->clear();
      v_ak4phi->clear();
      v_ak4mass->clear();
      v_ak4genpt->clear();

      for(uint ijet=0;ijet<ak4pfjets_p4().size();ijet++){
     
        v_ak4pt->push_back(ak4pfjets_p4().at(ijet).pt());
        if(ijet < jup_ak4pfjets_p4().size()) v_ak4pt_jup->push_back(jup_ak4pfjets_p4().at(ijet).pt());
        if(ijet < jdown_ak4pfjets_p4().size()) v_ak4pt_jdown->push_back(jdown_ak4pfjets_p4().at(ijet).pt());
       // cout<<"jet pt"<<ak4pfjets_p4().at(ijet).pt()<<endl;
        v_ak4eta->push_back(ak4pfjets_p4().at(ijet).eta());
        v_ak4phi->push_back(ak4pfjets_p4().at(ijet).phi());
        v_ak4mass->push_back(ak4pfjets_p4().at(ijet).mass());
        float genpT =0;
        if(!is_data()){
          float dR = 999;
          int imatch = -1;
          for(uint igjet=0;igjet<ak4genjets_p4().size();igjet++){
            float this_dR = getdR(ak4pfjets_p4().at(ijet).phi(),ak4genjets_p4().at(igjet).phi(),ak4pfjets_p4().at(ijet).eta(),ak4genjets_p4().at(igjet).eta());
            if (this_dR < dR) {imatch = igjet; dR = this_dR;}
          }
          if(imatch>=0 && dR < 0.3) genpT = ak4genjets_p4().at(imatch).pt();
        }
        v_ak4genpt->push_back(genpT);

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
      v_ak8pt_jup->clear();
      v_ak8pt_jdown->clear();
      v_ak8eta->clear();
      v_ak8phi->clear();
      v_ak8mass->clear();
      // v_ak8genpt->clear();

      for(uint ijet=0;ijet<ak8pfjets_p4().size();ijet++){
        v_ak8pt->push_back(ak8pfjets_p4().at(ijet).pt());
        if(ijet < jup_ak8pfjets_p4().size()) v_ak8pt_jup->push_back(jup_ak8pfjets_p4().at(ijet).pt());
        if(ijet < jdown_ak8pfjets_p4().size()) v_ak8pt_jdown->push_back(jdown_ak8pfjets_p4().at(ijet).pt());
        v_ak8eta->push_back(ak8pfjets_p4().at(ijet).eta());
        v_ak8phi->push_back(ak8pfjets_p4().at(ijet).phi());
        v_ak8mass->push_back(ak8pfjets_p4().at(ijet).mass());
        //Stop baby has no ak8 genjets.
        // float genpT =0;
        // if(!is_data()){
        //   float dR = 999;
        //   int imatch = -1;
        //   for(uint igjet=0;igjet<ak8pfjets_p4().size();igjet++){
        //     float this_dR = getdR(ak8pfjets_p4().at(ijet).phi(),ak8genjets_p4().at(igjet).phi(),ak8pfjets_p4().at(ijet).eta(),ak8genjets_p4().at(igjet).eta());
        //     if (this_dR < dR) {imatch = igjet; dR = this_dR;}
        //   }
        //   if(imatch>=0 && dR < 0.3) genpT = ak8genjets_p4().at(imatch).pt();
        // }
        // v_ak8genpt->push_back(genpT);

      }

      nfatjets=v_ak8pt->size();

      nWHLeptons=0;
      nleps_tm=0;
      leps_pt->clear();
      leps_eta->clear();
      leps_phi->clear();
      leps_pdgid->clear();
      leps_fromW->clear();
      vetoleps_pt->clear();
      vetoleps_eta->clear();
      vetoleps_phi->clear();
      vetoleps_pdgid->clear();
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

      gen_pt->clear(); 
      gen_eta->clear(); 
      gen_phi->clear(); 
      gen_m->clear(); 
      gen_id->clear(); 
      gen_idx->clear(); 
      gen_motherid->clear(); 
      gen_motheridx->clear(); 
      gen_gmotherid->clear(); 
      gen_gmotheridx->clear(); 

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
          //this is a subset of goodleps (so POG ID is already applied)
          if(lep1_p4().pt()>30&&lep1_relIso()*lep1_p4().pt()<5&&abs(lep1_p4().eta())<1.4442) nWHLeptons++;

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
          if(lep1_p4().pt()>25&&lep1_relIso()*lep1_p4().pt()<5&&abs(lep1_p4().eta())<2.1) nWHLeptons++;
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
      
      if(nvetoleps()>0){
        vetoleps_pt->push_back(lep1_p4().pt());
        vetoleps_phi->push_back(lep1_p4().phi());
        vetoleps_eta->push_back(lep1_p4().eta());
        vetoleps_pdgid->push_back(lep1_pdgid());
      }
      if(nvetoleps()>1){
        vetoleps_pt->push_back(lep2_p4().pt());
        vetoleps_phi->push_back(lep2_p4().phi());
        vetoleps_eta->push_back(lep2_p4().eta());
        vetoleps_pdgid->push_back(lep2_pdgid());
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
          if(lep2_p4().pt()>30&&lep2_relIso()*lep2_p4().pt()<5&&abs(lep2_p4().eta())<1.4442) nWHLeptons++;
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
          if(lep2_p4().pt()>25&&lep2_relIso()*lep2_p4().pt()<5&&abs(lep2_p4().eta())<2.1) nWHLeptons++;
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
      // JER. use ak4pfjets_rho
      vector <float> jet_JER_factor, jet_JER_factor_up, jet_JER_factor_down;
      vector<Double_t> GenJetPt;
      for (auto& genjet : ak4genjets_p4()) {
        GenJetPt.push_back(genjet.pt());
      }
      for(uint ijet=0;ijet<ak4pfjets_deepCSV().size();ijet++){
        res.loadVariable("JetEta", ak4pfjets_p4().at(ijet).eta());
        res.loadVariable("Rho", ak4pfjets_rho());
        res.loadVariable("JetPt", ak4pfjets_p4().at(ijet).pt());

        auto smearing = res.smear(ak4pfjets_p4().at(ijet), ak4genjets_p4(), GenJetPt, 0);
        auto smearing_up = res.smear(ak4pfjets_p4().at(ijet), ak4genjets_p4(), GenJetPt, 1);
        auto smearing_down = res.smear(ak4pfjets_p4().at(ijet), ak4genjets_p4(), GenJetPt, -1);
        //auto matching = res.match();
        //cout << "   MC resolution:        " << res.getResolution() << endl;
        //cout << "   data/MC scale factor: " << res.getScaleFactor(0) << endl;
        //cout << "   jet eta:              " << ak4pfjets_p4().at(ijet).eta() << endl;
        //cout << "   jet phi:              " << ak4pfjets_p4().at(ijet).phi() << endl;
        //cout << "   is matched:           " << matching[0] << endl;
        //cout << "   original jet pT:      " << ak4pfjets_p4().at(ijet).pt() << endl;
        //cout << "   smeared jet pT:       " << smearing[0] << endl;
        //cout << endl;
        
        jet_JER_factor.push_back(smearing[0]/ak4pfjets_p4().at(ijet).pt());
        jet_JER_factor_up.push_back(smearing_up[0]/ak4pfjets_p4().at(ijet).pt());
        jet_JER_factor_down.push_back(smearing_down[0]/ak4pfjets_p4().at(ijet).pt());
      }
      

      ///Kinematic variables
      // Use CSV sorted for mbb
      // central
      vector <pair<int, float>> jet_csv_pairs;
      //Only 30 GeV jets passing ID, inside eta 2.4 should survive to babies.
      for(uint ijet=0;ijet<ak4pfjets_deepCSV().size();ijet++){
        float btagvalue = ak4pfjets_deepCSV().at(ijet);
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

      // JER + variations
      jer_mbb  = -999;
      jerup_mbb  = -999;
      jerdown_mbb  = -999;
      jercdown_mbb  = -999;
      jer_mct  = -999;
      jerup_mct  = -999;
      jerdown_mct  = -999;
      jercdown_mct  = -999;
      jer_ptbb = -999;
      jerup_ptbb = -999;
      jerdown_ptbb = -999;
      jercdown_ptbb = -999;
      //int jer_ngoodjets = 0;
      if(ngoodjets()>1){
        float jet0_pt, jet1_pt;
        TLorentzVector jet0, jet1;
        float ptb1,ptb2,dPhibb;

        // central
        jet0_pt = ak4pfjets_p4().at(jet_csv_pairs.at(0).first).pt()*jet_JER_factor.at(jet_csv_pairs.at(0).first);
        jet1_pt = ak4pfjets_p4().at(jet_csv_pairs.at(1).first).pt()*jet_JER_factor.at(jet_csv_pairs.at(1).first);
        jet0.SetPtEtaPhiM(jet0_pt, ak4pfjets_p4().at(jet_csv_pairs.at(0).first).eta(), ak4pfjets_p4().at(jet_csv_pairs.at(0).first).phi(), 0);
        jet1.SetPtEtaPhiM(jet1_pt, ak4pfjets_p4().at(jet_csv_pairs.at(1).first).eta(), ak4pfjets_p4().at(jet_csv_pairs.at(1).first).phi(), 0);
        jer_mbb = (jet0 + jet1).M()  ; 
        jer_ptbb = (jet0 + jet1).Pt()  ; 
        
        ptb1 = ak4pfjets_p4().at(jet_csv_pairs.at(0).first).pt()*jet_JER_factor.at(jet_csv_pairs.at(0).first);
        ptb2 = ak4pfjets_p4().at(jet_csv_pairs.at(1).first).pt()*jet_JER_factor.at(jet_csv_pairs.at(1).first);
        dPhibb = getdphi(jet0.Phi(), jet1.Phi());
        jer_mct = sqrt(2*ptb1*ptb2*(1+cos(dPhibb)));  

        // up
        jet0_pt = ak4pfjets_p4().at(jet_csv_pairs.at(0).first).pt()*jet_JER_factor_up.at(jet_csv_pairs.at(0).first);
        jet1_pt = ak4pfjets_p4().at(jet_csv_pairs.at(1).first).pt()*jet_JER_factor_up.at(jet_csv_pairs.at(1).first);
        jet0.SetPtEtaPhiM(jet0_pt, ak4pfjets_p4().at(jet_csv_pairs.at(0).first).eta(), ak4pfjets_p4().at(jet_csv_pairs.at(0).first).phi(), 0);
        jet1.SetPtEtaPhiM(jet1_pt, ak4pfjets_p4().at(jet_csv_pairs.at(1).first).eta(), ak4pfjets_p4().at(jet_csv_pairs.at(1).first).phi(), 0);
        jerup_mbb = (jet0 + jet1).M()  ; 
        jerup_ptbb = (jet0 + jet1).Pt()  ; 
        ptb1 = ak4pfjets_p4().at(jet_csv_pairs.at(0).first).pt()*jet_JER_factor_up.at(jet_csv_pairs.at(0).first);
        ptb2 = ak4pfjets_p4().at(jet_csv_pairs.at(1).first).pt()*jet_JER_factor_up.at(jet_csv_pairs.at(1).first);
        jerup_mct = sqrt(2*ptb1*ptb2*(1+cos(dPhibb))); //dphi bb doesn't change

        // down
        jet0_pt = ak4pfjets_p4().at(jet_csv_pairs.at(0).first).pt()*jet_JER_factor_down.at(jet_csv_pairs.at(0).first);
        jet1_pt = ak4pfjets_p4().at(jet_csv_pairs.at(1).first).pt()*jet_JER_factor_down.at(jet_csv_pairs.at(1).first);
        jet0.SetPtEtaPhiM(jet0_pt, ak4pfjets_p4().at(jet_csv_pairs.at(0).first).eta(), ak4pfjets_p4().at(jet_csv_pairs.at(0).first).phi(), 0);
        jet1.SetPtEtaPhiM(jet1_pt, ak4pfjets_p4().at(jet_csv_pairs.at(1).first).eta(), ak4pfjets_p4().at(jet_csv_pairs.at(1).first).phi(), 0);
        jerdown_mbb = (jet0 + jet1).M()  ; 
        jerdown_ptbb = (jet0 + jet1).Pt()  ; 
        ptb1 = ak4pfjets_p4().at(jet_csv_pairs.at(0).first).pt()*jet_JER_factor_down.at(jet_csv_pairs.at(0).first);
        ptb2 = ak4pfjets_p4().at(jet_csv_pairs.at(1).first).pt()*jet_JER_factor_down.at(jet_csv_pairs.at(1).first);
        jerdown_mct = sqrt(2*ptb1*ptb2*(1+cos(dPhibb)));  //dphi bb doesn't change

        // custom down variation
        jet0_pt = ak4pfjets_p4().at(jet_csv_pairs.at(0).first).pt()*jet_JER_factor_down.at(jet_csv_pairs.at(0).first)/jet_JER_factor.at(jet_csv_pairs.at(0).first);
        jet1_pt = ak4pfjets_p4().at(jet_csv_pairs.at(1).first).pt()*jet_JER_factor_down.at(jet_csv_pairs.at(1).first)/jet_JER_factor.at(jet_csv_pairs.at(1).first);
        jet0.SetPtEtaPhiM(jet0_pt, ak4pfjets_p4().at(jet_csv_pairs.at(0).first).eta(), ak4pfjets_p4().at(jet_csv_pairs.at(0).first).phi(), 0);
        jet1.SetPtEtaPhiM(jet1_pt, ak4pfjets_p4().at(jet_csv_pairs.at(1).first).eta(), ak4pfjets_p4().at(jet_csv_pairs.at(1).first).phi(), 0);
        jercdown_mbb = (jet0 + jet1).M()  ; 
        jercdown_ptbb = (jet0 + jet1).Pt()  ; 
        ptb1 = ak4pfjets_p4().at(jet_csv_pairs.at(0).first).pt()*jet_JER_factor_down.at(jet_csv_pairs.at(0).first)/jet_JER_factor.at(jet_csv_pairs.at(0).first);
        ptb2 = ak4pfjets_p4().at(jet_csv_pairs.at(1).first).pt()*jet_JER_factor_down.at(jet_csv_pairs.at(1).first)/jet_JER_factor.at(jet_csv_pairs.at(1).first);
        jercdown_mct = sqrt(2*ptb1*ptb2*(1+cos(dPhibb)));  //dphi bb doesn't change
        
      }


      // JES up
      vector <pair<int, float>> jup_csv_pairs;
      for(uint ijet = 0; ijet < jup_ak4pfjets_deepCSV().size(); ijet++) {
        float btagvalue = jup_ak4pfjets_deepCSV().at(ijet);
        jup_csv_pairs.push_back(make_pair(ijet,btagvalue));
      }
      sort(jup_csv_pairs.begin(), jup_csv_pairs.end(), sortIndexbyCSV);


      jup_mbb  = -999;
      jup_mct  = -999;
      jup_ptbb = -999;
      if(jup_ngoodjets()>1) {
        jup_mbb = (jup_ak4pfjets_p4().at(jup_csv_pairs.at(0).first) + jup_ak4pfjets_p4().at(jup_csv_pairs.at(1).first)).mass();
        jup_ptbb = (jup_ak4pfjets_p4().at(jup_csv_pairs.at(0).first) + jup_ak4pfjets_p4().at(jup_csv_pairs.at(1).first)).pt();

        float ptb1, ptb2, dPhibb;
        ptb1 = jup_ak4pfjets_p4().at(jup_csv_pairs.at(0).first).pt();
        ptb2 = jup_ak4pfjets_p4().at(jup_csv_pairs.at(1).first).pt();
        dPhibb = getdphi(jup_ak4pfjets_p4().at(jup_csv_pairs.at(0).first).phi(),jup_ak4pfjets_p4().at(jup_csv_pairs.at(1).first).phi());
        jup_mct = sqrt(2*ptb1*ptb2*(1+cos(dPhibb)));
      }
      // down
      vector <pair<int, float>> jdown_csv_pairs;
      for(uint ijet = 0; ijet < jdown_ak4pfjets_deepCSV().size(); ijet++) {
        float btagvalue = jdown_ak4pfjets_deepCSV().at(ijet);
        jdown_csv_pairs.push_back(make_pair(ijet,btagvalue));
      }
      sort(jdown_csv_pairs.begin(), jdown_csv_pairs.end(), sortIndexbyCSV);

      jdown_mbb  = -999;
      jdown_mct  = -999;
      jdown_ptbb = -999;
      if(jdown_ngoodjets()>1) {
        jdown_mbb = (jdown_ak4pfjets_p4().at(jdown_csv_pairs.at(0).first) + jdown_ak4pfjets_p4().at(jdown_csv_pairs.at(1).first)).mass();
        jdown_ptbb = (jdown_ak4pfjets_p4().at(jdown_csv_pairs.at(0).first) + jdown_ak4pfjets_p4().at(jdown_csv_pairs.at(1).first)).pt();

        float ptb1, ptb2, dPhibb;
        ptb1 = jdown_ak4pfjets_p4().at(jdown_csv_pairs.at(0).first).pt();
        ptb2 = jdown_ak4pfjets_p4().at(jdown_csv_pairs.at(1).first).pt();
        dPhibb = getdphi(jdown_ak4pfjets_p4().at(jdown_csv_pairs.at(0).first).phi(),jdown_ak4pfjets_p4().at(jdown_csv_pairs.at(1).first).phi());
        jdown_mct = sqrt(2*ptb1*ptb2*(1+cos(dPhibb)));
      }

      ///Gen information: susy particles, quarks, bosons, leptons

      //susy
      for (size_t q = 0; q < gensusy_id().size(); ++q) {
       if (!gensusy_isLastCopy().at(q)) continue;
       gen_pt->push_back(gensusy_p4().at(q).pt());
       gen_phi->push_back(gensusy_p4().at(q).phi());
       gen_eta->push_back(gensusy_p4().at(q).eta());
       gen_m->push_back(gensusy_p4().at(q).mass());
       gen_id->push_back(gensusy_id().at(q));
       gen_idx->push_back(gensusy__genpsidx().at(q));
       gen_motherid->push_back(gensusy_motherid().at(q));
       gen_motheridx->push_back(gensusy_motheridx().at(q));
       gen_gmotherid->push_back(gensusy_gmotherid().at(q));
       gen_gmotheridx->push_back(gensusy_gmotheridx().at(q));
     } 
     //quarks
     for (size_t q = 0; q < genqs_id().size(); ++q) {
      if (!genqs_isLastCopy().at(q)) continue;  
      if (genqs_p4().at(q).pt()<1) continue;
      if (abs(genqs_id().at(q)) >= 4 || genqs_isfromt().at(q)){
        gen_pt->push_back(genqs_p4().at(q).pt());
        gen_phi->push_back(genqs_p4().at(q).phi());
        gen_eta->push_back(genqs_p4().at(q).eta());
        gen_m->push_back(genqs_p4().at(q).mass());
        gen_id->push_back(genqs_id().at(q));
        gen_idx->push_back(genqs__genpsidx().at(q));
        gen_motherid->push_back(genqs_motherid().at(q));
        gen_motheridx->push_back(genqs_motheridx().at(q));
        gen_gmotherid->push_back(genqs_gmotherid().at(q));
        gen_gmotheridx->push_back(genqs_gmotheridx().at(q));
      }
    }
    //bosons
    for (size_t q = 0; q < genbosons_id().size(); ++q) {
      if (!genbosons_isLastCopy().at(q)) continue;
      if (genbosons_p4().at(q).pt()<1) continue;
      gen_pt->push_back(genbosons_p4().at(q).pt());
      gen_phi->push_back(genbosons_p4().at(q).phi());
      gen_eta->push_back(genbosons_p4().at(q).eta());
      gen_m->push_back(genbosons_p4().at(q).mass());
      gen_id->push_back(genbosons_id().at(q));
      gen_idx->push_back(genbosons__genpsidx().at(q));
      gen_motherid->push_back(genbosons_motherid().at(q));
      gen_motheridx->push_back(genbosons_motheridx().at(q));
      gen_gmotherid->push_back(genbosons_gmotherid().at(q));
      gen_gmotheridx->push_back(genbosons_gmotheridx().at(q));
    }
    //leptons
    for (size_t q = 0; q < genleps_id().size(); ++q) {
      if (!genleps_isLastCopy().at(q)) continue;
      if (genleps_p4().at(q).pt()<1) continue;
      if(genleps_fromHardProcessFinalState().at(q) || genleps_fromHardProcessDecayed().at(q) || abs(genleps_motherid().at(q))==15 || abs(genleps_gmotherid().at(q))==15 || abs(genleps_gmotherid().at(q))==24){
        gen_pt->push_back(genleps_p4().at(q).pt());
        gen_phi->push_back(genleps_p4().at(q).phi());
        gen_eta->push_back(genleps_p4().at(q).eta());
        gen_m->push_back(genleps_p4().at(q).mass());
        gen_id->push_back(genleps_id().at(q));
        gen_idx->push_back(genleps__genpsidx().at(q));
        gen_motherid->push_back(genleps_motherid().at(q));
        gen_motheridx->push_back(genleps_motheridx().at(q));
        gen_gmotherid->push_back(genleps_gmotherid().at(q));
        gen_gmotheridx->push_back(genleps_gmotheridx().at(q));
      }
    }
    //neutrinos

    float maxnupt =0;
    for (size_t q = 0; q < gennus_id().size(); ++q) {
      if (!gennus_isLastCopy().at(q)) continue; 
      if (gennus_p4().at(q).pt()<1) continue; 
      gen_pt->push_back(gennus_p4().at(q).pt());
      if(gennus_p4().at(q).pt() > maxnupt) maxnupt=gennus_p4().at(q).pt();
      gen_phi->push_back(gennus_p4().at(q).phi());
      gen_eta->push_back(gennus_p4().at(q).eta());
      gen_m->push_back(gennus_p4().at(q).mass());
      gen_id->push_back(gennus_id().at(q));
      gen_idx->push_back(gennus__genpsidx().at(q));
      gen_motherid->push_back(gennus_motherid().at(q));
      gen_motheridx->push_back(gennus_motheridx().at(q));
      gen_gmotherid->push_back(gennus_gmotherid().at(q));
      gen_gmotheridx->push_back(gennus_gmotheridx().at(q));
    }


      //cout<<"Got to pass"<<endl;
      // fillEfficiencyHistos(testVec[0], "filters");
      pass=true;
      if(is_data()) pass = isgoodrun && !duplicate;
      stitch =true;
      //cout<<dsname<<" "<<genmet()<<" "<<dsname.Contains("TTJets")<<" "<<!dsname.Contains("genMET")<<endl;

      if(dsname.Contains("TTJets") && !dsname.Contains("genMET") && ((year()<2018 && genmet()>150.) || (year()==2018 && genmet()>120.))) stitch = false;
      
      //for 2018, inclusive and met binned don't match between 80 and 120 GeV. Use inclusive for this region instead of METBinned.
      if(dsname.Contains("TTJets") && dsname.Contains("genMET") &&  year()==2018 && genmet()<=120.) stitch = false;

      //cout<<stitch<<endl;
      
      if((dsname.Contains("W1Jets") || dsname.Contains("W2Jets") || dsname.Contains("W3Jets") || dsname.Contains("W4Jets"))  && !dsname.Contains("NuPt-200") && maxnupt>200.) stitch = false;




       // Require at least 1 good vertex
      if (nvtxs() < 1) pass=false;


      if (babyver > 31.5) {
        if ( !is_fastsim_ && !filt_met() ) pass=false;  // use the CORE function passesMETfiltersRun2()
      } else if (doTopTagging) {
        // Recommended filters for the legacy analysis
        if ( !filt_goodvtx() ) pass=false;
        if ( !is_fastsim_ && !filt_globalsupertighthalo2016() ) pass=false;
        if ( !filt_hbhenoise() ) pass=false;
        if ( !filt_hbheisonoise() )   pass=false;
        if ( !filt_ecaltp() ) pass=false;
        if ( !filt_badMuonFilter() ) pass=false;
        // if ( !filt_badChargedCandidateFilter() ) pass=false; // no longer recommended
        if ( is_data() && !filt_eebadsc() ) pass=false;
        if ( year_ >= 2017 && !filt_ecalbadcalib() ) pass=false;  // only for 2017 and 2018
      } else if (samplever.find("v24") == 0) {
        // Filters used in Moriond17 study, keep for sync check
        if ( !filt_met() ) pass=false;
        if ( !filt_goodvtx() ) pass=false;
        if ( firstGoodVtxIdx() == -1 ) pass=false;
        if ( !filt_badMuonFilter() ) pass=false;
        if ( !filt_badChargedCandidateFilter() ) pass=false;
        if ( !filt_jetWithBadMuon() ) pass=false;
        if ( !filt_pfovercalomet() ) pass=false;
        // if ( filt_duplicatemuons() ) pass=false; // Temporary, breaks for old 16 babies
        // if ( filt_badmuons() ) pass=false;
        // if ( !filt_nobadmuons() ) pass=false;
      }

      pass_old = pass; //preserve value for comparison to old babies

            // HEM issue for 2018, that excess of electron happens at -4.7 < eta < -1.4, -1.6 < phi < -0.8
      if (doHEMElectronVeto && year_ == 2018 && is_data() && run() >= 319077) {
        if (abs(lep2_pdgid()) == 11 && lep2_p4().eta() < -1.4 && lep2_p4().phi() > -1.6 && lep2_p4().phi() < -0.8)
          pass=false; // veto the events
      }

      if (doHEMJetVeto && year_ == 2018 && is_data() && run() >= 319077) {
        bool hasHEMjet = false;
        for (auto jet : ak4pfjets_p4()) {  // the jets are cleaned with leptons already
          if (jet.eta() < -1.4 && jet.phi() > -1.6 && jet.phi() < -0.8)
            hasHEMjet = true;
        }
        if (hasHEMjet) pass=false;
      }


      // Apply met filters
      //if (doTopTagging) {
      //   // Recommended filters for the legacy analysis
      // switch (year_) {
      //   case 2018:  // 2017 and 2018 uses the same set of filters
      //   case 2017:
      //     if ( !filt_ecalbadcalib() ) pass=false;
      //   case 2016:
      //     if ( !filt_goodvtx() ) pass=false;
      //     if ( !is_fastsim_ && !filt_globalsupertighthalo2016() ) pass=false;
      //     if ( !filt_hbhenoise() ) pass=false;
      //     if ( !filt_hbheisonoise() )   pass=false;
      //     if ( !filt_ecaltp() ) pass=false;
      //     if ( !filt_badMuonFilter() ) pass=false;
      //     if ( !filt_badChargedCandidateFilter() ) pass=false;
      //     if ( is_data() && !filt_eebadsc() ) pass=false;
      // }

      // if (is_fastsim_) {
      //   if ( !filt_fastsimjets() ) pass=false;
      // }
      // } else if (samplever.find("v24") == 0) {
      //   // Filters used in Moriond17 study, keep for sync check
      //   if ( !filt_met() ) continue;
      //   if ( !filt_goodvtx() ) continue;
      //   if ( firstGoodVtxIdx() == -1 ) continue;
      //   if ( !filt_badMuonFilter() ) continue;
      //   if ( !filt_badChargedCandidateFilter() ) continue;
      //   if ( !filt_jetWithBadMuon() ) continue;
      //   if ( !filt_pfovercalomet() ) continue;
      //   if ( filt_duplicatemuons() ) continue; // Temporary, breaks for old 16 babies
      //   if ( filt_badmuons() ) continue;
      //   if ( !filt_nobadmuons() ) continue;
      // }

      //cout<<"About to fill tree"<<endl;
      extraTree->Fill();
      //cout<<"Filled tree"<<endl;
 

     // if (event > 10) break;  // for debugging purpose
    } // end of event loop


    delete tree;
    file.Close();
    outfile_->Write();
    outfile_->Close();
    // delete extraTree;
  } // end of file loop

  cout << "[WHLooper::looper] processed  " << nEventsTotal << " events" << endl;
  if ( nEventsChain != nEventsTotal )
    cout << "WARNING: Number of events from files is not equal to total number of events" << endl;

  outfile_->Write();
  outfile_->Close();
  bmark->Stop("benchmark");
  cout << endl;
  cout << nEventsTotal << " Events Processed, where " << nDuplicates << " duplicates were skipped.";
  cout << "------------------------------" << endl;
  cout << "CPU  Time:   " << Form( "%.01f s", bmark->GetCpuTime("benchmark")  ) << endl;
  cout << "Real Time:   " << Form( "%.01f s", bmark->GetRealTime("benchmark") ) << endl;
  cout << endl;
  delete bmark;

  return;
}








