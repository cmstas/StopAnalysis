from __future__ import print_function

import time

from metis.Sample import SNTSample
from metis.Sample import DirectorySample
from metis.CondorTask import CondorTask
from metis.StatsParser import StatsParser
from metis.Utils import do_cmd

if __name__ == "__main__":

    snt_samples = {
        # "stopbaby" : "/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/RunIISummer17MiniAOD-92X_upgrade2017_realistic_v10_ext1-v1/MINIAODSIM",
        # "stopbaby" : "/TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer17MiniAOD-92X_upgrade2017_realistic_v10-v3/MINIAODSIM",
        # "stopbaby" : "/SMS-T8bbstausnu_XCha0p5_XStau0p25_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUSummer16Fast_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
        # "stopbaby" : "/SMS-T8bbstausnu_XCha0p5_XStau0p5_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUSummer16Fast_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
        # "stopbaby" : "/SMS-T8bbstausnu_XCha0p5_XStau0p75_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUSummer16Fast_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",

        ################################
        # 2017 ReReco (94X)
        # Latest cms4: CMS4_V09-04-19

        # "data_2017B_met" : "/MET/Run2017B-31Mar2018-v1/MINIAOD",
        # "data_2017C_met" : "/MET/Run2017C-31Mar2018-v1/MINIAOD",
        # "data_2017E_met" : "/MET/Run2017E-31Mar2018-v1/MINIAOD",
        # "data_2017D_met" : "/MET/Run2017D-31Mar2018-v1/MINIAOD",
        # "data_2017F_met" : "/MET/Run2017F-31Mar2018-v1/MINIAOD",

        # "data_2017B_singleel" : "/SingleElectron/Run2017B-31Mar2018-v1/MINIAOD",
        # "data_2017C_singleel" : "/SingleElectron/Run2017C-31Mar2018-v1/MINIAOD",
        # "data_2017D_singleel" : "/SingleElectron/Run2017D-31Mar2018-v1/MINIAOD",
        # "data_2017E_singleel" : "/SingleElectron/Run2017E-31Mar2018-v1/MINIAOD",
        # "data_2017F_singleel" : "/SingleElectron/Run2017F-31Mar2018-v1/MINIAOD",

        # "data_2017B_singlemu" : "/SingleMuon/Run2017B-31Mar2018-v1/MINIAOD",
        # "data_2017C_singlemu" : "/SingleMuon/Run2017C-31Mar2018-v1/MINIAOD",
        # "data_2017D_singlemu" : "/SingleMuon/Run2017D-31Mar2018-v1/MINIAOD",
        # "data_2017E_singlemu" : "/SingleMuon/Run2017E-31Mar2018-v1/MINIAOD",
        # "data_2017F_singlemu" : "/SingleMuon/Run2017F-31Mar2018-v1/MINIAOD",

        # "data_2017B_jetht" : "/JetHT/Run2017B-31Mar2018-v1/MINIAOD",
        # "data_2017C_jetht" : "/JetHT/Run2017C-31Mar2018-v1/MINIAOD",
        # "data_2017D_jetht" : "/JetHT/Run2017D-31Mar2018-v1/MINIAOD",
        # "data_2017E_jetht" : "/JetHT/Run2017E-31Mar2018-v1/MINIAOD",
        # "data_2017F_jetht" : "/JetHT/Run2017F-31Mar2018-v1/MINIAOD",

        ################################
        # 2017 MC (94X MiniAODv2)
        # Latest cms4: CMS4_V09-04-19

        # # "TTJets_incl_amcnlo" : "/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM", # 153'531'390
        # "TTJets_2lep_madgraph" : "/TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM", # 28'380'110
        # "TTJets_1lep_t_madgraph" : "/TTJets_SingleLeptFromT_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM", # 61'761'347
        # "TTJets_1lep_tbar_madgraph" : "/TTJets_SingleLeptFromTbar_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM", # 55'703'759

        # "W1JetsToLNu_madgraph" : "/W1JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v3/MINIAODSIM",
        # "W2JetsToLNu_madgraph" : "/W2JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v4/MINIAODSIM",
        # "W3JetsToLNu_madgraph" : "/W3JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # "W4JetsToLNu_madgraph" : "/W4JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",

        # "ST_tW_antitop_5f_noHadDecays_powheg" : "/ST_tW_antitop_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # "ST_tW_top_5f_noHadDecays_powheg" : "/ST_tW_top_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # "ST_schan_4f_lepDecays_amcnlo" : "/ST_s-channel_4f_leptonDecays_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",

        # "TTZToLLNuNu_M10_amcnlo" : "/TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # "TTWJetsToLNu_amcnlo" : "/TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # "WZTo3LNu_amcnlo" : "/WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",

        # "ttwJets_madgraph" : "/ttWJets_TuneCP5_13TeV_madgraphMLM_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
        # "WZ_incl" : "/WZ_TuneCP5_13TeV-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",

        ################################
        # 2016 Legacy ReReco (94X)
        # Latest cms4: CMS4_V09-04-17

        # "data_2016B_met_v1" : "/MET/Run2016B-17Jul2018_ver1-v1/MINIAOD",
        # "data_2016B_met_v2" : "/MET/Run2016B-17Jul2018_ver2-v1/MINIAOD",
        # "data_2016C_met" : "/MET/Run2016C-17Jul2018-v1/MINIAOD",
        # "data_2016D_met" : "/MET/Run2016D-17Jul2018-v1/MINIAOD",
        # "data_2016E_met" : "/MET/Run2016E-17Jul2018-v1/MINIAOD",
        # "data_2016F_met" : "/MET/Run2016F-17Jul2018-v1/MINIAOD",
        # "data_2016G_met" : "/MET/Run2016G-17Jul2018-v1/MINIAOD",
        # "data_2016H_met" : "/MET/Run2016H-17Jul2018-v2/MINIAOD",

        # "data_2016B_singleel_v1" : "/SingleElectron/Run2016B-17Jul2018_ver1-v1/MINIAOD",
        # "data_2016B_singleel_v2" : "/SingleElectron/Run2016B-17Jul2018_ver2-v1/MINIAOD",
        # "data_2016C_singleel" : "/SingleElectron/Run2016C-17Jul2018-v1/MINIAOD",
        # "data_2016D_singleel" : "/SingleElectron/Run2016D-17Jul2018-v1/MINIAOD",
        # "data_2016E_singleel" : "/SingleElectron/Run2016E-17Jul2018-v1/MINIAOD",
        # "data_2016F_singleel" : "/SingleElectron/Run2016F-17Jul2018-v1/MINIAOD",
        # "data_2016G_singleel" : "/SingleElectron/Run2016G-17Jul2018-v1/MINIAOD",
        # "data_2016H_singleel" : "/SingleElectron/Run2016H-17Jul2018-v1/MINIAOD",

        # "data_2016B_singlemu_v1" : "/SingleMuon/Run2016B-17Jul2018_ver1-v1/MINIAOD",
        # "data_2016B_singlemu_v2" : "/SingleMuon/Run2016B-17Jul2018_ver2-v1/MINIAOD",
        # "data_2016C_singlemu" : "/SingleMuon/Run2016C-17Jul2018-v1/MINIAOD",
        # "data_2016D_singlemu" : "/SingleMuon/Run2016D-17Jul2018-v1/MINIAOD",
        # "data_2016E_singlemu" : "/SingleMuon/Run2016E-17Jul2018-v1/MINIAOD",
        # "data_2016F_singlemu" : "/SingleMuon/Run2016F-17Jul2018-v1/MINIAOD",
        # "data_2016G_singlemu" : "/SingleMuon/Run2016G-17Jul2018-v1/MINIAOD",
        # "data_2016H_singlemu" : "/SingleMuon/Run2016H-17Jul2018-v1/MINIAOD",

        # "data_2016B_jetht" : "/JetHT/Run2016B-17Jul2018_ver1-v1/MINIAOD",
        # "data_2016B_jetht" : "/JetHT/Run2016B-17Jul2018_ver2-v2/MINIAOD",
        # "data_2016C_jetht" : "/JetHT/Run2016C-17Jul2018-v1/MINIAOD",
        # "data_2016D_jetht" : "/JetHT/Run2016D-17Jul2018-v1/MINIAOD",
        # "data_2016E_jetht" : "/JetHT/Run2016E-17Jul2018-v1/MINIAOD",
        # "data_2016F_jetht" : "/JetHT/Run2016F-17Jul2018-v1/MINIAOD",
        # "data_2016G_jetht" : "/JetHT/Run2016G-17Jul2018-v1/MINIAOD",
        # "data_2016H_jetht" : "/JetHT/Run2016H-17Jul2018-v1/MINIAOD",

        ################################
        # 2016 MC (94X MiniAODv3)
        # Latest cms4: CMS4_V09-04-17

        ################################
        # 2016 MC (80X MiniAODv3)
        # Latest cms4: CMS4_V08-00-06

        # # Fastsim signals
        # "SMS_T2tt_400to1200_80X"  : "/SMS-T2tt_mStop-400to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM",
        # "SMS_T2tt_1200to1500_80X" : "/SMS-T2tt_mStop-1200to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM",
        # "SMS_T2bW_80X"   : "/SMS-T2bW_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM",
        # "SMS_T2bt_80X"   : "/SMS-T2bt_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM",
        # "SMS_TChiWH_80X" : "/SMS-TChiWH_WToLNu_HToBB_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUSummer16Fast_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",

        # ttbar
        "TTJets_2lep_80X_ext1" : "/TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM",
        "TTJets_1lep_t_80X_ext1" : "/TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM",
        "TTJets_1lep_tbar_80X_ext1" : "/TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM",
        # "TTJets_2lep_madgraph_80X" : "/TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
        # "TTJets_1lep_tbar_madgraph_80X" : "/TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",

        # WJets
        "W1JetsToLNu_80X" : "/W1JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
        "W2JetsToLNu_80X" : "/W2JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
        "W3JetsToLNu_80X" : "/W3JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM",
        "W4JetsToLNu_80X" : "/W4JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1/MINIAODSIM",
        "W1Jets_NuPt200_80X" : "/W1JetsToLNu_NuPt-200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
        "W2Jets_NuPt200_80X" : "/W2JetsToLNu_NuPt-200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
        "W3Jets_NuPt200_80X" : "/W3JetsToLNu_NuPt-200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
        "W4Jets_NuPt200_80X" : "/W4JetsToLNu_NuPt-200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_backup_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM",

        # single t
        "ST_tW_top_5f_noHadDecays_80X" : "/ST_tW_top_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM",
        "ST_tW_antitop_5f_noHadDecays_80X" : "/ST_tW_antitop_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM",

        # ttV
        "TTZToLLNuNu_M10_80X" : "/TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1/MINIAODSIM",
        "TTWJetsToLNu_80X" : "/TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v3/MINIAODSIM",
        # "/ttWJets_13TeV_madgraphMLM/RunIISummer16MiniAODv2-80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
        # "/ttZJets_13TeV_madgraphMLM/RunIISummer16MiniAODv2-80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",

        # diBoson
        "WZTo1L3Nu_80X" : "/WZTo1L3Nu_13TeV_amcatnloFXFX_madspin_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",


        ################################
        # 2018 Data PromptReco
        # Latest cms4: CMS4_V9-04-1?

        # # 2018 PromptReco
        # "data_2018Av1_singlemu" : "/SingleMuon/Run2018A-PromptReco-v1/MINIAOD",
        # "data_2018Av2_singlemu" : "/SingleMuon/Run2018A-PromptReco-v2/MINIAOD",
        # "data_2018Av3_singlemu" : "/SingleMuon/Run2018A-PromptReco-v3/MINIAOD",
        # "data_2018Bv1_singlemu" : "/SingleMuon/Run2018B-PromptReco-v1/MINIAOD",
        # "data_2018Bv2_singlemu" : "/SingleMuon/Run2018B-PromptReco-v2/MINIAOD",

        # "data_2018Av1_egamma" : "/EGamma/Run2018A-PromptReco-v1/MINIAOD",
        # "data_2018Av2_egamma" : "/EGamma/Run2018A-PromptReco-v2/MINIAOD",
        # "data_2018Av3_egamma" : "/EGamma/Run2018A-PromptReco-v3/MINIAOD",
        # "data_2018Bv1_egamma" : "/EGamma/Run2018B-PromptReco-v1/MINIAOD",
        # "data_2018Bv2_egamma" : "/EGamma/Run2018B-PromptReco-v2/MINIAOD",

        # "data_2018Av1_met" : "/MET/Run2018A-PromptReco-v1/MINIAOD",
        # "data_2018Av2_met" : "/MET/Run2018A-PromptReco-v2/MINIAOD",
        # "data_2018Av3_met" : "/MET/Run2018A-PromptReco-v3/MINIAOD",
        # "data_2018Bv1_met" : "/MET/Run2018B-PromptReco-v1/MINIAOD",
        # "data_2018Bv2_met" : "/MET/Run2018B-PromptReco-v2/MINIAOD",

        # "data_2018Av1_jetht" : "/JetHT/Run2018A-PromptReco-v1/MINIAOD",
        # "data_2018Av2_jetht" : "/JetHT/Run2018A-PromptReco-v2/MINIAOD",
        # "data_2018Av3_jetht" : "/JetHT/Run2018A-PromptReco-v3/MINIAOD",
        # "data_2018Bv1_jetht" : "/JetHT/Run2018B-PromptReco-v1/MINIAOD",
        # "data_2018Bv2_jetht" : "/JetHT/Run2018B-PromptReco-v2/MINIAOD",

        # # SingleMuon samples
        # "data_2017Bv1_singlemu" : "/SingleMuon/Run2017B-PromptReco-v1/MINIAOD",
        # "data_2017Bv2_singlemu" : "/SingleMuon/Run2017B-PromptReco-v2/MINIAOD",
        # "data_2017Cv1_singlemu" : "/SingleMuon/Run2017C-PromptReco-v1/MINIAOD",
        # "data_2017Cv2_singlemu" : "/SingleMuon/Run2017C-PromptReco-v2/MINIAOD",
        # "data_2017Cv3_singlemu" : "/SingleMuon/Run2017C-PromptReco-v3/MINIAOD",
        # "data_2017Dv1_singlemu" : "/SingleMuon/Run2017D-PromptReco-v1/MINIAOD",
        # "data_2017Ev1_singlemu" : "/SingleMuon/Run2017E-PromptReco-v1/MINIAOD",
        # "data_2017Fv1_singlemu" : "/SingleMuon/Run2017F-PromptReco-v1/MINIAOD",

        # # SingleElectron samples
        # "data_2017Bv1_singleel" : "/SingleElectron/Run2017B-PromptReco-v1/MINIAOD",
        # "data_2017Bv2_singleel" : "/SingleElectron/Run2017B-PromptReco-v2/MINIAOD",
        # "data_2017Cv1_singleel" : "/SingleElectron/Run2017C-PromptReco-v1/MINIAOD",
        # "data_2017Cv2_singleel" : "/SingleElectron/Run2017C-PromptReco-v2/MINIAOD",
        # "data_2017Cv3_singleel" : "/SingleElectron/Run2017C-PromptReco-v3/MINIAOD",
        # "data_2017Dv1_singleel" : "/SingleElectron/Run2017D-PromptReco-v1/MINIAOD",
        # "data_2017Ev1_singleel" : "/SingleElectron/Run2017E-PromptReco-v1/MINIAOD",
        # "data_2017Fv1_singleel" : "/SingleElectron/Run2017F-PromptReco-v1/MINIAOD",

        # # MET samples
        # "data_2017Bv1_met" : "/MET/Run2017B-PromptReco-v1/MINIAOD",
        # "data_2017Bv2_met" : "/MET/Run2017B-PromptReco-v2/MINIAOD",
        # "data_2017Cv1_met" : "/MET/Run2017C-PromptReco-v1/MINIAOD",
        # "data_2017Cv2_met" : "/MET/Run2017C-PromptReco-v2/MINIAOD",
        # "data_2017Cv3_met" : "/MET/Run2017C-PromptReco-v3/MINIAOD",
        # "data_2017Dv1_met" : "/MET/Run2017D-PromptReco-v1/MINIAOD",
        # "data_2017Ev1_met" : "/MET/Run2017E-PromptReco-v1/MINIAOD",
        # "data_2017Fv1_met" : "/MET/Run2017F-PromptReco-v1/MINIAOD",

        # # DoubleMuon samples
        # "data_2017Bv1_doublemu" : "/DoubleMuon/Run2017B-PromptReco-v1/MINIAOD",
        # "data_2017Bv2_doublemu" : "/DoubleMuon/Run2017B-PromptReco-v2/MINIAOD",
        # "data_2017Cv1_doublemu" : "/DoubleMuon/Run2017C-PromptReco-v1/MINIAOD",
        # "data_2017Cv2_doublemu" : "/DoubleMuon/Run2017C-PromptReco-v2/MINIAOD",
        # "data_2017Cv3_doublemu" : "/DoubleMuon/Run2017C-PromptReco-v3/MINIAOD",
        # "data_2017Dv1_doublemu" : "/DoubleMuon/Run2017D-PromptReco-v1/MINIAOD",
        # "data_2017Ev1_doublemu" : "/DoubleMuon/Run2017E-PromptReco-v1/MINIAOD",
        # "data_2017Fv1_doublemu" : "/DoubleMuon/Run2017F-PromptReco-v1/MINIAOD",

        # # DoubleEG samples
        # "data_2017Bv1_doubleeg" : "/DoubleEG/Run2017B-PromptReco-v1/MINIAOD",
        # "data_2017Bv2_doubleeg" : "/DoubleEG/Run2017B-PromptReco-v2/MINIAOD",
        # "data_2017Cv1_doubleeg" : "/DoubleEG/Run2017C-PromptReco-v1/MINIAOD",
        # "data_2017Cv2_doubleeg" : "/DoubleEG/Run2017C-PromptReco-v2/MINIAOD",
        # "data_2017Cv3_doubleeg" : "/DoubleEG/Run2017C-PromptReco-v3/MINIAOD",
        # "data_2017Dv1_doubleeg" : "/DoubleEG/Run2017D-PromptReco-v1/MINIAOD",
        # "data_2017Ev1_doubleeg" : "/DoubleEG/Run2017E-PromptReco-v1/MINIAOD",
        # "data_2017Fv1_doubleeg" : "/DoubleEG/Run2017F-PromptReco-v1/MINIAOD",

        # # MuonEG samples
        # "data_2017Bv1_muoneg" : "/MuonEG/Run2017B-PromptReco-v1/MINIAOD",
        # "data_2017Cv1_muoneg" : "/MuonEG/Run2017C-PromptReco-v1/MINIAOD",
        # "data_2017Bv2_muoneg" : "/MuonEG/Run2017B-PromptReco-v2/MINIAOD",
        # "data_2017Cv2_muoneg" : "/MuonEG/Run2017C-PromptReco-v2/MINIAOD",
        # "data_2017Cv3_muoneg" : "/MuonEG/Run2017C-PromptReco-v3/MINIAOD",
        # "data_2017Dv1_muoneg" : "/MuonEG/Run2017D-PromptReco-v1/MINIAOD",
        # "data_2017Ev1_muoneg" : "/MuonEG/Run2017E-PromptReco-v1/MINIAOD",
        # "data_2017Fv1_muoneg" : "/MuonEG/Run2017F-PromptReco-v1/MINIAOD",

    }

    # Make a directory sample, giving it the location and a dataset name for bookkeeping purposes
    # The globber must be customized (by default, it is *.root) in order to pick up the text files

    # Make a CondorTask (3 in total, one for each input)
    maker_tasks = []
    merge_tasks = []

    for babyname, dsname in snt_samples.items():
        cmsswver = "CMSSW_10_1_0"
        scramarch = "slc6_amd64_gcc700"
        tarfile = "tarfiles/input_v29_11.tar.gz"
        tag = "v29_11"
        cms3tag = "CMS4_V08-00-06" if "80X" in dsname else None
        extraarg = "1" if "SMS" in dsname else "0"
        # if "TTJets" in dsname and True: # to add switch later
        #     extraarg += " topcands"
        maker_task = CondorTask(
            sample = SNTSample(dataset=dsname,tag=cms3tag),
            files_per_output = 20 if "data" in babyname else 2,
            tag = tag,
            outdir_name = "stopBaby_" + babyname,
            output_name = "stopbaby.root",
            executable = "condor_executable.sh",
            cmssw_version = cmsswver,
            scram_arch = scramarch,
            arguments = extraarg, # isFastsim
            tarfile = tarfile,
            # condor_submit_params = {"sites": "UAF,T2_US_UCSD,UCSB"},
            condor_submit_params = {"use_xrootd": True},
            # max_jobs = 1,      # temporary for submission test
            no_load_from_backup = True,
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
            condor_submit_params = {"sites":"T2_US_UCSD"},
            output_is_tree = True,
            # check_expectedevents = True,
            tag = tag,
            cmssw_version = "CMSSW_9_2_8",
            scram_arch = "slc6_amd64_gcc530",
            no_load_from_backup = True,
        )
        maker_tasks.append(maker_task)
        merge_tasks.append(merge_task)


    for i in range(100):
        total_summary = {}

        for maker_task, merge_task in zip(maker_tasks,merge_tasks):
            maker_task.process()

            if maker_task.complete():
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
