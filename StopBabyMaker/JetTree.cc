#include "JetTree.h"
#include "CMS3.h"
#include "Config.h"
#include "JetSelections.h"
#include "btagsf/BTagCalibrationStandalone.h"
#include "Math/VectorUtil.h"
#include "TLorentzVector.h"

#include "TFTopTagger/include/TopTaggerResults.h"
#include "TFTopTagger/include/TopTaggerUtilities.h"

using namespace tas;
 
JetTree::JetTree() : doResolveTopMVA(false) {}

JetTree::JetTree(const std::string &prefix) : prefix_(prefix), doResolveTopMVA(false) {}

JetTree::~JetTree () {
  // delete resTopMVA;
}

void JetTree::InitTopMVA(ResolvedTopMVA* resTopMVAptr) {
  doResolveTopMVA = true;
  resTopMVA = resTopMVAptr;

  tftagger = new TopTagger;
  tftagger->setCfgFile("TFTopTagger/TopTagger.cfg");
}

void JetTree::InitBtagSFTool(bool isFastsim_) {
    isFastsim = isFastsim_;
    // DeepCSV version of SFs
    calib = new BTagCalibration("DeepCSV", "btagsf/run2_25ns/"+gconf.fn_btagSF_DeepCSV);
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

    // DeepCSV fastsim version of SFs
    calib_fastsim = new BTagCalibration("deepcsv", "btagsf/run2_fastsim/"+gconf.fn_btagSF_FS_DeepCSV);
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

    TH2D* h_btag_eff_b_temp = NULL;
    TH2D* h_btag_eff_c_temp = NULL;
    TH2D* h_btag_eff_udsg_temp = NULL;
    TH2D* h_tight_btag_eff_b_temp = NULL;
    TH2D* h_tight_btag_eff_c_temp = NULL;
    TH2D* h_tight_btag_eff_udsg_temp = NULL;
    TH2D* h_loose_btag_eff_b_temp = NULL;
    TH2D* h_loose_btag_eff_c_temp = NULL;
    TH2D* h_loose_btag_eff_udsg_temp = NULL;
    if(isFastsim){
      // Created using https://github.com/cmstas/bTagEfficiencyTools. TODO: change to deepCSV version
      feff =  new TFile("btagsf/run2_fastsim/btageff__SMS-T1bbbb-T1qqqq_25ns_Moriond17.root");
    } else {
      // TODO: create efficiency in the phase space of the stop analysis
      if (gconf.cmssw_ver == 94)
        feff =  new TFile("btagsf/run2_25ns/btageff__ttbar_amc_94X_deepCSV.root");
      else if (gconf.cmssw_ver == 80)
        feff =  new TFile("btagsf/run2_25ns/btageff__ttbar_powheg_pythia8_25ns_Moriond17_deepCSV.root");
    }
    if (!feff) throw std::invalid_argument("JetTree.cc: btagsf file does not exist!");
    h_btag_eff_b_temp = (TH2D*) feff->Get("h2_BTaggingEff_csv_med_Eff_b");
    h_btag_eff_c_temp = (TH2D*) feff->Get("h2_BTaggingEff_csv_med_Eff_c");
    h_btag_eff_udsg_temp = (TH2D*) feff->Get("h2_BTaggingEff_csv_med_Eff_udsg");
    h_tight_btag_eff_b_temp = (TH2D*) feff->Get("h2_BTaggingEff_csv_tight_Eff_b");
    h_tight_btag_eff_c_temp = (TH2D*) feff->Get("h2_BTaggingEff_csv_tight_Eff_c");
    h_tight_btag_eff_udsg_temp = (TH2D*) feff->Get("h2_BTaggingEff_csv_tight_Eff_udsg");
    h_loose_btag_eff_b_temp = (TH2D*) feff->Get("h2_BTaggingEff_csv_loose_Eff_b");
    h_loose_btag_eff_c_temp = (TH2D*) feff->Get("h2_BTaggingEff_csv_loose_Eff_c");
    h_loose_btag_eff_udsg_temp = (TH2D*) feff->Get("h2_BTaggingEff_csv_loose_Eff_udsg");

    if (!h_btag_eff_b_temp) throw std::invalid_argument("JetTree.cc: can't find the btagging efficiency histogram!");
    h_btag_eff_b = (TH2D*) h_btag_eff_b_temp->Clone("h_btag_eff_b");
    h_btag_eff_c = (TH2D*) h_btag_eff_c_temp->Clone("h_btag_eff_c");
    h_btag_eff_udsg = (TH2D*) h_btag_eff_udsg_temp->Clone("h_btag_eff_udsg");
    h_tight_btag_eff_b = (TH2D*) h_tight_btag_eff_b_temp->Clone("h_tight_btag_eff_b");
    h_tight_btag_eff_c = (TH2D*) h_tight_btag_eff_c_temp->Clone("h_tight_btag_eff_c");
    h_tight_btag_eff_udsg = (TH2D*) h_tight_btag_eff_udsg_temp->Clone("h_tight_btag_eff_udsg");
    h_loose_btag_eff_b = (TH2D*) h_loose_btag_eff_b_temp->Clone("h_loose_btag_eff_b");
    h_loose_btag_eff_c = (TH2D*) h_loose_btag_eff_c_temp->Clone("h_loose_btag_eff_c");
    h_loose_btag_eff_udsg = (TH2D*) h_loose_btag_eff_udsg_temp->Clone("h_loose_btag_eff_udsg");

    //cout << h_btag_eff_b->Integral() << " " << h_btag_eff_c->Integral() << " " << h_btag_eff_udsg->Integral() << " " << endl; 
    //cout << h_tight_btag_eff_b->Integral() << " " << h_tight_btag_eff_c->Integral() << " " << h_tight_btag_eff_udsg->Integral() << " " << endl;
    //cout << h_loose_btag_eff_b->Integral() << " " << h_loose_btag_eff_c->Integral() << " " << h_loose_btag_eff_udsg->Integral() << " " << endl;
    //feff->Close();
    //h_btag_eff_b = h_btag_eff_b_;
    //h_btag_eff_c = h_btag_eff_c_;
    //h_btag_eff_udsg = h_btag_eff_udsg_;
    std::cout << "loaded btag SFs" << std::endl;
    return;
}

float JetTree::getBtagEffFromFile(float pt, float eta, int mcFlavour, int WP, bool isFastsim){
    if(WP<0||WP>2){
      std::cout << "babyMaker::getBtagEffFromFile: ERROR: wrong WP" << std::endl;
      return 1.;
    }
    if(WP==BTagEntry::OP_MEDIUM && (!h_btag_eff_b || !h_btag_eff_c || !h_btag_eff_udsg)) {
      std::cout << "babyMaker::getBtagEffFromFile: ERROR: missing input hists for medium WP" << std::endl;
      return 1.;
    }
    if(WP==BTagEntry::OP_LOOSE && (!h_loose_btag_eff_b || !h_loose_btag_eff_c || !h_loose_btag_eff_udsg)) {
      std::cout << "babyMaker::getBtagEffFromFile: ERROR: missing input hists for loose WP" << std::endl;
      return 1.;
    }
    if(WP==BTagEntry::OP_TIGHT && (!h_tight_btag_eff_b || !h_tight_btag_eff_c || !h_tight_btag_eff_udsg)) {
      std::cout << "babyMaker::getBtagEffFromFile: ERROR: missing input hists for tight WP" << std::endl;
      return 1.;
    }
    //have fastsim already done in looper
//    if(isFastsim && (!h_btag_eff_b_fastsim || !h_btag_eff_c_fastsim || !h_btag_eff_udsg_fastsim)) {
//      std::cout << "babyMaker::getBtagEffFromFile: ERROR: missing input hists" << std::endl;
//      return 1.;
    //}
    // only use pt bins up to 400 GeV for charm and udsg
    float pt_cutoff = std::max(20.,std::min(399.,double(pt)));
    TH2D* h(0);
    if(WP==BTagEntry::OP_MEDIUM){
      if (abs(mcFlavour) == 5) {
	h = h_btag_eff_b;
	pt_cutoff = std::max(20.,std::min(599.,double(pt)));
      }
      else if (abs(mcFlavour) == 4) {
	h = h_btag_eff_c;
      }
      else {
	h = h_btag_eff_udsg;
      }
    }
    if(WP==BTagEntry::OP_LOOSE){
      if (abs(mcFlavour) == 5) {
	h = h_loose_btag_eff_b;
	pt_cutoff = std::max(20.,std::min(599.,double(pt)));
      }
      else if (abs(mcFlavour) == 4) {
	h = h_loose_btag_eff_c;
      }
      else {
	h = h_loose_btag_eff_udsg;
      }
    }
    if(WP==BTagEntry::OP_TIGHT){
      if (abs(mcFlavour) == 5) {
	h = h_tight_btag_eff_b;
	pt_cutoff = std::max(20.,std::min(599.,double(pt)));
      }
      else if (abs(mcFlavour) == 4) {
	h = h_tight_btag_eff_c;
      }
      else {
	h = h_tight_btag_eff_udsg;
      }
    }
    int binx = h->GetXaxis()->FindBin(pt_cutoff);
    int biny = h->GetYaxis()->FindBin(fabs(eta));
    return h->GetBinContent(binx,biny);
}

void JetTree::FillCommon(std::vector<unsigned int> alloverlapjets_idx, FactorizedJetCorrector* corrector, float& btagprob_data, float &btagprob_mc, float& btagprob_heavy_UP, float& btagprob_heavy_DN, float& btagprob_light_UP, float& btagprob_light_DN, float& btagprob_FS_UP, float& btagprob_FS_DN, float& loosebtagprob_data, float &loosebtagprob_mc, float &loosebtagprob_heavy_UP, float& loosebtagprob_heavy_DN, float& loosebtagprob_light_UP, float& loosebtagprob_light_DN, float& loosebtagprob_FS_UP, float& loosebtagprob_FS_DN, float& tightbtagprob_data, float& tightbtagprob_mc, float& tightbtagprob_heavy_UP, float& tightbtagprob_heavy_DN, float& tightbtagprob_light_UP, float& tightbtagprob_light_DN, float& tightbtagprob_FS_UP, float& tightbtagprob_FS_DN, unsigned int overlep1_idx, unsigned int overlep2_idx, bool applynewcorr, JetCorrectionUncertainty* jetcorr_uncertainty, int JES_type, bool applyBtagSFs, bool isFastsim)
{
    // fill info for ak4pfjets
    int nGoodJets=0.;
    int nFailJets=0.;
    unsigned int jindex=0;
    float HT = 0.;
    float JET_PT = 0.;
    int nbtags_med = 0;
    int nbtags_tight = 0;
    int nbtags_loose = 0;
    float dPhiM = 0.;
    float btagdisc = 0.;   
    unsigned int leadbtag_idx = 0;

    float htssm = 0.;
    float htosm = 0.;
    float htratiom = 0.;

    nskimjets = 0;
    nskimbtagmed = 0;
    nskimbtagtight = 0;
    nskimbtagloose = 0;

    // Figure out which convention is in use for DeepCSV discriminator names
    // and store it statically so we don't have to re-find it for every event
    static TString deepCSV_prefix = "NULL";
    if( deepCSV_prefix == "NULL" ) {
      for( TString discName : pfjets_bDiscriminatorNames() ) {
        if( discName.Contains("pfDeepCSV") ) { // 2017 convention
          deepCSV_prefix = "pfDeepCSV";
          break;
        }
        else if( discName.Contains("deepFlavour") ) { // 2016 convention
          deepCSV_prefix = "deepFlavour";
          break;
        }
      } // end loop over b discriminator names

      if( deepCSV_prefix == "NULL" ) {
        cout << "Error in JetTree.cc: Can't find DeepCSV discriminator names!" << endl;
        exit(1);
      }
    } // end if prefix == "NULL"

    //apply JEC
    vector<float> newjecorr;
    newjecorr.clear();
    vector<pair <int, LorentzVector> > sortedJets_pt;

    vector<LorentzVector> p4sCorrJets; // store corrected p4 for ALL jets, so indices match CMS3 ntuple
    vector<LorentzVector> p4sUCorrJets;
    p4sCorrJets.clear();
    p4sUCorrJets.clear();
    for(unsigned int iJet = 0; iJet < cms3.pfjets_p4().size(); iJet++){
        LorentzVector pfjet_p4_cor = cms3.pfjets_p4().at(iJet);
          // get uncorrected jet p4 to use as input for corrections
        LorentzVector pfjet_p4_uncor = pfjets_p4().at(iJet) * cms3.pfjets_undoJEC().at(iJet);

	double corr = 1;
 	if(applynewcorr){
          // get L1FastL2L3Residual total correction
          corrector->setRho   ( cms3.evt_fixgridfastjet_all_rho() );
          corrector->setJetA  ( cms3.pfjets_area().at(iJet)       );
          corrector->setJetPt ( pfjet_p4_uncor.pt()               );
          corrector->setJetEta( pfjet_p4_uncor.eta()              );
          corr = corrector->getCorrection();
	}

        // check for negative correction
        if (corr < 0. && fabs(pfjet_p4_uncor.eta()) < 4.7) {
          std::cout << "ScanChain::Looper: WARNING: negative jet correction: " << corr
                    << ", raw jet pt: " << pfjet_p4_uncor.pt() << ", eta: " << pfjet_p4_uncor.eta() << std::endl;
        }

        // include protections here on jet kinematics to prevent rare warnings/crashes
        double var = 1.;
        if (!evt_isRealData() && JES_type != 0 && pfjet_p4_uncor.pt()*corr > 0. && fabs(pfjet_p4_uncor.eta()) < 5.4) {
          jetcorr_uncertainty->setJetEta(pfjet_p4_uncor.eta());
          jetcorr_uncertainty->setJetPt(pfjet_p4_uncor.pt() * corr); // must use CORRECTED pt
          double unc = jetcorr_uncertainty->getUncertainty(true);
          var = (1. + JES_type * unc);
        }

        // apply new JEC to p4
        pfjet_p4_cor = pfjet_p4_uncor * corr*var;
        newjecorr.push_back(corr);
        if(applynewcorr) p4sCorrJets.push_back(pfjet_p4_cor);
        else p4sCorrJets.push_back(pfjets_p4().at(iJet));
        p4sUCorrJets.push_back(pfjet_p4_uncor);
    }
    sortedJets_pt =  sort_pt(p4sCorrJets,JET_PT);

    vector<TLorentzVector> ak4jets_TLV;

    for (size_t idx = 0; idx < pfjets_p4().size(); ++idx)
    {
        jindex = sortedJets_pt.at(idx).first;
        //deal with the overlaps
        const bool fillOverleps = false;
        if(fillOverleps && jindex == overlep1_idx){
		ak4pfjet_overlep1_p4  = p4sCorrJets.at(jindex);
                ak4pfjet_overlep1_CSV = pfjets_pfCombinedInclusiveSecondaryVertexV2BJetTag().at(jindex);
                ak4pfjet_overlep1_deepCSV = getbtagvalue(deepCSV_prefix+"JetTags:probb",jindex) + getbtagvalue(deepCSV_prefix+"JetTags:probbb",jindex);
		//ak4pfjet_overlep1_pu_id = pfjets_pileupJetId().at(jindex);
                ak4pfjet_overlep1_chf = pfjets_chargedHadronE().at(jindex)/ (pfjets_undoJEC().at(jindex)*p4sCorrJets[jindex].energy());
                ak4pfjet_overlep1_nhf = pfjets_neutralHadronE().at(jindex)/ (pfjets_undoJEC().at(jindex)*p4sCorrJets[jindex].energy());
                ak4pfjet_overlep1_cef = pfjets_chargedEmE().at(jindex)/ (pfjets_undoJEC().at(jindex)*p4sCorrJets[jindex].energy());
                ak4pfjet_overlep1_nef = pfjets_neutralEmE().at(jindex)/ (pfjets_undoJEC().at(jindex)*p4sCorrJets[jindex].energy());
                ak4pfjet_overlep1_muf = pfjets_muonE().at(jindex)/ (pfjets_undoJEC().at(jindex)*p4sCorrJets[jindex].energy());
                ak4pfjet_overlep1_cm  = pfjets_chargedMultiplicity().at(jindex);
                ak4pfjet_overlep1_nm  = cms3.pfjets_neutralMultiplicity().at(jindex);
	}
        else if(fillOverleps && jindex == overlep2_idx){
                ak4pfjet_overlep2_p4  = p4sCorrJets.at(jindex);
                ak4pfjet_overlep2_CSV = pfjets_pfCombinedInclusiveSecondaryVertexV2BJetTag().at(jindex);
                ak4pfjet_overlep2_deepCSV = getbtagvalue(deepCSV_prefix+"JetTags:probb",jindex) + getbtagvalue(deepCSV_prefix+"JetTags:probbb",jindex);
                //ak4pfjet_overlep2_pu_id = pfjets_pileupJetId().at(jindex);
                ak4pfjet_overlep2_chf = pfjets_chargedHadronE().at(jindex)/ (pfjets_undoJEC().at(jindex)*p4sCorrJets[jindex].energy());
                ak4pfjet_overlep2_nhf = pfjets_neutralHadronE().at(jindex)/ (pfjets_undoJEC().at(jindex)*p4sCorrJets[jindex].energy());
                ak4pfjet_overlep2_cef = pfjets_chargedEmE().at(jindex)/ (pfjets_undoJEC().at(jindex)*p4sCorrJets[jindex].energy());
                ak4pfjet_overlep2_nef = pfjets_neutralEmE().at(jindex)/ (pfjets_undoJEC().at(jindex)*p4sCorrJets[jindex].energy());
                ak4pfjet_overlep2_muf = pfjets_muonE().at(jindex)/ (pfjets_undoJEC().at(jindex)*p4sCorrJets[jindex].energy());
                ak4pfjet_overlep2_cm  = pfjets_chargedMultiplicity().at(jindex);
                ak4pfjet_overlep2_nm  = cms3.pfjets_neutralMultiplicity().at(jindex);
        }

        //remove overlaps & apply preselections
        if(jindex == overlep1_idx || jindex == overlep2_idx) continue; //remove ovelaps from your jet collection

	bool skipthis = false;
	for(size_t jdx = 0; jdx < alloverlapjets_idx.size(); ++jdx){
	  if(alloverlapjets_idx.at(jdx)==jindex) {
	    skipthis = true;
	    break;
	  }
	}
	if(skipthis) continue; //remove all overlaps from jet collection

        //Jet selections
        if(p4sCorrJets.at(jindex).pt() < m_ak4_pt_cut) continue;
        if(fabs(p4sCorrJets.at(jindex).eta()) > m_ak4_eta_cut) continue;
        bool passJetID = (gconf.year < 2017)? ((gconf.cmssw_ver < 94)? isLoosePFJetV2(jindex) : isTightPFJetV2(jindex)) : isTightPFJet_2017_v1(jindex);
        if(!passJetID) ++nFailJets;
        if(!isFastsim && m_ak4_passid && !passJetID) continue;
        nskimjets++;
        bool is_jetpt30 = (p4sCorrJets.at(jindex).pt() >= 30);
        if (is_jetpt30) nGoodJets++;
        // bool is_jetpt20 = (p4sCorrJets.at(jindex).pt() >= 20);

        ak4pfjets_p4.push_back(p4sCorrJets.at(jindex));
        ak4pfjets_pt.push_back(p4sCorrJets.at(jindex).pt());
        ak4pfjets_eta.push_back(p4sCorrJets.at(jindex).eta());
        ak4pfjets_phi.push_back(p4sCorrJets.at(jindex).phi());
        ak4pfjets_mass.push_back(p4sCorrJets.at(jindex).mass());

        ak4jets_TLV.emplace_back(ak4pfjets_p4.back().Px(), ak4pfjets_p4.back().Py(), ak4pfjets_p4.back().Pz(), ak4pfjets_p4.back().E());

        dphi_ak4pfjet_met.push_back(getdphi(p4sCorrJets.at(jindex).phi(), evt_pfmetPhi()));//this can be false - due to correction to pfmet, but it gets corrected later

        // various b-tagging values
        ak4pfjets_deepCSVb.push_back(getbtagvalue(deepCSV_prefix+"JetTags:probb", jindex));
        ak4pfjets_deepCSVbb.push_back(getbtagvalue(deepCSV_prefix+"JetTags:probbb", jindex));
        ak4pfjets_deepCSVc.push_back(getbtagvalue(deepCSV_prefix+"JetTags:probc", jindex));
        ak4pfjets_deepCSVl.push_back(getbtagvalue(deepCSV_prefix+"JetTags:probudsg", jindex));

        float value_deepCSV = ak4pfjets_deepCSVb.back() + ak4pfjets_deepCSVbb.back(); // save for later
        ak4pfjets_deepCSV.push_back(value_deepCSV);
        ak4pfjets_CSV.push_back(pfjets_pfCombinedInclusiveSecondaryVertexV2BJetTag().at(jindex));
        ak4pfjets_mva.push_back(getbtagvalue("pfCombinedMVAV2BJetTags", jindex));
        ak4pfjets_cvsl.push_back(getbtagvalue("pfCombinedCvsLJetTags", jindex));

        // jet ids
        ak4pfjets_puid.push_back(loosePileupJetId(jindex));
        ak4pfjets_parton_flavor.push_back(pfjets_partonFlavour().at(jindex));
	ak4pfjets_hadron_flavor.push_back(pfjets_hadronFlavour().at(jindex));
        ak4pfjets_loose_puid.push_back(loosePileupJetId(jindex));
        ak4pfjets_loose_pfid.push_back(isLoosePFJetV2(jindex));
        //ak4pfjets_medium_pfid.push_back(isMediumPFJetV2(jindex));
        ak4pfjets_tight_pfid.push_back(isTightPFJetV2(jindex));

        // QG likelihood variables
        ak4pfjets_ptD.push_back(pfjets_ptDistribution().at(jindex));
        ak4pfjets_axis1.push_back(pfjets_axis1().at(jindex));
        ak4pfjets_axis2.push_back(pfjets_axis2().at(jindex));
        ak4pfjets_mult.push_back(pfjets_totalMultiplicity().at(jindex));

        double jetenergy_inv = 1. / (pfjets_undoJEC().at(jindex) * pfjets_p4()[jindex].energy());
        ak4pfjets_chf.push_back(pfjets_chargedHadronE().at(jindex) * jetenergy_inv);
        ak4pfjets_nhf.push_back(pfjets_neutralHadronE().at(jindex) * jetenergy_inv);
        ak4pfjets_cef.push_back(pfjets_chargedEmE().at(jindex)     * jetenergy_inv);
        ak4pfjets_nef.push_back(pfjets_neutralEmE().at(jindex)     * jetenergy_inv);
	ak4pfjets_muf.push_back(pfjets_muonE().at(jindex)          * jetenergy_inv);
        ak4pfjets_elf.push_back(pfjets_electronE().at(jindex)      * jetenergy_inv);
        ak4pfjets_phf.push_back(pfjets_photonE().at(jindex)        * jetenergy_inv);
        ak4pfjets_hhf.push_back(pfjets_hfHadronE().at(jindex)      * jetenergy_inv);
        ak4pfjets_hef.push_back(pfjets_hfEmE().at(jindex)          * jetenergy_inv);

        ak4pfjets_cm.push_back(pfjets_chargedMultiplicity().at(jindex));
        ak4pfjets_nm.push_back(pfjets_neutralMultiplicity().at(jindex));
        ak4pfjets_chm.push_back(pfjets_chargedHadronMultiplicity().at(jindex));
        ak4pfjets_nhm.push_back(pfjets_neutralHadronMultiplicity().at(jindex));
        ak4pfjets_em.push_back(pfjets_electronMultiplicity().at(jindex));
        ak4pfjets_mm.push_back(pfjets_muonMultiplicity().at(jindex));
        ak4pfjets_pm.push_back(pfjets_photonMultiplicity().at(jindex));
	
	if (!evt_isRealData() && pfjets_mc3dr().size()>0) {
          ak4pfjets_mc3dr.push_back(pfjets_mc3dr().at(jindex));
          ak4pfjets_mc3id.push_back(pfjets_mc3_id().at(jindex));
          ak4pfjets_mc3idx.push_back(pfjets_mc3idx().at(jindex));
          ak4pfjets_mcmotherid.push_back(pfjets_mc_motherid().at(jindex));
	}

	//HTRatio
	dPhiM = getdphi(evt_pfmetPhi(), p4sCorrJets.at(jindex).phi() );
	if ( dPhiM  < (TMath::Pi()/2) ) htssm = htssm + p4sCorrJets.at(jindex).pt();
        else htosm = htosm + p4sCorrJets.at(jindex).pt();	

        HT = HT + p4sCorrJets.at(jindex).pt();

	float eff(1.), effloose(1.), efftight(1.);
	BTagEntry::JetFlavor flavor = BTagEntry::FLAV_UDSG;
	if (abs(pfjets_hadronFlavour().at(jindex)) == 5) flavor = BTagEntry::FLAV_B;
	else if (abs(pfjets_hadronFlavour().at(jindex)) == 4) flavor = BTagEntry::FLAV_C;
	float weight_cent(1.), weight_UP(1.), weight_DN(1.), weight_FS_UP(1.), weight_FS_DN(1.);
	float weight_loose_cent(1.), weight_loose_UP(1.), weight_loose_DN(1.), weight_loose_FS_UP(1.), weight_loose_FS_DN(1.);
	float weight_tight_cent(1.), weight_tight_UP(1.), weight_tight_DN(1.), weight_tight_FS_UP(1.), weight_tight_FS_DN(1.);
	if(applyBtagSFs && is_jetpt30){
	  //put all b-tag issues outside what is possible
	  effloose = getBtagEffFromFile(p4sCorrJets[jindex].pt(),p4sCorrJets[jindex].eta(), pfjets_hadronFlavour().at(jindex), 0, isFastsim);
	  eff      = getBtagEffFromFile(p4sCorrJets[jindex].pt(),p4sCorrJets[jindex].eta(), pfjets_hadronFlavour().at(jindex), 1, isFastsim);
	  efftight = getBtagEffFromFile(p4sCorrJets[jindex].pt(),p4sCorrJets[jindex].eta(), pfjets_hadronFlavour().at(jindex), 2, isFastsim);
          if (eff == 0 || efftight == 0)
            cerr << "JetTree.cc: Error: 0 btag eff from file found! :" << eff << " " << effloose << " " << efftight << ", pt = " << p4sCorrJets[jindex].pt() << endl;
	  // cout<<"read uncertainty from btagsf reader:"<<endl;
          weight_cent       = reader_medium ->eval_auto_bounds("central", flavor, p4sCorrJets[jindex].eta(), p4sCorrJets[jindex].pt());
          weight_UP         = reader_medium ->eval_auto_bounds("up",      flavor, p4sCorrJets[jindex].eta(), p4sCorrJets[jindex].pt());
          weight_DN         = reader_medium ->eval_auto_bounds("down",    flavor, p4sCorrJets[jindex].eta(), p4sCorrJets[jindex].pt());
          weight_loose_cent = reader_loose  ->eval_auto_bounds("central", flavor, p4sCorrJets[jindex].eta(), p4sCorrJets[jindex].pt());
          weight_loose_UP   = reader_loose  ->eval_auto_bounds("up",      flavor, p4sCorrJets[jindex].eta(), p4sCorrJets[jindex].pt());
          weight_loose_DN   = reader_loose  ->eval_auto_bounds("down",    flavor, p4sCorrJets[jindex].eta(), p4sCorrJets[jindex].pt());
          weight_tight_cent = reader_tight  ->eval_auto_bounds("central", flavor, p4sCorrJets[jindex].eta(), p4sCorrJets[jindex].pt());
          weight_tight_UP   = reader_tight  ->eval_auto_bounds("up",      flavor, p4sCorrJets[jindex].eta(), p4sCorrJets[jindex].pt());
          weight_tight_DN   = reader_tight  ->eval_auto_bounds("down",    flavor, p4sCorrJets[jindex].eta(), p4sCorrJets[jindex].pt());

	  if (isFastsim) {
            weight_FS_UP       = reader_medium_FS ->eval_auto_bounds("up",      flavor, p4sCorrJets[jindex].eta(), p4sCorrJets[jindex].pt()) * weight_cent;
            weight_FS_DN       = reader_medium_FS ->eval_auto_bounds("down",    flavor, p4sCorrJets[jindex].eta(), p4sCorrJets[jindex].pt()) * weight_cent;
            weight_cent       *= reader_medium_FS ->eval_auto_bounds("central", flavor, p4sCorrJets[jindex].eta(), p4sCorrJets[jindex].pt());
            weight_UP         *= reader_medium_FS ->eval_auto_bounds("central", flavor, p4sCorrJets[jindex].eta(), p4sCorrJets[jindex].pt());//this is still just btagSF
            weight_DN         *= reader_medium_FS ->eval_auto_bounds("central", flavor, p4sCorrJets[jindex].eta(), p4sCorrJets[jindex].pt());//this is still just btagSF
            weight_loose_FS_UP = reader_loose_FS  ->eval_auto_bounds("up",      flavor, p4sCorrJets[jindex].eta(), p4sCorrJets[jindex].pt()) * weight_loose_cent;
            weight_loose_FS_DN = reader_loose_FS  ->eval_auto_bounds("down",    flavor, p4sCorrJets[jindex].eta(), p4sCorrJets[jindex].pt()) * weight_loose_cent;
            weight_loose_cent *= reader_loose_FS  ->eval_auto_bounds("central", flavor, p4sCorrJets[jindex].eta(), p4sCorrJets[jindex].pt());
            weight_loose_UP   *= reader_loose_FS  ->eval_auto_bounds("central", flavor, p4sCorrJets[jindex].eta(), p4sCorrJets[jindex].pt());//this is still just btagSF
            weight_loose_DN   *= reader_loose_FS  ->eval_auto_bounds("central", flavor, p4sCorrJets[jindex].eta(), p4sCorrJets[jindex].pt());//this is still just btagSF
            weight_tight_FS_UP = reader_tight_FS  ->eval_auto_bounds("up",      flavor, p4sCorrJets[jindex].eta(), p4sCorrJets[jindex].pt()) * weight_tight_cent;
            weight_tight_FS_DN = reader_tight_FS  ->eval_auto_bounds("down",    flavor, p4sCorrJets[jindex].eta(), p4sCorrJets[jindex].pt()) * weight_tight_cent;
            weight_tight_cent *= reader_tight_FS  ->eval_auto_bounds("central", flavor, p4sCorrJets[jindex].eta(), p4sCorrJets[jindex].pt());
            weight_tight_UP   *= reader_tight_FS  ->eval_auto_bounds("central", flavor, p4sCorrJets[jindex].eta(), p4sCorrJets[jindex].pt());//this is still just btagSF
            weight_tight_DN   *= reader_tight_FS  ->eval_auto_bounds("central", flavor, p4sCorrJets[jindex].eta(), p4sCorrJets[jindex].pt());//this is still just btagSF
	  }
	}

	//medium btag
	if (value_deepCSV > gconf.WP_DEEPCSV_MEDIUM) {
             ak4pfjets_passMEDbtag.push_back(true);
             nskimbtagmed++;
             if (is_jetpt30) nbtags_med++;
             if(nbtags_med == 1){
                ak4pfjets_leadMEDbjet_pt = p4sCorrJets.at(jindex).pt();  //plot leading bjet pT
                ak4pfjets_leadMEDbjet_p4 = p4sCorrJets.at(jindex);
             }
                ak4pfjets_MEDbjet_pt.push_back(p4sCorrJets.at(jindex).pt());
               //bool isFastsim = false; 
              // btag SF - not final yet
              if (!evt_isRealData() && applyBtagSFs && is_jetpt30) {
  //              cout<<"got uncertainty from btagsf reader:"<<endl;
                btagprob_data *= weight_cent * eff;
                btagprob_mc *= eff;
		if (flavor == BTagEntry::FLAV_UDSG) {
		  btagprob_light_UP *= weight_UP * eff;
		  btagprob_light_DN *= weight_DN * eff;
		  btagprob_heavy_UP *= weight_cent * eff;
		  btagprob_heavy_DN *= weight_cent * eff;
		} else {
		  btagprob_light_UP *= weight_cent * eff;
		  btagprob_light_DN *= weight_cent * eff;
		  btagprob_heavy_UP *= weight_UP * eff;
		  btagprob_heavy_DN *= weight_DN * eff;
                }
		if(isFastsim){
		  btagprob_FS_UP *= weight_FS_UP * eff;
		  btagprob_FS_DN *= weight_FS_DN * eff;
		}
//                cout<<"btagprob_err_heavy_UP"<<btagprob_err_heavy_UP<<endl;
               }
            }else{ 
             ak4pfjets_passMEDbtag.push_back(false);
             if (!evt_isRealData() && applyBtagSFs && is_jetpt30) { // fail med btag -- needed for SF event weights
              btagprob_data *= (1. - weight_cent * eff);
              btagprob_mc *= (1. - eff);
	      if (flavor == BTagEntry::FLAV_UDSG) {
		btagprob_light_UP *= (1. - weight_UP * eff);
		btagprob_light_DN *= (1. - weight_DN * eff);
		btagprob_heavy_UP *= (1. - weight_cent * eff);
		btagprob_heavy_DN *= (1. - weight_cent * eff);
              } else {
		btagprob_light_UP *= (1. - weight_cent * eff);
		btagprob_light_DN *= (1. - weight_cent * eff);
		btagprob_heavy_UP *= (1. - weight_UP * eff);
		btagprob_heavy_DN *= (1. - weight_DN * eff);
              }
	      if(isFastsim){
		btagprob_FS_UP *= (1. - weight_FS_UP * eff);
		btagprob_FS_DN *= (1. - weight_FS_DN * eff);
	      }
           }
        }//finish medium
        if (value_deepCSV > btagdisc) {
          btagdisc = value_deepCSV;
	  leadbtag_idx = jindex;
	}
	//loose btag
	if (value_deepCSV > gconf.WP_DEEPCSV_LOOSE) {
              nskimbtagloose++;
              if (is_jetpt30) nbtags_loose++;
              if (!evt_isRealData() && applyBtagSFs && is_jetpt30) {
                loosebtagprob_data *= weight_loose_cent * effloose;
                loosebtagprob_mc *= effloose;
		if (flavor == BTagEntry::FLAV_UDSG) {
		  loosebtagprob_light_UP *= weight_loose_UP * effloose;
		  loosebtagprob_light_DN *= weight_loose_DN * effloose;
		  loosebtagprob_heavy_UP *= weight_loose_cent * effloose;
		  loosebtagprob_heavy_DN *= weight_loose_cent * effloose;
		} else {
		  loosebtagprob_light_UP *= weight_loose_cent * effloose;
		  loosebtagprob_light_DN *= weight_loose_cent * effloose;
		  loosebtagprob_heavy_UP *= weight_loose_UP * effloose;
		  loosebtagprob_heavy_DN *= weight_loose_DN * effloose;
                }
		if(isFastsim){
		  loosebtagprob_FS_UP *= weight_loose_FS_UP * effloose;
		  loosebtagprob_FS_DN *= weight_loose_FS_DN * effloose;
		}
               }
            }else{ 
             if (!evt_isRealData() && applyBtagSFs && is_jetpt30) { // fail loose btag -- needed for SF event weights
              loosebtagprob_data *= (1. - weight_loose_cent * effloose);
              loosebtagprob_mc *= (1. - effloose);
	      if (flavor == BTagEntry::FLAV_UDSG) {
		loosebtagprob_light_UP *= (1. - weight_loose_UP * effloose);
		loosebtagprob_light_DN *= (1. - weight_loose_DN * effloose);
		loosebtagprob_heavy_UP *= (1. - weight_loose_cent * effloose);
		loosebtagprob_heavy_DN *= (1. - weight_loose_cent * effloose);
              } else {
		loosebtagprob_light_UP *= (1. - weight_loose_cent * effloose);
		loosebtagprob_light_DN *= (1. - weight_loose_cent * effloose);
		loosebtagprob_heavy_UP *= (1. - weight_loose_UP * effloose);
		loosebtagprob_heavy_DN *= (1. - weight_loose_DN * effloose);
              }
	      if(isFastsim){
		loosebtagprob_FS_UP *= (1. - weight_loose_FS_UP * effloose);
		loosebtagprob_FS_DN *= (1. - weight_loose_FS_DN * effloose);
	      }
           }
        }//finish loose
	//tight btag
	if (value_deepCSV > gconf.WP_DEEPCSV_TIGHT) {
             nskimbtagtight++;
             if (is_jetpt30) nbtags_tight++;
              if (!evt_isRealData() && applyBtagSFs && is_jetpt30) {
                tightbtagprob_data *= weight_tight_cent * efftight;
                tightbtagprob_mc *= efftight;
		if (flavor == BTagEntry::FLAV_UDSG) {
		  tightbtagprob_light_UP *= weight_tight_UP * efftight;
		  tightbtagprob_light_DN *= weight_tight_DN * efftight;
		  tightbtagprob_heavy_UP *= weight_tight_cent * efftight;
		  tightbtagprob_heavy_DN *= weight_tight_cent * efftight;
		} else {
		  tightbtagprob_light_UP *= weight_tight_cent * efftight;
		  tightbtagprob_light_DN *= weight_tight_cent * efftight;
		  tightbtagprob_heavy_UP *= weight_tight_UP * efftight;
		  tightbtagprob_heavy_DN *= weight_tight_DN * efftight;
                }
		if(isFastsim){
		  tightbtagprob_FS_UP *= weight_tight_FS_UP * efftight;
		  tightbtagprob_FS_DN *= weight_tight_FS_DN * efftight;
		}
               }
        } else { 
          if (!evt_isRealData() && applyBtagSFs && is_jetpt30) { // fail tight btag -- needed for SF event weights
              tightbtagprob_data *= (1. - weight_tight_cent * efftight);
              tightbtagprob_mc *= (1. - efftight);
	      if (flavor == BTagEntry::FLAV_UDSG) {
		tightbtagprob_light_UP *= (1. - weight_tight_UP * efftight);
		tightbtagprob_light_DN *= (1. - weight_tight_DN * efftight);
		tightbtagprob_heavy_UP *= (1. - weight_tight_cent * efftight);
		tightbtagprob_heavy_DN *= (1. - weight_tight_cent * efftight);
              } else {
		tightbtagprob_light_UP *= (1. - weight_tight_cent * efftight);
		tightbtagprob_light_DN *= (1. - weight_tight_cent * efftight);
		tightbtagprob_heavy_UP *= (1. - weight_tight_UP * efftight);
		tightbtagprob_heavy_DN *= (1. - weight_tight_DN * efftight);
              }
	      if(isFastsim){
		tightbtagprob_FS_UP *= (1. - weight_tight_FS_UP * efftight);
		tightbtagprob_FS_DN *= (1. - weight_tight_FS_DN * efftight);
	      }
           }
        }//finish tight
    }

    ak4pfjets_leadbtag_p4 = p4sCorrJets.at(leadbtag_idx);//highest discriminator jet

    ngoodjets = nGoodJets;
    nfailjets = nFailJets;
    ak4_HT = HT;
    HT=0;
    ngoodbtags = nbtags_med;
    nloosebtags = nbtags_loose;
    ntightbtags = nbtags_tight;

    ak4_htssm = htssm;
    ak4_htosm = htosm;
    htratiom   = htssm / (htosm + htssm);
    ak4_htratiom = htratiom; 

    nGoodJets = 0;

    // fill info for genjets
    if (!evt_isRealData()){
      for (size_t idx = 0; idx < genjets_p4NoMuNoNu().size(); ++idx){
        if (genjets_p4NoMuNoNu().at(idx).pt() < m_ak4_pt_cut) continue;
 
        ak4genjets_p4.push_back(genjets_p4NoMuNoNu().at(idx));
        if(genjets_p4NoMuNoNu().at(idx).pt() > 20.)  nGoodJets++;
      }
    }
    nGoodGenJets = nGoodJets;

    nGoodJets = 0;
    if (doResolveTopMVA) {
      vector<float> ak4pfjets_dcvsl;
      for (size_t j = 0; j < ak4pfjets_deepCSV.size(); ++j) {
        ak4pfjets_dcvsl.push_back(ak4pfjets_deepCSVc.at(j) / (ak4pfjets_deepCSVc.at(j) + ak4pfjets_deepCSVl.at(j)));
      }
      resTopMVA->setJetVecPtrs(&ak4pfjets_p4, &ak4pfjets_deepCSV, &ak4pfjets_dcvsl, &ak4pfjets_ptD, &ak4pfjets_axis1, &ak4pfjets_mult);
      std::vector<TopCand> resMVATopCands = resTopMVA->getTopCandidates(-1);
      for (auto tcand : resMVATopCands) {
        topcands_ak4idx.emplace_back( std::vector<int>{tcand.getIdxForb(), tcand.getIdxForj2(), tcand.getIdxForj3()} );
        topcands_disc.push_back( tcand.disc );
        topcands_p4.push_back( tcand.topcand );
        topcands_Wp4.push_back( tcand.wcand );
      }
    }

    const bool doResolveTopDNN = doResolveTopMVA;
    if (doResolveTopDNN) {
      ttUtility::ConstAK4Inputs<float> AK4Inputs(ak4jets_TLV, ak4pfjets_CSV);
      AK4Inputs.addSupplamentalVector("qgPtD",                                ak4pfjets_ptD);
      AK4Inputs.addSupplamentalVector("qgAxis1",                              ak4pfjets_axis1);
      AK4Inputs.addSupplamentalVector("qgAxis2",                              ak4pfjets_axis2);
      auto ak4jets_mult = vector<float>(ak4pfjets_mult.begin(), ak4pfjets_mult.end()); // because everything has to be vector<float>
      AK4Inputs.addSupplamentalVector("qgMult",                               ak4jets_mult);
      AK4Inputs.addSupplamentalVector("recoJetschargedHadronEnergyFraction",  ak4pfjets_chf);
      AK4Inputs.addSupplamentalVector("recoJetschargedEmEnergyFraction",      ak4pfjets_cef);
      AK4Inputs.addSupplamentalVector("recoJetsneutralEmEnergyFraction",      ak4pfjets_nef);
      AK4Inputs.addSupplamentalVector("recoJetsmuonEnergyFraction",           ak4pfjets_muf);
      AK4Inputs.addSupplamentalVector("recoJetsHFHadronEnergyFraction",       ak4pfjets_hhf);
      AK4Inputs.addSupplamentalVector("recoJetsHFEMEnergyFraction",           ak4pfjets_hef);
      AK4Inputs.addSupplamentalVector("recoJetsneutralEnergyFraction",        ak4pfjets_nhf);
      AK4Inputs.addSupplamentalVector("PhotonEnergyFraction",                 ak4pfjets_phf);
      AK4Inputs.addSupplamentalVector("ElectronEnergyFraction",               ak4pfjets_elf);
      AK4Inputs.addSupplamentalVector("ChargedHadronMultiplicity",            ak4pfjets_chm);
      AK4Inputs.addSupplamentalVector("NeutralHadronMultiplicity",            ak4pfjets_nhm);
      AK4Inputs.addSupplamentalVector("PhotonMultiplicity",                   ak4pfjets_pm);
      AK4Inputs.addSupplamentalVector("ElectronMultiplicity",                 ak4pfjets_em);
      AK4Inputs.addSupplamentalVector("MuonMultiplicity",                     ak4pfjets_mm);
      AK4Inputs.addSupplamentalVector("DeepCSVb",                             ak4pfjets_deepCSVb);
      AK4Inputs.addSupplamentalVector("DeepCSVc",                             ak4pfjets_deepCSVc);
      AK4Inputs.addSupplamentalVector("DeepCSVl",                             ak4pfjets_deepCSVl);
      AK4Inputs.addSupplamentalVector("DeepCSVbb",                            ak4pfjets_deepCSVbb);
      auto dummy_deepCSVcc = vector<float>(ak4jets_TLV.size(), 0); // Temporary dealing with deepCSVcc not present in 94X
      AK4Inputs.addSupplamentalVector("DeepCSVcc",                            dummy_deepCSVcc);

      std::vector<Constituent> constituents = ttUtility::packageConstituents(AK4Inputs);

      tftagger->runTagger(constituents);

      //retrieve the top tagger results object
      const TopTaggerResults& ttr = tftagger->getResults();

      //get reconstructed top
      const std::vector<TopObject*>& tftops = ttr.getTops();

      for (const TopObject* top : tftops) {
        tftops_p4.emplace_back(top->p().Px(), top->p().Py(), top->p().Pz(), top->p().E());
        tftops_disc.push_back(top->getDiscriminator());
        if (top->getNConstituents() != 3) cout << "[JetTree] Getting TF top that has " << top->getNConstituents() << " constituents!!\n";
        vector<float> subjet_pt, subjet_eta, subjet_phi;
        for (const Constituent* subjet : top->getConstituents()) {
          subjet_pt.push_back(subjet->p().Pt());
          subjet_eta.push_back(subjet->p().Eta());
          subjet_phi.push_back(subjet->p().Phi());
        }
        tftops_subjet_pt.push_back(subjet_pt);
        tftops_subjet_eta.push_back(subjet_eta);
        tftops_subjet_phi.push_back(subjet_phi);
      }
    }

    // Fill info for soft b-tags
    for (size_t i=0; i<svs_p4().size(); i++) {

      if (svs_p4().at(i).Pt() >= 20.0) continue;
      if (svs_nTrks().at(i) < 3) continue;
      if (svs_distXYval().at(i) >= 3.0) continue;
      if (svs_dist3Dsig().at(i) <= 4.0) continue;
      if (cos(svs_anglePV().at(i)) <= 0.98) continue;

      bool failDR = false;
      for (size_t j=0; j<ak4pfjets_p4.size(); j++) {
        if (ROOT::Math::VectorUtil::DeltaR( svs_p4().at(i), ak4pfjets_p4[j] ) <= 0.4) {
          failDR = true;
          break;
        }
      } // end loop over ak4 jets

      if (failDR) continue;

      softtags_p4.push_back( svs_p4().at(i) );

    } // end loop over SVs

    nsoftbtags = softtags_p4.size();
}
        
// fill info for ak8pfjets
void JetTree::FillAK8Jets(bool applynewcorr, FactorizedJetCorrector* corrector, JetCorrectionUncertainty* jetcorr_uncertainty, int JES_type)
{
  int nGoodJets = 0;

  for (size_t idx = 0; idx < ak8jets_p4().size(); ++idx) {
    if (ak8jets_p4()[idx].pt() < m_ak8_pt_cut) continue;
    if (fabs(ak8jets_p4()[idx].eta()) > m_ak8_eta_cut) continue;
    if (!isFastsim && m_ak8_passid && !isLoosePFJetV2(idx)) continue;
    nGoodJets++;

    float corr = 1;
    if(applynewcorr){
      LorentzVector ak8jet_p4_uncor = ak8jets_p4().at(idx) * ak8jets_undoJEC().at(idx);

      // get L1FastL2L3Residual total correction
      corrector->setRho   ( evt_fixgridfastjet_all_rho() );
      corrector->setJetA  ( ak8jets_area().at(idx)       );
      corrector->setJetPt ( ak8jet_p4_uncor.pt()       );
      corrector->setJetEta( ak8jet_p4_uncor.eta()      );
      corr = corrector->getCorrection();

      // check for negative correction
      if (corr < 0. && fabs(ak8jets_p4()[idx].eta()) < 4.7) {
        std::cout << "ScanChain::Looper: WARNING: negative ak8jet correction: " << corr
                  << ", raw jet pt: " << ak8jets_p4()[idx].pt() << ", eta: " << ak8jets_p4()[idx].eta() << std::endl;
      }

      // include protections here on jet kinematics to prevent rare warnings/crashes
      double var = 1.;
      if (!evt_isRealData() && JES_type != 0 && ak8jets_p4()[idx].pt()*corr > 0. && fabs(ak8jets_p4()[idx].eta()) < 5.4) {
        jetcorr_uncertainty->setJetPt (ak8jets_p4()[idx].pt() * corr); // must use CORRECTED pt
        jetcorr_uncertainty->setJetEta(ak8jets_p4()[idx].eta());
        double unc = jetcorr_uncertainty->getUncertainty(true);
        var = (1. + JES_type * unc);
      }

      // apply new JEC to p4
      ak8pfjets_p4.push_back(ak8jet_p4_uncor * corr * var);
    } else {
      ak8pfjets_p4.push_back(ak8jets_p4().at(idx));
    }

    ak8pfjets_tau1.push_back(ak8jets_nJettinessTau1().at(idx));
    ak8pfjets_tau2.push_back(ak8jets_nJettinessTau2().at(idx));
    ak8pfjets_tau3.push_back(ak8jets_nJettinessTau3().at(idx));
    ak8pfjets_parton_flavor.push_back(ak8jets_partonFlavour().at(idx));

    // Branches from deepAK8 object tagging
    ak8pfjets_deepdisc_qcd.push_back(ak8jets_deep_rawdisc_qcd().at(idx));
    ak8pfjets_deepdisc_top.push_back(ak8jets_deep_rawdisc_top().at(idx));
    ak8pfjets_deepdisc_w.push_back(ak8jets_deep_rawdisc_w().at(idx));
    ak8pfjets_deepdisc_z.push_back(ak8jets_deep_rawdisc_z().at(idx));
    ak8pfjets_deepdisc_zbb.push_back(ak8jets_deep_rawdisc_zbb().at(idx));
    ak8pfjets_deepdisc_hbb.push_back(ak8jets_deep_rawdisc_hbb().at(idx));
    ak8pfjets_deepdisc_h4q.push_back(ak8jets_deep_rawdisc_h4q().at(idx));

    // Branches no longer in cms4
    // ak8pfjets_top_mass.push_back(ak8jets_topJetMass().at(idx));
    // ak8pfjets_trimmed_mass.push_back(ak8jets_trimmedMass().at(idx));
    // ak8pfjets_filtered_mass.push_back(ak8jets_filteredMass().at(idx));
    // ak8pfjets_pruned_mass.push_back(ak8jets_prunedMass().at(idx));
    // ak8pfjets_softdrop_mass.push_back(ak8jets_softdropMass().at(idx));

    // ak8pfjets_pu_id.push_back(ak8jets_pileupJetId().at(idx));
  }

  nGoodAK8PFJets = nGoodJets;
  for (auto disc : ak8pfjets_deepdisc_top) {
    if (disc > lead_ak8deepdisc_top) lead_ak8deepdisc_top = disc;
  }

}

void JetTree::SetJetSelection (std::string cone_size, float pt_cut,float eta, bool id)
{
  if (cone_size == "ak4") { m_ak4_pt_cut = pt_cut; m_ak4_eta_cut = eta; m_ak4_passid = id; }
  else if (cone_size == "ak8") { m_ak8_pt_cut = pt_cut; m_ak8_eta_cut = eta; m_ak8_passid = id; }
  else {std::cout << "Invalid cone size." << std::endl;}

  return;
}
 
void JetTree::GetJetSelections (std::string cone_size)
{
    if (cone_size == "ak4") std::cout << "ak4 jet pt > " << m_ak4_pt_cut << std::endl;
    else if (cone_size == "ak8") std::cout << "ak8 jet pt > " << m_ak8_pt_cut << std::endl;
    else
    {
        std::cout << "ak4 jet pt > " << m_ak4_pt_cut << std::endl;        
        std::cout << "ak8 jet pt > " << m_ak8_pt_cut << std::endl;
    }
}
void JetTree::deleteBtagSFTool()
{
   
    delete calib;
    delete calib_fastsim;
    delete reader_medium;
    delete reader_tight;
    delete reader_loose;
    delete reader_medium_FS;
    delete reader_tight_FS;
    delete reader_loose_FS;
    
    delete feff;
    return;
}

void JetTree::Reset ()
{
    ak4pfjets_p4.clear();
    ak4pfjets_pt.clear();
    ak4pfjets_eta.clear();
    ak4pfjets_phi.clear();
    ak4pfjets_mass.clear();

    dphi_ak4pfjet_met.clear();
    ak4pfjets_qg_disc.clear();    
    ak4pfjets_CSV.clear();
    ak4pfjets_deepCSV.clear();
    ak4pfjets_mva.clear();
    ak4pfjets_puid.clear();
    ak4pfjets_parton_flavor.clear();
    ak4pfjets_hadron_flavor.clear();
    ak4pfjets_loose_puid.clear();
    ak4pfjets_loose_pfid.clear();
    ak4pfjets_medium_pfid.clear();
    ak4pfjets_tight_pfid.clear();

    ak4pfjets_cvsl.clear();
    ak4pfjets_deepCSVb.clear();
    ak4pfjets_deepCSVbb.clear();
    ak4pfjets_deepCSVc.clear();
    ak4pfjets_deepCSVl.clear();

    ak4pfjets_ptD.clear();
    ak4pfjets_axis1.clear();
    ak4pfjets_axis2.clear();
    ak4pfjets_mult.clear();

    topcands_ak4idx.clear();
    topcands_disc.clear();
    topcands_p4.clear();
    topcands_Wp4.clear();

    tftops_p4.clear();
    tftops_disc.clear();
    tftops_subjet_pt.clear();
    tftops_subjet_eta.clear();
    tftops_subjet_phi.clear();

    ak4pfjets_chf.clear();
    ak4pfjets_nhf.clear();
    ak4pfjets_cef.clear();
    ak4pfjets_nef.clear();
    ak4pfjets_muf.clear();
    ak4pfjets_elf.clear();
    ak4pfjets_phf.clear();
    ak4pfjets_hhf.clear();
    ak4pfjets_hef.clear();

    ak4pfjets_cm.clear();
    ak4pfjets_nm.clear();
    ak4pfjets_chm.clear();
    ak4pfjets_nhm.clear();
    ak4pfjets_em.clear();
    ak4pfjets_mm.clear();
    ak4pfjets_pm.clear();

    ak4pfjets_mc3dr.clear();
    ak4pfjets_mc3id.clear();
    ak4pfjets_mc3idx.clear();
    ak4pfjets_mcmotherid.clear();
 
    ak4_htssm    = -9999.;
    ak4_htosm    = -9999.;
    ak4_htratiom = -9999.; 

   //overlaps
    ak4pfjet_overlep1_p4 = LorentzVector(0,0, 0,0);
    ak4pfjet_overlep1_deepCSV = -9999;
    ak4pfjet_overlep1_CSV   = -9999;
    ak4pfjet_overlep1_pu_id = -9999;
    ak4pfjet_overlep1_chf   = -9999;
    ak4pfjet_overlep1_nhf   = -9999;
    ak4pfjet_overlep1_cef   = -9999;
    ak4pfjet_overlep1_nef   = -9999;
    ak4pfjet_overlep1_muf   = -9999;
    ak4pfjet_overlep1_cm    = -9999;
    ak4pfjet_overlep1_nm    = -9999;

    ak4pfjet_overlep2_p4 = LorentzVector(0,0, 0,0);
    ak4pfjet_overlep2_deepCSV = -9999;
    ak4pfjet_overlep2_CSV   = -9999;
    ak4pfjet_overlep2_pu_id = -9999;
    ak4pfjet_overlep2_chf   = -9999;
    ak4pfjet_overlep2_nhf   = -9999;
    ak4pfjet_overlep2_cef   = -9999;
    ak4pfjet_overlep2_nef   = -9999;
    ak4pfjet_overlep2_muf   = -9999;
    ak4pfjet_overlep2_cm    = -9999;
    ak4pfjet_overlep2_nm    = -9999;
  
    //ak8 
    ak8pfjets_p4.clear();
    ak8pfjets_tau1.clear();
    ak8pfjets_tau2.clear();
    ak8pfjets_tau3.clear();
    ak8pfjets_parton_flavor.clear();
    ak8pfjets_deepdisc_qcd.clear();
    ak8pfjets_deepdisc_top.clear();
    ak8pfjets_deepdisc_w.clear();
    ak8pfjets_deepdisc_z.clear();
    ak8pfjets_deepdisc_zbb.clear();
    ak8pfjets_deepdisc_hbb.clear();
    ak8pfjets_deepdisc_h4q.clear();
    lead_ak8deepdisc_top = -1;
 
    softtags_p4.clear();

    ak4genjets_p4.clear();
 
    ak4pfjets_MEDbjet_pt.clear();
    ak4pfjets_passMEDbtag.clear();
    ak4pfjets_leadMEDbjet_pt = -9999; 
    ak4pfjets_leadMEDbjet_p4 = LorentzVector(0,0, 0,0);
    ak4pfjets_leadbtag_p4 = LorentzVector(0,0, 0,0);
  
    nskimjets      = -9999;
    nskimbtagmed   = -9999;
    nskimbtagloose = -9999;
    nskimbtagtight = -9999;
    ngoodjets     = -9999;  
    nfailjets     = -9999;  
    ak4_HT 	  = -9999.; 
    nGoodAK8PFJets = -9999;
    nGoodGenJets  = -9999;
    ngoodbtags    = -9999;
    nloosebtags    = -9999;
    ntightbtags    = -9999;
    nanalysisbtags = -9999;
    nsoftbtags    = -9999;
}
 
void JetTree::SetAK4Branches (TTree* tree)
{
    tree->Branch(Form("%snskimjets", prefix_.c_str()) , &nskimjets);
    tree->Branch(Form("%snskimbtagmed", prefix_.c_str()) , &nskimbtagmed);
    tree->Branch(Form("%snskimbtagloose", prefix_.c_str()) , &nskimbtagloose);
    tree->Branch(Form("%snskimbtagtight", prefix_.c_str()) , &nskimbtagtight);
    tree->Branch(Form("%sngoodjets", prefix_.c_str()) , &ngoodjets);
    tree->Branch(Form("%sngoodbtags", prefix_.c_str()) , &ngoodbtags);
    tree->Branch(Form("%snloosebtags", prefix_.c_str()) , &nloosebtags);
    tree->Branch(Form("%sntightbtags", prefix_.c_str()) , &ntightbtags);
    tree->Branch(Form("%snanalysisbtags", prefix_.c_str()) , &nanalysisbtags);
    tree->Branch(Form("%sak4_HT", prefix_.c_str()) , &ak4_HT);
    tree->Branch(Form("%sak4_htratiom", prefix_.c_str()) , &ak4_htratiom);
    tree->Branch(Form("%sdphi_ak4pfjet_met", prefix_.c_str()) , &dphi_ak4pfjet_met);

    tree->Branch(Form("%sak4pfjets_p4", prefix_.c_str()) , &ak4pfjets_p4);

    tree->Branch(Form("%sak4pfjets_passMEDbtag", prefix_.c_str()) , &ak4pfjets_passMEDbtag);
    tree->Branch(Form("%sak4pfjets_deepCSV", prefix_.c_str()) , &ak4pfjets_deepCSV);
    tree->Branch(Form("%sak4pfjets_CSV", prefix_.c_str()) , &ak4pfjets_CSV);
    tree->Branch(Form("%sak4pfjets_mva", prefix_.c_str()) , &ak4pfjets_mva);
    tree->Branch(Form("%sak4pfjets_parton_flavor", prefix_.c_str()) , &ak4pfjets_parton_flavor);
    tree->Branch(Form("%sak4pfjets_hadron_flavor", prefix_.c_str()) , &ak4pfjets_hadron_flavor);
    tree->Branch(Form("%sak4pfjets_loose_puid", prefix_.c_str()) , &ak4pfjets_loose_puid);
    tree->Branch(Form("%sak4pfjets_loose_pfid", prefix_.c_str()) , &ak4pfjets_loose_pfid);

    tree->Branch(Form("%sak4pfjets_leadMEDbjet_p4", prefix_.c_str()) , &ak4pfjets_leadMEDbjet_p4);
    tree->Branch(Form("%sak4pfjets_leadbtag_p4", prefix_.c_str()) , &ak4pfjets_leadbtag_p4);
    tree->Branch(Form("%sak4genjets_p4", prefix_.c_str()) , &ak4genjets_p4); 

    tree->Branch(Form("%ssofttags_p4", prefix_.c_str()) , &softtags_p4);
    tree->Branch(Form("%snsoftbtags", prefix_.c_str()) , &nsoftbtags);
}

void JetTree::SetAK8Branches (TTree* tree)
{
    tree->Branch(Form("%sak8pfjets_p4", prefix_.c_str()) , &ak8pfjets_p4);
    tree->Branch(Form("%sak8pfjets_tau1", prefix_.c_str()) , &ak8pfjets_tau1);
    tree->Branch(Form("%sak8pfjets_tau2", prefix_.c_str()) , &ak8pfjets_tau2);
    tree->Branch(Form("%sak8pfjets_tau3", prefix_.c_str()) , &ak8pfjets_tau3);
    tree->Branch(Form("%sak8pfjets_parton_flavor", prefix_.c_str()) , &ak8pfjets_parton_flavor);
    tree->Branch(Form("%snGoodAK8PFJets", prefix_.c_str()) , &nGoodAK8PFJets);

    tree->Branch(Form("%sak8pfjets_deepdisc_qcd", prefix_.c_str()) , &ak8pfjets_deepdisc_qcd);
    tree->Branch(Form("%sak8pfjets_deepdisc_top", prefix_.c_str()) , &ak8pfjets_deepdisc_top);
    tree->Branch(Form("%sak8pfjets_deepdisc_w", prefix_.c_str()) , &ak8pfjets_deepdisc_w);
    tree->Branch(Form("%sak8pfjets_deepdisc_z", prefix_.c_str()) , &ak8pfjets_deepdisc_z);
    tree->Branch(Form("%sak8pfjets_deepdisc_zbb", prefix_.c_str()) , &ak8pfjets_deepdisc_zbb);
    tree->Branch(Form("%sak8pfjets_deepdisc_hbb", prefix_.c_str()) , &ak8pfjets_deepdisc_hbb);
    tree->Branch(Form("%sak8pfjets_deepdisc_h4q", prefix_.c_str()) , &ak8pfjets_deepdisc_h4q);
    tree->Branch(Form("%slead_ak8deepdisc_top", prefix_.c_str()) , &lead_ak8deepdisc_top);
}

void JetTree::SetAK4Branches_Overleps (TTree* tree)
{
    tree->Branch(Form("%sak4pfjet_overlep1_p4", prefix_.c_str()) , &ak4pfjet_overlep1_p4);                                                                                    
    tree->Branch(Form("%sak4pfjet_overlep1_deepCSV", prefix_.c_str()) , &ak4pfjet_overlep1_deepCSV);
    tree->Branch(Form("%sak4pfjet_overlep1_CSV", prefix_.c_str()) , &ak4pfjet_overlep1_CSV);
    tree->Branch(Form("%sak4pfjet_overlep1_pu_id",prefix_.c_str()) , &ak4pfjet_overlep1_pu_id);
    tree->Branch(Form("%sak4pfjet_overlep1_chf", prefix_.c_str()) , &ak4pfjet_overlep1_chf);
    tree->Branch(Form("%sak4pfjet_overlep1_nhf", prefix_.c_str()) , &ak4pfjet_overlep1_nhf);
    tree->Branch(Form("%sak4pfjet_overlep1_cef", prefix_.c_str()) , &ak4pfjet_overlep1_cef);
    tree->Branch(Form("%sak4pfjet_overlep1_nef", prefix_.c_str()) , &ak4pfjet_overlep1_nef);
    tree->Branch(Form("%sak4pfjet_overlep1_muf", prefix_.c_str()) , &ak4pfjet_overlep1_muf);
    tree->Branch(Form("%sak4pfjet_overlep1_cm", prefix_.c_str()) , &ak4pfjet_overlep1_cm);
    tree->Branch(Form("%sak4pfjet_overlep1_nm", prefix_.c_str()) , &ak4pfjet_overlep1_nm);

    tree->Branch(Form("%sak4pfjet_overlep2_p4", prefix_.c_str()) , &ak4pfjet_overlep2_p4);
    tree->Branch(Form("%sak4pfjet_overlep2_deepCSV", prefix_.c_str()) , &ak4pfjet_overlep2_deepCSV);
    tree->Branch(Form("%sak4pfjet_overlep2_CSV", prefix_.c_str()) , &ak4pfjet_overlep2_CSV);
    tree->Branch(Form("%sak4pfjet_overlep2_pu_id",prefix_.c_str()) , &ak4pfjet_overlep2_pu_id);
    tree->Branch(Form("%sak4pfjet_overlep2_chf", prefix_.c_str()) , &ak4pfjet_overlep2_chf);
    tree->Branch(Form("%sak4pfjet_overlep2_nhf", prefix_.c_str()) , &ak4pfjet_overlep2_nhf);
    tree->Branch(Form("%sak4pfjet_overlep2_cef", prefix_.c_str()) , &ak4pfjet_overlep2_cef);
    tree->Branch(Form("%sak4pfjet_overlep2_nef", prefix_.c_str()) , &ak4pfjet_overlep2_nef);
    tree->Branch(Form("%sak4pfjet_overlep2_muf", prefix_.c_str()) , &ak4pfjet_overlep2_muf);
    tree->Branch(Form("%sak4pfjet_overlep2_cm", prefix_.c_str()) , &ak4pfjet_overlep2_cm);
    tree->Branch(Form("%sak4pfjet_overlep2_nm", prefix_.c_str()) , &ak4pfjet_overlep2_nm);
}

void JetTree::SetAK4Branches_SynchTools (TTree* tree)
{
    tree->Branch(Form("%sak4pfjets_pt", prefix_.c_str()) , &ak4pfjets_pt);
    tree->Branch(Form("%sak4pfjets_eta", prefix_.c_str()) , &ak4pfjets_eta);
    tree->Branch(Form("%sak4pfjets_phi", prefix_.c_str()) , &ak4pfjets_phi);
    tree->Branch(Form("%sak4pfjets_mass", prefix_.c_str()) , &ak4pfjets_mass);
}

void JetTree::SetAK4Branches_EF(TTree* tree)
{
    tree->Branch(Form("%sak4pfjets_chf", prefix_.c_str()) , &ak4pfjets_chf);
    tree->Branch(Form("%sak4pfjets_nhf", prefix_.c_str()) , &ak4pfjets_nhf);
    tree->Branch(Form("%sak4pfjets_cef", prefix_.c_str()) , &ak4pfjets_cef);
    tree->Branch(Form("%sak4pfjets_nef", prefix_.c_str()) , &ak4pfjets_nef);
    tree->Branch(Form("%sak4pfjets_muf", prefix_.c_str()) , &ak4pfjets_muf);
    tree->Branch(Form("%sak4pfjets_muf", prefix_.c_str()) , &ak4pfjets_muf);
    tree->Branch(Form("%sak4pfjets_elf", prefix_.c_str()) , &ak4pfjets_elf);
    tree->Branch(Form("%sak4pfjets_phf", prefix_.c_str()) , &ak4pfjets_phf);
    tree->Branch(Form("%sak4pfjets_hhf", prefix_.c_str()) , &ak4pfjets_hhf);
    tree->Branch(Form("%sak4pfjets_hef", prefix_.c_str()) , &ak4pfjets_hef);

    tree->Branch(Form("%sak4pfjets_cm",  prefix_.c_str()) , &ak4pfjets_cm);
    tree->Branch(Form("%sak4pfjets_nm",  prefix_.c_str()) , &ak4pfjets_nm);
    tree->Branch(Form("%sak4pfjets_chm", prefix_.c_str()) , &ak4pfjets_chm);
    tree->Branch(Form("%sak4pfjets_nhm", prefix_.c_str()) , &ak4pfjets_nhm);
    tree->Branch(Form("%sak4pfjets_em",  prefix_.c_str()) , &ak4pfjets_em);
    tree->Branch(Form("%sak4pfjets_mm",  prefix_.c_str()) , &ak4pfjets_mm);
    tree->Branch(Form("%sak4pfjets_pm",  prefix_.c_str()) , &ak4pfjets_pm);
}

void JetTree::SetAK4Branches_TopTag(TTree* tree)
{
    tree->Branch(Form("%sak4pfjets_cvsl", prefix_.c_str()) , &ak4pfjets_cvsl);
    tree->Branch(Form("%sak4pfjets_ptD", prefix_.c_str()) , &ak4pfjets_ptD);
    tree->Branch(Form("%sak4pfjets_axis1", prefix_.c_str()) , &ak4pfjets_axis1);
    tree->Branch(Form("%sak4pfjets_axis2", prefix_.c_str()) , &ak4pfjets_axis2);
    tree->Branch(Form("%sak4pfjets_mult", prefix_.c_str()) , &ak4pfjets_mult);

    tree->Branch(Form("%sak4pfjets_deepCSVb", prefix_.c_str()) , &ak4pfjets_deepCSVb);
    tree->Branch(Form("%sak4pfjets_deepCSVbb", prefix_.c_str()) , &ak4pfjets_deepCSVbb);
    tree->Branch(Form("%sak4pfjets_deepCSVc", prefix_.c_str()) , &ak4pfjets_deepCSVc);
    tree->Branch(Form("%sak4pfjets_deepCSVl", prefix_.c_str()) , &ak4pfjets_deepCSVl);

    tree->Branch(Form("%stopcands_ak4idx", prefix_.c_str()) , &topcands_ak4idx);
    tree->Branch(Form("%stopcands_disc", prefix_.c_str()) , &topcands_disc);
    tree->Branch(Form("%stopcands_p4", prefix_.c_str()) , &topcands_p4);
    tree->Branch(Form("%stopcands_Wp4", prefix_.c_str()) , &topcands_Wp4);

    tree->Branch(Form("%stftops_p4", prefix_.c_str()) , &tftops_p4);
    tree->Branch(Form("%stftops_disc", prefix_.c_str()) , &tftops_disc);
    tree->Branch(Form("%stftops_subjet_pt", prefix_.c_str()) , &tftops_subjet_pt);
    tree->Branch(Form("%stftops_subjet_eta", prefix_.c_str()) , &tftops_subjet_eta);
    tree->Branch(Form("%stftops_subjet_phi", prefix_.c_str()) , &tftops_subjet_phi);
}

void JetTree::SetAK4Branches_Other(TTree* tree)
{
//these are not completely useless but could go?
    tree->Branch(Form("%snGoodGenJets", prefix_.c_str()) , &nGoodGenJets); 
    tree->Branch(Form("%snfailjets", prefix_.c_str()) , &nfailjets);
    tree->Branch(Form("%sak4_htssm", prefix_.c_str()) , &ak4_htssm);                                                                 
    tree->Branch(Form("%sak4_htosm", prefix_.c_str()) , &ak4_htosm);
    tree->Branch(Form("%sak4pfjets_qg_disc", prefix_.c_str()) , &ak4pfjets_qg_disc);
    tree->Branch(Form("%sak4pfjets_MEDbjet_pt",prefix_.c_str()) , &ak4pfjets_MEDbjet_pt);                                            
    tree->Branch(Form("%sak4pfjets_leadMEDbjet_pt",prefix_.c_str()) , &ak4pfjets_leadMEDbjet_pt);   
    tree->Branch(Form("%sak4pfjets_medium_pfid", prefix_.c_str()) , &ak4pfjets_medium_pfid);
    tree->Branch(Form("%sak4pfjets_tight_pfid", prefix_.c_str()) , &ak4pfjets_tight_pfid);
    tree->Branch(Form("%sak4pfjets_puid", prefix_.c_str()) , &ak4pfjets_puid); 
    tree->Branch(Form("%sak4pfjets_mc3dr", prefix_.c_str()) , &ak4pfjets_mc3dr);
    tree->Branch(Form("%sak4pfjets_mc3id", prefix_.c_str()) , &ak4pfjets_mc3id);                                        
    tree->Branch(Form("%sak4pfjets_mc3idx", prefix_.c_str()) , &ak4pfjets_mc3idx);
    tree->Branch(Form("%sak4pfjets_mcmotherid", prefix_.c_str()) , &ak4pfjets_mcmotherid);
}
