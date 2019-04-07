#! /usr/bin/python
# coding: utf-8

import os
import sys
from math import sqrt
import ROOT as r
import numpy as np
from errors import *
from pytable import Table

# # # # # # # # # # # # # # # # # # # # # # # #
# Yields extraction and handling
# # # # # # # # # # # # # # # # # # # # # # # #

def getBinningFromTopoSRs(f, srNames, hname='metbins'):
    allbins = []
    for sr in srNames:
        hist = f.Get(sr+'/h_'+hname)
        bins = []
        if not hist:
            print "Can't find", sr+'/h_'+hname, "!!!";
            allbins.append([0])
        else:
            for ibin in range(1, hist.GetNbinsX()+1):
                cut_lower = str(int(hist.GetBinLowEdge(ibin)))
                cut_upper = int(hist.GetBinLowEdge(ibin+1))
                cut_upper = str(cut_upper) if cut_upper < 1200 else 'Inf'
                bins.append([cut_lower, cut_upper])
            allbins.append(bins)

    return allbins

def getYieldsFromTopoSRs(f, srNames, hname='metbins', suf=''):
    yields = []
    for sr in srNames:
        hist = f.Get(sr+'/h_'+hname+suf)
        if not hist:
            yields.append(0)
        else:
            yields.append(float(hist.Integral()))
    return yields

def getYieldAndErrsFromTopoSRs(f, srNames, hname='metbins', suf=''):
    yields = []
    yerrs = []
    for sr in srNames:
        hist = f.Get(sr+'/h_'+hname+suf)
        if not hist:
            yields.append(0)
            yerrs.append(0)
        else:
            yerr = r.Double()
            yields.append(float(hist.IntegralAndError(0,-1,yerr)))
            yerrs.append(float(yerr))

    return yields, yerrs

def getYieldEInTopoBins(f, srNames, hname='metbins', backup_hname=None):
    yields = []
    for sr in srNames:
        hist = f.Get(sr+'/h_'+hname)
        if not hist and backup_hname:
            hist = f.Get(sr+'/h_'+backup_hname)
        if not hist:
            print "Can't find", sr+'/h_'+hname, "from", f.GetName(), "!! Returning 0s";
            hist = f.Get(sr+'/h_metbins')
            if not hist: print "This should not happen!"
            yields.append([E(0,0)]*hist.GetNbinsX())
            continue
        topoylds = []
        for ibin in range(1, hist.GetNbinsX()+1):
            topoylds.append(E(hist.GetBinContent(ibin), hist.GetBinError(ibin)))
        yields.append(topoylds)

    return yields

def getYieldEInFlatBins(f, srNames, hname='metbins', backup_hname=None):
    return  sum(getYieldEInTopoBins(f, srNames, hname, backup_hname),[])


# # # # # # # # # # # # # # # # # # # # # # # #
# Additional statistic tools
# # # # # # # # # # # # # # # # # # # # # # # #

def StoBErr(s, b, se, be):
    v = (s+2*b)**2 * se**2 + s**2 * be**2
    v /= 4*(s+b)**3
    return sqrt(v)


def getPoissonUpDn(y):
    alpha = 1-0.6827
    yup = r.Math.gamma_quantile_c(alpha/2, y+1, 1)
    ydn = 0 if y==0 else r.Math.gamma_quantile(alpha/2, y, 1)
    return yup, ydn

def getPoissonErrors(y):
    yup, ydn = getPoissonUpDn(y)
    return yup-y, y-ydn

# # # # # # # # # # # # # # # # # # # # # # # #
# SR yield hist makers
# # # # # # # # # # # # # # # # # # # # # # # #

def drawSRyieldHist(hist, hname, xlabels, savename='sigYieldHist.pdf'):

    c1 = r.TCanvas("c1", "c1", 1500, 800) # todo: adjustable canvas width later

    r.gStyle.SetOptStat("")
    r.gStyle.SetPadGridX(0)
    r.gStyle.SetPadGridY(0)
    r.gStyle.SetPadTickX(1)
    r.gStyle.SetPadTickY(1)
    r.gStyle.SetFrameBorderMode(0)

    mainPad = r.TPad("p1", "p1", 0.0, 0.05, 1.0, 1.0)
    ratioPad = r.TPad("p2", "p2", 0.0, 0.0, 1.0, 0.23)
    mainPad.SetTopMargin(0.08)
    mainPad.SetLeftMargin(0.10)
    mainPad.SetRightMargin(0.05)
    mainPad.SetBottomMargin(0.22)
    mainPad.SetLogy()
    mainPad.Draw()
    ratioPad.SetTopMargin(0.05)
    ratioPad.SetLeftMargin(0.10)
    ratioPad.SetRightMargin(0.05)

    mainPad.cd()

    hist.GetXaxis().SetLabelSize(0.048)
    histGetXaxis().LabelsOption("v")
    for i, lab in enumerate(xlabels):
        hist.GetXaxis().SetBinLabel(i+1, xlabels)
    hist.GetYaxis().SetTitle("N Events")
    hist.GetYaxis().SetTitleOffset(0.5)
    hist.GetYaxis().SetTitleSize(0.06)
    hist.GetYaxis().SetRangeUser(0.1, 4000)
    hist.Draw("hist")

    leg = r.TLegend(0.42, 0.74, 0.68, 0.89)
    leg.AddEntry(hist, hname)
    leg.Draw()

    c1.SaveAs(savename)
