from __future__ import print_function

import time

from metis.Sample import SNTSample
from metis.Sample import DirectorySample
from metis.CondorTask import CondorTask
from metis.StatsParser import StatsParser
from metis.Utils import do_cmd


if __name__ == "__main__":

    resub_samples = {}

    with open('unfinished50.txt') as fin:
        lines = fin.readlines()
        for line in lines:
            if line[0] == "#": continue
            line = line.split()
            srlst = []
            for f in line[1].split(','):
                srlst.append(f)
            resub_samples[line[0]] = srlst

    # print(resub_samples)

    # Make a directory sample, giving it the location and a dataset name for bookkeeping purposes
    # The globber must be customized (by default, it is *.root) in order to pick up the text files

    # Make a CondorTask (3 in total, one for each input)
    maker_tasks = []
    merge_tasks = []

    for dsname, srlst in resub_samples.items():
        cmsswver = "CMSSW_9_4_1"
        scramarch = "slc6_amd64_gcc630"
        tarfile = "input.tar.gz"
        tag = "test3"
        child_tasks = []
        for srf in srlst:
            fname = srf.split('/')[-1]
            fdir = '/'.join(srf.split('/')[:-1])
            print(fdir, fname)
            ifile = fname.split('.')[0].split('_')[-1]
            maker_task = CondorTask(
                sample = DirectorySample(
                    dataset=dsname+'_'+ifile,
                    location=fdir,
                    globber=fname,
                ),
                # open_dataset = True,
                files_per_output = 1,
                tag = tag,
                cmssw_version = cmsswver,
                scram_arch = scramarch,
                tarfile = tarfile,
                executable = "condor_executable.sh",
                outdir_name = "resub_"+dsname,
                output_name = "stopbaby_"+ifile+".root",
                arguments = "1" if "SMS" in dsname else "0", # isFastsim
                condor_submit_params = {"sites": "UAF,T2_US_UCSD"},
                # no_load_from_backup = True,
            )
            # print(maker_task.get_sample().get_datasetname())
            # print(maker_task.get_sample().get_files())
            # print(maker_task.get_outputdir())
            # print(maker_task.get_io_mapping())
            child_tasks.append(maker_task)
        
        merge_task = CondorTask(
            sample = DirectorySample( 
                dataset="merge_"+dsname,
                location=child_tasks[0].get_outputdir(),
            ),
            executable = "ProjectMetis/metis/executables/condor_hadd.sh",
            open_dataset = False,
            files_per_output = 100,
            output_dir = maker_task.get_outputdir() + "/merged",
            output_name = dsname.split('v25_7_')[1] + ".root",
            condor_submit_params = {"sites":"UAF"},
            output_is_tree = True,
            check_expectedevents = True,
            tag = tag,
            cmssw_version = "CMSSW_9_2_8",
            scram_arch = "slc6_amd64_gcc530",
        )
        # print(merge_task.get_sample().get_datasetname())
        # print(merge_task.get_sample().info["location"])

        maker_tasks.append(child_tasks)
        merge_tasks.append(merge_task)
        

    for i in range(100):
        total_summary = {}
        
        for child_tasks, merge_task in zip(maker_tasks,merge_tasks):
            all_child_finishes = True
            for maker_task in child_tasks:
                maker_task.process()
                if not maker_task.complete(): 
                    all_child_finishes = False

            if all_child_finishes:
                do_cmd("mkdir -p {}/merged".format(maker_task.get_outputdir()))
                merge_task.reset_io_mapping()
                merge_task.update_mapping()
                merge_task.process()

            if merge_task.complete():
                outfile = '{}/{}_1.root'.format(merge_task.get_outputdir(), merge_task.output_name.split('.')[0])
                target = outfile.split('resub')[0]
                tmp = merge_task.get_sample().get_datasetname().split('_')
                target += 'stopBaby_{}/{}'.format('_'.join(tmp[1:-2]), merge_task.output_name)
                # print('cp {} {}'.format(outfile, target))
                print('cp {} {}'.format(outfile, target))
                do_cmd('cp {} {}'.format(outfile, target))
                print(target, "finished!!")

            total_summary[maker_task.get_sample().get_datasetname()] = maker_task.get_task_summary()
            total_summary[merge_task.get_sample().get_datasetname()] = merge_task.get_task_summary()

        # parse the total summary and write out the dashboard
        StatsParser(data=total_summary, webdir="~/public_html/dump/metis_hack/").do()

        # Clean all jobs that has ran over 2 hrs and resubmit
        do_cmd("condor_q -w | grep resub | grep '0+01:' | awk '{print $1}' | xargs condor_rm")

        # 5 min power nap
        time.sleep(5.*60)

