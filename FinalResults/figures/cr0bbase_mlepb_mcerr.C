void cr0bbase_mlepb_mcerr()
{
//=========Macro generated from canvas: c1_n44/c1_n44
//=========  (Wed Jul 10 08:42:55 2019) by ROOT version6.10/09
   TCanvas *c1_n44 = new TCanvas("c1_n44", "c1_n44",0,0,700,700);
   gStyle->SetOptStat(0);
   c1_n44->SetHighLightColor(2);
   c1_n44->Range(-92.30769,0.001391815,676.9231,5.08338);
   c1_n44->SetFillColor(0);
   c1_n44->SetBorderMode(0);
   c1_n44->SetBorderSize(2);
   c1_n44->SetLogy();
   c1_n44->SetTicky(1);
   c1_n44->SetLeftMargin(0.12);
   c1_n44->SetTopMargin(0.08);
   c1_n44->SetBottomMargin(0.12);
   c1_n44->SetFrameBorderMode(0);
   c1_n44->SetFrameBorderMode(0);
   
   TPad* mainPad = new TPad("pad1", "pad1", 0.0, 0.20, 1.0, 0.99);
   mainPad->Range(-92.30769,0.001391815,676.9231,5.08338);
   mainPad->SetFillColor(0);
   mainPad->SetBorderMode(0);
   mainPad->SetBorderSize(2);
   mainPad->SetLogy();
   mainPad->SetTicky(1);
   mainPad->SetLeftMargin(0.12);
   mainPad->SetBottomMargin(0.12);
   mainPad->SetTopMargin(0.08);
   mainPad->SetFrameBorderMode(0);
   mainPad->SetFrameBorderMode(0);
   mainPad->Draw();
 
   TPad* ratioPad = new TPad("pad2", "pad2", 0.0, 0.02, 1.0, 0.20);
   ratioPad->SetTopMargin(0.06);
   ratioPad->SetLeftMargin(0.12);
   ratioPad->SetBottomMargin(0.07);
   ratioPad->Draw();

   mainPad->cd();


   THStack *hs = new THStack();
   hs->SetName("hs");
   hs->SetTitle("");
   hs->SetMinimum(8.886792);
   hs->SetMaximum(27956.79);
   
   TH1F *hs_stack_22 = new TH1F("hs_stack_22","",24,0,600);
   hs_stack_22->SetMinimum(3.232888);
   hs_stack_22->SetMaximum(47513.91);
   hs_stack_22->SetDirectory(0);
   hs_stack_22->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   hs_stack_22->SetLineColor(ci);
   hs_stack_22->GetXaxis()->SetTitle("M_{#font[12]{l}b} [GeV]");
   hs_stack_22->GetXaxis()->SetLabelFont(42);
   hs_stack_22->GetXaxis()->SetTitleOffset(1.1);
   hs_stack_22->GetXaxis()->SetTitleFont(42);
   hs_stack_22->GetYaxis()->SetTitle("Events / 25 GeV");
   hs_stack_22->GetYaxis()->SetLabelFont(42);
   hs_stack_22->GetYaxis()->SetTitleOffset(1.2);
   hs_stack_22->GetYaxis()->SetTitleFont(42);
   hs_stack_22->GetZaxis()->SetLabelFont(42);
   hs_stack_22->GetZaxis()->SetLabelSize(0.035);
   hs_stack_22->GetZaxis()->SetTitleSize(0.035);
   hs_stack_22->GetXaxis()->SetLabelSize(0.042);
   hs_stack_22->GetXaxis()->SetTitleSize(0.045);
   hs_stack_22->GetYaxis()->SetLabelSize(0.042);
   hs_stack_22->GetYaxis()->SetTitleSize(0.045);
   hs_stack_22->GetZaxis()->SetTitleFont(42);
   hs->SetHistogram(hs_stack_22);
   
   
   TH1D *h_mlepb_1lepTop_stack_1 = new TH1D("h_mlepb_1lepTop_stack_1","",24,0,600);
   h_mlepb_1lepTop_stack_1->SetBinContent(1,0.3888252);
   h_mlepb_1lepTop_stack_1->SetBinContent(2,2.96264);
   h_mlepb_1lepTop_stack_1->SetBinContent(3,10.99956);
   h_mlepb_1lepTop_stack_1->SetBinContent(4,14.0495);
   h_mlepb_1lepTop_stack_1->SetBinContent(5,12.0018);
   h_mlepb_1lepTop_stack_1->SetBinContent(6,6.575862);
   h_mlepb_1lepTop_stack_1->SetBinContent(7,5.899916);
   h_mlepb_1lepTop_stack_1->SetBinContent(8,4.473769);
   h_mlepb_1lepTop_stack_1->SetBinContent(9,4.836683);
   h_mlepb_1lepTop_stack_1->SetBinContent(10,3.023068);
   h_mlepb_1lepTop_stack_1->SetBinContent(11,1.54918);
   h_mlepb_1lepTop_stack_1->SetBinContent(12,3.706523);
   h_mlepb_1lepTop_stack_1->SetBinContent(13,3.28411);
   h_mlepb_1lepTop_stack_1->SetBinContent(14,2.583659);
   h_mlepb_1lepTop_stack_1->SetBinContent(15,1.652979);
   h_mlepb_1lepTop_stack_1->SetBinContent(16,2.094288);
   h_mlepb_1lepTop_stack_1->SetBinContent(17,1.454853);
   h_mlepb_1lepTop_stack_1->SetBinContent(18,0.8132741);
   h_mlepb_1lepTop_stack_1->SetBinContent(19,1.146606);
   h_mlepb_1lepTop_stack_1->SetBinContent(20,0.3704407);
   h_mlepb_1lepTop_stack_1->SetBinContent(21,0.6234374);
   h_mlepb_1lepTop_stack_1->SetBinContent(22,1.547491);
   h_mlepb_1lepTop_stack_1->SetBinContent(23,1.425497);
   h_mlepb_1lepTop_stack_1->SetBinContent(24,15.18881);
   h_mlepb_1lepTop_stack_1->SetBinError(1,0.2310789);
   h_mlepb_1lepTop_stack_1->SetBinError(2,0.738051);
   h_mlepb_1lepTop_stack_1->SetBinError(3,1.630497);
   h_mlepb_1lepTop_stack_1->SetBinError(4,1.714831);
   h_mlepb_1lepTop_stack_1->SetBinError(5,1.82354);
   h_mlepb_1lepTop_stack_1->SetBinError(6,1.169072);
   h_mlepb_1lepTop_stack_1->SetBinError(7,1.191843);
   h_mlepb_1lepTop_stack_1->SetBinError(8,0.8969717);
   h_mlepb_1lepTop_stack_1->SetBinError(9,1.041394);
   h_mlepb_1lepTop_stack_1->SetBinError(10,0.7555181);
   h_mlepb_1lepTop_stack_1->SetBinError(11,0.5628146);
   h_mlepb_1lepTop_stack_1->SetBinError(12,0.8762937);
   h_mlepb_1lepTop_stack_1->SetBinError(13,0.8702716);
   h_mlepb_1lepTop_stack_1->SetBinError(14,0.8079743);
   h_mlepb_1lepTop_stack_1->SetBinError(15,0.7037921);
   h_mlepb_1lepTop_stack_1->SetBinError(16,0.5969408);
   h_mlepb_1lepTop_stack_1->SetBinError(17,0.7333127);
   h_mlepb_1lepTop_stack_1->SetBinError(18,0.3710332);
   h_mlepb_1lepTop_stack_1->SetBinError(19,0.3998902);
   h_mlepb_1lepTop_stack_1->SetBinError(20,0.2391217);
   h_mlepb_1lepTop_stack_1->SetBinError(21,0.2971109);
   h_mlepb_1lepTop_stack_1->SetBinError(22,0.6557471);
   h_mlepb_1lepTop_stack_1->SetBinError(23,0.5656085);
   h_mlepb_1lepTop_stack_1->SetBinError(24,1.983834);
   h_mlepb_1lepTop_stack_1->SetEntries(716);

   ci = TColor::GetColor("#ff6666");
   h_mlepb_1lepTop_stack_1->SetFillColor(ci);
   h_mlepb_1lepTop_stack_1->GetXaxis()->SetTitle("M_{#it{l}b} [GeV]");
   h_mlepb_1lepTop_stack_1->GetXaxis()->SetLabelFont(42);
   h_mlepb_1lepTop_stack_1->GetXaxis()->SetLabelSize(0.035);
   h_mlepb_1lepTop_stack_1->GetXaxis()->SetTitleSize(0.035);
   h_mlepb_1lepTop_stack_1->GetXaxis()->SetTitleFont(42);
   h_mlepb_1lepTop_stack_1->GetYaxis()->SetLabelFont(42);
   h_mlepb_1lepTop_stack_1->GetYaxis()->SetLabelSize(0.035);
   h_mlepb_1lepTop_stack_1->GetYaxis()->SetTitleSize(0.035);
   h_mlepb_1lepTop_stack_1->GetYaxis()->SetTitleOffset(0);
   h_mlepb_1lepTop_stack_1->GetYaxis()->SetTitleFont(42);
   h_mlepb_1lepTop_stack_1->GetZaxis()->SetLabelFont(42);
   h_mlepb_1lepTop_stack_1->GetZaxis()->SetLabelSize(0.035);
   h_mlepb_1lepTop_stack_1->GetZaxis()->SetTitleSize(0.035);
   h_mlepb_1lepTop_stack_1->GetZaxis()->SetTitleFont(42);
   hs->Add(h_mlepb_1lepTop_stack_1,"");
   
   TH1D *h_mlepb_Znunu_stack_2 = new TH1D("h_mlepb_Znunu_stack_2","",24,0,600);
   h_mlepb_Znunu_stack_2->SetBinContent(1,1.114681);
   h_mlepb_Znunu_stack_2->SetBinContent(2,29.48146);
   h_mlepb_Znunu_stack_2->SetBinContent(3,65.74218);
   h_mlepb_Znunu_stack_2->SetBinContent(4,85.78015);
   h_mlepb_Znunu_stack_2->SetBinContent(5,78.29449);
   h_mlepb_Znunu_stack_2->SetBinContent(6,81.09349);
   h_mlepb_Znunu_stack_2->SetBinContent(7,67.20494);
   h_mlepb_Znunu_stack_2->SetBinContent(8,61.33262);
   h_mlepb_Znunu_stack_2->SetBinContent(9,54.15931);
   h_mlepb_Znunu_stack_2->SetBinContent(10,47.92276);
   h_mlepb_Znunu_stack_2->SetBinContent(11,40.86161);
   h_mlepb_Znunu_stack_2->SetBinContent(12,37.69913);
   h_mlepb_Znunu_stack_2->SetBinContent(13,30.63644);
   h_mlepb_Znunu_stack_2->SetBinContent(14,25.64619);
   h_mlepb_Znunu_stack_2->SetBinContent(15,18.56941);
   h_mlepb_Znunu_stack_2->SetBinContent(16,19.21492);
   h_mlepb_Znunu_stack_2->SetBinContent(17,14.84467);
   h_mlepb_Znunu_stack_2->SetBinContent(18,13.53714);
   h_mlepb_Znunu_stack_2->SetBinContent(19,12.31886);
   h_mlepb_Znunu_stack_2->SetBinContent(20,10.91711);
   h_mlepb_Znunu_stack_2->SetBinContent(21,15.37436);
   h_mlepb_Znunu_stack_2->SetBinContent(22,11.65745);
   h_mlepb_Znunu_stack_2->SetBinContent(23,7.19139);
   h_mlepb_Znunu_stack_2->SetBinContent(24,75.02587);
   h_mlepb_Znunu_stack_2->SetBinError(1,0.6761763);
   h_mlepb_Znunu_stack_2->SetBinError(2,3.185997);
   h_mlepb_Znunu_stack_2->SetBinError(3,4.749047);
   h_mlepb_Znunu_stack_2->SetBinError(4,5.353578);
   h_mlepb_Znunu_stack_2->SetBinError(5,5.285135);
   h_mlepb_Znunu_stack_2->SetBinError(6,5.293853);
   h_mlepb_Znunu_stack_2->SetBinError(7,4.854224);
   h_mlepb_Znunu_stack_2->SetBinError(8,4.535118);
   h_mlepb_Znunu_stack_2->SetBinError(9,4.143419);
   h_mlepb_Znunu_stack_2->SetBinError(10,3.959769);
   h_mlepb_Znunu_stack_2->SetBinError(11,3.542488);
   h_mlepb_Znunu_stack_2->SetBinError(12,3.317247);
   h_mlepb_Znunu_stack_2->SetBinError(13,4.012173);
   h_mlepb_Znunu_stack_2->SetBinError(14,2.807182);
   h_mlepb_Znunu_stack_2->SetBinError(15,2.406508);
   h_mlepb_Znunu_stack_2->SetBinError(16,2.715483);
   h_mlepb_Znunu_stack_2->SetBinError(17,2.181449);
   h_mlepb_Znunu_stack_2->SetBinError(18,2.131738);
   h_mlepb_Znunu_stack_2->SetBinError(19,1.798422);
   h_mlepb_Znunu_stack_2->SetBinError(20,1.718231);
   h_mlepb_Znunu_stack_2->SetBinError(21,1.901674);
   h_mlepb_Znunu_stack_2->SetBinError(22,1.919184);
   h_mlepb_Znunu_stack_2->SetBinError(23,1.273944);
   h_mlepb_Znunu_stack_2->SetBinError(24,4.750872);
   h_mlepb_Znunu_stack_2->SetEntries(47757);

   ci = TColor::GetColor("#9966cc");
   h_mlepb_Znunu_stack_2->SetFillColor(ci);
   h_mlepb_Znunu_stack_2->GetXaxis()->SetTitle("M_{#it{l}b} [GeV]");
   h_mlepb_Znunu_stack_2->GetXaxis()->SetLabelFont(42);
   h_mlepb_Znunu_stack_2->GetXaxis()->SetLabelSize(0.035);
   h_mlepb_Znunu_stack_2->GetXaxis()->SetTitleSize(0.035);
   h_mlepb_Znunu_stack_2->GetXaxis()->SetTitleFont(42);
   h_mlepb_Znunu_stack_2->GetYaxis()->SetLabelFont(42);
   h_mlepb_Znunu_stack_2->GetYaxis()->SetLabelSize(0.035);
   h_mlepb_Znunu_stack_2->GetYaxis()->SetTitleSize(0.035);
   h_mlepb_Znunu_stack_2->GetYaxis()->SetTitleOffset(0);
   h_mlepb_Znunu_stack_2->GetYaxis()->SetTitleFont(42);
   h_mlepb_Znunu_stack_2->GetZaxis()->SetLabelFont(42);
   h_mlepb_Znunu_stack_2->GetZaxis()->SetLabelSize(0.035);
   h_mlepb_Znunu_stack_2->GetZaxis()->SetTitleSize(0.035);
   h_mlepb_Znunu_stack_2->GetZaxis()->SetTitleFont(42);
   hs->Add(h_mlepb_Znunu_stack_2,"");
   
   TH1D *h_mlepb_2lep_stack_3 = new TH1D("h_mlepb_2lep_stack_3","",24,0,600);
   h_mlepb_2lep_stack_3->SetBinContent(1,4.444424);
   h_mlepb_2lep_stack_3->SetBinContent(2,82.16146);
   h_mlepb_2lep_stack_3->SetBinContent(3,214.6136);
   h_mlepb_2lep_stack_3->SetBinContent(4,317.5366);
   h_mlepb_2lep_stack_3->SetBinContent(5,335.9041);
   h_mlepb_2lep_stack_3->SetBinContent(6,234.5595);
   h_mlepb_2lep_stack_3->SetBinContent(7,138.0642);
   h_mlepb_2lep_stack_3->SetBinContent(8,101.8902);
   h_mlepb_2lep_stack_3->SetBinContent(9,79.12995);
   h_mlepb_2lep_stack_3->SetBinContent(10,76.08434);
   h_mlepb_2lep_stack_3->SetBinContent(11,58.13376);
   h_mlepb_2lep_stack_3->SetBinContent(12,50.20331);
   h_mlepb_2lep_stack_3->SetBinContent(13,41.36793);
   h_mlepb_2lep_stack_3->SetBinContent(14,30.39821);
   h_mlepb_2lep_stack_3->SetBinContent(15,28.61927);
   h_mlepb_2lep_stack_3->SetBinContent(16,25.2388);
   h_mlepb_2lep_stack_3->SetBinContent(17,25.51172);
   h_mlepb_2lep_stack_3->SetBinContent(18,17.33248);
   h_mlepb_2lep_stack_3->SetBinContent(19,21.45343);
   h_mlepb_2lep_stack_3->SetBinContent(20,14.03025);
   h_mlepb_2lep_stack_3->SetBinContent(21,10.90383);
   h_mlepb_2lep_stack_3->SetBinContent(22,9.445163);
   h_mlepb_2lep_stack_3->SetBinContent(23,11.20338);
   h_mlepb_2lep_stack_3->SetBinContent(24,83.82245);
   h_mlepb_2lep_stack_3->SetBinError(1,0.8961777);
   h_mlepb_2lep_stack_3->SetBinError(2,4.258626);
   h_mlepb_2lep_stack_3->SetBinError(3,7.180927);
   h_mlepb_2lep_stack_3->SetBinError(4,8.23729);
   h_mlepb_2lep_stack_3->SetBinError(5,9.083393);
   h_mlepb_2lep_stack_3->SetBinError(6,7.689531);
   h_mlepb_2lep_stack_3->SetBinError(7,5.286267);
   h_mlepb_2lep_stack_3->SetBinError(8,5.814779);
   h_mlepb_2lep_stack_3->SetBinError(9,3.96244);
   h_mlepb_2lep_stack_3->SetBinError(10,7.280663);
   h_mlepb_2lep_stack_3->SetBinError(11,3.587867);
   h_mlepb_2lep_stack_3->SetBinError(12,3.199823);
   h_mlepb_2lep_stack_3->SetBinError(13,2.915536);
   h_mlepb_2lep_stack_3->SetBinError(14,3.000854);
   h_mlepb_2lep_stack_3->SetBinError(15,2.437211);
   h_mlepb_2lep_stack_3->SetBinError(16,2.228947);
   h_mlepb_2lep_stack_3->SetBinError(17,2.595023);
   h_mlepb_2lep_stack_3->SetBinError(18,1.832538);
   h_mlepb_2lep_stack_3->SetBinError(19,7.034881);
   h_mlepb_2lep_stack_3->SetBinError(20,1.656579);
   h_mlepb_2lep_stack_3->SetBinError(21,1.545662);
   h_mlepb_2lep_stack_3->SetBinError(22,1.329533);
   h_mlepb_2lep_stack_3->SetBinError(23,1.590283);
   h_mlepb_2lep_stack_3->SetBinError(24,4.303151);
   h_mlepb_2lep_stack_3->SetEntries(20802);

   ci = TColor::GetColor("#66ccff");
   h_mlepb_2lep_stack_3->SetFillColor(ci);
   h_mlepb_2lep_stack_3->GetXaxis()->SetTitle("M_{#it{l}b} [GeV]");
   h_mlepb_2lep_stack_3->GetXaxis()->SetLabelFont(42);
   h_mlepb_2lep_stack_3->GetXaxis()->SetLabelSize(0.035);
   h_mlepb_2lep_stack_3->GetXaxis()->SetTitleSize(0.035);
   h_mlepb_2lep_stack_3->GetXaxis()->SetTitleFont(42);
   h_mlepb_2lep_stack_3->GetYaxis()->SetLabelFont(42);
   h_mlepb_2lep_stack_3->GetYaxis()->SetLabelSize(0.035);
   h_mlepb_2lep_stack_3->GetYaxis()->SetTitleSize(0.035);
   h_mlepb_2lep_stack_3->GetYaxis()->SetTitleOffset(0);
   h_mlepb_2lep_stack_3->GetYaxis()->SetTitleFont(42);
   h_mlepb_2lep_stack_3->GetZaxis()->SetLabelFont(42);
   h_mlepb_2lep_stack_3->GetZaxis()->SetLabelSize(0.035);
   h_mlepb_2lep_stack_3->GetZaxis()->SetTitleSize(0.035);
   h_mlepb_2lep_stack_3->GetZaxis()->SetTitleFont(42);
   hs->Add(h_mlepb_2lep_stack_3,"");
   
   TH1D *h_mlepb_1lepW_stack_4 = new TH1D("h_mlepb_1lepW_stack_4","",24,0,600);
   h_mlepb_1lepW_stack_4->SetBinContent(1,11.82565);
   h_mlepb_1lepW_stack_4->SetBinContent(2,227.9453);
   h_mlepb_1lepW_stack_4->SetBinContent(3,496.8212);
   h_mlepb_1lepW_stack_4->SetBinContent(4,633.037);
   h_mlepb_1lepW_stack_4->SetBinContent(5,710.3649);
   h_mlepb_1lepW_stack_4->SetBinContent(6,669.047);
   h_mlepb_1lepW_stack_4->SetBinContent(7,643.9118);
   h_mlepb_1lepW_stack_4->SetBinContent(8,577.2626);
   h_mlepb_1lepW_stack_4->SetBinContent(9,522.1562);
   h_mlepb_1lepW_stack_4->SetBinContent(10,445.1138);
   h_mlepb_1lepW_stack_4->SetBinContent(11,396.881);
   h_mlepb_1lepW_stack_4->SetBinContent(12,331.2914);
   h_mlepb_1lepW_stack_4->SetBinContent(13,275.686);
   h_mlepb_1lepW_stack_4->SetBinContent(14,243.4881);
   h_mlepb_1lepW_stack_4->SetBinContent(15,209.6014);
   h_mlepb_1lepW_stack_4->SetBinContent(16,193.4238);
   h_mlepb_1lepW_stack_4->SetBinContent(17,157.2862);
   h_mlepb_1lepW_stack_4->SetBinContent(18,140.1176);
   h_mlepb_1lepW_stack_4->SetBinContent(19,122.3636);
   h_mlepb_1lepW_stack_4->SetBinContent(20,104.2117);
   h_mlepb_1lepW_stack_4->SetBinContent(21,98.72262);
   h_mlepb_1lepW_stack_4->SetBinContent(22,85.32194);
   h_mlepb_1lepW_stack_4->SetBinContent(23,83.48165);
   h_mlepb_1lepW_stack_4->SetBinContent(24,743.3119);
   h_mlepb_1lepW_stack_4->SetBinError(1,0.509432);
   h_mlepb_1lepW_stack_4->SetBinError(2,9.795588);
   h_mlepb_1lepW_stack_4->SetBinError(3,9.007376);
   h_mlepb_1lepW_stack_4->SetBinError(4,7.94835);
   h_mlepb_1lepW_stack_4->SetBinError(5,14.07432);
   h_mlepb_1lepW_stack_4->SetBinError(6,8.445254);
   h_mlepb_1lepW_stack_4->SetBinError(7,15.56661);
   h_mlepb_1lepW_stack_4->SetBinError(8,8.279937);
   h_mlepb_1lepW_stack_4->SetBinError(9,14.37221);
   h_mlepb_1lepW_stack_4->SetBinError(10,7.440254);
   h_mlepb_1lepW_stack_4->SetBinError(11,12.70848);
   h_mlepb_1lepW_stack_4->SetBinError(12,6.076583);
   h_mlepb_1lepW_stack_4->SetBinError(13,3.453231);
   h_mlepb_1lepW_stack_4->SetBinError(14,4.594156);
   h_mlepb_1lepW_stack_4->SetBinError(15,3.874313);
   h_mlepb_1lepW_stack_4->SetBinError(16,5.788104);
   h_mlepb_1lepW_stack_4->SetBinError(17,3.126809);
   h_mlepb_1lepW_stack_4->SetBinError(18,4.20657);
   h_mlepb_1lepW_stack_4->SetBinError(19,3.608723);
   h_mlepb_1lepW_stack_4->SetBinError(20,1.993071);
   h_mlepb_1lepW_stack_4->SetBinError(21,3.283545);
   h_mlepb_1lepW_stack_4->SetBinError(22,3.717539);
   h_mlepb_1lepW_stack_4->SetBinError(23,6.691581);
   h_mlepb_1lepW_stack_4->SetBinError(24,12.82047);
   h_mlepb_1lepW_stack_4->SetEntries(381991);

   ci = TColor::GetColor("#ffcc66");
   h_mlepb_1lepW_stack_4->SetFillColor(ci);
   h_mlepb_1lepW_stack_4->GetXaxis()->SetTitle("M_{#it{l}b} [GeV]");
   h_mlepb_1lepW_stack_4->GetXaxis()->SetLabelFont(42);
   h_mlepb_1lepW_stack_4->GetXaxis()->SetLabelSize(0.035);
   h_mlepb_1lepW_stack_4->GetXaxis()->SetTitleSize(0.035);
   h_mlepb_1lepW_stack_4->GetXaxis()->SetTitleFont(42);
   h_mlepb_1lepW_stack_4->GetYaxis()->SetLabelFont(42);
   h_mlepb_1lepW_stack_4->GetYaxis()->SetLabelSize(0.035);
   h_mlepb_1lepW_stack_4->GetYaxis()->SetTitleSize(0.035);
   h_mlepb_1lepW_stack_4->GetYaxis()->SetTitleOffset(0);
   h_mlepb_1lepW_stack_4->GetYaxis()->SetTitleFont(42);
   h_mlepb_1lepW_stack_4->GetZaxis()->SetLabelFont(42);
   h_mlepb_1lepW_stack_4->GetZaxis()->SetLabelSize(0.035);
   h_mlepb_1lepW_stack_4->GetZaxis()->SetTitleSize(0.035);
   h_mlepb_1lepW_stack_4->GetZaxis()->SetTitleFont(42);
   hs->Add(h_mlepb_1lepW_stack_4,"");
   hs->Draw("hist");
   
   TH1D *h_mlepb_1lepTop__43 = new TH1D("h_mlepb_1lepTop__43","",24,0,600);
   h_mlepb_1lepTop__43->SetBinContent(1,17.77358);
   h_mlepb_1lepTop__43->SetBinContent(2,342.5509);
   h_mlepb_1lepTop__43->SetBinContent(3,788.1766);
   h_mlepb_1lepTop__43->SetBinContent(4,1050.403);
   h_mlepb_1lepTop__43->SetBinContent(5,1136.565);
   h_mlepb_1lepTop__43->SetBinContent(6,991.2758);
   h_mlepb_1lepTop__43->SetBinContent(7,855.0808);
   h_mlepb_1lepTop__43->SetBinContent(8,744.9592);
   h_mlepb_1lepTop__43->SetBinContent(9,660.2821);
   h_mlepb_1lepTop__43->SetBinContent(10,572.144);
   h_mlepb_1lepTop__43->SetBinContent(11,497.4255);
   h_mlepb_1lepTop__43->SetBinContent(12,422.9003);
   h_mlepb_1lepTop__43->SetBinContent(13,350.9745);
   h_mlepb_1lepTop__43->SetBinContent(14,302.1162);
   h_mlepb_1lepTop__43->SetBinContent(15,258.4431);
   h_mlepb_1lepTop__43->SetBinContent(16,239.9718);
   h_mlepb_1lepTop__43->SetBinContent(17,199.0975);
   h_mlepb_1lepTop__43->SetBinContent(18,171.8005);
   h_mlepb_1lepTop__43->SetBinContent(19,157.2825);
   h_mlepb_1lepTop__43->SetBinContent(20,129.5295);
   h_mlepb_1lepTop__43->SetBinContent(21,125.6242);
   h_mlepb_1lepTop__43->SetBinContent(22,107.972);
   h_mlepb_1lepTop__43->SetBinContent(23,103.3019);
   h_mlepb_1lepTop__43->SetBinContent(24,917.349);
   h_mlepb_1lepTop__43->SetBinError(1,1.254299);
   h_mlepb_1lepTop__43->SetBinError(2,11.17071);
   h_mlepb_1lepTop__43->SetBinError(3,12.56624);
   h_mlepb_1lepTop__43->SetBinError(4,12.75267);
   h_mlepb_1lepTop__43->SetBinError(5,17.65934);
   h_mlepb_1lepTop__43->SetBinError(6,12.6429);
   h_mlepb_1lepTop__43->SetBinError(7,17.18278);
   h_mlepb_1lepTop__43->SetBinError(8,11.12389);
   h_mlepb_1lepTop__43->SetBinError(9,15.5085);
   h_mlepb_1lepTop__43->SetBinError(10,11.16315);
   h_mlepb_1lepTop__43->SetBinError(11,13.68372);
   h_mlepb_1lepTop__43->SetBinError(12,7.676962);
   h_mlepb_1lepTop__43->SetBinError(13,6.10574);
   h_mlepb_1lepTop__43->SetBinError(14,6.216469);
   h_mlepb_1lepTop__43->SetBinError(15,5.218899);
   h_mlepb_1lepTop__43->SetBinError(16,6.797097);
   h_mlepb_1lepTop__43->SetBinError(17,4.669855);
   h_mlepb_1lepTop__43->SetBinError(18,5.073007);
   h_mlepb_1lepTop__43->SetBinError(19,8.118292);
   h_mlepb_1lepTop__43->SetBinError(20,3.118667);
   h_mlepb_1lepTop__43->SetBinError(21,4.107966);
   h_mlepb_1lepTop__43->SetBinError(22,4.438583);
   h_mlepb_1lepTop__43->SetBinError(23,7.01777);
   h_mlepb_1lepTop__43->SetBinError(24,14.47024);
   h_mlepb_1lepTop__43->SetEntries(451266);

   ci = TColor::GetColor("#333333");
   h_mlepb_1lepTop__43->SetFillColor(ci);
   h_mlepb_1lepTop__43->SetFillStyle(3244);
   h_mlepb_1lepTop__43->GetXaxis()->SetTitle("M_{#it{l}b} [GeV]");
   h_mlepb_1lepTop__43->GetXaxis()->SetLabelFont(42);
   h_mlepb_1lepTop__43->GetXaxis()->SetLabelSize(0.035);
   h_mlepb_1lepTop__43->GetXaxis()->SetTitleSize(0.035);
   h_mlepb_1lepTop__43->GetXaxis()->SetTitleFont(42);
   h_mlepb_1lepTop__43->GetYaxis()->SetLabelFont(42);
   h_mlepb_1lepTop__43->GetYaxis()->SetLabelSize(0.035);
   h_mlepb_1lepTop__43->GetYaxis()->SetTitleSize(0.035);
   h_mlepb_1lepTop__43->GetYaxis()->SetTitleOffset(0);
   h_mlepb_1lepTop__43->GetYaxis()->SetTitleFont(42);
   h_mlepb_1lepTop__43->GetZaxis()->SetLabelFont(42);
   h_mlepb_1lepTop__43->GetZaxis()->SetLabelSize(0.035);
   h_mlepb_1lepTop__43->GetZaxis()->SetTitleSize(0.035);
   h_mlepb_1lepTop__43->GetZaxis()->SetTitleFont(42);
   h_mlepb_1lepTop__43->Draw("E2SAME");
   
   TH1D *h_mlepb__44 = new TH1D("h_mlepb__44","",24,0,600);
   h_mlepb__44->SetBinContent(1,24);
   h_mlepb__44->SetBinContent(2,359);
   h_mlepb__44->SetBinContent(3,794);
   h_mlepb__44->SetBinContent(4,1040);
   h_mlepb__44->SetBinContent(5,1136);
   h_mlepb__44->SetBinContent(6,1011);
   h_mlepb__44->SetBinContent(7,876);
   h_mlepb__44->SetBinContent(8,808);
   h_mlepb__44->SetBinContent(9,649);
   h_mlepb__44->SetBinContent(10,523);
   h_mlepb__44->SetBinContent(11,488);
   h_mlepb__44->SetBinContent(12,450);
   h_mlepb__44->SetBinContent(13,361);
   h_mlepb__44->SetBinContent(14,287);
   h_mlepb__44->SetBinContent(15,265);
   h_mlepb__44->SetBinContent(16,231);
   h_mlepb__44->SetBinContent(17,212);
   h_mlepb__44->SetBinContent(18,173);
   h_mlepb__44->SetBinContent(19,179);
   h_mlepb__44->SetBinContent(20,119);
   h_mlepb__44->SetBinContent(21,111);
   h_mlepb__44->SetBinContent(22,95);
   h_mlepb__44->SetBinContent(23,95);
   h_mlepb__44->SetBinContent(24,857);
   h_mlepb__44->SetBinError(1,4.898979);
   h_mlepb__44->SetBinError(2,18.9473);
   h_mlepb__44->SetBinError(3,28.17801);
   h_mlepb__44->SetBinError(4,32.24903);
   h_mlepb__44->SetBinError(5,33.7046);
   h_mlepb__44->SetBinError(6,31.79623);
   h_mlepb__44->SetBinError(7,29.5973);
   h_mlepb__44->SetBinError(8,28.42534);
   h_mlepb__44->SetBinError(9,25.47548);
   h_mlepb__44->SetBinError(10,22.86919);
   h_mlepb__44->SetBinError(11,22.09072);
   h_mlepb__44->SetBinError(12,21.2132);
   h_mlepb__44->SetBinError(13,19);
   h_mlepb__44->SetBinError(14,16.94107);
   h_mlepb__44->SetBinError(15,16.27882);
   h_mlepb__44->SetBinError(16,15.19868);
   h_mlepb__44->SetBinError(17,14.56022);
   h_mlepb__44->SetBinError(18,13.15295);
   h_mlepb__44->SetBinError(19,13.37909);
   h_mlepb__44->SetBinError(20,10.90871);
   h_mlepb__44->SetBinError(21,10.53565);
   h_mlepb__44->SetBinError(22,9.746794);
   h_mlepb__44->SetBinError(23,9.746794);
   h_mlepb__44->SetBinError(24,29.27456);
   h_mlepb__44->SetEntries(11145);
   h_mlepb__44->SetMarkerStyle(20);
   h_mlepb__44->SetMarkerSize(0.8);
   h_mlepb__44->GetXaxis()->SetTitle("M_{#it{l}b} [GeV]");
   h_mlepb__44->GetXaxis()->SetLabelFont(42);
   h_mlepb__44->GetXaxis()->SetLabelSize(0.035);
   h_mlepb__44->GetXaxis()->SetTitleSize(0.035);
   h_mlepb__44->GetXaxis()->SetTitleFont(42);
   h_mlepb__44->GetYaxis()->SetLabelFont(42);
   h_mlepb__44->GetYaxis()->SetLabelSize(0.035);
   h_mlepb__44->GetYaxis()->SetTitleSize(0.035);
   h_mlepb__44->GetYaxis()->SetTitleOffset(0);
   h_mlepb__44->GetYaxis()->SetTitleFont(42);
   h_mlepb__44->GetZaxis()->SetLabelFont(42);
   h_mlepb__44->GetZaxis()->SetLabelSize(0.035);
   h_mlepb__44->GetZaxis()->SetTitleSize(0.035);
   h_mlepb__44->GetZaxis()->SetTitleFont(42);
   // h_mlepb__44->Draw("SAME E0");
   
   Double_t _fx3112[24] = {
   12.5,
   37.5,
   62.5,
   87.5,
   112.5,
   137.5,
   162.5,
   187.5,
   212.5,
   237.5,
   262.5,
   287.5,
   312.5,
   337.5,
   362.5,
   387.5,
   412.5,
   437.5,
   462.5,
   487.5,
   512.5,
   537.5,
   562.5,
   587.5};
   Double_t _fy3112[24] = {
   24,
   359,
   794,
   1040,
   1136,
   1011,
   876,
   808,
   649,
   523,
   488,
   450,
   361,
   287,
   265,
   231,
   212,
   173,
   179,
   119,
   111,
   95,
   95,
   857};
   Double_t _felx3112[24] = {
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5};
   Double_t _fely3112[24] = {
   4.864704,
   18.93887,
   28.17268,
   32.24454,
   33.70036,
   31.79165,
   29.59229,
   28.42007,
   25.46946,
   22.86237,
   22.08363,
   21.20578,
   18.9916,
   16.93156,
   16.26889,
   15.188,
   14.54904,
   13.1405,
   13.36686,
   10.89359,
   10.51998,
   9.729809,
   9.729809,
   29.26948};
   Double_t _fehx3112[24] = {
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5};
   Double_t _fehy3112[24] = {
   5.967055,
   19.96533,
   29.19047,
   33.26009,
   34.71524,
   32.80742,
   30.60922,
   29.43771,
   26.48914,
   23.88429,
   23.10632,
   22.2294,
   20.01799,
   17.96115,
   17.29968,
   16.22098,
   15.58346,
   14.17861,
   14.40433,
   11.93955,
   11.56756,
   10.78125,
   10.78125,
   30.28661};
   TGraphAsymmErrors *grae = new TGraphAsymmErrors(24,_fx3112,_fy3112,_felx3112,_fehx3112,_fely3112,_fehy3112);
   grae->SetName("");
   grae->SetTitle("");
   grae->SetFillColor(1);
   grae->SetMarkerStyle(20);
   grae->SetMarkerSize(0.8);
   
   TH1F *Graph_Graph3112 = new TH1F("Graph_Graph3112","",100,0,660);
   Graph_Graph3112->SetMinimum(17.22177);
   Graph_Graph3112->SetMaximum(1285.873);
   Graph_Graph3112->SetDirectory(0);
   Graph_Graph3112->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph3112->SetLineColor(ci);
   Graph_Graph3112->GetXaxis()->SetLabelFont(42);
   Graph_Graph3112->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph3112->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph3112->GetXaxis()->SetTitleFont(42);
   Graph_Graph3112->GetYaxis()->SetLabelFont(42);
   Graph_Graph3112->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph3112->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph3112->GetYaxis()->SetTitleOffset(0);
   Graph_Graph3112->GetYaxis()->SetTitleFont(42);
   Graph_Graph3112->GetZaxis()->SetLabelFont(42);
   Graph_Graph3112->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph3112->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph3112->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_Graph3112);
   
   grae->Draw(" pz");

   TLegend *leg = new TLegend(0.23,0.65,0.89,0.915,NULL,"brNDC");
   leg->SetNColumns(2);
   leg->SetBorderSize(0);
   leg->SetTextSize(0.042);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);

   TLegendEntry *entry;
   entry=leg->AddEntry("h_met_2lep","Lost lepton","f");
   ci = TColor::GetColor("#66ccff");
   entry->SetFillColor(ci);
   entry->SetFillStyle(1001);
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);

   entry=leg->AddEntry("h_met_1lepTop","1#font[12]{l} (from t)","f");
   ci = TColor::GetColor("#ff6666");
   entry->SetFillColor(ci);
   entry->SetFillStyle(1001);
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);

   entry=leg->AddEntry("h_met_1lepW","1#font[12]{l} (not from t)","f");
   ci = TColor::GetColor("#ffcc66");
   entry->SetFillColor(ci);
   entry->SetFillStyle(1001);
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);

   entry=leg->AddEntry("h_met_Znunu","Z#rightarrow #nu#bar{#nu}","f");
   ci = TColor::GetColor("#9966cc");
   entry->SetFillColor(ci);
   entry->SetFillStyle(1001);
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);

   entry=leg->AddEntry("h_mlepb_Znunu","Stat. unc.","f");
   ci = TColor::GetColor("#333333");
   entry->SetFillColor(ci);
   entry->SetFillStyle(3244);
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(0);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);

   entry=leg->AddEntry("h_met","Observed","lpf");
   entry->SetFillStyle(1001);
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(20);
   entry->SetMarkerSize(0.8);
   entry->SetTextFont(42);

   leg->Draw();
   TLatex *   tex = new TLatex(0.89,0.93,"137 fb^{-1} (13 TeV)");
   tex->SetNDC();
   tex->SetTextAlign(31);
   tex->SetTextFont(42);
   tex->SetTextSize(0.045);
   tex->SetLineWidth(2);
   tex->Draw();
   tex = new TLatex(0.13,0.93,"CMS");
   tex->SetNDC();
   tex->SetTextSize(0.056);
   tex->SetLineWidth(2);
   tex->Draw();

   ratioPad->cd();
   
   TH1D* h_axis_ratio = new TH1D("ratio_axis","", 24,0,600);
   h_axis_ratio->GetYaxis()->SetNdivisions(4);
   h_axis_ratio->GetYaxis()->SetRangeUser(0, 2);
   h_axis_ratio->GetYaxis()->SetTitle("Obs. / Sim.");
   h_axis_ratio->GetYaxis()->SetTitleOffset(0.28);
   h_axis_ratio->GetYaxis()->SetTitleSize(0.185);
   h_axis_ratio->GetYaxis()->SetLabelSize(0.18);
   h_axis_ratio->GetYaxis()->SetTickLength(0.01);
   h_axis_ratio->GetXaxis()->SetTickLength(0.07);
   h_axis_ratio->GetXaxis()->SetTitleSize(0.0);
   h_axis_ratio->GetXaxis()->SetLabelSize(0.0);
   h_axis_ratio->Draw("axis");

   TH1D *h_mlepb_Znunu__151 = new TH1D("h_mlepb_Znunu__151","",24,0,600);
   h_mlepb_Znunu__151->SetBinContent(1,1);
   h_mlepb_Znunu__151->SetBinContent(2,1);
   h_mlepb_Znunu__151->SetBinContent(3,1);
   h_mlepb_Znunu__151->SetBinContent(4,1);
   h_mlepb_Znunu__151->SetBinContent(5,1);
   h_mlepb_Znunu__151->SetBinContent(6,1);
   h_mlepb_Znunu__151->SetBinContent(7,1);
   h_mlepb_Znunu__151->SetBinContent(8,1);
   h_mlepb_Znunu__151->SetBinContent(9,1);
   h_mlepb_Znunu__151->SetBinContent(10,1);
   h_mlepb_Znunu__151->SetBinContent(11,1);
   h_mlepb_Znunu__151->SetBinContent(12,1);
   h_mlepb_Znunu__151->SetBinContent(13,1);
   h_mlepb_Znunu__151->SetBinContent(14,1);
   h_mlepb_Znunu__151->SetBinContent(15,1);
   h_mlepb_Znunu__151->SetBinContent(16,1);
   h_mlepb_Znunu__151->SetBinContent(17,1);
   h_mlepb_Znunu__151->SetBinContent(18,1);
   h_mlepb_Znunu__151->SetBinContent(19,1);
   h_mlepb_Znunu__151->SetBinContent(20,1);
   h_mlepb_Znunu__151->SetBinContent(21,1);
   h_mlepb_Znunu__151->SetBinContent(22,1);
   h_mlepb_Znunu__151->SetBinContent(23,1);
   h_mlepb_Znunu__151->SetBinContent(24,1);
   h_mlepb_Znunu__151->SetBinError(1,0.070571);
   h_mlepb_Znunu__151->SetBinError(2,0.03261036);
   h_mlepb_Znunu__151->SetBinError(3,0.01594344);
   h_mlepb_Znunu__151->SetBinError(4,0.01214074);
   h_mlepb_Znunu__151->SetBinError(5,0.01553746);
   h_mlepb_Znunu__151->SetBinError(6,0.01275417);
   h_mlepb_Znunu__151->SetBinError(7,0.02009492);
   h_mlepb_Znunu__151->SetBinError(8,0.01493221);
   h_mlepb_Znunu__151->SetBinError(9,0.02348769);
   h_mlepb_Znunu__151->SetBinError(10,0.01951109);
   h_mlepb_Znunu__151->SetBinError(11,0.02750908);
   h_mlepb_Znunu__151->SetBinError(12,0.01815312);
   h_mlepb_Znunu__151->SetBinError(13,0.01739654);
   h_mlepb_Znunu__151->SetBinError(14,0.02057642);
   h_mlepb_Znunu__151->SetBinError(15,0.02019361);
   h_mlepb_Znunu__151->SetBinError(16,0.02832457);
   h_mlepb_Znunu__151->SetBinError(17,0.02345512);
   h_mlepb_Znunu__151->SetBinError(18,0.02952847);
   h_mlepb_Znunu__151->SetBinError(19,0.05161601);
   h_mlepb_Znunu__151->SetBinError(20,0.02407689);
   h_mlepb_Znunu__151->SetBinError(21,0.03270042);
   h_mlepb_Znunu__151->SetBinError(22,0.04110863);
   h_mlepb_Znunu__151->SetBinError(23,0.06793456);
   h_mlepb_Znunu__151->SetBinError(24,0.01577398);
   h_mlepb_Znunu__151->SetEntries(451290);

   ci = TColor::GetColor("#333333");
   h_mlepb_Znunu__151->SetFillColor(ci);
   h_mlepb_Znunu__151->SetFillStyle(3244);
   h_mlepb_Znunu__151->GetXaxis()->SetTitle("M_{#it{l}b} [GeV]");
   h_mlepb_Znunu__151->GetXaxis()->SetLabelFont(42);
   h_mlepb_Znunu__151->GetXaxis()->SetLabelSize(0.035);
   h_mlepb_Znunu__151->GetXaxis()->SetTitleSize(0.035);
   h_mlepb_Znunu__151->GetXaxis()->SetTitleFont(42);
   h_mlepb_Znunu__151->GetYaxis()->SetLabelFont(42);
   h_mlepb_Znunu__151->GetYaxis()->SetLabelSize(0.035);
   h_mlepb_Znunu__151->GetYaxis()->SetTitleSize(0.035);
   h_mlepb_Znunu__151->GetYaxis()->SetTitleOffset(0);
   h_mlepb_Znunu__151->GetYaxis()->SetTitleFont(42);
   h_mlepb_Znunu__151->GetZaxis()->SetLabelFont(42);
   h_mlepb_Znunu__151->GetZaxis()->SetLabelSize(0.035);
   h_mlepb_Znunu__151->GetZaxis()->SetTitleSize(0.035);
   h_mlepb_Znunu__151->GetZaxis()->SetTitleFont(42);
   h_mlepb_Znunu__151->Draw("SAME E2");

   TLine* hline_1 = new TLine(0, 1, 600, 1);
   hline_1->SetLineStyle(2);
   hline_1->SetLineWidth(2);
   hline_1->SetLineColor(kGray+2);
   hline_1->Draw();

   Double_t _fx3114[24] = {
   12.5,
   37.5,
   62.5,
   87.5,
   112.5,
   137.5,
   162.5,
   187.5,
   212.5,
   237.5,
   262.5,
   287.5,
   312.5,
   337.5,
   362.5,
   387.5,
   412.5,
   437.5,
   462.5,
   487.5,
   512.5,
   537.5,
   562.5,
   587.5};
   Double_t _fy3114[24] = {
   1.350319,
   1.048019,
   1.007388,
   0.9900959,
   0.9995026,
   1.019898,
   1.024465,
   1.084623,
   0.9829132,
   0.9141056,
   0.9810514,
   1.064081,
   1.028565,
   0.9499657,
   1.025371,
   0.9626133,
   1.064805,
   1.006982,
   1.13808,
   0.91871,
   0.8835874,
   0.8798574,
   0.9196344,
   0.9342137};
   Double_t _felx3114[24] = {
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5};
   Double_t _fely3114[24] = {
   0.2737042,
   0.05528777,
   0.03574412,
   0.0306973,
   0.02965106,
   0.03207145,
   0.03460759,
   0.03814984,
   0.03857361,
   0.03995913,
   0.04439585,
   0.05014367,
   0.05411107,
   0.05604322,
   0.06294961,
   0.06329078,
   0.07307495,
   0.07648696,
   0.08498636,
   0.08410126,
   0.08374162,
   0.09011415,
   0.09418807,
   0.03190659};
   Double_t _fehx3114[24] = {
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5,
   12.5};
   Double_t _fehy3114[24] = {
   0.335726,
   0.05828427,
   0.03703544,
   0.03166411,
   0.03054399,
   0.03309616,
   0.03579688,
   0.03951587,
   0.04011791,
   0.04174525,
   0.04645181,
   0.05256417,
   0.05703545,
   0.05945114,
   0.06693807,
   0.06759537,
   0.07827053,
   0.0825295,
   0.09158256,
   0.09217631,
   0.09208066,
   0.09985222,
   0.1043664,
   0.03301536};
   // TGraphAsymmErrors *grae = new TGraphAsymmErrors(24,_fx3114,_fy3114,_felx3114,_fehx3114,_fely3114,_fehy3114);
   grae = new TGraphAsymmErrors(24,_fx3114,_fy3114,_felx3114,_fehx3114,_fely3114,_fehy3114);
   grae->SetName("");
   grae->SetTitle("");
   grae->SetFillColor(1);
   grae->SetMarkerStyle(20);
   grae->SetMarkerSize(0.8);
   
   TH1F *Graph_Graph_Graph31133114 = new TH1F("Graph_Graph_Graph31133114","",100,0,660);
   Graph_Graph_Graph31133114->SetMinimum(0.7001131);
   Graph_Graph_Graph31133114->SetMaximum(1.775675);
   Graph_Graph_Graph31133114->SetDirectory(0);
   Graph_Graph_Graph31133114->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph_Graph31133114->SetLineColor(ci);
   Graph_Graph_Graph31133114->GetXaxis()->SetLabelFont(42);
   Graph_Graph_Graph31133114->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph_Graph31133114->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph_Graph31133114->GetXaxis()->SetTitleFont(42);
   Graph_Graph_Graph31133114->GetYaxis()->SetLabelFont(42);
   Graph_Graph_Graph31133114->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph_Graph31133114->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph_Graph31133114->GetYaxis()->SetTitleOffset(0);
   Graph_Graph_Graph31133114->GetYaxis()->SetTitleFont(42);
   Graph_Graph_Graph31133114->GetZaxis()->SetLabelFont(42);
   Graph_Graph_Graph31133114->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph_Graph31133114->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph_Graph31133114->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_Graph_Graph31133114);
   
   grae->Draw(" pz");


   c1_n44->Modified();
   c1_n44->cd();
   c1_n44->SetSelected(c1_n44);

   c1_n44->Print("cr0bbase_mlepb_mcerr.pdf");
}
