import os
import sys
import ROOT as r

from dataMCplotMaker.dataMCplotMaker import dataMCplot

# def makeSRplots(fbkgs, fsigs=None, fdata=None, dirs, hists):

#     opts = {}
#     opts["outputName"] = "plots/test.pdf"
#     optstr = "--darkColorLines --lumi 2.89 --topYaxisTitle Data/Pred. --type Preliminary --dataName data2017 --outOfFrame "

#     for dirstr in Dirs:
#         for hstr in Hists:
#             if 'h_' not in hstr: hstr = 'h_' + hstr

#             vh_bkg = []
#             for f in fbkgs:
#                 h = f.Get(dirstr+'/'+hstr)
#                 vh_bkg.append(h)

#             if h1 == None or h2 == None:
#                 continue
#             # if h2.Integral() <= 0: continue
#             # scale = h1.Integral()/h2.Integral()
#             # h2.Scale(scale)

#             # bgs = [h2,]
#             # opts["outputName"] = "plots/"+h1.GetName()+".pdf"
#             # opts["xAxisOverride"] = h1.GetXaxis().GetTitle()
#             # print opts["outputName"], opts["xAxisOverride"]
#             # color = [r.kAzure-3,]

#             # title = "title"
#             # subttl = "subttl"

#             # dataMCplot(h1, bgs=[h2,], titles=["data2016",], colors=color, opts=opts, opts_str=optstr, title=title, subtitle=subttl)



if __name__ == "__main__":

    os.system("mkdir -p plots")

    r.gROOT.SetBatch(1)
    r.gStyle.SetOptStat(0)

    opts = {}
    opts["outputName"] = "plots/test.pdf"
    optstr = "--darkColorLines --lumi 2.89 --topYaxisTitle Data/Pred. --type Preliminary --dataName data2017 --outOfFrame "

    f1 = r.TFile("../StopLooper/output/3binsMp4Rp9_120ifb/allBkg_25ns.root")
    f2 = r.TFile("../StopLooper/output/3binsMp4Rp9_120ifb/SMS_T2tt.root")

    # Dirs = ["srbase", "cr2lbase", "cr0bbase"]
    # Hists = ["h_metbins", "h_met", "h_njets", "h_nbjets", "h_tmod", "h_mlepb"]

    Dirs = ["srbase"]
    Hists = ["met"]
    Hists = ['h_'+hn for hn in Hists]

    for dirstr in Dirs:
        for hstr in Hists:
            h1 = f1.Get(dirstr+'/'+hstr)
            h2 = f2.Get(dirstr+'/'+hstr)

            print h1, h2
            if not h1 or not h2:
                continue

            # scale = h1.Integral()/h2.Integral()
            # h2.Scale(scale)

            opts["outputName"] = "plots/"+h1.GetName()+".pdf"
            opts["xAxisOverride"] = h1.GetXaxis().GetTitle()
            print opts["outputName"], opts["xAxisOverride"]
            color = [r.kAzure-3,]

            title = "title"
            subttl = "subttl"

            dataMCplot(h2, bgs=[h1,h2], titles=["h1","h2"], colors=color, opts=opts, opts_str=optstr, title=title, subtitle=subttl)

            # h1.Scale(1./h1.Integral())
            # h2.Scale(1./h2.Integral())

            # c1 = r.TCanvas("c1", "c1", 800, 600)
            # h1.Draw()
            # c1.Print('plots/'+hstr+'.pdf')

            # hratio = h1.Clone('ratio'+hstr[1:])
            # hratio.Divide(h2)
            # for i in range(1, hratio.GetNbinsX()):
            #     if (hratio.GetBinContent(i) < 0):
            #         hratio.SetBinContent(i, 0)
            #         hratio.SetBinError(i, 0)

            # hratio.GetYaxis().SetRangeUser(0, 7)
            # hratio.SetTitle('ratio'+hstr[1:])
            # hratio.Draw()

            # c1.Print('plots/'+hstr+'.pdf')
