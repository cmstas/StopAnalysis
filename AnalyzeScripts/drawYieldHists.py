#! /usr/bin/python
# coding: utf-8

import os
import sys
from math import sqrt
import ROOT as r
import numpy as np
import pyRootPlotMaker.ppmUtils as ppu
from utilities.errors import *
from utilities.yields_utils import *

sig_label = {
    'T2tt' : '#tilde{t}#rightarrowt#tilde{#chi}^{0}_{1}',
    'T2bW' : '#tilde{t}#rightarrowb#tilde{#chi}^{#pm}_{1}',
    'T2bt' : '#tilde{t}#rightarrowt#tilde{#chi}^{0}_{1}/b#tilde{#chi}^{#pm}_{1}',
    'T2tb' : '#tilde{t}#rightarrowt#tilde{#chi}^{0}_{1}/b#tilde{#chi}^{#pm}_{1}',
    'ttbarDM_scalar' : 't#bar{t}DM[scalar]',
    'ttbarDM_pseudo' : 't#bar{t}DM[pseudo]',
}

def getLabelsTemporary(srNames):

    std_labels = [
        "A0:[600,750]",
        "A0:[750,+#infty]",
        "A1:[350,450]",
        "A1:[450,600]",
        "A2:[250,600]",
        "B:[250,450]",
        "B:[450,700]",
        "B:[700,+#infty]",
        "C:[350,450]",
        "C:[450,550]",
        "C:[550,650]",
        "C:[650,800]",
        "C:[800,+#infty]",
        "D:[250,350]",
        "D:[350,450]",
        "D:[450,600]",
        "D:[600,+#infty]",
        "E0:[450,600]",
        "E0:[600,+#infty]",
        "E1:[250,350]",
        "E1:[350,450]",
        "E2:[250,350]",
        "E2:[350,450]",
        "E3:[250,350]",
        "E3:[350,450]",
        "F:[250,350]",
        "F:[350,450]",
        "F:[450,+#infty]",
        "G0:[450,550]",
        "G0:[550,750]",
        "G0:[750,+#infty]",
        "G1:[250,350]",
        "G1:[350,450]",
        "G2:[250,350]",
        "G2:[350,450]",
        "G3:[250,350]",
        "G3:[350,450]",
        "H:[250,500]",
        "H:[500,+#infty]",
    ]

    cr0b_labels = [
        "A0:[600,750]",
        "A0:[750,+#infty]",
        "A1:[350,450]",
        "A1:[450,600]",
        # "A2:[250,600]",
        "B:[250,450]",
        "B:[450,700]",
        "B:[700,+#infty]",
        "C:[350,450]",
        "C:[450,550]",
        "C:[550,650]",
        "C:[650,800]",
        "C:[800,+#infty]",
        "D:[250,350]",
        "D:[350,450]",
        "D:[450,600]",
        "D:[600,+#infty]",
        "E0:[450,600]",
        "E0:[600,+#infty]",
        "E1:[250,350]",
        "E1:[350,450]",
        # "E2:[250,350]",
        # "E2:[350,450]",
        "F:[250,350]",
        "F:[350,450]",
        "F:[450,+#infty]",
        "G0:[450,550]",
        "G0:[550,750]",
        "G0:[750,+#infty]",
        "G1:[250,350]",
        "G1:[350,450]",
        # "G2:[250,350]",
        # "G2:[350,450]",
        "H:[250,500]",
        "H:[500,+#infty]",
    ]

    cor_labels = [
        'I:[250,350]',
        'I:[350,450]',
        'I:[450,550]',
        'I:[550,750]',
        'I:[750,+#infty]',
        'J:[250,350]',
        'J:[350,450]',
        'J:[450,550]',
        'J:[550,750]',
        'J:[750,+#infty]',
    ]

    if len(srNames) >= 18:
        return std_labels+cor_labels
    elif len(srNames) == 16:
        return std_labels
    elif len(srNames) == 13:
        return cr0b_labels+cor_labels
    elif len(srNames) == 11:
        return cr0b_labels
    elif len(srNames) == 1 and srNames[0][-1] == 'I':
        return cor_labels

    return std_labels+cor_labels

def DrawTextPad(textpad):
    textpad.cd()
    textpad.SetFillColor(0);
    textpad.SetBorderMode(0);
    textpad.SetBorderSize(2);
    textpad.SetTickx(1);
    textpad.SetTicky(1);
    textpad.SetLeftMargin(0.07);
    textpad.SetRightMargin(0.05);
    textpad.SetTopMargin(0.07);
    textpad.SetBottomMargin(0.28);
    textpad.SetFrameFillStyle(0);
    textpad.SetFrameBorderMode(0);

    txt = r.TLatex()
    txt.SetNDC()
    txt.SetTextFont(42)
    txt.SetTextAlign(13)
    txt.SetLineWidth(2)
    txt.SetTextSize(0.075)
    step = 0.06
    lmar = textpad.GetLeftMargin()
    tmar = 1-textpad.GetTopMargin()

    txt.DrawLatex(lmar+0.09, tmar-step*0 ,"  N_{J}")
    ls1 = r.TLine();
    ls1.DrawLineNDC(0.05, tmar-step*0.8, 0.87, tmar-step*0.8)

    ls1.DrawLineNDC(lmar+0.085, tmar-step*8.8, lmar+0.085, tmar)

    txt.DrawLatex(lmar, tmar-step*1 ,"A   2#minus3")
    txt.DrawLatex(lmar, tmar-step*2 ,"B   2#minus3")
    txt.DrawLatex(lmar, tmar-step*3 ,"C   #geq 4")
    txt.DrawLatex(lmar, tmar-step*4 ,"D   #geq 4")
    txt.DrawLatex(lmar, tmar-step*5 ,"E   #geq 4")
    txt.DrawLatex(lmar, tmar-step*6 ,"F   #geq 4")
    txt.DrawLatex(lmar, tmar-step*7 ,"G   #geq 4")
    txt.DrawLatex(lmar, tmar-step*8 ,"H   #geq 4")

    ls1.DrawLineNDC(lmar+0.25, tmar-step*8.8, lmar+0.25, tmar)

    txt.DrawLatex(lmar+0.28, tmar-step*0 ," #font[12]{t}_{mod}  ")
    txt.DrawLatex(lmar+0.28, tmar-step*1 ," > 10 ")
    txt.DrawLatex(lmar+0.28, tmar-step*2 ," > 10 ")
    txt.DrawLatex(lmar+0.28, tmar-step*3 ," #leq 0 ")
    txt.DrawLatex(lmar+0.28, tmar-step*4 ," #leq 0 ")
    txt.DrawLatex(lmar+0.28, tmar-step*5 ,"0#minus10 ")
    txt.DrawLatex(lmar+0.28, tmar-step*6 ,"0#minus10 ")
    txt.DrawLatex(lmar+0.28, tmar-step*7 ," > 10 ")
    txt.DrawLatex(lmar+0.28, tmar-step*8 ," > 10 ")

    ls1.DrawLineNDC(lmar+0.47, tmar-step*8.8, lmar+0.47, tmar)
    txt.DrawLatex(lmar+0.48, tmar-step*0 ," M_{#font[12]{l}b} [GeV] ")
    txt.DrawLatex(lmar+0.51, tmar-step*1 ," #leq 175")
    txt.DrawLatex(lmar+0.51, tmar-step*2 ," > 175")
    txt.DrawLatex(lmar+0.51, tmar-step*3 ," #leq 175")
    txt.DrawLatex(lmar+0.51, tmar-step*4 ," > 175")
    txt.DrawLatex(lmar+0.51, tmar-step*5 ," #leq 175")
    txt.DrawLatex(lmar+0.51, tmar-step*6 ," > 175")
    txt.DrawLatex(lmar+0.51, tmar-step*7 ," #leq 175")
    txt.DrawLatex(lmar+0.51, tmar-step*8 ," > 175")

    # txt.DrawLatex(lmar, tmar-step*9.2 ,"#splitline{X0: Incl tag,    X1: No top,}{X2: Mer. top,  X3: Res. top}")
    txt.DrawLatex(lmar, tmar-step*9.3  ,"X0: Inclusive")
    txt.DrawLatex(lmar, tmar-step*10.1 ,"X1: Untagged")
    # txt.DrawLatex(lmar, tmar-step*10.9 ,"X2: Boosted top")
    # txt.DrawLatex(lmar, tmar-step*11.7 ,"X3: Resolved top")
    txt.DrawLatex(lmar, tmar-step*10.9 ,"X2: Merged t quark tag")
    txt.DrawLatex(lmar, tmar-step*11.7 ,"X3: Resolved t quark tag")

    txt.DrawLatex(lmar, tmar-step*13 ," I: N_{J} #geq 5, N_{b,med} #geq 1")
    txt.DrawLatex(lmar, tmar-step*14 ,"J: N_{J} #geq 3, N_{b,soft} #geq 1")


def DrawHeaderText(canvas, lumi=137, prelim=True):
    tCMS = r.TLatex()
    tCMS.SetNDC(1)
    tCMS.SetTextFont(61)
    tCMS.SetTextAlign(11)
    tCMS.SetTextSize(0.0675)
    canvas.cd()
    tCMS.DrawLatex(canvas.GetLeftMargin(), 1.0-canvas.GetTopMargin()+0.012, "CMS")

    tplm = r.TLatex()
    tplm.SetNDC(1)
    tplm.SetTextFont(52)
    tplm.SetTextAlign(11)
    tplm.SetTextSize(0.052)
    canvas.cd()
    if prelim == True:
        tplm.DrawLatex(canvas.GetLeftMargin()+0.059, 1.0-canvas.GetTopMargin()+0.012, "Preliminary")
    elif type(prelim) == str:
        tplm.DrawLatex(canvas.GetLeftMargin()+0.059, 1.0-canvas.GetTopMargin()+0.012, prelim)

    ttext = r.TLatex()
    ttext.SetNDC(1)
    ttext.SetTextFont(42)
    ttext.SetTextAlign(31)
    ttext.SetTextSize(0.058)
    canvas.cd()
    text = "{0} {1}^{{-1}} ({2} TeV)".format(lumi,"fb",13)
    ttext.DrawLatex(1.0-canvas.GetRightMargin()-0.01, 1.0-canvas.GetTopMargin()+0.012, text)


def getSRHistFromYieldE(ylds, hname, htitle=None, fcolor=None, lcolor=None, lstyle=None):
    if htitle is None: htitle = hname
    n_srbins = len(ylds)

    hist = r.TH1D(hname, htitle, n_srbins, 0, n_srbins)
    for i, yld in enumerate(ylds):
        hist.SetBinContent(i+1, yld.val)
        hist.SetBinError(i+1, yld.err)

    if fcolor is not None:
        hist.SetLineWidth(1)
        hist.SetLineColor(r.kGray+2)
        # hist.SetLineColor(fcolor+2)
        hist.SetFillColor(fcolor)

    if lcolor is not None:
        hist.SetLineWidth(3)
        hist.SetLineColor(lcolor)

    if lstyle is not None:
        hist.SetLineWidth(3)
        hist.SetLineStyle(lstyle)

    return hist

def drawSRyieldHist(hist, xlabels, hleg=None, savename='sigYieldHist.pdf', drawops='hist', linear=False, yrange=None, hline=None, ytitle='Events'):
    if yrange is None: yrange = [0.001, 100]

    width = max(len(xlabels)*40, 1200)
    c0 = r.TCanvas('c0', 'c0', width, 800) # todo: adjustable canvas width later

    r.gStyle.SetOptStat('')
    r.gStyle.SetPadGridX(0)
    r.gStyle.SetPadGridY(0)
    r.gStyle.SetPadTickX(1)
    r.gStyle.SetPadTickY(1)
    r.gStyle.SetFrameBorderMode(0)

    mainPad = r.TPad('1', '1', 0.0, 0.05, 1.0, 1.0)
    ratioPad = r.TPad('2', '2', 0.0, 0.0, 1.0, 0.23)

    r.SetOwnership(c0, False)
    r.SetOwnership(mainPad, False)
    r.SetOwnership(ratioPad, False)

    mainPad.SetTopMargin(0.08)
    mainPad.SetLeftMargin(0.10)
    mainPad.SetRightMargin(0.05)
    mainPad.SetBottomMargin(0.22)
    if not linear: mainPad.SetLogy()
    mainPad.Draw()
    ratioPad.SetTopMargin(0.05)
    ratioPad.SetLeftMargin(0.10)
    ratioPad.SetRightMargin(0.05)

    mainPad.cd()

    hist.GetXaxis().SetLabelSize(0.048)
    for i, lab in enumerate(xlabels):
        hist.GetXaxis().SetBinLabel(i+1, lab)
    hist.GetXaxis().LabelsOption('v')

    hist.GetYaxis().SetTitle(ytitle)
    hist.GetYaxis().SetTitleOffset(0.5)
    hist.GetYaxis().SetTitleSize(0.06)
    hist.GetYaxis().SetRangeUser(yrange[0], yrange[1])
    hist.Draw(drawops)

    if hleg is None: hleg = hist.GetNname()
    if hleg is not 'noleg':
        leg = r.TLegend(0.42, 0.80, 0.68, 0.89)
        leg.AddEntry(hist, hleg)
        leg.Draw()

    if hline is not None:
        line = r.TF1('line1', '{}'.format(hline), 0, len(xlabels)+1)
        line.SetLineStyle(2)
        line.SetLineColor(r.kGray+2)
        line.Draw('same')

    c0.SaveAs(savename)

def drawSRyieldStack(hstk, xlabels, legitems=None, savename='sigYieldHist.pdf', drawops='hist', linear=False, hline=None,
                     hdata=None, hsigs=None, gsyst=None, ytitle='Events', noRatio=False, noBkgError=False, yrange=None,
                     drawTextPad=False, signals=[]):

    r.gStyle.SetOptStat('')
    r.gStyle.SetPadGridX(0)
    r.gStyle.SetPadGridY(0)
    r.gStyle.SetPadTickX(1)
    r.gStyle.SetPadTickY(1)
    r.gStyle.SetFrameBorderMode(0)

    width = max(len(xlabels)*40, 1200) if noRatio else max(len(xlabels)*50, 1600)

    if not hdata and not gsyst: noRatio = True

    if yrange == None: yrange = [ 0.1, 5000 ]

    if drawTextPad and not noRatio:
        c0 = r.TCanvas('c0', 'c0', width+400, 1200)
        mainPad = r.TPad('1', '1', 0.0, 0.22, 0.82, 0.99)
        ratioPad = r.TPad('2', '2', 0.0, 0.0, 0.82, 0.245)
        textPad = r.TPad('3', '3', 0.79, 0.02, 1.0, 0.99)
    elif not noRatio:
        c0 = r.TCanvas('c0', 'c0', width, 1200)
        mainPad = r.TPad('1', '1', 0.0, 0.20, 1.0, 0.99)
        ratioPad = r.TPad('2', '2', 0.0, 0.02, 1.0, 0.23)
    else:
        c0 = r.TCanvas('c0', 'c0', width, 1000) # todo: adjustable canvas width later
        mainPad = r.TPad('1', '1', 0.0, 0.05, 1.0, 1.0)
        ratioPad = r.TPad('2', '2', 0.0, 0.0, 1.0, 0.23)

    r.SetOwnership(c0, False)
    r.SetOwnership(mainPad, False)
    r.SetOwnership(ratioPad, False)

    mainPad.SetTopMargin(0.08)
    mainPad.SetLeftMargin(0.10)
    mainPad.SetRightMargin(0.05)
    mainPad.SetBottomMargin(0.22)
    if not linear: mainPad.SetLogy()
    mainPad.Draw()
    ratioPad.SetTopMargin(0.05)
    ratioPad.SetLeftMargin(0.10)
    ratioPad.SetRightMargin(0.05)
    ratioPad.SetBottomMargin(0.2)
    if not noRatio: ratioPad.Draw()
    if drawTextPad: textPad.Draw()
    mainPad.cd()

    if hstk.GetHists().GetSize() < 1:
        print 'Invalid hist stack passed in!!'; return False

    htot = hstk.GetHists().At(0).Clone('h_tot')

    for i in range(1, hstk.GetHists().GetSize()):
        htot.Add(hstk.GetHists().At(i))

    htot.GetXaxis().SetLabelSize(0.048)
    for i, lab in enumerate(xlabels):
        htot.GetXaxis().SetBinLabel(i+1, lab)

    htot.GetXaxis().LabelsOption('v')
    htot.GetYaxis().SetTitle(ytitle)
    htot.GetYaxis().SetTitleOffset(0.45)
    htot.GetYaxis().SetTitleSize(0.058)
    htot.GetYaxis().SetTickSize(0.015)
    htot.GetYaxis().SetRangeUser(yrange[0], yrange[1])
    # htot.GetYaxis().SetRangeUser(0.0001, 10)

    htot.Draw(drawops)
    hstk.Draw('histsame')
    htot.Draw('axissame');

    g_bkgerr = None
    if not noBkgError and not gsyst:
        h_err = htot.Clone('h_err')
        h_err.SetFillStyle(3244)
        h_err.SetFillColor(r.kGray+2)
        h_err.Draw("E2same")
    elif gsyst:
        g_bkgerr = r.TGraphAsymmErrors()
        for i in range(gsyst.GetN()):
            ybkg = htot.GetBinContent(i+1)
            g_bkgerr.SetPoint(i, i+0.5, ybkg)
            g_bkgerr.SetPointError(i, 0.5, 0.5, ybkg*gsyst.GetErrorYhigh(i), ybkg*gsyst.GetErrorYlow(i))
        g_bkgerr.SetFillStyle(3244)
        g_bkgerr.SetFillColor(r.kGray+2)
        g_bkgerr.Draw("E2same")

    if hdata != None:
        g_data = r.TGraphAsymmErrors()
        ppu.ConvertToPoissonGraph(hdata, g_data, drawZeros=True)
        if drawTextPad:
            for i in range(g_data.GetN()):
                g_data.SetPointEXlow(i, 0)
                g_data.SetPointEXhigh(i, 0)
        # g_data.SetPointError(g_data.GetN()-1, 0, 0, 0, 0)
        g_data.SetMarkerStyle(20)
        g_data.SetMarkerSize(2)
        g_data.SetLineWidth(1)

        # draw the graph and then axes again on top
        g_data.Draw("SAME P")

        # hdata.SetMarkerStyle(20)
        # hdata.SetMarkerSize(1.2)
        # hdata.Draw('PEsame')

    if type(hsigs) == list:
        # sig_colors = [  r.kTeal, r.kPink, r.kOrange-7, r.kCyan, ]
        sig_colors = [  r.kGreen, r.kCyan, r.kMagenta, r.kOrange, r.kSpring,  ]
        sig_linesty = [ 2, 7, 9, 1, 2 ]
        for i, (sn, hsig) in enumerate(hsigs):
            hsig.SetLineStyle(sig_linesty[i]);
            hsig.SetLineWidth(2);
            hsig.SetLineColor(sig_colors[i]);
            hsig.Draw('same hist')

    if type(legitems) == list:
        leg = r.TLegend(0.52, 0.76, 0.76, 0.89)
        if width < 1000:
            # leg = r.TLegend(0.65, 0.64, 0.91, 0.89)
            leg = r.TLegend(0.52, 0.78, 0.89, 0.89)
        if len(legitems) > 2:
            leg.SetNColumns(2)
        if hdata != None:
            leg = r.TLegend(0.47, 0.68, 0.76, 0.89)
            if 'CR0b' in savename: 
                leg = r.TLegend(0.42, 0.76, 0.71, 0.89)
            if len(xlabels)==39:
                leg = r.TLegend(0.56, 0.68, 0.9, 0.89)
            leg.SetNColumns(2)

        # for i in range(len(legitems)-1, -1, -1):
        #     leg.AddEntry(hstk.GetHists().At(i), legitems[i], 'lf')
        #     if hdata != None and i == len(legitems)-2:
        #         leg.AddEntry(g_data, 'Data', 'lp')
        for i in [4,1,3,0,2]:
            if hdata != None and i == len(legitems):
                leg.AddEntry(g_data, 'Observed', 'lp')
            elif i < len(legitems):
                leg.AddEntry(hstk.GetHists().At(i), legitems[i], 'lf')

        if g_bkgerr != None:
            leg.AddEntry(g_bkgerr, 'Total Uncert.', 'lpf')

        if type(hsigs) == list:
            # leg.Clear()
            leg2 = r.TLegend(0.13, 0.75, 0.45, 0.89)
            if len(hsigs) > 2:
                leg2.SetNColumns(2)
            leg2.SetBorderSize(0)
            leg2.SetTextSize(0.047)
            for i, (sn, hsig) in enumerate(hsigs):
                leg2.AddEntry(hsig, sn, 'lf')
            leg2.Draw()

        # leg.SetEntrySeparation(0.2)
        leg.SetTextSize(0.047)
        # leg.SetMargin(0.2)
        leg.Draw()


    if drawTextPad:
        vlineI = r.TLine(39, 0, 39, yrange[1])
        if 'CR0b' in savename: vlineI = r.TLine(30, 0, 30, yrange[1])
        vlineI.SetLineStyle(7)
        vlineI.SetLineWidth(2)
        vlineI.SetLineColor(r.kGray+2)
        vlineI.Draw()
        vlineJ = r.TLine(44, 0, 44, yrange[1])
        if 'CR0b' in savename: vlineJ = r.TLine(35, 0, 35, yrange[1])
        vlineJ.SetLineStyle(7)
        vlineJ.SetLineWidth(2)
        vlineJ.SetLineColor(r.kGray+2)
        vlineJ.Draw()


    if type(hsigs) == list and len(hsigs) > 1:
        DrawHeaderText(mainPad, prelim='Supplementary')
    if len(xlabels) == 39:
        DrawHeaderText(mainPad, prelim='  Preliminary')
    else:
        DrawHeaderText(mainPad, prelim=False)

    ratioPad.cd()
    ratiomax = 4
    if not noRatio:
        h_axis_ratio = r.TH1D('ratio_axis','', htot.GetNbinsX(), 0, htot.GetNbinsX())
        h_axis_ratio.GetYaxis().SetNdivisions(4)
        h_axis_ratio.GetYaxis().SetLabelSize(0.15)
        h_axis_ratio.GetYaxis().SetRangeUser(0,ratiomax)
        # h_axis_ratio.GetYaxis().SetTitle('Ratios  ')
        h_axis_ratio.GetYaxis().SetTitle('Obs./Exp.')
        h_axis_ratio.GetYaxis().SetTitleOffset(0.13)
        h_axis_ratio.GetYaxis().SetTitleSize(0.18)
        h_axis_ratio.GetYaxis().SetTickLength(0.01)
        h_axis_ratio.GetXaxis().SetTickLength(0.07)
        h_axis_ratio.GetXaxis().SetTitle('Signal Regions')
        h_axis_ratio.GetXaxis().SetTitleSize(0.15)
        h_axis_ratio.GetXaxis().SetTitleOffset(0.45)
        h_axis_ratio.GetXaxis().SetLabelSize(0.)
        h_axis_ratio.Draw('axis')

        line = r.TF1('line1', '1', 0, len(xlabels)+1)
        line.SetLineStyle(2)
        line.SetLineColor(r.kGray+2)
        line.Draw('same')

        if gsyst != None:
            gsyst.SetFillStyle(3244)
            gsyst.SetFillColor(r.kGray+2)
            gsyst.Draw('same 2')
            line.Draw('same')
            h_axis_ratio.Draw('axissame')

        if hdata:
            hRatio = hdata.Clone('h_ratio')
            # hRatio.Divide(hdata, htot, 1, 1, 'B')
            hRatio.Divide(htot)
            ratioGraph = r.TGraphAsymmErrors()
            for i in range(1, htot.GetNbinsX()+1):
                htot.SetBinError(i, 0)
            ppu.GetPoissonRatioGraph(htot,hdata,ratioGraph)
            if drawTextPad:
                for i in range(ratioGraph.GetN()):
                    ratioGraph.SetPointEXlow(i, 0)
                    ratioGraph.SetPointEXhigh(i, 0)
            ratioGraph.SetMarkerStyle(20)
            ratioGraph.SetMarkerSize(2.2)
            ratioGraph.SetMarkerColor(r.kGray+3)
            ratioGraph.SetLineColor(r.kGray+3)
            ratioGraph.SetLineWidth(2)
            ratioGraph.Draw('PZsame')

        if drawTextPad:
            rvlI = r.TLine(39, 0, 39, ratiomax)
            if 'CR0b' in savename: rvlI = r.TLine(30, 0, 30, ratiomax)
            rvlI.SetLineStyle(7)
            rvlI.SetLineWidth(2)
            rvlI.SetLineColor(r.kGray+2)
            rvlI.Draw()
            rvlJ = r.TLine(44, 0, 44, ratiomax)
            if 'CR0b' in savename: rvlJ = r.TLine(35, 0, 35, ratiomax)
            rvlJ.SetLineStyle(7)
            rvlJ.SetLineWidth(2)
            rvlJ.SetLineColor(r.kGray+2)
            rvlJ.Draw()

    if drawTextPad:
        DrawTextPad(textPad)

    c0.SaveAs(savename)
    # c0.SaveAs(savename[:-4]+".C")
    c0.Clear()


def drawBkgEstimateHists(indir, srNames, suf='', bkgType='lostlep'):

    if bkgType == 'lostlep':
        f1 = r.TFile(indir+'/lostlepton_run2.root')
    elif bkgType == '1lepW':
        f1 = r.TFile(indir+'/1lepFromW_run2.root')

    preds  = [ y.round(2) for y in sum(getYieldEInTopoBins(f1, srNames, 'metbins'), []) ]
    alpha  = [ y.round(2) for y in sum(getYieldEInTopoBins(f1, srNames, 'alphaHist'), []) ]
    purity = [ y.round(2) for y in sum(getYieldEInTopoBins(f1, srNames, 'CRpurity'), []) ]
    yMC_SR = sum(getYieldEInTopoBins(f1, srNames, 'MCyields_SR'), [])
    yMC_CR = sum(getYieldEInTopoBins(f1, srNames, 'MCyields_CR'), [])
    yld_CR = sum(getYieldEInTopoBins(f1, srNames, 'datayields_CR'), [])

    rat_MC = [ (d/m).round(2) for d, m in zip(yld_CR, yMC_CR) ]

    # xlabels = ["{}:[{},{}]".[sr, bins] for sr, bins in zip(srNames, metrange)]
    xlabels = getLabelsTemporary(srNames)

    h_alpha = getSRHistFromYieldE(alpha, 'h_alpha', '', lcolor=r.kRed-7)
    drawSRyieldHist(h_alpha, xlabels, 'Transfer Factors', bkgType+suf+'_alphas_log.pdf', 'PE', ytitle='')
    yrange = [0, 1.8] if bkgType == 'lostlep' else [0, 0.8] if bkgType == '1lepW' else [0, 2.5]
    drawSRyieldHist(h_alpha, xlabels, 'Transfer Factors', bkgType+suf+'_alphas_lin.pdf', 'PE', True, yrange , ytitle='')

    # h_ratio = getSRHistFromYieldE(rat_MC, 'hrat_dataMC_CR', '', lcolor=r.kGray+3)
    # drawSRyieldHist(h_ratio, xlabels, 'noleg', bkgType+suf+'_CRratio.pdf', 'PE', True, [0, 2], 1)

    h_purity = getSRHistFromYieldE(purity, 'h_CRpurity', '', lcolor=r.kGray+2)
    drawSRyieldHist(h_purity, xlabels, 'noleg', bkgType+suf+'_CRpurity.pdf', 'PE', True, [0, 1.2], 1, ytitle='')


def drawBkgCompositionStack(indir, srNames=None, savename='bkgCompostion_std.pdf', plotData=False, normalize=False, drawTextPad=False, drawSignal=False):

    # -------------------------------------------------------
    # Draw test bkg composition / expected yields hists
    if srNames == None:
        srNames = ['srA0', 'srA1', 'srA2', 'srB', 'srC','srD', 'srE0', 'srE1', 'srE2', 'srE3', 'srF', 'srG0', 'srG1', 'srG2', 'srG3', 'srH',]

    # f_bkg = r.TFile('../StopLooper/output/combRun2_v37_s5/allBkg_run2.root')
    f_bkg = r.TFile(indir+'/allBkg_run2.root','read')

    y_all  = [ y for y in sum(getYieldEInTopoBins(f_bkg, srNames, 'metbins'), []) ]

    y_2lep = [ y for y in sum(getYieldEInTopoBins(f_bkg, srNames, 'metbins_2lep'), []) ]
    y_1lt  = [ y for y in sum(getYieldEInTopoBins(f_bkg, srNames, 'metbins_1lepTop'), []) ]
    y_1lW  = [ y for y in sum(getYieldEInTopoBins(f_bkg, srNames, 'metbins_1lepW'), []) ]
    y_Zinv = [ y for y in sum(getYieldEInTopoBins(f_bkg, srNames, 'metbins_Znunu'), []) ]

    if not plotData and normalize:
        y_tot  = [ y1+y2+y3+y4 for y1, y2, y3, y4 in zip(y_2lep, y_1lt, y_1lW, y_Zinv)]
        y_2lep = [ yi/ya for yi, ya in zip(y_2lep, y_tot) ]
        y_1lt  = [ yi/ya for yi, ya in zip(y_1lt , y_tot) ]
        y_1lW  = [ yi/ya for yi, ya in zip(y_1lW , y_tot) ]
        y_Zinv = [ yi/ya for yi, ya in zip(y_Zinv, y_tot) ]

    h_2lep = getSRHistFromYieldE(y_2lep, 'h_SRyields_2lep', '', fcolor=r.kAzure+6)
    h_1lW  = getSRHistFromYieldE(y_1lW , 'h_SRyields_1lW' , '', fcolor=r.kOrange-4)
    h_1lt  = getSRHistFromYieldE(y_1lt , 'h_SRyields_1lt' , '', fcolor=r.kRed-7)
    h_Zinv = getSRHistFromYieldE(y_Zinv, 'h_SRyields_Zinv', '', fcolor=r.kViolet-8)

    signals=None
    if type(drawSignal) == str:
        signame = drawSignal
        signals = []
        f_ttH = r.TFile(indir+'/'+signame+'_run2.root','read')
        y_sig = [ y.round(4) for y in sum(getYieldEInTopoBins(f_ttH, srNames, 'metbins'), []) ]
        h_sig = getSRHistFromYieldE(y_sig, 'h_SRyields_'+signame , '', lstyle=1 )
        signals.append((signame, h_sig))

    hstk = r.THStack('hs1', '')
    if 'cr0b' in srNames[0]:
        legname = ['1lepTop', 'Zinv', '2lep', '1lepW']
        hstk.Add(h_1lt)
        hstk.Add(h_Zinv)
        hstk.Add(h_2lep)
        hstk.Add(h_1lW)
    elif 'cr2l' in srNames[0]:
        legname = ['Z#rightarrow#nu#nu', '1lepW', '1lepTop', 't#bar{t}/tW#rightarrow2#font[12]{l}']
        hstk.Add(h_Zinv)
        hstk.Add(h_1lW)
        hstk.Add(h_1lt)
        hstk.Add(h_2lep)
    else:
        legname = ['1#font[12]{l} from t', 'Z#rightarrow#nu#nu', '1#font[12]{l} from W', 't#bar{t}/tW#rightarrow2#font[12]{l}']
        hstk.Add(h_1lt)
        hstk.Add(h_Zinv)
        hstk.Add(h_1lW)
        hstk.Add(h_2lep)

    xlabels = getLabelsTemporary(srNames)

    if plotData:
        f_data = r.TFile(indir+'/allData_run2.root','read')
        y_data  = [ y.round(2) for y in sum(getYieldEInTopoBins(f_data, srNames, 'metbins'), []) ]
        h_data = getSRHistFromYieldE(y_data, 'h_SRyields_data', '', fcolor=r.kBlack)

        drawSRyieldStack(hstk, xlabels, legname, savename, 'hist', hdata=h_data, hsigs=signals, drawTextPad=drawTextPad)
    elif normalize:
        drawSRyieldStack(hstk, xlabels, legname, savename, 'hist', linear=True, yrange=[0.0,1.3], noBkgError=True, ytitle='Bkg Fraction')
    else:
        drawSRyieldStack(hstk, xlabels, legname, savename, 'hist')

    f_bkg.Close()


def drawBkgPredictionStack(indir, srNames=None, savename='bkgPrediction_std.pdf', ysuf='run2', plotData=False, plotRatio=True, scale=1, 
                           drawTextPad=False, drawSignal=False, grayScale=False):

    # -------------------------------------------------------
    # Draw test bkg composition / expected yields hists
    if srNames == None:
        srNames = ['srA0', 'srA1', 'srA2', 'srB', 'srC','srD', 'srE0', 'srE1', 'srE2', 'srE3', 'srF', 'srG0', 'srG1', 'srG2', 'srG3', 'srH', 'srI',]

    f_lostlep = r.TFile(indir+'/lostlepton_'+ysuf+'.root','read')
    f_1lepW   = r.TFile(indir+'/1lepFromW_'+ysuf+'.root','read')
    f_1lepTop = r.TFile(indir+'/1lepFromTop_'+ysuf+'.root','read')
    f_Zinv    = r.TFile(indir+'/ZToNuNu_'+ysuf+'.root','read')

    # y_all  = [ y.round(2) for y in sum(getYieldEInTopoBins(f_bkg, srNames, 'metbins'), []) ]
    # scale = 1

    y_2lep = [ (scale*y).round(4) for y in sum(getYieldEInTopoBins(f_lostlep, srNames, 'metbins'), []) ]
    y_1lt  = [ (scale*y).round(4) for y in sum(getYieldEInTopoBins(f_1lepTop, srNames, 'metbins'), []) ]
    y_1lW  = [ (scale*y).round(4) for y in sum(getYieldEInTopoBins(f_1lepW,   srNames, 'metbins'), []) ]
    y_Zinv = [ (scale*y).round(4) for y in sum(getYieldEInTopoBins(f_Zinv,    srNames, 'metbins'), []) ]
    y_tot  = [ (y1+y2+y3+y4) for y1, y2, y3, y4 in zip(y_2lep, y_1lt, y_1lW, y_Zinv)]

    h_2lep = getSRHistFromYieldE(y_2lep, 'h_SRyields_lostlep' , '', fcolor=r.kAzure+8   if not grayScale else 16)
    h_1lW  = getSRHistFromYieldE(y_1lW , 'h_SRyields_1lepTop' , '', fcolor=r.kOrange-2  if not grayScale else 15)
    h_1lt  = getSRHistFromYieldE(y_1lt , 'h_SRyields_1lepW'   , '', fcolor=r.kRed-7     if not grayScale else 13)
    h_Zinv = getSRHistFromYieldE(y_Zinv, 'h_SRyields_Zinv'    , '', fcolor=r.kMagenta-3 if not grayScale else 14)

    if type(drawSignal) == list:
        f_sig = {}
        signals = []
        for signame in drawSignal:
            sigtype = '_'.join(signame.split('_')[:-2])
            mstop = int(signame.split('_')[-2])
            mlsp  = int(signame.split('_')[-1])
            if sigtype not in f_sig: f_sig[sigtype] = r.TFile(indir+'/SMS_{}_{}.root'.format(sigtype, ysuf), 'read')
            y_sig = [ (scale*y).round(4) for y in sum(getYieldEInTopoBins3D(f_sig[sigtype], srNames, mstop, mlsp, 'metbins'), []) ]
            h_sig = getSRHistFromYieldE(y_sig, 'h_SRyields_'+signame , '', lstyle=1 )
            signals.append(('{}({},{})'.format(sig_label.get(sigtype,'sigtype'),mstop,mlsp), h_sig))

    elif type(drawSignal) == str:
        signame = drawSignal
        signals = []
        f_ttH = r.TFile(indir+'/'+signame+'_'+ysuf+'.root','read')
        y_sig = [ (scale*y).round(4) for y in sum(getYieldEInTopoBins(f_ttH, srNames, 'metbins'), []) ]
        h_sig = getSRHistFromYieldE(y_sig, 'h_SRyields_'+signame , '', lstyle=1 )
        signals.append((signame, h_sig))

    # legname = ['t#bar{t}/tW#rightarrow1#font[12]{l}', 'Z#rightarrow#nu#nu', 'W+jets', 'Lost lepton']
    # legname = ['1#font[12]{l} from top', 'Z#rightarrow#nu#nu', '1#font[12]{l} not from top', 'Lost lepton']
    legname = ['1#font[12]{l} (from t)', 'Z#rightarrow#nu#bar{#nu}', '1#font[12]{l} (not from t)', 'Lost lepton']
    hstk = r.THStack('hs1', '')
    hstk.Add(h_1lt)
    hstk.Add(h_Zinv)
    hstk.Add(h_1lW)
    hstk.Add(h_2lep)

    xlabels = getLabelsTemporary(srNames)

    yrange = [0.1*scale, 5000*scale] if scale != 1.0 else None

    # TODO: to take the size of thie systs from the combine output!!
    systup = [0.0,]*len(y_tot)
    systdn = [0.0,]*len(y_tot)
    for s in ['MC','data']:
        stat_2lep = [ (scale*y).round(4) for y in sum(getYieldEInTopoBins(f_lostlep, srNames, 'metbins_'+s+'Stats'), []) ]
        stat_1lt  = [ (scale*E(y.val, y.val)).round(4) for y in y_1lt ]
        stat_1lW  = [ (scale*y).round(4) for y in sum(getYieldEInTopoBins(f_1lepW,   srNames, 'metbins_'+s+'Stats', 'metbins'), []) ]
        stat_Zinv = y_Zinv
        stat_tot = [ y1+y2+y3+y4 for y1, y2, y3, y4 in zip(stat_2lep, stat_1lt, stat_1lW, stat_Zinv)]
        for i in range(len(systup)):
            if stat_tot[i].val <= 0:
                staterr = y_tot[i].err/ y_tot[i].val
            else:
                staterr = stat_tot[i].err / stat_tot[i].val
            systup[i] = sqrt(systup[i]**2 + staterr**2)
            systdn[i] = systup[i]

    # systs = ['jes','metRes', 'lepSF','alphas', 'q2', 'ISR',  'WbXsec', 'pdf', 'tauSF']
    systs = ['jes','metRes', 'metTTbar', 'WbXsec', 'ISR', 'lepSF', 'ttagSF', 'softbSF']

    for sys in systs:
        sysup_2lep = [ (scale*y).round(4).val for y in sum(getYieldEInTopoBins(f_lostlep, srNames, 'metbins_'+sys+'Up', 'metbins'), []) ]
        sysup_1lt  = [ (scale*y).round(4).val for y in sum(getYieldEInTopoBins(f_1lepTop, srNames, 'metbins_'+sys+'Up', 'metbins'), []) ]
        sysup_1lW  = [ (scale*y).round(4).val for y in sum(getYieldEInTopoBins(f_1lepW,   srNames, 'metbins_'+sys+'Up', 'metbins'), []) ]
        sysup_Zinv = [ (scale*y).round(4).val for y in sum(getYieldEInTopoBins(f_Zinv,    srNames, 'metbins_'+sys+'Up', 'metbins'), []) ]
        sysup_tot = [ y1+y2+y3+y4 for y1, y2, y3, y4 in zip(sysup_2lep, sysup_1lt, sysup_1lW, sysup_Zinv)]
        for i in range(len(systup)):
            systup[i] = sqrt(systup[i]**2 + (sysup_tot[i]/y_tot[i].val-1)**2)

        sysdn_2lep = [ (scale*y).round(4).val for y in sum(getYieldEInTopoBins(f_lostlep, srNames, 'metbins_'+sys+'Dn', 'metbins'), []) ]
        sysdn_1lt  = [ (scale*y).round(4).val for y in sum(getYieldEInTopoBins(f_1lepTop, srNames, 'metbins_'+sys+'Dn', 'metbins'), []) ]
        sysdn_1lW  = [ (scale*y).round(4).val for y in sum(getYieldEInTopoBins(f_1lepW,   srNames, 'metbins_'+sys+'Dn', 'metbins'), []) ]
        sysdn_Zinv = [ (scale*y).round(4).val for y in sum(getYieldEInTopoBins(f_Zinv,    srNames, 'metbins_'+sys+'Dn', 'metbins'), []) ]
        sysdn_tot = [ y1+y2+y3+y4 for y1, y2, y3, y4 in zip(sysdn_2lep, sysdn_1lt, sysdn_1lW, sysdn_Zinv)]
        for i in range(len(systdn)):
            systdn[i] = sqrt(systdn[i]**2 + (1-sysdn_tot[i]/y_tot[i].val)**2)

    if systs != None:
        g_sys = r.TGraphAsymmErrors()
        for i in range(len(systup)):
            thisPoint = g_sys.GetN()
            g_sys.SetPoint(thisPoint, i+0.5, 1)
            g_sys.SetPointError(thisPoint, 0.5, 0.5, systup[i], systdn[i])

    if plotData:
        f_data = r.TFile(indir+'/allData_'+ysuf+'.root')
        if type(plotData) == str:
            f_data = r.TFile(indir+'/'+plotData+'.root')
        y_data = [ y.round(2) for y in sum(getYieldEInTopoBins(f_data, srNames, 'metbins'), []) ]
        h_data = getSRHistFromYieldE(y_data, 'h_SRyields_yields', '', fcolor=r.kBlack)
        if drawSignal:
            drawSRyieldStack(hstk, xlabels, legname, savename, 'hist', hdata=h_data, hsigs=signals, gsyst=g_sys, yrange=yrange, drawTextPad=drawTextPad)
        else:
            drawSRyieldStack(hstk, xlabels, legname, savename, 'hist', hdata=h_data, gsyst=g_sys, yrange=yrange, drawTextPad=drawTextPad)
    else:
        drawSRyieldStack(hstk, xlabels, legname, savename, 'hist', gsyst=g_sys, yrange=yrange)

    f_lostlep.Close()
    f_1lepW.Close()
    f_1lepTop.Close()
    f_Zinv.Close()


def drawBkgCompositionStackCorr():

    # -------------------------------------------------------
    # Draw test bkg composition hists
    srNames = ['srI',]

    f_ttZ = r.TFile('../StopLooper/output/samp17_v13_tf/TTZToLLNuNu.root')
    f_ttW = r.TFile('../StopLooper/output/samp17_v13_tf/TTWJetsToLNu.root')

    y_ttZ  = [ y.round(2) for y in sum(getYieldEInTopoBins(f_ttZ, srNames, 'metbins'), []) ]
    y_ttW  = [ y.round(2) for y in sum(getYieldEInTopoBins(f_ttW, srNames, 'metbins'), []) ]

    h_ttZ = getSRHistFromYieldE(y_ttZ, 'h_SRyields_ttZ', '', lcolor=r.kOrange, fcolor=r.kOrange)
    h_ttW = getSRHistFromYieldE(y_ttW, 'h_SRyields_ttW', '', lcolor=r.kGreen-3, fcolor=r.kGreen-3)

    hstk = r.THStack('hs1', '')
    hstk.Add(h_ttW)
    hstk.Add(h_ttZ)

    # xlabels = getLabelsTemporary()
    xlabels = ['I:[250,350]', 'I:[350,450]', 'I:[450,550]', 'I:[550,700]', 'I:[700,+#infty]']

    drawSRyieldStack(hstk, xlabels, ['ttW', 'ttZ'], 'bkgCompostion_ttV.pdf', 'hist', True)

def drawHEMVetoEffect():

    srNames = ['srA0', 'srA1', 'srA2', 'srB', 'srC','srD', 'srE0', 'srE1', 'srE2', 'srE3', 'srF', 'srG0', 'srG1', 'srG2', 'srG3', 'srH', 'srI',]

    f_bkg_w = r.TFile('../StopLooper/output/samp18_v39_s5_hemrwtd/allBkg_18.root')
    f_bkg_n = r.TFile('../StopLooper/output/samp18_v39_s5/allBkg_18.root')

    f_sig_w = r.TFile('../StopLooper/output/samp18_v39_s5_hemrwtd/SMS_T2tt_18.root')
    f_sig_n = r.TFile('../StopLooper/output/samp18_v39_s5/SMS_T2tt_18.root')

    y_bkg_w = [ y for y in sum(getYieldEInTopoBins(f_bkg_w, srNames, 'metbins'), []) ]
    y_bkg_n = [ y for y in sum(getYieldEInTopoBins(f_bkg_n, srNames, 'metbins'), []) ]

    y_sig1_w = [ y for y in sum(getYieldEInTopoBins(f_sig_w, srNames, 'metbins_1200_50'), []) ]
    y_sig1_n = [ y for y in sum(getYieldEInTopoBins(f_sig_n, srNames, 'metbins_1200_50'), []) ]

    y_sig2_w = [ y for y in sum(getYieldEInTopoBins(f_sig_w, srNames, 'metbins_800_400'), []) ]
    y_sig2_n = [ y for y in sum(getYieldEInTopoBins(f_sig_n, srNames, 'metbins_800_400'), []) ]

    rat_bkg  = [ get_efficiency(n,d) for n, d in zip(y_bkg_w, y_bkg_n) ]
    rat_sig1 = [ get_efficiency(n,d) for n, d in zip(y_sig1_w, y_sig1_n) ]
    rat_sig2 = [ get_efficiency(n,d) for n, d in zip(y_sig2_w, y_sig2_n) ]

    xlabels = getLabelsTemporary(srNames)
    # xlabels = srNames
    hrat_bkg  = getSRHistFromYieldE(rat_bkg, 'h_SRyields_HEMjetVeto_bkg', '')
    hrat_sig1 = getSRHistFromYieldE(rat_sig1,'h_SRyields_HEMjetVeto_1200_50', '')
    hrat_sig2 = getSRHistFromYieldE(rat_sig2,'h_SRyields_HEMjetVeto_800_400', '')
    hrat_bkg.SetLineWidth(2)
    hrat_sig1.SetLineWidth(2)
    hrat_sig2.SetLineWidth(2)

    drawSRyieldHist(hrat_bkg,  xlabels, 'noleg', 'HEMVetos_allbkg_18.pdf', 'PE', True, [0.6, 1.1], 1, '')
    drawSRyieldHist(hrat_sig1, xlabels, 'noleg', 'HEMVetos_T2tt_1200_50_18.pdf', 'PE', True, [0.6, 1.1], 1, '')
    drawSRyieldHist(hrat_sig2, xlabels, 'noleg', 'HEMVetos_T2tt_800_400_18.pdf', 'PE', True, [0.6, 1.1], 1, '')

    # y_bkg_w = [ y.round(3) for y in sum(getYieldEInTopoBins(f_bkg_w, srNames, 'metbins'), []) ]
    # y_bkg_n = [ y.round(3) for y in sum(getYieldEInTopoBins(f_bkg_n, srNames, 'metbins'), []) ]


def drawDifftt1lMETres():

    srNames = ['srA0', 'srA1', 'srA2', 'srB', 'srC','srD', 'srE0', 'srE1', 'srE2', 'srE3', 'srF', 'srG0', 'srG1', 'srG2', 'srG3', 'srH', 'srI', 'srJ']

    f_bkg_w = r.TFile('../StopLooper/output/combRun2_v31_s6/allBkg_run2.root')
    f_bkg_n = r.TFile('../StopLooper/output/combRun2_v39_s6_noMETres/allBkg_run2.root')

    y_bkg_w = [ y for y in sum(getYieldEInTopoBins(f_bkg_w, srNames, 'metbins_1lepTop'), []) ]
    y_bkg_n = [ y for y in sum(getYieldEInTopoBins(f_bkg_n, srNames, 'metbins_1lepTop'), []) ]

    rat_bkg  = [ E(0,n.val) if d.val <= 0.01 else n/d for n, d in zip(y_bkg_w, y_bkg_n) ]

    xlabels = getLabelsTemporary(srNames)
    # xlabels = srNames
    hrat_bkg  = getSRHistFromYieldE(rat_bkg, 'h_SRyields_metres_tt1l', '')
    hrat_bkg.SetLineWidth(2)

    drawSRyieldHist(hrat_bkg,  xlabels, 'noleg', 'METres_1lepTop_run2.pdf', 'PE', True, [0.0, 3], 1, '')

    # y_bkg_w = [ y.round(3) for y in sum(getYieldEInTopoBins(f_bkg_w, srNames, 'metbins'), []) ]
    # y_bkg_n = [ y.round(3) for y in sum(getYieldEInTopoBins(f_bkg_n, srNames, 'metbins'), []) ]

def drawDiffMHTCutEffect():

    srNames = ['srA0', 'srA1', 'srA2', 'srB', 'srC','srD', 'srE0', 'srE1', 'srE2', 'srE3', 'srF', 'srG0', 'srG1', 'srG2', 'srG3', 'srH', 'srI', 'srJ']

    f_bkg_w = r.TFile('../StopLooper/output/samp17_v31_s3_mhtcut/allBkg_17.root')
    f_bkg_n = r.TFile('../StopLooper/output/samp17_v31_s3/allBkg_17.root')

    f_sig_w = r.TFile('../StopLooper/output/samp18_v31_s3_mhtcut/SMS_T2tt_18.root')
    f_sig_n = r.TFile('../StopLooper/output/samp18_v31_s3/SMS_T2tt_18.root')

    y_bkg_w = [ y for y in sum(getYieldEInTopoBins(f_bkg_w, srNames, 'metbins'), []) ]
    y_bkg_n = [ y for y in sum(getYieldEInTopoBins(f_bkg_n, srNames, 'metbins'), []) ]

    y_sig1_w = [ y for y in sum(getYieldEInTopoBins(f_sig_w, srNames, 'metbins_1200_50'), []) ]
    y_sig1_n = [ y for y in sum(getYieldEInTopoBins(f_sig_n, srNames, 'metbins_1200_50'), []) ]

    y_sig2_w = [ y for y in sum(getYieldEInTopoBins(f_sig_w, srNames, 'metbins_800_400'), []) ]
    y_sig2_n = [ y for y in sum(getYieldEInTopoBins(f_sig_n, srNames, 'metbins_800_400'), []) ]

    rat_bkg  = [ get_efficiency(n,d) for n, d in zip(y_bkg_w, y_bkg_n) ]
    rat_sig1 = [ get_efficiency(n,d) for n, d in zip(y_sig1_w, y_sig1_n) ]
    rat_sig2 = [ get_efficiency(n,d) for n, d in zip(y_sig2_w, y_sig2_n) ]

    xlabels = getLabelsTemporary(srNames)
    # xlabels = srNames
    hrat_bkg  = getSRHistFromYieldE(rat_bkg, 'h_SRyields_diffMHTcut_bkg', '')
    hrat_sig1 = getSRHistFromYieldE(rat_sig1,'h_SRyields_diffMHTcut_1200_50', '')
    hrat_sig2 = getSRHistFromYieldE(rat_sig2,'h_SRyields_diffMHTcut_800_400', '')
    hrat_bkg.SetLineWidth(2)
    hrat_sig1.SetLineWidth(2)
    hrat_sig2.SetLineWidth(2)

    # drawSRyieldHist(hrat_bkg,  xlabels, 'noleg', 'HT5overHTcut_allbkg_17.png', 'PE', True, [0.6, 1.1], 1, '')
    # drawSRyieldHist(hrat_sig1, xlabels, 'noleg', 'HT5overHTcut_T2tt_1200_50_18.pdf', 'PE', True, [0.6, 1.1], 1, '')
    # drawSRyieldHist(hrat_sig2, xlabels, 'noleg', 'HT5overHTcut_T2tt_800_400_18.pdf', 'PE', True, [0.6, 1.1], 1, '')

    drawSRyieldHist(hrat_bkg,  xlabels, 'noleg', 'HT5overHTcut_allbkg_17.pdf', 'PE', True, [0.6, 1.1], 1, '')
    # drawSRyieldHist(hrat_sig1, xlabels, 'noleg', 'MHTcut_T2tt_1200_50_18.pdf', 'PE', True, [0.6, 1.1], 1, '')
    # drawSRyieldHist(hrat_sig2, xlabels, 'noleg', 'MHTcut_T2tt_800_400_18.pdf', 'PE', True, [0.6, 1.1], 1, '')

    # y_bkg_w = [ y.round(3) for y in sum(getYieldEInTopoBins(f_bkg_w, srNames, 'metbins'), []) ]
    # y_bkg_n = [ y.round(3) for y in sum(getYieldEInTopoBins(f_bkg_n, srNames, 'metbins'), []) ]


def drawL1prefireEffect():

    srNames = ['srA0', 'srA1', 'srA2', 'srB', 'srC','srD', 'srE0', 'srE1', 'srE2', 'srE3', 'srF', 'srG0', 'srG1', 'srG2', 'srG3', 'srH', 'srI',]

    f_bkg_w = r.TFile('../StopLooper/output/samp17_v39_s1/allBkg_17.root')
    f_bkg_n = r.TFile('../StopLooper/output/samp17_v39_s2/allBkg_17.root')

    f_sig_w = r.TFile('../StopLooper/output/samp17_v39_s1/SMS_T2tt_17.root')
    f_sig_n = r.TFile('../StopLooper/output/samp17_v39_s2/SMS_T2tt_17.root')

    y_bkg_w = [ y for y in sum(getYieldEInTopoBins(f_bkg_w, srNames, 'metbins'), []) ]
    y_bkg_n = [ y for y in sum(getYieldEInTopoBins(f_bkg_n, srNames, 'metbins'), []) ]

    y_sig1_w = [ y for y in sum(getYieldEInTopoBins(f_sig_w, srNames, 'metbins_1200_50'), []) ]
    y_sig1_n = [ y for y in sum(getYieldEInTopoBins(f_sig_n, srNames, 'metbins_1200_50'), []) ]

    y_sig2_w = [ y for y in sum(getYieldEInTopoBins(f_sig_w, srNames, 'metbins_800_400'), []) ]
    y_sig2_n = [ y for y in sum(getYieldEInTopoBins(f_sig_n, srNames, 'metbins_800_400'), []) ]

    # y_bkg_w = [ sum(y, E(0,0)) for y in getYieldEInTopoBins(f_bkg_w, srNames, 'metbins') ]
    # y_bkg_n = [ sum(y, E(0,0)) for y in getYieldEInTopoBins(f_bkg_n, srNames, 'metbins') ]

    rat_bkg  = [ get_efficiency(n,d) for n, d in zip(y_bkg_w, y_bkg_n) ]
    rat_sig1 = [ get_efficiency(n,d) for n, d in zip(y_sig1_w, y_sig1_n) ]
    rat_sig2 = [ get_efficiency(n,d) for n, d in zip(y_sig2_w, y_sig2_n) ]

    xlabels = getLabelsTemporary(srNames)
    # xlabels = srNames
    hrat_bkg  = getSRHistFromYieldE(rat_bkg, 'h_SRyields_L1wgtd_bkg', '')
    hrat_sig1 = getSRHistFromYieldE(rat_sig1,'h_SRyields_L1wgtd_1200_50', '')
    hrat_sig2 = getSRHistFromYieldE(rat_sig2,'h_SRyields_L1wgtd_800_400', '')
    hrat_bkg.SetLineWidth(2)
    hrat_sig1.SetLineWidth(2)
    hrat_sig2.SetLineWidth(2)

    drawSRyieldHist(hrat_bkg,  xlabels, 'noleg', 'L1prefire_allbkg_17.pdf', 'PE', True, [0.6, 1.1], 1, '')
    drawSRyieldHist(hrat_sig1, xlabels, 'noleg', 'L1prefire_T2tt_1200_50_17.pdf', 'PE', True, [0.6, 1.1], 1, '')
    drawSRyieldHist(hrat_sig2, xlabels, 'noleg', 'L1prefire_T2tt_800_400_17.pdf', 'PE', True, [0.6, 1.1], 1, '')

    # y_bkg_w = [ y.round(3) for y in sum(getYieldEInTopoBins(f_bkg_w, srNames, 'metbins'), []) ]
    # y_bkg_n = [ y.round(3) for y in sum(getYieldEInTopoBins(f_bkg_n, srNames, 'metbins'), []) ]


def drawImpactInTransferFactor():

    srNames = ['srA0', 'srA1', 'srA2', 'srB', 'srC','srD', 'srE0', 'srE1', 'srE2', 'srE3', 'srF', 'srG0', 'srG1', 'srG2', 'srG3', 'srH', 'srI', 'srJ']

    f_bkg_w = r.TFile('../StopLooper/output/samp17_v31_s3/lostlepton_17.root')
    f_bkg_n = r.TFile('../StopLooper/output/samp17_v39_s3/lostlepton_17.root')

    y_bkg_w = [ y for y in sum(getYieldEInTopoBins(f_bkg_w, srNames, 'alphaHist'), []) ]
    y_bkg_n = [ y for y in sum(getYieldEInTopoBins(f_bkg_n, srNames, 'alphaHist'), []) ]

    y_err_w = [ 0 if y.val == 0 else round(y.err/y.val,2) for y in sum(getYieldEInTopoBins(f_bkg_w, srNames, 'metbins_MCStats'), []) ]
    y_err_n = [ 0 if y.val == 0 else round(y.err/y.val,2) for y in sum(getYieldEInTopoBins(f_bkg_n, srNames, 'metbins_MCStats'), []) ]

    # rat_bkg  = [ get_efficiency(n,d) for n, d in zip(y_bkg_w, y_bkg_n) ]
    rat_bkg  = [ E((n/d).round(2).val, en) for n, d, en, ed in zip(y_bkg_w, y_bkg_n, y_err_w, y_err_n) ]

    xlabels = getLabelsTemporary(srNames)
    hrat_bkg  = getSRHistFromYieldE(rat_bkg, 'h_SRyields_TFimpact', '')
    hrat_bkg.SetLineWidth(2)

    drawSRyieldHist(hrat_bkg,  xlabels, 'noleg', 'ImpactOnTF_METRes_17.pdf', 'PE', True, [0.6, 1.4], 1, '')

def drawYieldsRatioComparison(srNames=None, savename='srYieldCompare_METResOnvsOff_17.pdf'):

    if srNames == None:
        srNames = ['srA0', 'srA1', 'srA2', 'srB', 'srC','srD', 'srE0', 'srE1', 'srE2', 'srE3', 'srF', 'srG0', 'srG1', 'srG2', 'srG3', 'srH', 'srI',]

    f1 = r.TFile('../StopLooper/output/samp17_v39_mrs2/allBkg_17.root')
    f2 = r.TFile('../StopLooper/output/samp17_v39_mrs0/allBkg_17.root')

    yld1 = [ y for y in sum(getYieldEInTopoBins(f1, srNames, 'metbins'), []) ]
    yld2 = [ y for y in sum(getYieldEInTopoBins(f2, srNames, 'metbins'), []) ]

    # rat_bkg  = [ get_efficiency(n,d) for n, d in zip(yld1, yld2) ]
    rat_bkg  = [ (n/d).round(4) for n, d in zip(yld1, yld2) ]

    xlabels = getLabelsTemporary(srNames)
    # xlabels = srNames
    hrat_bkg  = getSRHistFromYieldE(rat_bkg, 'h_SRyields_L1wgtd_bkg', '')
    hrat_bkg.SetLineWidth(2)

    drawSRyieldHist(hrat_bkg,  xlabels, 'noleg', savename, 'PE', True, [0.6, 1.4], 1)


def drawAllPlots():

    bvsuf = 'v31_s21'
    indir = '../StopLooper/output/combRun2_'+bvsuf
    # indir = '../StopLooper/output/samp17_v39_mrs2'

    # f18 = r.TFile('../StopLooper/output/samp18_'+bvsuf+'/lostlepton_18.root')
    # f17 = r.TFile('../StopLooper/output/samp17_'+bvsuf+'/lostlepton_17.root')
    # f16 = r.TFile('../StopLooper/output/samp16_'+bvsuf+'/lostlepton_16.root')
    # frun2 = r.TFile('../StopLooper/output/combRun2_'+bvsuf+'/lostlepton_run2.root')

    # srNames = ['srI',]

    # drawBkgEstimateHists(f18, srNames, '18')
    # drawBkgEstimateHists(f17, srNames, '17')
    # drawBkgEstimateHists(f16, srNames, '16')

    srNames = ['srA0', 'srA1', 'srA2', 'srB', 'srC', 'srD', 'srE0', 'srE1', 'srE2', 'srE3', 'srF', 'srG0', 'srG1', 'srG2', 'srG3', 'srH', 'srI', 'srJ']
    # drawBkgCompositionStack(indir, srNames, 'SRCompostion_all_s12.pdf')
    # drawBkgCompositionStack(indir, srNames, 'SRCompostion_norm_s12.pdf', normalize=True)

    # drawYieldsRatioComparison()

    drawBkgEstimateHists(indir, srNames, 'run2', 'lostlep')

    crNames = [ sr.replace('sr', 'cr2l') for sr in srNames ]
    # drawBkgCompositionStack(indir, crNames, 'CR2lYields_s12_all.pdf', plotData=True)
    drawBkgCompositionStack(indir, crNames, 'CR2lYields_s21_atp.pdf', plotData=True, drawTextPad=True)

    # drawYieldsRatioComparison(crNames, 'cr2lYieldCompare_METResOnvsOff_17.pdf')

    srNames = ['srA0', 'srA1', 'srB', 'srC', 'srD', 'srE0', 'srE1', 'srF', 'srG0', 'srG1', 'srH', 'srI', 'srJ']
    # drawBkgEstimateHists(indir, srNames, 'run2', '1lepW')

    crNames = [ sr.replace('sr', 'cr0b') for sr in srNames ]
    # drawBkgCompositionStack(indir, crNames, 'CR0bYields_s12_all.pdf', plotData=True)
    drawBkgCompositionStack(indir, crNames, 'CR0bYields_s21_atp.pdf', plotData=True, drawTextPad=True)

    # drawYieldsRatioComparison(crNames, 'cr0bYieldCompare_METResOnvsOff_17.pdf')

    # # # # # # # # # # # # # # #
    # Data driven predictions
    # srNames = ['srA0', 'srA1', 'srA2', 'srB', 'srC', 'srD', 'srE0', 'srE1', 'srE2', 'srE3', 'srF', 'srG0', 'srG1', 'srG2', 'srG3', 'srH', 'srI', 'srJ',]
    # drawBkgPredictionStack(indir, srNames, 'bkgPrediction_all.pdf')

    exit()
    # # # # # # # # # # # # # # #
    # Standard search bins only

    srNames = ['srA0', 'srA1', 'srA2', 'srB', 'srC', 'srD', 'srE0', 'srE1', 'srE2', 'srE3', 'srF', 'srG0', 'srG1', 'srG2', 'srG3', 'srH', ]
    drawBkgCompositionStack(indir, srNames, 'SRCompostion_std.pdf')

    crNames = [ sr.replace('sr', 'cr2l') for sr in srNames ]
    drawBkgCompositionStack(indir, crNames, 'CR2lYields_std.pdf', plotData=True)

    srNames = ['srA0', 'srA1', 'srB', 'srC', 'srD', 'srE0', 'srE1', 'srF', 'srG0', 'srG1', 'srH', ]
    # drawBkgEstimateHists(indir, srNames, 'run2', '1lepW')

    crNames = [ sr.replace('sr', 'cr0b') for sr in srNames ]
    drawBkgCompositionStack(indir, crNames, 'CR0bYields_std.pdf', plotData=True)

    drawBkgPredictionStack(indir, srNames, 'bkgPrediction_std.pdf')

    # drawBkgCompositionStack(indir, srNames, 'bkgCompostion_all.pdf')
    # drawBkgCompositionStack(['srI',], 'bkgCompostion_cor6.pdf')
    # drawBkgCompositionStack(['cr2lI'], 'CR2lCompostion_cor6.pdf')
    # drawBkgCompositionStack(['cr0bI'], 'CR0bCompostion_cor6.pdf')

    # drawL1prefireEffect()
    # drawHEMVetoEffect()

    # # # # # # # # # # # # # # #
    # Year separated

    indir16 = '../StopLooper/output/samp16_'+bvsuf
    indir17 = '../StopLooper/output/samp17_'+bvsuf
    indir18 = '../StopLooper/output/samp18_'+bvsuf

    srNames = ['srA0', 'srA1', 'srA2', 'srB', 'srC', 'srD', 'srE0', 'srE1', 'srE2', 'srE3', 'srF', 'srG0', 'srG1', 'srG2', 'srG3', 'srH', 'srI', 'srJ',]
    drawBkgCompositionStack(indir16, srNames, 'SRCompostion_2016_all.pdf')

    crNames = [ sr.replace('sr', 'cr0b') for sr in srNames ]
    drawBkgCompositionStack(indir16, crNames, 'CR0bYields_2016_all.pdf', plotData=True)



if __name__ == '__main__':

    r.gROOT.SetBatch(1)

    bvsuf = 'v31_s21'
    indir = '../StopLooper/output/combRun2_'+bvsuf
    indir16 = '../StopLooper/output/samp16_'+bvsuf

    srNames = ['srA0', 'srA1', 'srA2', 'srB', 'srC', 'srD', 'srE0', 'srE1', 'srE2', 'srE3', 'srF', 'srG0', 'srG1', 'srG2', 'srG3', 'srH', 'srI', 'srJ']
    # drawBkgPredictionStack(indir16, srNames, 'bkgPrediction_2016.pdf', '16', plotData=True)

    # drawBkgPredictionStack(indir, srNames, 'bkgPrediction_run2.pdf', 'run2', plotData=False)
    # drawBkgPredictionStack(indir, srNames, 'bkgPrediction_unblind_2017BCDE.pdf', 'run2', plotData='data_2017BCDE_5fb')
    # drawBkgPredictionStack(indir, srNames, 'bkgPrediction_unblind_2017_10fb.pdf', 'run2', plotData='data_2017_10fb')
    # drawBkgPredictionStack(indir, srNames, 'bkgPrediction_unblind_2018_15fb.pdf', 'run2', plotData='data_2018ABD_15fb', scale=15.0/137.2)
    # drawBkgPredictionStack(indir, srNames, 'bkgPrediction_unblind_2018_full.pdf', 'run2', plotData='allData_18', scale=59.7/137.2)
    # drawBkgPredictionStack(indir, srNames, 'bkgPrediction_unblind_2016_full.pdf', 'run2', plotData='allData_16', scale=35.9/137.2)
    # drawBkgPredictionStack(indir, srNames, 'bkgPrediction_unblind_run2_61fb.pdf', 'run2', plotData='data_run2_61fb', scale=60.9/137.2)
    # drawBkgPredictionStack(indir, srNames, 'Results_prefit_run2_noJ_s8.pdf', 'run2', plotData='allData_run2')
    # drawBkgPredictionStack(indir, srNames, 'Results_prefit_run2_all_s12.pdf', 'run2', plotData='allData_run2')
    # drawBkgPredictionStack(indir, srNames, 'Results_prefit_test2_s12.pdf', 'run2', plotData='allData_run2')
    # drawBkgPredictionStack(indir, srNames, 'Results_prefit_test_s21_6.pdf', 'run2', plotData='allData_run2', drawTextPad=True) # results

    sig_pts = ['T2tt_1050_100', 'T2bW_950_100', 'T2bt_750_400']
    # drawBkgPredictionStack(indir, srNames, 'Results_prefit_s21_sigs_2.pdf', 'run2', plotData='allData_run2', drawTextPad=True, drawSignal=sig_pts) # signal

    sig_pts = ['T2tt_850_400', 'T2tt_1050_100']
    # drawBkgPredictionStack(indir, srNames, 'Results_prefit_s21_sigs_3.pdf', 'run2', plotData='allData_run2', drawTextPad=True, drawSignal=sig_pts) # signal

    srNames = ['srA0', 'srA1', 'srA2', 'srB', 'srC', 'srD', 'srE0', 'srE1', 'srE2', 'srE3', 'srF', 'srG0', 'srG1', 'srG2', 'srG3', 'srH',]
    # drawBkgPredictionStack(indir, srNames, 'Results_prefit_ttH_s7_3.pdf', 'run2', plotData='allData_run2', drawTextPad=True, drawSignal='ttHtoInv') # signal
    # crNames = [ sr.replace('sr', 'cr2l') for sr in srNames ]
    # drawBkgCompositionStack(indir, crNames, 'CR2lYields_s7_ttH_1.pdf', plotData=True, drawTextPad=True, drawSignal='ttHtoInv')

    # ttbarDM signals
    indir = '../StopLooper/output/sampttDM_v32_m10'
    sig_pts = ['ttbarDM_pseudo_200_1', 'ttbarDM_scalar_100_1']
    drawBkgPredictionStack(indir, srNames, 'Results_prefit_ttDM_m10_1.pdf', 'run2', plotData='allData_run2', drawTextPad=True, drawSignal=sig_pts) # signal


    bkgnames = ['2l', '1lW', 'znunu',  '1ltop',]
    systnames = ['jes', 'metTTbar', ]
    # GetUncertaintiesFromDatacard('../CombineAnalysis/datacards/scan_samp16_v39_m9/combinedcards/datacard_std_T2tt_1250_50.txt', bkgnames, systnames, True)

    # drawAllPlots()

    # drawDiffMHTCutEffect()

    # drawDifftt1lMETres()

    # drawImpactInTransferFactor()

    bvsuf = 'v31_cor_w3'
    indir = '../StopLooper/output/combRun2_'+bvsuf
    srNames = ['srI', 'srJ0', 'srJ3',]

    # drawBkgPredictionStack(indir, srNames, 'Results_prefit_run2_w3.pdf', 'run2', plotData='allData_run2')
