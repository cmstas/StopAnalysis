# CombineAnalysis for Stop 1l


## Current workflow
The CombineAnalysis takes histogram outputs after the background estimates, which should be in the same format as after StopLooper.
- For a quick analysis without data, the script `fakeBkgEstimates.C` will do the naive transformation.

### newCardMaker.C
- The cardmaker to work on the new output format
- Various switches are on the header lines of the files
- Support either doing a signal scan or just make all cards for a single mass point
- Inputfile names are hardcoded to certain filenames and can be changed inside the newCardMaker function
Example usage
``` bash
# Make cards for scan
root -lbq newCardMaker.C'("T2tt","../StopLooper/output/temp", "datacards/scan_temp")'
# Make cards for 1 single mass point
root -lbq newCardMaker.C'(1, "T2tt_800_400","../StopLooper/output/temp", "datacards/scan_temp")'
```

### locallimits.py:
As oppose to batchlimits, is good for lightweight partial analysis, will call `CombineCards.sh` to combine the output from `newCardMaker.C` before proceeding to limit calcuation using the combine tool. 
The [HiggsCombineTool](https://twiki.cern.ch/twiki/bin/viewauth/CMS/SWGuideHiggsAnalysisCombinedLimit) is needed for this step.
> Note that only certain CMSSW releases support the HiggsCombineTool, so make sure you are checking out the one that listed in the Twiki
- The combined cards will be put into `combinedcards` under the same card folder
- The rootfile output of the asymptotic limit will be put into the `limits` folder, with subfolder name same as the card dir
- The print (log) version of the limit can be found under the `combinedcards`

### Make2DLimitHistos.C:
as Make2DSignifHistos.C, but for limits
- In here, the signaltype string also contains analysisID, e.g. "std_T2tt"
- Now expect as input as a folder with the combine output rootfile as done by `locallimits.py`, or the untarred one from the batch output.

#### Produce nice limit plot:
Check out the general SUSY [package](https://github.com/CMS-SUS-XPAG/PlotsSMS.git) 
- prepare a config file that tells the script where to locate the hisgtograms generated from the Make2DSignifHistos.C
- run `python python/makeSMSplots.py <model>_<label>_<tag>.cfg <outputname>` and get the nice limit plots

## Legacy files

### makeDataCards.C:
- make single-bin datacards: need input files from all backgrounds, data and signal to work
- the functions creating datacards (either a single one- or a group up to all scans for all mass points) are the functions called makeDataCards*.
All other functions are helper functions
- Usually I use makeDataCardsOneScan() that makes all datacards for one scan (e.g. T2tt) for one analysis (e.g. general or compressed): The analysisID has however to be defined. Currently only the general/standard analysis is defined.
- All default settings for flags is what we also use as default.
- functions called analyis* are specific to each analysis (e.g. general) and need to be defined beforehand.
- For filenames/histogram names, you need to go through the load* functions, the main makeDataCardsOneBinBasis() function, as well as the UpdatePlottingHisto() - that last one makes histograms for plotting/table making.

Currently, if one histogram name is spelled wrongly, the code just crashes.
Also if the file/histogram defined in UpdatePlottingHisto() does not exist, the code crashes.

### CombineCards.sh:
Combine all SR of single-bin datacards into a combined datacard.
- You need to input signal such as "std_T2tt_850_150" as the first argument
- the directory where the single-bin datacards can be passed in as the second argument
- the maximal number of bins is passed in as the 3rd argument 
  - in principle one could set this very high, program will just run slower.

### CombineCardsScan.sh:
helper function to run on a complete scan. There is a bunch of hardcoded stuff, but you can overwrite those with arguments.
- some variables don't do anything, as the script uses those defined in CombineCards.sh

### CombineCardsAllScans.sh:
obvious

### Make2DSignifHistos.C:
out of the Combine output make 2D histograms with p-value and significances (sigma). Currently written that you need to run expected priori/posteriori and observed first.
- in here, the signaltype string also contains analysisID, e.g. "std_T2tt"
- main function is Make2DSignifHistos() with default flags being correct.
- expect as input the combine output tars as done by the batch submission scripts in the subdirectories.

### CovarianceAndCorrelation:
Out of a set of one big datacard containing all SR bins (signal is not important here - only background), create combine output that allows to make a covariance matrix histogram, that then can be transformed into a correlation matix histogram.
- This one does not run on a tagged version of combine - need untagged version...

### batchlimit:
batch submission for running combine on condor using datacards:
- setup.sh: Define all the ALLCAPS variables on top
- batch.sh: This is what you need to run, but define before "NAME" (analyisID_ScanName) and "postfit" (true used for final results, for studies use false)
- cleanup.sh: each time you run batch.sh - this will create tar files from the datacarddir - WAIT until all your jobs are running, then you can cleanup.
ALSO WAIT before rerunning batch.sh until all jobs are running.

### batchsignificance:
batch submission for running combine on condor using datacards
- similar to batchlimit - in batch.sh instead of "postfit" you need to define "observed" and "posteriori"
