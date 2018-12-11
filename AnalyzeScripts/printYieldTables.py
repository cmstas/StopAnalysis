#! /usr/bin/python
# coding: utf-8

import os
import sys
from math import sqrt
import ROOT as r
import numpy as np
from utilities.errors import *
from utilities.pytable import Table

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

def StoBErr(s, b, se, be):
    v = (s+2*b)**2 * se**2 + s**2 * be**2
    v /= 4*(s+b)**3
    return sqrt(v)

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
    tab.add_column(crname+'MC SR', [y.round(2) for y in sum(yMC_SR,[])])
    tab.add_column(crname+'MC CR', [y.round(2) for y in sum(yMC_CR,[])])
    tab.add_column(crname+'Purity', [y.round(2).val for y in sum(purity,[])])
    tab.add_column(crname+'TF_{CR}^{SR}', [y.round(2).val for y in sum(alpha,[])])
    tab.add_column(crname+'data CR', [y.round(2) for y in sum(yld_CR,[])])
    tab.add_column(crname+'CR data/MC', [(d/m).round(2) for d, m in zip(sum(yld_CR,[]), sum(yMC_CR,[]))])
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


if __name__ == '__main__':

    # os.system('mkdir -p plots')

    r.gROOT.SetBatch(1)


    # printCutflowNumbers(f1, 'testCutflow', 'cutflow1')

    f1 = r.TFile('../StopLooper/output/samp17_v13_tf/lostlepton_noMETextrp.root')
    f2 = r.TFile('../StopLooper/output/samp17_v13_tf/lostlepton_wMETextrp.root')

    srNames = ['srA0', 'srA1', 'srA2', 'srB0', 'srC0','srD0', 'srE0', 'srE1', 'srE2', 'srE3', 'srF0', 'srG0', 'srG1', 'srG2', 'srG3', 'srH0',]

    print '\n ----------------------------------- lost lepton w/o MET extrapolation ---------------------------------------'
    tab1 = printTableDataDriven(f1, srNames)
    tab1.set_theme_latex()
    tab1.print_pdf('lostlep_noMETextrp.pdf')

    print '\n ----------------------------------- lost lepton w/ MET extrapolation ----------------------------------------'
    tab2 = printTableDataDriven(f2, srNames)
    tab2.set_theme_latex()
    tab2.print_pdf('lostlep_wMETextrp.pdf')

    print '\n ----------------------------------- lost lepton w/ MET extrapolation ----------------------------------------'
    tab3 = printTableWithMETextrpInfo(f1, f2, srNames)
    tab3.set_theme_latex()
    tab3.print_pdf('lostlep_METextrpInfo.pdf')
