bvsuf=v31_s21_syr
ysuf=16

indir=../StopLooper/output/samp${ysuf}_$bvsuf
outdir=scan_samp${ysuf}_$bvsuf

# root -b -q fakeBkgEstimates.C"(\"$indir\",\"$indir\",\"$ysuf\")" || return $?
# root -b -q ../AnalyzeScripts/makeBkgEstimates.C"(\"$indir\",\"$indir\",\"$ysuf\", \"yearSeparateSyst=false\")" || return $?
# root -b -q ../AnalyzeScripts/makeBkgEstimates.C"(\"$indir\",\"$indir\",\"$ysuf\", \"extr_threshold=1.31\")" || return $?


sigscan=std_T2tt
# root -b -q newCardMaker.C"(\"${sigscan:4}\",\"$indir\",\"datacards/$outdir\",\"$ysuf\")" || return $?
# python locallimits.py "datacards/$outdir" ${sigscan}
# root -b -q Make2DLimitHistos.C"(\"${sigscan}\", \"limits/$outdir\")"
# echo -e "Limits for $sigscan in $outdir is done!\\a"
# mv limits/$outdir/histo limits/$outdir/histo_s16v3_v30

# root -b -q newCardMaker.C"(\"ttbarDM_scalar\",\"../StopLooper/output/samp16_v31_s21_syr\",\"datacards/scan_ttbarDM_scalar_m10_yr16\",\"16\")"
# root -b -q newCardMaker.C"(\"ttbarDM_pseudo\",\"../StopLooper/output/samp16_v31_s21_syr\",\"datacards/scan_ttbarDM_pseudo_m10_yr16\",\"16\")"
py locallimits.py datacards/scan_ttbarDM_scalar_m10_yr16/ std_ttbarDM_scalar --nolimits
py locallimits.py datacards/scan_ttbarDM_pseudo_m10_yr16/ std_ttbarDM_pseudo --nolimits

return 0

# sigscan=std_T2bW
# root -b -q newCardMaker.C"(\"${sigscan:4}\",\"$indir\",\"datacards/$outdir\")" || return $?
# python locallimits.py "datacards/$outdir" ${sigscan}
# root -b -q Make2DLimitHistos.C"(\"${sigscan}\", \"limits/$outdir\")"
# echo -e "Limits for $sigscan in $outdir is done!\\a"

# sigscan=std_T2tb
# root -b -q newCardMaker.C"(\"${sigscan:4}\",\"$indir\",\"datacards/$outdir\")" || return $?
# python locallimits.py "datacards/$outdir" ${sigscan}
# root -b -q Make2DLimitHistos.C"(\"${sigscan}\", \"limits/$outdir\")"
# echo -e "Limits for $sigscan in $outdir is done!\\a"

function runLimits {
    root -b -q newCardMaker.C"(\"$sigscan\",\"$indir\",\"datacards/$outdir\",\"$ysuf\")" || return $?
    python locallimits.py "datacards/$outdir" std_${sigscan} --nolimits # --runsignif
    # root -b -q Make2DLimitHistos.C"(\"std_${sigscan}\", \"limits/$outdir\")"
    # root -b -q Make2DSignifHistos.C"(\"std_${sigscan}\", \"significances/$outdir\")"
    # root -b -q MakeSignificancePlot.C"(\"significances/$outdir\", \"std_${sigscan}\", 0)"
    # root -b -q MakeSignificancePlot.C"(\"significances/$outdir\", \"std_${sigscan}\", 1)"
    echo -e "Limits for $sigscan in $outdir is done!\\a"
}

ysuf=17
indir=../StopLooper/output/samp${ysuf}_$bvsuf
outdir=scan_samp${ysuf}_$bvsuf

# root -b -q ../AnalyzeScripts/makeBkgEstimates.C"(\"$indir\",\"$indir\",\"$ysuf\", \"extr_threshold=1.5\")" || return $?

ysuf=18
indir=../StopLooper/output/samp${ysuf}_$bvsuf
outdir=scan_samp${ysuf}_$bvsuf

# root -b -q ../AnalyzeScripts/makeBkgEstimates.C"(\"$indir\",\"$indir\",\"$ysuf\", \"extr_threshold=2.0\")" || return $?

# bvsuf=v31_cor_w3
# bvsuf=v31_s13_noMETttbarSF
# bvsuf=v31_s21
bvsuf=v31_s21
# bvsuf=v31_m17sync

ysuf=run2
indir=../StopLooper/output/combRun2_$bvsuf
outdir=scan_comb${ysuf}_yrcomb8_${bvsuf}

# root -b -q fakeBkgEstimates.C"(\"$indir\",\"$indir\",\"$ysuf\")" || return $?
# root -b -q ../AnalyzeScripts/makeBkgEstimates.C"(\"$indir\",\"$indir\",\"$ysuf\")" || return $?
# root -b -q ../AnalyzeScripts/makeBkgEstimates.C"(\"$indir\",\"$indir\",\"$ysuf\",\"extrShapeFromCR=false\")" || return $?
# root -b -q ../AnalyzeScripts/makeBkgEstimates.C"(\"$indir\",\"./\",\"$ysuf\",\"useMetExtrapolation=false\")" || return $?

sigscan=T2tt
root -b -q newCardMaker.C"(\"$sigscan\",\"$indir\",\"datacards/$outdir\",\"$ysuf\")" || return $?
# root -b -q newCardMaker.C"(\"$sigscan\",\"$indir\",\"datacards/$outdir\",\"$ysuf\")"
python locallimits.py "datacards/$outdir" std_${sigscan}   --nolimits # --noupdate --runsignif 
python locallimits.py "datacards/$outdir" tcor_${sigscan}  --nolimits # --noupdate --runsignif 
python locallimits.py "datacards/$outdir" Wcor_${sigscan}  --nolimits # --noupdate --runsignif 

# # eva40
# root -b -q Make2DLimitHistos.C"(\"comb_${sigscan}\", \"limits/$outdir\")"
# root -b -q Make2DLimitHistos.C"(\"std_${sigscan}\", \"limits/$outdir\")"
# root -b -q Make2DLimitHistos.C"(\"tcor_${sigscan}\", \"limits/$outdir\")"
# root -b -q Make2DLimitHistos.C"(\"Wcor_${sigscan}\", \"limits/$outdir\")"
# root -b -q Make2DSignifHistos.C"(\"comb_${sigscan}\", \"significances/$outdir\")"
# root -b -q MakeSignificancePlot.C"(\"significances/$outdir\", \"comb_${sigscan}\", 0)"
# root -b -q MakeSignificancePlot.C"(\"significances/$outdir\", \"comb_${sigscan}\", 1)"
echo -e "Limits for $sigscan in $outdir is done!\\a"
# # mkcp limits/$outdir/histo/* limits/$outdir/histo_scan
# runLimits

sigscan=T2bW
runLimits

sigscan=T2bt
runLimits

# root -b -q systTableMaker.C"(\"T2bt\",\"../StopLooper/output/combRun2_v31_s13\",\"datacards/sigcont\",\"run2\")"

outdir=${outdir}_stau

sigscan=T8bbstausnu_XStau0p75
# root -b -q newCardMaker.C"(\"$sigscan\",\"$indir\",\"datacards/$outdir\",\"$ysuf\")" || return $?
# python locallimits.py "datacards/$outdir" std_${sigscan} # --runsignif # --nolimits
# root -b -q Make2DLimitHistos.C"(\"std_${sigscan}\", \"limits/$outdir\")"
# root -b -q Make2DSignifHistos.C"(\"std_${sigscan}\", \"significances/$outdir\")"
# root -b -q MakeSignificancePlot.C"(\"significances/$outdir\", \"std_${sigscan}\", 0)"
# root -b -q MakeSignificancePlot.C"(\"significances/$outdir\", \"std_${sigscan}\", 1)"

sigscan=T8bbstausnu_XStau0p5
# root -b -q newCardMaker.C"(\"$sigscan\",\"$indir\",\"datacards/$outdir\",\"$ysuf\")" || return $?
# python locallimits.py "datacards/$outdir" std_${sigscan} # --runsignif # --nolimits
# root -b -q Make2DLimitHistos.C"(\"std_${sigscan}\", \"limits/$outdir\")"
# runLimits

sigscan=T8bbstausnu_XStau0p25
# root -b -q newCardMaker.C"(\"$sigscan\",\"$indir\",\"datacards/$outdir\",\"$ysuf\")" || return $?
# python locallimits.py "datacards/$outdir" std_${sigscan} # --runsignif # --nolimits
# root -b -q Make2DLimitHistos.C"(\"std_${sigscan}\", \"limits/$outdir\")"
# runLimits

# Generate card for ttH signals
# (interactive) $ .L ../AnalyzeScripts/makeBkgEstimates.C; $ postProcessOnly()
# (edit) newCardMaker.C # remove the fastsim unc for signals
# root -b -q newCardMaker.C"(1)" # indir to be changed in the preloaded argument
# cd datacards/ttHtoInv_s7_v2/ && rename _-1_-1 "" * && cd -
# python locallimits.py "datacards/ttHtoInv_s7_v2" std_ttHtoInv

# Generate cards for ttDM signals

# cd ../StopLooper/output/sampttDM_v32_m10
# for i in ../combRun2_v31_s21/*; do ln -s $i; done
# rm SMS_*.root
# for i in TTbarDMJets_*.root; do ln -s $i SMS_ttbarDM_${i:12:6}_${i:20:2}.root; done
# hadd -f SMS_ttbarDM_scalar_run2.root SMS_ttbarDM_scalar_1?.root
# hadd -f SMS_ttbarDM_pseudo_run2.root SMS_ttbarDM_pseudo_1?.root
# cd -

# root -b -q newCardMaker.C"(\"ttbarDM_scalar\",\"../StopLooper/output/sampttDM_v32_m10\",\"datacards/scan_ttbarDM_scalar_m10_v2\",\"run2\")"
# root -b -q newCardMaker.C"(\"ttbarDM_pseudo\",\"../StopLooper/output/sampttDM_v32_m10\",\"datacards/scan_ttbarDM_pseudo_m10_v2\",\"run2\")"

# py locallimits.py datacards/scan_ttbarDM_scalar_m10_v2/ std_ttbarDM_scalar --nolimits
# py locallimits.py datacards/scan_ttbarDM_pseudo_m10_v2/ std_ttbarDM_pseudo --nolimits

# unset indir
# unset outdir
# unset sigscan

# bvsuf=v37_c3
# ysuf=run2
# indir=../StopLooper/output/combRun2_$bvsuf
# outdir=scan_comb${ysuf}_$bvsuf

# root -b -q ../AnalyzeScripts/makeBkgEstimates.C"(\"$indir\",\"$indir\",\"$ysuf\")" || return $?

# sigscan=T2tt
# root -b -q newCardMaker.C"(\"$sigscan\",\"$indir\",\"datacards/$outdir\",\"$ysuf\")" || return $?
# python locallimits.py "datacards/$outdir" std_${sigscan}
# python locallimits.py "datacards/$outdir" compressed_${sigscan}
# root -b -q Make2DLimitHistos.C"(\"std_${sigscan}\", \"limits/$outdir\")"
# root -b -q Make2DLimitHistos.C"(\"compressed_${sigscan}\", \"limits/$outdir\")"
# echo -e "Limits for $sigscan in $outdir is done!\\a"
# mkcp limits/$outdir/histo/* limits/$outdir/histo_scan

# bvsuf=v37_c1
# ysuf=run2
# indir=../StopLooper/output/combRun2_$bvsuf
# outdir=scan_comb${ysuf}_$bvsuf

# root -b -q ../AnalyzeScripts/makeBkgEstimates.C"(\"$indir\",\"$indir\",\"$ysuf\")" || return $?

# sigscan=T2tt
# root -b -q newCardMaker.C"(\"$sigscan\",\"$indir\",\"datacards/$outdir\",\"$ysuf\")" || return $?
# python locallimits.py "datacards/$outdir" compressed_${sigscan}
# root -b -q Make2DLimitHistos.C"(\"compressed_${sigscan}\", \"limits/$outdir\")"
# echo -e "Limits for $sigscan in $outdir is done!\\a"
# mkcp limits/$outdir/histo/* limits/$outdir/histo_scan
