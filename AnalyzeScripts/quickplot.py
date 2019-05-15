import os
import sys
from math import sqrt
import ROOT as r
# import numpy as np

r.gROOT.SetBatch(1)
r.gStyle.SetOptStat('')

def moveOverFlowToLastBin1D(hist, newXmax = 800):
    nbin = hist.GetNbinsX();
    if hist.GetBinContent(nbin+1) > 0:
        err = r.Double();
        hist.SetBinContent(nbin, hist.IntegralAndError(nbin, -1, err));
        hist.SetBinError(nbin, err);
        hist.SetBinContent(nbin+1, 0);
        hist.SetBinError(nbin+1, 0);

    hist.GetXaxis().SetRangeUser(hist.GetXaxis().GetXmin(), newXmax)

    return hist


def DrawHeaderText(canvas, lumi=137.2):
    tCMS = r.TLatex()
    tCMS.SetNDC(1)
    tCMS.SetTextFont(61)
    tCMS.SetTextAlign(11)
    tCMS.SetTextSize(0.0625)
    canvas.cd()
    tCMS.DrawLatex(canvas.GetLeftMargin(), 1.0-canvas.GetTopMargin()+0.012, "CMS")

    tplm = r.TLatex()
    tplm.SetNDC(1)
    tplm.SetTextFont(52)
    tplm.SetTextAlign(11)
    tplm.SetTextSize(0.048)
    canvas.cd()
    tplm.DrawLatex(canvas.GetLeftMargin()+0.105, 1.0-canvas.GetTopMargin()+0.012, "Preliminary")

    ttext = r.TLatex()
    ttext.SetNDC(1)
    ttext.SetTextFont(42)
    ttext.SetTextAlign(31)
    ttext.SetTextSize(0.048)
    canvas.cd()
    text = "{0} {1}^{{-1}} ({2} TeV)".format(lumi,"fb",13)
    ttext.DrawLatex(1.0-canvas.GetRightMargin()-0.01, 1.0-canvas.GetTopMargin()+0.012, text)

def plotHT5overHT():
    f1 = r.TFile('../StopLooper/output/samp17_v31_m12_noht5/data_2017BtoE.root')
    f2 = r.TFile('../StopLooper/output/samp17_v31_m12_noht5/data_2017F.root')

    # f1 = r.TFile('../StopLooper/output/samp18_v31_m12_noht5/data_2018A.root')
    # f2 = r.TFile('../StopLooper/output/samp18_v31_m12_noht5/data_2018D.root')

    h1 = f1.Get('cr0bbase/h_ht5overht')
    h2 = f2.Get('cr0bbase/h_ht5overht')

    h1.Scale(1.*h2.GetBinContent(1)/h1.GetBinContent(1))

    c0 = r.TCanvas('c1', 'c1', 800, 600)

    c0.SetLogy()

    h1.SetLineColor(r.kRed)
    h1.SetLineWidth(2)
    h2.SetLineWidth(2)
    h1.Rebin(2)
    h2.Rebin(2)

    leg = r.TLegend(0.65,0.8,0.88,0.9)
    leg.AddEntry(h1, '2017B-E')
    leg.AddEntry(h2, '2017F')
    # leg.AddEntry(h1, '2018A')
    # leg.AddEntry(h2, '2018D')

    h2.Draw('hist')
    h1.Draw('histsame')

    leg.Draw()

    c0.SaveAs('ht5overht_2017_rb2.pdf')


def plotDataMETinEMuCR(src=0, plot='metbins', hdir='cremuA1', bsuf='v31_m16_noMETttbarSF'):

    r.gStyle.SetOptStat('')
    r.gStyle.SetPadGridX(0)
    r.gStyle.SetPadGridY(0)
    r.gStyle.SetPadTickX(1)
    r.gStyle.SetPadTickY(1)
    r.gStyle.SetFrameBorderMode(0)

    srcstr = 'allData' if src == 0 else 'allBkg'
    f16 = r.TFile('../StopLooper/output/samp16_'+bsuf+'/'+srcstr+'_16.root')
    f17 = r.TFile('../StopLooper/output/samp17_'+bsuf+'/'+srcstr+'_17.root')
    f18 = r.TFile('../StopLooper/output/samp18_'+bsuf+'/'+srcstr+'_18.root')

    h16 = f16.Get(hdir+'/h_'+plot)
    h17 = f17.Get(hdir+'/h_'+plot)
    h18 = f18.Get(hdir+'/h_'+plot)

    c0 = r.TCanvas('c0', 'c0', 800, 800)

    mainPad = r.TPad('1', '1', 0.0, 0.20, 1.0, 0.99)
    ratioPad = r.TPad('2', '2', 0.0, 0.02, 1.0, 0.24)

    r.SetOwnership(c0, False)
    r.SetOwnership(mainPad, False)
    r.SetOwnership(ratioPad, False)

    mainPad.SetTopMargin(0.08)
    mainPad.SetLeftMargin(0.12)
    mainPad.SetRightMargin(0.05)
    mainPad.SetBottomMargin(0.15)
    mainPad.SetLogy()
    mainPad.Draw()
    ratioPad.SetTopMargin(0.05)
    ratioPad.SetLeftMargin(0.12)
    ratioPad.SetRightMargin(0.05)
    ratioPad.SetBottomMargin(0.10)
    ratioPad.Draw()

    mainPad.cd()

    h16.SetTitle('')
    h16.GetYaxis().SetTitle('Events')
    h16.GetYaxis().SetTitleOffset(0.75)
    h16.GetYaxis().SetTitleSize(0.058)
    h16.GetYaxis().SetTickSize(0.015)
    # h16.GetYaxis().SetRangeUser(0, 10000)
    # h16.GetXaxis().SetLabelSize(0)
    h16.GetXaxis().SetTitleSize(0.04)
    # print h16.GetXaxis().GetTitleOffset()

    xmax = 800 if h16.GetXaxis().GetXmax() > 800 else h16.GetXaxis().GetXmax()
    moveOverFlowToLastBin1D(h16, xmax)
    moveOverFlowToLastBin1D(h17, xmax)
    moveOverFlowToLastBin1D(h18, xmax)

    if 'lep2pt' in plot:
        h16.Rebin(3)
        h17.Rebin(3)
        h18.Rebin(3)

    h16.SetLineWidth(2)
    h17.SetLineWidth(2)
    h18.SetLineWidth(2)

    h16.SetMarkerSize(1.3)
    h17.SetMarkerSize(1.3)
    h18.SetMarkerSize(1.3)

    h16.SetMarkerStyle(24)
    h17.SetMarkerStyle(25)
    h18.SetMarkerStyle(26)

    h16.SetLineColor(r.kBlack)
    h17.SetLineColor(r.kRed)
    # h18.SetLineColor(r.kGreen)

    h17.Scale(35.9/42.0)
    h18.Scale(35.9/59.9)

    h16.Draw()
    h17.Draw("same")
    h18.Draw("same")

    leg = r.TLegend(0.68, 0.68, 0.92, 0.89)

    srcstr = 'data' if src == 0 else 'MC'
    leg.AddEntry(h16, srcstr+' 2016')
    leg.AddEntry(h17, srcstr+' 2017')
    leg.AddEntry(h18, srcstr+' 2018')
    leg.Draw()

    DrawHeaderText(mainPad, 35.9)

    ratioPad.cd()
    
    h_axis_ratio = r.TH1D('ratio_axis','', h16.GetNbinsX(), h16.GetXaxis().GetXmin(), xmax)
    h_axis_ratio.GetYaxis().SetNdivisions(4)
    h_axis_ratio.GetYaxis().SetLabelSize(0.15)
    h_axis_ratio.GetYaxis().SetRangeUser(0, 2)
    h_axis_ratio.GetYaxis().SetTitle('year / 2016')
    h_axis_ratio.GetYaxis().SetTitleOffset(0.25)
    h_axis_ratio.GetYaxis().SetTitleSize(0.18)
    h_axis_ratio.GetYaxis().SetTickLength(0.01)
    h_axis_ratio.GetXaxis().SetTickLength(0.07)
    h_axis_ratio.GetXaxis().SetTitleSize(0.0)
    h_axis_ratio.GetXaxis().SetLabelSize(0.0)
    h_axis_ratio.Draw('axis')

    line = r.TF1('line1', '1', h16.GetXaxis().GetXmin(), xmax)
    line.SetLineStyle(2)
    line.SetLineColor(r.kGray+2)
    line.Draw('same')

    rat17 = h17.Clone('ratio_17vs16')
    rat18 = h18.Clone('ratio_18vs16')
    rat17.Divide(h16)
    rat18.Divide(h16)

    rat17.Draw("same")
    rat18.Draw("same")

    c0.SaveAs(hdir+'_'+plot+'_'+srcstr+'.pdf')

def plotTopTaggerMatchingEfficiency(mstop = 1200, mlsp = 100, base = 'genbase'):

    ffull = r.TFile('../StopLooper/output/samp17_v31_ttagtest/T2tt_mStop{}_mLSP{}_f17v2_1.root'.format(mstop,mlsp))
    ffast = r.TFile('../StopLooper/output/samp17_v31_ttagtest/SMS_T2tt_17.root')

    dfull = ffull.Get('testGeneral/hden_rtag_{}_tftop_pt'.format(base))
    nfull = ffull.Get('testGeneral/hnum_rtag_{}_tftop_pt'.format(base))

    hfull = nfull.Clone('h_fullsim_{}_{}'.format(mstop,mlsp))
    hfull.Divide(dfull)

    dfast = ffast.Get('testGeneral/hden_rtag_{}_tftop_pt_{}_{}'.format(base,mstop,mlsp))
    nfast = ffast.Get('testGeneral/hnum_rtag_{}_tftop_pt_{}_{}'.format(base,mstop,mlsp))

    hfast = nfast.Clone('h_fastsim_{}_{}'.format(mstop,mlsp))
    hfast.Divide(dfast)

    efull = r.TEfficiency(nfull, dfull)
    efull.SetName('eff_{}_fullsim_T2tt_{}_{}'.format(base,mstop,mlsp))

    efast = r.TEfficiency(nfast, dfast)
    efast.SetName('eff_{}_fastsim_T2tt_{}_{}'.format(base,mstop,mlsp))

    hsf = hfull.Clone('SF_{}_T2tt_{}_{}'.format(base,mstop,mlsp))
    hsf.Divide(hfast)

    outfile = r.TFile('tempSF.root', 'update')
    efull.Write()
    efast.Write()
    hfast.Write()
    hfull.Write()
    hsf.Write()
    # esf.Write()

def plotTopTaggerSF3pt(tag = 'rtag', base = 'genbase'):

    ffull1 = r.TFile('../StopLooper/output/samp17_v31_ttagtest/T2tt_mStop{}_mLSP{}_f17v2_1.root'.format(1200,100))
    ffull2 = r.TFile('../StopLooper/output/samp17_v31_ttagtest/T2tt_mStop{}_mLSP{}_f17v2_1.root'.format(850,100))
    ffull3 = r.TFile('../StopLooper/output/samp17_v31_ttagtest/T2tt_mStop{}_mLSP{}_f17v2_1.root'.format(650,350))
    ffast = r.TFile('../StopLooper/output/samp17_v31_ttagtest/SMS_T2tt_17.root')

    ext = 'tftop_' if tag == 'rtag' else 'ak8top_' if tag == 'mtag' else ''

    nfull1 = ffull1.Get('testGeneral/hnum_{}_{}_{}pt'.format(tag,base,ext))
    nfull2 = ffull2.Get('testGeneral/hnum_{}_{}_{}pt'.format(tag,base,ext))
    nfull3 = ffull3.Get('testGeneral/hnum_{}_{}_{}pt'.format(tag,base,ext))
    dfull1 = ffull1.Get('testGeneral/hden_{}_{}_{}pt'.format(tag,base,ext))
    dfull2 = ffull2.Get('testGeneral/hden_{}_{}_{}pt'.format(tag,base,ext))
    dfull3 = ffull3.Get('testGeneral/hden_{}_{}_{}pt'.format(tag,base,ext))

    nfull = nfull1.Clone('hn_fullsim_3pt')
    nfull.Add(nfull2)
    nfull.Add(nfull3)
    dfull = dfull1.Clone('hd_fullsim_3pt')
    dfull.Add(dfull2)
    dfull.Add(dfull3)

    dfast1 = ffast.Get('testGeneral/hden_{}_{}_{}pt_{}_{}'.format(tag,base,ext,1200,100))
    dfast2 = ffast.Get('testGeneral/hden_{}_{}_{}pt_{}_{}'.format(tag,base,ext,850,100))
    dfast3 = ffast.Get('testGeneral/hden_{}_{}_{}pt_{}_{}'.format(tag,base,ext,650,350))
    nfast1 = ffast.Get('testGeneral/hnum_{}_{}_{}pt_{}_{}'.format(tag,base,ext,1200,100))
    nfast2 = ffast.Get('testGeneral/hnum_{}_{}_{}pt_{}_{}'.format(tag,base,ext,850,100))
    nfast3 = ffast.Get('testGeneral/hnum_{}_{}_{}pt_{}_{}'.format(tag,base,ext,650,350))

    nfast = nfast1.Clone('hn_fastsim_3pt')
    nfast.Add(nfast2)
    nfast.Add(nfast3)
    dfast = dfast1.Clone('hd_fastsim_3pt')
    dfast.Add(dfast2)
    dfast.Add(dfast3)

    nfull.Rebin(2)
    dfull.Rebin(2)
    nfast.Rebin(2)
    dfast.Rebin(2)

    hfull = nfull.Clone('heff_{}_fullsim'.format(base))
    hfull.Divide(nfull, dfull, 1, 1, 'B')

    hfast = nfast.Clone('heff_{}_fastsim'.format(base))
    hfast.Divide(nfast, dfast, 1, 1, 'B')

    efull = r.TEfficiency(nfull, dfull)
    efull.SetName('eff_{}_fullsim'.format(base))

    efast = r.TEfficiency(nfast, dfast)
    efast.SetName('eff_{}_fastsim'.format(base))

    hsf = hfull.Clone('SF_{}'.format(base))
    hsf.Divide(hfast)

    outfile = r.TFile(tag+'SF_rb2.root', 'update')
    efull.Write()
    efast.Write()
    hfast.Write()
    hfull.Write()
    hsf.Write()
    # esf.Write()

if __name__ == '__main__':

    # plotDataMETinEMuCR(0, 'metbins')
    # plotDataMETinEMuCR(1, 'metbins')
    # plotDataMETinEMuCR(0, 'rlmetbins')
    # plotDataMETinEMuCR(1, 'rlmetbins')
    # plotDataMETinEMuCR(0, 'ptttbar_b1')
    # plotDataMETinEMuCR(1, 'ptttbar_b1')
    # plotDataMETinEMuCR(0, 'njets')
    # plotDataMETinEMuCR(1, 'njets')
    # plotDataMETinEMuCR(0, 'lep2pt')
    # plotDataMETinEMuCR(1, 'lep2pt')

    # plotTopTaggerSF3pt('rtag', 'genbase')
    # plotTopTaggerSF3pt('rtag', 'tagbase')
    # plotTopTaggerSF3pt('mtag', 'genbase')
    # plotTopTaggerSF3pt('mtag', 'tagbase')

    plotTopTaggerSF3pt('softb', 'tagbase')

    # plotTopTaggerMatchingEfficiency(1200, 100)
    # plotTopTaggerMatchingEfficiency(850,  100)
    # plotTopTaggerMatchingEfficiency(650,  350)

    # plotTopTaggerMatchingEfficiency(1200, 100, 'tagbase')
    # plotTopTaggerMatchingEfficiency(850,  100, 'tagbase')
    # plotTopTaggerMatchingEfficiency(650,  350, 'tagbase')

