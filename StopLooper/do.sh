#!/bin/bash

make -j 12 || return $?


# INDIR=/nfs-7/userdata/haweber/stopbabiesInteractive/2017_5p8fbinv
# INDIR=/nfs-7/userdata/haweber/tupler_babies/merged/Stop_1l/v2017/output/
INDIR=/nfs-7/userdata/sicheng/stopbabies/merged_v25_7
# OUTDIR=output/temp6
# OUTDIR=output/temp4
OUTDIR=output/temp
# OUTDIR=output/data2017_rwgtd
LOGDIR=logs

mkdir -p ${OUTDIR}
mkdir -p ${LOGDIR}

# declare -a Samples=(data_*2017C data_*2017B data_*2017A)
# declare -a Samples=(data_2017dilep)
# declare -a Samples=(data_2017Cv3)
# declare -a Samples=(data_met_run2017B_v2)
# declare -a Samples=(data_2017B data_2017C data_2017D data_2017E data_2017F)
declare -a Samples=(data_2017D)
# eclare -a Samples=(data_singlemu data data_met data_singleel)

# for SAMPLE in ${Samples[@]}; do
#     # echo ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} '>&' ${LOGDIR}/log_${SAMPLE}.txt
#     # ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
#     # eval "nohup ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} >&  ${LOGDIR}/log_${SAMPLE}.txt &"
#     echo nice -n 10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
#     eval "nohup nice -n -10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
# done

# 2017 MC test
INDIR=/nfs-7/userdata/sicheng/stopbabies/merged_v25_4
# OUTDIR=output/temp
# INDIR=/nfs-7/userdata/sicheng/stopbabies/merged_v26_1
# INDIR=/hadoop/cms/store/user/sicheng/ProjectMetis/stopBaby_SMS_T2bW_madgraph_v26_1/merged
# OUTDIR=output/temp_v26_1
# INDIR=/nfs-7/userdata/sicheng/stopbabies/merged_v26_2
OUTDIR=output/temp
# INDIR=/nfs-7/userdata/stopRun2/analysis2016_SUS-16-051_35p9fbinv/v22/skim
# OUTDIR=output/samples2016
# INDIR=/hadoop/cms/store/user/sicheng/ProjectMetis/stopBaby_TTJets_amcnlo_v26_3/merged
# OUTDIR=output/temp_v26_3

mkdir -p ${OUTDIR}
mkdir -p ${LOGDIR}

# declare -a Samples=(TTJets W2Jets W3Jets W4Jets)
declare -a Samples=(TTJets)
# declare -a Samples=(W2to4JetsToLNu)
# declare -a Samples=(TTJets ttbar SMS_T2tt_mStop-400to1200)
# declare -a Samples=(Signal_T2tt_mStop_400to1200)
# declare -a Samples=(merged_TTJets) # merged_SMS_T2bW

# for SAMPLE in ${Samples[@]}; do
#     # echo ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} '>&' ${LOGDIR}/log_${SAMPLE}.txt
#     # ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
#     echo nice -n 10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
#     eval "nohup nice -n -10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
# done

# 2016 signal remade
# INDIR=/nfs-7/userdata/sicheng/stopbabies/skimmed_v25_4
# declare -a Samples=(SMS_T2bW SMS_T2tb SMS_T2tt)
# declare -a Samples=(SMS_T2tt_mStop-400to1200)

# INDIR=/hadoop/cms/store/user/sicheng/ProjectMetis/stopBaby_SMS_T2tt_mStop-400to1200_madgraph_v26_1/
# INDIR=/hadoop/cms/store/user/sicheng/ProjectMetis/stopBaby_TTJets_amcnlo_v26_1
# OUTDIR=output/temp_v26_1
# mkdir -p ${OUTDIR}
# mkdir -p ${LOGDIR}

declare -a Samples=(stopbaby)

# for SAMPLE in ${Samples[@]}; do
#     echo ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} '>&' ${LOGDIR}/log_${SAMPLE}.txt
#     ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
#     # eval "nohup ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
#     # echo nice -n 10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
#     # eval "nohup nice -n -10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
# done

# 2016 data for comparison
# INDIR=/nfs-7/userdata/stopRun2/analysis2016_SUS-16-051_35p9fbinv/v24/output
INDIR=/nfs-7/userdata/stopRun2/analysis2016_SUS-16-051_35p9fbinv/v24/skim
# OUTDIR=output/data2016_rwgtd
# OUTDIR=output/data2016
# OUTDIR=output/temp5
# OUTDIR=output/samples2016
OUTDIR=output/temp8

mkdir -p ${OUTDIR}
mkdir -p ${LOGDIR}

# declare -a Samples=(data_single_muon)
# declare -a Samples=(data_2016B data_2016C data_2016D data_2016E data_2016F data_2016G data_2016H)
# declare -a Samples=(data_2016)
declare -a Samples=(all_2016)
# declare -a Samples=(data_2016all data_2016dilep)
# declare -a Samples=(data_single_muon)
# declare -a Samples=(data_single_muon data data_met data_single_electron)
# declare -a Samples=(data_muon_eg)

for SAMPLE in ${Samples[@]}; do
    echo ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} '>&' ${LOGDIR}/log_${SAMPLE}.txt
    ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
    # eval "nohup ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
    # echo nice -n 10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
    # eval "nohup nice -n -10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
done

# 2016 MC for comparison
INDIR=/nfs-7/userdata/stopRun2/analysis2016_SUS-16-051_35p9fbinv/v22/skim
OUTDIR=output/mc2016

mkdir -p ${OUTDIR}
mkdir -p ${LOGDIR}

declare -a Samples=()
Samples+=( ttbar_diLept  )    # ttbar 
Samples+=( ttbar_singleLeptFromT_madgraph_pythia8_ext1 ttbar_singleLeptFromTbar_madgraph_pythia8_ext1 )    # ttbar
Samples+=( t_sch_4f t_tch_4f tbar_tch t_tW_5f t_tbarW_5f ) # singleT
Samples+=( W1Jets W2Jets W3Jets W4JetsToLNu_madgraph_pythia8_25ns DYJets )            # Vjets : Wjets + DY
Samples+=( ttWJets ttZJets WW WZ ZZ )                      # rare  : ttV + diboson

# declare -a Samples=(DYJetsToLL TTGJets TTWJets TTZ W1Jets W2Jets W3Jets W4Jets W4Jets WJetsToLNu WW WZ ZZ tZq ttbar ttWJets ttZJets t_sch t_tW t_tbarW t_tch tbar_tch)

# for SAMPLE in ${Samples[@]}; do
#     # echo ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
#     # ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
#     # eval "nohup ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
#     echo nice -n 10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
#     eval "nohup nice -n -10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}_2016.txt &"
# done

# Monitor the running
while : ; do
    sleep 5
    RunningJobs=`jobs | grep runStopLooper`
    [[ $RunningJobs == "" ]] && break
done

echo -e 'All jobs are done!\a'

# # Merge outputs
# pushd output/data2016
# hadd -f all_data_2016.root data_2016?.root > /dev/null
# popd > /dev/null

# pushd output/mc2016
# hadd -f ttbar_singleLept.root ttbar_singleLeptFrom*ext1.root > /dev/null 
# hadd -f ttbar.root    ttbar_diLept.root ttbar_singleLept.root > /dev/null
# hadd -f singleT.root  t_sch_4f.root t_tch_4f.root tbar_tch.root t_tW_5f.root t_tbarW_5f.root  > /dev/null
# hadd -f Vjets.root    W1Jets.root W2Jets.root W3Jets.root W4Jets*.root DYJets.root > /dev/null
# hadd -f rare.root     ttWJets.root ttZJets.root WW.root WZ.root ZZ.root  > /dev/null
# hadd -f all_mc_2016.root  ttbar.root singleT.root Vjets.root rare.root > /dev/null
# popd > /dev/null 

# Special operations on output files
# . temp.sh output/data2017_rwgtd
