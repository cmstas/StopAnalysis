#!/bin/bash

make -j 12 || return $?

# runBabyMaker takes six arguments: ./runBabyMaker sample_name nevents file_number outpath samplelist isFastSim
#  Need to provide at least sample_name; nevents=-1 (-1=all events), file_number=-1 (-1=merged_ntuple_*.root), output=/nfs-7/userdata/stopRun2/, samplelist=sample_2017.dat, isFastSim=false by default
# ./runBabyMaker data_single_muon_Run2017B_MINIAOD_PromptReco-v1 10000 1 
./runBabyMaker TTJets_V07_test 3000 1 ./ sample_2017.dat 0
# ./runBabyMaker TTJets_V07_test -1 1 ./ sample_2017.dat 0
# ./runBabyMaker stopbaby -1 1 ./ /hadoop/cms/store/user/namin/run2_moriond17_cms4/ProjectMetis/SMS-T8bbstausnu_XCha0p5_XStau0p25_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv2-PUSummer16Fast_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1_MINIAODSIM_CMS4_V00-00-02_2017Sep27/merged_ntuple_15.root 1
# ./runBabyMaker DYJetsToLL_M50_madgraph_pythia8_25ns 10000 1 
# ./runBabyMaker ttbar_madgraph_pythia8_25ns 10000 1 
