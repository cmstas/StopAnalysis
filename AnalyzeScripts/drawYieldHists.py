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

    cor_labels = [
        'I:[250,350]',
        'I:[350,450]',
        'I:[450,550]',
        # 'I:[550,750]',
        # 'I:[750,+#infty]',
        'I:[550,+#infty]',
    ]

    if len(srNames) == 17:
        return std_labels+cor_labels
    elif len(srNames) == 16:
        return std_labels
    elif len(srNames) == 1 and srNames[0][-1] == 'I':
        return cor_labels

    return std_labels+cor_labels

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

def drawSRyieldStack(hstk, xlabels, legitems=None, savename='sigYieldHist.pdf', drawops='hist', linear=False, hline=None, hdata=None, hsig=None):

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

    if type(legitems) == list:
        leg = r.TLegend(0.58, 0.78, 0.78, 0.89)
        if width < 1000:
            # leg = r.TLegend(0.65, 0.64, 0.91, 0.89)
            leg = r.TLegend(0.58, 0.78, 0.89, 0.89)
        if len(legitems) > 2: 
            leg.SetNColumns(2)
        for i in range(len(legitems)):
            leg.AddEntry(hstk.GetHists().At(i), legitems[i])
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

    # f_bkg = r.TFile('../StopLooper/output/combRun2_v37_s5/allBkg_run2.root')
    f_bkg = r.TFile('../StopLooper/output/combRun2_v37_c6/allBkg_run2.root')

    y_all  = [ y.round(2) for y in sum(getYieldEInTopoBins(f_bkg, srNames, 'metbins'), []) ]

    y_2lep = [ y.round(2) for y in sum(getYieldEInTopoBins(f_bkg, srNames, 'metbins_2lep'), []) ]
    y_1lt  = [ y.round(2) for y in sum(getYieldEInTopoBins(f_bkg, srNames, 'metbins_1lepTop'), []) ]
    y_1lW  = [ y.round(2) for y in sum(getYieldEInTopoBins(f_bkg, srNames, 'metbins_1lepW'), []) ]
    y_Zinv = [ y.round(2) for y in sum(getYieldEInTopoBins(f_bkg, srNames, 'metbins_Znunu'), []) ]

    h_2lep = getSRHistFromYieldE(y_2lep, 'h_SRyields_y_2lep', '', fcolor=r.kAzure+7)
    h_1lW  = getSRHistFromYieldE(y_1lW , 'h_SRyields_y_1lW' , '', fcolor=r.kOrange-2)
    h_1lt  = getSRHistFromYieldE(y_1lt , 'h_SRyields_y_1lt' , '', fcolor=r.kRed-7)
    h_Zinv = getSRHistFromYieldE(y_Zinv, 'h_SRyields_y_Zinv', '', fcolor=r.kMagenta-3)

    legname = ['1lepTop', 'Zinv', '1lepW', '2lep']
    hstk = r.THStack('hs1', '')
    hstk.Add(h_1lt)
    hstk.Add(h_Zinv)
    if 'cr0b' in srNames[0]:
        legname = ['1lepTop', 'Zinv', '2lep', '1lepW']
        hstk.Add(h_2lep)
        hstk.Add(h_1lW)
    else:
        hstk.Add(h_1lW)
        hstk.Add(h_2lep)

    xlabels = getLabelsTemporary(srNames)

    drawSRyieldStack(hstk, xlabels, legname, savename, 'hist')

    f_bkg.Close()


def drawCRYieldHists(srNames=None, savename='CR2l_std.pdf', showData=False):

    # -------------------------------------------------------
    # Draw test bkg composition / expected yields hists
    if srNames == None:
        srNames = ['srA0', 'srA1', 'srA2', 'srB', 'srC','srD', 'srE0', 'srE1', 'srE2', 'srE3', 'srF', 'srG0', 'srG1', 'srG2', 'srG3', 'srH',]

    srNames = [ sr.replace('sr', 'cr2l') for sr in srNames ]

    f_bkg = r.TFile('../StopLooper/output/combRun2_v37_s5/allBkg_run2.root')

    y_all  = [ y.round(2) for y in sum(getYieldEInTopoBins(f_bkg, srNames, 'metbins'), []) ]

    y_2lep = [ y.round(2) for y in sum(getYieldEInTopoBins(f_bkg, srNames, 'metbins_2lep'), []) ]
    y_1lt  = [ y.round(2) for y in sum(getYieldEInTopoBins(f_bkg, srNames, 'metbins_1lepTop'), []) ]
    y_1lW  = [ y.round(2) for y in sum(getYieldEInTopoBins(f_bkg, srNames, 'metbins_1lepW'), []) ]
    y_Zinv = [ y.round(2) for y in sum(getYieldEInTopoBins(f_bkg, srNames, 'metbins_Znunu'), []) ]

    h_2lep = getSRHistFromYieldE(y_2lep, 'h_SRyields_y_2lep', '', fcolor=r.kAzure+7)
    h_1lW  = getSRHistFromYieldE(y_1lW , 'h_SRyields_y_1lW' , '', fcolor=r.kOrange-2)
    h_1lt  = getSRHistFromYieldE(y_1lt , 'h_SRyields_y_1lt' , '', fcolor=r.kRed-7)
    h_Zinv = getSRHistFromYieldE(y_Zinv, 'h_SRyields_y_Zinv', '', fcolor=r.kMagenta-3)

    y_dat = None
    if showData:
        f_data = r.TFile('../StopLooper/output/combRun2_v37_s5/allData_run2.root')
        y_data = [ y.round(2) for y in sum(getYieldEInTopoBins(f_dat, srNames, 'metbins'), []) ]
        h_data = getSRHistFromYieldE(y_data, 'h_SRyields_y_2lep', '', fcolor=r.kAzure+7)

    legname = ['1lepTop', 'Zinv', '1lepW', '2lep']
    hstk = r.THStack('hs1', '')
    hstk.Add(h_1lt)
    hstk.Add(h_Zinv)
    if 'cr0b' in srNames[0]:
        legname = ['1lepTop', 'Zinv', '2lep', '1lepW']
        hstk.Add(h_2lep)
        hstk.Add(h_1lW)
    else:
        hstk.Add(h_1lW)
        hstk.Add(h_2lep)

    xlabels = getLabelsTemporary(srNames)

    drawSRyieldStack(hstk, xlabels, legname, savename, 'hist')

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

    drawSRyieldHist(hrat_bkg,  xlabels, 'noleg', 'L1prefire_allbkg_17.pdf', 'PE', True, [0.6, 1.1], 1)
    drawSRyieldHist(hrat_sig1, xlabels, 'noleg', 'L1prefire_T2tt_1200_50_17.pdf', 'PE', True, [0.6, 1.1], 1)
    drawSRyieldHist(hrat_sig2, xlabels, 'noleg', 'L1prefire_T2tt_800_400_17.pdf', 'PE', True, [0.6, 1.1], 1)

    # y_bkg_w = [ y.round(3) for y in sum(getYieldEInTopoBins(f_bkg_w, srNames, 'metbins'), []) ]
    # y_bkg_n = [ y.round(3) for y in sum(getYieldEInTopoBins(f_bkg_n, srNames, 'metbins'), []) ]



if __name__ == '__main__':

    r.gROOT.SetBatch(1)

    # bvsuf = 'v37_s4'

    # f18 = r.TFile('../StopLooper/output/samp18_'+bvsuf+'/lostlepton_18.root')
    # f17 = r.TFile('../StopLooper/output/samp17_'+bvsuf+'/lostlepton_17.root')
    # f16 = r.TFile('../StopLooper/output/samp16_'+bvsuf+'/lostlepton_16.root')
    # frun2 = r.TFile('../StopLooper/output/combRun2_'+bvsuf+'/lostlepton_run2.root')

    srNames = ['srA0', 'srA1', 'srA2', 'srB', 'srC','srD', 'srE0', 'srE1', 'srE2', 'srE3', 'srF', 'srG0', 'srG1', 'srG2', 'srG3', 'srH', 'srI',]
    # srNames = ['srI',]

    # drawBkgEstimateHists(f18, srNames, '18')
    # drawBkgEstimateHists(f17, srNames, '17')
    # drawBkgEstimateHists(f16, srNames, '16')
    # drawBkgEstimateHists(frun2, srNames, 'run2')

    # drawBkgCompositionStack(srNames, 'bkgCompostion_all.pdf')

    # drawBkgCompositionStack(['srI',], 'bkgCompostion_cor6.pdf')
    # drawBkgCompositionStack(['cr2lI'], 'CR2lCompostion_cor6.pdf')
    # drawBkgCompositionStack(['cr0bI'], 'CR0bCompostion_cor6.pdf')

    drawL1prefireEffect()


