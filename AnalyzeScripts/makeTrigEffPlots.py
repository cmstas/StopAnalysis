# coding: utf-8

import os
import sys
import ROOT as r
import numpy as np
from math import sqrt

r.gROOT.SetBatch(1)
r.gStyle.SetOptStat(0)

def getTEfficiency(f, hn, ysuf="", folder="testGeneral"):

    hnum = f.Get(folder+"/hnum2d_"+hn)
    hden = f.Get(folder+"/hden2d_"+hn)

    if not hnum: print " >> Cannot find "+folder+"/hnum2d_"+hn+" !!"
    if not hden: print " >> Cannot find "+folder+"/hden2d_"+hn+" !!"
    
    heff = r.TEfficiency(hnum, hden)
    heff.SetName("heff"+ysuf+"_"+hn)
    heff.SetTitle(hn)

    return heff

def fitEfficiency2D():
    f1 = r.TF1("f1", "[0]*TMath::Erf((x-[1])/[2])+[3]",  20, 200)
    f2 = r.TF1("f2", "[0]*TMath::Erf((x-[1])/[2])+[3]", 150, 550)

def printTextForHist(c1, hist, name):
    hist.SetMarkerSize(1.9)
    hist.GetXaxis().SetRangeUser(20,50)
    hist.GetYaxis().SetRangeUser(200,500)
    hist.Draw("text")
    c1.Print(name+".pdf")

def printEfficiency(c1, eff, name, rerange=True):
    heff = eff.GetCopyPassedHisto()
    heff.Divide(heff, eff.GetCopyTotalHisto(), 1, 1, "B")
    heff.SetMarkerSize(1.4)
    if rerange:
        heff.GetXaxis().SetRangeUser(20,50)
        heff.GetYaxis().SetRangeUser(200,500)
    heff.SetMaximum(1)
    heff.SetMinimum(0)
    heff.Draw("colz")
    heff.Draw("text same")
    c1.Print(name +".pdf")


if __name__ == "__main__":

    f17 = r.TFile("../StopLooper/output/data17_jetht/data_jetht_2017_v1.root")
    eff17 = getTEfficiency(f17, "trigeff_metrl_lep1pt", "17")
    eff17_mu = getTEfficiency(f17, "trigeff_metrl_lep1pt_mu", "17")
    eff17_el = getTEfficiency(f17, "trigeff_metrl_lep1pt_el", "17")

    f16 = r.TFile("../StopLooper/output/data16_jetht/data_jetht_2016_v3.root")
    eff16 = getTEfficiency(f16, "trigeff_metrl_lep1pt", "16")
    eff16_mu = getTEfficiency(f16, "trigeff_metrl_lep1pt_mu", "16")
    eff16_el = getTEfficiency(f16, "trigeff_metrl_lep1pt_el", "16")

    sreff17 = getTEfficiency(f17, "trigeff_met_lep1pt", "17")
    sreff17_mu = getTEfficiency(f17, "trigeff_met_lep1pt_mu", "17")
    sreff17_el = getTEfficiency(f17, "trigeff_met_lep1pt_el", "17")

    sreff16 = getTEfficiency(f16, "trigeff_met_lep1pt", "16")
    sreff16_mu = getTEfficiency(f16, "trigeff_met_lep1pt_mu", "16")
    sreff16_el = getTEfficiency(f16, "trigeff_met_lep1pt_el", "16")

    # move efficiencies from old file to a single combined file
    fold = r.TFile("../StopCORE/inputs/trigger/TriggerEff_2016.root")
    oeff16_el = fold.Get("Efficiency_ge2l_metrl_el")
    oeff16_mu = fold.Get("Efficiency_ge2l_metrl_mu")
    osreff16_el = fold.Get("Efficiency_ge1l_el")
    osreff16_mu = fold.Get("Efficiency_ge1l_mu")

    # fn17 = r.TFile("../StopLooper/output/data17_jetht/data_jetht_2017_addAK8ToBase.root")
    # effn17_mu = getTEfficiency(fn17, "trigeff_metrl_lep1pt_mu", "17_extb")
    # effn17_el = getTEfficiency(fn17, "trigeff_metrl_lep1pt_el", "17_extb")

    # fn16 = r.TFile("../StopLooper/output/data16_jetht/data_jetht_2016_addAK8ToBase.root")
    # effn16_mu = getTEfficiency(fn16, "trigeff_metrl_lep1pt_mu", "16_extb")
    # effn16_el = getTEfficiency(fn16, "trigeff_metrl_lep1pt_el", "16_extb")

    fn18 = r.TFile("../StopLooper/output/jetht_v30/data_2018.root")
    effn18_mu = getTEfficiency(fn18, "trigeff_metrl_lep1pt_mu", "18_extb")
    effn18_el = getTEfficiency(fn18, "trigeff_metrl_lep1pt_el", "18_extb")

    fn17 = r.TFile("../StopLooper/output/jetht_v30/data_2017.root")
    effn17_mu = getTEfficiency(fn17, "trigeff_metrl_lep1pt_mu", "17_extb")
    effn17_el = getTEfficiency(fn17, "trigeff_metrl_lep1pt_el", "17_extb")

    fn16 = r.TFile("../StopLooper/output/jetht_v30/data_2016.root")
    effn16_mu = getTEfficiency(fn16, "trigeff_metrl_lep1pt_mu", "16_extb")
    effn16_el = getTEfficiency(fn16, "trigeff_metrl_lep1pt_el", "16_extb")

    fd17 = r.TFile("../StopLooper/output/data17_jetht/data_jetht_2017_useDilepForCR2l.root")
    effd17_mu = getTEfficiency(fd17, "trigeff_metrl_lep1pt_mu", "17_dilep")
    effd17_el = getTEfficiency(fd17, "trigeff_metrl_lep1pt_el", "17_dilep")

    fd16 = r.TFile("../StopLooper/output/data16_jetht/data_jetht_2016_useDilepForCR2l.root")
    effd16_mu = getTEfficiency(fd16, "trigeff_metrl_lep1pt_mu", "16_dilep")
    effd16_el = getTEfficiency(fd16, "trigeff_metrl_lep1pt_el", "16_dilep")

    fout = r.TFile("TrigEfficiencies_all.root", "RECREATE")

    eff17.Write()
    eff17_mu.Write()
    eff17_el.Write()

    eff16.Write()
    eff16_mu.Write()
    eff16_el.Write()

    effn16_mu.Write()
    effn16_el.Write()
    effn17_mu.Write()
    effn17_el.Write()
    effn18_mu.Write()
    effn18_el.Write()

    effd16_mu.Write()
    effd16_el.Write()
    effd17_mu.Write()
    effd17_el.Write()

    sreff17.Write()
    sreff17_mu.Write()
    sreff17_el.Write()

    sreff16.Write()
    sreff16_mu.Write()
    sreff16_el.Write()

    oeff16_el.Write()
    oeff16_mu.Write()
    osreff16_el.Write()
    osreff16_mu.Write()
    
    # eff17.Fit()
    fout.Close()

    c1 = r.TCanvas("c1", "c1", 800, 600)

    printTextForHist(c1, eff16_mu.GetCopyTotalHisto(),  "hden16_mu")
    printTextForHist(c1, oeff16_mu.GetCopyTotalHisto(), "hdeno16_mu")
    printTextForHist(c1, eff16_el.GetCopyTotalHisto(),  "hden16_el")
    printTextForHist(c1, oeff16_el.GetCopyTotalHisto(), "hdeno16_el")
    printTextForHist(c1, eff17_mu.GetCopyPassedHisto(), "hnum17_mu")
    printTextForHist(c1, eff17_el.GetCopyPassedHisto(), "hnum17_el")
    printTextForHist(c1, eff17_mu.GetCopyTotalHisto(),  "hden17_mu")
    printTextForHist(c1, eff17_el.GetCopyTotalHisto(),  "hden17_el")

    printTextForHist(c1, effn17_mu.GetCopyTotalHisto(),  "hdenn17_mu")
    printTextForHist(c1, effn17_el.GetCopyTotalHisto(),  "hdenn17_el")
    printTextForHist(c1, effn16_mu.GetCopyTotalHisto(),  "hdenn16_mu")
    printTextForHist(c1, effn16_el.GetCopyTotalHisto(),  "hdenn16_el")

    r.gStyle.SetPaintTextFormat("4.3f")
    # r.gStyle.SetPalette(r.kRainBow)
    # r.gStyle.SetPalette(r.kDeepSea)
    r.gStyle.SetPalette(r.kBrownCyan)

    printEfficiency(c1, eff16_mu,    "heff16_mu")
    printEfficiency(c1, eff16_el,    "heff16_el")
    printEfficiency(c1, oeff16_mu,   "hoeff16_mu")
    printEfficiency(c1, oeff16_el,   "hoeff16_el")
    printEfficiency(c1, eff17_mu,    "heff17_mu")
    printEfficiency(c1, eff17_el,    "heff17_el")
    printEfficiency(c1, effn18_mu,   "heffn18_mu")
    printEfficiency(c1, effn18_el,   "heffn18_el")
    printEfficiency(c1, effn17_mu,   "heffn17_mu")
    printEfficiency(c1, effn17_el,   "heffn17_el")
    printEfficiency(c1, effn16_mu,   "heffn16_mu")
    printEfficiency(c1, effn16_el,   "heffn16_el")
    printEfficiency(c1, effd17_mu,   "heffd17_mu")
    printEfficiency(c1, effd17_el,   "heffd17_el")
    printEfficiency(c1, effd16_mu,   "heffd16_mu")
    printEfficiency(c1, effd16_el,   "heffd16_el")
    printEfficiency(c1, sreff17_mu,  "hsreff17_mu")
    printEfficiency(c1, sreff17_el,  "hsreff17_el")
    printEfficiency(c1, sreff16_mu,  "hsreff16_mu")
    printEfficiency(c1, sreff16_el,  "hsreff16_el")
    printEfficiency(c1, osreff16_mu, "hosreff16_mu")
    printEfficiency(c1, osreff16_el, "hosreff16_el")

