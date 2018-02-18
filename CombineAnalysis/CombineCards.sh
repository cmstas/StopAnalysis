#! /bin/bash

if [ $# -eq 0 ]; then
    echo You should provide at least the signal.
    exit 0
fi

#check cmssw and if it contains HiggsCombine
if [ -z $CMSSW_BASE ]; then
    echo "CMSSW_BASE var not set, run cmsenv, exiting..."
    exit 1
fi
DIRECTORY=`echo $CMSSW_BASE/src/HiggsAnalysis`
if [ ! -d "$DIRECTORY" ]; then
    echo "Your CMSSW release does not have HiggsCombined installed. Please do."
    echo "Please read https://twiki.cern.ch/twiki/bin/viewauth/CMS/SWGuideHiggsAnalysisCombinedLimit"
    echo "You should have ${DIRECTORY}. Exiting..."
    exit 1
fi

carddir='datacards/correlated/'
fakedata=false
maxbins=128
signal=$1

#signal='T2tt_425_325'

[ $# -gt 1 ] && carddir=$2
[ $# -gt 2 ] && maxbins=$3

Name=datacard_${signal}

#echo ${Name}

combinestring=combineCards.py
validcommand=false

counter=0
for i in `seq 1 ${maxbins}`; do
    ifile=${carddir}/${Name}_bin${i}.txt
    if [ ! -e "$ifile" ] && [ ! -f "$ifile" ] && [ ! -s "$ifile" ]; then
        invalidfile=$ifile
        continue
    fi
    counter=$((counter+1))
    validcommand=true
    chnum=$(($i + 1))
    combinestring="${combinestring} ch${i}=$ifile"
    #echo ${combinestring}
done

if [ $validcommand = true ]; then
    mkdir -p ${carddir}combinedcards/
    eval ${combinestring} -S > ${carddir}combinedcards/${Name}.txt
    #echo ${combinestring}
    echo "Combined cards for ${Name} into ${carddir}combinedcards/${Name}.txt"
else
    echo "Input file $invalidfile does not exist. Didn't combine ${carddir}combinedcards/${Name}.txt"
fi
