#!/usr/bin/env python
from __future__ import print_function, division

import os
import sys
from math import sqrt
import ROOT as r
# import numpy as np
from utilities.errors import *

r.gROOT.SetBatch(1)
r.gStyle.SetOptStat('')

def moveOverFlowToLastBin1D(hist, newXmax=-1.0):
    nbin = hist.GetNbinsX();
    if hist.GetBinContent(nbin+1) > 0:
        err = r.Double();
        hist.SetBinContent(nbin, hist.IntegralAndError(nbin, -1, err));
        hist.SetBinError(nbin, err);
        hist.SetBinContent(nbin+1, 0);
        hist.SetBinError(nbin+1, 0);

    if newXmax > 0:
        hist.GetXaxis().SetRangeUser(hist.GetXaxis().GetXmin(), newXmax)

    return hist


def DrawHeaderText(canvas, lumi=137.2, state="Preliminary"):
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
    tplm.SetTextSize(0.054)
    canvas.cd()
    tplm.DrawLatex(canvas.GetLeftMargin()+0.1, 1.0-canvas.GetTopMargin()+0.012, state)

    if lumi:
        ttext = r.TLatex()
        ttext.SetNDC(1)
        ttext.SetTextFont(42)
        ttext.SetTextAlign(31)
        ttext.SetTextSize(0.048)
        canvas.cd()
        text = "{0} {1}^{{-1}} ({2} TeV)".format(lumi,"fb",13)
        ttext.DrawLatex(1.0-canvas.GetRightMargin()-0.01, 1.0-canvas.GetTopMargin()+0.012, text)


def makeplot(hdir, hname='met', samp='', rebin=1, y2=None, newrange=None, norm=True, *arg, **kwargs):

    sampsuf = '' if samp == '' else '_'+samp
    doscale = (norm != False)

    # keyword arguments
    h2 = kwargs.get('h2', True)
    h3 = kwargs.get('h3', False)
    fname = kwargs.get('fname', '')
    sdir  = kwargs.get('sdir', 'temp')
    # hname = kwargs.get('hname', '')
    fh1 = r.TFile( kwargs.get('fname1', fname) )
    fh2 = r.TFile( kwargs.get('fname2', fname) ) if h2 else None
    fh3 = r.TFile( kwargs.get('fname3', fname) ) if h3 else None
    hn1 = '{0}{3}/h_{1}{2}{3}'.format( kwargs.get('hdir1', hdir), kwargs.get('hname1', hname), kwargs.get('hsuf1', ''), kwargs.get('ssuf1', sampsuf))
    hn2 = '{0}{3}/h_{1}{2}{3}'.format( kwargs.get('hdir2', hdir), kwargs.get('hname2', hname), kwargs.get('hsuf2', ''), kwargs.get('ssuf2', sampsuf)) if h2 else None
    hn3 = '{0}{3}/h_{1}{2}{3}'.format( kwargs.get('hdir3', hdir), kwargs.get('hname3', hname), kwargs.get('hsuf3', ''), kwargs.get('ssuf3', sampsuf)) if h3 else None
    hh1 =  kwargs.get('hist1', fh1.Get(hn1))
    hh2 =  kwargs.get('hist2', fh2.Get(hn2) if h2 else None)
    hh3 =  kwargs.get('hist3', fh3.Get(hn3) if h3 else None)
    nh1 = '{} {}'.format( kwargs.get('legn', ''), kwargs.get('ysuf1', '') )
    nh2 = '{} {}'.format( kwargs.get('legn', ''), kwargs.get('ysuf2', '') )
    nh3 = '{} {}'.format( kwargs.get('legn', ''), kwargs.get('ysuf3', '') )
    rtype = kwargs.get('rtype', 'Ratio')

    if not hh1: print( 'Cannot find {} in {}'.format( hn1, kwargs.get('fname1', fname)) )
    if h2 and not hh2: print( 'Cannot find hist {} in {}'.format(hn2, kwargs.get('fname2', fname)) )
    if h3 and not hh3: print( 'Cannot find hist {} in {}'.format(hn3, kwargs.get('fname3', fname)) )

    setrange = True if type(newrange) == list and len(newrange) > 1 else False
    newmax = newrange[1] if setrange else -1
    if kwargs.get('moveOFbin', True):
        moveOverFlowToLastBin1D(hh1,newmax)
        if h2: moveOverFlowToLastBin1D(hh2,newmax)
        if h3: moveOverFlowToLastBin1D(hh3,newmax)

    if rebin != 1:
        hh1.Rebin(rebin)
        if h2: hh2.Rebin(rebin)
        if h3: hh3.Rebin(rebin)

    if setrange:
        hh1.GetXaxis().SetRangeUser(newrange[0], newrange[1])
        if h2: hh2.GetXaxis().SetRangeUser(newrange[0], newrange[1])
        if h3: hh3.GetXaxis().SetRangeUser(newrange[0], newrange[1])

    c0 = r.TCanvas('c0', 'c0', 800, 800)

    if h2 and rtype:
        ratpad_height = kwargs.get("ratpad_size", 0.20)
        mainPad = r.TPad('1', '1', 0.0, ratpad_height, 1.0, 0.99)
        ratioPad = r.TPad('2', '2', 0.0, 0.02, 1.0, ratpad_height+0.04)
    else:
        c0 = r.TCanvas('c0', 'c0', 800, 640)
        mainPad = r.TPad('1', '1', 0.0, 0.00, 1.0, 0.99)

    r.SetOwnership(c0, False)
    r.SetOwnership(mainPad, False)
    if h2 and rtype: r.SetOwnership(ratioPad, False)

    mainPad.SetTopMargin(0.08)
    mainPad.SetLeftMargin(0.12)
    mainPad.SetRightMargin(0.05)
    mainPad.SetBottomMargin(0.14)
    if kwargs.get('logy', True):
        mainPad.SetLogy()
    else:
        mainPad.SetLogy(0)
    mainPad.Draw()
    if h2 and rtype:
        ratioPad.SetTopMargin(0.05)
        ratioPad.SetLeftMargin(0.12)
        ratioPad.SetRightMargin(0.05)
        ratioPad.SetBottomMargin(0.10)
        ratioPad.Draw()

    mainPad.cd()

    hh1.SetTitle('')
    if kwargs.get('xtitle', None):
        hh1.GetXaxis().SetTitle(kwargs.get('xtitle'))
    hh1.GetYaxis().SetTitle(kwargs.get('ytitle', 'Events'))
    hh1.GetYaxis().SetTitleOffset(0.75)
    hh1.GetYaxis().SetTitleSize(0.058)
    hh1.GetYaxis().SetTickSize(0.015)
    if h2: hh1.GetXaxis().SetTitleSize(0.046)
    else: hh1.GetXaxis().SetTitleSize(0.054)

    hh1.SetLineWidth(2)
    if h2: hh2.SetLineWidth(2)
    if h3: hh3.SetLineWidth(2)

    hh1.SetMarkerSize(1.3)
    if h2: hh2.SetMarkerSize(1.3)
    if h3: hh3.SetMarkerSize(1.3)

    hh1.SetMarkerStyle(25)
    if h2: hh2.SetMarkerStyle(kwargs.get('marker2', 26))
    if h3: hh3.SetMarkerStyle(kwargs.get('marker3', 24))

    hh1.SetLineColor(r.kGreen+3)
    # if h2: hh2.SetLineColor(r.kOrange+3)
    if h2: hh2.SetLineColor(kwargs.get('color2', r.kRed))
    if h3: hh3.SetLineColor(kwargs.get('color3', r.kBlue))

    if not h3 and 'data' in nh2:
        hh2.SetMarkerStyle(20)
        hh2.SetLineColor(r.kBlack)

    if kwargs.get('ymax', None):
        hh1.SetMaximum(kwargs.get('ymax'))
    if kwargs.get('ymin', None):
        hh1.SetMinimum(kwargs.get('ymin'))

    if 'TFvals' in fname:
        # c0 = r.TCanvas('c0', 'c0', 800, 500)
        mainPad.SetLogy(0)
        if 'eq0j' in sampsuf:
            hh1.SetMaximum(0.5)
        else:
            hh1.SetMaximum(0.25)
        hh1.SetMinimum(0.0)
        hh1.GetYaxis().SetTitle('Weights')

    if kwargs.get('prt_scale', False) and h2:
        err1 = r.Double()
        int1 = hh1.IntegralAndError(0,-1,err1)
        ehi1 = E(int1, err1)
        int2 = hh2.IntegralAndError(0,-1,err1)
        ehi2 = E(int2, err1)
        escale = ehi1/ehi2
        print('scale = {} for {}'.format(escale.round(4), hname))

    scale = hh1.Integral()/hh2.Integral() if h2 else 1
    if norm == 'lumi':
        if h2: hh2.Scale(35.92/41.53)
        if h3: hh3.Scale(35.9/59.74)
    elif type(norm) == list:
        if h2: hh2.Scale(norm[0])
        if h3: hh3.Scale(norm[1])
    elif doscale:
        if h2: hh2.Scale(hh1.Integral()/hh2.Integral())
        if h3: hh3.Scale(hh1.Integral()/hh3.Integral())

    hh1.Draw()
    if h2: hh2.Draw("same")
    if h3: hh3.Draw("same")

    legpos = 'tr'
    txtpos = (0.58, 0.64)
    selpos = (0.86, 0.56)

    if 'dphi' in hname or 'n' == hname[0] or 'Djj' in hname  :
        legpos = 'bl'
    elif 'Efrac' in hname or 'eta' in hname:
        legpos = 'bm'
    elif 'TFvals' in fname:
        legpos = 'br'

    legpos = kwargs.get('legpos', legpos)

    if legpos.lower() == 'tl':
        legpos = [0.14, 0.68, 0.5, 0.89]
        txtpos = (0.14, 0.64)
    elif legpos.lower() == 'bl':
        legpos = [0.14, 0.18, 0.5, 0.39]
        txtpos = (0.14, 0.4)
        selpos = (0.16, 0.84)
    elif legpos.lower() == 'tm':
        legpos = [0.34, 0.68, 0.7, 0.89]
        txtpos = (0.34, 0.64)
    elif legpos.lower() == 'bm':
        legpos = [0.34, 0.18, 0.7, 0.39]
        txtpos = (0.34, 0.4)
    elif legpos.lower() == 'bl':
        legpos = [0.14, 0.18, 0.5, 0.39]
        txtpos = (0.14, 0.40)
    elif legpos.lower() == 'br':
        legpos = [0.58, 0.18, 0.92, 0.39]
        txtpos = (0.58, 0.40)
    else:
        legpos = [0.58, 0.68, 0.92, 0.89]
        txtpos = (0.58, 0.64)

    if type(legpos) != list:
        print(legpos)

    if h2: 
        leg = r.TLegend(*legpos)
        leg.AddEntry(hh1, nh1)
        leg.AddEntry(hh2, nh2)
        if h3: leg.AddEntry(hh3, nh3)
        leg.Draw()

    selstr = kwargs.get('selstr', '')

    atxt = r.TLatex();
    atxt.SetNDC();
    atxt.SetTextSize(0.04);
    # atxt.SetTextAlign(22);
    if h2 and doscale and norm != 'arb.' and norm != 'lumi':
        atxt.DrawLatex(txtpos[0], txtpos[1], "{0} scaled by {1:.3f}".format(nh2,scale));

    # atxt.DrawLatex(txtpos[0], txtpos[1], "Int. (#gamma-trig) = {0}".format(hh2.Integral()));
    # atxt.DrawLatex(txtpos[0], txtpos[1]-0.035, "Int. (#font[12]{{ll}}-trig) = {0}".format(hh1.Integral()));

    atxt.SetTextSize(0.08);
    if not h3:
        atxt.DrawLatex(selpos[0], selpos[1], selstr);

    lumi = 35.9 if '16' in samp else 41.5 if '17' in samp else 59.7 if '18' in samp else kwargs.get('lumi', None)
    DrawHeaderText(mainPad, lumi)
    # DrawHeaderText(mainPad, None, 'Simulation')
    # DrawHeaderText(mainPad, None, 'Preliminary')

    if h2 and rtype:
        ratioPad.cd()

        h_axis_ratio = r.TH1D('ratio_axis','', hh1.GetNbinsX(), hh1.GetXaxis().GetXmin(), hh1.GetXaxis().GetXmax())
        h_axis_ratio.GetYaxis().SetNdivisions(4)
        h_axis_ratio.GetYaxis().SetLabelSize(0.15)
        h_axis_ratio.GetYaxis().SetRangeUser(kwargs.get('ratioMin', 0), kwargs.get('ratioMax', 2))
        if 'TFvals' in fname and 'eq0j' in sampsuf:
            h_axis_ratio.GetYaxis().SetRangeUser(0, 4)
        h_axis_ratio.GetYaxis().SetTitle(rtype)
        h_axis_ratio.GetYaxis().SetTitleOffset(0.25)
        h_axis_ratio.GetYaxis().SetTitleSize(0.18)
        h_axis_ratio.GetYaxis().SetTickLength(0.01)
        h_axis_ratio.GetXaxis().SetTickLength(0.07)
        h_axis_ratio.GetXaxis().SetTitleSize(0.0)
        h_axis_ratio.GetXaxis().SetLabelSize(0.0)
        if setrange: h_axis_ratio.GetXaxis().SetRangeUser(newrange[0], newrange[1])
        h_axis_ratio.Draw('axis')

        line = r.TF1('line1', '1', hh1.GetXaxis().GetXmin(), hh1.GetXaxis().GetXmax())
        line.SetLineStyle(2)
        line.SetLineColor(r.kGray+2)
        line.Draw('same')

        rat = hh2.Clone('ratio_gjetsvsdy')
        if kwargs.get('is_eff', False):
            rat = r.TEfficiency(rat, hh1)
            rat.SetName("heff_h2vsh1")
            rat.SetMarkerSize(1.3)
            rat.SetMarkerStyle(kwargs.get('marker2', 26))
            rat.SetLineColor(kwargs.get('color2', r.kRed))
        elif kwargs.get('use_binrat', False):
            rat.Divide(rat, hh1, 1, 1, 'B')
        else:
            rat.Divide(hh1)
        rat.Draw("same")
        ret_hrat = kwargs.get('ret_hrat', None)
        if type(ret_hrat) == str:
            ret_hrat = rat.GetName()
    if h3 and rtype:
        rat2 = hh3.Clone('ratio_h3vsdy')
        if kwargs.get('is_eff', False):
            rat2 = r.TEfficiency(rat2, hh1)
            rat2.SetName("heff_h2vsh1")
            rat2.SetMarkerSize(1.3)
            rat2.SetMarkerStyle(kwargs.get('marker3', 24))
            rat2.SetLineColor(kwargs.get('color3', r.kBlue))
        elif kwargs.get('use_binrat', False):
            rat2.Divide(rat2, hh1, 1, 1, 'B')
        else:
            rat2.Divide(hh1)
        rat2.Draw("same")
        ret_hrat2 = kwargs.get('ret_hrat2', None)
        if ret_hrat2: rat2.Copy(kwargs['ret_hrat2'])

    if kwargs.get('prt_rat', False):
        for i in range(1, rat.GetNbinsX()+1):
            rav = rat.GetBinContent(i)
            rae = rat.GetBinError(i)
            ra = E(rav, rae)
            print( "{},".format(ra.round(3) if ra.val > 0 else 0.0), end='')
        print( "" )

    savename = 'plots/{}/{}_{}_{}{}.pdf'.format(sdir, kwargs.get('snprf', 'plot'), hdir, hname, sampsuf)
    os.system('mkdir -p plots/{}'.format(sdir))
    c0.SaveAs(savename)
    print( 'Plot generated as ', savename)
    if kwargs.get('png', False):
        c0.SaveAs(savename.replace('.pdf','.png'))
    if kwargs.get('show', False): os.system('ic '+savename)


def makeSyncCheckPlot():
    
    indir = '../StopLooper/output/sampttDM_v32_m10_temp'
    args = {
        'h3'     : True,
        'fname1' : '../StopLooper/output/combRun2_v32_m10_temp/SMS_T2tt_mStop400to1200_run2.root',
        'fname2' : indir+'/TTbarDMJets_scalar_run2.root',
        'fname3' : indir+'/TTbarDMJets_pseudo_run2.root',
        'sdir'   : 'SyncCheck0l/',
        'ysuf2'  : 'scalar 1 400',
        'ysuf3'  : 'pseudo 1 400',
        'ysuf1'  : 'T2tt 750 1',
        'hsuf2'  : '_400_1',
        'hsuf3'  : '_400_1',
        'hsuf1'  : '_750_1',
        'rtype'  : 'Ratio',
        'norm'   : False,
        'lumi'   : 137,
        'show'   : True,
        'logy'   : False,
        # 'legpos' : 'bl',
        'snprf'  : 'T2tt_run2_lin',
        'savepng': True,
    }

    makeplot('testGeneral', 'check_met', '', rebin=2, **args)
    makeplot('testGeneral', 'check_met_uncorr', '', rebin=2, **args)
    makeplot('testGeneral', 'check_genmet', '', rebin=2, **args)
    makeplot('testGeneral', 'check_njets', '', rebin=1, **args)
    makeplot('testGeneral', 'check_nbjets', '', rebin=1, **args)

def plotMETfiltEff():
    args = {
        'h3'     : False,
        'fname1' : '../StopLooper/output/samp18_v32_s12_nometfilt/allData_18.root',
        # 'fname2' : '../StopLooper/output/samp18_v32_s11/allData_18.root',
        'fname2' : '../StopLooper/output/samp18_v32_s11_tighthalo/allData_18.root',
        # 'fname2' : indir+'/TTbarDMJets_scalar_run2.root',
        'sdir'   : 'MetFilt/',
        'ysuf1'  : 'w/o MET filt',
        'ysuf2'  : 'w/  MET filt',
        'rtype'  : 'Ratio',
        'is_eff' : True,
        'norm'   : False,
        'lumi'   : 137,
        'show'   : True,
    }

    makeplot('srsbfmet4', 'met_h', '', **args)
    # makeplot('cr2lsbfmet4', 'met', '', **args)
    # makeplot('cr2lsbfmet4', 'rlmet_h', '', **args)


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

    srcstr = 'allData' if src == 0 else 'allBkg' if src == 1 else 'ttbar_fastsim' if src == 2 else 'allBkg'
    if src == 0: srcstr = 'allData'
    elif src == 1: srcstr = 'allBkg'
    elif src == 2: srcstr = 'ttbar_fastsim'
    elif src == 3: srcstr = 'SMS_T2tt_425_325'
    elif src == 4: srcstr = 'TTTo2L2Nu'

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
    # print( h16.GetXaxis().GetTitleOffset() )

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

    # # Temporary
    # h17.Scale(1.*h18.Integral()/h17.Integral())

    h16.Draw()
    h17.Draw("same")
    h18.Draw("same")

    leg = r.TLegend(0.68, 0.68, 0.92, 0.89)

    srcstr = 'data' if src == 0 else 'MC' if src == 1 else 'ttFS' if src == 2 else 'T2tt' if src == 3 else 'MC'
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

def plotDoubleRatio(hdir='cremuA1', bsuf='v31_cremu4', suf='ge1sb'):

    r.gStyle.SetOptStat('')
    r.gStyle.SetPadGridX(0)
    r.gStyle.SetPadGridY(0)
    r.gStyle.SetPadTickX(1)
    r.gStyle.SetPadTickY(1)
    r.gStyle.SetFrameBorderMode(0)

    fdat = r.TFile('../StopLooper/output/combRun2_'+bsuf+'/allData_run2.root')
    fbkg = r.TFile('../StopLooper/output/combRun2_'+bsuf+'/allBkg_run2.root')

    hdat0 = fdat.Get(hdir+'/h_nvtxs')
    hdat1 = fdat.Get(hdir+'/h_nvtxs_'+suf)

    hbkg0 = fbkg.Get(hdir+'/h_nvtxs')
    hbkg1 = fbkg.Get(hdir+'/h_nvtxs_'+suf)

    c0 = r.TCanvas('c0', 'c0', 800, 600)

    mainPad = r.TPad('1', '1', 0.0, 0.30, 1.0, 0.99)
    ratioPad = r.TPad('2', '2', 0.0, 0.02, 1.0, 0.32)

    r.SetOwnership(c0, False)
    r.SetOwnership(mainPad, False)
    r.SetOwnership(ratioPad, False)

    mainPad.SetTopMargin(0.10)
    mainPad.SetLeftMargin(0.12)
    mainPad.SetRightMargin(0.05)
    mainPad.SetBottomMargin(0.15)
    # mainPad.SetLogy()
    mainPad.Draw()
    ratioPad.SetTopMargin(0.05)
    ratioPad.SetLeftMargin(0.12)
    ratioPad.SetRightMargin(0.05)
    ratioPad.SetBottomMargin(0.10)
    ratioPad.Draw()

    mainPad.cd()


    rbkg = hbkg1.Clone("ratio_bkg")
    rbkg.Divide(hbkg1,hbkg0,1,1,'B')

    rdat = hdat1.Clone("ratio_dat")
    rdat.Divide(hdat1,hdat0,1,1,'B')

    rbkg.SetLineWidth(2)
    rdat.SetLineWidth(2)

    # rbkg.SetMarkerSize(1.3)
    # rdat.SetMarkerSize(1.3)

    rbkg.SetMarkerStyle(24)
    rdat.SetMarkerStyle(20)

    rbkg.SetMarkerColor(r.kBlue)
    rdat.SetLineColor(r.kBlack)

    if 'lt10' in suf :
        rbkg.SetTitle('lead soft-b p_{T} < 10 GeV')
    elif 'gt10' in suf :
        rbkg.SetTitle('lead soft-b p_{T} > 10 GeV')

    rbkg.GetYaxis().SetTitle('(N_{soft-b} #geq 1) / (N_{soft-b} #geq 0) ')
    rbkg.GetYaxis().SetTitleOffset(0.75)
    rbkg.GetYaxis().SetTitleSize(0.058)
    rbkg.GetYaxis().SetTickSize(0.015)
    rbkg.GetYaxis().SetRangeUser(0, 0.3)
    rbkg.GetXaxis().SetTitleSize(0.05)
    rbkg.GetXaxis().SetTitleOffset(0.9)
    print( rbkg.GetXaxis().GetTitleOffset() )

    # print( rdat.GetBinContent(20) )
    # print( rbkg.GetBinContent(20) )

    rbkg.Draw()
    rdat.Draw("same")

    leg = r.TLegend(0.68, 0.68, 0.92, 0.89)

    leg.AddEntry(rdat, 'R(data) ')
    leg.AddEntry(rbkg, 'R(MC) ')
    leg.Draw()

    # DrawHeaderText(mainPad, 35.9)

    ratioPad.cd()

    h_axis_ratio = r.TH1D('ratio_axis','', rbkg.GetNbinsX(), rbkg.GetXaxis().GetXmin(), rbkg.GetXaxis().GetXmax())
    h_axis_ratio.GetYaxis().SetNdivisions(4)
    h_axis_ratio.GetYaxis().SetLabelSize(0.15)
    h_axis_ratio.GetYaxis().SetRangeUser(0, 2)
    h_axis_ratio.GetYaxis().SetTitle('R(data) / R(MC)')
    h_axis_ratio.GetYaxis().SetTitleOffset(0.4)
    h_axis_ratio.GetYaxis().SetTitleSize(0.12)
    h_axis_ratio.GetYaxis().SetTickLength(0.01)
    h_axis_ratio.GetXaxis().SetTickLength(0.07)
    h_axis_ratio.GetXaxis().SetTitleSize(0.0)
    h_axis_ratio.GetXaxis().SetLabelSize(0.0)
    h_axis_ratio.Draw('axis')

    line = r.TF1('line1', '1', rbkg.GetXaxis().GetXmin(), rbkg.GetXaxis().GetXmax())
    line.SetLineStyle(2)
    line.SetLineColor(r.kGray+2)
    line.Draw('same')

    dubr = rdat.Clone('ratio_17vs16')
    dubr.Divide(rbkg)
    dubr.SetMarkerStyle(20)
    dubr.SetMarkerColor(r.kBlue)
    dubr.SetLineColor(r.kBlue)

    dubr.Draw("same")

    c0.SaveAs('doubleRatio_nvtx_'+suf+'.pdf')

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

def plotTopTaggerSF3pt(tag = 'rtag', base = 'genbase', samp = '2017', suf = ''):

    # ffull1 = r.TFile('../StopLooper/output/samp17_v31_ttagtest/T2tt_mStop{}_mLSP{}_f17v2_1.root'.format(1200,100))
    # ffull2 = r.TFile('../StopLooper/output/samp17_v31_ttagtest/T2tt_mStop{}_mLSP{}_f17v2_1.root'.format(850,100))
    # ffull3 = r.TFile('../StopLooper/output/samp17_v31_ttagtest/T2tt_mStop{}_mLSP{}_f17v2_1.root'.format(650,350))
    # ffast = r.TFile('../StopLooper/output/samp17_v31_ttagtest/SMS_T2tt_17.root')

    ssuf = 'a18v1' if samp == '2018' else 'f17v2' if samp == '2017' else 's16v3' if samp == '2016' else ''

    ffull1 = r.TFile('../StopLooper/output/samp1x_v31_ttagtest9/T2tt_mStop{}_mLSP{}_{}.root'.format(650, 350, ssuf))
    ffast = r.TFile('../StopLooper/output/samp1x_v31_ttagtest9/SMS_T2tt_{}.root'.format(samp[2:]))

    ext = 'tftop_pt' if tag == 'rtag' else 'ak8top_pt' if tag == 'mtag' else ''

    if 'bin' in suf:
        ext += 'bin'

    nfull1 = ffull1.Get('testGeneral/hnum_{}_{}_{}'.format(tag,base,ext))
    dfull1 = ffull1.Get('testGeneral/hden_{}_{}_{}'.format(tag,base,ext))

    nfull = nfull1.Clone('hn_{}_{}_fullsim_3pt'.format(tag,base))
    dfull = dfull1.Clone('hd_{}_{}_fullsim_3pt'.format(tag,base))

    # dfast1 = ffast.Get('testGeneral/hden_{}_{}_{}_{}_{}'.format(tag,base,ext,650,350))
    # nfast1 = ffast.Get('testGeneral/hnum_{}_{}_{}_{}_{}'.format(tag,base,ext,650,350))

    dfast1 = ffast.Get('testGeneral/hden_{}_{}_{}_{}_{}'.format(tag,base,ext,850,100))
    nfast1 = ffast.Get('testGeneral/hnum_{}_{}_{}_{}_{}'.format(tag,base,ext,850,100))

    nfast = nfast1.Clone('hn_fastsim_3pt')
    dfast = dfast1.Clone('hd_fastsim_3pt')

    # for mstop, mlsp in [(625,325), (675,375), (600,300), (700,400), (800,50), (900,150), (1250, 150), (1150, 50)]:
    # for mstop, mlsp in [(625,325), (675,375), (600,300), (700,400)]:
    for mstop, mlsp in [(800,50), (900,150)]:
        dfastX = ffast.Get('testGeneral/hden_{}_{}_{}_{}_{}'.format(tag,base,ext,mstop,mlsp))
        nfastX = ffast.Get('testGeneral/hnum_{}_{}_{}_{}_{}'.format(tag,base,ext,mstop,mlsp))
        nfast.Add(nfastX)
        dfast.Add(dfastX)

    # if samp != '2018':
    #     ffull2 = r.TFile('../StopLooper/output/samp1x_v31_ttagtest9/T2tt_mStop{}_mLSP{}_{}.root'.format(850, 100, ssuf))

    #     nfull2 = ffull2.Get('testGeneral/hnum_{}_{}_{}'.format(tag,base,ext))
    #     dfull2 = ffull2.Get('testGeneral/hden_{}_{}_{}'.format(tag,base,ext))
    #     nfull.Add(nfull2)
    #     dfull.Add(dfull2)

    #     nfast2 = ffast.Get('testGeneral/hnum_{}_{}_{}_{}_{}'.format(tag,base,ext,850,100))
    #     dfast2 = ffast.Get('testGeneral/hden_{}_{}_{}_{}_{}'.format(tag,base,ext,850,100))
    #     nfast.Add(nfast2)
    #     dfast.Add(dfast2)

    # if samp != '2016':
    #     ffull3 = r.TFile('../StopLooper/output/samp1x_v31_ttagtest9/T2tt_mStop{}_mLSP{}_{}.root'.format(1200, 100, ssuf))

    #     nfull3 = ffull3.Get('testGeneral/hnum_{}_{}_{}'.format(tag,base,ext))
    #     dfull3 = ffull3.Get('testGeneral/hden_{}_{}_{}'.format(tag,base,ext))
    #     nfull.Add(nfull3)
    #     dfull.Add(dfull3)

    #     dfast3 = ffast.Get('testGeneral/hden_{}_{}_{}_{}_{}'.format(tag,base,ext,1200,100))
    #     nfast3 = ffast.Get('testGeneral/hnum_{}_{}_{}_{}_{}'.format(tag,base,ext,1200,100))
    #     nfast.Add(nfast3)
    #     dfast.Add(dfast3)

    moveOverFlowToLastBin1D(dfull)
    moveOverFlowToLastBin1D(nfull)
    moveOverFlowToLastBin1D(dfast)
    moveOverFlowToLastBin1D(nfast)

    if 'bin' not in suf:
        nfull.Rebin(2)
        dfull.Rebin(2)
        nfast.Rebin(2)
        dfast.Rebin(2)

    hfull = nfull.Clone('heff_{}_{}_fullsim'.format(tag,base))
    hfull.Divide(nfull, dfull, 1, 1, 'B')

    hfast = nfast.Clone('heff_{}_{}_fastsim'.format(tag,base))
    hfast.Divide(nfast, dfast, 1, 1, 'B')

    efull = r.TEfficiency(nfull, dfull)
    efull.SetName('eff_{}_{}_fullsim'.format(tag,base))

    efast = r.TEfficiency(nfast, dfast)
    efast.SetName('eff_{}_{}_fastsim'.format(tag,base))

    hsf = hfull.Clone('SF_{}_{}'.format(tag, base))
    hsf.Divide(hfast)

    outfile = r.TFile('{}SF_{}.root'.format(tag, suf), 'update')
    outdir = outfile.Get(samp)
    if not outdir: outdir = outfile.mkdir(samp)
    outdir.cd()
    efull.Write()
    efast.Write()
    nfull.Write()
    dfull.Write()
    hfast.Write()
    hfull.Write()
    hsf.Write()
    # esf.Write()

def getTotalEff(hnum, hden):
    err = r.Double(0)
    nbin = hnum.GetNbinsX()
    itn = hnum.IntegralAndError(1,nbin+1,err)
    ein = E(itn, err)
    itd = hden.IntegralAndError(1,nbin+1,err)
    eid = E(itd, err)
    return get_efficiency(ein, eid)

def plotFSSFfromTTbar(tag='softb',base='genbase',samp='2017',nlep='2lep'):

    # ffull = r.TFile('../StopLooper/output/samp{}_v31_ttagtest/TTJets_{}_{}_1.root'.format(samp[1:3],nlep,samp))
    # ffast = r.TFile('../StopLooper/output/samp{}_v31_ttagtest/TTJets_{}_{}fs_1.root'.format(samp[1:3],nlep,samp))

    # ffull = r.TFile('../StopLooper/output/samp{}_v31_ttagtest/ttbar_{}.root'.format(samp[1:3],samp[1:3]))
    # ffast = r.TFile('../StopLooper/output/samp{}_v31_ttagtest/ttbar_fastsim_nosbFSSF_{}.root'.format(samp[1:3],samp[1:3]))

    ffull = r.TFile('../StopLooper/output/samp1x_v31_ttagtest9/ttbar_{}.root'.format(samp[2:]))
    ffast = r.TFile('../StopLooper/output/samp1x_v31_ttagtest9/ttbar_fastsim_{}.root'.format(samp[2:]))

    ext = 'tftop_' if tag == 'rtag' else 'ak8top_' if tag == 'mtag' else ''
    # ext = 'tftop_' if tag == 'rtag' else 'gentop_' if tag == 'mtag' else ''
    # ext = 'gen' if 'soft' in tag and 'genbase' in base else ''

    # psuf = '_{}_{}'.format(m1, m2) if m1 > 0 else ''
    psuf = ''

    dfull = ffull.Get('testGeneral/hden_{}_{}_{}ptbin'.format(tag,base,ext))
    nfull = ffull.Get('testGeneral/hnum_{}_{}_{}ptbin'.format(tag,base,ext))
    dfast = ffast.Get('testGeneral/hden_{}_{}_{}ptbin{}'.format(tag,base,ext,psuf))
    nfast = ffast.Get('testGeneral/hnum_{}_{}_{}ptbin{}'.format(tag,base,ext,psuf))

    # dfull = ffull.Get('cremuB2/hden_{}_{}_{}pt'.format(tag,base,ext))
    # nfull = ffull.Get('cremuB2/hnum_{}_{}_{}pt'.format(tag,base,ext))
    # dfast = ffast.Get('cremuB2/hden_{}_{}_{}pt'.format(tag,base,ext))
    # nfast = ffast.Get('cremuB2/hnum_{}_{}_{}pt'.format(tag,base,ext))

    moveOverFlowToLastBin1D(dfull)
    moveOverFlowToLastBin1D(nfull)
    moveOverFlowToLastBin1D(dfast)
    moveOverFlowToLastBin1D(nfast)

    nfull.Rebin(2)
    dfull.Rebin(2)
    nfast.Rebin(2)
    dfast.Rebin(2)

    hfull = nfull.Clone('h_fullsim_{}_{}'.format(tag,base))
    hfull.Divide(dfull)

    hfull = nfull.Clone('heff_{}_{}_fullsim'.format(tag, base))
    hfull.Divide(nfull, dfull, 1, 1, 'B')

    hfast = nfast.Clone('heff_{}_{}_fastsim'.format(tag, base))
    hfast.Divide(nfast, dfast, 1, 1, 'B')

    efull = r.TEfficiency(nfull, dfull)
    efull.SetName('eff_{}_{}_fullsim'.format(tag, base))

    efast = r.TEfficiency(nfast, dfast)
    efast.SetName('eff_{}_{}_fastsim'.format(tag, base))

    hsf = hfull.Clone('SF_{}_{}'.format(tag, base))
    hsf.Divide(hfast)

    # eefull = getTotalEff(nfull, dfull)
    # eefast = getTotalEff(nfast, dfast)
    # esf = eefull/eefast
    # hsf.SetBinContent(1, esf.val)     # fill first bin with the average
    # hsf.SetBinError(1, esf.err)

    # outfile = r.TFile('{}SF_{}_{}v2.root'.format(tag,samp,nlep), 'update')
    outfile = r.TFile('{}SF_{}.root'.format(tag,nlep), 'update')
    outdir = outfile.Get(samp+psuf)
    if not outdir: outdir = outfile.mkdir(samp+psuf)
    outdir.cd()
    efull.Write()
    efast.Write()
    hfast.Write()
    hfull.Write()
    nfull.Write()
    dfull.Write()
    # nfast.Write()
    hsf.Write()
    # esf.Write()


def plotFSFSfromTTbar(hdir, hname='met',samp='2017', rebin=1, y2=None):

    ffull = r.TFile('../StopLooper/output/samp1x_v31_test11/ttbar_{}.root'.format(samp[2:]))
    ffast = r.TFile('../StopLooper/output/samp1x_v31_test11/ttbar_fastsim_{}.root'.format(samp[2:]))
    if y2: fy2 = r.TFile('../StopLooper/output/samp1x_v31_test11/ttbar_{}.root'.format(y2[2:]))
    samptype = 'madgraph'

    ffull = r.TFile('../StopLooper/output/samp1x_v31_test11/tt2l_17.root'.format(samp[2:]))
    ffast = r.TFile('../StopLooper/output/samp1x_v31_test11/tt2l_18.root'.format(samp[2:]))
    if y2: fy2 = r.TFile('../StopLooper/output/samp1x_v31_test11/tt2l_{}.root'.format(y2[2:]))
    samptype = 'powheg'

    hfull = ffull.Get('{}/h_{}'.format(hdir,hname))
    hfast = ffast.Get('{}/h_{}'.format(hdir,hname))
    if y2: hy2 = fy2.Get('{}/h_{}'.format(hdir,hname))

    # ffull = r.TFile('/home/users/sicheng/working/quicktests/genjetChecks/hists_ttbar_CP5_17.root'.format(samp[2:]))
    # ffast = r.TFile('/home/users/sicheng/working/quicktests/genjetChecks/hists_ttbar_CP2_17.root'.format(samp[2:]))
    # # ffast = r.TFile('/home/users/sicheng/working/quicktests/genjetChecks/hists_ttbar_M2_16.root'.format(samp[2:]))
    # if y2: fy2 = r.TFile('/home/users/sicheng/working/quicktests/genjetChecks/hists_ttbar_{}.root'.format(y2[2:]))
    # samptype = 'madgraph'

    # y2 = None
    # ffull = r.TFile('/home/users/sicheng/working/quicktests/genjetChecks/hists_tt2l_17.root'.format(samp[2:]))
    # ffast = r.TFile('/home/users/sicheng/working/quicktests/genjetChecks/hists_tt2l_16.root'.format(samp[2:]))
    # samptype = 'powheg'

    # hfull = ffull.Get('h_{}'.format(hname))
    # hfast = ffast.Get('h_{}'.format(hname))
    # if y2: hy2 = fy2.Get('h_{}'.format(hname))

    moveOverFlowToLastBin1D(hfull)
    moveOverFlowToLastBin1D(hfast)
    if y2: moveOverFlowToLastBin1D(hy2)

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

    hfull.SetTitle('')
    hfull.GetYaxis().SetTitle('Events')
    hfull.GetYaxis().SetTitleOffset(0.75)
    hfull.GetYaxis().SetTitleSize(0.058)
    hfull.GetYaxis().SetTickSize(0.015)
    hfull.GetXaxis().SetTitleSize(0.04)

    hfull.SetLineWidth(2)
    hfast.SetLineWidth(2)
    if y2: hy2.SetLineWidth(2)

    hfull.SetMarkerSize(1.3)
    hfast.SetMarkerSize(1.3)
    if y2: hy2.SetMarkerSize(1.3)

    hfull.SetMarkerStyle(25)
    hfast.SetMarkerStyle(26)
    if y2: hy2.SetMarkerStyle(26)

    hfull.SetLineColor(r.kBlack)
    # hfast.SetLineColor(r.kRed)
    if y2: hy2.SetLineColor(r.kRed)
    else: hfast.SetLineColor(r.kRed)

    hfast.Scale(hfull.Integral()/hfast.Integral())
    if y2: hy2.Scale(hfull.Integral()/hy2.Integral())

    if rebin != 1:
        hfull.Rebin(rebin)
        hfast.Rebin(rebin)
        if y2: hy2.Rebin(rebin)

    hfull.Draw()
    hfast.Draw("same")
    if y2: hy2.Draw("same")

    leg = r.TLegend(0.54, 0.68, 0.92, 0.89)

    if 'mother' in hname:
        leg = r.TLegend(0.24, 0.68, 0.62, 0.89)
    elif 'Efrac' in hname:
        leg = r.TLegend(0.34, 0.18, 0.7, 0.39)
    elif 'n' == hname[0]:
        leg = r.TLegend(0.14, 0.18, 0.5, 0.39)

    if samptype == 'madgraph':
        leg.AddEntry(hfull, 'ttbar '+samp[2:]+' full (TuneCP5)')
        leg.AddEntry(hfast, 'ttbar '+samp[2:]+' fast (TuneCP2)')
        # leg.AddEntry(hfast, 'ttbar 16'+' (TuneCUETP8M2)')
        if y2: leg.AddEntry(hy2, 'ttbar '+y2[2:]+' (TuneCUETP8M1)')
    if samptype == 'powheg':
        if y2:
            leg.AddEntry(hfull, 'ttbar '+' 17 (TuneCP5)')
            leg.AddEntry(hfast, 'ttbar '+' 18 (TuneCP5)')
            leg.AddEntry(hy2, 'ttbar '+y2[2:]+' (TuneCUETP8M2)')
        else:
            leg.AddEntry(hfull, 'ttbar 17'+' (TuneCP5)')
            leg.AddEntry(hfast, 'ttbar 16'+' (TuneCUETP8M2)')
    leg.Draw()

    # DrawHeaderText(mainPad, 35.9 if '16' in samp else 41.5 if '17' in samp else 59.7 if '18' in samp else 137)
    DrawHeaderText(mainPad, None, 'Simulation')

    ratioPad.cd()

    h_axis_ratio = r.TH1D('ratio_axis','', hfull.GetNbinsX(), hfull.GetXaxis().GetXmin(), hfull.GetXaxis().GetXmax())
    h_axis_ratio.GetYaxis().SetNdivisions(4)
    h_axis_ratio.GetYaxis().SetLabelSize(0.15)
    h_axis_ratio.GetYaxis().SetRangeUser(0, 2)
    # h_axis_ratio.GetYaxis().SetTitle('fast / full')
    h_axis_ratio.GetYaxis().SetTitle('Ratios')
    h_axis_ratio.GetYaxis().SetTitleOffset(0.25)
    h_axis_ratio.GetYaxis().SetTitleSize(0.18)
    h_axis_ratio.GetYaxis().SetTickLength(0.01)
    h_axis_ratio.GetXaxis().SetTickLength(0.07)
    h_axis_ratio.GetXaxis().SetTitleSize(0.0)
    h_axis_ratio.GetXaxis().SetLabelSize(0.0)
    h_axis_ratio.Draw('axis')

    line = r.TF1('line1', '1', hfull.GetXaxis().GetXmin(), hfull.GetXaxis().GetXmax())
    line.SetLineStyle(2)
    line.SetLineColor(r.kGray+2)
    line.Draw('same')

    rat = hfast.Clone('ratio_17vs16')
    rat.Divide(hfull)
    rat.Draw("same")

    if y2:
        rat2 = hy2.Clone('ratio_y2vs16')
        rat2.Divide(hfull)
        rat2.Draw("same")

    os.system('mkdir -p plots/genbcomp_madgraph')
    os.system('mkdir -p plots/genbcomp_powheg')

    if samptype == 'madgraph':
        c0.SaveAs('plots/ttbar_madgraph_comp/{}_{}_{}.pdf'.format(hdir, hname, samp))
        # c0.SaveAs('plots/genbcomp_madgraph/{}_{}_{}.pdf'.format(hdir, hname, samp))
    if samptype == 'powheg':
        c0.SaveAs('plots/ttbar_powheg_comp/{}_{}_{}.pdf'.format(hdir, hname, samp))
        # c0.SaveAs('plots/genbcomp_powheg/{}_{}_{}.pdf'.format(hdir, hname, samp))



def makeFSSFplots(samp='2017', fn='mtagSF_tGv1', hn='mtag_genbase'):

    f1 = r.TFile('./'+fn+'.root','read')

    h1 = f1.Get(samp+'/SF_'+hn)
    # h1.GetXaxis().SetTitle('p_{T}(AK8-jet) [GeV]')
    h1.GetXaxis().SetTitle('p_{T}(top candidate) [GeV]')
    h1.GetYaxis().SetRangeUser(0.6,1.4)
    h1.SetLineWidth(2)
    h1.SetMarkerStyle(24)
    h1.GetYaxis().SetTitle('SF (Full/Fast)')

    # avg = float(h1.GetBinContent(1))
    # line1 = r.TF1("l1", "{}".format(avg), 0, 50)
    # line1.SetLineStyle(2)
    # line1.SetLineWidth(2)
    # line1.SetLineColor(r.kGray+2)

    # h1.SetBinContent(1,0)
    # h1.SetBinError(1,0)

    c0 = r.TCanvas('c0', 'c0', 800, 600)

    # Do fit
    print( samp )
    # h1.SetBinContent(1,0)
    # h1.SetBinError(1,0)
    h1.Fit('pol0')
    print( '' )
    # h1.GetXaxis().SetRangeUser(0,45)

    h1.Draw()
    # line1.Draw('same')

    # txt = r.TText(0.7,0.7, '{:.3f}'.format(avg))
    # txt.Draw()
    DrawHeaderText(c0,False)

    c0.Print('hSF_{}_{}_{}.pdf'.format(hn.split('_')[0], fn.split('_')[1], samp))
    c0.Clear()

    h1 = f1.Get(samp+'/heff_'+hn+'_fastsim')
    h1.GetXaxis().SetTitle('p_{T}(AK8-jet) [GeV]')
    # h1.GetYaxis().SetRangeUser(0.0,0.6)
    # h1.GetXaxis().SetRangeUser(0.0,45)
    h1.SetLineWidth(2)
    h1.SetMarkerStyle(24)
    h1.GetYaxis().SetTitle('Efficiency')

    h2 = f1.Get(samp+'/heff_'+hn+'_fullsim')
    h2.SetLineWidth(2)
    h2.SetLineColor(r.kRed)
    h2.SetMarkerStyle(25)

    h1.Draw()
    h2.Draw('same')

    leg = r.TLegend(0.65,0.7,0.88,0.86)
    leg.AddEntry(h1, 'Fastsim {} '.format(samp[2:]))
    leg.AddEntry(h2, 'Fullsim {} '.format(samp[2:]))

    leg.Draw()

    DrawHeaderText(c0,False)

    c0.Print('heff_{}_{}_{}.pdf'.format(hn.split('_')[0], fn.split('_')[1], samp))


def printRatiosFast(hdir='cremuB1', bsuf='v31_ttagtest', ysuf='17', hn='genb_genpt_unwgtd'):

    # fnum = r.TFile('../StopLooper/output/samp{}_{}/ttbar_{}.root'.format(ysuf, bsuf, ysuf))
    # fden = r.TFile('../StopLooper/output/samp{}_{}/ttbar_fastsim_{}.root'.format(ysuf, bsuf, ysuf))
    fnum = r.TFile('../StopLooper/output/samp1x_{}/ttbar_{}.root'.format(bsuf, ysuf))
    fden = r.TFile('../StopLooper/output/samp1x_{}/ttbar_fastsim_{}.root'.format(bsuf, ysuf))

    hnum = fnum.Get(hdir+'/h_'+hn)
    hden = fden.Get(hdir+'/h_'+hn)

    hrat = hnum.Clone('hrat_'+hn)
    hrat.Scale(hden.Integral(0,-1)/hnum.Integral(0,-1))
    hrat.Divide(hrat,hden,1,1,'B')

    for ibin in range(1, hrat.GetNbinsX()+1):
        print( ibin, '{:.3f}'.format(hrat.GetBinContent(ibin)) )

    # fout = r.TFile('./scaler.root','recreate')

def makeSinglePlot():
    r.gStyle.SetOptStat('')
    r.gStyle.SetPadGridX(0)
    r.gStyle.SetPadGridY(0)
    r.gStyle.SetPadTickX(1)
    r.gStyle.SetPadTickY(1)
    r.gStyle.SetFrameBorderMode(0)

    f = r.TFile('../StopLooper/output/samp18_v32_s1_refans/TTJets_2lep_a18v1.root')
    h = f.Get('testGeneral/h_genlep2_eta')

    c0 = r.TCanvas('c0', 'c0', 800, 600)

    mainPad = r.TPad('1', '1', 0.0, 0.00, 1.0, 0.99)

    r.SetOwnership(c0, False)
    r.SetOwnership(mainPad, False)

    mainPad.SetTopMargin(0.08)
    mainPad.SetLeftMargin(0.12)
    mainPad.SetRightMargin(0.05)
    mainPad.SetBottomMargin(0.15)
    mainPad.SetLogy()
    mainPad.Draw()

    mainPad.cd()

    h.SetTitle('')
    h.GetYaxis().SetTitle('Events')
    h.GetYaxis().SetTitleOffset(0.92)
    h.GetYaxis().SetTitleSize(0.052)
    h.GetYaxis().SetLabelSize(0.048)
    # h.GetYaxis().SetTickSize(0.015)
    # h.GetXaxis().SetTitleOffset(0.75)
    h.GetXaxis().SetTitleSize(0.052)
    h.GetXaxis().SetLabelSize(0.048)
    h.GetXaxis().SetTitle('#eta (sub-leading gen-lepton)')

    print( h.GetXaxis().GetLabelSize(), h.GetXaxis().GetTitleOffset() )


    h.SetLineWidth(2)
    # h.SetMarkerSize(1.2)
    h.SetMarkerStyle(24)
    h.SetLineColor(r.kBlack)

    h.Draw()

    leg = r.TLegend(0.72, 0.8, 0.91, 0.86 )
    leg.AddEntry(h, 't#bar{t}#rightarrow 2#font[12]{l}2#nu')
    leg.SetBorderSize(0)
    leg.Draw()

    DrawHeaderText(mainPad, None, 'Simulation')

    c0.Print('h_genlep2_eta.pdf')

if __name__ == '__main__':

    # makeSinglePlot()
    # makeSyncCheckPlot()
    plotMETfiltEff()

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
    # plotDataMETinEMuCR(0, 'jet1pt')
    # plotDataMETinEMuCR(0, 'jet2pt')

    # plotDataMETinEMuCR(2, 'nisrmatch', bsuf='v31_cremu4')
    # plotDataMETinEMuCR(1, 'nisrmatch', bsuf='v31_cremu4')
    # plotDataMETinEMuCR(0, 'njets', bsuf='v31_cremu4')
    # plotDataMETinEMuCR(3, 'nisrmatch', bsuf='v31_cremu4')

    # plotDataMETinEMuCR(0, 'ptbb', bsuf='v31_cremu4')
    # plotDataMETinEMuCR(0, 'ptll', bsuf='v31_cremu4')
    # plotDataMETinEMuCR(0, 'nbjets', bsuf='v31_cremu4')
    # plotDataMETinEMuCR(1, 'nbjets', bsuf='v31_cremu4')
    # plotDataMETinEMuCR(0, 'nsbtags', bsuf='v31_cremu4')
    # plotDataMETinEMuCR(1, 'nsbtags', bsuf='v31_cremu4')

    # plotDataMETinEMuCR(4, 'met_uncorr', bsuf='v31_s21_cremu5_noISR', hdir='cremuA0')
    # plotDataMETinEMuCR(4, 'met_uncorr_b1', bsuf='v31_s21_cremu5_noISR', hdir='cremuA0')
    # plotDataMETinEMuCR(4, 'genb_mother_nott', bsuf='v31_s21_cremu5_noISR', hdir='cremuA0')
    # plotDataMETinEMuCR(4, 'genbpt_fromg', bsuf='v31_s21_cremu5_noISR', hdir='cremuA0')
    # plotDataMETinEMuCR(4, 'genbpt_fromt_fc', bsuf='v31_s21_cremu5_noISR', hdir='cremuA0')
    # plotDataMETinEMuCR(4, 'genbpt_fromt_lc', bsuf='v31_s21_cremu5_noISR', hdir='cremuA0')
    # plotDataMETinEMuCR(4, 'lead_genbpt', bsuf='v31_s21_cremu5_noISR', hdir='cremuA0')
    # plotDataMETinEMuCR(4, 'lead_genbpt_b1', bsuf='v31_s21_cremu5_noISR', hdir='cremuA0')

    # plotFSFSfromTTbar('cremuA0', 'genb_mother_nott',samp='2018')
    # plotFSFSfromTTbar('cremuA0', 'genbpt_fromg',samp='2018')
    # plotFSFSfromTTbar('cremuA0', 'genbpt_fromt_lc',samp='2018')

    # plotFSFSfromTTbar('testGeneral', 'genb_fromtop_genpt',samp='2016', rebin=2)
    # plotFSFSfromTTbar('testGeneral', 'genb_notfromtop_genpt',samp='2016', rebin=2)
    # plotFSFSfromTTbar('testGeneral', 'genb_fromg_genpt',samp='2016', rebin=2)
    # plotFSFSfromTTbar('testGeneral', 'genb_frompp_genpt',samp='2016', rebin=2)

    # plotFSFSfromTTbar('testGeneral', 'genb_fromtop_genpt',samp='2017', rebin=2, y2='2016')
    # plotFSFSfromTTbar('testGeneral', 'genb_FC_fromtop_genpt',samp='2017', rebin=2, y2='2016')
    # plotFSFSfromTTbar('testGeneral', 'genb_notfromtop_genpt',samp='2017', rebin=2, y2='2016')
    # plotFSFSfromTTbar('testGeneral', 'genb_fromg_genpt',samp='2017', rebin=2, y2='2016')
    # plotFSFSfromTTbar('testGeneral', 'genb_frompp_genpt',samp='2017', rebin=2, y2='2016')
    # plotFSFSfromTTbar('testGeneral', 'genb_notfromtop_mother',samp='2017', y2='2016')
    # plotFSFSfromTTbar('testGeneral', 'matched_bgenjetpt',samp='2017', rebin=2, y2='2016')
    # plotFSFSfromTTbar('testGeneral', 'matched_genbgenjet_DR',samp='2017', rebin=2, y2='2016')
    # plotFSFSfromTTbar('testGeneral', 'matched_genbgenjet_Efrac',samp='2017', rebin=2, y2='2016')
    # plotFSFSfromTTbar('testGeneral', 'bgenjet_fromtop_genpt',samp='2017', rebin=2, y2='2016')
    # plotFSFSfromTTbar('testGeneral', 'bgenjet_fromg_genpt',samp='2017', rebin=2, y2='2016')
    # plotFSFSfromTTbar('testGeneral', 'bgenjet_frompp_genpt',samp='2017', rebin=2, y2='2016')
    # plotFSFSfromTTbar('testGeneral', 'matched_bjetpt',samp='2017', rebin=2, y2='2016')
    # plotFSFSfromTTbar('testGeneral', 'matched_bjetgenb_DR',samp='2017', rebin=2, y2='2016')
    # plotFSFSfromTTbar('testGeneral', 'matched_bjetgenb_Efrac',samp='2017', rebin=2, y2='2016')
    # plotFSFSfromTTbar('testGeneral', 'matched_bjet_deepCSV',samp='2017', rebin=2, y2='2016')
    # plotFSFSfromTTbar('testGeneral', 'bjet_fromg_deepCSV',samp='2017', rebin=2, y2='2016')
    # plotFSFSfromTTbar('testGeneral', 'bjet_frompp_deepCSV',samp='2017', rebin=2, y2='2016')
    # plotFSFSfromTTbar('testGeneral', 'bjet_fromtop_deepCSV',samp='2017', rebin=2, y2='2016')
    # plotFSFSfromTTbar('testGeneral', 'bjet_fromtop_pt',samp='2017', rebin=2, y2='2016')
    # plotFSFSfromTTbar('testGeneral', 'bjet_frompp_pt',samp='2017', rebin=2, y2='2016')
    # plotFSFSfromTTbar('testGeneral', 'bjet_fromg_pt',samp='2017', rebin=2, y2='2016')
    # plotFSFSfromTTbar('testGeneral', 'njet_bmatch',samp='2017', y2='2016')
    # plotFSFSfromTTbar('testGeneral', 'nbjet_match',samp='2017', y2='2016')
    # plotFSFSfromTTbar('testGeneral', 'nbgenjet',samp='2017', y2='2016')

    # PlotFSFSfromTTbar('testGeneral', 'genb_fromtop_genpt',samp='2018', rebin=2)
    # plotFSFSfromTTbar('testGeneral', 'genb_notfromtop_genpt',samp='2018', rebin=2)
    # plotFSFSfromTTbar('testGeneral', 'genb_fromg_genpt',samp='2018', rebin=2)
    # plotFSFSfromTTbar('testGeneral', 'genb_frompp_genpt',samp='2018', rebin=2)

    # plotTopTaggerSF3pt('rtag', 'genbase')
    # plotTopTaggerSF3pt('rtag', 'tagbase')
    # plotTopTaggerSF3pt('mtag', 'genbase')
    # plotTopTaggerSF3pt('mtag', 'tagbase')

    # plotTopTaggerSF3pt('softb', 'tagbase')

    # plotFSSFfromTTbar('softb', 'tagbase')
    # plotFSSFfromTTbar('softb', 'genbase')
    # plotFSSFfromTTbar('softb', 'tagbase', '2018')
    # plotFSSFfromTTbar('softb', 'genbase', '2018')
    # plotFSSFfromTTbar('softb', 'tagbase', '2016')
    # plotFSSFfromTTbar('softb', 'genbase', '2016')

    # plotFSSFfromTTbar('softb', 'genbase',  '2016', 'fromtv2')
    # plotFSSFfromTTbar('softb', 'genbase',  '2017', 'fromtv2')
    # plotFSSFfromTTbar('softb', 'genbase',  '2018', 'fromtv2')
    # plotFSSFfromTTbar('softb', 'genbase2', '2016', 'fromtv2')
    # plotFSSFfromTTbar('softb', 'genbase2', '2017', 'fromtv2')
    # plotFSSFfromTTbar('softb', 'genbase2', '2018', 'fromtv2')
    # plotFSSFfromTTbar('softb', 'genbase3', '2016', 'fromtv2')
    # plotFSSFfromTTbar('softb', 'genbase3', '2017', 'fromtv2')
    # plotFSSFfromTTbar('softb', 'genbase3', '2018', 'fromtv2')
    # plotFSSFfromTTbar('softb', 'objbase',  '2016', 'fromtv2')
    # plotFSSFfromTTbar('softb', 'objbase',  '2017', 'fromtv2')
    # plotFSSFfromTTbar('softb', 'objbase',  '2018', 'fromtv2')

    # plotFSSFfromTTbar('softb', 'genbase3', '2016', 'B2fromtv1')
    # plotFSSFfromTTbar('softb', 'genbase3', '2017', 'B2fromtv1')
    # plotFSSFfromTTbar('softb', 'genbase3', '2018', 'B2fromtv1')

    # plotFSSFfromTTbar('softb', 'qmatch', '2016', 'B2fromtv3')
    # plotFSSFfromTTbar('softb', 'qmatch', '2017', 'B2fromtv3')
    # plotFSSFfromTTbar('softb', 'qmatch', '2018', 'B2fromtv3')
    # plotFSSFfromTTbar('softb', 'cmatch', '2016', 'B2fromtv3')
    # plotFSSFfromTTbar('softb', 'cmatch', '2017', 'B2fromtv3')
    # plotFSSFfromTTbar('softb', 'cmatch', '2018', 'B2fromtv3')

    # plotTopTaggerSF3pt('mtag', 'genbase', '2016', 'tG3ptbinv2')
    # plotTopTaggerSF3pt('mtag', 'genbase', '2017', 'tG3ptbinv2')
    # plotTopTaggerSF3pt('mtag', 'genbase', '2018', 'tG3ptbinv2')

    # plotFSSFfromTTbar('rtag', 'genbase', '2016', 'tGtptbinv1')
    # plotFSSFfromTTbar('rtag', 'genbase', '2017', 'tGtptbinv1')
    # plotFSSFfromTTbar('rtag', 'genbase', '2018', 'tGtptbinv1')

    # plotFSSFfromTTbar('mtag', 'genbase', '2016', 'tGv6fb', m1=850,  m2=100)
    # plotFSSFfromTTbar('mtag', 'genbase', '2017', 'tGv6fb', m1=850,  m2=100)
    # plotFSSFfromTTbar('mtag', 'genbase', '2017', 'tGv6fb', m1=1200, m2=100)
    # plotFSSFfromTTbar('mtag', 'genbase', '2016', 'tGv6fb', m1=650,  m2=350)
    # plotFSSFfromTTbar('mtag', 'genbase', '2017', 'tGv6fb', m1=650,  m2=350)
    # plotFSSFfromTTbar('mtag', 'genbase', '2018', 'tGv6fb', m1=650,  m2=350)

    # printRatiosFast()

    # makeFSSFplots('2016')
    # makeFSSFplots('2017')
    # makeFSSFplots('2018')
    # makeFSSFplots('2016', 'mtagSF_tGv4bin')
    # makeFSSFplots('2017', 'mtagSF_tGv4bin')
    # makeFSSFplots('2018', 'mtagSF_tGv4bin')
    # makeFSSFplots('2018', 'mtagSF_tGv4_ak8Mrwgtd')

    # makeFSSFplots('2016', 'mtagSF_tG3ptbinv2')
    # makeFSSFplots('2017', 'mtagSF_tG3ptbinv2')
    # makeFSSFplots('2018', 'mtagSF_tG3ptbinv2')

    # plotTopTaggerSF3pt('rtag', 'genbase', '2016', 'tG3ptbinv5')
    # plotTopTaggerSF3pt('rtag', 'genbase', '2017', 'tG3ptbinv5')
    # plotTopTaggerSF3pt('rtag', 'genbase', '2018', 'tG3ptbinv5')

    # makeFSSFplots('2016', 'rtagSF_tG3ptbinv4', 'rtag_genbase')
    # makeFSSFplots('2017', 'rtagSF_tG3ptbinv4', 'rtag_genbase')
    # makeFSSFplots('2018', 'rtagSF_tG3ptbinv4', 'rtag_genbase')

    # plotDoubleRatio()
    # plotDoubleRatio(suf='lsbptgt10')
    # plotDoubleRatio(suf='lsbptlt10')

    # plotTopTaggerMatchingEfficiency(1200, 100)
    # plotTopTaggerMatchingEfficiency(850,  100)
    # plotTopTaggerMatchingEfficiency(650,  350)

    # plotTopTaggerMatchingEfficiency(1200, 100, 'tagbase')
    # plotTopTaggerMatchingEfficiency(850,  100, 'tagbase')
    # plotTopTaggerMatchingEfficiency(650,  350, 'tagbase')
