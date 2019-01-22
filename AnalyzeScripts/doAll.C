{

  cout << "Loading dataMCplotMaker...\n";
  // gROOT->ProcessLine(".L ~/tas/Software/dataMCplotMaker/dataMCplotMaker.cc++");
  gSystem->Load("~/tas/Software/dataMCplotMaker/dataMCplotMaker_cc.so");
  gROOT->ProcessLine(".L compareDistributions.C+");
  gROOT->ProcessLine("compareDistributions()");
  // gROOT->ProcessLine(".x compare_data_17vs16.C");
  // gROOT->ProcessLine(".L makePlots.C+");
  // gROOT->ProcessLine("makePlots()");
  
}
