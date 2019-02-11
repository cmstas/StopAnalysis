#! /usr/bin/python
# coding: utf-8

import os
import sys
from math import sqrt
import ROOT as r
import numpy as np
from utilities.errors import *
from utilities.yields_utils import *


def getLabelsTemporary():

    xlabels = [
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

    return xlabels

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

def drawSRyieldHist(hist, xlabels, hleg=None, savename='sigYieldHist.pdf', drawops='hist', linear=False, yrange=None, hline=None):
    if yrange is None: yrange = [0.001, 100]

    c0 = r.TCanvas('c0', 'c0', 1500, 800) # todo: adjustable canvas width later

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

    hist.GetYaxis().SetTitle('N Events')
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

def drawSRyieldStack(hstk, xlabels, hleg=None, savename='sigYieldHist.pdf', drawops='hist', linear=False, hline=None):

    c0 = r.TCanvas('c0', 'c0', 1500, 800) # todo: adjustable canvas width later

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

    if hstk.GetHists().GetSize() < 1:
        print 'Invalid hist stack passed in!!'; return False

    htot = hstk.GetHists().At(0).Clone('h_tot')

    for i in range(1, hstk.GetHists().GetSize()):
        htot.Add(hstk.GetHists().At(i))

    htot.GetXaxis().SetLabelSize(0.048)
    for i, lab in enumerate(xlabels):
        htot.GetXaxis().SetBinLabel(i+1, lab)

    htot.GetXaxis().LabelsOption('v')
    htot.GetYaxis().SetTitle('N Events')
    htot.GetYaxis().SetTitleOffset(0.5)
    htot.GetYaxis().SetTitleSize(0.06)
    # htot.GetYaxis().SetRangeUser(0.0001, 10)

    htot.Draw(drawops)
    hstk.Draw('histsame')

    if type(hleg) == list:
        leg = r.TLegend(0.42, 0.80, 0.68, 0.89)
        for i in range(len(hleg)):
            leg.AddEntry(hstk.GetHists().At(i), hleg[i])
        leg.Draw()

    c0.SaveAs(savename)


def drawSRyieldHistBlind(f1, srNames, suf=''):

    preds  = [ y.round(2) for y in sum(getYieldEInTopoBins(f1, srNames, 'metbins'), []) ]
    alpha  = [ y.round(2) for y in sum(getYieldEInTopoBins(f1, srNames, 'alphaHist'), []) ]
    purity = [ y.round(2) for y in sum(getYieldEInTopoBins(f1, srNames, 'CRpurity'), []) ]
    yMC_SR = sum(getYieldEInTopoBins(f1, srNames, 'MCyields_SR'), [])
    yMC_CR = sum(getYieldEInTopoBins(f1, srNames, 'MCyields_CR'), [])
    yld_CR = sum(getYieldEInTopoBins(f1, srNames, 'datayields_CR'), [])

    rat_MC = [ (d/m).round(2) for d, m in zip(yld_CR, yMC_CR) ]

    metrange = getBinningFromTopoSRs(f1, srNames)
    # xlabels = ["{}:[{},{}]".[sr, bins] for sr, bins in zip(srNames, metrange)]
    xlabels = getLabelsTemporary()

    h_alpha = getSRHistFromYieldE(alpha, 'h_alpha', '', lcolor=r.kRed-7)
    drawSRyieldHist(h_alpha, xlabels, 'Transfer Factors', 'lostlep'+suf+'_alphas_log.pdf', 'PE')
    drawSRyieldHist(h_alpha, xlabels, 'Transfer Factors', 'lostlep'+suf+'_alphas_lin.pdf', 'PE', True, [0, 2.5])

    h_ratio = getSRHistFromYieldE(rat_MC, 'hrat_dataMC_CR', '', lcolor=r.kGray+3)
    drawSRyieldHist(h_ratio, xlabels, 'noleg', 'lostlep'+suf+'_CRratio.pdf', 'PE', True, [0, 2], 1)

    h_purity = getSRHistFromYieldE(purity, 'h_CRpurity', '', lcolor=r.kGray+2)
    drawSRyieldHist(h_purity, xlabels, 'noleg', 'lostlep'+suf+'_CRpurity.pdf', 'PE', True, [0, 1.2], 1)


def drawBkgCompositionStack(srNames):

    # -------------------------------------------------------
    # Draw test bkg composition hists

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


if __name__ == '__main__':

    r.gROOT.SetBatch(1)

    bvsuf = 'v30_s1'

    f18 = r.TFile('../StopLooper/output/samp18_'+bvsuf+'/lostlepton.root')
    f17 = r.TFile('../StopLooper/output/samp17_'+bvsuf+'/lostlepton.root')
    f16 = r.TFile('../StopLooper/output/samp16_'+bvsuf+'/lostlepton.root')
    frun2 = r.TFile('../StopLooper/output/combRun2_'+bvsuf+'/lostlepton.root')

    srNames = ['srA0', 'srA1', 'srA2', 'srB0', 'srC0','srD0', 'srE0', 'srE1', 'srE2', 'srE3', 'srF0', 'srG0', 'srG1', 'srG2', 'srG3', 'srH0',]
    # srNames = ['srI',]

    # drawSRyieldHistBlind(f18, srNames, '18')
    # drawSRyieldHistBlind(f17, srNames, '17')
    # drawSRyieldHistBlind(f16, srNames, '16')
    drawSRyieldHistBlind(frun2, srNames, 'run2')

    # srNames = ['srI',]
    # drawBkgCompositionStack(srNames)
