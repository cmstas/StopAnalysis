#!/bin/bash

make -j 12 || return $?


INDIR=/nfs-7/userdata/haweber/stopbabiesInteractive/2017_5p8fbinv
# INDIR=/nfs-7/userdata/haweber/tupler_babies/merged/Stop_1l/v2017/output/
# OUTDIR=output/temp6
# OUTDIR=output/temp4
OUTDIR=output/temp
LOGDIR=logs
mkdir -p ${OUTDIR}
mkdir -p ${LOGDIR}

# declare -a Samples=(data_*2017C data_*2017B data_*2017A)
# declare -a Samples=(data_2017)
# declare -a Samples=(data_2017dilep)
declare -a Samples=(data_met)
# declare -a Samples=(data_singlemu)
# declare -a Samples=(data_singlemu data data_met data_singleel)

# for SAMPLE in ${Samples[@]}; do
#     echo ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} '>&' ${LOGDIR}/log_${SAMPLE}.txt
#     # ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
#     # eval "nohup ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} >&  ${LOGDIR}/log_${SAMPLE}.txt &"
#     # echo nice -n 10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
#     eval "nohup nice -n -10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
# done

# 2017 MC test
# INDIR=/nfs-7/userdata/sicheng/stopbabies/merged_v25_4
# OUTDIR=output/temp
# INDIR=/nfs-7/userdata/sicheng/stopbabies/merged_v26_1
# OUTDIR=output/temp_v26_1
INDIR=/nfs-7/userdata/stopRun2/analysis2016_SUS-16-051_35p9fbinv/v22/skim
OUTDIR=output/samples2016

mkdir -p ${OUTDIR}
mkdir -p ${LOGDIR}

# declare -a Samples=(TTJets W2Jets W3Jets W4Jets)
# declare -a Samples=(ttbar)
# declare -a Samples=(TTJets)
# declare -a Samples=(SMS_T2tt_mStop-400to1200)
declare -a Samples=(ttbar Signal_T2tt_mStop_400to1200)

for SAMPLE in ${Samples[@]}; do
    # echo ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} '>&' ${LOGDIR}/log_${SAMPLE}.txt
    # ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
    echo nice -n 10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
    eval "nohup nice -n -10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
done

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
# OUTDIR=output/data2016
# OUTDIR=output/temp5
OUTDIR=output/samples2016
# OUTDIR=output/temp1
mkdir -p ${OUTDIR}
mkdir -p ${LOGDIR}

# declare -a Samples=(data_single_muon)
declare -a Samples=(data_2016)
# declare -a Samples=(data_2016dilep)
# declare -a Samples=(data_met)
# declare -a Samples=(data_single_muon data data_met data_single_electron)
# declare -a Samples=(data_muon_eg)

# for SAMPLE in ${Samples[@]}; do
#     echo ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} '>&' ${LOGDIR}/log_${SAMPLE}.txt
#     # ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
#     # eval "nohup ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
#     # echo nice -n 10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
#     eval "nohup nice -n -10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
# done

# 2016 MC for comparison
INDIR=/nfs-7/userdata/stopRun2/analysis2016_SUS-16-051_35p9fbinv/v22/skim
OUTDIR=output/mc2016

mkdir -p ${OUTDIR}
mkdir -p ${LOGDIR}

# declare -a Samples=(ttbar)
declare -a Samples=(DYJetsToLL TTGJets TTWJets TTZ W1Jets W2Jets W3Jets W4Jets W4Jets WJetsToLNu WW WZ ZZ tZq ttbar ttWJets ttZJets t_sch t_tW t_tbarW t_tch tbar_tch)

# for SAMPLE in ${Samples[@]}; do
#     echo ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
#     # ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
#     eval "nohup ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
#     # echo nice -n 10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
#     # eval "nohup nice -n 10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}_2016.txt &"
# done

# Monitor the running
while : ; do
    sleep 5
    RunningJobs=`jobs | grep runStopLooper`
    [[ $RunningJobs == "" ]] && break
done    

echo -e 'All jobs are done!\a'

. temp.sh
