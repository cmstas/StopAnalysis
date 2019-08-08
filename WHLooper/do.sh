#!/bin/bash

datetag=2019_07_24
babytag=signal

OUTDIR=/home/users/rheller/wh_babies/babies_${babytag}_${datetag}/
LOGDIR=/home/users/rheller/wh_babies/babies_${babytag}_${datetag}/log/


###2016 signal
# INDIR="/home/users/rheller/babies/merged_noskim/"
# declare -a Samples=()
# Samples+=( SMS-TChiWH )

# mkdir -p ${OUTDIR}; mkdir -p ${LOGDIR}
# for SAMPLE in ${Samples[@]}; do
#     # ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
#     #echo ./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR} '>&' ${LOGDIR}/log_${SAMPLE}.txt
#     #eval "./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR}"
#     #'>&' ${LOGDIR}/log_${SAMPLE}.txt
#     #eval "nohup nice -n -10 ./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
# done

########################
# 2016 MC

datetag=2019_07_24
babytag=v31_2

OUTDIR=/home/users/rheller/wh_babies/babies_${babytag}_${datetag}/
LOGDIR=/home/users/rheller/wh_babies/babies_${babytag}_${datetag}/log/

# # Moriond17 samples

INDIR=/nfs-7/userdata/sicheng/stopbabies/merged/s16v3_v31_2/

#INDIR=/nfs-7/userdata/sicheng/stopbabies/merged/s16v3_v30_9/ before july

#/nfs-7/userdata/sicheng/stopbabies/merged/s16v3_v30_3/
#/nfs-7/userdata/sicheng/stopbabies/merged_v29_11/
#/nfs-7/userdata/sicheng/stopbabies/skimmed/s16v3_v29_14


OUTDIR16=$OUTDIR

declare -a Samples=()
Samples+=( TTJets_1lep_top_s16v3_ext0 TTJets_1lep_top_s16v3_ext1 TTJets_1lep_tbar_s16v3_ext0 TTJets_1lep_tbar_s16v3_ext1)
Samples+=( TTJets_2lep_s16v3_ext0 TTJets_2lep_s16v3_ext1)
#Samples+=( ST_schan ST_tW_top ST_tW_tbar ST_tchan ) #ST_tW_tbar ST_tchan )      # singleT
Samples+=( W1Jets W2Jets W3Jets W4Jets)       # Vjets : Wjets + DY
#Samples+=( TTZToLLNuNu TTWJetsToLNu WZTo1L3Nu WWTo1L1Nu2Q)  # rare  : ttV + diboson
#Samples+=( WWTo2L2Nu WZTo1L1Nu2Q WZTo2L2Q WZTo3LNu_amcnlo TTWJetsToQQ TTZToQQ)      # rare : smaller contributions

#failed in first batch
# declare -a Samples=()
# Samples+=( TTJets_2lep_80X_ext1)
# Samples+=( ST_tW_antitop )      # singleT
#Samples+=( W1Jets W2Jets W3Jets W4Jets)       # Vjets : Wjets + DY     # rare : smaller contributions


# mkdir -p ${OUTDIR}; mkdir -p ${LOGDIR}
# for SAMPLE in ${Samples[@]}; do
#     # ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
#     #echo ./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR} '>&' ${LOGDIR}/log_${SAMPLE}.txt
#     #eval "./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR}"
#     #'>&' ${LOGDIR}/log_${SAMPLE}.txt
#    #eval "nohup nice -n -10 ./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
# done



########################
# 2016 Data

datetag=2019_04_03
babytag=v31_1

OUTDIR=/home/users/rheller/wh_babies/babies_${babytag}_${datetag}/
LOGDIR=/home/users/rheller/wh_babies/babies_${babytag}_${datetag}/log/

INDIR=/nfs-7/userdata/sicheng/stopbabies/merged/data_v31_1/
#/nfs-7/userdata/sicheng/stopbabies/merged/data_v30_3/
#/nfs-7/userdata/sicheng/stopbabies/merged_v29_14/

declare -a Samples=(data_2016B data_2016C data_2016D data_2016E data_2016F data_2016G data_2016H)
# declare -a Samples=(data_2016)

# mkdir -p ${OUTDIR}; mkdir -p ${LOGDIR}
# for SAMPLE in ${Samples[@]}; do
#     # ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
#     #echo ./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR}  '>&' ${LOGDIR}/log_${SAMPLE}.txt
#     #eval "nohup nice -n -10 ./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"

# done

datetag=2019_07_24
babytag=v31_1

### met binned 2016 ttbar
OUTDIR=/home/users/rheller/wh_babies/babies_${babytag}_${datetag}/
LOGDIR=/home/users/rheller/wh_babies/babies_${babytag}_${datetag}/log/
INDIR=/home/users/rheller/metBinned/


#declare -a Samples=(TTJets_2lep_met150_s16v3 TTJets_1lep_top_met150_s16v3 TTJets_1lep_tbar_met150_s16v3)
declare -a Samples=(TTJets_2lep_met150_s16v3 )
# declare -a Samples=(data_2016)

# mkdir -p ${OUTDIR}; mkdir -p ${LOGDIR}
# for SAMPLE in ${Samples[@]}; do
#     # ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
#     echo ./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR}  '>&' ${LOGDIR}/log_${SAMPLE}.txt
#     eval "nohup nice -n -10 ./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"

# done

datetag=2019_07_24
babytag=v32_0
INDIR=/home/users/rheller/z_merged/
OUTDIR=/home/users/rheller/wh_babies/babies_${babytag}_${datetag}/
LOGDIR=/home/users/rheller/wh_babies/babies_${babytag}_${datetag}/log/

declare -a Samples=(DYJetsToLL_M50_s16v3 DYJetsToLL_Zpt150_s16v3)
# declare -a Samples=(data_2016)

mkdir -p ${OUTDIR}; mkdir -p ${LOGDIR}
for SAMPLE in ${Samples[@]}; do
    # ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
    echo ./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR}  '>&' ${LOGDIR}/log_${SAMPLE}.txt
    eval "nohup nice -n -10 ./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"

done








