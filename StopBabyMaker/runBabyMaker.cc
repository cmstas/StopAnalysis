// -*- C++ -*-
// Baby Maker for the SNT Stop Analysis

#include "looper.h"
#include "TChain.h"
#include "TString.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

vector<TString> loadFromSampleList(char *type, const char *filename, char *input){

  vector<TString> output;
  char buffer[500];
  char StringValue[1000];
  ifstream IN(input);
  while( IN.getline(buffer, 500, '\n') ){
    // ok = false;
    if (buffer[0] == '#') {
      continue; // Skip lines commented with '#'
    }
    if( !strcmp(buffer, "SAMPLE")){
      bool add = false;
      IN.getline(buffer, 500, '\n');
      sscanf(buffer, "Name\t%s", StringValue);
      if((string)StringValue==(string)type) add=true;
      IN.getline(buffer, 500, '\n');
      sscanf(buffer, "Path\t%s", StringValue);
      if(add){
        std::ostringstream addStream;
        addStream << StringValue << filename;
        TString addString = addStream.str().c_str();
        output.push_back(addString);
      }
    }
  }
  return output;
}

TString parseArg(const TString& input, TString arg, const TString dfval="") {
  if (input.Contains(arg)) {
    if (!arg.EndsWith("=")) arg += "=";
    int sidx = input.Index(arg) + arg.Sizeof();
    int eidx = input.Index(",", sidx);
    if (eidx < 0) eidx = input.Sizeof();
    return input(sidx, eidx-sidx);
  } else {
    return dfval;
  }
}

int main(int argc, char **argv){

  //
  // Input sanitation
  //
  //isFastsim = true does not work, need to use = 1
  if(argc<2){
    cout << "[runBabyMaker] Function takes six arguments: ./runBabyMaker <sample_name> <nevents> <file_number> <outpath> <samplelist>/<filenames> <isFastSim>" << endl;
    cout << "[runBabyMaker] Need to provide at least sample_name; nevents=-1 (-1=all events), file_number=-1 (-1=merged_ntuple_*.root), output=/nfs-7/userdata/stopRun2/, samplelist=sample_2017.dat, isFastSim=false by default" << endl;
    return 0;
  }

  //
  // Use arguments to set run parameters
  //
  int nevents = -1;
  if(argc>2) nevents = atoi(argv[2]);

  int ifile=-1;
  if(argc>3) ifile = atoi(argv[3]);

  char* dirpath = ".";
  if(argc>4) dirpath = argv[4];

  //const char* filename = (ifile == -1 ? "*postprocess.root" : Form("%spostprocess.root"));
  const char* filename = (ifile == -1 ? "merged_ntuple_*.root" : Form("merged_ntuple_%i.root", ifile));
  //const char* filename = "ntuple_TTJets_HT-1200to2500_new.root";
  // cout << "[runBabyMaker] The output file will be: " << filename << endl;

  const char* suffix = ifile == -1 ? "" : Form("_%i", ifile);

  char *input = "sample_2017.dat";
  if(argc>5) input = argv[5];

  // Use the last argument also control the type of skim
  int sampletype(0);
  if(argc>6) sampletype = atoi(argv[6]);
  bool isFastsim = (sampletype == 1);
  bool doPhotonSkim = (sampletype == 2);

  TString extrargs;
  if(argc>7) extrargs = argv[7];

  //
  // Initialize looper
  //
  babyMaker *mylooper = new babyMaker();

  //
  // Skim Parameters
  //
  mylooper->skim_nvtx            = 1;
  mylooper->skim_met             = 50;

  mylooper->skim_nGoodLep        = 1;
  mylooper->skim_goodLep_el_pt   = 20.0;
  mylooper->skim_goodLep_el_eta  = 1.4442;
  mylooper->skim_goodLep_mu_pt   = 20.0;
  mylooper->skim_goodLep_mu_eta  = 2.4;

  mylooper->skim_looseLep_el_pt  = 10.0;
  mylooper->skim_looseLep_el_eta = 2.4;
  mylooper->skim_looseLep_mu_pt  = 10.0;
  mylooper->skim_looseLep_mu_eta = 2.4;

  mylooper->skim_vetoLep_el_pt   = 5.0;
  mylooper->skim_vetoLep_el_eta  = 2.4;
  mylooper->skim_vetoLep_mu_pt   = 5.0;
  mylooper->skim_vetoLep_mu_eta  = 2.4;

  mylooper->skim_nJets           = 2;
  mylooper->skim_nBJets          = 0;
  mylooper->skim_jet_pt          = 30.0;
  mylooper->skim_jet_eta         = 2.4;
  mylooper->analysis_jet_pt      = 30;
  mylooper->analysis_bjet_pt     = 30;

  mylooper->applyJECfromFile    = true;  //THIS FLAG DECIDES NOW TOO IF JESUP/DOWN VALUES ARE CALCULATED
  mylooper->applyAK8JECfromFile = false; // not needed as DeepAK8 uses uncorrected jets
  mylooper->JES_type            = 0;  //0 central, 1 up, -1 down <-- obsolete, not to be changed

  mylooper->applyBtagSFs       = true;
  mylooper->applyLeptonSFs     = true;
  mylooper->applyVetoLeptonSFs = true;
  mylooper->apply2ndLepVeto    = false;
  mylooper->isFastsim          = isFastsim;

  mylooper->skim_jet_ak8_pt    = 200.0;
  mylooper->skim_jet_ak8_eta   = 2.4;

  mylooper->skim_nPhotons      = 0;
  mylooper->skim_ph_pt         = 60.0;
  mylooper->skim_ph_eta        = 2.4;

  mylooper->filltaus        =  true;
  mylooper->filltracks      =  true;
  mylooper->fillZll         =  false;
  mylooper->fillPhoton      =  false;
  mylooper->fillMETfilt     =  true;
  mylooper->fill2ndlep      =  false;
  mylooper->fillExtraEvtVar =  false;

  mylooper->fillAK8         =  true;
  mylooper->fillTopTag      =  true;
  mylooper->fillAK4EF       =  false;
  mylooper->fillAK4_Other   =  false;
  mylooper->fillOverleps    =  false;
  mylooper->fillAK4Synch    =  false;
  mylooper->fillElID        =  false;
  mylooper->fillIso         =  false;
  mylooper->fillLepSynch    =  false;

  if (isFastsim)
    cout << "[runBabyMaker] >> Fastsim switch on! Note that the looper will still determine it based on sample name." << endl;

  if (doPhotonSkim) {
    cout << "[runBabyMaker] >> Doing photon skim for the sample!" << endl;
    mylooper->fillPhoton    = true;
    mylooper->skim_nPhotons = 1;
    mylooper->skim_nGoodLep = 0;
    mylooper->skim_met      = 100;
  }

  TString newskim_met = parseArg(extrargs, "skim_met");
  if (newskim_met.IsFloat()) {
    cout << "[runBabyMaker] >> Changing the skim_met to new value: " << newskim_met.Atof() << endl;
    mylooper->skim_met = newskim_met.Atof();
  }

  TString topCandTreeName = parseArg(extrargs, "topcandTree");
  if (!topCandTreeName.IsNull()) {
    topCandTreeName = Form("%s%s.root", topCandTreeName.Data(), suffix);
    cout << "[runBabyMaker] >> Will generate a separate topcand tree " << topCandTreeName << endl;
    mylooper->runTopCandTreeMaker = true;
    mylooper->topcandTreeMaker = new TopCandTree("tree", topCandTreeName.Data(), "ttbar");
  }

  // Input sanitation
  if( !(mylooper->skim_goodLep_mu_pt > mylooper->skim_looseLep_mu_pt &&
        mylooper->skim_looseLep_mu_pt > mylooper->skim_vetoLep_mu_pt) ){
    cout << "[runBabyMaker]  Problem with muon pT hierachy for good, loose, and veto pT!" << endl;
    cout << "[runBabyMaker]    Exiting..." << endl;
    return 0;
  }

  if( !(mylooper->skim_goodLep_el_pt > mylooper->skim_looseLep_el_pt &&
        mylooper->skim_looseLep_el_pt > mylooper->skim_vetoLep_el_pt) ){
    cout << "[runBabyMaker]  Problem with electron pT hierarchy for good, loose, and veto pT!" << endl;
    cout << "[runBabyMaker]    Exiting..." << endl;
    return 0;
  }

  //
  // Intialize TChain, load samples
  //
  TChain *sample = new TChain("Events");

  if (input[0] == '/') {
    // Determine from hostname to see if need xrootd for the file
    char hostname_cstr[64];
    gethostname(hostname_cstr, 64);
    TString hostname(hostname_cstr);
    cout << ">>> Hostname is " << hostname << endl;
    bool useXrootd = !hostname.Contains("t2.ucsd.edu");
    if (!useXrootd) {  // one last check in case /hadoop is still not mounted
      ifstream hadoopdir("/hadoop/cms");
      useXrootd = (!hadoopdir.good());
    }
    if (useXrootd) cout << ">>> Using xrootd for the input files." << endl;

    vector<TString> vecInFiles;
    TString infiles(input);
    while (infiles.Contains(',')) {
      TString fn = infiles(0, infiles.Index(','));
      vecInFiles.push_back( fn );
      infiles.Remove(0, fn.Length()+1);
    }
    vecInFiles.push_back( infiles );

    for (TString file : vecInFiles) {
      if (useXrootd && file.Contains("/hadoop"))
        file.ReplaceAll("/hadoop/cms", "root://cmsxrootd.fnal.gov/");
      cout << "Add sample " << file << " to files to be processed." << endl;
      sample->Add(file);
    }
  } else {
    vector<TString> samplelist = loadFromSampleList(argv[1], filename, input);//new
    bool fileexists = true;
    for(unsigned int i = 0; i<samplelist.size(); ++i){
      if(ifile>=0){
        //check if ifile exists - works not for *
        ifstream infile(samplelist[i].Data());
        fileexists = infile.good();
      }
      if(fileexists){
        cout << "Add sample " << samplelist[i] << " to files to be processed." << endl;
        sample->Add(samplelist[i].Data());
      }
    }
  }

  // Run Looper
  mylooper->looper(sample, Form("%s%s", argv[1], suffix), nevents, dirpath);

  return 0;
}
