#41, 42, 130, 140, 145, 197, 201, 
for i in `seq 201 201`;
do
   ./runBabyMaker T14t_mGluino1200_mLSP800_FastSim -1 ${i} ./ /hadoop/cms/store/group/snt/run2_mc2017//SMS-T1tttt_TuneCP2_13TeV-madgraphMLM-pythia8_RunIIFall17MiniAODv2-PUFall17Fast_pilot_94X_mc2017_realistic_v15-v3_MINIAODSIM_CMS4_V10-02-05/merged_ntuple_${i}.root 1
done
