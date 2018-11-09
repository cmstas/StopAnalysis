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
# INDIR=/nfs-7/userdata/sicheng/stopbabies/skimmed_v27_5
INDIR=/nfs-7/userdata/sicheng/stopbabies/merged_v27_5/shuffle
# INDIR=/nfs-7/userdata/sicheng/stopbabies/merged_v27_6
# INDIR=/nfs-7/userdata/sicheng/stopbabies/merged_v27_6/shuffle2
OUTDIR=output/temp2018data2
LOGDIR=$OUTDIR/logs

mkdir -p ${OUTDIR}
mkdir -p ${LOGDIR}

# declare -a Samples=()
# declare -a Samples=(data_2018A data_2018B)
# declare -a Samples=(data_2018B)
# declare -a Samples=(data_2018Av1_met data_2018Bv1_met)
# declare -a Samples=(data_2018Av1_singlemu data_2018Bv1_singlemu)
# Samples+=( data_2018Av1_egamma data_2018Bv1_egamma )
# declare -a Samples=(data_2018A_met data_2018B_met data_2018A_singlemu data_2018A_egamma data_2018B_singlemu data_2018B_egamma data_2018A_jetht data_2018B_jetht)
# declare -a Samples=(data_2018_jetht data_2018_singlemu data_2018_met data_2018_egamma)
declare -a Samples=(data_2018_met)

# for SAMPLE in ${Samples[@]}; do
#     # ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
#     echo ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} '>&' ${LOGDIR}/log_${SAMPLE}.txt
#     eval "nohup nice -n -10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
# done

# 2018 MC
# INDIR=/home/users/sicheng/temp/TopTagger/StopAnalysis/StopBabyMaker
# INDIR=/nfs-7/userdata/sicheng/stopbabies/merged_v28_4
INDIR=/hadoop/cms/store/user/sicheng/ProjectMetis/stopBaby_TTJets_amcnlo_v28_5/
OUTDIR=output/tempNewTagger5
LOGDIR=$OUTDIR/logs

mkdir -p ${OUTDIR}
mkdir -p ${LOGDIR}

# # declare -a Samples=(TTJets_amcnlo_0 TTJets_amcnlo_1 TTJets_amcnlo_2)
# # declare -a Samples=(TTJets_amcnlo_0 TTJets_amcnlo_1 TTJets_amcnlo_2 TTJets_amcnlo_3 TTJets_amcnlo_4 TTJets_amcnlo_5 TTJets_amcnlo_6 TTJets_amcnlo_7)
# declare -a Samples=(stopbaby)
# for SAMPLE in ${Samples[@]}; do
#     ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
#     echo ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} '>&' ${LOGDIR}/log_${SAMPLE}.txt
#     # eval "nohup nice -n -10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
# done

# 2017 Data

# INDIR=/nfs-7/userdata/haweber/stopbabiesInteractive/2017_5p8fbinv
# INDIR=/nfs-7/userdata/haweber/tupler_babies/merged/Stop_1l/v2017/output/
# INDIR=/nfs-7/userdata/sicheng/stopbabies/merged_v25_7
# INDIR=/nfs-7/userdata/sicheng/stopbabies/merged_v27_5/shuffle
# INDIR=/nfs-7/userdata/sicheng/stopbabies/merged_v28_6
# INDIR=/nfs-7/userdata/sicheng/stopbabies/skimmed_v28_6
INDIR=/nfs-7/userdata/sicheng/stopbabies/merged_v29_9
# INDIR=/nfs-7/userdata/sicheng/stopbabies/skimmed_v29_9
OUTDIR=output/samp17_mtest
LOGDIR=$OUTDIR/logs

mkdir -p ${OUTDIR}
mkdir -p ${LOGDIR}

declare -a Samples=(data_2017B data_2017C data_2017D data_2017E data_2017F)
# declare -a Samples=(data_2017B_met data_2017C_met data_2017D_met data_2017E_met data_2017F_met)
# declare -a Samples=(data_2017B_singlemu data_2017C_singlemu data_2017D_singlemu data_2017E_singlemu data_2017F_singlemu)
# Samples+=(data_2017B_singleel data_2017C_singleel data_2017D_singleel data_2017E_singleel data_2017F_singleel)

for SAMPLE in ${Samples[@]}; do
    # ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
    echo ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} '>&' ${LOGDIR}/log_${SAMPLE}.txt
    eval "nohup nice -n -10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
done

# 2017 MC test

# INDIR=/nfs-7/userdata/sicheng/stopbabies/merged_v25_4
# INDIR=/hadoop/cms/store/user/sicheng/ProjectMetis/stopBaby_ttbar_semileptonic_madgraph_v27_2/skimmed
# INDIR=/hadoop/cms/store/user/sicheng/ProjectMetis/stopBaby_TTJets_incl_amcnlo_v29_12/merged
# INDIR=/hadoop/cms/store/user/sicheng/ProjectMetis/stopBaby_TTJets_1lep_tbar_madgraph_v29_4/merged
# INDIR=/nfs-7/userdata/sicheng/stopbabies/merged_v28_10
# INDIR=/nfs-7/userdata/sicheng/stopbabies/skimmed_v28_10
# OUTDIR=output/testResTopJet20_12
LOGDIR=${OUTDIR}/logs

mkdir -p ${OUTDIR}
mkdir -p ${LOGDIR}

declare -a Samples=()
Samples+=( TTJets_2lep_madgraph_0 TTJets_2lep_madgraph_1 TTJets_2lep_madgraph_2 )
Samples+=( TTJets_1lep_t_madgraph_0 TTJets_1lep_t_madgraph_1 TTJets_1lep_t_madgraph_2 )
Samples+=( TTJets_1lep_tbar_madgraph_0 TTJets_1lep_tbar_madgraph_1  )
Samples+=( ST_tW_top ST_tW_antitop ST_schan )      # singleT
Samples+=( W1JetsToLNu W2JetsToLNu W3JetsToLNu W4JetsToLNu)       # Vjets : Wjets + DY
Samples+=( TTZToLLNuNu TTWJetsToLNu WZTo3LNu )      # rare  : ttV + diboson
# declare -a Samples=(TTJets W2Jets W3Jets W4Jets)
# declare -a Samples=(TTJets_incl_amcnlo)
# declare -a Samples=(stopbaby)
# Samples+=( TTJets_incl_amcnlo_0 TTJets_incl_amcnlo_1 TTJets_incl_amcnlo_2 TTJets_incl_amcnlo_3 )  # ttbar incl
# Samples+=( TTJets_incl_amcnlo_0 TTJets_incl_amcnlo_1 TTJets_incl_amcnlo_2 )    # ttbar

for SAMPLE in ${Samples[@]}; do
    # ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
    echo ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} '>&' ${LOGDIR}/log_${SAMPLE}.txt
    eval "nohup nice -n -10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
done

# 2016 data
# INDIR=/nfs-7/userdata/stopRun2/analysis2016_SUS-16-051_35p9fbinv/v24/skim
# INDIR=/nfs-7/userdata/stopRun2/analysis2016_SUS-16-051_35p9fbinv/v24/output
INDIR=/nfs-7/userdata/sicheng/stopbabies/merged_v25_10
# INDIR=/nfs-7/userdata/sicheng/stopbabies/merged_v28_6
# OUTDIR=output/temp
# OUTDIR=output/samp16_incl_80x
# LOGDIR=${OUTDIR}/logs
# cd ../StopCORE; cp stop_1l_babyAnalyzer.h.old stop_1l_babyAnalyzer.h; cp stop_1l_babyAnalyzer.cc.old stop_1l_babyAnalyzer.cc; mkc; cd -

mkdir -p ${OUTDIR}
mkdir -p ${LOGDIR}

# declare -a Samples=(all_2016_samples)
declare -a Samples=(data_2016B data_2016C data_2016D data_2016E data_2016F data_2016G data_2016H)
# declare -a Samples=(data_met)

# for SAMPLE in ${Samples[@]}; do
#     # ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
#     echo ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} '>&' ${LOGDIR}/log_${SAMPLE}.txt
#     eval "nohup nice -n -10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
# done

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
# INDIR=/nfs-7/userdata/sicheng/stopbabies/skimmed_v25_9
# OUTDIR=output/tempI2
# INDIR=/hadoop/cms/store/user/sicheng/ProjectMetis/stopBaby_TTJets_amcnlo_80X_v28_7/merged/
# INDIR=/hadoop/cms/store/user/sicheng/ProjectMetis/stopBaby_TTJets_madgraph_80X_v28_7/merged
# INDIR=../StopBabyMaker
# OUTDIR=output/samp16

mkdir -p ${OUTDIR}
mkdir -p ${LOGDIR}

declare -a Samples=()
Samples+=( ttbar_diLept ttbar_singleLeptFromT_madgraph ttbar_singleLeptFromTbar_madgraph )    # ttbar
Samples+=( t_tW_top t_tW_antitop )      # singleT
Samples+=( W1Jets W2Jets W3Jets W4Jets)       # Vjets : Wjets + DY
Samples+=( TTZToLLNuNu WZTo1L3Nu )            # rare  : ttV + diboson

# declare -a Samples=(ttbar_singleLeptFromT_amcnlo_94X)
# declare -a Samples=(TTJets_amcnlo_80X)

# for SAMPLE in ${Samples[@]}; do
#     # ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
#     echo ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} '>&' ${LOGDIR}/log_${SAMPLE}.txt
#     eval "nohup nice -n -10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
# done


# 2016 signal
# INDIR=/nfs-7/userdata/sicheng/stopbabies/skimmed_v25_9
# INDIR=/nfs-7/userdata/stopRun2/analysis2016_SUS-16-051_35p9fbinv/v22/skim
# INDIR=/nfs-7/userdata/stopRun2/analysis2016_SUS-16-051_35p9fbinv/v22_usegenMET/skim
INDIR=/hadoop/cms/store/user/sicheng/ProjectMetis/stopBaby_SMS_T2tt_mStop-400to1200_madgraph_v29_10
# OUTDIR=output/fastsim80X
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
declare -a Samples=(stopbaby_1 stopbaby_2 stopbaby_3 stopbaby_4 stopbaby_5 stopbaby_6 )

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
pushd ${OUTDIR}
hadd -f ttbar_17.root    TTJets*.root > /dev/null
hadd -f singleT_17.root  ST_*.root  > /dev/null
hadd -f Vjets_17.root    W?Jets*.root > /dev/null
hadd -f rare_17.root     TTZ*.root WZ*.root TTW*.root > /dev/null
hadd -f allBkg_17.root   ttbar_17.root singleT_17.root Vjets_17.root rare_17.root > /dev/null
hadd -f allData_17.root  data_2017*.root > /dev/null
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
