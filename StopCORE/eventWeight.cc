#include "eventWeight.h"

using namespace stop1l;

// SNT CORE function
/// topPtWeight() copied from MCSelections.cc
float topPtWeight(float pt_top, float pt_tbar) {
  if (pt_top < 0. || pt_tbar < 0.) return 1.;
  float weight_top  = exp(0.0615 - 0.0005 * pt_top );
  float weight_tbar = exp(0.0615 - 0.0005 * pt_tbar);
  return sqrt( weight_top * weight_tbar );
}

evtWgtInfo::Util::Util( systID systematic ) {

  switch( systematic ) {

    case( k_nominal ):
      label       = "nominal";
      title       = "Nominal";
      break;
    case( k_JESUp ):
      label       = "jesUp";
      title       = "JES Up";
      break;
    case( k_JESDown ):
      label       = "jesDn";
      title       = "JES Down";
      break;
    case( k_diLepTriggerUp ):
      label       = "diLepTriggerSFUp";
      title       = "DiLepton Trigger Scale Factor, Up";
      break;
    case( k_diLepTriggerDown ):
      label       = "diLepTriggerSFDn";
      title       = "DiLepton Trigger Scale Factor, Down";
      break;
    case( k_cr2lTriggerUp ):
      label       = "cr2lTriggerSFUp";
      title       = "CR2l Trigger Scale Factor, Up";
      break;
    case( k_cr2lTriggerDown ):
      label       = "cr2lTriggerSFDn";
      title       = "CR2l Trigger Scale Factor, Down";
      break;
    case( k_bTagEffHFUp ):
      label       = "bTagEffHFUp";
      title       = "bTag Efficicency, Heavy Flavour, Up";
      break;
    case( k_bTagEffHFDown ):
      label       = "bTagEffHFDn";
      title       = "bTag Efficicency, Heavy Flavour, Down";
      break;
    case( k_bTagEffLFUp ):
      label       = "bTagEffLFUp";
      title       = "bTag Efficicency, Light Flavour, Up";
      break;
    case( k_bTagEffLFDown ):
      label       = "bTagEffLFDn";
      title       = "bTag Efficicency, Light Flavour, Down";
      break;
    case( k_bTagFSEffUp ):
      label       = "bTagFSEffUp";
      title       = "bTag Fastsim Efficiency, Up";
      break;
    case( k_bTagFSEffDown ):
      label       = "bTagFSEffDn";
      title       = "bTag Fastsim Efficiency, Down";
      break;
    case( k_lepSFUp ):
      label       = "lepSFUp";
      title       = "Lepton Scale Factor, Up";
      break;
    case( k_lepSFDown ):
      label       = "lepSFDn";
      title       = "Lepton Scale Factor, Down";
      break;
    case( k_lepFSSFUp ):
      label       = "lepFSSFUp";
      title       = "Lepton Fastsim Scale Factor, Up";
      break;
    case( k_lepFSSFDown ):
      label       = "lepFSSFDn";
      title       = "Lepton Fastsim Scale Factor, Down";
      break;
    case( k_topPtSFUp ):
      label       = "topPtSFUp";
      title       = "top pT Scale Factor, Up";
      break;
    case( k_topPtSFDown ):
      label       = "topPtSFDn";
      title       = "top pT Scale Factor, Down";
      break;
    case( k_metResUp ):
      label       = "metResUp";
      title       = "MET Resolution, Up";
      break;
    case( k_metResDown ):
      label       = "metResDn";
      title       = "MET Resolution, Down";
      break;
    case( k_metTTbarUp ):
      label       = "metTTbarUp";
      title       = "MET ttbar SF, Up";
      break;
    case( k_metTTbarDown ):
      label       = "metTTbarDn";
      title       = "MET ttbar SF, Down";
      break;
    case( k_ttbarSysPtUp ):
      label       = "ttbarSysPtUp";
      title       = "ttbar/tW system pT, Up";
      break;
    case( k_ttbarSysPtDown ):
      label       = "ttbarSysPtDn";
      title       = "ttbar/tW system pT, Down";
      break;
    case( k_nuPtSF_Up ):
      label       = "nuPtUp";
      title       = "nuetrino pT, Up";
      break;
    case( k_nuPtSF_Down ):
      label       = "nuPtDn";
      title       = "nuetrino pT, Down";
      break;
    case( k_WwidthSF_Up ):
      label       = "WwidthSFUp";
      title       = "W Width, Up";
      break;
    case( k_WwidthSF_Down ):
      label       = "WwidthSFDn";
      title       = "W Width, Down";
      break;
    case( k_WbXsec_Up ):
      label       = "WbXsecUp";
      title       = "W+b x-section, Up";
      break;
    case( k_WbXsec_Down ):
      label       = "WbXsecDn";
      title       = "W+b x-section, Down";
      break;
    case( k_pdfUp ):
      label       = "pdfUp";
      title       = "PDF, Up";
      break;
    case( k_pdfDown ):
      label       = "pdfDn";
      title       = "PDF, Down";
      break;
    case( k_alphasUp ):
      label       = "alphasUp";
      title       = "Alpha S, Up";
      break;
    case( k_alphasDown ):
      label       = "alphasDn";
      title       = "Alpha S, Down";
      break;
    case( k_q2Up ):
      label       = "q2Up";
      title       = "Q2, Up";
      break;
    case( k_q2Down ):
      label       = "q2Dn";
      title       = "Q2, Down";
      break;
    case( k_lumiUp ):
      label       = "lumiUp";
      title       = "Luminosity, Up";
      break;
    case( k_lumiDown ):
      label       = "lumiDn";
      title       = "Luminosity, Down";
      break;
    case( k_ISRUp ):
      label       = "ISRUp";
      title       = "ISR, Up";
      break;
    case( k_ISRDown ):
      label       = "ISRDn";
      title       = "ISR, Down";
      break;
    case( k_xsecUp ):
      label       = "xsecUp";
      title       = "Signal X-Sec, Up";
      break;
    case( k_xsecDown ):
      label       = "xsecDn";
      title       = "Signal X-Sec, Down";
      break;
    case( k_puUp ):
      label       = "pileupUp";
      title       = "Pileup Reweight, Up";
      break;
    case( k_puDown ):
      label       = "pileupDn";
      title       = "Pileup Reweight, Down";
      break;
    case( k_tauSFUp ):
      label       = "tauSFUp";
      title       = "Tau Efficiency SF, Up";
      break;
    case( k_tauSFDown ):
      label       = "tauSFDn";
      title       = "Tau Efficiency SF, Down";
      break;
    default:
      std::cout << "[eventWeight] >> Could not find systematic info from systematic enum provided!" << std::endl;
      label        = "NO SYSTEMATIC INFO FOUND FROM ENUM PROVIDED";
      title        = "NO SYSTEMATIC INFO FOUND FROM ENUM PROVIDED";
      break;
  } // end systematic switch

}

//////////////////////////////////////////////////////////////////////

evtWgtInfo::evtWgtInfo() {
  is_bkg_     = false;
  is_data_    = false;
  is_fastsim_ = false;
  sync16      = false;
  verbose     = false;

  // Initialize Switches for additional SFs
  apply_diLepTrigger_sf = false;
  apply_cr2lTrigger_sf  = false;
  apply_bTag_sf         = false;
  apply_bTagFS_sf       = false;
  apply_lep_sf          = false;
  apply_vetoLep_sf      = false;
  apply_tau_sf          = false;
  apply_lepFS_sf        = false;
  apply_topPt_sf        = false;
  apply_metRes_sf       = false;
  apply_metTTbar_sf     = false;
  apply_ttbarSysPt_sf   = false;
  apply_WbXsec_sf       = false;
  apply_ISR_sf          = false;
  apply_pu_sf           = false;
  apply_sample_sf       = false;
  apply_genweights_unc  = true;

  // Initialize baby weights histograms
  h_sig_counter         = nullptr;
  h_sig_counter_nEvents = nullptr;
  h_bkg_counter         = nullptr;
  h_cr2lTrigger_sf_el   = nullptr;
  h_cr2lTrigger_sf_mu   = nullptr;
  h_pu_wgt              = nullptr;
  h_pu_wgt_up           = nullptr;
  h_pu_wgt_dn           = nullptr;
  h_recoEff_tau         = nullptr;

  // Utilty Var Constants
  year = 0;
  lumi = 137.42;         // Legacy lumi of RunII
  lumi_err = lumi*0.05;  // 5% uncertainty may be enough?

  // Initialize event weights and related variables
  initializeWeights();
  sf_extra_file = 1.0;

}

//////////////////////////////////////////////////////////////////////

void evtWgtInfo::Setup(string samplestr, int inyear, bool applyUnc, bool useBTagUtils, bool useLepSFUtils) {

  samptype = findSampleType(samplestr);

  year = inyear;

  if (year == 2016) {
    lumi = 35.922;         // 2016 lumi
    lumi_err = lumi*0.025; // 2.6% uncertainty was used for Moriond17
  } else if (year == 2017) {
    lumi = 41.529;         // 2017 lumi
    lumi_err = lumi*0.023;
  } else if (year == 2018) {
    lumi = 59.97;          // projected 2018 lumi
    lumi_err = lumi*0.05;  // preliminary value
  }

  sf_extra_file = 1.0;  // reset file weight for each file

  // Get sample info from enum
  if ( samptype == data ) {
    is_data_ = true;
  } else if ( samptype == fastsim ) {
    is_fastsim_ = true;
  }
  is_bkg_ = !is_data_ && !is_fastsim_;

  // The event counter hists setup is postponed to the file loop
  apply_genweights_unc = applyUnc;

  // Decision to use Utilities vs direct from baby for bTag and lep SFs
  useBTagSFs_fromFiles = useBTagUtils;
  useLepSFs_fromFiles  = useLepSFUtils;

  if (sync16 && (useBTagUtils || useLepSFUtils)) {
    cout << "[eventWeight] >> Cannot load these SFs from file when doing sync studies, since they are have changed!" << endl;
    useLepSFUtils = useBTagUtils = false;
  }

  // Get Signal XSection File
  if ( is_fastsim_ ) {
    f_sig_xsec = new TFile("../StopCORE/inputs/signal_xsec/xsec_stop_13TeV.root","read");
    h_sig_xsec = (TH1D*) f_sig_xsec->Get("stop");
  }

  // Get SR trigger histos
  if ( !is_data_ && apply_cr2lTrigger_sf) {
    f_cr2lTrigger_sf = new TFile("../StopCORE/inputs/trigger/TrigEfficiencies_all.root","read");
    if (sync16) {
      h_cr2lTrigger_sf_el = (TEfficiency*) f_cr2lTrigger_sf->Get("Efficiency_ge2l_metrl_el");
      h_cr2lTrigger_sf_mu = (TEfficiency*) f_cr2lTrigger_sf->Get("Efficiency_ge2l_metrl_mu");
    } else if (year == 2016) {
      h_cr2lTrigger_sf_el = (TEfficiency*) f_cr2lTrigger_sf->Get("heff16_extb_trigeff_metrl_lep1pt_el");
      h_cr2lTrigger_sf_mu = (TEfficiency*) f_cr2lTrigger_sf->Get("heff16_extb_trigeff_metrl_lep1pt_mu");
    } else if (year == 2017) {
      h_cr2lTrigger_sf_el = (TEfficiency*) f_cr2lTrigger_sf->Get("heff17_extb_trigeff_metrl_lep1pt_el");
      h_cr2lTrigger_sf_mu = (TEfficiency*) f_cr2lTrigger_sf->Get("heff17_extb_trigeff_metrl_lep1pt_mu");
    } else if (year == 2018) {
      h_cr2lTrigger_sf_el = (TEfficiency*) f_cr2lTrigger_sf->Get("heff18_extb_trigeff_metrl_lep1pt_el");
      h_cr2lTrigger_sf_mu = (TEfficiency*) f_cr2lTrigger_sf->Get("heff18_extb_trigeff_metrl_lep1pt_mu");
    }
    if (!h_cr2lTrigger_sf_mu || !h_cr2lTrigger_sf_el) {
      cout << "[eventWeight::Setup] >> Cannot find histogram for cr2lTrigger!! Turning it off!!" << endl;
      apply_cr2lTrigger_sf = false;
    } else {
      max_metrl  = h_cr2lTrigger_sf_mu->GetTotalHistogram()->GetYaxis()->GetXmax() - 0.001;
      max_lep1pt = h_cr2lTrigger_sf_mu->GetTotalHistogram()->GetXaxis()->GetXmax() - 0.001;
    }
  }

  // Initialize bTag SF machinery
  if ( !is_data_ && useBTagSFs_fromFiles ) {
    bTagSFUtil = new eventWeight_bTagSF( is_fastsim_ );
  }

  // Initialize Lepton Scale Factors
  if ( !is_data_ && useLepSFs_fromFiles ) {
    lepSFUtil  = new eventWeight_lepSF();
    lepSFUtil->setup(is_fastsim_, year, "../StopCORE/inputs/lepsf");
  }

  // Get pileup wgt histo
  if ( !is_data_ && apply_pu_sf) {
    f_pu = new TFile("../StopCORE/inputs/pileup/pileup_wgts.root", "read");
    h_pu_wgt = (TH1D*) f_pu->Get("h_pileup_wgt");
    h_pu_wgt_up = (TH1D*) f_pu->Get("h_pileup_wgt_up");
    h_pu_wgt_dn = (TH1D*) f_pu->Get("h_pileup_wgt_down");
  }

  // Get lep reco histo
  if ( !is_data_ && apply_tau_sf) {
    TString lepeff_file = "Moriond17/lepeff__moriond17__ttbar_powheg_pythia8_25ns.root";
    f_lepEff = new TFile("../StopCORE/inputs/lepsf/"+lepeff_file, "read");
    h_recoEff_tau = (TH2D*) f_lepEff->Get("h2_lepEff_vetoSel_Eff_tau");
  }

}

//////////////////////////////////////////////////////////////////////

void evtWgtInfo::Cleanup() {

  if ( is_data_ ) {
    f_cr2lTrigger_sf->Close();
    delete f_cr2lTrigger_sf;

    if (useBTagSFs_fromFiles) delete bTagSFUtil;
    if (useLepSFs_fromFiles) delete lepSFUtil;
  }
  if ( is_fastsim_ ) {
    f_sig_xsec->Close();
    delete f_sig_xsec;
  }
}

//////////////////////////////////////////////////////////////////////

void evtWgtInfo::resetEvent() {
  event_ready = false;
  add2ndLepToMet = false;
}

//////////////////////////////////////////////////////////////////////

void evtWgtInfo::getCounterHistogramFromBaby( TFile *sourceFile ) {

  // Get counter histogram from source file
  if ( is_fastsim_ ) {
    h_sig_counter = (TH3D*) sourceFile->Get("h_counterSMS");
    h_sig_counter_nEvents = (TH2D*) sourceFile->Get("histNEvts");
  }
  else if ( !is_data_ ) {
    h_bkg_counter = (TH1D*) sourceFile->Get("h_counter");
  }

}

//////////////////////////////////////////////////////////////////////

void evtWgtInfo::initializeWeights() {

  // Variables to form baseline event weight
  mStop = 0;
  mLSP = 0;
  mChargino = 0;

  jes_type = 0;

  nEvents = 1.0;
  xsec = 1.0;
  xsec_err = 0.0;

  // Event weights for each systematic
  sf_nominal = 1.0;

  sf_diLepTrigger = 1.0;
  sf_diLepTrigger_up = 1.0;
  sf_diLepTrigger_dn = 1.0;

  sf_cr2lTrigger = 1.0;
  sf_cr2lTrigger_up = 1.0;
  sf_cr2lTrigger_dn = 1.0;

  sf_bTag = 1.0;
  sf_bTagEffHF_up = 1.0;
  sf_bTagEffHF_dn = 1.0;
  sf_bTagEffLF_up = 1.0;
  sf_bTagEffLF_dn = 1.0;
  sf_bTag_FS_up = 1.0;
  sf_bTag_FS_dn = 1.0;
  sf_bTag_tight = 1.0;
  sf_bTagEffHF_tight_up = 1.0;
  sf_bTagEffHF_tight_dn = 1.0;
  sf_bTagEffLF_tight_up = 1.0;
  sf_bTagEffLF_tight_dn = 1.0;
  sf_bTag_tight_FS_up = 1.0;
  sf_bTag_tight_FS_dn = 1.0;

  sf_lep = 1.0;
  sf_lep_up = 1.0;
  sf_lep_dn = 1.0;

  sf_vetoLep = 1.0;
  sf_vetoLep_up = 1.0;
  sf_vetoLep_dn = 1.0;

  sf_tau = 1.0;
  sf_tau_up = 1.0;
  sf_tau_dn = 1.0;

  sf_lepFS = 1.0;
  sf_lepFS_up = 1.0;
  sf_lepFS_dn = 1.0;

  sf_topPt = 1.0;
  sf_topPt_up = 1.0;
  sf_topPt_dn = 1.0;

  sf_metRes = 1.0;
  sf_metRes_up = 1.0;
  sf_metRes_dn = 1.0;
  sf_metRes_corridor = 1.0;
  sf_metRes_corridor_up = 1.0;
  sf_metRes_corridor_dn = 1.0;

  sf_metTTbar = 1.0;
  sf_metTTbar_up = 1.0;
  sf_metTTbar_dn = 1.0;

  sf_ttbarSysPt = 1.0;
  sf_ttbarSysPt_up = 1.0;
  sf_ttbarSysPt_dn = 1.0;

  sf_ISR = 1.0;
  sf_ISR_up = 1.0;
  sf_ISR_dn = 1.0;

  sf_nuPt_up = 1.0;
  sf_nuPt_dn = 1.0;

  sf_Wwidth_up = 1.0;
  sf_Wwidth_dn = 1.0;

  sf_WbXsec_up = 1.0;
  sf_WbXsec_dn = 1.0;

  sf_pdf_up = 1.0;
  sf_pdf_dn = 1.0;

  sf_alphas_up = 1.0;
  sf_alphas_dn = 1.0;

  sf_q2_up = 1.0;
  sf_q2_dn = 1.0;

  sf_lumi = 1.0;
  sf_lumi_up = 1.0;
  sf_lumi_dn = 1.0;

  sf_pu = 1.0;
  sf_pu_up = 1.0;
  sf_pu_dn = 1.0;

  sf_xsec_up = 1.0;
  sf_xsec_dn = 1.0;

  sf_sample = 1.0;

  for (int iSys=0; iSys<k_nSyst; iSys++) sys_wgts[iSys]=1.0;
  for (int iSys=0; iSys<k_nSyst; iSys++) sys_wgts_corridor[iSys]=1.0;

  return;
}

//////////////////////////////////////////////////////////////////////

void evtWgtInfo::calculateWeightsForEvent() {

  initializeWeights();

  // If sample is data
  if ( is_data_ ) return;

  // Get SUSY masses if sample is signal scan
  if ( is_fastsim_ ) getSusyMasses( mStop, mLSP );

  // Get nEvents
  getNEvents( nEvents );

  // Get xSec
  getXSecWeight( xsec, sf_xsec_up, sf_xsec_dn );

  // Get Scale1fb*Lumi Wgt
  getScaleToLumiWeight( sf_nominal );

  // get dilepton trigger sf
  if (apply_diLepTrigger_sf)
    getDiLepTriggerWeight( sf_diLepTrigger, sf_diLepTrigger_up, sf_diLepTrigger_dn );

  // get CR2l trigger sf <-- when to apply this sf?
  if (apply_cr2lTrigger_sf)
    getCR2lTriggerWeight( sf_cr2lTrigger, sf_cr2lTrigger_up, sf_cr2lTrigger_dn );

  // btag SF, medium=1 and tight=2
  if (apply_bTag_sf) {
    getBTagWeight( 1, sf_bTag, sf_bTagEffHF_up, sf_bTagEffHF_dn, sf_bTagEffLF_up, sf_bTagEffLF_dn, sf_bTag_FS_up, sf_bTag_FS_dn );
    getBTagWeight( 2, sf_bTag_tight, sf_bTagEffHF_tight_up, sf_bTagEffHF_tight_dn, sf_bTagEffLF_tight_up, sf_bTagEffLF_tight_dn, sf_bTag_tight_FS_up, sf_bTag_tight_FS_dn );
  }

  // lepSFs
  if (apply_lep_sf || apply_vetoLep_sf || apply_lepFS_sf)
    getLepSFWeight( sf_lep, sf_lep_up, sf_lep_dn, sf_lepFS, sf_lepFS_up, sf_lepFS_dn, sf_vetoLep, sf_vetoLep_up, sf_vetoLep_dn );

  // tau SF
  if (apply_tau_sf)
    getTauSFWeight( sf_tau, sf_tau_up, sf_tau_dn );

  // top pT Reweighting
  if (samptype == ttbar)
    getTopPtWeight( sf_topPt, sf_topPt_up, sf_topPt_dn );

  // MET resolution scale factors <-- would be easier to do it by just scaling the histogram afterwards <-- todo: not forget this
  if (apply_metRes_sf && (samptype == ttbar || samptype == singletop || samptype == Wjets))
    getMetResWeight( sf_metRes, sf_metRes_up, sf_metRes_dn );

  if (apply_metRes_corridor_sf)
    getMetResWeight_corridor( sf_metRes_corridor, sf_metRes_corridor_up, sf_metRes_corridor_dn );

  // MET ttbar scale factors
  if (apply_metTTbar_sf && is_bkg_ && (samptype == ttbar || samptype == singletop))
    getMetTTbarWeight( sf_metTTbar, sf_metTTbar_up, sf_metTTbar_dn );

  // ttbar system pT scale factor, apply_ttbarSysPt_sf=false: uncertainty only
  if (is_bkg_ && (samptype == ttbar || samptype == singletop))
    getTTbarSysPtSF( sf_ttbarSysPt, sf_ttbarSysPt_up, sf_ttbarSysPt_dn );

  // ISR Correction
  if (apply_ISR_sf && (samptype == ttbar || samptype == fastsim))
    getISRnJetsWeight( sf_ISR, sf_ISR_up, sf_ISR_dn );

  // Pileup Reweighting
  if (apply_pu_sf) {
    getPileupWeight( sf_pu, sf_pu_up, sf_pu_dn );
  }

  // Lumi Variation
  getLumi( sf_lumi, sf_lumi_up, sf_lumi_dn );

  // Scale Factors for Uncertainties
  // Scale factors for 1l from W background events only
  if (is_bkg_ && babyAnalyzer.is1lepFromW()) {
    // Nuetrino pT scale factor
    // getNuPtSF( sf_nuPt_up, sf_nuPt_dn );

    // // W width scale factor
    // getWwidthSF( sf_Wwidth_up, sf_Wwidth_dn );

    // W+HF xsec uncertainty
    if (apply_WbXsec_sf && samptype == Wjets)
      getWbXSecSF( sf_WbXsec_up, sf_WbXsec_dn );
  }

  // Pre-check genweights size
  if ( apply_genweights_unc && babyAnalyzer.genweights().size() >= 110 ) {
    // PDF Uncertainty
    getPDFWeight( sf_pdf_up, sf_pdf_dn );

    // Alpha Strong, QCD variation
    getAlphasWeight( sf_alphas_up, sf_alphas_dn );

    // Q2 Variation, muF, muR
    getQ2Weight( sf_q2_up, sf_q2_dn );
  }

  // Determine if tight btag SF should be used <-- temporary
  use_tight_bTag = ( babyAnalyzer.Mlb_closestb() >= 175. && babyAnalyzer.ntightbtags() >= 1 );

  //
  // Systematic Weights
  //
  double evt_wgt = 1.0;

  // This is scale1fb*lumi = lumi*1000*xsec/nEvents
  evt_wgt *= sf_nominal;

  // Apply CR2l Trigger scale factor
  evt_wgt *= sf_cr2lTrigger;
  // Apply bTag scale factor
  evt_wgt *= (use_tight_bTag)? sf_bTag_tight : sf_bTag;
  // Apply lepton scale factor
  evt_wgt *= sf_lep * sf_vetoLep * sf_lepFS;
  // Apply tau sf
  evt_wgt *= sf_tau;
  // Apply top pT sf
  evt_wgt *= sf_topPt;
  // Apply met resolution sf
  evt_wgt *= sf_metRes;
  // Apply met ttbar sf
  evt_wgt *= sf_metTTbar;
  // Apply ttbar system pT SF (will be 1 if not ttbar/tW 2l)
  evt_wgt *= sf_ttbarSysPt;
  // Apply ISR SF ( switched to ISRnjets )
  evt_wgt *= sf_ISR;
  // Apply Pileup Wgt
  evt_wgt *= sf_pu;
  // Apply sample weight (for WJets stitching)
  evt_wgt *= sf_sample;
  // Apply any extra weight that was passed in
  evt_wgt *= sf_extra_file;

  //
  // Systematic Weights
  //
  for (int iSys = 0; iSys < k_nSyst; iSys++) {
    if (iSys > 2 && !doingSystematic( systID(iSys) )) continue;
    double sys_wgt = evt_wgt;

    switch (iSys) {
      case k_nominal:
      case k_JESUp:
      case k_JESDown:
        break;
      case k_diLepTriggerUp:
        sys_wgt *= (sf_diLepTrigger_up/sf_diLepTrigger); break;
      case k_diLepTriggerDown:
        sys_wgt *= (sf_diLepTrigger_dn/sf_diLepTrigger); break;
      case k_cr2lTriggerUp:
        sys_wgt *= (sf_cr2lTrigger_up/sf_cr2lTrigger); break;
      case k_cr2lTriggerDown:
        sys_wgt *= (sf_cr2lTrigger_dn/sf_cr2lTrigger); break;
      case k_bTagEffHFUp:
        sys_wgt *= (use_tight_bTag)? (sf_bTagEffHF_tight_up/sf_bTag_tight) : (sf_bTagEffHF_up/sf_bTag); break;
      case k_bTagEffHFDown:
        sys_wgt *= (use_tight_bTag)? (sf_bTagEffHF_tight_dn/sf_bTag_tight) : (sf_bTagEffHF_dn/sf_bTag); break;
      case k_bTagEffLFUp:
        sys_wgt *= (use_tight_bTag)? (sf_bTagEffLF_tight_up/sf_bTag_tight) : (sf_bTagEffLF_up/sf_bTag); break;
      case k_bTagEffLFDown:
        sys_wgt *= (use_tight_bTag)? (sf_bTagEffLF_tight_dn/sf_bTag_tight) : (sf_bTagEffLF_dn/sf_bTag); break;
      case k_bTagFSEffUp:  // bTag SF FastSim
        sys_wgt *= (use_tight_bTag)? (sf_bTag_tight_FS_up/sf_bTag_tight) : (sf_bTag_FS_up/sf_bTag); break;
      case k_bTagFSEffDown:
        sys_wgt *= (use_tight_bTag)? (sf_bTag_tight_FS_dn/sf_bTag_tight) : (sf_bTag_FS_dn/sf_bTag); break;
      case k_lepSFUp:
        sys_wgt *= (sf_lep_up*sf_vetoLep_up)/(sf_lep*sf_vetoLep); break;
      case k_lepSFDown:
        sys_wgt *= (sf_lep_dn*sf_vetoLep_dn)/(sf_lep*sf_vetoLep); break;
      case k_tauSFUp:
        sys_wgt *= (sf_tau_up)/sf_tau; break;
      case k_tauSFDown:
        sys_wgt *= (sf_tau_dn)/sf_tau; break;
      case k_lepFSSFUp:  // Lepton SF FastSim/FullsSim
        sys_wgt *= (sf_lepFS_up)/sf_lepFS; break;
      case k_lepFSSFDown:
        sys_wgt *= (sf_lepFS_dn)/sf_lepFS; break;
      case k_topPtSFUp:
        sys_wgt *= (sf_topPt_up/sf_topPt); break;
      case k_topPtSFDown:
        sys_wgt *= (sf_topPt_dn/sf_topPt); break;
      case k_metResUp:
        sys_wgt *= sf_metRes_up/sf_metRes; break;
      case k_metResDown:
        sys_wgt *= sf_metRes_dn/sf_metRes; break;
      case k_metTTbarUp:
        sys_wgt *= sf_metTTbar_up/sf_metTTbar; break;
      case k_metTTbarDown:
        sys_wgt *= sf_metTTbar_dn/sf_metTTbar; break;
      case k_ttbarSysPtUp:
        sys_wgt *= (sf_ttbarSysPt_up/sf_ttbarSysPt); break;
      case k_ttbarSysPtDown:
        sys_wgt *= (sf_ttbarSysPt_dn/sf_ttbarSysPt); break;
      case k_nuPtSF_Up:
        sys_wgt *= sf_nuPt_up; break;
      case k_nuPtSF_Down:
        sys_wgt *= sf_nuPt_dn; break;
      case k_WwidthSF_Up:
        sys_wgt *= sf_Wwidth_up; break;
      case k_WwidthSF_Down:
        sys_wgt *= sf_Wwidth_dn; break;
      case k_WbXsec_Up:  // W+HF XSec Up
        sys_wgt *= sf_WbXsec_up; break;
      case k_WbXsec_Down:
        sys_wgt *= sf_WbXsec_dn; break;
      case k_pdfUp:
        sys_wgt *= sf_pdf_up; break;
      case k_pdfDown:
        sys_wgt *= sf_pdf_dn; break;
      case k_alphasUp:  // Alpha Strong Up
        sys_wgt *= sf_alphas_up; break;
      case k_alphasDown:
        sys_wgt *= sf_alphas_dn; break;
      case k_q2Up:
        sys_wgt *= sf_q2_up; break;
      case k_q2Down:
        sys_wgt *= sf_q2_dn; break;
      case k_lumiUp:
        sys_wgt *= sf_lumi_up/sf_lumi; break;
      case k_lumiDown:
        sys_wgt *= sf_lumi_dn/sf_lumi; break;
      case k_ISRUp:
        sys_wgt *= (sf_ISR_up/sf_ISR); break;
      case k_ISRDown:
        sys_wgt *= (sf_ISR_dn/sf_ISR); break;
      case k_xsecUp:
        sys_wgt *= (sf_xsec_up/xsec); break;
      case k_xsecDown:
        sys_wgt *= (sf_xsec_dn/xsec); break;
      case k_puUp:  // Pileup Up
        sys_wgt *= (sf_pu_up/sf_pu); break;
      case k_puDown:
        sys_wgt *= (sf_pu_dn/sf_pu); break;
    }

    // Corridor regions use alternate MET resolution weights
    double wgt_corridor = sys_wgt;
    if (apply_metRes_corridor_sf && apply_metRes_sf) {
      if ( iSys==k_metResUp ) wgt_corridor *= sf_metRes_corridor_up / sf_metRes_up;
      else if ( iSys==k_metResDown ) wgt_corridor *= sf_metRes_corridor_dn / sf_metRes_dn;
      else wgt_corridor *= sf_metRes_corridor / sf_metRes;
    }

    // To change the bTag scale factor if JES move it to that bin <-- this is a ugly temporary solution
    // until a better workflow can be designed
    if ( (iSys==k_JESUp && use_tight_bTag != (babyAnalyzer.jup_ntightbtags() >= 175 && babyAnalyzer.Mlb_closestb_jup() > 0)) ||
         (iSys==k_JESDown && use_tight_bTag != (babyAnalyzer.jdown_ntightbtags() >= 175 && babyAnalyzer.Mlb_closestb_jdown() > 0)) )
      sys_wgt *= (use_tight_bTag)? sf_bTag/sf_bTag_tight : sf_bTag_tight/sf_bTag;

    // Fill Array Element
    sys_wgts[iSys] = sys_wgt;
    sys_wgts_corridor[iSys] = wgt_corridor;

  } // end loop over systematics

  event_ready = true;
}

//////////////////////////////////////////////////////////////////////

void evtWgtInfo::getSusyMasses( int &mStop, int &mLSP ) {
  if ( is_fastsim_ ) {
    mStop     = babyAnalyzer.mass_stop();
    mLSP      = babyAnalyzer.mass_lsp();
  } else {
    mStop = 0;
    mLSP  = 0;
  }
}

void evtWgtInfo::getNEvents( int &nEvts ) {
  if ( is_fastsim_ ) {
    nEvts = (int) h_sig_counter_nEvents->GetBinContent(h_sig_counter->FindBin(mStop,mLSP));
  } else {
    nEvts = h_bkg_counter->GetBinContent(22);
  }
}

void evtWgtInfo::getXSecWeight( double &xsec_val, double &weight_xsec_up, double &weight_xsec_dn ) {
  if ( is_data_ ) return;

  xsec_val = 1.0;
  double xsec_err = 0.0;

  if ( !is_fastsim_ ) {
    xsec_val = babyAnalyzer.xsec();
  } else {
    // getSusyMasses(mStop,mLSP);
    xsec_val = h_sig_xsec->GetBinContent(h_sig_xsec->FindBin(mStop));
    xsec_err = h_sig_xsec->GetBinError(h_sig_xsec->FindBin(mStop));
  }

  weight_xsec_up = (xsec_val+xsec_err);
  weight_xsec_dn = (xsec_val-xsec_err);
}

void evtWgtInfo::getScaleToLumiWeight( double &wgt ) {
  wgt = 1.0;
  if ( is_data_ ) return;

  if ( is_fastsim_ ) {
    // getXSecWeight( xsec, sf_xsec_up, sf_xsec_dn );
    // getNEvents( nEvents );
    wgt = lumi*1000.0*xsec/(double)nEvents;
  } else {
    wgt = babyAnalyzer.scale1fb()*lumi;
  }
}

//////////////////////////////////////////////////////////////////////

void evtWgtInfo::getDiLepTriggerWeight( double &wgt_trigger, double &wgt_trigger_up, double &wgt_trigger_dn ) {

  wgt_trigger = 1.0;
  wgt_trigger_up = 1.0;
  wgt_trigger_dn = 1.0;

  if ( !apply_diLepTrigger_sf ) return;
  if ( is_data_ ) return;

  double sf_val = 1.0;
  double sf_err = 0.0;

  int dilep_signature = abs(babyAnalyzer.lep1_pdgid()) + abs(babyAnalyzer.lep2_pdgid());
  // DiElectron Trigger
  if ( dilep_signature == 22 ) {
    sf_val = 0.868705; // 35.876fb, reMiniAOD
    sf_err = 0.00457008;
  }
  // MuE Trigger
  else if ( dilep_signature == 24 ) {
    sf_val = 0.862895; // 35.876fb, reMiniAOD
    sf_err = 0.00204966;
  }
  // DiMuon Trigger
  else if ( dilep_signature == 26 ) {
    sf_val = 0.804283; // 35.876fb, reMiniAOD
    sf_err = 0.00266679;
  }

  // Calculate Scale Factor
  wgt_trigger    = sf_val;
  wgt_trigger_up = (sf_val + sf_err );
  wgt_trigger_dn = (sf_val - sf_err );

  return;
}


void evtWgtInfo::getCR2lTriggerWeight( double &wgt_trigger, double &wgt_trigger_up, double &wgt_trigger_dn ) {

  wgt_trigger = 1.0;
  wgt_trigger_up = 1.0;
  wgt_trigger_dn = 1.0;

  if ( !apply_cr2lTrigger_sf ) return;
  if ( is_data_ ) return;
  if ( !add2ndLepToMet ) return;

  // Flag for 1 reco lepton events, since there should be no difference
  // in the two variables since pfmet_rl is initialized with pfmet
  double eps=0.001;
  if ( babyAnalyzer.pfmet() < (babyAnalyzer.pfmet_rl()+eps) &&
       babyAnalyzer.pfmet() > (babyAnalyzer.pfmet_rl()-eps)    ) return;

  double sf_val    = 1.0;
  double sf_err_up = 0.0;
  double sf_err_dn = 0.0;

  double met = babyAnalyzer.pfmet_rl();
  // const double max_metrl = 499.9;
  const double min_metrl = 250.1;
  met = (met < min_metrl)? min_metrl : (met > max_metrl)? max_metrl : met;

  double lep_pt = babyAnalyzer.lep1_p4().Pt();
  // const double max_lep1pt = 49.9;
  const double min_lep1pt = 20.1;
  lep_pt = (lep_pt < min_lep1pt)? min_lep1pt : (lep_pt > max_lep1pt)? max_lep1pt : lep_pt;

  if ( abs(babyAnalyzer.lep1_pdgid()) == 11 ) {
    int bin   = h_cr2lTrigger_sf_el->FindFixBin( lep_pt, met );
    sf_val    = h_cr2lTrigger_sf_el->GetEfficiency( bin );
    sf_err_up = h_cr2lTrigger_sf_el->GetEfficiencyErrorUp( bin );
    sf_err_dn = h_cr2lTrigger_sf_el->GetEfficiencyErrorLow( bin );
  }
  else if ( abs(babyAnalyzer.lep1_pdgid()) == 13 ) {
    int bin   = h_cr2lTrigger_sf_mu->FindFixBin( lep_pt, met );
    sf_val    = h_cr2lTrigger_sf_mu->GetEfficiency( bin );
    sf_err_up = h_cr2lTrigger_sf_mu->GetEfficiencyErrorUp( bin );
    sf_err_dn = h_cr2lTrigger_sf_mu->GetEfficiencyErrorLow( bin );
  }

  wgt_trigger = sf_val;
  wgt_trigger_up = sf_val + sf_err_up;
  wgt_trigger_dn = sf_val - sf_err_dn;

}

//////////////////////////////////////////////////////////////////////

void evtWgtInfo::getBTagWeight( int WP, double &wgt_btagsf, double &wgt_btagsf_hf_up, double &wgt_btagsf_hf_dn, double &wgt_btagsf_lf_up, double &wgt_btagsf_lf_dn, double &wgt_btagsf_fs_up, double &wgt_btagsf_fs_dn ) {

  // Working Point, WP = {0, 1, 2} = {loose, medium, tight}
  wgt_btagsf       = 1.0;
  wgt_btagsf_hf_up = 1.0;
  wgt_btagsf_hf_dn = 1.0;
  wgt_btagsf_lf_up = 1.0;
  wgt_btagsf_lf_dn = 1.0;
  wgt_btagsf_fs_up = 1.0;
  wgt_btagsf_fs_dn = 1.0;

  if ( !apply_bTag_sf ) return;
  if ( WP < 0 || WP > 2 ) return;

  // IF deriving SFs on the Looper level
  if ( useBTagSFs_fromFiles ) {
    getBTagWeight_fromFiles( WP, wgt_btagsf, wgt_btagsf_hf_up, wgt_btagsf_hf_dn, wgt_btagsf_lf_up, wgt_btagsf_lf_dn, wgt_btagsf_fs_up, wgt_btagsf_fs_dn );
  }
  // Else taking SFs from babies
  else {
    // Loose WP
    if ( WP==0 ) {
      wgt_btagsf       = babyAnalyzer.weight_loosebtagsf();
      wgt_btagsf_hf_up = babyAnalyzer.weight_loosebtagsf_heavy_UP();
      wgt_btagsf_hf_dn = babyAnalyzer.weight_loosebtagsf_heavy_DN();
      wgt_btagsf_lf_up = babyAnalyzer.weight_loosebtagsf_light_UP();
      wgt_btagsf_lf_dn = babyAnalyzer.weight_loosebtagsf_light_DN();

      if ( is_fastsim_ && apply_bTag_sf ) {
        wgt_btagsf_fs_up = babyAnalyzer.weight_loosebtagsf_fastsim_UP();
        wgt_btagsf_fs_dn = babyAnalyzer.weight_loosebtagsf_fastsim_DN();
      }
    } // end if Loose WP

    // Medium WP
    else if ( WP==1 ) {
      wgt_btagsf       = babyAnalyzer.weight_btagsf();
      wgt_btagsf_hf_up = babyAnalyzer.weight_btagsf_heavy_UP();
      wgt_btagsf_hf_dn = babyAnalyzer.weight_btagsf_heavy_DN();
      wgt_btagsf_lf_up = babyAnalyzer.weight_btagsf_light_UP();
      wgt_btagsf_lf_dn = babyAnalyzer.weight_btagsf_light_DN();

      if ( is_fastsim_ && apply_bTag_sf ) {
        wgt_btagsf_fs_up = babyAnalyzer.weight_btagsf_fastsim_UP();
        wgt_btagsf_fs_dn = babyAnalyzer.weight_btagsf_fastsim_DN();
      }
    } // end if Medium WP

    // Tight WP
    else if ( WP==2 ) {
      wgt_btagsf       = babyAnalyzer.weight_tightbtagsf();
      wgt_btagsf_hf_up = babyAnalyzer.weight_tightbtagsf_heavy_UP();
      wgt_btagsf_hf_dn = babyAnalyzer.weight_tightbtagsf_heavy_DN();
      wgt_btagsf_lf_up = babyAnalyzer.weight_tightbtagsf_light_UP();
      wgt_btagsf_lf_dn = babyAnalyzer.weight_tightbtagsf_light_DN();

      if ( is_fastsim_ && apply_bTag_sf ) {
        wgt_btagsf_fs_up = babyAnalyzer.weight_tightbtagsf_fastsim_UP();
        wgt_btagsf_fs_dn = babyAnalyzer.weight_tightbtagsf_fastsim_DN();
      }
    } // end if Tight WP

  } // end if taking btag SFs from babies

  // Normalization
  // getNEvents( nEvents );

  // Signal Sample Normalization
  if ( is_fastsim_ ) {
    // getSusyMasses(mStop,mLSP);

    // Loose WP Signal Normalization
    if ( WP==0 ) {
      wgt_btagsf       *= ( nEvents / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,44)) );
      wgt_btagsf_hf_up *= ( nEvents / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,45)) );
      wgt_btagsf_hf_dn *= ( nEvents / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,47)) );
      wgt_btagsf_lf_up *= ( nEvents / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,46)) );
      wgt_btagsf_lf_dn *= ( nEvents / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,48)) );

      if ( apply_bTagFS_sf ) {
        wgt_btagsf_fs_up *= ( nEvents / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,49)) );
        wgt_btagsf_fs_dn *= ( nEvents / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,50)) );
      }
    } // end if Loose WP

    // Medium WP Signal Normalization
    else if ( WP==1 ) {
      wgt_btagsf       *= ( nEvents / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,14)) );
      wgt_btagsf_hf_up *= ( nEvents / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,15)) );
      wgt_btagsf_hf_dn *= ( nEvents / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,17)) );
      wgt_btagsf_lf_up *= ( nEvents / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,16)) );
      wgt_btagsf_lf_dn *= ( nEvents / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,18)) );

      if ( apply_bTagFS_sf ) {
        wgt_btagsf_fs_up *= ( nEvents / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,22)) );
        wgt_btagsf_fs_dn *= ( nEvents / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,23)) );
      }
    } // end if Medium WP

    // Tight WP Signal Normalization
    else if ( WP==2 ) {
      wgt_btagsf       *= ( nEvents / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,37)) );
      wgt_btagsf_hf_up *= ( nEvents / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,38)) );
      wgt_btagsf_hf_dn *= ( nEvents / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,40)) );
      wgt_btagsf_lf_up *= ( nEvents / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,39)) );
      wgt_btagsf_lf_dn *= ( nEvents / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,41)) );

      if ( apply_bTagFS_sf ) {
        wgt_btagsf_fs_up *= ( nEvents / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,42)) );
        wgt_btagsf_fs_dn *= ( nEvents / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,43)) );
      }
    } // end if Tight WP

  } // end if normalizing signal

  // Background Sample Normalization
  else {
    // Loose WP Bkg Normalization
    if ( WP==0 ) {
      wgt_btagsf       *= ( nEvents / h_bkg_counter->GetBinContent(44) );
      wgt_btagsf_hf_up *= ( nEvents / h_bkg_counter->GetBinContent(45) );
      wgt_btagsf_hf_dn *= ( nEvents / h_bkg_counter->GetBinContent(47) );
      wgt_btagsf_lf_up *= ( nEvents / h_bkg_counter->GetBinContent(46) );
      wgt_btagsf_lf_dn *= ( nEvents / h_bkg_counter->GetBinContent(48) );
    } // end if Loose WP

    // Medium WP Bkg Normalization
    else if ( WP==1 ) {
      wgt_btagsf       *= ( nEvents / h_bkg_counter->GetBinContent(14) );
      wgt_btagsf_hf_up *= ( nEvents / h_bkg_counter->GetBinContent(15) );
      wgt_btagsf_hf_dn *= ( nEvents / h_bkg_counter->GetBinContent(17) );
      wgt_btagsf_lf_up *= ( nEvents / h_bkg_counter->GetBinContent(16) );
      wgt_btagsf_lf_dn *= ( nEvents / h_bkg_counter->GetBinContent(18) );
    } // end if Medium WP

    // Tight WP Bkg Normalization
    else if ( WP==2 ) {
      wgt_btagsf       *= ( nEvents / h_bkg_counter->GetBinContent(37) );
      wgt_btagsf_hf_up *= ( nEvents / h_bkg_counter->GetBinContent(38) );
      wgt_btagsf_hf_dn *= ( nEvents / h_bkg_counter->GetBinContent(40) );
      wgt_btagsf_lf_up *= ( nEvents / h_bkg_counter->GetBinContent(39) );
      wgt_btagsf_lf_dn *= ( nEvents / h_bkg_counter->GetBinContent(41) );
    } // end if Tight WP

  } // end if normalizing bkg

}

//////////////////////////////////////////////////////////////////////

void evtWgtInfo::getBTagWeight_fromFiles( int WP, double &wgt_btagsf, double &wgt_btagsf_hf_up, double &wgt_btagsf_hf_dn, double &wgt_btagsf_lf_up, double &wgt_btagsf_lf_dn, double &wgt_btagsf_fs_up, double &wgt_btagsf_fs_dn ) {

  vector< double > jet_pt;
  vector< double > jet_eta;
  vector< double > jet_deepCSV;
  vector< int >    jet_flavour;

  for (int iJet=0; iJet<(int)babyAnalyzer.ak4pfjets_p4().size(); iJet++) {
    jet_pt.push_back( (double)babyAnalyzer.ak4pfjets_p4().at(iJet).Pt() );
    jet_eta.push_back( (double)babyAnalyzer.ak4pfjets_p4().at(iJet).Eta() );
    jet_deepCSV.push_back( (double)babyAnalyzer.ak4pfjets_deepCSV().at(iJet) );
    jet_flavour.push_back( (int)abs(babyAnalyzer.ak4pfjets_hadron_flavor().at(iJet)) );
  }

  bTagSFUtil->getBTagWeight( WP, jet_pt, jet_eta, jet_deepCSV, jet_flavour, wgt_btagsf, wgt_btagsf_hf_up, wgt_btagsf_hf_dn, wgt_btagsf_lf_up, wgt_btagsf_lf_dn, wgt_btagsf_fs_up, wgt_btagsf_fs_dn );

}

//////////////////////////////////////////////////////////////////////

void evtWgtInfo::getLepSFWeight( double &weight_lepSF, double &weight_lepSF_Up, double &weight_lepSF_Dn, double &weight_lepFSSF, double &weight_lepFSSF_Up, double &weight_lepFSSF_Dn, double &weight_vetoLepSF, double &weight_vetoLepSF_Up, double &weight_vetoLepSF_Dn ) {

  weight_lepSF = 1.0;
  weight_lepSF_Up = 1.0;
  weight_lepSF_Dn = 1.0;
  weight_lepFSSF = 1.0;
  weight_lepFSSF_Up = 1.0;
  weight_lepFSSF_Dn = 1.0;
  weight_vetoLepSF = 1.0;
  weight_vetoLepSF_Up = 1.0;
  weight_vetoLepSF_Dn = 1.0;

  if ( !apply_lep_sf && !apply_vetoLep_sf && !apply_lepFS_sf ) return;

  if ( useLepSFs_fromFiles ) {
    getLepSFWeight_fromFiles( weight_lepSF, weight_lepSF_Up, weight_lepSF_Dn, weight_lepFSSF, weight_lepFSSF_Up, weight_lepFSSF_Dn, weight_vetoLepSF, weight_vetoLepSF_Up, weight_vetoLepSF_Dn );
  } else {
    weight_lepSF = babyAnalyzer.weight_lepSF();
    weight_lepSF_Up = babyAnalyzer.weight_lepSF_up();
    weight_lepSF_Dn = babyAnalyzer.weight_lepSF_down();

    if ( apply_vetoLep_sf ) {
      weight_vetoLepSF = babyAnalyzer.weight_vetoLepSF();
      weight_vetoLepSF_Up = babyAnalyzer.weight_vetoLepSF_up();
      weight_vetoLepSF_Dn = babyAnalyzer.weight_vetoLepSF_down();
    }
    if ( apply_lepFS_sf ) {
      weight_lepFSSF = babyAnalyzer.weight_lepSF_fastSim();
      weight_lepFSSF_Up = babyAnalyzer.weight_lepSF_fastSim_up();
      weight_lepFSSF_Dn = babyAnalyzer.weight_lepSF_fastSim_down();
    }

    // getNEvents( nEvents );
    // getSusyMasses(mStop,mLSP);

    if ( is_fastsim_ ) {
      weight_lepSF    *= ( nEvents / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,27)) );
      weight_lepSF_Up *= ( nEvents / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,28)) );
      weight_lepSF_Dn *= ( nEvents / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,29)) );

      if ( apply_vetoLep_sf ) {
        weight_vetoLepSF    *= ( nEvents / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,30)) );
        weight_vetoLepSF_Up *= ( nEvents / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,31)) );
        weight_vetoLepSF_Dn *= ( nEvents / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,32)) );
      }
      if ( apply_lepFS_sf ) {
        weight_lepFSSF    *= ( nEvents / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,33)) );
        weight_lepFSSF_Up *= ( nEvents / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,34)) );
        weight_lepFSSF_Dn *= ( nEvents / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,35)) );
      }

    } else {
      weight_lepSF    *= ( nEvents / h_bkg_counter->GetBinContent(h_bkg_counter->FindBin(28)) );
      weight_lepSF_Up *= ( nEvents / h_bkg_counter->GetBinContent(h_bkg_counter->FindBin(29)) );
      weight_lepSF_Dn *= ( nEvents / h_bkg_counter->GetBinContent(h_bkg_counter->FindBin(30)) );

      if ( apply_vetoLep_sf ) {
        weight_vetoLepSF    *= ( nEvents / h_bkg_counter->GetBinContent(h_bkg_counter->FindBin(31)) );
        weight_vetoLepSF_Up *= ( nEvents / h_bkg_counter->GetBinContent(h_bkg_counter->FindBin(32)) );
        weight_vetoLepSF_Dn *= ( nEvents / h_bkg_counter->GetBinContent(h_bkg_counter->FindBin(33)) );
      }
    }
  }

  if ( !apply_lep_sf ) {
    weight_lepSF = 1.0;
    weight_lepSF_Up = 1.0;
    weight_lepSF_Dn = 1.0;
  }
  if ( !apply_vetoLep_sf ) {
    weight_vetoLepSF = 1.0;
    weight_vetoLepSF_Up = 1.0;
    weight_vetoLepSF_Dn = 1.0;
  }
  if ( !apply_lepFS_sf || !is_fastsim_ ) {
    weight_lepFSSF = 1.0;
    weight_lepFSSF_Up = 1.0;
    weight_lepFSSF_Dn = 1.0;
  }

}

//////////////////////////////////////////////////////////////////////

void evtWgtInfo::getLepSFWeight_fromFiles( double &weight_lepSF, double &weight_lepSF_Up, double &weight_lepSF_Dn, double &weight_lepFSSF, double &weight_lepFSSF_Up, double &weight_lepFSSF_Dn, double &weight_vetoLepSF, double &weight_vetoLepSF_Up, double &weight_vetoLepSF_Dn ) {

  weight_lepSF = 1.0;
  weight_lepSF_Up = 1.0;
  weight_lepSF_Dn = 1.0;
  weight_lepFSSF = 1.0;
  weight_lepFSSF_Up = 1.0;
  weight_lepFSSF_Dn = 1.0;
  weight_vetoLepSF = 1.0;
  weight_vetoLepSF_Up = 1.0;
  weight_vetoLepSF_Dn = 1.0;

  if ( is_data_ ) return;

  std::vector< float > recoLep_pt, recoLep_eta, genLostLep_pt, genLostLep_eta;
  std::vector< int > recoLep_pdgid, genLostLep_pdgid;
  std::vector< bool > recoLep_isSel;

  const double matched_dr = 0.1;

  if ( babyAnalyzer.nvetoleps()>=1 ) {
    recoLep_pt.push_back( babyAnalyzer.lep1_p4().Pt() );
    recoLep_eta.push_back( babyAnalyzer.lep1_p4().Eta() );
    recoLep_pdgid.push_back( babyAnalyzer.lep1_pdgid() );
    if ( ( (abs(babyAnalyzer.lep1_pdgid())==13 && babyAnalyzer.lep1_passMediumID()) ||
           (abs(babyAnalyzer.lep1_pdgid())==11 && babyAnalyzer.lep1_passMediumID()) ) &&
         babyAnalyzer.lep1_p4().Pt()>20.0 && fabs(babyAnalyzer.lep1_p4().Eta())<2.4 ) {
      recoLep_isSel.push_back( true );
    } else {
      recoLep_isSel.push_back( false );
    }

    if (babyAnalyzer.nvetoleps()>=2) {
      recoLep_pt.push_back( babyAnalyzer.lep2_p4().Pt() );
      recoLep_eta.push_back( babyAnalyzer.lep2_p4().Eta() );
      recoLep_pdgid.push_back( babyAnalyzer.lep2_pdgid() );
      if ( ( (abs(babyAnalyzer.lep2_pdgid())==13 && babyAnalyzer.lep2_passMediumID()) ||
             (abs(babyAnalyzer.lep2_pdgid())==11 && babyAnalyzer.lep2_passMediumID()) ) &&
           babyAnalyzer.lep2_p4().Pt()>20.0 && fabs(babyAnalyzer.lep2_p4().Eta())<2.4 ) {
        recoLep_isSel.push_back( true );
      } else {
        recoLep_isSel.push_back( false );
      }

    } // end if >=2 vetoLeptons

  } // end if >=1 vetoLeptons


  // Find Gen Lost lepton
  if ( babyAnalyzer.is2lep() && babyAnalyzer.nvetoleps()==1 ) {

    // Find gen lepton matched to reco lepton
    for (int iGen=0; iGen<(int)babyAnalyzer.genleps_p4().size(); iGen++) {

      if ( abs(babyAnalyzer.genleps_id().at(iGen))!=11 && abs(babyAnalyzer.genleps_id().at(iGen))!=13 ) continue;
      if ( !babyAnalyzer.genleps_fromHardProcessFinalState().at(iGen) ) continue;
      if ( !babyAnalyzer.genleps_isLastCopy().at(iGen) ) continue;
      if ( ROOT::Math::VectorUtil::DeltaR(babyAnalyzer.genleps_p4().at(iGen), babyAnalyzer.lep1_p4())<matched_dr ) continue;
      if ( babyAnalyzer.genleps_p4().at(iGen).Pt()<5 || fabs(babyAnalyzer.genleps_p4().at(iGen).Eta())>2.4 ) continue;

      genLostLep_pt.push_back( babyAnalyzer.genleps_p4().at(iGen).Pt() );
      genLostLep_eta.push_back( babyAnalyzer.genleps_p4().at(iGen).Eta() );
      genLostLep_pdgid.push_back( babyAnalyzer.genleps_id().at(iGen) );

    } // end loop over genleps

  } // end if lostLep event

  lepSFUtil->getLepWeight( recoLep_pt, recoLep_eta, recoLep_pdgid, recoLep_isSel, genLostLep_pt, genLostLep_eta, genLostLep_pdgid, weight_lepSF, weight_lepSF_Up, weight_lepSF_Dn, weight_lepFSSF, weight_lepFSSF_Up, weight_lepFSSF_Dn, weight_vetoLepSF, weight_vetoLepSF_Up, weight_vetoLepSF_Dn );

}

//////////////////////////////////////////////////////////////////////

void evtWgtInfo::getTauSFWeight( double &weight_tau, double &weight_tau_up, double &weight_tau_dn ) {

  weight_tau = 1.0;
  weight_tau_up = 1.0;
  weight_tau_dn = 1.0;

  if ( !apply_tau_sf ) return;
  if ( is_data_ ) return;
  if ( !babyAnalyzer.is2lep() ) return;

  // Check if there is had tau in event
  for (int iGen=0; iGen<(int)babyAnalyzer.genleps_p4().size(); iGen++) {
    if ( abs(babyAnalyzer.genleps_id().at(iGen))!=15 ) continue; // Looking for a tau
    if ( !babyAnalyzer.genleps_isLastCopy().at(iGen) ) continue; // Must be last copy in truth history
    if ( !babyAnalyzer.genleps_fromHardProcessFinalState().at(iGen) && !babyAnalyzer.genleps_fromHardProcessDecayed().at(iGen) ) continue; // Must be from hard process
    if ( babyAnalyzer.genleps_gentaudecay().at(iGen)!=3 && babyAnalyzer.genleps_gentaudecay().at(iGen)!=4 ) continue; // Must be hadronically decaying
    if ( babyAnalyzer.genleps_p4().at(iGen).Pt()<20.0 ) continue; // Must be in pT acceptance
    if ( fabs(babyAnalyzer.genleps_p4().at(iGen).Eta())>2.3 ) continue; // Must be in eta acceptance

    // Get reco tau eff
    int binX = h_recoEff_tau->GetXaxis()->FindBin( babyAnalyzer.genleps_p4().at(iGen).Pt() );
    int binY = h_recoEff_tau->GetYaxis()->FindBin( fabs(babyAnalyzer.genleps_p4().at(iGen).Eta()) );
    double eff = h_recoEff_tau->GetBinContent( binX, binY );

    // Old cutbase medium
    double sf = 0.91;
    double sf_err = 0.05;
    // MVA medium
    if (year == 2016) {
      sf = 0.97;
      sf_err = 0.05;
    } else if (year == 2017) {
      sf = 0.89;
      sf_err = 0.03;
    }

    double sf_up = sf + sf_err;
    double sf_dn = sf - sf_err;

    if ( eff>0.0 && babyAnalyzer.PassTrackVeto() ) {
      if ( babyAnalyzer.PassTauVeto() ) {
        // If passes tau filter, means tau was lost
        weight_tau = (1.0-(eff*sf))/(1.0-eff);
        weight_tau_up = (1.0-(eff*sf_up))/(1.0-eff);
        weight_tau_dn = (1.0-(eff*sf_dn))/(1.0-eff);
      } else {
        // Else, tau was found
        weight_tau = sf;
        weight_tau_up = sf_up;
        weight_tau_dn = sf_dn;
      }
    } // end if eff>0.0
  } // end loop over gen leps

}

//////////////////////////////////////////////////////////////////////

void evtWgtInfo::getTopPtWeight( double &weight_topPt, double &weight_topPt_up, double &weight_topPt_dn ) {

  weight_topPt    = 1.0;
  weight_topPt_up = 1.0;
  weight_topPt_dn = 1.0;

  if ( is_data_ ) return;
  // if ( sampletype != "ttbar" ) return;

  double genTopPt    = 0.0;
  double genTopBarPt = 0.0;
  double nTopsFound = 0;
  for (int iTop=0; iTop<(int)babyAnalyzer.genqs_p4().size(); iTop++) {
    if (!babyAnalyzer.genqs_isLastCopy().at(iTop)) continue;
    if (babyAnalyzer.genqs_id().at(iTop)==6) {
      genTopPt = babyAnalyzer.genqs_p4().at(iTop).Pt();
      nTopsFound++;
    }
    if (babyAnalyzer.genqs_id().at(iTop)==-6) {
      genTopBarPt = babyAnalyzer.genqs_p4().at(iTop).Pt();
      nTopsFound++;
    }
  }

  if ( !(genTopPt>0.0) )    cout << "    Did not find gen top for top pT reweighting" << endl;
  if ( !(genTopBarPt>0.0) ) cout << "    Did not find gen topBar for top pT reweighting" << endl;
  if ( nTopsFound != 2 )    cout << "    Did not find 2 tops for top pT reweighting, nFound=" << nTopsFound << endl;

  double weight_temp = topPtWeight(genTopPt, genTopBarPt);

  if ( apply_topPt_sf ) {
    weight_topPt    = weight_temp;
    weight_topPt_up = ( ((weight_temp-1.0)*2.0)+1.0 );
    weight_topPt_dn = 1.0;
  }
  // if !apply_topPt_sf still use as uncertainty
  else {
    weight_topPt    = 1.0;
    weight_topPt_up = weight_temp;
    weight_topPt_dn = 1.0;
  }

}

//////////////////////////////////////////////////////////////////////

void evtWgtInfo::getMetResWeight( double &weight_metRes, double &weight_metRes_up, double &weight_metRes_dn ) {

  // What is this for? How is it calculated? Why does this need to be applied in the looper not just scale afterwards?

  weight_metRes    = 1.0;
  weight_metRes_up = 1.0;
  weight_metRes_dn = 1.0;

  if (!apply_metRes_sf) return;

  double sf_val = 1.0;
  double sf_err = 0.0;

  if (!is_bkg_) return;

  // ttbar, tW, wjets
  // if ( sampletype != "ttbar" && sampletype != "singletop" && sampletype != "wjets" ) return;

  // 2lep events only
  // if ( !babyAnalyzer.is2lep() ) return;

  // nJets bin
  int nJets = babyAnalyzer.ngoodjets();
  //in case of evaluating for JES variatione events, use nominal nJets, is <2, then use 2jet
  if ( nJets<2 ) nJets = 2;

  // modTopness bin
  double modTop = babyAnalyzer.topnessMod();
  if ( add2ndLepToMet ) modTop = babyAnalyzer.topnessMod_rl();

  // mlb bin
  double mlb = babyAnalyzer.Mlb_closestb();
  int nMedBTags = babyAnalyzer.ngoodbtags();

  int nTightTags = babyAnalyzer.ntightbtags();

  bool is0b = ( (nMedBTags==0) || (nMedBTags>=1 && nTightTags==0 && mlb>175.0) );
  if ( is0b ) mlb = ( babyAnalyzer.lep1_p4() + babyAnalyzer.ak4pfjets_leadbtag_p4() ).M();

  // Met bin
  double met = babyAnalyzer.pfmet();
  if (add2ndLepToMet) met = babyAnalyzer.pfmet_rl();

  // in case of evaluating for JES variation events, then use nominal pfmet, and if nominal<250, use lowest met bin
  if ( met<250.0 ) met = 250.0;

  //
  // Find SF
  //

  // Region A
  if ( nJets<4 && modTop>=10 && mlb<175 ) {

    if ( met>=250.0 && met<350.0 ) {
      sf_val = 0.99;
      sf_err = 0.01;
    } else if ( met>=350.0 && met<450.0 ) {
      sf_val = 1.04;
      sf_err = 0.01;
    } else if ( met>=450.0 && met<600.0 ) {
      sf_val = 1.11;
      sf_err = 0.01;
    } else if ( met>=600.0 ) {
      sf_val = 1.17;
      sf_err = 0.03;
    } else {  // shouldn't get here
      sf_val = 1.00;
      sf_err = 0.00;
    }
  } // end if region A


  // Region B
  if ( nJets<4 && modTop>=10 && mlb>=175 ) {

    if ( met>=250.0 && met<450.0 ) {
      sf_val = 1.00;
      sf_err = 0.01;
    } else if ( met>=450.0 && met<600.0 ) {
      sf_val = 1.11;
      sf_err = 0.01;
    } else if ( met>=600.0 ) {
      sf_val = 1.17;
      sf_err = 0.02;
    }

  } // end if region B


  // Region C
  if ( nJets>=4 && modTop<0.0 && mlb<175 ) {

    if ( met>=250.0 && met<350.0 ) {
      sf_val = 0.99;
      sf_err = 0.01;
    } else if ( met>=350.0 && met<450.0 ) {
      sf_val = 1.05;
      sf_err = 0.01;
    } else if ( met>=450.0 && met<550.0 ) {
      sf_val = 1.07;
      sf_err = 0.04;
    } else if ( met>=550.0 && met<650.0 ) {
      sf_val = 1.10;
      sf_err = 0.04;
    } else if ( met>=650.0 ) {
      sf_val = 1.14;
      sf_err = 0.05;
    }

  } // end if region C


  // Region D
  if ( nJets>=4 && modTop<0.0 && mlb>=175.0 ) {

    if ( met>=250.0 && met<350.0 ) {
      sf_val = 0.99;
      sf_err = 0.01;
    } else if ( met>=350.0 && met<450.0 ) {
      sf_val = 1.05;
      sf_err = 0.01;
    } else if ( met>=450.0 && met<550.0 ) {
      sf_val = 1.07;
      sf_err = 0.04;
    } else if ( met>=550.0 ) {
      sf_val = 1.11;
      sf_err = 0.03;
    }

  } // end if region D


  // Region E
  if ( nJets>=4 && modTop>=0.0 && modTop<10.0 && mlb<175 ) {

    if ( met>=250.0 && met<350.0 ) {
      sf_val = 0.98;
      sf_err = 0.01;
    } else if ( met>=350.0 && met<550.0 ) {
      sf_val = 1.06;
      sf_err = 0.01;
    } else if ( met>=550.0 ) {
      sf_val = 1.10;
      sf_err = 0.03;
    }

  } // end if region E


  // Region F
  if ( nJets>=4 && modTop>=0.0 && modTop<10.0 && mlb>=175 ) {

    if ( met>=250.0 && met<450.0 ) {
      sf_val = 0.99;
      sf_err = 0.01;
    } else if ( met>=450.0 ) {
      sf_val = 1.07;
      sf_err = 0.02;
    }

  } // end if region F


  // Region G
  if ( nJets>=4 && modTop>=10.0 && mlb<175 ) {

    if ( met>=250.0 && met<350.0 ) {
      sf_val = 0.98;
      sf_err = 0.01;
    } else if ( met>=350.0 && met<450.0 ) {
      sf_val = 1.06;
      sf_err = 0.01;
    } else if ( met>=450.0 && met<600.0 ) {
      sf_val = 1.07;
      sf_err = 0.02;
    } else if ( met>=600.0 ) {
      sf_val = 1.08;
      sf_err = 0.04;
    }

  } // end if region G


  // Region H
  if ( nJets>=4 && modTop>=10.0 && mlb>=175 ) {

    if ( met>=250.0 && met<450.0 ) {
      sf_val = 0.99;
      sf_err = 0.01;
    } else if ( met>=450.0 ) {
      sf_val = 1.07;
      sf_err = 0.02;
    }

  } // end if region H

  // 50% uncertainty on difference between no sf and applying it
  sf_err = fabs(0.5*(1.0-sf_val));

  weight_metRes    = sf_val;
  weight_metRes_up = (sf_val + sf_err);
  weight_metRes_dn = (sf_val - sf_err);

}

//////////////////////////////////////////////////////////////////////

void evtWgtInfo::getMetResWeight_corridor( double &weight_metRes, double &weight_metRes_up, double &weight_metRes_dn ) {

  weight_metRes    = 1.0;
  weight_metRes_up = 1.0;
  weight_metRes_dn = 1.0;

  if (!apply_metRes_sf) return;


  double sf_val = 1.0;
  double sf_err = 0.0;

  if ( is_data_ ) return;
  if ( is_fastsim_ ) return;

  // ttbar, tW
  if ( samptype != ttbar && samptype != singletop && samptype != Wjets ) return;

  // 2lep events only
  //if ( !babyAnalyzer.is2lep() ) return;

  // Met bin
  double met = babyAnalyzer.pfmet();
  if (add2ndLepToMet) met = babyAnalyzer.pfmet_rl();

  // in case of evaluating for JES variation events, then use nominal pfmet, and if nominal<250, use lowest met bin
  if ( met<250.0 ) met = 250.0;

  //
  // Find SF
  //

  if ( met>=250.0 && met<350.0 ) {
    sf_val = 0.98;
    sf_err = 0.02;
  } else if ( met>=350.0 && met<450.0 ) {
    sf_val = 1.04;
    sf_err = 0.02;
  } else if ( met>=450.0 && met<550.0 ) {
    sf_val = 1.07;
    sf_err = 0.04;
  } else if ( met>=550.0 ) {
    sf_val = 1.07;
    sf_err = 0.05;
  }

  // 50% uncertainty on difference between no sf and applying it
  sf_err = fabs(0.5*(1.0-sf_val));

  weight_metRes    = sf_val;
  weight_metRes_up = (sf_val + sf_err);
  weight_metRes_dn = (sf_val - sf_err);

  return;
}

//////////////////////////////////////////////////////////////////////

void evtWgtInfo::getMetTTbarWeight( double &weight_metTTbar, double &weight_metTTbar_up, double &weight_metTTbar_dn ) {

  weight_metTTbar    = 1.0;
  weight_metTTbar_up = 1.0;
  weight_metTTbar_dn = 1.0;

  if (!apply_metTTbar_sf) return;

  double sf_val = 1.0;
  double sf_err = 0.0;

  if ( is_data_ ) return;
  if ( is_fastsim_ ) return;

  // // ttbar, tW
  // if ( sampletype != "ttbar" && sampletype != "singletop") return;

  // 2lep events only
  if ( !babyAnalyzer.is2lep() ) return;

  int nGoodJets = babyAnalyzer.ngoodjets();

  double modTopness = babyAnalyzer.topnessMod();
  if ( add2ndLepToMet) modTopness = babyAnalyzer.topnessMod_rl();

  double mlb = babyAnalyzer.Mlb_closestb();

  int nTightTags = babyAnalyzer.ntightbtags();

  double met = babyAnalyzer.pfmet();
  if (add2ndLepToMet) met = babyAnalyzer.pfmet_rl();

  //
  // Find SF
  //

  // Region B
  if ( nGoodJets<4 && modTopness>=10.0 && mlb>=175.0 && nTightTags>=1 ) {
    if ( met>450.0 && met<600.0 ) {
      sf_val = 1.00;
      sf_err = 0.24;
    } else if ( met>=600.0 ) {
      sf_val = 0.99;
      sf_err = 0.39;
    }
  }

  // Region E
  if ( nGoodJets>=4 && modTopness>=0.0 && modTopness<10.0 && mlb<175.0 ) {
    if ( met>350.0 && met<550.0 ) {
      sf_val = 1.04;
      sf_err = 0.10;
    } else if ( met>=550.0 ) {
      sf_val = 0.62;
      sf_err = 0.18;
    }
  }

  // Region F
  if ( nGoodJets>=4 && modTopness>=0.0 && modTopness<10.0 && mlb>=175.0 && nTightTags>=1 ) {
    if ( met>250.0 && met<450.0 ) {
      sf_val = 1.02;
      sf_err = 0.05;
    } else if ( met>=450.0 ) {
      sf_val = 0.62;
      sf_err = 0.10;
    }
  }

  // Region H
  if ( nGoodJets>=4 && modTopness>=10.0 && mlb>=175.0 && nTightTags>=1) {
    if ( met>250.0 && met<450.0 ) {
      sf_val = 1.02;
      sf_err = 0.05;
    } else if ( met>=450.0 ) {
      sf_val = 0.62;
      sf_err = 0.10;
    }
  }

  // 50% uncertainty on difference between no sf and applying it
  //sf_err = fabs(0.5*(1.0-sf_val));

  weight_metTTbar    = sf_val;
  weight_metTTbar_up = (sf_val + sf_err);
  weight_metTTbar_dn = (sf_val - sf_err);

  return;
}

//////////////////////////////////////////////////////////////////////

void evtWgtInfo::getTTbarSysPtSF( double &weight_ttbarSysPt, double &weight_ttbarSysPt_up, double &weight_ttbarSysPt_dn ) {

  weight_ttbarSysPt    = 1.0;
  weight_ttbarSysPt_up = 1.0;
  weight_ttbarSysPt_dn = 1.0;

  double sf_val = 1.0;
  double sf_err = 0.0;

  if ( !babyAnalyzer.is2lep() ) return;

  // Get ttbar/tW system pT
  LorentzVector system_LV(0.0,0.0,0.0,0.0);

  // Lep1 LV
  system_LV += babyAnalyzer.lep1_p4();

  // Lep2 LV, if available
  if ( babyAnalyzer.nvetoleps()>1 ) system_LV += babyAnalyzer.lep2_p4();

  // Highest DeepCSV Jet
  int jet1_idx = -1;
  double max_deepcsv = -99.9;
  for (int iJet=0; iJet<(int)babyAnalyzer.ak4pfjets_p4().size(); iJet++) {
    if ( babyAnalyzer.ak4pfjets_deepCSV().at(iJet) > max_deepcsv ) {
      jet1_idx = iJet;
      max_deepcsv  = babyAnalyzer.ak4pfjets_deepCSV().at(iJet);
    }
  }
  if (jet1_idx>=0) system_LV += babyAnalyzer.ak4pfjets_p4().at(jet1_idx);

  // 2nd Highest DeepCSV Jets
  int jet2_idx = -1;
  max_deepcsv = -99.9;
  for (int iJet=0; iJet<(int)babyAnalyzer.ak4pfjets_p4().size(); iJet++) {
    if ( iJet==jet1_idx ) continue;
    if ( babyAnalyzer.ak4pfjets_deepCSV().at(iJet) > max_deepcsv ) {
      jet2_idx = iJet;
      max_deepcsv = babyAnalyzer.ak4pfjets_deepCSV().at(iJet);
    }
  }
  if (jet2_idx>=0) system_LV += babyAnalyzer.ak4pfjets_p4().at(jet2_idx);

  // Met
  LorentzVector met_TLV( babyAnalyzer.pfmet()*cos(babyAnalyzer.pfmet_phi()), babyAnalyzer.pfmet()*sin(babyAnalyzer.pfmet_phi()), 0.0, babyAnalyzer.pfmet() );
  system_LV += met_TLV;

  // Get system Pt
  double system_pt = system_LV.Pt();

  const vector<double> ptcats = {   0,   50,  100,  150,  200,  250,  350,  450, };
  const vector<double> sfvals = {1.02, 0.99, 0.98, 0.95, 0.99, 1.01, 1.07, 1.09, };
  const vector<double> sferrs = {0.01, 0.01, 0.01, 0.01, 0.02, 0.02, 0.04, 0.05, };

  int icat = std::upper_bound(ptcats.begin(), ptcats.end(), system_pt) - ptcats.begin() - 1;

  // true=sf, false=uncertainty only
  if (!apply_ttbarSysPt_sf) sf_val = 1.0;
  else sf_val = sfvals[icat];
  sf_err = sferrs[icat];

  weight_ttbarSysPt    = sf_val;
  weight_ttbarSysPt_up = (sf_val + sf_err );
  weight_ttbarSysPt_dn = (sf_val - sf_err );

}

//////////////////////////////////////////////////////////////////////

void evtWgtInfo::getNuPtSF( double &weight_nuPt_up, double &weight_nuPt_dn ) {

  weight_nuPt_up = 1.0;
  weight_nuPt_dn = 1.0;

  double sf_val = 1.0;
  double sf_err = 0.0;

  if ( !babyAnalyzer.is1lepFromW() ) return;

  if ( babyAnalyzer.ngoodjets()==2 &&
       babyAnalyzer.topnessMod()>=6.4 ) {

    if ( babyAnalyzer.pfmet()>=250.0 ) sf_err = 0.0654;
    if ( babyAnalyzer.pfmet()>=350.0 ) sf_err = 0.1796;

  } // end if nJets==2 modTop>6.4

  if ( babyAnalyzer.ngoodjets()==3 &&
       babyAnalyzer.MT2W()>=200.0     ) {

    if ( babyAnalyzer.pfmet()>=250.0 ) sf_err = 0.1310;
    if ( babyAnalyzer.pfmet()>=350.0 ) sf_err = 0.1530;

  } // end if nJets==3 && MT2W>200

  if ( babyAnalyzer.ngoodjets()>=4 &&
       babyAnalyzer.MT2W()>=200.0     ) {

    if ( babyAnalyzer.pfmet()>=250.0 ) sf_err = 0.1280;
    if ( babyAnalyzer.pfmet()>=350.0 ) sf_err = 0.1840;
    if ( babyAnalyzer.pfmet()>=450.0 ) sf_err = 0.2220;

  } // end if nJets>=4 && MT2W>200

  weight_nuPt_up = (sf_val + sf_err);
  weight_nuPt_dn = (sf_val - sf_err);

}

//////////////////////////////////////////////////////////////////////

void evtWgtInfo::getWwidthSF( double &weight_Wwidth_up, double &weight_Wwidth_dn ) {

  weight_Wwidth_up = 1.0;
  weight_Wwidth_dn = 1.0;

  double sf_val = 1.0;
  double sf_err = 0.0;

  if ( !babyAnalyzer.is1lepFromW() ) return;

  if ( babyAnalyzer.ngoodjets()==2 &&
       babyAnalyzer.topnessMod()>=6.4 ) {

    sf_err = 0.03;
  } // end if nJets==2 modTop>6.4
  else if ( babyAnalyzer.ngoodjets()==3 &&
            babyAnalyzer.MT2W()>=200.0     ) {

    sf_err = 0.03;
  } // end if nJets==3 && MT2W>200
  else if ( babyAnalyzer.ngoodjets()>=4 &&
            babyAnalyzer.MT2W()>=200.0     ) {

    sf_err = 0.03;
  } // end if nJets>=4 && MT2W>200

  weight_Wwidth_up = (sf_val + sf_err);
  weight_Wwidth_dn = (sf_val - sf_err);

}

//////////////////////////////////////////////////////////////////////

void evtWgtInfo::getWbXSecSF( double &weight_WbXsec_up, double &weight_WbXsec_dn ) {

  weight_WbXsec_up = 1.0;
  weight_WbXsec_dn = 1.0;

  double sf_val = 1.0;
  double sf_err = 0.0;

  if ( !babyAnalyzer.is1lepFromW() ) return;

  bool isWHF = false;
  // Question: Is this the correct way to get events from W+b production?
  // TODO: check how was pfjet->hadronFlavour() assigned
  for (auto hadFlavor : babyAnalyzer.ak4pfjets_hadron_flavor()) {
    if (abs(hadFlavor) == 5) {
      isWHF = true;
      break;
    } // end if W+bjets
  } // end loop over jets

  if ( !isWHF ) return;

  sf_err = 0.50;

  weight_WbXsec_up = (sf_val + sf_err);
  weight_WbXsec_dn = (sf_val - sf_err);

}

//////////////////////////////////////////////////////////////////////

void evtWgtInfo::getPDFWeight( double &weight_pdf_up, double &weight_pdf_dn ) {

  if ( babyAnalyzer.genweights().size() < 110 ) return;

  weight_pdf_up = babyAnalyzer.pdf_up_weight();
  weight_pdf_dn = babyAnalyzer.pdf_down_weight();

  // Normalization
  if ( is_fastsim_ ) {
    weight_pdf_up *= (nEvents/h_sig_counter->GetBinContent(mStop,mLSP,10));
    weight_pdf_dn *= (nEvents/h_sig_counter->GetBinContent(mStop,mLSP,11));
  } else {
    weight_pdf_up *= (nEvents/h_bkg_counter->GetBinContent(10));
    weight_pdf_dn *= (nEvents/h_bkg_counter->GetBinContent(11));
  }

  if ( weight_pdf_up<0.0 ) weight_pdf_up = fabs(weight_pdf_up);
  if ( weight_pdf_dn<0.0 ) weight_pdf_dn = fabs(weight_pdf_dn);

}

//////////////////////////////////////////////////////////////////////

void evtWgtInfo::getAlphasWeight( double &weight_alphas_up, double &weight_alphas_dn ) {

  if ( babyAnalyzer.genweights().size() < 110 ) return;

  if (sync16) {  // Old method, to be used before v27
    if ( is_fastsim_ ) {
      weight_alphas_up = babyAnalyzer.genweights().at(110)/babyAnalyzer.genweights().at(1);
      weight_alphas_dn = babyAnalyzer.genweights().at(111)/babyAnalyzer.genweights().at(1);
      weight_alphas_up *= (  h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,1)) / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,12))  );
      weight_alphas_dn *= (  h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,1)) / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,13))  );
    } else {
      weight_alphas_up = babyAnalyzer.genweights().at(109)/babyAnalyzer.genweights().at(0);
      weight_alphas_dn = babyAnalyzer.genweights().at(110)/babyAnalyzer.genweights().at(0);
      weight_alphas_up *= ( h_bkg_counter->GetBinContent(1) / h_bkg_counter->GetBinContent(12) );
      weight_alphas_dn *= ( h_bkg_counter->GetBinContent(1) / h_bkg_counter->GetBinContent(13) );
    }
    return;
  }

  // New method, start from v27
  weight_alphas_up = babyAnalyzer.weight_alphas_up();
  weight_alphas_dn = babyAnalyzer.weight_alphas_down();

  if ( is_fastsim_ ) {
    weight_alphas_up *= ( nEvents / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,12)) );
    weight_alphas_dn *= ( nEvents / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,13)) );
  } else {
    weight_alphas_up *= ( nEvents / h_bkg_counter->GetBinContent(12) );
    weight_alphas_dn *= ( nEvents / h_bkg_counter->GetBinContent(13) );
  }

  if ( weight_alphas_up < 0.0 || weight_alphas_dn < 0.0 ) {
    weight_alphas_up = 1.0;
    weight_alphas_dn = 1.0;
  }
}

//////////////////////////////////////////////////////////////////////

void evtWgtInfo::getQ2Weight( double &weight_q2_up, double &weight_q2_dn ) {

  if ( babyAnalyzer.genweights().size() < 110 ) return;

  if (sync16) { // Old method, obsolete from v27
    if ( is_fastsim_ ) {
      weight_q2_up = babyAnalyzer.genweights().at(5)/babyAnalyzer.genweights().at(1);
      weight_q2_dn = babyAnalyzer.genweights().at(9)/babyAnalyzer.genweights().at(1);
      weight_q2_up *= ( h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,1)) / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,5)) );
      weight_q2_dn *= ( h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,1)) / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,9)) );
    } else {
      weight_q2_up = babyAnalyzer.genweights().at(4)/babyAnalyzer.genweights().at(0);
      weight_q2_dn = babyAnalyzer.genweights().at(8)/babyAnalyzer.genweights().at(0);
      weight_q2_up *= ( h_bkg_counter->GetBinContent(1) / h_bkg_counter->GetBinContent(5) );
      weight_q2_dn *= ( h_bkg_counter->GetBinContent(1) / h_bkg_counter->GetBinContent(9) );
    }
    return;
  }

  // New method, start from v27
  weight_q2_up = babyAnalyzer.weight_Q2_up();
  weight_q2_dn = babyAnalyzer.weight_Q2_down();

  if ( is_fastsim_ ) {
    weight_q2_up *= ( nEvents / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,5)) );
    weight_q2_dn *= ( nEvents / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,9)) );
  } else {
    weight_q2_up *= ( nEvents / h_bkg_counter->GetBinContent(5) );
    weight_q2_dn *= ( nEvents / h_bkg_counter->GetBinContent(9) );
  }

  if ( weight_q2_up < 0.0 || weight_q2_dn < 0.0 ) {
    weight_q2_up = 1.0;
    weight_q2_dn = 1.0;
  }
}

//////////////////////////////////////////////////////////////////////

void evtWgtInfo::getLumi( double &weight_lumi, double &weight_lumi_up, double &weight_lumi_dn ) {

  weight_lumi    = lumi;
  weight_lumi_up = (lumi+lumi_err);
  weight_lumi_dn = (lumi-lumi_err);
}

//////////////////////////////////////////////////////////////////////

void evtWgtInfo::getISRWeight( double &weight_ISR, double &weight_ISR_up, double &weight_ISR_dn ) {

  weight_ISR    = babyAnalyzer.weight_ISR();
  weight_ISR_up = babyAnalyzer.weight_ISRup();
  weight_ISR_dn = babyAnalyzer.weight_ISRdown();

  // getNEvents(nEvents);
  if ( is_fastsim_ ) {
    weight_ISR    *= ( nEvents / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,19)) );
    weight_ISR_up *= ( nEvents / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,20)) );
    weight_ISR_dn *= ( nEvents / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,21)) );
  }

}

//////////////////////////////////////////////////////////////////////

void evtWgtInfo::getISRnJetsWeight( double &weight_ISR, double &weight_ISR_up, double &weight_ISR_dn ) {

  // if ( sampletype != "ttbar" ) return;

  weight_ISR    = babyAnalyzer.weight_ISRnjets();
  weight_ISR_up = babyAnalyzer.weight_ISRnjets_UP();
  weight_ISR_dn = babyAnalyzer.weight_ISRnjets_DN();

  // getNEvents(nEvents);

  if ( is_fastsim_ ) {
    weight_ISR    *= ( nEvents / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,24)) );
    weight_ISR_up *= ( nEvents / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,25)) );
    weight_ISR_dn *= ( nEvents / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,26)) );
  } else {
    weight_ISR    *= ( nEvents / h_bkg_counter->GetBinContent(25) );
    weight_ISR_up *= ( nEvents / h_bkg_counter->GetBinContent(26) );
    weight_ISR_dn *= ( nEvents / h_bkg_counter->GetBinContent(27) );
  }

}

//////////////////////////////////////////////////////////////////////

void evtWgtInfo::getISRnJetsWeight_local( double &weight_ISR, double &weight_ISR_up, double &weight_ISR_dn ) {

  weight_ISR = 1.0;

  // WARNING, normalization is not available, only to be used for shape comparison
  int nisrjets = babyAnalyzer.NISRjets();

  if (year == 2016) {
    if      (nisrjets == 0) weight_ISR = 1.000;
    else if (nisrjets == 1) weight_ISR = 0.920;
    else if (nisrjets == 2) weight_ISR = 0.821;
    else if (nisrjets == 3) weight_ISR = 0.715;
    else if (nisrjets == 4) weight_ISR = 0.662;
    else if (nisrjets == 5) weight_ISR = 0.561;
    else if (nisrjets >= 6) weight_ISR = 0.511;
  }
  else if (year == 2017) {
    if      (nisrjets == 0) weight_ISR = 1.0468;
    else if (nisrjets == 1) weight_ISR = 0.9880;
    else if (nisrjets == 2) weight_ISR = 0.9378;
    else if (nisrjets == 3) weight_ISR = 0.8640;
    else if (nisrjets == 4) weight_ISR = 0.8972;
    else if (nisrjets == 5) weight_ISR = 0.8707;
    else if (nisrjets >= 6) weight_ISR = 0.8128;
  }

  // Get the uncertainties of the ISR-njet weight base on the number of ISR jets matched
  float isr_unc = 0.;
  if (year >= 2016) {
    // Moriond 2017 values
    if      (nisrjets == 0) isr_unc = 0.000;
    else if (nisrjets == 1) isr_unc = 0.040;
    else if (nisrjets == 2) isr_unc = 0.090;
    else if (nisrjets == 3) isr_unc = 0.143;
    else if (nisrjets == 4) isr_unc = 0.170;
    else if (nisrjets == 5) isr_unc = 0.221;
    else if (nisrjets >= 6) isr_unc = 0.258;
  }

  weight_ISR_up = weight_ISR + isr_unc;
  weight_ISR_dn = weight_ISR - isr_unc;
}

//////////////////////////////////////////////////////////////////////

inline void evtWgtInfo::getPileupWeight( double &weight_pu, double &weight_pu_up, double &weight_pu_dn ) {
  weight_pu    = babyAnalyzer.weight_PU();
  weight_pu_up = babyAnalyzer.weight_PUup();
  weight_pu_dn = babyAnalyzer.weight_PUdown();
}

//////////////////////////////////////////////////////////////////////

bool evtWgtInfo::doingSystematic( systID isyst ) {
  if (is_data_) return false;
  if (isyst > k_JESDown && jes_type != 0) return false;
  switch (isyst) {
    case k_nominal:
    case k_JESUp:
      return jes_type == k_JESUp;
    case k_JESDown:
      return jes_type == k_JESDown;  // <-- todo: decide actions for this later
    case k_diLepTriggerUp:
    case k_diLepTriggerDown:
      return apply_diLepTrigger_sf;
    case k_cr2lTriggerUp:
    case k_cr2lTriggerDown:
      return add2ndLepToMet && apply_cr2lTrigger_sf;
    case k_bTagEffHFUp:
    case k_bTagEffHFDown:
    case k_bTagEffLFUp:
    case k_bTagEffLFDown:
      return apply_bTag_sf;
    case k_bTagFSEffUp:
    case k_bTagFSEffDown:
      return apply_bTagFS_sf;
    case k_lepSFUp:
    case k_lepSFDown:
      return apply_lep_sf;
    case k_lepFSSFUp:
    case k_lepFSSFDown:
      return apply_lepFS_sf;
    case k_topPtSFUp:
    case k_topPtSFDown:
      return is_bkg_; // apply_topPt_sf =true: do sf, =false: uncertainty only
    case k_metResUp:
    case k_metResDown:
      return apply_metRes_sf;
    case k_metTTbarUp:
    case k_metTTbarDown:
      return apply_metTTbar_sf;
    case k_ttbarSysPtUp:
    case k_ttbarSysPtDown:
      return is_bkg_; // apply_ttbarSysPt_sf =true: do sf, =false: uncertainty only
    case k_nuPtSF_Up:
    case k_nuPtSF_Down:
    case k_WwidthSF_Up:
    case k_WwidthSF_Down:
      return false;
    case k_WbXsec_Up:
    case k_WbXsec_Down:
      return apply_WbXsec_sf;
    case k_pdfUp:
    case k_pdfDown:
    case k_alphasUp:
    case k_alphasDown:
    case k_q2Up:
    case k_q2Down:
      return apply_genweights_unc;  // <-- deduced from bkgEstimate_diLepton.C
    case k_lumiUp:
    case k_lumiDown:
      return false;  // <-- No need ot include this in the looper
    case k_ISRUp:
    case k_ISRDown:
      return apply_ISR_sf;
    case k_xsecUp:
    case k_xsecDown:
      return false;  // <-- todo: look this up
    case k_puUp:
    case k_puDown:
      return apply_pu_sf;
    case k_tauSFUp:
    case k_tauSFDown:
      return apply_tau_sf;
    default:
      return false;
  }
}

// Currently only in a test state
evtWgtInfo::SampleType evtWgtInfo::findSampleType( string samplestr ) {
  TString sample(samplestr);

  SampleType samptype(unknown);

  if (sample.BeginsWith("data")) samptype = data;
  else if (sample.BeginsWith("ttbar") || sample.BeginsWith("TTJets")) samptype = ttbar;
  else if (sample.BeginsWith("t_") || sample.BeginsWith("ST_")) samptype = singletop;
  else if (sample.BeginsWith("ttW") || sample.BeginsWith("TTW")) samptype = ttW;
  else if (sample.BeginsWith("ttZ") || sample.BeginsWith("TTZ")) samptype = ttZ;
  else if (sample.BeginsWith("W") && sample.Contains("Jets")) samptype = Wjets;
  else if (sample.BeginsWith("WZ")) samptype = WZ;
  else if (sample.BeginsWith("SMS") || sample.BeginsWith("Signal") ) samptype = fastsim;
  else cout << "[eventWeight] >> Cannot assigned sampletype for " << samplestr << endl;

  if (verbose) cout << "[eventWeight] >> The assigned sampletype based on " << samplestr << " is " << samptype << endl;
  return samptype;
}

string evtWgtInfo::getLabel( systID isyst ) {
  Util util(isyst);
  return util.label;
}

double evtWgtInfo::getWeight( systID isyst, bool is_cr2l ) {
  if (is_cr2l != add2ndLepToMet) {
    add2ndLepToMet = is_cr2l;
    event_ready = false;
  }
  if (!event_ready)
    calculateWeightsForEvent();

  return sys_wgts[isyst];
}

void evtWgtInfo::setDefaultSystematics( int syst_set ) {
  switch (syst_set) {
    // Set of systematics used in the Moriond17 analysis
    case stop_Moriond17:
      apply_cr2lTrigger_sf = true;  // only !=1 if pfmet!=pfmet_rl ie no weight for ==1lepton events in SR and CR0b
      apply_bTag_sf        = true;  // event weight, product of all jet wgts
      apply_lep_sf         = true;  // both lep1 and lep2 (if available) are multiplied together
      apply_vetoLep_sf     = true;  // this is actually the lost lepton sf, only !=1 if there is >=2 genLeptons and ==1 recoLeptons in the event
      apply_tau_sf         = true;
      apply_topPt_sf       = false; // true=sf, false=uncertainty
      apply_metRes_sf      = true;
      apply_metTTbar_sf    = false;
      apply_ttbarSysPt_sf  = false; // true=sf, false=uncertainty, only !=1.0 for madgraph tt2l, tW2l
      apply_WbXsec_sf      = true;
      apply_ISR_sf         = true;  // only !=1.0 for signal
      apply_pu_sf          = true;
      apply_sample_sf      = true;
      if (is_fastsim_) {
        apply_lepFS_sf     = true;
        apply_bTagFS_sf    = true;
        apply_metRes_sf    = false;
        apply_tau_sf       = false;
        apply_WbXsec_sf    = false;
        apply_vetoLep_sf   = false;  // <-- why??
        apply_pu_sf        = false;  // <-- why?
      }
      break;

    // Set of (incomplete) systematics prepared for legacy analysis using 94X samples
    case stop_Run2:
      apply_cr2lTrigger_sf = true;   // not available yet
      apply_bTag_sf        = true;
      apply_lep_sf         = true;   // available but not updated yet
      apply_vetoLep_sf     = true;   // same as above
      apply_tau_sf         = true;   // same as above
      apply_topPt_sf       = false;
      apply_metRes_sf      = false;  // not developed for 94X yet
      apply_metTTbar_sf    = false;
      apply_ttbarSysPt_sf  = false;
      apply_WbXsec_sf      = false;  // not dev
      apply_ISR_sf         = false;  // not available yet
      apply_pu_sf          = false;  // not available yet
      apply_sample_sf      = false;  // no multiple sample available yet
      if (is_fastsim_) {
        apply_lepFS_sf     = false;  // no fast sim yet
        apply_bTagFS_sf    = false;
      }
      break;

    case test_alloff:
      apply_cr2lTrigger_sf = false;
      apply_bTag_sf        = false;
      apply_lep_sf         = false;
      apply_vetoLep_sf     = false;
      apply_tau_sf         = false;
      apply_topPt_sf       = false;
      apply_metRes_sf      = false;
      apply_metTTbar_sf    = false;
      apply_ttbarSysPt_sf  = false;
      apply_WbXsec_sf      = false;
      apply_ISR_sf         = false;
      apply_pu_sf          = false;
      apply_sample_sf      = false;
      if (is_fastsim_) {
        apply_lepFS_sf     = false;  // no fast sim yet
        apply_bTagFS_sf    = false;
      }
      break;
  }

}

double evtWgtInfo::getZSampleWeightFromCR3l( TString fname, bool apply ) {
  double sf_3l = 1.0;
  if (fname.Contains("TTZ"))
    sf_3l = 1.14;
  else if (fname.Contains("WZ"))
    sf_3l = 1.21;

  if (apply) sf_extra_file *= sf_3l;
  return sf_3l;
}

double evtWgtInfo::getExtSampleWeightSummer16v2( TString fname, bool apply ) {

  double result = 1.0;

  if (!apply_sample_sf) return result;

  if (fname.Contains("ttbar_diLept_madgraph_pythia8_ext1"))
    result = 23198554.0 / (23198554.0+5689986.0);
  else if (fname.Contains("ttbar_diLept_madgraph_pythia8"))
    result = 5689986.0 / (23198554.0+5689986.0);
  else if (fname.Contains("t_tW_5f_powheg_pythia8_noHadDecays_ext1"))
    result = (3145334.0)/(4473156.0+3145334.0);
  else if (fname.Contains("t_tW_5f_powheg_pythia8_noHadDecays"))
    result = (4473156.0)/(4473156.0+3145334.0);
  else if (fname.Contains("t_tbarW_5f_powheg_pythia8_noHadDecays_ext1"))
    result = (3146940.0)/(5029568.0+3146940.0);
  else if (fname.Contains("t_tbarW_5f_powheg_pythia8_noHadDecays"))
    result = (5029568.0)/(5029568.0+3146940.0);

  if (apply) sf_extra_file *= result;
  sync16 = true; // if this function is called, must be running sync checks

  return result;
}

double evtWgtInfo::getExtSampleWeightSummer16v3( TString fname, bool apply ) {
  double sf = 1.0;

  if (fname.Contains("TTJets_2lep_s16v3_ext1"))
    sf = 24767666.0 / (24767666.0+6068369.0);
  else if (fname.Contains("TTJets_2lep_s16v3_ext0"))
    sf =  6068369.0 / (24767666.0+6068369.0);
  else if (fname.Contains("TTJets_1lep_t_s16v3_ext1"))
    sf = 49664175.0 / (49664175.0+11957043.0);
  else if (fname.Contains("TTJets_1lep_t_s16v3_ext0"))
    sf = 11957043.0 / (49664175.0+11957043.0);
  else if (fname.Contains("TTJets_1lep_tbar_s16v3_ext1"))
    sf = 48387865.0 / (48387865.0+11955887.0);
  else if (fname.Contains("TTJets_1lep_tbar_s16v3_ext0"))
    sf = 11955887.0 / (48387865.0+11955887.0);
  else if (fname.Contains("DYJetsToLL_M50_s16v3_ext2"))
    sf = 96531428.0 / (96531428.0+49748967.0);
  else if (fname.Contains("DYJetsToLL_M50_s16v3_ext1"))
    sf = 49748967.0 / (96531428.0+49748967.0);
  // to be updated...

  if (verbose) cout << "[eventWeight] >> The sample weight to be scaled by is: " << sf << endl;
  if (apply) sf_extra_file *= sf;
  return sf;
}

double evtWgtInfo::getExtSampleWeightFall17v2( TString fname, bool apply ) {

  double sf = 1.0;
  if (fname.Contains("_v29_") && fname.Contains("JetsToLNu") && !fname.Contains("NuPt-200")) {
    // Temporary fix for missing k-factor in the WNJets xsec
    if      (fname.Contains("W1JetsToLNu")) sf = 1.238;
    else if (fname.Contains("W2JetsToLNu")) sf = 1.231;
    else if (fname.Contains("W3JetsToLNu")) sf = 1.231;
    else if (fname.Contains("W4JetsToLNu")) sf = 1.144;
  }
  if (fname.Contains("WWToLNuQQ_f17v2_ext1"))
    sf = 9643134.0 / (9643134.0+8330285.0);
  else if (fname.Contains("WWToLNuQQ_f17v2"))
    sf = 8330285.0 / (9643134.0+8330285.0);
  else if (fname.Contains("DYJetsToLL_M50_f17v2_ext1"))
    sf = 49125561.0 / (49125561.0+48675378.0);
  else if (fname.Contains("DYJetsToLL_M50_f17v2"))
    sf = 48675378.0 / (49125561.0+48675378.0);

  if (apply) sf_extra_file *= sf;
  return sf;
}

//////////////////////////////////////////////////////////////////////
