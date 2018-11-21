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

########################
# 2018 Data

# INDIR=/nfs-7/userdata/sicheng/stopbabies/skimmed_v27_5
INDIR=/nfs-7/userdata/sicheng/stopbabies/merged_v27_5/shuffle
OUTDIR=output/temp2018data2
LOGDIR=$OUTDIR/logs

# declare -a Samples=(data_2018A_met data_2018B_met data_2018A_singlemu data_2018A_egamma data_2018B_singlemu data_2018B_egamma data_2018A_jetht data_2018B_jetht)
declare -a Samples=(data_2018_jetht data_2018_singlemu data_2018_met data_2018_egamma)

# mkdir -p ${OUTDIR}; mkdir -p ${LOGDIR}
# for SAMPLE in ${Samples[@]}; do
#     # ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
#     echo ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} '>&' ${LOGDIR}/log_${SAMPLE}.txt
#     eval "nohup nice -n -10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
# done

########################
# 2018 MC

# INDIR=/nfs-7/userdata/sicheng/stopbabies/merged_v28_4
INDIR=/hadoop/cms/store/user/sicheng/ProjectMetis/stopBaby_TTJets_amcnlo_v28_5/
OUTDIR=output/tempNewTagger5
LOGDIR=$OUTDIR/logs
OUTDIR18=$OUTDIR

# # declare -a Samples=(TTJets_amcnlo_0 TTJets_amcnlo_1 TTJets_amcnlo_2)
# declare -a Samples=(stopbaby)

# mkdir -p ${OUTDIR}; mkdir -p ${LOGDIR}
# for SAMPLE in ${Samples[@]}; do
#     ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
#     echo ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} '>&' ${LOGDIR}/log_${SAMPLE}.txt
#     # eval "nohup nice -n -10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
# done

########################
# 2017 Data

# INDIR=/nfs-7/userdata/haweber/stopbabiesInteractive/2017_5p8fbinv
# INDIR=/nfs-7/userdata/haweber/tupler_babies/merged/Stop_1l/v2017/output/
INDIR=/nfs-7/userdata/sicheng/stopbabies/skimmed_v29_13
OUTDIR=output/samp17_v13_bdt
# OUTDIR=output/data17_jetht
LOGDIR=$OUTDIR/logs

declare -a Samples=(data_2017)
# declare -a Samples=(data_2017B data_2017C data_2017D data_2017E data_2017F)

mkdir -p ${OUTDIR}; mkdir -p ${LOGDIR}
for SAMPLE in ${Samples[@]}; do
    # ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
    echo ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} '>&' ${LOGDIR}/log_${SAMPLE}.txt
    eval "nohup nice -n -10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
done

########################
# 2017 MC

# INDIR=/nfs-7/userdata/sicheng/stopbabies/merged_v25_4
# INDIR=/nfs-7/userdata/sicheng/stopbabies/merged_v28_10
# INDIR=/nfs-7/userdata/sicheng/stopbabies/skimmed_v29_13
# OUTDIR=output/testResTopJet20_12
LOGDIR=${OUTDIR}/logs
OUTDIR17=$OUTDIR

declare -a Samples=()
Samples+=( TTJets_2lep_madgraph_0 TTJets_2lep_madgraph_1 TTJets_2lep_madgraph_2 )
Samples+=( TTJets_1lep_t_madgraph TTJets_1lep_tbar_madgraph )
Samples+=( ST_tW_top ST_tW_antitop ST_schan ST_tchan )      # singleT
Samples+=( W1JetsToLNu W2JetsToLNu W3JetsToLNu W4JetsToLNu)       # Vjets : Wjets + DY
# Samples+=( W1Jets_NuPt200 W2Jets_NuPt200 W3Jets_NuPt200 W4Jets_NuPt200)
Samples+=( TTZToLLNuNu TTWJetsToLNu )      # rare  : ttV 
Samples+=( WZTo1L3Nu WZTo3LNu WZTo2L2Q )      # diboson

mkdir -p ${OUTDIR}; mkdir -p ${LOGDIR}
for SAMPLE in ${Samples[@]}; do
    # ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
    echo ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} '>&' ${LOGDIR}/log_${SAMPLE}.txt
    eval "nohup nice -n -10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
done

########################
# 2016 data

# INDIR=/nfs-7/userdata/stopRun2/analysis2016_SUS-16-051_35p9fbinv/v24/output
# INDIR=/nfs-7/userdata/sicheng/stopbabies/merged_v25_10
INDIR=/nfs-7/userdata/sicheng/stopbabies/skimmed_v29_14
OUTDIR=output/samp16_v14_bdt
# OUTDIR=output/data16_jetht
LOGDIR=${OUTDIR}/logs
# cd ../StopCORE; cp stop_1l_babyAnalyzer.h.old stop_1l_babyAnalyzer.h; cp stop_1l_babyAnalyzer.cc.old stop_1l_babyAnalyzer.cc; mkc; cd -

# declare -a Samples=(all_2016_samples)
declare -a Samples=(data_2016B data_2016C data_2016D data_2016E data_2016F data_2016G data_2016H)
# declare -a Samples=(data_2016)

mkdir -p ${OUTDIR}; mkdir -p ${LOGDIR}
for SAMPLE in ${Samples[@]}; do
    # ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
    echo ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} '>&' ${LOGDIR}/log_${SAMPLE}.txt
    eval "nohup nice -n -10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
done

########################
# 2016 MC

# # Moriond17 samples
# INDIR=/nfs-7/userdata/stopRun2/analysis2016_SUS-16-051_35p9fbinv/v22/skim
# declare -a Samples=()
# Samples+=( ttbar_diLept  )    # ttbar
# Samples+=( ttbar_singleLeptFromT_madgraph_pythia8_ext1 ttbar_singleLeptFromTbar_madgraph_pythia8_ext1 )    # ttbar
# Samples+=( t_sch_4f t_tch_4f tbar_tch t_tW_5f t_tbarW_5f ) # singleT
# Samples+=( W1Jets W2Jets W3Jets W4JetsToLNu_madgraph_pythia8_25ns DYJets )            # Vjets : Wjets + DY
# Samples+=( ttWJets ttZJets WW WZ ZZ )                      # rare  : ttV + diboson
# Samples+=( Signal_T2tt )

# INDIR=/nfs-7/userdata/sicheng/stopbabies/merged_v25_9
INDIR=/nfs-7/userdata/sicheng/stopbabies/skimmed_v29_11
# OUTDIR=output/samp16_80x_11
LOGDIR=$OUTDIR/logs
OUTDIR16=$OUTDIR

declare -a Samples=()
Samples+=( TTJets_2lep_80X_ext1_0 TTJets_2lep_80X_ext1_1)
Samples+=( TTJets_1lep_t TTJets_1lep_tbar )
Samples+=( ST_tW_top ST_tW_antitop )      # singleT
Samples+=( W1Jets W2Jets W3Jets W4Jets)       # Vjets : Wjets + DY
Samples+=( TTZToLLNuNu TTWJetsToLNu WZTo1L3Nu )      # rare  : ttV + diboson

mkdir -p ${OUTDIR}; mkdir -p ${LOGDIR}
for SAMPLE in ${Samples[@]}; do
    # ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
    echo ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} '>&' ${LOGDIR}/log_${SAMPLE}.txt
    eval "nohup nice -n -10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
done


########################
# 2016 signal

INDIR=/nfs-7/userdata/sicheng/stopbabies/skimmed_v29_14
# INDIR=/nfs-7/userdata/stopRun2/analysis2016_SUS-16-051_35p9fbinv/v22/skim
# INDIR=/nfs-7/userdata/stopRun2/analysis2016_SUS-16-051_35p9fbinv/v22_usegenMET/skim
# OUTDIR=output/fastsim80X
# OUTDIR=output/temp12
# LOGDIR=$OUTDIR/logs

declare -a Samples=()
Samples+=( SMS_T2tt_400to1200_80X_0 SMS_T2tt_400to1200_80X_1 SMS_T2tt_400to1200_80X_2 )
Samples+=( SMS_T2tt_400to1200_80X_3 SMS_T2tt_400to1200_80X_4 )
Samples+=( SMS_T2bW_80X_0 SMS_T2bW_80X_1 SMS_T2bW_80X_2 SMS_T2bW_80X_3 SMS_T2bW_80X_4 )
Samples+=( SMS_T2bt_80X_0 SMS_T2bt_80X_1 SMS_T2bt_80X_2 SMS_T2bt_80X_3 )
# Samples+=( Signal_T2tt Signal_T2bW Signal_T2tb )

mkdir -p ${OUTDIR}; mkdir -p ${LOGDIR}
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

echo -e 'All looper jobs done!\a'

# pushd output/temp2018data1/
# hadd -f data_2018_all.root  data_2018?.root > /dev/null
# popd > /dev/null
# pushd output/temp2017data1/
# hadd -f data_2017_all.root  data_2017?.root > /dev/null
# popd > /dev/null
# pushd output/temp2016data9/
# # hadd -f data_2016_all.root  data_2016?.root > /dev/null
# cp data_single_lepton_met.root data_2016_all.root 
# popd > /dev/null

# Local merge for the v28_6+ babies
pushd ${OUTDIR17}
hadd -f ttbar_17.root    TTJets*.root > /dev/null
hadd -f singleT_17.root  ST_*.root  > /dev/null
hadd -f Vjets_17.root    W?Jets*.root > /dev/null
hadd -f rare_17.root     TTZ*.root WZ*.root TTW*.root > /dev/null
hadd -f allBkg_17.root   ttbar_17.root singleT_17.root Vjets_17.root rare_17.root > /dev/null
hadd -f allData_17.root  data_2017*.root > /dev/null
popd > /dev/null

pushd ${OUTDIR16}
hadd -f ttbar_16.root    TTJets*.root > /dev/null
hadd -f singleT_16.root  ST_*.root  > /dev/null
hadd -f Vjets_16.root    W?Jets*.root > /dev/null
hadd -f rare_16.root     TTZ*.root TTW*.root WZ*.root > /dev/null
hadd -f allBkg_16.root   ttbar_16.root singleT_16.root Vjets_16.root rare_16.root > /dev/null
hadd -f allData_16.root  data_2016*.root > /dev/null
hadd -f SMS_T2tt.root      SMS_T2tt_*.root > /dev/null
hadd -f SMS_T2bW.root      SMS_T2bW_*.root > /dev/null
hadd -f SMS_T2tb.root      SMS_T2tb_*.root > /dev/null
popd > /dev/null

# # Local merge for the v25_9 babies
# pushd ${OUTDIR}
# hadd -f ttbar_16.root    ttbar_*Lept*.root > /dev/null
# hadd -f singleT_16.root  t_tW_*.root  > /dev/null
# hadd -f Vjets_16.root    W1Jets*.root W2Jets*.root W3Jets*.root W4Jets*.root > /dev/null
# hadd -f rare_16.root     TTZ*.root WZ*.root  > /dev/null
# hadd -f allBkg_16.root   ttbar_16.root singleT_16.root Vjets_16.root rare_16.root > /dev/null
# hadd -f allData_16.root  data_2016*.root > /dev/null
# # hadd -f SMS_T2tt.root      SMS_T2tt_mStop_400to1200_madgraph_?.root > /dev/null
# # hadd -f SMS_T2bW.root      SMS_T2bW_madgraph_?.root > /dev/null
# # hadd -f SMS_T2tb.root      SMS_T2tb_madgraph_?.root > /dev/null
# # rm SMS_T2*_madgraph_?.root
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
