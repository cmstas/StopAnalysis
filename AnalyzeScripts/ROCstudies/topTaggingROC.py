import os
import sys
import ROOT as r
import numpy as np
from math import sqrt

# sys.path.insert(0,'/home/users/sicheng/tas/Software/dataMCplotMaker/')
# from dataMCplotMaker import dataMCplot

def makeROClist(hgood, hfake, *args, **kwargs):

    raw_yields = kwargs.get("raw_yields", False)
    hrange = kwargs.get("hrange", 1.2) # Range of the disc histogram, 2.2 for BDT tagger, 1.2 for TF tagger

    bindiv = hgood.GetNbinsX() / hrange
    startbin = hgood.FindBin(1)
    stopbin = hgood.GetNbinsX()+1 # human define for aesthetic
    cut_at_eff = 0.6             # human define for aesthetic
    print "startbin is", startbin
    print "bindiv is", bindiv

    ngood = hgood.Integral()
    nfake = hfake.Integral()

    if raw_yields:
        ngood = 1
        nfake = 1

    lst_eff = np.array([], dtype=float)
    lst_fkr = np.array([], dtype=float)
    lst_tpt = np.array([], dtype=float)

    for i in range(1, stopbin):
        ibin = startbin - i
        if ibin < 0: break
        disc = 1 - i / bindiv

        tageff = hgood.Integral(ibin, -1) / ngood
        fkrate = hfake.Integral(ibin, -1) / nfake

        if not raw_yields and tageff > cut_at_eff: break

        lst_eff = np.append(lst_eff, tageff)
        lst_fkr = np.append(lst_fkr, fkrate)
        lst_tpt = np.append(lst_tpt, disc)

    return lst_eff, lst_fkr, lst_tpt


def StoBErr(s, b, se, be):
    v = (s+2*b)**2 * se**2 + s**2 * be**2
    v /= 4*(s+b)**3
    return sqrt(v)

def getNSigPointsNew(f1):
    count = float(0)
    hmpt = f1.Get("srbase/h2d_signal_masspts")
    if not hmpt:
        print "Cannot find masspt hist, returning 1."
        return 1;
    for ibx in range(1, hmpt.GetNbinsX()+1):
        for iby in range(1, hmpt.GetNbinsY()+1):
            if hmpt.GetBinContent(ibx, iby) > 0:
                count += 1
    return count

if __name__ == "__main__":

    os.system("mkdir -p plots")

    r.gROOT.SetBatch(1)

    # f1 = r.TFile("../../StopLooper/output/temp12/SMS_T2tt.root")
    # f2 = r.TFile("../../StopLooper/output/temp11/allBkg_25ns.root")
    # hgood = f1.Get("srNJet2/h_leadtopcand_finedisc")
    # hfake = f2.Get("srNJet2/h_leadtopcand_finedisc")

    # f1 = r.TFile("../../StopLooper/output/tempNewTagger2/TTJets_amcnlo.root")
    f1 = r.TFile("../../StopLooper/output/tempNewTagger5/stopbaby.root")
    f2 = f1

    hgood = f1.Get("testTopTagging/h_lead_topcand_finedisc")
    hfake = f1.Get("testTopTagging/h_lead_fakecand_finedisc")

    if not hgood: print "Cannot find hgood!"
    if not hfake: print "Cannot find hfake!"

    ngood = hgood.Integral()
    nfake = hfake.Integral()

    lst_tageff, lst_fkrate, lst_disc = makeROClist(hgood, hfake)

    for i, disc in enumerate(lst_disc):
        if disc == 0.98 or disc == 0.9 or disc == 0.5:
            print disc, lst_tageff[i], lst_fkrate[i]
        if disc == 0.98 :
            p98_tageff = np.array([lst_tageff[i]], dtype=float)
            p98_fkrate = np.array([lst_fkrate[i]], dtype=float)

    lst_sigyld, lst_bkgyld, lst_disc = makeROClist(hgood, hfake, raw_yields=True)
    lst_sigyld /= getNSigPointsNew(f1)
    print lst_sigyld[-1], lst_bkgyld[-1]
    lst_sigyld *= 34.03 / lst_sigyld[-1]
    lst_bkgyld *= 825.9 / lst_bkgyld[-1]
    print lst_sigyld[-1], lst_bkgyld[-1]

    lst_StoSnB = np.array([lst_sigyld[i] / sqrt(lst_sigyld[i]+lst_bkgyld[i]) for i in range(len(lst_sigyld))], dtype=float)
    lst_StoB = np.array([lst_sigyld[i] / sqrt(lst_bkgyld[i]) for i in range(len(lst_sigyld))], dtype=float)
    # print lst_sigyld, lst_bkgyld, lst_disc, lst_StoSnB

    groc = r.TGraph(lst_tageff.size, lst_tageff, lst_fkrate)
    gstob = r.TGraph(lst_disc.size, lst_disc, lst_StoSnB)
    # print lst_disc[-100:]
    # print lst_sigyld[-100:]
    # print lst_bkgyld[-100:]
    # print lst_StoSnB[-100:]

    c1 = r.TCanvas("c1", "c1", 800, 600)

    gstob.SetTitle("Graph for signal significane (S/#sqrt{S+B}) vs disc")
    gstob.GetXaxis().SetTitle("top discriminator")
    gstob.GetYaxis().SetTitle("S / #sqrt{S+B}")
    gstob.Draw()

    c1.Print("stob_test_tftop.pdf")

    c1.Clear()

    groc.SetTitle("Graph for fake rate vs tagging efficiency")
    groc.GetXaxis().SetTitle("tt1l evt tagging eff.")
    groc.GetYaxis().SetTitle("tt2l evt fake rate")
    groc.SetLineColor(r.kGreen+2)
    groc.SetLineWidth(2)

    # groc.SetTitle("Graph for background vs signal efficiency")
    # groc.GetXaxis().SetTitle("sig eff.")
    # groc.GetYaxis().SetTitle("bkg eff.")

    groc.Draw()

    c1.Print("roc_test_tftop.pdf")

    # c1.Clear()

    hgood = f1.Get("testTopTagging/h_lead_tftop_finedisc")
    hfake = f1.Get("testTopTagging/h_lead_faketftop_finedisc")

    if not hgood: print "Cannot find hgood!"
    if not hfake: print "Cannot find hfake!"

    lst_eff, lst_fkr, lst_disc = makeROClist(hgood, hfake)

    print lst_disc
    for i, disc in enumerate(lst_disc):
        if disc == 0.998 or disc == 0.995 or disc == 0.99 or disc == 0.98:
            print "TF: ", disc, lst_tageff[i], lst_fkrate[i]

    tfroc = r.TGraph(lst_eff.size, lst_eff, lst_fkr)
    tfroc.SetLineColor(r.kOrange+7)
    tfroc.SetLineWidth(2)
    tfroc.SetTitle("Graph for fake rate vs tagging efficiency")
    tfroc.GetXaxis().SetTitle("tag eff.")
    tfroc.GetYaxis().SetTitle("fake rate")
    tfroc.Draw("same")

    leg = r.TLegend(0.16, 0.68, 0.42, 0.86)
    leg.AddEntry(groc, "BDT top tagger")
    leg.AddEntry(tfroc, "TF top tagger")
    leg.Draw()

    c1.Print("roc_test_BDTvsTF.pdf")

    hgood = f1.Get("testTopTagging/h_chi2_disc1")
    hfake = f2.Get("testTopTagging/h_chi2fake_disc1")

    # hgood = f1.Get("srNJet2/h_chi2_disc")
    # hfake = f2.Get("srNJet2/h_chi2_disc")

    if not hgood: print "Cannot find hgood!"
    if not hfake: print "Cannot find hfake!"

    lst_eff, lst_fkr, _ = makeROClist(hgood, hfake)

    chi2roc = r.TGraph(lst_eff.size, lst_eff, lst_fkr)
    chi2roc.SetLineColor(r.kCyan-3)
    chi2roc.SetLineWidth(2)
    chi2roc.SetTitle("Graph for fake rate vs tagging efficiency")
    chi2roc.GetXaxis().SetTitle("tag eff.")
    chi2roc.GetYaxis().SetTitle("fake rate")
    chi2roc.Draw("same")

    # leg = r.TLegend(0.2, 0.7, 0.36, 0.8)
    # leg.AddEntry(groc, "top tagger")
    leg.AddEntry(chi2roc, "had #chi^{2}")
    leg.Draw()

    c1.Print("roc_test_AddChi2.pdf")

    hgood = f1.Get("srNJet2/h_tmod_finedisc")
    hfake = f2.Get("srNJet2/h_tmod_finedisc")

    if not hgood: print "Cannot find hgood!"
    if not hfake: print "Cannot find hfake!"

    lst_eff, lst_fkr, lst_disc = makeROClist(hgood, hfake)
    for i, disc in enumerate(lst_disc):
        if (disc > 0.66 and disc < 0.67) or (disc > -0.001 and disc < 0.001):
            print disc, lst_eff[i], lst_fkr[i]

    tmodroc = r.TGraph(lst_eff.size, lst_eff, lst_fkr)
    tmodroc.SetLineColor(r.kRed)
    tmodroc.SetLineWidth(2)
    tmodroc.SetTitle("Graph for fake rate vs tagging efficiency")
    tmodroc.GetXaxis().SetTitle("tag eff.")
    tmodroc.GetYaxis().SetTitle("fake rate")
    tmodroc.Draw("same")

    # leg = r.TLegend(0.2, 0.7, 0.36, 0.8)
    # leg.AddEntry(groc, "top tagger")
    leg.AddEntry(tmodroc, "tmod")
    leg.Draw()

    # c1.Print("roc_srNJet2_tmod.pdf")

    # fout = r.TFile("restagROCs_dm200to400.root", "update")
    # groc.Write("roc_ltc_dm200to400_ge4j")
    # chi2roc.Write("roc_chi2_dm200to400_ge4j")
    # tmodroc.Write("roc_tmod_dm200to400_ge4j")
    # fout.Close()
