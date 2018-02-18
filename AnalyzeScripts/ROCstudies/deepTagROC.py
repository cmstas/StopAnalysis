import os
import sys
import ROOT as r
import numpy as np
from math import sqrt

def makeROClist(hgood, hfake, *args, **kwargs):

    raw_yields = kwargs.get("raw_yields", False)
    ak8evt_only = kwargs.get("ak8evt_only", False)

    bindiv = hgood.GetNbinsX() / 1.2
    startbin = hgood.FindBin(1)
    stopbin = hgood.FindBin(0) if ak8evt_only else 0
    cut_at_eff = 0.5             # human define for aesthetic
    print "startbin is", startbin
    print "bindiv is", bindiv

    ngood = hgood.Integral(stopbin, -1) if not raw_yields else 1
    nfake = hfake.Integral(stopbin, -1) if not raw_yields else 1

    lst_eff = np.array([], dtype=float)
    lst_fkr = np.array([], dtype=float)
    lst_tpt = np.array([], dtype=float)

    for i in range(1, hgood.GetNbinsX()+1):
        ibin = startbin - i
        if ibin < stopbin: break
        disc = 1 - i / bindiv

        tageff = hgood.Integral(ibin, -1) / ngood
        fkrate = hfake.Integral(ibin, -1) / nfake

        if not raw_yields and tageff > cut_at_eff: break

        lst_eff = np.append(lst_eff, tageff)
        lst_fkr = np.append(lst_fkr, fkrate)
        lst_tpt = np.append(lst_tpt, disc)

    return lst_eff, lst_fkr, lst_tpt


def StoNErr(s, b, se, be):
    v = (s+2*b)**2 * se**2 + s**2 * be**2
    v /= 4*(s+b)**3
    return sqrt(v)


def getROCSigVBkg(f1, f2, sr, histname="h_lead_deepdisc_top"):
    hgood = f1.Get(sr+'/'+histname)
    hfake = f2.Get(sr+'/'+histname)

    if not hgood: print "Cannot find hgood!"
    if not hfake: print "Cannot find hfake!"

    lst_tageff, lst_fkrate, lst_disc = makeROClist(hgood, hfake, ak8evt_only=True)
    # print lst_tageff, lst_disc

    for i, disc in enumerate(lst_disc):
        if disc == 0.9 or disc == 0.8 or disc == 0.6 or disc == 0.4:
            print disc, lst_tageff[i], lst_fkrate[i]

    groc = r.TGraph(lst_tageff.size, lst_tageff, lst_fkrate)

    groc.SetTitle("Graph for fake rate vs tagging efficiency")
    groc.GetXaxis().SetTitle("signal eff.")
    groc.GetYaxis().SetTitle("bkg eff.")

    return groc

if __name__ == "__main__":

    os.system("mkdir -p plots")

    r.gROOT.SetBatch(1)

    # f1 = r.TFile("../../StopLooper/output/temp11/SMS_T2tt_mStop_400to1200.root")
    # f1 = r.TFile("../../StopLooper/output/temp11/ttbar_1lep.root")
    # f2 = r.TFile("../../StopLooper/output/temp11/ttbar_2lep.root")
    f1 = r.TFile("../../StopLooper/output/temp11/SMS_T2tt.root")
    f2 = r.TFile("../../StopLooper/output/temp11/allBkg_25ns.root")
    # f2 = f1

    # hgood = f1.Get("testTopTagging/h_lead_deepdisc_top")
    # hfake = f2.Get("testTopTagging/h_lead_deepdisc_faketop")

    hgood = f1.Get("srbase/h_lead_deepdisc_top")
    hfake = f2.Get("srbase/h_lead_deepdisc_top")

    if not hgood: print "Cannot find hgood!"
    if not hfake: print "Cannot find hfake!"

    lst_tageff, lst_fkrate, lst_disc = makeROClist(hgood, hfake, ak8evt_only=True)
    # print lst_tageff, lst_disc

    for i, disc in enumerate(lst_disc):
        if disc == 0.9 or disc == 0.8 or disc == 0.6 or disc == 0.3:
            print disc, lst_tageff[i], lst_fkrate[i]

    groc = r.TGraph(lst_tageff.size, lst_tageff, lst_fkrate)

    c1 = r.TCanvas("c1", "c1", 800, 600)

    groc.SetTitle("Graph for fake rate vs tagging efficiency")
    groc.GetXaxis().SetTitle("signal eff.")
    groc.GetYaxis().SetTitle("bkgd eff.")
    groc.SetLineWidth(3)

    # groc.SetTitle("Graph for background vs signal efficiency")
    # groc.GetXaxis().SetTitle("sig eff.")
    # groc.GetYaxis().SetTitle("bkg eff.")

    groc.Draw()

    c1.Print("roc_deepdisc_top.pdf")

    # hgood = f1.Get("testTopTagging/h_lead_bindisc_top")
    # hfake = f2.Get("testTopTagging/h_lead_bindisc_faketop")

    # hgood = f1.Get("srbase/h_lead_bindisc_top")
    # hfake = f2.Get("srbase/h_lead_bindisc_top")

    # if not hgood: print "Cannot find hgood!"
    # if not hfake: print "Cannot find hfake!"

    # lst_tageff, lst_fkrate, lst_disc = makeROClist(hgood, hfake, ak8evt_only=True)
    # # print lst_tageff, lst_disc

    # grocb = r.TGraph(lst_tageff.size, lst_tageff, lst_fkrate)
    # grocb.SetTitle("Graph for fake rate vs tagging efficiency")
    # grocb.SetLineColor(r.kBlue)
    # grocb.GetXaxis().SetTitle("tagging eff.")
    # grocb.GetYaxis().SetTitle("fake rate")
    # grocb.Draw("same")

    # leg = r.TLegend(0.2, 0.7, 0.36, 0.8)
    # leg.AddEntry(groc, "raw disc")
    # leg.AddEntry(grocb, "binerized")
    # leg.Draw()


    # fxra = r.TFile("temp.root")
    # gak4 = fxra.Get("roc_ltc_dm600")
    # gak4.SetLineColor(r.kCyan)
    # gak4.Draw("same")

    # leg = r.TLegend(0.2, 0.7, 0.36, 0.8)
    # leg.AddEntry(groc, "merged")
    # leg.AddEntry(gak4, "resolved")
    # leg.Draw()

    # c1.Print("roc_mvr_dm600.pdf")

    c1.Clear()

    hgood = f1.Get("srNJet2/h_lead_deepdisc_top")
    hfake = f2.Get("srNJet2/h_lead_deepdisc_top")

    lst_sigyld, lst_bkgyld, lst_disc = makeROClist(hgood, hfake, raw_yields=True)
    # lst_sigyld /= 52.
    print lst_sigyld[-1], lst_bkgyld[-1]
    lst_sigyld /= 36.0;
    print lst_sigyld[-1], lst_bkgyld[-1]

    lst_StoSnB = np.array([lst_sigyld[i] / sqrt(lst_sigyld[i]+lst_bkgyld[i]) for i in range(len(lst_sigyld))], dtype=float)
    # lst_StoB = np.array([lst_sigyld[i] / sqrt(lst_bkgyld[i]) for i in range(len(lst_sigyld))], dtype=float)
    # print lst_sigyld, lst_bkgyld, lst_disc, lst_StoSnB

    gstob = r.TGraph(lst_disc.size, lst_disc, lst_StoSnB)

    gstob.SetTitle("Graph for signal significane (S/#sqrt{S+B}) vs disc")
    gstob.GetXaxis().SetTitle("top discriminator")
    gstob.GetYaxis().SetTitle("S / #sqrt{S+B}")
    gstob.SetLineWidth(3)
    gstob.Draw()

    c1.Print("stob_deepdisc_top_NJet2.pdf")

    c1.Clear()

    f1 = r.TFile("../../StopLooper/output/temp11/SMS_T2tt_ge1100_dm600_incl.root")
    f2 = r.TFile("../../StopLooper/output/temp11/allBkg_25ns.root")

    leg = r.TLegend(0.17, 0.6, 0.36, 0.8)

    srlist = ['srNJetTMod1', 'srNJet2']
    colors = [r.kBlack, r.kCyan]
    grs = []
    for i, sr in enumerate(srlist):
        gr = getROCSigVBkg(f1, f2, sr)
        gr.SetLineColor(colors[i])
        gr.SetLineWidth(3)
        grs.append(gr)
        # if i == 0:
        #     print sr
        #     gr.Draw()
        # else:
        #     print sr
        #     gr.Draw("same")
        # leg.AddEntry(gr, sr)

    grs[0].Draw()
    grs[1].Draw("same")

    leg.AddEntry(grs[0], "2to3j, merged")
    leg.AddEntry(grs[1], "ge4j, merged")

    fxra = r.TFile("temp2.root")
    gak4 = fxra.Get("roc_ltc_dm600_ge4j")
    gak4.SetLineColor(r.kRed)
    gak4.SetLineWidth(3)
    gak4.Draw("same")
    leg.AddEntry(gak4, "ge4j, resolved")
    gchi2 = fxra.Get("roc_chi2_dm600_ge4j")
    gchi2.SetLineColor(r.kBlue)
    gchi2.SetLineWidth(3)
    gchi2.Draw("same")
    leg.AddEntry(gchi2, "ge4j, had #chi^{2}")

    leg.Draw()

    c1.Print("rocs_incl_mvr_dm600.pdf")

    # c1.Clear()


