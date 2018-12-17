#! /usr/bin/python
# coding: utf-8

import os
import sys
from math import sqrt
import ROOT as r
import numpy as np
from utilities.errors import *
from utilities.pytable import Table
from utilities.yields_utils import *


def printTableDataDriven(f, srNames, crname=''):
    if crname != '' and crname[-1] != ' ': crname += ' '

    tab = Table()
    preds  = getYieldEInTopoBins(f, srNames, 'metbins')
    alpha  = getYieldEInTopoBins(f, srNames, 'alphaHist')
    purity = getYieldEInTopoBins(f, srNames, 'CRpurity')
    yMC_SR = getYieldEInTopoBins(f, srNames, 'MCyields_SR')
    yMC_CR = getYieldEInTopoBins(f, srNames, 'MCyields_CR')
    yld_CR = getYieldEInTopoBins(f, srNames, 'datayields_CR')

    metrange = getBinningFromTopoSRs(f, srNames)
    tab.add_column('SR name', sum([[sr]*n for sr, n in zip(srNames, map(len, preds))], []))
    tab.add_column('MET [GeV]', [m[0]+' -- '+m[1] for m in sum(metrange, [])])
    tab.add_column(crname+'MC SR', [y.round(2) for y in sum(yMC_SR,[])])
    tab.add_column(crname+'MC CR', [y.round(2) for y in sum(yMC_CR,[])])
    tab.add_column(crname+'Purity', [y.round(2) for y in sum(purity,[])])
    tab.add_column(crname+'TF_{CR}^{SR}', [y.round(2).val for y in sum(alpha,[])])
    tab.add_column(crname+'data CR', [y.round(2) for y in sum(yld_CR,[])])
    tab.add_column(crname+'R_{CR}(data/MC)', [(d/m).round(2) for d, m in zip(sum(yld_CR,[]), sum(yMC_CR,[]))])
    tab.add_column(crname+'Prediction', [y.round(2) for y in sum(preds,[])])

    tab.print_table()

    return tab

def printTableWithMETextrpInfo(f1, f2, srNames, crname=''):
    if crname != '' and crname[-1] != ' ': crname += ' '

    tab = Table()
    preds1 = getYieldEInTopoBins(f1, srNames, 'metbins')
    alpha1 = getYieldEInTopoBins(f1, srNames, 'alphaHist')
    purity = getYieldEInTopoBins(f1, srNames, 'CRpurity')
    yMC_SR = getYieldEInTopoBins(f1, srNames, 'MCyields_SR')
    yMC_CR = getYieldEInTopoBins(f1, srNames, 'MCyields_CR')
    yld_CR = getYieldEInTopoBins(f1, srNames, 'datayields_CR')

    yMCaCR = getYieldEInTopoBins(f2, srNames, 'MCyields_CR')
    yldaCR = getYieldEInTopoBins(f2, srNames, 'datayields_CR')
    preds2 = getYieldEInTopoBins(f2, srNames, 'metbins')
    alpha2 = getYieldEInTopoBins(f2, srNames, 'alphaHist')

    metrange = getBinningFromTopoSRs(f1, srNames)

    tab.add_column('SR name', sum([[sr]*n for sr, n in zip(srNames, map(len, preds1))], []))
    tab.add_column('MET range [GeV]', [m[0]+' -- '+m[1] for m in sum(metrange, [])])
    tab.add_column(crname+'MC SR', [y.round(2) for y in sum(yMC_SR,[])])
    tab.add_column(crname+'MC CR raw', [y.round(2) for y in sum(yMC_CR,[])])
    tab.add_column(crname+'Purity', [y.round(2).val for y in sum(purity,[])])
    tab.add_column(crname+'TF_{CR}^{SR}', [y.round(2) for y in sum(alpha1,[])])
    tab.add_column(crname+'data CR raw', [y.round(2) for y in sum(yld_CR,[])])
    tab.add_column(crname+'CR data/MC raw', [(d/m).round(2) for d, m in zip(sum(yld_CR,[]), sum(yMC_CR,[]))])
    tab.add_column(crname+'Pred (no extrp)', [y.round(2) for y in sum(preds1,[])])

    tab.add_column(crname+'MC CR comb', [y.round(2) for y in sum(yMCaCR,[])])
    tab.add_column(crname+'data CR comb', [y.round(2) for y in sum(yldaCR,[])])
    tab.add_column(crname+'CR data/MC comb', [(d/m).round(2) for d, m in zip(sum(yldaCR,[]), sum(yMCaCR,[]))])
    tab.add_column(crname+'Pred (MET extrp)', [y.round(2) for y in sum(preds2,[])])

    tab.print_table()

    return tab


def makeBkgEstimateTablesLostLepton():

    # f1 = r.TFile('../StopLooper/output/samp17_v13_tf/lostlepton.root')
    # f2 = r.TFile('../StopLooper/output/samp17_v13_tf_v1/lostlepton_wMETextrp.root')
    f1 = r.TFile('../StopLooper/output/comb1617_tf/lostlepton.root')

    srNames = ['srA0', 'srA1', 'srA2', 'srB0', 'srC0','srD0', 'srE0', 'srE1', 'srE2', 'srE3', 'srF0', 'srG0', 'srG1', 'srG2', 'srG3', 'srH0',]

    print '\n ----------------------------------- lost lepton w/o MET extrapolation ---------------------------------------'
    tab1 = printTableDataDriven(f1, srNames)
    tab1.set_theme_latex()
    tab1.print_pdf('lostlep1617_allSRs.pdf')

    # print '\n ----------------------------------- lost lepton w/ MET extrapolation ----------------------------------------'
    # tab2 = printTableDataDriven(f2, srNames)
    # tab2.set_theme_latex()
    # tab2.print_pdf('lostlep_wMETextrp.pdf')

    # print '\n ----------------------------------- lost lepton w/ MET extrapolation ----------------------------------------'
    # tab3 = printTableWithMETextrpInfo(f1, f2, srNames)
    # tab3.set_theme_latex()
    # tab3.print_pdf('lostlep_METextrpInfo.pdf')

    print '\n ----------------------------------- lost lepton separate by top tags ----------------------------------------'

    srNames = ['srA0', 'srB0', 'srC0','srD0', 'srE0', 'srF0', 'srG0', 'srH0',]
    tab0 = printTableDataDriven(f1, srNames)
    tab0.set_theme_latex()
    tab0.print_pdf('lostlep1617_inclTopTags.pdf')

    srNames = ['srA1', 'srE1', 'srG1',]
    tab1 = printTableDataDriven(f1, srNames)
    tab1.set_theme_latex()
    tab1.print_pdf('lostlep1617_noTopTags.pdf')

    srNames = ['srA2', 'srE2', 'srG2',]
    tab2 = printTableDataDriven(f1, srNames)
    tab2.set_theme_latex()
    tab2.print_pdf('lostlep1617_mergeTags.pdf')

    srNames = ['srE3', 'srG3',]
    tab3 = printTableDataDriven(f1, srNames)
    tab3.set_theme_latex()
    tab3.print_pdf('lostlep1617_tfresTags.pdf')

    # f3 = r.TFile('../StopLooper/output/samp16_v14_tf_80X/lostlepton.root')

    # print '\n ----------------------------------- lost lepton separate by top tags ----------------------------------------'

    # srNames = ['srA0', 'srB0', 'srC0','srD0', 'srE0', 'srF0', 'srG0', 'srH0',]
    # tab0 = printTableDataDriven(f3, srNames)
    # tab0.set_theme_latex()
    # tab0.print_pdf('lostlep16_inclTopTags.pdf')

    # srNames = ['srA1', 'srE1', 'srG1',]
    # tab1 = printTableDataDriven(f3, srNames)
    # tab1.set_theme_latex()
    # tab1.print_pdf('lostlep16_noTopTags.pdf')

    # srNames = ['srA2', 'srE2', 'srG2',]
    # tab2 = printTableDataDriven(f3, srNames)
    # tab2.set_theme_latex()
    # tab2.print_pdf('lostlep16_mergeTags.pdf')

    # srNames = ['srE3', 'srG3',]
    # tab3 = printTableDataDriven(f3, srNames)
    # tab3.set_theme_latex()
    # tab3.print_pdf('lostlep16_tfresTags.pdf')



if __name__ == '__main__':

    # os.system('mkdir -p plots')

    r.gROOT.SetBatch(1)

    makeBkgEstimateTablesLostLepton()

    
