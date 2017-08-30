#ifndef STOPREGIONS_H
#define STOPREGIONS_H

#include <string>
#include "TTree.h"
#include "SR.h"

std::vector<SR> getStopSignalRegions();
std::vector<SR> getStopSignalRegionsTopological();
std::vector<SR> getStopControlRegions();
std::vector<SR> getStopControlRegionsDilepton();
std::vector<SR> getStopControlRegionsNoBTags();
std::vector<SR> getStopControlRegionsEMu();

#endif // STOPREGIONS_H
