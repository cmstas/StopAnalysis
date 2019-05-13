bvsuf=v31_m17sync
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
bvsuf=v31_s16
# bvsuf=v31_m17sync

ysuf=run2
indir=../StopLooper/output/combRun2_$bvsuf
outdir=scan_comb${ysuf}_${bvsuf}

# root -b -q fakeBkgEstimates.C"(\"$indir\",\"$indir\",\"$ysuf\")" || return $?
root -b -q ../AnalyzeScripts/makeBkgEstimates.C"(\"$indir\",\"$indir\",\"$ysuf\")" || return $?
# root -b -q ../AnalyzeScripts/makeBkgEstimates.C"(\"$indir\",\"$indir\",\"$ysuf\",\"extrShapeFromCR=false\")" || return $?
# root -b -q ../AnalyzeScripts/makeBkgEstimates.C"(\"$indir\",\"./\",\"$ysuf\",\"useMetExtrapolation=false\")" || return $?

sigscan=T2tt
root -b -q newCardMaker.C"(\"$sigscan\",\"$indir\",\"datacards/$outdir\",\"$ysuf\")" || return $?
python locallimits.py "datacards/$outdir" std_${sigscan}  --runsignif # --nolimits
python locallimits.py "datacards/$outdir" tcor_${sigscan} --runsignif # --nolimits
python locallimits.py "datacards/$outdir" Wcor_${sigscan} --runsignif # --nolimits
root -b -q Make2DLimitHistos.C"(\"comb_${sigscan}\", \"limits/$outdir\")"
root -b -q Make2DLimitHistos.C"(\"std_${sigscan}\", \"limits/$outdir\")"
root -b -q Make2DLimitHistos.C"(\"tcor_${sigscan}\", \"limits/$outdir\")"
root -b -q Make2DLimitHistos.C"(\"Wcor_${sigscan}\", \"limits/$outdir\")"
echo -e "Limits for $sigscan in $outdir is done!\\a"
# mkcp limits/$outdir/histo/* limits/$outdir/histo_scan

sigscan=T2bW
root -b -q newCardMaker.C"(\"$sigscan\",\"$indir\",\"datacards/$outdir\",\"$ysuf\")" || return $?
python locallimits.py "datacards/$outdir" std_${sigscan}  --runsignif # --nolimits
root -b -q Make2DLimitHistos.C"(\"std_${sigscan}\", \"limits/$outdir\")"

sigscan=T2bt
root -b -q newCardMaker.C"(\"$sigscan\",\"$indir\",\"datacards/$outdir\",\"$ysuf\")" || return $?
python locallimits.py "datacards/$outdir" std_${sigscan}  --runsignif # --nolimits
root -b -q Make2DLimitHistos.C"(\"std_${sigscan}\", \"limits/$outdir\")"

# root -b -q systTableMaker.C"(\"T2bt\",\"../StopLooper/output/combRun2_v31_s13\",\"datacards/sigcont\",\"run2\")"

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
