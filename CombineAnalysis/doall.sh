indir="../StopLooper/output/temp_2bins"
outdir="scan_2bins"

root -b -q fakeBkgEstimates.C"(\"$indir\")"
root -b -q newCardMaker.C"(\"T2tt\",\"$indir\",\"datacards/$outdir\")"
python locallimits.py "datacards/$outdir"
root -b -q Make2DLimitHistos.C"(\"std_T2tt\", \"limits/$outdir\")"

unset indir
unset outdir

