#include <iostream>
#include <string>
#include "TChain.h"
#include "TString.h"

#include "StopLooper.h"
#include "../StopCORE/sampleInfo.h"

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

  auto fillChain = [&](sampleInfo::sampleUtil sample, TChain* chain) {
    string inputdir = sample.baby_i_o.first;
    for (auto fname : sample.inputBabies) {
      string filepath = inputdir + fname;
      if (input_dir.find("/output") != string::npos)
        filepath.replace(filepath.find("skim"), 4, "output");
      chain->Add(filepath.c_str());
    }
  };

  if (sample.find("sample") != string::npos) {
    sampleInfo::vect_id sampleList;
    if (sample == "all_2016_samples") sampleList = sampleInfo::getSampleList_SR();
    else if (sample == "2016data_sample") sampleList = sampleInfo::vect_id{sampleInfo::ID::k_single_lepton_met};
    else if (sample == "2016ttbar_sample") sampleList = sampleInfo::vect_id{sampleInfo::ID::k_ttbar_singleLeptFromT_madgraph_pythia8_ext1};
    for (auto sampleid : sampleList) {
      sampleInfo::sampleUtil sample(sampleid);
      string samplestr = sample.label;
      TChain *chain = new TChain("t");
      fillChain(sample, chain);
      StopLooper stop;
      stop.looper(chain, samplestr, output_dir);
      delete chain;
    }
    return 0;
  }

  cout << "[runStopLooper]: infile = " << infile << endl;
  ch->Add(infile);

  if (ch->GetEntries() == 0) {
    cout << "ERROR: no entries in chain. filename was: " << infile << endl;
    return 2;
  }

  StopLooper stop;
  stop.looper(ch, sample, output_dir);

  return 0;
}
