#include "eventWeightInfo.h"

//////////////////////////////////////////////////////////////////////

eventWeightInfo::eventWeightInfo( sampleInfo::ID sample, bool useBTagUtils, bool useLepSFUtils ){
  
  // Get sample info from enum
  sample_info = new sampleInfo::sampleUtil( sample );

  // Utilty Var Constants
  DR_MATCHED = 0.1;
  LUMI       = 0.8042;
  LUMI_ERR   = LUMI*0.04;

  // Initialize Switches for additional SFs
  apply_bTag_sf    = false;
  apply_lep_sf     = false;
  apply_vetoLep_sf = false;
  apply_lepFS_sf   = false;
  apply_topPt_sf   = false;
  apply_metRes_sf  = false;
  apply_nJetsK3_sf = false;
  apply_nJetsK4_sf = false;
  apply_diNuPt_sf  = false;
  apply_ISR_sf     = false;
  apply_sample_sf  = false;

  // Decision to use Utilities vs direct from baby for bTag and lep SFs
  useBTagSFs_fromUtils = useBTagUtils;
  useLepSFs_fromUtils  = useLepSFUtils;
  

  // Initialize event weights and related variables
  initializeWeights();


  // Initialize baby weights histograms
  h_sig_counter = NULL;
  h_sig_counter_nEvents = NULL;
  h_bkg_counter = NULL;


  // Get Signal XSection File
  if( sample_info->isSignal ){
    f_sig_xsec = new TFile("../StopCORE/inputs/signal_xsec/xsec_stop_13TeV.root","read");
    h_sig_xsec = (TH1D*)f_sig_xsec->Get("stop");
  }


  // Initialize bTag SF machinery
  if( !sample_info->isData && useBTagSFs_fromUtils ){
    //bTagSFUtil = new eventWeight_bTagSF( sample_info->isFastsim );
  }

  
  // Initialize Lepton Scale Factors
  if( !sample_info->isData && useLepSFs_fromUtils ){
    lepSFUtil  = new eventWeight_lepSF( sample_info->isFastsim );
  }
   
}

//////////////////////////////////////////////////////////////////////

eventWeightInfo::~eventWeightInfo(){
  
  sample_info->~sampleUtil();
  
  if( !sample_info->isData && useBTagSFs_fromUtils){
    //delete bTagSFUtil;
  }
  
  if( !sample_info->isData && useLepSFs_fromUtils){
    delete lepSFUtil;
  }
  
  if( sample_info->isSignal ){
    f_sig_xsec->Close();
    delete f_sig_xsec;
  }
  
}

//////////////////////////////////////////////////////////////////////

void eventWeightInfo::getWeightHistogramFromBaby( TFile *sourceFile ){

  // Get counter histogram from source file
  if( sample_info->isSignal ){
    h_sig_counter = (TH3D*)sourceFile->Get("h_counterSMS");
    h_sig_counter_nEvents = (TH2D*)sourceFile->Get("histNEvts");
  }
  else if( !sample_info->isData ){
    h_bkg_counter = (TH1D*)sourceFile->Get("h_counter");
  }

  return;
  
}

//////////////////////////////////////////////////////////////////////

void eventWeightInfo::initializeWeights(){

  // Variables to form baseline event weight
  nEvents = 1.0;
  xsec = 1.0;
  xsec_err = 0.0;

  mStop = 1.0;
  mLSP = 1.0;
  mChargino = 1.0;
  
  // Event weights for each systematic
  sf_nominal = 1.0;
  
  sf_bTag = 1.0;
  sf_bTagEffHF_up = 1.0;
  sf_bTagEffHF_dn = 1.0;
  sf_bTagEffLF_up = 1.0;
  sf_bTagEffLF_dn = 1.0;
  
  sf_lep = 1.0;
  sf_lep_up = 1.0;
  sf_lep_dn = 1.0;
  
  sf_vetoLep = 1.0;
  sf_vetoLep_up = 1.0;
  sf_vetoLep_dn = 1.0;
  
  sf_lepFS = 1.0;
  sf_lepFS_up = 1.0;
  sf_lepFS_dn = 1.0;
  
  sf_topPt = 1.0;
  sf_topPt_up = 1.0;
  sf_topPt_dn = 1.0;
  
  sf_metRes = 1.0;
  sf_metRes_up = 1.0;
  sf_metRes_dn = 1.0;
  
  sf_nJetsK3 = 1.0;
  sf_nJetsK3_up = 1.0;
  sf_nJetsK3_dn = 1.0;
  
  sf_nJetsK4 = 1.0;
  sf_nJetsK4_up = 1.0;
  sf_nJetsK4_dn = 1.0;
  
  sf_diNuPt = 1.0;
  sf_diNuPt_up = 1.0;
  sf_diNuPt_dn = 1.0;
  
  sf_ISR = 1.0;
  sf_ISR_up = 1.0;
  sf_ISR_dn = 1.0;
  
  sf_nuPt_up = 1.0;
  sf_nuPt_dn = 1.0;
  
  sf_Wwidth_up = 1.0;
  sf_Wwidth_dn = 1.0;
  
  sf_hfXsec_up = 1.0;
  sf_hfXsec_dn = 1.0;
  
  sf_alphas_up = 1.0;
  sf_alphas_dn = 1.0;
  
  sf_q2_up = 1.0;
  sf_q2_dn = 1.0;
  
  sf_lumi = 1.0;
  sf_lumi_up = 1.0;
  sf_lumi_dn = 1.0;
  
  sf_xsec_up = 1.0;
  sf_xsec_dn = 1.0;

  sf_sample = 1.0;
  
  return;
}


//////////////////////////////////////////////////////////////////////

void eventWeightInfo::getEventWeights(){
  
  initializeWeights();


  // If sample is data
  if( sample_info->isData ) return;
 

  // If sample is signal scan
  if( sample_info->isSignal ) getSusyMasses( mStop, mLSP );
  

  // Get nEvents
  getNEvents( nEvents );


  // Get xSec
  getXSecWeight( xsec, sf_xsec_up, sf_xsec_dn );  


  // Get Scale1fb*Lumi Wgt
  getScaleToLumiWeight( sf_nominal );

  
  // btag
  getBTagWeight( sf_bTag, sf_bTagEffHF_up, sf_bTagEffHF_dn, sf_bTagEffLF_up, sf_bTagEffLF_dn ); 
  
 
  // lepSFs
  getLepSFWeight( sf_lep, sf_lep_up, sf_lep_dn, sf_lepFS, sf_lepFS_up, sf_lepFS_dn, sf_vetoLep, sf_vetoLep_up, sf_vetoLep_dn );

  
  // top pT Reweighting
  getTopPtWeight( sf_topPt, sf_topPt_up, sf_topPt_dn );


  // MET resolution scale factors
  getMetResWeight( sf_metRes, sf_metRes_up, sf_metRes_dn );

  
  // nJets, ==3, K3, scale factor
  getNJetsSF_K3( sf_nJetsK3, sf_nJetsK3_up, sf_nJetsK3_dn );

  
  // nJets, >=4, K4, scale factor
  getNJetsSF_K4( sf_nJetsK4, sf_nJetsK4_up, sf_nJetsK4_dn );


  // diNuetrino pT scale factor
  getDiNuPtSF( sf_diNuPt, sf_diNuPt_up, sf_diNuPt_dn );


  // Nuetrino pT scale factor
  getNuPtSF( sf_nuPt_up, sf_nuPt_dn );


  // W width scale factor
  getWwidthSF( sf_Wwidth_up, sf_Wwidth_dn );

  
  // W+HF xsec uncertainty
  getWJetsHFXSecSF( sf_hfXsec_up, sf_hfXsec_dn );
  

  // Alpha Strong, QCD variation
  getAlphasWeight( sf_alphas_up, sf_alphas_dn );
  

  // Q2 Variation, muF, muR
  getQ2Weight( sf_q2_up, sf_q2_dn );
  

  // Lumi Variation
  getLumi( sf_lumi, sf_lumi_up, sf_lumi_dn );


  // ISR Correction
  if( sample_info->isSignal ) getISRWeight( sf_ISR, sf_ISR_up, sf_ISR_dn );
  

  // Sample Scale Factor
  sf_sample = getSampleWeight( sample_info->id );
  
  return;
}

//////////////////////////////////////////////////////////////////////

void eventWeightInfo::getSusyMasses( double &mStop, double &mLSP ){

  mStop = 1.0;
  mLSP  = 1.0;
  //mChargino = 1.0;

  if( sample_info->isSignal ){
    mStop     = babyAnalyzer.mass_stop();
    mLSP      = babyAnalyzer.mass_lsp();
    //mChargino = babyAnalyzer.mass_chargino();
  }

  return;
}

//////////////////////////////////////////////////////////////////////

void eventWeightInfo::getNEvents( int &nEvts ){

  nEvts = 1;

  if( sample_info->isSignal ){

    getSusyMasses(mStop,mLSP);

    //nEvts = (int)h_sig_counter_nEvents->GetBinContent(h_sig_counter->FindBin(mStop,mLSP));
  }
  else{
    //nEvts = h_bkg_counter->GetBinContent(22);
  }

  return;
}

//////////////////////////////////////////////////////////////////////

void eventWeightInfo::getScaleToLumiWeight( double &wgt ){

  wgt = 1.0;

  if( sample_info->isData ) return;

  if( sample_info->isSignal ){
    getSusyMasses(mStop,mLSP);
    getXSecWeight( xsec, sf_xsec_up, sf_xsec_dn );  
    getNEvents( nEvents );
    wgt = LUMI*1000.0*xsec/(double)nEvents;
  }
  else{
    wgt = babyAnalyzer.scale1fb()*LUMI;
  }

  return;
}

//////////////////////////////////////////////////////////////////////

void eventWeightInfo::getBTagWeight( double &wgt_btagsf, double &wgt_btagsf_hf_up, double &wgt_btagsf_hf_dn, double &wgt_btagsf_lf_up, double &wgt_btagsf_lf_dn ){

  wgt_btagsf       = 1.0;
  wgt_btagsf_hf_up = 1.0;
  wgt_btagsf_hf_dn = 1.0;
  wgt_btagsf_lf_up = 1.0;
  wgt_btagsf_lf_dn = 1.0;

  if( !apply_bTag_sf ) return;

  if( useBTagSFs_fromUtils ){
    //getBTagWeight_fromUtils( wgt_btagsf, wgt_btagsf_hf_up, wgt_btagsf_hf_dn, wgt_btagsf_lf_up, wgt_btagsf_lf_dn ); 
  }
  else{
    wgt_btagsf       = babyAnalyzer.weight_btagsf();
    wgt_btagsf_hf_up = babyAnalyzer.weight_btagsf_heavy_UP();
    wgt_btagsf_hf_dn = babyAnalyzer.weight_btagsf_heavy_DN();
    wgt_btagsf_lf_up = babyAnalyzer.weight_btagsf_light_UP();
    wgt_btagsf_lf_dn = babyAnalyzer.weight_btagsf_light_DN();
  }
  
  // Normalization
  getNEvents( nEvents );
  
  if( sample_info->isSignal ){
    getSusyMasses(mStop,mLSP);
  
    wgt_btagsf       *= ( nEvents / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,14)) );
    wgt_btagsf_hf_up *= ( nEvents / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,15)) );
    wgt_btagsf_hf_dn *= ( nEvents / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,17)) );
    wgt_btagsf_lf_up *= ( nEvents / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,16)) );
    wgt_btagsf_lf_dn *= ( nEvents / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,18)) );
  }
  else{
    wgt_btagsf       *= ( nEvents / h_bkg_counter->GetBinContent(14) );
    wgt_btagsf_hf_up *= ( nEvents / h_bkg_counter->GetBinContent(15) );
    wgt_btagsf_hf_dn *= ( nEvents / h_bkg_counter->GetBinContent(17) );
    wgt_btagsf_lf_up *= ( nEvents / h_bkg_counter->GetBinContent(16) );
    wgt_btagsf_lf_dn *= ( nEvents / h_bkg_counter->GetBinContent(18) );
  }
  
}

//////////////////////////////////////////////////////////////////////

void eventWeightInfo::getBTagWeight_fromUtils( double &wgt_btagsf, double &wgt_btagsf_hf_up, double &wgt_btagsf_hf_dn, double &wgt_btagsf_lf_up, double &wgt_btagsf_lf_dn ){

  vector< double > jet_pt, jet_eta;
  vector< int > jet_flavour;
  vector< bool > jet_passCSV;

  for(int iJet=0; iJet<(int)babyAnalyzer.ak4pfjets_p4().size(); iJet++){
    jet_pt.push_back( (double)babyAnalyzer.ak4pfjets_p4().at(iJet).Pt() );
    jet_eta.push_back( (double)babyAnalyzer.ak4pfjets_p4().at(iJet).Eta() );
    jet_flavour.push_back( (int)abs(babyAnalyzer.ak4pfjets_hadron_flavor().at(iJet)) );
    jet_passCSV.push_back( (bool)babyAnalyzer.ak4pfjets_passMEDbtag().at(iJet) );
  }
  //bTagSFUtil->getBTagWeight( jet_pt, jet_eta, jet_flavour, jet_passCSV, wgt_btagsf, wgt_btagsf_hf_up, wgt_btagsf_hf_dn, wgt_btagsf_lf_up, wgt_btagsf_lf_dn );

  return;

}

//////////////////////////////////////////////////////////////////////

void eventWeightInfo::getLepSFWeight( double &weight_lepSF, double &weight_lepSF_Up, double &weight_lepSF_Dn, double &weight_lepFSSF, double &weight_lepFSSF_Up, double &weight_lepFSSF_Dn, double &weight_vetoLepSF, double &weight_vetoLepSF_Up, double &weight_vetoLepSF_Dn ){

  weight_lepSF = 1.0;
  weight_lepSF_Up = 1.0;
  weight_lepSF_Dn = 1.0;
  weight_lepFSSF = 1.0;
  weight_lepFSSF_Up = 1.0;
  weight_lepFSSF_Dn = 1.0;
  weight_vetoLepSF = 1.0;
  weight_vetoLepSF_Up = 1.0;
  weight_vetoLepSF_Dn = 1.0;

  if( !apply_lep_sf && !apply_vetoLep_sf && !apply_lepFS_sf ) return;
  
  if( useLepSFs_fromUtils ){
    getLepSFWeight_fromUtils( weight_lepSF, weight_lepSF_Up, weight_lepSF_Dn, weight_lepFSSF, weight_lepFSSF_Up, weight_lepFSSF_Dn, weight_vetoLepSF, weight_vetoLepSF_Up, weight_vetoLepSF_Dn );
  }
  else{
    //sf_lep = babyAnalyzer.weight_lepSF();
    //sf_lep_up = babyAnalyzer.weight_lepSF_up();
    //sf_lep_dn = babyAnalyzer.weight_lepSF_dn();
    
    //sf_vetoLep = babyAnalyzer.weight_vetoLepSF();
    //sf_vetoLep_up = babyAnalyzer.weight_vetoLepSF_up();
    //sf_vetoLep_dn = babyAnalyzer.weight_vetoLepSF_dn();
    
    //sf_lepFS = babyAnalyzer.weight_lepSF_fastSim();
    //sf_lepFS_up = babyAnalyzer.weight_lepSF_fastSim_up();
    //sf_lepFS_dn = babyAnalyzer.weight_lepSF_fastSim_dn();
  }

  
  if( !apply_lep_sf ){
    weight_lepSF = 1.0;
    weight_lepSF_Up = 1.0;
    weight_lepSF_Dn = 1.0;
  }
  if( !apply_vetoLep_sf ){
    weight_vetoLepSF = 1.0;
    weight_vetoLepSF_Up = 1.0;
    weight_vetoLepSF_Dn = 1.0;
  }
  if( !apply_lepFS_sf ){
    weight_lepFSSF = 1.0;
    weight_lepFSSF_Up = 1.0;
    weight_lepFSSF_Dn = 1.0;
  }
  

  return;
}

//////////////////////////////////////////////////////////////////////

void eventWeightInfo::getLepSFWeight_fromUtils( double &weight_lepSF, double &weight_lepSF_Up, double &weight_lepSF_Dn, double &weight_lepFSSF, double &weight_lepFSSF_Up, double &weight_lepFSSF_Dn, double &weight_vetoLepSF, double &weight_vetoLepSF_Up, double &weight_vetoLepSF_Dn ){

  weight_lepSF = 1.0;
  weight_lepSF_Up = 1.0;
  weight_lepSF_Dn = 1.0;
  weight_lepFSSF = 1.0;
  weight_lepFSSF_Up = 1.0;
  weight_lepFSSF_Dn = 1.0;
  weight_vetoLepSF = 1.0;
  weight_vetoLepSF_Up = 1.0;
  weight_vetoLepSF_Dn = 1.0;

  std::vector< double > recoLep_pt, recoLep_eta, genLostLep_pt, genLostLep_eta;
  std::vector< int > recoLep_pdgid, genLostLep_pdgid;
  std::vector< bool > recoLep_isSel;
  
  if( babyAnalyzer.nvetoleps()==1 ){
    recoLep_pt.push_back( babyAnalyzer.lep1_p4().Pt() );
    recoLep_eta.push_back( babyAnalyzer.lep1_p4().Eta() );
    recoLep_pdgid.push_back( babyAnalyzer.lep1_pdgid() );
    if( (abs(babyAnalyzer.lep1_pdgid())==13 && 
	 babyAnalyzer.lep1_passTightID() ) ||
	(abs(babyAnalyzer.lep1_pdgid())==11 && 
	 babyAnalyzer.lep1_passMediumID() )   ){
      recoLep_isSel.push_back( true );
    }
    else{
      recoLep_isSel.push_back( false );
    }
  }
  
  if( babyAnalyzer.nvetoleps()>=2 ){
    recoLep_pt.push_back( babyAnalyzer.lep1_p4().Pt() );
    recoLep_eta.push_back( babyAnalyzer.lep1_p4().Eta() );
    recoLep_pdgid.push_back( babyAnalyzer.lep1_pdgid() );
    if( (abs(babyAnalyzer.lep1_pdgid())==13 && 
	 babyAnalyzer.lep1_passTightID())  ||
	(abs(babyAnalyzer.lep1_pdgid())==11 && 
	 babyAnalyzer.lep1_passMediumID())    ){
      recoLep_isSel.push_back( true );
    }
    else{
      recoLep_isSel.push_back( false );
    }
    
    recoLep_pt.push_back( babyAnalyzer.lep2_p4().Pt() );
    recoLep_eta.push_back( babyAnalyzer.lep2_p4().Eta() );
    recoLep_pdgid.push_back( babyAnalyzer.lep2_pdgid() );
    if( (abs(babyAnalyzer.lep2_pdgid())==13 && 
	 babyAnalyzer.lep2_passTightID())  ||
	(abs(babyAnalyzer.lep2_pdgid())==11 && 
	 babyAnalyzer.lep2_passMediumID())    ){
      recoLep_isSel.push_back( true );
    }
    else{
      recoLep_isSel.push_back( false );
    }

  }
 


  if( babyAnalyzer.is2lep() && babyAnalyzer.nvetoleps()==1 ){

    // Find gen lepton matched to reco lepton
    int matched_idx = -1;
    double min_dr   = 99.9;
    for(int iGen=0; iGen<(int)babyAnalyzer.genleps_p4().size(); iGen++){
      if( !babyAnalyzer.genleps_isLastCopy().at(iGen) ) continue;
      
      // Preferentially match to same flavour lepton before tau
      if( (abs(babyAnalyzer.genleps_id().at(iGen))==11 && 
	   abs(babyAnalyzer.lep1_pdgid())==11                      ) ||
	  (abs(babyAnalyzer.genleps_id().at(iGen))==13 &&
	   abs(babyAnalyzer.lep1_pdgid())==13                      )    ){

	double min_dr_temp = ROOT::Math::VectorUtil::DeltaR( babyAnalyzer.genleps_p4().at(iGen), babyAnalyzer.lep1_p4() );
	if( min_dr_temp < min_dr ){
	  min_dr = min_dr_temp;
	  if( min_dr < DR_MATCHED ){
	    matched_idx = iGen;
	  }
	}
	
      } // end if dr matched to same flavour lepton
    

    } // end loop over gen leptons

    
    if( matched_idx < 0 ){
      for(int iGen=0; iGen<(int)babyAnalyzer.genleps_p4().size(); iGen++){
	if( !babyAnalyzer.genleps_isLastCopy().at(iGen) ) continue;
	if( abs(babyAnalyzer.genleps_id().at(iGen))==15 ){
	 
	  double min_dr_temp = ROOT::Math::VectorUtil::DeltaR( babyAnalyzer.genleps_p4().at(iGen), babyAnalyzer.lep1_p4() );
	  if( min_dr_temp < min_dr ){
	    min_dr = min_dr_temp;
	    if( min_dr < DR_MATCHED ){
	      matched_idx = iGen;
	    }
	  }
	  
	} // end if gen/reco matched to tau
      } // end loop over gen leptons
    } // end if reco lepton not matched

    if( matched_idx >= 0 ){
      // Find gen lost lepton
      double genLostPt  = -99.9;
      double genLostEta = -99.9;
      int genLostPdgid  = -99;
      bool foundLostLep = false;
      for(int iGen=0; iGen<(int)babyAnalyzer.genleps_p4().size(); iGen++){
	if( iGen == matched_idx ) continue;
	if( !babyAnalyzer.genleps_isLastCopy().at(iGen) ) continue;
	if( !babyAnalyzer.genleps_fromHardProcessFinalState().at(iGen) ) continue;
	if( abs(babyAnalyzer.genleps_id().at(iGen))!=11 && 
	    abs(babyAnalyzer.genleps_id().at(iGen))!=13    ) continue;
	
	genLostPt    = babyAnalyzer.genleps_p4().at(iGen).Pt();
	genLostEta   = babyAnalyzer.genleps_p4().at(iGen).Eta();
	genLostPdgid = babyAnalyzer.genleps_id().at(iGen);
	foundLostLep = true;
	
      } // end loop over gen leptons
    
      if( foundLostLep ){
	genLostLep_pt.push_back( genLostPt );
	genLostLep_eta.push_back( genLostEta );
	genLostLep_pdgid.push_back( genLostPdgid );
      }

    } // end if selected lepton is matched

  } // end if is gen 2lep event with ==1 reco lepton 

  lepSFUtil->getLepWeight( recoLep_pt, recoLep_eta, recoLep_pdgid, recoLep_isSel, genLostLep_pt, genLostLep_eta, genLostLep_pdgid, weight_lepSF, weight_lepSF_Up, weight_lepSF_Dn, weight_lepFSSF, weight_lepFSSF_Up, weight_lepFSSF_Dn, weight_vetoLepSF, weight_vetoLepSF_Up, weight_vetoLepSF_Dn );


  return;

}

//////////////////////////////////////////////////////////////////////

void eventWeightInfo::getTopPtWeight( double &weight_topPt, double &weight_topPt_up, double &weight_topPt_dn ){

  weight_topPt    = 1.0;
  weight_topPt_up = 1.0;
  weight_topPt_dn = 1.0;

  if( sample_info->isData ) return;

  if( sample_info->id != sampleInfo::k_ttbar_powheg_pythia8 &&
      sample_info->id != sampleInfo::k_ttbar_powheg_pythia8_ext3 &&
      sample_info->id != sampleInfo::k_ttbar_madgraph_pythia8 &&
      sample_info->id != sampleInfo::k_ttbar_singleLeptFromT_madgraph_pythia8 &&
      sample_info->id != sampleInfo::k_ttbar_singleLeptFromT_madgraph_pythia8_ext1 &&
      sample_info->id != sampleInfo::k_ttbar_singleLeptFromTbar_madgraph_pythia8 &&
      sample_info->id != sampleInfo::k_ttbar_singleLeptFromTbar_madgraph_pythia8_ext1 &&
      sample_info->id != sampleInfo::k_ttbar_diLept_madgraph_pythia8 &&
      sample_info->id != sampleInfo::k_ttbar_diLept_madgraph_pythia8_ext1 ) return;
  
  
  double genTopPt    = 0.0;
  double genTopBarPt = 0.0;
  double nTopsFound = 0;
  for(int iTop=0; iTop<(int)babyAnalyzer.genqs_p4().size(); iTop++){
    if(!babyAnalyzer.genqs_isLastCopy().at(iTop)) continue;
    if(babyAnalyzer.genqs_id().at(iTop)==6){
      genTopPt = babyAnalyzer.genqs_p4().at(iTop).Pt();
      nTopsFound++;
    }
    if(babyAnalyzer.genqs_id().at(iTop)==-6){
      genTopBarPt = babyAnalyzer.genqs_p4().at(iTop).Pt();
      nTopsFound++;
    }
  }

  if( !(genTopPt>0.0) )    cout << "    Did not find gen top for top pT reweighting" << endl;
  if( !(genTopBarPt>0.0) ) cout << "    Did not find gen topBar for top pT reweighting" << endl;
  if( nTopsFound!=2 )      cout << "    Did not find 2 tops for top pT reweighting, nFound=" << nTopsFound << endl;


  double weight_temp = topPtWeight(genTopPt, genTopBarPt);

  
  if( apply_topPt_sf ){
    weight_topPt    = weight_temp;
    weight_topPt_up = ( ((weight_temp-1.0)*2.0)+1.0 );
    weight_topPt_dn = 1.0;
  }
  // if !apply_topPt_sf still use as uncertainty
  else{
    weight_topPt    = 1.0;
    weight_topPt_up = weight_temp;
    weight_topPt_dn = 1.0;
  }


  return;
}

//////////////////////////////////////////////////////////////////////

void eventWeightInfo::getMetResWeight( double &weight_metRes, double &weight_metRes_up, double &weight_metRes_dn ){

  weight_metRes    = 1.0;
  weight_metRes_up = 1.0;
  weight_metRes_dn = 1.0;

  if(!apply_metRes_sf) return;

  double sf_val = 1.0;
  double sf_err = 0.0;

  if( sample_info->isData ) return;
  if( sample_info->isSignal ) return;

  if( babyAnalyzer.ngoodjets()==2 ){

    if( babyAnalyzer.pfmet()>=250.0 && babyAnalyzer.pfmet()<350.0 ){
      sf_val = 1.032;
      sf_err = 0.009;
    }

    if( babyAnalyzer.pfmet()>=350.0 ){
      sf_val = 0.913;
      sf_err = 0.024;
    }

  } // end if nJets==2

  if( babyAnalyzer.ngoodjets()==3 ){

    if( babyAnalyzer.pfmet()>=250.0 && babyAnalyzer.pfmet()<350.0 ){
      sf_val = 1.032;
      sf_err = 0.014;
    }

    if( babyAnalyzer.pfmet()>=350.0 ){
      sf_val = 0.928;
      sf_err = 0.030;
    }

  } // end if nJets==3

  if( babyAnalyzer.ngoodjets()>=4 ){

    if( babyAnalyzer.MT2W()<200.0 ){

      if( babyAnalyzer.pfmet()>=250.0 && babyAnalyzer.pfmet()<325.0 ){
	sf_val = 1.048;
	sf_err = 0.025;
      }

      if( babyAnalyzer.pfmet()>=325.0 ){
	sf_val = 0.944;
	sf_err = 0.029;
      }

    } // end if MT2W<200.0
    
    if( babyAnalyzer.MT2W()>=200.0 ){
      
      if( babyAnalyzer.pfmet()>=250.0 && babyAnalyzer.pfmet()<350.0 ){
	sf_val = 1.050;
	sf_err = 0.021;
      }
      
      if( babyAnalyzer.pfmet()>=350.0 && babyAnalyzer.pfmet()<450.0 ){
	sf_val = 0.910;
	sf_err = 0.051;
      }

      if( babyAnalyzer.pfmet()>=450.0 ){
	sf_val = 0.924;
	sf_err = 0.063;
      }

    } // end if MT2W>=200.0
    

  } // end if nJets>=4
  

  weight_metRes    = sf_val;
  weight_metRes_up = (sf_val + sf_err);
  weight_metRes_dn = (sf_val - sf_err);

  return;
}

//////////////////////////////////////////////////////////////////////

void eventWeightInfo::getNJetsSF_K3( double &weight_K3, double &weight_K3_up, double &weight_K3_dn ){

  weight_K3    = 1.0;
  weight_K3_up = 1.0;
  weight_K3_dn = 1.0;

  if(!apply_nJetsK3_sf) return;

  double sf_val = 1.0;
  double sf_err = 1.0;

  if( sample_info->isData ) return;
  if( sample_info->isSignal ) return;

  if( sample_info->id != sampleInfo::k_ttbar_powheg_pythia8 &&
      sample_info->id != sampleInfo::k_ttbar_powheg_pythia8_ext3 ) return;

  if( !babyAnalyzer.is2lep() ) return;

  if( babyAnalyzer.ngoodjets()==3 ){

    sf_val = 1.08;
    sf_err = 0.06;
    
    weight_K3    = sf_val;
    weight_K3_up = (sf_val + sf_err);
    weight_K3_dn = (sf_val - sf_err);
    
  }

  return;
}

//////////////////////////////////////////////////////////////////////

void eventWeightInfo::getNJetsSF_K4( double &weight_K4, double &weight_K4_up, double &weight_K4_dn ){

  weight_K4    = 1.0;
  weight_K4_up = 1.0;
  weight_K4_dn = 1.0;

  if(!apply_nJetsK4_sf) return;

  double sf_val = 1.0;
  double sf_err = 1.0;

  if( sample_info->isData ) return;
  if( sample_info->isSignal ) return;

  if( sample_info->id != sampleInfo::k_ttbar_powheg_pythia8 &&
      sample_info->id != sampleInfo::k_ttbar_powheg_pythia8_ext3 ) return;

  if( !babyAnalyzer.is2lep() ) return;

  if( babyAnalyzer.ngoodjets()>=4 ){

    sf_val = 0.92;
    sf_err = 0.06;
    
    weight_K4    = sf_val;
    weight_K4_up = (sf_val + sf_err);
    weight_K4_dn = (sf_val - sf_err);

  }

  return;
}

//////////////////////////////////////////////////////////////////////

void eventWeightInfo::getDiNuPtSF( double &weight_diNuPt, double &weight_diNuPt_up, double &weight_diNuPt_dn ){

  weight_diNuPt    = 1.0;
  weight_diNuPt_up = 1.0;
  weight_diNuPt_dn = 1.0;

  if(!apply_diNuPt_sf) return;

  double sf_val = 1.0;
  double sf_err = 1.0;

  if( sample_info->isData ) return;
  if( sample_info->isSignal ) return;

  if( sample_info->id != sampleInfo::k_ttbar_powheg_pythia8 &&
      sample_info->id != sampleInfo::k_ttbar_powheg_pythia8_ext3 ) return;

  if( !babyAnalyzer.is2lep() ) return;

  if( babyAnalyzer.pfmet()>=250.0 && babyAnalyzer.pfmet()<350.0 ){
    sf_val = 1.054;
    sf_err = 0.043;
  }

  if( babyAnalyzer.pfmet()>=350.0 && babyAnalyzer.pfmet()<450.0 ){
    sf_val = 0.947;
    sf_err = 0.201;
  }

  if( babyAnalyzer.pfmet()>=450.0 ){
    sf_val = 0.729;
    sf_err = 0.320;
  }

  weight_diNuPt    = sf_val;
  weight_diNuPt_up = (sf_val + sf_err );
  weight_diNuPt_dn = (sf_val - sf_err );

  return;
}

//////////////////////////////////////////////////////////////////////

void eventWeightInfo::getNuPtSF( double &weight_nuPt_up, double &weight_nuPt_dn ){

  weight_nuPt_up = 1.0;
  weight_nuPt_dn = 1.0;

  double sf_val = 1.0;
  double sf_err = 0.0;

  if( sample_info->isData ) return;
  if( sample_info->isSignal ) return;

  if( !babyAnalyzer.is1lepFromW() ) return;

  if( babyAnalyzer.ngoodjets()==2 && 
      babyAnalyzer.topnessMod()>=6.4 ){
    
    if( babyAnalyzer.pfmet()>=250.0 ) sf_err = 0.0654;
    if( babyAnalyzer.pfmet()>=350.0 ) sf_err = 0.1796;

  } // end if nJets==2 modTop>6.4

  if( babyAnalyzer.ngoodjets()==3 &&
      babyAnalyzer.MT2W()>=200.0     ){

    if( babyAnalyzer.pfmet()>=250.0 ) sf_err = 0.1310;
    if( babyAnalyzer.pfmet()>=350.0 ) sf_err = 0.1530;
 
  } // end if nJets==3 && MT2W>200

  if( babyAnalyzer.ngoodjets()>=4 &&
      babyAnalyzer.MT2W()>=200.0     ){
    
    if( babyAnalyzer.pfmet()>=250.0 ) sf_err = 0.1280;
    if( babyAnalyzer.pfmet()>=350.0 ) sf_err = 0.1840;
    if( babyAnalyzer.pfmet()>=450.0 ) sf_err = 0.2220;
    
  } // end if nJets>=4 && MT2W>200

  weight_nuPt_up = (sf_val + sf_err);
  weight_nuPt_dn = (sf_val - sf_err);

  return;

}

//////////////////////////////////////////////////////////////////////

void eventWeightInfo::getWwidthSF( double &weight_Wwidth_up, double &weight_Wwidth_dn ){

  weight_Wwidth_up = 1.0;
  weight_Wwidth_dn = 1.0;

  double sf_val = 1.0;
  double sf_err = 0.0;

  if( sample_info->isData ) return;
  if( sample_info->isSignal ) return;

  if( !babyAnalyzer.is1lepFromW() ) return;

  if( babyAnalyzer.ngoodjets()==2 && 
      babyAnalyzer.topnessMod()>=6.4 ){
    
    sf_err = 0.03;

  } // end if nJets==2 modTop>6.4

  if( babyAnalyzer.ngoodjets()==3 &&
      babyAnalyzer.MT2W()>=200.0     ){

    sf_err = 0.03;

  } // end if nJets==3 && MT2W>200

  if( babyAnalyzer.ngoodjets()>=4 &&
      babyAnalyzer.MT2W()>=200.0     ){
    
    sf_err = 0.03;

  } // end if nJets>=4 && MT2W>200

  weight_Wwidth_up = (sf_val + sf_err);
  weight_Wwidth_dn = (sf_val - sf_err);

  return;

}

//////////////////////////////////////////////////////////////////////

void eventWeightInfo::getWJetsHFXSecSF( double &weight_hfXsec_up, double &weight_hfXsec_dn ){

  weight_hfXsec_up = 1.0;
  weight_hfXsec_dn = 1.0;
  
  double sf_val = 1.0;
  double sf_err = 0.0;

  if( sample_info->isData ) return;
  if( sample_info->isSignal ) return;

  if( sample_info->id != sampleInfo::k_WJetsToLNu &&
      sample_info->id != sampleInfo::k_WJetsToLNu_amcnlo_pythia8 &&
      sample_info->id != sampleInfo::k_WJetsToLNu_HT100ToInf_madgraph_pythia8 &&
      sample_info->id != sampleInfo::k_WJetsToLNu_HT100To200_madgraph_pythia8 &&
      sample_info->id != sampleInfo::k_WJetsToLNu_HT200To400_madgraph_pythia8 &&
      sample_info->id != sampleInfo::k_WJetsToLNu_HT400To600_madgraph_pythia8 &&
      sample_info->id != sampleInfo::k_WJetsToLNu_HT600ToInf_madgraph_pythia8 &&
      sample_info->id != sampleInfo::k_WJetsToLNu_HT600To800_madgraph_pythia8 &&
      sample_info->id != sampleInfo::k_WJetsToLNu_HT800To1200_madgraph_pythia8 &&
      sample_info->id != sampleInfo::k_WJetsToLNu_HT1200To2500_madgraph_pythia8 &&
      sample_info->id != sampleInfo::k_WJetsToLNu_HT2500ToInf_madgraph_pythia8 ) return;
  
  if( !babyAnalyzer.is1lepFromW() ) return;

  bool isWHF = false;
  for(int iJet= 0; iJet<(int)babyAnalyzer.ak4pfjets_p4().size(); iJet++){
    if(abs(babyAnalyzer.ak4pfjets_hadron_flavor().at(iJet))==5){
      isWHF = true;
      break;
    } // end if W+bets
  } // end loop over jets

  if( !isWHF ) return;

  if( babyAnalyzer.ngoodjets()==2 && 
      babyAnalyzer.topnessMod()>=6.4 ){
    
    sf_err = 0.50;

  } // end if nJets==2 modTop>6.4

  if( babyAnalyzer.ngoodjets()==3 &&
      babyAnalyzer.MT2W()>=200.0     ){

    sf_err = 0.50;

  } // end if nJets==3 && MT2W>200

  if( babyAnalyzer.ngoodjets()>=4 &&
      babyAnalyzer.MT2W()>=200.0     ){
    
    sf_err = 0.50;

  } // end if nJets>=4 && MT2W>200

  weight_hfXsec_up = (sf_val + sf_err);
  weight_hfXsec_dn = (sf_val - sf_err);

  return;

}

//////////////////////////////////////////////////////////////////////

void eventWeightInfo::getAlphasWeight( double &weight_alphas_up, double &weight_alphas_dn ){

  weight_alphas_up = 1.0;
  weight_alphas_dn = 1.0;

  if( sample_info->isData ) return;

  if( babyAnalyzer.genweights().size() < 110 ) return;
  
  weight_alphas_up = babyAnalyzer.genweights().at(109)/babyAnalyzer.genweights().at(0);
  weight_alphas_dn = babyAnalyzer.genweights().at(110)/babyAnalyzer.genweights().at(0);

  // Normalization
  if( sample_info->isSignal ){
    weight_alphas_up *= (  h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,1)) / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,12))  );
    weight_alphas_dn *= (  h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,1)) / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,13))  );
  }
  else{
    //weight_alphas_up *= ( h_bkg_counter->GetBinContent(1) / h_bkg_counter->GetBinContent(12) );
    //weight_alphas_dn *= ( h_bkg_counter->GetBinContent(1) / h_bkg_counter->GetBinContent(13) );
  }

  if( weight_alphas_up<0.0 || weight_alphas_dn<0.0 ){
    weight_alphas_up = 1.0;
    weight_alphas_dn = 1.0;
  }

  return;
}

//////////////////////////////////////////////////////////////////////

void eventWeightInfo::getQ2Weight( double &weight_q2_up, double &weight_q2_dn ){

  weight_q2_up = 1.0;
  weight_q2_dn = 1.0;

  if( sample_info->isData ) return;

  if( babyAnalyzer.genweights().size() < 110 ) return;

  weight_q2_up = babyAnalyzer.genweights().at(4)/babyAnalyzer.genweights().at(0);
  weight_q2_dn = babyAnalyzer.genweights().at(8)/babyAnalyzer.genweights().at(0);

  // Normalization
  if( sample_info->isSignal ){
    
    weight_q2_up *= ( h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,1)) / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,5)) );
    weight_q2_dn *= ( h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,1)) / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,9)) );
  }
  else{
    //weight_q2_up *= ( h_bkg_counter->GetBinContent(1) / h_bkg_counter->GetBinContent(5) );
    //weight_q2_dn *= ( h_bkg_counter->GetBinContent(1) / h_bkg_counter->GetBinContent(9) );
  }


  return;
}

//////////////////////////////////////////////////////////////////////

void eventWeightInfo::getLumi( double &weight_lumi, double &weight_lumi_up, double &weight_lumi_dn ){

  weight_lumi    = 1.0;
  weight_lumi_up = 1.0;
  weight_lumi_dn = 1.0;

  if( sample_info->isData ) return;
  
  weight_lumi    = LUMI;
  weight_lumi_up = (LUMI+LUMI_ERR);
  weight_lumi_dn = (LUMI-LUMI_ERR);

  return;

}

//////////////////////////////////////////////////////////////////////

void eventWeightInfo::getISRWeight( double &weight_ISR, double &weight_ISR_up, double &weight_ISR_dn ){

  weight_ISR    = 1.0;
  weight_ISR_up = 1.0;
  weight_ISR_dn = 1.0;

  if(!apply_ISR_sf) return;

  if( sample_info->isData ) return;
  if( !sample_info->isSignal ) return;

  weight_ISR    = babyAnalyzer.weight_ISR();
  weight_ISR_up = babyAnalyzer.weight_ISRup();
  weight_ISR_dn = babyAnalyzer.weight_ISRdown();

  // Normalization
  getNEvents(nEvents);

  weight_ISR    *= ( nEvents / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,19)) );
  weight_ISR_up *= ( nEvents / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,20)) );
  weight_ISR_dn *= ( nEvents / h_sig_counter->GetBinContent(h_sig_counter->FindBin(mStop,mLSP,21)) );

  return;
}

//////////////////////////////////////////////////////////////////////

void eventWeightInfo::getXSecWeight( double &weight_xsec, double &weight_xsec_up, double &weight_xsec_dn ){

  weight_xsec    = 1.0;
  weight_xsec_up = 1.0;
  weight_xsec_dn = 1.0;

  double xsec_val = 1.0;
  double xsec_err = 0.0;
  
  if( sample_info->isData ) return;

  if( !sample_info->isSignal ){
    xsec_val = babyAnalyzer.xsec();
  }
  else{
    getSusyMasses(mStop,mLSP);
    xsec_val = h_sig_xsec->GetBinContent(h_sig_xsec->FindBin(mStop));
    xsec_err = h_sig_xsec->GetBinError(h_sig_xsec->FindBin(mStop));
  }
  weight_xsec    = xsec_val;
  weight_xsec_up = (xsec_val+xsec_err);
  weight_xsec_dn = (xsec_val-xsec_err);

  return;

}

//////////////////////////////////////////////////////////////////////

double eventWeightInfo::getSampleWeight( sampleInfo::ID sample ){

  double result = 1.0;

  if(!apply_sample_sf ) return result;

  switch( sample ){

  default:
    break;
  };

  return result;
}

//////////////////////////////////////////////////////////////////////
