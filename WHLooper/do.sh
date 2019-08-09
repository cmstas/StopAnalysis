#!/bin/bash

datetag=2019_08_09
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

datetag=2019_08_09
babytag=v31_2

OUTDIR=/home/users/rheller/wh_babies/babies_${babytag}_${datetag}/s16v3/
LOGDIR=/home/users/rheller/wh_babies/babies_${babytag}_${datetag}/s16v3/log/

# # 94X samples
INDIR=/nfs-7/userdata/sicheng/stopbabies/merged/s16v3_v31_2/

#INDIR=/nfs-7/userdata/sicheng/stopbabies/merged/s16v3_v30_9/ before july

#/nfs-7/userdata/sicheng/stopbabies/merged/s16v3_v30_3/
#/nfs-7/userdata/sicheng/stopbabies/merged_v29_11/
#/nfs-7/userdata/sicheng/stopbabies/skimmed/s16v3_v29_14

declare -a Samples=()
Samples+=( TTJets_1lep_top_s16v3_ext0 TTJets_1lep_top_s16v3_ext1 TTJets_1lep_tbar_s16v3_ext0 TTJets_1lep_tbar_s16v3_ext1)
Samples+=( TTJets_2lep_s16v3_ext0 TTJets_2lep_s16v3_ext1)
Samples+=( ST_schan ST_tW_top ST_tW_tbar ST_tchan ) #ST_tW_tbar ST_tchan )      # singleT
Samples+=( W1Jets W2Jets W3Jets W4Jets)       # Vjets : Wjets + DY
Samples+=( TTZ TTW WZ WW DYJets)  # rare  : ttV + diboson
#Samples+=( WWTo2L2Nu WZTo1L1Nu2Q WZTo2L2Q WZTo3LNu_amcnlo TTWJetsToQQ TTZToQQ)      # rare : smaller contributions


mkdir -p ${OUTDIR}; mkdir -p ${LOGDIR}
for SAMPLE in ${Samples[@]}; do

  	echo ./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR}  '>&' ${LOGDIR}/log_${SAMPLE}.txt
   	eval "nohup nice -n -10 ./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
done



########################
# 2017 MC

datetag=2019_08_09
babytag=v31_2

OUTDIR=/home/users/rheller/wh_babies/babies_${babytag}_${datetag}/f17v2/
LOGDIR=/home/users/rheller/wh_babies/babies_${babytag}_${datetag}/f17v2/log/

INDIR=/nfs-7/userdata/sicheng/stopbabies/merged/f17v2_v31_2/


declare -a Samples=()
Samples+=( TTJets_1lep_top TTJets_1lep_tbar)
Samples+=( TTJets_2lep)
Samples+=( ST_schan ST_tW_top ST_tW_tbar ST_tchan )    # singleT
Samples+=( W1Jets W2Jets W3Jets W4Jets)       # Vjets : Wjets + DY
Samples+=( TTZ TTW WZ WW DYJets)  # rare  : ttV + diboson
#Samples+=( WWTo2L2Nu WZTo1L1Nu2Q WZTo2L2Q WZTo3LNu_amcnlo TTWJetsToQQ TTZToQQ)      # rare : smaller contributions


mkdir -p ${OUTDIR}; mkdir -p ${LOGDIR}
for SAMPLE in ${Samples[@]}; do

  	echo ./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR}  '>&' ${LOGDIR}/log_${SAMPLE}.txt
   	eval "nohup nice -n -10 ./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
done


########################
# 2016 Data

datetag=2019_08_09
babytag=v31_1

OUTDIR=/home/users/rheller/wh_babies/babies_${babytag}_${datetag}/
LOGDIR=/home/users/rheller/wh_babies/babies_${babytag}_${datetag}/log/

INDIR=/nfs-7/userdata/sicheng/stopbabies/merged/data_v31_1/
#/nfs-7/userdata/sicheng/stopbabies/merged/data_v30_3/
#/nfs-7/userdata/sicheng/stopbabies/merged_v29_14/

declare -a Samples=(data_2016B data_2016C data_2016D data_2016E data_2016F data_2016G data_2016H)
# declare -a Samples=(data_2016)

mkdir -p ${OUTDIR}; mkdir -p ${LOGDIR}
for SAMPLE in ${Samples[@]}; do
    echo ./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR}  '>&' ${LOGDIR}/log_${SAMPLE}.txt
    eval "nohup nice -n -10 ./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
done


########################
# 2017/18 Data

datetag=2019_08_09
babytag=v31_2

OUTDIR=/home/users/rheller/wh_babies/babies_${babytag}_${datetag}/
LOGDIR=/home/users/rheller/wh_babies/babies_${babytag}_${datetag}/log/

INDIR=/nfs-7/userdata/sicheng/stopbabies/merged/data_v31_2/
#/nfs-7/userdata/sicheng/stopbabies/merged/data_v30_3/
#/nfs-7/userdata/sicheng/stopbabies/merged_v29_14/

declare -a Samples=(data_2017B data_2017C data_2017D data_2017E data_2017F )
Samples+=( data_2018A data_2018B data_2018C data_2018D )


mkdir -p ${OUTDIR}; mkdir -p ${LOGDIR}
for SAMPLE in ${Samples[@]}; do
    echo ./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR}  '>&' ${LOGDIR}/log_${SAMPLE}.txt
    eval "nohup nice -n -10 ./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
done


### met binned 2016 ttbar
datetag=2019_08_09
babytag=v30_9


OUTDIR=/home/users/rheller/wh_babies/babies_${babytag}_${datetag}/s16v3/
LOGDIR=/home/users/rheller/wh_babies/babies_${babytag}_${datetag}/s16v3/log/
INDIR=/nfs-7/userdata/stopRun2/run2babies/merged/s16v3_v30_9/


declare -a Samples=(TTJets_2lep_met150_s16v3 TTJets_1lep_top_met150_s16v3 TTJets_1lep_tbar_met150_s16v3)

 mkdir -p ${OUTDIR}; mkdir -p ${LOGDIR}
 for SAMPLE in ${Samples[@]}; do
     echo ./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR}  '>&' ${LOGDIR}/log_${SAMPLE}.txt
     eval "nohup nice -n -10 ./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
 done


### met binned 2017 ttbar
datetag=2019_08_09
babytag=v30_9

OUTDIR=/home/users/rheller/wh_babies/babies_${babytag}_${datetag}/f17v2/
LOGDIR=/home/users/rheller/wh_babies/babies_${babytag}_${datetag}/f17v2/log/
INDIR=/nfs-7/userdata/stopRun2/run2babies/merged/f17v2_v30_9/


declare -a Samples=(TTJets_2lep_met150 TTJets_1lep_top_met150 TTJets_1lep_tbar_met150)

 mkdir -p ${OUTDIR}; mkdir -p ${LOGDIR}
 for SAMPLE in ${Samples[@]}; do
     echo ./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR}  '>&' ${LOGDIR}/log_${SAMPLE}.txt
     eval "nohup nice -n -10 ./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
 done



#### special DY samples
datetag=2019_08_09
babytag=v32_0z
INDIR=/home/users/rheller/z_merged/
OUTDIR=/home/users/rheller/wh_babies/babies_${babytag}_${datetag}/
LOGDIR=/home/users/rheller/wh_babies/babies_${babytag}_${datetag}/log/

declare -a Samples=(DYJetsToLL)
#declare -a Samples=(DYJetsToLL_M50_s16v3 DYJetsToLL_Zpt150_s16v3)
# declare -a Samples=(data_2016)

mkdir -p ${OUTDIR}; mkdir -p ${LOGDIR}
for SAMPLE in ${Samples[@]}; do
    echo ./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR}  '>&' ${LOGDIR}/log_${SAMPLE}.txt
    eval "nohup nice -n -10 ./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"

done








