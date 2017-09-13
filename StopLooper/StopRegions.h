#ifndef STOPREGIONS_H
#define STOPREGIONS_H

#include <string>
#include "TTree.h"
#include "SR.h"

std::vector<SR> getStopSignalRegions();
std::vector<SR> getStopControlRegions();
std::vector<SR> getStopControlRegionsDilepton();
std::vector<SR> getStopControlRegionsNoBTags();
std::vector<SR> getStopCrosscheckRegionsEMu();

std::vector<SR> getStopSignalRegionsTopological();
std::vector<SR> getStopControlRegionsNoBTagsTopological();

#endif // STOPREGIONS_H
