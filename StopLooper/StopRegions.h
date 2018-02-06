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

std::vector<SR> getStopSignalRegionsBinInTopTag();
std::vector<SR> getStopControlRegionsNoBTagsBinInTopTag();
std::vector<SR> getStopControlRegionsDileptonBinInTopTag();

std::vector<SR> getStopCrosscheckRegionsEMu();
std::vector<SR> getStopInclusiveRegionsTopological();

#endif // STOPREGIONS_H
