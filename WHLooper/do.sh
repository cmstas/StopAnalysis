#!/bin/bash

datetag=2019_01_08

OUTDIR=/home/users/rheller/wh_babies/babies_${datetag}/
LOGDIR=/home/users/rheller/wh_babies/babies_${datetag}/log/
########################
# 2016 MC

# # Moriond17 samples

INDIR=/nfs-7/userdata/sicheng/stopbabies/merged_v29_11/
#/nfs-7/userdata/sicheng/stopbabies/skimmed/s16v3_v29_14


OUTDIR16=$OUTDIR

# declare -a Samples=()
# Samples+=( TTJets_2lep_s16v3_ext1_0 TTJets_2lep_s16v3_ext1_1 TTJets_2lep_s16v3_ext0_1)
# Samples+=( TTJets_1lep_t TTJets_1lep_tbar )
# Samples+=( ST_tW_top ST_tW_tbar ST_tchan )      # singleT
# Samples+=( W1Jets W2Jets W3Jets W4Jets)       # Vjets : Wjets + DY
# Samples+=( TTZToLLNuNu TTWJetsToLNu WZTo1L3Nu WWTo1L1Nu2Q)  # rare  : ttV + diboson
# Samples+=( WWTo2L2Nu WZTo1L1Nu2Q WZTo2L2Q WZTo3LNu_amcnlo TTWJetsToQQ TTZToQQ)      # rare : smaller contributions

#failed in first batch
declare -a Samples=()
Samples+=( TTJets_2lep_80X_ext1)
Samples+=( ST )      # singleT
#Samples+=( W1Jets W2Jets W3Jets W4Jets)       # Vjets : Wjets + DY     # rare : smaller contributions


mkdir -p ${OUTDIR}; mkdir -p ${LOGDIR}
for SAMPLE in ${Samples[@]}; do
    # ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
    echo ./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR} '>&' ${LOGDIR}/log_${SAMPLE}.txt
    #eval "./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR}"
    #'>&' ${LOGDIR}/log_${SAMPLE}.txt
    #eval "nohup nice -n -10 ./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
done



########################
# 2016 Data

INDIR=/nfs-7/userdata/sicheng/stopbabies/merged_v29_14/

declare -a Samples=(data_2016B data_2016C data_2016D data_2016E data_2016F data_2016G data_2016H)
# declare -a Samples=(data_2016)

mkdir -p ${OUTDIR}; mkdir -p ${LOGDIR}
for SAMPLE in ${Samples[@]}; do
    # ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
    echo  echo ./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR}  '>&' ${LOGDIR}/log_${SAMPLE}.txt
    eval "nohup nice -n -10 ./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"

done
