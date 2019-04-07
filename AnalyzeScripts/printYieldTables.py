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

def printDataDrivenTable(f, srNames, crname='', systs=None, doAsymErr=True):
    if crname != '' and crname[-1] != ' ': crname += ' '

    tab = Table()
    preds  = getYieldEInFlatBins(f, srNames, 'metbins')
    alpha  = getYieldEInFlatBins(f, srNames, 'alphaHist')
    yMC_SR = getYieldEInFlatBins(f, srNames, 'MCyields_SR')
    yMC_CR = getYieldEInFlatBins(f, srNames, 'MCyields_CR')
    yld_CR = getYieldEInFlatBins(f, srNames, 'datayields_CR')
    purity = getYieldEInTopoBins(f, srNames, 'CRpurity')

    # errMC = [ 0 if m.val <= 0.01 else round(m.err/m.val,4) for m in getYieldEInFlatBins(f, srNames, 'metbins_MCStats', 'metbins')]
    systup = [a.err for a in alpha]
    systdn = systup

    if type(systs) == list:
        for sys in systs:
            predup = getYieldEInFlatBins(f, srNames, 'metbins_'+sys+'Up', 'metbins')
            preddn = getYieldEInFlatBins(f, srNames, 'metbins_'+sys+'Dn', 'metbins')
            sysup = [ 0 if ycen.val <= 0 else round(yup.val/ycen.val-1, 4) for yup, ycen in zip(predup, preds)]
            sysdn = [ 0 if ycen.val <= 0 else round(ydn.val/ycen.val-1, 4) for ydn, ycen in zip(preddn, preds)]
            for i in range(len(preds)):
                sup = sysup[i]
                sdn = sysdn[i]
                if sysup[i] < 0 and sysdn[i] > 0:
                    sdn = sysup[i]
                    sup = sysdn[i]
                elif not (sysup[i] > 0 and sysdn[i] < 0):
                    print 'Syst:', sys, 'at bin', i, 'has single sided variation! sysup =', sysup[i], 'sysdn =', sysdn[i]
                    if np.isnan(sup): sup = 0
                    if np.isnan(sdn): sdn = 0
                    sup = max(abs(sup), abs(sdn))
                    sdn = sup

                systup[i] = sqrt(systup[i]**2 + sup**2)
                systdn[i] = sqrt(systdn[i]**2 + sdn**2)

    metrange = getBinningFromTopoSRs(f, srNames)
    tab.add_column('SR name', sum([[sr]*n for sr, n in zip(srNames, map(len, purity))], []))
    tab.add_column('MET [GeV]', [m[0]+' -- '+m[1] for m in sum(metrange, [])])
    tab.add_column(crname+'MC SR', [y.round(2) for y in yMC_SR])
    tab.add_column(crname+'MC CR', [y.round(2) for y in yMC_CR])
    tab.add_column(crname+'Purity', [y.round(2) for y in sum(purity, [])])
    if doAsymErr:
        pderr = [ getPoissonErrors(y.val) for y in yld_CR]
        preds = [ y.round(2).val for y in preds]
        alpha = [ a.round(2).val for a in alpha]
        # staterr = [ (sqrt(d[0]**2 + m**2), sqrt(d[1]**2 + m**2)) for d, m in zip(errdat, errMC)]
        tab.add_column(crname+'TF$_{CR}^{SR}$', [ '${0}^{{+{1}}}_{{-{2}}}$'.format(a, round(a*(1+sup),2), round(a*(1-sdn),2)) for a, sup, sdn in zip(alpha, systup, systdn)])
        tab.add_column(crname+'Data CR', [ '${0}^{{+{1}}}_{{-{2}}}$'.format(int(y.val), round(e[0],2), round(e[1],2)) for y, e in zip(yld_CR, pderr)]) 
        # tab.add_column(crname+'R$_{CR}$(data/MC)', [ '({})%'.format((d/m*100).round(1)) for d, m in zip(yld_CR, yMC_CR)])
        tab.add_column(crname+'R$_{CR}$(data/MC)', [(d/m).round(2) for d, m in zip(yld_CR, yMC_CR)])
        tab.add_column(crname+'Prediction', [ '${0}^{{+{1}+{3}}}_{{-{2}-{4}}}$'.format(y, round(pde[0]*a,2), round(pde[1]*a,2), round(sup*y,2), round(sdn*y,2)) for y,pde,a,sup,sdn in zip(preds, pderr, alpha, systup, systdn)])
    else:
        tab.add_column(crname+'TF$_{CR}^{SR}$', [ E(a.val, a.val*max(sup,sdn)).round(2) for a, sup, sdn in zip(alpha, systup, systdn)])
        tab.add_column(crname+'data CR', [y.round(2) for y in yld_CR])
        tab.add_column(crname+'R$_{CR}$(data/MC)', [(d/m).round(2) for d, m in zip(yld_CR, yMC_CR)])
        tab.add_column(crname+'Prediction', [y.round(2) for y in preds])

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


def makeBkgEstimateTablesLostLepton(indir, ysuf='run2'):

    # f1 = r.TFile('../StopLooper/output/samp17_v13_tf/lostlepton.root')
    # f2 = r.TFile('../StopLooper/output/samp17_v13_tf_v1/lostlepton_wMETextrp.root')
    # f1 = r.TFile('../StopLooper/output/comb1617_tf/lostlepton.root')
    # f1 = r.TFile('../StopLooper/output/combRun2_v37_c5/lostlepton_run2.root')
    f1 = r.TFile(indir+'/lostlepton_'+ysuf+'.root')

    srNames = ['srA0', 'srA1', 'srA2', 'srB', 'srC','srD', 'srE0', 'srE1', 'srE2', 'srE3', 'srF', 'srG0', 'srG1', 'srG2', 'srG3', 'srH', 'srI', 'srJ']

    print '\n ----------------------------------- lost lepton w/ MET extrapolation ----------------------------------------'
    # tab1 = printTableDataDriven(f1, srNames)
    syst = ['jes',]
    syst = ['jes', 'ISR', 'lepSF', 'metTTbar', 'L1prefire', 'pileup', 'pdf', 'q2', 'alphas', 'metRes', 'tauSF']
    tab1 = printDataDrivenTable(f1, srNames, systs=syst)
    tab1.set_theme_latex()
    tab1.print_pdf('lostlepRun2_allSRs.pdf')

    # print '\n ----------------------------------- lost lepton w/o MET extrapolation ---------------------------------------'
    # tab2 = printTableDataDriven(f2, srNames)
    # tab2.set_theme_latex()
    # tab2.print_pdf('lostlep_wMETextrp.pdf')

    # print '\n ----------------------------------- lost lepton w/ MET extrapolation ----------------------------------------'
    # tab3 = printTableWithMETextrpInfo(f1, f2, srNames)
    # tab3.set_theme_latex()
    # tab3.print_pdf('lostlep_METextrpInfo.pdf')

    # print '\n ----------------------------------- lost lepton separate by top tags ----------------------------------------'

    # srNames = ['srA0', 'srB', 'srC','srD', 'srE0', 'srF', 'srG0', 'srH',]
    # tab0 = printTableDataDriven(f1, srNames)
    # tab0.set_theme_latex()
    # tab0.print_pdf('lostlepRun2_inclTopTags.pdf')

    # srNames = ['srA1', 'srE1', 'srG1',]
    # tab1 = printTableDataDriven(f1, srNames)
    # tab1.set_theme_latex()
    # tab1.print_pdf('lostlepRun2_noTopTags.pdf')

    # srNames = ['srA2', 'srE2', 'srG2',]
    # tab2 = printTableDataDriven(f1, srNames)
    # tab2.set_theme_latex()
    # tab2.print_pdf('lostlepRun2_mergeTags.pdf')

    # srNames = ['srE3', 'srG3',]
    # tab3 = printTableDataDriven(f1, srNames)
    # tab3.set_theme_latex()
    # tab3.print_pdf('lostlepRun2_tfresTags.pdf')

    # print '\n ----------------------------------- lost lepton for corridor ----------------------------------------'
    # srNames = ['srI','srJ']
    # tab4 = printTableDataDriven(f1, srNames)
    # tab4.set_theme_latex()
    # tab4.print_pdf('lostlepRun2_corridor.pdf')


def makeBkgEstimateTables1LepFromW(indir):
    # f1 = r.TFile('../StopLooper/output/combRun2_v37_s5/1lepFromW_run2.root')
    f1 = r.TFile(indir+'/1lepFromW_run2.root')

    # srNames = ['srA0', 'srA1', 'srA2', 'srB', 'srC','srD', 'srE0', 'srE1', 'srE2', 'srE3', 'srF', 'srG0', 'srG1', 'srG2', 'srG3', 'srH',]
    srNames = ['srA0', 'srA1', 'srB', 'srC','srD', 'srE0', 'srE1', 'srF', 'srG0', 'srG1', 'srH', 'srI', 'srJ']

    print '\n ----------------------------------- W+jets all ---------------------------------------'
    # tab1 = printTableDataDriven(f1, srNames)
    syst = ['jes', 'bTagEffHF', 'bTagEffLF', 'WbXsec', 'L1prefire', 'pileup', 'pdf', 'q2', 'alphas', 'metRes']
    tab1 = printDataDrivenTable(f1, srNames, systs=syst)
    tab1.set_theme_latex()
    tab1.print_pdf('1lepWRun2_allDDSRs.pdf')

    print '\n ----------------------------------- W+jets separate by top tags ----------------------------------------'

    # srNames = ['srA0', 'srB', 'srC','srD', 'srE0', 'srF', 'srG0', 'srH',]
    # tab0 = printTableDataDriven(f1, srNames)
    # tab0.set_theme_latex()
    # tab0.print_pdf('1lepWRun2_inclTopTags.pdf')

    # srNames = ['srA1', 'srE1', 'srG1',]
    # tab1 = printTableDataDriven(f1, srNames)
    # tab1.set_theme_latex()
    # tab1.print_pdf('1lepWRun2_noTopTags.pdf')

    srNames = ['srA2', 'srE2', 'srG2',]
    # tab2 = printTableDataDriven(f1, srNames)
    # tab2.set_theme_latex()
    # tab2.print_pdf('1lepWRun2_mergeTags.pdf')

    srNames = ['srE3', 'srG3',]
    # tab3 = printTableDataDriven(f1, srNames)
    # tab3.set_theme_latex()
    # tab3.print_pdf('1lepWRun2_tfresTags.pdf')

    print '\n ----------------------------------- W+jets separate by Inclusive CR ----------------------------------------'

    # srNames = ['srA1', 'srA2',]
    # tab1 = printTableDataDriven(f1, srNames)
    # tab1.set_theme_latex()
    # tab1.print_pdf('1lepWRun2_srA.pdf')

    # srNames = ['srE1', 'srE2', 'srE3',]
    # tab2 = printTableDataDriven(f1, srNames)
    # tab2.set_theme_latex()
    # tab2.print_pdf('1lepWRun2_srE.pdf')

    # srNames = ['srG1', 'srG2', 'srG3',]
    # tab3 = printTableDataDriven(f1, srNames)
    # tab3.set_theme_latex()
    # tab3.print_pdf('1lepWRun2_srG.pdf')

    print '\n ----------------------------------- W+jets for corridor ----------------------------------------'
    # srNames = ['srI','srJ']
    # tab4 = printTableDataDriven(f1, srNames)
    # tab4.set_theme_latex()
    # tab4.print_pdf('1lepWRun2_corridor.pdf')


def makeMETExtrInfoTable():
    srNames = ['srA0', 'srA1', 'srA2', 'srB', 'srC','srD', 'srE0', 'srE1', 'srE2', 'srE3', 'srF', 'srG0', 'srG1', 'srG2', 'srG3', 'srH', 'srI', 'srJ',]
    # srNames = ['srA0', 'srA1', 'srB', 'srC', ]

    f1 = r.TFile('../StopLooper/output/combRun2_v39_s6/lostlepton_run2.root')
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


def makeBkgCompositionTable(f1, srNames, savename='run2', fdata=None, nofraction=False):

    # bkgs = ['2lep', '1lepW', '1lepTop', 'Znunu']
    bkgs = ['2lep', '1lepW', '1lepTop', 'Znunu']

    preds_tot = getYieldEInTopoBins(f1, srNames, 'metbins')
    metrange = getBinningFromTopoSRs(f1, srNames)

    if not nofraction:
        tab = Table()
        tab.add_column('SR name', sum([[sr]*n for sr, n in zip(srNames, map(len, preds_tot))], []))
        tab.add_column('MET [GeV]', [m[0]+' -- '+m[1] for m in sum(metrange, [])])
        for bkg in bkgs:
            preds_bkg = getYieldEInTopoBins(f1, srNames, 'metbins_'+bkg)
            tab.add_column(bkg, [get_efficiency(n,d).pct_rep() for n,d in zip(sum(preds_bkg,[]),sum(preds_tot,[]))])

        tab.print_table()
        tab.set_theme_latex()
        tab.print_pdf('bkgCompositionFraction_'+savename+'.pdf')

    tab2 = Table()
    tab2.add_column('SR name', sum([[sr]*n for sr, n in zip(srNames, map(len, preds_tot))], []))
    tab2.add_column('MET [GeV]', [m[0]+' -- '+m[1] for m in sum(metrange, [])])
    for bkg in bkgs:
        preds_bkg = getYieldEInTopoBins(f1, srNames, 'metbins_'+bkg)
        tab2.add_column(bkg, [y.round(2) for y in sum(preds_bkg,[])])
    tab2.add_column('SM Total', [y.round(2) for y in sum(preds_tot,[])])

    if fdata != None:
        ylds_tot = getYieldEInTopoBins(fdata, srNames, 'metbins')
        tab2.add_column('Data', [y.round(2) for y in sum(ylds_tot,[])])
        tab2.add_column('R(Data/MC)', [(d/m).round(2) for d,m in zip(sum(ylds_tot,[]),sum(preds_tot,[]))])

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


def printAllTables():

    bvsuf = 'v39_s6'

    f1 = r.TFile('../StopLooper/output/combRun2_'+bvsuf+'/allBkg_run2.root')
    f2 = r.TFile('../StopLooper/output/combRun2_'+bvsuf+'/allData_run2.root')

    srNames = ['srA0', 'srA1', 'srA2', 'srB', 'srC','srD', 'srE0', 'srE1', 'srE2', 'srE3', 'srF', 'srG0', 'srG1', 'srG2', 'srG3', 'srH', 'srI', 'srJ']
    cr2lNames = [ sr.replace('sr', 'cr2l') for sr in srNames ]
    cr0bNames = [ sr.replace('sr', 'cr0b') for sr in srNames ]

    makeBkgCompositionTable(f1, srNames,  'run2_SR_all')
    makeBkgCompositionTable(f1, cr2lNames, 'run2_CR2l_all', fdata=f2)
    makeBkgCompositionTable(f1, cr0bNames, 'run2_CR0b_all', fdata=f2)

    srNames = ['srA0', 'srA1', 'srA2', 'srB', 'srC','srD', 'srE0', 'srE1', 'srE2', 'srE3', 'srF', 'srG0', 'srG1', 'srG2', 'srG3', 'srH']
    cr2lNames = [ sr.replace('sr', 'cr2l') for sr in srNames ]
    cr0bNames = [ sr.replace('sr', 'cr0b') for sr in srNames ]

    makeBkgCompositionTable(f1, srNames,  'run2_SR_std')
    makeBkgCompositionTable(f1, cr2lNames, 'run2_CR2l_std')
    makeBkgCompositionTable(f1, cr0bNames, 'run2_CR0b_std')

    # # # # # # # # # # # # # # #
    # Data driven estimated table

    indir = '../StopLooper/output/combRun2_'+bvsuf

    makeBkgEstimateTablesLostLepton(indir)
    makeBkgEstimateTables1LepFromW(indir)

    # # # # # # # # # # # # # # #
    # Year separated results

    srNames = ['srA0', 'srA1', 'srA2', 'srB', 'srC','srD', 'srE0', 'srE1', 'srE2', 'srE3', 'srF', 'srG0', 'srG1', 'srG2', 'srG3', 'srH', 'srI', 'srJ']
    cr2lNames = [ sr.replace('sr', 'cr2l') for sr in srNames ]
    cr0bNames = [ sr.replace('sr', 'cr0b') for sr in srNames ]

    f16 = r.TFile('../StopLooper/output/samp16_'+bvsuf+'/allBkg_16.root')
    d16 = r.TFile('../StopLooper/output/samp16_'+bvsuf+'/allData_16.root')
    makeBkgCompositionTable(f16, srNames,   '2016_SR_all')
    makeBkgCompositionTable(f16, cr2lNames, '2016_CR2l_all', fdata=d16)
    makeBkgCompositionTable(f16, cr0bNames, '2016_CR0b_all', fdata=d16)

    f17 = r.TFile('../StopLooper/output/samp17_'+bvsuf+'/allBkg_17.root')
    d17 = r.TFile('../StopLooper/output/samp17_'+bvsuf+'/allData_17.root')
    makeBkgCompositionTable(f17, srNames,   '2017_SR_all')
    makeBkgCompositionTable(f17, cr2lNames, '2017_CR2l_all', fdata=d17)
    makeBkgCompositionTable(f17, cr0bNames, '2017_CR0b_all', fdata=d17)

    f18 = r.TFile('../StopLooper/output/samp18_'+bvsuf+'/allBkg_18.root')
    d18 = r.TFile('../StopLooper/output/samp18_'+bvsuf+'/allData_18.root')
    makeBkgCompositionTable(f18, srNames,   '2018_SR_all')
    makeBkgCompositionTable(f18, cr2lNames, '2018_CR2l_all', fdata=d18)
    makeBkgCompositionTable(f18, cr0bNames, '2018_CR0b_all', fdata=d18)



def makePredictionTable(indir, srNames, ysuf='run2', showdata=False):

    f_lostlep = r.TFile(indir+'/lostlepton_'+ysuf+'.root','read')
    f_1lepW   = r.TFile(indir+'/1lepFromW_'+ysuf+'.root','read')
    f_1lepTop = r.TFile(indir+'/1lepFromTop_'+ysuf+'.root','read')
    f_Zinv    = r.TFile(indir+'/ZToNuNu_'+ysuf+'.root','read')
    f_data    = r.TFile(indir+'/allData_'+ysuf+'.root','read')

    y_temp = getYieldEInTopoBins(f_lostlep, srNames, 'metbins')
    y_2lep = [ y for y in sum(getYieldEInTopoBins(f_lostlep, srNames, 'metbins'), []) ]
    y_1lt  = [ y for y in sum(getYieldEInTopoBins(f_1lepTop, srNames, 'metbins'), []) ]
    y_1lW  = [ y for y in sum(getYieldEInTopoBins(f_1lepW,   srNames, 'metbins'), []) ]
    y_Zinv = [ y for y in sum(getYieldEInTopoBins(f_Zinv,    srNames, 'metbins'), []) ]
    y_bkgd = [ (y1+y2+y3+y4) for y1, y2, y3, y4 in zip(y_2lep, y_1lt, y_1lW, y_Zinv)]
    y_data = [ y.round(0).val for y in sum(getYieldEInTopoBins(f_data,    srNames, 'metbins'), []) ]

    metrange = getBinningFromTopoSRs(f_data, srNames)

    tab = Table()
    tab.add_column('SR name', sum([[sr]*n for sr, n in zip(srNames, map(len, y_temp))], []))
    tab.add_column('MET [GeV]', [m[0]+' -- '+m[1] for m in sum(metrange, [])])
    tab.add_column('lostlep', [ y.round(2) for y in y_2lep])
    tab.add_column('1lepW', [ y.round(2) for y in y_1lW])
    tab.add_column('1lepTop', [ y.round(2) for y in y_1lt])
    tab.add_column('Znunu', [ y.round(2) for y in y_Zinv])
    tab.add_column('Exp Total', [ y.round(2) for y in y_bkgd])
    tab.add_column('Data', y_data)

    tab.print_table()
    tab.set_theme_latex()
    tab.print_pdf('unblinded_'+ysuf+'.pdf')


if __name__ == '__main__':

    r.gROOT.SetBatch(1)

    # printAllTables()

    bvsuf = 'v30_m2'
    # f1 = r.TFile('../StopLooper/output/combRun2_'+bvsuf+'/allBkg_run2.root')
    srNames = ['srA0', 'srA1', 'srA2', 'srB', 'srC','srD', 'srE0', 'srE1', 'srE2', 'srE3', 'srF', 'srG0', 'srG1', 'srG2', 'srG3', 'srH', 'srI', 'srJ']
    # makeBkgCompositionTable(f1, srNames, 'run2_rare_all')

    indir16 = '../StopLooper/output/samp16_'+bvsuf
    # makePredictionTable(indir16, srNames, '16', True)
    # makePredictionTable(indir16, srNames, '16', True)

    bvsuf = 'v31_m1'
    indir = '../StopLooper/output/combRun2_'+bvsuf
    makeBkgEstimateTablesLostLepton(indir)
    makeBkgEstimateTables1LepFromW(indir)

    # makeMETExtrInfoTable()
    # makeSystUncertaintyMaxTable(indir, srNames)
    # makeBkgEstimationTableWJets()
    # makeTFComparisonTable()
