from __future__ import print_function

import time

from metis.Sample import SNTSample
from metis.Sample import DirectorySample
from metis.CondorTask import CondorTask
from metis.StatsParser import StatsParser
from metis.Utils import do_cmd

def getNfilesPerJob(babyname):
    if babyname.endswith('met'): return 20
    if babyname.endswith('singlemu'): return 20
    if babyname.endswith('singleel'): return 20
    if babyname.endswith('egamma'): return 20
    if 'data' in babyname: return 25
    if '150to250' in babyname: return 4
    if 'SMS' in babyname: return 2
    if 'T2tt' in babyname: return 1
    if 'TTJets_2lep' in babyname: return 1
    if 'TTJets_1lep' in babyname: return 3
    if 'W1Jets' in babyname: return 5
    if 'NuPt200' in babyname: return 1
    if 'TTZToLLNuNu' in babyname: return 1
    if 'TTWJetsToLNu' in babyname: return 1
    if 'TTWJets' in babyname: return 2
    if 'ST_tW' in babyname: return 3
    if 'To2L2Q' in babyname: return 10
    if 'To2Q2Nu' in babyname: return 20
    if 'TTTo2L2Nu' in babyname: return 5

    return 5

if __name__ == "__main__":

    snt_samples = {

        ################################
        # 2018 Data ReReco + PromptReco for eraD
        # Latest cms4: CMS4_V9-04-1?

        # "data_2018A_met" : "/MET/Run2018A-17Sep2018-v1/MINIAOD",
        # "data_2018B_met" : "/MET/Run2018B-17Sep2018-v1/MINIAOD",
        # "data_2018C_met" : "/MET/Run2018C-17Sep2018-v1/MINIAOD",
        # "data_2018D_met" : "/MET/Run2018D-PromptReco-v2/MINIAOD",

        # "data_2018A_singlemu" : "/SingleMuon/Run2018A-17Sep2018-v2/MINIAOD",
        # "data_2018B_singlemu" : "/SingleMuon/Run2018B-17Sep2018-v1/MINIAOD",
        # "data_2018C_singlemu" : "/SingleMuon/Run2018C-17Sep2018-v1/MINIAOD",
        # "data_2018D_singlemu" : "/SingleMuon/Run2018D-PromptReco-v2/MINIAOD",

        # "data_2018A_egamma" : "/EGamma/Run2018A-17Sep2018-v2/MINIAOD",
        # "data_2018B_egamma" : "/EGamma/Run2018B-17Sep2018-v1/MINIAOD",
        # "data_2018C_egamma" : "/EGamma/Run2018C-17Sep2018-v1/MINIAOD",
        # "data_2018D_egamma" : "/EGamma/Run2018D-PromptReco-v2/MINIAOD",

        # "data_2018A_muoneg" : "/MuonEG/Run2018A-17Sep2018-v1/MINIAOD",
        # "data_2018B_muoneg" : "/MuonEG/Run2018B-17Sep2018-v1/MINIAOD",
        # "data_2018C_muoneg" : "/MuonEG/Run2018C-17Sep2018-v1/MINIAOD",
        # "data_2018D_muoneg" : "/MuonEG/Run2018D-PromptReco-v2/MINIAOD",

        # "data_2018A_jetht"  : "/JetHT/Run2018A-17Sep2018-v1/MINIAOD",
        # "data_2018B_jetht"  : "/JetHT/Run2018C-17Sep2018-v1/MINIAOD",
        # "data_2018C_jetht"  : "/JetHT/Run2018B-17Sep2018-v1/MINIAOD",
        # "data_2018D_jetht"  : "/JetHT/Run2018D-PromptReco-v2/MINIAOD",

        ################################
        # 2018 MC (102X MiniAOD)
        # Latest cms4: CMS4_V10-02-04

        # "SMS_T2tt_mStop150to250_a18v1"   : "/SMS-T2tt_mStop-150to250_TuneCP2_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-PUFall18Fast_102X_upgrade2018_realistic_v15-v3/MINIAODSIM",
        # "SMS_T2tt_mStop250to350_a18v1"   : "/SMS-T2tt_mStop-250to350_TuneCP2_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-PUFall18Fast_102X_upgrade2018_realistic_v15-v3/MINIAODSIM",
        # "SMS_T2tt_mStop350to400_a18v1"   : "/SMS-T2tt_mStop-350to400_TuneCP2_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-PUFall18Fast_102X_upgrade2018_realistic_v15-v3/MINIAODSIM",
        # "SMS_T2tt_mStop400to1200_a18v1"  : "/SMS-T2tt_mStop-400to1200_TuneCP2_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-PUFall18Fast_102X_upgrade2018_realistic_v15-v2/MINIAODSIM",
        # "SMS_T2tt_mStop1200to2000_a18v1" : "/SMS-T2tt_mStop-1200to2000_TuneCP2_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-PUFall18Fast_102X_upgrade2018_realistic_v15-v2/MINIAODSIM",

        # "SMS_T2bW_a18v1" : "/SMS-T2bW_TuneCP2_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-PUFall18Fast_102X_upgrade2018_realistic_v15-v2/MINIAODSIM",
        # "SMS_T2bt_a18v1" : "/SMS-T2bt_TuneCP2_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-PUFall18Fast_102X_upgrade2018_realistic_v15-v2/MINIAODSIM",

        # "SMS_T8bbstausnu_XStau0p75_a18v1" : "/SMS-T8bbstausnu_mStop-200to1800_XCha0p5_XStau0p75_TuneCP2_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-PUFall18Fast_102X_upgrade2018_realistic_v15-v2/MINIAODSIM",
        # "SMS_T8bbstausnu_XStau0p5_a18v1"  : "/SMS-T8bbstausnu_mStop-200to1800_XCha0p5_XStau0p5_TuneCP2_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-PUFall18Fast_102X_upgrade2018_realistic_v15-v2/MINIAODSIM",
        # "SMS_T8bbstausnu_XStau0p25_a18v1" : "/SMS-T8bbstausnu_XCha0p5_mStop-200to1800_XStau0p25_TuneCP2_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-PUFall18Fast_102X_upgrade2018_realistic_v15-v2/MINIAODSIM",

        # ttbar
        # "TTJets_2lep_a18v1"      : "/TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM",  # pythia 8.230-gnimlf4
        # "TTJets_1lep_top_a18v1"  : "/TTJets_SingleLeptFromT_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM",
        # "TTJets_1lep_tbar_a18v1" : "/TTJets_SingleLeptFromTbar_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM",

        # # W+jets
        # "W1Jets_NuPt200_a18v1" : "/W1JetsToLNu_NuPt-200_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM",
        # "W2Jets_NuPt200_a18v1" : "/W2JetsToLNu_NuPt-200_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM",
        # "W3Jets_NuPt200_a18v1" : "/W3JetsToLNu_NuPt-200_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM",
        # "W4Jets_NuPt200_a18v1" : "/W4JetsToLNu_NuPt-200_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM",
        # "W1JetsToLNu_a18v1"    : "/W1JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2/MINIAODSIM",
        # "W2JetsToLNu_a18v1"    : "/W2JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2/MINIAODSIM",
        # "W3JetsToLNu_a18v1"    : "/W3JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2/MINIAODSIM",
        # "W4JetsToLNu_a18v1"    : "/W4JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2/MINIAODSIM",

        # # single top
        # "ST_tW_top_noHadDecays_a18v1"    : "/ST_tW_top_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v3/MINIAODSIM",
        # "ST_tW_tbar_noHadDecays_a18v1"   : "/ST_tW_antitop_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v3/MINIAODSIM",
        # "ST_tchan_top_inclDecays_a18v1"  : "/ST_t-channel_top_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM",
        # "ST_tchan_tbar_inclDecays_a18v1" : "/ST_t-channel_antitop_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM",
        # "ST_schan_4f_lepDecays_a18v1"    : "/ST_s-channel_4f_leptonDecays_TuneCP5_13TeV-madgraph-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v4/MINIAODSIM",

        # # TTV
        # "TTZToLLNuNu_a18v1" : "/TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM",
        # "TTWJetsToLNu_a18v1" : "/TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM",

        # # diboson
        # "WZTo1L3Nu_a18v1" : "/WZTo1L3Nu_13TeV_amcatnloFXFX_madspin_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM",
        # "WZTo2L2Q_a18v1" : "/WZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM",
        # "WZTo3LNu_a18v1" : "/WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM",

        # "WWTo2L2Nu_a18v1" : "/WWTo2L2Nu_NNPDF31_TuneCP5_13TeV-powheg-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM",
        # "WWToLNuQQ_a18v1" : "/WWToLNuQQ_NNPDF31_TuneCP5_13TeV-powheg-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM",

        # "ZZTo2L2Nu_a18v1" : "/ZZTo2L2Nu_TuneCP5_13TeV_powheg_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM",
        # "ZZTo4L_a18v1"    : "/ZZTo4L_TuneCP5_13TeV_powheg_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext2-v2/MINIAODSIM",
        # "ZZTo2L2Q_a18v1"  : "/ZZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM",
        # "ZZTo2Q2Nu_a18v1" : "/ZZTo2Q2Nu_TuneCP5_13TeV_amcatnloFXFX_madspin_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM",

        # # DY jets
        # "DYJetsToLL_madgraph_a18v1" : "/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM",
        # "DYJetsToLL_amcnlo_a18v1"   : "/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM",
        # "DYJetsToLL_Zpt150_a18v1" : "/DYJetsToLL_M-50_Zpt-150toInf_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM",

        # # tt+gamma
        # "TTGamma_SingleLept_a18v1" : "/TTGamma_SingleLeptFromTbar_TuneCP5_13TeV_madgraph_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM",
        # "TTGamma_SingleLept_a18v1" : "/TTGamma_SingleLeptFromT_TuneCP5_13TeV_madgraph_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM",
        # "TTGamma_Dilept_Tune_a18v1" : "/TTGamma_Dilept_TuneCP5_13TeV_madgraph_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM",

        # Extra
        # "TTTo2L2Nu_a18v1" : "/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM",
        # "TTJets_amcnlo_a18v1" : "/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM",
        # "ttZJets_a18v1" : "/ttZJets_TuneCP5_13TeV_madgraphMLM_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM"
        # "TTZZ_a18v1" : "/TTZZ_TuneCP5_13TeV-madgraph-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM",
        # "TTZH_a18v1" : "/TTZH_TuneCP5_13TeV-madgraph-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM",
        # "TTWW_a18v1" : "/TTWW_TuneCP5_13TeV-madgraph-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM",
        # "TTWZ_a18v1" : "/TTWZ_TuneCP5_13TeV-madgraph-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM",
        # "WZ_incl_a18v1" : "/WZ_TuneCP5_13TeV-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM",
        # "WZTo3LNu_a18v1" : "/WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM",
        # "ZZTo2L2Nu_a18v1_ext2" : "/ZZTo2L2Nu_TuneCP5_13TeV_powheg_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext2-v2/MINIAODSIM",

        ################################
        # 2017 ReReco (94X)
        # Latest cms4: CMS4_V10-02-05

        # "data_2017B_met" : "/MET/Run2017B-31Mar2018-v1/MINIAOD",
        # "data_2017C_met" : "/MET/Run2017C-31Mar2018-v1/MINIAOD",
        # "data_2017D_met" : "/MET/Run2017D-31Mar2018-v1/MINIAOD",
        # "data_2017E_met" : "/MET/Run2017E-31Mar2018-v1/MINIAOD",
        # "data_2017F_31Mar_met" : "/MET/Run2017F-31Mar2018-v1/MINIAOD",
        # # "data_2017F_09May_met" : "/MET/Run2017F-09May2018-v1/MINIAOD",

        # "data_2017B_singleel" : "/SingleElectron/Run2017B-31Mar2018-v1/MINIAOD",
        # "data_2017C_singleel" : "/SingleElectron/Run2017C-31Mar2018-v1/MINIAOD",
        # "data_2017D_singleel" : "/SingleElectron/Run2017D-31Mar2018-v1/MINIAOD",
        # "data_2017E_singleel" : "/SingleElectron/Run2017E-31Mar2018-v1/MINIAOD",
        # "data_2017F_31Mar_singleel" : "/SingleElectron/Run2017F-31Mar2018-v1/MINIAOD",
        # # "data_2017F_09May_singleel" : "/SingleElectron/Run2017F-09May2018-v1/MINIAOD",

        # "data_2017B_singlemu" : "/SingleMuon/Run2017B-31Mar2018-v1/MINIAOD",
        # "data_2017C_singlemu" : "/SingleMuon/Run2017C-31Mar2018-v1/MINIAOD",
        # "data_2017D_singlemu" : "/SingleMuon/Run2017D-31Mar2018-v1/MINIAOD",
        # "data_2017E_singlemu" : "/SingleMuon/Run2017E-31Mar2018-v1/MINIAOD",
        # "data_2017F_31Mar_singlemu" : "/SingleMuon/Run2017F-31Mar2018-v1/MINIAOD",
        # # "data_2017F_09May_singlemu" : "/SingleMuon/Run2017F-09May2018-v1/MINIAOD",

        # "data_2017B_muoneg" : "/MuonEG/Run2017B-31Mar2018-v1/MINIAOD",
        # "data_2017C_muoneg" : "/MuonEG/Run2017C-31Mar2018-v1/MINIAOD",
        # "data_2017D_muoneg" : "/MuonEG/Run2017D-31Mar2018-v1/MINIAOD",
        # "data_2017E_muoneg" : "/MuonEG/Run2017E-31Mar2018-v1/MINIAOD",
        # "data_2017F_31Mar_muoneg" : "/MuonEG/Run2017F-31Mar2018-v1/MINIAOD",
        # "data_2017F_09May_muoneg" : "/MuonEG/Run2017F-09May2018-v1/MINIAOD",

        # "data_2017B_jetht" : "/JetHT/Run2017B-31Mar2018-v1/MINIAOD",
        # "data_2017C_jetht" : "/JetHT/Run2017C-31Mar2018-v1/MINIAOD",
        # "data_2017D_jetht" : "/JetHT/Run2017D-31Mar2018-v1/MINIAOD",
        # "data_2017E_jetht" : "/JetHT/Run2017E-31Mar2018-v1/MINIAOD",
        # "data_2017F_31Mar_jetht" : "/JetHT/Run2017F-31Mar2018-v1/MINIAOD",
        # "data_2017F_09May_jetht" : "/JetHT/Run2017F-09May2018-v1/MINIAOD",

        # "data_2017B_photon" : "/SinglePhoton/Run2017B-31Mar2018-v1/MINIAOD",
        # "data_2017C_photon" : "/SinglePhoton/Run2017C-31Mar2018-v1/MINIAOD",
        # "data_2017D_photon" : "/SinglePhoton/Run2017D-31Mar2018-v1/MINIAOD",
        # "data_2017E_photon" : "/SinglePhoton/Run2017E-31Mar2018-v1/MINIAOD",
        # "data_2017F_31Mar_photon" : "/SinglePhoton/Run2017F-31Mar2018-v1/MINIAOD",
        # "data_2017F_09May_photon" : "/SinglePhoton/Run2017F-09May2018-v1/MINIAOD",

        ################################
        # 2017 MC (94X MiniAODv2)
        # Latest cms4: CMS4_V10-02-05

        # # Fastsim signals for 2018
        # "SMS_T2tt_mStop1200to2000_f17v2_ext2" : "/SMS-T2tt_mStop-1200to2000_TuneCP2_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PUFall17Fast_94X_mc2017_realistic_v15_ext1-v1/MINIAODSIM",
        # "SMS_T2tt_mStop400to1200_f17v2_ext2" : "/SMS-T2tt_mStop-400to1200_TuneCP2_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PUFall17Fast_94X_mc2017_realistic_v15_ext1-v1/MINIAODSIM",
        # "SMS_T2tt_mStop350to400_f17v2_ext2" : "/SMS-T2tt_mStop-350to400_TuneCP2_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PUFall17Fast_94X_mc2017_realistic_v15_ext1-v1/MINIAODSIM",
        # "SMS_T2tt_mStop250to350_f17v2_ext2" : "/SMS-T2tt_mStop-250to350_TuneCP2_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PUFall17Fast_94X_mc2017_realistic_v15_ext1-v1/MINIAODSIM",
        # "SMS_T2tt_mStop150to250_f17v2_ext2" : "/SMS-T2tt_mStop-150to250_TuneCP2_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PUFall17Fast_94X_mc2017_realistic_v15_ext1-v1/MINIAODSIM",

        # # Fastsim signals
        # "SMS_T2tt_mStop1200to2000_f17v2_ext0" : "/SMS-T2tt_mStop-1200to2000_TuneCP2_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PUFall17Fast_94X_mc2017_realistic_v15-v1/MINIAODSIM",
        # "SMS_T2tt_mStop400to1200_f17v2_ext0" : "/SMS-T2tt_mStop-400to1200_TuneCP2_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PUFall17Fast_94X_mc2017_realistic_v15-v1/MINIAODSIM",
        # "SMS_T2tt_mStop350to400_f17v2_ext0" : "/SMS-T2tt_mStop-350to400_TuneCP2_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PUFall17Fast_94X_mc2017_realistic_v15-v1/MINIAODSIM",
        # "SMS_T2tt_mStop250to350_f17v2_ext0" : "/SMS-T2tt_mStop-250to350_TuneCP2_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PUFall17Fast_94X_mc2017_realistic_v15-v1/MINIAODSIM",
        # "SMS_T2tt_mStop150to250_f17v2_ext0" : "/SMS-T2tt_mStop-150to250_TuneCP2_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PUFall17Fast_94X_mc2017_realistic_v15-v1/MINIAODSIM",

        # "SMS_T2bW_f17v2" : "/SMS-T2bW_TuneCP2_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PUFall17Fast_94X_mc2017_realistic_v15-v1/MINIAODSIM",
        # "SMS_T2bt_f17v2" : "/SMS-T2bt_TuneCP2_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PUFall17Fast_94X_mc2017_realistic_v15-v1/MINIAODSIM",

        # "SMS_T8bbstausnu_XStau0p75_f17v2" :  "/SMS-T8bbstausnu_mStop-200to1800_XCha0p5_XStau0p75_TuneCP2_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PUFall17Fast_94X_mc2017_realistic_v15-v1/MINIAODSIM",
        # "SMS_T8bbstausnu_XStau0p5_f17v2"  :  "/SMS-T8bbstausnu_mStop-200to1800_XCha0p5_XStau0p5_TuneCP2_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PUFall17Fast_94X_mc2017_realistic_v15-v1/MINIAODSIM",
        # "SMS_T8bbstausnu_XStau0p25_f17v2" :  "/SMS-T8bbstausnu_XCha0p5_mStop-200to1800_XStau0p25_TuneCP2_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PUFall17Fast_94X_mc2017_realistic_v15-v1/MINIAODSIM",

        # ttbar
        # "TTJets_2lep_f17v2"      : "/TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM", # 28'380'110
        # "TTJets_1lep_top_f17v2"  : "/TTJets_SingleLeptFromT_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM", # 61'761'347
        # "TTJets_1lep_tbar_f17v2" : "/TTJets_SingleLeptFromTbar_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM", # 55'703'759

        # # W+jets
        # "W1JetsToLNu_f17v2" : "/W1JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v3/MINIAODSIM",
        # "W2JetsToLNu_f17v2" : "/W2JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v4/MINIAODSIM",
        # "W3JetsToLNu_f17v2" : "/W3JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # "W4JetsToLNu_f17v2" : "/W4JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v2/MINIAODSIM",

        # "W1Jets_NuPt200_f17v2" : "/W1JetsToLNu_NuPt-200_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # "W2Jets_NuPt200_f17v2" : "/W2JetsToLNu_NuPt-200_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # "W3Jets_NuPt200_f17v2" : "/W3JetsToLNu_NuPt-200_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # "W4Jets_NuPt200_f17v2" : "/W4JetsToLNu_NuPt-200_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",

        # # single top
        # "ST_tW_tbar_5f_noHadDecays_f17v2" : "/ST_tW_antitop_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # "ST_tW_top_5f_noHadDecays_f17v2" : "/ST_tW_top_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # "ST_tchan_top_inclDecays_f17v2" : "/ST_t-channel_top_4f_inclusiveDecays_TuneCP5_13TeV-powhegV2-madspin-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # "ST_tchan_tbar_inclDecays_f17v2" : "/ST_t-channel_antitop_4f_inclusiveDecays_TuneCP5_13TeV-powhegV2-madspin-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM",
        # "ST_schan_4f_lepDecays_f17v2" : "/ST_s-channel_4f_leptonDecays_TuneCP5_PSweights_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1/MINIAODSIM",

        # # ttV
        # "TTZToLLNuNu_M10_f17v2" : "/TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # "TTZToQQ_f17v2"         : "/TTZToQQ_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # "TTZToLL_M1to10_f17v2"  : "/TTZToLL_M-1to10_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # "TTWJetsToLNu_f17v2"    : "/TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # "TTWJetsToQQ_f17v2"     : "/TTWJetsToQQ_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # "TTWZ_f17v2"            : "/TTWZ_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",

        # # diboson
        # "WZTo1L3Nu_f17v2" : "/WZTo1L3Nu_13TeV_amcatnloFXFX_madspin_pythia8_v2/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # "WZTo1L1Nu2Q_f17v2" : "/WZTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM",
        # "WZTo2L2Q_f17v2" : "/WZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # "WZTo3LNu_f17v2" : "/WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1/MINIAODSIM",

        # "WWTo2L2Nu_f17v2" : "/WWTo2L2Nu_NNPDF31_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # "WWToLNuQQ_f17v2" : "/WWToLNuQQ_NNPDF31_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # "WWToLNuQQ_f17v2_ext1" : "/WWToLNuQQ_NNPDF31_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/MINIAODSIM",

        # "ZZTo4L_f17v2" : "/ZZTo4L_13TeV_powheg_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # "ZZTo2L2Nu_f17v2" : "/ZZTo2L2Nu_13TeV_powheg_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # "ZZTo2L2Q_f17v2" : "/ZZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # "ZZTo2Q2Nu_f17v2" : "/ZZTo2Q2Nu_TuneCP5_13TeV_amcatnloFXFX_madspin_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",

        # # DY samples 
        # "DYJetsToLL_M50_f17v2" : "/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017RECOSIMstep_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # "DYJetsToLL_M50_f17v2_ext1" : "/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017RECOSIMstep_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/MINIAODSIM",

        # # Full sim single points
        # "T2tt_mStop1200_mLSP100_v17v2" : "/SMS-T2tt_mStop-1200_mLSP-100_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # "T2tt_mStop650_mLSP350_v17v2" : "/SMS-T2tt_mStop-650_mLSP-350_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # "T2tt_mStop850_mLSP100_v17v2" : "/SMS-T2tt_mStop-850_mLSP-100_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",

        # # Fastsim ttbar
        # "TTJets_2lep_f17v2fs" : "/TTJets_DiLept_TuneCP2_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PUFall17Fast_lhe_94X_mc2017_realistic_v15-v1/MINIAODSIM",
        # "TTJets_1lep_top_f17v2fs" : "/TTJets_SingleLeptFromT_TuneCP2_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PUFall17Fast_lhe_94X_mc2017_realistic_v15-v1/MINIAODSIM",
        # "TTJets_1lep_tbar_f17v2fs" : "/TTJets_SingleLeptFromTbar_TuneCP2_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PUFall17Fast_lhe_94X_mc2017_realistic_v15-v1/MINIAODSIM",

        # "TTJets_2lep_a18v1fs" : "/TTJets_DiLept_TuneCP2_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-PUFall18Fast_lhe_102X_upgrade2018_realistic_v15-v2/MINIAODSIM",
        # "TTJets_1lep_top_a18v1fs" : "/TTJets_SingleLeptFromT_TuneCP2_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-PUFall18Fast_lhe_102X_upgrade2018_realistic_v15-v2/MINIAODSIM",
        # "TTJets_1lep_tbar_a18v1fs" : "/TTJets_SingleLeptFromTbar_TuneCP2_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-PUFall18Fast_lhe_102X_upgrade2018_realistic_v15-v2/MINIAODSIM",

        # "TTJets_2lep_s16v3fs" : "/TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUSummer16v3Fast_lhe_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM",
        # "TTJets_1lep_top_s16v3fs" : "/TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUSummer16v3Fast_lhe_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM",
        # "TTJets_1lep_tbar_s16v3fs" : "/TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUSummer16v3Fast_lhe_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM",

        # # # extra samples
        # # "ttZJets_f17v2" : "/ttZJets_TuneCP5_13TeV_madgraphMLM_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # # "ttWJets_f17v2" : "/ttWJets_TuneCP5_13TeV_madgraphMLM_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # # "WZ_incl_f17v2" : "/WZ_TuneCP5_13TeV-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",

        # "TTJets_amcnlo_f17v2" : "/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM", # 153'531'390
        # "TTTo2L2Nu_f17v2" : "/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1/MINIAODSIM", # 9'000'000,
        # "TTTo2L2Nu_f17v2" : "/TTTo2L2Nu_TuneCP5_PSweights_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v2/MINIAODSIM", # 69'155'808
        # "WJetsToLNu_incl_f17v2" : "/WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v2/MINIAODSIM",
        # "WJetsToLNu_HT2500ToInf_f17v2"  : "/WJetsToLNu_HT-2500ToInf_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v3/MINIAODSIM",
        # "WJetsToLNu_HT1200To2500_f17v2" : "/WJetsToLNu_HT-1200To2500_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # "WJetsToLNu_HT800To1200_f17v2"  : "/WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # "WJetsToLNu_HT600To800_f17v2"   : "/WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # "WJetsToLNu_HT400To600_f17v2"   : "/WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # "WJetsToLNu_HT200To400_f17v2"   : "/WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # "WJetsToLNu_HT100To200_f17v2"   : "/WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM",

        # # ttbar+gamma
        # "TTGamma_1lep_top_f17v2" : "/TTGamma_SingleLeptFromT_TuneCP5_PSweights_13TeV_madgraph_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # "TTGamma_1lep_tbar_f17v2" : "/TTGamma_SingleLeptFromTbar_TuneCP5_PSweights_13TeV_madgraph_pythia8/RunIIFall17MiniAOD-PU2017_94X_mc2017_realistic_v11-v1/MINIAODSIM",
        # "TTGamma_2lep_f17v2" : "/TTGamma_Dilept_TuneCP5_PSweights_13TeV_madgraph_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # "TTGJets_incl_f17v2" : "/TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1/MINIAODSIM",

        # # gamma+jets
        # "GJets_HT40to100_f17v2"  : "/GJets_HT-40To100_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_1core_94X_mc2017_realistic_v14_ext1-v1/MINIAODSIM",
        # "GJets_HT100to200_f17v2" : "/GJets_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # "GJets_HT200to400_f17v2" : "/GJets_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # "GJets_HT400to600_f17v2" : "/GJets_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # "GJets_HT600toInf_f17v2" : "/GJets_HT-600ToInf_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",

        # Extra samples
        # "W4JetsToLNu_f17v2_oldpmx" : "/W4JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # "ST_tW_tbar_5f_noHadDecays_f17v2_oldpmx" : "/ST_tW_antitop_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # "ST_tW_top_5f_noHadDecays_f17v2_oldpmx" : "/ST_tW_top_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # "ST_tchan_tbar_4f_inclDecays_f17v2_oldpmx" : "/ST_t-channel_antitop_4f_inclusiveDecays_TuneCP5_13TeV-powhegV2-madspin-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # "WZTo3LNu_f17v2_oldpmx" : "/WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # "WWTo1L1Nu2Q_f17v2" : "/WWTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM", # faulty

        ################################
        # 2016 Legacy ReReco (94X)
        # Latest cms4: CMS4_V09-04-17

        # # "data_2016B_met_v1" : "/MET/Run2016B-17Jul2018_ver1-v1/MINIAOD",
        # "data_2016B_met" : "/MET/Run2016B-17Jul2018_ver2-v1/MINIAOD",
        # "data_2016C_met" : "/MET/Run2016C-17Jul2018-v1/MINIAOD",
        # "data_2016D_met" : "/MET/Run2016D-17Jul2018-v1/MINIAOD",
        # "data_2016E_met" : "/MET/Run2016E-17Jul2018-v1/MINIAOD",
        # "data_2016F_met" : "/MET/Run2016F-17Jul2018-v1/MINIAOD",
        # "data_2016G_met" : "/MET/Run2016G-17Jul2018-v1/MINIAOD",
        # "data_2016H_met" : "/MET/Run2016H-17Jul2018-v2/MINIAOD",

        # # "data_2016B_singleel_v1" : "/SingleElectron/Run2016B-17Jul2018_ver1-v1/MINIAOD",
        # "data_2016B_singleel" : "/SingleElectron/Run2016B-17Jul2018_ver2-v1/MINIAOD",
        # "data_2016C_singleel" : "/SingleElectron/Run2016C-17Jul2018-v1/MINIAOD",
        # "data_2016D_singleel" : "/SingleElectron/Run2016D-17Jul2018-v1/MINIAOD",
        # "data_2016E_singleel" : "/SingleElectron/Run2016E-17Jul2018-v1/MINIAOD",
        # "data_2016F_singleel" : "/SingleElectron/Run2016F-17Jul2018-v1/MINIAOD",
        # "data_2016G_singleel" : "/SingleElectron/Run2016G-17Jul2018-v1/MINIAOD",
        # "data_2016H_singleel" : "/SingleElectron/Run2016H-17Jul2018-v1/MINIAOD",

        # # "data_2016B_singlemu_v1" : "/SingleMuon/Run2016B-17Jul2018_ver1-v1/MINIAOD",
        # "data_2016B_singlemu" : "/SingleMuon/Run2016B-17Jul2018_ver2-v1/MINIAOD",
        # "data_2016C_singlemu" : "/SingleMuon/Run2016C-17Jul2018-v1/MINIAOD",
        # "data_2016D_singlemu" : "/SingleMuon/Run2016D-17Jul2018-v1/MINIAOD",
        # "data_2016E_singlemu" : "/SingleMuon/Run2016E-17Jul2018-v1/MINIAOD",
        # "data_2016F_singlemu" : "/SingleMuon/Run2016F-17Jul2018-v1/MINIAOD",
        # "data_2016G_singlemu" : "/SingleMuon/Run2016G-17Jul2018-v1/MINIAOD",
        # "data_2016H_singlemu" : "/SingleMuon/Run2016H-17Jul2018-v1/MINIAOD",

        # # "data_2016B_muoneg_v1" : "/MuonEG/Run2016B-17Jul2018_ver1-v1/MINIAOD",
        # "data_2016B_muoneg" : "/MuonEG/Run2016B-17Jul2018_ver2-v1/MINIAOD",
        # "data_2016C_muoneg" : "/MuonEG/Run2016C-17Jul2018-v1/MINIAOD",
        # "data_2016D_muoneg" : "/MuonEG/Run2016D-17Jul2018-v1/MINIAOD",
        # "data_2016E_muoneg" : "/MuonEG/Run2016E-17Jul2018-v2/MINIAOD",
        # "data_2016F_muoneg" : "/MuonEG/Run2016F-17Jul2018-v1/MINIAOD",
        # "data_2016G_muoneg" : "/MuonEG/Run2016G-17Jul2018-v1/MINIAOD",
        # "data_2016H_muoneg" : "/MuonEG/Run2016H-17Jul2018-v1/MINIAOD",

        # # "data_2016B_jetht_v1" : "/JetHT/Run2016B-17Jul2018_ver1-v1/MINIAOD",
        # "data_2016B_jetht" : "/JetHT/Run2016B-17Jul2018_ver2-v2/MINIAOD",
        # "data_2016C_jetht" : "/JetHT/Run2016C-17Jul2018-v1/MINIAOD",
        # "data_2016D_jetht" : "/JetHT/Run2016D-17Jul2018-v1/MINIAOD",
        # "data_2016E_jetht" : "/JetHT/Run2016E-17Jul2018-v1/MINIAOD",
        # "data_2016F_jetht" : "/JetHT/Run2016F-17Jul2018-v1/MINIAOD",
        # "data_2016G_jetht" : "/JetHT/Run2016G-17Jul2018-v1/MINIAOD",
        # "data_2016H_jetht" : "/JetHT/Run2016H-17Jul2018-v1/MINIAOD",

        # # "data_2016B_photon_v1" : "/SinglePhoton/Run2016B-17Jul2018_ver1-v1/MINIAOD",
        # "data_2016B_photon" : "/SinglePhoton/Run2016B-17Jul2018_ver2-v2/MINIAOD",
        # "data_2016C_photon" : "/SinglePhoton/Run2016C-17Jul2018-v1/MINIAOD",
        # "data_2016D_photon" : "/SinglePhoton/Run2016D-17Jul2018-v1/MINIAOD",
        # "data_2016E_photon" : "/SinglePhoton/Run2016E-17Jul2018-v1/MINIAOD",
        # "data_2016F_photon" : "/SinglePhoton/Run2016F-17Jul2018-v1/MINIAOD",
        # "data_2016G_photon" : "/SinglePhoton/Run2016G-17Jul2018-v1/MINIAOD",
        # "data_2016H_photon" : "/SinglePhoton/Run2016H-17Jul2018-v1/MINIAOD",

        ################################
        # 2016 MC (94X MiniAODv3)
        # Latest cms4: CMS4_V09-04-17

        # # Fastsim signals
        # "SMS_T2tt_mStop1200to2000_s16v3" : "/SMS-T2tt_mStop-1200to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUSummer16v3Fast_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM",
        # "SMS_T2tt_mStop400to1200_s16v3" : "/SMS-T2tt_mStop-400to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUSummer16v3Fast_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM",
        # "SMS_T2tt_mStop350to400_s16v3" : "/SMS-T2tt_mStop-350to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUSummer16v3Fast_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM",
        # "SMS_T2tt_mStop250to350_s16v3" : "/SMS-T2tt_mStop-250to350_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUSummer16v3Fast_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM",
        # "SMS_T2tt_mStop150to250_s16v3" : "/SMS-T2tt_mStop-150to250_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUSummer16v3Fast_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM",

        # "SMS_T2bW_s16v3" : "/SMS-T2bW_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUSummer16v3Fast_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM",
        # "SMS_T2bt_s16v3" : "/SMS-T2bt_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUSummer16v3Fast_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM",

        # "SMS_T8bbstausnu_XStau0p75_s16v3" : "/SMS-T8bbstausnu_mStop-200to1800_XCha0p5_XStau0p75_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUSummer16v3Fast_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM",
        # "SMS_T8bbstausnu_XStau0p5_s16v3"  : "/SMS-T8bbstausnu_mStop-200to1800_XCha0p5_XStau0p5_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUSummer16v3Fast_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM",
        # "SMS_T8bbstausnu_XStau0p25_s16v3" : "/SMS-T8bbstausnu_XCha0p5_mStop-200to1800_XStau0p25_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUSummer16v3Fast_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM",

        # ttbar
        # "TTJets_2lep_s16v3_ext0"      : "/TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM",
        # "TTJets_2lep_s16v3_ext1"      : "/TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2/MINIAODSIM",
        # "TTJets_1lep_top_s16v3_ext0"  : "/TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM",
        # "TTJets_1lep_top_s16v3_ext1"  : "/TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2/MINIAODSIM",
        # "TTJets_1lep_tbar_s16v3_ext0" : "/TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM",
        # "TTJets_1lep_tbar_s16v3_ext1" : "/TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2/MINIAODSIM",
        # "/TTJets_Dilept_TuneCUETP8M2T4_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM",
        # "/TTJets_TuneCUETP8M2T4_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM",

        # # W+jets
        # "W1JetsToLNu_s16v3" : "/W1JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM",
        # "W2JetsToLNu_s16v3" : "/W2JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2/MINIAODSIM",
        # "W3JetsToLNu_s16v3" : "/W3JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2/MINIAODSIM",
        # "W4JetsToLNu_s16v3" : "/W4JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext2-v1/MINIAODSIM",
        # "W1Jets_NuPt200_s16v3" : "/W1JetsToLNu_NuPt-200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM",
        # "W2Jets_NuPt200_s16v3" : "/W2JetsToLNu_NuPt-200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM",
        # "W3Jets_NuPt200_s16v3" : "/W3JetsToLNu_NuPt-200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM",
        # "W4Jets_NuPt200_s16v3" : "/W4JetsToLNu_NuPt-200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_backup_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM",

        # # single top
        # "ST_tW_tbar_5f_noHadDecays_s16v3"   : "/ST_tW_antitop_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2/MINIAODSIM",
        # "ST_tW_top_5f_noHadDecays_s16v3"    : "/ST_tW_top_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2/MINIAODSIM",
        # "ST_tchan_tbar_4f_inclDecays_s16v3" : "/ST_t-channel_antitop_4f_inclusiveDecays_13TeV-powhegV2-madspin-pythia8_TuneCUETP8M1/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM",
        # "ST_tchan_top_4f_inclDecays_s16v3"  : "/ST_t-channel_top_4f_inclusiveDecays_13TeV-powhegV2-madspin-pythia8_TuneCUETP8M1/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM",
        # "ST_schan_lepDecays_s16v3"          : "/ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM",

        # # ttV
        # "TTWJetsToLNu_s16v3" : "/TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext2-v1/MINIAODSIM",
        # "TTWJetsToQQ_s16v3"  : "/TTWJetsToQQ_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM",
        # "TTZToLLNuNu_s16v3"  : "/TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext2-v1/MINIAODSIM",
        # "TTZToQQ_s16v3"      : "/TTZToQQ_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM",

        # # Diboson
        # "WWTo2L2Nu_s16v3" : "/WWTo2L2Nu_13TeV-powheg/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM",
        # "WWToLNuQQ_s16v3" : "/WWToLNuQQ_13TeV-powheg/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM",
        # # "WWTo1L1Nu2Q_s16v3" : "/WWTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM",

        # "WZTo1L3Nu_s16v3" : "/WZTo1L3Nu_13TeV_amcatnloFXFX_madspin_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM",
        # "WZTo1L1Nu2Q_s16v3" : "/WZTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM",
        # "WZTo2L2Q_s16v3" : "/WZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM",
        # # "WZTo3LNu_powheg_s16v3" : "/WZTo3LNu_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v1/MINIAODSIM",
        # "WZTo3LNu_amcnlo_s16v3" : "/WZTo3LNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM",

        # "ZZTo4L_s16v3"    : "/ZZTo4L_13TeV_powheg_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM",
        # "ZZTo2L2Nu_s16v3" : "/ZZTo2L2Nu_13TeV_powheg_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM",
        # "ZZTo2L2Q_s16v3"  : "/ZZTo2L2Q_13TeV_powheg_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM",
        # "ZZTo2Q2Nu_s16v3" : "/ZZTo2Q2Nu_13TeV_amcatnloFXFX_madspin_pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM",

        # # DY samples (for CRemu)
        # "DYJetsToLL_M50_s16v3_ext1" : "/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2/MINIAODSIM",
        # "DYJetsToLL_M50_s16v3_ext2" : "/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext2-v2/MINIAODSIM",
        # "DYJetsToLL_Zpt150_s16v3" : "/DYJetsToLL_M-50_Zpt-150toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM",

        # # TTG samples
        # "TTGJets_s16v3" : "/TTGJets_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2/MINIAODSIM",
        # "TTGamma_1lep_tbar_s16v3" : "/TTGamma_SingleLeptFromTbar_TuneCUETP8M2T4_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM",

        # Extra samples
        # "TTTo2L2Nu_s16v3" : "/TTTo2L2Nu_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM",
        # "WWTo2L2Nu_doublescattering_s16v3" : "/WWTo2L2Nu_DoubleScattering_13TeV-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM",
        # "WWW_4f_s16v3" : "/WWW_4F_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM",
        # "WWZ_s16v3" : "/WWZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM",
        # "WW_s16v3" : "/WW_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2/MINIAODSIM",
        # "WZZ_incl_s16v3" : "/WZZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM",

        ################################
        # 2016 MC (80X MiniAODv2)  <-- obsolete now since we have most of 94X MiniAODv3 already
        # Latest cms4: CMS4_V08-00-06

        # # Fastsim signals
        # "SMS_T2tt_400to1200_80X"  : "/SMS-T2tt_mStop-400to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM",
        # "SMS_T2tt_1200to1500_80X" : "/SMS-T2tt_mStop-1200to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM",
        # "SMS_T2bW_80X"   : "/SMS-T2bW_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM",
        # "SMS_T2bt_80X"   : "/SMS-T2bt_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM",
        # "SMS_TChiWH_80X" : "/SMS-TChiWH_WToLNu_HToBB_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUSummer16Fast_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",

        # # ttbar
        # "TTJets_2lep_80X_ext1" : "/TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM",
        # "TTJets_1lep_top_80X_ext1" : "/TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM",
        # "TTJets_1lep_tbar_80X_ext1" : "/TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM",
        # "TTJets_2lep_madgraph_80X" : "/TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
        # "TTJets_1lep_tbar_madgraph_80X" : "/TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",

        # # W+jets
        # "W1JetsToLNu_80X" : "/W1JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
        # "W2JetsToLNu_80X" : "/W2JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
        # "W3JetsToLNu_80X" : "/W3JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM",
        # "W4JetsToLNu_80X" : "/W4JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1/MINIAODSIM",
        # "W1Jets_NuPt200_80X" : "/W1JetsToLNu_NuPt-200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
        # "W2Jets_NuPt200_80X" : "/W2JetsToLNu_NuPt-200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
        # "W3Jets_NuPt200_80X" : "/W3JetsToLNu_NuPt-200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
        # "W4Jets_NuPt200_80X" : "/W4JetsToLNu_NuPt-200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_backup_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM",

        # # single t
        # "ST_tW_top_5f_noHadDecays_80X" : "/ST_tW_top_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM",
        # "ST_tW_antitop_5f_noHadDecays_80X" : "/ST_tW_antitop_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM",

        # # ttV
        # "TTZToLLNuNu_M10_80X" : "/TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1/MINIAODSIM",
        # "TTWJetsToLNu_80X" : "/TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v3/MINIAODSIM",
        # # "/ttWJets_13TeV_madgraphMLM/RunIISummer16MiniAODv2-80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
        # # "/ttZJets_13TeV_madgraphMLM/RunIISummer16MiniAODv2-80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",

        # # diBoson
        # "WZTo1L3Nu_80X" : "/WZTo1L3Nu_13TeV_amcatnloFXFX_madspin_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",

        # # gamma+jets
        # "GJets_HT40to100"  : "/GJets_HT-40To100_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_1core_94X_mc2017_realistic_v14_ext1-v1/MINIAODSIM",
        # "GJets_HT100to200" : "/GJets_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # "GJets_HT200to400" : "/GJets_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # # "GJets_HT400to600" : "/GJets_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # "GJets_HT400to600" : "/GJets_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAOD-94X_mc2017_realistic_v10-v2/MINIAODSIM",
        # "GJets_HT600toInf" : "/GJets_HT-600ToInf_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",

        # # Extra samples for WH
        # "SMS_TChiWH_s16v3" : "/SMS-TChiWH_WToLNu_HToBB_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUSummer16v3Fast_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM",
        # "SMS_TChiWH_f17v2" : "/SMS-TChiWH_WToLNu_HToBB_TuneCP2_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PUFall17Fast_94X_mc2017_realistic_v15-v1/MINIAODSIM",
        # "SMS_TChiWH_a18v1" : "/SMS-TChiWH_WToLNu_HToBB_TuneCP2_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-PUFall18Fast_102X_upgrade2018_realistic_v15-v1/MINIAODSIM",

        # "WJets_0J_amcnlo_f17v2_ext0" : "/WJetsToLNu_0J_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM",
        # "WJets_1J_amcnlo_f17v2_ext0" : "/WJetsToLNu_1J_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # "WJets_1J_amcnlo_f17v2_ext1" : "/WJetsToLNu_1J_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/MINIAODSIM",
        # "WJets_2J_amcnlo_f17v2_ext0" : "/WJetsToLNu_2J_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # # "WJets_2J_amcnlo_f17v2_ext1" : "/WJetsToLNu_2J_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v2/MINIAODSIM",

        # "WJets_0J_amcnlo_a18v1" : "/WJetsToLNu_0J_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM",
        # "WJets_1J_amcnlo_a18v1" : "/WJetsToLNu_1J_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM",
        # "WJets_2J_amcnlo_a18v1" : "/WJetsToLNu_2J_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM",

        # "TTJets_2lep_met150_s16v3"      : "/TTJets_DiLept_genMET-150_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM",
        # "TTJets_1lep_top_met150_s16v3"  : "/TTJets_SingleLeptFromT_genMET-150_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM",
        # "TTJets_1lep_tbar_met150_s16v3" : "/TTJets_SingleLeptFromTbar_genMET-150_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM",

        # "TTJets_2lep_met150_f17v2"      : "/TTJets_DiLept_genMET-150_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # "TTJets_1lep_top_met150_f17v2"  : "/TTJets_SingleLeptFromTbar_genMET-150_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM",
        # "TTJets_1lep_tbar_met150_f17v2" : "/TTJets_SingleLeptFromT_genMET-150_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",

        # "TTJets_1lep_tbar_met80_a18v1" : "/TTJets_SingleLeptFromTbar_genMET-80_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM",
        # "TTJets_1lep_top_met80_a18v1"  : "/TTJets_SingleLeptFromT_genMET-80_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM",
        # "TTJets_2lep_met80_a18v1"      : "/TTJets_DiLept_genMET-80_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM",

    }

    # Make a directory sample, giving it the location and a dataset name for bookkeeping purposes
    # The globber must be customized (by default, it is *.root) in order to pick up the text files

    # Make a CondorTask (3 in total, one for each input)
    maker_tasks = []
    merge_tasks = []

    for babyname, dsname in snt_samples.items():
        cmsswver = "CMSSW_10_1_0"
        scramarch = "slc6_amd64_gcc700"
        tag = "v32_0"
        # if "a18v1" in babyname: tag = "v31_8"
        tarfile = "tarfiles/input_"+tag+".tar.gz"
        cms3tag = "CMS4_V08-00-06" if "80X" in dsname else None
        excltag = "CMS4_V09-04-18*"
        samptyp = "1" if "SMS" in babyname or "Fast" in dsname else "2" if "/TTG" in dsname else "0"
        extrarg = ""
        if "TTJets" in babyname and "met150" in babyname: 
            tag = "v30_9"
            tarfile = "tarfiles/input_"+tag+".tar.gz"
        elif "WJets" in babyname and "amcnlo" in babyname: 
            tag = "v30_9"
            tarfile = "tarfiles/input_"+tag+".tar.gz"
        elif "TTJets" in babyname: 
            tag = "v31_9"
            tarfile = "tarfiles/input_"+tag+".tar.gz"
            # extrarg = " skim_nJets=1,skim_met=250"
        # if samptyp == "1" and "f17v2_ext1" in babyname: extrarg = " applyMETRecipeV2=false"
        if "/GJets" in dsname or "Photon" in dsname:
            samptyp = "2"
            extrarg = " skim_nGoodLep=0,skim_met_emuEvt=150"
        if "/DYJetsToLL" in dsname and tag == "v32_0":
            tag = "v32_0z"
            extrarg = " skim_nGoodLep=2,skim_met_ZllEvt=0"
        if "TTTo2L2Nu" in dsname:
            extrarg = " skim_nGoodLep=2,skim_met=250" # only for emu region
        # extrarg = " topcandTree=topcands" if "TTJets" in dsname else ""
        # renew = True if 'TTJets' in babyname else False
        renew = True
        minfrac = 0.97 if 'SMS' in babyname else 0.98 if '1J' in babyname else 1
        # minfrac = 1 if 'SMS' in babyname else 0.93 if '1J' in babyname else 0.75
        maker_task = CondorTask(
            sample = SNTSample(dataset=dsname, tag=cms3tag, exclude_tag_pattern=excltag),
            files_per_output = getNfilesPerJob(babyname),
            tag = tag,
            outdir_name = "stopbaby_" + babyname,
            output_name = "stopbaby.root",
            executable = "condor_executable.sh",
            cmssw_version = cmsswver,
            scram_arch = scramarch,
            arguments = samptyp + extrarg,
            tarfile = tarfile,
            # condor_submit_params = {"sites": "T2_US_UCSD"},
            min_completion_fraction = minfrac,
            # condor_submit_params = {"sites": "UAF"},
            condor_submit_params = {"use_xrootd": True},
            # max_jobs = 1,      # temporary for submission test
            # no_load_from_backup = renew,
            # recopy_inputs = True,
        )
        merge_task = CondorTask(
            sample = DirectorySample(
                dataset="merge_"+babyname,
                location=maker_task.get_outputdir(),
            ),
            # open_dataset = True, flush = True,
            executable = "merge_executable.sh",
            tarfile = "merge_scripts.tar.gz",
            files_per_output = 100000,
            output_dir = maker_task.get_outputdir() + "/merged",
            output_name = babyname + ".root",
            output_is_tree = True,
            # check_expectedevents = True,
            tag = tag,
            # condor_submit_params = {"sites":"T2_US_UCSD"},
            # cmssw_version = "CMSSW_9_2_8",
            # scram_arch = "slc6_amd64_gcc530",
            condor_submit_params = {"sites":"T2_US_UCSD,UAF"},
            cmssw_version = "CMSSW_10_2_14",
            scram_arch = "slc6_amd64_gcc700",
            # recopy_inputs = True,
            # no_load_from_backup = True,
            min_completion_fraction = minfrac,
        )
        maker_tasks.append(maker_task)
        merge_tasks.append(merge_task)
        # tasks_progs.append(0)

    for i in range(100):
        total_summary = {}

        for maker_task, merge_task in zip(maker_tasks,merge_tasks):
            maker_task.process()

            frac = maker_task.complete(return_fraction=True)
            if frac >= merge_task.min_completion_fraction:
            # if maker_task.complete():
                do_cmd("mkdir -p {}/merged".format(maker_task.get_outputdir()))
                do_cmd("mkdir -p {}/skimmed".format(maker_task.get_outputdir()))
                merge_task.reset_io_mapping()
                merge_task.update_mapping()
                merge_task.process()

            total_summary[maker_task.get_sample().get_datasetname()] = maker_task.get_task_summary()
            total_summary[merge_task.get_sample().get_datasetname()] = merge_task.get_task_summary()

        # parse the total summary and write out the dashboard
        StatsParser(data=total_summary, webdir="~/public_html/dump/metis_stopbaby/").do()

        # 1 hr power nap
        time.sleep(15.*60)

    # If it's complete, make a dummy sample out of the output directory
    # in order to pick up the files. Then cat out the contents and sum
    # them up. This should be 3*2*10 = 100
