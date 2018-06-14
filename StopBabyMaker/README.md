# StopBabyMaker

This code makes stop babies from CMS4 ntuples, produced by the [NtupleMaker](https://github.com/cmstas/Ntuple).

## Setup

Require that [CORE](https://github.com/cmstas/CORE) to be checked out exactly under the `StopAnalysis` folder. 
Soft links are created for efficiencies/correction files that are pointing to those in CORE.
Make sure your local copy is up to date. Compile CORE, then compile the babymaker.

```
cd ../CORE && make -j 12 && cd -
make -j 12
```

(If you already have some compiled code, you may wish to run `make
clean` first).

## Running the babymaker

The main exectuable is `runBabyMaker`. To run it, you need to provide at least the first argument, and a maximum of six arguments.

`./runBabyMaker sample_name nevents file_index outpath sampleList isFastsim`

- **sample\_name**: This is the (base) output file name for the stop baby. Ideally this should be representative of the sample being ran on.
- **nevents**: This is how many events you want to run over. A value of -1 means all events in the ntuple. If you don't provide a value, -1 is used automatically.
- **file\_number**: This number specifies which file you want to run over (i.e. merged\_ntuple\_X.root). The default value is -1, which means all files.
- **outpath**: This is the path where you want the stop babies to be written. The default value is `./`.
- **sampleList**: This a comma separated list of files that you would like the job to run through, it also support files that are accessible from xrootd 
        (Starts with `/store/...` or `/hadoop/...`, this will also activated if the program is not running at ucsd.).
- **isFastsim**: This should be a boolean of 1 or 0 telling if the sample is a fastSim sample or not. The default value is 0 (not fastSim).


### Another (old) way to run the babymaker

`./runBabyMaker sample_name nevents file_number outpath sampleList isFastsim`

- **sample\_name**: This is the compact name for the sample you want to run on. These sample names are defined in the various sample\*.dat files.
- **nevents**: This is how many events you want to run over. A value of -1 means all events in the ntuple. If you don't provide a value, -1 is used automatically.
- **file\_number**: This number specifies which file you want to run over (i.e. merged\_ntuple\_X.root). The default value is -1, which means all files.
- **sampleList**: This parameter tells the babyMaker which .dat file contains the sample\_name you want to run on. The default value is sample_2017.dat.
- **isFastsim**: Put a 1 if your desired sample is a fastSim sample. The default value is 0 (not fastSim).

## Submitting batch jobs to the grid

Use ProjectMetis through sub-configurations in `batch_metis`. Edit the `$METIS_PATH` in `batch_metis/setup.sh` if you already have one checked out, or leave it empty if not.

Before proceeding, make sure that the babymaker is compiled and tested.

``` bash
cd batch_metis
. setup.sh
```

