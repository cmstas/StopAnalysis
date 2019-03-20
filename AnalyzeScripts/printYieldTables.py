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
    # f1 = r.TFile('../StopLooper/output/samp16_v30_s3/lostlepton.root')
    # f2 = r.TFile('../StopLooper/output/samp16_v30_ISRoff/lostlepton.root')

    # f1 = r.TFile('../StopLooper/output/samp17_v30_ISRon/lostlepton.root')
    # f2 = r.TFile('../StopLooper/output/samp17_v30_s3/lostlepton.root')

    # f1 = r.TFile('lostlepton16.root')
    # f2 = r.TFile('lostlepton_17.root')

    # f1 = r.TFile('../StopLooper/output/samp17_v37_m4/1lepFromW_17.root')
    # f2 = r.TFile('../StopLooper/output/samp17_v30_m4/1lepFromW_17.root')

    # f1 = r.TFile('../StopLooper/output/samp17_v39_mrs0/lostlepton_17.root')
    # f2 = r.TFile('../StopLooper/output/samp17_v39_mrs2/lostlepton_17.root')

    f1 = r.TFile('../StopLooper/output/samp17_v39_mrs0/1lepFromW_17.root')
    f2 = r.TFile('../StopLooper/output/samp17_v39_mrs2/1lepFromW_17.root')

    # f1 = r.TFile('../StopLooper/output/samp16_v30_s3/lostlepton.root')
    # f2 = r.TFile('../StopLooper/output/samp16_v30_genttscaled/lostlepton.root')

    suf1 = 'mrsOff'
    suf2 = 'mrsOn'

    # srNames = ['srA0', 'srA1', 'srA2', 'srB', 'srC','srD', 'srE0', 'srE1', 'srE2', 'srE3', 'srF', 'srG0', 'srG1', 'srG2', 'srG3', 'srH', 'srI']
    srNames = ['srA0', 'srA1', 'srA2', 'srB', 'srC', 'srD', 'srE0', 'srF', 'srG0', 'srH', 'srI']

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
    tab.add_column('MC SR '+suf1, [(1*y).round(2) for y in sum(yMC_SR1,[])])
    tab.add_column('MC SR '+suf2, [(1*y).round(2) for y in sum(yMC_SR2,[])])
    tab.add_column('MC CR '+suf1, [(1*y).round(2) for y in sum(yMC_CR1,[])])
    tab.add_column('MC CR '+suf2, [(1*y).round(2) for y in sum(yMC_CR2,[])])
    tab.add_column('TF_{CR}^{SR} '+suf1, [(1*y).round(3) for y in sum(alpha1,[])])
    tab.add_column('TF_{CR}^{SR} '+suf2, [(1*y).round(3) for y in sum(alpha2,[])])
    tab.add_column('Diff(TF_{{{0}}}/TF_{{{1}}})'.format(suf1,suf2), [((y1-y2)/y1).round(2).pct_rep() if y1.val > 0 else 0 for y1, y2 in zip(sum(alpha1,[]), sum(alpha2,[]))])

    tab.add_column('data CR ', [(1*y).round(2) for y in sum(yld_CR1,[])])
    # tab.add_column('data CR '+suf1, [(1*y).round(2) for y in sum(yld_CR1,[])])
    # tab.add_column('data CR '+suf2, [(1*y).round(2) for y in sum(yld_CR2,[])])
    # tab.add_column('R_{CR}(data/MC) '+suf1, [(d/m).round(2) for d, m in zip(sum(yld_CR1,[]), sum(yMC_CR1,[]))])
    # tab.add_column('R_{CR}(data/MC) '+suf2, [(d/m).round(2) for d, m in zip(sum(yld_CR2,[]), sum(yMC_CR2,[]))])
    # tab.add_column('Pred '+suf1, [(1*y).round(2) for y in sum(preds1,[])])
    # tab.add_column('Pred '+suf1+' \\times 42/36', [(y*(42.0/35.9)).round(2) for y in sum(preds1,[])])
    # tab.add_column('Pred '+suf2, [(1*y).round(2) for y in sum(preds2,[])])
    # tab.add_column('Diff(Pred_{{0}}/Pred_{{1}})'.format(suf1,suf2), [((y1*(42.0/35.9)-y2)/y2).round(2).pct_rep() if y2.val > 0 else 0 for y1, y2 in zip(sum(preds1,[]), sum(preds2,[]))])

    tab.print_table()
    tab.set_theme_latex()
    tab.print_pdf('compareCR0b_MRSOffvsOn.pdf')


def makeBkgEstimateTablesLostLepton(indir):

    # f1 = r.TFile('../StopLooper/output/samp17_v13_tf/lostlepton.root')
    # f2 = r.TFile('../StopLooper/output/samp17_v13_tf_v1/lostlepton_wMETextrp.root')
    # f1 = r.TFile('../StopLooper/output/comb1617_tf/lostlepton.root')
    # f1 = r.TFile('../StopLooper/output/combRun2_v37_c5/lostlepton_run2.root')
    f1 = r.TFile(indir+'/lostlepton_run2.root')

    srNames = ['srA0', 'srA1', 'srA2', 'srB', 'srC','srD', 'srE0', 'srE1', 'srE2', 'srE3', 'srF', 'srG0', 'srG1', 'srG2', 'srG3', 'srH',]

    # print '\n ----------------------------------- lost lepton w/o MET extrapolation ---------------------------------------'
    # tab1 = printTableDataDriven(f1, srNames)
    # tab1.set_theme_latex()
    # tab1.print_pdf('lostlepRun2_allSRs.pdf')

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

    print '\n ----------------------------------- lost lepton for corridor ----------------------------------------'
    srNames = ['srI',]
    tab4 = printTableDataDriven(f1, srNames)
    tab4.set_theme_latex()
    tab4.print_pdf('lostlepRun2_corridor5.pdf')


def makeBkgEstimateTables1LepFromW(indir):
    # f1 = r.TFile('../StopLooper/output/combRun2_v37_s5/1lepFromW_run2.root')
    f1 = r.TFile(indir+'/1lepFromW_run2.root')

    srNames = ['srA0', 'srA1', 'srA2', 'srB', 'srC','srD', 'srE0', 'srE1', 'srE2', 'srE3', 'srF', 'srG0', 'srG1', 'srG2', 'srG3', 'srH',]

    print '\n ----------------------------------- W+jets w/o MET extrapolation ---------------------------------------'
    tab1 = printTableDataDriven(f1, srNames)
    tab1.set_theme_latex()
    tab1.print_pdf('1lepWRun2_allSRs.pdf')

    print '\n ----------------------------------- W+jets separate by top tags ----------------------------------------'

    srNames = ['srA0', 'srB', 'srC','srD', 'srE0', 'srF', 'srG0', 'srH',]
    tab0 = printTableDataDriven(f1, srNames)
    tab0.set_theme_latex()
    tab0.print_pdf('1lepWRun2_inclTopTags.pdf')

    srNames = ['srA1', 'srE1', 'srG1',]
    tab1 = printTableDataDriven(f1, srNames)
    tab1.set_theme_latex()
    tab1.print_pdf('1lepWRun2_noTopTags.pdf')

    srNames = ['srA2', 'srE2', 'srG2',]
    tab2 = printTableDataDriven(f1, srNames)
    tab2.set_theme_latex()
    tab2.print_pdf('1lepWRun2_mergeTags.pdf')

    srNames = ['srE3', 'srG3',]
    tab3 = printTableDataDriven(f1, srNames)
    tab3.set_theme_latex()
    tab3.print_pdf('1lepWRun2_tfresTags.pdf')

    print '\n ----------------------------------- W+jets separate by Inclusive CR ----------------------------------------'

    srNames = ['srA1', 'srA2',]
    tab1 = printTableDataDriven(f1, srNames)
    tab1.set_theme_latex()
    tab1.print_pdf('1lepWRun2_srA.pdf')

    srNames = ['srE1', 'srE2', 'srE3',]
    tab2 = printTableDataDriven(f1, srNames)
    tab2.set_theme_latex()
    tab2.print_pdf('1lepWRun2_srE.pdf')

    srNames = ['srG1', 'srG2', 'srG3',]
    tab3 = printTableDataDriven(f1, srNames)
    tab3.set_theme_latex()
    tab3.print_pdf('1lepWRun2_srG.pdf')

    print '\n ----------------------------------- W+jets for corridor ----------------------------------------'
    srNames = ['srI',]
    tab4 = printTableDataDriven(f1, srNames)
    tab4.set_theme_latex()
    tab4.print_pdf('1lepWRun2_corridor.pdf')


def makeMETExtrInfoTable():
    srNames = ['srA0', 'srA1', 'srA2', 'srB', 'srC','srD', 'srE0', 'srE1', 'srE2', 'srE3', 'srF', 'srG0', 'srG1', 'srG2', 'srG3', 'srH', ]
    # srNames = ['srA0', 'srA1', 'srB', 'srC', ]

    f1 = r.TFile('../StopLooper/output/combRun2_v39_s2/lostlepton_run2.root')
    f2 = r.TFile('./lostlepton_run2.root')

    alpha1  = getYieldEInTopoBins(f1, srNames, 'alphaHist')
    alpha2  = getYieldEInTopoBins(f2, srNames, 'alphaHist')
    yMC_CR1 = getYieldEInTopoBins(f1, srNames, 'MCyields_CR')
    yMC_CR2 = getYieldEInTopoBins(f2, srNames, 'MCyields_CR')
    yld_CR1 = getYieldEInTopoBins(f1, srNames, 'datayields_CR')
    yld_CR2 = getYieldEInTopoBins(f2, srNames, 'datayields_CR')

    xrtsrs, metbin = zip(*[ (sr, [ i for i, y in enumerate(y2) if y not in y1]) for sr, y1, y2 in zip(srNames, alpha1, alpha2) if y1[-1] != y2[-1] ])
    alpha3  = [ [ y for i, y in enumerate(y2) if y not in y1] for sr, y1, y2 in zip(srNames, alpha1, alpha2) if y1[-1] != y2[-1] ]
    yMC_CR3 = [ [ y for i, y in enumerate(y2) if y not in y1] for sr, y1, y2 in zip(srNames, yMC_CR1, yMC_CR2) if y1[-1] != y2[-1] ]
    yld_CR3 = [ [ y for i, y in enumerate(y2) if y not in y1] for sr, y1, y2 in zip(srNames, yld_CR1, yld_CR2) if y1[-1] != y2[-1] ]


    metrange = getBinningFromTopoSRs(f1, xrtsrs)
    metrange = [ [rng for i, rng in enumerate(rngs) if i in ibins ] for rngs, ibins in zip(metrange, metbin)]

    tab = Table()
    tab.add_column('SR name', sum([[sr]*n for sr, n in zip(xrtsrs, map(len, alpha3))], []))
    # tab.add_column('metbin', sum(metbin, []))
    tab.add_column('MET [GeV]', [m[0]+' -- '+m[1] for m in sum(metrange, [])])
    tab.add_column('M^{CR} Expected', [y.round(3) for y in sum(yMC_CR3, [])])
    # print len(sum(yMC_CR3, [])), len(sum(yld_CR3, []))
    # tab.add_column('N^{CR} Observed', [y.round(3) for y in sum(yld_CR3, [])])
    tab.add_column('Transfer Factor', [y.round(3) for y in sum(alpha3, [])])

    # tab.print_table()
    tab.set_theme_latex()
    tab.print_table()
    tab.print_pdf('lostlep_METextrBins.pdf')


def makeBkgCompositionTable(f1, srNames, savename='run2'):

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
    tab.print_pdf('bkgCompositionFraction_'+savename+'.pdf')

    tab2 = Table()
    tab2.add_column('SR name', sum([[sr]*n for sr, n in zip(srNames, map(len, preds_tot))], []))
    tab2.add_column('MET [GeV]', [m[0]+' -- '+m[1] for m in sum(metrange, [])])
    for bkg in bkgs:
        preds_bkg = getYieldEInTopoBins(f1, srNames, 'metbins_'+bkg)
        tab2.add_column(bkg, [y.round(2) for y in sum(preds_bkg,[])])
    tab2.add_column('Total', [y.round(2) for y in sum(preds_tot,[])])
    tab2.print_table()
    tab2.set_theme_latex()
    tab2.print_pdf('bkgCompositionYields_'+savename+'.pdf')


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


def makeSystUncertaintyMaxTable(indir, srNames, bkgType='lostlep'):

    if bkgType == 'lostlep':
        f1 = r.TFile(indir+'/lostlepton_run2.root')
    elif bkgType == '1lepW':
        f1 = r.TFile(indir+'/1lepFromW_run2.root')

    preds = sum(getYieldEInTopoBins(f1, srNames, 'metbins'), [])

    tab = Table()

    systs = ['jes', 'bTagEffHF', 'bTagEffLF', 'lepSF', 'alphas', 'q2', 'tauSF', 'pdf', 'pileup', 'cr2lTriggerSF',]
    for sys in systs:
        y_sysUp = sum(getYieldEInTopoBins(f1, srNames, 'metbins_'+sys+'Up'), [])
        y_sysDn = sum(getYieldEInTopoBins(f1, srNames, 'metbins_'+sys+'Dn'), [])
        maxErrUp = [ 0 if cen.val < 0.0001 else round(abs(2*(sup.val-cen.val)/(sup.val+cen.val)), 3) for sup, cen in zip(y_sysUp, preds) ]
        maxErrDn = [ 0 if cen.val < 0.0001 else round(abs(2*(sdn.val-cen.val)/(sdn.val+cen.val)), 3) for sdn, cen in zip(y_sysDn, preds) ]
        maxErr = [ max(meu, med) for meu, med in zip(maxErrUp, maxErrDn) ]

        print sys, round(100*min(maxErr),1), '-', round(100*max(maxErr), 1)
        # print sys, maxErrUp
        # print sys, sum(maxErr, 0) / len(maxErr)


if __name__ == '__main__':

    # os.system('mkdir -p plots')

    r.gROOT.SetBatch(1)

    f1 = r.TFile('../StopLooper/output/combRun2_v39_m1/allBkg_run2.root')
    srNames = ['srA0', 'srA1', 'srA2', 'srB', 'srC','srD', 'srE0', 'srE1', 'srE2', 'srE3', 'srF', 'srG0', 'srG1', 'srG2', 'srG3', 'srH', 'srI']

    # makeBkgCompositionTable(f1, srNames,  'run2_all')
    # makeBkgCompositionTable(f1, ['srI',], 'run2_cor')

    indir = '../StopLooper/output/combRun2_v39_s2'

    # makeBkgEstimateTablesLostLepton(indir)
    # makeBkgEstimateTables1LepFromW(indir)

    # f1 = r.TFile('../StopLooper/output/combRun2_v37_c5/allBkg_run2.root')
    # makeBkgCompositionTable(f1, ['srI',], 'run2_cor5')

    srNames = ['srA0', 'srA1', 'srA2', 'srB', 'srC','srD', 'srE0', 'srE1', 'srE2', 'srE3', 'srF', 'srG0', 'srG1', 'srG2', 'srG3', 'srH']
    # makeSystUncertaintyMaxTable(indir, srNames)

    # makeBkgEstimationTableWJets()

    makeTFComparisonTable()

    # makeMETExtrInfoTable()
