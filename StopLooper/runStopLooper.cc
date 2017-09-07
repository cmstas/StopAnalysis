#include <iostream>
#include <string>
#include "TChain.h"
#include "TString.h"

#include "StopLooper.h"

using namespace std;

int main(int argc, char** argv)
{

  if (argc < 4) {
    cout << "USAGE: runStopLooper <input_dir> <sample> <output_dir>" << endl;
    return 1;
  }

  string input_dir(argv[1]);
  string sample(argv[2]);
  string output_dir(argv[3]);

  TChain *ch = new TChain("t");
  TString infile = Form("%s/%s*.root", input_dir.c_str(), sample.c_str());
 
  // 2017Data
  bool doData2017B = false;
  bool doData2017C = false;
  bool doData2017dilep = false;
  bool doData2016 = false;
  bool doData2016dilep = false;

  if (sample == "data_2017") {
    doData2017B = true;
    doData2017C = true;
  }
  else if (sample == "data_2017B") doData2017B = true;
  else if (sample == "data_2017C") doData2017C = true;
  else if (sample == "data_2017dilep") doData2017dilep = true;
  else if (sample == "data_2016") doData2016 = true;
  else if (sample == "data_2016dilep") doData2016dilep = true;
  else {
    cout << __FILE__ << ':' << __LINE__ << ": infile= " << infile << endl;
    ch->Add(infile);
  }

  if (doData2017B) {
    ch->Add(Form("%s/data_met_run2017B_v2.root", input_dir.c_str()));
    ch->Add(Form("%s/data_met_run2017B_v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_singleel_run2017B_v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_singleel_run2017B_v2.root", input_dir.c_str()));
    ch->Add(Form("%s/data_singlemu_run2017B_v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_singlemu_run2017B_v2.root", input_dir.c_str()));
  }
  if (doData2017C) {
    ch->Add(Form("%s/data_singlemu_run2017C_v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_singlemu_run2017C_v2.root", input_dir.c_str()));
    ch->Add(Form("%s/data_singlemu_run2017C_v3.root", input_dir.c_str()));
    ch->Add(Form("%s/data_singleel_run2017C_v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_singleel_run2017C_v2.root", input_dir.c_str()));
    ch->Add(Form("%s/data_singleel_run2017C_v3.root", input_dir.c_str()));
    ch->Add(Form("%s/data_met_run2017C_v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_met_run2017C_v2.root", input_dir.c_str()));
    ch->Add(Form("%s/data_met_run2017C_v3.root", input_dir.c_str()));
  }
  if (doData2017dilep) {
    ch->Add(Form("%s/data_double_eg_Run2017B_MINIAOD_PromptReco-v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_double_eg_Run2017B_MINIAOD_PromptReco-v2.root", input_dir.c_str()));
    ch->Add(Form("%s/data_double_eg_Run2017C_MINIAOD_PromptReco-v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_double_eg_Run2017C_MINIAOD_PromptReco-v2.root", input_dir.c_str()));
    ch->Add(Form("%s/data_double_eg_Run2017C_MINIAOD_PromptReco-v3.root", input_dir.c_str()));
    ch->Add(Form("%s/data_double_mu_Run2017B_MINIAOD_PromptReco-v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_double_mu_Run2017B_MINIAOD_PromptReco-v2.root", input_dir.c_str()));
    ch->Add(Form("%s/data_muon_eg_Run2017B_MINIAOD_PromptReco-v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_muon_eg_Run2017B_MINIAOD_PromptReco-v2.root", input_dir.c_str()));
    ch->Add(Form("%s/data_muon_eg_Run2017C_MINIAOD_PromptReco-v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_double_mu_Run2017C_MINIAOD_PromptReco-v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_double_mu_Run2017C_MINIAOD_PromptReco-v2.root", input_dir.c_str()));
    ch->Add(Form("%s/data_double_mu_Run2017C_MINIAOD_PromptReco-v3.root", input_dir.c_str()));
    ch->Add(Form("%s/data_muon_eg_Run2017C_MINIAOD_PromptReco-v2.root", input_dir.c_str()));
    ch->Add(Form("%s/data_muon_eg_Run2017C_MINIAOD_PromptReco-v3.root", input_dir.c_str()));
  }
  
  if (doData2016) {
    ch->Add(Form("%s/data_single_muon_Run2016C_MINIAOD_03Feb2017-v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_single_muon_Run2016E_MINIAOD_03Feb2017-v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_single_muon_Run2016H_MINIAOD_03Feb2017_ver3-v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_single_muon_Run2016B_MINIAOD_03Feb2017_ver2-v2.root", input_dir.c_str()));
    ch->Add(Form("%s/data_single_muon_Run2016D_MINIAOD_03Feb2017-v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_single_muon_Run2016F_MINIAOD_03Feb2017-v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_single_muon_Run2016G_MINIAOD_03Feb2017-v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_single_muon_Run2016H_MINIAOD_03Feb2017_ver2-v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_single_electron_Run2016B_MINIAOD_03Feb2017_ver2-v2.root", input_dir.c_str()));
    ch->Add(Form("%s/data_single_electron_Run2016C_MINIAOD_03Feb2017-v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_single_electron_Run2016D_MINIAOD_03Feb2017-v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_single_electron_Run2016F_MINIAOD_03Feb2017-v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_single_electron_Run2016H_MINIAOD_03Feb2017_ver2-v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_single_electron_Run2016H_MINIAOD_03Feb2017_ver3-v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_single_electron_Run2016E_MINIAOD_03Feb2017-v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_single_electron_Run2016G_MINIAOD_03Feb2017-v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_met_Run2016B_MINIAOD_03Feb2017_ver2-v2.root", input_dir.c_str()));
    ch->Add(Form("%s/data_met_Run2016C_MINIAOD_03Feb2017-v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_met_Run2016D_MINIAOD_03Feb2017-v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_met_Run2016E_MINIAOD_03Feb2017-v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_met_Run2016F_MINIAOD_03Feb2017-v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_met_Run2016G_MINIAOD_03Feb2017-v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_met_Run2016H_MINIAOD_03Feb2017_ver2-v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_met_Run2016H_MINIAOD_03Feb2017_ver3-v1.root", input_dir.c_str()));
  }

  if (doData2016dilep) {
    ch->Add(Form("%s/data_double_eg_Run2016B_MINIAOD_03Feb2017_ver2-v2.root", input_dir.c_str()));
    ch->Add(Form("%s/data_double_eg_Run2016C_MINIAOD_03Feb2017-v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_double_eg_Run2016D_MINIAOD_03Feb2017-v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_double_eg_Run2016E_MINIAOD_03Feb2017-v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_double_eg_Run2016F_MINIAOD_03Feb2017-v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_double_eg_Run2016G_MINIAOD_03Feb2017-v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_double_eg_Run2016H_MINIAOD_03Feb2017_ver2-v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_double_eg_Run2016H_MINIAOD_03Feb2017_ver3-v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_double_mu_Run2016B_MINIAOD_03Feb2017_ver2-v2.root", input_dir.c_str()));
    ch->Add(Form("%s/data_double_mu_Run2016C_MINIAOD_03Feb2017-v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_double_mu_Run2016D_MINIAOD_03Feb2017-v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_double_mu_Run2016E_MINIAOD_03Feb2017-v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_double_mu_Run2016F_MINIAOD_03Feb2017-v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_double_mu_Run2016G_MINIAOD_03Feb2017-v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_double_mu_Run2016H_MINIAOD_03Feb2017_ver2-v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_double_mu_Run2016H_MINIAOD_03Feb2017_ver3-v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_muon_eg_Run2016B_MINIAOD_03Feb2017_ver2-v2.root", input_dir.c_str()));
    ch->Add(Form("%s/data_muon_eg_Run2016C_MINIAOD_03Feb2017-v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_muon_eg_Run2016E_MINIAOD_03Feb2017-v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_muon_eg_Run2016F_MINIAOD_03Feb2017-v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_muon_eg_Run2016G_MINIAOD_03Feb2017-v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_muon_eg_Run2016H_MINIAOD_03Feb2017_ver3-v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_muon_eg_Run2016D_MINIAOD_03Feb2017-v1.root", input_dir.c_str()));
    ch->Add(Form("%s/data_muon_eg_Run2016H_MINIAOD_03Feb2017_ver2-v1.root", input_dir.c_str()));
  }

  if (ch->GetEntries() == 0) {
    cout << "ERROR: no entries in chain. filename was: " << infile << endl;
    return 2;
  }

  StopLooper stop;
  stop.looper(ch, sample, output_dir);

  return 0;
}
