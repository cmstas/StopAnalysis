#ifndef METCORRECTIONHANDLER_H
#define METCORRECTIONHANDLER_H

#include <vector>
#include <string>
#include <unordered_map>
#include <utility>
// #include "Samples.h"
#include "METObject.h"
#include "SystematicVariations.h"
#include "TString.h"

class METCorrectionHandler{
 public:
  METCorrectionHandler();
  ~METCorrectionHandler();

  enum DataVersion{
    kCMSSW_8_0_X,
    kCMSSW_9_4_X,
    kCMSSW_10_X
  };

  bool setup(int year = 2016, TString period = "2017", std::string METSFpath = "METSFs");
  void reset();

  bool hasMETCorrection() const{ return applyCorrection; }
  void correctMET(float const& genMET, float const& genMETPhi, METObject* obj, bool useFastSim) const;

  std::vector<TString> getValidDataPeriods();
  float getIntegratedLuminosity(TString const& period);
  void printParameters() const;

  int theDataYear;
  DataVersion theDataVersion;
  TString theDataPeriod;

 protected:
  bool applyCorrection;
  std::vector<float> lumilist;
  std::vector<std::vector<std::pair<float, float>>> values_data_map;
  std::unordered_map<SystematicsHelpers::SystematicVariationTypes, std::vector<std::vector<std::pair<float, float>>>> values_MC_map;

  void readFile(TString const& strinput);

};



#endif
