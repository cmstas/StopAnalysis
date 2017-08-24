#!/bin/bash

make -j 12 || return $?


INDIR=/nfs-7/userdata/haweber/stopbabiesInteractive/2017_5p8fbinv

OUTDIR=output/temp
LOGDIR=logs
mkdir -p ${OUTDIR}
mkdir -p ${LOGDIR}

declare -a Samples=(data_singlemu)

for SAMPLE in ${Samples[@]}; do
    echo ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
    # ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
    eval "nohup ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"

    # echo nice -n 10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
    # eval "nohup nice -n 10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
done

# 2016 data for comparison
INDIR=/nfs-7/userdata/stopRun2/analysis2016_SUS-16-051_35p9fbinv/v24/output
OUTDIR=output/data2016

mkdir -p ${OUTDIR}
mkdir -p ${LOGDIR}

declare -a Samples=(data_single_muon)

for SAMPLE in ${Samples[@]}; do
    echo ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
    # ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
    eval "nohup ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"

    # echo nice -n 10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
    # eval "nohup nice -n 10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}_2016.txt &"
done

