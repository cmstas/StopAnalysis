# New Stop Baby Looper

## Dependency
* First do `make` in directory `StopAnalysis/StopBabyMaker/stop_variables/` the `StopCORE` needs it.
* A link to the SNT `CORE` dir is required to be under the `StopAnalysis` folder, make sure that the CORE is compiled in the `cms4` branch.
* The new looper framework might still depends on other `StopCORE` modules being compiled/compilable, 
  please make sure that the `Makefile` in `StopBabyMaker/stop_variables/` and `StopCORE` has the correct dependencies.
  
## To Run
All the following are based on the directory `StopAnalysis/StopLooper`
### Simple start
``` bash
. do.sh
```

### Signal/Control Region definition
The signal and control region are defined in the file `StopRegions.cc`, making use of the `SR` class.

### Control the plots for SR and CR
The plots to be made for each SR and CR are defined in the function `StopLooper::fillHistosForSR()`. 
Additional plots of interest can be added anywhere in the looper 


## TODO
* To add TODO to the TODOs
* To finish the TODOs listed here as much as possible
* Get rid of the `dummy.root` (find a way to prevent hists from being destroyed after file close)
* Add automation script to monitor the running looper proceed with further command when finishes
* To make duplicate removal to work global for all data
* To facilitate the looper with parallel programing
