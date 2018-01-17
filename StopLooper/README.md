# New Stop Baby Looper

## Dependency
* The analysis code here has been developed under the `CMSSW_9_4_X` environment and with `SCRAM_ARCH=slc6_amd64_gcc630`,
  other version might also work but the performace is not guaranteed.
* A link to the SNT `CORE` dir is required to be under the `StopAnalysis` folder, make sure that the CORE is in the `master` 
  branch and compiled under the same environment as this code.
  
## To Run
All the following are based on the directory `StopAnalysis/StopLooper`
### Simple start
Modify the file `do.sh` to include the disired input stopbaby location, output directory, and the samples to run on. For each sample, the looper will 
include all stopbabies that start with the sample name.
Then simply do
``` bash
. do.sh
```

### Signal/Control Region definition
The signal and control region are defined in the file `StopRegions.cc`, making use of the `SR` class.
The current active function for singal regions is `getStopSignalRegionsTopological()`, and `getStopControlRegionsNoBTags()` for 0 b-tagged CR, etc...

### Output format
The output of the looper are by default root files with yield histograms for each sample that ran over.
The yield histogram is named `h_metbins` and there's one for each topological region. 
The yield histograms with systematic variations can be found under the same SR folder.

Currently the analysis that can be done directly on these yield histograms has not be developed yet. 
As a temporary (or long term) subsititue, use the script `../AnalysisScripts/transformOutput.py` (still under development)
to transform the yields into the old format, and the existing scripts of the analysis can be used from this point.

### Control the plots for SR and CR
The plots to be made for each SR and CR are defined in the function `StopLooper::fillHistosForSR()`. 
Additional plots of interest can be added anywhere into the looper by the `plot1D` function.

## TODO
* To add TODO to the TODOs
* To finish the TODOs listed here as much as possible
* To add all components to the analysis looper
* Make a setup script that does setup automatically
* Get rid of the `dummy.root` (find a way to prevent hists from being destroyed after file close)
* Facilitate the looper with parallel programing (future)
* To trim down the PlotUtilities and make new hist ploting function that can be more useful
* Implement the new values based SR/CR finding algorithm and test if that improves efficiency
