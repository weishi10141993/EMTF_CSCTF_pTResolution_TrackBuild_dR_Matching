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
						    if( F("trk_pt", I("reco_dR_match_iTrk", ireco) )  >  F("reco_pt", ireco) * 7.0/8.0){
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
						    if( F("trk_pt", I("reco_dR_match_iTrk", ireco) )  >  F("reco_pt", ireco) * 7.0/8.0){
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
						    SMRecoPtMatchBX0DoubleMuInc->Fill( F("reco_pt", ireco) ); 
						    //Plateau
						    if( F("trk_pt", I("reco_dR_match_iTrk", ireco) )  >  F("reco_pt", ireco) * 7.0/8.0){
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
  
  SMRecoPtNoMatch->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtNoUniqueMatch->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatch->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtUniqueMatchBX0->GetXaxis()->SetTitle("RECO pT[GeV]");

  SMRecoPtMatchSingleMuModes->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchSingleMuModesBX0->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchSingleMuModesBX0Plateau->GetXaxis()->SetTitle("RECO pT[GeV]");
	
  SMRecoPtMatchDoubleMuModes->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchDoubleMuModesBX0->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchDoubleMuModesBX0Plateau->GetXaxis()->SetTitle("RECO pT[GeV]");
	
  SMRecoPtMatchMuOpenModes->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchMuOpenModesBX0->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchMuOpenModesBX0Plateau->GetXaxis()->SetTitle("RECO pT[GeV]");

  SMRecoPt->Write();
  //divide histograms for all modes
  //SingleMu
  TCanvas *CSingleMuModes = new TCanvas("CSingleMuModes","SingleMuModes",700,500);
  THStack *SSingleMuModes = new THStack("SSingleMuModes","SingleMuModes");
  CSingleMuModes->cd();
  TH1F *SMRecoPtMatchSingleMuModesClone = (TH1F*)SMRecoPtMatchSingleMuModes->Clone("SMRecoPtMatchSingleMuModesClone");
  TH1F *SMRecoPtMatchSingleMuModesBX0Clone = (TH1F*)SMRecoPtMatchSingleMuModesBX0->Clone("SMRecoPtMatchSingleMuModesBX0Clone");
  TH1F *SMRecoPtMatchSingleMuModesBX0PlateauClone = (TH1F*)SMRecoPtMatchSingleMuModesBX0Plateau->Clone("SMRecoPtMatchSingleMuModesBX0PlateauClone");
  SMRecoPtMatchSingleMuModesClone->SetLineColor(1);//blk
  SMRecoPtMatchSingleMuModesBX0Clone->SetLineColor(2);//red
  SMRecoPtMatchSingleMuModesBX0PlateauClone->SetLineColor(4);//blue
  gStyle->SetOptStat(0);
  SMRecoPtMatchSingleMuModesClone->Divide(SMRecoPt);
  SMRecoPtMatchSingleMuModesBX0Clone->Divide(SMRecoPt);
  SMRecoPtMatchSingleMuModesBX0PlateauClone->Divide(SMRecoPt);
  SSingleMuModes->Add(SMRecoPtMatchSingleMuModesClone);
  SSingleMuModes->Add(SMRecoPtMatchSingleMuModesBX0Clone);
  SSingleMuModes->Add(SMRecoPtMatchSingleMuModesBX0PlateauClone);
  SSingleMuModes->Draw("nostack");
  SSingleMuModes->GetXaxis()->SetTitle("RECO pT[GeV]");
  SSingleMuModes->GetYaxis()->SetTitle("Fraction of RECO muons");
  SSingleMuModes->Modified();
        
  TLegend* LSingleMuModes = new TLegend(0.6,0.6,0.9,0.9);
  TString LMatchSingleMuModes = "";
  LMatchSingleMuModes = LMatchSingleMuModes + "RecoMu Match";
  TString LMatchSingleMuModesBX0 = "";
  LMatchSingleMuModesBX0 = LMatchSingleMuModesBX0 + "RecoMu Unique Match && TrkBX=0";
  TString LMatchSingleMuModesBX0Plateau = "";
  LMatchSingleMuModesBX0Plateau = LMatchSingleMuModesBX0Plateau + "RecoMu Unique Match && TrkBX=0 && Plateau";
  LSingleMuModes->AddEntry(SMRecoPtMatchSingleMuModesClone, LMatchSingleMuModes);
  LSingleMuModes->AddEntry(SMRecoPtMatchSingleMuModesBX0Clone, LMatchSingleMuModesBX0);
  LSingleMuModes->AddEntry(SMRecoPtMatchSingleMuModesBX0PlateauClone, LMatchSingleMuModesBX0Plateau);
  LSingleMuModes->SetFillStyle(0);
  LSingleMuModes->SetBorderSize(0);
  LSingleMuModes->Draw(); 
  CSingleMuModes->Write();
  
  //DoubleMu
  TCanvas *CDoubleMuModes = new TCanvas("CDoubleMuModes","DoubleMuModes",700,500);
  THStack *SDoubleMuModes = new THStack("SDoubleMuModes","DoubleMuModes");
  CDoubleMuModes->cd();
  TH1F *SMRecoPtMatchDoubleMuModesClone = (TH1F*)SMRecoPtMatchDoubleMuModes->Clone("SMRecoPtMatchDoubleMuModesClone");
  TH1F *SMRecoPtMatchDoubleMuModesBX0Clone = (TH1F*)SMRecoPtMatchDoubleMuModesBX0->Clone("SMRecoPtMatchDoubleMuModesBX0Clone");
  TH1F *SMRecoPtMatchDoubleMuModesBX0PlateauClone = (TH1F*)SMRecoPtMatchDoubleMuModesBX0Plateau->Clone("SMRecoPtMatchDoubleMuModesBX0PlateauClone");
  SMRecoPtMatchDoubleMuModesClone->SetLineColor(1);//blk
  SMRecoPtMatchDoubleMuModesBX0Clone->SetLineColor(2);//red
  SMRecoPtMatchDoubleMuModesBX0PlateauClone->SetLineColor(4);//blue
  gStyle->SetOptStat(0);
  SMRecoPtMatchDoubleMuModesClone->Divide(SMRecoPt);
  SMRecoPtMatchDoubleMuModesBX0Clone->Divide(SMRecoPt);
  SMRecoPtMatchDoubleMuModesBX0PlateauClone->Divide(SMRecoPt);
  SDoubleMuModes->Add(SMRecoPtMatchDoubleMuModesClone);
  SDoubleMuModes->Add(SMRecoPtMatchDoubleMuModesBX0Clone);
  SDoubleMuModes->Add(SMRecoPtMatchDoubleMuModesBX0PlateauClone);
  SDoubleMuModes->Draw("nostack");
  SDoubleMuModes->GetXaxis()->SetTitle("RECO pT[GeV]");
  SDoubleMuModes->GetYaxis()->SetTitle("Fraction of RECO muons");
  SDoubleMuModes->Modified();
        
  TLegend* LDoubleMuModes = new TLegend(0.6,0.6,0.9,0.9);
  TString LMatchDoubleMuModes = "";
  LMatchDoubleMuModes = LMatchDoubleMuModes + "RecoMu Match";
  TString LMatchDoubleMuModesBX0 = "";
  LMatchDoubleMuModesBX0 = LMatchDoubleMuModesBX0 + "RecoMu Unique Match && TrkBX=0";
  TString LMatchDoubleMuModesBX0Plateau = "";
  LMatchDoubleMuModesBX0Plateau = LMatchDoubleMuModesBX0Plateau + "RecoMu Unique Match && TrkBX=0 && Plateau";
  LDoubleMuModes->AddEntry(SMRecoPtMatchDoubleMuModesClone, LMatchDoubleMuModes);
  LDoubleMuModes->AddEntry(SMRecoPtMatchDoubleMuModesBX0Clone, LMatchDoubleMuModesBX0);
  LDoubleMuModes->AddEntry(SMRecoPtMatchDoubleMuModesBX0PlateauClone, LMatchDoubleMuModesBX0Plateau);
  LDoubleMuModes->SetFillStyle(0);
  LDoubleMuModes->SetBorderSize(0);
  LDoubleMuModes->Draw(); 
  CDoubleMuModes->Write();
  
  //MuOpen
  TCanvas *CMuOpenModes = new TCanvas("CMuOpenModes","MuOpenModes",700,500);
  THStack *SMuOpenModes = new THStack("SMuOpenModes","MuOpenModes");
  CMuOpenModes->cd();
  TH1F *SMRecoPtMatchMuOpenModesClone = (TH1F*)SMRecoPtMatchMuOpenModes->Clone("SMRecoPtMatchMuOpenModesClone");
  TH1F *SMRecoPtMatchMuOpenModesBX0Clone = (TH1F*)SMRecoPtMatchMuOpenModesBX0->Clone("SMRecoPtMatchMuOpenModesBX0Clone");
  TH1F *SMRecoPtMatchMuOpenModesBX0PlateauClone = (TH1F*)SMRecoPtMatchMuOpenModesBX0Plateau->Clone("SMRecoPtMatchMuOpenModesBX0PlateauClone");
  SMRecoPtMatchMuOpenModesClone->SetLineColor(1);//blk
  SMRecoPtMatchMuOpenModesBX0Clone->SetLineColor(2);//red
  SMRecoPtMatchMuOpenModesBX0PlateauClone->SetLineColor(4);//blue
  gStyle->SetOptStat(0);
  SMRecoPtMatchMuOpenModesClone->Divide(SMRecoPt);
  SMRecoPtMatchMuOpenModesBX0Clone->Divide(SMRecoPt);
  SMRecoPtMatchMuOpenModesBX0PlateauClone->Divide(SMRecoPt);
  SMuOpenModes->Add(SMRecoPtMatchMuOpenModesClone);
  SMuOpenModes->Add(SMRecoPtMatchMuOpenModesBX0Clone);
  SMuOpenModes->Add(SMRecoPtMatchMuOpenModesBX0PlateauClone);
  SMuOpenModes->Draw("nostack");
  SMuOpenModes->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMuOpenModes->GetYaxis()->SetTitle("Fraction of RECO muons");
  SMuOpenModes->Modified();
        
  TLegend* LMuOpenModes = new TLegend(0.6,0.6,0.9,0.9);
  TString LMatchMuOpenModes = "";
  LMatchMuOpenModes = LMatchMuOpenModes + "RecoMu Match";
  TString LMatchMuOpenModesBX0 = "";
  LMatchMuOpenModesBX0 = LMatchMuOpenModesBX0 + "RecoMu Unique Match && TrkBX=0";
  TString LMatchMuOpenModesBX0Plateau = "";
  LMatchMuOpenModesBX0Plateau = LMatchMuOpenModesBX0Plateau + "RecoMu Unique Match && TrkBX=0 && Plateau";
  LMuOpenModes->AddEntry(SMRecoPtMatchMuOpenModesClone, LMatchMuOpenModes);
  LMuOpenModes->AddEntry(SMRecoPtMatchMuOpenModesBX0Clone, LMatchMuOpenModesBX0);
  LMuOpenModes->AddEntry(SMRecoPtMatchMuOpenModesBX0PlateauClone, LMatchMuOpenModesBX0Plateau);
  LMuOpenModes->SetFillStyle(0);
  LMuOpenModes->SetBorderSize(0);
  LMuOpenModes->Draw(); 
  CMuOpenModes->Write();
	
  //divide histograms for eff
  TCanvas *CBadMatchEff = new TCanvas("CBadMatchEff","CBadMatchEff",700,500);
  THStack *SBadMatchEff = new THStack("SBadMatchEff","SBadMatchEff");
  CBadMatchEff->cd();
  TH1F *SMRecoPtNoMatchClone = (TH1F*)SMRecoPtNoMatch->Clone("SMRecoPtNoMatchClone");
  TH1F *SMRecoPtNoUniqueMatchClone = (TH1F*)SMRecoPtNoUniqueMatch->Clone("SMRecoPtNoUniqueMatchClone");
  TH1F *SMRecoPtMatchClone = (TH1F*)SMRecoPtMatch->Clone("SMRecoPtMatchClone");
  TH1F *SMRecoPtUniqueMatchBX0Clone = (TH1F*)SMRecoPtUniqueMatchBX0->Clone("SMRecoPtUniqueMatchBX0Clone");
  SMRecoPtNoMatchClone->SetLineColor(1);//red
  SMRecoPtNoUniqueMatchClone->SetLineColor(2);//blk
  SMRecoPtMatchClone->SetLineColor(3);//green
  SMRecoPtUniqueMatchBX0Clone->SetLineColor(4);//blue
  gStyle->SetOptStat(0);
  SMRecoPtNoMatchClone->Divide(SMRecoPt);
  SMRecoPtNoUniqueMatchClone->Divide(SMRecoPt);
  SMRecoPtMatchClone->Divide(SMRecoPt);
  SMRecoPtUniqueMatchBX0Clone->Divide(SMRecoPt);
  SBadMatchEff->Add(SMRecoPtNoMatchClone);
  SBadMatchEff->Add(SMRecoPtNoUniqueMatchClone);
  SBadMatchEff->Add(SMRecoPtMatchClone);
  SBadMatchEff->Add(SMRecoPtUniqueMatchBX0Clone);
  SBadMatchEff->Draw("nostack");
  SBadMatchEff->GetXaxis()->SetTitle("RECO pT[GeV]");
  SBadMatchEff->GetYaxis()->SetTitle("Fraction of RECO muons");
  CBadMatchEff->Modified();
        
  TLegend* LBadMatchEff = new TLegend(0.6,0.6,0.9,0.9);
  TString LNoMatch = "";
  LNoMatch = LNoMatch + "RecoMu No Match";
  TString LNoUniqueMatch = "";
  LNoUniqueMatch = LNoUniqueMatch + "RecoMu Not Uniquely Matched";
  TString LMatch = "";
  LMatch = LMatch + "RecoMu Match";
  TString LUniqueMatchBX0 = "";
  LUniqueMatchBX0 = LUniqueMatchBX0 + "RecoMu Unique Match && trk BX=0";
  LBadMatchEff->AddEntry(SMRecoPtNoMatchClone, LNoMatch);
  LBadMatchEff->AddEntry(SMRecoPtNoUniqueMatchClone, LNoUniqueMatch);
  LBadMatchEff->AddEntry(SMRecoPtMatchClone, LMatch);
  LBadMatchEff->AddEntry(SMRecoPtUniqueMatchBX0Clone, LUniqueMatchBX0);
  LBadMatchEff->SetFillStyle(0);
  LBadMatchEff->SetBorderSize(0);
  LBadMatchEff->Draw(); 
  CBadMatchEff->Write();

//rate ratio
  double RatioSingleMuModes[30]={0};
  double RatioSingleMuModesdBX0[30]={0};
  double RatioSingleMuModesRecoSoft[30]={0};
  double RatioSingleMuModesRecoOnly[30]={0};

  double RatioDoubleMuModes[30]={0};
  double RatioDoubleMuModesdBX0[30]={0};
  double RatioDoubleMuModesRecoSoft[30]={0};
  double RatioDoubleMuModesRecoOnly[30]={0};
	
  double RatioMuOpenModes[30]={0};
  double RatioMuOpenModesdBX0[30]={0};
  double RatioMuOpenModesRecoSoft[30]={0};
  double RatioMuOpenModesRecoOnly[30]={0};
  
  TCanvas *CRate=new TCanvas("CRate","All Modes",700,500);
	
  TCanvas *CRateSingleMuModes=new TCanvas("CRateSingleMuModes","SingleMuModes",700,500);
  TCanvas *CRateDoubleMuModes=new TCanvas("CRateDoubleMuModes","DoubleMuModes",700,500);
  TCanvas *CRateMuOpenModes=new TCanvas("CRateMuOpenModes","MuOpenModes",700,500);
	
//all modes
  TMultiGraph *mg=new TMultiGraph();
  CRate->cd();
  mg->SetTitle("All Modes");
  mg->Add(Rate);
  mg->Add(RatedBX0);
  mg->Add(RateRecoSoft);
  mg->Add(RateRecoOnly);
  mg->Draw("L");
  mg->Write();
//SingleMu
  TMultiGraph *mgSingleMuModes=new TMultiGraph();
  for(int i=0;i<30;i++){
	  RatioSingleMuModes[i] = 1.0*CountSingleMuModes[i]/Count[i];
	  RatioSingleMuModesdBX0[i] = 1.0*CountSingleMuModesdBX0[i]/Count[i];
          RatioSingleMuModesRecoSoft[i] = 1.0*CountSingleMuModesRecoSoft[i]/Count[i];
          RatioSingleMuModesRecoOnly[i] = 1.0*CountSingleMuModesRecoOnly[i]/Count[i];
  }
  TGraph *RateRatioSingleMuModes = new TGraph(30, TrigPT, RatioSingleMuModes); RateRatioSingleMuModes->SetMarkerStyle(22); RateRatioSingleMuModes->SetLineColor(1);
  TGraph *RateRatioSingleMuModesdBX0 = new TGraph(30, TrigPT, RatioSingleMuModesdBX0); RateRatioSingleMuModesdBX0->SetMarkerStyle(22); RateRatioSingleMuModesdBX0->SetLineColor(2);
  TGraph *RateRatioSingleMuModesRecoSoft = new TGraph(30, TrigPT, RatioSingleMuModesRecoSoft); RateRatioSingleMuModesRecoSoft->SetMarkerStyle(22); RateRatioSingleMuModesRecoSoft->SetLineColor(3);
  TGraph *RateRatioSingleMuModesRecoOnly = new TGraph(30, TrigPT, RatioSingleMuModesRecoOnly); RateRatioSingleMuModesRecoOnly->SetMarkerStyle(22); RateRatioSingleMuModesRecoOnly->SetLineColor(4);
  CRateSingleMuModes->cd();
  mgSingleMuModes->SetTitle("SingleMu Modes");
  mgSingleMuModes->Add(RateRatioSingleMuModes);
  mgSingleMuModes->Add(RateRatioSingleMuModesdBX0);
  mgSingleMuModes->Add(RateRatioSingleMuModesRecoSoft);
  mgSingleMuModes->Add(RateRatioSingleMuModesRecoOnly);
  mgSingleMuModes->Draw("LP");
  mgSingleMuModes->Write();
	
//DoubleMu
  TMultiGraph *mgDoubleMuModes=new TMultiGraph();
  for(int i=0;i<30;i++){
	  RatioDoubleMuModes[i] = 1.0*CountDoubleMuModes[i]/Count[i];
	  RatioDoubleMuModesdBX0[i] = 1.0*CountDoubleMuModesdBX0[i]/Count[i];
          RatioDoubleMuModesRecoSoft[i] = 1.0*CountDoubleMuModesRecoSoft[i]/Count[i];
          RatioDoubleMuModesRecoOnly[i] = 1.0*CountDoubleMuModesRecoOnly[i]/Count[i];
  }
  TGraph *RateRatioDoubleMuModes = new TGraph(30, TrigPT, RatioDoubleMuModes); RateRatioDoubleMuModes->SetMarkerStyle(22); RateRatioDoubleMuModes->SetLineColor(1);
  TGraph *RateRatioDoubleMuModesdBX0 = new TGraph(30, TrigPT, RatioDoubleMuModesdBX0); RateRatioDoubleMuModesdBX0->SetMarkerStyle(22); RateRatioDoubleMuModesdBX0->SetLineColor(2);
  TGraph *RateRatioDoubleMuModesRecoSoft = new TGraph(30, TrigPT, RatioDoubleMuModesRecoSoft); RateRatioDoubleMuModesRecoSoft->SetMarkerStyle(22); RateRatioDoubleMuModesRecoSoft->SetLineColor(3);
  TGraph *RateRatioDoubleMuModesRecoOnly = new TGraph(30, TrigPT, RatioDoubleMuModesRecoOnly); RateRatioDoubleMuModesRecoOnly->SetMarkerStyle(22); RateRatioDoubleMuModesRecoOnly->SetLineColor(4);
  CRateDoubleMuModes->cd();
  mgDoubleMuModes->SetTitle("DoubleMu Modes");
  mgDoubleMuModes->Add(RateRatioDoubleMuModes);
  mgDoubleMuModes->Add(RateRatioDoubleMuModesdBX0);
  mgDoubleMuModes->Add(RateRatioDoubleMuModesRecoSoft);
  mgDoubleMuModes->Add(RateRatioDoubleMuModesRecoOnly);
  mgDoubleMuModes->Draw("LP");
  mgDoubleMuModes->Write();
	
//MuOpen
  TMultiGraph *mgMuOpenModes=new TMultiGraph();
  for(int i=0;i<30;i++){
	  RatioMuOpenModes[i] = 1.0*CountMuOpenModes[i]/Count[i];
	  RatioMuOpenModesdBX0[i] = 1.0*CountMuOpenModesdBX0[i]/Count[i];
          RatioMuOpenModesRecoSoft[i] = 1.0*CountMuOpenModesRecoSoft[i]/Count[i];
          RatioMuOpenModesRecoOnly[i] = 1.0*CountMuOpenModesRecoOnly[i]/Count[i];
  }
  TGraph *RateRatioMuOpenModes = new TGraph(30, TrigPT, RatioMuOpenModes); RateRatioMuOpenModes->SetMarkerStyle(22); RateRatioMuOpenModes->SetLineColor(1);
  TGraph *RateRatioMuOpenModesdBX0 = new TGraph(30, TrigPT, RatioMuOpenModesdBX0); RateRatioMuOpenModesdBX0->SetMarkerStyle(22); RateRatioMuOpenModesdBX0->SetLineColor(2);
  TGraph *RateRatioMuOpenModesRecoSoft = new TGraph(30, TrigPT, RatioMuOpenModesRecoSoft); RateRatioMuOpenModesRecoSoft->SetMarkerStyle(22); RateRatioMuOpenModesRecoSoft->SetLineColor(3);
  TGraph *RateRatioMuOpenModesRecoOnly = new TGraph(30, TrigPT, RatioMuOpenModesRecoOnly); RateRatioMuOpenModesRecoOnly->SetMarkerStyle(22); RateRatioMuOpenModesRecoOnly->SetLineColor(4);
  CRateMuOpenModes->cd();
  mgMuOpenModes->SetTitle("MuOpen Modes");
  mgMuOpenModes->Add(RateRatioMuOpenModes);
  mgMuOpenModes->Add(RateRatioMuOpenModesdBX0);
  mgMuOpenModes->Add(RateRatioMuOpenModesRecoSoft);
  mgMuOpenModes->Add(RateRatioMuOpenModesRecoOnly);
  mgMuOpenModes->Draw("LP");
  mgMuOpenModes->Write();
 
  //intermidiate plots
  SMRecoPtNoMatch->Write();
  SMRecoPtNoUniqueMatch->Write();
  SMRecoPtMatch->Write();
  SMRecoPtUniqueMatchBX0->Write();
	
  SMRecoPtMatchSingleMuModes->Write();
  SMRecoPtMatchSingleMuModesBX0->Write();
  SMRecoPtMatchSingleMuModesBX0Plateau->Write();

  SMRecoPtMatchDoubleMuModes->Write();
  SMRecoPtMatchDoubleMuModesBX0->Write();
  SMRecoPtMatchDoubleMuModesBX0Plateau->Write();
	
  SMRecoPtMatchMuOpenModes->Write();
  SMRecoPtMatchMuOpenModesBX0->Write();
  SMRecoPtMatchMuOpenModesBX0Plateau->Write();
  
  myPlot.Close();
  
} // End function: void 
