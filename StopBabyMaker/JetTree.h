#ifndef JETTREE_H
#define JETTREE_H
 
#include <string>
#include <vector>
#include "Math/LorentzVector.h"
#include "JetCorrector.h"
#include "StopSelections.h"
#include "btagsf/BTagCalibrationStandalone.h"
#include "TH2.h" 
#include "ResTopTagger/ResolvedTopMVA.h"
#include "TFTopTagger/include/TopTagger.h"
// forward declarations
class TTree;
 
// typedefs
typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > LorentzVector;
typedef std::vector<LorentzVector> vecLorentzVector;
typedef std::vector<float> vecd;
typedef std::vector<bool> vecb;
typedef std::vector<int> veci;
typedef std::vector<std::vector<int> > vecii;
 
class JetTree
{
public:
    // constructor/destructor
    JetTree ();
    JetTree (const std::string &prefix);
    virtual ~JetTree ();
 
    void Reset ();
    void deleteBtagSFTool();
    void InitBtagSFTool(bool isFastsim_);
    void InitTopMVA(ResolvedTopMVA* resTopMVAptr);
    void SetBranches (TTree* tree);
    void SetAK4Branches_Other(TTree* tree);
    void SetAK4Branches_EF(TTree* tree);
    void SetAK4Branches_SynchTools (TTree* tree);
    void SetAK4Branches_Overleps (TTree* tree);
    void SetAK4Branches_TopTag (TTree* tree);
    void SetAK8Branches (TTree* tree);
    void SetAK4Branches (TTree* tree);
    void SetAliases (TTree* tree);
    void FillCommon(std::vector<unsigned int> alloverlapjets_idx,  FactorizedJetCorrector* corrector, float& btagprob_data, float &btagprob_mc, float &btagprob_heavy_UP, float& btagprob_heavy_DN, float& btagprob_light_UP, float& btagprob_light_DN, float& btagprob_FS_UP, float& btagprob_FS_DN, float& loosebtagprob_data, float &loosebtagprob_mc, float &loosebtagprob_heavy_UP, float& loosebtagprob_heavy_DN, float& loosebtagprob_light_UP, float& loosebtagprob_light_DN, float& loosebtagprob_FS_UP, float& loosebtagprob_FS_DN, float& tightbtagprob_data, float &tightbtagprob_mc, float &tightbtagprob_heavy_UP, float& tightbtagprob_heavy_DN, float& tightbtagprob_light_UP, float& tightbtagprob_light_DN, float& tightbtagprob_FS_UP, float& tightbtagprob_FS_DN, unsigned int overlep1_idx, unsigned int overlep2_idx, bool applynewcorr, JetCorrectionUncertainty* jetcorr_uncertainty, int JES_type, bool applyBtagSFs, bool isFastsim);
    void FillAK8Jets(bool applynewcorr, FactorizedJetCorrector* ak8corrector, JetCorrectionUncertainty* ak8jetcorr_uncertainty, int JES_type);
    float getBtagEffFromFile(float pt, float eta, int mcFlavour, int WP, bool isFastsim);
    void SetJetSelection (std::string cone_size, float pt_cut, float eta, bool id, float goodjet_pt = 30, float goodb_pt = 30);
    void GetJetSelections (std::string cone_size = "");
protected:

    std::string prefix_;

public:
   // branch objects
 
    // ak4 PF jets
    int nskimjets;
    int nskimbtagmed;
    int nskimbtagtight;
    int nskimbtagloose;
    int ngoodjets;
    int nfailjets; //jets not passing loose jid
    int nGoodAK8PFJets;
    int nGoodGenJets;
    int ngoodbtags;
    int nloosebtags;
    int ntightbtags;
    int nanalysisbtags;
    float ak4_HT;
    float ak4_HTeta5;
    float ak4_MHT_pt;
    float ak4_MHT_phi;
    
    float ak4_htssm;
    float ak4_htosm;
    float ak4_htratiom;

    vecd dphi_ak4pfjet_met;
    vecLorentzVector ak4pfjets_p4;
    vecd ak4pfjets_pt;
    vecd ak4pfjets_eta;
    vecd ak4pfjets_phi;
    vecd ak4pfjets_mass;

    LorentzVector ak4pfjets_leadMEDbjet_p4;
    vecd ak4pfjets_qg_disc;    
    vecd ak4pfjets_CSV;
    vecd ak4pfjets_deepCSV;
    vecd ak4pfjets_mva;

    vecd ak4pfjets_cvsl;
    vecd ak4pfjets_deepCSVb;
    vecd ak4pfjets_deepCSVbb;
    vecd ak4pfjets_deepCSVc;
    vecd ak4pfjets_deepCSVl;

    vecd ak4pfjets_puid;
    veci ak4pfjets_parton_flavor;
    veci ak4pfjets_hadron_flavor;
    vecb ak4pfjets_loose_puid;
    vecb ak4pfjets_loose_pfid;
    vecb ak4pfjets_medium_pfid;
    vecb ak4pfjets_tight_pfid;
    vecb ak4pfjets_passMEDbtag;
    float ak4pfjets_leadMEDbjet_pt;
    vecd ak4pfjets_MEDbjet_pt;
    LorentzVector ak4pfjets_leadbtag_p4;

    vecd ak4pfjets_ptD;
    vecd ak4pfjets_axis1;
    vecd ak4pfjets_axis2;
    veci ak4pfjets_mult;

    vecd ak4pfjets_chf;       
    vecd ak4pfjets_nhf;
    vecd ak4pfjets_cef;
    vecd ak4pfjets_nef;
    vecd ak4pfjets_muf;
    vecd ak4pfjets_elf;
    vecd ak4pfjets_phf;
    vecd ak4pfjets_hhf;
    vecd ak4pfjets_hef;

    vecd ak4pfjets_cm;
    vecd ak4pfjets_nm;
    vecd ak4pfjets_chm;
    vecd ak4pfjets_nhm;
    vecd ak4pfjets_em;
    vecd ak4pfjets_mm;
    vecd ak4pfjets_pm;

    veci ak4pfjets_mc3dr;
    veci ak4pfjets_mc3id;
    veci ak4pfjets_mc3idx;
    veci ak4pfjets_mcmotherid;

   //overlaps for selected leptons only
    LorentzVector ak4pfjet_overlep1_p4;
    float ak4pfjet_overlep1_CSV;
    float ak4pfjet_overlep1_deepCSV;
    float ak4pfjet_overlep1_pu_id;
    float ak4pfjet_overlep1_chf;       
    float ak4pfjet_overlep1_nhf;
    float ak4pfjet_overlep1_cef;
    float ak4pfjet_overlep1_nef;
    float ak4pfjet_overlep1_muf;
    int ak4pfjet_overlep1_cm;
    int ak4pfjet_overlep1_nm;

    LorentzVector ak4pfjet_overlep2_p4;
    float ak4pfjet_overlep2_CSV;
    float ak4pfjet_overlep2_deepCSV;
    float ak4pfjet_overlep2_pu_id;
    float ak4pfjet_overlep2_chf;
    float ak4pfjet_overlep2_nhf;
    float ak4pfjet_overlep2_cef;
    float ak4pfjet_overlep2_nef;
    float ak4pfjet_overlep2_muf;
    int ak4pfjet_overlep2_cm;
    int ak4pfjet_overlep2_nm;
 
    // ak8 PF jets
    vecLorentzVector ak8pfjets_p4;
    vecd ak8pfjets_tau1;
    vecd ak8pfjets_tau2;
    vecd ak8pfjets_tau3;
    veci ak8pfjets_parton_flavor;
    vecd ak8pfjets_deepdisc_qcd;
    vecd ak8pfjets_deepdisc_top;
    vecd ak8pfjets_deepdisc_w;
    vecd ak8pfjets_deepdisc_z;
    vecd ak8pfjets_deepdisc_zbb;
    vecd ak8pfjets_deepdisc_hbb;
    vecd ak8pfjets_deepdisc_h4q;
    float lead_ak8deepdisc_top;
     
    // genjets    
    vecLorentzVector ak4genjets_p4;    
 
    // soft btag
    vecLorentzVector softtags_p4;
    vecLorentzVector scndvtxs_p4;
    vecb scndvtxs_passSofttag;
    int nsoftbtags;

    // resolved top tagger -- BDT version
    bool doResolveTopMVA;
    ResolvedTopMVA* resTopMVA;
    vecii topcands_ak4idx;
    vecd topcands_disc;
    vecLorentzVector topcands_p4;
    vecLorentzVector topcands_Wp4;

    // resolved top tagger -- tensorflow veriosn
    TopTagger* tftagger;
    vecLorentzVector tftops_p4;
    vecd tftops_disc;
    vector<vecd> tftops_subjet_pt;
    vector<vecd> tftops_subjet_eta;
    vector<vecd> tftops_subjet_phi;

private:
    float m_ak4_pt_cut;
    float m_ak8_pt_cut;
    float m_ak4_eta_cut;
    float m_ak8_eta_cut;
    float m_goodjet_pt;
    float m_goodbjet_pt;
    bool m_ak4_passid;
    bool m_ak8_passid;
    bool isFastsim;
    BTagCalibration* calib;
    BTagCalibrationReader* reader_medium;
    BTagCalibrationReader* reader_loose;
    BTagCalibrationReader* reader_tight;
    BTagCalibration* calib_fastsim;
    BTagCalibrationReader* reader_medium_FS;
    BTagCalibrationReader* reader_loose_FS;
    BTagCalibrationReader* reader_tight_FS;
    TFile *feff;
    TH2D* h_btag_eff_b;
    TH2D* h_btag_eff_c;
    TH2D* h_btag_eff_udsg;
    TH2D* h_loose_btag_eff_b;
    TH2D* h_loose_btag_eff_c;
    TH2D* h_loose_btag_eff_udsg;
    TH2D* h_tight_btag_eff_b;
    TH2D* h_tight_btag_eff_c;
    TH2D* h_tight_btag_eff_udsg;
};
 
#endif
