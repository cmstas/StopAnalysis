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
- The output of the looper are by default root files with yield histograms for each sample that were ran over.
- The yield histogram is named `h_metbins` and there's one for each topological region. 
- The yield histogram for fastsim samples is named `hSMS_metbins` and is a 3D histogram.
- The yield histograms with systematic variations can be found under the same SR folder with the syst as suffix to main yield hist.
- Other interesting kinematic plots are plotted for each SR and can be found under the SR folder as well.

Further processing scripts for the yield/kinematic histograms can be found at `../AnalysisScripts` folder.

### Control the plots for SR and CR
The plots to be made for each SR and CR are defined in the function `StopLooper::fillHistosForSR()`, and `StopLooper::fillHistosForCRXX()`.
Additional plots of interest can be added anywhere into the looper by the `plot1D` function.
Other testing plots may be defined in separate functions (that can easily get commented out when not needed).

## TODO
* To add TODO to the TODOs
* To finish the TODOs listed here as much as possible
* Change the values map into map of function pointers, and adjust the SR class accrodingly
* Make a setup script that does setup automatically (maybe not needed)
* Get rid of the `dummy.root` (find a way to prevent hists from being destroyed after file close)
* Facilitate the looper with parallel programing (if possible)
* To trim down the Utilities.h and make new hist ploting function that can be more useful
* Implement the new values based SR/CR finding algorithm and test if that improves efficiency
