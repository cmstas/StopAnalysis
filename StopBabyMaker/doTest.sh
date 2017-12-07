#!/bin/bash

make -j 12 || return $?

# runBabyMaker takes six arguments: ./runBabyMaker sample_name nevents file_number outpath samplelist isFastSim
#  Need to provide at least sample_name; nevents=-1 (-1=all events), file_number=-1 (-1=merged_ntuple_*.root), output=/nfs-7/userdata/stopRun2/, samplelist=sample_2017.dat, isFastSim=false by default
# ./runBabyMaker data_single_muon_Run2017B_MINIAOD_PromptReco-v1 10000 1 
# ./runBabyMaker TTJets_V07_test 3000 1 ./ sample_2017.dat 0
# ./runBabyMaker TTJets_V07_test -1 1 ./ sample_2017.dat 0
# ./runBabyMaker stopbaby -1 1 ./ /hadoop/cms/store/user/namin/ProjectMetis/SingleMuon_Run2017E-PromptReco-v1_MINIAOD_CMS4_V00-00-06/merged_ntuple_10.root 0
# ./runBabyMaker stopbaby 3000 1 ./ /hadoop/cms/store/user/sicheng/ProjectMetis/TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISummer17MiniAOD-92X_upgrade2017_realistic_v10-v3_MINIAODSIM_CMS4_V00-00-07_test/merged_ntuple_1.root 0
# ./runBabyMaker stopbaby 3000 1 ./ /hadoop/cms/store/user/sicheng/ProjectMetis/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8_RunIISummer17MiniAOD-92X_upgrade2017_realistic_v10_ext1-v1_MINIAODSIM_CMS4_V00-00-07_test2/merged_ntuple_1.root 0
# ./runBabyMaker stopbaby 3000 1 ./ /hadoop/cms/store/user/sicheng/ProjectMetis/W2JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer17MiniAOD-92X_upgrade2017_realistic_v10-v1_MINIAODSIM_CMS4_V00-00-07_test2/merged_ntuple_69.root 0
./runBabyMaker SMS_stopbaby 9000 2 ./ /hadoop/cms/store/user/sicheng/ProjectMetis/SMS-T2tt_mStop-400to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1_MINIAODSIM_CMS4_V00-00-02_test/merged_ntuple_56.root 1
# ./runBabyMaker stopbaby 9000 1 ./ /hadoop/cms/store/user/sicheng/ProjectMetis/SMS-T2bt_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1_MINIAODSIM_CMS4_V00-00-02_test/merged_ntuple_1.root 1
# ./runBabyMaker Signal_stopbaby 3000 2 ./ /home/users/sicheng/working/ProjectMetis/test/workspace/CMSSW_8_0_26_patch1/ntuple.root 1
# ./runBabyMaker stopbaby 3000 1 ./ /hadoop/cms/store/user/sicheng/ProjectMetis/TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISummer17MiniAOD-92X_upgrade2017_realistic_v10-v3_MINIAODSIM_CMS4_V00-00-07_test/merged_ntuple_1.root,/hadoop/cms/store/user/sicheng/ProjectMetis/TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISummer17MiniAOD-92X_upgrade2017_realistic_v10-v3_MINIAODSIM_CMS4_V00-00-07_test/merged_ntuple_2.root,/hadoop/cms/store/user/sicheng/ProjectMetis/TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISummer17MiniAOD-92X_upgrade2017_realistic_v10-v3_MINIAODSIM_CMS4_V00-00-07_test/merged_ntuple_3.root 0
# ./runBabyMaker stopbaby -1 1 ./ 
# ./runBabyMaker DYJetsToLL_M50_madgraph_pythia8_25ns 10000 1 
# ./runBabyMaker ttbar_madgraph_pythia8_25ns 10000 1 
