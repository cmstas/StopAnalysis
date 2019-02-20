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
    tab.add_column(crname+'TF_{CR}^{SR}', [y.round(2) for y in sum(alpha,[])])
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


def makeTFComparisonTable():
    f1 = r.TFile('../StopLooper/output/samp16_v30_s3/lostlepton.root')
    f2 = r.TFile('../StopLooper/output/samp16_v30_ISRoff/lostlepton.root')

    f1 = r.TFile('../StopLooper/output/samp17_v30_ISRon/lostlepton.root')
    f2 = r.TFile('../StopLooper/output/samp17_v30_s3/lostlepton.root')

    f1 = r.TFile('lostlepton16.root')
    f2 = r.TFile('lostlepton17.root')

    suf1 = '16'
    suf2 = '17'

    srNames = ['srA0', 'srA1', 'srA2', 'srB', 'srC','srD', 'srE0', 'srE1', 'srE2', 'srE3', 'srF', 'srG0', 'srG1', 'srG2', 'srG3', 'srH',]

    tab = Table()
    preds1  = getYieldEInTopoBins(f1, srNames, 'metbins')
    alpha1  = getYieldEInTopoBins(f1, srNames, 'alphaHist')
    purity1 = getYieldEInTopoBins(f1, srNames, 'CRpurity')
    yMC_SR1 = getYieldEInTopoBins(f1, srNames, 'MCyields_SR')
    yMC_CR1 = getYieldEInTopoBins(f1, srNames, 'MCyields_CR')
    yld_CR1 = getYieldEInTopoBins(f1, srNames, 'datayields_CR')

    preds2  = getYieldEInTopoBins(f2, srNames, 'metbins')
    alpha2  = getYieldEInTopoBins(f2, srNames, 'alphaHist')
    purity2 = getYieldEInTopoBins(f2, srNames, 'CRpurity')
    yMC_SR2 = getYieldEInTopoBins(f2, srNames, 'MCyields_SR')
    yMC_CR2 = getYieldEInTopoBins(f2, srNames, 'MCyields_CR')
    yld_CR2 = getYieldEInTopoBins(f2, srNames, 'datayields_CR')

    metrange = getBinningFromTopoSRs(f1, srNames)
    tab.add_column('SR name', sum([[sr]*n for sr, n in zip(srNames, map(len, preds1))], []))
    tab.add_column('MET [GeV]', [m[0]+' -- '+m[1] for m in sum(metrange, [])])
    tab.add_column('MC SR '+suf1, [y.round(2) for y in sum(yMC_SR1,[])])
    tab.add_column('MC CR '+suf1, [y.round(2) for y in sum(yMC_CR1,[])])
    tab.add_column('MC SR '+suf2, [y.round(2) for y in sum(yMC_SR2,[])])
    tab.add_column('MC CR '+suf2, [y.round(2) for y in sum(yMC_CR2,[])])
    tab.add_column('TF_{CR}^{SR} '+suf1, [y.round(2) for y in sum(alpha1,[])])
    tab.add_column('TF_{CR}^{SR} '+suf2, [y.round(2) for y in sum(alpha2,[])])
    tab.add_column('Diff(TF_{{0}}/TF_{{1}})'.format(suf1,suf2), [((y1-y2)/y1).round(2).pct_rep() if y1.val > 0 else 0 for y1, y2 in zip(sum(alpha1,[]), sum(alpha2,[]))])

    tab.add_column('data CR '+suf1, [y.round(2) for y in sum(yld_CR1,[])])
    tab.add_column('data CR '+suf2, [y.round(2) for y in sum(yld_CR2,[])])
    # tab.add_column('R_{CR}(data/MC)', [(d/m).round(2) for d, m in zip(sum(yld_CR,[]), sum(yMC_CR,[]))])
    tab.add_column('Pred '+suf1, [y.round(2) for y in sum(preds1,[])])
    tab.add_column('Pred '+suf1+' \\times 42/36', [(y*(42.0/35.9)).round(2) for y in sum(preds1,[])])
    tab.add_column('Pred '+suf2, [y.round(2) for y in sum(preds2,[])])
    tab.add_column('Diff(Pred_{{0}}/Pred_{{1}})'.format(suf1,suf2), [((y1*(42.0/35.9)-y2)/y2).round(2).pct_rep() if y2.val > 0 else 0 for y1, y2 in zip(sum(preds1,[]), sum(preds2,[]))])

    tab.print_table()
    tab.set_theme_latex()
    tab.print_pdf('compareTF_lostlep_16vs17_more.pdf')


def makeBkgEstimateTablesLostLepton():

    # f1 = r.TFile('../StopLooper/output/samp17_v13_tf/lostlepton.root')
    # f2 = r.TFile('../StopLooper/output/samp17_v13_tf_v1/lostlepton_wMETextrp.root')
    # f1 = r.TFile('../StopLooper/output/comb1617_tf/lostlepton.root')
    f1 = r.TFile('../StopLooper/output/combRun2_v30_s3/lostlepton.root')

    # srNames = ['srA0', 'srA1', 'srA2', 'srB0', 'srC0','srD0', 'srE0', 'srE1', 'srE2', 'srE3', 'srF0', 'srG0', 'srG1', 'srG2', 'srG3', 'srH0',]
    srNames = ['srA0', 'srA1', 'srA2', 'srB', 'srC','srD', 'srE0', 'srE1', 'srE2', 'srE3', 'srF', 'srG0', 'srG1', 'srG2', 'srG3', 'srH',]

    print '\n ----------------------------------- lost lepton w/o MET extrapolation ---------------------------------------'
    tab1 = printTableDataDriven(f1, srNames)
    tab1.set_theme_latex()
    tab1.print_pdf('lostlepRun2_allSRs.pdf')

    # print '\n ----------------------------------- lost lepton w/ MET extrapolation ----------------------------------------'
    # tab2 = printTableDataDriven(f2, srNames)
    # tab2.set_theme_latex()
    # tab2.print_pdf('lostlep_wMETextrp.pdf')

    # print '\n ----------------------------------- lost lepton w/ MET extrapolation ----------------------------------------'
    # tab3 = printTableWithMETextrpInfo(f1, f2, srNames)
    # tab3.set_theme_latex()
    # tab3.print_pdf('lostlep_METextrpInfo.pdf')

    print '\n ----------------------------------- lost lepton separate by top tags ----------------------------------------'

    srNames = ['srA0', 'srB', 'srC','srD', 'srE0', 'srF', 'srG0', 'srH',]
    tab0 = printTableDataDriven(f1, srNames)
    tab0.set_theme_latex()
    tab0.print_pdf('lostlepRun2_inclTopTags.pdf')

    srNames = ['srA1', 'srE1', 'srG1',]
    tab1 = printTableDataDriven(f1, srNames)
    tab1.set_theme_latex()
    tab1.print_pdf('lostlepRun2_noTopTags.pdf')

    srNames = ['srA2', 'srE2', 'srG2',]
    tab2 = printTableDataDriven(f1, srNames)
    tab2.set_theme_latex()
    tab2.print_pdf('lostlepRun2_mergeTags.pdf')

    srNames = ['srE3', 'srG3',]
    tab3 = printTableDataDriven(f1, srNames)
    tab3.set_theme_latex()
    tab3.print_pdf('lostlepRun2_tfresTags.pdf')

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


def makeBkgCompositionTable():
    f1 = r.TFile('../StopLooper/output/combRun2_v30_s3/allBkg_run2.root')

    srNames = ['srA0', 'srA1', 'srA2', 'srB', 'srC','srD', 'srE0', 'srE1', 'srE2', 'srE3', 'srF', 'srG0', 'srG1', 'srG2', 'srG3', 'srH',]
    bkgs = ['2lep', '1lepW', '1lepTop', 'Znunu']

    preds_tot = getYieldEInTopoBins(f1, srNames, 'metbins')
    metrange = getBinningFromTopoSRs(f1, srNames)

    tab = Table()
    tab.add_column('SR name', sum([[sr]*n for sr, n in zip(srNames, map(len, preds_tot))], []))
    tab.add_column('MET [GeV]', [m[0]+' -- '+m[1] for m in sum(metrange, [])])
    for bkg in bkgs:
        preds_bkg = getYieldEInTopoBins(f1, srNames, 'metbins_'+bkg)
        tab.add_column(bkg, [(n/d).pct_rep() for n,d in zip(sum(preds_bkg,[]),sum(preds_tot,[]))])

    tab.print_table()
    tab.set_theme_latex()
    tab.print_pdf('bkgCompositionFraction_run2.pdf')

    tab2 = Table()
    tab2.add_column('SR name', sum([[sr]*n for sr, n in zip(srNames, map(len, preds_tot))], []))
    tab2.add_column('MET [GeV]', [m[0]+' -- '+m[1] for m in sum(metrange, [])])
    for bkg in bkgs:
        preds_bkg = getYieldEInTopoBins(f1, srNames, 'metbins_'+bkg)
        tab2.add_column(bkg, [y.round(2) for y in sum(preds_bkg,[])])
    tab2.add_column('Total', [y.round(2) for y in sum(preds_tot,[])])
    tab2.print_table()
    tab2.set_theme_latex()
    tab2.print_pdf('bkgCompositionYields_run2.pdf')


def makeBkgEstimationTableWJets():
    f1 = r.TFile('../StopLooper/output/combRun2_v30_s3/allBkg_run2.root')

    srNames = ['srA0', 'srA1', 'srA2', 'srB', 'srC','srD', 'srE0', 'srE1', 'srE2', 'srE3', 'srF', 'srG0', 'srG1', 'srG2', 'srG3', 'srH',]
    crNames = [sr.replace('sr','cr0b') for sr in srNames]

    metrange = getBinningFromTopoSRs(f1, srNames)
    yMC_SR_1lepW = getYieldEInTopoBins(f1, srNames, 'metbins_1lepW')
    yMC_CR_1lepW = getYieldEInTopoBins(f1, crNames, 'metbins_1lepW')
    yMC_SR_total = getYieldEInTopoBins(f1, srNames, 'metbins')
    yMC_CR_total = getYieldEInTopoBins(f1, crNames, 'metbins')

    tab = Table()
    tab.add_column('SR name', sum([[sr]*n for sr, n in zip(srNames, map(len, yMC_CR_total))], []))
    tab.add_column('MET [GeV]', [m[0]+' -- '+m[1] for m in sum(metrange, [])])
    tab.add_column('1lepW SR', [y.round(2) for y in sum(yMC_SR_1lepW,[])])
    tab.add_column('1lepW / allbkg', [(n/d).pct_rep() for n,d in zip(sum(yMC_SR_1lepW,[]),sum(yMC_SR_total,[]))])
    tab.add_column('CR Exp total', [y.round(2) for y in sum(yMC_CR_total,[])])
    tab.add_column('Transfer Factor', [(s/c).round(2) for s, c in zip(sum(yMC_SR_1lepW,[]),sum(yMC_CR_total,[]))])
    tab.add_column('CR Purity', [(n/d).pct_rep() for n,d in zip(sum(yMC_CR_1lepW,[]),sum(yMC_CR_total,[]))])

    tab.print_table()
    tab.set_theme_latex()
    tab.print_pdf('WJetsEstimation_pre1_run2.pdf')

if __name__ == '__main__':

    # os.system('mkdir -p plots')

    r.gROOT.SetBatch(1)

    # makeBkgEstimateTablesLostLepton()

    # makeBkgCompositionTable()

    # makeBkgEstimationTableWJets()

    makeTFComparisonTable()
