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
    doMultiRow = True
    if doMultiRow:
        pderr = [ getPoissonErrors(y.val) for y in yld_CR]
        preds = [ y.val for y in preds]
        alpha = [ a.val for a in alpha]

        tfstr   = [ '${0}^{{+{1}}}_{{-{2}}}$'.format(*rond(a, a*(1+sup), a*(1-sdn))) for a, sup, sdn in zip(alpha, systup, systdn)]
        yCRstr  = [ '${0}^{{+{1}}}_{{-{2}}}$'.format(*rond(int(y.val), e[0], e[1])) for y, e in zip(yld_CR, pderr)]
        Ratstr  = [ (d/m).round(2) for d, m in zip(yld_CR, yMC_CR)]
        predstr = [ '${0}^{{+{1}+{3}}}_{{-{2}-{4}}}$'.format(rond(y,pde[1]*a)[0], rond(y, pde[0]*a)[1],rond(y,pde[1]*a)[1], rond(y, sup*y)[1], rond(y, sdn*y)[1]) for y,pde,a,sup,sdn in zip(preds, pderr, alpha, systup, systdn)]
        for i in range(len(preds)-1):
            if yld_CR[i] == yld_CR[i+1]:
                # tfstr[i] = '\\multirow{{2}}{{*}}{{${0}^{{+{1}}}_{{-{2}}}}}$'.format(*rond(a, a*(1+sup), a*(1-sdn)))
                # tfstr[i+1] = '  '
                yCRstr[i] = '\\multirow{{2}}{{*}}{{${0}^{{+{1}}}_{{-{2}}}$}}'.format(*rond(int(yld_CR[i].val+yld_CR[i+1].val), *getPoissonErrors(yld_CR[i].val+yld_CR[i+1].val)))
                yCRstr[i+1] = '                        '
                Ratstr[i] = '\\multirow{{2}}{{*}}{{{0}}}'.format((yld_CR[i]/(yMC_CR[i]+yMC_CR[i+1])).round(2))
                Ratstr[i+1] = '                        '
        tab.add_column(crname+'TF$_{CR}^{SR}$', tfstr)
        tab.add_column(crname+'Data CR', yCRstr)
        tab.add_column(crname+'R$_{CR}$(data/MC)', Ratstr)
        tab.add_column(crname+'Prediction', predstr)

    elif doAsymErr:
        pderr = [ getPoissonErrors(y.val) for y in yld_CR]
        preds = [ y.val for y in preds]
        alpha = [ a.val for a in alpha]
        # staterr = [ (sqrt(d[0]**2 + m**2), sqrt(d[1]**2 + m**2)) for d, m in zip(errdat, errMC)]
        tab.add_column(crname+'TF$_{CR}^{SR}$', [ '${0}^{{+{1}}}_{{-{2}}}$'.format(*rond(a, a*(1+sup), a*(1-sdn))) for a, sup, sdn in zip(alpha, systup, systdn)])
        tab.add_column(crname+'Data CR', [ '${0}^{{+{1}}}_{{-{2}}}$'.format(*rond(int(y.val), e[0], e[1])) for y, e in zip(yld_CR, pderr)])
        # tab.add_column(crname+'R$_{CR}$(data/MC)', [ '({})%'.format((d/m*100).round(1)) for d, m in zip(yld_CR, yMC_CR)])
        tab.add_column(crname+'R$_{CR}$(data/MC)', [(d/m).round(2) for d, m in zip(yld_CR, yMC_CR)])
        tab.add_column(crname+'Prediction', [ '${0}^{{+{1}+{3}}}_{{-{2}-{4}}}$'.format(rond(y, pde[1]*a)[0], rond(y, pde[0]*a)[1],rond(y,pde[1]*a)[1], rond(y, sup*y)[1],rond(y, sdn*y)[1]) for y,pde,a,sup,sdn in zip(preds, pderr, alpha, systup, systdn)])
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

    bkgtype = 'lostlepton'

    f1 = r.TFile('../StopLooper/output/combRun2_v31_s13/'+bkgtype+'_run2.root')
    f2 = r.TFile('./'+bkgtype+'_run2.root')

    alpha1  = getYieldEInTopoBins(f1, srNames, 'alphaHist')
    alpha2  = getYieldEInTopoBins(f2, srNames, 'alphaHist')
    yMC_CR1 = getYieldEInTopoBins(f1, srNames, 'MCyields_CR')
    yMC_CR2 = getYieldEInTopoBins(f2, srNames, 'MCyields_CR')
    yMC_SR1 = getYieldEInTopoBins(f1, srNames, 'MCyields_SR')
    yld_CR1 = getYieldEInTopoBins(f1, srNames, 'datayields_CR')
    yld_CR2 = getYieldEInTopoBins(f2, srNames, 'datayields_CR')

    xrtsrs, metbin = zip(*[ (sr, [ i for i, y in enumerate(y2) if y not in y1]) for sr, y1, y2 in zip(srNames, alpha1, alpha2) if y1[-1] != y2[-1] ])
    alpha3  = [ [ y for i, y in enumerate(y2) if y not in y1] for sr, y1, y2 in zip(srNames, alpha1, alpha2) if y1[-1] != y2[-1] ]
    yMC_CR3 = [ [ y for i, y in enumerate(y2) if y not in y1] for sr, y1, y2 in zip(srNames, yMC_CR1, yMC_CR2) if y1[-1] != y2[-1] ]
    yMC_SR3 = [ [ s for s, y in zip(s1, y2) if y not in y1] for sr, s1, y1, y2 in zip(srNames, yMC_SR1, yMC_CR1, yMC_CR2) if y1[-1] != y2[-1] ]
    yld_CR3 = [ [ y for i, y in enumerate(y2) if y not in y1] for sr, y1, y2 in zip(srNames, yld_CR1, yld_CR2) if y1[-1] != y2[-1] ]


    metrange = getBinningFromTopoSRs(f1, xrtsrs)
    metrange = [ [rng for i, rng in enumerate(rngs) if i in ibins ] for rngs, ibins in zip(metrange, metbin)]

    tab = Table()
    tab.add_column('SR name', sum([[sr]*n for sr, n in zip(xrtsrs, map(len, alpha3))], []))
    # tab.add_column('metbin', sum(metbin, []))
    tab.add_column('MET [GeV]', [m[0]+' -- '+m[1] for m in sum(metrange, [])])
    tab.add_column('$M^{SR}$ Expected', [y.round(3) for y in sum(yMC_SR3, [])])
    tab.add_column('$M^{CR}$ Expected', [y.round(3) for y in sum(yMC_CR3, [])])
    # print len(sum(yMC_CR3, [])), len(sum(yld_CR3, []))
    # tab.add_column('N^{CR} Observed', [y.round(3) for y in sum(yld_CR3, [])])
    tab.add_column('Transfer Factor', [y.round(3) for y in sum(alpha3, [])])

    tab.print_table()
    tab.set_theme_latex()
    # tab.print_table()
    tab.print_pdf(bkgtype+'_METextrBins.pdf')


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


def printCutFlowTable(f1, sufs=['',], hname='cutflow', savename='T2tt', tab=None):

    if tab == None: tab = Table()
    for suf in sufs:
        cfhist = getYieldEInTopoBins(f1, ['testCutflow',], '{}{}'.format(hname, suf))
        tab.add_column(suf, ['${} \\pm {}$'.format(*rond(y.val, y.err)) for y in sum(cfhist,[])])
    tab.print_table()
    tab.set_theme_latex()
    tab.print_pdf('cutflow_'+savename+'.pdf')
    return tab

def makeSigYieldTable(f1, srNames, sigpoints=[], savename='run2', tab=None):

    metrange = getBinningFromTopoSRs(f1, srNames)

    if tab == None: tab = Table()
    tab.add_column('SR name', sum([[sr]*n for sr, n in zip(srNames, map(len, metrange))], []))
    tab.add_column('MET [GeV]', [m[0]+' -- '+m[1] for m in sum(metrange, [])])
    for mstop, mlsp in sigpoints:
        preds_sig = getYieldEInTopoBins3D(f1, srNames, mstop, mlsp, 'metbins')
        tab.add_column('({},{})'.format(mstop,mlsp), ['${} \\pm {}$'.format(*rond(y.val, y.err)) for y in sum(preds_sig,[])])
    tab.print_table()
    tab.set_theme_latex()
    tab.print_pdf('sigYields_'+savename+'.pdf')
    return tab

def makeSignalYieldTables(indir, ysuf='run2'):


    srNames   = ['srA0', 'srA1', 'srA2', 'srB', 'srC','srD', 'srE0', 'srE1', 'srE2', 'srE3', 'srF', 'srG0', 'srG1', 'srG2', 'srG3', 'srH', 'srI', 'srJ']
    
    f_t2tt = r.TFile(indir+'/SMS_T2tt_'+ysuf+'.root','read')
    # pts_t2tt = [(1200,100), (1050,50), (850,100), (650,350), (525, 350), (437, 350)]
    pts_t2tt = [(1200,100), (1050,50), (850,100), (650,350)]
    pts_corr = [(500, 325), (425, 350), (400, 300)]

    f_t2bW = r.TFile(indir+'/SMS_T2bW_'+ysuf+'.root','read')
    pts_t2bW = [(950,100), (800,350), (600,400)]

    f_t2bt = r.TFile(indir+'/SMS_T2bt_'+ysuf+'.root','read')
    pts_t2bt = [(900,50), (750,400), (500,300)]

    makeSigYieldTable(f_t2tt, srNames, pts_t2tt, 'test_T2tt')
    makeSigYieldTable(f_t2bW, srNames, pts_t2bW, 'test_T2bW')
    makeSigYieldTable(f_t2bt, srNames, pts_t2bt, 'test_T2bt')

    t2tt_sufs = ['_{}_{}'.format(mstop,mlsp) for mstop, mlsp in pts_t2tt]
    t2bW_sufs = ['_{}_{}'.format(mstop,mlsp) for mstop, mlsp in pts_t2bW]
    t2bt_sufs = ['_{}_{}'.format(mstop,mlsp) for mstop, mlsp in pts_t2bt]
    corr_sufs = ['_{}_{}'.format(mstop,mlsp) for mstop, mlsp in pts_corr]

    print '------------- T2tt --------------'
    printCutFlowTable(f_t2tt, t2tt_sufs, 'cutflow', 'cutflow_T2tt')
    printCutFlowTable(f_t2tt, t2tt_sufs, 'selcat',  'selcat_T2tt')
    printCutFlowTable(f_t2tt, corr_sufs, 'corcutflow', 'corcutflow_T2tt')
    printCutFlowTable(f_t2tt, corr_sufs, 'corselcat',  'corselcat_T2tt')
    print '------------- T2bW --------------'
    printCutFlowTable(f_t2bW, t2bW_sufs, 'cutflow', 'cutflow_T2bW')
    printCutFlowTable(f_t2bW, t2bW_sufs, 'selcat',  'selcat_T2bW')
    print '------------- T2bt --------------'
    printCutFlowTable(f_t2bt, t2bt_sufs, 'cutflow', 'cutflow_T2bt')
    printCutFlowTable(f_t2bt, t2bt_sufs, 'selcat',  'selcat_T2bt')


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

    bvsuf = 'v31_s21'

    f1 = r.TFile('../StopLooper/output/combRun2_'+bvsuf+'/allBkg_run2.root')
    f2 = r.TFile('../StopLooper/output/combRun2_'+bvsuf+'/allData_run2.root')

    srNames = ['srA0', 'srA1', 'srA2', 'srB', 'srC','srD', 'srE0', 'srE1', 'srE2', 'srE3', 'srF', 'srG0', 'srG1', 'srG2', 'srG3', 'srH', 'srI', 'srJ']
    cr2lNames = [ sr.replace('sr', 'cr2l') for sr in srNames ]
    cr0bNames = [ sr.replace('sr', 'cr0b') for sr in srNames ]

    # makeBkgCompositionTable(f1, srNames,  'run2_SR_all')
    # makeBkgCompositionTable(f1, cr2lNames, 'run2_CR2l_all', fdata=f2)
    # makeBkgCompositionTable(f1, cr0bNames, 'run2_CR0b_all', fdata=f2)

    srNames = ['srA0', 'srA1', 'srA2', 'srB', 'srC','srD', 'srE0', 'srE1', 'srE2', 'srE3', 'srF', 'srG0', 'srG1', 'srG2', 'srG3', 'srH']
    cr2lNames = [ sr.replace('sr', 'cr2l') for sr in srNames ]
    cr0bNames = [ sr.replace('sr', 'cr0b') for sr in srNames ]

    # makeBkgCompositionTable(f1, srNames,  'run2_SR_std')
    # makeBkgCompositionTable(f1, cr2lNames, 'run2_CR2l_std')
    # makeBkgCompositionTable(f1, cr0bNames, 'run2_CR0b_std')

    # # # # # # # # # # # # # # #
    # Data driven estimated table

    indir = '../StopLooper/output/combRun2_'+bvsuf

    # makeBkgEstimateTablesLostLepton(indir)
    # makeBkgEstimateTables1LepFromW(indir)

    # # # # # # # # # # # # # # #
    # Year separated results

    srNames = ['srA0', 'srA1', 'srA2', 'srB', 'srC','srD', 'srE0', 'srE1', 'srE2', 'srE3', 'srF', 'srG0', 'srG1', 'srG2', 'srG3', 'srH', 'srI', 'srJ']
    cr2lNames = [ sr.replace('sr', 'cr2l') for sr in srNames ]
    cr0bNames = [ sr.replace('sr', 'cr0b') for sr in srNames ]

    f16 = r.TFile('../StopLooper/output/combRun2_'+bvsuf+'/allBkg_16.root')
    d16 = r.TFile('../StopLooper/output/combRun2_'+bvsuf+'/allData_16.root')
    makeBkgCompositionTable(f16, srNames,   '2016_SR_all')
    makeBkgCompositionTable(f16, srNames,   '2016_SRwd_all', fdata=d16)
    makeBkgCompositionTable(f16, cr2lNames, '2016_CR2l_all', fdata=d16)
    makeBkgCompositionTable(f16, cr0bNames, '2016_CR0b_all', fdata=d16)

    f17 = r.TFile('../StopLooper/output/combRun2_'+bvsuf+'/allBkg_17.root')
    d17 = r.TFile('../StopLooper/output/combRun2_'+bvsuf+'/allData_17.root')
    makeBkgCompositionTable(f17, srNames,   '2017_SR_all')
    makeBkgCompositionTable(f17, srNames,   '2017_SRwd_all', fdata=d17)
    makeBkgCompositionTable(f17, cr2lNames, '2017_CR2l_all', fdata=d17)
    makeBkgCompositionTable(f17, cr0bNames, '2017_CR0b_all', fdata=d17)

    f18 = r.TFile('../StopLooper/output/combRun2_'+bvsuf+'/allBkg_18.root')
    d18 = r.TFile('../StopLooper/output/combRun2_'+bvsuf+'/allData_18.root')
    makeBkgCompositionTable(f18, srNames,   '2018_SR_all')
    makeBkgCompositionTable(f18, srNames,   '2018_SRwd_all', fdata=d18)
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

def makeBetterPredictionTable(indir, ysuf='run2', showdata=False, showUpDn=True):

    f_lostlep = r.TFile(indir+'/lostlepton_'+ysuf+'.root','read')
    f_1lepW   = r.TFile(indir+'/1lepFromW_'+ysuf+'.root','read')
    f_1lepTop = r.TFile(indir+'/1lepFromTop_'+ysuf+'.root','read')
    f_Zinv    = r.TFile(indir+'/ZToNuNu_'+ysuf+'.root','read')
    f_data    = r.TFile(indir+'/allData_'+ysuf+'.root','read')

    srNames   = ['srA0', 'srA1', 'srA2', 'srB', 'srC','srD', 'srE0', 'srE1', 'srE2', 'srE3', 'srF', 'srG0', 'srG1', 'srG2', 'srG3', 'srH', 'srI', 'srJ']
    srNamesDD = ['srA0', 'srA1', 'srB', 'srC','srD', 'srE0', 'srE1', 'srF', 'srG0', 'srG1', 'srH', 'srI', 'srJ']
    srNamesMC = ['srA2', 'srE2', 'srE3', 'srG2', 'srG3', ]

    syst2l  = ['jes', 'ISR', 'lepSF', 'metTTbar', 'L1prefire', 'pileup', 'pdf', 'q2', 'alphas', 'metRes', 'tauSF', 'softbSF']
    syst1lW = ['jes', 'bTagEffHF', 'bTagEffLF', 'WbXsec', 'L1prefire', 'pileup', 'pdf', 'q2', 'alphas', 'metRes', 'softbSF']
    syst1lt = ['jes', 'L1prefire', 'pileup', 'pdf', 'q2', 'alphas', 'metRes', 'ttagSF', 'softbSF']
    systZnu = ['jes', 'L1prefire', 'pileup', 'pdf', 'q2', 'alphas', 'metRes', 'ttagSF', 'softbSF']

    str2l, pred2l, e2lup, e2ldn = getPredStrDataDriven(f_lostlep, srNames, syst2l, showUpDn)

    str1l, pred1l, e1lup, e1ldn = getPredStrDataDriven(f_1lepW, srNamesDD, syst1lW, showUpDn)
    str1lmc, pred1lmc, e1lmcup, e1lmcdn = getPredStrMCDriven(f_1lepW, srNamesMC, syst1lW)
    print pred1lmc
    str1l = str1l[:4] + str1lmc[:1] + str1l[4:20] + str1lmc[1:5] + str1l[20:28] + str1lmc[5:9] + str1l[28:]
    pred1l = pred1l[:4] + pred1lmc[:1] + pred1l[4:20] + pred1lmc[1:5] + pred1l[20:28] + pred1lmc[5:9] + pred1l[28:]
    e1lup = e1lup[:4] + e1lmcup[:1] + e1lup[4:20] + e1lmcup[1:5] + e1lup[20:28] + e1lmcup[5:9] + e1lup[28:]
    e1ldn = e1ldn[:4] + e1lmcdn[:1] + e1ldn[4:20] + e1lmcdn[1:5] + e1ldn[20:28] + e1lmcdn[5:9] + e1ldn[28:]

    str1lt, pred1lt, e1ltup, e1ltdn = getPredStrMCDriven(f_1lepTop, srNames, 1.0)
    strZnu, predZnu, Znuup, Znudn = getPredStrMCDriven(f_Zinv, srNames, systZnu)

    ytot = [ p2+pw+p1+pz for p2, pw, p1, pz in zip(pred2l, pred1l, pred1lt, predZnu)]
    yeup = [ sqrt(p2**2+pw**2+p1**2+pz**2) for p2, pw, p1, pz in zip(e2lup, e1lup, e1ltup, Znuup)]
    yedn = [ sqrt(p2**2+pw**2+p1**2+pz**2) for p2, pw, p1, pz in zip(e2ldn, e1ldn, e1ltdn, Znudn)]
    if showUpDn:
        strtot = [ '${0}^{{+{1}}}_{{-{2}}}$'.format(*rond(y, eup, edn)) for y, eup, edn in zip(ytot, yeup, yedn) ]
    else:
        strtot = [ '${0} \pm {1}$'.format(*rond(y, (eup+edn)*0.5)) for y, eup, edn in zip(ytot, yeup, yedn) ]

    if showdata:
        # strdat = [ '${0}^{{+{1}}}_{{-{2}}}$'.format(int(y.val), round(getPoissonErrors(y.val)[0],2), round(getPoissonErrors(y.val)[1],2)) for y in getYieldEInFlatBins(f_data, srNames)]
        strdat = [ '{0}'.format(int(y.val)) for y in getYieldEInFlatBins(f_data, srNames)]

    metrange = getBinningFromTopoSRs(f_data, srNames)

    tab = Table()
    tab.add_column('SR name', sum([[sr]*n for sr, n in zip(srNames, map(len, metrange))], []))
    tab.add_column('MET [GeV]', [m[0]+' -- '+m[1] for m in sum(metrange, [])])

    tab.add_column('lostlep', str2l)
    tab.add_column('1lepW',   str1l)
    tab.add_column('1lepTop', str1lt)
    tab.add_column('Znunu',   strZnu)
    tab.add_column('Exp Total', strtot)

    if showdata:
        tab.add_column('Data', strdat)
    else:
        tab.add_column('Data', [ '--' for i in strtot])

    tab.print_table()
    tab.set_theme_latex()
    if showUpDn:
        tab.print_pdf('ResultTable_'+ysuf+'.pdf')
    else:
        tab.print_pdf('ResultTable_'+ysuf+'_sym.pdf')


if __name__ == '__main__':

    r.gROOT.SetBatch(1)

    # printAllTables()

    bvsuf = 'v30_m2'
    # f1 = r.TFile('../StopLooper/output/combRun2_'+bvsuf+'/allBkg_run2.root')
    srNames = ['srA0', 'srA1', 'srA2', 'srB', 'srC','srD', 'srE0', 'srE1', 'srE2', 'srE3', 'srF', 'srG0', 'srG1', 'srG2', 'srG3', 'srH', 'srI', 'srJ']
    # makeBkgCompositionTable(f1, srNames, 'run2_rare_all')

    indir16 = '../StopLooper/output/samp16_'+bvsuf
    # makePredictionTable(indir16, srNames, '16', True)

    bvsuf = 'v31_s21' # final result!!
    indir = '../StopLooper/output/combRun2_'+bvsuf
    # makeBkgEstimateTablesLostLepton(indir)
    # makeBkgEstimateTables1LepFromW(indir)

    # makePredictionTable(indir, srNames, 'run2', True)
    # makeBetterPredictionTable(indir, 'run2', True, False) # result plot!!

    # makeMETExtrInfoTable()
    # makeSystUncertaintyMaxTable(indir, srNames)
    # makeBkgEstimationTableWJets()
    # makeTFComparisonTable()

    makeSignalYieldTables('../StopLooper/output/combRun2_v31_s21_cf4')
