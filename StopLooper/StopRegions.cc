#include "PlotUtilities.h"
#include "StopRegions.h"

const float fInf = std::numeric_limits<float>::max();

std::vector<SR> getStopSignalRegionsTopological() {
  SR base;

  SR srbase;
  srbase.SetName("srbase");
  srbase.SetVar("mt", 150, fInf);
  srbase.SetVar("met", 250, fInf);
  srbase.SetVar("mt2w", 0, fInf);
  srbase.SetVar("mlb", 0, fInf);
  srbase.SetVar("tmod", -fInf, fInf);
  srbase.SetVar("nlep", 1, 2);
  srbase.SetVar("nvlep", 1, 2);
  srbase.SetVar("passvetos", 1, 2);
  srbase.SetVar("njet", 2, fInf);
  srbase.SetVar("nbjet", 1, fInf);
  srbase.SetVar("dphijmet", 0.8, 3.14159);

  SR sr;
  std::vector<SR> SRvec;

  SRvec.emplace_back(srbase);

  // SR 2-3j

  sr = srbase;
  sr.SetName("srA");
  sr.SetDetailName("2to3j_tmod10toInf_mlb0to175");
  sr.SetVar("njet", 2, 4);
  sr.SetVar("tmod", 10, fInf);
  sr.SetVar("mlb", 175, fInf);
  sr.SetVar("met", 250, fInf);
  sr.SetMETBins({250, 350, 450, 600, 1500});
  SRvec.emplace_back(sr);

  sr.SetName("srB");
  sr.SetDetailName("2to3j_tmod10toInf_mlb175toInf");
  sr.SetVar("njet", 2, 4);
  sr.SetVar("tmod", 10, fInf);
  sr.SetVar("mlb", 175, fInf);
  sr.SetMETBins({250, 450, 600, 1500});
  SRvec.emplace_back(sr);

  sr.SetName("srC");
  sr.SetDetailName("geq4j_tmodlt0_mlb0to175");
  sr.SetVar("njet", 4, fInf);
  sr.SetVar("tmod", -fInf, 0);
  sr.SetVar("mlb", 0, 175);
  sr.SetMETBins({250, 350, 450, 550, 650, 1500});
  SRvec.emplace_back(sr);

  sr.SetName("srD");
  sr.SetDetailName("geq4j_tmodlt0_mlb175toInf");
  sr.SetVar("tmod", -fInf, 0);
  sr.SetVar("mlb", 175, fInf);
  sr.SetMETBins({250, 350, 450, 550, 1500});
  SRvec.emplace_back(sr);

  sr.SetName("srE");
  sr.SetDetailName("geq4j_tmod0to10_mlb0to175");
  sr.SetVar("tmod", 0, 10);
  sr.SetVar("mlb", 0, 175);
  sr.SetMETBins({250, 350, 450, 1500});
  SRvec.emplace_back(sr);

  sr.SetName("srF");
  sr.SetDetailName("geq4j_tmod0to10_mlb175toInf");
  sr.SetVar("tmod", 0, 10);
  sr.SetVar("mlb", 175, fInf);
  sr.SetMETBins({250, 450, 1500});
  SRvec.emplace_back(sr);

  sr.SetName("srG");
  sr.SetDetailName("geq4j_tmod0to10_mlb0to175");
  sr.SetVar("tmod", 0, 10);
  sr.SetVar("mlb", 0, 175);
  sr.SetMETBins({250, 350, 450, 600, 1500});
  SRvec.emplace_back(sr);

  sr.SetName("srH");
  sr.SetDetailName("geq4j_tmod10toInf_mlb175toInf");
  sr.SetVar("njet", 4, -1);
  sr.SetVar("met", 250, 450);
  sr.SetMETBins({250, 450, 1500});
  SRvec.emplace_back(sr);

  // Compressed regions
  sr.SetName("srI");
  sr.SetDetailName("geq5j_lpt0to150");
  sr.SetVar("mt", 150, fInf);
  sr.SetVar("njet", 5, fInf);
  sr.SetVar("nbjet", 1, fInf);
  sr.SetVar("lep1pt", 0, 150);
  sr.SetVar("met", 250, 350);
  sr.SetVar("dphilmet", 0, 2);
  sr.SetVar("dphijmet", 0.5, 3.1416);
  sr.SetMETBins({250, 350, 450, 550, 1500});
  SRvec.emplace_back(sr);


  return SRvec;
}

std::vector<SR> getStopControlRegionsNoBTagsTopological() {

  std::vector<SR> CRvec;

  // SR crbase;
  // crbase.SetName("cr0bbase");
  // crbase.SetVar("mt", 0, fInf);
  // crbase.SetVar("met", 250, fInf);
  // crbase.SetVar("nlep", 1, 2);
  // crbase.SetVar("njet", 2, fInf);
  // crbase.SetVar("nbjet", 0, 1);
  // crbase.SetVar("dphijmet", 0.8, 3.14159);
  // crbase.SetAllowDummyVars(1);
  // CRvec.emplace_back(crbase);
  // SR cr;

  std::vector<SR> SRvec = getStopSignalRegions();
  for (SR cr : SRvec) {
    cr.SetName(cr.GetName().replace(0, 2, "cr0b"));
    cr.SetAllowDummyVars(1);
    cr.RemoveVar("mlb");
    cr.RemoveVar("tmod");
    cr.SetVar("nbjet", 0, 1);
    CRvec.emplace_back(cr);
  }

  return CRvec;
}

std::vector<SR> getStopSignalRegions() {
  SR base;

  SR srbase;
  srbase.SetVar("mt", 150, fInf);
  srbase.SetVar("met", 250, fInf);
  srbase.SetVar("mt2w", 0, fInf);
  srbase.SetVar("mlb", 0, fInf);
  srbase.SetVar("tmod", -fInf, fInf);
  srbase.SetVar("nlep", 1, 2);
  srbase.SetVar("nvlep", 1, 2);
  srbase.SetVar("passvetos", 1, 2);
  srbase.SetVar("njet", 2, fInf);
  srbase.SetVar("nbjet", 1, fInf);
  srbase.SetVar("dphijmet", 0.8, 3.14159);

  SR sr;
  std::vector<SR> SRvec;

  SRvec.emplace_back(srbase);

  // SR 2-3j

  sr.SetName("srA1");
  sr.SetDetailName("2to3j_tmod10toInf_mlb0to175_met250to350");
  sr.SetVar("njet", 2, 4);
  sr.SetVar("tmod", 10, -1);
  sr.SetVar("mlb", 175, -1);
  sr.SetVar("met", 250, 350);
  SRvec.emplace_back(sr);

  sr.SetName("srA2");
  sr.SetDetailName("2to3j_tmod10toInf_mlb0to175_met350to450");
  sr.SetVar("met", 350, 450);
  SRvec.emplace_back(sr);

  sr.SetName("srA3");
  sr.SetDetailName("2to3j_tmod10toInf_mlb0to175_met450to600");
  sr.SetVar("met", 450, 600);
  SRvec.emplace_back(sr);

  sr.SetName("srA4");
  sr.SetDetailName("2to3j_tmod10toInf_mlb0to175_met600toInf");
  sr.SetVar("met", 600, -1);
  SRvec.emplace_back(sr);


  // sr = srbase;
  // sr.SetName("srB1");
  // sr.SetDetailName("2to3j_tmod10toInf_mlb175toInf_met250to450");
  // sr.SetVar("mlb", 175, -1);
  // sr.SetVar("met", 250, 450);
  // SRvec.emplace_back(sr);

  // sr.SetName("srB2");
  // sr.SetDetailName("2to3j_tmod10toInf_mlb175toInf_met450to600");
  // sr.SetVar("met", 450, 600);
  // SRvec.emplace_back(sr);

  // sr.SetName("srB3");
  // sr.SetDetailName("2to3j_tmod10toInf_mlb175toInf_met600toInf");
  // sr.SetVar("met", 600, -1);
  // SRvec.emplace_back(sr);


  // sr.SetName("srH1");
  // sr.SetDetailName("geq4j_tmod10toInf_mlb175toInf_met250to450");
  // sr.SetVar("njet", 4, -1);
  // sr.SetVar("met", 250, 450);
  // SRvec.emplace_back(sr);

  // sr.SetName("srH2");
  // sr.SetDetailName("geq4j_tmod10toInf_mlb175toInf_met450toInf");
  // sr.SetVar("met", 450, -1);
  // SRvec.emplace_back(sr);


  // sr.SetName("srE1");
  // sr.SetDetailName("geq4j_tmod0to10_mlb0to175_met350to550");
  // sr.SetVar("tmod", 0, 10);
  // sr.SetVar("mlb", 0, 175);
  // sr.SetVar("met", 350, 550);
  // SRvec.emplace_back(sr);

  // sr.SetName("srE2");
  // sr.SetDetailName("geq4j_tmod0to10_mlb0to175_met550toInf");
  // sr.SetVar("met", 550, -1);
  // SRvec.emplace_back(sr);


  // sr.SetName("srF1");
  // sr.SetDetailName("geq4j_tmod0to10_mlb175toInf_met250to450");
  // sr.SetVar("mlb", 175, -1);
  // sr.SetVar("met", 250, 450);
  // SRvec.emplace_back(sr);

  // sr.SetName("srF2");
  // sr.SetDetailName("geq4j_tmod0to10_mlb175toInf_met450toInf");
  // sr.SetVar("met", 450, -1);
  // SRvec.emplace_back(sr);

  // sr.SetName("srI1");
  // sr.SetDetailName("geq5j_lpt0to150_met250to350");
  // sr.SetVar("mt", 150, -1);
  // sr.SetVar("njet", 5, -1);
  // sr.SetVar("nbjet", 1, -1);
  // sr.SetVar("lep1pt", 0, 150);
  // sr.SetVar("met", 250, 350);
  // sr.SetVar("dphilmet", 0, 2);
  // sr.SetVar("dphijmet", 0.5, 3.1416);
  // SRvec.emplace_back(sr);

  // sr.SetName("srI2");
  // sr.SetDetailName("geq5j_lpt0to150_met350to450");
  // sr.SetVar("met", 350, 450);
  // SRvec.emplace_back(sr);

  // sr.SetName("srI3");
  // sr.SetDetailName("geq5j_lpt0to150_met450to550");
  // sr.SetVar("met", 450, 550);
  // SRvec.emplace_back(sr);

  // sr.SetName("srI4");
  // sr.SetDetailName("geq5j_lpt0to150_met550toInf");
  // sr.SetVar("met", 550, -1);
  // SRvec.emplace_back(sr);


  return SRvec;
}

std::vector<SR> getStopControlRegionsDilepton() {
  std::vector<SR> CRvec;

  SR crbase;
  crbase.SetAllowDummyVars(1);

  crbase.SetName("cr2lbase");
  crbase.SetVar("mt_rl", 150, fInf);
  crbase.SetVar("met_rl", 250, fInf);
  crbase.SetVar("tmod_rl", -fInf, fInf); // temporary until the inefficiency is understood
  crbase.SetVar("nvlep", 2, 3);
  crbase.SetVar("nlep_rl", 2, 3);
  crbase.SetVar("njet", 2, fInf);
  crbase.SetVar("nbjet", 1, fInf);
  crbase.SetVar("dphijmet_rl", 0.8, 3.14159);
  CRvec.emplace_back(crbase);

  SR cr;

  cr.SetAllowDummyVars(1);
  cr.SetName("cr2ltest0");         // test for zpeak lumi
  cr.SetVar("met", 50, fInf);
  cr.SetVar("nvlep", 2, 3);
  cr.SetVar("nlep_rl", 2, 3);
  cr.SetVar("passlep1pt", 1, 2); // mu 40, e 45
  cr.SetVar("njet", 2, fInf);
  CRvec.emplace_back(cr);

  cr.SetName("cr2ltest1");         // test
  cr.SetVar("met", 50, fInf);
  cr.SetVar("nvlep", 2, 3);
  cr.SetVar("nlep_rl", 2, 3);
  // cr.SetVar("lep1pt", 40, fInf);
  cr.SetVar("passlep1pt", 1, 2); // mu 40, e 45
  cr.SetVar("njet", 2, fInf);
  cr.SetVar("nbjet", 1, fInf);
  // cr.SetVar("dphijmet_rl", 0.8, 3.14159);
  CRvec.emplace_back(cr);

  cr.SetName("cr2ltest3");         // test
  cr.SetVar("mt", 150, fInf);
  CRvec.emplace_back(cr);

  cr.SetName("cr2ltest4");         // test
  cr.SetVar("met", 150, fInf);
  CRvec.emplace_back(cr);

  // cr.SetName("cr2ltest5");         // test
  // cr.SetVar("nbjet", 1, fInf);
  // CRvec.emplace_back(cr);

  cr = crbase;
  cr.SetName("cr2lincl1");
  cr.SetDetailName("2to3j_inclusive");
  cr.SetVar("njet", 2, 4);
  CRvec.emplace_back(cr);

  cr.SetName("cr2lincl2");
  cr.SetDetailName("geq4j_inclusive");
  cr.SetVar("njet", 4, fInf);
  CRvec.emplace_back(cr);


  // cr = crbase;
  // cr.SetName("cr2lB1");
  // cr.SetDetailName("2to3j_tmod10toInf_mlb175toInf_met250to450");
  // cr.SetVar("mlb", 175, -1);
  // cr.SetVar("ntbtag", 1, -1);
  // cr.SetVar("met_rl", 250, 450);
  // CRvec.emplace_back(cr);

  // cr.SetName("cr2lB2");
  // cr.SetDetailName("2to3j_tmod10toInf_mlb175toInf_met450to600");
  // cr.SetVar("met_rl", 450, 600);
  // CRvec.emplace_back(cr);

  // cr.SetName("cr2lB3");
  // cr.SetDetailName("2to3j_tmod10toInf_mlb175toInf_met600toInf");
  // cr.SetVar("met_rl", 600, -1);
  // CRvec.emplace_back(cr);


  // cr.SetName("cr2lH1");
  // cr.SetDetailName("geq4j_tmod10toInf_mlb175toInf_met250to450");
  // cr.SetVar("njet", 4, -1);
  // cr.SetVar("met_rl", 250, 450);
  // CRvec.emplace_back(cr);

  // cr.SetName("cr2lH2");
  // cr.SetDetailName("geq4j_tmod10toInf_mlb175toInf_met450toInf");
  // cr.SetVar("met_rl", 450, -1);
  // CRvec.emplace_back(cr);


  // cr.SetName("cr2lE1");
  // cr.SetDetailName("geq4j_tmod0to10_mlb0to175_met350to550");
  // cr.SetVar("tmod_rl", 0, 10);
  // cr.SetVar("met_rl", 350, 550);
  // cr.SetVar("mlb", 0, 175);
  // cr.RemoveVar("ntbtag");
  // CRvec.emplace_back(cr);

  // cr.SetName("cr2lE2");
  // cr.SetDetailName("geq4j_tmod0to10_mlb0to175_met550toInf");
  // cr.SetVar("met_rl", 550, -1);
  // CRvec.emplace_back(cr);


  // cr.SetName("cr2lF1");
  // cr.SetDetailName("geq4j_tmod0to10_mlb175toInf_met250to450");
  // cr.SetVar("met_rl", 250, 450);
  // cr.SetVar("ntbtag", 1, -1);
  // cr.SetVar("mlb", 175, -1);
  // CRvec.emplace_back(cr);

  // cr.SetName("cr2lF2");
  // cr.SetDetailName("geq4j_tmod0to10_mlb175toInf_met450toInf");
  // cr.SetVar("met_rl", 450, -1);
  // CRvec.emplace_back(cr);

  return CRvec;
}

std::vector<SR> getStopControlRegionsNoBTags() {

  std::vector<SR> CRvec;

  SR crbase;
  crbase.SetName("cr0bbase");
  crbase.SetVar("mt", 150, fInf);
  crbase.SetVar("met", 150, fInf);
  crbase.SetVar("nlep", 1, 2);
  crbase.SetVar("nvlep", 1, 2);
  crbase.SetVar("passvetos", 1, 2);
  crbase.SetVar("njet", 2, fInf);
  crbase.SetVar("nbjet", 0, 1);
  crbase.SetVar("dphijmet", 0.8, 3.14159);
  crbase.SetAllowDummyVars(1);
  CRvec.emplace_back(crbase);

  SR cr;

  // CR tests
  cr.SetAllowDummyVars(1);
  cr.SetName("cr0btest1");         // test for no cut
  cr.SetVar("met", 150, fInf);
  cr.SetVar("nlep", 1, 2);
  cr.SetVar("nvleps", 1, 2);
  cr.SetVar("passvetos", 1, 2);
  // cr.SetVar("lep1pt", 40, fInf);
  cr.SetVar("passlep1pt", 1, 2); // mu 40, e 45
  cr.SetVar("njet", 2, fInf);
  cr.SetVar("nbjet", 0, 1);
  cr.SetVar("dphijmet", 0.8, 3.14159);
  CRvec.emplace_back(cr);

  cr.SetName("cr0btestdilep");
  cr.SetVar("nlep", 2, 3);
  cr.SetVar("osdilep", 1, 2);
  CRvec.emplace_back(cr);
  cr.SetVar("nlep", 1, 2);
  cr.RemoveVar("osdilep");

  cr.SetName("cr0btest2");         // test
  cr.SetVar("mt", 150, fInf);
  CRvec.emplace_back(cr);

  cr.SetName("cr0btest3");         // test
  cr.SetVar("met", 250, fInf);
  cr.RemoveVar("passLep1pt");
  cr.SetVar("lep1pt", 20, fInf);
  CRvec.emplace_back(cr);

  cr.SetName("crtest5");         // test
  cr.SetVar("mt", 0, fInf);
  CRvec.emplace_back(cr);

  // cr.SetName("cr0btest4");         // test
  // cr.SetVar("dphijmet", 0.8, 3.14159);
  // CRvec.emplace_back(cr);

  // cr.SetName("crtest5");         // test
  // cr.SetVar("tmod", 0, fInf);
  // CRvec.emplace_back(cr);

  
  // CR0b 
  cr = crbase;
  cr.SetName("cr0bincl1");
  cr.SetDetailName("2to3j_met250toInf");
  cr.SetVar("mt", 150, fInf);
  cr.SetVar("njet", 2, 4);
  cr.SetVar("met", 250, fInf);
  CRvec.emplace_back(cr);

  cr.SetName("cr0bincl2");
  cr.SetDetailName("geq4j_met250toInf");
  cr.SetVar("njet", 4, fInf);
  cr.SetVar("met", 250, fInf);
  CRvec.emplace_back(cr);

  // std::vector<SR> SRvec = getStopSignalRegions();
  // for (SR cr : SRvec) {
  //   cr.SetName(cr.GetName().replace(0, 2, "cr0b"));
  //   cr.SetAllowDummyVars(1);
  //   cr.RemoveVar("mlb");
  //   cr.RemoveVar("tmod");
  //   cr.SetVar("nbjet", 0, 1);
  //   CRvec.emplace_back(cr);
  // }

  return CRvec;
}

std::vector<SR> getStopControlRegionsEMu() {
  std::vector<SR> CRvec;

  SR crbase;

  crbase.SetName("cremu_base");
  crbase.SetVar("nlep", 1, 3);
  crbase.SetVar("met", 50, fInf);
  crbase.SetVar("njet", 2, fInf);
  crbase.SetAllowDummyVars(1);

  SR cr = crbase;
  cr.SetName("cremu1");
  cr.SetDetailName("geq0b_met50toInf");
  crbase.SetVar("nbjet", 0, fInf);
  CRvec.emplace_back(cr);

  cr.SetName("cremu2");
  cr.SetDetailName("geq1b_met50toInf");
  crbase.SetVar("nbjet", 1, fInf);
  CRvec.emplace_back(cr);

  cr.SetName("cremu3");
  cr.SetDetailName("geq2b_met50toInf");
  crbase.SetVar("nbjet", 2, fInf);
  CRvec.emplace_back(cr);

  return CRvec;
}
