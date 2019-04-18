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

def GetSystRatioGraph(h_mc, h_data, g_ratio, drawZeros=True, useMCErr=True):
    pass

def GetSystTemporary(bkgtype, systype):
    if bkgtype == 'lostlep':
        systups = {
            'ISRUp'       : [ 0.006,   0.014,   0.003,   0.010,   0.005,   0.002,   0.001,   0.001,   0.012,   0.021,   0.029,   0.002,   0.001,   0.012,   0.014,   0.037,   0.070,   0.044,   0.001,   0.001,   0.006,   0.019,   0.051,   0.010,   0.000,   0.030,   0.025,   0.065,   0.023,   0.057,   0.086,   0.003,   0.004,   0.000,   0.000,   0.078,   0.069,   0.078,   0.022 ],
            'alphasUp'    : [ 0.065,   0.095,   0.000,   0.000,   0.066,   0.009,   0.022,   0.021,   0.001,   0.001,   0.005,   0.019,   0.036,   0.000,   0.000,   0.044,   0.030,   0.047,   0.045,   0.001,   0.016,   0.000,   0.000,   0.015,   0.000,   0.035,   0.046,   0.069,   0.023,   0.025,   0.055,   0.021,   0.024,   0.000,   0.000,   0.042,   0.063,   0.039,   0.007 ],
            'bTagEffHFUp' : [ 0.017,   0.028,   0.002,   0.000,   0.001,   0.004,   0.027,   0.014,   0.001,   0.003,   0.002,   0.015,   0.001,   0.001,   0.000,   0.007,   0.037,   0.004,   0.039,   0.002,   0.005,   0.006,   0.014,   0.002,   0.000,   0.002,   0.014,   0.017,   0.010,   0.004,   0.016,   0.007,   0.005,   0.000,   0.000,   0.017,   0.019,   0.002,   0.019 ],
            'bTagEffLFUp' : [ 0.003,   0.002,   0.003,   0.006,   0.001,   0.009,   0.047,   0.050,   0.006,   0.007,   0.003,   0.003,   0.023,   0.011,   0.002,   0.000,   0.013,   0.005,   0.009,   0.000,   0.003,   0.009,   0.026,   0.009,   0.000,   0.007,   0.003,   0.000,   0.009,   0.001,   0.002,   0.004,   0.001,   0.000,   0.000,   0.036,   0.031,   0.054,   0.235 ],
            'jesUp'       : [ 0.011,   0.025,   0.028,   0.001,   0.043,   0.044,   0.041,   0.042,   0.008,   0.008,   0.015,   0.018,   0.054,   0.005,   0.009,   0.007,   0.001,   0.041,   0.343,   0.007,   0.037,   0.030,   0.102,   0.037,   0.000,   0.128,   0.169,   0.063,   0.018,   0.041,   0.034,   0.091,   0.009,   0.000,   0.000,   0.999,   0.151,   0.030,   0.068 ],
            'lepSFUp'     : [ 0.009,   0.000,   0.018,   0.005,   0.009,   0.011,   0.006,   0.003,   0.019,   0.051,   0.022,   0.010,   0.011,   0.027,   0.040,   0.019,   0.002,   0.015,   0.018,   0.019,   0.009,   0.008,   0.003,   0.032,   0.000,   0.022,   0.009,   0.003,   0.018,   0.019,   0.022,   0.044,   0.095,   0.000,   0.000,   0.060,   0.059,   0.334,   0.006 ],
            'pileupUp'    : [ 0.125,   0.088,   0.005,   0.004,   0.035,   0.041,   0.107,   0.007,   0.002,   0.005,   0.008,   0.055,   0.027,   0.002,   0.003,   0.011,   0.056,   0.044,   0.074,   0.007,   0.001,   0.047,   0.041,   0.012,   0.000,   0.006,   0.194,   0.011,   0.018,   0.047,   0.036,   0.007,   0.064,   0.000,   0.000,   0.072,   0.056,   0.065,   0.039 ],
            'q2Up'        : [ 0.009,   0.005,   0.000,   0.000,   0.018,   0.009,   0.004,   0.020,   0.001,   0.002,   0.004,   0.004,   0.011,   0.000,   0.000,   0.015,   0.003,   0.015,   0.000,   0.001,   0.014,   0.000,   0.000,   0.005,   0.000,   0.020,   0.005,   0.305,   0.001,   0.012,   0.036,   0.001,   0.013,   0.000,   0.000,   0.017,   0.005,   0.025,   0.026 ],
            'tauSFUp'     : [ 0.009,   0.008,   0.005,   0.004,   0.005,   0.005,   0.009,   0.009,   0.006,   0.006,   0.006,   0.007,   0.006,   0.005,   0.006,   0.007,   0.011,   0.007,   0.006,   0.004,   0.005,   0.003,   0.003,   0.003,   0.000,   0.002,   0.011,   0.000,   0.005,   0.015,   0.015,   0.008,   0.005,   0.000,   0.000,   0.006,   0.006,   0.005,   0.002 ],
        }
        systdns = {
            'ISRDn'       :  [ 0.007,   0.016,   0.004,   0.011,   0.005,   0.002,   0.001,   0.001,   0.016,   0.030,   0.040,   0.003,   0.002,   0.017,   0.019,   0.050,   0.113,   0.057,   0.007,   0.001,   0.009,   0.028,   0.083,   0.014,   0.000,   0.038,   0.032,   0.089,   0.032,   0.079,   0.130,   0.003,   0.005,   0.000,   0.000,   0.092,   0.084,   0.115,   0.026 ],
            'alphasDn'    :  [ 0.039,   0.027,   0.000,   0.000,   0.007,   0.029,   0.008,   0.004,   0.001,   0.014,   0.021,   0.027,   0.030,   0.000,   0.000,   0.002,   0.038,   0.030,   0.030,   0.002,   0.001,   0.000,   0.000,   0.026,   0.000,   0.014,   0.078,   0.039,   0.029,   0.022,   0.018,   0.013,   0.021,   0.000,   0.000,   0.020,   0.012,   0.045,   0.013 ],
            'bTagEffHFDn' :  [ 0.017,   0.027,   0.002,   0.000,   0.001,   0.004,   0.027,   0.014,   0.001,   0.003,   0.002,   0.016,   0.001,   0.001,   0.000,   0.007,   0.035,   0.004,   0.036,   0.002,   0.005,   0.006,   0.014,   0.002,   0.000,   0.002,   0.016,   0.016,   0.011,   0.003,   0.016,   0.007,   0.005,   0.000,   0.000,   0.017,   0.019,   0.002,   0.020 ],
            'bTagEffLFDn' :  [ 0.003,   0.002,   0.003,   0.006,   0.001,   0.009,   0.054,   0.054,   0.006,   0.007,   0.003,   0.004,   0.022,   0.011,   0.002,   0.000,   0.013,   0.005,   0.009,   0.000,   0.002,   0.009,   0.028,   0.009,   0.000,   0.007,   0.003,   0.000,   0.009,   0.001,   0.001,   0.004,   0.001,   0.000,   0.000,   0.035,   0.031,   0.051,   0.191 ],
            'jesDn'       :  [ 0.013,   0.029,   0.009,   0.072,   0.176,   0.003,   0.037,   0.052,   0.005,   0.004,   0.004,   0.001,   0.011,   0.010,   0.006,   0.062,   0.061,   0.079,   0.082,   0.034,   0.020,   0.017,   0.009,   0.019,   0.000,   0.066,   0.556,   0.123,   0.042,   0.032,   0.094,   0.001,   0.058,   0.000,   0.000,   0.086,   0.297,   0.021,   0.078 ],
            'lepSFDn'     :  [ 0.030,   0.025,   0.055,   0.031,   0.051,   0.047,   0.040,   0.025,   0.059,   0.094,   0.064,   0.051,   0.053,   0.066,   0.081,   0.057,   0.040,   0.056,   0.063,   0.059,   0.050,   0.031,   0.039,   0.074,   0.000,   0.056,   0.036,   0.044,   0.059,   0.061,   0.069,   0.087,   0.159,   0.000,   0.000,   0.103,   0.102,   0.558,   0.040 ],
            'pileupDn'    :  [ 0.172,   0.098,   0.005,   0.009,   0.034,   0.063,   0.109,   0.031,   0.001,   0.009,   0.013,   0.056,   0.053,   0.002,   0.002,   0.020,   0.058,   0.030,   0.179,   0.007,   0.000,   0.019,   0.059,   0.011,   0.000,   0.001,   0.161,   0.049,   0.006,   0.066,   0.042,   0.007,   0.057,   0.000,   0.000,   0.044,   0.074,   0.069,   0.057 ],
            'q2Dn'        :  [ 0.013,   0.014,   0.000,   0.000,   0.020,   0.011,   0.004,   0.025,   0.002,   0.002,   0.003,   0.002,   0.008,   0.000,   0.000,   0.019,   0.004,   0.024,   0.002,   0.000,   0.015,   0.000,   0.000,   0.009,   0.000,   0.025,   0.006,   0.320,   0.004,   0.014,   0.040,   0.001,   0.016,   0.000,   0.000,   0.021,   0.001,   0.034,   0.036 ],
            'tauSFDn'     :  [ 0.009,   0.008,   0.005,   0.004,   0.005,   0.005,   0.009,   0.009,   0.006,   0.006,   0.006,   0.007,   0.006,   0.005,   0.006,   0.007,   0.012,   0.007,   0.006,   0.004,   0.006,   0.003,   0.003,   0.003,   0.000,   0.002,   0.011,   0.000,   0.006,   0.015,   0.015,   0.008,   0.005,   0.000,   0.000,   0.006,   0.006,   0.005,   0.002 ],
        }

        return systups.get(systype), systdns.get(systype)

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

def DrawHeaderText(canvas, lumi=137.2):
    tCMS = r.TLatex()
    tCMS.SetNDC(1)
    tCMS.SetTextFont(61)
    tCMS.SetTextAlign(11)
    tCMS.SetTextSize(0.0625)
    canvas.cd()
    tCMS.DrawLatex(canvas.GetLeftMargin(), 1.0-canvas.GetTopMargin()+0.01, "CMS")

    tplm = r.TLatex()
    tplm.SetNDC(1)
    tplm.SetTextFont(52)
    tplm.SetTextAlign(11)
    tplm.SetTextSize(0.052)
    canvas.cd()
    tplm.DrawLatex(canvas.GetLeftMargin()+0.054, 1.0-canvas.GetTopMargin()+0.01, "Preliminary")

    ttext = r.TLatex()
    ttext.SetNDC(1)
    ttext.SetTextFont(42)
    ttext.SetTextAlign(31)
    ttext.SetTextSize(0.052)
    canvas.cd()
    text = "{0} {1}^{{-1}} ({2} TeV)".format(lumi,"fb",13)
    ttext.DrawLatex(1.0-canvas.GetRightMargin()-0.01, 1.0-canvas.GetTopMargin()+0.01, text)


def getSRHistFromYieldE(ylds, hname, htitle=None, fcolor=None, lcolor=None):
    if htitle is None: htitle = hname
    n_srbins = len(ylds)

    hist = r.TH1D(hname, htitle, n_srbins, 0, n_srbins)
    for i, yld in enumerate(ylds):
        hist.SetBinContent(i+1, yld.val)
        hist.SetBinError(i+1, yld.err)

    if fcolor is not None:
        hist.SetLineColor(r.kGray+2)
        hist.SetFillColor(fcolor)

    if lcolor is not None:
        hist.SetLineWidth(3)
        hist.SetLineColor(lcolor)

    return hist

def drawSRyieldHist(hist, xlabels, hleg=None, savename='sigYieldHist.pdf', drawops='hist', linear=False, yrange=None, hline=None, ytitle='N Events'):
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
                     hdata=None, hsigs=None, gsyst=None, ytitle='N Events', noRatio=False, noBkgError=False, yrange=None):

    r.gStyle.SetOptStat('')
    r.gStyle.SetPadGridX(0)
    r.gStyle.SetPadGridY(0)
    r.gStyle.SetPadTickX(1)
    r.gStyle.SetPadTickY(1)
    r.gStyle.SetFrameBorderMode(0)

    width = max(len(xlabels)*40, 1200) if noRatio else max(len(xlabels)*50, 1600)
    c0 = r.TCanvas('c0', 'c0', width, 1000) # todo: adjustable canvas width later
    mainPad = r.TPad('1', '1', 0.0, 0.05, 1.0, 1.0)
    ratioPad = r.TPad('2', '2', 0.0, 0.0, 1.0, 0.23)

    if not hdata and not gsyst: noRatio = True
    if not noRatio:
        c0 = r.TCanvas('c0', 'c0', width, 1200)
        mainPad = r.TPad('1', '1', 0.0, 0.20, 1.0, 0.99)
        ratioPad = r.TPad('2', '2', 0.0, 0.02, 1.0, 0.23)

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
    if not noRatio: ratioPad.Draw()
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
    htot.GetYaxis().SetRangeUser(0.1, 5000)
    if yrange != None:
        htot.GetYaxis().SetRangeUser(yrange[0], yrange[1])
    # htot.GetYaxis().SetRangeUser(0.0001, 10)

    htot.Draw(drawops)
    hstk.Draw('histsame')
    htot.Draw('axissame');

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
        # g_data.SetPointError(g_data.GetN()-1, 0, 0, 0, 0)
        g_data.SetMarkerStyle(20)
        g_data.SetMarkerSize(1.2)
        g_data.SetLineWidth(1)

        # draw the graph and then axes again on top
        g_data.Draw("SAME P")

        # hdata.SetMarkerStyle(20)
        # hdata.SetMarkerSize(1.2)
        # hdata.Draw('PEsame')

    if type(hsigs) == list:
        sig_colors = [  r.kTeal, r.kPink, r.kCyan, r.kGray+2,]
        for i, hsig in enumerate(hsigs):
            hsig.SetLineStyle(2);
            hsig.SetLineWidth(2);
            hsig.SetLineColor(sig_colors[i]);
            hsig.Draw('same')

    if type(legitems) == list:
        leg = r.TLegend(0.58, 0.76, 0.76, 0.89)
        if width < 1000:
            # leg = r.TLegend(0.65, 0.64, 0.91, 0.89)
            leg = r.TLegend(0.58, 0.78, 0.89, 0.89)
        if len(legitems) > 2:
            leg.SetNColumns(2)
        if hdata != None:
            leg = r.TLegend(0.49, 0.76, 0.76, 0.89)
            leg.SetNColumns(3)
        for i in range(len(legitems)-1, -1, -1):
            leg.AddEntry(hstk.GetHists().At(i), legitems[i], 'lf')
            if hdata != None and i == len(legitems)-2:
                leg.AddEntry(hdata, 'data', 'lp')

        leg.Draw()

    DrawHeaderText(mainPad)

    ratioPad.cd()
    if not noRatio:
        h_axis_ratio = r.TH1D('ratio_axis','', htot.GetNbinsX(), 0, htot.GetNbinsX())
        h_axis_ratio.GetYaxis().SetNdivisions(4)
        h_axis_ratio.GetYaxis().SetLabelSize(0.15)
        h_axis_ratio.GetYaxis().SetRangeUser(0,4)
        h_axis_ratio.GetYaxis().SetTitle('Ratios  ')
        h_axis_ratio.GetYaxis().SetTitleOffset(0.45)
        h_axis_ratio.GetYaxis().SetTitleSize(1)
        h_axis_ratio.GetXaxis().SetTickLength(0.07)
        h_axis_ratio.GetXaxis().SetTitleSize(0.)
        h_axis_ratio.GetXaxis().SetLabelSize(0.)
        h_axis_ratio.Draw('axis')

        line = r.TF1('line1', '1', 0, len(xlabels)+1)
        line.SetLineStyle(2)
        line.SetLineColor(r.kGray+2)
        line.Draw('same')

        if gsyst != None:
            gsyst.SetFillStyle(1001)
            gsyst.SetFillColor(r.kGray)
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
            ratioGraph.SetMarkerStyle(20)
            ratioGraph.SetMarkerSize(1.6)
            ratioGraph.SetMarkerColor(r.kGray+3)
            ratioGraph.SetLineColor(r.kGray+3)
            ratioGraph.SetLineWidth(2)
            ratioGraph.Draw('PZsame')

    c0.SaveAs(savename)
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


def drawBkgCompositionStack(indir, srNames=None, savename='bkgCompostion_std.pdf', plotData=False, normalize=False):

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
        y_2lep = [ yi/ya for yi, ya in zip(y_2lep, y_all) ]
        y_1lt  = [ yi/ya for yi, ya in zip(y_1lt , y_all) ]
        y_1lW  = [ yi/ya for yi, ya in zip(y_1lW , y_all) ]
        y_Zinv = [ yi/ya for yi, ya in zip(y_Zinv, y_all) ]

    h_2lep = getSRHistFromYieldE(y_2lep, 'h_SRyields_2lep', '', fcolor=r.kAzure+6)
    h_1lW  = getSRHistFromYieldE(y_1lW , 'h_SRyields_1lW' , '', fcolor=r.kOrange-4)
    h_1lt  = getSRHistFromYieldE(y_1lt , 'h_SRyields_1lt' , '', fcolor=r.kRed-7)
    h_Zinv = getSRHistFromYieldE(y_Zinv, 'h_SRyields_Zinv', '', fcolor=r.kViolet-8)

    hstk = r.THStack('hs1', '')
    if 'cr0b' in srNames[0]:
        legname = ['1lepTop', 'Zinv', '2lep', '1lepW']
        hstk.Add(h_1lt)
        hstk.Add(h_Zinv)
        hstk.Add(h_2lep)
        hstk.Add(h_1lW)
    elif 'cr2l' in srNames[0]:
        legname = ['Zinv', '1lepW', '1lepTop', '2lep']
        hstk.Add(h_Zinv)
        hstk.Add(h_1lW)
        hstk.Add(h_1lt)
        hstk.Add(h_2lep)
    else:
        legname = ['1lepTop', 'Zinv', '1lepW', '2lep']
        hstk.Add(h_1lt)
        hstk.Add(h_Zinv)
        hstk.Add(h_1lW)
        hstk.Add(h_2lep)

    xlabels = getLabelsTemporary(srNames)

    if plotData:
        f_data = r.TFile(indir+'/allData_run2.root','read')
        y_data  = [ y.round(2) for y in sum(getYieldEInTopoBins(f_data, srNames, 'metbins'), []) ]
        h_data = getSRHistFromYieldE(y_data, 'h_SRyields_data', '', fcolor=r.kBlack)

        drawSRyieldStack(hstk, xlabels, legname, savename, 'hist', hdata=h_data)
    elif normalize:
        drawSRyieldStack(hstk, xlabels, legname, savename, 'hist', linear=True, yrange=[0.0,1.3], noBkgError=True, ytitle='Bkg Fraction')
    else:
        drawSRyieldStack(hstk, xlabels, legname, savename, 'hist')

    f_bkg.Close()


def drawBkgPredictionStack(indir, srNames=None, savename='bkgPrediction_std.pdf', ysuf='run2', plotData=False, plotRatio=True, scale=1):

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

    h_2lep = getSRHistFromYieldE(y_2lep, 'h_SRyields_lostlep' , '', fcolor=r.kAzure+8)
    h_1lW  = getSRHistFromYieldE(y_1lW , 'h_SRyields_1lepTop' , '', fcolor=r.kOrange-2)
    h_1lt  = getSRHistFromYieldE(y_1lt , 'h_SRyields_1lepW'   , '', fcolor=r.kRed-7)
    h_Zinv = getSRHistFromYieldE(y_Zinv, 'h_SRyields_Zinv'    , '', fcolor=r.kMagenta-3)

    legname = ['1lepTop', 'Z#rightarrow#nu#nu', 'W+jets', 'lost-lep']
    hstk = r.THStack('hs1', '')
    hstk.Add(h_1lt)
    hstk.Add(h_Zinv)
    hstk.Add(h_1lW)
    hstk.Add(h_2lep)

    xlabels = getLabelsTemporary(srNames)

    yrange = [0.1*scale, 5000*scale] if scale != 1.0 else None

    systup = [0.0,]*len(y_tot)
    systdn = [0.0,]*len(y_tot)
    for s in ['MC','data']:
        stat_2lep = [ (scale*y).round(4) for y in sum(getYieldEInTopoBins(f_lostlep, srNames, 'metbins_'+s+'Stats'), []) ]
        stat_1lt  = y_1lt
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
    # systs = ['jes','metRes', 'lepSF', 'ISR',]
    systs = ['jes','metRes', 'metTTbar', 'WbXsec', 'ISR', 'lepSF' ]
    # systs = ['lepSF',  ]

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
        drawSRyieldStack(hstk, xlabels, legname, savename, 'hist', hdata=h_data, gsyst=g_sys, yrange=yrange)
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

    bvsuf = 'v31_s12'
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
    # drawBkgCompositionStack(indir, srNames, 'SRCompostion_norm_s12.pdf', normalize=True)

    # drawYieldsRatioComparison()

    # drawBkgEstimateHists(indir, srNames, 'run2', 'lostlep')

    crNames = [ sr.replace('sr', 'cr2l') for sr in srNames ]
    drawBkgCompositionStack(indir, crNames, 'CR2lYields_s12_all.pdf', plotData=True)

    # drawYieldsRatioComparison(crNames, 'cr2lYieldCompare_METResOnvsOff_17.pdf')

    srNames = ['srA0', 'srA1', 'srB', 'srC', 'srD', 'srE0', 'srE1', 'srF', 'srG0', 'srG1', 'srH', 'srI', 'srJ']
    # drawBkgEstimateHists(indir, srNames, 'run2', '1lepW')

    crNames = [ sr.replace('sr', 'cr0b') for sr in srNames ]
    # drawBkgCompositionStack(indir, crNames, 'CR0bYields_s12_all.pdf', plotData=True)

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

    bvsuf = 'v31_s12'
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
    drawBkgPredictionStack(indir, srNames, 'Results_prefit_run2_all_s12.pdf', 'run2', plotData='allData_run2')

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
