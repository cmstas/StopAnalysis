# StopBabyMaker

This code makes baby ntuples from the CMS4 ntuples, see [NtupleMaker](https://github.com/cmstas/NtupleMaker) for more detail.
The output will facilitate the StopLooper top produce anaylsis result.

## Setup
Make sure the submodule CORE under the parent folder is initiated (or cloned with `--recurse-submodules`). 
Enviroment setup would be easiest to be within an CMSSW environment (a simple `cmsenv` in would suffice, no checkout needed).
The analysis code has been developed under the `CMSSW_10_1_0` environment and with `SCRAM_ARCH=slc6_amd64_gcc700`. A higher 
version is not recommended since it (the newer ROOT version) will break essential part of the stop_variable code.
Then simply do
``` bash
make -j 12
```

If CORE is updated later, you will need manually re-compile CORE for the babymaker.
```
cd ../CORE && make clean && make -j 12 && cd -
make -j 12
```

## Running the babymaker

The main exectuable is `runBabyMaker`. To run it, you need to provide at least the first argument, and a maximum of six arguments.

`./runBabyMaker sample_name nevents file_index outpath sampleList isFastsim`

- **sample\_name**: This is the (base) output file name for the stop baby. Ideally this should be representative of the sample being ran on.
- **nevents**: This is how many events you want to run over. A value of -1 means all events in the ntuple. If you don't provide a value, -1 is used automatically.
- **file\_number**: This number specifies which file you want to run over (i.e. merged\_ntuple\_X.root). The default value is -1, which means all files.
- **outpath**: This is the path where you want the stop babies to be written. The default value is `./`.
- **sampleList**: This a comma separated list of files that you would like the job to run through, it also support files that are accessible from xrootd 
  (starts with `/store/...` or `/hadoop/...`, this will automatically activated if the program is not running at ucsd.).
- **isFastsim**: This should be a boolean of 1 or 0 telling if the sample is a fastSim sample or not. The default value is 0 (not fastSim).

### Miscellaneous
- The file `scale1fbs.txt` need to be updated if need to run on any private MC sample.
- If an extra argument is provide (e.g. "topcand") the babymaker would also incur the TopCandTree and produce an addtional flat ntuple
  (e.g. "topcand.root" same as the arg) for top tagger training purpose.

### Another (old) way to run the babymaker <-- obsolete
    The following arguments can also be changed to the following provided a samples.dat list is present. These are not used anymore 
    since now ProjectMetis manages the sample locations instead (see batch_submit part).
- **sample\_name**: This is the compact name for the sample you want to run on. These sample names are defined in the various sample\*.dat files.
- **sampleList**: This parameter tells the babyMaker which .dat file contains the sample\_name you want to run on. The default value is sample_2017.dat.

## Notes on the developments
- Soft links have been used to access some efficiencies/correction files that are in CORE or StopCORE.
- A lot of switches can be found at `runBabyMaker.cc`
- Currently, JEC would always have to be applied locally if want to have jup and jdown variables calculated.
- The looper shall automatically figure out the year of the sample, if it's located in a path that include it's sample name.
- Whenever new branches are added, `StopCORE/StopTree.h` would need to updated to facilitate their use in StopLooper.

## Submitting batch jobs to the grid

Use ProjectMetis through sub-configurations in `batch_metis`. Edit the `$METIS_PATH` in `batch_metis/setup.sh` if you already have one checked out, or leave it empty if not.

Before proceeding, make sure that the babymaker is compiled and tested.

``` bash
cd batch_metis
. setup.sh
```

