#!/bin/bash

make -j 12 || return $?


INDIR=/nfs-7/userdata/haweber/stopbabiesInteractive/2017_5p8fbinv

OUTDIR=output/temp4
LOGDIR=logs
mkdir -p ${OUTDIR}
mkdir -p ${LOGDIR}

# declare -a Samples=(data_*_2017C data_*_2017B data_*_2017A)
# declare -a Samples=(data)
# declare -a Samples=(data_singlemu)
declare -a Samples=(data_singlemu data data_met)

for SAMPLE in ${Samples[@]}; do
    echo ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} '>&' ${LOGDIR}/log_${SAMPLE}.txt
    # ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
    # eval "nohup ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} >&  ${LOGDIR}/log_${SAMPLE}.txt &"
    # echo nice -n 10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
    eval "nohup nice -n -10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
done

# 2016 data for comparison
INDIR=/nfs-7/userdata/stopRun2/analysis2016_SUS-16-051_35p9fbinv/v24/output
# INDIR=/nfs-7/userdata/stopRun2/analysis2016_SUS-16-051_35p9fbinv/v24/skim
# OUTDIR=output/data2016
OUTDIR=output/temp3
mkdir -p ${OUTDIR}
mkdir -p ${LOGDIR}

# declare -a Samples=(data_single_muon)
# declare -a Samples=(data)
declare -a Samples=(data_single_muon data data_met)

for SAMPLE in ${Samples[@]}; do
    echo ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} '>&' ${LOGDIR}/log_${SAMPLE}_2016.txt
    # ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
    # eval "nohup ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}_2016.txt &"
    # echo nice -n 10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
    eval "nohup nice -n -10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}_2016.txt &"
done

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


# 2016 Signal
declare -a Samples=(Signal_T2bW Signal_T2tb Signal_T2tt)

# Monitor the running
while : ; do
    sleep 5
    RunningJobs=`jobs`
    [[ $RunningJobs == "" ]] && break
done    
echo All jobs are done!
echo -e '\a'

# . temp.sh
