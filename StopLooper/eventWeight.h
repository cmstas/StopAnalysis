#ifndef EVENTWEIGHT_H
#define EVENTWEIGHT_H

// StopCORE
#include "../StopCORE/eventWeight_bTagSF.h"
#include "../StopCORE/eventWeight_lepSF.h"
#include "../StopCORE/StopTree.h"

// ROOT
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TEfficiency.h"
#include "Math/GenVector/VectorUtil.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > LorentzVector;

// STL
#include <string>
#include <vector>


class evtWgtInfo {

 public:

  enum systID {
    k_nominal,
    k_JESUp,
    k_JESDown,
    k_diLepTriggerUp,
    k_diLepTriggerDown,
    k_cr2lTriggerUp,
    k_cr2lTriggerDown,
    k_bTagEffHFUp,
    k_bTagEffHFDown,
    k_bTagEffLFUp,
    k_bTagEffLFDown,
    k_bTagFSEffUp,
    k_bTagFSEffDown,
    k_lepSFUp,
    k_lepSFDown,
    k_lepFSSFUp,
    k_lepFSSFDown,
    k_topPtSFUp,
    k_topPtSFDown,
    k_metResUp,
    k_metResDown,
    k_metTTbarUp,
    k_metTTbarDown,
    k_ttbarSysPtUp,
    k_ttbarSysPtDown,
    k_nuPtSF_Up,
    k_nuPtSF_Down,
    k_WwidthSF_Up,
    k_WwidthSF_Down,
    k_WbXsec_Up,
    k_WbXsec_Down,
    k_pdfUp,
    k_pdfDown,
    k_alphasUp,
    k_alphasDown,
    k_q2Up,
    k_q2Down,
    k_lumiUp,
    k_lumiDown,
    k_ISRUp,
    k_ISRDown,
    k_xsecUp,
    k_xsecDown,
    k_puUp,
    k_puDown,
    k_tauSFUp,
    k_tauSFDown,
    k_nSyst
  };

  // Keep this class for potential usage in the future
  class Util {
   public:
    std::string label;
    std::string title;

    Util( systID systematic );
    ~Util() {}
  };

  // Lost lepton analyis uses met with 2nd lepton removed
  bool add2ndLepToMet;

  enum SampleType {data, ttbar, Wjets, singletop, ttW, ttZ, WZ, diboson, fastsim, unknown=-1 } samptype;
  bool isTChi;
  // Counter histograms stored in babies
  TH1D *h_bkg_counter;
  TH2D *h_sig_counter_nEvents;
  TH3D *h_sig_counter;

  TFile *f_sig_xsec;
  TH1D *h_sig_xsec;

  // SR trigger efficiency histos
  TFile *f_cr2lTrigger_sf;
  TEfficiency *h_cr2lTrigger_sf_el;
  TEfficiency *h_cr2lTrigger_sf_mu;
  double max_metrl;
  double max_lep1pt;

  // bTag machinery
  bool useBTagSFs_fromFiles;
  eventWeight_bTagSF *bTagSFUtil;

  // Lepton SF machinery
  bool useLepSFs_fromFiles;
  eventWeight_lepSF *lepSFUtil;

  // Pileup weight machinery;
  TFile *f_pu;
  TH1D *h_pu_wgt;
  TH1D *h_pu_wgt_up;
  TH1D *h_pu_wgt_dn;

  // Tau Wgt Machinery
  TFile *f_lepEff;
  TH2D *h_recoEff_tau;

  // Event weights for each systematic
  double sys_wgts[k_nSyst];
  double sys_wgts_corridor[k_nSyst];

  // Variables to form baseline event weight
  int mStop;
  int mLSP;
  int mChargino;

  int jes_type;
  int year;

  // Switch for Moriond17 scheme for sync studies
  bool sync16;
  bool verbose;

  int    nEvents;
  double xsec;
  double xsec_err;
  double kFactor;
  double lumi;
  double lumi_err;

  double sf_nominal;

  bool   apply_diLepTrigger_sf;
  double sf_diLepTrigger;
  double sf_diLepTrigger_up;
  double sf_diLepTrigger_dn;

  bool   apply_cr2lTrigger_sf;
  double sf_cr2lTrigger;
  double sf_cr2lTrigger_up;
  double sf_cr2lTrigger_dn;

  bool   apply_bTag_sf;
  bool   use_tight_bTag;
  double sf_bTag;
  double sf_bTagEffHF_up;
  double sf_bTagEffHF_dn;
  double sf_bTagEffLF_up;
  double sf_bTagEffLF_dn;
  double sf_bTag_tight;
  double sf_bTagEffHF_tight_up;
  double sf_bTagEffHF_tight_dn;
  double sf_bTagEffLF_tight_up;
  double sf_bTagEffLF_tight_dn;

  bool   apply_bTagFS_sf;
  double sf_bTag_FS_up;
  double sf_bTag_FS_dn;
  double sf_bTag_tight_FS_up;
  double sf_bTag_tight_FS_dn;

  bool   apply_lep_sf;
  double sf_lep;
  double sf_lep_up;
  double sf_lep_dn;

  bool   apply_vetoLep_sf;
  double sf_vetoLep;
  double sf_vetoLep_up;
  double sf_vetoLep_dn;

  bool   apply_tau_sf;
  double sf_tau;
  double sf_tau_up;
  double sf_tau_dn;

  bool   apply_lepFS_sf;
  double sf_lepFS;
  double sf_lepFS_up;
  double sf_lepFS_dn;

  bool   apply_topPt_sf;
  double sf_topPt;
  double sf_topPt_up;
  double sf_topPt_dn;

  bool   apply_metRes_sf;
  double sf_metRes;
  double sf_metRes_up;
  double sf_metRes_dn;
  bool   apply_metRes_corridor_sf;
  double sf_metRes_corridor;
  double sf_metRes_corridor_up;
  double sf_metRes_corridor_dn;

  bool   apply_metTTbar_sf;
  double sf_metTTbar;
  double sf_metTTbar_up;
  double sf_metTTbar_dn;

  bool   apply_ttbarSysPt_sf;
  double sf_ttbarSysPt;
  double sf_ttbarSysPt_up;
  double sf_ttbarSysPt_dn;

  bool   apply_ISR_sf;
  double sf_ISR;
  double sf_ISR_up;
  double sf_ISR_dn;

  bool apply_pu_sf;
  double sf_pu;
  double sf_pu_up;
  double sf_pu_dn;

  bool   apply_sample_sf;
  double sf_sample;

  // <-- todo: look up why no switches for variables below 
  double sf_nuPt_up;
  double sf_nuPt_dn;

  double sf_Wwidth_up;
  double sf_Wwidth_dn;

  bool apply_WbXsec_sf;
  double sf_WbXsec_up;
  double sf_WbXsec_dn;

  bool apply_genweights_unc;
  double sf_pdf_up;
  double sf_pdf_dn;

  double sf_alphas_up;
  double sf_alphas_dn;

  double sf_q2_up;
  double sf_q2_dn;

  double sf_lumi;
  double sf_lumi_up;
  double sf_lumi_dn;

  double sf_xsec_up;
  double sf_xsec_dn;

  double sf_extra_file;  // special weight that only initialized at constructor

  evtWgtInfo();
  void Setup(std::string sample, int year = 0, bool applyUnc=true, bool useBTagUtils=false, bool useLepSFUtils=false);
  void Cleanup();
  void resetEvent();
  void getCounterHistogramFromBaby( TFile *sourceFile );
  void setExtraFileWeight(double sf_extra) { sf_extra_file = sf_extra; }

  void initializeWeights();
  void calculateWeightsForEvent();
  void getSusyMasses( int &mStop, int &mLSP );
  void getNEvents( int &nEvts );
  void getXSecWeight( double &weight_xsec, double &weight_xsec_up, double &weight_xsec_dn );
  void getLumi( double &weight_lumi, double &weight_lumi_up, double &weight_lumi_dn );
  void getScaleToLumiWeight( double &wgt );
  void getDiLepTriggerWeight( double &wgt_trigger, double &wgt_trigger_up, double &wgt_trigger_dn );
  void getCR2lTriggerWeight( double &wgt_trigger, double &wgt_trigger_up, double &wgt_trigger_dn );
  void getBTagWeight( int WP, double &wgt_btagsf, double &wgt_btagsf_hf_up, double &wgt_btagsf_hf_dn, double &wgt_btagsf_lf_up, double &wgt_btagsf_lf_dn, double &wgt_btagsf_fs_up, double &wgt_btagsf_fs_dn );
  void getBTagWeight_fromFiles( int WP, double &wgt_btagsf, double &wgt_btagsf_hf_up, double &wgt_btagsf_hf_dn, double &wgt_btagsf_lf_up, double &wgt_btagsf_lf_dn, double &wgt_btagsf_tight_fs_up, double &wgt_btagsf_tight_fs_dn );
  void getLepSFWeight( double &weight_lepSF, double &weight_lepSF_Up, double &weight_lepSF_Dn, double &weight_lepFSSF, double &weight_lepFSSF_Up, double &weight_lepFSSF_Dn, double &weight_vetoLepSF, double &weight_vetoLepSF_Up, double &weight_vetoLepSF_Dn );
  void getTauSFWeight( double &weight_tau, double &weight_tau_up, double &weight_tau_dn );
  void getLepSFWeight_fromFiles( double &weight_lepSF, double &weight_lepSF_Up, double &weight_lepSF_Dn, double &weight_lepFSSF, double &weight_lepFSSF_Up, double &weight_lepFSSF_Dn, double &weight_vetoLepSF, double &weight_vetoLepSF_Up, double &weight_vetoLepSF_Dn );
  void getTopPtWeight( double &weight_topPt, double &weight_topPt_up, double &weight_topPt_dn );
  void getMetResWeight( double &weight_metRes, double &weight_metRes_up, double &weight_metRes_dn );
  void getMetResWeight_corridor( double &weight_metRes, double &weight_metRes_up, double &weight_metRes_dn );
  void getMetTTbarWeight( double &weight_metTTbar, double &weight_metTTbar_up, double &weight_metTTbar_dn );
  void getTTbarSysPtSF( double &weight_ttbarSysPt, double &weight_ttbarSysPt_up, double &weight_ttbarSysPt_dn );
  void getNuPtSF( double &weight_nuPt_up, double &weight_nuPt_dn );
  void getWwidthSF( double &weight_Wwidth_up, double &weight_Wwidth_dn );
  void getWbXSecSF( double &weight_WbXsec_up, double &weight_WbXsec_dn );
  void getPDFWeight( double &weight_pdf_up, double &weight_pdf_dn );
  void getAlphasWeight( double &weight_alphas_up, double &weight_alphas_dn );
  void getQ2Weight( double &weight_q2_up, double &weight_q2_dn );
  void getISRWeight( double &weight_ISR, double &weight_ISR_up, double &weight_ISR_dn );
  void getISRnJetsWeight( double &weight_ISR, double &weight_ISR_up, double &weight_ISR_dn );
  void getPileupWeight( double &weight_pu, double &weight_pu_up, double &weight_pu_dn );

  double getZSampleWeightFromCR3l( TString fname, bool apply=true );
  double getExtSampleWeightSummer16v2( TString sample, bool apply=true );
  double getExtSampleWeightSummer16v3( TString sample, bool apply=true );
  double getExtSampleWeightFall17v2( TString sample, bool apply=true );
  void setDefaultSystematics( int syst_set=0 );
  bool doingSystematic( systID systid );
  SampleType findSampleType( std::string samplestr);  // <-- todo: finish this function
  std::string getLabel( systID systid );
  double getWeight( systID systid, bool cr2lregions=false );

 private:
  bool is_bkg_;
  bool is_data_;
  bool is_fastsim_;
  bool event_ready;

}; // end class def


//////////////////////////////////////////////////////////////////////

#endif
