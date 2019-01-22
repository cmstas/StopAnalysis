indir=../StopLooper/output/samp16_v14_tf
outdir=scan_samp16_v14_tf
ysuf=16

# root -b -q fakeBkgEstimates.C"(\"$indir\",\"$indir\",\"$ysuf\")" || return $?
# root -b -q ../AnalyzeScripts/makeBkgEstimates.C"(\"$indir\",\"$indir\",\"$ysuf\")" || return $?

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

# indir=../StopLooper/output/samp17_v13_tf
# outdir=scan_samp17_v13_tf
# ysuf=17
# root -b -q ../AnalyzeScripts/makeBkgEstimates.C"(\"$indir\",\"$indir\",\"$ysuf\")" || return $?

indir=../StopLooper/output/comb1617_v30_a1
outdir=scan_run2_a1
ysuf=run2
root -b -q ../AnalyzeScripts/makeBkgEstimates.C"(\"$indir\",\"$indir\",\"$ysuf\")" || return $?

sigscan=std_T2tt
root -b -q newCardMaker.C"(\"${sigscan:4}\",\"$indir\",\"datacards/$outdir\",\"$ysuf\")" || return $?
python locallimits.py "datacards/$outdir" ${sigscan}
root -b -q Make2DLimitHistos.C"(\"${sigscan}\", \"limits/$outdir\")"
echo -e "Limits for $sigscan in $outdir is done!\\a"
mv limits/$outdir/histo limits/$outdir/histo_v30_a1

unset indir
unset outdir
unset sigscan
