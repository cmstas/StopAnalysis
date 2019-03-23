#include "eventWeight_lepSF.h"

using namespace std;
const float ferr = 0.01;

//////////////////////////////////////////////////////////////////////

void eventWeight_lepSF::setup( bool isFastsim, int inyear = 2017, TString fileloc = "lepsf" ) {

  sampleIsFastsim = isFastsim;
  year = inyear;
  filepath = fileloc;

  xmin_h_el_SF = 1.0;
  xmax_h_el_SF = 1.0;
  ymin_h_el_SF = 1.0;
  ymax_h_el_SF = 1.0;
  xmin_h_el_SF_veto = 1.0;
  xmax_h_el_SF_veto = 1.0;
  ymin_h_el_SF_veto = 1.0;
  ymax_h_el_SF_veto = 1.0;
  xmin_h_el_SF_tracking = 1.0;
  xmax_h_el_SF_tracking = 1.0;
  ymin_h_el_SF_tracking = 1.0;
  ymax_h_el_SF_tracking = 1.0;
  xmin_h_mu_SF = 1.0;
  xmax_h_mu_SF = 1.0;
  ymin_h_mu_SF = 1.0;
  ymax_h_mu_SF = 1.0;
  // xmin_h_mu_SF_tracking = 1.0;
  // xmax_h_mu_SF_tracking = 1.0;
  xmin_h_mu_SF_veto = 1.0;
  xmax_h_mu_SF_veto = 1.0;
  ymin_h_mu_SF_veto = 1.0;
  ymax_h_mu_SF_veto = 1.0;
  xmin_h_el_FS = 1.0;
  xmax_h_el_FS = 1.0;
  ymin_h_el_FS = 1.0;
  ymax_h_el_FS = 1.0;
  xmin_h_el_veto_FS = 1.0;
  xmax_h_el_veto_FS = 1.0;
  ymin_h_el_veto_FS = 1.0;
  ymax_h_el_veto_FS = 1.0;
  xmin_h_mu_FS = 1.0;
  xmax_h_mu_FS = 1.0;
  ymin_h_mu_FS = 1.0;
  ymax_h_mu_FS = 1.0;
  xmin_h_mu_veto_FS = 1.0;
  xmax_h_mu_veto_FS = 1.0;
  ymin_h_mu_veto_FS = 1.0;
  ymax_h_mu_veto_FS = 1.0;
  xmin_h_el_vetoLepEff = 1.0;
  xmax_h_el_vetoLepEff = 1.0;
  ymin_h_el_vetoLepEff = 1.0;
  ymax_h_el_vetoLepEff = 1.0;
  xmin_h_mu_vetoLepEff = 1.0;
  xmax_h_mu_vetoLepEff = 1.0;
  ymin_h_mu_vetoLepEff = 1.0;
  ymax_h_mu_vetoLepEff = 1.0;

  cout << "[eventWeight_lepSF] Loading lepton scale factors with year " << inyear << " from " << fileloc << endl;

  // filepath = "../StopCORE/inputs/lepsf/";
  // if (year == 2016) filepath += "Moriond17";
  // else if (year == 2017) filepath += "analysis2017_41p9ifb";
  // else if (year == 2018) filepath += "analysis2017_41p9ifb";  // use same as 2017 for now

  // Electron file
  TH2F *h_el_SF_id_temp;
  TH2F *h_el_SF_iso_temp;
  TH2F *h_el_SF_tracking_temp;
  TH2F *h_el_SF_trklowpt_temp;
  TH2F *h_el_SF_veto_id_temp;
  TH2F *h_el_SF_veto_iso_temp;
  TH2F *h_el_FS_ID_temp;
  TH2F *h_el_FS_Iso_temp;
  TH2F *h_el_veto_FS_ID_temp;
  TH2F *h_el_veto_FS_Iso_temp;
  if (year == 16) {
    f_el_SF          = new TFile(filepath+"/scaleFactors.root", "read");
    f_el_SF_tracking = new TFile(filepath+"/egammaEffi.txt_EGM2D.root", "read");

    // Grab selected el histos
    h_el_SF_id_temp       = (TH2F*)f_el_SF->Get("GsfElectronToCutBasedSpring15M");
    h_el_SF_iso_temp      = (TH2F*)f_el_SF->Get("MVAVLooseElectronToMini");
    h_el_SF_tracking_temp = (TH2F*)f_el_SF_tracking->Get("EGamma_SF2D");

    // Grab veto el histos
    h_el_SF_veto_id_temp  = (TH2F*)f_el_SF->Get("GsfElectronToCutBasedSpring15V");
    h_el_SF_veto_iso_temp = (TH2F*)f_el_SF->Get("MVAVLooseElectronToMini2");

    // Fastsim/Fullsim el files
    f_el_FS_ID  = new TFile(filepath+"/sf_el_mediumCB.root", "read");
    f_el_FS_Iso = new TFile(filepath+"/sf_el_mini01.root", "read");

    f_el_veto_FS_ID  = new TFile(filepath+"/sf_el_vetoCB.root", "read");
    f_el_veto_FS_Iso = new TFile(filepath+"/sf_el_mini02.root", "read");

    // Grab fastsim/fullsim selected el histos
    h_el_FS_ID_temp  = (TH2F*)f_el_FS_ID->Get("histo2D");
    h_el_FS_Iso_temp = (TH2F*)f_el_FS_Iso->Get("histo2D");

    // Grab fastsim/fullsim veto el histos
    h_el_veto_FS_ID_temp  = (TH2F*)f_el_veto_FS_ID->Get("histo2D");
    h_el_veto_FS_Iso_temp = (TH2F*)f_el_veto_FS_Iso->Get("histo2D");
  }
  else if (year == 2016) {
    // From Twiki: https://twiki.cern.ch/twiki/bin/view/CMS/SUSLeptonSF#Electrons_AN1
    f_el_SF           = new TFile(filepath+"/ElectronScaleFactors_Run2016.root", "read");
    f_el_SF_tracking = new TFile(filepath+"/EGM2D_BtoH_GT20GeV_RecoSF_Legacy2016.root", "read");
    f_el_SF_trklowpt = new TFile(filepath+"/EGM2D_BtoH_low_RecoSF_Legacy2016.root", "read");

    h_el_SF_id_temp        = (TH2F*)f_el_SF->Get("Run2016_CutBasedMediumNoIso94XV2");
    h_el_SF_iso_temp       = (TH2F*)f_el_SF->Get("Run2016_Mini");
    h_el_SF_tracking_temp = (TH2F*)f_el_SF_tracking->Get("EGamma_SF2D");
    h_el_SF_trklowpt_temp = (TH2F*)f_el_SF_trklowpt->Get("EGamma_SF2D");

    // Grab veto el histos
    h_el_SF_veto_id_temp  = (TH2F*)f_el_SF->Get("Run2016_CutBasedVetoNoIso94XV2");
    h_el_SF_veto_iso_temp = (TH2F*)f_el_SF->Get("Run2016_Mini2");

    // Fastsim/Fullsim el files
    // TODO: to be updated use Morion16 files for now
    f_el_FS_ID  = new TFile(filepath+"/sf_el_mediumCB.root", "read");
    f_el_FS_Iso = new TFile(filepath+"/sf_el_mini01.root", "read");

    f_el_veto_FS_ID  = new TFile(filepath+"/sf_el_vetoCB.root", "read");
    f_el_veto_FS_Iso = new TFile(filepath+"/sf_el_mini02.root", "read");

    // Grab fastsim/fullsim selected el histos
    h_el_FS_ID_temp  = (TH2F*)f_el_FS_ID->Get("histo2D");
    h_el_FS_Iso_temp = (TH2F*)f_el_FS_Iso->Get("histo2D");

    // Grab fastsim/fullsim veto el histos
    h_el_veto_FS_ID_temp  = (TH2F*)f_el_veto_FS_ID->Get("histo2D");
    h_el_veto_FS_Iso_temp = (TH2F*)f_el_veto_FS_Iso->Get("histo2D");
  }
  else if (year == 2017) {
    // From Twiki: https://twiki.cern.ch/twiki/bin/view/CMS/SUSLeptonSF#Electrons_AN1
    f_el_SF          = new TFile(filepath+"/ElectronScaleFactors_Run2017.root", "read");
    f_el_SF_tracking = new TFile(filepath+"/egammaEffi.txt_EGM2D_runBCDEF_passingRECO.root", "read");
    f_el_SF_trklowpt = new TFile(filepath+"/egammaEffi.txt_EGM2D_runBCDEF_passingRECO_lowEt.root", "read");

    h_el_SF_id_temp       = (TH2F*)f_el_SF->Get("Run2017_CutBasedMediumNoIso94XV2");
    h_el_SF_iso_temp      = (TH2F*)f_el_SF->Get("Run2017_MVAVLooseTightIP2DMini");
    h_el_SF_tracking_temp = (TH2F*)f_el_SF_tracking->Get("EGamma_SF2D");
    h_el_SF_trklowpt_temp = (TH2F*)f_el_SF_trklowpt->Get("EGamma_SF2D");

    // Grab veto el histos
    h_el_SF_veto_id_temp  = (TH2F*)f_el_SF->Get("Run2017_CutBasedVetoNoIso94XV2");
    h_el_SF_veto_iso_temp = (TH2F*)f_el_SF->Get("Run2017_MVAVLooseTightIP2DMini2");

    // Fastsim/Fullsim el files
    f_el_FS_ID  = new TFile(filepath+"/detailed_ele_full_fast_sf_17.root", "read");

    // Grab fastsim/fullsim selected el histos
    h_el_FS_ID_temp  = (TH2F*)f_el_FS_ID->Get("CutBasedMediumNoIso94XV2_sf");
    h_el_FS_Iso_temp = (TH2F*)f_el_FS_ID->Get("MVAVLooseTightIP2DMini_sf");

    // Grab fastsim/fullsim veto el histos
    h_el_veto_FS_ID_temp  = (TH2F*)f_el_FS_ID->Get("CutBasedVetoNoIso94XV2_sf");
    h_el_veto_FS_Iso_temp = (TH2F*)f_el_FS_ID->Get("MVAVLooseTightIP2DMini2_sf");
  }
  else if (year == 2018) {
    // From Twiki: https://twiki.cern.ch/twiki/bin/view/CMS/SUSLeptonSF#Electrons_AN1
    f_el_SF          = new TFile(filepath+"/ElectronScaleFactors_Run2018.root", "read");
    f_el_SF_tracking = new TFile(filepath+"/egammaEffi.txt_EGM2D.root", "read");
    f_el_SF_trklowpt = new TFile(filepath+"/egammaEffi.txt_EGM2D_low.root", "read");

    h_el_SF_id_temp       = (TH2F*)f_el_SF->Get("Run2018_CutBasedMediumNoIso94XV2");
    h_el_SF_iso_temp      = (TH2F*)f_el_SF->Get("Run2018_Mini");
    h_el_SF_tracking_temp = (TH2F*)f_el_SF_tracking->Get("EGamma_SF2D");
    h_el_SF_trklowpt_temp = (TH2F*)f_el_SF_tracking->Get("EGamma_SF2D");

    // Grab veto el histos
    h_el_SF_veto_id_temp  = (TH2F*)f_el_SF->Get("Run2018_CutBasedVetoNoIso94XV2");
    h_el_SF_veto_iso_temp = (TH2F*)f_el_SF->Get("Run2018_Mini2");

    // Fastsim/Fullsim el files
    f_el_FS_ID  = new TFile(filepath+"/detailed_ele_full_fast_sf_18.root", "read");

    // Grab fastsim/fullsim selected el histos
    h_el_FS_ID_temp  = (TH2F*)f_el_FS_ID->Get("CutBasedMediumNoIso94XV2_sf");
    h_el_FS_Iso_temp = (TH2F*)f_el_FS_ID->Get("MVAVLooseTightIP2DMini_sf");

    // Grab fastsim/fullsim veto el histos
    h_el_veto_FS_ID_temp  = (TH2F*)f_el_FS_ID->Get("CutBasedVetoNoIso94XV2_sf");
    h_el_veto_FS_Iso_temp = (TH2F*)f_el_FS_ID->Get("MVAVLooseTightIP2DMini2_sf");
  }

  // Muon files
  if (year == 2016) {
    // Twiki: https://twiki.cern.ch/twiki/bin/view/CMS/SUSLeptonSF#Muons_AN1
    f_mu_SF_id       = new TFile(filepath+"/TnP_NUM_MediumID_DENOM_generalTracks_VAR_map_pt_eta.root", "read");
    f_mu_SF_iso      = new TFile(filepath+"/TnP_NUM_MiniIsoTight_DENOM_MediumID_VAR_map_pt_eta.root", "read"); // double unc
    f_mu_SF_ip       = new TFile(filepath+"/TnP_NUM_TightIP2D_DENOM_MediumID_VAR_map_pt_eta.root", "read"); // double unc
    // f_mu_SF_tracking = new TFile(filepath+"/Tracking_EfficienciesAndSF_BCDEFGH.root", "read");

    f_mu_SF_veto_id  = new TFile(filepath+"/TnP_NUM_LooseID_DENOM_generalTracks_VAR_map_pt_eta.root", "read");
    f_mu_SF_veto_iso = new TFile(filepath+"/TnP_NUM_MiniIsoTight_DENOM_LooseID_VAR_map_pt_eta.root", "read");
    f_mu_SF_veto_ip  = new TFile(filepath+"/TnP_NUM_MediumIP2D_DENOM_LooseID_VAR_map_pt_eta.root", "read"); // double unc for this

    // Fastsim/Fullsim mu files
    f_mu_FS_ID  = new TFile(filepath+"/sf_mu_mediumID.root", "read"); // double unc for this
    f_mu_FS_Iso = new TFile(filepath+"/sf_mu_mediumID_mini02.root", "read"); // double unc for this
    f_mu_FS_Ip  = new TFile(filepath+"/sf_mu_mediumID_tightIP2D.root", "read"); // double unc for this

    f_mu_veto_FS_ID  = new TFile(filepath+"/sf_mu_looseID.root", "read");
    f_mu_veto_FS_Iso = new TFile(filepath+"/sf_mu_looseID_mini02.root", "read");
    f_mu_veto_FS_Ip  = new TFile(filepath+"/sf_mu_mediumID_looseIP2D.root", "read"); // double unc for this
  }
  else if (year == 2017) {
    f_mu_SF_id       = new TFile(filepath+"/RunBCDEF_SF_ID.root", "read");
    f_mu_SF_iso      = new TFile(filepath+"/sf_mu_iso_susy_2017.root", "read"); // double unc
    f_mu_SF_ip       = nullptr;  // included in the ID SF as MediumPromptID

    f_mu_SF_veto_id  = new TFile(filepath+"/RunBCDEF_SF_ID.root", "read");
    f_mu_SF_veto_iso = new TFile(filepath+"/sf_mu_iso_susy_2017.root", "read");
    f_mu_SF_veto_ip  = nullptr;  // nothing available, not applying

    // Fastsim/Fullsim mu files
    // TODO: update the muon fastsim SFs
    f_mu_FS_ID  = new TFile(filepath+"/sf_mu_mediumID.root", "read"); // double unc for this
    f_mu_FS_Iso = new TFile(filepath+"/sf_mu_mediumID_mini02.root", "read"); // double unc for this
    f_mu_FS_Ip  = new TFile(filepath+"/sf_mu_mediumID_tightIP2D.root", "read"); // double unc for this

    f_mu_veto_FS_ID  = new TFile(filepath+"/sf_mu_looseID.root", "read");
    f_mu_veto_FS_Iso = new TFile(filepath+"/sf_mu_looseID_mini02.root", "read");
    f_mu_veto_FS_Ip  = new TFile(filepath+"/sf_mu_mediumID_looseIP2D.root", "read"); // double unc for this
  }
  else if (year == 2018) {
    f_mu_SF_id       = new TFile(filepath+"/RunABCD_SF_ID.root", "read");
    f_mu_SF_iso      = new TFile(filepath+"/sf_mu_iso_susy_2017.root", "read"); // TODO: to be updated
    f_mu_SF_ip       = nullptr;  // included in the ID SF as MediumPromptID

    f_mu_SF_veto_id  = new TFile(filepath+"/RunABCD_SF_ID.root", "read");
    f_mu_SF_veto_iso = new TFile(filepath+"/sf_mu_iso_susy_2017.root", "read");
    f_mu_SF_veto_ip  = nullptr;  // nothing available, not applying

    // Fastsim/Fullsim mu files
    // TODO: update the muon fastsim SFs
    f_mu_FS_ID  = new TFile(filepath+"/sf_mu_mediumID.root", "read"); // double unc for this
    f_mu_FS_Iso = new TFile(filepath+"/sf_mu_mediumID_mini02.root", "read"); // double unc for this
    f_mu_FS_Ip  = new TFile(filepath+"/sf_mu_mediumID_tightIP2D.root", "read"); // double unc for this

    f_mu_veto_FS_ID  = new TFile(filepath+"/sf_mu_looseID.root", "read");
    f_mu_veto_FS_Iso = new TFile(filepath+"/sf_mu_looseID_mini02.root", "read");
    f_mu_veto_FS_Ip  = new TFile(filepath+"/sf_mu_mediumID_looseIP2D.root", "read"); // double unc for this
  }

  // Veto lepton reco efficiency files
  if (year == 2016) {
    f_vetoLep_eff = new TFile(filepath+"/lepeff__moriond17__ttbar_powheg_pythia8_25ns.root", "read");
  } else if (year >= 2017) {
    f_vetoLep_eff = new TFile(filepath+"/lepeffs_tt2l_madgraph_mc2017.root", "read");
  }

  // Grab selected mu histos
  TH2F *h_mu_SF_id_temp  = (year == 2016)? (TH2F*)f_mu_SF_id->Get("SF") : (TH2F*)f_mu_SF_id->Get("NUM_MediumPromptID_DEN_genTracks_pt_abseta"); // including IP
  TH2F *h_mu_SF_iso_temp = (year == 2016)? (TH2F*)f_mu_SF_iso->Get("SF") : nullptr;
  TH2F *h_mu_SF_ip_temp  = (year == 2016)? (TH2F*)f_mu_SF_ip->Get("SF") : nullptr;
  // Muon Tracking is no longer recommended
  // TGraphAsymmErrors *h_mu_SF_tracking_temp  = (TGraphAsymmErrors*)f_mu_SF_tracking->Get("ratio_eff_aeta_dr030e030_corr");

  // Grab veto mu histos
  TH2F *h_mu_SF_veto_id_temp  = (year == 2016)? (TH2F*)f_mu_SF_veto_id->Get("SF")  : (TH2F*)f_mu_SF_veto_id->Get("NUM_LooseID_DEN_genTracks_pt_abseta");
  TH2F *h_mu_SF_veto_iso_temp = (year == 2016)? (TH2F*)f_mu_SF_veto_iso->Get("SF") : nullptr;
  TH2F *h_mu_SF_veto_ip_temp  = (year == 2016)? (TH2F*)f_mu_SF_veto_ip->Get("SF")  : nullptr;

  if (year >= 2017) {
    // Because the binning between the iso SF from SUSY and the ID SUSY from Muon POG is different, need to rebin the iso hist
    // Rebin also the veto SFs in the same loop, assuming the binning are the same (they should be, they have to be)
    h_mu_SF_iso_temp = (TH2F*)h_mu_SF_id_temp->Clone("h_mu_iso_SF_temp");
    h_mu_SF_veto_iso_temp = (TH2F*)h_mu_SF_id_temp->Clone("h_mu_veto_iso_SF_temp");
    auto h_mu_SF_iso_org = (TH2F*) f_mu_SF_iso->Get("TnP_MC_NUM_MiniIso02Cut_DEN_MediumID_PAR_pt_eta"); // note that we should use miniIso01 in the ideal case
    auto h_mu_SF_veto_iso_org = (TH2F*) f_mu_SF_iso->Get("TnP_MC_NUM_MiniIso02Cut_DEN_LooseID_PAR_pt_eta");
    for (int xbin = 1; xbin < h_mu_SF_iso_temp->GetNbinsX()+1; ++xbin) {
      for (int ybin = 1; ybin < h_mu_SF_iso_temp->GetNbinsY()+1; ++ybin) {
        int ibin_horg = h_mu_SF_iso_org->FindBin(h_mu_SF_iso_temp->GetXaxis()->GetBinCenter(xbin), h_mu_SF_iso_temp->GetYaxis()->GetBinCenter(ybin));
        h_mu_SF_iso_temp->SetBinContent(xbin, ybin, h_mu_SF_iso_org->GetBinContent(ibin_horg));
        h_mu_SF_iso_temp->SetBinError(xbin, ybin, h_mu_SF_iso_org->GetBinError(ibin_horg));
        h_mu_SF_veto_iso_temp->SetBinContent(xbin, ybin, h_mu_SF_veto_iso_org->GetBinContent(ibin_horg));
        h_mu_SF_veto_iso_temp->SetBinError(xbin, ybin, h_mu_SF_veto_iso_org->GetBinError(ibin_horg));
      }
    }
  }

  // Grab fastsim/fullsim selected mu histos
  TH2F *h_mu_FS_ID_temp  = (TH2F*)f_mu_FS_ID->Get("histo2D");
  TH2F *h_mu_FS_Iso_temp = (TH2F*)f_mu_FS_Iso->Get("histo2D");
  TH2F *h_mu_FS_Ip_temp  = (TH2F*)f_mu_FS_Ip->Get("histo2D");

  // Grab fastsim/fullsim veto mu histos
  TH2F *h_mu_veto_FS_ID_temp  = (TH2F*)f_mu_veto_FS_ID->Get("histo2D");
  TH2F *h_mu_veto_FS_Iso_temp = (TH2F*)f_mu_veto_FS_Iso->Get("histo2D");
  TH2F *h_mu_veto_FS_Ip_temp  = (TH2F*)f_mu_veto_FS_Ip->Get("histo2D");


  // Grab mc eff for veto lepton (for lost lepto SFs) histos
  TH2F *h_el_vetoLepEff_temp = (year == 2016)? (TH2F*)f_vetoLep_eff->Get("h2_lepEff_vetoSel_Eff_el") : (TH2F*)f_vetoLep_eff->Get("heff17_lepeff_veto_el");
  TH2F *h_mu_vetoLepEff_temp = (year == 2016)? (TH2F*)f_vetoLep_eff->Get("h2_lepEff_vetoSel_Eff_mu") : (TH2F*)f_vetoLep_eff->Get("heff17_lepeff_veto_mu");


  // Get final fullsim, selected el, sfs
  TH2F *h_el_SF_id  = (TH2F*)h_el_SF_id_temp->Clone("h_el_SF_id");
  TH2F *h_el_SF_iso = (TH2F*)h_el_SF_iso_temp->Clone("h_el_SF_iso");
  h_el_SF = (TH2F*)h_el_SF_id->Clone("h_el_SF");
  h_el_SF->Multiply(h_el_SF_iso);
  xmin_h_el_SF = h_el_SF->GetXaxis()->GetBinLowEdge(1)+ferr;
  xmax_h_el_SF = h_el_SF->GetXaxis()->GetBinLowEdge(h_el_SF->GetNbinsX()+1)-ferr;
  ymin_h_el_SF = h_el_SF->GetYaxis()->GetBinLowEdge(1)+ferr;
  ymax_h_el_SF = h_el_SF->GetYaxis()->GetBinLowEdge(h_el_SF->GetNbinsY()+1)-ferr;

  // Get final fullsim, selected el, tracking sfs
  h_el_SF_tracking = (TH2F*)h_el_SF_tracking_temp->Clone("h_el_SF_tracking");
  xmin_h_el_SF_tracking = h_el_SF_tracking->GetXaxis()->GetBinLowEdge(1)+ferr;
  xmax_h_el_SF_tracking = h_el_SF_tracking->GetXaxis()->GetBinLowEdge(h_el_SF_tracking->GetNbinsX()+1)-ferr;
  ymin_h_el_SF_tracking = h_el_SF_tracking->GetYaxis()->GetBinLowEdge(1);
  ymax_h_el_SF_tracking = h_el_SF_tracking->GetYaxis()->GetBinLowEdge(h_el_SF_tracking->GetNbinsY()+1)-ferr;
  h_el_SF_trklowpt = (TH2F*)h_el_SF_trklowpt_temp->Clone("h_el_SF_trklowpt");
  ymin_h_el_SF_trklowpt = h_el_SF_trklowpt->GetYaxis()->GetBinLowEdge(1)+ferr;
  ymax_h_el_SF_trklowpt = h_el_SF_trklowpt->GetYaxis()->GetBinLowEdge(h_el_SF_trklowpt->GetNbinsY()+1)-ferr;

  // Get final fullsim, veto el, sfs
  TH2F *h_el_SF_veto_id  = (TH2F*)h_el_SF_veto_id_temp->Clone("h_el_SF_veto_id");
  TH2F *h_el_SF_veto_iso = (TH2F*)h_el_SF_veto_iso_temp->Clone("h_el_SF_veto_iso");
  h_el_SF_veto = (TH2F*)h_el_SF_veto_id->Clone("h_el_SF_veto");
  h_el_SF_veto->Multiply(h_el_SF_veto_iso);
  // Double unc. on veto electron sfs, since we go lower in pT
  for (int x=1; x <= h_el_SF_veto->GetNbinsX(); x++) {
    for (int y=1; y <= h_el_SF_veto->GetNbinsY(); y++) {
      h_el_SF_veto->SetBinError(x,y,h_el_SF_veto->GetBinError(x,y)*2.0);
    }
  }
  xmin_h_el_SF_veto = h_el_SF_veto->GetXaxis()->GetBinLowEdge(1)+ferr;
  xmax_h_el_SF_veto = h_el_SF_veto->GetXaxis()->GetBinLowEdge(h_el_SF_veto->GetNbinsX()+1)-ferr;
  ymin_h_el_SF_veto = h_el_SF_veto->GetYaxis()->GetBinLowEdge(1)+ferr;
  ymax_h_el_SF_veto = h_el_SF_veto->GetYaxis()->GetBinLowEdge(h_el_SF_veto->GetNbinsY()+1)-ferr;

  // Get final fullsim, selected mu, sfs
  TH2F *h_mu_SF_id  = (TH2F*)h_mu_SF_id_temp->Clone("h_mu_SF_id");
  TH2F *h_mu_SF_iso = (TH2F*)h_mu_SF_iso_temp->Clone("h_mu_SF_iso");
  // Double unc. on selected muon iso sfs, since not for our exact wp
  for (int x=1; x <= h_mu_SF_iso->GetNbinsX(); x++) {
    for (int y=1; y <= h_mu_SF_iso->GetNbinsY(); y++) {
      h_mu_SF_iso->SetBinError(x,y,h_mu_SF_iso->GetBinError(x,y)*2.0);
    }
  }
  h_mu_SF = (TH2F*)h_mu_SF_id->Clone("h_mu_SF");
  h_mu_SF->Multiply(h_mu_SF_iso);

  xmin_h_mu_SF = h_mu_SF->GetXaxis()->GetBinLowEdge(1)+ferr;
  xmax_h_mu_SF = h_mu_SF->GetXaxis()->GetBinLowEdge(h_mu_SF->GetNbinsX()+1)-ferr;
  ymin_h_mu_SF = h_mu_SF->GetYaxis()->GetBinLowEdge(1)+ferr;
  ymax_h_mu_SF = h_mu_SF->GetYaxis()->GetBinLowEdge(h_mu_SF->GetNbinsY()+1)-ferr;

  // Muon tracking is no longer recommended (effectively ~1)
  // int nX = h_mu_SF_tracking_temp->GetN();
  // Double_t *x_val = h_mu_SF_tracking_temp->GetX();
  // Double_t *y_val = h_mu_SF_tracking_temp->GetY();
  // Double_t *y_err_up = h_mu_SF_tracking_temp->GetEYhigh();
  // Double_t *y_err_low = h_mu_SF_tracking_temp->GetEYhigh();
  // h_mu_SF_tracking = new TH1D("h_mu_SF_tracking","h_mu_SF_tracking",nX-1,x_val);
  // for (int i=0; i<nX; i++) {
  //   h_mu_SF_tracking->SetBinContent(i+1,y_val[i]);
  //   h_mu_SF_tracking->SetBinError(i+1,std::max(y_err_up[i],y_err_low[i]));
  // }
  // xmin_h_mu_SF_tracking = h_mu_SF_tracking->GetXaxis()->GetBinLowEdge(1)+ferr;
  // xmax_h_mu_SF_tracking = h_mu_SF_tracking->GetXaxis()->GetBinLowEdge(h_mu_SF_tracking->GetNbinsX()+1)-ferr;
  // // ymin_h_mu_SF_tracking = h_mu_SF_tracking->GetYaxis()->GetBinLowEdge(1)+ferr;
  // // ymax_h_mu_SF_tracking = h_mu_SF_tracking->GetYaxis()->GetBinLowEdge(h_mu_SF_tracking->GetNbinsY()+1)-ferr;

  // Get final fullsim, veto mu, sfs
  TH2F *h_mu_SF_veto_id  = (TH2F*)h_mu_SF_veto_id_temp->Clone("h_mu_SF_veto_id");
  TH2F *h_mu_SF_veto_iso = (TH2F*)h_mu_SF_veto_iso_temp->Clone("h_mu_SF_veto_iso");
  h_mu_SF_veto = (TH2F*)h_mu_SF_veto_id->Clone("h_mu_SF_veto");
  h_mu_SF_veto->Multiply(h_mu_SF_veto_iso);

  xmin_h_mu_SF_veto = h_mu_SF_veto->GetXaxis()->GetBinLowEdge(1)+ferr;
  xmax_h_mu_SF_veto = h_mu_SF_veto->GetXaxis()->GetBinLowEdge(h_mu_SF_veto->GetNbinsX()+1)-ferr;
  ymin_h_mu_SF_veto = h_mu_SF_veto->GetYaxis()->GetBinLowEdge(1)+ferr;
  ymax_h_mu_SF_veto = h_mu_SF_veto->GetYaxis()->GetBinLowEdge(h_mu_SF_veto->GetNbinsY()+1)-ferr;

  // Since there's no IP SFs from SUSY in 2017, only apply them in 2016
  if (year <= 2016) {
    TH2F *h_mu_SF_ip  = (TH2F*)h_mu_SF_ip_temp->Clone("h_mu_SF_ip");
    // Double unc. on selected muon ip sfs, since not for our exact wp
    for (int x=1; x<=(int)h_mu_SF_ip->GetNbinsX(); x++) {
      for (int y=1; y<=(int)h_mu_SF_ip->GetNbinsY(); y++) {
        h_mu_SF_ip->SetBinError(x,y,h_mu_SF_ip->GetBinError(x,y)*2.0);
      }
    }
    h_mu_SF->Multiply(h_mu_SF_ip);

    TH2F *h_mu_SF_veto_ip  = (TH2F*)h_mu_SF_veto_ip_temp->Clone("h_mu_SF_veto_ip");
    // Double unc. on veto muon ip sfs, since not for our exact wp
    for (int x=1; x<=(int)h_mu_SF_veto_ip->GetNbinsX(); x++) {
      for (int y=1; y<=(int)h_mu_SF_veto_ip->GetNbinsY(); y++) {
        h_mu_SF_veto_ip->SetBinError(x,y,h_mu_SF_veto_ip->GetBinError(x,y)*2.0);
      }
    }
    h_mu_SF_veto->Multiply(h_mu_SF_veto_ip);
  }

  // Get final fullsim/fastsim, selected el, sfs
  TH2F* h_el_FS_ID  = (TH2F*)h_el_FS_ID_temp->Clone("h_el_FS_ID");
  TH2F* h_el_FS_Iso = (TH2F*)h_el_FS_Iso_temp->Clone("h_el_FS_Iso");
  h_el_FS = (TH2F*)h_el_FS_ID->Clone("h_el_FS");
  h_el_FS->Multiply(h_el_FS_Iso);
  xmin_h_el_FS = h_el_FS->GetXaxis()->GetBinLowEdge(1)+ferr;
  xmax_h_el_FS = h_el_FS->GetXaxis()->GetBinLowEdge(h_el_FS->GetNbinsX()+1)-ferr;
  ymin_h_el_FS = h_el_FS->GetYaxis()->GetBinLowEdge(1)+ferr;
  ymax_h_el_FS = h_el_FS->GetYaxis()->GetBinLowEdge(h_el_FS->GetNbinsY()+1)-ferr;

  // Get final fullsim/fastsim, veto el, sfs
  TH2F* h_el_veto_FS_ID  = (TH2F*)h_el_veto_FS_ID_temp->Clone("h_el_veto_FS_ID");
  TH2F* h_el_veto_FS_Iso = (TH2F*)h_el_veto_FS_Iso_temp->Clone("h_el_veto_FS_Iso");
  h_el_veto_FS = (TH2F*)h_el_veto_FS_ID->Clone("h_el_FS");
  h_el_veto_FS->Multiply(h_el_veto_FS_Iso);
  xmin_h_el_veto_FS = h_el_veto_FS->GetXaxis()->GetBinLowEdge(1)+ferr;
  xmax_h_el_veto_FS = h_el_veto_FS->GetXaxis()->GetBinLowEdge(h_el_veto_FS->GetNbinsX()+1)-ferr;
  ymin_h_el_veto_FS = h_el_veto_FS->GetYaxis()->GetBinLowEdge(1)+ferr;
  ymax_h_el_veto_FS = h_el_veto_FS->GetYaxis()->GetBinLowEdge(h_el_veto_FS->GetNbinsY()+1)-ferr;

  // Get final fullsim/fastsim, selected mu, sfs
  TH2F* h_mu_FS_ID  = (TH2F*)h_mu_FS_ID_temp->Clone("h_mu_FS_ID");
  TH2F* h_mu_FS_Iso = (TH2F*)h_mu_FS_Iso_temp->Clone("h_mu_FS_Iso");
  TH2F* h_mu_FS_Ip  = (TH2F*)h_mu_FS_Ip_temp->Clone("h_mu_FS_Ip");
  // Double unc. on selected muon FS iso sfs, since not for our exact wp
  for (int x=1; x<=(int)h_mu_FS_Iso->GetNbinsX(); x++) {
    for (int y=1; y<=(int)h_mu_FS_Iso->GetNbinsY(); y++) {
      h_mu_FS_Iso->SetBinError(x,y,h_mu_FS_Iso->GetBinError(x,y)*2.0);
    }
  }
  // Double unc. on selected muon FS ip sfs, since not for our exact wp
  for (int x=1; x<=(int)h_mu_FS_Ip->GetNbinsX(); x++) {
    for (int y=1; y<=(int)h_mu_FS_Ip->GetNbinsY(); y++) {
      h_mu_FS_Ip->SetBinError(x,y,h_mu_FS_Ip->GetBinError(x,y)*2.0);
    }
  }
  h_mu_FS = (TH2F*)h_mu_FS_ID->Clone("h_mu_FS");
  h_mu_FS->Multiply(h_mu_FS_Iso);
  h_mu_FS->Multiply(h_mu_FS_Ip);
  xmin_h_mu_FS = h_mu_FS->GetXaxis()->GetBinLowEdge(1)+ferr;
  xmax_h_mu_FS = h_mu_FS->GetXaxis()->GetBinLowEdge(h_mu_FS->GetNbinsX()+1)-ferr;
  ymin_h_mu_FS = h_mu_FS->GetYaxis()->GetBinLowEdge(1)+ferr;
  ymax_h_mu_FS = h_mu_FS->GetYaxis()->GetBinLowEdge(h_mu_FS->GetNbinsY()+1)-ferr;

  // Get final fullsim/fastsim, veto mu, sfs
  TH2F* h_mu_veto_FS_ID  = (TH2F*)h_mu_veto_FS_ID_temp->Clone("h_mu_veto_FS_ID");
  TH2F* h_mu_veto_FS_Iso = (TH2F*)h_mu_veto_FS_Iso_temp->Clone("h_mu_veto_FS_Iso");
  TH2F* h_mu_veto_FS_Ip  = (TH2F*)h_mu_veto_FS_Ip_temp->Clone("h_mu_veto_FS_Ip");
  // Double unc. on selected muon FS ip sfs, since not for our exact wp
  for (int x=1; x <= h_mu_veto_FS_Ip->GetNbinsX(); x++) {
    for (int y=1; y <= h_mu_veto_FS_Ip->GetNbinsY(); y++) {
      h_mu_veto_FS_Ip->SetBinError(x,y,h_mu_veto_FS_Ip->GetBinError(x,y)*2.0);
    }
  }
  h_mu_veto_FS = (TH2F*)h_mu_veto_FS_ID->Clone("h_mu_veto_FS");
  h_mu_veto_FS->Multiply(h_mu_veto_FS_Iso);
  h_mu_veto_FS->Multiply(h_mu_veto_FS_Ip);
  xmin_h_mu_veto_FS = h_mu_veto_FS->GetXaxis()->GetBinLowEdge(1)+ferr;
  xmax_h_mu_veto_FS = h_mu_veto_FS->GetXaxis()->GetBinLowEdge(h_mu_veto_FS->GetNbinsX()+1)-ferr;
  ymin_h_mu_veto_FS = h_mu_veto_FS->GetYaxis()->GetBinLowEdge(1)+ferr;
  ymax_h_mu_veto_FS = h_mu_veto_FS->GetYaxis()->GetBinLowEdge(h_mu_veto_FS->GetNbinsY()+1)-ferr;


  // Lepton efficiencies for Lost Leptons
  h_el_vetoLepEff = (TH2F*)h_el_vetoLepEff_temp->Clone("h_el_vetoLepEff");
  h_mu_vetoLepEff = (TH2F*)h_mu_vetoLepEff_temp->Clone("h_mu_vetoLepEff");
  xmin_h_el_vetoLepEff = h_el_vetoLepEff->GetXaxis()->GetBinLowEdge(1)+ferr;
  xmax_h_el_vetoLepEff = h_el_vetoLepEff->GetXaxis()->GetBinLowEdge(h_el_vetoLepEff->GetNbinsX()+1)-ferr;
  ymin_h_el_vetoLepEff = h_el_vetoLepEff->GetYaxis()->GetBinLowEdge(1)+ferr;
  ymax_h_el_vetoLepEff = h_el_vetoLepEff->GetYaxis()->GetBinLowEdge(h_el_vetoLepEff->GetNbinsY()+1)-ferr;
  xmin_h_mu_vetoLepEff = h_mu_vetoLepEff->GetXaxis()->GetBinLowEdge(1)+ferr;
  xmax_h_mu_vetoLepEff = h_mu_vetoLepEff->GetXaxis()->GetBinLowEdge(h_mu_vetoLepEff->GetNbinsX()+1)-ferr;
  ymin_h_mu_vetoLepEff = h_mu_vetoLepEff->GetYaxis()->GetBinLowEdge(1)+ferr;
  ymax_h_mu_vetoLepEff = h_mu_vetoLepEff->GetYaxis()->GetBinLowEdge(h_mu_vetoLepEff->GetNbinsY()+1)-ferr;

}

//////////////////////////////////////////////////////////////////////

eventWeight_lepSF::~eventWeight_lepSF() {

  // Fullsim Electron file
  delete f_el_SF;
  delete f_el_SF_tracking;

  // Fullsim Muon files
  delete f_mu_SF_id;
  delete f_mu_SF_iso;
  delete f_mu_SF_ip;
  // delete f_mu_SF_tracking;
  delete f_mu_SF_veto_id;
  delete f_mu_SF_veto_iso;
  delete f_mu_SF_veto_ip;

  // Fullsim/Fastsim Electron files
  delete f_el_FS_ID;
  delete f_el_FS_Iso;
  delete f_el_veto_FS_ID;
  delete f_el_veto_FS_Iso;

  // Fullsim/Fastsim Muon files
  delete f_mu_FS_ID;
  delete f_mu_FS_Iso;
  delete f_mu_FS_Ip;
  delete f_mu_veto_FS_ID;
  delete f_mu_veto_FS_Iso;
  delete f_mu_veto_FS_Ip;

  // Lepton MC reco efficiency for veto lep IDs
  delete f_vetoLep_eff;

}

//////////////////////////////////////////////////////////////////////

void eventWeight_lepSF::getLepWeight(vector<float> recoLep_pt, vector<float> recoLep_eta, vector<int> recoLep_pdgid, vector<bool> recoLep_isSel, vector<float> genLostLep_pt, vector<float> genLostLep_eta, vector<int> genLostLep_pdgid, double &weight_lepSF, double &weight_lepSF_Up, double &weight_lepSF_Dn, double &weight_lepSF_FS, double &weight_lepSF_FS_Up, double &weight_lepSF_FS_Dn, double &weight_vetoLepSF, double &weight_vetoLepSF_Up, double &weight_vetoLepSF_Dn ) {

  weight_lepSF        = 1.0;
  weight_lepSF_Up     = 1.0;
  weight_lepSF_Dn     = 1.0;
  weight_lepSF_FS     = 1.0;
  weight_lepSF_FS_Up  = 1.0;
  weight_lepSF_FS_Dn  = 1.0;
  weight_vetoLepSF    = 1.0;
  weight_vetoLepSF_Up = 1.0;
  weight_vetoLepSF_Dn = 1.0;

  // Input sanitation
  if ( recoLep_pt.size() != recoLep_eta.size() ) {
    std::cout << "lep sf vectors for lep pt, eta are different sizes, exiting..." << std::endl;
    return;
  }
  if ( recoLep_eta.size() != recoLep_pdgid.size() ) {
    std::cout << "lep sf vectors for lep eta, pdgid are different sizes, exiting..." << std::endl;
    return;
  }
  if ( genLostLep_pt.size() != genLostLep_eta.size() ) {
    std::cout << "lep sf vectors for lep pt, eta are different sizes, exiting..." << std::endl;
    return;
  }
  if ( genLostLep_eta.size() != genLostLep_pdgid.size() ) {
    std::cout << "lep sf vectors for lep eta, pdgid are different sizes, exiting..." << std::endl;
    return;
  }

  // Reco Lepton Scale Factors
  // int binX = -99;
  // int binY = -99;
  for (size_t iLep=0; iLep < recoLep_pt.size(); iLep++) {

    if (abs(recoLep_pdgid[iLep]) == 11) {

      int binX = h_el_SF->GetXaxis()->FindBin( std::max( std::min(xmax_h_el_SF, recoLep_eta[iLep]), xmin_h_el_SF ) ); // SCeta
      int binY = h_el_SF->GetYaxis()->FindBin( std::max( std::min(ymax_h_el_SF, recoLep_pt[iLep] ), ymin_h_el_SF ) );
      if (year == 2016) {
        binX = h_el_SF->GetXaxis()->FindBin( std::max( std::min(xmax_h_el_SF, recoLep_pt[iLep] ), xmin_h_el_SF ) );
        binY = h_el_SF->GetYaxis()->FindBin( std::max( std::min(ymax_h_el_SF, recoLep_eta[iLep]), ymin_h_el_SF ) );
      }
      int bin_FS = -1;
      if (sampleIsFastsim) {
        bin_FS = h_el_FS->FindBin( std::max( std::min(xmax_h_el_FS, recoLep_pt[iLep]), xmin_h_el_FS ), std::max( std::min(ymax_h_el_FS, fabs(recoLep_eta[iLep]) ), ymin_h_el_FS ) );
      }

      if (recoLep_isSel[iLep]) {
        weight_lepSF    *= h_el_SF->GetBinContent( binX, binY );
        weight_lepSF_Up *= ( weight_lepSF + h_el_SF->GetBinError( binX, binY ) );
        weight_lepSF_Dn *= ( weight_lepSF - h_el_SF->GetBinError( binX, binY ) );

        if (sampleIsFastsim) {
          weight_lepSF_FS    *= h_el_FS->GetBinContent(bin_FS);
          weight_lepSF_FS_Up *= (weight_lepSF_FS + h_el_FS->GetBinError(bin_FS));
          weight_lepSF_FS_Dn *= (weight_lepSF_FS - h_el_FS->GetBinError(bin_FS));
        }
      } else {
        // Should have the same binning as above
        // int binX = h_el_SF_veto->GetXaxis()->FindBin( std::max( std::min(xmax_h_el_SF_veto, recoLep_pt[iLep]), xmin_h_el_SF_veto ) );
        // int binY = h_el_SF_veto->GetYaxis()->FindBin( std::max( std::min(ymax_h_el_SF_veto, fabs(recoLep_eta[iLep]) ), ymin_h_el_SF_veto ) );
        weight_lepSF    *= h_el_SF_veto->GetBinContent( binX, binY );
        weight_lepSF_Up *= ( weight_lepSF + h_el_SF_veto->GetBinError( binX, binY ) );
        weight_lepSF_Dn *= ( weight_lepSF - h_el_SF_veto->GetBinError( binX, binY ) );

        if (sampleIsFastsim) {
          weight_lepSF_FS    *= h_el_veto_FS->GetBinContent(bin_FS);
          weight_lepSF_FS_Up *= (weight_lepSF_FS + h_el_veto_FS->GetBinError(bin_FS));
          weight_lepSF_FS_Dn *= (weight_lepSF_FS - h_el_veto_FS->GetBinError(bin_FS));
        }
      }

      if (recoLep_pt[iLep] >= ymin_h_el_SF_tracking) {
        binX = h_el_SF_tracking->GetXaxis()->FindBin( std::max( std::min(xmax_h_el_SF_tracking, recoLep_eta[iLep]), xmin_h_el_SF_tracking) );
        binY = h_el_SF_tracking->GetYaxis()->FindBin( std::max( std::min(ymax_h_el_SF_tracking, recoLep_pt[iLep]),  ymin_h_el_SF_tracking) );
        weight_lepSF *= h_el_SF_tracking->GetBinContent( binX, binY );
        weight_lepSF_Up *= ( h_el_SF_tracking->GetBinContent(binX,binY) + h_el_SF_tracking->GetBinError(binX,binY) );
        weight_lepSF_Dn *= ( h_el_SF_tracking->GetBinContent(binX,binY) - h_el_SF_tracking->GetBinError(binX,binY) );
      } else {
        binX = h_el_SF_trklowpt->GetXaxis()->FindBin( std::max( std::min(xmax_h_el_SF_tracking, recoLep_eta[iLep]), xmin_h_el_SF_tracking) );
        binY = h_el_SF_trklowpt->GetYaxis()->FindBin( std::max( std::min(ymax_h_el_SF_trklowpt, recoLep_pt[iLep]),  ymin_h_el_SF_trklowpt) );
        weight_lepSF *= h_el_SF_trklowpt->GetBinContent( binX, binY );
        weight_lepSF_Up *= ( h_el_SF_trklowpt->GetBinContent(binX,binY) + h_el_SF_trklowpt->GetBinError(binX,binY) );
        weight_lepSF_Dn *= ( h_el_SF_trklowpt->GetBinContent(binX,binY) - h_el_SF_trklowpt->GetBinError(binX,binY) );
      }
    } // end if lep if el
    else if (abs(recoLep_pdgid[iLep]) == 13) {

      if (recoLep_isSel[iLep]) {
        int binX = h_mu_SF->GetXaxis()->FindBin( std::max( std::min(xmax_h_mu_SF, recoLep_pt[iLep]), xmin_h_mu_SF ) );
        int binY = h_mu_SF->GetYaxis()->FindBin( std::max( std::min(ymax_h_mu_SF, fabs(recoLep_eta[iLep])), ymin_h_mu_SF ) );
        weight_lepSF    *= h_mu_SF->GetBinContent( binX, binY );
        weight_lepSF_Up *= ( weight_lepSF + h_mu_SF->GetBinError( binX, binY ) );
        weight_lepSF_Dn *= ( weight_lepSF - h_mu_SF->GetBinError( binX, binY ) );

        if (sampleIsFastsim) {
          int bin_FS  = h_mu_FS->FindBin( std::max( std::min(xmax_h_mu_FS, (float)recoLep_pt[iLep]), xmin_h_mu_FS ), std::max( std::min(ymax_h_mu_FS, float(fabs(recoLep_eta[iLep]) )), ymin_h_mu_FS ) );
          weight_lepSF_FS    *= h_mu_FS->GetBinContent(bin_FS);
          weight_lepSF_FS_Up *= weight_lepSF_FS + h_mu_FS->GetBinError(bin_FS);
          weight_lepSF_FS_Dn *= weight_lepSF_FS - h_mu_FS->GetBinError(bin_FS);
        }
      } else {
        int binX = h_mu_SF_veto->GetXaxis()->FindBin( std::max( std::min(xmax_h_mu_SF_veto, recoLep_pt[iLep]), xmin_h_mu_SF_veto ) );
        int binY = h_mu_SF_veto->GetYaxis()->FindBin( std::max( std::min(ymax_h_mu_SF_veto, fabs(recoLep_eta[iLep])), ymin_h_mu_SF_veto ) );
        weight_lepSF    *= h_mu_SF_veto->GetBinContent( binX, binY );
        weight_lepSF_Up *= ( weight_lepSF + h_mu_SF_veto->GetBinError( binX, binY ) );
        weight_lepSF_Dn *= ( weight_lepSF - h_mu_SF_veto->GetBinError( binX, binY ) );

        if (sampleIsFastsim) {
          int bin_FS  = h_mu_veto_FS->FindBin( std::max( std::min(xmax_h_mu_veto_FS, recoLep_pt[iLep]), xmin_h_mu_veto_FS ), std::max( std::min(ymax_h_mu_veto_FS, fabs(recoLep_eta[iLep])), ymin_h_mu_veto_FS ) );
          weight_lepSF_FS    *= h_mu_veto_FS->GetBinContent(bin_FS);
          weight_lepSF_FS_Up *= weight_lepSF_FS + h_mu_veto_FS->GetBinError(bin_FS);
          weight_lepSF_FS_Dn *= weight_lepSF_FS - h_mu_veto_FS->GetBinError(bin_FS);
        }
      }

      // Muon tracking is no longer recommended for both 2016 and 2017 data
      // int binX = h_mu_SF_tracking->GetXaxis()->FindBin( std::max( std::min(xmax_h_mu_SF_tracking, recoLep_eta[iLep]), xmin_h_mu_SF_tracking ) );
      // weight_lepSF *= h_mu_SF_tracking->GetBinContent( binX );
      // weight_lepSF_Up *= ( h_mu_SF_tracking->GetBinContent(binX) + h_mu_SF_tracking->GetBinError(binX) );
      // weight_lepSF_Dn *= ( h_mu_SF_tracking->GetBinContent(binX) - h_mu_SF_tracking->GetBinError(binX) );
    } // end if 2nd lep is mu

  } // end loop over reco leptons


  if (sampleIsFastsim) return;

  // GenLostLepton scale factors
  int binX = -99;
  int binY = -99;
  for (size_t iLep=0; iLep < genLostLep_pt.size(); iLep++) {

    // if ( abs(genLostLep_pdgid[iLep])!=11 && abs(genLostLep_pdgid[iLep])!=13 ) continue;
    if ( genLostLep_pt[iLep] <= 5.0 ) continue;
    if ( fabs(genLostLep_eta[iLep]) >= 2.4 ) continue;

    // Gen Lost Electron
    if ( abs(genLostLep_pdgid[iLep])==11 ) {

      binX = h_el_vetoLepEff->GetXaxis()->FindBin( std::max( std::min(xmax_h_el_vetoLepEff, genLostLep_pt[iLep]), xmin_h_el_vetoLepEff ) );
      binY = h_el_vetoLepEff->GetYaxis()->FindBin( std::max( std::min(ymax_h_el_vetoLepEff, fabs(genLostLep_eta[iLep])), ymin_h_el_vetoLepEff ) );
      double vetoEff = h_el_vetoLepEff->GetBinContent( binX, binY );

      binX = h_el_SF_veto->GetXaxis()->FindBin( std::max( std::min(xmax_h_el_SF_veto, genLostLep_eta[iLep]), xmin_h_el_SF_veto ) );
      binY = h_el_SF_veto->GetYaxis()->FindBin( std::max( std::min(ymax_h_el_SF_veto, genLostLep_pt[iLep]),  ymin_h_el_SF_veto ) );
      double vetoLepSF_temp    = h_el_SF_veto->GetBinContent( binX, binY );
      double vetoLepSF_temp_Up = (vetoLepSF_temp + h_el_SF_veto->GetBinError(binX, binY));
      double vetoLepSF_temp_Dn = (vetoLepSF_temp - h_el_SF_veto->GetBinError(binX, binY));

      if ( (1-vetoEff)>0.0 ) {
        weight_vetoLepSF    *= ( 1-(vetoEff*vetoLepSF_temp) )/( 1-vetoEff );
        weight_vetoLepSF_Up *= ( 1-(vetoEff*vetoLepSF_temp_Up) )/( 1-vetoEff );
        weight_vetoLepSF_Dn *= ( 1-(vetoEff*vetoLepSF_temp_Dn) )/( 1-vetoEff );
      } else {
        weight_vetoLepSF    *= 1.0;
        weight_vetoLepSF_Up *= 1.0;
        weight_vetoLepSF_Dn *= 1.0;
      }

    } // end if genLostLepton is electron

    // Gen Lost Muon
    else if ( abs(genLostLep_pdgid[iLep])==13 ) {

      binX = h_mu_vetoLepEff->GetXaxis()->FindBin( std::max( std::min(xmax_h_mu_vetoLepEff, genLostLep_pt[iLep]), xmin_h_mu_vetoLepEff ) );
      binY = h_mu_vetoLepEff->GetYaxis()->FindBin( std::max( std::min(ymax_h_mu_vetoLepEff, fabs(genLostLep_eta[iLep])), ymin_h_mu_vetoLepEff ) );
      double vetoEff = h_mu_vetoLepEff->GetBinContent( binX, binY );

      binX = h_mu_SF_veto->GetXaxis()->FindBin( std::max( std::min(xmax_h_mu_SF_veto, genLostLep_pt[iLep]), xmin_h_mu_SF_veto ) );
      binY = h_mu_SF_veto->GetYaxis()->FindBin( std::max( std::min(ymax_h_mu_SF_veto, fabs(genLostLep_eta[iLep])), ymin_h_mu_SF_veto ) );
      double vetoLepSF_temp    = h_mu_SF_veto->GetBinContent( binX, binY );
      double vetoLepSF_temp_Up = (vetoLepSF_temp + h_mu_SF_veto->GetBinError(binX, binY));
      double vetoLepSF_temp_Dn = (vetoLepSF_temp - h_mu_SF_veto->GetBinError(binX, binY));

      if ( (1-vetoEff)>0.0 ) {
        weight_vetoLepSF    *= ( 1-(vetoEff*vetoLepSF_temp) )/( 1-vetoEff );
        weight_vetoLepSF_Up *= ( 1-(vetoEff*vetoLepSF_temp_Up) )/( 1-vetoEff );
        weight_vetoLepSF_Dn *= ( 1-(vetoEff*vetoLepSF_temp_Dn) )/( 1-vetoEff );
      } else {
        weight_vetoLepSF    *= 1.0;
        weight_vetoLepSF_Up *= 1.0;
        weight_vetoLepSF_Dn *= 1.0;
      }

    } // end if genLostLepton is muon

    break; // break after finding 2nd hard gen lepton
  } // end loop over genLostLeptons

  return;
}

//////////////////////////////////////////////////////////////////////
