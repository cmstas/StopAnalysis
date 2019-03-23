## Instructions for MET correction

There should be a container class METObject for all MET-related variables, SystematicVariations.h for an enumeration of systematics, METCorrectionHandler for 
the actual corrector, and Samples for indicating which year/data period we are looking at. The METSFs folder contains the parameters in a human-readable text file format.
Once you copy them, you will see a few compiler errors due to missing variables (like missing STOP1LPKGDATAPATH variable, which you need to replace with the 
path of where you put METSFs folder). These errors should be easy to fix (let me know if you have any questions or experience difficulties).
All parameters are derived assuming MC is always METv2 (regardless of data used), and data is v2 in 31Mar2018 recos (eras B-F) or v0 in 09May re-reco of Era F. 
Consequently, I don’t have MC v2 -> F09May v2 (unless there is really a good reason).
The main operation principle can be summarized with the code snippet at the end of this message.


## Example

// Setup the env.
SampleHelpers::theDataYear=2017;
SampleHelpers::theDataVersion=SampleHelpers::kCMSSW_9_4_X; // Doesn’t matter rn, but won’t hurt
SampleHelpers::theDataPeriod = period; // period is “2017” for average 2017, “2017B”…”2017F” for 31Mar recos, and “2017F-09May2018” for 2017F 09 May re-reco. “2017” is what you should use for the sum of B-F, and dedicated period flags to compare only to dedicated eras
 
// Setup the corrector
metCorrector.setup();
 
// Do this for MC or data
// pfmet, pfmet_JEC* are after jet energy scale corrections and MET recipe (hence JER is the same as nominal since JER smearing is not propagated in the current MET recipes)
      METObject metobj;
      metobj.extras.met
        = metobj.extras.met_METup = metobj.extras.met_METdn
        = metobj.extras.met_JERup = metobj.extras.met_JERdn
        = metobj.extras.met_PUup = metobj.extras.met_PUdn
        = pfmet;
      metobj.extras.met_JECup = pfmet_JECup;
      metobj.extras.met_JECdn = pfmet_JECdn;
      metobj.extras.phi
        = metobj.extras.phi_METup = metobj.extras.phi_METdn
        = metobj.extras.phi_JERup = metobj.extras.phi_JERdn
        = metobj.extras.phi_PUup = metobj.extras.phi_PUdn
        = pfmetPhi;
      metobj.extras.phi_JECup = pfmetPhi_JECup;
      metobj.extras.phi_JECdn = pfmetPhi_JECdn;
 
// Then do this only on MC
// genMET and genMETPhi are gen_met and gen_metPhi branch aliases in CMS4
// metobj variables are overwritten after this call, so beware…
metCorrector.correctMET(genMET, genMETPhi, &metobj, isFastSim); // Last flag is for fast sim., but there are no separate factors, so it doesn’t matter
 
// Once metobj is corrected, use the corresponding variables in the corresponding systematics (i.e. if you are considering histograms after the pile-up SF up systematic application, use met_PUup, not nominal met!)
// The reason is that the corrections are data-driven, so the MET resolution after whatever systematic effect should still match the data-extracted resolution.
// The resolution is affected by the JECs, JERs and PU, so their effect on the resolution needs to be taken out; this is why you have different parameterizations for the three.
// MET up/dn is the systematic variations on the data-driven method and should be applied as a separate systematic from now on, uncorrelated with JEC/JER/PU.
