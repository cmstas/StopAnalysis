#!/bin/bash

make -j 12 || return $?

# runBabyMaker takes six arguments: ./runBabyMaker sample_name nevents file_number outpath samplelist isFastSim
#  Need to provide at least sample_name; nevents=-1 (-1=all events), file_number=-1 (-1=merged_ntuple_*.root), output=/nfs-7/userdata/stopRun2/, samplelist=sample_2017.dat, isFastSim=false by default
# ./runBabyMaker data_single_muon_Run2017B_MINIAOD_PromptReco-v1 10000 1 
# ./runBabyMaker TTJets_V07_test 3000 1 ./ sample_2017.dat 0
# ./runBabyMaker TTJets_V07_test -1 1 ./ sample_2017.dat 0
# ./runBabyMaker stopbaby 3000 1 ./ /hadoop/cms/store/user/sicheng/ProjectMetis/TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISummer17MiniAOD-92X_upgrade2017_realistic_v10-v3_MINIAODSIM_CMS4_V00-00-07_test/merged_ntuple_1.root 0
./runBabyMaker stopbaby 3000 1 ./ /hadoop/cms/store/user/sicheng/ProjectMetis/TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISummer17MiniAOD-92X_upgrade2017_realistic_v10-v3_MINIAODSIM_CMS4_V00-00-07_test/merged_ntuple_1.root,/hadoop/cms/store/user/sicheng/ProjectMetis/TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISummer17MiniAOD-92X_upgrade2017_realistic_v10-v3_MINIAODSIM_CMS4_V00-00-07_test/merged_ntuple_2.root,/hadoop/cms/store/user/sicheng/ProjectMetis/TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISummer17MiniAOD-92X_upgrade2017_realistic_v10-v3_MINIAODSIM_CMS4_V00-00-07_test/merged_ntuple_3.root 0
# ./runBabyMaker stopbaby -1 1 ./ 
# ./runBabyMaker DYJetsToLL_M50_madgraph_pythia8_25ns 10000 1 
# ./runBabyMaker ttbar_madgraph_pythia8_25ns 10000 1 
