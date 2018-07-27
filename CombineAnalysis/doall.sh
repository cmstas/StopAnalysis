indir=../StopLooper/output/newbin15_120ifb
outdir=scan_newbin15_120ifb

root -b -q fakeBkgEstimates.C"(\"$indir\")" || return $?
# root -b -q ../AnalyzeScripts/makeBkgEstimates.C"(\"$indir\",\"$indir\")" || return $?

sigscan=std_T2tt
root -b -q newCardMaker.C"(\"${sigscan:4}\",\"$indir\",\"datacards/$outdir\")" || return $?
python locallimits.py "datacards/$outdir" ${sigscan}
root -b -q Make2DLimitHistos.C"(\"${sigscan}\", \"limits/$outdir\")"
echo -e "Limits for $sigscan in $outdir is done!\\a"
mv limits/$outdir/histo limits/$outdir/histo_76ifb

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

unset indir
unset outdir
unset sigscan
