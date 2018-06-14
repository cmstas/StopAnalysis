#include "eventWeight_bTagSF.h"

//////////////////////////////////////////////////////////////////////

eventWeight_bTagSF::eventWeight_bTagSF( bool isFastsim ){

  sampleIsFastsim = isFastsim;

  // DeepCSV working points 94X
  BTAG_LSE = 0.1522;
  BTAG_MED = 0.4941;
  BTAG_TGT = 0.8001;

  std::cout << "[eventWeight_bTagSF] Loading btag SFs..." << std::endl;

  // 25s version of SFs
  calib = new BTagCalibration("DeepCSV", "btagsf/run2_25ns/DeepCSV_94XSF_V2_B_F.csv"); // DeepCSV version of SFs
  reader_medium = new BTagCalibrationReader(BTagEntry::OP_MEDIUM, "central", {"up", "down"});
  reader_medium->load(*calib, BTagEntry::FLAV_B, "comb");
  reader_medium->load(*calib, BTagEntry::FLAV_C, "comb");
  reader_medium->load(*calib, BTagEntry::FLAV_UDSG, "incl");
  reader_tight = new BTagCalibrationReader(BTagEntry::OP_TIGHT, "central", {"up", "down"});
  reader_tight->load(*calib, BTagEntry::FLAV_B, "comb");
  reader_tight->load(*calib, BTagEntry::FLAV_C, "comb");
  reader_tight->load(*calib, BTagEntry::FLAV_UDSG, "incl");
  reader_loose = new BTagCalibrationReader(BTagEntry::OP_LOOSE, "central", {"up", "down"});
  reader_loose->load(*calib, BTagEntry::FLAV_B, "comb");
  reader_loose->load(*calib, BTagEntry::FLAV_C, "comb");
  reader_loose->load(*calib, BTagEntry::FLAV_UDSG, "incl");

  calib_fastsim = new BTagCalibration("deepcsv", "btagsf/run2_fastsim/fastsim_deepcsv_ttbar_26_1_2017.csv"); // DeepCSV fastsim version of SFs
  reader_medium_FS = new BTagCalibrationReader(BTagEntry::OP_MEDIUM, "central", {"up", "down"});
  reader_medium_FS->load(*calib_fastsim, BTagEntry::FLAV_B, "fastsim");
  reader_medium_FS->load(*calib_fastsim, BTagEntry::FLAV_C, "fastsim");
  reader_medium_FS->load(*calib_fastsim, BTagEntry::FLAV_UDSG, "fastsim");
  reader_tight_FS = new BTagCalibrationReader(BTagEntry::OP_TIGHT, "central", {"up", "down"});
  reader_tight_FS->load(*calib_fastsim, BTagEntry::FLAV_B, "fastsim");
  reader_tight_FS->load(*calib_fastsim, BTagEntry::FLAV_C, "fastsim");
  reader_tight_FS->load(*calib_fastsim, BTagEntry::FLAV_UDSG, "fastsim");
  reader_loose_FS = new BTagCalibrationReader(BTagEntry::OP_LOOSE, "central", {"up", "down"});
  reader_loose_FS->load(*calib_fastsim, BTagEntry::FLAV_B, "fastsim");
  reader_loose_FS->load(*calib_fastsim, BTagEntry::FLAV_C, "fastsim");
  reader_loose_FS->load(*calib_fastsim, BTagEntry::FLAV_UDSG, "fastsim");

  if(isFastsim){
    // Created using https://github.com/cmstas/bTagEfficiencyTools. Todo: change to deepCSV later
    feff =  new TFile("../StopBabyMaker/btagsf/btageff__SMS-T1bbbb-T1qqqq_25ns_Moriond17.root");
  } else {
    // Todo: create efficiency in the phase space of the stop analysis
    feff =  new TFile("../StopBabyMaker/btagsf/btageff__ttbar_powheg_pythia8_25ns_Moriond17_deepCSV.root");
  }
  if (!feff) throw std::invalid_argument("eventWeight_bTagSF.cc: btagsf file does not exist!");

  h_btag_eff_b = (TH2D*) feff->Get("h2_BTaggingEff_csv_med_Eff_b")->Clone("h_btag_eff_b");
  h_btag_eff_c = (TH2D*) feff->Get("h2_BTaggingEff_csv_med_Eff_c")->Clone("h_btag_eff_c");
  h_btag_eff_udsg = (TH2D*) feff->Get("h2_BTaggingEff_csv_med_Eff_udsg")->Clone("h_btag_eff_udsg");
  h_tight_btag_eff_b = (TH2D*) feff->Get("h2_BTaggingEff_csv_tight_Eff_b")->Clone("h_tight_btag_eff_b");
  h_tight_btag_eff_c = (TH2D*) feff->Get("h2_BTaggingEff_csv_tight_Eff_c")->Clone("h_tight_btag_eff_c");
  h_tight_btag_eff_udsg = (TH2D*) feff->Get("h2_BTaggingEff_csv_tight_Eff_udsg")->Clone("h_tight_btag_eff_udsg");
  h_loose_btag_eff_b = (TH2D*) feff->Get("h2_BTaggingEff_csv_loose_Eff_b")->Clone("h_loose_btag_eff_b");
  h_loose_btag_eff_c = (TH2D*) feff->Get("h2_BTaggingEff_csv_loose_Eff_c")->Clone("h_loose_btag_eff_c");
  h_loose_btag_eff_udsg = (TH2D*) feff->Get("h2_BTaggingEff_csv_loose_Eff_udsg")->Clone("h_loose_btag_eff_udsg");

}

//////////////////////////////////////////////////////////////////////

eventWeight_bTagSF::~eventWeight_bTagSF(){

  delete calib;
  delete calib_fastsim;
  delete reader_medium;
  delete reader_tight;
  delete reader_loose;
  delete reader_medium_FS;
  delete reader_tight_FS;
  delete reader_loose_FS;

  feff->Close();

}

//////////////////////////////////////////////////////////////////////

void eventWeight_bTagSF::getBTagWeight( int WP, std::vector< double > jet_pt, std::vector< double > jet_eta, std::vector< double > jet_CSV, std::vector< int > jet_flavour, double &weight_btagsf, double &weight_btagsf_heavy_UP, double &weight_btagsf_heavy_DN, double &weight_btagsf_light_UP, double &weight_btagsf_light_DN, double &weight_btagsf_FS_UP, double &weight_btagsf_FS_DN ){

  // Initialize output variables
  weight_btagsf          = 1.0;
  weight_btagsf_heavy_UP = 1.0;
  weight_btagsf_heavy_DN = 1.0;
  weight_btagsf_light_UP = 1.0;
  weight_btagsf_light_DN = 1.0;
  weight_btagsf_FS_UP = 1.0;
  weight_btagsf_FS_DN = 1.0;

  // Input sanitation
  if( WP<0 || WP>2 ){
    std::cout << "btag sf WP needs an argument of 0, 1, or 2, for loose, medium, or tight ID" << std::endl;
  }

  if( jet_pt.size()!=jet_eta.size() ){
    std::cout << "btag sf vectors for jet pt, eta are different sizes, exiting..." << std::endl;
    return;
  }
  if( jet_eta.size()!=jet_flavour.size() ){
    std::cout << "btag sf vectors for jet eta, flavour are different sizes, exiting..." << std::endl;
    return;
  }
  if( jet_flavour.size()!=jet_CSV.size() ){
    std::cout << "btag sf vectors for jet flavour, passCSV are different sizes, exiting..." << std::endl;
    return;
  }

  // Initialize utility variables
  double btagprob_eff  = 1.0;
  double btagprob_mc   = 1.0;
  double btagprob_data = 1.0;

  double btagprob_err_heavy_UP = 1.0;
  double btagprob_err_heavy_DN = 1.0;
  double btagprob_err_light_UP = 1.0;
  double btagprob_err_light_DN = 1.0;
  double btagprob_err_FS_UP    = 1.0;
  double btagprob_err_FS_DN    = 1.0;

  double weight_cent  = 1.0;
  double weight_UP    = 1.0;
  double weight_DN    = 1.0;
  double weight_FS_UP = 1.0;
  double weight_FS_DN = 1.0;

  // Loop over jet vectors
  for(int iJet=0; iJet<(int)jet_pt.size(); iJet++){

    BTagEntry::JetFlavor flavor = BTagEntry::FLAV_UDSG;

    // Get jet pT, eta, within cutoffs of efficiency file
    int binx=-99;
    int biny=-99;
    TH2D* h_eff = NULL;

    double pt_eff  = std::max(20.0, std::min(399.0, jet_pt[iJet])); // min 20 GeV in the eff hist
    double eta_eff = std::min(2.39, fabs(jet_eta[iJet]) );

    // double pt_reader  = std::max(30.0, std::min(669.0, jet_pt[iJet]));
    // double eta_reader = std::min(2.39, fabs(jet_eta[iJet]) );

    // bJets
    if( abs(jet_flavour[iJet])==5 ){
      flavor = BTagEntry::FLAV_B;
      pt_eff = std::max(20.0,std::min(599.0, jet_pt[iJet])); // max pt of 600.0 GeV for b

      if( WP==BTagEntry::LOOSE  ) h_eff = h_loose_btag_eff_b;
      if( WP==BTagEntry::MEDIUM ) h_eff = h_btag_eff_b;
      if( WP==BTagEntry::TIGHT  ) h_eff = h_tight_btag_eff_b;
    }
    // Charm
    else if( abs(jet_flavour[iJet])==4 ){
      flavor = BTagEntry::FLAV_C;

      if( WP==BTagEntry::LOOSE  ) h_eff = h_loose_btag_eff_c;
      if( WP==BTagEntry::MEDIUM ) h_eff = h_btag_eff_c;
      if( WP==BTagEntry::TIGHT  ) h_eff = h_tight_btag_eff_c;
    }
    // UDSG
    else{
      flavor = BTagEntry::FLAV_UDSG;

      if( WP==BTagEntry::LOOSE  ) h_eff = h_loose_btag_eff_udsg;
      if( WP==BTagEntry::MEDIUM ) h_eff = h_btag_eff_udsg;
      if( WP==BTagEntry::TIGHT  ) h_eff = h_tight_btag_eff_udsg;
    }

    binx = h_eff->GetXaxis()->FindBin(pt_eff);
    biny = h_eff->GetYaxis()->FindBin(eta_eff);
    btagprob_eff = h_eff->GetBinContent(binx,biny);

    if( WP==BTagEntry::LOOSE ){
      weight_cent = reader_loose->eval_auto_bounds("central", flavor, jet_eta[iJet], jet_pt[iJet]);
      weight_UP   = reader_loose->eval_auto_bounds("up",      flavor, jet_eta[iJet], jet_pt[iJet]);
      weight_DN   = reader_loose->eval_auto_bounds("down",    flavor, jet_eta[iJet], jet_pt[iJet]);
    }
    else if( WP==BTagEntry::MEDIUM ){
      weight_cent = reader_medium->eval_auto_bounds("central", flavor, jet_eta[iJet], jet_pt[iJet]);
      weight_UP   = reader_medium->eval_auto_bounds("up",      flavor, jet_eta[iJet], jet_pt[iJet]);
      weight_DN   = reader_medium->eval_auto_bounds("down",    flavor, jet_eta[iJet], jet_pt[iJet]);
    }
    else if( WP==BTagEntry::TIGHT ){
      weight_cent = reader_tight->eval_auto_bounds("central", flavor, jet_eta[iJet], jet_pt[iJet]);
      weight_UP   = reader_tight->eval_auto_bounds("up",      flavor, jet_eta[iJet], jet_pt[iJet]);
      weight_DN   = reader_tight->eval_auto_bounds("down",    flavor, jet_eta[iJet], jet_pt[iJet]);
    }

    // extra SF for fastsim
    if(sampleIsFastsim) {

      if( WP==BTagEntry::LOOSE ){
        weight_FS_UP = reader_loose_FS->eval_auto_bounds("up",      flavor, jet_eta[iJet], jet_pt[iJet]) * weight_cent;
        weight_FS_DN = reader_loose_FS->eval_auto_bounds("down",    flavor, jet_eta[iJet], jet_pt[iJet]) * weight_cent;
        weight_cent *= reader_loose_FS->eval_auto_bounds("central", flavor, jet_eta[iJet], jet_pt[iJet]);
        weight_UP   *= reader_loose_FS->eval_auto_bounds("central", flavor, jet_eta[iJet], jet_pt[iJet]);//this is still just btagSF
        weight_DN   *= reader_loose_FS->eval_auto_bounds("central", flavor, jet_eta[iJet], jet_pt[iJet]);//this is still just btagSF
      }
      else if( WP==BTagEntry::MEDIUM ){
        weight_FS_UP = reader_medium_FS->eval_auto_bounds("up",      flavor, jet_eta[iJet], jet_pt[iJet]) * weight_cent;
        weight_FS_DN = reader_medium_FS->eval_auto_bounds("down",    flavor, jet_eta[iJet], jet_pt[iJet]) * weight_cent;
        weight_cent *= reader_medium_FS->eval_auto_bounds("central", flavor, jet_eta[iJet], jet_pt[iJet]);
        weight_UP   *= reader_medium_FS->eval_auto_bounds("central", flavor, jet_eta[iJet], jet_pt[iJet]);//this is still just btagSF
        weight_DN   *= reader_medium_FS->eval_auto_bounds("central", flavor, jet_eta[iJet], jet_pt[iJet]);//this is still just btagSF

      }
      else if( WP==BTagEntry::TIGHT ){
        weight_FS_UP = reader_tight_FS->eval_auto_bounds("up",      flavor, jet_eta[iJet], jet_pt[iJet]) * weight_cent;
        weight_FS_DN = reader_tight_FS->eval_auto_bounds("down",    flavor, jet_eta[iJet], jet_pt[iJet]) * weight_cent;
        weight_cent *= reader_tight_FS->eval_auto_bounds("central", flavor, jet_eta[iJet], jet_pt[iJet]);
        weight_UP   *= reader_tight_FS->eval_auto_bounds("central", flavor, jet_eta[iJet], jet_pt[iJet]);//this is still just btagSF
        weight_DN   *= reader_tight_FS->eval_auto_bounds("central", flavor, jet_eta[iJet], jet_pt[iJet]);//this is still just btagSF
      }

    }


    // Check if CSV passes WP
    bool passWP = false;
    if( WP==0 && jet_CSV[iJet]>BTAG_LSE ) passWP = true;
    if( WP==1 && jet_CSV[iJet]>BTAG_MED ) passWP = true;
    if( WP==2 && jet_CSV[iJet]>BTAG_TGT ) passWP = true;

    // If jet is bTagged
    if( passWP ){

      btagprob_data *= (weight_cent * btagprob_eff);
      btagprob_mc   *= btagprob_eff;

      if (flavor == BTagEntry::FLAV_UDSG) {
        btagprob_err_light_UP *= weight_UP*btagprob_eff;
        btagprob_err_light_DN *= weight_DN*btagprob_eff;
        btagprob_err_heavy_UP *= weight_cent*btagprob_eff;
        btagprob_err_heavy_DN *= weight_cent*btagprob_eff;
      }
      else {
        btagprob_err_light_UP *= weight_cent*btagprob_eff;
        btagprob_err_light_DN *= weight_cent*btagprob_eff;
        btagprob_err_heavy_UP *= weight_UP*btagprob_eff;
        btagprob_err_heavy_DN *= weight_DN*btagprob_eff;
      }

      if(sampleIsFastsim){
        btagprob_err_FS_UP *= weight_FS_UP*btagprob_eff;
        btagprob_err_FS_DN *= weight_FS_DN*btagprob_eff;
      }

    } // end if jet is bTagged

    // if jet fails bTag
    else{

      btagprob_data *= (1. - weight_cent * btagprob_eff);
      btagprob_mc   *= (1. - btagprob_eff);

      if (flavor == BTagEntry::FLAV_UDSG) {
        btagprob_err_light_UP *= (1. - weight_UP * btagprob_eff);
        btagprob_err_light_DN *= (1. - weight_DN * btagprob_eff);
        btagprob_err_heavy_UP *= (1. - weight_cent * btagprob_eff);
        btagprob_err_heavy_DN *= (1. - weight_cent * btagprob_eff);
      }
      else {
        btagprob_err_light_UP *= (1. - weight_cent * btagprob_eff);
        btagprob_err_light_DN *= (1. - weight_cent * btagprob_eff);
        btagprob_err_heavy_UP *= (1. - weight_UP * btagprob_eff);
        btagprob_err_heavy_DN *= (1. - weight_DN * btagprob_eff);
      }

      if(sampleIsFastsim){
        btagprob_err_FS_UP *= (1. - weight_FS_UP * btagprob_eff);
        btagprob_err_FS_DN *= (1. - weight_FS_DN * btagprob_eff);
      }

    } // end if jet not bTagged


  } // end loop over jets


  // Set values of btagsf weights
  weight_btagsf          = (btagprob_data / btagprob_mc);
  weight_btagsf_heavy_UP = (btagprob_err_heavy_UP / btagprob_mc);
  weight_btagsf_light_UP = (btagprob_err_light_UP / btagprob_mc);
  weight_btagsf_heavy_DN = (btagprob_err_heavy_DN / btagprob_mc);
  weight_btagsf_light_DN = (btagprob_err_light_DN / btagprob_mc);
  weight_btagsf_FS_UP    = (btagprob_err_FS_UP / btagprob_mc);
  weight_btagsf_FS_DN    = (btagprob_err_FS_DN / btagprob_mc);

  return;
}

//////////////////////////////////////////////////////////////////////
