#! /usr/bin/python
# coding: utf-8

import os
import sys
from math import sqrt
import ROOT as r
import numpy as np
from utilities.errors import *
from utilities.yields_utils import *


def GetPoissonRatioGraph(h_mc, h_data, g_ratio, drawZeros=True, useMCErr=True):
    alpha = 1-0.6827
    for i in range(1,h_mc.GetNbinsX()+1):
        x = h_mc.GetBinCenter(i)
        mcy = h_mc.GetBinContent(i)
        datay = h_data.GetBinContent(i)
        if mcy==0 or (datay==0 and not drawZeros):
            continue
        mcerr = h_mc.GetBinError(i)
        if not useMCErr:
            mcerr = 0
        dataerrup = r.Math.gamma_quantile_c(alpha/2, datay+1, 1) - datay
        dataerrdown = 0 if datay==0 else (datay-r.Math.gamma_quantile(alpha/2, datay, 1))

        ratio = datay/mcy
        rerrup = r.TMath.Sqrt((dataerrup/mcy)**2 + (mcerr*datay/mcy**2)**2)
        rerrdown = r.TMath.Sqrt((dataerrdown/mcy)**2 + (mcerr*datay/mcy**2)**2)

        xerr = h_mc.GetBinWidth(i)/2
        thisPoint = g_ratio.GetN()
        g_ratio.SetPoint(thisPoint, x, ratio)
        g_ratio.SetPointError(thisPoint, xerr, xerr, rerrdown, rerrup)

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
        # 'I:[550,+#infty]',
    ]

    if len(srNames) == 17:
        return std_labels+cor_labels
    elif len(srNames) == 16:
        return std_labels
    elif len(srNames) == 12:
        return cr0b_labels+cor_labels
    elif len(srNames) == 11:
        return cr0b_labels
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
                     hdata=None, hsigs=None, ytitle='N Events', noRatio=False, noBkgError=False):

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

    if not hdata: noRatio = True
    if not noRatio:
        c0 = r.TCanvas('c0', 'c0', width, 1200)
        mainPad = r.TPad('1', '1', 0.0, 0.20, 1.0, 1.0)
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
    htot.GetYaxis().SetTitleOffset(0.5)
    htot.GetYaxis().SetTitleSize(0.06)
    htot.GetYaxis().SetRangeUser(0.1, 5000)
    # htot.GetYaxis().SetRangeUser(0.0001, 10)

    htot.Draw(drawops)
    hstk.Draw('histsame')
    htot.Draw('axissame');

    if not noBkgError:
        h_err = htot.Clone('h_err')
        h_err.SetFillStyle(3244)
        h_err.SetFillColor(r.kGray+2)
        h_err.Draw("E2same")

    if hdata != None:
        hdata.SetMarkerStyle(20)
        hdata.SetMarkerSize(1.2)
        hdata.SetLineWidth(2)
        hdata.Draw('PEsame')

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
            leg = r.TLegend(0.52, 0.76, 0.82, 0.89)
            leg.SetNColumns(3)
        for i in range(len(legitems)-1, -1, -1):
            leg.AddEntry(hstk.GetHists().At(i), legitems[i], 'lf')
            if hdata != None and i == len(legitems)-2:
                leg.AddEntry(hdata, 'data', 'lp')

        leg.Draw()

    ratioPad.cd()
    if not noRatio:
        h_axis_ratio = r.TH1D('ratio_axis','', htot.GetNbinsX(), 0, htot.GetNbinsX())
        h_axis_ratio.GetYaxis().SetTitleOffset(0.18)
        h_axis_ratio.GetYaxis().SetTitleSize(0.22)
        h_axis_ratio.GetYaxis().SetNdivisions(4)
        h_axis_ratio.GetYaxis().SetLabelSize(0.15)
        h_axis_ratio.GetYaxis().SetRangeUser(0, 2)
        h_axis_ratio.GetYaxis().SetTitle('Ratios  ')
        h_axis_ratio.GetYaxis().SetTitleOffset(0.5)
        h_axis_ratio.GetXaxis().SetTickLength(0.07)
        h_axis_ratio.GetXaxis().SetTitleSize(0.)
        h_axis_ratio.GetXaxis().SetLabelSize(0.)
        h_axis_ratio.Draw('axis')

        hRatio = hdata.Clone('h_ratio')
        # hRatio.Divide(hdata, htot, 1, 1, 'B')
        hRatio.Divide(htot)
        ratioGraph = r.TGraphAsymmErrors()
        GetPoissonRatioGraph(htot,hdata,ratioGraph)
        ratioGraph.SetMarkerStyle(20)
        ratioGraph.SetMarkerSize(1.6)
        ratioGraph.SetMarkerColor(r.kGray+3)
        ratioGraph.SetLineColor(r.kGray+3)
        ratioGraph.SetLineWidth(2)
        ratioGraph.Draw('PZsame')

        line = r.TF1('line1', '0.92', 0, len(xlabels)+1)
        line.SetLineStyle(2)
        line.SetLineColor(r.kGray+2)
        line.Draw('same')

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


def drawBkgCompositionStack(indir, srNames=None, savename='bkgCompostion_std.pdf', plotData=False):

    # -------------------------------------------------------
    # Draw test bkg composition / expected yields hists
    if srNames == None:
        srNames = ['srA0', 'srA1', 'srA2', 'srB', 'srC','srD', 'srE0', 'srE1', 'srE2', 'srE3', 'srF', 'srG0', 'srG1', 'srG2', 'srG3', 'srH',]

    # f_bkg = r.TFile('../StopLooper/output/combRun2_v37_s5/allBkg_run2.root')
    f_bkg = r.TFile(indir+'/allBkg_run2.root','read')

    y_all  = [ y.round(2) for y in sum(getYieldEInTopoBins(f_bkg, srNames, 'metbins'), []) ]

    y_2lep = [ y.round(2) for y in sum(getYieldEInTopoBins(f_bkg, srNames, 'metbins_2lep'), []) ]
    y_1lt  = [ y.round(2) for y in sum(getYieldEInTopoBins(f_bkg, srNames, 'metbins_1lepTop'), []) ]
    y_1lW  = [ y.round(2) for y in sum(getYieldEInTopoBins(f_bkg, srNames, 'metbins_1lepW'), []) ]
    y_Zinv = [ y.round(2) for y in sum(getYieldEInTopoBins(f_bkg, srNames, 'metbins_Znunu'), []) ]

    h_2lep = getSRHistFromYieldE(y_2lep, 'h_SRyields_2lep', '', fcolor=r.kAzure+7)
    h_1lW  = getSRHistFromYieldE(y_1lW , 'h_SRyields_1lW' , '', fcolor=r.kOrange-2)
    h_1lt  = getSRHistFromYieldE(y_1lt , 'h_SRyields_1lt' , '', fcolor=r.kRed-7)
    h_Zinv = getSRHistFromYieldE(y_Zinv, 'h_SRyields_Zinv', '', fcolor=r.kMagenta-3)

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
    else:
        drawSRyieldStack(hstk, xlabels, legname, savename, 'hist')

    f_bkg.Close()


def drawBkgPredictionStack(indir, srNames=None, savename='bkgPrediction_std.pdf', plotData=False):

    # -------------------------------------------------------
    # Draw test bkg composition / expected yields hists
    if srNames == None:
        srNames = ['srA0', 'srA1', 'srA2', 'srB', 'srC','srD', 'srE0', 'srE1', 'srE2', 'srE3', 'srF', 'srG0', 'srG1', 'srG2', 'srG3', 'srH', 'srI',]

    f_lostlep = r.TFile(indir+'/lostlepton_run2.root','read')
    f_1lepW   = r.TFile(indir+'/1lepFromW_run2.root','read')
    f_1lepTop = r.TFile(indir+'/1lepFromTop_run2.root','read')
    f_Zinv    = r.TFile(indir+'/ZToNuNu_run2.root','read')

    # y_all  = [ y.round(2) for y in sum(getYieldEInTopoBins(f_bkg, srNames, 'metbins'), []) ]

    y_2lep = [ y.round(2) for y in sum(getYieldEInTopoBins(f_lostlep, srNames, 'metbins'), []) ]
    y_1lt  = [ y.round(2) for y in sum(getYieldEInTopoBins(f_1lepTop, srNames, 'metbins'), []) ]
    y_1lW  = [ y.round(2) for y in sum(getYieldEInTopoBins(f_1lepW,   srNames, 'metbins'), []) ]
    y_Zinv = [ y.round(2) for y in sum(getYieldEInTopoBins(f_Zinv,    srNames, 'metbins'), []) ]

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

    if plotData:
        f_data = r.TFile(indir+'/allData_run2.root')
        y_data = [ y.round(2) for y in sum(getYieldEInTopoBins(f_data, srNames, 'metbins'), []) ]
        h_data = getSRHistFromYieldE(y_data, 'h_SRyields_yields', '', fcolor=r.kBlack)
        drawSRyieldStack(hstk, xlabels, legname, savename, 'hist', hdata=h_data)
    else:
        drawSRyieldStack(hstk, xlabels, legname, savename, 'hist')

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


if __name__ == '__main__':

    r.gROOT.SetBatch(1)

    bvsuf = 'v39_m4'
    # indir = '../StopLooper/output/combRun2_'+bvsuf
    indir = '../StopLooper/output/samp17_v39_mrs2'

    # f18 = r.TFile('../StopLooper/output/samp18_'+bvsuf+'/lostlepton_18.root')
    # f17 = r.TFile('../StopLooper/output/samp17_'+bvsuf+'/lostlepton_17.root')
    # f16 = r.TFile('../StopLooper/output/samp16_'+bvsuf+'/lostlepton_16.root')
    # frun2 = r.TFile('../StopLooper/output/combRun2_'+bvsuf+'/lostlepton_run2.root')

    # srNames = ['srI',]

    # drawBkgEstimateHists(f18, srNames, '18')
    # drawBkgEstimateHists(f17, srNames, '17')
    # drawBkgEstimateHists(f16, srNames, '16')

    srNames = ['srA0', 'srA1', 'srA2', 'srB', 'srC', 'srD', 'srE0', 'srE1', 'srE2', 'srE3', 'srF', 'srG0', 'srG1', 'srG2', 'srG3', 'srH', 'srI',]
    # drawBkgCompositionStack(indir, srNames, 'bkgCompostion_all.pdf')

    # drawYieldsRatioComparison()

    # drawBkgEstimateHists(indir, srNames, 'run2', 'lostlep')

    crNames = [ sr.replace('sr', 'cr2l') for sr in srNames ]
    # drawBkgCompositionStack(indir, crNames, 'CR2lYields_all_mrs2.pdf', plotData=True)

    # drawYieldsRatioComparison(crNames, 'cr2lYieldCompare_METResOnvsOff_17.pdf')

    # srNames = ['srA0', 'srA1', 'srB', 'srC', 'srD', 'srE0', 'srE1', 'srF', 'srG0', 'srG1', 'srH', 'srI',]
    # drawBkgEstimateHists(indir, srNames, 'run2', '1lepW')

    crNames = [ sr.replace('sr', 'cr0b') for sr in srNames ]
    # drawBkgCompositionStack(indir, crNames, 'CR0bYields_all_mrs2.pdf', plotData=True)

    # drawYieldsRatioComparison(crNames, 'cr0bYieldCompare_METResOnvsOff_17.pdf')

    # srNames = ['srA0', 'srA1', 'srA2', 'srB', 'srC', 'srD', 'srE0', 'srE1', 'srE2', 'srE3', 'srF', 'srG0', 'srG1', 'srG2', 'srG3', 'srH',]
    # drawBkgPredictionStack(indir, srNames, 'bkgPrediction_std.pdf')

    # drawBkgCompositionStack(indir, srNames, 'bkgCompostion_all.pdf')
    # drawBkgCompositionStack(['srI',], 'bkgCompostion_cor6.pdf')
    # drawBkgCompositionStack(['cr2lI'], 'CR2lCompostion_cor6.pdf')
    # drawBkgCompositionStack(['cr0bI'], 'CR0bCompostion_cor6.pdf')

    # drawL1prefireEffect()
    drawHEMVetoEffect()

