import os
import sys
import ROOT as r
sys.path.insert(0,'/home/users/sicheng/tas/Software/dataMCplotMaker/')

from dataMCplotMaker import dataMCplot

if __name__ == "__main__":

    os.system("mkdir -p plots")

    r.gROOT.SetBatch(1)

    opts = {}
    opts["outputName"] = "plots/test.pdf"
    optstr = "--darkColorLines --lumi 2.89 --topYaxisTitle Data/Pred. --type Preliminary --dataName data2017 --outOfFrame "

    f1 = r.TFile("../StopLooper/output/temp/all_data_Run2017.root")
    f2 = r.TFile("../StopLooper/output/data2016/all_data_Run2016.root")

    Dirs = ["cr2lbase", "cr2lincl1", "cr2lincl2"]
    Hists = ["h_metbins", "h_rlmetbins", "h_njets", "h_nbjets", "h_tmod", "h_mlepb"]

    for dirstr in Dirs:
        for hstr in Hists:
            h1 = f1.Get(dirstr+'/'+hstr)
            h2 = f2.Get(dirstr+'/'+hstr)

            if h2.Integral() <= 0: continue
            scale = h1.Integral()/h2.Integral()
            h2.Scale(scale)

            bgs = [h2,]
            opts["outputName"] = "plots/"+h1.GetName()+".pdf"
            opts["xAxisOverride"] = h1.GetXaxis().GetTitle()
            print opts["outputName"], opts["xAxisOverride"], scale
            color = [r.kAzure-3,]

            title = "title"
            subttl = "subttl"

            dataMCplot(h1, bgs=[h2,], titles=["data2016",], colors=color, opts=opts, opts_str=optstr, title=title, subtitle=subttl)

