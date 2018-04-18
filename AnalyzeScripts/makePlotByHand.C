// -*- C++ -*-
// Usage: root -l -b -q makePlotByHand.C

using namespace std;

TH1D* fillSRYieldsPlot(vector<TFile*> samples, vector<string> dirs, string suffix = "", int n_srbins = 27) {
  size_t nsamp = samples.size();
  size_t ndirs = dirs.size();

  TH1D* srhist = new TH1D(Form("h_sryields%s", suffix.c_str()), "SR Yields Hist", n_srbins, 0, n_srbins);

  vector<double> bgtot(n_srbins, 0.);
  vector<double> bgerr(n_srbins, 0.);
  int srbin = 0;

  for (size_t idir = 0; idir < ndirs; ++idir) {
    string dir = dirs.at(idir);
    TString histname = Form("%s/h_metbins", dir.c_str());
    TH1D* h_metbins = (TH1D*) samples.at(0)->Get(histname);
    int n_metbins = 0;
    if (h_metbins) {
      n_metbins = h_metbins->GetNbinsX();
    } else {
      cout << "Couldn't get number of met bins" << std::endl;
      return srhist;
    }

    for (int ibin = 1; ibin <= n_metbins; ++ibin) {
      for (int isamp = 0 ; isamp < nsamp; ++isamp) {
        TString fullhistname = dir + "/h_metbins" + suffix;
        TH1D* h = (TH1D*) samples.at(isamp)->Get(fullhistname);
        double yield = 0.;
        double err = 0.;
        if (h) {
          if(ibin != n_metbins) {
            yield = h->GetBinContent(ibin);
            err = h->GetBinError(ibin);
            bgtot.at(srbin) += yield;
            bgerr.at(srbin) = sqrt(pow(bgerr.at(srbin),2) + pow(err,2));
          } else if (ibin == h->GetXaxis()->GetNbins()) { // last bin: include overflow
            yield = h->IntegralAndError(ibin, -1, err);
            bgtot.at(srbin) += yield;
            bgerr.at(srbin) = sqrt(pow(bgerr.at(srbin),2) + pow(err,2));
          } else {
            cout << "Shouldn't get here" << std::endl;
            return srhist;
          }
        }
      } // loop over all samples
      ++srbin;
    }
  }

  for (int ibin = 0; ibin < n_srbins; ++ibin) {
    srhist->SetBinContent(ibin+1, bgtot[ibin]);
    srhist->SetBinError(ibin+1, bgerr[ibin]);
  }

  return srhist;
}

THStack* fillSRYieldsStackHist(vector<TFile*> samples, vector<string> dirs, vector<Color_t> colors, vector<string> suffixs = {""}, int n_srbins = 27, float scale = 1.) {

  THStack* srhstack = new THStack(Form("stack_sryields%s", ""), "SR Yields Hist");

  for (int isuf = 0 ; isuf < suffixs.size(); ++isuf) {
    TH1D* srhist = new TH1D(Form("h_sryields%s", suffixs[isuf].c_str()), "SR Yields Hist", n_srbins, 0, n_srbins);
    int srbin = 0;
    vector<double> bgtot(n_srbins, 0.);
    vector<double> bgerr(n_srbins, 0.);
    for (string dir : dirs) {
      TString histname = Form("%s/h_metbins", dir.c_str());
      TH1D* h_metbins = (TH1D*) samples.at(0)->Get(histname);
      int n_metbins = 0;
      if (h_metbins) {
        n_metbins = h_metbins->GetNbinsX();
      } else {
        cout << "Couldn't get number of met bins" << std::endl;
        return srhstack;
      }

      for (int ibin = 1; ibin <= n_metbins; ++ibin) {
        TString fullhistname = dir + "/h_metbins" + suffixs[isuf];
        TH1D* h = (TH1D*) samples.at(0)->Get(fullhistname);
        double yield = 0.;
        double err = 0.;
        if (h) {
          if(ibin != n_metbins) {  // not last bin
            bgtot.at(srbin) = h->GetBinContent(ibin);
            bgerr.at(srbin) = h->GetBinError(ibin);
          } else if (ibin == h->GetXaxis()->GetNbins()) {// last bin: include overflow
            bgtot.at(srbin) = h->IntegralAndError(ibin, -1, err);
            bgerr.at(srbin) = err;
          } else {
            cout << "Shouldn't get here" << std::endl;
            cout << "n_metbins: " << n_metbins << " h->XaxisNbins: " << h->GetXaxis()->GetNbins() << endl;
            return srhstack;
          }
        }
        ++srbin;
      } // loop over all samples
    }

    for (int ibin = 0; ibin < n_srbins; ++ibin) {
      srhist->SetBinContent(ibin+1, bgtot[ibin]*scale);
      srhist->SetBinError(ibin+1, bgerr[ibin]*scale);
    }
    if (colors.size() > isuf) {
      srhist->SetLineColor(kGray+2);
      srhist->SetFillColor(colors.at(isuf));
    }
    srhstack->Add(srhist);
  }

  return srhstack;
}

void makeSigYieldsComparisonHist() {

  const int type = 3;
  const int n_srbins = 20;

  TFile* forg = new TFile("../StopLooper/output/samples2016/allBkg_25ns.root");
  TFile* fbkg = new TFile("../StopLooper/output/newbin0_35p9ifb/allBkg_25ns.root");

  TFile* fsig0 = new TFile("../StopLooper/output/temp14/Signal_T2tt.root");
  TFile* fsig2 = new TFile("../StopLooper/output/newbin0_35p9ifb/SMS_T2tt.root");

  // vector<string> dirs0 = {"srA", "srB", "srC", "srD", "srE", "srF", "srG", "srH", };
  vector<string> dirs0 = {"srC", "srD", "srE", "srF", "srG", "srH", };
  vector<string> dirs1 = {"srA1", "srB1", "srC1", "srD1", "srE1", "srF1", "srG1", "srH1", };
  vector<string> dirs2 = {"srA2", "srB2", "srC2", "srD2", "srE2", "srF2", "srG2", "srH2",  };
  vector<string> dirs3 = {"srC3", "srD3", "srE3", "srF3", "srG3", "srH3",  };
  vector<string> genclasses = {"_1lepTop", "_1lepW", "_Znunu", "_2lep", };

  dirs2 = dirs3;

  vector<Color_t> colors = {kRed-7, kSpring-5, kOrange-2, kAzure+7, kCyan-7, kMagenta-7, kTeal+6, kGray+2};
  THStack* hSR_stk = fillSRYieldsStackHist({fbkg}, dirs2, colors, genclasses, n_srbins, 120/35.9);
  TH1D* hSR_org = fillSRYieldsPlot({forg}, dirs0, "", n_srbins);
  hSR_org->SetName("h_SRyields_org");
  hSR_org->Scale(120/35.9);
  hSR_org->SetTitle("");

  // Only works for having only 1 signal for now. Should be using individual
  TH1D* hSR_mMT = fillSRYieldsPlot({fbkg}, dirs2, "", n_srbins);
  hSR_mMT->SetName("h_SRyields_mMT");
  hSR_mMT->Scale(120/35.9);
  TH1D* hRatio = (TH1D*) hSR_mMT->Clone("h_ratio");
  hRatio->Divide(hSR_mMT, hSR_org, 1, 1, "B");
  hSR_mMT->Scale(120/35.9);

  TH1D* hSR_sig0 = fillSRYieldsPlot({fsig0}, dirs0, "_800_400", n_srbins);
  TH1D* hSR_sig2 = fillSRYieldsPlot({fsig2}, dirs2, "_800_400", n_srbins);
  // TEfficiency* hRatio_sig = new TEfficiency(*hSR_sig2, *hSR_sig0);
  // hRatio_sig->SetName("h_ratio_sig");
  TH1D* hRatio_sig = (TH1D*) hSR_sig2->Clone("h_ratio_sig");
  hRatio_sig->Divide(hSR_sig2, hSR_sig0, 1, 1, "B");

  TCanvas* c0 = new TCanvas("c0", "c0", 1080, 800);
  gStyle->SetOptStat("");
  gStyle->SetPadGridX(0);
  gStyle->SetPadGridY(0);
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetFrameBorderMode(0);
  // c0->SetCanvasSize(600, 700);
  TPad* mainPad = new TPad("1", "1", 0.0, 0.20, 1.0, 1.0);
  TPad* ratioPad = new TPad("2", "2", 0.0, 0.0, 1.0, 0.23);
  mainPad->SetTopMargin(0.08);
  mainPad->SetLeftMargin(0.10);
  mainPad->SetRightMargin(0.05);
  mainPad->SetBottomMargin(0.22);
  mainPad->SetLogy();
  mainPad->Draw();
  ratioPad->SetTopMargin(0.05);
  ratioPad->SetLeftMargin(0.10);
  ratioPad->SetRightMargin(0.05);
  ratioPad->Draw();

  mainPad->cd();

  // hSR_org->SetLineColor(kOrange+1);
  // hSR_org->SetFillColor(kOrange+1);
  hSR_org->SetFillStyle(3002);
  hSR_org->SetLineColorAlpha(kOrange+1, 0.3);
  hSR_org->SetFillColorAlpha(kOrange+1, 0.7);

  hSR_org->GetXaxis()->SetLabelSize(0.052);
  if (type == 2) {
    hSR_org->GetXaxis()->SetBinLabel( 1, "A:[250,350]"      );
    hSR_org->GetXaxis()->SetBinLabel( 2, "A:[350,450]"      );
    hSR_org->GetXaxis()->SetBinLabel( 3, "A:[450,600]"      );
    hSR_org->GetXaxis()->SetBinLabel( 4, "A:[600, +#infty]" );
    hSR_org->GetXaxis()->SetBinLabel( 5, "B:[250,450]"      );
    hSR_org->GetXaxis()->SetBinLabel( 6, "B:[450,600]"      );
    hSR_org->GetXaxis()->SetBinLabel( 7, "B:[600, +#infty]" );
    hSR_org->GetXaxis()->SetBinLabel( 8, "C:[250,350]"      );
    hSR_org->GetXaxis()->SetBinLabel( 9, "C:[350,450]"      );
    hSR_org->GetXaxis()->SetBinLabel(10, "C:[450,550]"      );
    hSR_org->GetXaxis()->SetBinLabel(11, "C:[550,650]"      );
    hSR_org->GetXaxis()->SetBinLabel(12, "C:[650, +#infty]" );
    hSR_org->GetXaxis()->SetBinLabel(13, "D:[250,350]"      );
    hSR_org->GetXaxis()->SetBinLabel(14, "D:[350,450]"      );
    hSR_org->GetXaxis()->SetBinLabel(15, "D:[450,550]"      );
    hSR_org->GetXaxis()->SetBinLabel(16, "D:[550, +#infty]" );
    hSR_org->GetXaxis()->SetBinLabel(17, "E:[250,350]"      );
    hSR_org->GetXaxis()->SetBinLabel(18, "E:[350,550]"      );
    hSR_org->GetXaxis()->SetBinLabel(19, "E:[550, +#infty]" );
    hSR_org->GetXaxis()->SetBinLabel(20, "F:[250,450]"      );
    hSR_org->GetXaxis()->SetBinLabel(21, "F:[450, +#infty]" );
    hSR_org->GetXaxis()->SetBinLabel(22, "G:[250,350]"      );
    hSR_org->GetXaxis()->SetBinLabel(23, "G:[350,450]"      );
    hSR_org->GetXaxis()->SetBinLabel(24, "G:[450,600]"      );
    hSR_org->GetXaxis()->SetBinLabel(25, "G:[600, +#infty]" );
    hSR_org->GetXaxis()->SetBinLabel(26, "H:[250,450]"      );
    hSR_org->GetXaxis()->SetBinLabel(27, "H:[450, +#infty]" );
  } else if (type == 3) {
    hSR_org->GetXaxis()->SetBinLabel( 1, "C:[250,350]"      );
    hSR_org->GetXaxis()->SetBinLabel( 2, "C:[350,450]"      );
    hSR_org->GetXaxis()->SetBinLabel( 3, "C:[450,550]"      );
    hSR_org->GetXaxis()->SetBinLabel( 4, "C:[550,650]"      );
    hSR_org->GetXaxis()->SetBinLabel( 5, "C:[650, +#infty]" );
    hSR_org->GetXaxis()->SetBinLabel( 6, "D:[250,350]"      );
    hSR_org->GetXaxis()->SetBinLabel( 7, "D:[350,450]"      );
    hSR_org->GetXaxis()->SetBinLabel( 8, "D:[450,550]"      );
    hSR_org->GetXaxis()->SetBinLabel( 9, "D:[550, +#infty]" );
    hSR_org->GetXaxis()->SetBinLabel(10, "E:[250,350]"      );
    hSR_org->GetXaxis()->SetBinLabel(11, "E:[350,550]"      );
    hSR_org->GetXaxis()->SetBinLabel(12, "E:[550, +#infty]" );
    hSR_org->GetXaxis()->SetBinLabel(13, "F:[250,450]"      );
    hSR_org->GetXaxis()->SetBinLabel(14, "F:[450, +#infty]" );
    hSR_org->GetXaxis()->SetBinLabel(15, "G:[250,350]"      );
    hSR_org->GetXaxis()->SetBinLabel(16, "G:[350,450]"      );
    hSR_org->GetXaxis()->SetBinLabel(17, "G:[450,600]"      );
    hSR_org->GetXaxis()->SetBinLabel(18, "G:[600, +#infty]" );
    hSR_org->GetXaxis()->SetBinLabel(19, "H:[250,450]"      );
    hSR_org->GetXaxis()->SetBinLabel(20, "H:[450, +#infty]" );
  }
   
  hSR_org->GetXaxis()->LabelsOption("v");

  hSR_org->GetYaxis()->SetTitle("N Events");
  hSR_org->GetYaxis()->SetTitleOffset(0.6);
  hSR_org->GetYaxis()->SetTitleSize(0.06);
  hSR_org->GetYaxis()->SetRangeUser(0.06, 4000);
  hSR_org->Draw("hist");
  // hSR_mMT->SetFillColor(kAzure+7);
  // hSR_mMT->Draw("axissame");
  hSR_stk->Draw("histsame");
  hSR_stk->Draw("axissame");
  // hSR_mMT->SetLineColor(kAzure-3);
  // hSR_mMT->Draw("histsame");

  hSR_sig0->SetLineStyle(2);
  hSR_sig2->SetLineStyle(1);
  hSR_sig0->SetLineWidth(3);
  hSR_sig2->SetLineWidth(3);
  hSR_sig0->Scale(120/35.9);
  hSR_sig2->Scale(120/35.9);
  hSR_sig0->SetLineColor(kGray+2);
  hSR_sig2->SetLineColor(kTeal);

  for (int i = 0; i < n_srbins; ++i) {
    hSR_sig0->SetBinError(i, 0);
    hSR_sig2->SetBinError(i, 0);
  }
  hSR_sig0->Draw("same");
  hSR_sig2->Draw("same");

  TLegend* leg = new TLegend(0.42, 0.76, 0.68, 0.89);
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.042);
  leg->SetNColumns(2);
  leg->AddEntry(hSR_stk->GetHists()->At(0), "1lepTop");
  leg->AddEntry(hSR_stk->GetHists()->At(1), "1lepW");
  leg->AddEntry(hSR_stk->GetHists()->At(2), "Z#nu#nu");
  leg->AddEntry(hSR_stk->GetHists()->At(3), "lostlep");

  leg->Draw("same");

  TLegend* leg2 = new TLegend(0.68, 0.69, 0.84, 0.89);
  leg2->SetFillColor(0);
  leg2->SetFillStyle(0);
  leg2->SetBorderSize(0);
  leg2->SetTextSize(0.042);
  leg2->AddEntry(hSR_org, "Total background");
  if (type == 2) {
  leg2->AddEntry(hSR_sig0, "T2tt(1200,50) org");
  leg2->AddEntry(hSR_sig2, "T2tt(1200,50) mtag");
  } else if (type == 3) {
    leg2->AddEntry(hSR_sig0, "T2tt(800,400) org");
    leg2->AddEntry(hSR_sig2, "T2tt(800,400) rtag");
  }

  leg2 ->Draw("same");


  ratioPad->cd();
  TH1D* h_axis_ratio = new TH1D("ratio_axis","", n_srbins, 0, n_srbins);
  h_axis_ratio->GetYaxis()->SetTitleOffset(0.18);
  h_axis_ratio->GetYaxis()->SetTitleSize(0.22);
  h_axis_ratio->GetYaxis()->SetNdivisions(7);
  h_axis_ratio->GetYaxis()->SetLabelSize(0.15);
  h_axis_ratio->GetYaxis()->SetRangeUser(0, 0.6);
  h_axis_ratio->GetYaxis()->SetTitle("Ratios  ");
  h_axis_ratio->GetXaxis()->SetTickLength(0.07);
  h_axis_ratio->GetXaxis()->SetTitleSize(0.);
  h_axis_ratio->GetXaxis()->SetLabelSize(0.);
  h_axis_ratio->Draw("axis");

  hRatio->SetMarkerStyle(20);
  hRatio->SetMarkerSize(1.2);
  hRatio->SetMarkerColor(kGray+3);
  hRatio->SetLineColor(kGray+3);
  hRatio->SetLineWidth(2);
  hRatio->Draw("same");
  hRatio_sig->SetMarkerStyle(4);
  hRatio_sig->SetMarkerColor(kTeal);
  hRatio_sig->SetLineColor(kTeal);
  hRatio_sig->SetLineWidth(2);
  hRatio_sig->Draw("same");

  c0->SaveAs(Form("sigYieldsCompareHist_%s.pdf", "test"));

  bool printHistContent = false;
  if (printHistContent) {
    for (int i = 1; i <= hSR_org->GetNbinsX(); i++) {
      if (hSR_org->GetXaxis()->GetBinLabel(i)[0] == '1') continue;
      cout << "SR " << hSR_org->GetXaxis()->GetBinLabel(i) << " has bkgd: " << setprecision(3) << hSR_org->GetBinContent(i) << " +- "
           << hSR_org->GetBinError(i) << " while signal: " << hSR_mMT->GetBinContent(i) << " +- " << hSR_mMT->GetBinError(i) << endl;
    }
  }

  delete hSR_org;
  delete hSR_mMT;
  delete hSR_stk;
  delete c0;
  delete leg;
  delete h_axis_ratio;
}

int makePlotByHand() {

  makeSigYieldsComparisonHist();

  return 0;
}
