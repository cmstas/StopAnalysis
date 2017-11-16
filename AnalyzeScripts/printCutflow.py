import os
import sys
import ROOT as r
import numpy as np

# sys.path.insert(0,'/home/users/sicheng/tas/Software/dataMCplotMaker/')
# from dataMCplotMaker import dataMCplot

def getYieldsFromSRs(f, srNames):
    yield_org = []
    yield_wtc = []
    for sr in srNames:
        horg = f.Get(sr+'/h_metbins')
        hwtc = f.Get(sr+'/h_metbins_wtc')
        yield_org.append(int(horg.Integral()))
        if not hwtc:
            yield_wtc.append(0)
        else:
            yield_wtc.append(int(hwtc.Integral()))

    return yield_org, yield_wtc

def printYieldComparisonTable(srNames, yield_org, yield_wtc):
    
    print    ' | {:<9s}'.format(''),
    for sr in srNames:
        print '| {:^8s}'.format(sr),
    print '|\n |  original',
    for i in range(len(srNames)):
        print '| {:8d}'.format(yield_org[i]),
    print '|\n | w/ toptag',
    for i in range(len(srNames)):
        print '| {:8d}'.format(yield_wtc[i]),
    print '|\n |     ratio',
    for i in range(len(srNames)):
        if yield_org[i] == 0:
            print '| {:^8s}'.format('--'),
        else:
            print '| {:7.2f}%'.format(100. * yield_wtc[i] / yield_org[i]),
    print '|'

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
    f2 = r.TFile('../StopLooper/output/temp/W4Jets.root')

    # printCutflowNumbers(f1, 'testCutflow', 'cutflow1')
    # printCutflowNumbers(f1, 'testCutflow', 'cutflow2')
    # printCutflowNumbers(f1, 'testCutflow', 'cutflow3')


    srNames = ['srbase', 'srA', 'srB', 'srC', 'srD', 'srE', 'srF', 'srG', 'srH', 'srI',]
    y1_org, y1_wtc = getYieldsFromSRs(f1, srNames)
    printYieldComparisonTable(srNames, y1_org, y1_wtc)

    # y2_org, y2_wtc = getYieldsFromSRs(f2, srNames)

    
