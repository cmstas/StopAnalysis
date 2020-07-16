#!/bin/bash


datetag=2020_07_16
babytag=v33_8


########################
# 2016 Data

OUTDIR=/home/users/rheller/wh_babies/babies_${babytag}_${datetag}/
LOGDIR=/home/users/rheller/wh_babies/babies_${babytag}_${datetag}/log/

INDIR=/hadoop/cms/store/user/dspitzba/stopbabies/data2016_v33_4/
#/nfs-7/userdata/sicheng/stopbabies/merged/data_v30_3/
#/nfs-7/userdata/sicheng/stopbabies/merged_v29_14/

declare -a Samples=(data_2016B data_2016C data_2016D data_2016E data_2016F data_2016G data_2016H)
# declare -a Samples=(data_2016)

mkdir -p ${OUTDIR}; mkdir -p ${LOGDIR}
for SAMPLE in ${Samples[@]}; do
    echo ./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR}  '>&' ${LOGDIR}/log_${SAMPLE}.txt
    #eval "nohup nice -n -10 ./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
done


########################
# 2017 Data

OUTDIR=/home/users/rheller/wh_babies/babies_${babytag}_${datetag}/
LOGDIR=/home/users/rheller/wh_babies/babies_${babytag}_${datetag}/log/

INDIR=/hadoop/cms/store/user/dspitzba/stopbabies/data2017_v33_4/
#/nfs-7/userdata/sicheng/stopbabies/merged/data_v30_3/
#/nfs-7/userdata/sicheng/stopbabies/merged_v29_14/

declare -a Samples=(data_2017B data_2017C data_2017D data_2017E data_2017F )


mkdir -p ${OUTDIR}; mkdir -p ${LOGDIR}
for SAMPLE in ${Samples[@]}; do
    echo ./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR}  '>&' ${LOGDIR}/log_${SAMPLE}.txt
    #eval "nohup nice -n -10 ./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
done

########################
# 2018 Data

OUTDIR=/home/users/rheller/wh_babies/babies_${babytag}_${datetag}/
LOGDIR=/home/users/rheller/wh_babies/babies_${babytag}_${datetag}/log/

INDIR=/hadoop/cms/store/user/dspitzba/stopbabies/data2018_v33_4/
#/nfs-7/userdata/sicheng/stopbabies/merged/data_v30_3/
#/nfs-7/userdata/sicheng/stopbabies/merged_v29_14/

declare -a Samples=(data_2018A data_2018B data_2018C data_2018D )


mkdir -p ${OUTDIR}; mkdir -p ${LOGDIR}
for SAMPLE in ${Samples[@]}; do
    echo ./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR}  '>&' ${LOGDIR}/log_${SAMPLE}.txt
    #eval "nohup nice -n -10 ./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
done


###2016 signal

OUTDIR=/home/users/rheller/wh_babies/babies_${babytag}_${datetag}/
LOGDIR=/home/users/rheller/wh_babies/babies_${babytag}_${datetag}/log/

INDIR=/hadoop/cms/store/user/dspitzba/stopbabies/s16v3_v33_4/
#INDIR="/home/users/rheller/babies/merged_noskim/"
declare -a Samples=()
Samples+=( SMS_TChiWH )

mkdir -p ${OUTDIR}; mkdir -p ${LOGDIR}
for SAMPLE in ${Samples[@]}; do
    echo ./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR}  '>&' ${LOGDIR}/log_${SAMPLE}.txt
    #eval "nohup nice -n -10 ./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
done


##2017 Signal
OUTDIR=/home/users/rheller/wh_babies/babies_${babytag}_${datetag}/
LOGDIR=/home/users/rheller/wh_babies/babies_${babytag}_${datetag}/log/

INDIR=/hadoop/cms/store/user/dspitzba/stopbabies/f17v2_v33_4/
#INDIR="/home/users/rheller/babies/merged_noskim/"
declare -a Samples=()
Samples+=( SMS_TChiWH )

mkdir -p ${OUTDIR}; mkdir -p ${LOGDIR}
for SAMPLE in ${Samples[@]}; do
    echo ./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR}  '>&' ${LOGDIR}/log_${SAMPLE}.txt
    #eval "nohup nice -n -10 ./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
done

##2018 Signal
OUTDIR=/home/users/rheller/wh_babies/babies_${babytag}_${datetag}/
LOGDIR=/home/users/rheller/wh_babies/babies_${babytag}_${datetag}/log/
mkdir -p ${OUTDIR}; mkdir -p ${LOGDIR}
# INDIR=/hadoop/cms/store/user/dspitzba/stopbabies/a18v1_v33_4/

#FullSim Signal
INDIR=/hadoop/cms/store/user/dspitzba/ProjectMetis/stopbaby_SMS_TChiWH_mCh750_mLSP1_a18v1_v33_8/merged/
# INDIR=/hadoop/cms/store/user/dspitzba/ProjectMetis/stopbaby_SMS_TChiWH_mCh350_mLSP100_a18v1_v33_8/merged/
# INDIR=/hadoop/cms/store/user/dspitzba/ProjectMetis/stopbaby_SMS_TChiWH_mCh350_mLSP100_s16v3_v33_8/merged/
# INDIR=/hadoop/cms/store/user/dspitzba/ProjectMetis/stopbaby_SMS_TChiWH_mCh750_mLSP1_s16v3_v33_8/merged/
#INDIR="/home/users/rheller/babies/merged_noskim/"
declare -a Samples=()
Samples+=( SMS_TChiWH_mCh )
for SAMPLE in ${Samples[@]}; do
    echo ./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR}  '>&' ${LOGDIR}/log_${SAMPLE}_a18_m750.txt
    eval "nohup nice -n -10 ./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
done


########################
# 2016 MC

OUTDIR=/home/users/rheller/wh_babies/babies_${babytag}_${datetag}/s16v3/
LOGDIR=/home/users/rheller/wh_babies/babies_${babytag}_${datetag}/s16v3/log/


INDIR=/hadoop/cms/store/user/dspitzba/stopbabies/s16v3_v33_4/

#INDIR=/nfs-7/userdata/sicheng/stopbabies/merged/s16v3_v30_9/ before july

#/nfs-7/userdata/sicheng/stopbabies/merged/s16v3_v30_3/
#/nfs-7/userdata/sicheng/stopbabies/merged_v29_11/
#/nfs-7/userdata/sicheng/stopbabies/skimmed/s16v3_v29_14

declare -a Samples=()
# Samples+=( TTJets_1lep_top_s16v3_ext0 TTJets_1lep_top_s16v3_ext1 TTJets_1lep_tbar_s16v3_ext0 TTJets_1lep_tbar_s16v3_ext1)
# Samples+=( TTJets_2lep_s16v3_ext0 TTJets_2lep_s16v3_ext1)
# Samples+=( TTJets_1lep_top_met150 TTJets_1lep_tbar_met150  TTJets_2lep_met150)
# Samples+=( ST_schan ST_tW_top ST_tW_tbar ST_tchan ) #ST_tW_tbar ST_tchan )      # singleT
# Samples+=( W1Jets W2Jets W3Jets W4Jets)       # Vjets : Wjets + DY
# Samples+=( TTZ TTW WZ WW DYJetsToLL_M50_amcnlo DYJetsToLL_M50_s16v3 DYJetsToLL_Zpt150 )  # rare  : ttV + diboson
Samples+=( ZZ )  # rare  : ttV + diboson

mkdir -p ${OUTDIR}; mkdir -p ${LOGDIR}
for SAMPLE in ${Samples[@]}; do
    echo ./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR}  '>&' ${LOGDIR}/log_${SAMPLE}.txt
    #eval "nohup nice -n -10 ./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
done


########################
# 2017 MC



OUTDIR=/home/users/rheller/wh_babies/babies_${babytag}_${datetag}/f17v2/
LOGDIR=/home/users/rheller/wh_babies/babies_${babytag}_${datetag}/f17v2/log/

#INDIR=/hadoop/cms/store/user/dspitzba/stopbabies/f17v2_v33_4/
INDIR=/hadoop/cms/store/user/dspitzba/ProjectMetis/stopbaby_WWToLNuQQ_f17v2_v33_7/merged/

declare -a Samples=()
# Samples+=( DYJetsToLL_M50_f17v2_ext1 ) 
# Samples+=( TTJets_1lep_top_f17v2  TTJets_1lep_tbar_f17v2 TTJets_2lep_f17v2)
# Samples+=( TTJets_1lep_top_met150 TTJets_1lep_tbar_met150  TTJets_2lep_met150)
# Samples+=( ST_schan ST_tW_top ST_tW_tbar ST_tchan )    # singleT
# Samples+=( W1Jets W2Jets W3Jets W4Jets WJets_0J_amcnlo WJets_1J_amcnlo WJets_2J_amcnlo)       # Vjets : Wjets + DY
# Samples+=( TTZ TTW WZ WW DYJetsToLL_M50_f17v2 DYJetsToLL_M50_f17v2_ext1 DYJetsToLL_amcnlo_M50_f17v2_1 DYJetsToLL_amcnlo_M50_f17v2_ext1)  # rare  : ttV + diboson
 ## Samples+=( WWTo2L2Nu WZTo1L1Nu2Q WZTo2L2Q WZTo3LNu_amcnlo TTWJetsToQQ TTZToQQ ZZ)      # rare : smaller contributions
Samples+=( WWToLNuQQ )      # rare : smaller contributions


mkdir -p ${OUTDIR}; mkdir -p ${LOGDIR}
for SAMPLE in ${Samples[@]}; do

  	echo ./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR}  '>&' ${LOGDIR}/log_${SAMPLE}.txt
   	#eval "nohup nice -n -10 ./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
done

########################
# 2018 MC


OUTDIR=/home/users/rheller/wh_babies/babies_${babytag}_${datetag}/a18v1/
LOGDIR=/home/users/rheller/wh_babies/babies_${babytag}_${datetag}/a18v1/log/

#INDIR=/hadoop/cms/store/user/dspitzba/stopbabies/a18v1_v33_4/
INDIR=/hadoop/cms/store/user/dspitzba/ProjectMetis/stopbaby_TTJets_2lep_met80_a18v1_v33_7/merged/


declare -a Samples=()
# Samples+=( TTJets_2lep_met80_a18v1_10 TTJets_2lep_met80_a18v1_11 TTJets_2lep_met80_a18v1_12 TTJets_2lep_met80_a18v1_13 TTJets_2lep_met80_a18v1_14 TTJets_2lep_met80_a18v1_15  )
# Samples+=( TTJets_1lep_top_a18v1  TTJets_1lep_tbar_a18v1 TTJets_2lep_a18v1)
# Samples+=( TTJets_1lep_top_met80 TTJets_1lep_tbar_met80  TTJets_2lep_met80)
# Samples+=( ST_schan ST_tW_top ST_tW_tbar ST_tchan )    # singleT
# Samples+=( W1Jets W2Jets W3Jets W4Jets  WJets_0J_amcnlo  WJets_1J_amcnlo WJets_2J_amcnlo)       # Vjets : Wjets + DY
# Samples+=( TTZ TTW WZ WW DYJetsToLL_Zpt150 DYJetsToLL_amcnlo DYJetsToLL_madgraph)  # rare  : ttV + diboson
#Samples+=( WWTo2L2Nu WZTo1L1Nu2Q WZTo2L2Q WZTo3LNu_amcnlo TTWJetsToQQ TTZToQQ ZZ)      # rare : smaller contributions
Samples+=( TTJets_2lep_met80)


mkdir -p ${OUTDIR}; mkdir -p ${LOGDIR}
for SAMPLE in ${Samples[@]}; do

    echo ./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR}  '>&' ${LOGDIR}/log_${SAMPLE}.txt
 #   eval "nohup nice -n -10 ./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
done

exit 1
########################
#Jet HT data for trigger study

OUTDIR=/home/users/rheller/wh_babies/babies_${babytag}_${datetag}/
LOGDIR=/home/users/rheller/wh_babies/babies_${babytag}_${datetag}/log/

INDIR=/hadoop/cms/store/user/sicheng/stopbabies/merged/data_v30_3/jetht/
#/nfs-7/userdata/sicheng/stopbabies/merged/data_v30_3/
#/nfs-7/userdata/sicheng/stopbabies/merged_v29_14/

declare -a Samples=(data_2017B data_2017C data_2017D data_2017E data_2017F )
Samples+=( data_2018A data_2018B data_2018C data_2018D )
Samples+=( data_2016B data_2016C data_2016D data_2016E data_2016F data_2016G data_2016H )


mkdir -p ${OUTDIR}; mkdir -p ${LOGDIR}
for SAMPLE in ${Samples[@]}; do
    echo ./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR}  '>&' ${LOGDIR}/log_${SAMPLE}.txt
    #eval "nohup nice -n -10 ./processWH.py ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
done

