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

# skimtype=skimmed
skimtype=merged

vsuf=v31_s21_cf4

OUTDIR18=output/samp18_$vsuf
OUTDIR17=output/samp17_$vsuf
OUTDIR16=output/samp16_$vsuf
OUTDIRRUN2=output/combRun2_$vsuf

run18dat=0
run18bkg=0
run18sig=0

run17dat=0
run17bkg=0
run17sig=0

run16dat=0
run16bkg=0
run16sig=0

# OUTDIRRUN2=""

function runLooperJobs {
    # [[ -d ${OUTDIR} ]] && rm -r ${OUTDIR}
    mkdir -p ${OUTDIR}; mkdir -p ${LOGDIR}
    cp do.sh StopLooper.cc StopRegions.cc ${LOGDIR}
    for SAMPLE in ${Samples[@]}; do
        # ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR}
        echo ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} '>&' ${LOGDIR}/log_${SAMPLE}.txt
        eval "nohup nice -n 10 ./runStopLooper ${INDIR} ${SAMPLE} ${OUTDIR} >& ${LOGDIR}/log_${SAMPLE}.txt &"
    done
}

########################
# 2018 Data

# INDIR=/nfs-7/userdata/sicheng/stopbabies/$skimtype/data_v30_10
INDIR=/nfs-7/userdata/sicheng/stopbabies/$skimtype/data_v31_2
# INDIR=/hadoop/cms/store/user/sicheng/ProjectMetis/stopbaby_data_2018D_met_v31_2
OUTDIR=$OUTDIR18
# INDIR=/nfs-7/userdata/sicheng/stopbabies/merged/data_v30_3/jetht
# OUTDIR=output/jetht_v30
LOGDIR=$OUTDIR/logs

declare -a Samples=(data_2018A data_2018B data_2018C data_2018D)
# declare -a Samples=(data_2018)
# declare -a Samples=(stopbaby)
# declare -a Samples=(data_2018A_jetht data_2018B_jetht data_2018C_jetht data_2018D_jetht)
# declare -a Samples=(data_2018 data_2017 data_2016)

[[ $run18dat == 1 ]] && runLooperJobs

########################
# 2018 MC

# INDIR=/nfs-7/userdata/sicheng/stopbabies/$skimtype/a18v1_v30_10
INDIR=/nfs-7/userdata/sicheng/stopbabies/$skimtype/a18v1_v31_2
# INDIR=/nfs-7/userdata/sicheng/stopbabies/$skimtype/ttbar_v31_8_ge1j
# OUTDIR=output/samp18_v30
# LOGDIR=$OUTDIR/logs

# # declare -a Samples=(TTJets_amcnlo_0 TTJets_amcnlo_1 TTJets_amcnlo_2)
declare -a Samples=()
Samples+=( TTJets_2lep_a18v1_0 TTJets_2lep_a18v1_1 TTJets_2lep_a18v1_2 TTJets_2lep_a18v1_3 TTJets_2lep_a18v1_4 )
Samples+=( TTJets_1lep_top TTJets_1lep_tbar )
Samples+=( ST_tW_top ST_tW_tbar ST_schan ST_tchan )   # singleT
Samples+=( W1Jets W2Jets W3Jets W4Jets DYJets )       # Vjets : Wjets + DY
Samples+=( TTZToLLNuNu TTWJetsToLNu )      # rare  : ttV
Samples+=( WZTo1L3Nu WZTo3LNu WZTo2L2Q WWToLNuQQ WWTo2L2Nu ZZ )   # diboson

# Samples+=( W1JetsToLNu W2JetsToLNu W3JetsToLNu W4JetsToLNu DYJets )       # Vjets : Wjets + DY
# Samples+=( W1Jets_NuPt200 W2Jets_NuPt200 W3Jets_NuPt200  )       # Vjets : Wjets + DY
# Samples+=( TTJets_1lep_top_a18v1_0 TTJets_1lep_top_a18v1_1 TTJets_1lep_top_a18v1_2 )
# Samples+=( TTJets_1lep_tbar_a18v1_0 TTJets_1lep_tbar_a18v1_1 TTJets_1lep_tbar_a18v1_2 )
# Samples+=( W1Jets W2Jets W3Jets W4JetsToLNu DYJets )
# Samples+=( W4Jets_NuPt200_a18v1_0 W4Jets_NuPt200_a18v1_1 W4Jets_NuPt200_a18v1_2 W4Jets_NuPt200_a18v1_3 )
# Samples+=( W4Jets_NuPt200_a18v1_4 W4Jets_NuPt200_a18v1_5 W4Jets_NuPt200_a18v1_6 )
# Samples+=( TTTo2L2Nu )

[[ $run18bkg == 1 ]] && runLooperJobs

########################
# 2018 Signal <-- use f17v2_ext1 until a18 ones come out

# INDIR=/nfs-7/userdata/sicheng/stopbabies/$skimtype/f17v2_v30_8
# INDIR=/nfs-7/userdata/sicheng/stopbabies/$skimtype/f17v2_v31_2
INDIR=/nfs-7/userdata/sicheng/stopbabies/$skimtype/signal_v31_8
# INDIR=/nfs-7/userdata/sicheng/stopbabies/$skimtype/fastsim_v31_6
# declare -a Samples=(T2tt_mStop850_mLSP100 T2tt_mStop650_mLSP350 T2tt_mStop1200_mLSP100)
declare -a Samples=()

Samples+=( SMS_T2tt_mStop1200to2000_a18v1_0 SMS_T2tt_mStop1200to2000_a18v1_1 SMS_T2tt_mStop1200to2000_a18v1_2 )
Samples+=( SMS_T2tt_mStop1200to2000_a18v1_3 SMS_T2tt_mStop1200to2000_a18v1_4 )
Samples+=( SMS_T2tt_mStop400to1200_a18v1_0 SMS_T2tt_mStop400to1200_a18v1_1 SMS_T2tt_mStop400to1200_a18v1_2 SMS_T2tt_mStop400to1200_a18v1_3  )
Samples+=( SMS_T2tt_mStop400to1200_a18v1_4 SMS_T2tt_mStop400to1200_a18v1_5 SMS_T2tt_mStop400to1200_a18v1_6 SMS_T2tt_mStop400to1200_a18v1_7 )
Samples+=( SMS_T2tt_mStop400to1200_a18v1_8 SMS_T2tt_mStop400to1200_a18v1_9 )
Samples+=( SMS_T2tt_mStop350to400_a18v1_0 SMS_T2tt_mStop350to400_a18v1_1 SMS_T2tt_mStop350to400_a18v1_2 SMS_T2tt_mStop350to400_a18v1_3 )
Samples+=( SMS_T2tt_mStop350to400_a18v1_4 SMS_T2tt_mStop350to400_a18v1_5 SMS_T2tt_mStop350to400_a18v1_6 SMS_T2tt_mStop350to400_a18v1_7 )
Samples+=( SMS_T2tt_mStop250to350_a18v1_0 SMS_T2tt_mStop250to350_a18v1_1 SMS_T2tt_mStop250to350_a18v1_2 SMS_T2tt_mStop250to350_a18v1_3  )
Samples+=( SMS_T2tt_mStop250to350_a18v1_4 SMS_T2tt_mStop250to350_a18v1_5 )
Samples+=( SMS_T2tt_mStop150to250_a18v1_0 SMS_T2tt_mStop150to250_a18v1_1 SMS_T2tt_mStop150to250_a18v1_2 SMS_T2tt_mStop150to250_a18v1_3 )

Samples+=( SMS_T2bW_a18v1_0 SMS_T2bW_a18v1_1 SMS_T2bW_a18v1_2 SMS_T2bW_a18v1_3 SMS_T2bW_a18v1_4 )
Samples+=( SMS_T2bW_a18v1_5 SMS_T2bW_a18v1_6 SMS_T2bW_a18v1_7 SMS_T2bW_a18v1_8 SMS_T2bW_a18v1_9 )
Samples+=( SMS_T2bt_a18v1_0 SMS_T2bt_a18v1_1 SMS_T2bt_a18v1_2 SMS_T2bt_a18v1_3 SMS_T2bt_a18v1_4 )
Samples+=( SMS_T2bt_a18v1_5 SMS_T2bt_a18v1_6 SMS_T2bt_a18v1_7 )

# Samples+=( SMS_T8bbstausnu_XStau0p25_a18v1_0 SMS_T8bbstausnu_XStau0p25_a18v1_1 SMS_T8bbstausnu_XStau0p25_a18v1_2 )
# Samples+=( SMS_T8bbstausnu_XStau0p25_a18v1_3 SMS_T8bbstausnu_XStau0p25_a18v1_4 SMS_T8bbstausnu_XStau0p25_a18v1_5 )
# Samples+=( SMS_T8bbstausnu_XStau0p25_a18v1_6 SMS_T8bbstausnu_XStau0p25_a18v1_7 SMS_T8bbstausnu_XStau0p25_a18v1_8 )
# Samples+=( SMS_T8bbstausnu_XStau0p25_a18v1_9 SMS_T8bbstausnu_XStau0p25_a18v1_A SMS_T8bbstausnu_XStau0p25_a18v1_B )
# Samples+=( SMS_T8bbstausnu_XStau0p25_a18v1_C SMS_T8bbstausnu_XStau0p25_a18v1_D SMS_T8bbstausnu_XStau0p25_a18v1_E )
# Samples+=( SMS_T8bbstausnu_XStau0p25_a18v1_F )
# Samples+=( SMS_T8bbstausnu_XStau0p5_a18v1_0 SMS_T8bbstausnu_XStau0p5_a18v1_1 SMS_T8bbstausnu_XStau0p5_a18v1_2 )
# Samples+=( SMS_T8bbstausnu_XStau0p5_a18v1_3 SMS_T8bbstausnu_XStau0p5_a18v1_4 SMS_T8bbstausnu_XStau0p5_a18v1_5 )
# Samples+=( SMS_T8bbstausnu_XStau0p5_a18v1_6 SMS_T8bbstausnu_XStau0p5_a18v1_7 SMS_T8bbstausnu_XStau0p5_a18v1_8 )
# Samples+=( SMS_T8bbstausnu_XStau0p5_a18v1_9 SMS_T8bbstausnu_XStau0p5_a18v1_A SMS_T8bbstausnu_XStau0p5_a18v1_B )
# Samples+=( SMS_T8bbstausnu_XStau0p5_a18v1_C SMS_T8bbstausnu_XStau0p5_a18v1_D SMS_T8bbstausnu_XStau0p5_a18v1_E )
# Samples+=( SMS_T8bbstausnu_XStau0p5_a18v1_F SMS_T8bbstausnu_XStau0p5_a18v1_G )
# Samples+=( SMS_T8bbstausnu_XStau0p75_a18v1_0 SMS_T8bbstausnu_XStau0p75_a18v1_1 SMS_T8bbstausnu_XStau0p75_a18v1_2 )
# Samples+=( SMS_T8bbstausnu_XStau0p75_a18v1_3 SMS_T8bbstausnu_XStau0p75_a18v1_4 SMS_T8bbstausnu_XStau0p75_a18v1_5 )
# Samples+=( SMS_T8bbstausnu_XStau0p75_a18v1_6 SMS_T8bbstausnu_XStau0p75_a18v1_7 SMS_T8bbstausnu_XStau0p75_a18v1_8 )
# Samples+=( SMS_T8bbstausnu_XStau0p75_a18v1_9 SMS_T8bbstausnu_XStau0p75_a18v1_A SMS_T8bbstausnu_XStau0p75_a18v1_B )
# Samples+=( SMS_T8bbstausnu_XStau0p75_a18v1_C SMS_T8bbstausnu_XStau0p75_a18v1_D SMS_T8bbstausnu_XStau0p75_a18v1_E )
# Samples+=( SMS_T8bbstausnu_XStau0p75_a18v1_F )

[[ $run18sig == 1 ]] && runLooperJobs

########################
# 2017 Data

# INDIR=/nfs-7/userdata/haweber/stopbabiesInteractive/2017_5p8fbinv
# INDIR=/nfs-7/userdata/haweber/tupler_babies/merged/Stop_1l/v2017/output/
# INDIR=/nfs-7/userdata/sicheng/stopbabies/$skimtype/data_v30_7
# INDIR=/nfs-7/userdata/sicheng/stopbabies/$skimtype/data_v30_3
INDIR=/nfs-7/userdata/sicheng/stopbabies/$skimtype/data_v31_2
OUTDIR=$OUTDIR17
# OUTDIR=output/data17_v30_leptrig
LOGDIR=$OUTDIR/logs

# declare -a Samples=(data_2017)
declare -a Samples=(data_2017B data_2017C data_2017D data_2017E data_2017F)
# declare -a Samples=(data_2017B data_2017C data_2017D data_2017E data_2017F_09May data_2017F_31Mar)

[[ $run17dat == 1 ]] && runLooperJobs

########################
# 2017 MC

# INDIR=/nfs-7/userdata/sicheng/stopbabies/merged_v25_4
# INDIR=/nfs-7/userdata/sicheng/stopbabies/merged_v28_10
# INDIR=/nfs-7/userdata/sicheng/stopbabies/skimmed_v29_13
# INDIR=/hadoop/cms/store/user/sicheng/ProjectMetis/stopbaby_TTTo2L2Nu_f17v2_v31_2/merged
# INDIR=/hadoop/cms/store/user/sicheng/ProjectMetis/stopbaby_TTJets_amcnlo_f17v2_v31_2/merged
# INDIR=/nfs-7/userdata/sicheng/stopbabies/$skimtype/f17v2_v30_9
INDIR=/nfs-7/userdata/sicheng/stopbabies/$skimtype/f17v2_v31_2
# OUTDIR=output/samp17_$output17
LOGDIR=${OUTDIR}/logs

declare -a Samples=()
Samples+=( TTJets_2lep_f17v2_0 TTJets_2lep_f17v2_1 TTJets_2lep_f17v2_2 TTJets_2lep_f17v2_3 TTJets_2lep_f17v2_4 )
Samples+=( TTJets_1lep_top TTJets_1lep_tbar )
Samples+=( W1Jets W2Jets W3Jets W4Jets DYJets )       # Vjets : Wjets + DY
Samples+=( ST_tW_top ST_tW_tbar ST_schan ST_tchan )   # singleT
Samples+=( TTZToLLNuNu TTWJetsToLNu TTWJetsToQQ TTZToQQ TTZToLL_M1to10 ) # rare  : ttV
Samples+=( WZTo1L3Nu WZTo3LNu WZTo2L2Q WZTo1L1Nu2Q WWToLNuQQ WWTo2L2Nu TTWZ ZZ )   # diboson

# Samples+=( TTJets_amcnlo_f17v2_0 TTJets_amcnlo_f17v2_1 TTJets_amcnlo_f17v2_2 TTJets_amcnlo_f17v2_3 TTJets_amcnlo_f17v2_4 )
# Samples+=( TTJets_amcnlo_f17v2_5 TTJets_amcnlo_f17v2_6 TTJets_amcnlo_f17v2_7)
# Samples+=( TTJets_1lep_top_f17v2_0 TTJets_1lep_top_f17v2_1 TTJets_1lep_top_f17v2_2 TTJets_1lep_top_f17v2_3 )
# Samples+=( TTJets_1lep_tbar_f17v2_0 TTJets_1lep_tbar_f17v2_1 TTJets_1lep_tbar_f17v2_2 TTJets_1lep_tbar_f17v2_3 )
# Samples+=( W1Jets W2Jets W3Jets W4JetsToLNu DYJets )       # Vjets : Wjets + DY
# Samples+=( W4Jets_NuPt200_f17v2_0 W4Jets_NuPt200_f17v2_1 W4Jets_NuPt200_f17v2_2 )
# Samples+=( W4Jets_NuPt200_f17v2_3 W4Jets_NuPt200_f17v2_4 W4Jets_NuPt200_f17v2_5 )
# Samples+=( TTTo2L2Nu )

[[ $run17bkg == 1 ]] && runLooperJobs

########################
# 2017 Signal

# INDIR=/nfs-7/userdata/sicheng/stopbabies/$skimtype/f17v2_v31_2
INDIR=/nfs-7/userdata/sicheng/stopbabies/$skimtype/signal_v31_8
# INDIR=/nfs-7/userdata/sicheng/stopbabies/$skimtype/fastsim_v31_6
# declare -a Samples=(T2tt_mStop850_mLSP100 T2tt_mStop650_mLSP350 T2tt_mStop1200_mLSP100)
declare -a Samples=()

Samples+=( SMS_T2tt_mStop1200to2000_f17v2_ext0_0 SMS_T2tt_mStop1200to2000_f17v2_ext0_1 SMS_T2tt_mStop1200to2000_f17v2_ext0_2 )
Samples+=( SMS_T2tt_mStop400to1200_f17v2_ext0_0 SMS_T2tt_mStop400to1200_f17v2_ext0_1 SMS_T2tt_mStop400to1200_f17v2_ext0_2 )
Samples+=( SMS_T2tt_mStop400to1200_f17v2_ext0_3 SMS_T2tt_mStop400to1200_f17v2_ext0_4 )
Samples+=( SMS_T2tt_mStop400to1200_f17v2_ext0_5 SMS_T2tt_mStop400to1200_f17v2_ext0_6 )
Samples+=( SMS_T2tt_mStop350to400_f17v2_ext0_0 SMS_T2tt_mStop350to400_f17v2_ext0_1 SMS_T2tt_mStop350to400_f17v2_ext0_2 )
Samples+=( SMS_T2tt_mStop350to400_f17v2_ext0_3 SMS_T2tt_mStop350to400_f17v2_ext0_4 )
Samples+=( SMS_T2tt_mStop250to350_f17v2_ext0_0 SMS_T2tt_mStop250to350_f17v2_ext0_1 )
Samples+=( SMS_T2tt_mStop250to350_f17v2_ext0_2 SMS_T2tt_mStop250to350_f17v2_ext0_3 )
Samples+=( SMS_T2tt_mStop150to250_f17v2_ext0_0 SMS_T2tt_mStop150to250_f17v2_ext0_1 )

Samples+=( SMS_T2bW_f17v2_0 SMS_T2bW_f17v2_1 SMS_T2bW_f17v2_2 SMS_T2bW_f17v2_3 )
Samples+=( SMS_T2bW_f17v2_4 SMS_T2bW_f17v2_5 SMS_T2bW_f17v2_6 SMS_T2bW_f17v2_7 )
Samples+=( SMS_T2bt_f17v2_0 SMS_T2bt_f17v2_1 SMS_T2bt_f17v2_2 SMS_T2bt_f17v2_3 SMS_T2bt_f17v2_4 )

# Samples+=( SMS_T8bbstausnu_XStau0p25_f17v2_0 SMS_T8bbstausnu_XStau0p25_f17v2_1 SMS_T8bbstausnu_XStau0p25_f17v2_2 )
# Samples+=( SMS_T8bbstausnu_XStau0p25_f17v2_3 SMS_T8bbstausnu_XStau0p25_f17v2_4 SMS_T8bbstausnu_XStau0p25_f17v2_5 )
# Samples+=( SMS_T8bbstausnu_XStau0p25_f17v2_6 SMS_T8bbstausnu_XStau0p25_f17v2_7 SMS_T8bbstausnu_XStau0p25_f17v2_8 )
# Samples+=( SMS_T8bbstausnu_XStau0p25_f17v2_9 )
# Samples+=( SMS_T8bbstausnu_XStau0p5_f17v2_0 SMS_T8bbstausnu_XStau0p5_f17v2_1 SMS_T8bbstausnu_XStau0p5_f17v2_2 )
# Samples+=( SMS_T8bbstausnu_XStau0p5_f17v2_3 SMS_T8bbstausnu_XStau0p5_f17v2_4 SMS_T8bbstausnu_XStau0p5_f17v2_5 )
# Samples+=( SMS_T8bbstausnu_XStau0p5_f17v2_6 SMS_T8bbstausnu_XStau0p5_f17v2_7 SMS_T8bbstausnu_XStau0p5_f17v2_8 )
# Samples+=( SMS_T8bbstausnu_XStau0p5_f17v2_9 SMS_T8bbstausnu_XStau0p5_f17v2_A )
# Samples+=( SMS_T8bbstausnu_XStau0p75_f17v2_0 SMS_T8bbstausnu_XStau0p75_f17v2_1 SMS_T8bbstausnu_XStau0p75_f17v2_2 )
# Samples+=( SMS_T8bbstausnu_XStau0p75_f17v2_3 SMS_T8bbstausnu_XStau0p75_f17v2_4 SMS_T8bbstausnu_XStau0p75_f17v2_5 )
# Samples+=( SMS_T8bbstausnu_XStau0p75_f17v2_6 SMS_T8bbstausnu_XStau0p75_f17v2_7 SMS_T8bbstausnu_XStau0p75_f17v2_8 )
# Samples+=( SMS_T8bbstausnu_XStau0p75_f17v2_9 SMS_T8bbstausnu_XStau0p75_f17v2_A )

[[ $run17sig == 1 ]] && runLooperJobs

########################
# 2016 data

# INDIR=/nfs-7/userdata/stopRun2/analysis2016_SUS-16-051_35p9fbinv/v24/output
# INDIR=/nfs-7/userdata/sicheng/stopbabies/merged_v25_10
# INDIR=/nfs-7/userdata/sicheng/stopbabies/skimmed_v29_14
INDIR=/nfs-7/userdata/sicheng/stopbabies/$skimtype/data_v31_8
OUTDIR=$OUTDIR16
# OUTDIR=output/samp16_v30_s3
# OUTDIR=output/data16_jetht
LOGDIR=${OUTDIR}/logs
# cd ../StopCORE; cp stop_1l_babyAnalyzer.h.old stop_1l_babyAnalyzer.h; cp stop_1l_babyAnalyzer.cc.old stop_1l_babyAnalyzer.cc; mkc; cd -

# declare -a Samples=(all_2016_samples)
declare -a Samples=(data_2016B data_2016C data_2016D data_2016E data_2016F data_2016G data_2016H)
# declare -a Samples=(data_2016)

[[ $run16dat == 1 ]] && runLooperJobs

########################
# 2016 MC

# # Moriond17 samples
# INDIR=/nfs-7/userdata/stopRun2/analysis2016_SUS-16-051_35p9fbinv/v22/skim
# declare -a Samples=()
# Samples+=( ttbar_diLept  )    # ttbar
# Samples+=( ttbar_singleLeptFromT_madgraph_pythia8_ext1 ttbar_singleLeptFromTbar_madgraph_pythia8_ext1 )    # ttbar
# Samples+=( t_sch_4f t_tch_4f tbar_tch t_tW_5f t_tbarW_5f ) # singleT
# Samples+=( W1Jets W2Jets W3Jets W4Jets DYJets )            # Vjets : Wjets + DY
# Samples+=( ttWJets ttZJets WW WZ ZZ )                      # rare  : ttV + diboson
# # Samples+=( Signal_T2tt )

# INDIR=/nfs-7/userdata/sicheng/stopbabies/skimmed_v25_9
# INDIR=/nfs-7/userdata/sicheng/stopbabies/skimmed_v29_11
# INDIR=/nfs-7/userdata/sicheng/stopbabies/skimmed/s16v3_v29_14
# INDIR=/nfs-7/userdata/sicheng/stopbabies/$skimtype/s16v3_v30_9
INDIR=/nfs-7/userdata/sicheng/stopbabies/$skimtype/s16v3_v31_2
# OUTDIR=output/samp16_$output16
# LOGDIR=$OUTDIR/logs

declare -a Samples=()
Samples+=( TTJets_2lep_s16v3_ext0_1 TTJets_2lep_s16v3_ext1_0 TTJets_2lep_s16v3_ext1_1 )
Samples+=( TTJets_2lep_s16v3_ext1_2 TTJets_2lep_s16v3_ext1_3 )
Samples+=( TTJets_1lep_top TTJets_1lep_tbar )
Samples+=( ST_tW_top ST_tW_tbar ST_tchan ST_schan )    # singleT
Samples+=( W1Jets W2Jets W3Jets W4Jets DYJets )       # Vjets : Wjets + DY
Samples+=( TTZToLLNuNu TTWJetsToLNu TTWJetsToQQ TTZToQQ )  # rare  : ttV
Samples+=( WZTo1L3Nu WZTo1L1Nu2Q WZTo3LNu_amcnlo WZTo2L2Q WWTo2L2Nu WWToLNuQQ ZZ )      # rare : smaller contributions
# Samples+=( WZTo1L3Nu WZTo1L1Nu2Q WZTo3LNu_powheg WZTo2L2Q WWTo2L2Nu WWToLNuQQ ZZ )      # rare : smaller contributions

# Samples+=( TTJets_1lep_tbar_s16v3_ext0_1 TTJets_1lep_tbar_s16v3_ext1_0 )
# Samples+=( TTJets_1lep_tbar_s16v3_ext1_1 TTJets_1lep_tbar_s16v3_ext1_2 )
# Samples+=( TTJets_1lep_top_s16v3_ext0_1 TTJets_1lep_top_s16v3_ext1_0 )
# Samples+=( TTJets_1lep_top_s16v3_ext1_1 TTJets_1lep_top_s16v3_ext1_2 )
# Samples+=( W4Jets_NuPt200_s16v3_0 W4Jets_NuPt200_s16v3_1 W4Jets_NuPt200_s16v3_2 )
# Samples+=( W4Jets_NuPt200_s16v3_3 W4Jets_NuPt200_s16v3_4 W4Jets_NuPt200_s16v3_5 )
# Samples+=( W1Jets W2Jets W3Jets W4JetsToLNu DYJets )       # Vjets : Wjets + DY
# Samples+=( W1Jets_NuPt200 W2Jets_NuPt200 W3Jets_NuPt200 )       # Vjets : Wjets + DY
# Samples+=( W1JetsToLNu W2JetsToLNu W3JetsToLNu W4JetsToLNu DYJets )       # Vjets : Wjets + DY
# Samples+=( TTTo2L2Nu )

[[ $run16bkg == 1 ]] && runLooperJobs

########################
# 2016 signal

# INDIR=/nfs-7/userdata/sicheng/stopbabies/$skimtype/s16v3_v30_9
# INDIR=/nfs-7/userdata/sicheng/stopbabies/$skimtype/s16v3_v31_2
INDIR=/nfs-7/userdata/sicheng/stopbabies/$skimtype/signal_v31_8
# INDIR=/nfs-7/userdata/sicheng/stopbabies/$skimtype/fastsim_v31_6
# INDIR=/nfs-7/userdata/stopRun2/analysis2016_SUS-16-051_35p9fbinv/v22/skim
# INDIR=/nfs-7/userdata/stopRun2/analysis2016_SUS-16-051_35p9fbinv/v22_usegenMET/skim
# OUTDIR=output/temp12
# LOGDIR=$OUTDIR/logs

declare -a Samples=()

Samples+=( SMS_T2tt_mStop1200to2000_s16v3_0 SMS_T2tt_mStop1200to2000_s16v3_1 SMS_T2tt_mStop1200to2000_s16v3_2 )
Samples+=( SMS_T2tt_mStop400to1200_s16v3_0 SMS_T2tt_mStop400to1200_s16v3_1 SMS_T2tt_mStop400to1200_s16v3_2 )
Samples+=( SMS_T2tt_mStop400to1200_s16v3_3 SMS_T2tt_mStop400to1200_s16v3_4 )
Samples+=( SMS_T2tt_mStop400to1200_s16v3_5 SMS_T2tt_mStop400to1200_s16v3_6 )
Samples+=( SMS_T2tt_mStop350to400_s16v3_0 SMS_T2tt_mStop350to400_s16v3_1 SMS_T2tt_mStop350to400_s16v3_2 )
Samples+=( SMS_T2tt_mStop350to400_s16v3_3 SMS_T2tt_mStop350to400_s16v3_4)
Samples+=( SMS_T2tt_mStop250to350_s16v3_0 SMS_T2tt_mStop250to350_s16v3_1 )
Samples+=( SMS_T2tt_mStop250to350_s16v3_2 SMS_T2tt_mStop250to350_s16v3_3 )
Samples+=( SMS_T2tt_mStop150to250_s16v3_0 SMS_T2tt_mStop150to250_s16v3_1)

Samples+=( SMS_T2bW_s16v3_0 SMS_T2bW_s16v3_1 SMS_T2bW_s16v3_2 SMS_T2bW_s16v3_3 )
Samples+=( SMS_T2bW_s16v3_4 SMS_T2bW_s16v3_5 SMS_T2bW_s16v3_6 )
Samples+=( SMS_T2bt_s16v3_0 SMS_T2bt_s16v3_1 SMS_T2bt_s16v3_2 SMS_T2bt_s16v3_3 SMS_T2bt_s16v3_4 )

# Samples+=( SMS_T8bbstausnu_XStau0p25_s16v3_0 SMS_T8bbstausnu_XStau0p25_s16v3_1 SMS_T8bbstausnu_XStau0p25_s16v3_2 )
# Samples+=( SMS_T8bbstausnu_XStau0p25_s16v3_3 SMS_T8bbstausnu_XStau0p25_s16v3_4 SMS_T8bbstausnu_XStau0p25_s16v3_5 )
# Samples+=( SMS_T8bbstausnu_XStau0p25_s16v3_6 SMS_T8bbstausnu_XStau0p25_s16v3_7 SMS_T8bbstausnu_XStau0p25_s16v3_8 )
# Samples+=( SMS_T8bbstausnu_XStau0p25_s16v3_9 )
# Samples+=( SMS_T8bbstausnu_XStau0p5_s16v3_0 SMS_T8bbstausnu_XStau0p5_s16v3_1 SMS_T8bbstausnu_XStau0p5_s16v3_2 )
# Samples+=( SMS_T8bbstausnu_XStau0p5_s16v3_3 SMS_T8bbstausnu_XStau0p5_s16v3_4 SMS_T8bbstausnu_XStau0p5_s16v3_5 )
# Samples+=( SMS_T8bbstausnu_XStau0p5_s16v3_6 SMS_T8bbstausnu_XStau0p5_s16v3_7 SMS_T8bbstausnu_XStau0p5_s16v3_8 )
# Samples+=( SMS_T8bbstausnu_XStau0p5_s16v3_9 )
# Samples+=( SMS_T8bbstausnu_XStau0p75_s16v3_0 SMS_T8bbstausnu_XStau0p75_s16v3_1 SMS_T8bbstausnu_XStau0p75_s16v3_2 )
# Samples+=( SMS_T8bbstausnu_XStau0p75_s16v3_3 SMS_T8bbstausnu_XStau0p75_s16v3_4 SMS_T8bbstausnu_XStau0p75_s16v3_5 )
# Samples+=( SMS_T8bbstausnu_XStau0p75_s16v3_6 SMS_T8bbstausnu_XStau0p75_s16v3_7 SMS_T8bbstausnu_XStau0p75_s16v3_8 )
# Samples+=( SMS_T8bbstausnu_XStau0p75_s16v3_9 )

# Samples+=( SMS_T2tt_400to1200_80X_0 SMS_T2tt_400to1200_80X_1 SMS_T2tt_400to1200_80X_2 )
# Samples+=( SMS_T2tt_400to1200_80X_3 SMS_T2tt_400to1200_80X_4 )
# Samples+=( SMS_T2bt_80X_0 SMS_T2bt_80X_1 SMS_T2bt_80X_2 SMS_T2bt_80X_3 )

[[ $run16sig == 1 ]] && runLooperJobs

# # Extra
# INDIR=/nfs-7/userdata/sicheng/stopbabies/$skimtype/ttbar_v31_8_ge1j
# OUTDIR=output/samp1x_v31_ttagtest4
# LOGDIR=${OUTDIR}/logs
# declare -a Samples=()
# Samples+=( TTJets_2lep_a18v1fs TTJets_1lep_top_a18v1fs TTJets_1lep_tbar_a18v1fs )
# Samples+=( TTJets_2lep_f17v2fs TTJets_1lep_top_f17v2fs TTJets_1lep_tbar_f17v2fs )
# Samples+=( TTJets_2lep_s16v3fs TTJets_1lep_top_s16v3fs TTJets_1lep_tbar_s16v3fs )
# Samples+=( TTJets_2lep_a18v1_emu1J_0 TTJets_2lep_a18v1_emu1J_1 TTJets_2lep_a18v1_emu1J_2 )
# Samples+=( TTJets_2lep_f17v2_emu1J_0 TTJets_2lep_f17v2_emu1J_1 TTJets_2lep_f17v2_emu1J_2 )
# Samples+=( TTJets_2lep_s16v3_ext1_emu1J_0 TTJets_2lep_s16v3_ext1_emu1J_2 )
# runLooperJobs

INDIR=/nfs-7/userdata/sicheng/stopbabies/$skimtype/ttbar_v31_9
OUTDIR=output/samp1x_v31_test11
LOGDIR=${OUTDIR}/logs
declare -a Samples=()
Samples+=( TTJets_1lep_top_a18v1fs TTJets_1lep_tbar_a18v1fs )
Samples+=( TTJets_1lep_top_f17v2fs TTJets_1lep_tbar_f17v2fs )
Samples+=( TTJets_1lep_top_s16v3fs TTJets_1lep_tbar_s16v3fs )
Samples+=( TTJets_1lep_top_a18v1_1 TTJets_1lep_tbar_a18v1_1 )
Samples+=( TTJets_1lep_top_f17v2_1 TTJets_1lep_tbar_f17v2_1 )
Samples+=( TTJets_1lep_top_s16v3_ext0_1 TTJets_1lep_tbar_s16v3_ext0_1 )
Samples+=( TTJets_1lep_top_a18v1_2 TTJets_1lep_tbar_a18v1_2 )
Samples+=( TTJets_1lep_top_f17v2_2 TTJets_1lep_tbar_f17v2_2 )
Samples+=( TTJets_1lep_top_a18v1_0 TTJets_1lep_tbar_a18v1_0 )
Samples+=( TTJets_1lep_top_f17v2_0 TTJets_1lep_tbar_f17v2_0 )
Samples+=( TTTo2L2Nu_a18v1_1 TTTo2L2Nu_a18v1_2 TTTo2L2Nu_a18v1_3 )
Samples+=( TTTo2L2Nu_a18v1_4 TTTo2L2Nu_a18v1_5 TTTo2L2Nu_a18v1_6 TTTo2L2Nu_a18v1_7 )
Samples+=( TTTo2L2Nu_a18v1_8 TTTo2L2Nu_a18v1_9 TTTo2L2Nu_f17v2_0 TTTo2L2Nu_f17v2_1 )
Samples+=( TTTo2L2Nu_s16v3_0 TTTo2L2Nu_s16v3_1 TTTo2L2Nu_s16v3_2 TTTo2L2Nu_s16v3_3 )
# runLooperJobs

# INDIR=/nfs-7/userdata/sicheng/stopbabies/$skimtype/signal_v31_9
# OUTDIR=output/samp1x_v31_ttagtest9
# LOGDIR=${OUTDIR}/logs
# declare -a Samples=()
# Samples+=( T2tt_mStop1200_mLSP100_f17v2 T2tt_mStop650_mLSP350_a18v1 T2tt_mStop650_mLSP350_f17v2 ) 
# Samples+=( T2tt_mStop650_mLSP350_s16v3 T2tt_mStop850_mLSP100_f17v2 T2tt_mStop850_mLSP100_s16v3 )
# Samples+=( T2tt_mStop1200_mLSP100_a18v1 )
# runLooperJobs

INDIR=/nfs-7/userdata/sicheng/stopbabies/merged/corsig_v32_0
OUTDIR=output/samp1x_v32_s1_cf
LOGDIR=${OUTDIR}/logs
declare -a Samples=()
Samples+=( T2tt_3J_xqcut20_mStop175_mLSP1_s16v3 T2tt_3J_xqcut20_mStop250_mLSP50_s16v3 T2tt_3J_xqcut20_mStop250_mLSP75_s16v3 )
Samples+=( T2tt_3J_xqcut20_mStop175_mLSP1_f17v2 T2tt_3J_xqcut20_mStop250_mLSP50_f17v2 T2tt_3J_xqcut20_mStop250_mLSP75_f17v2 )
# Samples+=( T2tt_3J_xqcut20_mStop250_mLSP100_f17v2 )
Samples+=( T2tt_3J_xqcut20_mStop175_mLSP1_a18v1 T2tt_3J_xqcut20_mStop250_mLSP50_a18v1 T2tt_3J_xqcut20_mStop250_mLSP75_a18v1 )
# Samples+=( T2tt_3J_xqcut20_mStop250_mLSP100_a18v1 )
# ( T2tt_3J_xqcut20_mStop150_mLSP50_s16v3 T2tt_3J_xqcut20_mStop225_mLSP50_s16v3 T2tt_3J_xqcut20_mStop200_mLSP50_s16v3 )
# ( T2tt_3J_xqcut20_mStop250_mLSP150_s16v3 T2tt_3J_xqcut20_mStop300_mLSP150_s16v3 T2tt_3J_xqcut20_mStop325_mLSP150_s16v3 )
# ( T2tt_3J_xqcut20_mStop350_mLSP150_s16v3 )
# runLooperJobs

########################
# Merge output samples with hadd
########################

# Monitor the running
while : ; do
    sleep 5
    RunningJobs=`jobs | grep runStopLooper`
    [[ $RunningJobs == "" ]] && break
done

echo -e 'All looper jobs done!\a'
# Local merge for the v29_11+ babies
pushd ${OUTDIR18}
[[ $run18bkg == 1 ]] && hadd -f tt2l_18.root     TTJets_2lep*.root > /dev/null
[[ $run18bkg == 1 ]] && hadd -f tt1l_18.root     TTJets_1lep*.root > /dev/null
[[ $run18bkg == 1 ]] && hadd -f ttbar_18.root    TTJets*.root > /dev/null
[[ $run18bkg == 1 ]] && hadd -f singleT_18.root  ST_*.root  > /dev/null
# [[ $run18bkg == 1 ]] && hadd -f Vjets_18.root    W?Jets*.root DYJets*.root > /dev/null
[[ $run18bkg == 1 ]] && hadd -f Vjets_18.root    W?Jets*.root > /dev/null
[[ $run18bkg == 1 ]] && hadd -f rare_18.root     TTZ*.root TTW*.root WZ*.root WW*.root ZZ*.root > /dev/null
[[ $run18bkg == 1 ]] && hadd -f allBkg_18.root   ttbar_18.root singleT_18.root Vjets_18.root rare_18.root > /dev/null
[[ $run18dat == 1 ]] && hadd -f allData_18.root  data_2018*.root > /dev/null
[[ $run18sig == 1 ]] && hadd -f SMS_T2tt_18.root SMS_T2tt_*to*.root > /dev/null && rm SMS_T2tt_*to*.root
[[ $run18sig == 1 ]] && hadd -f SMS_T2bW_18.root SMS_T2bW_*_?.root > /dev/null && rm SMS_T2bW_*_?.root
[[ $run18sig == 1 ]] && hadd -f SMS_T2bt_18.root SMS_T2bt_*_?.root > /dev/null && rm SMS_T2bt_*_?.root
# [[ $run18sig == 1 ]] && hadd -f SMS_T8bbstausnu_XStau0p75_18.root SMS_T8bbstausnu_XStau0p75_*_?.root > /dev/null && rm SMS_T8bbstausnu_XStau0p75_*_?.root
# [[ $run18sig == 1 ]] && hadd -f SMS_T8bbstausnu_XStau0p5_18.root SMS_T8bbstausnu_XStau0p5_*_?.root > /dev/null && rm SMS_T8bbstausnu_XStau0p5_*_?.root
# [[ $run18sig == 1 ]] && hadd -f SMS_T8bbstausnu_XStau0p25_18.root SMS_T8bbstausnu_XStau0p25_*_?.root > /dev/null && rm SMS_T8bbstausnu_XStau0p25_*_?.root
popd > /dev/null

# Local merge for the v29_11+ babies
pushd ${OUTDIR17}
[[ $run17bkg == 1 ]] && hadd -f tt2l_17.root     TTJets_2lep*.root > /dev/null
[[ $run17bkg == 1 ]] && hadd -f tt1l_17.root     TTJets_1lep*.root > /dev/null
[[ $run17bkg == 1 ]] && hadd -f ttbar_17.root    TTJets_?lep*.root > /dev/null
[[ $run17bkg == 1 ]] && hadd -f singleT_17.root  ST_*.root  > /dev/null
[[ $run17bkg == 1 ]] && hadd -f Vjets_17.root    W?Jets*.root DYJets*.root > /dev/null
[[ $run17bkg == 1 ]] && hadd -f rare_17.root     TTZ*.root TTW*.root WZ*.root WW*.root ZZ*.root > /dev/null
[[ $run17bkg == 1 ]] && hadd -f allBkg_17.root   ttbar_17.root singleT_17.root Vjets_17.root rare_17.root > /dev/null
[[ $run17dat == 1 ]] && hadd -f allData_17.root  data_2017?.root > /dev/null
[[ $run17dat == 1 ]] && hadd -f data_2017BtoE.root data_2017[B-E].root > /dev/null
[[ $run17sig == 1 ]] && hadd -f SMS_T2tt_17.root SMS_T2tt_*to*.root > /dev/null && rm SMS_T2tt_*to*.root
[[ $run17sig == 1 ]] && hadd -f SMS_T2bW_17.root SMS_T2bW_*_?.root > /dev/null && rm SMS_T2bW_*_?.root
[[ $run17sig == 1 ]] && hadd -f SMS_T2bt_17.root SMS_T2bt_*_?.root > /dev/null && rm SMS_T2bt_*_?.root
# [[ $run17sig == 1 ]] && hadd -f SMS_T8bbstausnu_XStau0p75_17.root SMS_T8bbstausnu_XStau0p75_*_?.root > /dev/null && rm SMS_T8bbstausnu_XStau0p75_*_?.root
# [[ $run17sig == 1 ]] && hadd -f SMS_T8bbstausnu_XStau0p5_17.root SMS_T8bbstausnu_XStau0p5_*_?.root > /dev/null && rm SMS_T8bbstausnu_XStau0p5_*_?.root
# [[ $run17sig == 1 ]] && hadd -f SMS_T8bbstausnu_XStau0p25_17.root SMS_T8bbstausnu_XStau0p25_*_?.root > /dev/null && rm SMS_T8bbstausnu_XStau0p25_*_?.root
popd > /dev/null

pushd ${OUTDIR16}
[[ $run16bkg == 1 ]] && hadd -f tt2l_16.root     TTJets_2lep*.root > /dev/null
[[ $run16bkg == 1 ]] && hadd -f tt1l_16.root     TTJets_1lep*.root > /dev/null
[[ $run16bkg == 1 ]] && hadd -f ttbar_16.root    TTJets*.root > /dev/null
[[ $run16bkg == 1 ]] && hadd -f singleT_16.root  ST_*.root  > /dev/null
[[ $run16bkg == 1 ]] && hadd -f Vjets_16.root    W?Jets*.root DYJets*.root > /dev/null
[[ $run16bkg == 1 ]] && hadd -f rare_16.root     TTZ*.root TTW*.root WZ*.root WW*.root ZZ*.root > /dev/null
[[ $run16bkg == 1 ]] && hadd -f allBkg_16.root   ttbar_16.root singleT_16.root Vjets_16.root rare_16.root > /dev/null
[[ $run16dat == 1 ]] && hadd -f allData_16.root  data_2016*.root > /dev/null
[[ $run16sig == 1 ]] && hadd -f SMS_T2tt_16.root SMS_T2tt_*to*.root > /dev/null && rm SMS_T2tt_*to*.root
[[ $run16sig == 1 ]] && hadd -f SMS_T2bW_16.root SMS_T2bW_*_?.root > /dev/null && rm SMS_T2bW_*_?.root
[[ $run16sig == 1 ]] && hadd -f SMS_T2bt_16.root SMS_T2bt_*_?.root > /dev/null && rm SMS_T2bt_*_?.root
# [[ $run16sig == 1 ]] && hadd -f SMS_T8bbstausnu_XStau0p75_16.root SMS_T8bbstausnu_XStau0p75_*_?.root > /dev/null && rm SMS_T8bbstausnu_XStau0p75_*_?.root
# [[ $run16sig == 1 ]] && hadd -f SMS_T8bbstausnu_XStau0p5_16.root SMS_T8bbstausnu_XStau0p5_*_?.root > /dev/null && rm SMS_T8bbstausnu_XStau0p5_*_?.root
# [[ $run16sig == 1 ]] && hadd -f SMS_T8bbstausnu_XStau0p25_16.root SMS_T8bbstausnu_XStau0p25_*_?.root > /dev/null && rm SMS_T8bbstausnu_XStau0p25_*_?.root
popd > /dev/null

# cd output/samp1x_v31_ttagtest4
# hadd -f ttbar_fastsim_16.root TTJ*s16v3fs.root > /dev/null && hadd -f ttbar_16.root TTJ*_s16v3_ext1_emu1J*.root > /dev/null
# hadd -f ttbar_fastsim_17.root TTJ*f17v2fs.root > /dev/null && hadd -f ttbar_17.root TTJ*_f17v2_emu1J_?.root > /dev/null
# hadd -f ttbar_fastsim_18.root TTJ*a18v1fs.root > /dev/null && hadd -f ttbar_18.root TTJ*_a18v1_emu1J_?.root > /dev/null
# cd -

# cd output/samp1x_v31_test11
# hadd -f ttbar_fastsim_16.root TTJ*s16v3fs.root > /dev/null && hadd -f ttbar_16.root TTJ*_s16v3_ext0_?.root > /dev/null
# hadd -f ttbar_fastsim_17.root TTJ*f17v2fs.root > /dev/null && hadd -f ttbar_17.root TTJ*_f17v2_?.root > /dev/null
# hadd -f ttbar_fastsim_18.root TTJ*a18v1fs.root > /dev/null && hadd -f ttbar_18.root TTJ*_a18v1_?.root > /dev/null
# hadd -f tt2l_18.root TTTo2L*a18v1*.root > /dev/null && hadd -f tt2l_17.root TTTo2L*_f17v2_?.root > /dev/null  && hadd -f tt2l_16.root TTTo2L*_s16v3_?.root > /dev/null
# cd -

# cd output/samp16_v31_ttagtest && hadd -f ttbar_fastsim_16.root TTJ*fs.root > /dev/null && hadd -f ttbar_16.root TTJ*v?_ext0_?.root > /dev/null && cd -
# cd output/samp17_v31_ttagtest && hadd -f ttbar_fastsim_17.root TTJ*fs.root > /dev/null && hadd -f ttbar_17.root TTJ*v?_?.root > /dev/null && cd -
# cd output/samp18_v31_ttagtest && hadd -f ttbar_fastsim_18.root TTJ*fs.root > /dev/null && hadd -f ttbar_18.root TTJ*v?_?.root > /dev/null && cd -

# Make combined results
if [ ! ${OUTDIRRUN2} == "" ]; then
    mkdir -p ${OUTDIRRUN2}
    cp ${OUTDIR16}/*_16.root ${OUTDIRRUN2}
    cp ${OUTDIR17}/*_17.root ${OUTDIRRUN2}
    cp ${OUTDIR18}/*_18.root ${OUTDIRRUN2}
    pushd ${OUTDIRRUN2}
    # rm 1lepFrom*.root ZToNuNu_*.root lostlepton_*.root
    for isamp in *_17.root; do
        jsamp=${isamp%_17.root}
        hadd -f ${jsamp}_run2.root ${jsamp}_16.root $isamp ${jsamp}_18.root > /dev/null
    done
    # rm *_1?.root
    popd > /dev/null
fi

# # Local merge for the moriond17 samples
# pushd output/samples2016
# hadd -f ttbar_25ns.root    ttbar*Lept*.root > /dev/null
# hadd -f singleT_25ns.root  t_t*_noHadDecays*.root t*_4f_*.root > /dev/null
# hadd -f Vjets_25ns.root    W?Jets*.root DYJets*m10To50*.root > /dev/null
# hadd -f rare_25ns.root     ttZ*.root WZ*.root ttW*.root WW*.root ZZ*.root > /dev/null
# hadd -f allBkg_25ns.root   ttbar_25ns.root singleT_25ns.root Vjets_25ns.root rare_25ns.root > /dev/null
# popd > /dev/null

echo -e 'All merge jobs done!\a'

unset RunningJobs
