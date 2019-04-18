bvsuf=v31_s8
ysuf=16

indir=../StopLooper/output/samp${ysuf}_$bvsuf
outdir=scan_samp${ysuf}_$bvsuf

# root -b -q fakeBkgEstimates.C"(\"$indir\",\"$indir\",\"$ysuf\")" || return $?
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
bvsuf=v31_s12

ysuf=run2
indir=../StopLooper/output/combRun2_$bvsuf
outdir=scan_comb${ysuf}_${bvsuf}

# root -b -q fakeBkgEstimates.C"(\"$indir\",\"$indir\",\"$ysuf\")" || return $?
# root -b -q ../AnalyzeScripts/makeBkgEstimates.C"(\"$indir\",\"$indir\",\"$ysuf\")" || return $?
# root -b -q ../AnalyzeScripts/makeBkgEstimates.C"(\"$indir\",\"./\",\"$ysuf\",\"MetExtrFor2l=false\")" || return $?

sigscan=T2tt
root -b -q newCardMaker.C"(\"$sigscan\",\"$indir\",\"datacards/$outdir\",\"$ysuf\")" || return $?
# python locallimits.py "datacards/$outdir" std_${sigscan}
python locallimits.py "datacards/$outdir" tcor_${sigscan}
python locallimits.py "datacards/$outdir" Wcor_${sigscan}
# root -b -q Make2DLimitHistos.C"(\"comb_${sigscan}\", \"limits/$outdir\")"
# root -b -q Make2DLimitHistos.C"(\"std_${sigscan}\", \"limits/$outdir\")"
# root -b -q Make2DLimitHistos.C"(\"tcor_${sigscan}\", \"limits/$outdir\")"
# root -b -q Make2DLimitHistos.C"(\"Wcor_${sigscan}\", \"limits/$outdir\")"
# echo -e "Limits for $sigscan in $outdir is done!\\a"
# mkcp limits/$outdir/histo/* limits/$outdir/histo_scan

# python locallimits.py "datacards/$outdir" srJ0_${sigscan}
# python locallimits.py "datacards/$outdir" srJ1_${sigscan}
# python locallimits.py "datacards/$outdir" srJ3_${sigscan}
# python locallimits.py "datacards/$outdir" srJ4_${sigscan}
# python locallimits.py "datacards/$outdir" srJ5_${sigscan}

# root -b -q Make2DLimitHistos.C"(\"srJ0_${sigscan}\", \"limits/$outdir\")"
# root -b -q Make2DLimitHistos.C"(\"srJ1_${sigscan}\", \"limits/$outdir\")"
# root -b -q Make2DLimitHistos.C"(\"srJ3_${sigscan}\", \"limits/$outdir\")"
# root -b -q Make2DLimitHistos.C"(\"srJ4_${sigscan}\", \"limits/$outdir\")"
# root -b -q Make2DLimitHistos.C"(\"srJ5_${sigscan}\", \"limits/$outdir\")"


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
