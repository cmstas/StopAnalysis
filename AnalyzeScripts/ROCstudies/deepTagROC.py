import os
import sys
import ROOT as r
import numpy as np
from math import sqrt

def makeROClist(hgood, hfake, *args, **kwargs):

    raw_yields = kwargs.get("raw_yields", False)
    ak8evt_only = kwargs.get("ak8evt_only", False)
    get_errors = kwargs.get("get_errors", False)

    bindiv = hgood.GetNbinsX() / 1.2
    startbin = hgood.FindBin(1)
    stopbin = hgood.FindBin(0) if ak8evt_only else 0
    cut_at_eff = 0.7             # human define for aesthetic
    print "startbin is", startbin
    print "bindiv is", bindiv

    ngood = hgood.Integral(stopbin, -1) if not raw_yields else 1
    nfake = hfake.Integral(stopbin, -1) if not raw_yields else 1

    lst_eff = np.array([], dtype=float)
    lst_fkr = np.array([], dtype=float)
    lst_tpt = np.array([], dtype=float)
    lst_terr = np.array([], dtype=float)
    lst_ferr = np.array([], dtype=float)

    for i in range(1, hgood.GetNbinsX()+1):
        ibin = startbin - i
        if ibin < stopbin: break
        disc = 1 - i / bindiv

        terr = r.Double()
        ferr = r.Double()
        tageff = hgood.IntegralAndError(ibin, -1, terr) / ngood
        fkrate = hfake.IntegralAndError(ibin, -1, ferr) / nfake

        if tageff < 0 : tageff = 0
        if fkrate < 0 : fkrate = 0

        if not raw_yields and tageff > cut_at_eff: break

        lst_eff = np.append(lst_eff, tageff)
        lst_fkr = np.append(lst_fkr, fkrate)
        lst_tpt = np.append(lst_tpt, disc)
        if get_errors:
            lst_terr = np.append(lst_terr, terr / ngood)
            lst_ferr = np.append(lst_ferr, ferr / nfake)

    if get_errors:
        return lst_eff, lst_fkr, lst_tpt, lst_terr, lst_ferr

    return lst_eff, lst_fkr, lst_tpt


def StoNErr(s, b, se, be):
    v = (s+2*b)**2 * se**2 + s**2 * be**2
    v /= 4*(s+b)**3
    return sqrt(v)


def getROCSigVBkg(f1, f2, sr, hname="h_deepttag", starval=[]):
    hgood = f1.Get(sr+'/'+hname)
    hfake = f2.Get(sr+'/'+hname)

    if not hgood: print "Cannot find hgood!"
    if not hfake: print "Cannot find hfake!"

    lst_tageff, lst_fkrate, lst_disc = makeROClist(hgood, hfake, ak8evt_only=True)
    # print lst_tageff, lst_disc

    for i, disc in enumerate(lst_disc):
        for sv in starval:
            if abs(disc-sv) < 0.001:
                print disc, lst_tageff[i], lst_fkrate[i]

    groc = r.TGraph(lst_tageff.size, lst_tageff, lst_fkrate)

    groc.SetTitle("Graph for Signal vs Background efficiency")
    groc.GetXaxis().SetTitle("signal eff.")
    groc.GetYaxis().SetTitle("bkg eff.")
    groc.SetLineWidth(3)

    return groc

def getNSigPoints(f1):
    count = float(0)
    for k in f1.Get("srbase").GetListOfKeys():
        hn = k.GetName()
        if "h_metbins" in hn and "_jesUp" in hn:
            count += 1
    return count

def getStoNSigVBkg(f1, f2, sr, hname="h_deepttag"):
    hgood = f1.Get(sr+'/'+hname)
    hfake = f2.Get(sr+'/'+hname)

    if not hgood: print "Cannot find hgood!"
    if not hfake: print "Cannot find hfake!"

    lst_sigyld, lst_bkgyld, lst_disc, lst_sigerr, lst_bkgerr = makeROClist(hgood, hfake, raw_yields=True, get_errors=True)

    nsigs = getNSigPoints(f1)
    print "Found", nsigs, "signal points at " + f1.GetName().split('/')[-1] + ":" + sr
    lst_sigyld /= nsigs;
    lst_sigerr /= nsigs;

    lst_StoN = np.array([lst_sigyld[i] / sqrt(lst_sigyld[i]+lst_bkgyld[i]) for i in range(len(lst_sigyld)) if lst_sigyld[i] > 0], dtype=float)
    lst_StoNerr = np.array([StoNErr(lst_sigyld[i],lst_bkgyld[i],lst_sigerr[i],lst_bkgerr[i]) for i in range(len(lst_sigyld)) if lst_sigyld[i] > 0], dtype=float)

    gston = r.TGraphErrors(lst_disc.size, lst_disc, lst_StoN, r.nullptr, lst_StoNerr)

    gston.SetTitle("Graph for Signal Significane (S/#sqrt{S+B}) vs disc")

    gston.GetXaxis().SetTitle("top discriminator")
    gston.GetYaxis().SetTitle("S / #sqrt{S+B}")
    gston.SetLineWidth(3)

    return gston

if __name__ == "__main__":

    os.system("mkdir -p plots")

    r.gROOT.SetBatch(1)

    f1 = r.TFile("../../StopLooper/output/temp11/SMS_T2tt.root")
    f2 = r.TFile("../../StopLooper/output/temp11/allBkg_25ns.root")
    f3 = r.TFile("../../StopLooper/output/temp11/SMS_T2bW.root")

    c1 = r.TCanvas("c1", "c1", 800, 600)

    # groc = getROCSigVBkg(f1, f2, "srbase", "h_deepttag", starval=[0.4, 0.6])
    # groc.Draw()
    # # c1.Print("roc_deepdisc_top.pdf")

    # grocb = getROCSigVBkg(f1, f2, "srbase", "h_binttag")
    # grocb.SetLineColor(r.kRed)
    # grocb.Draw("same")

    # leg = r.TLegend(0.2, 0.7, 0.36, 0.8)
    # leg.AddEntry(groc, "raw disc")
    # leg.AddEntry(grocb, "binerized")
    # leg.Draw()

    # c1.Print("roc_rawvsbin_top_base_dm600.pdf")

    # fxra = r.TFile("temp.root")
    # gak4 = fxra.Get("roc_ltc_dm600")
    # gak4.SetLineColor(r.kCyan)
    # gak4.Draw("same")

    # c1.Print("roc_mvr_dm600.pdf")

    c1.Clear()

    grocw = getROCSigVBkg(f1, f2, "srbase", "h_deepWtag", starval=[0.4, 0.6, 0.8])
    grocw.Draw()
    c1.Print("roc_deepdisc_W_base.pdf")

    # S/sqrt(S+B) curves with errors
    c1.Clear()

    gstob = getStoNSigVBkg(f1, f2, "srbase", "h_deepWtag")
    gstob.Draw()

    c1.Print("stob_deepdisc_W_base.pdf")

    # Comined ROC curves with tmod, resolved tagger, chi2, etc
    c1.Clear()

    f1 = r.TFile("../../StopLooper/output/temp11/SMS_T2tt.root")
    f2 = r.TFile("../../StopLooper/output/temp11/allBkg_25ns.root")

    leg = r.TLegend(0.14, 0.6, 0.36, 0.85)

    srlist = ['srNJetMET1', 'srNJetMET2']
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

    fxra = r.TFile("temp3.root")
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
    gtmod = fxra.Get("roc_tmod_dm600_ge4j")
    gtmod.SetLineColor(r.kMagenta)
    gtmod.SetLineWidth(3)
    gtmod.Draw("same")
    leg.AddEntry(gtmod, "ge4j, tmod")

    leg.Draw()

    # c1.Print("rocs_incl_mvr_dm600.pdf")

    # c1.Clear()
