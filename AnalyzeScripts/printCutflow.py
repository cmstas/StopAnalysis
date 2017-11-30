import os
import sys
import ROOT as r
import numpy as np

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
            print '| {:7.2f}%'.format(100. * yield_wtc[i] / yield_org[i]),
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

if __name__ == '__main__':

    # os.system('mkdir -p plots')

    r.gROOT.SetBatch(1)

    f1 = r.TFile('../StopLooper/output/temp/TTJets.root')
    f2 = r.TFile('../StopLooper/output/temp/SMS_T2tt_mStop-400to1200.root')
    # f2 = r.TFile('../StopLooper/output/temp_v26_1/T2tt_btag20.root')
    f4 = r.TFile('../StopLooper/output/temp_v26_1/TTJets.root')
    f2 = r.TFile('../StopLooper/output/temp_v26_1/ttbar.root')
    f3 = r.TFile('../StopLooper/output/temp_v26_1/SMS_T2tt_mStop-400to1200.root')
    # f1 = r.TFile('../StopLooper/output/samples2016/ttbar.root')
    f1 = r.TFile('../StopLooper/output/samples2016/data_2016.root')
    # f2 = r.TFile('../StopLooper/output/samples2016/ttbar.root')
    # f3 = r.TFile('../StopLooper/output/samples2016/Signal_T2tt_mStop_400to1200.root')

    # printCutflowNumbers(f1, 'testCutflow', 'cutflow1')
    # printCutflowNumbers(f1, 'testCutflow', 'cutflow2')
    # printCutflowNumbers(f1, 'testCutflow', 'cutflow3')

    # printCutflowNumbers(f2, 'testCutflow', 'cutflow1')
    # printCutflowNumbers(f2, 'testCutflow', 'cutflow2')
    # printCutflowNumbers(f2, 'testCutflow', 'cutflow3')


    # srNames = ['srbase', 'srA', 'srB', 'srC', 'srD', 'srE', 'srF', 'srG', 'srH', 'srI',]
    # # print '\n +-------------------------------- ttbar ---------------------------------------------'
    # print '\n |-------------------------------- data2016 ---------------------------------------------'
    # y1_org, y1_wtc = getYieldsFromSRs(f1, srNames)
    # printYieldComparisonTable(srNames, y1_org, y1_wtc)

    print '\n |-------------------------------- data2016 ---------------------------------------------'
    srNames = ['srA', 'srB', 'srC', 'srD', 'srE', 'srF', 'srG', 'srH', 'srI',]
    y1_org, y1_wtc = getYieldsForAllSRs(f1, srNames)
    printAllSRYieldTable(srNames, y1_org)

    print '\n |-------------------------------- ttbar2016 ---------------------------------------------'
    y2_org, y2_wtc = getYieldsForAllSRs(f2, srNames)
    printAllSRYieldTable(srNames, y2_org)

    # print '\n |-------------------------------- T2tt2016, mStop 800, mLSP 600 ---------------------------------------------'
    # y3_org, y3_wtc = getYieldsForAllSRs(f3, srNames, '_800_600')
    # printAllSRYieldTable(srNames, y3_org, is_signal=True)

    print '\n |-------------------------------- T2tt2016, mStop 900, mLSP 300 ---------------------------------------------'
    y3_org, y3_wtc = getYieldsForAllSRs(f3, srNames, '_900_300')
    printAllSRYieldTable(srNames, y3_org, is_signal=True)

    print '\n |-------------------------------- T2tt2016, mStop 1200, mLSP 200 ---------------------------------------------'
    y4_org, y4_wtc = getYieldsForAllSRs(f3, srNames, '_1200_200')
    printAllSRYieldTable(srNames, y4_org, is_signal=True)

    # print '\n +-------------------------------------------------------------'

    srNames = ['srC', 'srD', 'srE', 'srF', 'srG', 'srH', 'srI',]

    print '\n |----------------------------------- ttbar_powheg ----------------------------------------'
    y2_org, y2_wtc = getYieldsFromSRs(f2, srNames)
    printYieldComparisonTable(srNames, y2_org, y2_wtc)

    print '\n |----------------------------------- TTJets_amcnlo ---------------------------------------'
    y4_org, y4_wtc = getYieldsFromSRs(f4, srNames)
    printYieldComparisonTable(srNames, y4_org, y4_wtc)

    print '\n |-------------------------------- mStop 600, mLSP 450 ------------------------------------'
    sy1_org, sy1_wtc = getYieldsFromSRs(f3, srNames, '_600_450')
    printYieldComparisonTable(srNames, sy1_org, sy1_wtc)

    print '\n |-------------------------------- mStop 800, mLSP 200 ------------------------------------'
    sy2_org, sy2_wtc = getYieldsFromSRs(f3, srNames, '_800_200')
    printYieldComparisonTable(srNames, sy2_org, sy2_wtc)

    print '\n |-------------------------------- mStop 800, mLSP 600 ------------------------------------'
    sy3_org, sy3_wtc = getYieldsFromSRs(f3, srNames, '_800_600')
    printYieldComparisonTable(srNames, sy3_org, sy3_wtc)

    print '\n |-------------------------------- mStop 900, mLSP 300 ------------------------------------'
    sy4_org, sy4_wtc = getYieldsFromSRs(f3, srNames, '_900_300')
    printYieldComparisonTable(srNames, sy4_org, sy4_wtc)

    print '\n |-------------------------------- mStop 1200, mLSP 200 -----------------------------------'
    sy5_org, sy5_wtc = getYieldsFromSRs(f3, srNames, '_1200_200')
    printYieldComparisonTable(srNames, sy5_org, sy5_wtc)


    # srNames = ['srC1', 'srC2', 'srC3', 'srC4', 'srC5', 'srD1', 'srD2', 'srD3', 'srD4', 'srE', 'srF', 'srG', 'srH', 'srI',]
    # print '\n |-------------------------------- mStop 900, mLSP 300 ---------------------------------------------'
    # sy4_org, sy4_wtc = getYieldsForAllSRs(f3, srNames, '_900_300')
    # printYieldComparisonTable(srNames, sy4_org, sy4_wtc)

