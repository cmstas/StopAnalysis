#! /usr/bin/python
# coding: utf-8

import os
import sys
import ROOT as r

def combineInclusives(binContents, binErrors, binLabels, nbins):          # temporary

    binContents.extend([0.0 for i in range(42)])
    binErrors.extend([0.0 for i in range(42)])
    binLabels.extend(["inclusive_{}".format(i) for i in range(42)])

    for i, label in enumerate(binLabels):
        if "2to3j" in label:
            binContents[nbins+0] += binContents[i]
        elif "geq4j" in label:
            binContents[nbins+1] += binContents[i]

        if "2to3j" in label and "tmod10toInf" in label:
            binContents[nbins+2] += binContents[i]
        elif "geq4j" in label and "tmodlt0" in label:
            binContents[nbins+3] += binContents[i]
        elif "geq4j" in label and "tmod0to10" in label:
            binContents[nbins+4] += binContents[i]
        elif "geq4j" in label and "tmod10toInf" in label:
            binContents[nbins+5] += binContents[i]

        if    "2to3j_tmod10toInf" in label and "_1" in label:
            binContents[nbins+6] += binContents[i]
        elif  "2to3j_tmod10toInf" in label and "_2" in label:
            binContents[nbins+7] += binContents[i]
        elif  "2to3j_tmod10toInf" in label and "_3" in label:
            binContents[nbins+8] += binContents[i]
        elif  "2to3j_tmod10toInf" in label and "_4" in label:
            binContents[nbins+9] += binContents[i]
        elif  "2to3j_tmod10toInf" in label and "_5" in label:
            binContents[nbins+10] += binContents[i]
        elif  "2to3j_tmod10toInf" in label and "_6" in label:
            binContents[nbins+11] += binContents[i]
        elif  "2to3j_tmod10toInf" in label and "_7" in label:
            binContents[nbins+12] += binContents[i]
        elif  "2to3j_tmod10toInf" in label and "_8" in label:
            binContents[nbins+13] += binContents[i]

        elif "geq4_tmodlt0" in label and "_1" in label:
            binContents[nbins+14] += binContents[i]
        elif "geq4_tmodlt0" in label and "_1" in label:
            binContents[nbins+15] += binContents[i]
        elif "geq4_tmodlt0" in label and "_1" in label:
            binContents[nbins+16] += binContents[i]
        elif "geq4_tmodlt0" in label and "_1" in label:
            binContents[nbins+17] += binContents[i]
        elif "geq4_tmodlt0" in label and "_1" in label:
            binContents[nbins+18] += binContents[i]
        elif "geq4_tmodlt0" in label and "_1" in label:
            binContents[nbins+19] += binContents[i]
        elif "geq4_tmodlt0" in label and "_1" in label:
            binContents[nbins+20] += binContents[i]

        elif "geq4_tmod0to10" in label and "_1" in label:
            binContents[nbins+21] += binContents[i]
        elif "geq4_tmod0to10" in label and "_1" in label:
            binContents[nbins+22] += binContents[i]
        elif "geq4_tmod0to10" in label and "_1" in label:
            binContents[nbins+23] += binContents[i]
        elif "geq4_tmod0to10" in label and "_1" in label:
            binContents[nbins+24] += binContents[i]
        elif "geq4_tmod0to10" in label and "_1" in label:
            binContents[nbins+25] += binContents[i]
        elif "geq4_tmod0to10" in label and "_1" in label:
            binContents[nbins+26] += binContents[i]

        elif "geq4_tmod10toInf" in label and "_1" in label:
            binContents[nbins+27] += binContents[i]
        elif "geq4_tmod10toInf" in label and "_1" in label:
            binContents[nbins+28] += binContents[i]
        elif "geq4_tmod10toInf" in label and "_1" in label:
            binContents[nbins+29] += binContents[i]
        elif "geq4_tmod10toInf" in label and "_1" in label:
            binContents[nbins+30] += binContents[i]
        elif "geq4_tmod10toInf" in label and "_1" in label:
            binContents[nbins+31] += binContents[i]
        elif "geq4_tmod10toInf" in label and "_1" in label:
            binContents[nbins+32] += binContents[i]
        elif "geq4_tmod10toInf" in label and "_1" in label:
            binContents[nbins+33] += binContents[i]

    return nbins + 42

def fillFromYieldHist(dirkeys, binContents, binErrors, binLabels, nbins):
    for idir in dirkeys:
        if 'I' in idir: break
        hyield = fin.Get(idir+"/h_metbins"+suf)
        nbin = hyield.GetNbinsX()
        nbins += nbin
        for i in range(1, nbin+1):
            binContents.append(hyield.GetBinContent(i))
            binErrors.append(hyield.GetBinError(i))
            binLabels.append(hyield.GetTitle()+"_{}".format(i))
    
    return nbins

if __name__ == '__main__':

    inputdir = '../StopLooper/output/temp/'
    outputdir = '../StopAnalyzer/Output/Histos'
    os.system('mkdir -p '+outputdir)


    # Can use os command to get all files in the inputdir
    files_todo = [ 'data_2016.root', ]
    for fn in files_todo:
        fin = r.TFile(inputdir+fn)
        fout = r.TFile(outputdir+fn, "RECREATE")

        allDirkeys = fin.GetListOfKeys()
        allDirkeys = [ key.GetName() for key in allDirkeys ]

        suf = ""                # prepare for different syst variation

        # Get ready for the target hist components
        nbins = 0
        binLabels = []
        binContents = []
        binErrors = []

        srkeys = [ key for key in allDirkeys if 'sr' in key and 'base' not in key]
        cr0bkeys = [ key for key in allDirkeys if 'cr0b' in key and 'base' not in key]
        cr2lkeys = [ key for key in allDirkeys if 'cr2l' in key and 'base' not in key]

        dirkeys = srkeys
        nbins = fillFromYieldHist(srkeys, binContents, binErrors, binLabels, nbins)
        nbins = combineInclusives(binContents,binErrors,binLabels, nbins)

        print nbins, len(binLabels)
        print binContents
        
        histname = "h_yields__SR_bulk__genClassy_incl__systematic_nominal"
        yieldhist = r.TH1D(histname, "Yields, Signal Regions, extnded to 30fb, binned with Mlb, and using tight bTagging", nbins, 0, nbins)

