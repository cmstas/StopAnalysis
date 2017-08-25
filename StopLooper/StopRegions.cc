#include "StopRegions.h"

const float fInf = std::numeric_limits<float>::max();

std::vector<SR> getStopSignalRegions() {
  SR base;

  SR srbase;
  srbase.SetVar("mt", 50, -1);
  srbase.SetVar("met", 50, -1);
  srbase.SetVar("mt2w", 100, -1);
  srbase.SetVar("mlb", 0, -1);
  srbase.SetVar("tmod", 0, 2);
  srbase.SetVar("nlep", 1, 2);
  srbase.SetVar("njet", 2, -1);
  srbase.SetVar("nbjet", 1, -1);
  srbase.SetVar("dphijmet", 0.8, 3.14159);

  SR sr;
  std::vector<SR> SRvec;

  SRvec.emplace_back(srbase);

  // SR 2-3j

  sr = srbase;
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


  sr = srbase;
  sr.SetName("srB1");
  sr.SetDetailName("2to3j_tmod10toInf_mlb175toInf_met250to450");
  sr.SetVar("mlb", 175, -1);
  sr.SetVar("met", 250, 450);
  SRvec.emplace_back(sr);

  sr.SetName("srB2");
  sr.SetDetailName("2to3j_tmod10toInf_mlb175toInf_met450to600");
  sr.SetVar("met", 450, 600);
  SRvec.emplace_back(sr);

  sr.SetName("srB3");
  sr.SetDetailName("2to3j_tmod10toInf_mlb175toInf_met600toInf");
  sr.SetVar("met", 600, -1);
  SRvec.emplace_back(sr);


  sr.SetName("srH1");
  sr.SetDetailName("geq4j_tmod10toInf_mlb175toInf_met250to450");
  sr.SetVar("njet", 4, -1);
  sr.SetVar("met", 250, 450);
  SRvec.emplace_back(sr);

  sr.SetName("srH2");
  sr.SetDetailName("geq4j_tmod10toInf_mlb175toInf_met450toInf");
  sr.SetVar("met", 450, -1);
  SRvec.emplace_back(sr);


  sr.SetName("srE1");
  sr.SetDetailName("geq4j_tmod0to10_mlb0to175_met350to550");
  sr.SetVar("tmod", 0, 10);
  sr.SetVar("mlb", 0, 175);
  sr.SetVar("met", 350, 550);
  SRvec.emplace_back(sr);

  sr.SetName("srE2");
  sr.SetDetailName("geq4j_tmod0to10_mlb0to175_met550toInf");
  sr.SetVar("met", 550, -1);
  SRvec.emplace_back(sr);


  sr.SetName("srF1");
  sr.SetDetailName("geq4j_tmod0to10_mlb175toInf_met250to450");
  sr.SetVar("mlb", 175, -1);
  sr.SetVar("met", 250, 450);
  SRvec.emplace_back(sr);

  sr.SetName("srF2");
  sr.SetDetailName("geq4j_tmod0to10_mlb175toInf_met450toInf");
  sr.SetVar("met", 450, -1);
  SRvec.emplace_back(sr);


  sr = srbase;
  sr.SetName("sr1");
  sr.SetVar("njet", 2, 3);
  sr.SetVar("tmod", 10, -1);
  sr.SetVar("mlb", 0, 175);
  sr.SetVar("met", 250, 350);
  SRvec.emplace_back(sr);

  sr.SetName("sr2");
  sr.SetVar("met", 350, 450);
  SRvec.emplace_back(sr);

  sr.SetName("sr3");
  sr.SetVar("met", 450, 600);
  SRvec.emplace_back(sr);

  sr.SetName("sr4");
  sr.SetVar("met", 600, -1);
  SRvec.emplace_back(sr);

  sr.SetName("sr7");
  sr.SetVar("mlb", 175, -1);
  SRvec.emplace_back(sr);

  sr.SetName("sr6");
  sr.SetVar("met", 450, 600);
  SRvec.emplace_back(sr);

  sr.SetName("sr5");
  sr.SetVar("met", 250, 450);
  SRvec.emplace_back(sr);

  sr.SetName("sr8");

  return SRvec;
}

std::vector<SR> getStopControlRegionsDilepton() {
  std::vector<SR> CRvec;

  SR crbase;
  crbase.SetAllowDummyVars(1);

  crbase.SetName("cr2ltest");   // test
  crbase.SetVar("mt_rl", 0, fInf);
  crbase.SetVar("met_rl", 0, fInf);
  crbase.SetVar("tmod_rl", -fInf, fInf);
  crbase.SetVar("njet", 2, fInf);
  crbase.SetVar("nbjet", 0, fInf);
  crbase.SetVar("dphijmet_rl", 0, 3.14159);
  crbase.SetVar("nlep", 2, 3);
  CRvec.emplace_back(crbase);

  crbase.SetName("cr2lbase");
  crbase.SetVar("mt_rl", 150, fInf);
  crbase.SetVar("met_rl", 250, fInf);
  crbase.SetVar("mt2w_rl", 100, fInf);
  crbase.SetVar("nlep", 1, 3);
  crbase.SetVar("tmod_rl", 0, fInf);
  crbase.SetVar("nbjet", 1, fInf);
  crbase.SetVar("dphijmet_rl", 0.8, 3.14159);
  CRvec.emplace_back(crbase);

  SR cr;

  cr = crbase;
  cr.SetName("cr2lincl1");
  cr.SetDetailName("2to3j_inclusive");
  cr.SetVar("njet", 2, 4);
  CRvec.emplace_back(cr);

  cr.SetName("cr2lincl2");
  cr.SetDetailName("geq4j_inclusive");
  cr.SetVar("njet", 4, fInf);
  CRvec.emplace_back(cr);


  cr = crbase;
  cr.SetName("cr2lB1");
  cr.SetDetailName("2to3j_tmod10toInf_mlb175toInf_met250to450");
  cr.SetVar("mlb", 175, -1);
  cr.SetVar("met_rl", 250, 450);
  CRvec.emplace_back(cr);

  cr.SetName("cr2lB2");
  cr.SetDetailName("2to3j_tmod10toInf_mlb175toInf_met450to600");
  cr.SetVar("met_rl", 450, 600);
  CRvec.emplace_back(cr);

  cr.SetName("cr2lB3");
  cr.SetDetailName("2to3j_tmod10toInf_mlb175toInf_met600toInf");
  cr.SetVar("met_rl", 600, -1);
  CRvec.emplace_back(cr);


  cr.SetName("cr2lH1");
  cr.SetDetailName("geq4j_tmod10toInf_mlb175toInf_met250to450");
  cr.SetVar("njet", 4, -1);
  cr.SetVar("met_rl", 250, 450);
  CRvec.emplace_back(cr);

  cr.SetName("cr2lH2");
  cr.SetDetailName("geq4j_tmod10toInf_mlb175toInf_met450toInf");
  cr.SetVar("met_rl", 450, -1);
  CRvec.emplace_back(cr);


  cr.SetName("cr2lE1");
  cr.SetDetailName("geq4j_tmod0to10_mlb0to175_met350to550");
  cr.SetVar("tmod_rl", 0, 10);
  cr.SetVar("mlb", 0, 175);
  cr.SetVar("met_rl", 350, 550);
  CRvec.emplace_back(cr);

  cr.SetName("cr2lE2");
  cr.SetDetailName("geq4j_tmod0to10_mlb0to175_met550toInf");
  cr.SetVar("met_rl", 550, -1);
  CRvec.emplace_back(cr);


  cr.SetName("cr2lF1");
  cr.SetDetailName("geq4j_tmod0to10_mlb175toInf_met250to450");
  cr.SetVar("mlb", 175, -1);
  cr.SetVar("met_rl", 250, 450);
  CRvec.emplace_back(cr);

  cr.SetName("cr2lF2");
  cr.SetDetailName("geq4j_tmod0to10_mlb175toInf_met450toInf");
  cr.SetVar("met_rl", 450, -1);
  CRvec.emplace_back(cr);

  return CRvec;
}

std::vector<SR> getStopControlRegionsNoBTags() {
  SR base;

  SR crbase;
  crbase.SetVar("mt", 50, -1);
  crbase.SetVar("met", 50, -1);
  crbase.SetVar("mt2w", 100, -1);
  crbase.SetVar("mlb", 0, -1);
  crbase.SetVar("tmod", 0, 2);
  crbase.SetVar("nlep", 1, 2);
  crbase.SetVar("njet", 2, -1);
  crbase.SetVar("nbjet", 0, -1);
  crbase.SetVar("dphijmet", 0, 3.14159);
  // crbase.SetVar("ntightb", 0, -1);

  SR cr;
  std::vector<SR> CRvec;

  // CR 0b
  crbase.SetName("cr0b");
  crbase.SetVar("nbjet", 0, 1);

  cr = crbase;
  cr.SetName(crbase.GetName()+"_base");
  CRvec.emplace_back(cr);

  cr = crbase;
  cr.SetName(crbase.GetName()+"_4jets_highmlb_lowdm");
  CRvec.emplace_back(cr);

  cr.SetName(crbase.GetName()+"_4jets_highmlb_meddm");
  CRvec.emplace_back(cr);

  cr.SetName(crbase.GetName()+"_4jets_highmlb_highdm");
  crbase.SetVar("ntightb", 0, 1);
  CRvec.emplace_back(cr);

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
