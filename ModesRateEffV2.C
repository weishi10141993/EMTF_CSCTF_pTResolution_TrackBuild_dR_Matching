#include "TFile.h"
#include "TSystem.h"
#include "TChain.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH2.h"

//****************************
//* USER modify below ONLY   *
//****************************
#include "Read_FlatNtuple.h" //make sure it's up-to-date with the Ntuple
//================================
//= Part I: Kinematics
//================================
const bool verbose = false; // Debug
const int PT_UP = 30;//Reco pT range
const int PT_LOW = 0;
const int Log2_PT_UP = 15;//upper limit in single mu files
const int Log2_PT_LOW = 0;
const float ETA_UP = 2.4;//Reco eta @station 2
const float ETA_LOW = 1.25;
const int MAX_EVT = -1;   // Max number of events to process
const int PRT_EVT = 10000;   // Print every N events
//2017 modes
const int SingleMu[4]={15,14,13,11};
const int DoubleMuInc[7]={12,10,7,15,14,13,11};
const int MuOpenInc[11]={9,6,5,3,12,10,7,15,14,13,11};
/*
//2018 modes
const int SingleMu[4]={15,14,13,11};
const int DoubleMuInc[7]={9,10,7,15,14,13,11};
const int MuOpenInc[11]={12,6,5,3,9,10,7,15,14,13,11};
*/
//==============
//= Part II: I/O
//==============
TString store = "/home/ws13/TMVA/TMVA/INPUT/"; //main dir
TString in_dir = "Ntuples/"; //sub dir
TString outFile = "/home/ws13/TMVA/TMVA/Study/ModesRateEff"; //output
TString SingleMu_files[1] = { 
	  		    "NTuple_SingleMuon_FlatNtuple_Run_306154_2018_05_07_SingleMu_2017_emul.root"//2017 emulator
			    //"NTuple_SingleMuon_FlatNtuple_Run_306154_2018_05_07_SingleMu_2018_emul_dTh4.root"//2018 emulator changes
	                    //"NTuple_SingleMuon_FlatNtuple_Run_306154_2018_05_07_SingleMu_2018_emul_dTh6.root"  
                            //"NTuple_SingleMuon_FlatNtuple_Run_306154_2018_05_07_SingleMu_2018_emul_dTh8.root"
};   
TString ZeroBias_files[1] = { 
	  		    "NTuple_ZeroBias1_FlatNtuple_Run_306091_2018_05_07_ZB1_2017_emul.root"//2017 emulator: 1487898 processed events
		            //"NTuple_ZeroBias1_FlatNtuple_Run_306091_2018_05_07_ZB1_2018_emul_dTh4.root"//2018 emulator: 1487898 
		            //"NTuple_ZeroBias1_FlatNtuple_Run_306091_2018_05_07_ZB1_2018_emul_dTh6.root"//1475029
		 	    //"NTuple_ZeroBias1_FlatNtuple_Run_306091_2018_05_07_ZB1_2018_emul_dTh8.root"//1487898
};
const double scale = 28500000./1487898;//Hz
const int nSingleMu = 4;//# of modes
const int nDoubleMuInc = 7;
const int nMuOpenInc = 11;
const int USESingleMu = 1;//# of SM files to use
const int USEZerobias = 1;//# of ZB files to use
//****************************
//* USER modify above ONLY   *
//****************************

void ModesRateEffV2() {
   
  // Initialize empty file to access each file in the list
  TFile *file_tmp(0);

  // List of input files
  std::vector<TString> SM_in_file_names;//SingleMu
  std::vector<TString> ZB_in_file_names;//Zerobias
  TString SM_file_name;
  TString ZB_file_name;
  
  for (int i = 0; i < USESingleMu; i++) {
	  SM_file_name.Form( "%s/%s/%s", store.Data(), in_dir.Data(), SingleMu_files[i].Data() );
  	  std::cout << "Adding file " << SM_file_name.Data() << std::endl;
          SM_in_file_names.push_back(SM_file_name.Data());
  }
  for (int i = 0; i < USEZerobias; i++) {
	  ZB_file_name.Form( "%s/%s/%s", store.Data(), in_dir.Data(), ZeroBias_files[i].Data() );
  	  std::cout << "Adding file " << ZB_file_name.Data() << std::endl;
          ZB_in_file_names.push_back(ZB_file_name.Data());
  }
  
  // Open all input files
  for (int i = 0; i < SM_in_file_names.size(); i++) {
    if ( !gSystem->AccessPathName(SM_in_file_names.at(i)) )
      file_tmp = TFile::Open( SM_in_file_names.at(i) ); // Check if file exists
    if (!file_tmp) {
      std::cout << "ERROR: could not open data file " << SM_in_file_names.at(i) << std::endl;
      return;
    }
  }

  for (int i = 0; i < ZB_in_file_names.size(); i++) {
    if ( !gSystem->AccessPathName(ZB_in_file_names.at(i)) )
      file_tmp = TFile::Open( ZB_in_file_names.at(i) ); // Check if file exists
    if (!file_tmp) {
      std::cout << "ERROR: could not open data file " << ZB_in_file_names.at(i) << std::endl;
      return;
    }
  }
	
  // Add tree from the input files to the TChain
  TChain *SM_in_chain= new TChain("FlatNtupleData/tree");
  for (int i = 0; i < SM_in_file_names.size(); i++) {
    SM_in_chain->Add( SM_in_file_names.at(i) );
  }
	
  TChain *ZB_in_chain= new TChain("FlatNtupleData/tree");
  for (int i = 0; i < ZB_in_file_names.size(); i++) {
    ZB_in_chain->Add( ZB_in_file_names.at(i) );
  }
	
  TString SMRecoPtTitle="";
  SMRecoPtTitle = SMRecoPtTitle + "RECO pT [" + Form("%d", PT_LOW)+", "+ Form("%d", PT_UP) + "]GeV, looseID, ReachStationOne, " + "abs(eta_St2) [" + Form("%.2f", ETA_LOW)+", "+ Form("%.2f", ETA_UP) + "]";
  TString SMLogRecoPtTitle="";
  SMLogRecoPtTitle = SMLogRecoPtTitle + "Log2(RECO pT) [" + Form("%d", Log2_PT_LOW)+", "+ Form("%d", Log2_PT_UP) + "], looseID, ReachStationOne, " + "abs(eta_St2) [" + Form("%.2f", ETA_LOW)+", "+ Form("%.2f", ETA_UP) + "]";
  TH1F *SMRecoPt = new TH1F("SMRecoPt", SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);
  TH1F *SMLogRecoPt = new TH1F("SMLogRecoPt", SMLogRecoPtTitle, Log2_PT_UP-Log2_PT_LOW, Log2_PT_LOW, Log2_PT_UP);
	
  TH1F *SMRecoPtMatchBX0SingleMu = new TH1F("SMRecoPtMatchBX0SingleMu", "Match BX0 "+ SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);
  TH1F *SMRecoPtMatchBX0SingleMuPlateau = new TH1F("SMRecoPtMatchBX0SingleMuPlateau", "Match BX0 Plateau "+ SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);	
  TH1F *SMRecoPtUniqueMatchBX0SingleMu = new TH1F("SMRecoPtUniqueMatchBX0SingleMu", "Match BX0 Unique "+ SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);
  TH1F *SMLogRecoPtMatchBX0SingleMu = new TH1F("SMLogRecoPtMatchBX0SingleMu", "Match BX0 "+ SMLogRecoPtTitle, Log2_PT_UP-Log2_PT_LOW, Log2_PT_LOW, Log2_PT_UP);
  TH1F *SMLogRecoPtMatchBX0SingleMuPlateau = new TH1F("SMLogRecoPtMatchBX0SingleMuPlateau", "Match BX0 Plateau "+ SMLogRecoPtTitle, Log2_PT_UP-Log2_PT_LOW, Log2_PT_LOW, Log2_PT_UP);	
  TH1F *SMLogRecoPtUniqueMatchBX0SingleMu = new TH1F("SMLogRecoPtUniqueMatchBX0SingleMu", "Match BX0 Unique "+ SMLogRecoPtTitle, Log2_PT_UP-Log2_PT_LOW, Log2_PT_LOW, Log2_PT_UP);
	
  TH1F *SMRecoPtMatchBX0DoubleMuInc = new TH1F("SMRecoPtMatchBX0DoubleMuInc", "Match BX0 "+ SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);
  TH1F *SMRecoPtMatchBX0DoubleMuIncPlateau = new TH1F("SMRecoPtMatchBX0DoubleMuIncPlateau", "Match BX0 Plateau"+ SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);	
  TH1F *SMRecoPtUniqueMatchBX0DoubleMuInc = new TH1F("SMRecoPtUniqueMatchBX0DoubleMuInc", "Match BX0 Unique"+ SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);
  TH1F *SMLogRecoPtMatchBX0DoubleMuInc = new TH1F("SMLogRecoPtMatchBX0DoubleMuInc", "Match BX0 "+ SMLogRecoPtTitle, Log2_PT_UP-Log2_PT_LOW, Log2_PT_LOW, Log2_PT_UP);
  TH1F *SMLogRecoPtMatchBX0DoubleMuIncPlateau = new TH1F("SMLogRecoPtMatchBX0DoubleMuIncPlateau", "Match BX0 Plateau "+ SMLogRecoPtTitle, Log2_PT_UP-Log2_PT_LOW, Log2_PT_LOW, Log2_PT_UP);	
  TH1F *SMLogRecoPtUniqueMatchBX0DoubleMuInc = new TH1F("SMLogRecoPtUniqueMatchBX0DoubleMuInc", "Match BX0 Unique "+ SMLogRecoPtTitle, Log2_PT_UP-Log2_PT_LOW, Log2_PT_LOW, Log2_PT_UP);

  TH1F *SMRecoPtMatchBX0MuOpenInc = new TH1F("SMRecoPtMatchBX0MuOpenInc", "Match BX0 "+ SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);
  TH1F *SMRecoPtMatchBX0MuOpenIncPlateau = new TH1F("SMRecoPtMatchBX0MuOpenIncPlateau", "Match BX0 Plateau"+ SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);	
  TH1F *SMRecoPtUniqueMatchBX0MuOpenInc = new TH1F("SMRecoPtUniqueMatchBX0MuOpenInc", "Match BX0 Unique"+ SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);
  TH1F *SMLogRecoPtMatchBX0MuOpenInc = new TH1F("SMLogRecoPtMatchBX0MuOpenInc", "Match BX0 "+ SMLogRecoPtTitle, Log2_PT_UP-Log2_PT_LOW, Log2_PT_LOW, Log2_PT_UP);
  TH1F *SMLogRecoPtMatchBX0MuOpenIncPlateau = new TH1F("SMLogRecoPtMatchBX0MuOpenIncPlateau", "Match BX0 Plateau "+ SMLogRecoPtTitle, Log2_PT_UP-Log2_PT_LOW, Log2_PT_LOW, Log2_PT_UP);	
  TH1F *SMLogRecoPtUniqueMatchBX0MuOpenInc = new TH1F("SMLogRecoPtUniqueMatchBX0MuOpenInc", "Match BX0 Unique "+ SMLogRecoPtTitle, Log2_PT_UP-Log2_PT_LOW, Log2_PT_LOW, Log2_PT_UP);

  //Initialize variables for rate
  double TrigPT[30]={0};
  for(int i=0;i<30;i++){
	  TrigPT[i]=i*1.0;
  }
  
  double Count[30]={0};
  double CountSingleMu[30]={0};
  double CountDoubleMuInc[30]={0};
  double CountMuOpenInc[30]={0};
	
  InitializeMaps();
  SetBranchAddresses(SM_in_chain);
  SetBranchAddresses(ZB_in_chain);
	
  std::cout << "\n******* About to loop over the SingleMu events *******" << std::endl;
  int nSMEvents = SM_in_chain->GetEntries();
	
  //Loop SingleMu 
  for (int iEvt = 0; iEvt < nSMEvents; iEvt++) {
    if (iEvt > MAX_EVT && MAX_EVT !=-1) break;
    if ( (iEvt % PRT_EVT) == 0 ) {
      std::cout << "\n*************************************" << std::endl;
      std::cout << "Looking at event " << iEvt << " out of " << nSMEvents << std::endl;
      std::cout << "*************************************" << std::endl;
    }

    SM_in_chain->GetEntry(iEvt);
    
    // From Read_FlatNtuple.h, use 'I("branch_name")' to get an integer branch value, 'F("branch_name") to get a float
    // Print info for unpacked EMTF tracks
    if (verbose) std::cout << "\n" << I("nRecoMuons") << " reco muons in the event" << std::endl;
	
    //Skip events without HLT fired
    if ( I("nRecoMuonsTrig")==0 ) continue;
	
    //==================
    //Normal track study
    //==================
    for (int ireco = 0; ireco < I("nRecoMuons"); ireco++) {   
	    
	    //HLT match
	    if( I("nRecoMuonsTrig")>=2 || 
	      ( I("nRecoMuonsTrig")==1 && I("nRecoMuonsTrigCen")==1 && I("reco_trig_ID", ireco) < 0 ) 
	      ){
		    
		    //select RECO muon
		    if( F("reco_pt", ireco) > 0 && 
		        I("reco_ID_loose", ireco) == 1 &&
		        ( F("reco_pt", ireco) < 16 || I("reco_ID_medium", ireco) == 1 ) &&
		        ( F("reco_pt", ireco) < 64 || I("reco_ID_tight", ireco) == 1 ) &&
		        fabs( F("reco_eta_St1",ireco) ) > ETA_LOW && fabs( F("reco_eta_St1", ireco) ) < ETA_UP &&
		        fabs( F("reco_eta",ireco) ) > ETA_LOW && fabs( F("reco_eta", ireco) ) < ETA_UP
		      ){
			    if(F("reco_pt", ireco) >= PT_LOW && F("reco_pt", ireco) <= PT_UP){
				    SMRecoPt->Fill( F("reco_pt", ireco) );
			    }
			    if(log2( F("reco_pt", ireco) ) >= Log2_PT_LOW && log2( F("reco_pt", ireco) ) <= Log2_PT_UP){
				    SMLogRecoPt->Fill( log2( F("reco_pt", ireco) ) );
			    }
			    
			    //***********************
		            //* IsRECOMatch && BX=0 *
		            //***********************
			    if( I("reco_dR_match_nTrk", ireco) > 0 &&
			        I("trk_BX", I("reco_dR_match_iTrk", ireco) ) == 0 
			      ){
				    
				    //SingleMu
				    for(int i=0;i<nSingleMu;i++){
					    if( I("trk_mode", I("reco_dR_match_iTrk", ireco) ) == SingleMu[i]){
						    if(F("reco_pt", ireco) >= PT_LOW && F("reco_pt", ireco) <= PT_UP){
							    SMRecoPtMatchBX0SingleMu->Fill( F("reco_pt", ireco) ); 
						    }
						    if(log2( F("reco_pt", ireco) ) >= Log2_PT_LOW && log2( F("reco_pt", ireco) ) <= Log2_PT_UP){
							    SMLogRecoPtMatchBX0SingleMu->Fill( log2( F("reco_pt", ireco) ) );
						    }
						    //Plateau
						    if( F("trk_pt", I("reco_dR_match_iTrk", ireco)) > F("reco_pt", ireco) * 7.0/8.0 ||
						        F("trk_pt", I("reco_dR_match_iTrk", ireco)) > 22
						      ){
							    if(F("reco_pt", ireco) >= PT_LOW && F("reco_pt", ireco) <= PT_UP){
								    SMRecoPtMatchBX0SingleMuPlateau->Fill( F("reco_pt", ireco) );
							    }
							    if(log2( F("reco_pt", ireco) ) >= Log2_PT_LOW && log2( F("reco_pt", ireco) ) <= Log2_PT_UP){
								    SMLogRecoPtMatchBX0SingleMuPlateau->Fill( log2( F("reco_pt", ireco) ) );
							    }
						    }
						    //Unique match
						    if( I("reco_dR_match_unique", ireco) == 1 ){
							    if(F("reco_pt", ireco) >= PT_LOW && F("reco_pt", ireco) <= PT_UP){
								    SMRecoPtUniqueMatchBX0SingleMu->Fill( F("reco_pt", ireco) ); 
							    }
							    if(log2( F("reco_pt", ireco) ) >= Log2_PT_LOW && log2( F("reco_pt", ireco) ) <= Log2_PT_UP){
								    SMLogRecoPtUniqueMatchBX0SingleMu->Fill( log2( F("reco_pt", ireco) ) );
							    }
						    }
					    }//end if
				    }//End for
				    
				    //DMu Inc
				    for(int j=0;j<nDoubleMuInc;j++){
					    if( I("trk_mode", I("reco_dR_match_iTrk", ireco) ) == DoubleMuInc[j]){
						    if(F("reco_pt", ireco) >= PT_LOW && F("reco_pt", ireco) <= PT_UP){
							    SMRecoPtMatchBX0DoubleMuInc->Fill( F("reco_pt", ireco) );
						    }
						    if(log2( F("reco_pt", ireco) ) >= Log2_PT_LOW && log2( F("reco_pt", ireco) ) <= Log2_PT_UP){
							    SMLogRecoPtMatchBX0DoubleMuInc->Fill( log2( F("reco_pt", ireco) ) );
						    }
						    //Plateau
						    if( F("trk_pt", I("reco_dR_match_iTrk", ireco)) > F("reco_pt", ireco) * 7.0/8.0 ||
						        F("trk_pt", I("reco_dR_match_iTrk", ireco)) > 22
						      ){
							    if(F("reco_pt", ireco) >= PT_LOW && F("reco_pt", ireco) <= PT_UP){
								    SMRecoPtMatchBX0DoubleMuIncPlateau->Fill( F("reco_pt", ireco) );
							    }
							    if(log2( F("reco_pt", ireco) ) >= Log2_PT_LOW && log2( F("reco_pt", ireco) ) <= Log2_PT_UP){
								    SMLogRecoPtMatchBX0DoubleMuIncPlateau->Fill( log2( F("reco_pt", ireco) ) );
							    }
						    }
						    //Unique match
						    if( I("reco_dR_match_unique", ireco) == 1 ){
							    if(F("reco_pt", ireco) >= PT_LOW && F("reco_pt", ireco) <= PT_UP){
								    SMRecoPtUniqueMatchBX0DoubleMuInc->Fill( F("reco_pt", ireco) ); 
							    }
							    if(log2( F("reco_pt", ireco) ) >= Log2_PT_LOW && log2( F("reco_pt", ireco) ) <= Log2_PT_UP){
								    SMLogRecoPtUniqueMatchBX0DoubleMuInc->Fill( log2( F("reco_pt", ireco) ) );
							    }
						    }
					    }//end if
				    }//End for
				    
				    //MuO Inc
				    for(int k=0;k<nMuOpenInc;k++){
					    if( I("trk_mode", I("reco_dR_match_iTrk", ireco) ) == MuOpenInc[k]){
						    if(F("reco_pt", ireco) >= PT_LOW && F("reco_pt", ireco) <= PT_UP){
							    SMRecoPtMatchBX0MuOpenInc->Fill( F("reco_pt", ireco) ); 
						    }
						    if(log2( F("reco_pt", ireco) ) >= Log2_PT_LOW && log2( F("reco_pt", ireco) ) <= Log2_PT_UP){
							    SMLogRecoPtMatchBX0MuOpenInc->Fill( log2( F("reco_pt", ireco) ) );
						    }
						    //Plateau
						    if( F("trk_pt", I("reco_dR_match_iTrk", ireco)) > F("reco_pt", ireco) * 7.0/8.0 ||
						        F("trk_pt", I("reco_dR_match_iTrk", ireco)) > 22
						      ){
							    if(F("reco_pt", ireco) >= PT_LOW && F("reco_pt", ireco) <= PT_UP){
								    SMRecoPtMatchBX0MuOpenIncPlateau->Fill( F("reco_pt", ireco) );
							    }
							    if(log2( F("reco_pt", ireco) ) >= Log2_PT_LOW && log2( F("reco_pt", ireco) ) <= Log2_PT_UP){
								    SMLogRecoPtMatchBX0MuOpenIncPlateau->Fill( log2( F("reco_pt", ireco) ) );
							    }
						    }
						    //Unique match
						    if( I("reco_dR_match_unique", ireco) == 1 ){
							    if(F("reco_pt", ireco) >= PT_LOW && F("reco_pt", ireco) <= PT_UP){
								    SMRecoPtUniqueMatchBX0MuOpenInc->Fill( F("reco_pt", ireco) );
							    } 
							    if(log2( F("reco_pt", ireco) ) >= Log2_PT_LOW && log2( F("reco_pt", ireco) ) <= Log2_PT_UP){
								    SMLogRecoPtUniqueMatchBX0MuOpenInc->Fill( log2( F("reco_pt", ireco) ) );
							    }
						    }
					    }//end if
				    }//End for MuO
				    
			    }//end IsRECOMatch && BX=0
			    
		    }//end if selection on reco mu
		    
	    }//end if nRecoMuMatchHLT
	    
    }//end loop over SingleMu
    
  } // End loop events
  std::cout << "\n******* Finished looping over the SingleMu events *******" << std::endl;
	
  std::cout << "\n******* About to loop over the Zerobias events *******" << std::endl;
  int nZBEvents = ZB_in_chain->GetEntries();
  for (int iEvt = 0; iEvt < nZBEvents; iEvt++) {
    if (iEvt > MAX_EVT && MAX_EVT !=-1) break;
    if ( (iEvt % PRT_EVT) == 0 ) {
      std::cout << "\n*************************************" << std::endl;
      std::cout << "Looking at event " << iEvt << " out of " << nZBEvents << std::endl;
      std::cout << "*************************************" << std::endl;
    }

    ZB_in_chain->GetEntry(iEvt);
   
    if (verbose) std::cout << "\n" << I("nTracks") << " tracks in the event" << std::endl;
    for (int itrack = 0; itrack < I("nTracks"); itrack++) {
	    if( I("trk_BX", itrack) == 0 && 
	        fabs( F("trk_eta", itrack) ) >= ETA_LOW && 
	        ( I("trk_mode_neighbor", itrack ) != I("trk_mode", itrack ) ) 
	      ){
		    for(int i=0;i<30;i++){
			    
			    if( F("trk_pt", itrack)>= TrigPT[i]){  
				   Count[i]++;   
				   //SingleMu 
				   for(int m=0;m<nSingleMu;m++){
					    if( I("trk_mode", itrack) == SingleMu[m] ){
						    CountSingleMu[i]++; 
					    }//end if
				   }//end single mu
				   //DoubleMu Inclusive
				   for(int j=0;j<nDoubleMuInc;j++){
					    if( I("trk_mode", itrack) == DoubleMuInc[j] ){
						    CountDoubleMuInc[i]++; 
					    }
				   }//end double mu
				   //MuOpen Inclusive
				    for(int k=0;k<nMuOpenInc;k++){
					    if( I("trk_mode", itrack) == MuOpenInc[k] ){
						    CountMuOpenInc[i]++; 
					    }
				    }//end mu open 
			    } //end if      
			    
		    }//end for 
	    }//select over trks
    }//end loop over tracks
    
  } // End loop events
  std::cout << "\n******* Finished looping over the Zerobias events *******" << std::endl;

  delete SM_in_chain;
  delete ZB_in_chain;
  std::cout << "\nDone with macro. Exiting.\n" << std::endl;
	
  //write to output file
  outFile = outFile + "_eta_" + Form("%.2f", ETA_LOW) + "_" + Form("%.2f", ETA_UP) + ".root";
  TFile myPlot(outFile,"RECREATE");
        
  SMRecoPt->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMLogRecoPt->GetXaxis()->SetTitle("log2(RECO pT)");
 
  //SingleMu
  SMRecoPtMatchBX0SingleMu->Divide(SMRecoPt);
  SMRecoPtMatchBX0SingleMu->SetTitle("SingleMu: IsRecoMatch && BX0");
  SMRecoPtMatchBX0SingleMu->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchBX0SingleMu->GetYaxis()->SetTitle("Fraction of RECO muons");
	
  SMRecoPtMatchBX0SingleMuPlateau->Divide(SMRecoPt);
  SMRecoPtMatchBX0SingleMuPlateau->SetTitle("SingleMu: IsRecoMatch && BX0 && Plateau");
  SMRecoPtMatchBX0SingleMuPlateau->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchBX0SingleMuPlateau->GetYaxis()->SetTitle("Fraction of RECO muons");
	
  SMRecoPtUniqueMatchBX0SingleMu->Divide(SMRecoPt);
  SMRecoPtUniqueMatchBX0SingleMu->SetTitle("SingleMu: IsRecoMatch && BX0 && Unique");
  SMRecoPtUniqueMatchBX0SingleMu->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtUniqueMatchBX0SingleMu->GetYaxis()->SetTitle("Fraction of RECO muons");
	
  SMLogRecoPtMatchBX0SingleMu->Divide(SMLogRecoPt);
  SMLogRecoPtMatchBX0SingleMu->SetTitle("SingleMu: IsRecoMatch && BX0");
  SMLogRecoPtMatchBX0SingleMu->GetXaxis()->SetTitle("log2(RECO pT)");
  SMLogRecoPtMatchBX0SingleMu->GetYaxis()->SetTitle("Fraction of RECO muons");

  SMLogRecoPtMatchBX0SingleMuPlateau->Divide(SMLogRecoPt);
  SMLogRecoPtMatchBX0SingleMuPlateau->SetTitle("SingleMu: IsRecoMatch && BX0 && Plateau");
  SMLogRecoPtMatchBX0SingleMuPlateau->GetXaxis()->SetTitle("log2(RECO pT)");
  SMLogRecoPtMatchBX0SingleMuPlateau->GetYaxis()->SetTitle("Fraction of RECO muons");
	
  SMLogRecoPtUniqueMatchBX0SingleMu->Divide(SMLogRecoPt);
  SMLogRecoPtUniqueMatchBX0SingleMu->SetTitle("SingleMu: IsRecoMatch && BX0 && Unique");
  SMLogRecoPtUniqueMatchBX0SingleMu->GetXaxis()->SetTitle("log2(RECO pT)");
  SMLogRecoPtUniqueMatchBX0SingleMu->GetYaxis()->SetTitle("Fraction of RECO muons");
  
  //DoubleMu Inclusive
  SMRecoPtMatchBX0DoubleMuInc->Divide(SMRecoPt);
  SMRecoPtMatchBX0DoubleMuInc->SetTitle("DoubleMu Inclusive: IsRecoMatch && BX0");
  SMRecoPtMatchBX0DoubleMuInc->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchBX0DoubleMuInc->GetYaxis()->SetTitle("Fraction of RECO muons");
	
  SMRecoPtMatchBX0DoubleMuIncPlateau->Divide(SMRecoPt);
  SMRecoPtMatchBX0DoubleMuIncPlateau->SetTitle("DoubleMu Inclusive: IsRecoMatch && BX0 && Plateau");
  SMRecoPtMatchBX0DoubleMuIncPlateau->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchBX0DoubleMuIncPlateau->GetYaxis()->SetTitle("Fraction of RECO muons");
	
  SMRecoPtUniqueMatchBX0DoubleMuInc->Divide(SMRecoPt);
  SMRecoPtUniqueMatchBX0DoubleMuInc->SetTitle("DoubleMu Inclusive: IsRecoMatch && BX0 && Unique");
  SMRecoPtUniqueMatchBX0DoubleMuInc->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtUniqueMatchBX0DoubleMuInc->GetYaxis()->SetTitle("Fraction of RECO muons");

  SMLogRecoPtMatchBX0DoubleMuInc->Divide(SMLogRecoPt);
  SMLogRecoPtMatchBX0DoubleMuInc->SetTitle("DoubleMu Inclusive: IsRecoMatch && BX0");
  SMLogRecoPtMatchBX0DoubleMuInc->GetXaxis()->SetTitle("log2(RECO pT)");
  SMLogRecoPtMatchBX0DoubleMuInc->GetYaxis()->SetTitle("Fraction of RECO muons");
	
  SMLogRecoPtMatchBX0DoubleMuIncPlateau->Divide(SMLogRecoPt);
  SMLogRecoPtMatchBX0DoubleMuIncPlateau->SetTitle("DoubleMu Inclusive: IsRecoMatch && BX0 && Plateau");
  SMLogRecoPtMatchBX0DoubleMuIncPlateau->GetXaxis()->SetTitle("log2(RECO pT)");
  SMLogRecoPtMatchBX0DoubleMuIncPlateau->GetYaxis()->SetTitle("Fraction of RECO muons");
	
  SMLogRecoPtUniqueMatchBX0DoubleMuInc->Divide(SMLogRecoPt);
  SMLogRecoPtUniqueMatchBX0DoubleMuInc->SetTitle("DoubleMu Inclusive: IsRecoMatch && BX0 && Unique");
  SMLogRecoPtUniqueMatchBX0DoubleMuInc->GetXaxis()->SetTitle("log2(RECO pT)");
  SMLogRecoPtUniqueMatchBX0DoubleMuInc->GetYaxis()->SetTitle("Fraction of RECO muons");

  //MuOpen
  SMRecoPtMatchBX0MuOpenInc->Divide(SMRecoPt);
  SMRecoPtMatchBX0MuOpenInc->SetTitle("MuOpen Inclusive: IsRecoMatch && BX0");
  SMRecoPtMatchBX0MuOpenInc->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchBX0MuOpenInc->GetYaxis()->SetTitle("Fraction of RECO muons");
	
  SMRecoPtMatchBX0MuOpenIncPlateau->Divide(SMRecoPt);
  SMRecoPtMatchBX0MuOpenIncPlateau->SetTitle("MuOpen Inclusive: IsRecoMatch && BX0 && Plateau");
  SMRecoPtMatchBX0MuOpenIncPlateau->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchBX0MuOpenIncPlateau->GetYaxis()->SetTitle("Fraction of RECO muons");

  SMRecoPtUniqueMatchBX0MuOpenInc->Divide(SMRecoPt);
  SMRecoPtUniqueMatchBX0MuOpenInc->SetTitle("MuOpen Inclusive: IsRecoMatch && BX0 && Unique");
  SMRecoPtUniqueMatchBX0MuOpenInc->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtUniqueMatchBX0MuOpenInc->GetYaxis()->SetTitle("Fraction of RECO muons");
	
  SMLogRecoPtMatchBX0MuOpenInc->Divide(SMLogRecoPt);
  SMLogRecoPtMatchBX0MuOpenInc->SetTitle("MuOpen Inclusive: IsRecoMatch && BX0");
  SMLogRecoPtMatchBX0MuOpenInc->GetXaxis()->SetTitle("log2(RECO pT)");
  SMLogRecoPtMatchBX0MuOpenInc->GetYaxis()->SetTitle("Fraction of RECO muons");
	
  SMLogRecoPtMatchBX0MuOpenIncPlateau->Divide(SMLogRecoPt);
  SMLogRecoPtMatchBX0MuOpenIncPlateau->SetTitle("MuOpen Inclusive: IsRecoMatch && BX0 && Plateau");
  SMLogRecoPtMatchBX0MuOpenIncPlateau->GetXaxis()->SetTitle("log2(RECO pT)");
  SMLogRecoPtMatchBX0MuOpenIncPlateau->GetYaxis()->SetTitle("Fraction of RECO muons");

  SMLogRecoPtUniqueMatchBX0MuOpenInc->Divide(SMLogRecoPt);
  SMLogRecoPtUniqueMatchBX0MuOpenInc->SetTitle("MuOpen Inclusive: IsRecoMatch && BX0 && Unique");
  SMLogRecoPtUniqueMatchBX0MuOpenInc->GetXaxis()->SetTitle("log2(RECO pT)");
  SMLogRecoPtUniqueMatchBX0MuOpenInc->GetYaxis()->SetTitle("Fraction of RECO muons");

  double Rate[30]={0};//Hz
  double RateSingleMu[30]={0};
  double RateDoubleMuInc[30]={0};
  double RateMuOpenInc[30]={0};

  for(int i=0;i<30;i++){
	  Rate[i] = 1.0*Count[i]*scale;
	  RateSingleMu[i] = 1.0*CountSingleMu[i]*scale;
          RateDoubleMuInc[i] = 1.0*CountDoubleMuInc[i]*scale;
          RateMuOpenInc[i] = 1.0*CountMuOpenInc[i]*scale;
  }

  //Rate plots
  TH1F *HRate = new TH1F("HRate", "Rate", PT_UP-PT_LOW, PT_LOW, PT_UP); HRate->GetXaxis()->SetTitle("Trig pT[GeV]"); HRate->GetYaxis()->SetTitle("Rate[Hz]");
  TH1F *HRateSingleMu = new TH1F("HRateSingleMu", "SingleMu", PT_UP-PT_LOW, PT_LOW, PT_UP); HRateSingleMu->GetXaxis()->SetTitle("Trig pT[GeV]"); HRateSingleMu->GetYaxis()->SetTitle("Rate[Hz]");//SingleMu only
  TH1F *HRateDoubleMuInc = new TH1F("HRateDoubleMuInc", "DoubleMu Inclusive", PT_UP-PT_LOW, PT_LOW, PT_UP); HRateDoubleMuInc->GetXaxis()->SetTitle("Trig pT[GeV]"); HRateDoubleMuInc->GetYaxis()->SetTitle("Rate[Hz]");//DoubleMu Inc
  TH1F *HRateMuOpenInc = new TH1F("HRateMuOpenInc", "MuOpen Inclusive", PT_UP-PT_LOW, PT_LOW, PT_UP); HRateMuOpenInc->GetXaxis()->SetTitle("Trig pT[GeV]"); HRateMuOpenInc->GetYaxis()->SetTitle("Rate[Hz]");//MuOpen Inc
	
  for(int i=0;i<30;i++){
	  HRate->Fill(TrigPT[i],Rate[i]);
	  HRateSingleMu->Fill(TrigPT[i],RateSingleMu[i]);
	  HRateDoubleMuInc->Fill(TrigPT[i],RateDoubleMuInc[i]);
	  HRateMuOpenInc->Fill(TrigPT[i],RateMuOpenInc[i]);
  }
	
  HRate->Write();
  HRateSingleMu->Write();
  HRateDoubleMuInc->Write();	  
  HRateMuOpenInc->Write();	 
	 
  //plots
  SMRecoPt->Write();
  SMLogRecoPt->Write();
	
  SMRecoPtMatchBX0SingleMu->Write();
  SMRecoPtMatchBX0SingleMuPlateau->Write();
  SMRecoPtUniqueMatchBX0SingleMu->Write();
	
  SMLogRecoPtMatchBX0SingleMu->Write();
  SMLogRecoPtMatchBX0SingleMuPlateau->Write();
  SMLogRecoPtUniqueMatchBX0SingleMu->Write();

  SMRecoPtMatchBX0DoubleMuInc->Write();
  SMRecoPtMatchBX0DoubleMuIncPlateau->Write();
  SMRecoPtUniqueMatchBX0DoubleMuInc->Write();

  SMLogRecoPtMatchBX0DoubleMuInc->Write();
  SMLogRecoPtMatchBX0DoubleMuIncPlateau->Write();
  SMLogRecoPtUniqueMatchBX0DoubleMuInc->Write();
	
  SMRecoPtMatchBX0MuOpenInc->Write();
  SMRecoPtMatchBX0MuOpenIncPlateau->Write();
  SMRecoPtUniqueMatchBX0MuOpenInc->Write();

  SMLogRecoPtMatchBX0MuOpenInc->Write();
  SMLogRecoPtMatchBX0MuOpenIncPlateau->Write();
  SMLogRecoPtUniqueMatchBX0MuOpenInc->Write();
  
  myPlot.Close();
  
} // End function: void 
