#! /usr/bin/env python
import os
import sys
import ROOT
# run in batch mode to suppress plot windows
ROOT.gROOT.SetBatch(1)

from MT2PlotMaker import *

exts = ['pdf']
bkgnames = ['ttbar', 'Vjets', 'singleT', 'rare']

base_plots = [
    ("met",True,None,None),
    ("mt",True,None,None),
    ("njets",True,None,None),
    ("metphi",True,None,None),
    ("njets",True,None,None),
    ("nbjets",True,None,None),
    ("tmod",True,None,None),
    ("mlepb",True,None,None),
    ("dphijmet",True,None,None),
    ("nvtxs",False,None,None),

    ("nsbtags",True,None,None),
    ("nak8jets",True,None,None),
    ("deepttag",True,None,None,6),
    # ("resttag",True,None,None,4),
    ("bdtttag",True,None,None,4),
    ("tfttag",True,None,None,4),
]

obj_plots = [
    ("lep1pt",True,None,None),
    ("lep1eta",True,None,None),
    # ("lep2pt",True,None,None),
    # ("lep2eta",True,None,None),

    ("jet1pt",True,None,None),
    # ("jet2pt",True,None,None),
    ("jet1eta",True,None,None),
    ("jet2eta",True,None,None),
]

rl_plots = [
    # ("mll",True,None,None,3),
    ("rlmet",True,None,None),
    ("rlmt",True,None,None),
    ("nleps",True,None,None),
    # ("rlmet_h",True,None,None),
    # ("rlmt_h",True,None,None),
    ("rltmod",True,None,None),
    ("rldphijmet",True,None,None),
    ("rldphilmet",True,None,None),
]

cor_plots = [
    ("met",True,None,None),
    ("mt",True,None,None),
    ("njets",True,None,None),
    ("metphi",True,None,None),
    ("njets",True,None,None),
    ("nbjets",True,None,None),
    ("dphijmet",True,None,None),
    ("dphilmet",True,None,None),
    ("nvtxs",False,None,None),

    ("lep1pt",True,None,None),
    ("nsbtags",True,None,None),
    ("nak8jets",True,None,None),
]

emu_plots = [
    ("met_h",True,None,None),
    ("mt_h",True,None,None),
    ("mll",True,None,None),
    ("ptll",True,None,None),

    ("topness",True,None,None),

    # ("metorg",True,None,None),
    # ("jetpt_200nonb",True,None,None),
    # ("njet_200nonb",True,None,None),

    ("mtttbar",True,None,None),
    ("ptttbar",True,None,None),
    ("ptttbar_b1",True,None,None),

]

def addsuff(plotset, suf):
    for i in range(len(plotset)):
        x = list(plotset[i])
        x[0] += suf
        plotset[i] = tuple(x)
    return plotset


def makeCRbasePlotsRun2():

    exts = ['pdf']
    bkgnames = ['ttbar', 'Vjets', 'singleT', 'rare']
    bkgrar0b = ['Vjets', 'ttbar', 'singleT', 'rare']

    srNames = ['cr2lbase', 'cr0bbase']
    # srNames = ['cr0bbase']
    # srNames = ['cr2lI', 'cr0bI']

    bvsuf = 'v39_s1'

    # 2016 parameters
    input_dir = '../../StopLooper/output/samp16_'+bvsuf
    output_dir = 'plots16_'+bvsuf
    bkg_set = [fn+'_16' for fn in bkgnames]
    dataname = 'allData_16'

    for sr in srNames:
        plot_set = base_plots+rl_plots if ('cr2l' in sr) else base_plots
        bkg_rearr = [bkg_set[1], bkg_set[0]] + bkg_set[2:] if 'cr0b' in sr else bkg_set
        MT2PlotMaker(input_dir, bkg_rearr, dataname, sr, plot_set, output_dir, exts)

    # 2017 parameters
    input_dir = '../../StopLooper/output/samp17_'+bvsuf
    output_dir = 'plots17_'+bvsuf
    bkg_set = [fn+'_17' for fn in bkgnames]
    dataname = 'allData_17'

    for sr in srNames:
        plot_set = base_plots+rl_plots if ('cr2l' in sr) else base_plots
        bkg_rearr = [bkg_set[1], bkg_set[0]] + bkg_set[2:] if 'cr0b' in sr else bkg_set
        MT2PlotMaker(input_dir, bkg_rearr, dataname, sr, plot_set, output_dir, exts)

    # 2018 parameters
    input_dir = '../../StopLooper/output/samp18_'+bvsuf
    output_dir = 'plots18_'+bvsuf
    bkg_set = [fn+'_18' for fn in bkgnames]
    dataname = 'allData_18'

    # for sr in srNames:
    #     plot_set = base_plots+rl_plots if ('cr2l' in sr) else base_plots
    #     bkg_rearr = [bkg_set[1], bkg_set[0]] + bkg_set[2:] if 'cr0b' in sr else bkg_set
    #     MT2PlotMaker(input_dir, bkg_rearr, dataname, sr, plot_set, output_dir, exts)

    # Combined dataset parameters
    input_dir = '../../StopLooper/output/combRun2_'+bvsuf
    output_dir = 'plotsRun2_'+bvsuf
    bkg_set = [fn+'_run2' for fn in bkgnames]
    dataname = 'allData_run2'
    signame = 'SMS_T2tt_run2'

    # for sr in srNames:
    #     plot_set = base_plots+rl_plots if ('cr2l' in sr) else base_plots
    #     bkg_rearr = [bkg_set[1], bkg_set[0]] + bkg_set[2:] if 'cr0b' in sr else bkg_set
    #     # MT2PlotMaker(input_dir, bkg_rearr, dataname, sr, plot_set, output_dir, exts)
    #     MT2PlotMaker(input_dir, bkg_rearr, dataname, sr, plot_set, output_dir, exts, signame=signame, sig_points=['800_675',])

    # srNames = ['cr2lbase', 'cr0bbase', 'srsbmet', 'cr2lsbmet']

    # for sr in srNames:
    #     plot_set = base_plots+rl_plots if ('cr2l' in sr) else base_plots
    #     plot_set = plot_set[1:] if 'sbmet' in sr else base_plots+emu_plots if 'cremu' in sr else plot_set
    #     bkg_rearr = [bkg_set[1], bkg_set[0]] + bkg_set[2:] if 'cr0b' in sr else bkg_set
    #     MT2PlotMaker(input_dir, bkg_rearr, dataname, sr, plot_set, output_dir, exts, scaleMC=True)



# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Special plots
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

def makeCRemuPlots():

    # for sr in srNames:
    #     plot_set = base_plots+obj_plots+rl_plots if ('cr2l' in sr) else base_plots+obj_plots
    #     plot_set = addsuff(plot_set[:2],'_h')+plot_set[2:] if 'sb' in sr else base_plots+obj_plots+rl_plots+emu_plots if 'cremu' in sr else plot_set
    #     # plot_set = [(x+'_h' if (x == "met" or x == "rlmet") else x) for x in plot_set] if 'sbmet' in sr else plot_set
    #     # addsuff(plot_set, '_el')
    #     bkg_rearr = [bkg_set[1], bkg_set[0]] + bkg_set[2:] if 'cr0b' in sr else bkg_set
    #     MT2PlotMaker(input_dir, bkg_rearr, dataname, sr, plot_set, output_dir, exts, scaleMC=True)
    #     # MT2PlotMaker(input_dir, bkg_rearr, dataname, sr, plot_set, output_dir, exts, scaleMC=True, suffix='_jesDn')


    input_dir = '../../StopLooper/output/samp16_v30_s3'
    output_dir = 'plots16_v30_s3'
    bkg_set = [fn+'_16' for fn in bkgnames]
    dataname = 'allData_16'
    srNames = ['cremuA0', 'cremuA1']

    # for sr in srNames:
    #     plot_set = [("ptttbar",True,None,None), ("ptttbar_b1",True,None,None)]
    #     bkg_rearr = [bkg_set[1], bkg_set[0]] + bkg_set[2:] if 'cr0b' in sr else bkg_set
    #     MT2PlotMaker(input_dir, bkg_rearr, dataname, sr, plot_set, output_dir, exts, systset=['ISR'])

    input_dir = '../../StopLooper/output/samp17_v30_s3'
    output_dir = 'plots17_v30_s3'
    bkg_set = [fn+'_17' for fn in bkgnames]
    dataname = 'allData_17'
    srNames = ['cremuA0', 'cremuA1']

    # for sr in srNames:
    #     plot_set = [("ptttbar",True,None,None), ("ptttbar_b1",True,None,None)]
    #     bkg_rearr = [bkg_set[1], bkg_set[0]] + bkg_set[2:] if 'cr0b' in sr else bkg_set
    #     MT2PlotMaker(input_dir, bkg_rearr, dataname, sr, plot_set, output_dir, exts, systset=['ISR'])


sb_plots = [
    ("met_h",True,None,None),
    ("met_s",True,(150, 250),None),
    ("mt_s",True,None,None),
    ("metphi",True,None,None),
    ("dphijmet",True,None,None),
    ("dphilmet",True,None,None),
    ("njets",True,None,None),
    ("nbjets",True,None,None),
    ("lep1pt",True,None,None),
    ("lep1eta",True,None,None),
    ("lep1phi",True,None,None),
    ("dphiWlep",True,None,None),
    ("dphiWmet",True,None,None),
    ("nvtxs",False,None,None),
    ("tmod",True,None,None),
]

# # # # # # # # # # # # # # # # # # # # # # # # #
# Study for the 2017 samples with MET recipe v2

def makePlotsvMETandErra():
    input_dir = '../../StopLooper/output/combRun2_v37_m3'
    # input_dir = '../../StopLooper/output/data17_v30_leptrig'
    # output_dir = 'plots17_31MarFvs09MayF_metv0'
    output_dir = 'plots17_vMETandEra'
    # bkg_set = ['data_2017F_31Mar_METv2',]
    bkg_set = ['data_2017BtoE_METv2',]
    dataname = 'data_2017F_09May_METv2'
    srNames = ['cr2lbase', 'cr2lsbmet',]
    # srNames = ['cremuA0', 'cremuA1',]
    # srNames += ['srsbmet2', 'cr2lsbmet2', 'srsbmt', 'cr2lsbmt']
    srNames = ['srsbmet2','cr0bsbmet2',]
    # srNames = ['srsbmet2']
    # srNames = ['srsbmt']

    plot_set = sb_plots
    # plot_set = base_plots

    # for sr in srNames:
    #     MT2PlotMaker(input_dir, bkg_set, dataname, sr, plot_set, output_dir, exts, ratioType=1, datatitle=dataname)

    # bkg_set = ['data_2017BtoE_METv2',]
    # dataname = 'data_2017F_31Mar_METv2'
    # for sr in srNames:
    #     MT2PlotMaker(input_dir, bkg_set, dataname, sr, plot_set, output_dir, exts, ratioType=1, datatitle=dataname)
    #     os.system('rm -r {0}/31MarFvsBtoE_metv2_{1}; mv {0}/{1} {0}/31MarFvsBtoE_metv2_{1}'.format(output_dir, sr))

    # bkg_set = ['data_2017BtoE_METv0',]
    # dataname = 'data_2017F_09May_METv0'
    # for sr in srNames:
    #     MT2PlotMaker(input_dir, bkg_set, dataname, sr, plot_set, output_dir, exts, ratioType=1, datatitle=dataname)
    #     os.system('rm -r {0}/09MayFvsBtoE_metv0_{1}; mv {0}/{1} {0}/09MayFvsBtoE_metv0_{1}'.format(output_dir, sr))

    # bkg_set = ['data_2017BtoE_METv2',]
    # dataname = 'data_2017F_09May_METv2'
    # for sr in srNames:
    #     MT2PlotMaker(input_dir, bkg_set, dataname, sr, plot_set, output_dir, exts, ratioType=1, datatitle=dataname)
    #     os.system('rm -r {0}/09MayFvsBtoE_metv2_{1}; mv {0}/{1} {0}/09MayFvsBtoE_metv2_{1}'.format(output_dir, sr))

    # bkg_set = ['data_2017BtoE_METv2',]
    # dataname = 'data_2017F_09May_METv0'
    # for sr in srNames:
    #     MT2PlotMaker(input_dir, bkg_set, dataname, sr, plot_set, output_dir, exts, ratioType=1, datatitle=dataname)
    #     os.system('rm -r {0}/09MayFvsBtoE_metv0vsv2_{1}; mv {0}/{1} {0}/09MayFvsBtoE_metv0vsv2_{1}'.format(output_dir, sr))

    # bkg_set = ['data_2017F_09May_METv2',]
    # dataname = 'data_2017F_09May_METv0'
    # for sr in srNames:
    #     MT2PlotMaker(input_dir, bkg_set, dataname, sr, plot_set, output_dir, exts, ratioType=1, datatitle=dataname)
    #     os.system('rm -r {0}/09MayF_metv0vsv2_{1}; mv {0}/{1} {0}/09MayF_metv0vsv2_{1}'.format(output_dir, sr))

    # bkg_set = ['data_2017F_31Mar_METv2',]
    # dataname = 'data_2017F_09May_METv0'
    # for sr in srNames:
    #     MT2PlotMaker(input_dir, bkg_set, dataname, sr, plot_set, output_dir, exts, ratioType=1, datatitle=dataname)
    #     os.system('rm -r {0}/09MayFvs31MarF_metv0vsv2_{1}; mv {0}/{1} {0}/09MayFvs31MarF_metv0vsv2_{1}'.format(output_dir, sr))

    # bkg_set = ['data_2017BtoE_METv2',]
    # dataname = 'data_2017BtoE_METv0'
    # for sr in srNames:
    #     MT2PlotMaker(input_dir, bkg_set, dataname, sr, plot_set, output_dir, exts, ratioType=1, datatitle=dataname)
    #     os.system('rm -r {0}/BtoE_metv0vsv2_{1}; mv {0}/{1} {0}/BtoE_metv0vsv2_{1}'.format(output_dir, sr))

    input_dir = '../../StopLooper/output/samp17_v37_m3'
    output_dir = 'plots_samp17_v37'
    bkg_set = ['Vjets_17', 'ttbar_17', 'singleT_17', 'rare_17']
    # bkg_set = ['ttbar_17', 'Vjets_17', 'singleT_17', 'rare_17']
    dataname = 'allData_17'
    # dataname = 'data_2017F_31Mar'
    srNames = ['cr0bsbmet2','cr0bsbmt','cr0bsbfull']
    # srNames = ['srsbmet2']
    for sr in srNames:
        MT2PlotMaker(input_dir, bkg_set, dataname, sr, plot_set, output_dir, exts, ratioType=1, datatitle=dataname)
        os.system('rm -r {0}/{1}_v37; mv {0}/{1} {0}/{1}_v37'.format(output_dir, sr))


def makePlots17vs18():
    # input_dir = '../../StopLooper/output/samp17_v37_m3'
    input_dir = '../../StopLooper/output/combRun2_v37_m3'
    output_dir = 'plots_17vs18_metv0'
    bkg_set = ['allData_17',]
    dataname = 'allData_18'
    # srNames = ['srsbmet2','cr0bsbmet2',]
    # srNames = ['cremuA0','cremuA1' ]
    # srNames = ['cr2lbase','cr0bbase' ]
    srNames = ['cr0bbase', ]
    # plot_set = sb_plots
    plot_set = base_plots
    # plot_set = base_plots

    # bkg_set = ['data_2017BtoE_METv0',]
    # # dataname = 'allData_18'
    # for sr in srNames:
    #     MT2PlotMaker(input_dir, bkg_set, dataname, sr, plot_set, output_dir, exts, ratioType=1, datatitle=dataname)
    #     os.system('rm -r {0}/{1}_18vs17BtoE_metv0; mv {0}/{1} {0}/{1}_18vs17BtoE_metv0'.format(output_dir, sr))

    # MT2PlotMaker(input_dir, bkg_set, dataname, 'srsbmet3', sb_plots, output_dir, exts, ratioType=1, scaleMC=0.49)


def makeComparisomPlotsSystScaling():
    input_dir = '../../StopLooper/output/combRun2_v37_m3'

    srNames = ['cremuA0', 'cremuA1']
    # srNames += ['srsbmet2', ]
    # srNames = ['srsbmet2','cr0bsbmet2','srsbmet2m','cr0bsbmet2m']
    gen_plots = [
        ("genttbar_pt",True,None,None),
        ("genttbar_mt",True,None,None),
        ("genttbar_p",True,None,None),
        ("genttbar_M",True,None,None),
        ("nisrmatch",True,None,None),
        ("genttbar_ptb1",True,None,None),
    ]

    output_dir = 'plots_16vs17_metv0'
    bkg_set = ['allData_16',]
    dataname = 'allData_17'

    # for sr in srNames:
    #     MT2PlotMaker(input_dir, bkg_set, dataname, sr, emu_plots, output_dir, exts, ratioType=1, datatitle=dataname)
    #     os.system("mv {0}/{1} {0}/data_{1}".format(output_dir, sr))

    input_dir = '../../StopLooper/output/samp17_v30_cremu4'
    output_dir = 'plots_16vs17'
    bkg_set = ['ttbar_16_unscaled',]
    dataname = 'ttbar_17'

    # for sr in srNames:
    #     MT2PlotMaker(input_dir, bkg_set, dataname, sr, emu_plots+gen_plots, output_dir, exts, ratioType=1, datatitle=dataname)
    #     os.system("rm -r {0}/ttbar_{1}_uns16; mv {0}/{1} {0}/ttbar_{1}_uns16".format(output_dir, sr))

    bkg_set = ['ttbar_16_scaled',]
    dataname = 'ttbar_17'

    # for sr in srNames:
    #     MT2PlotMaker(input_dir, bkg_set, dataname, sr, emu_plots+gen_plots, output_dir, exts, ratioType=1, datatitle=dataname)
    #     os.system("rm -r {0}/ttbar_{1}_s16v17; mv {0}/{1} {0}/ttbar_{1}_s16v17".format(output_dir, sr))

    bkg_set = ['ttbar_16_unscaled',]
    dataname = 'ttbar_16_scaled'

    # for sr in srNames:
    #     MT2PlotMaker(input_dir, bkg_set, dataname, sr, emu_plots+gen_plots, output_dir, exts, ratioType=1, datatitle=dataname)
    #     os.system("rm -r {0}/ttbar_{1}_s16v16; mv {0}/{1} {0}/ttbar_{1}_s16v16".format(output_dir, sr))


if __name__ == '__main__':

    # makeCRbasePlotsRun2()

    makePlotsvMETandErra()

    # makeCRemuPlots()

