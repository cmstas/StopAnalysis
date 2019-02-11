import os
import numpy as np
import ROOT
import pyRootPlotMaker.pyRootPlotMaker as ppm
import MT2PlotUtils as utils

def MT2PlotMaker(rootdir, samples, data, dirname, plots, output_dir=".", exts=["pdf"], tag="", scaleMC=True, suffix = None, systset=None, lumi=None, ratioType=0):
    ''' 
    rootdir contains output of MT2Looper, samples are names of the .root files,
    data is the name of the data file, dirname is the directory within the root file
    to extract plots from, plots are a list of plot definitions from MT2PlotDefs
    exts is a list of file extensions to produce
    note that dirname can be a '+' separated string of directories to add together 
    '''

    h_bkg_vecs = [[] for x in plots]
    h_data = []

    dirnames = [s.strip() for s in dirname.split("+")]

    if lumi==None:
        if 'samp16' in rootdir: lumi = 35.9
        if 'samp17' in rootdir: lumi = 42.0
        if 'samp18' in rootdir: lumi = 42.0
        if 'Run2'   in rootdir: lumi = 137.4

    ## deal with suffixes
    # crdybaseInclLowPtOF means that we should take the plots in crdybaseIncl that end in LowPt
    # crdybaseInclLowPtSF means that we should take the plots in crdybaseIncl that end in LowPtemu
    if suffix != None and suffix != '' and suffix[0] != '_':
        suffix = '_'+suffix

    systs = [None for x in plots]
    drawSystematicBand = False
    if systset is not None and type(systset)==list:
        drawSystematicBand = True
        systs = [[] for x in plots]
        h_bkg_vecs_syst_up = [[[] for s in systset] for p in plots ]
        h_bkg_vecs_syst_dn = [[[] for s in systset] for p in plots ]
           
    #     nbins = h_mt2bins_data.GetNbinsX()
    #     systs = [0 for i in range(nbins)]
    #     ## get systematic in first bin
    #     incr = 0
    #     for ibin in range(2,nbins+1):
    #         incr += 0.4 / (nbins-1) * (ibin-1) * h_mt2bins_data.GetBinContent(i)
    #     systs[0] = incr/h_mt2bins_data.GetBinContent(1)
    #     ## get systematics in other bins
    #     for ibin in range(2,nbins+1):
    #         systs[ibin-1] = 0.4 / (nbins-1) * (ibin-1)


    ## get background histograms
    for isamp in range(len(samples)):
        # get the root file for the given sample. This assumes that frag/fake photons come from qcd_ht.root
        fn = os.path.join(rootdir, samples[isamp]+".root")
        fid = ROOT.TFile(fn)

        for iplot in range(len(plots)):
            vn = plots[iplot][0]
            if suffix != None: vn += suffix

            h_bkg_vecs[iplot].append( fid.Get(dirnames[0]+"/h_"+vn) )

            # histogram won't exist if there are no events. Replace it with None, handle later
            if type(h_bkg_vecs[iplot][-1])==type(ROOT.TObject()):
                h_bkg_vecs[iplot][-1] = None
            else:
                h_bkg_vecs[iplot][-1].SetDirectory(0)
                # handle the case with more than one directory
                for idir in range(1, len(dirnames)):
                    h_bkg_vecs[iplot][-1].Add(fid.Get(dirnames[idir]+"/h_"+vn))

            if drawSystematicBand:
                for isys, sys in enumerate(systset):
                    h_bkg_vecs_syst_up[iplot][isys].append( fid.Get("{}/h_{}_{}Up".format(dirnames[0],vn,sys)) )
                    h_bkg_vecs_syst_dn[iplot][isys].append( fid.Get("{}/h_{}_{}Dn".format(dirnames[0],vn,sys)) )
                    h_bkg_vecs_syst_up[iplot][isys][-1].SetDirectory(0)
                    h_bkg_vecs_syst_dn[iplot][isys][-1].SetDirectory(0)

        fid.Close()

    # deal with nonexistent histograms <-- 
    for i in range(len(plots)):
        firstGood = -1
        for isamp in range(len(samples)):
            if h_bkg_vecs[i][isamp] != None:
                firstGood = isamp
                break
        if firstGood==-1:
            raise RuntimeError("all background histograms are empty for {0}/h_{1}!".format(dirname,plots[i][0]))
        for isamp in range(len(samples)):
            if h_bkg_vecs[i][isamp] == None:
                h_bkg_vecs[i][isamp] = h_bkg_vecs[i][firstGood].Clone()
                h_bkg_vecs[i][isamp].Reset()
        if drawSystematicBand:
            h_bkg_tot = h_bkg_vecs[i][firstGood].Clone()
            syst_up = [0.0] * h_bkg_tot.GetNbinsX()
            syst_dn = [0.0] * h_bkg_tot.GetNbinsX()
            for isamp in range(len(samples)):
                if isamp != firstGood and h_bkg_vecs[i][isamp] != None:
                    h_bkg_tot.Add(h_bkg_vecs[i][isamp])
            for isys, sys in enumerate(systset):
                h_bkg_tot_syst_up = h_bkg_vecs_syst_up[i][isys][firstGood].Clone()
                h_bkg_tot_syst_dn = h_bkg_vecs_syst_dn[i][isys][firstGood].Clone()
                for isamp in range(len(samples)):
                    if isamp == firstGood: continue
                    if h_bkg_vecs_syst_up[i][isys][isamp] != None:
                        h_bkg_tot_syst_up.Add(h_bkg_vecs_syst_up[i][isys][isamp])
                    if h_bkg_vecs_syst_dn[i][isys][isamp] != None:
                        h_bkg_tot_syst_dn.Add(h_bkg_vecs_syst_dn[i][isys][isamp])
                h_bkg_tot_syst_up.Scale(h_bkg_tot.Integral()/h_bkg_tot_syst_up.Integral())
                h_bkg_tot_syst_dn.Scale(h_bkg_tot.Integral()/h_bkg_tot_syst_dn.Integral())
                h_bkg_tot_syst_up.Divide(h_bkg_tot)
                h_bkg_tot_syst_dn.Divide(h_bkg_tot)
                for ibin in range(1, h_bkg_tot.GetNbinsX()+1):
                    sysup = h_bkg_tot_syst_up.GetBinContent(ibin)-1
                    sysdn = h_bkg_tot_syst_dn.GetBinContent(ibin)-1
                    h_bkg_tot_syst_up.SetBinError(ibin, abs(sysup))
                    h_bkg_tot_syst_dn.SetBinError(ibin, abs(sysdn))
                    h_bkg_tot_syst_up.SetBinContent(ibin, 1)
                    h_bkg_tot_syst_dn.SetBinContent(ibin, 1)
                    # Temporary
                    syst_up[ibin-1] = ((syst_up[ibin-1])**2 + sysup**2)**0.5
                    syst_dn[ibin-1] = ((syst_dn[ibin-1])**2 + sysdn**2)**0.5

            systs[i] = syst_up # Temporary
            for ibin in range(len(syst_up)):
                systs[i][ibin] = max(syst_up[ibin], syst_dn[ibin])

    ## get data histograms
    datatitle = 'data'
    if data==None:
        h_data = [None for i in plots]
    else:
        data_file = os.path.join(rootdir, data+".root")
        fid = ROOT.TFile(data_file)
        for pl in plots:
            vn = pl[0]
            # if suffix != None:
            #     vn += suffix
            h_data.append( fid.Get(dirnames[0]+"/h_"+vn) )
            if type(h_data[-1])==type(ROOT.TObject()):
                raise Exception("No {0}/h_{1} histogram for {2}!".format(dirname, vn, data))
            h_data[-1].SetDirectory(0)
            # handle the case with more than one directory
            for idir in range(1, len(dirnames)):
                h_data[-1].Add(fid.Get(dirnames[idir]+"/h_"+vn))
        fid.Close()

    # make the output directory if it doesn't exist
    if not os.path.isdir(os.path.join(output_dir,dirname+tag)):
        os.makedirs(os.path.join(output_dir,dirname+tag))

    # make all of the plots
    for i in range(len(plots)):
        vn = plots[i][0]
        if suffix != None: vn += suffix
        userMin,userMax = None,None
        if plots[i][3]!=None:
            userMin = plots[i][3][0]
            userMax = plots[i][3][1]
        if len(plots[i]) >= 5:
            utils.Rebin(h_bkg_vecs[i],h_data[i], plots[i][4])
        doOverflow = True
        if len(plots[i]) >= 6:
            doOverflow = plots[i][5]
        markerSize=0.8
        title = utils.GetCRName(dirname)
        # title = None
        xAxisTitle = utils.GetVarName(vn)
        unit = utils.GetUnit(vn)
        subtitles = utils.GetSubtitles(dirname)
        if h_data[i]!=None:
            if scaleMC:
                subLegText = ["MC scaled by {datamcsf}","# Data events: {ndata}"]
            else:
                subLegText = ["# Data events: {ndata}"]
        else:
            subLegText = None
        # subLegText = None
        sns = [utils.GetSampleName(s) for s in samples]
        colors = [utils.GetColor(s) for s in samples]
        for ext in exts:
            saveAs = os.path.join(output_dir,dirname+tag,"{0}_{1}.{2}".format(dirname,vn,ext))
            ppm.plotDataMC(h_bkg_vecs[i], sns, h_data[i], doPause=False, xAxisTitle=xAxisTitle, lumi=lumi, lumiUnit='fb',
                           title=title, subtitles=subtitles, dataTitle=datatitle, xRangeUser=plots[i][2], isLog=plots[i][1], saveAs=saveAs,
                           scaleMCtoData=scaleMC, xAxisUnit=unit, userMin=userMin, userMax=userMax, doSort=False, customColors=colors,
                           markerSize=markerSize, titleSize=0.035, subtitleSize=0.033, legCoords=(0.60,0.70,0.87,0.895),
                           subLegText=subLegText, subLegTextSize=0.036, doBkgError=True, doOverflow=doOverflow, cmsTextSize=0.04,
                           convertToPoisson=False, drawZeros=False, drawSystematicBand=drawSystematicBand, systematics=systs[i], ratioType=ratioType)



