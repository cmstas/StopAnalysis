#! /usr/bin/python
# coding: utf-8

import os
import sys
from math import sqrt
import ROOT as r
import numpy as np
from errors import E

# sys.path.insert(0,'/home/users/sicheng/tas/Software/dataMCplotMaker/')
# from dataMCplotMaker import dataMCplot

def getYieldsFromSRs(f, srNames, suf = ''):
    yield_org = []
    yield_wtc = []
    for sr in srNames:
        horg = f.Get(sr+'/h_metbins'+suf)
        hwtc = f.Get(sr+'/h_metbins'+suf+'_wtc')
        if not horg:
            yield_org.append(0)
        else:
            yield_org.append(float(horg.Integral()))
        if not hwtc:
            yield_wtc.append(0)
        else:
            yield_wtc.append(float(hwtc.Integral()))

    return yield_org, yield_wtc

def getYieldsForAllSRs(f, srNames, suf = ''):
    yield_org = []
    yield_wtc = []
    for sr in srNames:
        horg = f.Get(sr+'/h_metbins'+suf)
        hwtc = f.Get(sr+'/h_metbins'+suf+'_wtc')
        if not horg:
            print "Can't find", sr+'/h_metbins'+suf, "!!!"
            continue
        for ibin in range(1, horg.GetNbinsX()+1):
            yield_org.append(round(horg.GetBinContent(ibin), 2))
            if not hwtc:
                yield_wtc.append(0)
            else:
                yield_wtc.append(round(hwtc.GetBinContent(ibin), 2))

    return yield_org, yield_wtc

def getYErrsFromSRs(f, srNames, suf = ''):
    yerr_org = []
    yerr_wtc = []
    for sr in srNames:
        horg = f.Get(sr+'/h_metbins'+suf)
        hwtc = f.Get(sr+'/h_metbins'+suf+'_wtc')
        if not horg:
            yerr_org.append(0)
        else:
            yerr = r.Double()
            horg.IntegralAndError(0,-1,yerr)
            yerr_org.append(yerr)
        if not hwtc:
            yerr_wtc.append(0)
        else:
            yerr = r.Double()
            hwtc.IntegralAndError(0,-1,yerr)
            yerr_wtc.append(yerr)

    return yerr_org, yerr_wtc

def getYErrsForAllSRs(f, srNames, suf = ''):
    yerr_org = []
    yerr_wtc = []
    for sr in srNames:
        horg = f.Get(sr+'/h_metbins'+suf)
        hwtc = f.Get(sr+'/h_metbins'+suf+'_wtc')
        if not horg:
            print "Can't find", sr+'/h_metbins'+suf, "!!!"
            continue
        for ibin in range(1, horg.GetNbinsX()+1):
            yerr_org.append(round(horg.GetBinError(ibin), 2))
            if not hwtc:
                yerr_wtc.append(0)
            else:
                yerr_wtc.append(round(hwtc.GetBinError(ibin), 2))

    return yerr_org, yerr_wtc

def printAllSRYieldTable(srNames, yield_org, is_signal=False):

    print    ' | {:<9s}'.format(''),
    # for sr in srNames:
    #     print '| {:^8s}'.format(sr),
    print '|\n |  original',
    for i in range(len(yield_org)):
        if is_signal:
            print '|{:2.2f}'.format(yield_org[i]),
        else:
            print '|{:3.1f}'.format(yield_org[i]),
    print '|'

def printYieldComparisonTable(srNames, yield_org, yield_wtc):

    print    ' | {:<9s}'.format(''),
    for sr in srNames:
        print '| {:^8s}'.format(sr),
    print '|\n |  original',
    for i in range(len(srNames)):
        print '| {:8.1f}'.format(yield_org[i]),
    print '|\n | w/ toptag',
    for i in range(len(srNames)):
        print '| {:8.1f}'.format(yield_wtc[i]),
    print '|\n |     ratio',
    for i in range(len(srNames)):
        if yield_org[i] == 0:
            print '| {:^8s}'.format('--'),
        else:
            print '| {:7.1f}%'.format(100. * yield_wtc[i] / yield_org[i]),
    print '|\n |-----------------------------------------------------------------------------------------'

def printYieldAndErrTable2(srNames, yield_org, yield_wtc, yerr_org, yerr_wtc):
    print    ' | {:<16s}'.format(''),
    for sr in srNames:
        print '| {:^13s}'.format(sr),
    print '|\n |  sigsig original',
    for i in range(len(srNames)):
        print '| {:6.2f}'.format(yield_org[i]), '± {:<4.2f}'.format(yerr_org[i]),
    print '|\n | sigsig w/ toptag',
    for i in range(len(srNames)):
        print '| {:6.2f}'.format(yield_wtc[i]), '± {:<4.2f}'.format(yerr_wtc[i]),
    print '|\n |-----------------------------------------------------------------------------------------'

def printYieldAndErrComparisonTable(srNames, yield_org, yield_wtc, yerr_org, yerr_wtc):

    hden = r.TH1F("hden", "hden", len(yield_org), 0, len(yield_org)+1)
    hnum = r.TH1F("hnum", "hnum", len(yield_org), 0, len(yield_org)+1)

    print    ' | {:<16s}'.format(''),
    for sr in srNames:
        print '| {:^13s}'.format(sr),
    print '|\n |  yields original',
    for i in range(len(srNames)):
        print '| {:6.2f}'.format(yield_org[i]), '± {:<4.2f}'.format(yerr_org[i]),
        hden.SetBinContent(i+1, yield_org[i])
        hden.SetBinError(i+1, yerr_org[i])
    print '|\n | yields w/ toptag',
    for i in range(len(srNames)):
        print '| {:6.2f}'.format(yield_wtc[i]), '± {:<4.2f}'.format(yerr_wtc[i]),
        hnum.SetBinContent(i+1, yield_wtc[i])
        hnum.SetBinError(i+1, yerr_wtc[i])
    print '|\n |            ratio',
    heff = r.TEfficiency(hnum, hden)
    for i in range(len(srNames)):
        if yield_org[i] == 0:
            print '| {:^6s} ± {:^4s}'.format('--', '--'),
        else:
            # p = yield_wtc[i] / yield_org[i]
            # print '| {:5.2f}%'.format(100. * p), '± {:3.2f}%'.format(100. * sqrt(p * (1-p) / yield_org[i])),
            print '| {:5.1f}%'.format(100. * heff.GetEfficiency(i+1)), '± {:<3.1f}%'.format(100. * heff.GetEfficiencyErrorUp(i+1)),
    print '|\n |-----------------------------------------------------------------------------------------'

def printCutflowNumbers(f, sr, cfname):
    horg = f.Get(sr+'/h_'+cfname+'_org')
    hwtc = f.Get(sr+'/h_'+cfname+'_wtc')

    print    ' | {:<9s}'.format(cfname),
    for i in range(horg.GetNbinsX()):
        print '| {:^8s}'.format(horg.GetXaxis().GetBinLabel(i+1)),
    print '|\n |  original',
    for i in range(horg.GetNbinsX()):
        print '| {:8d}'.format(int(horg.GetBinContent(i+1))),
    print '|\n | w/ toptag',
    for i in range(horg.GetNbinsX()):
        print '| {:8d}'.format(int(hwtc.GetBinContent(i+1))),
    print '|'

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

if __name__ == '__main__':

    # os.system('mkdir -p plots')

    r.gROOT.SetBatch(1)

    f1 = r.TFile('../StopLooper/output/samples2016/data_2016.root')
    f2 = r.TFile('../StopLooper/output/samples2016/ttbar_powheg.root')
    f3 = r.TFile('../StopLooper/output/samples2016/Signal_T2tt_mStop_400to1200.root')
    f4 = r.TFile('../StopLooper/output/samples2016/ttbar_diLept.root')

    f1 = r.TFile('../StopLooper/output/temp8/all_mc_2016.root')

    f2 = r.TFile('../StopLooper/output/temp9/allBkg_25ns.root')
    f3 = r.TFile('../StopLooper/output/temp9/SMS_T2tt_mStop_400to1200.root')

    # printCutflowNumbers(f1, 'testCutflow', 'cutflow1')

    srNames = ['srbase', 'srA', 'srB', 'srC', 'srD', 'srE', 'srF', 'srG', 'srH', 'srI',]

    # print '\n |-------------------------------- data2016 ---------------------------------------------'
    # y1_org, y1_wtc = getYieldsFromSRs(f1, srNames)
    # ye1_org, ye1_wtc = getYErrsFromSRs(f1, srNames)
    # # printYieldComparisonTable(srNames, y1_org, y1_wtc)
    # printYieldAndErrComparisonTable(srNames, y1_org, y1_wtc, ye1_org, ye1_wtc)

    # print '\n |-------------------------------- 2016 data ---------------------------------------------'
    # srNames = ['srA', 'srB', 'srC', 'srD', 'srE', 'srF', 'srG', 'srH', 'srI',]
    # y1_org, y1_wtc = getYieldsForAllSRs(f1, srNames)
    # printAllSRYieldTable(srNames, y1_org)

    # print '\n |-------------------------------- CR0b ---------------------------------------------'
    # srNames = ['cr0bA', 'cr0bB', 'cr0bC', 'cr0bD', 'cr0bE', 'cr0bF', 'cr0bG', 'cr0bH', 'cr0bI',]
    # y1_org, y1_wtc = getYieldsForAllSRs(f1, srNames)
    # printAllSRYieldTable(srNames, y1_org)

    # print '\n |-------------------------------- CR2l ---------------------------------------------'
    # srNames = ['cr2lA', 'cr2lB', 'cr2lC', 'cr2lD', 'cr2lE', 'cr2lF', 'cr2lG', 'cr2lH', 'cr2lI',]
    # y1_org, y1_wtc = getYieldsForAllSRs(f1, srNames)
    # printAllSRYieldTable(srNames, y1_org)

    # print '\n |-------------------------------- 2016 ttbar_powheg ---------------------------------------------'
    # y2_org, y2_wtc = getYieldsForAllSRs(f2, srNames)
    # printAllSRYieldTable(srNames, y2_org)

    # print '\n |-------------------------------- 2016 ttbar_diLept ---------------------------------------------'
    # y4_org, y4_wtc = getYieldsForAllSRs(f4, srNames)
    # printAllSRYieldTable(srNames, y4_org)

    # print '\n |-------------------------------- 2016 T2tt, mStop 800, mLSP 600 ---------------------------------------------'
    # y3_org, y3_wtc = getYieldsForAllSRs(f3, srNames, '_800_600')
    # printAllSRYieldTable(srNames, y3_org, is_signal=True)

    # print '\n |-------------------------------- 2016 T2tt, mStop 900, mLSP 300 ---------------------------------------------'
    # y3_org, y3_wtc = getYieldsForAllSRs(f3, srNames, '_900_300')
    # printAllSRYieldTable(srNames, y3_org, is_signal=True)

    # print '\n |-------------------------------- 2016 T2tt, mStop 1200, mLSP 200 ---------------------------------------------'
    # y4_org, y4_wtc = getYieldsForAllSRs(f3, srNames, '_1200_200')
    # printAllSRYieldTable(srNames, y4_org, is_signal=True)

    # print '\n +-------------------------------------------------------------'

    srNames = ['srC', 'srD', 'srE', 'srF', 'srG', 'srH', 'srI',]

    # print '\n |----------------------------------- ttbar_powheg ----------------------------------------'
    # y2_org, y2_wtc = getYieldsFromSRs(f2, srNames)
    # ye2_org, ye2_wtc = getYErrsFromSRs(f2, srNames)
    # printYieldAndErrComparisonTable(srNames, y2_org, y2_wtc, ye2_org, ye2_wtc)

    # # print '\n |----------------------------------- TTJets_amcnlo ---------------------------------------'
    # # y4_org, y4_wtc = getYieldsFromSRs(f4, srNames)
    # # ye4_org, ye4_wtc = getYErrsFromSRs(f4, srNames)
    # # printYieldAndErrComparisonTable(srNames, y4_org, y4_wtc, ye4_org, ye4_wtc)

    print '\n |----------------------------------- allBkg_25ns ----------------------------------------'
    y2_org, y2_wtc = getYieldsFromSRs(f2, srNames)
    ye2_org, ye2_wtc = getYErrsFromSRs(f2, srNames)
    printYieldAndErrComparisonTable(srNames, y2_org, y2_wtc, ye2_org, ye2_wtc)

    # print '\n |-------------------------------- mStop 600, mLSP 450 ------------------------------------'
    # sy1_org, sy1_wtc = getYieldsFromSRs(f3, srNames, '_600_450')
    # ye1_org, ye1_wtc = getYErrsFromSRs(f3, srNames, '_600_450')
    # printYieldAndErrComparisonTable(srNames, sy1_org, sy1_wtc, ye1_org, ye1_wtc)

    print '\n |-------------------------------- mStop 800, mLSP 400 ------------------------------------'
    sy_org, sy_wtc = getYieldsFromSRs(f3, srNames, '_800_400')
    sye_org, sye_wtc = getYErrsFromSRs(f3, srNames, '_800_400')
    printYieldAndErrComparisonTable(srNames, sy_org, sy_wtc, sye_org, sye_wtc)
    quickStoBtable(y2_org, ye2_org, sy_org, sye_org, y2_wtc, ye2_wtc, sy_wtc, sye_wtc)

    print '\n |-------------------------------- mStop 800, mLSP 500 ------------------------------------'
    sy_org, sy_wtc = getYieldsFromSRs(f3, srNames, '_800_500')
    sye_org, sye_wtc = getYErrsFromSRs(f3, srNames, '_800_500')
    printYieldAndErrComparisonTable(srNames, sy_org, sy_wtc, sye_org, sye_wtc)
    quickStoBtable(y2_org, ye2_org, sy_org, sye_org, y2_wtc, ye2_wtc, sy_wtc, sye_wtc)

    print '\n |-------------------------------- mStop 800, mLSP 600 ------------------------------------'
    sy_org, sy_wtc = getYieldsFromSRs(f3, srNames, '_800_600')
    sye_org, sye_wtc = getYErrsFromSRs(f3, srNames, '_800_600')
    printYieldAndErrComparisonTable(srNames, sy_org, sy_wtc, sye_org, sye_wtc)
    quickStoBtable(y2_org, ye2_org, sy_org, sye_org, y2_wtc, ye2_wtc, sy_wtc, sye_wtc)

    print '\n |-------------------------------- mStop 900, mLSP 600 ------------------------------------'
    sy_org, sy_wtc = getYieldsFromSRs(f3, srNames, '_900_600')
    sye_org, sye_wtc = getYErrsFromSRs(f3, srNames, '_900_600')
    printYieldAndErrComparisonTable(srNames, sy_org, sy_wtc, sye_org, sye_wtc)
    quickStoBtable(y2_org, ye2_org, sy_org, sye_org, y2_wtc, ye2_wtc, sy_wtc, sye_wtc)

    # print '\n |-------------------------------- mStop 900, mLSP 700 ------------------------------------'
    # sy4_org, sy4_wtc = getYieldsFromSRs(f3, srNames, '_900_700')
    # sye4_org, sye4_wtc = getYErrsFromSRs(f3, srNames, '_900_700')
    # printYieldAndErrComparisonTable(srNames, sy4_org, sy4_wtc, sye4_org, sye4_wtc)

    # print '\n |-------------------------------- mStop 1200, mLSP 200 -----------------------------------'
    # sy5_org, sy5_wtc = getYieldsFromSRs(f3, srNames, '_1200_200')
    # sye5_org, sye5_wtc = getYErrsFromSRs(f3, srNames, '_1200_200')
    # printYieldAndErrComparisonTable(srNames, sy5_org, sy5_wtc, sye5_org, sye5_wtc)

    # print '\n |----------------------------------- TTJets_amcnlo ---------------------------------------'
    # y4_org, y4_wtc = getYieldsForAllSRs(f4, srNames)
    # printAllSRYieldTable(srNames, y4_org)

    # print '\n |------------------------------ TTJets_amcnlo bTagSF_HFUp --------------------------------'
    # y4_org, y4_wtc = getYieldsForAllSRs(f4, srNames, "_bTagEffHFUp")
    # printAllSRYieldTable(srNames, y4_org)

    # print '\n |------------------------------ TTJets_amcnlo bTagSF_HFDn --------------------------------'
    # y4_org, y4_wtc = getYieldsForAllSRs(f4, srNames, "_bTagEffHFDn")
    # printAllSRYieldTable(srNames, y4_org)

    # print '\n |------------------------------ TTJets_amcnlo bTagSF_LFUp --------------------------------'
    # y4_org, y4_wtc = getYieldsForAllSRs(f4, srNames, "_bTagEffLFUp")
    # printAllSRYieldTable(srNames, y4_org)

    # print '\n |------------------------------ TTJets_amcnlo bTagSF_LFDn --------------------------------'
    # y4_org, y4_wtc = getYieldsForAllSRs(f4, srNames, "_bTagEffLFDn")
    # printAllSRYieldTable(srNames, y4_org)


    # lprdir = '../StopLooper/output/temp8/'
    # files_todo = os.listdir(lprdir)
    # files_todo = filter(lambda x : x != "dummy.root", files_todo)
    # for fn in files_todo:
    #     flpr = r.TFile(lprdir + fn)
    #     print '|-------------------------------', fn, '--------------------------|'
    #     y1_org, y1_wtc = getYieldsForAllSRs(flpr, srNames)
    #     printAllSRYieldTable(srNames, y1_org)
    #     print ''
        
