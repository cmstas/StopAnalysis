#! /usr/bin/python
# coding: utf-8

import os
import sys
from math import sqrt
import ROOT as r

r.gROOT.SetBatch(1)
r.gStyle.SetOptStat(0)

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

    fnum = r.TFile('limits/scan_combrun2_v37_s5/histo/Limits2DHistograms_compressed_T2tt_prefit.root')
    fden = r.TFile('limits/scan_combrun2_v37_s5/histo/Limits2DHistograms_std_T2tt_prefit.root')


    hden = fden.Get('hExpOrg')
    hnum = fnum.Get('hExpOrg')

    hrat = hnum.Clone('ratio_hExpOrg_compressed_vs_std')
    hrat.Divide(hden)

    c1 = r.TCanvas("c1", "c1", 1200, 900)

    # r.gStyle.SetPalette(r.kBrownCyan)
    r.gStyle.SetPalette(r.kRainBow)
    r.gStyle.SetPaintTextFormat("1.2f")

    hrat.GetZaxis().SetRangeUser(0, 2.5)
    hrat.GetXaxis().SetRangeUser(150, 1150)
    hrat.GetYaxis().SetRangeUser(0, 1000)

    hrat.SetTitle('Corridor+shape vs Standard')
    hrat.SetMarkerSize(0.7)
    # hrat.SetTitle('2016 Corridor vs Standard')
    hrat.Draw('colz')
    hrat.Draw('textsame')

    # c1.Print('temp_c5vsc3_scan.pdf')
    c1.Print('temp_s5_corvsstd_scan.pdf')


if __name__ == '__main__':

    indir = '/home/users/sicheng/working/StopAnalysis/CombineAnalysis/limits/scan_combrun2_v37_s5/histo'

    # compareCorridorStandard(indir)

    compareSkimsCorridor()
