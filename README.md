# StopAnalysis
SnT Stop-1l Analysis
The analysis try to look for / exclude the hypothetical top squark (stop) predicted by the supersymmetry.

## Setup
``` bash
git clone --recurse-submodules git@github.com:cmstas/StopAnalysis.git && cd StopAnalysis
```

## Analysis work flow in steps

### StopBabyMaker
The StopBabyMaker is the start point of the analysis, as it select and pre-calculate only the useful branches for stop-1l 
from the cms4 samples and store them as baby ntuples. The following analysis will all based on the output stopbabies, through
`StopCORE/StopTree.h` for the accessing the branches. 
Please refer to the README in the subfolder for more details.

### StopLooper
The StopLooper holds the main analysis looper, with some of the shared code with the babymaker in the `StopCORE` folder. 
The `StopLooper` should be the single framework that generate yields for signal regions and also other supporting analysis 
(e.g. trigger efficiencies measurements). To enabled/disable a submodule simply comment/uncomment the corresponding function 
within the main looper.
Please refer to the README in the subfolder for more details.

### AnalyzeScripts
Scripts that do analysis base on the looper output (e.g. plot maker, ROC plotter, etc).
The data-driven background estimation script can also be found here.

### CombineAnalysis
Using HiggsCombineTool to produce results/exclusion limits on the stop & LSP masses.
Refer to the README in the subfolder for details.


