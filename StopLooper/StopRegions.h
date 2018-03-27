#ifndef STOPREGIONS_H
#define STOPREGIONS_H

#include <string>
#include "TTree.h"
#include "SR.h"

std::vector<SR> getStopControlRegionsNoBTagsTopological(std::vector<SR>&& SRvec);
std::vector<SR> getStopControlRegionsDileptonTopological(std::vector<SR>&& SRvec);

std::vector<SR> getStopSignalRegionsTopological();
std::vector<SR> getStopControlRegionsNoBTagsTopological();
std::vector<SR> getStopControlRegionsDileptonTopological();

std::vector<SR> getStopSignalRegionsNewMETBinning();
std::vector<SR> getStopControlRegionsNoBTagsNewMETBinning();
std::vector<SR> getStopControlRegionsDileptonNewMETBinning();

std::vector<SR> getStopSignalRegionsAddResTagBin();
std::vector<SR> getStopControlRegionsNoBTagsAddResTagBin();
std::vector<SR> getStopControlRegionsDileptonAddResTagBin();

std::vector<SR> getStopSignalRegionsAddTopTagBins();
std::vector<SR> getStopControlRegionsNoBTagsAddTopTagBins();
std::vector<SR> getStopControlRegionsDileptonAddTopTagBins();

std::vector<SR> getStopSignalRegionsBinInResolvedTag();
std::vector<SR> getStopControlRegionsNoBTagsBinInResolvedTag();
std::vector<SR> getStopControlRegionsDileptonBinInResolvedTag();

std::vector<SR> getStopSignalRegionsBinInMergedTag();
std::vector<SR> getStopControlRegionsNoBTagsBinInMergedTag();
std::vector<SR> getStopControlRegionsDileptonBinInMergedTag();

std::vector<SR> getStopSignalRegionsBooleanTopTags();
std::vector<SR> getStopControlRegionsNoBTagsBooleanTopTags();
std::vector<SR> getStopControlRegionsDileptonBooleanTopTags();


std::vector<SR> getStopCrosscheckRegionsEMu();
std::vector<SR> getStopInclusiveRegionsTopological();

#endif // STOPREGIONS_H
