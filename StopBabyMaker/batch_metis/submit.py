from __future__ import print_function

import time
import os

import itertools
import json
import traceback

from metis.Sample import SNTSample
from metis.Sample import DirectorySample
from metis.CondorTask import CondorTask
from metis.StatsParser import StatsParser
from metis.Utils import send_email
from metis.Utils import do_cmd


if __name__ == "__main__":

    dataset_names = [
        # "/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/RunIISummer17MiniAOD-92X_upgrade2017_realistic_v10_ext1-v1/MINIAODSIM",
        "/TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer17MiniAOD-92X_upgrade2017_realistic_v10-v3/MINIAODSIM",
    ]

    # Make a directory sample, giving it the location and a dataset name for bookkeeping purposes
    # The globber must be customized (by default, it is *.root) in order to pick up the text files

    # Make a CondorTask (3 in total, one for each input)
    task = CondorTask(
            sample = SNTSample(dataset=dataset_names[0]),
            files_per_output = 1,
            tag = "v25",
            output_name = "TTJets_V07_test.root",
            executable = "condor_executable.sh",
            cmssw_version = "CMSSW_9_4_0_pre3",
            scram_arch = "slc6_amd64_gcc700",
            arguments = "testarg1",
            tarfile = "input.tar.gz",
            condor_submit_params = {"sites": "UAF,T2_US_UCSD,UCSB"},
            no_load_from_backup = True, # for the purpose of the example, don't use a backup
    )
    # do_cmd("rm -rf {0}".format(task.get_outputdir()))

    # Process and sleep until complete
    is_complete = False
    for t in [5.0, 5.0, 10.0, 15.0, 20.0]:
        task.process()
        print("Sleeping for {0} seconds".format(int(t)))
        time.sleep(t)
        is_complete = task.complete()
        if is_complete: break

    # If it's complete, make a dummy sample out of the output directory
    # in order to pick up the files. Then cat out the contents and sum
    # them up. This should be 3*2*10 = 100
    if is_complete:
        print("Job completed! Checking outputs...")
        outsamp = DirectorySample(location=task.get_outputdir(), dataset="/Blah/blah/BLAH", globber="*.txt")
        tot = 0
        for f in outsamp.get_files():
            mf = MutableFile(f.get_name())
            tot += int(mf.cat())
        print("It looks like we found 3*2*10 = {0}".format(tot))

