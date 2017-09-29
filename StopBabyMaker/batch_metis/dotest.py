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
from metis.File import MutableFile


def test1():
    dataset_names = [
        # "/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/RunIISummer17MiniAOD-92X_upgrade2017_realistic_v10_ext1-v1/MINIAODSIM",
        "/Dummy_test_StopBabyMaker_v25/CMS4",
    ]
    basedir = "/hadoop/cms/store/user/{0}/metis_test/example/".format(os.getenv("USER"))

    for i in range(10000):

        total_summary = {}
        for dsname in dataset_names:

            try:
                task = CondorTask(
                    sample = DirectorySample(
                        location=basedir,
                        dataset=dsname,
                    ),
                    files_per_output = 1,
                    output_name = "merged_ntuple.root",
                    tag = "v25",
                    scram_arch = "slc6_amd64_gcc700",
                    executable = "condor_executable.sh",
                    arguments = "testarg1",
                    tarfile = "input.tar.gz",
                    condor_submit_params = {"sites": "UAF,T2_US_UCSD,UCSB"},
                    no_load_from_backup = True, # for the purpose of the example, don't use a backup
                )

                task.process()
            except:
                traceback_string = traceback.format_exc()
                print("Runtime error:\n{0}".format(traceback_string))
                # send_email(subject="metis error", body=traceback_string)

            total_summary[dsname] = task.get_task_summary()

        StatsParser(data=total_summary, webdir="~/public_html/dump/metis/").do()

        time.sleep(5)


def test2():
    dataset_names = [
        # "/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/RunIISummer17MiniAOD-92X_upgrade2017_realistic_v10_ext1-v1/MINIAODSIM",
        "/Dummy_test_StopBabyMaker_v25/CMS4",
    ]

    # Make a base directory
    basedir = "/hadoop/cms/store/user/{0}/metis_test/example/".format(os.getenv("USER"))
    MutableFile(basedir).touch()

    # Make a directory sample, giving it the location and a dataset name for bookkeeping purposes
    # The globber must be customized (by default, it is *.root) in order to pick up the text files
    ds = DirectorySample(location=basedir, dataset="/TEST/Examplev1/TEST", globber="*.txt")

    # Make a CondorTask (3 in total, one for each input)
    task = CondorTask(
            sample = ds,
            files_per_output = 1,
            tag = "v0",
            output_name = "ttbar_powheg_pythia8_92X.root",
            executable = "condor_executable.sh",
            cmssw_version = "CMSSW_9_3_1",
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


if __name__ == "__main__":
    test2()
