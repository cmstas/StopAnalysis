#! /usr/bin/python
# coding: utf-8

import os
import sys
from math import sqrt
import ROOT as r

r.gROOT.SetBatch(1)
r.gStyle.SetOptStat(0)

def compareCorridor(indir):
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



if __name__ == '__main__':

    indir = '/home/users/sicheng/working/StopAnalysis/CombineAnalysis/limits/scan_combrun2_v37_s5/histo'

    compareCorridor(indir)
