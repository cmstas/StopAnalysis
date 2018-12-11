#! /usr/bin/python
# coding: utf-8

import os
import sys
from math import sqrt
import ROOT as r
import numpy as np
from errors import *
from pytable import Table

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

def getYieldsForAllSRs(f, srNames, hname='metbins', suf='', digit=2):
    ylds = []
    fullhname = sr+'/h_'+hname+suf
    for sr in srNames:
        hist = f.Get(fullhname)
        if not hist:
            print "Can't find", fullhname, "!!!"
            continue
        for ibin in range(1, hist.GetNbinsX()+1):
            ylds.append(round(hist.GetBinContent(ibin), digit))

    return ylds

def getYErrsForAllSRs(f, srNames, hname='metbins', suf='', digit=2):
    yerrs = []
    fullhname = sr+'/h_'+hname+suf
    for sr in srNames:
        hist = f.Get(fullhname)
        if not hist:
            print "Can't find", fullhname, "!!!"; continue
        for ibin in range(1, hist.GetNbinsX()+1):
            yerrs.append(round(hist.GetBinError(ibin), digit))
    return yerrs

def printLine(ylds, label='yields', statstyle='{:^2.2f}'):
    print '| '+label,
    for i in range(len(ylds)):
        print '| '+statstyle.format(ylds[i]),
    print ' |\n'

def printTable(lines, labels, colname, statstyle='{:^2.2f}'):
    print ' | {:<8s}'.format(''),
    for sr in colname:
        print '| {:^4s}'.format(sr),
    print ' |\n'
    for line, label in zip(lines, labels):
        printLine(line, label, statsttyle)

def printColumns(cols, errcols, labels=None, statstyle='{:^2.2f}'):
    if len(cols) != len(errcols):
        print "Number of columns and errors doesn't match!"; return
    if lables:
        if len(labels) != len(cols):
            print "Number of columns and labels doesn't match!"
        else:
            for lab in labels: print '| '+lab,
            print ' |\n'
    for j in range(len(cols[0])):
        for i in range(len(cols)):
            if not errcols[i] or len(errcols) == 0:
                print '| '+statstyle.format(cols[i][j]),
            else:
                print '| '+statstyle.format(cols[i][j])+'\pm'+statstyle.format(cols[i][j]),
        print ' |\n'

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

    tab.add_column('SR name', srNames)
    tab.add_column(crname+'MC SR', [y.round(2) for y in sum(yMC_SR,[])])
    tab.add_column(crname+'MC CR', [y.round(2) for y in sum(yMC_CR,[])])
    tab.add_column(crname+'Purity', [y.round(2).val for y in sum(purity,[])])
    tab.add_column(crname+'TF_{CR}^{SR}', [y.round(2).val for y in sum(alpha,[])])
    tab.add_column(crname+'data CR', [y.round(2) for y in sum(yld_CR,[])])
    tab.add_column(crname+'CR data/MC', [(d/m).round(2) for d, m in zip(sum(yld_CR,[]), sum(yMC_CR,[]))])
    tab.add_column(crname+'Prediction', [y.round(2) for y in sum(preds,[])])

    tab.print_table()

    return tab


if __name__ == '__main__':

    # os.system('mkdir -p plots')

    r.gROOT.SetBatch(1)


    # printCutflowNumbers(f1, 'testCutflow', 'cutflow1')

    f2 = r.TFile('../StopLooper/output/samp17_v13_tf/lostlepton.root')

    srNames = ['srA0', 'srA1', 'srA2', 'srB0', 'srC0','srD0', 'srE0', 'srE1', 'srE2', 'srE3', 'srF0', 'srG0', 'srG1', 'srG2', 'srG3', 'srH0',]

    print '\n |----------------------------------- lost lepton ----------------------------------------'

    printTableDataDriven(f2, srNames)
