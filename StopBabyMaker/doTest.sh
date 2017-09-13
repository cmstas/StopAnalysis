#!/bin/bash

make -j 12 || return $?

# runBabyMaker takes six arguments: ./runBabyMaker sample_name nevents file_number outpath samplelist isFastSim
#  Need to provide at least sample_name; nevents=-1 (-1=all events), file_number=-1 (-1=merged_ntuple_*.root), output=/nfs-7/userdata/stopRun2/, samplelist=sample_2017.dat, isFastSim=false by default
# ./runBabyMaker data_single_muon_Run2017B_MINIAOD_PromptReco-v1 10000 1 
# ./runBabyMaker ttbar_2017 1000 1 
./runBabyMaker DYJetsToLL_M50_madgraph_pythia8_25ns 1000 1 
# ./runBabyMaker ttbar_madgraph_pythia8_25ns 10000 1 
