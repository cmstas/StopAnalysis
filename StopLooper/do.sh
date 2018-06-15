#!/bin/bash

RunningJobs=`jobs | grep runStopLooper`
if [[ $RunningJobs == "" ]]; then
    make -j 12 || return $?
else
    echo "Running looper jobs exist, will not re-build the looper."
    read -p "Do append jobs? (use Ctrl-c to break if not) " dummy
    unset dummy
fi

LOGDIR=logs

# 2018 Data

# INDIR=/nfs-7/userdata/sicheng/stopbabies/merged_v27_3
INDIR=/nfs-7/userdata/sicheng/stopbabies/skimmed_v27_4
OUTDIR=output/temp2018data4

mkdir -p ${OUTDIR}
mkdir -p ${LOGDIR}

declare -a Samples=(data_2018A data_2018B)

for SAMPLE in ${Samples[@]}; do
    # ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
    echo ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} '>&' ${LOGDIR}/log_${SAMPLE}.txt
    eval "nohup nice -n -10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
done

# 2017 Data

# INDIR=/nfs-7/userdata/haweber/stopbabiesInteractive/2017_5p8fbinv
# INDIR=/nfs-7/userdata/haweber/tupler_babies/merged/Stop_1l/v2017/output/
# INDIR=/nfs-7/userdata/sicheng/stopbabies/merged_v25_7
# INDIR=/nfs-7/userdata/sicheng/stopbabies/skimmed_v25_7
OUTDIR=output/temp2017data4

mkdir -p ${OUTDIR}
mkdir -p ${LOGDIR}

# declare -a Samples=(data_2017B data_2017C data_2017D data_2017E data_2017F)
declare -a Samples=(data_2017B data_2017C data_2017D data_2017E data_2017F)

for SAMPLE in ${Samples[@]}; do
    # ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
    echo ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} '>&' ${LOGDIR}/log_${SAMPLE}.txt
    eval "nohup nice -n -10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
done

# 2017 MC test

# INDIR=/nfs-7/userdata/sicheng/stopbabies/merged_v25_4
INDIR=/hadoop/cms/store/user/sicheng/ProjectMetis/stopBaby_ttbar_semileptonic_madgraph_v27_2/skimmed
OUTDIR=output/tempIncl

mkdir -p ${OUTDIR}
mkdir -p ${LOGDIR}

# declare -a Samples=(TTJets W2Jets W3Jets W4Jets)
declare -a Samples=(ttbar_semileptonic)
# declare -a Samples=(merged_TTJets)

# for SAMPLE in ${Samples[@]}; do
#     # ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
#     # echo ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
#     eval "nohup nice -n -10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
# done

# 2016 data
# INDIR=/nfs-7/userdata/stopRun2/analysis2016_SUS-16-051_35p9fbinv/v24/skim
INDIR=/nfs-7/userdata/stopRun2/analysis2016_SUS-16-051_35p9fbinv/v24/output
# OUTDIR=output/samples2016
# INDIR=/nfs-7/userdata/sicheng/stopbabies/skimmed_v25_10
OUTDIR=output/temp2016data4

mkdir -p ${OUTDIR}
mkdir -p ${LOGDIR}

declare -a Samples=(2016data_sample)
# declare -a Samples=(data_2016B data_2016C data_2016D data_2016E data_2016F data_2016G data_2016H)
# declare -a Samples=(data_2016B)

for SAMPLE in ${Samples[@]}; do
    # ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
    echo ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} '>&' ${LOGDIR}/log_${SAMPLE}.txt
    eval "nohup nice -n -10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
done

# 2016 MC

# INDIR=/nfs-7/userdata/stopRun2/analysis2016_SUS-16-051_35p9fbinv/v22/skim
# declare -a Samples=()
# Samples+=( ttbar_diLept  )    # ttbar
# Samples+=( ttbar_singleLeptFromT_madgraph_pythia8_ext1 ttbar_singleLeptFromTbar_madgraph_pythia8_ext1 )    # ttbar
# Samples+=( t_sch_4f t_tch_4f tbar_tch t_tW_5f t_tbarW_5f ) # singleT
# Samples+=( W1Jets W2Jets W3Jets W4JetsToLNu_madgraph_pythia8_25ns DYJets )            # Vjets : Wjets + DY
# Samples+=( ttWJets ttZJets WW WZ ZZ )                      # rare  : ttV + diboson
# Samples+=( Signal_T2tt )

# INDIR=/nfs-7/userdata/sicheng/stopbabies/merged_v25_9
INDIR=/nfs-7/userdata/sicheng/stopbabies/skimmed_v25_9
# INDIR=/nfs-7/userdata/sicheng/stopbabies/skimmed_v27_1
OUTDIR=output/tempI2

mkdir -p ${OUTDIR}
mkdir -p ${LOGDIR}

declare -a Samples=()
Samples+=( ttbar_diLept ttbar_singleLeptFromT_madgraph ttbar_singleLeptFromTbar_madgraph )    # ttbar
Samples+=( t_tW_top t_tW_antitop )      # singleT
Samples+=( W1Jets W2Jets W3Jets W4Jets)       # Vjets : Wjets + DY
Samples+=( TTZToLLNuNu WZTo1L3Nu )            # rare  : ttV + diboson

# declare -a Samples=(ttbar_singleLeptFromT_amcnlo_94X)

# for SAMPLE in ${Samples[@]}; do
#     # ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
#     echo ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} '>&' ${LOGDIR}/log_${SAMPLE}.txt
#     eval "nohup nice -n -10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
# done


# 2016 signal
INDIR=/nfs-7/userdata/sicheng/stopbabies/skimmed_v25_9
# INDIR=/nfs-7/userdata/stopRun2/analysis2016_SUS-16-051_35p9fbinv/v22/skim
# INDIR=/nfs-7/userdata/stopRun2/analysis2016_SUS-16-051_35p9fbinv/v22_usegenMET/skim
# OUTDIR=output/temp12

mkdir -p ${OUTDIR}
mkdir -p ${LOGDIR}

declare -a Samples=()
Samples+=( SMS_T2tt_mStop_400to1200_madgraph_0 SMS_T2tt_mStop_400to1200_madgraph_1 SMS_T2tt_mStop_400to1200_madgraph_2 )
Samples+=( SMS_T2tt_mStop_400to1200_madgraph_3 SMS_T2tt_mStop_400to1200_madgraph_4 SMS_T2tt_mStop_400to1200_madgraph_5 )
Samples+=( SMS_T2tt_mStop_400to1200_madgraph_6 SMS_T2tt_mStop_400to1200_madgraph_7 )
# Samples+=( SMS_T2bW_madgraph_0 SMS_T2bW_madgraph_1 SMS_T2bW_madgraph_2 )
# Samples+=( SMS_T2bW_madgraph_3 SMS_T2bW_madgraph_4 SMS_T2bW_madgraph_5 )
# Samples+=( SMS_T2bW_madgraph_6 SMS_T2bW_madgraph_7 )
# Samples+=( SMS_T2tb_madgraph_0 SMS_T2tb_madgraph_1 SMS_T2tb_madgraph_2 )
# Samples+=( SMS_T2tb_madgraph_3 SMS_T2tb_madgraph_4 SMS_T2tb_madgraph_5 )
# Samples+=( Signal_T2tt Signal_T2bW Signal_T2tb )
# Samples+=( Signal_T2tt )
# Samples+=( SMS_T2tt )

# for SAMPLE in ${Samples[@]}; do
#     echo ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} '>&' ${LOGDIR}/log_${SAMPLE}.txt
#     eval "nohup nice -n -10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
# done

# Monitor the running
while : ; do
    sleep 5
    RunningJobs=`jobs | grep runStopLooper`
    [[ $RunningJobs == "" ]] && break
done

echo -e 'All looper jobs done!\a'

pushd output/temp2018data4/
hadd -f data_2018_all.root  data_2018?.root > /dev/null
popd > /dev/null
pushd output/temp2017data4/
hadd -f data_2017_all.root  data_2017?.root > /dev/null
popd > /dev/null
pushd output/temp2016data4/
# hadd -f data_2016_all.root  data_2016?.root > /dev/null
cp data_single_lepton_met.root data_2016_all.root 
popd > /dev/null

# # Local merge for the v25_9 babies
# pushd ${OUTDIR}
# hadd -f ttbar_25ns.root    ttbar_*Lept*.root > /dev/null
# hadd -f singleT_25ns.root  t_tW_*.root  > /dev/null
# hadd -f Vjets_25ns.root    W1Jets*.root W2Jets*.root W3Jets*.root W4Jets*.root > /dev/null
# hadd -f rare_25ns.root     TTZ*.root WZ*.root  > /dev/null
# hadd -f allBkg_25ns.root   ttbar_25ns.root singleT_25ns.root Vjets_25ns.root rare_25ns.root > /dev/null
# # hadd -f allData_25ns.root  data_2016*.root > /dev/null
# hadd -f SMS_T2tt.root      SMS_T2tt_mStop_400to1200_madgraph_?.root > /dev/null
# # hadd -f SMS_T2bW.root      SMS_T2bW_madgraph_?.root > /dev/null
# # hadd -f SMS_T2tb.root      SMS_T2tb_madgraph_?.root > /dev/null
# rm SMS_T2*_madgraph_?.root
# popd > /dev/null

# # Local merge for the moriond17 samples
# pushd output/samples2016
# hadd -f ttbar_25ns.root    ttbar*Lept*.root > /dev/null
# hadd -f singleT_25ns.root  t_t*_noHadDecays*.root t*_4f_*.root > /dev/null
# hadd -f Vjets_25ns.root    W?Jets*.root DYJets*m10To50*.root > /dev/null
# hadd -f rare_25ns.root     ttZ*.root WZ*.root ttW*.root WW*.root ZZ*.root > /dev/null
# hadd -f allBkg_25ns.root   ttbar_25ns.root singleT_25ns.root Vjets_25ns.root rare_25ns.root > /dev/null
# popd > /dev/null

# # Temporary merge for toptagging studies
# pushd output/temp11
# cp ttbar_diLept.root ttbar_2lep.root
# hadd -f ttbar_1lep.root  ttbar_singleLept*.root > /dev/null
# hadd -f SMS_T2tt.root    SMS_T2tt_mStop_400to1200_madgraph_?.root > /dev/null
# popd > /dev/null

echo -e 'All merge jobs done!\a'

unset RunningJobs
