# MT2PlotUtils.py
# various plotting utilities

def GetColor(sample):
    if 'ttbar'   in sample: return 870  # kAzure+10
    if 'singleT' in sample: return 625  # kRed-7
    if 'Vjets'   in sample: return 798  # kOrange-2
    # if 'rare'    in sample: return 877  # kViolet-3
    if 'rare'    in sample: return 614  # kMagenta-2

    return 866                  # for everything else

def GetCRName(cr):
    names = {"crrlbase": "Removed Lepton CR",
             "crrlbaseJ": "Removed Lepton CR",
             "crrlbaseIncl": "Removed Lepton CR",
             "crgjbase": "#gamma+jets CR",
             "crgjbaseJ": "#gamma+jets CR",
             "crgjbaseIncl": "#gamma+jets CR",
             "crdybase": "Z #rightarrow #font[12]{l}^{#plus}#font[12]{l}^{#minus} CR",
             "crdybaseJ": "Z #rightarrow #font[12]{l}^{#plus}#font[12]{l}^{#minus} CR",
             "crdybaseIncl": "Z #rightarrow #font[12]{l}^{#plus}#font[12]{l}^{#minus} CR",
             "crdybaseInclLowPt": "#font[12]{l}^{#plus}#font[12]{l}^{#minus} CR",
             "crslbase": "Single Lepton CR",
             "crslbaseJ": "Single Lepton CR",
             "crslbaseVL": "Single Lepton CR",
             "crslbaseL": "Single Lepton CR",
             "crslbaseM": "Single Lepton CR",
             "crslbaseH": "Single Lepton CR",
             "crslbaseUH": "Single Lepton CR",
             "crslwjets": "Single Lepton CR (wjets)",
             "crslttbar": "Single Lepton CR (ttbar)",
             "crslelbase": "Single Lepton CR (els)",
             "crslmubase": "Single Lepton CR (mus)",
             "crqcdbaseJ": "QCD Monojet CR",
             }

    # use the above name if defined, otherwise use cr itself
    return names.get(cr,cr)

def GetSampleName(sample):
    names = {"wjets_ht": "W+Jets",
             "2015wjets_ht": "W+Jets",
             "wjets_incl": "W+Jets",
             "dyjetsll_ht": "Z(#font[12]{ll})+Jets",
             "2015dyjetsll_ht": "Z(#font[12]{ll})+Jets",
             "dyjetsll_incl": "Z(#font[12]{ll})+Jets",
             "zinv_ht": "Z(#nu#nu)+Jets",
             "2015zinv_ht": "Z(#nu#nu)+Jets",
             "top": "Top",
             "ww": "WW",
             "gjets_ht": "Prompt #gamma",
             "2015gjets_ht": "Prompt #gamma",
             "gjets_dr0p05_ht": "Prompt #gamma",
             "fakephoton": "Fake #gamma",
             "fragphoton": "Frag. #gamma",
             "qcd_ht": "QCD",
             "2015qcd_ht": "QCD",
             "lostlepFromCRs": "Lost Lepton",
             }

    # use the above name if defined, otherwise use sample itself
    return names.get(sample,sample)

def GetVarName(var):
    names = {"ht": "H_{T}",
             "met": "E_{T}^{miss}",
             "mt2": "M_{T2}",
             "mt2bins": "M_{T2}",
             "nJet30": "N(jet)",
             "nBJet20": "N(b jet)",
             "njets": "N(jet)",
             "nbjets": "N(b jet)",
             "leppt": "p_{T}(lep)",
             "lepeta": "#eta(lep)",
             "lep1pt": "p_{T}(lep1)",
             "lep1eta": "#eta(lep1)",
             "lep2pt": "p_{T}(lep2)",
             "lep2eta": "#eta(lep2)",
             "nlepveto": "N(lep veto)",
             "zllmass": "m_{#font[12]{ll}}",
             "gammaPt": "P_{T}(#gamma)",
             "gammaEta": "#eta(#gamma)",
             "J1pt": "Subleading jet p_{T}",
             }

    if "SigmaIetaIeta" in var:
        return "Photon #sigma_{i#eta i#eta}"

    # use the above name if defined, otherwise use var itself
    return names.get(var,var)

def GetUnit(vn):
    noUnit = ["nJet","nBJet","eta","nlep","drMin","SigmaIetaIeta"]
    for s in noUnit:
        if s.lower() in vn.lower():
            return None

    return "GeV"

def GetSubtitles(dirname):
    # do special cases first
    # if dirname=="crqcdbaseJ":
    #     return ["p_{T}(jet1) > 250 GeV", "N(jet) = 2"]

    subtitle = ["E_{T}^{miss} > 250 GeV", "M_{T} > 150 GeV", "#geq 2j, #geq 1b"]

    if "cr2l" in dirname:
        subtitle = ["E_{T}^{miss} (rl) > 250 GeV", "M_{T} > 150 GeV", "#geq 2j, #geq 1b"]
    if "cr0b" in dirname:
        subtitle[2] = "#geq 2j, == 0b"
    if "cremu" in dirname:
        subtitle = ["E_{T}^{miss} > 50 GeV", "M(e#mu) > 20 GeV", "#geq 2j, #geq 0b"]
        if   "A1" in dirname: subtitle[2] = "#geq 2j, #geq 1b"
        elif "A2" in dirname: subtitle[2] = "#geq 2j, #geq 2b"
        elif "B0" in dirname: subtitle[2] = "#geq 2j, 0b"
        elif "B1" in dirname: subtitle[2] = "#geq 2j, 1b"
            
    if "sbmet" in dirname:
        subtitle[0] = "150 GeV < E_{T}^{miss} < 250 GeV"

    return subtitle

def Rebin(h_bkg_vec, h_data, r):
    for h in h_bkg_vec:
        h.Rebin(r)
    if h_data!=None:
        h_data.Rebin(r)


