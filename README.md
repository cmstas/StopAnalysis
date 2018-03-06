# StopAnalysis
SnT Stop-1l Analysis
The analysis try to look for / exclude the hypothetical top squark (stop) predicted by the supersymmetry.

## Setup
Need a copy or a soft link to the snt CORE library under the main `StopAnalysis` directory.

## Analysis work flow in steps

### StopBabyMaker
Make baby ntuples for the stop analysis from the snt cms4 ntuples. The following analysis will all based on the output stopbabies.
Refer to the README in the subfolder for details.

### StopLooper
The new looper that works on the stop babies. Functions from `StopCORE` are used by the looper.
Refer to the README in the subfolder for details.

### AnalyzeScripts
Scripts that does analysis on the looper output. The data-driven background estimation can also be found here.

### CombineAnalysis
Using HiggsCombineTool to make results/limits on the stop mass.
Refer to the README in the subfolder for details.


