#! /usr/bin/python
# coding: utf-8

import os
import sys
from math import sqrt
import ROOT as r

r.gROOT.SetBatch(1)
r.gStyle.SetOptStat(0)

def DrawHeaderText(canvas, lumi=137.2):
    tCMS = r.TLatex()
    tCMS.SetNDC(1)
    tCMS.SetTextFont(61)
    tCMS.SetTextAlign(11)
    tCMS.SetTextSize(0.0675)
    canvas.cd()
    tCMS.DrawLatex(canvas.GetLeftMargin(), 1.0-canvas.GetTopMargin()+0.01, "CMS")

    tplm = r.TLatex()
    tplm.SetNDC(1)
    tplm.SetTextFont(52)
    tplm.SetTextAlign(11)
    tplm.SetTextSize(0.052)
    canvas.cd()
    tplm.DrawLatex(canvas.GetLeftMargin()+0.11, 1.0-canvas.GetTopMargin()+0.01, "Simulation")

    ttext = r.TLatex()
    ttext.SetNDC(1)
    ttext.SetTextFont(42)
    ttext.SetTextAlign(31)
    ttext.SetTextSize(0.052)
    canvas.cd()
    text = "{0} {1}^{{-1}} ({2} TeV)".format(lumi,"fb",13)
    ttext.DrawLatex(1.0-canvas.GetRightMargin()-0.01, 1.0-canvas.GetTopMargin()+0.01, text)

def compareCorridorStandard(indir):
    fden = r.TFile(indir+'/Limits2DHistograms_std_T2tt_prefit.root')
    fnum = r.TFile(indir+'/Limits2DHistograms_compressed_T2tt_prefit.root')

    hden = fden.Get('hExpOrg')
    hnum = fnum.Get('hExpOrg')

    hrat = hnum.Clone('ratio_hExpOrg_std_vs_compressed')
    hrat.Divide(hden)

    c1 = r.TCanvas("c1", "c1", 1200, 800)

    # r.gStyle.SetPalette(r.kBrownCyan)
    r.gStyle.SetPalette(r.kRainBow)
    r.gStyle.SetPaintTextFormat("1.2f")

    hrat.GetZaxis().SetRangeUser(0, 2.5)
    hrat.GetXaxis().SetRangeUser(150, 850)
    hrat.GetYaxis().SetRangeUser(0, 700)

    hrat.Draw('colz')
    hrat.Draw('textsame')

    c1.Print('temp.pdf')

def compareSkimsCorridor():
    # fnum = r.TFile('limits/scan_combrun2_v37_c1/histo/Limits2DHistograms_compressed_T2tt_prefit.root')
    # fnum = r.TFile('limits/scan_combrun2_v37_c2/histo_run2_v37_c2/Limits2DHistograms_compressed_T2tt_prefit.root')
    # fnum = r.TFile('limits/scan_combrun2_v37_c3/histo/Limits2DHistograms_compressed_T2tt_prefit.root')
    # fnum = r.TFile('limits/scan_combrun2_v37_c4/histo_run2_v37_c4/Limits2DHistograms_compressed_T2tt_prefit.root')
    # fden = r.TFile('limits/scan_combrun2_v37_c3/histo/Limits2DHistograms_std_T2tt_prefit.root')
    # fnum = r.TFile('limits/scan_combrun2_v37_c5/histo/Limits2DHistograms_compressed_T2tt_prefit.root')

    # fnum = r.TFile('limits/scan_combrun2_v31_s6/histo/Limits2DHistograms_tcor_T2tt_prefit.root')
    # fden = r.TFile('limits/scan_combrun2_v31_s6/histo/Limits2DHistograms_std_T2tt_prefit.root')

    # fnum = r.TFile('limits/scan_combrun2_v31_s7/histo/Limits2DHistograms_tcor_T2tt_prefit.root')
    # fden = r.TFile('limits/scan_combrun2_v31_s7/histo/Limits2DHistograms_std_T2tt_prefit.root')

    # fnum = r.TFile('limits/scan_combrun2_v31_cor_w3/histo/Limits2DHistograms_srJ0_T2tt_prefit.root')
    # fden = r.TFile('limits/scan_combrun2_v31_cor_w3/histo/Limits2DHistograms_srJ0_T2tt_prefit.root')
    # fden = r.TFile('limits/scan_combrun2_v31_s8/histo/Limits2DHistograms_Wcor_T2tt_prefit.root')

    # fnum = r.TFile('limits/scan_combrun2_v31_s13/histo/Limits2DHistograms_std_T2tt_prefit.root')
    # fden = r.TFile('limits/scan_combrun2_v31_s13_nottag/histo/Limits2DHistograms_std_T2tt_prefit.root')
    # fden = r.TFile('limits/scan_combrun2_v31_m17sync/histo/Limits2DHistograms_std_T2tt_prefit.root')

    # fnum = r.TFile('limits/scan_combrun2_v31_m17almtt/histo/Limits2DHistograms_std_T2tt_prefit.root')
    # fden = r.TFile('limits/scan_combrun2_v31_m17sync/histo/Limits2DHistograms_std_T2tt_prefit.root')

    fnum = r.TFile('limits/scan_combrun2_v31_s18/histo/Limits2DHistograms_comb_T2tt_postfit.root')
    fden = r.TFile('limits/scan_combrun2_v31_s17/histo/Limits2DHistograms_comb_T2tt_postfit.root')

    hden = fden.Get('hExpOrg')
    hnum = fnum.Get('hExpOrg')

    hrat = hnum.Clone('ratio_hExpOrg_tcor_vs_std')
    hrat.Divide(hden)

    c1 = r.TCanvas("c1", "c1", 1200, 900)

    # r.gStyle.SetPalette(r.kBrownCyan)
    r.gStyle.SetPalette(r.kRainBow)
    r.gStyle.SetPaintTextFormat("1.2f")

    hrat.GetZaxis().SetRangeUser(0, 2.5)
    hrat.GetXaxis().SetRangeUser(150, 1400)
    hrat.GetYaxis().SetRangeUser(0, 1000)

    # hrat.SetTitle('W corridor vs Standard')
    # hrat.SetTitle('New MET bins vs Moriond 17 binning')
    # hrat.SetTitle('full-split vs run2 strategy')
    hrat.SetTitle('softb >5 Gev vs >0 GeV')
    # hrat.SetTitle(' ge3j(num) vs ge5j(den)')
    hrat.SetMarkerSize(0.56)
    # hrat.SetTitle('2016 Corridor vs Standard')
    hrat.Draw('colz')
    hrat.Draw('textsame')

    # c1.Print('temp_c5vsc3_scan.pdf')
    c1.Print('temp_s18vss17_scan.pdf')

def getPassesLimit():

    fnum = r.TFile('limits/scan_combrun2_v31_s20/histo/Limits2DHistograms_comb_T2tt_postfit.root')
    # fnum = r.TFile('limits/scan_combrun2_v31_s20/histo/Limits2DHistograms_std_T2tt_postfit.root')

    hden = fnum.Get('hExpOrg')
    # hden = fnum.Get('hObsOrg')
    hnum = hden.Clone('color')

    # hrat = hnum.Clone('ratio_hExpOrg_tcor_vs_std')
    # hrat.Divide(hden)

    c1 = r.TCanvas("c1", "c1", 1200, 900)

    # r.gStyle.SetPalette(r.kBrownCyan)
    # r.gStyle.SetPalette(r.kRainBow)
    r.gStyle.SetPalette(r.kLightTemperature)
    r.gStyle.SetPaintTextFormat("1.2f")

    for bx in range(1, hnum.GetNbinsX()+1):
        for by in range(1, hnum.GetNbinsY()+1):
            ibin = hnum.GetBin(bx, by)
            rval = hnum.GetBinContent(ibin)
            if rval <= 0: continue
            if rval < 1:
                hnum.SetBinContent(ibin, 1)
            elif rval > 1:
                hnum.SetBinContent(ibin, 10)

    hden.GetXaxis().SetRangeUser(150, 1500)
    # hrat.SetTitle('W corridor vs Standard')
    # hrat.SetTitle('New MET bins vs Moriond 17 binning')
    hden.SetTitle('T2tt observed expected exclusion decision')
    # hrat.SetTitle(' ge3j(num) vs ge5j(den)')
    hden.SetMarkerSize(0.50)
    hnum.GetZaxis().SetLabelSize(0)
    # hnum.GetZaxis().SetBinLabel(1,'Pass')
    # hnum.GetZaxis().SetBinLabel(19,'Fail')
    hden.GetZaxis().SetRangeUser(0, 10)
    # hrat.SetTitle('2016 Corridor vs Standard')
    hden.Draw('text')
    hnum.Draw('colzsame')
    hden.Draw('textsame')

    # c1.Print('temp_c5vsc3_scan.pdf')
    c1.Print('temp_pass_std_T2tt_exp_s20.pdf')


def printSigCont():

    fstd = r.TFile('datacards/sigcont/sigcont_std_T2tt.root')
    ftcor = r.TFile('datacards/sigcont/sigcont_tcor_T2tt.root')
    fWcor = r.TFile('datacards/sigcont/sigcont_Wcor_T2tt.root')

    fbt = r.TFile('datacards/sigcont/sigcont_std_T2bt.root')
    fbW = r.TFile('datacards/sigcont/sigcont_std_T2bW.root')

    hstd2l = fstd.Get('h2d_sigcont2l_wgtd')
    hstd0b = fstd.Get('h2d_sigcont0b_wgtd')
    htcor2l = ftcor.Get('h2d_sigcont2l_wgtd')
    htcor0b = ftcor.Get('h2d_sigcont0b_wgtd')
    hWcor2l = fWcor.Get('h2d_sigcont2l_wgtd')
    hWcor0b = fWcor.Get('h2d_sigcont0b_wgtd')

    hbW2l = fbW.Get('h2d_sigcont2l_wgtd')
    hbW0b = fbW.Get('h2d_sigcont0b_wgtd')
    hbt2l = fbt.Get('h2d_sigcont2l_wgtd')
    hbt0b = fbt.Get('h2d_sigcont0b_wgtd')

    hcomb2l = hstd2l.Clone()
    hcomb0b = hstd0b.Clone()

    for mstop in range(150, 1300, 25):
        for mlsp in range(0, 1150, 25):
            if mlsp > mstop: continue
            ibin = htcor2l.FindBin(mstop, mlsp)
            if mstop - mlsp <= 150:
                hcomb2l.SetBinContent(ibin, hWcor2l.GetBinContent(ibin))
                hcomb0b.SetBinContent(ibin, hWcor0b.GetBinContent(ibin))
            elif mstop - mlsp < 225:
                hcomb2l.SetBinContent(ibin, htcor2l.GetBinContent(ibin))
                hcomb0b.SetBinContent(ibin, htcor0b.GetBinContent(ibin))

    # hname = ['hbW2l', 'hbW0b', 'hbt2l', 'hbt0b', 'hcomb2l', 'hcomb0b',]
    # hists = [hbW2l, hbW0b, hbt2l, hbt0b, hcomb2l, hcomb0b,]

    hname = ['hbW2l', 'hbW0b', 'hbt2l', 'hbt0b', 'hcomb2l', 'hcomb0b',]
    hists = [hbW2l, hbW0b, hbt2l, hbt0b, hcomb2l, hcomb0b,]

    title = ['T2bW CR2l', 'T2bW CR0b', 'T2bt CR2l', 'T2bt CR0b', 'T2tt CR2l', 'T2tt CR0b',]

    ymax  = [650, 650, 650, 650, 1150, 1150 ]
    xmin  = [100, 100, 100, 100, 150, 150,  ]
    zmax  = [0.20, 0.05, 0.20, 0.05, 0.30, 0.15  ]


    c1 = r.TCanvas("c1", "c1", 1200, 900)

    # r.gStyle.SetPalette(r.kBrownCyan)
    # r.gStyle.SetPalette(r.kRainBow)
    r.gStyle.SetPalette(r.kLightTemperature)
    # r.gStyle.SetPalette(r.kGreenPink)
    r.gStyle.SetPaintTextFormat("1.2f")

    c1.SetLeftMargin(0.12)
    c1.SetRightMargin(0.14)
    c1.SetBottomMargin(0.12)
    # c1.SetLogz()

    ttxt1 = r.TLatex()
    ttxt1.SetNDC(1)
    ttxt1.SetTextFont(42)
    ttxt1.SetTextAlign(12)
    ttxt1.SetTextSize(0.042)

    for i, hnum in enumerate(hists):
        c1.Clear()
        hnum.GetXaxis().SetRangeUser(xmin[i], 1450)
        hnum.GetYaxis().SetRangeUser(0, ymax[i])
        # hnum.GetZaxis().SetRangeUser(0, zmax[i])
        hnum.GetZaxis().SetRangeUser(0, 0.15)
        hnum.SetTitle('')
        hnum.SetMarkerSize(0.56)
        hnum.GetXaxis().SetTitle('m_{#tilde{t}} [GeV]')
        hnum.GetYaxis().SetTitle('m_{#tilde{#chi}_{1}^{0}} [GeV]')
        # print hnum.GetYaxis().GetTitleSize()
        # print hnum.GetXaxis().GetTitleSize()
        # print hnum.GetYaxis().GetTitleOffset()
        # print hnum.GetZaxis().GetTitleOffset()
        hnum.GetXaxis().SetTitleSize(0.05)
        hnum.GetYaxis().SetTitleSize(0.05)
        hnum.GetZaxis().SetTitleSize(0.04)
        hnum.GetYaxis().SetTitleOffset(1)
        hnum.GetZaxis().SetTitleOffset(1.1)
        hnum.GetZaxis().SetTitle('relative signal contamination correction')
        hnum.GetZaxis().SetNdivisions(5)
        hnum.Draw('colz')
        # hnum.Draw('textsame')
        ttxt1.DrawLatex(c1.GetLeftMargin()+0.024, 1.0-c1.GetTopMargin()-0.036, "Signal Contamination")
        ttxt1.DrawLatex(c1.GetLeftMargin()+0.024, 1.0-c1.GetTopMargin()-0.092, title[i])

        DrawHeaderText(c1)

        c1.Print('sigcont_'+hname[i]+'_lin.pdf')


if __name__ == '__main__':

    indir = '/home/users/sicheng/working/StopAnalysis/CombineAnalysis/limits/scan_combrun2_v37_s5/histo'

    # compareCorridorStandard(indir)

    # compareSkimsCorridor()

    getPassesLimit()

    # printSigCont()

