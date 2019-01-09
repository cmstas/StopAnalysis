#!/usr/bin/env python

import sys
import argparse
import fnmatch
import os
import glob
import ROOT
import utilities as util
import slim_baby as sb

#outDir = "/home/users/rheller/wh_babies_noskim/"



def processWH(babydir,sample,outdir):
	### ensure output directories exist
	util.ensureDir(outdir)
	util.ensureDir(outdir+"/branchesToAdd")

	###Calculate additional branches to add
	whLooperCommand = "./runWHLooper %s %s %s" % (babydir,sample,outdir+"/branchesToAdd/")
	print whLooperCommand
	os.system(whLooperCommand)

	### get list of babies###
	searchString = babydir+"/*"+sample+"*"
	print searchString
	babyList = [x for x in glob.glob(searchString)]

	print babyList
	for baby in babyList:
		print "Processing ",baby
		basename = baby.split("/")[-1];
		rulesFile = "slim_rules/wh.txt"
		outName = outdir+"/slim_"+basename
		print outName
		sb.slimNtuple(rulesFile, outName, [baby], False, False)

		zipCommand = "root -b -l -q \'zip.cc(\"%s\", \"%s\")\'" % (outName,outdir+"/branchesToAdd/"+basename.replace(".root","_wh.root"))
		print zipCommand 
		os.system(zipCommand)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Calculates additional branches for WH and removes unnecessary branches",
                                     formatter_class=argparse.ArgumentDefaultsHelpFormatter)
   
    parser.add_argument("babydir",
                        help="Baby directory")
    parser.add_argument("sample",
                        help="Sample tag")
    parser.add_argument("outdir",
                        help="Sample tag")

    args = parser.parse_args()

    processWH(args.babydir,args.sample,args.outdir)