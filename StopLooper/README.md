# New Stop Baby Looper

## Environment
* The analysis code here has been developed under the `CMSSW_10_1_0` environment and with `SCRAM_ARCH=slc6_amd64_gcc700`
(a simple `cmsenv` in would suffice, no checkout needed).
  
## To Run

### Simple start
Modify the file `do.sh` to include the disired input stopbaby location, output directory, and the samples to run on. 
For each sample, the looper will include all files that start with the sample name (disregarding suffixes).
The samples are separated by their type into several blocks and can be combined in the end.
Then simply do
``` bash
. do.sh
```
the script will send a finish signal after done.

## Basic logic
- Maps are used to ease the process of adding histograms, for anywhere within the looper.
- Yields are filled into the `h_metbins` histograms and other scripts will grab yields from these.
- Different samples can be run in parallel to save runtime (see `do.sh` for example) and `hadd`'ed in the end.

### Signal/Control Region definition
The signal and control region are defined in the file `StopRegions.cc`, making use of the `SR` class.
The file also keeps older signal region definitions or other inclusive regions for testing, they can be switched in the 
`StopLooper::SetSignalRegions()` function.

### Output format
- The output of the looper are by default root files with yield histograms for each sample that were ran over.
- The yield histogram is named `h_metbins` and there's one for each topological region. 
- The yield histogram for fastsim samples is named `hSMS_metbins` and is a 3D histogram.
- The yield histograms with systematic variations can be found under the same SR folder with the syst as suffix to main yield hist.
- Other interesting kinematic plots are plotted for each SR and can be found under the SR folder as well.
- JSRoot is recommended as the tool for a quick look at the produced histograms.

Further processing scripts for the yield/kinematic histograms can be found at `../AnalysisScripts` folder.

### Control the plots for SR and CR
The plots to be made for each SR and CR are defined in the function `StopLooper::fillHistosForSR()`, and `StopLooper::fillHistosForCRXX()`.
Additional plots of interest can be added anywhere into the looper by the `plot1d` function.
Other testing plots may be defined in separate functions (that can easily get commented out when not needed).

### Scale factors
The event weight and systematic variations are controlled in `eventWeight.cc`.
- The scale factors are applied only when the event selected by one of the SR or CR.
- The default list of systematics to apply can be changed , or turn on individually in the looper.
- Special file weight can be applied when combining extension samples, by running the predefined function `getSampleWeightXXX(fname)`
  or directly added on top to the variable `sf_extra_file`

### Miscellaneous
- `runStopLooper.cc` should only serve as the holder main() function. It's also doing some special name 
- Be ware of the templated `plotXd` function, where the range value will be interpreted as `double` and can cause problems with
  floating point errors
- Any pair of histogram that start with "hnum" and "hden" and have the same name afterwards, will be combined automatically 
  at the end of the looper to give a ratio hist. But beware that the ratio hist won't be correct after hadd.

## TODO
* To add more TODO to the TODOs
* Change the values map into map of function pointers, and adjust the SR class accrodingly
* Make a setup script that does setup automatically (maybe not needed)
* Get rid of the `dummy.root` (find a way to prevent hists from being destroyed after file close)
* Find a way to mix the use of enum and string to speed up map searches
* Implement the new values based SR/CR finding algorithm and test if that improves speed
