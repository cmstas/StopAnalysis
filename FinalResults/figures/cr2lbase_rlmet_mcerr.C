void cr2lbase_rlmet_mcerr()
{
//=========Macro generated from canvas: c1_n22/c1_n22
//=========  (Tue Sep 24 13:06:02 2019) by ROOT version6.10/09
   TCanvas *c1_n22 = new TCanvas("c1_n22", "c1_n22",0,0,700,700);
   gStyle->SetOptStat(0);
   c1_n22->SetHighLightColor(2);
   c1_n22->Range(157.6923,-1.418794,926.9231,6.343633);
   c1_n22->SetFillColor(0);
   c1_n22->SetBorderMode(0);
   c1_n22->SetBorderSize(2);
   c1_n22->SetLogy();
   c1_n22->SetTicky(1);
   c1_n22->SetLeftMargin(0.12);
   c1_n22->SetTopMargin(0.08);
   c1_n22->SetBottomMargin(0.12);
   c1_n22->SetFrameBorderMode(0);
   c1_n22->SetFrameBorderMode(0);
   
   TPad* mainPad = new TPad("pad1", "pad1", 0.0, 0.20, 1.0, 0.99);
   mainPad->Range(157.6923,-1.418794,926.9231,6.343633);
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
   hs->SetMinimum(0.8508261);
   hs->SetMaximum(252109.8);
   
   TH1F *hs_stack_12 = new TH1F("hs_stack_12","",24,250,850);
   hs_stack_12->SetMinimum(0.2277448);
   hs_stack_12->SetMaximum(528006.2);
   hs_stack_12->SetDirectory(0);
   hs_stack_12->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   hs_stack_12->SetLineColor(ci);
   hs_stack_12->GetXaxis()->SetTitle("p_{T}^{miss} [GeV]");
   hs_stack_12->GetXaxis()->SetRange(1,24);
   hs_stack_12->GetXaxis()->SetLabelFont(42);
   hs_stack_12->GetXaxis()->SetTitleOffset(1.1);
   hs_stack_12->GetXaxis()->SetTitleFont(42);
   hs_stack_12->GetYaxis()->SetTitle("Events / 25 GeV");
   hs_stack_12->GetYaxis()->SetLabelFont(42);
   hs_stack_12->GetYaxis()->SetTitleOffset(1.2);
   hs_stack_12->GetYaxis()->SetTitleFont(42);
   hs_stack_12->GetZaxis()->SetLabelFont(42);
   hs_stack_12->GetZaxis()->SetLabelSize(0.035);
   hs_stack_12->GetZaxis()->SetTitleSize(0.035);
   hs_stack_12->GetXaxis()->SetLabelSize(0.042);
   hs_stack_12->GetXaxis()->SetTitleSize(0.045);
   hs_stack_12->GetYaxis()->SetLabelSize(0.042);
   hs_stack_12->GetYaxis()->SetTitleSize(0.045);
   hs_stack_12->GetZaxis()->SetTitleFont(42);
   hs->SetHistogram(hs_stack_12);
   
   
   TH1D *h_rlmet_Znunu_stack_1 = new TH1D("h_rlmet_Znunu_stack_1","",24,250,850);
   h_rlmet_Znunu_stack_1->SetBinContent(1,4.777537);
   h_rlmet_Znunu_stack_1->SetBinContent(2,3.401388);
   h_rlmet_Znunu_stack_1->SetBinContent(3,2.30705);
   h_rlmet_Znunu_stack_1->SetBinContent(4,1.644521);
   h_rlmet_Znunu_stack_1->SetBinContent(5,1.298744);
   h_rlmet_Znunu_stack_1->SetBinContent(6,0.8458574);
   h_rlmet_Znunu_stack_1->SetBinContent(7,0.6101733);
   h_rlmet_Znunu_stack_1->SetBinContent(8,0.4975399);
   h_rlmet_Znunu_stack_1->SetBinContent(9,0.273677);
   h_rlmet_Znunu_stack_1->SetBinContent(10,0.3817974);
   h_rlmet_Znunu_stack_1->SetBinContent(11,0.1603267);
   h_rlmet_Znunu_stack_1->SetBinContent(12,0.1406987);
   h_rlmet_Znunu_stack_1->SetBinContent(13,0.04611287);
   h_rlmet_Znunu_stack_1->SetBinContent(14,0.1053129);
   h_rlmet_Znunu_stack_1->SetBinContent(15,0.05064935);
   h_rlmet_Znunu_stack_1->SetBinContent(16,0.04240985);
   h_rlmet_Znunu_stack_1->SetBinContent(17,0.04256537);
   h_rlmet_Znunu_stack_1->SetBinContent(18,0.01780579);
   h_rlmet_Znunu_stack_1->SetBinContent(19,0.0311505);
   h_rlmet_Znunu_stack_1->SetBinContent(20,0.0184166);
   h_rlmet_Znunu_stack_1->SetBinContent(21,0.01001228);
   h_rlmet_Znunu_stack_1->SetBinContent(22,0.02009618);
   h_rlmet_Znunu_stack_1->SetBinContent(23,0.004898696);
   h_rlmet_Znunu_stack_1->SetBinContent(24,0.007691244);
   h_rlmet_Znunu_stack_1->SetBinError(1,0.1632585);
   h_rlmet_Znunu_stack_1->SetBinError(2,0.1955941);
   h_rlmet_Znunu_stack_1->SetBinError(3,0.1156932);
   h_rlmet_Znunu_stack_1->SetBinError(4,0.1031267);
   h_rlmet_Znunu_stack_1->SetBinError(5,0.1371882);
   h_rlmet_Znunu_stack_1->SetBinError(6,0.1222213);
   h_rlmet_Znunu_stack_1->SetBinError(7,0.0601067);
   h_rlmet_Znunu_stack_1->SetBinError(8,0.0539253);
   h_rlmet_Znunu_stack_1->SetBinError(9,0.05054944);
   h_rlmet_Znunu_stack_1->SetBinError(10,0.09854809);
   h_rlmet_Znunu_stack_1->SetBinError(11,0.03122804);
   h_rlmet_Znunu_stack_1->SetBinError(12,0.04703401);
   h_rlmet_Znunu_stack_1->SetBinError(13,0.024643);
   h_rlmet_Znunu_stack_1->SetBinError(14,0.02097703);
   h_rlmet_Znunu_stack_1->SetBinError(15,0.01688448);
   h_rlmet_Znunu_stack_1->SetBinError(16,0.01669043);
   h_rlmet_Znunu_stack_1->SetBinError(17,0.01515085);
   h_rlmet_Znunu_stack_1->SetBinError(18,0.01202133);
   h_rlmet_Znunu_stack_1->SetBinError(19,0.01029999);
   h_rlmet_Znunu_stack_1->SetBinError(20,0.01093989);
   h_rlmet_Znunu_stack_1->SetBinError(21,0.007749385);
   h_rlmet_Znunu_stack_1->SetBinError(22,0.00741684);
   h_rlmet_Znunu_stack_1->SetBinError(23,0.005035961);
   h_rlmet_Znunu_stack_1->SetBinError(24,0.0141922);
   h_rlmet_Znunu_stack_1->SetEntries(16956);

   ci = TColor::GetColor("#9966cc");
   h_rlmet_Znunu_stack_1->SetFillColor(ci);
   h_rlmet_Znunu_stack_1->GetXaxis()->SetTitle("#slash{E}_{T} (removed lepton) [GeV]");
   h_rlmet_Znunu_stack_1->GetXaxis()->SetLabelFont(42);
   h_rlmet_Znunu_stack_1->GetXaxis()->SetLabelSize(0.035);
   h_rlmet_Znunu_stack_1->GetXaxis()->SetTitleSize(0.035);
   h_rlmet_Znunu_stack_1->GetXaxis()->SetTitleFont(42);
   h_rlmet_Znunu_stack_1->GetYaxis()->SetLabelFont(42);
   h_rlmet_Znunu_stack_1->GetYaxis()->SetLabelSize(0.035);
   h_rlmet_Znunu_stack_1->GetYaxis()->SetTitleSize(0.035);
   h_rlmet_Znunu_stack_1->GetYaxis()->SetTitleOffset(0);
   h_rlmet_Znunu_stack_1->GetYaxis()->SetTitleFont(42);
   h_rlmet_Znunu_stack_1->GetZaxis()->SetLabelFont(42);
   h_rlmet_Znunu_stack_1->GetZaxis()->SetLabelSize(0.035);
   h_rlmet_Znunu_stack_1->GetZaxis()->SetTitleSize(0.035);
   h_rlmet_Znunu_stack_1->GetZaxis()->SetTitleFont(42);
   hs->Add(h_rlmet_Znunu_stack_1,"");
   
   TH1D *h_rlmet_1lepW_stack_2 = new TH1D("h_rlmet_1lepW_stack_2","",24,250,850);
   h_rlmet_1lepW_stack_2->SetBinContent(1,17.55516);
   h_rlmet_1lepW_stack_2->SetBinContent(2,10.22799);
   h_rlmet_1lepW_stack_2->SetBinContent(3,8.629184);
   h_rlmet_1lepW_stack_2->SetBinContent(4,6.615638);
   h_rlmet_1lepW_stack_2->SetBinContent(5,3.543532);
   h_rlmet_1lepW_stack_2->SetBinContent(6,1.423673);
   h_rlmet_1lepW_stack_2->SetBinContent(7,3.06734);
   h_rlmet_1lepW_stack_2->SetBinContent(8,1.557252);
   h_rlmet_1lepW_stack_2->SetBinContent(9,1.040745);
   h_rlmet_1lepW_stack_2->SetBinContent(10,2.762116);
   h_rlmet_1lepW_stack_2->SetBinContent(11,0.3226921);
   h_rlmet_1lepW_stack_2->SetBinContent(12,0.6383928);
   h_rlmet_1lepW_stack_2->SetBinContent(13,0.6071425);
   h_rlmet_1lepW_stack_2->SetBinContent(14,0.1727536);
   h_rlmet_1lepW_stack_2->SetBinContent(15,0.3203739);
   h_rlmet_1lepW_stack_2->SetBinContent(16,0.1621923);
   h_rlmet_1lepW_stack_2->SetBinContent(17,0.2747615);
   h_rlmet_1lepW_stack_2->SetBinContent(18,0.05637334);
   h_rlmet_1lepW_stack_2->SetBinContent(19,0.2557708);
   h_rlmet_1lepW_stack_2->SetBinContent(20,0.02595697);
   h_rlmet_1lepW_stack_2->SetBinContent(21,0.0355945);
   h_rlmet_1lepW_stack_2->SetBinContent(22,0.009592222);
   h_rlmet_1lepW_stack_2->SetBinContent(23,0.0612697);
   h_rlmet_1lepW_stack_2->SetBinContent(24,0.3163231);
   h_rlmet_1lepW_stack_2->SetBinError(1,4.728792);
   h_rlmet_1lepW_stack_2->SetBinError(2,1.774184);
   h_rlmet_1lepW_stack_2->SetBinError(3,3.054784);
   h_rlmet_1lepW_stack_2->SetBinError(4,2.006266);
   h_rlmet_1lepW_stack_2->SetBinError(5,0.5217049);
   h_rlmet_1lepW_stack_2->SetBinError(6,0.2360372);
   h_rlmet_1lepW_stack_2->SetBinError(7,0.9863162);
   h_rlmet_1lepW_stack_2->SetBinError(8,0.3335789);
   h_rlmet_1lepW_stack_2->SetBinError(9,0.251582);
   h_rlmet_1lepW_stack_2->SetBinError(10,1.906464);
   h_rlmet_1lepW_stack_2->SetBinError(11,0.07642696);
   h_rlmet_1lepW_stack_2->SetBinError(12,0.2736778);
   h_rlmet_1lepW_stack_2->SetBinError(13,0.2424414);
   h_rlmet_1lepW_stack_2->SetBinError(14,0.05247143);
   h_rlmet_1lepW_stack_2->SetBinError(15,0.1579645);
   h_rlmet_1lepW_stack_2->SetBinError(16,0.1190016);
   h_rlmet_1lepW_stack_2->SetBinError(17,0.1568887);
   h_rlmet_1lepW_stack_2->SetBinError(18,0.04050584);
   h_rlmet_1lepW_stack_2->SetBinError(19,0.1712478);
   h_rlmet_1lepW_stack_2->SetBinError(20,0.01842761);
   h_rlmet_1lepW_stack_2->SetBinError(21,0.02109715);
   h_rlmet_1lepW_stack_2->SetBinError(22,0.009592222);
   h_rlmet_1lepW_stack_2->SetBinError(23,0.04916442);
   h_rlmet_1lepW_stack_2->SetBinError(24,0.1481096);
   h_rlmet_1lepW_stack_2->SetEntries(1796);

   ci = TColor::GetColor("#ffcc66");
   h_rlmet_1lepW_stack_2->SetFillColor(ci);
   h_rlmet_1lepW_stack_2->GetXaxis()->SetTitle("#slash{E}_{T} (removed lepton) [GeV]");
   h_rlmet_1lepW_stack_2->GetXaxis()->SetLabelFont(42);
   h_rlmet_1lepW_stack_2->GetXaxis()->SetLabelSize(0.035);
   h_rlmet_1lepW_stack_2->GetXaxis()->SetTitleSize(0.035);
   h_rlmet_1lepW_stack_2->GetXaxis()->SetTitleFont(42);
   h_rlmet_1lepW_stack_2->GetYaxis()->SetLabelFont(42);
   h_rlmet_1lepW_stack_2->GetYaxis()->SetLabelSize(0.035);
   h_rlmet_1lepW_stack_2->GetYaxis()->SetTitleSize(0.035);
   h_rlmet_1lepW_stack_2->GetYaxis()->SetTitleOffset(0);
   h_rlmet_1lepW_stack_2->GetYaxis()->SetTitleFont(42);
   h_rlmet_1lepW_stack_2->GetZaxis()->SetLabelFont(42);
   h_rlmet_1lepW_stack_2->GetZaxis()->SetLabelSize(0.035);
   h_rlmet_1lepW_stack_2->GetZaxis()->SetTitleSize(0.035);
   h_rlmet_1lepW_stack_2->GetZaxis()->SetTitleFont(42);
   hs->Add(h_rlmet_1lepW_stack_2,"");
   
   TH1D *h_rlmet_1lepTop_stack_3 = new TH1D("h_rlmet_1lepTop_stack_3","",24,250,850);
   h_rlmet_1lepTop_stack_3->SetBinContent(1,90.66621);
   h_rlmet_1lepTop_stack_3->SetBinContent(2,60.75989);
   h_rlmet_1lepTop_stack_3->SetBinContent(3,37.49879);
   h_rlmet_1lepTop_stack_3->SetBinContent(4,25.5672);
   h_rlmet_1lepTop_stack_3->SetBinContent(5,16.1306);
   h_rlmet_1lepTop_stack_3->SetBinContent(6,12.83835);
   h_rlmet_1lepTop_stack_3->SetBinContent(7,7.707671);
   h_rlmet_1lepTop_stack_3->SetBinContent(8,7.457757);
   h_rlmet_1lepTop_stack_3->SetBinContent(9,4.879529);
   h_rlmet_1lepTop_stack_3->SetBinContent(10,3.603323);
   h_rlmet_1lepTop_stack_3->SetBinContent(11,2.141847);
   h_rlmet_1lepTop_stack_3->SetBinContent(12,0.6947945);
   h_rlmet_1lepTop_stack_3->SetBinContent(13,1.33166);
   h_rlmet_1lepTop_stack_3->SetBinContent(14,0.7391403);
   h_rlmet_1lepTop_stack_3->SetBinContent(15,0.9815971);
   h_rlmet_1lepTop_stack_3->SetBinContent(16,0.6587106);
   h_rlmet_1lepTop_stack_3->SetBinContent(17,0.08257204);
   h_rlmet_1lepTop_stack_3->SetBinContent(18,0.4220294);
   h_rlmet_1lepTop_stack_3->SetBinContent(19,0.05482928);
   h_rlmet_1lepTop_stack_3->SetBinContent(20,0.3523729);
   h_rlmet_1lepTop_stack_3->SetBinContent(21,0.09413547);
   h_rlmet_1lepTop_stack_3->SetBinContent(24,0.3076866);
   h_rlmet_1lepTop_stack_3->SetBinError(1,4.141202);
   h_rlmet_1lepTop_stack_3->SetBinError(2,3.229994);
   h_rlmet_1lepTop_stack_3->SetBinError(3,2.548208);
   h_rlmet_1lepTop_stack_3->SetBinError(4,2.053488);
   h_rlmet_1lepTop_stack_3->SetBinError(5,1.687061);
   h_rlmet_1lepTop_stack_3->SetBinError(6,1.508354);
   h_rlmet_1lepTop_stack_3->SetBinError(7,1.06945);
   h_rlmet_1lepTop_stack_3->SetBinError(8,1.120671);
   h_rlmet_1lepTop_stack_3->SetBinError(9,0.9777851);
   h_rlmet_1lepTop_stack_3->SetBinError(10,0.7843175);
   h_rlmet_1lepTop_stack_3->SetBinError(11,0.583865);
   h_rlmet_1lepTop_stack_3->SetBinError(12,0.3036296);
   h_rlmet_1lepTop_stack_3->SetBinError(13,0.5538465);
   h_rlmet_1lepTop_stack_3->SetBinError(14,0.3331795);
   h_rlmet_1lepTop_stack_3->SetBinError(15,0.4323409);
   h_rlmet_1lepTop_stack_3->SetBinError(16,0.3298313);
   h_rlmet_1lepTop_stack_3->SetBinError(17,0.06292443);
   h_rlmet_1lepTop_stack_3->SetBinError(18,0.2257144);
   h_rlmet_1lepTop_stack_3->SetBinError(19,0.05482928);
   h_rlmet_1lepTop_stack_3->SetBinError(20,0.2932502);
   h_rlmet_1lepTop_stack_3->SetBinError(21,0.09413547);
   h_rlmet_1lepTop_stack_3->SetBinError(24,0.1930142);
   h_rlmet_1lepTop_stack_3->SetEntries(2497);

   ci = TColor::GetColor("#ff6666");
   h_rlmet_1lepTop_stack_3->SetFillColor(ci);
   h_rlmet_1lepTop_stack_3->GetXaxis()->SetTitle("#slash{E}_{T} (removed lepton) [GeV]");
   h_rlmet_1lepTop_stack_3->GetXaxis()->SetLabelFont(42);
   h_rlmet_1lepTop_stack_3->GetXaxis()->SetLabelSize(0.035);
   h_rlmet_1lepTop_stack_3->GetXaxis()->SetTitleSize(0.035);
   h_rlmet_1lepTop_stack_3->GetXaxis()->SetTitleFont(42);
   h_rlmet_1lepTop_stack_3->GetYaxis()->SetLabelFont(42);
   h_rlmet_1lepTop_stack_3->GetYaxis()->SetLabelSize(0.035);
   h_rlmet_1lepTop_stack_3->GetYaxis()->SetTitleSize(0.035);
   h_rlmet_1lepTop_stack_3->GetYaxis()->SetTitleOffset(0);
   h_rlmet_1lepTop_stack_3->GetYaxis()->SetTitleFont(42);
   h_rlmet_1lepTop_stack_3->GetZaxis()->SetLabelFont(42);
   h_rlmet_1lepTop_stack_3->GetZaxis()->SetLabelSize(0.035);
   h_rlmet_1lepTop_stack_3->GetZaxis()->SetTitleSize(0.035);
   h_rlmet_1lepTop_stack_3->GetZaxis()->SetTitleFont(42);
   hs->Add(h_rlmet_1lepTop_stack_3,"");
   
   TH1D *h_rlmet_2lep_stack_4 = new TH1D("h_rlmet_2lep_stack_4","",24,250,850);
   h_rlmet_2lep_stack_4->SetBinContent(1,5017.161);
   h_rlmet_2lep_stack_4->SetBinContent(2,3040.403);
   h_rlmet_2lep_stack_4->SetBinContent(3,1837.285);
   h_rlmet_2lep_stack_4->SetBinContent(4,1172.053);
   h_rlmet_2lep_stack_4->SetBinContent(5,759.1653);
   h_rlmet_2lep_stack_4->SetBinContent(6,524.7786);
   h_rlmet_2lep_stack_4->SetBinContent(7,347.5955);
   h_rlmet_2lep_stack_4->SetBinContent(8,239.0861);
   h_rlmet_2lep_stack_4->SetBinContent(9,162.5908);
   h_rlmet_2lep_stack_4->SetBinContent(10,117.1086);
   h_rlmet_2lep_stack_4->SetBinContent(11,79.61887);
   h_rlmet_2lep_stack_4->SetBinContent(12,54.33006);
   h_rlmet_2lep_stack_4->SetBinContent(13,42.21894);
   h_rlmet_2lep_stack_4->SetBinContent(14,30.00028);
   h_rlmet_2lep_stack_4->SetBinContent(15,20.61741);
   h_rlmet_2lep_stack_4->SetBinContent(16,14.42494);
   h_rlmet_2lep_stack_4->SetBinContent(17,11.76447);
   h_rlmet_2lep_stack_4->SetBinContent(18,10.35611);
   h_rlmet_2lep_stack_4->SetBinContent(19,5.886696);
   h_rlmet_2lep_stack_4->SetBinContent(20,5.442129);
   h_rlmet_2lep_stack_4->SetBinContent(21,4.048551);
   h_rlmet_2lep_stack_4->SetBinContent(22,2.893992);
   h_rlmet_2lep_stack_4->SetBinContent(23,1.635484);
   h_rlmet_2lep_stack_4->SetBinContent(24,8.146239);
   h_rlmet_2lep_stack_4->SetBinError(1,25.94033);
   h_rlmet_2lep_stack_4->SetBinError(2,20.25693);
   h_rlmet_2lep_stack_4->SetBinError(3,15.62282);
   h_rlmet_2lep_stack_4->SetBinError(4,12.62648);
   h_rlmet_2lep_stack_4->SetBinError(5,10.06445);
   h_rlmet_2lep_stack_4->SetBinError(6,8.697204);
   h_rlmet_2lep_stack_4->SetBinError(7,7.280346);
   h_rlmet_2lep_stack_4->SetBinError(8,5.664678);
   h_rlmet_2lep_stack_4->SetBinError(9,4.753822);
   h_rlmet_2lep_stack_4->SetBinError(10,4.127148);
   h_rlmet_2lep_stack_4->SetBinError(11,3.25318);
   h_rlmet_2lep_stack_4->SetBinError(12,2.661946);
   h_rlmet_2lep_stack_4->SetBinError(13,2.415582);
   h_rlmet_2lep_stack_4->SetBinError(14,2.058824);
   h_rlmet_2lep_stack_4->SetBinError(15,1.649976);
   h_rlmet_2lep_stack_4->SetBinError(16,1.421494);
   h_rlmet_2lep_stack_4->SetBinError(17,1.267038);
   h_rlmet_2lep_stack_4->SetBinError(18,1.260488);
   h_rlmet_2lep_stack_4->SetBinError(19,0.9681419);
   h_rlmet_2lep_stack_4->SetBinError(20,0.9291071);
   h_rlmet_2lep_stack_4->SetBinError(21,0.6995392);
   h_rlmet_2lep_stack_4->SetBinError(22,0.6037367);
   h_rlmet_2lep_stack_4->SetBinError(23,0.4459107);
   h_rlmet_2lep_stack_4->SetBinError(24,0.9968373);
   h_rlmet_2lep_stack_4->SetEntries(207482);

   ci = TColor::GetColor("#66ccff");
   h_rlmet_2lep_stack_4->SetFillColor(ci);
   h_rlmet_2lep_stack_4->GetXaxis()->SetTitle("#slash{E}_{T} (removed lepton) [GeV]");
   h_rlmet_2lep_stack_4->GetXaxis()->SetLabelFont(42);
   h_rlmet_2lep_stack_4->GetXaxis()->SetLabelSize(0.035);
   h_rlmet_2lep_stack_4->GetXaxis()->SetTitleSize(0.035);
   h_rlmet_2lep_stack_4->GetXaxis()->SetTitleFont(42);
   h_rlmet_2lep_stack_4->GetYaxis()->SetLabelFont(42);
   h_rlmet_2lep_stack_4->GetYaxis()->SetLabelSize(0.035);
   h_rlmet_2lep_stack_4->GetYaxis()->SetTitleSize(0.035);
   h_rlmet_2lep_stack_4->GetYaxis()->SetTitleOffset(0);
   h_rlmet_2lep_stack_4->GetYaxis()->SetTitleFont(42);
   h_rlmet_2lep_stack_4->GetZaxis()->SetLabelFont(42);
   h_rlmet_2lep_stack_4->GetZaxis()->SetLabelSize(0.035);
   h_rlmet_2lep_stack_4->GetZaxis()->SetTitleSize(0.035);
   h_rlmet_2lep_stack_4->GetZaxis()->SetTitleFont(42);
   hs->Add(h_rlmet_2lep_stack_4,"");
   hs->Draw("hist");
   
   TH1D *h_rlmet_Znunu__23 = new TH1D("h_rlmet_Znunu__23","",24,250,850);
   h_rlmet_Znunu__23->SetBinContent(1,5130.16);
   h_rlmet_Znunu__23->SetBinContent(2,3114.792);
   h_rlmet_Znunu__23->SetBinContent(3,1885.72);
   h_rlmet_Znunu__23->SetBinContent(4,1205.881);
   h_rlmet_Znunu__23->SetBinContent(5,780.1381);
   h_rlmet_Znunu__23->SetBinContent(6,539.8865);
   h_rlmet_Znunu__23->SetBinContent(7,358.9807);
   h_rlmet_Znunu__23->SetBinContent(8,248.5987);
   h_rlmet_Znunu__23->SetBinContent(9,168.7847);
   h_rlmet_Znunu__23->SetBinContent(10,123.8558);
   h_rlmet_Znunu__23->SetBinContent(11,82.24373);
   h_rlmet_Znunu__23->SetBinContent(12,55.80395);
   h_rlmet_Znunu__23->SetBinContent(13,44.20385);
   h_rlmet_Znunu__23->SetBinContent(14,31.01749);
   h_rlmet_Znunu__23->SetBinContent(15,21.97003);
   h_rlmet_Znunu__23->SetBinContent(16,15.28825);
   h_rlmet_Znunu__23->SetBinContent(17,12.16437);
   h_rlmet_Znunu__23->SetBinContent(18,10.85232);
   h_rlmet_Znunu__23->SetBinContent(19,6.228446);
   h_rlmet_Znunu__23->SetBinContent(20,5.838876);
   h_rlmet_Znunu__23->SetBinContent(21,4.188293);
   h_rlmet_Znunu__23->SetBinContent(22,2.92368);
   h_rlmet_Znunu__23->SetBinContent(23,1.701652);
   h_rlmet_Znunu__23->SetBinContent(24,8.77794);
   h_rlmet_Znunu__23->SetBinError(1,26.69154);
   h_rlmet_Znunu__23->SetBinError(2,20.59034);
   h_rlmet_Znunu__23->SetBinError(3,16.12175);
   h_rlmet_Znunu__23->SetBinError(4,12.94915);
   h_rlmet_Znunu__23->SetBinError(5,10.21912);
   h_rlmet_Znunu__23->SetBinError(6,8.831033);
   h_rlmet_Znunu__23->SetBinError(7,7.424527);
   h_rlmet_Znunu__23->SetBinError(8,5.784346);
   h_rlmet_Znunu__23->SetBinError(9,4.860117);
   h_rlmet_Znunu__23->SetBinError(10,4.614414);
   h_rlmet_Znunu__23->SetBinError(11,3.30619);
   h_rlmet_Znunu__23->SetBinError(12,2.693558);
   h_rlmet_Znunu__23->SetBinError(13,2.490215);
   h_rlmet_Znunu__23->SetBinError(14,2.086374);
   h_rlmet_Znunu__23->SetBinError(15,1.713061);
   h_rlmet_Znunu__23->SetBinError(16,1.464197);
   h_rlmet_Znunu__23->SetBinError(17,1.278354);
   h_rlmet_Znunu__23->SetBinError(18,1.281235);
   h_rlmet_Znunu__23->SetBinError(19,0.9847521);
   h_rlmet_Znunu__23->SetBinError(20,0.9745229);
   h_rlmet_Znunu__23->SetBinError(21,0.7062024);
   h_rlmet_Znunu__23->SetBinError(22,0.6038584);
   h_rlmet_Znunu__23->SetBinError(23,0.4486411);
   h_rlmet_Znunu__23->SetBinError(24,1.026195);
   h_rlmet_Znunu__23->SetEntries(228731);

   ci = TColor::GetColor("#333333");
   h_rlmet_Znunu__23->SetFillColor(ci);
   h_rlmet_Znunu__23->SetFillStyle(3244);
   h_rlmet_Znunu__23->SetLineWidth(0);
   h_rlmet_Znunu__23->GetXaxis()->SetTitle("#slash{E}_{T} (removed lepton) [GeV]");
   h_rlmet_Znunu__23->GetXaxis()->SetLabelFont(42);
   h_rlmet_Znunu__23->GetXaxis()->SetLabelSize(0.035);
   h_rlmet_Znunu__23->GetXaxis()->SetTitleSize(0.035);
   h_rlmet_Znunu__23->GetXaxis()->SetTitleFont(42);
   h_rlmet_Znunu__23->GetYaxis()->SetLabelFont(42);
   h_rlmet_Znunu__23->GetYaxis()->SetLabelSize(0.035);
   h_rlmet_Znunu__23->GetYaxis()->SetTitleSize(0.035);
   h_rlmet_Znunu__23->GetYaxis()->SetTitleOffset(0);
   h_rlmet_Znunu__23->GetYaxis()->SetTitleFont(42);
   h_rlmet_Znunu__23->GetZaxis()->SetLabelFont(42);
   h_rlmet_Znunu__23->GetZaxis()->SetLabelSize(0.035);
   h_rlmet_Znunu__23->GetZaxis()->SetTitleSize(0.035);
   h_rlmet_Znunu__23->GetZaxis()->SetTitleFont(42);
   h_rlmet_Znunu__23->Draw("E2SAME");
   
   TH1D *h_rlmet__24 = new TH1D("h_rlmet__24","",24,250,850);
   h_rlmet__24->SetBinContent(1,5262);
   h_rlmet__24->SetBinContent(2,3156);
   h_rlmet__24->SetBinContent(3,1887);
   h_rlmet__24->SetBinContent(4,1204);
   h_rlmet__24->SetBinContent(5,771);
   h_rlmet__24->SetBinContent(6,498);
   h_rlmet__24->SetBinContent(7,346);
   h_rlmet__24->SetBinContent(8,240);
   h_rlmet__24->SetBinContent(9,145);
   h_rlmet__24->SetBinContent(10,100);
   h_rlmet__24->SetBinContent(11,62);
   h_rlmet__24->SetBinContent(12,46);
   h_rlmet__24->SetBinContent(13,39);
   h_rlmet__24->SetBinContent(14,27);
   h_rlmet__24->SetBinContent(15,16);
   h_rlmet__24->SetBinContent(16,9);
   h_rlmet__24->SetBinContent(17,14);
   h_rlmet__24->SetBinContent(18,13);
   h_rlmet__24->SetBinContent(19,6);
   h_rlmet__24->SetBinContent(20,5);
   h_rlmet__24->SetBinContent(21,3);
   h_rlmet__24->SetBinContent(22,3);
   h_rlmet__24->SetBinContent(23,3);
   h_rlmet__24->SetBinContent(24,5);
   h_rlmet__24->SetBinError(1,72.53964);
   h_rlmet__24->SetBinError(2,56.17829);
   h_rlmet__24->SetBinError(3,43.43961);
   h_rlmet__24->SetBinError(4,34.6987);
   h_rlmet__24->SetBinError(5,27.76689);
   h_rlmet__24->SetBinError(6,22.31591);
   h_rlmet__24->SetBinError(7,18.60108);
   h_rlmet__24->SetBinError(8,15.49193);
   h_rlmet__24->SetBinError(9,12.04159);
   h_rlmet__24->SetBinError(10,10);
   h_rlmet__24->SetBinError(11,7.874008);
   h_rlmet__24->SetBinError(12,6.78233);
   h_rlmet__24->SetBinError(13,6.244998);
   h_rlmet__24->SetBinError(14,5.196152);
   h_rlmet__24->SetBinError(15,4);
   h_rlmet__24->SetBinError(16,3);
   h_rlmet__24->SetBinError(17,3.741657);
   h_rlmet__24->SetBinError(18,3.605551);
   h_rlmet__24->SetBinError(19,2.44949);
   h_rlmet__24->SetBinError(20,2.236068);
   h_rlmet__24->SetBinError(21,1.732051);
   h_rlmet__24->SetBinError(22,1.732051);
   h_rlmet__24->SetBinError(23,1.732051);
   h_rlmet__24->SetBinError(24,2.236068);
   h_rlmet__24->SetEntries(13862);
   h_rlmet__24->SetMarkerStyle(20);
   h_rlmet__24->SetMarkerSize(0.8);
   h_rlmet__24->GetXaxis()->SetTitle("#slash{E}_{T} (removed lepton) [GeV]");
   h_rlmet__24->GetXaxis()->SetLabelFont(42);
   h_rlmet__24->GetXaxis()->SetLabelSize(0.035);
   h_rlmet__24->GetXaxis()->SetTitleSize(0.035);
   h_rlmet__24->GetXaxis()->SetTitleFont(42);
   h_rlmet__24->GetYaxis()->SetLabelFont(42);
   h_rlmet__24->GetYaxis()->SetLabelSize(0.035);
   h_rlmet__24->GetYaxis()->SetTitleSize(0.035);
   h_rlmet__24->GetYaxis()->SetTitleOffset(0);
   h_rlmet__24->GetYaxis()->SetTitleFont(42);
   h_rlmet__24->GetZaxis()->SetLabelFont(42);
   h_rlmet__24->GetZaxis()->SetLabelSize(0.035);
   h_rlmet__24->GetZaxis()->SetTitleSize(0.035);
   h_rlmet__24->GetZaxis()->SetTitleFont(42);
   // h_rlmet__24->Draw("SAME E0");
   
   Double_t _fx3052[24] = {
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
   587.5,
   612.5,
   637.5,
   662.5,
   687.5,
   712.5,
   737.5,
   762.5,
   787.5,
   812.5,
   837.5};
   Double_t _fy3052[24] = {
   5262,
   3156,
   1887,
   1204,
   771,
   498,
   346,
   240,
   145,
   100,
   62,
   46,
   39,
   27,
   16,
   9,
   14,
   13,
   6,
   5,
   3,
   3,
   3,
   5};
   Double_t _felx3052[24] = {
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
   Double_t _fely3052[24] = {
   72.53891,
   56.17652,
   43.4367,
   34.69463,
   27.76146,
   22.3089,
   18.59248,
   15.48146,
   12.02795,
   9.983457,
   7.852869,
   6.757713,
   6.218222,
   5.163869,
   3.957873,
   2.943511,
   3.696566,
   3.558726,
   2.379969,
   2.159724,
   1.632727,
   1.632727,
   1.632727,
   2.159724};
   Double_t _fehx3052[24] = {
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
   Double_t _fehy3052[24] = {
   73.54583,
   57.18546,
   44.44825,
   35.70908,
   28.77952,
   23.33136,
   19.61943,
   16.51382,
   13.06958,
   11.03359,
   8.916552,
   7.831652,
   7.298524,
   6.260373,
   5.083169,
   4.110286,
   4.830479,
   4.697668,
   3.583712,
   3.382539,
   2.918242,
   2.918242,
   2.918242,
   3.382539};
   TGraphAsymmErrors *grae = new TGraphAsymmErrors(24,_fx3052,_fy3052,_felx3052,_fehx3052,_fely3052,_fehy3052);
   grae->SetName("");
   grae->SetTitle("");
   grae->SetFillColor(1);
   grae->SetMarkerStyle(20);
   grae->SetMarkerSize(0.8);
   
   TH1F *Graph_Graph3052 = new TH1F("Graph_Graph3052","",100,190,910);
   Graph_Graph3052->SetMinimum(1.230546);
   Graph_Graph3052->SetMaximum(5868.964);
   Graph_Graph3052->SetDirectory(0);
   Graph_Graph3052->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph3052->SetLineColor(ci);
   Graph_Graph3052->GetXaxis()->SetLabelFont(42);
   Graph_Graph3052->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph3052->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph3052->GetXaxis()->SetTitleFont(42);
   Graph_Graph3052->GetYaxis()->SetLabelFont(42);
   Graph_Graph3052->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph3052->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph3052->GetYaxis()->SetTitleOffset(0);
   Graph_Graph3052->GetYaxis()->SetTitleFont(42);
   Graph_Graph3052->GetZaxis()->SetLabelFont(42);
   Graph_Graph3052->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph3052->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph3052->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_Graph3052);
   
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

   entry=leg->AddEntry("h_rlmet_Znunu","Stat. unc.","f");
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
   
   TH1D* h_axis_ratio = new TH1D("ratio_axis","", 24,250,850);
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

   TH1D *h_rlmet_Znunu__71 = new TH1D("h_rlmet_Znunu__71","",24,250,850);
   h_rlmet_Znunu__71->SetBinContent(1,1);
   h_rlmet_Znunu__71->SetBinContent(2,1);
   h_rlmet_Znunu__71->SetBinContent(3,1);
   h_rlmet_Znunu__71->SetBinContent(4,1);
   h_rlmet_Znunu__71->SetBinContent(5,1);
   h_rlmet_Znunu__71->SetBinContent(6,1);
   h_rlmet_Znunu__71->SetBinContent(7,1);
   h_rlmet_Znunu__71->SetBinContent(8,1);
   h_rlmet_Znunu__71->SetBinContent(9,1);
   h_rlmet_Znunu__71->SetBinContent(10,1);
   h_rlmet_Znunu__71->SetBinContent(11,1);
   h_rlmet_Znunu__71->SetBinContent(12,1);
   h_rlmet_Znunu__71->SetBinContent(13,1);
   h_rlmet_Znunu__71->SetBinContent(14,1);
   h_rlmet_Znunu__71->SetBinContent(15,1);
   h_rlmet_Znunu__71->SetBinContent(16,1);
   h_rlmet_Znunu__71->SetBinContent(17,1);
   h_rlmet_Znunu__71->SetBinContent(18,1);
   h_rlmet_Znunu__71->SetBinContent(19,1);
   h_rlmet_Znunu__71->SetBinContent(20,1);
   h_rlmet_Znunu__71->SetBinContent(21,1);
   h_rlmet_Znunu__71->SetBinContent(22,1);
   h_rlmet_Znunu__71->SetBinContent(23,1);
   h_rlmet_Znunu__71->SetBinContent(24,1);
   h_rlmet_Znunu__71->SetBinError(1,0.005202867);
   h_rlmet_Znunu__71->SetBinError(2,0.0066105);
   h_rlmet_Znunu__71->SetBinError(3,0.008549391);
   h_rlmet_Znunu__71->SetBinError(4,0.01073834);
   h_rlmet_Znunu__71->SetBinError(5,0.01309911);
   h_rlmet_Znunu__71->SetBinError(6,0.0163572);
   h_rlmet_Znunu__71->SetBinError(7,0.02068225);
   h_rlmet_Znunu__71->SetBinError(8,0.02326781);
   h_rlmet_Znunu__71->SetBinError(9,0.02879477);
   h_rlmet_Znunu__71->SetBinError(10,0.03725634);
   h_rlmet_Znunu__71->SetBinError(11,0.0401999);
   h_rlmet_Znunu__71->SetBinError(12,0.04826824);
   h_rlmet_Znunu__71->SetBinError(13,0.05633479);
   h_rlmet_Znunu__71->SetBinError(14,0.06726446);
   h_rlmet_Znunu__71->SetBinError(15,0.0779726);
   h_rlmet_Znunu__71->SetBinError(16,0.09577269);
   h_rlmet_Znunu__71->SetBinError(17,0.1050901);
   h_rlmet_Znunu__71->SetBinError(18,0.1180609);
   h_rlmet_Znunu__71->SetBinError(19,0.1581056);
   h_rlmet_Znunu__71->SetBinError(20,0.1669025);
   h_rlmet_Znunu__71->SetBinError(21,0.1686134);
   h_rlmet_Znunu__71->SetBinError(22,0.2065405);
   h_rlmet_Znunu__71->SetBinError(23,0.2636503);
   h_rlmet_Znunu__71->SetBinError(24,0.1169062);
   h_rlmet_Znunu__71->SetEntries(228755);

   ci = TColor::GetColor("#333333");
   h_rlmet_Znunu__71->SetFillColor(ci);
   h_rlmet_Znunu__71->SetFillStyle(3244);
   h_rlmet_Znunu__71->GetXaxis()->SetTitle("#slash{E}_{T} (removed lepton) [GeV]");
   h_rlmet_Znunu__71->GetXaxis()->SetLabelFont(42);
   h_rlmet_Znunu__71->GetXaxis()->SetLabelSize(0.035);
   h_rlmet_Znunu__71->GetXaxis()->SetTitleSize(0.035);
   h_rlmet_Znunu__71->GetXaxis()->SetTitleFont(42);
   h_rlmet_Znunu__71->GetYaxis()->SetLabelFont(42);
   h_rlmet_Znunu__71->GetYaxis()->SetLabelSize(0.035);
   h_rlmet_Znunu__71->GetYaxis()->SetTitleSize(0.035);
   h_rlmet_Znunu__71->GetYaxis()->SetTitleOffset(0);
   h_rlmet_Znunu__71->GetYaxis()->SetTitleFont(42);
   h_rlmet_Znunu__71->GetZaxis()->SetLabelFont(42);
   h_rlmet_Znunu__71->GetZaxis()->SetLabelSize(0.035);
   h_rlmet_Znunu__71->GetZaxis()->SetTitleSize(0.035);
   h_rlmet_Znunu__71->GetZaxis()->SetTitleFont(42);
   h_rlmet_Znunu__71->Draw("SAME E2");

   TLine* hline_1 = new TLine(250, 1, 850, 1);
   hline_1->SetLineStyle(2);
   hline_1->SetLineWidth(2);
   hline_1->SetLineColor(kGray+2);
   hline_1->Draw();
   
   Double_t _fx3054[24] = {
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
   587.5,
   612.5,
   637.5,
   662.5,
   687.5,
   712.5,
   737.5,
   762.5,
   787.5,
   812.5,
   837.5};
   Double_t _fy3054[24] = {
   1.025699,
   1.01323,
   1.000679,
   0.9984405,
   0.9882865,
   0.9224161,
   0.9638401,
   0.9654115,
   0.8590826,
   0.8073904,
   0.7538568,
   0.8243145,
   0.8822761,
   0.8704767,
   0.7282647,
   0.5886873,
   1.150902,
   1.197901,
   0.9633221,
   0.8563293,
   0.7162822,
   1.026104,
   1.762992,
   0.5696098};
   Double_t _felx3054[24] = {
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
   Double_t _fely3054[24] = {
   0.0141397,
   0.0180354,
   0.02303455,
   0.0287712,
   0.03558532,
   0.04132146,
   0.05179243,
   0.06227493,
   0.07126207,
   0.08060547,
   0.09548288,
   0.1210974,
   0.1406715,
   0.1664825,
   0.1801487,
   0.1925342,
   0.3038848,
   0.327923,
   0.3821128,
   0.369887,
   0.389831,
   0.5584491,
   0.959495,
   0.24604};
   Double_t _fehx3054[24] = {
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
   Double_t _fehy3054[24] = {
   0.01433597,
   0.01835932,
   0.02357097,
   0.02961245,
   0.03689028,
   0.04321531,
   0.05465316,
   0.06642763,
   0.07743342,
   0.08908417,
   0.1084162,
   0.1403423,
   0.1651106,
   0.2018337,
   0.2313683,
   0.2688526,
   0.3971007,
   0.4328723,
   0.5753782,
   0.5793135,
   0.6967617,
   0.99814,
   1.714946,
   0.3853455};
   grae = new TGraphAsymmErrors(24,_fx3054,_fy3054,_felx3054,_fehx3054,_fely3054,_fehy3054);
   grae->SetName("");
   grae->SetTitle("");
   grae->SetFillColor(1);
   grae->SetMarkerStyle(20);
   grae->SetMarkerSize(0.8);
   
   TH1F *Graph_Graph_Graph30533054 = new TH1F("Graph_Graph_Graph30533054","",100,190,910);
   Graph_Graph_Graph30533054->SetMinimum(0.008132827);
   Graph_Graph_Graph30533054->SetMaximum(3.793376);
   Graph_Graph_Graph30533054->SetDirectory(0);
   Graph_Graph_Graph30533054->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph_Graph30533054->SetLineColor(ci);
   Graph_Graph_Graph30533054->GetXaxis()->SetLabelFont(42);
   Graph_Graph_Graph30533054->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph_Graph30533054->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph_Graph30533054->GetXaxis()->SetTitleFont(42);
   Graph_Graph_Graph30533054->GetYaxis()->SetLabelFont(42);
   Graph_Graph_Graph30533054->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph_Graph30533054->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph_Graph30533054->GetYaxis()->SetTitleOffset(0);
   Graph_Graph_Graph30533054->GetYaxis()->SetTitleFont(42);
   Graph_Graph_Graph30533054->GetZaxis()->SetLabelFont(42);
   Graph_Graph_Graph30533054->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph_Graph30533054->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph_Graph30533054->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_Graph_Graph30533054);
   
   grae->Draw(" pz");

   c1_n22->Modified();
   c1_n22->cd();
   c1_n22->SetSelected(c1_n22);


   // c1_n22->Print("cr2lbase_rlmet_004-a.pdf");
   c1_n22->Print("cr2lbase_rlmet_mcerr.pdf");
}
