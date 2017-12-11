import os
import sys
import ROOT as r
import numpy as np

# sys.path.insert(0,'/home/users/sicheng/tas/Software/dataMCplotMaker/')
# from dataMCplotMaker import dataMCplot

def makeROClist(hgood, hfake):

    bindiv = hgood.GetNbinsX() / 2.2
    startbin = hgood.FindBin(1)
    stopbin = 172               # human define for aesthetic 
    print "startbin is", startbin
    print "bindiv is", bindiv

    ngood = hgood.Integral()
    nfake = hfake.Integral()

    lst_eff = np.array([], dtype=float)
    lst_fkr = np.array([], dtype=float)
    lst_tpt = np.array([], dtype=float)

    for i in range(1, stopbin):
        ibin = startbin - i
        disc = 1 - i / bindiv
        
        tageff = hgood.Integral(ibin, -1) / ngood
        fkrate = hfake.Integral(ibin, -1) / nfake

        lst_eff = np.append(lst_eff, tageff)
        lst_fkr = np.append(lst_fkr, fkrate)
        lst_tpt = np.append(lst_tpt, disc)
        
    return lst_eff, lst_fkr, lst_tpt

if __name__ == "__main__":

    os.system("mkdir -p plots")

    r.gROOT.SetBatch(1)

    f1 = r.TFile("../StopLooper/output/temp_v26_2/TTJets.root")

    hcalcable = f1.Get("testTopTagging/h_calculable")
    hfakeable = f1.Get("testTopTagging/h_fakable")

    hgood = f1.Get("testTopTagging/h_topcand_finedisc")
    hfake = f1.Get("testTopTagging/h_fakecand_finedisc")

    h_tageffs = r.TH1F("h_tageffs", ";disc;portion", 1100, -1.1, 1.1)
    h_fkrates = r.TH1F("h_fakerates", ";disc;portion", 1100, -1.1, 1.1)

    ngood = hgood.Integral()
    nfake = hfake.Integral()

    # ngood = hcalcable.Integral()
    # nfake = hfakeable.Integral()

    lst_tageff = np.array([], dtype=float)
    lst_fkrate = np.array([], dtype=float)

    edgebin = hgood.FindBin(1)
    print "edgebin is", edgebin
    for i in range(1, 601):
        ibin = edgebin - i
        disc = 1 - i / 500.0
        
        tageff = hgood.Integral(ibin, -1) / ngood
        fkrate = hfake.Integral(ibin, -1) / nfake

        h_tageffs.Fill(ibin, tageff)
        h_fkrates.Fill(ibin, fkrate)

        lst_tageff = np.append(lst_tageff, tageff)
        lst_fkrate = np.append(lst_fkrate, fkrate)
        
        if disc == 0.98 or disc == 0.9:
            print disc, tageff, fkrate
        if disc == 0.98 :
            p98_tageff = np.array([tageff], dtype=float)
            p98_fkrate = np.array([fkrate], dtype=float)

    groc = r.TGraph(lst_tageff.size, lst_tageff, lst_fkrate)

    print p98_tageff, p98_fkrate
    gp98 = r.TGraph(p98_tageff.size, p98_tageff, p98_fkrate)

    c1 = r.TCanvas("c1", "c1", 600, 400)

    groc.SetTitle("Graph for fake rate vs tagging efficiency")
    groc.GetXaxis().SetTitle("tag eff.")
    groc.GetYaxis().SetTitle("fake rate")
    groc.Draw()

    gp98.SetMarkerSize(2)
    gp98.SetMarkerColor(r.kRed)
    gp98.SetMarkerStyle(8)
    gp98.Draw("same")

    c1.Print("roc_calcable.pdf")

    # c1.Clear()

    f2 = r.TFile("../StopLooper/output/temp_v26_2/TTJets.root")

    hgood = f2.Get("testTopTagging/h_chi2_disc2")
    hfake = f2.Get("testTopTagging/h_chi2fake_disc2")

    lst_eff, lst_fkr, _ = makeROClist(hgood, hfake)

    chi2roc = r.TGraph(lst_eff.size, lst_eff, lst_fkr)
    chi2roc.SetLineColor(r.kBlue)
    chi2roc.SetTitle("Graph for fake rate vs tagging efficiency for Chi2")
    chi2roc.GetXaxis().SetTitle("tag eff.")
    chi2roc.GetYaxis().SetTitle("fake rate")
    chi2roc.Draw("same")

    leg = r.TLegend(0.2, 0.7, 0.36, 0.8)
    leg.AddEntry(groc, "top tagger")
    leg.AddEntry(chi2roc, "had chi2")
    leg.Draw()

    c1.Print("roc_chi2.pdf")
