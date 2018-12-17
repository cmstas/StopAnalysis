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
                cut_upper = str(cut_upper) if cut_upper < 1200 else '+Inf'
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

def getYieldEInTopoBins(f, srNames, hname='metbins'):
    yields = []
    for sr in srNames:
        hist = f.Get(sr+'/h_'+hname)
        if not hist:
            print "Can't find", sr+'/h_'+hname, "!!!";
            hist = f.Get(sr+'/h_metbins')
            if not hist: print "This should not happen!"
            yields.append([E(0,0)]*hist.GetNbinsX())
            continue
        topoylds = []
        for ibin in range(1, hist.GetNbinsX()+1):
            topoylds.append(E(hist.GetBinContent(ibin), hist.GetBinError(ibin)))
        yields.append(topoylds)

    return yields


# # # # # # # # # # # # # # # # # # # # # # # #
# Additional statistic tools
# # # # # # # # # # # # # # # # # # # # # # # #

def StoBErr(s, b, se, be):
    v = (s+2*b)**2 * se**2 + s**2 * be**2
    v /= 4*(s+b)**3
    return sqrt(v)


# # # # # # # # # # # # # # # # # # # # # # # #
# Table makers for Data Driven estimates
# # # # # # # # # # # # # # # # # # # # # # # #

def printTableDataDriven(f, srNames, crname=''):
    if crname != '' and crname[-1] != ' ': crname += ' '

    tab = Table()
    preds  = getYieldEInTopoBins(f, srNames, 'metbins')
    alpha  = getYieldEInTopoBins(f, srNames, 'alphaHist')
    purity = getYieldEInTopoBins(f, srNames, 'CRpurity')
    yMC_SR = getYieldEInTopoBins(f, srNames, 'MCyields_SR')
    yMC_CR = getYieldEInTopoBins(f, srNames, 'MCyields_CR')
    yld_CR = getYieldEInTopoBins(f, srNames, 'datayields_CR')

    metrange = getBinningFromTopoSRs(f, srNames)
    tab.add_column('SR name', sum([[sr]*n for sr, n in zip(srNames, map(len, preds))], []))
    tab.add_column('MET [GeV]', [m[0]+' -- '+m[1] for m in sum(metrange, [])])
    tab.add_column(crname+'MC SR', [y.round(2) for y in sum(yMC_SR,[])])
    tab.add_column(crname+'MC CR', [y.round(2) for y in sum(yMC_CR,[])])
    tab.add_column(crname+'Purity', [y.round(2) for y in sum(purity,[])])
    tab.add_column(crname+'TF_{CR}^{SR}', [y.round(2).val for y in sum(alpha,[])])
    tab.add_column(crname+'data CR', [y.round(2) for y in sum(yld_CR,[])])
    tab.add_column(crname+'R_{CR}(data/MC)', [(d/m).round(2) for d, m in zip(sum(yld_CR,[]), sum(yMC_CR,[]))])
    tab.add_column(crname+'Prediction', [y.round(2) for y in sum(preds,[])])

    tab.print_table()

    return tab

def printTableWithMETextrpInfo(f1, f2, srNames, crname=''):
    if crname != '' and crname[-1] != ' ': crname += ' '

    tab = Table()
    preds1 = getYieldEInTopoBins(f1, srNames, 'metbins')
    alpha1 = getYieldEInTopoBins(f1, srNames, 'alphaHist')
    purity = getYieldEInTopoBins(f1, srNames, 'CRpurity')
    yMC_SR = getYieldEInTopoBins(f1, srNames, 'MCyields_SR')
    yMC_CR = getYieldEInTopoBins(f1, srNames, 'MCyields_CR')
    yld_CR = getYieldEInTopoBins(f1, srNames, 'datayields_CR')

    yMCaCR = getYieldEInTopoBins(f2, srNames, 'MCyields_CR')
    yldaCR = getYieldEInTopoBins(f2, srNames, 'datayields_CR')
    preds2 = getYieldEInTopoBins(f2, srNames, 'metbins')
    alpha2 = getYieldEInTopoBins(f2, srNames, 'alphaHist')

    metrange = getBinningFromTopoSRs(f1, srNames)

    tab.add_column('SR name', sum([[sr]*n for sr, n in zip(srNames, map(len, preds1))], []))
    tab.add_column('MET range [GeV]', [m[0]+' -- '+m[1] for m in sum(metrange, [])])
    tab.add_column(crname+'MC SR', [y.round(2) for y in sum(yMC_SR,[])])
    tab.add_column(crname+'MC CR raw', [y.round(2) for y in sum(yMC_CR,[])])
    tab.add_column(crname+'Purity', [y.round(2).val for y in sum(purity,[])])
    tab.add_column(crname+'TF_{CR}^{SR}', [y.round(2) for y in sum(alpha1,[])])
    tab.add_column(crname+'data CR raw', [y.round(2) for y in sum(yld_CR,[])])
    tab.add_column(crname+'CR data/MC raw', [(d/m).round(2) for d, m in zip(sum(yld_CR,[]), sum(yMC_CR,[]))])
    tab.add_column(crname+'Pred (no extrp)', [y.round(2) for y in sum(preds1,[])])

    tab.add_column(crname+'MC CR comb', [y.round(2) for y in sum(yMCaCR,[])])
    tab.add_column(crname+'data CR comb', [y.round(2) for y in sum(yldaCR,[])])
    tab.add_column(crname+'CR data/MC comb', [(d/m).round(2) for d, m in zip(sum(yldaCR,[]), sum(yMCaCR,[]))])
    tab.add_column(crname+'Pred (MET extrp)', [y.round(2) for y in sum(preds2,[])])

    tab.print_table()

    return tab


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
