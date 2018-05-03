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
const int Log2_PT_UP = 9;
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
	  		    "NTuple_SingleMuon_FlatNtuple_Run_306154_2018_05_01_SingleMu_2017_emul.root"//2017 emulator
			    //"NTuple_SingleMuon_FlatNtuple_Run_306154_2018_05_01_SingleMu_2018_emul_dTh4.root"//2018 emulator changes
	                    //"NTuple_SingleMuon_FlatNtuple_Run_306154_2018_05_01_SingleMu_2018_emul_dTh6.root"  
                            //"NTuple_SingleMuon_FlatNtuple_Run_306154_2018_05_01_SingleMu_2018_emul_dTh8.root"
};   
TString ZeroBias_files[4] = { 
	  		    //"NTuple_ZeroBias1_FlatNtuple_Run_306091_2018_03_02_ZB1.root",//Obselete
		            //"NTuple_ZeroBias2_FlatNtuple_Run_306091_2018_03_02_ZB2.root",
		            //"NTuple_ZeroBias3_FlatNtuple_Run_306091_2018_03_02_ZB3.root", 
		 	    //"NTuple_ZeroBias4_FlatNtuple_Run_306091_2018_03_02_ZB4.root"
};
//****************************
//* USER modify above ONLY   *
//****************************
const int nSingleMu = sizeof(SingleMu)/sizeof(int);//# of modes
const int nDoubleMuInc = sizeof(DoubleMuInc)/sizeof(int);
const int nMuOpenInc = sizeof(MuOpenInc)/sizeof(int);
const int USESingleMu = sizeof(SingleMu_files)/sizeof(string);//# of SM files to use
const int USEZerobias = sizeof(ZeroBias_files)/sizeof(string);//# of ZB files to use

void ModesRateEffV1() {
   
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
  TH1F *SMRecoPt = new TH1F("SMRecoPt", SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);
	
  TH1F *SMRecoPtNoMatch = new TH1F("SMRecoPtNoMatch", "No Match "+ SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);
  TH1F *SMRecoPtNoUniqueMatch = new TH1F("SMRecoPtNoUniqueMatch", "No Unique Match "+ SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);
  TH1F *SMRecoPtUniqueMatchBX0 = new TH1F("SMRecoPtUniqueMatchBX0", "Unique Match BX0 "+ SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);
  TH1F *SMRecoPtMatch = new TH1F("SMRecoPtMatch", "Match "+ SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);	

  //Has match
  TH1F *SMRecoPtMatchSingleMuModes = new TH1F("SMRecoPtMatchSingleMuModes", "Match SingleMu Modes" + SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);
  TH1F *SMRecoPtMatchDoubleMuModes = new TH1F("SMRecoPtMatchDoubleMuModes", "Match DoubleMu Modes" + SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);
  TH1F *SMRecoPtMatchMuOpenModes = new TH1F("SMRecoPtMatchMuOpenModes", "Match MuOpen Modes" + SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);
	
  TH1F *SMRecoPtMatchMode15 = new TH1F("SMRecoPtMatchMode15", "Match Mode15" + SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);
  TH1F *SMRecoPtMatchMode14 = new TH1F("SMRecoPtMatchMode14", "Match Mode14" + SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);
  TH1F *SMRecoPtMatchMode13 = new TH1F("SMRecoPtMatchMode13", "Match Mode13" + SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);
  TH1F *SMRecoPtMatchMode11 = new TH1F("SMRecoPtMatchMode11", "Match Mode11" + SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);
  
  TH1F *SMRecoPtMatchMode12 = new TH1F("SMRecoPtMatchMode12", "Match Mode12" + SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);
  TH1F *SMRecoPtMatchMode10 = new TH1F("SMRecoPtMatchMode10", "Match Mode10" + SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);
  TH1F *SMRecoPtMatchMode7 = new TH1F("SMRecoPtMatchMode7", "Match Mode7" + SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);

  TH1F *SMRecoPtMatchMode9 = new TH1F("SMRecoPtMatchMode9", "Match Mode9" + SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);
  TH1F *SMRecoPtMatchMode6 = new TH1F("SMRecoPtMatchMode6", "Match Mode6" + SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);
  TH1F *SMRecoPtMatchMode5 = new TH1F("SMRecoPtMatchMode5", "Match Mode5" + SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);
  TH1F *SMRecoPtMatchMode3 = new TH1F("SMRecoPtMatchMode3", "Match Mode3" + SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);
  
  //Unique match BX0
  TH1F *SMRecoPtMatchSingleMuModesBX0 = new TH1F("SMRecoPtMatchSingleMuModesBX0", "Unique Match SingleMu ModesBX0" + SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);
  TH1F *SMRecoPtMatchDoubleMuModesBX0 = new TH1F("SMRecoPtMatchDoubleMuModesBX0", "Unique Match DoubleMu ModesBX0" + SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);
  TH1F *SMRecoPtMatchMuOpenModesBX0 = new TH1F("SMRecoPtMatchMuOpenModesBX0", "Unique Match MuOpen ModesBX0" + SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);
	
  TH1F *SMRecoPtMatchMode15BX0 = new TH1F("SMRecoPtMatchMode15BX0", "Unique Match Mode15BX0" + SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);
  TH1F *SMRecoPtMatchMode14BX0 = new TH1F("SMRecoPtMatchMode14BX0", "Unique Match Mode14BX0" + SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);
  TH1F *SMRecoPtMatchMode13BX0 = new TH1F("SMRecoPtMatchMode13BX0", "Unique Match Mode13BX0" + SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);
  TH1F *SMRecoPtMatchMode11BX0 = new TH1F("SMRecoPtMatchMode11BX0", "Unique Match Mode11BX0" + SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);
	
  TH1F *SMRecoPtMatchMode12BX0 = new TH1F("SMRecoPtMatchMode12BX0", "Unique Match Mode12BX0" + SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);
  TH1F *SMRecoPtMatchMode10BX0 = new TH1F("SMRecoPtMatchMode10BX0", "Unique Match Mode10BX0" + SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);
  TH1F *SMRecoPtMatchMode7BX0 = new TH1F("SMRecoPtMatchMode7BX0", "Unique Match Mode7BX0" + SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);

  TH1F *SMRecoPtMatchMode9BX0 = new TH1F("SMRecoPtMatchMode9BX0", "Unique Match Mode9BX0" + SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);
  TH1F *SMRecoPtMatchMode6BX0 = new TH1F("SMRecoPtMatchMode6BX0", "Unique Match Mode6BX0" + SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);
  TH1F *SMRecoPtMatchMode5BX0 = new TH1F("SMRecoPtMatchMode5BX0", "Unique Match Mode5BX0" + SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);
  TH1F *SMRecoPtMatchMode3BX0 = new TH1F("SMRecoPtMatchMode3BX0", "Unique Match Mode3BX0" + SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);
  
  //Unique match BX0 Plateau
  TH1F *SMRecoPtMatchSingleMuModesBX0Plateau = new TH1F("SMRecoPtMatchSingleMuModesBX0Plateau", "Unique Match SingleMu ModesBX0Plateau" + SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);
  TH1F *SMRecoPtMatchDoubleMuModesBX0Plateau = new TH1F("SMRecoPtMatchDoubleMuModesBX0Plateau", "Unique Match DoubleMu ModesBX0Plateau" + SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);
  TH1F *SMRecoPtMatchMuOpenModesBX0Plateau = new TH1F("SMRecoPtMatchMuOpenModesBX0Plateau", "Unique Match MuOpen ModesBX0Plateau" + SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);
	
  TH1F *SMRecoPtMatchMode15BX0Plateau = new TH1F("SMRecoPtMatchMode15BX0Plateau", "Unique Match Mode15BX0Plateau" + SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);
  TH1F *SMRecoPtMatchMode14BX0Plateau = new TH1F("SMRecoPtMatchMode14BX0Plateau", "Unique Match Mode14BX0Plateau" + SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);
  TH1F *SMRecoPtMatchMode13BX0Plateau = new TH1F("SMRecoPtMatchMode13BX0Plateau", "Unique Match Mode13BX0Plateau" + SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);
  TH1F *SMRecoPtMatchMode11BX0Plateau = new TH1F("SMRecoPtMatchMode11BX0Plateau", "Unique Match Mode11BX0Plateau" + SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);

  TH1F *SMRecoPtMatchMode12BX0Plateau = new TH1F("SMRecoPtMatchMode12BX0Plateau", "Unique Match Mode12BX0Plateau" + SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);
  TH1F *SMRecoPtMatchMode10BX0Plateau = new TH1F("SMRecoPtMatchMode10BX0Plateau", "Unique Match Mode10BX0Plateau" + SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);
  TH1F *SMRecoPtMatchMode7BX0Plateau = new TH1F("SMRecoPtMatchMode7BX0Plateau", "Unique Match Mode7BX0Plateau" + SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);

  TH1F *SMRecoPtMatchMode9BX0Plateau = new TH1F("SMRecoPtMatchMode9BX0Plateau", "Unique Match Mode9BX0Plateau" + SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);
  TH1F *SMRecoPtMatchMode6BX0Plateau = new TH1F("SMRecoPtMatchMode6BX0Plateau", "Unique Match Mode6BX0Plateau" + SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);
  TH1F *SMRecoPtMatchMode5BX0Plateau = new TH1F("SMRecoPtMatchMode5BX0Plateau", "Unique Match Mode5BX0Plateau" + SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);
  TH1F *SMRecoPtMatchMode3BX0Plateau = new TH1F("SMRecoPtMatchMode3BX0Plateau", "Unique Match Mode3BX0Plateau" + SMRecoPtTitle, PT_UP-PT_LOW, PT_LOW, PT_UP);

  //Initialize variables for rate
  double TrigPT[30]={0};
  for(int i=0;i<30;i++){
	  TrigPT[i]=i*1.0;
  }
  double Count[30]={0};
  double CountdBX0[30]={0};
  double CountRecoSoft[30]={0};
  double CountRecoOnly[30]={0};
	
  double CountSingleMuModes[30]={0};
  double CountSingleMuModesdBX0[30]={0};
  double CountSingleMuModesRecoSoft[30]={0};
  double CountSingleMuModesRecoOnly[30]={0};
 
  double CountDoubleMuModes[30]={0};
  double CountDoubleMuModesdBX0[30]={0};
  double CountDoubleMuModesRecoSoft[30]={0};
  double CountDoubleMuModesRecoOnly[30]={0};
	
  double CountMuOpenModes[30]={0};
  double CountMuOpenModesdBX0[30]={0};
  double CountMuOpenModesRecoSoft[30]={0};
  double CountMuOpenModesRecoOnly[30]={0};

  double CountMode15[30]={0};
  double CountMode15dBX0[30]={0};
  double CountMode15RecoSoft[30]={0};
  double CountMode15RecoOnly[30]={0};
  double CountMode14[30]={0};
  double CountMode14dBX0[30]={0};
  double CountMode14RecoSoft[30]={0};
  double CountMode14RecoOnly[30]={0};
  double CountMode13[30]={0};
  double CountMode13dBX0[30]={0};
  double CountMode13RecoSoft[30]={0};
  double CountMode13RecoOnly[30]={0};
  double CountMode11[30]={0};
  double CountMode11dBX0[30]={0};
  double CountMode11RecoSoft[30]={0};
  double CountMode11RecoOnly[30]={0};
  double CountMode12[30]={0};
  double CountMode12dBX0[30]={0};
  double CountMode12RecoSoft[30]={0};
  double CountMode12RecoOnly[30]={0};
  double CountMode10[30]={0};
  double CountMode10dBX0[30]={0};
  double CountMode10RecoSoft[30]={0};
  double CountMode10RecoOnly[30]={0};
  double CountMode7[30]={0};
  double CountMode7dBX0[30]={0};
  double CountMode7RecoSoft[30]={0};
  double CountMode7RecoOnly[30]={0};
  double CountMode9[30]={0};
  double CountMode9dBX0[30]={0};
  double CountMode9RecoSoft[30]={0};
  double CountMode9RecoOnly[30]={0};
  double CountMode6[30]={0};
  double CountMode6dBX0[30]={0};
  double CountMode6RecoSoft[30]={0};
  double CountMode6RecoOnly[30]={0};
  double CountMode5[30]={0};
  double CountMode5dBX0[30]={0};
  double CountMode5RecoSoft[30]={0};
  double CountMode5RecoOnly[30]={0};
  double CountMode3[30]={0};
  double CountMode3dBX0[30]={0};
  double CountMode3RecoSoft[30]={0};
  double CountMode3RecoOnly[30]={0};
	
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
		    if( F("reco_pt", ireco) >= PT_LOW && F("reco_pt", ireco) <= PT_UP && 
		        I("reco_ID_loose", ireco) == 1 && I("reco_ID_station", ireco) == 1 &&
		        fabs( F("reco_eta_St2",ireco) ) >= ETA_LOW && fabs( F("reco_eta_St2", ireco) ) <= ETA_UP
		      ){
			    
			    SMRecoPt->Fill( F("reco_pt", ireco) );
			    
			    //***********************
		            //* IsRECOMatch && BX=0 *
		            //***********************
			    if( I("reco_dR_match_nTrk", ireco) > 0 &&
			        I("trk_BX", I("reco_dR_match_iTrk", ireco) ) == 0 
			      ){
				    
				    //SingleMu
				    for(int i=0;i<nSingleMu;i++){
					    if( I("trk_mode", I("reco_dR_match_iTrk", ireco) ) == SingleMu[i]){
						    SMRecoPtMatchBX0SingleMu->Fill( F("reco_pt", ireco) ); 
						    //Plateau
						    if( F("trk_pt", I("reco_dR_match_iTrk", ireco)) > F("reco_pt", ireco) * 7.0/8.0 ||
						        F("trk_pt", I("reco_dR_match_iTrk", ireco)) > 22
						      ){
							    SMRecoPtMatchBX0SingleMuPlateau->Fill( F("reco_pt", ireco) );
						    }
						    //Unique match
						    if( I("reco_dR_match_unique", ireco) == 1 ){
							    SMRecoPtUniqueMatchBX0SingleMu->Fill( F("reco_pt", ireco) ); 
						    }
					    }//end if
				    }//End for
				    
				    //DMu Inc
				    for(int j=0;j<nDoubleMuInc;j++){
					    if( I("trk_mode", I("reco_dR_match_iTrk", ireco) ) == DoubleMuInc[j]){
						    SMRecoPtMatchBX0DoubleMuInc->Fill( F("reco_pt", ireco) ); 
						    //Plateau
						    if( F("trk_pt", I("reco_dR_match_iTrk", ireco)) > F("reco_pt", ireco) * 7.0/8.0 ||
						        F("trk_pt", I("reco_dR_match_iTrk", ireco)) > 22
						      ){
							    SMRecoPtMatchBX0DoubleMuIncPlateau->Fill( F("reco_pt", ireco) );
						    }
						    //Unique match
						    if( I("reco_dR_match_unique", ireco) == 1 ){
							    SMRecoPtUniqueMatchBX0DoubleMuInc->Fill( F("reco_pt", ireco) ); 
						    }
					    }//end if
				    }//End for
				    
				    //MuO Inc
				    for(int k=0;k<nMuOpenInc;k++){
					    if( I("trk_mode", I("reco_dR_match_iTrk", ireco) ) == MuOpenInc[k]){
						    SMRecoPtMatchBX0MuOpenInc->Fill( F("reco_pt", ireco) ); 
						    //Plateau
						    if( F("trk_pt", I("reco_dR_match_iTrk", ireco)) > F("reco_pt", ireco) * 7.0/8.0 ||
						        F("trk_pt", I("reco_dR_match_iTrk", ireco)) > 22
						      ){
							    SMRecoPtMatchBX0MuOpenIncPlateau->Fill( F("reco_pt", ireco) );
						    }
						    //Unique match
						    if( I("reco_dR_match_unique", ireco) == 1 ){
							    SMRecoPtUniqueMatchBX0MuOpenInc->Fill( F("reco_pt", ireco) ); 
						    }
					    }//end if
				    }//End for
			    }//end IsRECOMatch && BX=0
			    
		    }//end if selection on reco mu
		    
	    }//end if nRecoMuMatchHLT
	    
    }//end loop over SingleMu
    
  } // End loop events
  std::cout << "\n******* Finished looping over the SingleMu events *******" << std::endl;

  delete SM_in_chain;
  delete ZB_in_chain;
  std::cout << "\nDone with macro. Exiting.\n" << std::endl;
	
  //write to output file
  outFile = outFile + "_eta_" + Form("%.2f", ETA_LOW) + "_" + Form("%.2f", ETA_UP) + ".root";
  TFile myPlot(outFile,"RECREATE");
        
  SMRecoPt->GetXaxis()->SetTitle("RECO pT[GeV]");
	
  SMRecoPtMatchBX0SingleMu->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchBX0SingleMuPlateau->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtUniqueMatchBX0SingleMu->GetXaxis()->SetTitle("RECO pT[GeV]");

  SMRecoPtMatchBX0DoubleMuInc->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchBX0DoubleMuIncPlateau->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtUniqueMatchBX0DoubleMuInc->GetXaxis()->SetTitle("RECO pT[GeV]");
	
  SMRecoPtMatchBX0MuOpenInc->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchBX0MuOpenIncPlateau->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtUniqueMatchBX0MuOpenInc->GetXaxis()->SetTitle("RECO pT[GeV]");
 
  //SingleMu
  TCanvas *CMatchBX0SingleMu = new TCanvas("CMatchBX0SingleMu","SingleMu: IsRecoMatch && BX0",700,500);
  CMatchBX0SingleMu->cd();
  TH1F *SMRecoPtMatchBX0SingleMuClone = (TH1F*)SMRecoPtMatchBX0SingleMu->Clone("SMRecoPtMatchBX0SingleMuClone");
  SMRecoPtMatchBX0SingleMuClone->SetLineColor(1);//blk
  gStyle->SetOptStat(0);
  SMRecoPtMatchBX0SingleMuClone->Divide(SMRecoPt);
  SMRecoPtMatchBX0SingleMuClone->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchBX0SingleMuClone->GetYaxis()->SetTitle("Fraction of RECO muons");
	
  TCanvas *CMatchBX0SingleMuPlateau = new TCanvas("CMatchBX0SingleMuPlateau","SingleMu: IsRecoMatch && BX0 && Plateau",700,500);
  CMatchBX0SingleMuPlateau->cd();
  TH1F *SMRecoPtMatchBX0SingleMuPlateauClone = (TH1F*)SMRecoPtMatchBX0SingleMuPlateau->Clone("SMRecoPtMatchBX0SingleMuPlateauClone");
  SMRecoPtMatchBX0SingleMuPlateauClone->SetLineColor(1);//blk
  gStyle->SetOptStat(0);
  SMRecoPtMatchBX0SingleMuPlateauClone->Divide(SMRecoPt);
  SMRecoPtMatchBX0SingleMuPlateauClone->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchBX0SingleMuPlateauClone->GetYaxis()->SetTitle("Fraction of RECO muons");
	
  TCanvas *CUniqueMatchBX0SingleMu = new TCanvas("UniqueMatchBX0SingleMu","SingleMu: IsRecoMatch && BX0 && Unique",700,500);
  CUniqueMatchBX0SingleMu->cd();
  TH1F *SMRecoPtUniqueMatchBX0SingleMuClone = (TH1F*)SMRecoPtUniqueMatchBX0SingleMu->Clone("SMRecoPtUniqueMatchBX0SingleMuClone");
  SMRecoPtUniqueMatchBX0SingleMuClone->SetLineColor(1);//blk
  gStyle->SetOptStat(0);
  SMRecoPtUniqueMatchBX0SingleMuClone->Divide(SMRecoPt);
  SMRecoPtUniqueMatchBX0SingleMuClone->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtUniqueMatchBX0SingleMuClone->GetYaxis()->SetTitle("Fraction of RECO muons");
  
  //DoubleMu Inclusive
  TCanvas *CMatchBX0DoubleMuInc = new TCanvas("CMatchBX0DoubleMuInc","DoubleMu Inclusive: IsRecoMatch && BX0",700,500);
  CMatchBX0DoubleMuInc->cd();
  TH1F *SMRecoPtMatchBX0DoubleMuIncClone = (TH1F*)SMRecoPtMatchBX0DoubleMuInc->Clone("SMRecoPtMatchBX0DoubleMuIncClone");
  SMRecoPtMatchBX0DoubleMuIncClone->SetLineColor(1);//blk
  gStyle->SetOptStat(0);
  SMRecoPtMatchBX0DoubleMuIncClone->Divide(SMRecoPt);
  SMRecoPtMatchBX0DoubleMuIncClone->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchBX0DoubleMuIncClone->GetYaxis()->SetTitle("Fraction of RECO muons");
	
  TCanvas *CMatchBX0DoubleMuIncPlateau = new TCanvas("CMatchBX0DoubleMuIncPlateau","DoubleMu Inclusive: IsRecoMatch && BX0 && Plateau",700,500);
  CMatchBX0DoubleMuIncPlateau->cd();
  TH1F *SMRecoPtMatchBX0DoubleMuIncPlateauClone = (TH1F*)SMRecoPtMatchBX0DoubleMuIncPlateau->Clone("SMRecoPtMatchBX0DoubleMuIncPlateauClone");
  SMRecoPtMatchBX0DoubleMuIncPlateauClone->SetLineColor(1);//blk
  gStyle->SetOptStat(0);
  SMRecoPtMatchBX0DoubleMuIncPlateauClone->Divide(SMRecoPt);
  SMRecoPtMatchBX0DoubleMuIncPlateauClone->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchBX0DoubleMuIncPlateauClone->GetYaxis()->SetTitle("Fraction of RECO muons");
	
  TCanvas *CUniqueMatchBX0DoubleMuInc = new TCanvas("UniqueMatchBX0DoubleMuInc","DoubleMu Inclusive: IsRecoMatch && BX0 && Unique",700,500);
  CUniqueMatchBX0DoubleMuInc->cd();
  TH1F *SMRecoPtUniqueMatchBX0DoubleMuIncClone = (TH1F*)SMRecoPtUniqueMatchBX0DoubleMuInc->Clone("SMRecoPtUniqueMatchBX0DoubleMuIncClone");
  SMRecoPtUniqueMatchBX0DoubleMuIncClone->SetLineColor(1);//blk
  gStyle->SetOptStat(0);
  SMRecoPtUniqueMatchBX0DoubleMuIncClone->Divide(SMRecoPt);
  SMRecoPtUniqueMatchBX0DoubleMuIncClone->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtUniqueMatchBX0DoubleMuIncClone->GetYaxis()->SetTitle("Fraction of RECO muons");

  //MuOpen
  TCanvas *CMatchBX0MuOpenInc = new TCanvas("CMatchBX0MuOpenInc","MuOpen Inclusive: IsRecoMatch && BX0",700,500);
  CMatchBX0MuOpenInc->cd();
  TH1F *SMRecoPtMatchBX0MuOpenIncClone = (TH1F*)SMRecoPtMatchBX0MuOpenInc->Clone("SMRecoPtMatchBX0MuOpenIncClone");
  SMRecoPtMatchBX0MuOpenIncClone->SetLineColor(1);//blk
  gStyle->SetOptStat(0);
  SMRecoPtMatchBX0MuOpenIncClone->Divide(SMRecoPt);
  SMRecoPtMatchBX0MuOpenIncClone->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchBX0MuOpenIncClone->GetYaxis()->SetTitle("Fraction of RECO muons");
	
  TCanvas *CMatchBX0MuOpenIncPlateau = new TCanvas("CMatchBX0MuOpenIncPlateau","MuOpen Inclusive: IsRecoMatch && BX0 && Plateau",700,500);
  CMatchBX0MuOpenIncPlateau->cd();
  TH1F *SMRecoPtMatchBX0MuOpenIncPlateauClone = (TH1F*)SMRecoPtMatchBX0MuOpenIncPlateau->Clone("SMRecoPtMatchBX0MuOpenIncPlateauClone");
  SMRecoPtMatchBX0MuOpenIncPlateauClone->SetLineColor(1);//blk
  gStyle->SetOptStat(0);
  SMRecoPtMatchBX0MuOpenIncPlateauClone->Divide(SMRecoPt);
  SMRecoPtMatchBX0MuOpenIncPlateauClone->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchBX0MuOpenIncPlateauClone->GetYaxis()->SetTitle("Fraction of RECO muons");
	
  TCanvas *CUniqueMatchBX0MuOpenInc = new TCanvas("UniqueMatchBX0MuOpenInc","MuOpen Inclusive: IsRecoMatch && BX0 && Unique",700,500);
  CUniqueMatchBX0MuOpenInc->cd();
  TH1F *SMRecoPtUniqueMatchBX0MuOpenIncClone = (TH1F*)SMRecoPtUniqueMatchBX0MuOpenInc->Clone("SMRecoPtUniqueMatchBX0MuOpenIncClone");
  SMRecoPtUniqueMatchBX0MuOpenIncClone->SetLineColor(1);//blk
  gStyle->SetOptStat(0);
  SMRecoPtUniqueMatchBX0MuOpenIncClone->Divide(SMRecoPt);
  SMRecoPtUniqueMatchBX0MuOpenIncClone->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtUniqueMatchBX0MuOpenIncClone->GetYaxis()->SetTitle("Fraction of RECO muons");
 
  //intermidiate plots
  SMRecoPt->Write();
	
  SMRecoPtMatchBX0SingleMuClone->Write();
  SMRecoPtMatchBX0SingleMuPlateauClone->Write();
  SMRecoPtUniqueMatchBX0SingleMuClone->Write();

  SMRecoPtMatchBX0DoubleMuIncClone->Write();
  SMRecoPtMatchBX0DoubleMuIncPlateauClone->Write();
  SMRecoPtUniqueMatchBX0DoubleMuIncClone->Write();
	
  SMRecoPtMatchBX0MuOpenIncClone->Write();
  SMRecoPtMatchBX0MuOpenIncPlateauClone->Write();
  SMRecoPtUniqueMatchBX0MuOpenIncClone->Write();
  
  myPlot.Close();
  
} // End function: void 
