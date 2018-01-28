from __future__ import print_function

import time

from metis.Sample import SNTSample
from metis.Sample import DirectorySample
from metis.CondorTask import CondorTask
from metis.StatsParser import StatsParser
from metis.Utils import do_cmd


if __name__ == "__main__":

    cms4_samples = {
        
        # Essential parts of the Moriond 16 samples
        "ttbar_diLept_madgraph_ext1" : "/hadoop/cms/store/user/sicheng/ProjectMetis/TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1_MINIAODSIM_CMS4_V00-00-02_toptag",
        "ttbar_singleLeptFromT_madgraph_ext1" : "/hadoop/cms/store/user/sicheng/ProjectMetis/TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1_MINIAODSIM_CMS4_V00-00-02_toptag",
        "ttbar_singleLeptFromTbar_madgraph_ext1" : "/hadoop/cms/store/user/sicheng/ProjectMetis/TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1_MINIAODSIM_CMS4_V00-00-02_toptag",

        "t_tW_top_5f_powheg_noHadDecays_ext1" : "/hadoop/cms/store/user/sicheng/ProjectMetis/ST_tW_top_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1_MINIAODSIM_CMS4_V00-00-02_toptag",
        "t_tW_antitop_5f_powheg_noHadDecays_ext1" : "/hadoop/cms/store/user/sicheng/ProjectMetis/ST_tW_antitop_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1_MINIAODSIM_CMS4_V00-00-02_toptag",

        "W1JetsToLNu_madgraph" : "/hadoop/cms/store/user/sicheng/ProjectMetis/W1JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1_MINIAODSIM_CMS4_V00-00-02_toptag",
        "W2JetsToLNu_madgraph" : "/hadoop/cms/store/user/sicheng/ProjectMetis/W2JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1_MINIAODSIM_CMS4_V00-00-02_toptag",
        "W3JetsToLNu_madgraph_ext1" : "/hadoop/cms/store/user/sicheng/ProjectMetis/W3JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1_MINIAODSIM_CMS4_V00-00-02_toptag",
        "W4JetsToLNu_madgraph_ext2" : "/hadoop/cms/store/user/sicheng/ProjectMetis/W4JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1_MINIAODSIM_CMS4_V00-00-02_toptag",
        "W1JetsToLNu_NuPt200_madgraph" : "/hadoop/cms/store/user/sicheng/ProjectMetis/W1JetsToLNu_NuPt-200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1_MINIAODSIM_CMS4_V00-00-02_toptag",
        "W2JetsToLNu_NuPt200_madgraph" : "/hadoop/cms/store/user/sicheng/ProjectMetis/W2JetsToLNu_NuPt-200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1_MINIAODSIM_CMS4_V00-00-02_toptag",
        "W3JetsToLNu_NuPt200_madgraph" : "/hadoop/cms/store/user/sicheng/ProjectMetis/W3JetsToLNu_NuPt-200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1_MINIAODSIM_CMS4_V00-00-02_toptag",
        "W4JetsToLNu_NuPt200_madgraph" : "/hadoop/cms/store/user/sicheng/ProjectMetis/W4JetsToLNu_NuPt-200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv2-PUMoriond17_backup_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2_MINIAODSIM_CMS4_V00-00-02_toptag",

        "TTZToLLNuNu_M10_amcnlo_ext2" : "/hadoop/cms/store/user/sicheng/ProjectMetis/TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1_MINIAODSIM_CMS4_V00-00-02_toptag",
        "WZTo1L3Nu_amcnlo" : "/hadoop/cms/store/user/sicheng/ProjectMetis/WZTo1L3Nu_13TeV_amcatnloFXFX_madspin_pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1_MINIAODSIM_CMS4_V00-00-02_toptag",

        # Singals
        # "SMS_T2tt_mStop_400to1200_madgraph" : "/hadoop/cms/store/user/sicheng/ProjectMetis/SMS-T2tt_mStop-400to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1_MINIAODSIM_CMS4_V00-00-02_toptag",
    }


    # Make a directory sample, giving it the location and a dataset name for bookkeeping purposes
    # The globber must be customized (by default, it is *.root) in order to pick up the text files

    # Make a CondorTask (3 in total, one for each input)
    maker_tasks = []
    merge_tasks = []

    for dsname, samploc in cms4_samples.items():
        cmsswver = "CMSSW_9_4_1"
        scramarch = "slc6_amd64_gcc630"
        tarfile = "input.tar.gz"
        tag = "v25_9"
        maker_task = CondorTask(
            sample = DirectorySample(
                dataset=dsname,
                location=samploc,
            ),
            # open_dataset = True,
            files_per_output = 1,
            tag = tag,
            cmssw_version = cmsswver,
            scram_arch = scramarch,
            tarfile = tarfile,
            executable = "condor_executable.sh",
            outdir_name = "stopBaby_" + dsname,
            output_name = "stopbaby.root",
            arguments = "1" if "SMS" in dsname else "0", # isFastsim
            condor_submit_params = {"sites": "UAF,T2_US_UCSD,UCSB"},
            # no_load_from_backup = True,
        )
        merge_task = CondorTask(
            sample = DirectorySample( 
                dataset="merge_"+dsname,
                location=maker_task.get_outputdir(),
            ),
            # open_dataset = True, flush = True,
            executable = "merge_executable.sh",
            tarfile = "merge_scripts.tar.gz",
            files_per_output = 100000,
            output_dir = maker_task.get_outputdir() + "/merged",
            output_name = "merged_" + dsname + ".root",
            condor_submit_params = {"sites":"UAF"},
            output_is_tree = True,
            # check_expectedevents = True,
            tag = tag,
            cmssw_version = "CMSSW_9_2_8",
            scram_arch = "slc6_amd64_gcc530",
            no_load_from_backup = True,
        )
        maker_tasks.append(maker_task)
        merge_tasks.append(merge_task)
        

    for i in range(100):
        total_summary = {}
        
        for maker_task, merge_task in zip(maker_tasks,merge_tasks):
            maker_task.process()

            if maker_task.complete():
                do_cmd("mkdir -p {}/merged".format(maker_task.get_outputdir()))
                do_cmd("mkdir -p {}/skimmed".format(maker_task.get_outputdir()))
                merge_task.reset_io_mapping()
                merge_task.update_mapping()
                merge_task.process()

            total_summary[maker_task.get_sample().get_datasetname()] = maker_task.get_task_summary()
            total_summary[merge_task.get_sample().get_datasetname()] = merge_task.get_task_summary()

        # parse the total summary and write out the dashboard
        StatsParser(data=total_summary, webdir="~/public_html/dump/metis_stopbaby/").do()

        # 1 hr power nap
        time.sleep(15.*60)

    # If it's complete, make a dummy sample out of the output directory
    # in order to pick up the files. Then cat out the contents and sum
    # them up. This should be 3*2*10 = 100

