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

def getStoNSigVBkg(f1, f2, sr, hname="h_deepttag", norm_sig=34.03, norm_bkg=825.9):
    hgood = f1.Get(sr+'/'+hname)
    hfake = f2.Get(sr+'/'+hname)

    if not hgood: print "Cannot find hgood!"
    if not hfake: print "Cannot find hfake!"

    lst_sigyld, lst_bkgyld, lst_disc, lst_sigerr, lst_bkgerr = makeROClist(hgood, hfake, raw_yields=True, get_errors=True)

    nsigs = getNSigPointsNew(f1)
    print "Found", nsigs, "signal points at " + f1.GetName().split('/')[-1] + ":" + sr
    lst_sigyld /= nsigs;
    lst_sigerr /= nsigs;

    norm_sig /= lst_sigyld[-1]
    norm_bkg /= lst_bkgyld[-1]
    lst_sigyld *= norm_sig
    lst_bkgyld *= norm_bkg
    lst_sigerr *= norm_sig
    lst_bkgerr *= norm_bkg

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

    f1 = r.TFile("../../StopLooper/output/samp17_s9/SMS_T2tt_dMgt600.root")
    f2 = r.TFile("../../StopLooper/output/samp17_s9/allBkg_17.root")
    # f3 = r.TFile("../../StopLooper/output/temp11/SMS_T2bW.root")

    c1 = r.TCanvas("c1", "c1", 800, 600)

    groc = getROCSigVBkg(f1, f2, "srbase", "h_deepttag", starval=[0.4, 0.6])
    groc.SetLineColor(r.kAzure)
    groc.SetLineWidth(3)
    groc.Draw()
    # c1.Print("roc_deepdisc_top.pdf")

    grocb = getROCSigVBkg(f1, f2, "srbase", "h_binttag")
    grocb.SetLineWidth(3)
    grocb.SetLineColor(r.kTeal)
    grocb.Draw("same")

    leg = r.TLegend(0.2, 0.64, 0.38, 0.8)
    leg.AddEntry(groc, "raw disc")
    leg.AddEntry(grocb, "binerized")
    leg.Draw()

    c1.Print("roc_rawvsbin_top_base_dm600.pdf")

    # fxra = r.TFile("temp.root")
    # gak4 = fxra.Get("roc_ltc_dm600")
    # gak4.SetLineColor(r.kCyan)
    # gak4.Draw("same")

    # c1.Print("roc_mvr_dm600.pdf")

    c1.Clear()

    gstob = getStoNSigVBkg(f1, f2, "srbase", "h_deepttag")
    gstob.Draw()
    c1.Print("stob_raw_top_base_dm600.pdf")

    c1.Clear()

    gstob = getStoNSigVBkg(f1, f2, "srbase", "h_binttag")
    gstob.Draw()
    c1.Print("stob_bin_top_base_dm600.pdf")

    exit()
    c1.Clear()

    grocw = getROCSigVBkg(f1, f2, "srbase", "h_deepWtag", starval=[0.3,])
    grocw.Draw()
    # c1.Print("roc_deepdisc_W_base.pdf")

    # S/sqrt(S+B) curves with errors
    c1.Clear()

    gstob = getStoNSigVBkg(f1, f2, "srbase", "h_deepWtag")
    gstob.Draw()

    # c1.Print("stob_deepdisc_W_base.pdf")

    # Comined ROC curves with tmod, resolved tagger, chi2, etc
    c1.Clear()

    # f1 = r.TFile("../../StopLooper/output/temp11/SMS_T2tt.root")
    # f2 = r.TFile("../../StopLooper/output/temp11/allBkg_25ns.root")

    f1 = r.TFile("../../StopLooper/output/oldbin_120ifb/SMS_T2tt.root")
    f2 = r.TFile("../../StopLooper/output/oldbin_120ifb/allBkg_25ns.root")

    leg = r.TLegend(0.14, 0.64, 0.6, 0.85)
    leg.SetTextSize(0.042)

    # srlist = ['srNJet1', 'srNJet2']
    srlist = ['srA','srC','srE']
    colors = [r.kBlack, r.kCyan, r.kGreen]
    grs = []
    for i, sr in enumerate(srlist):
        gr = getROCSigVBkg(f1, f2, sr, starval=[0.4,])
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

    grs[0].GetXaxis().SetTitle("T2tt Signal Efficiency")
    grs[0].GetYaxis().SetTitle("SM Background Efficiency")
    grs[0].GetXaxis().SetTitleSize(0.045)
    grs[0].GetYaxis().SetTitleSize(0.045)
    grs[0].GetXaxis().SetTitleOffset(0.9)
    grs[0].GetYaxis().SetTitleOffset(0.9)
    grs[0].GetXaxis().SetRangeUser(0.0, 0.65)
    grs[0].GetYaxis().SetRangeUser(0.01, 0.4)
    grs[0].SetLineColor(r.kAzure+1)
    grs[0].Draw()
    grs[1].Draw("same")

    # groc.Draw()
    # leg.AddEntry(groc, "merged tag")
    # leg.AddEntry(grs[0], "2-3j, merged tag, #DeltaM>600")
    # leg.AddEntry(grs[1], "#geq 4j, merged tag, #DeltaM>600")

    leg.AddEntry(grs[0], "srA")
    leg.AddEntry(grs[1], "srC")
    leg.AddEntry(grs[2], "srE")

    # fxra = r.TFile("restagROCs_dm200to400.root")
    # gak4 = fxra.Get("roc_ltc_dm200to400_ge4j")
    # gak4.SetLineColor(r.kRed-7)
    # gak4.SetLineWidth(3)
    # gak4.Draw("same")
    # leg.AddEntry(gak4, "#geq 4j, resolved tag, #DeltaM<400")
    # gchi2 = fxra.Get("roc_chi2_dm200to400_ge4j")
    # gchi2.SetLineColor(r.kGray+1)
    # gchi2.SetLineWidth(3)
    # gchi2.Draw("same")
    # leg.AddEntry(gchi2, "#geq 4j, had #chi^{2}, #DeltaM<400")
    # # gtmod = fxra.Get("roc_tmod_dm600_ge4j")
    # # gtmod.SetLineColor(r.kMagenta)
    # # gtmod.SetLineWidth(3)
    # # gtmod.Draw("same")
    # # leg.AddEntry(gtmod, "#geq 4j, tmod")
    # # gtmod2 = fxra.Get("roc_tmod_dm600_2to3j")
    # # gtmod2.SetLineColor(r.kAzure)
    # # gtmod2.SetLineWidth(3)
    # # gtmod2.Draw("same")
    # # leg.AddEntry(gtmod2, "2-3j, tmod")

    leg.Draw()

    c1.Print("rocs_topoSRs.pdf")

    # c1.Clear()
