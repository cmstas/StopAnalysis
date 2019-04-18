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


def getSystYieldAEInFlatBins(f, srNames, sys):
    pass
    preds = getYieldEInFlatBins(f, srNames, 'metbins')
    predup = getYieldEInFlatBins(f, srNames, 'metbins_'+sys+'Up', 'metbins')
    preddn = getYieldEInFlatBins(f, srNames, 'metbins_'+sys+'Dn', 'metbins')
    sysup = [ 0 if ycen.val <= 0 else round(yup.val/ycen.val-1, 4) for yup, ycen in zip(predup, preds)]
    sysdn = [ 0 if ycen.val <= 0 else round(ydn.val/ycen.val-1, 4) for ydn, ycen in zip(preddn, preds)]
    for i in range(len(preds)):
        sup = sysup[i]
        sdn = sysdn[i]
        if sysup[i] < 0 and sysdn[i] > 0:
            sdn = sysup[i]
            sup = sysdn[i]
        elif not (sysup[i] > 0 and sysdn[i] < 0):
            print 'Syst:', sys, 'at bin', i, 'has single sided variation! sysup =', sysup[i], 'sysdn =', sysdn[i]
            if np.isnan(sup): sup = 0
            if np.isnan(sdn): sdn = 0
            sup = max(abs(sup), abs(sdn))
            sdn = sup




def getAESystDataDriven(f, srNames, systs=None, doAsymErr=True):
    preds  = [ AE(y.val, y.err) for y in getYieldEInFlatBins(f, srNames, 'metbins')]
    alpha  = [ AE(y.val, y.err) for y in getYieldEInFlatBins(f, srNames, 'alphaHist')]
    yMC_SR = [ AE(y.val, y.err) for y in getYieldEInFlatBins(f, srNames, 'MCyields_SR')]
    yMC_CR = [ AE(y.val, y.err) for y in getYieldEInFlatBins(f, srNames, 'MCyields_CR')]
    yld_CR = [ AE(y.val, y.err) for y in getYieldEInFlatBins(f, srNames, 'datayields_CR')]
    purity = [ AE(y.val, y.err) for y in getYieldEInTopoBins(f, srNames, 'CRpurity')]

    systup = [a.eup for a in alpha]
    systdn = systup


    if type(systs) == list:
        for sys in systs:
            sysup = [ 0 if ycen.val <= 0 else round(yup.val/ycen.val-1, 4) for yup, ycen in zip(predup, preds)]
            sysdn = [ 0 if ycen.val <= 0 else round(ydn.val/ycen.val-1, 4) for ydn, ycen in zip(preddn, preds)]
            for i in range(len(preds)):
                sup = sysup[i]
                sdn = sysdn[i]
                if sysup[i] < 0 and sysdn[i] > 0:
                    sdn = sysup[i]
                    sup = sysdn[i]
                elif not (sysup[i] > 0 and sysdn[i] < 0):
                    print 'Syst:', sys, 'at bin', i, 'has single sided variation! sysup =', sysup[i], 'sysdn =', sysdn[i]
                    if np.isnan(sup): sup = 0
                    if np.isnan(sdn): sdn = 0
                    sup = max(abs(sup), abs(sdn))
                    sdn = sup

                systup[i] = sqrt(systup[i]**2 + sup**2)
                systdn[i] = sqrt(systdn[i]**2 + sdn**2)


def rond(y, errup=None, errdn=None, digit=2):
    if y > 1000: digit = 0
    if y > 100: digit = 1
    if y > 10: digit = 1
    if type(y) == float:
        y = round(y,digit)

    if errup == None:
        return y
    elif errdn == None:
        return y, round(errup,digit)
    else:
        return y, round(errup,digit), round(errdn,digit)


def getPredStrDataDriven(f, srNames, systs=None, showUpDn=True):
    preds  = getYieldEInFlatBins(f, srNames, 'metbins')
    alpha  = getYieldEInFlatBins(f, srNames, 'alphaHist')
    yMC_SR = getYieldEInFlatBins(f, srNames, 'MCyields_SR')
    yMC_CR = getYieldEInFlatBins(f, srNames, 'MCyields_CR')
    yld_CR = getYieldEInFlatBins(f, srNames, 'datayields_CR')
    purity = getYieldEInTopoBins(f, srNames, 'CRpurity')

    systup = [a.err for a in alpha]
    systdn = systup

    if type(systs) == list:
        for sys in systs:
            predup = getYieldEInFlatBins(f, srNames, 'metbins_'+sys+'Up', 'metbins')
            preddn = getYieldEInFlatBins(f, srNames, 'metbins_'+sys+'Dn', 'metbins')
            sysup = [ 0 if ycen.val <= 0 else round(yup.val/ycen.val-1, 4) for yup, ycen in zip(predup, preds)]
            sysdn = [ 0 if ycen.val <= 0 else round(ydn.val/ycen.val-1, 4) for ydn, ycen in zip(preddn, preds)]
            for i in range(len(preds)):
                sup = sysup[i]
                sdn = sysdn[i]
                if sysup[i] < 0 and sysdn[i] > 0:
                    sdn = sysup[i]
                    sup = sysdn[i]
                elif not (sysup[i] > 0 and sysdn[i] < 0):
                    print 'Syst:', sys, 'at bin', i, 'has single sided variation! sysup =', sysup[i], 'sysdn =', sysdn[i]
                    if np.isnan(sup): sup = 0
                    if np.isnan(sdn): sdn = 0
                    sup = max(abs(sup), abs(sdn))
                    sdn = sup

                systup[i] = sqrt(systup[i]**2 + sup**2)
                systdn[i] = sqrt(systdn[i]**2 + sdn**2)

    pderr = [ getPoissonErrors(y.val) for y in yld_CR]
    preds = [ y.round(2).val for y in preds]
    alpha = [ a.round(2).val for a in alpha]
    # TFs = [ '${0}^{{+{1}}}_{{-{2}}}$'.format(a, round(a*(1+sup),2), round(a*(1-sdn),2)) for a, sup, sdn in zip(alpha, systup, systdn)]
    # y_CR = [ '${0}^{{+{1}}}_{{-{2}}}$'.format(int(y.val), round(e[0],2), round(e[1],2)) for y, e in zip(yld_CR, pderr)])
    # R_CR = [(d/m).round(2) for d, m in zip(yld_CR, yMC_CR)]
    if showUpDn:
        pstr = [ '${0}^{{+{1}+{3}}}_{{-{2}-{4}}}$'.format(y, round(sup*y,2), round(sdn*y,2), round(pde[0]*a,2), round(pde[1]*a,2)) for y,pde,a,sup,sdn in zip(preds, pderr, alpha, systup, systdn)]
    else:
        # pstr = [ '${0}^{{+{1}}}_{{-{2}}}\pm{3}$'.format(y, round(pde[0]*a,2), round(pde[1]*a,2), max(round(sup*y,2), round(sdn*y,2))) for y,pde,a,sup,sdn in zip(preds, pderr, alpha, systup, systdn)]
        pstr = [ '${0}^{{+{1}}}_{{-{2}}}$'.format(*rond(y, sqrt((pde[0]*a)**2+(sup*y)**2), sqrt((pde[1]*a)**2+(sdn*y)**2), 2)) for y,pde,a,sup,sdn in zip(preds, pderr, alpha, systup, systdn)]
    peup = [ round(sqrt((pde[0]*a)**2 +(sup*y)**2),2) for y,pde,a,sup in zip(preds, pderr, alpha, systup)]
    pedn = [ round(sqrt((pde[1]*a)**2 +(sdn*y)**2),2) for y,pde,a,sdn in zip(preds, pderr, alpha, systdn)]

    return pstr, preds, peup, pedn

def getPredStrMCDriven(f, srNames, systs=None):
    preds  = getYieldEInFlatBins(f, srNames, 'metbins')

    systup = [0.0 if y.val <= 0 else round(y.err/y.val,4) for y in preds]
    systdn = systup

    if type(systs) == list:
        for sys in systs:
            predup = getYieldEInFlatBins(f, srNames, 'metbins_'+sys+'Up', 'metbins')
            preddn = getYieldEInFlatBins(f, srNames, 'metbins_'+sys+'Dn', 'metbins')
            sysup = [ 0 if ycen.val <= 0 else round(yup.val/ycen.val-1, 4) for yup, ycen in zip(predup, preds)]
            sysdn = [ 0 if ycen.val <= 0 else round(ydn.val/ycen.val-1, 4) for ydn, ycen in zip(preddn, preds)]
            for i in range(len(preds)):
                sup = sysup[i]
                sdn = sysdn[i]
                if sysup[i] < 0 and sysdn[i] > 0:
                    sdn = sysup[i]
                    sup = sysdn[i]
                elif not (sysup[i] > 0 and sysdn[i] < 0):
                    print 'Syst:', sys, 'at bin', i, 'has single sided variation! sysup =', sysup[i], 'sysdn =', sysdn[i]
                    if np.isnan(sup): sup = 0
                    if np.isnan(sdn): sdn = 0
                    sup = max(abs(sup), abs(sdn))
                    sdn = sup

                systup[i] = sqrt(systup[i]**2 + sup**2)
                systdn[i] = sqrt(systdn[i]**2 + sdn**2)
    elif type(systs) == float:
        systup = [systs for y in preds]
        systdn = systup

    preds = [ y.round(2).val for y in preds]
    # pstr = [ '${0}^{{+{1}}}_{{-{2}}}$'.format(y, round(sup*y,2), round(sdn*y,2)) for y,sup,sdn in zip(preds, systup, systdn)]
    pstr = [ '${0}\pm{1}$'.format(*rond(y, max(sup*y, sdn*y))) for y,sup,sdn in zip(preds, systup, systdn)]
    peup = [ round(sup*y,2) for y,sup in zip(preds, systup)]
    pedn = [ round(sdn*y,2) for y,sdn in zip(preds, systdn)]

    return pstr, preds, peup, pedn

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
