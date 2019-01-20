#! /usr/bin/python
# coding: utf-8

import os
import sys
from math import sqrt
import ROOT as r
import numpy as np
from errors import *
from utilities.errors import *
from utilities.pytable import Table

r.gROOT.SetBatch(1)
# sys.path.insert(0,'/home/users/sicheng/tas/Software/dataMCplotMaker/')
# from dataMCplotMaker import dataMCplot

def getYieldsFromTopoSRs(f, srNames, suf = ''):
    yields = []
    for sr in srNames:
        hist = f.Get(sr+'/h_metbins'+suf)
        if not hist:
            yields.append(0)
        else:
            yields.append(float(hist.Integral()))

    return yields

def getYieldAndErrsFromTopoSRs(f, srNames, suf = ''):
    yields = []
    yerrs = []
    for sr in srNames:
        hist = f.Get(sr+'/h_metbins'+suf)
        if not hist:
            yields.append(0)
            yerrs.append(0)
        else:
            yerr = r.Double()
            yields.append(float(hist.IntegralAndError(0,-1,yerr)))
            yerrs.append(float(yerr))

    return yields, yerrs

def getYieldEInTopoBins(f, srNames, suf = ''):
    yields = []
    for sr in srNames:
        hist = f.Get(sr+'/h_metbins'+suf)
        if not hist:
            print "Can't find", sr+'/h_metbins'+suf, "!!!"
            hist = f.Get(sr+'/h_metbins')
            if not hist: print "This should not happen!"
            yields.append([E(0,0)]*hist.GetNbinsX())
            continue
        topoylds = []
        for ibin in range(1, hist.GetNbinsX()+1):
            topoylds.append(E(hist.GetBinContent(ibin), hist.GetBinError(ibin)))

        yields.append(topoylds)

    return yields

def getPercentage(num, den, binom=False):
    rat = [100*get_efficiency(n, d) for n, d in zip(num, den)]
    rat = [r.round(1) for r in rat]
    rat = ["%s%% %s %s%%" % (str(r.val), u"\u00B1".encode("utf-8"), str(r.err)) for r in rat]
    return rat

def StoBErr(s, b, se, be):
    v = (s+2*b)**2 * se**2 + s**2 * be**2
    v /= 4*(s+b)**3
    return sqrt(v)

def quickStoBtable(y_org, ye_org, sy_org, sye_org, y_wtc, ye_wtc, sy_wtc, sye_wtc):
    stob_org = [s/sqrt(s+b) for s,b in zip(sy_org, y_org)]
    stobe_org = [StoBErr(*x) for x in zip(sy_org, y_org, sye_org, sye_org)]
    stob_wtc = [s/sqrt(s+b) for s,b in zip(sy_wtc, y_wtc)]
    stobe_wtc = [StoBErr(*x) for x in zip(sy_wtc, y_wtc, sye_wtc, sye_wtc)]
    printYieldAndErrTable2(srNames, stob_org, stob_wtc, stobe_org, stobe_wtc)

def printCRemuYields(year='16'):
    indir = '../StopLooper/output/samp'+year+'_v30_cremu/'
    # samples = ['ttbar', 'singleT', 'Vjets', 'rare', 'allBkg', 'allData']
    samples = ['tt2l', 'tt1l', 'singleT', 'Wjets', 'DYJets', 'rare', 'allBkg', 'allData']
    ysuf = '_'+year
    srNames = ['cremuA0', 'cremuA1', 'cremuA2',]

    tab = Table()
    tab.set_column_names(['srName']+srNames)
    f = dict()
    yldEs = dict()
    for samp in samples:
        
        filestr = indir+samp+ysuf+'.root'
        if not os.path.isfile(filestr): filestr = indir+samp+'.root'
        f[samp] = r.TFile(filestr)
        if not f[samp]: print '!!!'
        ylds, errs = getYieldAndErrsFromTopoSRs(f[samp], srNames)
        yldEs[samp] = [E(y,e) for y, e in zip(ylds, errs)]
        if samp == 'allData': tab.add_line()
        tab.add_row([samp] + [ye.round(2) for ye in yldEs[samp]])
   
    tab.add_row(['Data/MC'] + [(yd/yb).round(3) for yd, yb in zip(yldEs['allData'], yldEs['allBkg'])])

    tab.print_table()
    tab.set_theme_latex()
    tab.print_pdf('CRemuYields_'+year+'_Jan20.pdf')


if __name__ == '__main__':

    printCRemuYields('16')
    printCRemuYields('17')
    exit()

    f1 = r.TFile('../StopLooper/output/temp14/allBkg_25ns.root')
    f2 = r.TFile('../StopLooper/output/temp14/Signal_T2tt.root')
    f3 = r.TFile('../StopLooper/output/newbin0/allBkg_25ns.root')
    f4 = r.TFile('../StopLooper/output/newbin0/SMS_T2tt.root')

    srNames = ['srA', 'srB', 'srC', 'srD', 'srE', 'srF', 'srG', 'srH', 'srI',]

    bkgs = ['2lep', '1lepW', '1lepTop', 'Znunu']
    tab = Table()
    tab.set_column_names(['srName']+srNames)
    for bg in bkgs:
        ylds, errs = getYieldAndErrsFromTopoSRs(f1, srNames, '_'+bg)
        tab.add_row([bg] + [E(y,e).round(2) for y, e in zip(ylds, errs)])
    tab.add_line()
    bgylds, bgerrs = getYieldAndErrsFromTopoSRs(f1, srNames)
    bylds_org = [E(y,e).round(2) for y, e in zip(bgylds, bgerrs)]
    tab.add_row(['Total background'] + bylds_org)
    tab.add_line()
    sigylds, sigerrs = getYieldAndErrsFromTopoSRs(f2, srNames, '_1200_50')
    tab.add_row(['T2tt_1200_50'] + [E(y,e).round(2) for y, e in zip(sigylds, sigerrs)])

    # tab.print_table()

    srNames  = ['srA', 'srB', 'srC', 'srD', 'srE', 'srF', 'srG', 'srH']
    ntcNames = ['srA1', 'srB1', 'srC1', 'srD1', 'srE1', 'srF1', 'srG1', 'srH1',]
    wdtNames = ['srA2', 'srB2', 'srC2', 'srD2', 'srE2', 'srF2', 'srG2', 'srH2',]
    wrtNames = ['srA3', 'srB3', 'srC3', 'srD3', 'srE3', 'srF3', 'srG3', 'srH3',]

    # srNames = ['cr0bA', 'cr0bB', 'cr0bC', 'cr0bD', 'cr0bE', 'cr0bF', 'cr0bG', 'cr0bH',]
    # wdtNames = ['cr0bA_wdt', 'cr0bB_wdt', 'cr0bC_wdt', 'cr0bD_wdt', 'cr0bE_wdt', 'cr0bF_wdt', 'cr0bG_wdt', 'cr0bH_wdt',]
    # wrtNames = ['cr0bA_wrt', 'cr0bB_wrt', 'cr0bC_wrt', 'cr0bD_wrt', 'cr0bE_wrt', 'cr0bF_wrt', 'cr0bG_wrt', 'cr0bH_wrt',]
    # ntcNames = ['cr0bA_ndt', 'cr0bB_ndt', 'cr0bC_ntt', 'cr0bD_ntt', 'cr0bE_ntt', 'cr0bF_ntt', 'cr0bG_ntt', 'cr0bH_ntt',]

    scale = 1

    bkgs = ['2lep', '1lepW', '1lepTop', 'Znunu']
    tab = Table()
    tab.set_column_names(['srName']+srNames)
    for bg in bkgs:
        ylds, errs = getYieldAndErrsFromTopoSRs(f3, wdtNames, '_'+bg)
        tab.add_row([bg+' mtag bin'] + [E(y,e).round(2) * scale for y, e in zip(ylds, errs)])
    tab.add_line()
    for bg in bkgs:
        ylds, errs = getYieldAndErrsFromTopoSRs(f3, wrtNames, '_'+bg)
        tab.add_row([bg+' rtag bin'] + [E(y,e).round(2) * scale for y, e in zip(ylds, errs)])
    tab.add_line()
    for bg in bkgs:
        ylds, errs = getYieldAndErrsFromTopoSRs(f3, ntcNames, '_'+bg)
        tab.add_row([bg+' notag bin'] + [E(y,e).round(2) * scale for y, e in zip(ylds, errs)])
    tab.add_line()
    bgylds, bgerrs = getYieldAndErrsFromTopoSRs(f1, srNames)
    bylds_org = [E(y,e).round(2) * scale for y, e in zip(bgylds, bgerrs)]
    tab.add_row(['Total background'] + bylds_org)
    # wdtylds, wdterrs = getYieldAndErrsFromTopoSRs(f3, wdtNames)
    # bylds_wdt = [E(y,e).round(2) * scale for y, e in zip(wdtylds, wdterrs)]
    # tab.add_row(['Total bkg mtag'] + bylds_wdt)
    # beff = getPercentage(bylds_wdt, bylds_org)
    # tab.add_row(['mtag efficiency'] + beff)
    # tab.add_line()

    # for sigpt in ['_1200_50', '_800_400']:
    #     # ylds, errs = getYieldAndErrsFromTopoSRs(f4, wdtNames, sigpt)
    #     # tab.add_row(['T2tt'+sigpt+' wtb'] + [E(y,e).round(2) for y, e in zip(ylds, errs)])
    #     # ylds, errs = getYieldAndErrsFromTopoSRs(f4, ntcNames, sigpt)
    #     # tab.add_row(['T2tt'+sigpt+' ntb'] + [E(y,e).round(2) for y, e in zip(ylds, errs)])

    #     sigylds, sigerrs = getYieldAndErrsFromTopoSRs(f2, srNames, sigpt)
    #     sylds_org = [E(y,e).round(2) * scale for y, e in zip(sigylds, sigerrs)]
    #     tab.add_row(['T2tt'+sigpt+' org'] + sylds_org)
    #     wdtylds, wdterrs = getYieldAndErrsFromTopoSRs(f4, wdtNames, sigpt)
    #     sylds_wdt = [E(y,e).round(2) * scale for y, e in zip(wdtylds, wdterrs)]
    #     tab.add_row(['T2tt'+sigpt+' mtag'] + sylds_wdt)
    #     seff = getPercentage(sylds_wdt, sylds_org)
    #     tab.add_row(['T2tt'+sigpt+' eff'] + seff)

    #     # tab.add_line()
    #     ssb_org = [get_StoSB(s,b).round(2) for s,b in zip(sylds_org, bylds_org)]
    #     tab.add_row(['S/sqrt(S+B) org'] + ssb_org)
    #     ssb_wdt = [get_StoSB(s,b).round(2) for s,b in zip(sylds_wdt, bylds_wdt)]
    #     tab.add_row(['S/sqrt(S+B) mtag'] + ssb_wdt)

    #     tab.add_line()

    tab.print_table()

    # ssb_org = [get_StoSB(s,b).round(2) for s,b in zip(sylds_org, bylds_org)]
    # tab2.add_row(['SSB org'] + ssb_org)
    # ssb_wdt = [get_StoSB(s,b).round(2) for s,b in zip(sylds_wdt, bylds_wdt)]
    # tab2.add_row(['SSB wdt'] + ssb_wdt)

    # tab2.print_table()


    # f5 = r.TFile('../StopLooper/output/temp_org/allBkg_25ns.root')
    # f6 = r.TFile('../StopLooper/output/temp_org/SMS_T2tt.root')

    # f7 = r.TFile('../StopLooper/output/rtagbinned/allBkg_25ns.root')
    # f8 = r.TFile('../StopLooper/output/rtagbinned/SMS_T2tt.root')

    nttNames = ['srA1', 'srB1',]
    wdtNames = ['srA2', 'srB2',]

    # nttNames = ['srC1', 'srD1',]
    # wdtNames = ['srC2', 'srD2',]
    # wrtNames = ['srC3', 'srD3',]

    # nttNames = ['srE1', 'srF1', 'srG1', 'srH1',]
    # wdtNames = ['srE2', 'srF2', 'srG2', 'srH2',]
    # wrtNames = ['srE3', 'srF3', 'srG3', 'srH3',]

    # nttNames = ['srG1', 'srH1',]
    # wdtNames = ['srG2', 'srH2',]
    # wrtNames = ['srG3', 'srH3',]

    # nttNames = [sr.replace('sr','cr2l') for sr in nttNames]
    # wdtNames = [sr.replace('sr','cr2l') for sr in wdtNames]
    # wrtNames = [sr.replace('sr','cr2l') for sr in wrtNames]

    # nttNames = [sr.replace('sr','cr0b') for sr in nttNames]
    # wdtNames = [sr.replace('sr','cr0b') for sr in wdtNames]
    # wrtNames = [sr.replace('sr','cr0b') for sr in wrtNames]

    srNames = [sr.split('_')[0] for sr in wdtNames]

    tab2 = Table()

    colnames = []

    # bylds = getYieldEInTopoBins(f5, srNames)
    # sylds = getYieldEInTopoBins(f6, srNames, '_1200_50')

    bylds_org = getYieldEInTopoBins(f1, srNames)

    bylds_wdt = getYieldEInTopoBins(f3, wdtNames)
    bylds_wrt = getYieldEInTopoBins(f3, wrtNames)
    bylds_ntt = getYieldEInTopoBins(f3, nttNames)

    for sr, n in zip(wdtNames, map(len, bylds_wdt)):
        colnames += [sr.split('_')[0] + str(i) for i in range(1, n+1)]
    tab2.set_column_names(['srName'] + colnames)

    for bg in bkgs:
        bylds = getYieldEInTopoBins(f3, wdtNames, '_'+bg)
        tab2.add_row([bg+' mtag bin'] + [y.round(2) for y in sum(bylds,[])])
    tab2.add_line()

    for bg in bkgs:
        bylds = getYieldEInTopoBins(f3, wrtNames, '_'+bg)
        tab2.add_row([bg+' rtag bin'] + [y.round(2) for y in sum(bylds,[])])
    tab2.add_line()

    for bg in bkgs:
        bylds = getYieldEInTopoBins(f3, nttNames, '_'+bg)
        tab2.add_row([bg+' notag bin'] + [y.round(2) for y in sum(bylds,[])])
    tab2.add_line()

    # tab2.add_row(['mertag bkg'] + [y.round(2) for y in sum(bylds_wdt,[])])
    # tab2.add_row(['restag bkg'] + [y.round(2) for y in sum(bylds_wrt,[])])
    # tab2.add_row(['no tag bkg'] + [y.round(2) for y in sum(bylds_ntt,[])])

    tab2.add_row(['Total bkg'] + [y.round(2) for y in sum(bylds_org,[])])

    tab2.print_table()

    # tab3 = Table()

    # tab3.add_row(['tmod bkg'] + [y.round(2) for y in sum(bylds,[])])
    # tab3.add_row(['tmod sig'] + [y.round(2) for y in sum(sylds,[])])
    # tab3.add_row(['tmod SSB'] + [get_StoSB(s,b).round(2) for s,b in zip(sum(sylds,[]), sum(bylds,[]))])
    # tab3.add_line()

    # bylds = getYieldEInTopoBins(f7, srNames)
    # sylds = getYieldEInTopoBins(f8, srNames, '_1200_50')

    # tab3.add_row(['rtag bkg'] + [y.round(2) for y in sum(bylds,[])])
    # tab3.add_row(['rtag sig'] + [y.round(2) for y in sum(sylds,[])])
    # tab3.add_row(['rtag SSB'] + [get_StoSB(s,b).round(2) for s,b in zip(sum(sylds,[]), sum(bylds,[]))])

    # tab3.print_table()

    # tab3.add_row(['mertag bkg'] + [y.round(2) for y in sum(bylds_wdt,[])])
    # tab3.add_row(['mertag sig'] + [y.round(2) for y in sum(sylds_wdt,[])])
    # tab3.add_line()
    # tab3.add_row(['restag bkg'] + [y.round(2) for y in sum(bylds_wrt,[])])
    # tab3.add_row(['restag sig'] + [y.round(2) for y in sum(sylds_wrt,[])])
    # tab3.add_line()
    # tab3.add_row(['2 tags bkg'] + [y.round(2) for y in sum(bylds_rdt,[])])
    # tab3.add_row(['2 tags sig'] + [y.round(2) for y in sum(sylds_rdt,[])])

    # colnames = [sr.split('_')[0] for sr in wdtNames]
    # tab3.set_column_names(['srName'] + colnames)
    # for bn, names in zip(['no tag', 'mer tag', 'res tag', '2 tags'], [ntcNames, wdtNames, wrtNames, rdtNames]):
    #     bylds = [E(y,e).round(2) * scale for y, e in zip(*getYieldAndErrsFromTopoSRs(f5, names))]
    #     sylds = [E(y,e).round(2) * scale for y, e in zip(*getYieldAndErrsFromTopoSRs(f6, names, '_1200_50'))]
    #     tab3.add_row(['bkg '+bn] + bylds)
    #     tab3.add_row(['sig '+bn] + sylds)

    #     tab3.add_line()

    # tab3.print_table()
