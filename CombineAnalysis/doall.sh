bvsuf=v30_s3
ysuf=16

indir=../StopLooper/output/samp${ysuf}_$bvsuf
outdir=scan_samp${ysuf}_$bvsuf

# root -b -q fakeBkgEstimates.C"(\"$indir\",\"$indir\",\"$ysuf\")" || return $?
root -b -q ../AnalyzeScripts/makeBkgEstimates.C"(\"$indir\",\"$indir\",\"$ysuf\", \"extr_threshold=1.25\")" || return $?

# sigscan=std_T2tt
# root -b -q newCardMaker.C"(\"${sigscan:4}\",\"$indir\",\"datacards/$outdir\",\"$ysuf\")" || return $?
# python locallimits.py "datacards/$outdir" ${sigscan}
# root -b -q Make2DLimitHistos.C"(\"${sigscan}\", \"limits/$outdir\")"
# echo -e "Limits for $sigscan in $outdir is done!\\a"
# mv limits/$outdir/histo limits/$outdir/histo_s16v14_tf

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

root -b -q ../AnalyzeScripts/makeBkgEstimates.C"(\"$indir\",\"$indir\",\"$ysuf\", \"extr_threshold=1.5\")" || return $?

ysuf=18
indir=../StopLooper/output/samp${ysuf}_$bvsuf
outdir=scan_samp${ysuf}_$bvsuf

root -b -q ../AnalyzeScripts/makeBkgEstimates.C"(\"$indir\",\"$indir\",\"$ysuf\", \"extr_threshold=2.2\")" || return $?

ysuf=run2
indir=../StopLooper/output/combRun2_$bvsuf
outdir=scan_samp${ysuf}_$bvsuf

root -b -q ../AnalyzeScripts/makeBkgEstimates.C"(\"$indir\",\"$indir\",\"$ysuf\")" || return $?

# sigscan=std_T2tt
# root -b -q newCardMaker.C"(\"${sigscan:4}\",\"$indir\",\"datacards/$outdir\",\"$ysuf\")" || return $?
# python locallimits.py "datacards/$outdir" ${sigscan}
# root -b -q Make2DLimitHistos.C"(\"${sigscan}\", \"limits/$outdir\")"
# echo -e "Limits for $sigscan in $outdir is done!\\a"
# mv limits/$outdir/histo limits/$outdir/histo_v30_a1

unset indir
unset outdir
unset sigscan
