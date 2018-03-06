indir=../StopLooper/output/3binsMp7p3Rp9
outdir=scan_3binsMp7p3Rp9

root -b -q fakeBkgEstimates.C"(\"$indir\")" || return $?

sigscan=std_T2tt
root -b -q newCardMaker.C"(\"${sigscan:4}\",\"$indir\",\"datacards/$outdir\")" || return $?
python locallimits.py "datacards/$outdir" ${sigscan}
root -b -q Make2DLimitHistos.C"(\"${sigscan}\", \"limits/$outdir\")"
echo -e "Limits for $sigscan in $outdir is done!\\a"

# sigscan=std_T2bW
# root -b -q newCardMaker.C"(\"${sigscan:4}\",\"$indir\",\"datacards/$outdir\")" || return $?
# python locallimits.py "datacards/$outdir" ${sigscan}
# root -b -q Make2DLimitHistos.C"(\"${sigscan}\", \"limits/$outdir\")"
# echo -e "Limits for $sigscan in $outdir is done!\\a"

unset indir
unset outdir
unset sigscan
