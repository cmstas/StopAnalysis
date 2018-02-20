indir="../StopLooper/output/temp_4bins"
outdir="scan_4bins"

root -b -q fakeBkgEstimates.C"(\"$indir\")" || return $?
root -b -q newCardMaker.C"(\"T2tt\",\"$indir\",\"datacards/$outdir\")" || return $?
python locallimits.py "datacards/$outdir"
root -b -q Make2DLimitHistos.C"(\"std_T2tt\", \"limits/$outdir\")"

echo -e "Limits for $outdir is done!\\a"

unset indir
unset outdir

