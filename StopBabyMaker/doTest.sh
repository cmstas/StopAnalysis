#!/bin/bash

[[ -f Makefile ]] && { make -j 12 || return $?; }

# runBabyMaker takes six arguments: ./runBabyMaker <sample_name> [nevents] [file_number] <outpath> <samplelist> [isFastSim]
#  Need to provide at least sample_name; nevents=-1 (-1=all events), file_number=-1 (-1=merged_ntuple_*.root), output=/nfs-7/userdata/stopRun2/, samplelist=sample_2017.dat, isFastSim=false by default

# ./runBabyMaker stopbaby 3000 1 ./ /hadoop/cms/store/group/snt/run2_data2018_prompt/MET_Run2018A-PromptReco-v1_MINIAOD_CMS4_V10-01-00/merged_ntuple_97.root 0
# ./runBabyMaker stopbaby -1 3 ./ /hadoop/cms/store/group/snt/run2_mc2017/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_MINIAODSIM_CMS4_V09-04-17/merged_ntuple_541.root 0 topcands
# ./runBabyMaker stopbaby 10000 15 ./ /hadoop/cms/store/group/snt/run2_mc2017/SMS-T2tt_mStop-650_mLSP-350_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_MINIAODSIM_CMS4_V09-04-17/merged_ntuple_1.root 0 stopcands
# ./runBabyMaker stopbaby 10000 15 ./ /hadoop/cms/store/user/sicheng/ProjectMetis/SMS_TChiWH_WToLNu_HToBB_mChargino850_mLSP1_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_MINIAODSIM_CMS4_V09-04-13_decorr/merged_ntuple_1.root 0
# ./runBabyMaker stopbaby 10000 15 ./ /hadoop/cms/store/user/sicheng/ProjectMetis/SMS-TChiWH_WToLNu_HToBB_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv2-PUSummer16Fast_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1_MINIAODSIM_CMS4_V00-00-02_toptag/merged_ntuple_9.root 1
# ./runBabyMaker stopbaby 3000 9 ./ /hadoop/cms/store/user/sicheng/ProjectMetis/MET_Run2016F-03Feb2017-v1_MINIAOD_CMS4_V00-00-02_data16ttags/merged_ntuple_30.root 0

# # Test photon samples
# ./runBabyMaker stopbaby 3000 92 ./ /hadoop/cms/store/group/snt/run2_data2017/SinglePhoton_Run2017D-31Mar2018-v1_MINIAOD_CMS4_V09-04-19/merged_ntuple_8.root 0
# ./runBabyMaker stopbaby 3000 92 ./ /hadoop/cms/store/group/snt/run2_mc2017/GJets_HT-600ToInf_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_MINIAODSIM_CMS4_V09-04-19/merged_ntuple_1.root 0

# # Test samples for v29, with CMS4_V09-04-19
# ./runBabyMaker stopbaby 1000 3 ./ /hadoop/cms/store/group/snt/run2_mc2017/TTGamma_Dilept_TuneCP5_PSweights_13TeV_madgraph_pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_MINIAODSIM_CMS4_V09-04-19/merged_ntuple_1.root 0
# ./runBabyMaker stopbaby 1000 3 ./ /hadoop/cms/store/group/snt/run2_data2017//MET_Run2017B-31Mar2018-v1_MINIAOD_CMS4_V09-04-19/merged_ntuple_112.root 0
# ./runBabyMaker stopbaby 1000 3 ./ /hadoop/cms/store/group/snt/run2_data2016_94x/SinglePhoton_Run2016E-17Jul2018-v1_MINIAOD_CMS4_V09-04-17/merged_ntuple_48.root 0
# ./runBabyMaker stopbaby 1000 3 ./ /hadoop/cms/store/user/sicheng/ProjectMetis/SMS-TChiWH_WToLNu_HToBB_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv2-PUSummer16Fast_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1_MINIAODSIM_CMS4_V00-00-02_toptags/merged_ntuple_22.root 1
# ./runBabyMaker stopbaby 1000 3 ./ /hadoop/cms/store/user/sicheng/ProjectMetis/SMS-T2tt_mStop-400to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1_MINIAODSIM_CMS4_V08-00-06/merged_ntuple_22.root 1

# Test samples for v30, with CMS4_V10-02-04/5
./runBabyMaker stopbaby 1000 3 ./ /hadoop/cms/store/group/snt/run2_mc2017/TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_MINIAODSIM_CMS4_V10-02-05/merged_ntuple_19.root 0 skim_met_emuEvt=150,skim_met=150
./runBabyMaker stopbaby 1000 3 ./ /hadoop/cms/store/group/snt/run2_mc2017/SMS-T2tt_mStop-1200to2000_TuneCP2_13TeV-madgraphMLM-pythia8_RunIIFall17MiniAODv2-PUFall17Fast_94X_mc2017_realistic_v15-v1_MINIAODSIM_CMS4_V10-02-05/merged_ntuple_4.root 1
./runBabyMaker stopbaby 1000 3 ./ /hadoop/cms/store/group/snt/run2_data2017/SingleMuon_Run2017F-31Mar2018-v1_MINIAOD_CMS4_V10-02-05/merged_ntuple_1.root 0
./runBabyMaker stopbaby 1000 3 ./ /hadoop/cms/store/group/snt/run2_data2017/SinglePhoton_Run2017E-31Mar2018-v1_MINIAOD_CMS4_V10-02-05/merged_ntuple_1.root 2
./runBabyMaker stopbaby 1000 3 ./ /hadoop/cms/store/group/snt/run2_data2017/SingleMuon_Run2017F-09May2018-v1_MINIAOD_CMS4_V10-02-04/merged_ntuple_2.root 0
./runBabyMaker stopbaby 1000 3 ./ /hadoop/cms/store/group/snt/run2_data2016_94x/SingleMuon_Run2016G-17Jul2018-v1_MINIAOD_CMS4_V10-02-04/merged_ntuple_1.root 0
./runBabyMaker stopbaby 1000 3 ./ /hadoop/cms/store/group/snt/run2_mc2016_94x/TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2_MINIAODSIM_CMS4_V09-04-17/merged_ntuple_19.root 0
./runBabyMaker stopbaby 1000 3 ./ /hadoop/cms/store/group/snt/run2_mc2016_94x/SMS-T2tt_mStop-1200to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv3-PUSummer16v3Fast_94X_mcRun2_asymptotic_v3-v1_MINIAODSIM_CMS4_V10-02-05/merged_ntuple_1.root 1
./runBabyMaker stopbaby 1000 3 ./ /hadoop/cms/store/group/snt/run2_data2018/SingleMuon_Run2018D-PromptReco-v2_MINIAOD_CMS4_V10-02-04/merged_ntuple_1.root 0
./runBabyMaker stopbaby 1000 3 ./ /hadoop/cms/store/group/snt/run2_mc2018/TT_DiLept_TuneCP5_13TeV-amcatnlo-pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2_MINIAODSIM_CMS4_V10-02-04/merged_ntuple_10.root 0

# # Produce fullsim signals since they don't need to be batch jobs
# ./runBabyMaker T2tt_mStop1200_mLSP100 -1 1 ./ /hadoop/cms/store/group/snt/run2_mc2017/SMS-T2tt_mStop-1200_mLSP-100_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_MINIAODSIM_CMS4_V09-04-19/merged_ntuple_1.root 0
# ./runBabyMaker T2tt_mStop650_mLSP350 -1 1 ./ /hadoop/cms/store/group/snt/run2_mc2017/SMS-T2tt_mStop-650_mLSP-350_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_MINIAODSIM_CMS4_V09-04-19/merged_ntuple_1.root 0
# ./runBabyMaker T2tt_mStop850_mLSP100 -1 1 ./ /hadoop/cms/store/group/snt/run2_mc2017/SMS-T2tt_mStop-850_mLSP-100_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_MINIAODSIM_CMS4_V09-04-19/merged_ntuple_1.root 0
# ./runBabyMaker stopbaby 1000 3 ./ /hadoop/cms/store/group/snt/run2_mc2017/SMS-T2tt_mStop-1200_mLSP-100_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_MINIAODSIM_CMS4_V09-04-19/merged_ntuple_1.root 0
# ./runBabyMaker stopbaby 1000 3 ./ /hadoop/cms/store/group/snt/run2_mc2017/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIFall17MiniAODv2-PU2017RECOSIMstep_12Apr2018_94X_mc2017_realistic_v14-v1_MINIAODSIM_CMS4_V10-02-05/merged_ntuple_1.root 0

