# WH babymaker 

## Environment
* The analysis code here has been developed under the `CMSSW_10_1_0` environment and with `SCRAM_ARCH=slc6_amd64_gcc700`
(a simple `cmsenv` in would suffice, no checkout needed).
  
## To Run

### Simple start
Modify the file `do.sh` to include the desired input stopbaby location, output directory, and the samples to run on. 
For each sample, the looper will include all files that start with the sample name (disregarding suffixes).
The samples are separated by their type into several blocks and can be combined in the end.
Always make sure the date and output directories are correct!

Then simply do
``` 
./do.sh
```

## How it works
The processing happens in 3 steps:
  1. Calculation of additional variables, in particular event weights, to avoid computation at histogram-filling time.
     This is done in WHLooper.cc, structured like the Stop 1l looper and using the event weight utilities in StopCORE.
     The output is a small tree with only the branches added.
     WHLooper (like StopLooper) typically has a segfault at the very end of the job--this is okay, as long as you see it report how many events it processed.
     
  2. Removal (slimming) of most extraneous branches from Stop 1l baby. 
    A copy of the stop baby with most branches removed is made by slim_baby.py, according to rules in slim_rules
    
  3. Combining again the trees produced in steps 1 and 2 by zip.cc
