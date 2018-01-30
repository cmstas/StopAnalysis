#!/bin/bash

make -j 12 || return $?


# 2017 Data

# INDIR=/nfs-7/userdata/haweber/stopbabiesInteractive/2017_5p8fbinv
# INDIR=/nfs-7/userdata/haweber/tupler_babies/merged/Stop_1l/v2017/output/
INDIR=/nfs-7/userdata/sicheng/stopbabies/merged_v25_7
OUTDIR=output/temp
# OUTDIR=output/data2017_rwgtd
LOGDIR=logs

mkdir -p ${OUTDIR}
mkdir -p ${LOGDIR}

# declare -a Samples=(data_2017B data_2017C data_2017D data_2017E data_2017F)
declare -a Samples=(data_2017D)

# for SAMPLE in ${Samples[@]}; do
#     # ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
#     echo ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} '>&' ${LOGDIR}/log_${SAMPLE}.txt
#     eval "nohup nice -n -10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
# done

# 2017 MC test

INDIR=/nfs-7/userdata/sicheng/stopbabies/merged_v25_4
# INDIR=/hadoop/cms/store/user/sicheng/ProjectMetis/stopBaby_TTJets_amcnlo_v25_8/merged
OUTDIR=output/temp
# OUTDIR=output/samples2016

mkdir -p ${OUTDIR}
mkdir -p ${LOGDIR}

# declare -a Samples=(TTJets W2Jets W3Jets W4Jets)
declare -a Samples=(TTJets)
# declare -a Samples=(merged_TTJets)

# for SAMPLE in ${Samples[@]}; do
#     ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
#     # echo ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
#     # eval "nohup nice -n -10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
# done

# 2016 data for comparison
# INDIR=/nfs-7/userdata/stopRun2/analysis2016_SUS-16-051_35p9fbinv/v24/output
INDIR=/nfs-7/userdata/stopRun2/analysis2016_SUS-16-051_35p9fbinv/v24/skim
OUTDIR=output/temp8

mkdir -p ${OUTDIR}
mkdir -p ${LOGDIR}

# declare -a Samples=(data_2016B data_2016C data_2016D data_2016E data_2016F data_2016G data_2016H)
declare -a Samples=(all_2016)

# for SAMPLE in ${Samples[@]}; do
#     ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
#     echo ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} '>&' ${LOGDIR}/log_${SAMPLE}.txt
#     # eval "nohup nice -n -10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
# done

# 2016 MC

# INDIR=/nfs-7/userdata/stopRun2/analysis2016_SUS-16-051_35p9fbinv/v22/skim
# OUTDIR=output/mc2016
# Samples+=( ttbar_diLept  )    # ttbar
# Samples+=( ttbar_singleLeptFromT_madgraph_pythia8_ext1 ttbar_singleLeptFromTbar_madgraph_pythia8_ext1 )    # ttbar
# Samples+=( t_sch_4f t_tch_4f tbar_tch t_tW_5f t_tbarW_5f ) # singleT
# Samples+=( W1Jets W2Jets W3Jets W4JetsToLNu_madgraph_pythia8_25ns DYJets )            # Vjets : Wjets + DY
# Samples+=( ttWJets ttZJets WW WZ ZZ )                      # rare  : ttV + diboson

# INDIR=/nfs-7/userdata/sicheng/stopbabies/merged_v25_9
INDIR=/nfs-7/userdata/sicheng/stopbabies/skimmed_v25_9
OUTDIR=output/temp9

mkdir -p ${OUTDIR}
mkdir -p ${LOGDIR}

declare -a Samples=()
Samples+=( ttbar_diLept ttbar_singleLeptFromT_madgraph ttbar_singleLeptFromTbar_madgraph )    # ttbar
Samples+=( t_tW_top t_tW_antitop )      # singleT
Samples+=( W1Jets W2Jets W3Jets W4Jets)       # Vjets : Wjets + DY
Samples+=( TTZToLLNuNu WZTo1L3Nu )            # rare  : ttV + diboson

for SAMPLE in ${Samples[@]}; do
    echo ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} '>&' ${LOGDIR}/log_${SAMPLE}.txt
    eval "nohup nice -n -10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
done


# 2016 signal
INDIR=/nfs-7/userdata/sicheng/stopbabies/skimmed_v25_9
OUTDIR=output/temp9

mkdir -p ${OUTDIR}
mkdir -p ${LOGDIR}

declare -a Samples=(SMS_T2tt_mStop_400to1200)

for SAMPLE in ${Samples[@]}; do
    echo ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} '>&' ${LOGDIR}/log_${SAMPLE}.txt
    eval "nohup nice -n -10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
done

# Monitor the running
while : ; do
    sleep 5
    RunningJobs=`jobs | grep runStopLooper`
    [[ $RunningJobs == "" ]] && break
done

echo -e 'All jobs are done!\a'

pushd output/temp9
hadd -f ttbar_25ns.root    ttbar_diLept*.root ttbar_singleLept*.root > /dev/null
hadd -f singleT_25ns.root  t_tW_*.root  > /dev/null
hadd -f Vjets_25ns.root    W1Jets*.root W2Jets*.root W3Jets*.root W4Jets*.root > /dev/null
hadd -f rare_25ns.root     TTZ*.root WZ*.root  > /dev/null
hadd -f allBkg_25ns.root   ttbar_25ns.root singleT_25ns.root Vjets_25ns.root rare_25ns.root > /dev/null
popd > /dev/null

# Special operations on output files
# . temp.sh output/data2017_rwgtd
