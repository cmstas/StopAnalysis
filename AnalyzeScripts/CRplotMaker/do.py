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
    ("njets",True,None,None),
    ("metphi",True,None,None),
    ("nleps",True,None,None),
    ("njets",True,None,None),
    ("nbjets",True,None,None),
    ("tmod",True,None,None),
    ("mlepb",True,None,None),
    ("dphijmet",True,None,None),
    ("nvtxs",False,None,None),

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
    # ("rlmet_h",True,None,None),
    # ("rlmt_h",True,None,None),
    ("rlmet",True,None,None),
    ("rlmt",True,None,None),
    ("rltmod",True,None,None),
    ("rldphi",True,None,None),
]

emu_plots = [
    ("met_h",True,None,None),
    ("mt_h",True,None,None),
    ("mll",True,None,None),
    ("ptll",True,None,None),

    ("topness",True,None,None),
    ("metorg",True,None,None),

    ("jetpt_200nonb",True,None,None),
    ("njet_200nonb",True,None,None),

    ("mtttbar",True,None,None),
    ("ptttbar",True,None,None),
    ("ptttbar_b1",True,None,None),

]


# srNames = ['cremuA0', 'cremuA1' ]
# srNames = ['cr2lbase', 'cremuA0', 'cremuB0', 'cremuB1', 'cremuD0', 'cremuD1', 'cremuE0', ]
srNames = ['srsbmet']
srNames += ['cr0bbase', 'cr0bsbmet']
srNames += ['cr2lbase', 'cr2lsbmet']

# 2016 parameters
input_dir = '../../StopLooper/output/samp16_v30_s3'
output_dir = 'plots16_v30_s3'
bkg_set = [fn+'_16' for fn in bkgnames]
dataname = 'allData_16'

srNames += [ 'cremuA0', 'cremuA1', ]

for sr in srNames:
    plot_set = base_plots+rl_plots if ('cr2l' in sr) else base_plots
    plot_set = plot_set[1:] if 'sbmet' in sr else base_plots+emu_plots if 'cremu' in sr else plot_set
    bkg_rearr = [bkg_set[1], bkg_set[0]] + bkg_set[2:] if 'cr0b' in sr else bkg_set
    MT2PlotMaker(input_dir, bkg_rearr, dataname, sr, plot_set, output_dir, exts)

# 2017 parameters
input_dir = '../../StopLooper/output/samp17_v30_s3'
output_dir = 'plots17_v30_s3'
bkg_set = [fn+'_17' for fn in bkgnames]
dataname = 'allData_17'

def addSuffix(plotset, suf):
    for i in range(len(plotset)):
        x = list(plotset[i])
        x[0] += suf
        plotset[i] = tuple(x)

for sr in srNames:
    plot_set = base_plots+obj_plots+rl_plots if ('cr2l' in sr) else base_plots+obj_plots
    plot_set = plot_set[1:] if 'sbmet' in sr else base_plots+obj_plots+rl_plots+emu_plots if 'cremu' in sr else plot_set
    # plot_set = [(x+'_h' if (x == "met" or x == "rlmet") else x) for x in plot_set] if 'sbmet' in sr else plot_set
    # addSuffix(plot_set, '_el')
    bkg_rearr = [bkg_set[1], bkg_set[0]] + bkg_set[2:] if 'cr0b' in sr else bkg_set
    MT2PlotMaker(input_dir, bkg_rearr, dataname, sr, plot_set, output_dir, exts, scaleMC=True)
    # MT2PlotMaker(input_dir, bkg_rearr, dataname, sr, plot_set, output_dir, exts, scaleMC=True, suffix='_jesDn')

# 2018 parameters
input_dir = '../../StopLooper/output/samp18_v30_s3'
output_dir = 'plots18_v30_s3'
bkg_set = [fn+'_18' for fn in bkgnames]
dataname = 'allData_18'

for sr in srNames:
    plot_set = base_plots+obj_plots+rl_plots if ('cr2l' in sr) else base_plots+obj_plots
    plot_set = plot_set[1:] if 'sbmet' in sr else base_plots+obj_plots+rl_plots+emu_plots if 'cremu' in sr else plot_set
    # plot_set = [(x+'_h' if (x == "met" or x == "rlmet") else x) for x in plot_set] if 'sbmet' in sr else plot_set
    # addSuffix(plot_set, '_el')
    bkg_rearr = [bkg_set[1], bkg_set[0]] + bkg_set[2:] if 'cr0b' in sr else bkg_set
    MT2PlotMaker(input_dir, bkg_rearr, dataname, sr, plot_set, output_dir, exts, scaleMC=True)

# Combined dataset parameters
input_dir = '../../StopLooper/output/combRun2_v30_s1'
output_dir = 'plotsRun2_s1'
bkg_set = [fn+'_run2' for fn in bkgnames]
dataname = 'allData_run2'

# srNames = ['cr2lbase', 'cr0bbase', 'srsbmet', 'cr2lsbmet']

# for sr in srNames:
#     plot_set = base_plots+rl_plots if ('cr2l' in sr) else base_plots
#     plot_set = plot_set[1:] if 'sbmet' in sr else base_plots+emu_plots if 'cremu' in sr else plot_set
#     bkg_rearr = [bkg_set[1], bkg_set[0]] + bkg_set[2:] if 'cr0b' in sr else bkg_set
#     MT2PlotMaker(input_dir, bkg_rearr, dataname, sr, plot_set, output_dir, exts, scaleMC=True)


# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 
# Special plots
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 

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

input_dir = '../../StopLooper/output/samp17_v30_s3'
output_dir = 'plots17_data_erasplit'
bkg_set = ['data_2017BtoE',]
dataname = 'data_2017F'
srNames = ['cr2lbase', 'cr2lsbmet', 'cremuA0', 'cremuA1']
srNames += ['srsbmet2', 'cr2lsbmet2', 'srsbmt', 'cr2lsbmt']
srNames = ['srsbmet2m','srsbmet2r','cr0bsbmet2m']

# for sr in srNames:
#     plot_set = base_plots+obj_plots+rl_plots if ('cr2l' in sr) else base_plots+obj_plots
#     plot_set = plot_set[1:] if 'sbmet' in sr else base_plots+emu_plots+rl_plots if 'cremu' in sr else plot_set 
#     plot_set = [ ("mt_h",True,None,None) ]
#     bkg_rearr = bkg_set
#     MT2PlotMaker(input_dir, bkg_rearr, dataname, sr, plot_set, output_dir, exts, ratioType=1)

