#include "LeptonTree.h"
#include <algorithm>
#include <cmath>
#include "Math/GenVector/PtEtaPhiE4D.h"
#include "CMS3.h"
#include "Config.h"
#include "ElectronSelections.h"
#include "MuonSelections.h"
#include "MCSelections.h"
#include "VertexSelections.h"
#include "StopSelections.h"
#include "IsolationTools.h"
#include "MCSelections.h"

LeptonTree::LeptonTree ()
{
}

LeptonTree::LeptonTree (const std::string &prefix)
  : prefix_(prefix)
{
}

using namespace tas;

void LeptonTree::FillCommon (int id, int idx)
{
    if (idx < 0) return;
    int vtxidx = firstGoodVertex();
    // is_fromw = not evt_isRealData() ? leptonIsFromW(idx, id, true) : -999999;

    // general stuff
    p4		= abs(id)==11 ? els_p4().at(idx) : mus_p4().at(idx);
    pdgid	= id;
    pt 		= abs(id)==11 ? els_p4().at(idx).pt() : abs(id)==13 ? mus_p4().at(idx).pt() : -9999.;
    eta		= abs(id)==11 ? els_p4().at(idx).eta() : abs(id)==13 ? mus_p4().at(idx).eta() : -9999.;     
    phi         = abs(id)==11 ? els_p4().at(idx).phi() : abs(id)==13 ? mus_p4().at(idx).phi() : -9999.;
    mass        = abs(id)==11 ? els_p4().at(idx).mass() : abs(id)==13 ? mus_p4().at(idx).mass() : -9999.;
    charge      = abs(id)==11 ? els_charge().at(idx) : abs(id)==13 ? mus_charge().at(idx) : -9999;

    // mc stuff
    if (!evt_isRealData()) {
          mcp4      = abs(id)==11 ? els_mc_p4().at(idx) : mus_mc_p4().at(idx);
          mc_motherid = abs(id)==11 ? els_mc_motherid().at(idx) : abs(id)==13 ? mus_mc_motherid().at(idx) : -9999;
    
          if(isFromW(id, idx))              production_type = fromW;
          else if(isFromZ(id, idx))         production_type = fromZ;
          else if(isFromB(id, idx))         production_type = fromB;
          else if(isFromC(id, idx))         production_type = fromC;
          else if(isFromLight(id, idx))     production_type = fromLight;
          else if(isFromLightFake(id, idx)) production_type = fromLightFake;
          else production_type = none;
    }
    
    // electrons
    if (abs(id) == 11)
    {
	if (vtxidx >= 0) {
            d0 = els_dxyPV().at(idx);
            dz = els_dzPV().at(idx);
            d0err = els_d0Err().at(idx);
            dzerr = els_z0Err().at(idx);
    	}

        passLooseID  = electronID(idx, STOP_loose_v4);
        passMediumID = electronID(idx, STOP_medium_v4);
        passTightID  = electronID(idx, STOP_tight_v4);
        passVeto     = electronID(idx, STOP_veto_v4);

        if (gconf.year == 2016) {
            is_lepid_loose_noiso  = isLooseElectronPOGspring15noIso_v1(idx);
            is_lepid_medium_noiso = isMediumElectronPOGspring15noIso_v1(idx);
            is_lepid_tight_noiso  = isTightElectronPOGspring15noIso_v1(idx);
        } else if (gconf.year == 2017) {
            is_lepid_loose_noiso  = isLooseElectronPOGfall17noIso_v2(idx);
            is_lepid_medium_noiso = isMediumElectronPOGfall17noIso_v2(idx);
            is_lepid_tight_noiso  = isTightElectronPOGfall17noIso_v2(idx);
        } else if (gconf.year == 2018) {
            is_lepid_loose_noiso  = isLooseElectronPOGfall17noIso_v2(idx); // to be updated
            is_lepid_medium_noiso = isMediumElectronPOGfall17noIso_v2(idx);
            is_lepid_tight_noiso  = isTightElectronPOGfall17noIso_v2(idx);
        } else {
            cout << "LeptonTree.cc: gconf.year " << gconf.year << " is not in the listed!\n";
        }

        //id variables
        eoverpin        = els_eOverPIn().at(idx); 
        sigmaIEtaEta_fill5x5 = els_sigmaIEtaIEta_full5x5().at(idx);
        dEtaIn = els_dEtaIn().at(idx);
        dPhiIn = els_dPhiIn().at(idx);
        hOverE = els_hOverE().at(idx);
        ooEmooP = (1.0/els_ecalEnergy().at(idx)) - (els_eOverPIn().at(idx)/els_ecalEnergy() .at(idx));
        expectedMissingInnerHits = els_exp_innerlayers().at(idx);
        conversionVeto = els_conv_vtx_flag().at(idx);
        etaSC = els_etaSC().at(idx);
        ChiSqr = els_chi2().at(idx);

        //iso variables
        chiso     = els_pfChargedHadronIso().at(idx);
        nhiso     = els_pfNeutralHadronIso().at(idx);
        emiso     = els_pfPhotonIso().at(idx);
        deltaBeta = els_pfPUIso().at(idx);               

	//ISO
	relIso03DB = eleRelIso03(idx, STOP);
        relIso03EA = eleRelIso03EA(idx);

        miniRelIsoDB = elMiniRelIsoCMS3_DB(idx);
        miniRelIsoEA = elMiniRelIsoCMS3_EA(idx,gconf.ea_version);
        MiniIso      = miniRelIsoEA;
        relIso       = eleRelIso03EA(idx);
    } // end electron block

    // muons
    else if (abs(id) == 13)
    {
        //int trkidx = mus_trkidx().at(idx);
       // if (trkidx >= 0 && vtxidx >= 0) {
        d0 = mus_dxyPV().at(idx);
        dz = mus_dzPV().at(idx);
        d0err = mus_d0Err().at(idx);
        dzerr = mus_z0Err().at(idx);
       // }
        passLooseID = muonID(idx, STOP_loose_v4);
        passMediumID = muonID(idx, STOP_medium_v4);
        passTightID =  muonID(idx, STOP_tight_v4);
        passVeto = muonID(idx, STOP_loose_v4);

        if (gconf.cmssw_ver >= 94) {
            is_lepid_loose_noiso  = passesMuonPOG(muID::CutBasedIdLoose, idx);
            is_lepid_medium_noiso = passesMuonPOG(muID::CutBasedIdMedium, idx);
            is_lepid_tight_noiso  = passesMuonPOG(muID::CutBasedIdTight, idx);
        } else {
            is_lepid_loose_noiso  = isLooseMuonPOG(idx);
            is_lepid_medium_noiso = isMediumMuonPOG(idx);
            is_lepid_tight_noiso  = isTightMuonPOG(idx);
        }

        //iso variables
        chiso     = mus_isoR04_pf_ChargedHadronPt().at(idx);
        nhiso     = mus_isoR04_pf_NeutralHadronEt().at(idx);
        emiso     = mus_isoR04_pf_PhotonEt().at(idx);
        deltaBeta = mus_isoR04_pf_PUPt().at(idx);

   	//ISO
    	relIso03DB = muRelIso03(idx, STOP);
    	relIso03EA = muRelIso03EA(idx);
    	relIso04DB = muRelIso04DB(idx);

	miniRelIsoDB = muMiniRelIsoCMS3_DB(idx);
	miniRelIsoEA = muMiniRelIsoCMS3_EA(idx,gconf.ea_version);
	MiniIso      = miniRelIsoEA;
        relIso       = muRelIso03EA(idx);
    } // end muon block

}

void LeptonTree::Reset()
{
    //is_fromw        = -9999;
    charge          = -9999;
    pdgid           = -9999;
    production_type = none;
    d0              = -9999.;
    d0err           = -9999.;
    dz              = -9999.;
    dzerr           = -9999.;

    sigmaIEtaEta_fill5x5 = -9999.; 
    dEtaIn            = -9999.;
    dPhiIn            = -9999.;
    hOverE            = -9999.;
    ooEmooP           = -9999.;
    expectedMissingInnerHits = -9999.;
    conversionVeto    = -9999.;
    etaSC             = -9999.;
    ChiSqr            = -9999.;

    chiso           = -9999.;     	   
    nhiso           = -9999.;
    emiso           = -9999.;
    deltaBeta       = -9999.;

    //pfiso04         = -9999.;
    //pfiso03         = -9999.;
    relIso03DB      = -9999.;
    relIso03EA      = -9999.;    
    relIso04DB      = -9999.;
    miniRelIsoDB    = -9999.;
    miniRelIsoEA    = -9999.;
    MiniIso         = -9999.;

    mc_motherid     = -9999;

    passVeto     = false;
    passLooseID  = false;
    passMediumID = false;
    passTightID = false;

   // mus_pfcands_idx = -99999;    

    p4           = LorentzVector(0, 0, 0, 0);
    mcp4         = LorentzVector(0, 0, 0, 0);
    //pfp4         = LorentzVector(0, 0, 0, 0);

    pt		= -9999.;
    eta		= -9999.;
    mass 	= -9999.;
    phi		= -9999.;

    is_lepid_loose_noiso  = false;
    is_lepid_medium_noiso = false;
    is_lepid_tight_noiso  = false;

    dphiMET= -9999.;
    dphiMET_jup= -9999.;
    dphiMET_jdown= -9999.;
    dphiMET_rl= -9999.;
    dphiMET_rl_jup= -9999.;
    dphiMET_rl_jdown= -9999.;
    dphiMET_resup= -9999.;
    dphiMET_resdown= -9999.;
    dphiMET_rl_resup= -9999.;
    dphiMET_rl_resdown= -9999.;
    dphiGenMET= -9999.;
    dphiGenMET_rl= -9999.;

}

void LeptonTree::SetBranches(TTree* tree)
{
    tree->Branch(Form("%spdgid"           , prefix_.c_str()) , &pdgid           ); 
    tree->Branch(Form("%sproduction_type" , prefix_.c_str()) , &production_type );
     tree->Branch(Form("%sMiniIso"       , prefix_.c_str()) , &MiniIso);
     tree->Branch(Form("%srelIso"       , prefix_.c_str()) , &relIso       );

    tree->Branch(Form("%spassLooseID"   , prefix_.c_str()) , &passLooseID);
    tree->Branch(Form("%spassMediumID"   , prefix_.c_str()) , &passMediumID);
    tree->Branch(Form("%spassTightID"   , prefix_.c_str()) , &passTightID);
    tree->Branch(Form("%spassVeto"       , prefix_.c_str()) , &passVeto);

    tree->Branch(Form("%sp4"      , prefix_.c_str()) , "LorentzVector" , &p4      );
    tree->Branch(Form("%smcp4"    , prefix_.c_str()) , "LorentzVector" , &mcp4    );
    tree->Branch(Form("%smc_motherid" , prefix_.c_str()) , &mc_motherid);
    tree->Branch(Form("%setaSC"       , prefix_.c_str()) , &etaSC);

    tree->Branch(Form("%sdphiMET"   , prefix_.c_str()) , &dphiMET);
    tree->Branch(Form("%sdphiMET_jup"   , prefix_.c_str()) , &dphiMET_jup);
    tree->Branch(Form("%sdphiMET_jdown"   , prefix_.c_str()) , &dphiMET_jdown);
    tree->Branch(Form("%sdphiMET_rl"   , prefix_.c_str()) , &dphiMET_rl);
    tree->Branch(Form("%sdphiMET_rl_jup"   , prefix_.c_str()) , &dphiMET_rl_jup);
    tree->Branch(Form("%sdphiMET_rl_jdown"   , prefix_.c_str()) , &dphiMET_rl_jdown);
    tree->Branch(Form("%sdphiMET_resup"   , prefix_.c_str()) , &dphiMET_resup);
    tree->Branch(Form("%sdphiMET_resdown"   , prefix_.c_str()) , &dphiMET_resdown);
    tree->Branch(Form("%sdphiMET_rl_resup"   , prefix_.c_str()) , &dphiMET_rl_resup);
    tree->Branch(Form("%sdphiMET_rl_resdown"   , prefix_.c_str()) , &dphiMET_rl_resdown);
    tree->Branch(Form("%sdphiGenMET"   , prefix_.c_str()) , &dphiGenMET);
    tree->Branch(Form("%sdphiGenMET_rl"   , prefix_.c_str()) , &dphiGenMET_rl);
}


void LeptonTree::SetBranches_electronID(TTree* tree)
{
    tree->Branch(Form("%sd0"              , prefix_.c_str()) , &d0              ); 
    tree->Branch(Form("%sd0err"           , prefix_.c_str()) , &d0err           ); 
    tree->Branch(Form("%sdz"              , prefix_.c_str()) , &dz              ); 
    tree->Branch(Form("%sdzerr"           , prefix_.c_str()) , &dzerr           ); 

    tree->Branch(Form("%ssigmaIEtaEta_fill5x5"        , prefix_.c_str()) , &sigmaIEtaEta_fill5x5);
    tree->Branch(Form("%sdEtaIn"        , prefix_.c_str()) , &dEtaIn);
    tree->Branch(Form("%sdPhiIn"        , prefix_.c_str()) , &dPhiIn);
    tree->Branch(Form("%shOverE"        , prefix_.c_str()) , &hOverE);
    tree->Branch(Form("%sooEmooP"       , prefix_.c_str()) , &ooEmooP);
    tree->Branch(Form("%sexpectedMissingInnerHits"    , prefix_.c_str()) , &expectedMissingInnerHits);
    tree->Branch(Form("%sconversionVeto", prefix_.c_str()) , &conversionVeto);
    tree->Branch(Form("%sChiSqr"        , prefix_.c_str()) , &ChiSqr);
    tree->Branch(Form("%seoverpin"        , prefix_.c_str()) , &eoverpin        ); 
    tree->Branch(Form("%sis_lepid_loose_noiso" , prefix_.c_str()) , &is_lepid_loose_noiso);
    tree->Branch(Form("%sis_lepid_medium_noiso" , prefix_.c_str()) , &is_lepid_medium_noiso); 
    tree->Branch(Form("%sis_lepid_tight_noiso"  , prefix_.c_str()) , &is_lepid_tight_noiso); 
} 

void LeptonTree::SetBranches_Iso(TTree* tree)
{
    tree->Branch(Form("%schiso"       , prefix_.c_str()) , &chiso);
    tree->Branch(Form("%snhiso"       , prefix_.c_str()) , &nhiso);
    tree->Branch(Form("%semiso"       , prefix_.c_str()) , &emiso);
    tree->Branch(Form("%sdeltaBeta"   , prefix_.c_str()) , &deltaBeta);
    //tree->Branch(Form("%spfiso04"      , prefix_.c_str()) , &pfiso04         );
    //tree->Branch(Form("%spfiso03"        , prefix_.c_str()) , &pfiso03         );
     tree->Branch(Form("%srelIso03DB"       , prefix_.c_str()) , &relIso03DB       );
     tree->Branch(Form("%srelIso03EA"       , prefix_.c_str()) , &relIso03EA       );
     tree->Branch(Form("%srelIso04DB"       , prefix_.c_str()) , &relIso04DB       );

     tree->Branch(Form("%sminiRelIsoDB"       , prefix_.c_str()) , &miniRelIsoDB);
     tree->Branch(Form("%sminiRelIsoEA"       , prefix_.c_str()) , &miniRelIsoEA);
     tree->Branch(Form("%sMiniIso"       , prefix_.c_str()) , &MiniIso);

} 

void LeptonTree::SetBranches_SynchTools (TTree* tree)
{
   tree->Branch(Form("%spt"      , prefix_.c_str()) , &pt);
   tree->Branch(Form("%seta"      , prefix_.c_str()) , &eta);
   tree->Branch(Form("%sphi"      , prefix_.c_str()) , &phi);
   tree->Branch(Form("%smass"      , prefix_.c_str()) , &mass);
}
