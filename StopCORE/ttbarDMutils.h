#ifndef TTBARDM_UTILITIES_H
#define TTBARDM_UTILITIES_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <string>
#include <map>

// entry = (-1)^parity * (1000*mChi + mPhi)
// xsec is in pb (hopefully)

namespace ttdm {

const std::map<int,float> ttbarDM_xsecs = 
{
  {-10100  , 0.2286},   // 1 100 pseudoscalar 0.228600314554
  {-10150  , 0.1483},   // 1 150 pseudoscalar 0.148300204061
  {-10200  , 0.09949},  // 1 200 pseudoscalar 0.0994901368982
  {-10250  , 0.06868},  // 1 250 pseudoscalar 0.0686800945037
  {-10300  , 0.04864},  // 1 300 pseudoscalar 0.0486400669286
  {-10350  , 0.02807},  // 1 350 pseudoscalar 0.0280700386243
  {-10400  , 0.0145},   // 1 400 pseudoscalar 0.014500019952
  {-10450  , 0.009637}, // 1 450 pseudoscalar 0.00963701326051
  {-10500  , 0.006797}, // 1 500 pseudoscalar 0.00679700935267
  {-10050  , 0.36750},  // 1 50 pseudoscalar 0.36750050568
  {-200100 , 0.2294},   // 20 100 pseudoscalar 0.229400315654
  {-300100 , 0.2290},   // 30 100 pseudoscalar 0.229000315104
  {-400100 , 0.2296},   // 40 100 pseudoscalar 0.22960031593
  {-450100 , 0.2296},   // 45 100 pseudoscalar 0.22960031593
  {-490100 , 0.2303},   // 49 100 pseudoscalar 0.230300316893
  {-510100 , 0.02103},  // 51 100 pseudoscalar 0.0210300289373
  {-550100 , 0.008131}, // 55 100 pseudoscalar 0.00813101118826
  { 10100  , 0.7906},   // 1 100 scalar 0.790601087866
  { 10150  , 0.2561},   // 1 150 scalar 0.256100352394
  { 10200  , 0.1074},   // 1 200 scalar 0.107400147782
  { 10250  , 0.05633},  // 1 250 scalar 0.0563300775101
  { 10300  , 0.03465},  // 1 300 scalar 0.0346500476784
  { 10350  , 0.02358},  // 1 350 scalar 0.0235800324461
  { 10400  , 0.01447},  // 1 400 scalar 0.0144700199107
  { 10450  , 0.009340}, // 1 450 scalar 0.00934001285184
  { 10500  , 0.006406}, // 1 500 scalar 0.00640600881466
  { 10050  , 3.508},    // 1 50 scalar 3.50800482701
  { 200100 , 0.7924},   // 20 100 scalar 0.792401090342
  { 300100 , 0.7932},   // 30 100 scalar 0.793201091443
  { 400100 , 0.8037},   // 40 100 scalar 0.803701105891
  { 450100 , 0.8132},   // 45 100 scalar 0.813201118963
  { 490100 , 0.8218},   // 49 100 scalar 0.821801130797
  { 510100 , 0.009434}, // 51 100 scalar 0.00943401298118
  { 550100 , 0.004640}, // 55 100 scalar 0.00464000638464
};

const float getXsecTTbarDM(int parity, int mChi, int mPhi) {
  // parity = 1 for scalar, parity = -1 for pseudo scalar
  int entry = parity * (10000*mChi + mPhi);
  
  if (ttbarDM_xsecs.count(entry) == 0) {
    std::cout << "[getXsecTTbarDM] >> Cannot find entry for parity= " << parity << ", mPhi= " << mPhi << ", mChi= " << mChi << "!!" << endl;
    return 1.0;
  }
  
  return ttbarDM_xsecs.at(entry);
}


const std::pair<std::map<unsigned,std::pair<int,int>>*, std::map<std::pair<int,int>,unsigned>*> loadDMMassesFromFile(std::string fname) {
  auto* lmmap = new std::map<unsigned,std::pair<int,int>>;
  auto* memap = new std::map<std::pair<int,int>,unsigned>;
  ifstream ifile(fname);
  if (ifile.fail()) {
    std::cout << "[ERROR] >> File " << fname << " does not seem to exist!!" << endl;
  }
  std::string line;
  unsigned lumi(0);
  int mchi(0), mphi(0);
  unsigned nevt(0);
  while (getline(ifile, line)) {
    if (line[0] == '#') continue;
    istringstream lstream(line);
    string entry;
    for (int i = 0; lstream >> std::ws >> entry; ++i) {
      switch (i) {
        case 0: lumi = stoul(entry); break;
        case 1: mchi = stoi(entry); break;
        case 2: mphi = stoi(entry); break;
        case 3: nevt = stoul(entry); break;
        default: std::cout << " >> Line " << line << " from file " << fname << " has too many arguments" << endl;
      }
    }
    if (lmmap->count(lumi) == 1) {
      std::cout << "[ERROR] >> Lumi " << lumi << " from file " << fname << " already exist!!" << endl;
    }
    auto cppair = std::make_pair(mchi, mphi);
    (*lmmap)[lumi] = cppair;
    if (memap->count(cppair) == 0)
      (*memap)[cppair] = nevt;
    else
      (*memap)[cppair] += nevt;
  }

  return std::make_pair(lmmap, memap);
}

// const auto [lmmap_scalar_16, memap_scalar_16] = loadDMMassesFromFile("../StopCORE/inputs/configs/TTbarDMJets_Inclusive_scalar_RunIISummar16NanoAODv7_masses.txt");
// const auto [lmmap_scalar_17, memap_scalar_17] = loadDMMassesFromFile("../StopCORE/inputs/configs/TTbarDMJets_Inclusive_scalar_RunIIFall17NanoAODv7_masses.txt");
// const auto [lmmap_scalar_18, memap_scalar_18] = loadDMMassesFromFile("../StopCORE/inputs/configs/TTbarDMJets_Inclusive_scalar_RunIIAutumn18NanoAODv7_masses.txt");
// const auto [lmmap_pseudo_16, memap_pseudo_16] = loadDMMassesFromFile("../StopCORE/inputs/configs/TTbarDMJets_Inclusive_pseudoscalar_RunIISummar16NanoAODv7_masses.txt");
// const auto [lmmap_pseudo_17, memap_pseudo_17] = loadDMMassesFromFile("../StopCORE/inputs/configs/TTbarDMJets_Inclusive_pseudoscalar_RunIIFall17NanoAODv7_masses.txt");
// const auto [lmmap_pseudo_18, memap_pseudo_18] = loadDMMassesFromFile("../StopCORE/inputs/configs/TTbarDMJets_Inclusive_pseudoscalar_RunIIAutumn18NanoAODv7_masses.txt");


std::tuple<int,int,unsigned> getDMMassesByLumi(int type, int year, unsigned lumi, std::string path = "configs") {

  // type = 1 for scalar, type = -1 for pseudoscalar, type = 0 do nothing
  static const std::map<unsigned,std::pair<int,int>>* lmmap = nullptr;
  static const std::map<std::pair<int,int>,unsigned>* memap = nullptr;

  if (!lmmap || !memap) {
    std::string fname;
    if (type == 1) {
      if (year == 2016) fname = path+"/TTbarDMJets_Inclusive_scalar_RunIISummer16NanoAODv7_masses.txt";
      if (year == 2017) fname = path+"/TTbarDMJets_Inclusive_scalar_RunIIFall17NanoAODv7_masses.txt";
      if (year == 2018) fname = path+"/TTbarDMJets_Inclusive_scalar_RunIIAutumn18NanoAODv7_masses.txt";
    } else if (type == -1) {
      if (year == 2016) fname = path+"/TTbarDMJets_Inclusive_pseudoscalar_RunIISummer16NanoAODv7_masses.txt";
      if (year == 2017) fname = path+"/TTbarDMJets_Inclusive_pseudoscalar_RunIIFall17NanoAODv7_masses.txt";
      if (year == 2018) fname = path+"/TTbarDMJets_Inclusive_pseudoscalar_RunIIAutumn18NanoAODv7_masses.txt";
    }
    std::cout << "[getDMMassesByLumi] >> Loading masses from " << fname << endl;
    std::tie(lmmap, memap) = loadDMMassesFromFile(fname);
  }

  if (!lmmap || lmmap->count(lumi) == 0) {
    std::cout << "[getDMMassesByLumi] >> Cannot find entry for type= " << type << ", year= " << year << ", lumi= " << lumi << "!!" << endl;
    return std::make_tuple(-1, -1, 0);
  }
  auto& cppair = lmmap->at(lumi);

  if (!memap || memap->count(cppair) == 0) {
    std::cout << "[getDMMassesByLumi] >> Cannot find entry for mass pair mchi = " << cppair.first << ", mphi = " << cppair.second << "!!" << endl;
    return std::make_tuple(cppair.first, cppair.second, 0);
  }

  return std::make_tuple(cppair.first, cppair.second, memap->at(cppair));  // mchi, mphi, nevt
}

}

#endif  // TTBARDM_UTILITIES_H

