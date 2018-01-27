import time
import itertools
import json
import traceback

from metis.Sample import DBSSample
from metis.CMSSWTask import CMSSWTask
from metis.StatsParser import StatsParser
from metis.Utils import send_email


if __name__ == "__main__":


    infos = [
        # "/TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer17MiniAOD-92X_upgrade2017_realistic_v10-v3/MINIAODSIM|831.74364|1.0|1.0",
        # "/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/RunIISummer17MiniAOD-92X_upgrade2017_realistic_v10_ext1-v1/MINIAODSIM|730|1.139397|1.0",
        # "/WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer17MiniAOD-92X_upgrade2017_realistic_v10-v1/MINIAODSIM|50690.0|1.21|1.0",
        # "/W1JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer17MiniAOD-92X_upgrade2017_realistic_v10-v1/MINIAODSIM|9493|1.238|1.0",
        # "/W2JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer17MiniAOD-92X_upgrade2017_realistic_v10-v1/MINIAODSIM|3120|1.231|1.0",
        # "/W3JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer17MiniAOD-92X_upgrade2017_realistic_v10-v1/MINIAODSIM|942.3|1.231|1.0",
        # "/W4JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer17MiniAOD-92X_upgrade2017_realistic_v10-v1/MINIAODSIM|524.2|1.144|1.0",
        # "/QCD_Pt-170to300_EMEnriched_TuneCUETP8M1_13TeV_pythia8/RunIISummer17MiniAOD-92X_upgrade2017_realistic_v10-v2/MINIAODSIM|122700|1.0|0.17",

        "/SMS-T2tt_mStop-400to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM|1.0|1.0|1.0",
        # "/SMS-T2bW_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM|1.0|1.0|1.0",
        # "/SMS-T2bt_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM|1.0|1.0|1.0",
        # "/W1JetsToLNu_NuPt-200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM|2.128|1.11|1.0",
        # "/W3JetsToLNu_NuPt-200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM|3.248|1.52|1.0",
        # "/W4JetsToLNu_NuPt-200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_backup_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM|5.945|1.485|1.0",

        # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 
        # 2016 80X samples: http://www.t2.ucsd.edu/tastwiki/bin/view/CMS/Run2Samples25nsMoriond17 #
        # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 

        # ttbar
        '/TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM|57.35|1.5225|1',
        '/TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM|114.6|1.594|1',
        '/TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM|114.6|1.594|1',

        # WJets
        '/W1JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM|9493|1.238|1',
        '/W2JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM|3120.0|1.231|1',
        '/W3JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM|942.3|1.231|1',
        '/W4JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1/MINIAODSIM|524.2|1.144|1',
        '/W1JetsToLNu_NuPt-200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM|2.128|1.11|1',
        '/W2JetsToLNu_NuPt-200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM|3.721|1.33|1',
        '/W3JetsToLNu_NuPt-200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM|3.248|1.52|1',
        '/W4JetsToLNu_NuPt-200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_backup_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM|5.945|1.485|1',

        # single t
        '/ST_tW_top_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM|38.09|0.5135|1',
        '/ST_tW_antitop_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM|38.09|0.5135|1',

        # ttV
        '/TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1/MINIAODSIM|0.2529|1.0|1.0',
        # '/ttWJets_13TeV_madgraphMLM/RunIISummer16MiniAODv2-80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM|0.6105|1|1',
        # '/ttZJets_13TeV_madgraphMLM/RunIISummer16MiniAODv2-80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM|0.7826|1|1',

        # diBoson
        '/WZTo1L3Nu_13TeV_amcatnloFXFX_madspin_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM|3.05|1.00262|1',
    ]

    publish_to_dis = False

    ntuple_tasks = []
    for info in infos:
        dsname = info.split("|")[0].strip()
        xsec = float(info.split("|")[1].strip())
        kfact = float(info.split("|")[2].strip())
        efact = float(info.split("|")[3].strip())

        cmsswver = "CMSSW_9_2_8"
        scramarch = "slc6_amd64_gcc530"
        tarfile = "../tarfiles/lib_CMS4_V00-00-07_928.tar.gz"
        cms3tag = "CMS4_V00-00-07_test2"
        pset = "/home/users/sicheng/working/cms4dev/CMSSW_9_4_0_pre2_new/src/CMS3/NtupleMaker/test/main_pset.py"
        if "_80X_" in dsname:
            cmsswver = "CMSSW_8_0_26_patch1"
            scramarch = "slc6_amd64_gcc530"
            tarfile = "../tarfiles/lib_CMS4_V00-00-02_toptag_8026p1.tar.gz"
            cms3tag = "CMS4_V00-00-02_toptag"
            pset = "/home/users/sicheng/temp/cms4prod/CMSSW_8_0_26_patch1/src/CMS3/NtupleMaker/test/pset_moriondremc.py"

        task = CMSSWTask(
                sample = DBSSample(
                    dataset=dsname,
                    xsec=xsec,
                    kfact=kfact,
                    efact=efact,
                    ),
                events_per_output = 450e3,
                output_name = "merged_ntuple.root",
                tag = cms3tag,
                scram_arch = scramarch,
                global_tag = "", # if global tag blank, one from DBS is used
                pset = pset,
                # pset_args = "data=False",
                pset_args = "fastsim=True" if dsname.startswith("/SMS") else "data=False",
                cmssw_version = cmsswver,
                tarfile = tarfile,
                condor_submit_params = {"sites": "UAF"},
                # condor_submit_params = {"sites": "UAF,T2_US_UCSD,UCSB"},
                # condor_submit_params = {"use_xrootd": True},
                publish_to_dis = publish_to_dis,
        )
        ntuple_tasks.append(task)

    for i in range(100):

        total_summary = {}
        # for dsname in dataset_names:
        for task in ntuple_tasks:
            
            try:
                task.process()
            except:
                traceback_string = traceback.format_exc()
                print "Runtime error:\n{0}".format(traceback_string)
                # send_email(subject="metis error", body=traceback_string)

            # if not task.complete():
            #     f = open("unfinished.txt","a+")
            #     print ""
            #     for output in task.get_uncompleted_outputs():
            #         f.write("for ouput file <"+output.get_name()+">:\n")
            #         for inp in task.get_inputs_for_output(output):
            #             f.write(inp.get_name()+'\n')
            #     f.write("\n\n")
            #     f.close()

            total_summary[task.get_sample().get_datasetname()] = task.get_task_summary()

        StatsParser(data=total_summary, webdir="~/public_html/dump/metis_80XTopTag/").do()

        time.sleep(60.*30)

