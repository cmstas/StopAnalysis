#! /usr/bin/python
# coding: utf-8

import os
import sys
from math import sqrt
import ROOT as r
import numpy as np
from utilities.errors import *
from utilities.yields_utils import *


def getLabelsTemporary(srNames):

    if len(srNames) > 8:
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
    elif len(srNames) == 1 and srNames[0] == 'srI':
        xlabels = [
            'I:[250,350]',
            'I:[350,450]',
            'I:[450,550]',
            'I:[550,700]',
            'I:[700,+#infty]',
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

    width = max(len(xlabels)*40, 400)
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

    width = max(len(xlabels)*40, 400)
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
    htot.GetYaxis().SetRangeUser(0.1, 2000)
    # htot.GetYaxis().SetRangeUser(0.0001, 10)

    htot.Draw(drawops)
    hstk.Draw('histsame')
    htot.Draw('axissame');

    if type(hleg) == list:
        leg = r.TLegend(0.58, 0.78, 0.78, 0.89)
        if width < 800:
            leg = r.TLegend(0.65, 0.64, 0.91, 0.89)
            # leg = r.TLegend(0.35, 0.80, 0.91, 0.89)
        elif len(hleg) > 2: 
            leg.SetNColumns(2)
        for i in range(len(hleg)):
            leg.AddEntry(hstk.GetHists().At(i), hleg[i])
        leg.Draw()

    c0.SaveAs(savename)
    c0.Clear()


def drawBkgEstimateHists(f1, srNames, suf=''):

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
    drawSRyieldHist(h_alpha, xlabels, 'Transfer Factors', 'lostlep'+suf+'_alphas_log.pdf', 'PE')
    drawSRyieldHist(h_alpha, xlabels, 'Transfer Factors', 'lostlep'+suf+'_alphas_lin.pdf', 'PE', True, [0, 2.5])

    h_ratio = getSRHistFromYieldE(rat_MC, 'hrat_dataMC_CR', '', lcolor=r.kGray+3)
    drawSRyieldHist(h_ratio, xlabels, 'noleg', 'lostlep'+suf+'_CRratio.pdf', 'PE', True, [0, 2], 1)

    h_purity = getSRHistFromYieldE(purity, 'h_CRpurity', '', lcolor=r.kGray+2)
    drawSRyieldHist(h_purity, xlabels, 'noleg', 'lostlep'+suf+'_CRpurity.pdf', 'PE', True, [0, 1.2], 1)


def drawBkgCompositionStack(srNames=None, savename='bkgCompostion_std.pdf'):

    # -------------------------------------------------------
    # Draw test bkg composition / expected yields hists
    if srNames == None:
        srNames = ['srA0', 'srA1', 'srA2', 'srB', 'srC','srD', 'srE0', 'srE1', 'srE2', 'srE3', 'srF', 'srG0', 'srG1', 'srG2', 'srG3', 'srH',]

    f_bkg = r.TFile('../StopLooper/output/combRun2_v37_s4/allBkg_run2.root')

    y_all  = [ y.round(2) for y in sum(getYieldEInTopoBins(f_bkg, srNames, 'metbins'), []) ]

    y_2lep = [ y.round(2) for y in sum(getYieldEInTopoBins(f_bkg, srNames, 'metbins_2lep'), []) ]
    y_1lt  = [ y.round(2) for y in sum(getYieldEInTopoBins(f_bkg, srNames, 'metbins_1lepTop'), []) ]
    y_1lW  = [ y.round(2) for y in sum(getYieldEInTopoBins(f_bkg, srNames, 'metbins_1lepW'), []) ]
    y_Zinv = [ y.round(2) for y in sum(getYieldEInTopoBins(f_bkg, srNames, 'metbins_Znunu'), []) ]

    h_2lep = getSRHistFromYieldE(y_2lep, 'h_SRyields_y_2lep', '', fcolor=r.kAzure+7)
    h_1lW  = getSRHistFromYieldE(y_1lW , 'h_SRyields_y_1lW' , '', fcolor=r.kOrange-2)
    h_1lt  = getSRHistFromYieldE(y_1lt , 'h_SRyields_y_1lt' , '', fcolor=r.kRed-7)
    h_Zinv = getSRHistFromYieldE(y_Zinv, 'h_SRyields_y_Zinv', '', fcolor=r.kMagenta-3)

    hstk = r.THStack('hs1', '')
    hstk.Add(h_1lt)
    hstk.Add(h_Zinv)
    hstk.Add(h_1lW)
    hstk.Add(h_2lep)

    xlabels = getLabelsTemporary(srNames)

    drawSRyieldStack(hstk, xlabels, ['1lepTop', 'Zinv', '1lepW', '2lep'], savename, 'hist')

    f_bkg.Close()


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


if __name__ == '__main__':

    r.gROOT.SetBatch(1)

    # bvsuf = 'v37_s4'

    # f18 = r.TFile('../StopLooper/output/samp18_'+bvsuf+'/lostlepton_18.root')
    # f17 = r.TFile('../StopLooper/output/samp17_'+bvsuf+'/lostlepton_17.root')
    # f16 = r.TFile('../StopLooper/output/samp16_'+bvsuf+'/lostlepton_16.root')
    # frun2 = r.TFile('../StopLooper/output/combRun2_'+bvsuf+'/lostlepton_run2.root')

    srNames = ['srA0', 'srA1', 'srA2', 'srB', 'srC','srD', 'srE0', 'srE1', 'srE2', 'srE3', 'srF', 'srG0', 'srG1', 'srG2', 'srG3', 'srH',]
    # srNames = ['srI',]

    # drawBkgEstimateHists(f18, srNames, '18')
    # drawBkgEstimateHists(f17, srNames, '17')
    # drawBkgEstimateHists(f16, srNames, '16')
    # drawBkgEstimateHists(frun2, srNames, 'run2')

    drawBkgCompositionStack(srNames)

    srNames = ['srI',]
    drawBkgCompositionStack(srNames, 'bkgCompostion_cor.pdf')
