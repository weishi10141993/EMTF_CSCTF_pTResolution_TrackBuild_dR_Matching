#include "TFile.h"
#include "TSystem.h"
#include "TChain.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH2.h"

//*** USER modify below ONLY ***//
// List of input branches and functions to return values
#include "Read_FlatNtuple.h" //make sure it's up-to-date with the Ntuple
//================================================================
//Part I: Rate/eff by track mode
const bool verbose = false; // Print information
const int PT_UP = 30;//Reco pT range
const int PT_LOW = 0;
const float ETA_UP = 2.4;//Reco eta @station 2
const float ETA_LOW = 1.25;
const int MAX_EVT = 100000000;   // Max number of events to process
const int PRT_EVT = 10000;   // Print every N events
//================================================================
//Part II: EMTF unbiased setting for pT training
const int Bias_Pt = 26;//SingleMu is collected with IsoMu24
const float Bias_Eta = 1.0;//barrel/endcap distinction
const float Bias_Iso = 0.25;//reco mu iso
//================================================================
//Part III: I/O
TString store = "/home/ws13/TMVA/TMVA/INPUT/"; //main dir
TString in_dir = "Ntuples/"; //sub dir
TString outFile = "/home/ws13/TMVA/TMVA/Study/ModesRateEff"; //output
TString SingleMu_files[1] = { 
	  		    "NTuple_0.root"
};   
TString ZeroBias_files[4] = { 
	  		    "NTuple_ZeroBias1_FlatNtuple_Run_306091_2018_03_02_ZB1.root",
		            "NTuple_ZeroBias2_FlatNtuple_Run_306091_2018_03_02_ZB2.root",
		            "NTuple_ZeroBias3_FlatNtuple_Run_306091_2018_03_02_ZB3.root", 
		 	    "NTuple_ZeroBias4_FlatNtuple_Run_306091_2018_03_02_ZB4.root"
};
const int USESingleMu = 1;//# of SM files to use
const int USEZerobias = 4;//# of ZB files to use
//*** USER modify above ONLY ***//

void ModesRateEffV0() {
   
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
  TH1F *SMRecoPt = new TH1F("SMRecoPt", SMRecoPtTitle, 30, 0, 30);
	
  TH1F *SMRecoPtNoMatch = new TH1F("SMRecoPtNoMatch", "NoMatch "+ SMRecoPtTitle, 30, 0, 30);
  TH1F *SMRecoPtNoUniqueMatch = new TH1F("SMRecoPtNoUniqueMatch", "NoUniqueMatch "+ SMRecoPtTitle, 30, 0, 30);
  TH1F *SMRecoPtUniqueMatch = new TH1F("SMRecoPtUniqueMatch", "UniqueMatch "+ SMRecoPtTitle, 30, 0, 30);
  TH1F *SMRecoPtUniqueMatchBX0 = new TH1F("SMRecoPtUniqueMatchBX0", "UniqueMatchBX0 "+ SMRecoPtTitle, 30, 0, 30);
	
  //SingleMu only, DoubleMu only MuOpen Only
  TH1F *SMRecoPtMatchSingleMuModes = new TH1F("SMRecoPtMatchSingleMuModes", "Match SingleMu Modes" + SMRecoPtTitle, 30, 0, 30);
  TH1F *SMRecoPtMatchDoubleMuModes = new TH1F("SMRecoPtMatchDoubleMuModes", "Match DoubleMu Modes" + SMRecoPtTitle, 30, 0, 30);
  TH1F *SMRecoPtMatchMuOpenModes = new TH1F("SMRecoPtMatchMuOpenModes", "Match MuOpen Modes" + SMRecoPtTitle, 30, 0, 30);
	
  TH1F *SMRecoPtMatchSingleMuModesBX0 = new TH1F("SMRecoPtMatchSingleMuModesBX0", "Match SingleMu ModesBX0" + SMRecoPtTitle, 30, 0, 30);
  TH1F *SMRecoPtMatchDoubleMuModesBX0 = new TH1F("SMRecoPtMatchDoubleMuModesBX0", "Match DoubleMu ModesBX0" + SMRecoPtTitle, 30, 0, 30);
  TH1F *SMRecoPtMatchMuOpenModesBX0 = new TH1F("SMRecoPtMatchMuOpenModesBX0", "Match MuOpen ModesBX0" + SMRecoPtTitle, 30, 0, 30);
	
  TH1F *SMRecoPtMatchSingleMuModesBX0dBX0 = new TH1F("SMRecoPtMatchSingleMuModesBX0dBX0", "Match SingleMu ModesBX0dBX0" + SMRecoPtTitle, 30, 0, 30);
  TH1F *SMRecoPtMatchDoubleMuModesBX0dBX0 = new TH1F("SMRecoPtMatchDoubleMuModesBX0dBX0", "Match DoubleMu ModesBX0dBX0" + SMRecoPtTitle, 30, 0, 30);
  TH1F *SMRecoPtMatchMuOpenModesBX0dBX0 = new TH1F("SMRecoPtMatchMuOpenModesBX0dBX0", "Match MuOpen ModesBX0dBX0" + SMRecoPtTitle, 30, 0, 30);

  TH1F *SMRecoPtMatchSingleMuModesBX0Plateau = new TH1F("SMRecoPtMatchSingleMuModesBX0Plateau", "Match SingleMu ModesBX0Plateau" + SMRecoPtTitle, 30, 0, 30);
  TH1F *SMRecoPtMatchDoubleMuModesBX0Plateau = new TH1F("SMRecoPtMatchDoubleMuModesBX0Plateau", "Match DoubleMu ModesBX0Plateau" + SMRecoPtTitle, 30, 0, 30);
  TH1F *SMRecoPtMatchMuOpenModesBX0Plateau = new TH1F("SMRecoPtMatchMuOpenModesBX0Plateau", "Match MuOpen ModesBX0Plateau" + SMRecoPtTitle, 30, 0, 30);
	
  //SingluMu quality: Q>=12
  TH1F *SMRecoPtMatchMode15 = new TH1F("SMRecoPtMatchMode15", "Match Mode15" + SMRecoPtTitle, 30, 0, 30);
  TH1F *SMRecoPtMatchMode14 = new TH1F("SMRecoPtMatchMode14", "Match Mode14" + SMRecoPtTitle, 30, 0, 30);
  TH1F *SMRecoPtMatchMode13 = new TH1F("SMRecoPtMatchMode13", "Match Mode13" + SMRecoPtTitle, 30, 0, 30);
  TH1F *SMRecoPtMatchMode11 = new TH1F("SMRecoPtMatchMode11", "Match Mode11" + SMRecoPtTitle, 30, 0, 30);
  //DoubleMu quality: Q>=8
  TH1F *SMRecoPtMatchMode12 = new TH1F("SMRecoPtMatchMode12", "Match Mode12" + SMRecoPtTitle, 30, 0, 30);
  TH1F *SMRecoPtMatchMode10 = new TH1F("SMRecoPtMatchMode10", "Match Mode10" + SMRecoPtTitle, 30, 0, 30);
  TH1F *SMRecoPtMatchMode7 = new TH1F("SMRecoPtMatchMode7", "Match Mode7" + SMRecoPtTitle, 30, 0, 30);
  //MuOpen quality: Q>=4
  TH1F *SMRecoPtMatchMode9 = new TH1F("SMRecoPtMatchMode9", "Match Mode9" + SMRecoPtTitle, 30, 0, 30);
  TH1F *SMRecoPtMatchMode6 = new TH1F("SMRecoPtMatchMode6", "Match Mode6" + SMRecoPtTitle, 30, 0, 30);
  TH1F *SMRecoPtMatchMode5 = new TH1F("SMRecoPtMatchMode5", "Match Mode5" + SMRecoPtTitle, 30, 0, 30);
  TH1F *SMRecoPtMatchMode3 = new TH1F("SMRecoPtMatchMode3", "Match Mode3" + SMRecoPtTitle, 30, 0, 30);

  //SingluMu quality: Q>=12
  TH1F *SMRecoPtMatchMode15BX0 = new TH1F("SMRecoPtMatchMode15BX0", "Match Mode15BX0" + SMRecoPtTitle, 30, 0, 30);
  TH1F *SMRecoPtMatchMode14BX0 = new TH1F("SMRecoPtMatchMode14BX0", "Match Mode14BX0" + SMRecoPtTitle, 30, 0, 30);
  TH1F *SMRecoPtMatchMode13BX0 = new TH1F("SMRecoPtMatchMode13BX0", "Match Mode13BX0" + SMRecoPtTitle, 30, 0, 30);
  TH1F *SMRecoPtMatchMode11BX0 = new TH1F("SMRecoPtMatchMode11BX0", "Match Mode11BX0" + SMRecoPtTitle, 30, 0, 30);
  //DoubleMu quality: Q>=8
  TH1F *SMRecoPtMatchMode12BX0 = new TH1F("SMRecoPtMatchMode12BX0", "Match Mode12BX0" + SMRecoPtTitle, 30, 0, 30);
  TH1F *SMRecoPtMatchMode10BX0 = new TH1F("SMRecoPtMatchMode10BX0", "Match Mode10BX0" + SMRecoPtTitle, 30, 0, 30);
  TH1F *SMRecoPtMatchMode7BX0 = new TH1F("SMRecoPtMatchMode7BX0", "Match Mode7BX0" + SMRecoPtTitle, 30, 0, 30);
  //MuOpen quality: Q>=4
  TH1F *SMRecoPtMatchMode9BX0 = new TH1F("SMRecoPtMatchMode9BX0", "Match Mode9BX0" + SMRecoPtTitle, 30, 0, 30);
  TH1F *SMRecoPtMatchMode6BX0 = new TH1F("SMRecoPtMatchMode6BX0", "Match Mode6BX0" + SMRecoPtTitle, 30, 0, 30);
  TH1F *SMRecoPtMatchMode5BX0 = new TH1F("SMRecoPtMatchMode5BX0", "Match Mode5BX0" + SMRecoPtTitle, 30, 0, 30);
  TH1F *SMRecoPtMatchMode3BX0 = new TH1F("SMRecoPtMatchMode3BX0", "Match Mode3BX0" + SMRecoPtTitle, 30, 0, 30);

  //SingluMu quality: Q>=12
  TH1F *SMRecoPtMatchMode15BX0dBX0 = new TH1F("SMRecoPtMatchMode15BX0dBX0", "Match Mode15BX0dBX0" + SMRecoPtTitle, 30, 0, 30);
  TH1F *SMRecoPtMatchMode14BX0dBX0 = new TH1F("SMRecoPtMatchMode14BX0dBX0", "Match Mode14BX0dBX0" + SMRecoPtTitle, 30, 0, 30);
  TH1F *SMRecoPtMatchMode13BX0dBX0 = new TH1F("SMRecoPtMatchMode13BX0dBX0", "Match Mode13BX0dBX0" + SMRecoPtTitle, 30, 0, 30);
  TH1F *SMRecoPtMatchMode11BX0dBX0 = new TH1F("SMRecoPtMatchMode11BX0dBX0", "Match Mode11BX0dBX0" + SMRecoPtTitle, 30, 0, 30);
  //DoubleMu quality: Q>=8
  TH1F *SMRecoPtMatchMode12BX0dBX0 = new TH1F("SMRecoPtMatchMode12BX0dBX0", "Match Mode12BX0dBX0" + SMRecoPtTitle, 30, 0, 30);
  TH1F *SMRecoPtMatchMode10BX0dBX0 = new TH1F("SMRecoPtMatchMode10BX0dBX0", "Match Mode10BX0dBX0" + SMRecoPtTitle, 30, 0, 30);
  TH1F *SMRecoPtMatchMode7BX0dBX0 = new TH1F("SMRecoPtMatchMode7BX0dBX0", "Match Mode7BX0dBX0" + SMRecoPtTitle, 30, 0, 30);
  //MuOpen quality: Q>=4
  TH1F *SMRecoPtMatchMode9BX0dBX0 = new TH1F("SMRecoPtMatchMode9BX0dBX0", "Match Mode9BX0dBX0" + SMRecoPtTitle, 30, 0, 30);
  TH1F *SMRecoPtMatchMode6BX0dBX0 = new TH1F("SMRecoPtMatchMode6BX0dBX0", "Match Mode6BX0dBX0" + SMRecoPtTitle, 30, 0, 30);
  TH1F *SMRecoPtMatchMode5BX0dBX0 = new TH1F("SMRecoPtMatchMode5BX0dBX0", "Match Mode5BX0dBX0" + SMRecoPtTitle, 30, 0, 30);
  TH1F *SMRecoPtMatchMode3BX0dBX0 = new TH1F("SMRecoPtMatchMode3BX0dBX0", "Match Mode3BX0dBX0" + SMRecoPtTitle, 30, 0, 30);

  //SingluMu quality: Q>=12
  TH1F *SMRecoPtMatchMode15BX0Plateau = new TH1F("SMRecoPtMatchMode15BX0Plateau", "Match Mode15BX0Plateau" + SMRecoPtTitle, 30, 0, 30);
  TH1F *SMRecoPtMatchMode14BX0Plateau = new TH1F("SMRecoPtMatchMode14BX0Plateau", "Match Mode14BX0Plateau" + SMRecoPtTitle, 30, 0, 30);
  TH1F *SMRecoPtMatchMode13BX0Plateau = new TH1F("SMRecoPtMatchMode13BX0Plateau", "Match Mode13BX0Plateau" + SMRecoPtTitle, 30, 0, 30);
  TH1F *SMRecoPtMatchMode11BX0Plateau = new TH1F("SMRecoPtMatchMode11BX0Plateau", "Match Mode11BX0Plateau" + SMRecoPtTitle, 30, 0, 30);
  //DoubleMu quality: Q>=8
  TH1F *SMRecoPtMatchMode12BX0Plateau = new TH1F("SMRecoPtMatchMode12BX0Plateau", "Match Mode12BX0Plateau" + SMRecoPtTitle, 30, 0, 30);
  TH1F *SMRecoPtMatchMode10BX0Plateau = new TH1F("SMRecoPtMatchMode10BX0Plateau", "Match Mode10BX0Plateau" + SMRecoPtTitle, 30, 0, 30);
  TH1F *SMRecoPtMatchMode7BX0Plateau = new TH1F("SMRecoPtMatchMode7BX0Plateau", "Match Mode7BX0Plateau" + SMRecoPtTitle, 30, 0, 30);
  //MuOpen quality: Q>=4
  TH1F *SMRecoPtMatchMode9BX0Plateau = new TH1F("SMRecoPtMatchMode9BX0Plateau", "Match Mode9BX0Plateau" + SMRecoPtTitle, 30, 0, 30);
  TH1F *SMRecoPtMatchMode6BX0Plateau = new TH1F("SMRecoPtMatchMode6BX0Plateau", "Match Mode6BX0Plateau" + SMRecoPtTitle, 30, 0, 30);
  TH1F *SMRecoPtMatchMode5BX0Plateau = new TH1F("SMRecoPtMatchMode5BX0Plateau", "Match Mode5BX0Plateau" + SMRecoPtTitle, 30, 0, 30);
  TH1F *SMRecoPtMatchMode3BX0Plateau = new TH1F("SMRecoPtMatchMode3BX0Plateau", "Match Mode3BX0Plateau" + SMRecoPtTitle, 30, 0, 30);

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
    if (iEvt > MAX_EVT) break;
    if ( (iEvt % PRT_EVT) == 0 ) {
      std::cout << "\n*************************************" << std::endl;
      std::cout << "Looking at event " << iEvt << " out of " << nSMEvents << std::endl;
      std::cout << "*************************************" << std::endl;
    }

    SM_in_chain->GetEntry(iEvt);
    
    // From Read_FlatNtuple.h, use 'I("branch_name")' to get an integer branch value, 'F("branch_name") to get a float
    // Print info for unpacked EMTF tracks
    if (verbose) std::cout << "\n" << I("nRecoMuons") << " reco muons in the event" << std::endl;
	
    //===========================================================================
    // Trigger match
    //===========================================================================
    int BarrelRecoMu=0;
    int EndcapRecoMu=0;
    for (int ireco = 0; ireco < I("nRecoMuons"); ireco++) {
	    //RECO mu pT >  Bias pT, Iso < Bias_Iso, match St1 segment, medium ID
	    if( F("reco_pt", ireco) >= Bias_Pt && F("reco_iso", ireco) < Bias_Iso && I("reco_ID_station", ireco) == 1 && I("reco_ID_medium", ireco) == 1){
		    //Barrel Reco mu
		    if(fabs(F("reco_eta",ireco)) < Bias_Eta){
			    BarrelRecoMu++;
		    }
		    //Endcap Reco mu
		    if(fabs(F("reco_eta",ireco)) >= Bias_Eta){
			    EndcapRecoMu++;
		    }
	    }//select RECO mu
    }//end for ireco
	  
    for (int ireco = 0; ireco < I("nRecoMuons"); ireco++) {    
	    //==================
	    //Normal track study
	    //==================
	    if( F("reco_pt", ireco) >= PT_LOW && F("reco_pt", ireco) <= PT_UP && I("reco_ID_loose", ireco) == 1 && I("reco_ID_station", ireco) == 1 && fabs(F("reco_eta_St2",ireco)) >= ETA_LOW && fabs(F("reco_eta_St2", ireco) ) <= ETA_UP){
		   SMRecoPt->Fill( F("reco_pt", ireco) ); 
		    
		   if( I("reco_dR_match_nTrk", ireco) == 0 ){
			SMRecoPtNoMatch->Fill( F("reco_pt", ireco) ); 
		   }//no matched EMTF trk
		    
		   if( I("reco_dR_match_nTrk", ireco) > 0 && I("reco_dR_match_unique", ireco) == 0 ){
			SMRecoPtNoUniqueMatch->Fill( F("reco_pt", ireco) );   
		   }//no unique matched EMTF trk
		    
		   //reco mu has a EMTF trk match
		   if( I("reco_dR_match_unique", ireco) == 1 ){
			   
			//Uniquely matched EMTF trk all modes
			SMRecoPtUniqueMatch->Fill( F("reco_pt", ireco) ); 
			   
			if( I("trk_BX", I("reco_dR_match_iTrk", ireco) )  == 0 ){
				SMRecoPtUniqueMatchBX0->Fill( F("reco_pt", ireco) );   
			}//Uniquely matched EMTF trk && trk BX=0
			   
			//SingluMu
			if( I("trk_mode", I("reco_dR_match_iTrk", ireco) ) == 15 || 
			    I("trk_mode", I("reco_dR_match_iTrk", ireco) ) == 14 || 
			    I("trk_mode", I("reco_dR_match_iTrk", ireco) ) == 13 || 
			    I("trk_mode", I("reco_dR_match_iTrk", ireco) ) == 11){
				SMRecoPtMatchSingleMuModes->Fill( F("reco_pt", ireco) ); 
			        if( I("trk_BX", I("reco_dR_match_iTrk", ireco) )  == 0 ){
				    SMRecoPtMatchSingleMuModesBX0->Fill( F("reco_pt", ireco) ); 
				    if( I("trk_dBX", I("reco_dR_match_iTrk", ireco) )  == 0 ){
					    SMRecoPtMatchSingleMuModesBX0dBX0->Fill( F("reco_pt", ireco) ); 
				    }//require trk_dBX=0
				    if( F("trk_pt", I("reco_dR_match_iTrk", ireco) )  >  F("reco_pt", ireco) * 7.0/8.0){
					    SMRecoPtMatchSingleMuModesBX0Plateau->Fill( F("reco_pt", ireco) );
				    }//require trk_pt > reco_pt * (7/8)
			         }//require trk_BX=0
			}
			   
			//DoubleMu Inclusive
			if( I("trk_mode", I("reco_dR_match_iTrk", ireco) ) == 9  || 
			    I("trk_mode", I("reco_dR_match_iTrk", ireco) ) == 10 || 
			    I("trk_mode", I("reco_dR_match_iTrk", ireco) ) == 7  ||
			    I("trk_mode", I("reco_dR_match_iTrk", ireco) ) == 15 || 
			    I("trk_mode", I("reco_dR_match_iTrk", ireco) ) == 14 || 
			    I("trk_mode", I("reco_dR_match_iTrk", ireco) ) == 13 || 
			    I("trk_mode", I("reco_dR_match_iTrk", ireco) ) == 11){
				SMRecoPtMatchDoubleMuModes->Fill( F("reco_pt", ireco) ); 
			        if( I("trk_BX", I("reco_dR_match_iTrk", ireco) )  == 0 ){
				    SMRecoPtMatchDoubleMuModesBX0->Fill( F("reco_pt", ireco) ); 
				    if( I("trk_dBX", I("reco_dR_match_iTrk", ireco) )  == 0 ){
					    SMRecoPtMatchDoubleMuModesBX0dBX0->Fill( F("reco_pt", ireco) ); 
				    }//require trk_dBX=0
				    if( F("trk_pt", I("reco_dR_match_iTrk", ireco) )  >  F("reco_pt", ireco) * 7.0/8.0){
					    SMRecoPtMatchDoubleMuModesBX0Plateau->Fill( F("reco_pt", ireco) );
				    }//require trk_pt > reco_pt * (7/8)
			         }//require trk_BX=0
			}
			   
			//MuOpen Inclusive
			if( I("trk_mode", I("reco_dR_match_iTrk", ireco) ) == 12 || 
			    I("trk_mode", I("reco_dR_match_iTrk", ireco) ) == 6  || 
			    I("trk_mode", I("reco_dR_match_iTrk", ireco) ) == 5  || 
			    I("trk_mode", I("reco_dR_match_iTrk", ireco) ) == 3  ||
			    I("trk_mode", I("reco_dR_match_iTrk", ireco) ) == 9  || 
			    I("trk_mode", I("reco_dR_match_iTrk", ireco) ) == 10 || 
			    I("trk_mode", I("reco_dR_match_iTrk", ireco) ) == 7  ||
			    I("trk_mode", I("reco_dR_match_iTrk", ireco) ) == 15 || 
			    I("trk_mode", I("reco_dR_match_iTrk", ireco) ) == 14 || 
			    I("trk_mode", I("reco_dR_match_iTrk", ireco) ) == 13 || 
			    I("trk_mode", I("reco_dR_match_iTrk", ireco) ) == 11){
				SMRecoPtMatchMuOpenModes->Fill( F("reco_pt", ireco) ); 
			        if( I("trk_BX", I("reco_dR_match_iTrk", ireco) )  == 0 ){
				    SMRecoPtMatchMuOpenModesBX0->Fill( F("reco_pt", ireco) ); 
				    if( I("trk_dBX", I("reco_dR_match_iTrk", ireco) )  == 0 ){
					    SMRecoPtMatchMuOpenModesBX0dBX0->Fill( F("reco_pt", ireco) ); 
				    }//require trk_dBX=0
				    if( F("trk_pt", I("reco_dR_match_iTrk", ireco) )  >  F("reco_pt", ireco) * 7.0/8.0){
					    SMRecoPtMatchMuOpenModesBX0Plateau->Fill( F("reco_pt", ireco) );
				    }//require trk_pt > reco_pt * (7/8)
			         }//require trk_BX=0
			}
			  
			//each mode  
			switch ( I("trk_mode", I("reco_dR_match_iTrk", ireco) ) ) {
                            
                        case 15:
                            SMRecoPtMatchMode15->Fill( F("reco_pt", ireco) ); 
			    if( I("trk_BX", I("reco_dR_match_iTrk", ireco) )  == 0 ){
				    SMRecoPtMatchMode15BX0->Fill( F("reco_pt", ireco) ); 
				    if( I("trk_dBX", I("reco_dR_match_iTrk", ireco) )  == 0 ){
					    SMRecoPtMatchMode15BX0dBX0->Fill( F("reco_pt", ireco) ); 
				    }//require trk_dBX=0
				    if( F("trk_pt", I("reco_dR_match_iTrk", ireco) )  >  F("reco_pt", ireco) * 7.0/8.0){
					    SMRecoPtMatchMode15BX0Plateau->Fill( F("reco_pt", ireco) );
				    }//require trk_pt > reco_pt * (7/8)
			    }//require trk_BX=0
                            break; 
			case 14:
                            SMRecoPtMatchMode14->Fill( F("reco_pt", ireco) ); 
			    if( I("trk_BX", I("reco_dR_match_iTrk", ireco) )  == 0 ){
				    SMRecoPtMatchMode14BX0->Fill( F("reco_pt", ireco) ); 
				    if( I("trk_dBX", I("reco_dR_match_iTrk", ireco) )  == 0 ){
					    SMRecoPtMatchMode14BX0dBX0->Fill( F("reco_pt", ireco) ); 
				    }
				    if( F("trk_pt", I("reco_dR_match_iTrk", ireco) )  >  F("reco_pt", ireco) * 7.0/8.0){
					    SMRecoPtMatchMode14BX0Plateau->Fill( F("reco_pt", ireco) );
				    }
			    }
                            break;
		        case 13:
                            SMRecoPtMatchMode13->Fill( F("reco_pt", ireco) ); 
			    if( I("trk_BX", I("reco_dR_match_iTrk", ireco) )  == 0 ){
				    SMRecoPtMatchMode13BX0->Fill( F("reco_pt", ireco) ); 
				    if( I("trk_dBX", I("reco_dR_match_iTrk", ireco) )  == 0 ){
					    SMRecoPtMatchMode13BX0dBX0->Fill( F("reco_pt", ireco) ); 
				    }
				    if( F("trk_pt", I("reco_dR_match_iTrk", ireco) )  >  F("reco_pt", ireco) * 7.0/8.0){
					    SMRecoPtMatchMode13BX0Plateau->Fill( F("reco_pt", ireco) );
				    }
			    }
                            break; 
			case 11:
                            SMRecoPtMatchMode11->Fill( F("reco_pt", ireco) ); 
			    if( I("trk_BX", I("reco_dR_match_iTrk", ireco) )  == 0 ){
				    SMRecoPtMatchMode11BX0->Fill( F("reco_pt", ireco) ); 
				    if( I("trk_dBX", I("reco_dR_match_iTrk", ireco) )  == 0 ){
					    SMRecoPtMatchMode11BX0dBX0->Fill( F("reco_pt", ireco) ); 
				    }
				    if( F("trk_pt", I("reco_dR_match_iTrk", ireco) )  >  F("reco_pt", ireco) * 7.0/8.0){
					    SMRecoPtMatchMode11BX0Plateau->Fill( F("reco_pt", ireco) );
				    }
			    }
                            break;
			case 12:
                            SMRecoPtMatchMode12->Fill( F("reco_pt", ireco) ); 
			    if( I("trk_BX", I("reco_dR_match_iTrk", ireco) )  == 0 ){
				    SMRecoPtMatchMode12BX0->Fill( F("reco_pt", ireco) ); 
				    if( I("trk_dBX", I("reco_dR_match_iTrk", ireco) )  == 0 ){
					    SMRecoPtMatchMode12BX0dBX0->Fill( F("reco_pt", ireco) ); 
				    }
				    if( F("trk_pt", I("reco_dR_match_iTrk", ireco) )  >  F("reco_pt", ireco) * 7.0/8.0){
					    SMRecoPtMatchMode12BX0Plateau->Fill( F("reco_pt", ireco) );
				    }
			    }
                            break; 
			case 10:
                            SMRecoPtMatchMode10->Fill( F("reco_pt", ireco) ); 
			    if( I("trk_BX", I("reco_dR_match_iTrk", ireco) )  == 0 ){
				    SMRecoPtMatchMode10BX0->Fill( F("reco_pt", ireco) ); 
				    if( I("trk_dBX", I("reco_dR_match_iTrk", ireco) )  == 0 ){
					    SMRecoPtMatchMode10BX0dBX0->Fill( F("reco_pt", ireco) ); 
				    }
				    if( F("trk_pt", I("reco_dR_match_iTrk", ireco) )  >  F("reco_pt", ireco) * 7.0/8.0){
					    SMRecoPtMatchMode10BX0Plateau->Fill( F("reco_pt", ireco) );
				    }
			    }
                            break;
			case 7:
                            SMRecoPtMatchMode7->Fill( F("reco_pt", ireco) ); 
			    if( I("trk_BX", I("reco_dR_match_iTrk", ireco) )  == 0 ){
				    SMRecoPtMatchMode7BX0->Fill( F("reco_pt", ireco) ); 
				    if( I("trk_dBX", I("reco_dR_match_iTrk", ireco) )  == 0 ){
					    SMRecoPtMatchMode7BX0dBX0->Fill( F("reco_pt", ireco) ); 
				    }
				    if( F("trk_pt", I("reco_dR_match_iTrk", ireco) )  >  F("reco_pt", ireco) * 7.0/8.0){
					    SMRecoPtMatchMode7BX0Plateau->Fill( F("reco_pt", ireco) );
				    }
			    }
                            break; 
			case 9:
                            SMRecoPtMatchMode9->Fill( F("reco_pt", ireco) ); 
                            if( I("trk_BX", I("reco_dR_match_iTrk", ireco) )  == 0 ){
				    SMRecoPtMatchMode9BX0->Fill( F("reco_pt", ireco) ); 
				    if( I("trk_dBX", I("reco_dR_match_iTrk", ireco) )  == 0 ){
					    SMRecoPtMatchMode9BX0dBX0->Fill( F("reco_pt", ireco) ); 
				    }
				    if( F("trk_pt", I("reco_dR_match_iTrk", ireco) )  >  F("reco_pt", ireco) * 7.0/8.0){
					    SMRecoPtMatchMode9BX0Plateau->Fill( F("reco_pt", ireco) );
				    }
			    }
			    break;
			case 6:
                            SMRecoPtMatchMode6->Fill( F("reco_pt", ireco) ); 
			    if( I("trk_BX", I("reco_dR_match_iTrk", ireco) )  == 0 ){
				    SMRecoPtMatchMode6BX0->Fill( F("reco_pt", ireco) ); 
				    if( I("trk_dBX", I("reco_dR_match_iTrk", ireco) )  == 0 ){
					    SMRecoPtMatchMode6BX0dBX0->Fill( F("reco_pt", ireco) ); 
				    }
				    if( F("trk_pt", I("reco_dR_match_iTrk", ireco) )  >  F("reco_pt", ireco) * 7.0/8.0){
					    SMRecoPtMatchMode6BX0Plateau->Fill( F("reco_pt", ireco) );
				    }
			    }
                            break; 
			case 5:
                            SMRecoPtMatchMode5->Fill( F("reco_pt", ireco) ); 
			    if( I("trk_BX", I("reco_dR_match_iTrk", ireco) )  == 0 ){
				    SMRecoPtMatchMode5BX0->Fill( F("reco_pt", ireco) ); 
				    if( I("trk_dBX", I("reco_dR_match_iTrk", ireco) )  == 0 ){
					    SMRecoPtMatchMode5BX0dBX0->Fill( F("reco_pt", ireco) ); 
				    }
				    if( F("trk_pt", I("reco_dR_match_iTrk", ireco) )  >  F("reco_pt", ireco) * 7.0/8.0){
					    SMRecoPtMatchMode5BX0Plateau->Fill( F("reco_pt", ireco) );
				    }
			    }
                            break;
			case 3:
                            SMRecoPtMatchMode3->Fill( F("reco_pt", ireco) ); 
			    if( I("trk_BX", I("reco_dR_match_iTrk", ireco) )  == 0 ){
				    SMRecoPtMatchMode3BX0->Fill( F("reco_pt", ireco) ); 
				    if( I("trk_dBX", I("reco_dR_match_iTrk", ireco) )  == 0 ){
					    SMRecoPtMatchMode3BX0dBX0->Fill( F("reco_pt", ireco) ); 
				    }
				    if( F("trk_pt", I("reco_dR_match_iTrk", ireco) )  >  F("reco_pt", ireco) * 7.0/8.0){
					    SMRecoPtMatchMode3BX0Plateau->Fill( F("reco_pt", ireco) );
				    }
			    }
                            break;
                        default:
                            break;           
                        }//end switch
			   
		   }//matched to unique EMTF trk
		   
	    }//selection on reco mu
    }//end 2nd loop over SingleMu
    
  } // End loop events
  std::cout << "\n******* Finished looping over the SingleMu events *******" << std::endl;
  
  std::cout << "\n******* About to loop over the Zerobias events *******" << std::endl;
  int nZBEvents = ZB_in_chain->GetEntries();
  for (int iEvt = 0; iEvt < nZBEvents; iEvt++) {
    if (iEvt > MAX_EVT) break;
    if ( (iEvt % PRT_EVT) == 0 ) {
      std::cout << "\n*************************************" << std::endl;
      std::cout << "Looking at event " << iEvt << " out of " << nZBEvents << std::endl;
      std::cout << "*************************************" << std::endl;
    }

    ZB_in_chain->GetEntry(iEvt);
    
    // From Read_FlatNtuple.h, use 'I("branch_name")' to get an integer branch value, 'F("branch_name") to get a float
    // Print info for unpacked EMTF tracks
    if (verbose) std::cout << "\n" << I("nTracks") << " tracks in the event" << std::endl;
    for (int itrack = 0; itrack < I("nTracks"); itrack++) {
	    if( I("trk_BX", itrack) == 0 && fabs( F("trk_eta", itrack) ) >= ETA_LOW && I("trk_mode_neighbor", itrack ) < I("trk_mode", itrack ) ){
		    for(int i=0;i<30;i++){
			    
			    if( F("trk_pt", itrack)> TrigPT[i]){
				   Count[i]++; 
				   if( I("trk_dBX", itrack) == 0 ){
					CountdBX0[i]++;   
			           }
				   if( (I("trk_dR_match_nReco", itrack) + I("trk_dR_match_nRecoSoft", itrack)) >= 1 ){
				        CountRecoSoft[i]++;   
			           }
				   if( I("trk_dR_match_nReco", itrack) >= 1 ){
				        CountRecoOnly[i]++;   
			           }
				   //SingleMu Only
				   if( I("trk_mode", itrack ) == 15 || 
			               I("trk_mode", itrack ) == 14 || 
			    	       I("trk_mode", itrack ) == 13 || 
			    	       I("trk_mode", itrack ) == 11){
					    CountSingleMuModes[i]++; 
					    if( I("trk_dBX", itrack) == 0 ){
						 CountSingleMuModesdBX0[i]++;   
					    }
					    if( (I("trk_dR_match_nReco", itrack) + I("trk_dR_match_nRecoSoft", itrack)) >= 1 ){
						 CountSingleMuModesRecoSoft[i]++;   
					    }
					    if( I("trk_dR_match_nReco", itrack) >= 1 ){
						 CountSingleMuModesRecoOnly[i]++;   
					    }
				   }
				   //DoubleMu Only
				   if( I("trk_mode", itrack ) == 9  || 
			               I("trk_mode", itrack ) == 10 || 
			    	       I("trk_mode", itrack ) == 7  ||
				       I("trk_mode", itrack ) == 15 || 
			               I("trk_mode", itrack ) == 14 || 
			    	       I("trk_mode", itrack ) == 13 || 
			    	       I("trk_mode", itrack ) == 11){
					    CountDoubleMuModes[i]++; 
					    if( I("trk_dBX", itrack) == 0 ){
						 CountDoubleMuModesdBX0[i]++;   
					    }
					    if( (I("trk_dR_match_nReco", itrack) + I("trk_dR_match_nRecoSoft", itrack)) >= 1 ){
						 CountDoubleMuModesRecoSoft[i]++;   
					    }
					    if( I("trk_dR_match_nReco", itrack) >= 1 ){
						 CountDoubleMuModesRecoOnly[i]++;   
					    }
				   }
				   //MuOpen Only
				   if( I("trk_mode", itrack ) == 12 || 
			               I("trk_mode", itrack ) == 6  || 
			    	       I("trk_mode", itrack ) == 5  || 
			    	       I("trk_mode", itrack ) == 3  ||
				       I("trk_mode", itrack ) == 9  || 
			               I("trk_mode", itrack ) == 10 || 
			    	       I("trk_mode", itrack ) == 7  ||
				       I("trk_mode", itrack ) == 15 || 
			               I("trk_mode", itrack ) == 14 || 
			    	       I("trk_mode", itrack ) == 13 || 
			    	       I("trk_mode", itrack ) == 11){
					    CountMuOpenModes[i]++; 
					    if( I("trk_dBX", itrack) == 0 ){
						 CountMuOpenModesdBX0[i]++;   
					    }
					    if( (I("trk_dR_match_nReco", itrack) + I("trk_dR_match_nRecoSoft", itrack)) >= 1 ){
						 CountMuOpenModesRecoSoft[i]++;   
					    }
					    if( I("trk_dR_match_nReco", itrack) >= 1 ){
						 CountMuOpenModesRecoOnly[i]++;   
					    }
				   }
				   //each trk mode
				   switch ( I("trk_mode", itrack ) ) {
				    case 15:
					    CountMode15[i]++; 
					    if( I("trk_dBX", itrack) == 0 ){
						 CountMode15dBX0[i]++;   
					    }
					    if( (I("trk_dR_match_nReco", itrack) + I("trk_dR_match_nRecoSoft", itrack)) >= 1 ){
						 CountMode15RecoSoft[i]++;   
					    }
					    if( I("trk_dR_match_nReco", itrack) >= 1 ){
						 CountMode15RecoOnly[i]++;   
					    }
                            	            break;
				    case 14:
					    CountMode14[i]++; 
					    if( I("trk_dBX", itrack) == 0 ){
						 CountMode14dBX0[i]++;   
					    }
					    if( (I("trk_dR_match_nReco", itrack) + I("trk_dR_match_nRecoSoft", itrack)) >= 1 ){
						 CountMode14RecoSoft[i]++;   
					    }
					    if( I("trk_dR_match_nReco", itrack) >= 1 ){
						 CountMode14RecoOnly[i]++;   
					    }
                            	            break;
				    case 13:
					    CountMode13[i]++; 
					    if( I("trk_dBX", itrack) == 0 ){
						 CountMode13dBX0[i]++;   
					    }
					    if( (I("trk_dR_match_nReco", itrack) + I("trk_dR_match_nRecoSoft", itrack)) >= 1 ){
						 CountMode13RecoSoft[i]++;   
					    }
					    if( I("trk_dR_match_nReco", itrack) >= 1 ){
						 CountMode13RecoOnly[i]++;   
					    }
                            	            break;
				    case 11:
					    CountMode11[i]++; 
					    if( I("trk_dBX", itrack) == 0 ){
						 CountMode11dBX0[i]++;   
					    }
					    if( (I("trk_dR_match_nReco", itrack) + I("trk_dR_match_nRecoSoft", itrack)) >= 1 ){
						 CountMode11RecoSoft[i]++;   
					    }
					    if( I("trk_dR_match_nReco", itrack) >= 1 ){
						 CountMode11RecoOnly[i]++;   
					    }
                            	            break;
				    case 12:
					    CountMode12[i]++; 
					    if( I("trk_dBX", itrack) == 0 ){
						 CountMode12dBX0[i]++;   
					    }
					    if( (I("trk_dR_match_nReco", itrack) + I("trk_dR_match_nRecoSoft", itrack)) >= 1 ){
						 CountMode12RecoSoft[i]++;   
					    }
					    if( I("trk_dR_match_nReco", itrack) >= 1 ){
						 CountMode12RecoOnly[i]++;   
					    }
                            	            break;
				    case 10:
					    CountMode10[i]++; 
					    if( I("trk_dBX", itrack) == 0 ){
						 CountMode10dBX0[i]++;   
					    }
					    if( (I("trk_dR_match_nReco", itrack) + I("trk_dR_match_nRecoSoft", itrack)) >= 1 ){
						 CountMode10RecoSoft[i]++;   
					    }
					    if( I("trk_dR_match_nReco", itrack) >= 1 ){
						 CountMode10RecoOnly[i]++;   
					    }
                            	            break;
				    case 7:
					    CountMode7[i]++; 
					    if( I("trk_dBX", itrack) == 0 ){
						 CountMode7dBX0[i]++;   
					    }
					    if( (I("trk_dR_match_nReco", itrack) + I("trk_dR_match_nRecoSoft", itrack)) >= 1 ){
						 CountMode7RecoSoft[i]++;   
					    }
					    if( I("trk_dR_match_nReco", itrack) >= 1 ){
						 CountMode7RecoOnly[i]++;   
					    }
                            	            break;
				    case 9:
					    CountMode9[i]++; 
					    if( I("trk_dBX", itrack) == 0 ){
						 CountMode9dBX0[i]++;   
					    }
					    if( (I("trk_dR_match_nReco", itrack) + I("trk_dR_match_nRecoSoft", itrack)) >= 1 ){
						 CountMode9RecoSoft[i]++;   
					    }
					    if( I("trk_dR_match_nReco", itrack) >= 1 ){
						 CountMode9RecoOnly[i]++;   
					    }
                            	            break;
				    case 6:
					    CountMode6[i]++; 
					    if( I("trk_dBX", itrack) == 0 ){
						 CountMode6dBX0[i]++;   
					    }
					    if( (I("trk_dR_match_nReco", itrack) + I("trk_dR_match_nRecoSoft", itrack)) >= 1 ){
						 CountMode6RecoSoft[i]++;   
					    }
					    if( I("trk_dR_match_nReco", itrack) >= 1 ){
						 CountMode6RecoOnly[i]++;   
					    }
                            	            break;
				    case 5:
					    CountMode5[i]++; 
					    if( I("trk_dBX", itrack) == 0 ){
						 CountMode5dBX0[i]++;   
					    }
					    if( (I("trk_dR_match_nReco", itrack) + I("trk_dR_match_nRecoSoft", itrack)) >= 1 ){
						 CountMode5RecoSoft[i]++;   
					    }
					    if( I("trk_dR_match_nReco", itrack) >= 1 ){
						 CountMode5RecoOnly[i]++;   
					    }
                            	            break;
				    case 3:
					    CountMode3[i]++; 
					    if( I("trk_dBX", itrack) == 0 ){
						 CountMode3dBX0[i]++;   
					    }
					    if( (I("trk_dR_match_nReco", itrack) + I("trk_dR_match_nRecoSoft", itrack)) >= 1 ){
						 CountMode3RecoSoft[i]++;   
					    }
					    if( I("trk_dR_match_nReco", itrack) >= 1 ){
						 CountMode3RecoOnly[i]++;   
					    }
                            	            break;
			            default:
                                            break; 
			           }//end switch mode
			    }       
		    }//loop over trig pT  
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
  
  SMRecoPtNoMatch->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtNoUniqueMatch->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtUniqueMatch->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtUniqueMatchBX0->GetXaxis()->SetTitle("RECO pT[GeV]");

  SMRecoPtMatchSingleMuModes->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchSingleMuModesBX0->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchSingleMuModesBX0dBX0->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchSingleMuModesBX0Plateau->GetXaxis()->SetTitle("RECO pT[GeV]");
	
  SMRecoPtMatchDoubleMuModes->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchDoubleMuModesBX0->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchDoubleMuModesBX0dBX0->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchDoubleMuModesBX0Plateau->GetXaxis()->SetTitle("RECO pT[GeV]");
	
  SMRecoPtMatchMuOpenModes->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchMuOpenModesBX0->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchMuOpenModesBX0dBX0->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchMuOpenModesBX0Plateau->GetXaxis()->SetTitle("RECO pT[GeV]");
	
  SMRecoPtMatchMode15->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchMode14->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchMode13->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchMode11->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchMode12->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchMode10->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchMode7->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchMode9->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchMode6->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchMode5->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchMode3->GetXaxis()->SetTitle("RECO pT[GeV]");
	
  SMRecoPtMatchMode15BX0->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchMode14BX0->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchMode13BX0->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchMode11BX0->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchMode12BX0->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchMode10BX0->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchMode7BX0->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchMode9BX0->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchMode6BX0->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchMode5BX0->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchMode3BX0->GetXaxis()->SetTitle("RECO pT[GeV]");
	
  SMRecoPtMatchMode15BX0dBX0->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchMode14BX0dBX0->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchMode13BX0dBX0->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchMode11BX0dBX0->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchMode12BX0dBX0->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchMode10BX0dBX0->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchMode7BX0dBX0->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchMode9BX0dBX0->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchMode6BX0dBX0->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchMode5BX0dBX0->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchMode3BX0dBX0->GetXaxis()->SetTitle("RECO pT[GeV]");
	
  SMRecoPtMatchMode15BX0Plateau->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchMode14BX0Plateau->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchMode13BX0Plateau->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchMode11BX0Plateau->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchMode12BX0Plateau->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchMode10BX0Plateau->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchMode7BX0Plateau->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchMode9BX0Plateau->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchMode6BX0Plateau->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchMode5BX0Plateau->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtMatchMode3BX0Plateau->GetXaxis()->SetTitle("RECO pT[GeV]");

  SMRecoPt->Write();
  //divide histograms for all modes
  //SingleMu
  TCanvas *CSingleMuModes = new TCanvas("CSingleMuModes","SingleMuModes",700,500);
  THStack *SSingleMuModes = new THStack("SSingleMuModes","SingleMuModes");
  CSingleMuModes->cd();
  TH1F *SMRecoPtMatchSingleMuModesClone = (TH1F*)SMRecoPtMatchSingleMuModes->Clone("SMRecoPtMatchSingleMuModesClone");
  TH1F *SMRecoPtMatchSingleMuModesBX0Clone = (TH1F*)SMRecoPtMatchSingleMuModesBX0->Clone("SMRecoPtMatchSingleMuModesBX0Clone");
  TH1F *SMRecoPtMatchSingleMuModesBX0dBX0Clone = (TH1F*)SMRecoPtMatchSingleMuModesBX0dBX0->Clone("SMRecoPtMatchSingleMuModesBX0dBX0Clone");
  TH1F *SMRecoPtMatchSingleMuModesBX0PlateauClone = (TH1F*)SMRecoPtMatchSingleMuModesBX0Plateau->Clone("SMRecoPtMatchSingleMuModesBX0PlateauClone");
  SMRecoPtMatchSingleMuModesClone->SetLineColor(1);//blk
  SMRecoPtMatchSingleMuModesBX0Clone->SetLineColor(2);//red
  SMRecoPtMatchSingleMuModesBX0dBX0Clone->SetLineColor(3);//green
  SMRecoPtMatchSingleMuModesBX0PlateauClone->SetLineColor(4);//blue
  gStyle->SetOptStat(0);
  SMRecoPtMatchSingleMuModesClone->Divide(SMRecoPt);
  SMRecoPtMatchSingleMuModesBX0Clone->Divide(SMRecoPt);
  SMRecoPtMatchSingleMuModesBX0dBX0Clone->Divide(SMRecoPt);
  SMRecoPtMatchSingleMuModesBX0PlateauClone->Divide(SMRecoPt);
  SSingleMuModes->Add(SMRecoPtMatchSingleMuModesClone);
  SSingleMuModes->Add(SMRecoPtMatchSingleMuModesBX0Clone);
  SSingleMuModes->Add(SMRecoPtMatchSingleMuModesBX0dBX0Clone);
  SSingleMuModes->Add(SMRecoPtMatchSingleMuModesBX0PlateauClone);
  SSingleMuModes->Draw("nostack");
  SSingleMuModes->GetXaxis()->SetTitle("RECO pT[GeV]");
  SSingleMuModes->GetYaxis()->SetTitle("Fraction of RECO muons");
  SSingleMuModes->Modified();
        
  TLegend* LSingleMuModes = new TLegend(0.6,0.6,0.9,0.9);
  TString LMatchSingleMuModes = "";
  LMatchSingleMuModes = LMatchSingleMuModes + "RecoMu Unique Match";
  TString LMatchSingleMuModesBX0 = "";
  LMatchSingleMuModesBX0 = LMatchSingleMuModesBX0 + "RecoMu Unique Match && TrkBX=0";
  TString LMatchSingleMuModesBX0dBX0 = "";
  LMatchSingleMuModesBX0dBX0 = LMatchSingleMuModesBX0dBX0 + "RecoMu Unique Match && TrkBX=0 && TrkdBX=0";
  TString LMatchSingleMuModesBX0Plateau = "";
  LMatchSingleMuModesBX0Plateau = LMatchSingleMuModesBX0Plateau + "RecoMu Unique Match && TrkBX=0 && Plateau";
  LSingleMuModes->AddEntry(SMRecoPtMatchSingleMuModesClone, LMatchSingleMuModes);
  LSingleMuModes->AddEntry(SMRecoPtMatchSingleMuModesBX0Clone, LMatchSingleMuModesBX0);
  LSingleMuModes->AddEntry(SMRecoPtMatchSingleMuModesBX0dBX0Clone, LMatchSingleMuModesBX0dBX0);
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
  TH1F *SMRecoPtMatchDoubleMuModesBX0dBX0Clone = (TH1F*)SMRecoPtMatchDoubleMuModesBX0dBX0->Clone("SMRecoPtMatchDoubleMuModesBX0dBX0Clone");
  TH1F *SMRecoPtMatchDoubleMuModesBX0PlateauClone = (TH1F*)SMRecoPtMatchDoubleMuModesBX0Plateau->Clone("SMRecoPtMatchDoubleMuModesBX0PlateauClone");
  SMRecoPtMatchDoubleMuModesClone->SetLineColor(1);//blk
  SMRecoPtMatchDoubleMuModesBX0Clone->SetLineColor(2);//red
  SMRecoPtMatchDoubleMuModesBX0dBX0Clone->SetLineColor(3);//green
  SMRecoPtMatchDoubleMuModesBX0PlateauClone->SetLineColor(4);//blue
  gStyle->SetOptStat(0);
  SMRecoPtMatchDoubleMuModesClone->Divide(SMRecoPt);
  SMRecoPtMatchDoubleMuModesBX0Clone->Divide(SMRecoPt);
  SMRecoPtMatchDoubleMuModesBX0dBX0Clone->Divide(SMRecoPt);
  SMRecoPtMatchDoubleMuModesBX0PlateauClone->Divide(SMRecoPt);
  SDoubleMuModes->Add(SMRecoPtMatchDoubleMuModesClone);
  SDoubleMuModes->Add(SMRecoPtMatchDoubleMuModesBX0Clone);
  SDoubleMuModes->Add(SMRecoPtMatchDoubleMuModesBX0dBX0Clone);
  SDoubleMuModes->Add(SMRecoPtMatchDoubleMuModesBX0PlateauClone);
  SDoubleMuModes->Draw("nostack");
  SDoubleMuModes->GetXaxis()->SetTitle("RECO pT[GeV]");
  SDoubleMuModes->GetYaxis()->SetTitle("Fraction of RECO muons");
  SDoubleMuModes->Modified();
        
  TLegend* LDoubleMuModes = new TLegend(0.6,0.6,0.9,0.9);
  TString LMatchDoubleMuModes = "";
  LMatchDoubleMuModes = LMatchDoubleMuModes + "RecoMu Unique Match";
  TString LMatchDoubleMuModesBX0 = "";
  LMatchDoubleMuModesBX0 = LMatchDoubleMuModesBX0 + "RecoMu Unique Match && TrkBX=0";
  TString LMatchDoubleMuModesBX0dBX0 = "";
  LMatchDoubleMuModesBX0dBX0 = LMatchDoubleMuModesBX0dBX0 + "RecoMu Unique Match && TrkBX=0 && TrkdBX=0";
  TString LMatchDoubleMuModesBX0Plateau = "";
  LMatchDoubleMuModesBX0Plateau = LMatchDoubleMuModesBX0Plateau + "RecoMu Unique Match && TrkBX=0 && Plateau";
  LDoubleMuModes->AddEntry(SMRecoPtMatchDoubleMuModesClone, LMatchDoubleMuModes);
  LDoubleMuModes->AddEntry(SMRecoPtMatchDoubleMuModesBX0Clone, LMatchDoubleMuModesBX0);
  LDoubleMuModes->AddEntry(SMRecoPtMatchDoubleMuModesBX0dBX0Clone, LMatchDoubleMuModesBX0dBX0);
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
  TH1F *SMRecoPtMatchMuOpenModesBX0dBX0Clone = (TH1F*)SMRecoPtMatchMuOpenModesBX0dBX0->Clone("SMRecoPtMatchMuOpenModesBX0dBX0Clone");
  TH1F *SMRecoPtMatchMuOpenModesBX0PlateauClone = (TH1F*)SMRecoPtMatchMuOpenModesBX0Plateau->Clone("SMRecoPtMatchMuOpenModesBX0PlateauClone");
  SMRecoPtMatchMuOpenModesClone->SetLineColor(1);//blk
  SMRecoPtMatchMuOpenModesBX0Clone->SetLineColor(2);//red
  SMRecoPtMatchMuOpenModesBX0dBX0Clone->SetLineColor(3);//green
  SMRecoPtMatchMuOpenModesBX0PlateauClone->SetLineColor(4);//blue
  gStyle->SetOptStat(0);
  SMRecoPtMatchMuOpenModesClone->Divide(SMRecoPt);
  SMRecoPtMatchMuOpenModesBX0Clone->Divide(SMRecoPt);
  SMRecoPtMatchMuOpenModesBX0dBX0Clone->Divide(SMRecoPt);
  SMRecoPtMatchMuOpenModesBX0PlateauClone->Divide(SMRecoPt);
  SMuOpenModes->Add(SMRecoPtMatchMuOpenModesClone);
  SMuOpenModes->Add(SMRecoPtMatchMuOpenModesBX0Clone);
  SMuOpenModes->Add(SMRecoPtMatchMuOpenModesBX0dBX0Clone);
  SMuOpenModes->Add(SMRecoPtMatchMuOpenModesBX0PlateauClone);
  SMuOpenModes->Draw("nostack");
  SMuOpenModes->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMuOpenModes->GetYaxis()->SetTitle("Fraction of RECO muons");
  SMuOpenModes->Modified();
        
  TLegend* LMuOpenModes = new TLegend(0.6,0.6,0.9,0.9);
  TString LMatchMuOpenModes = "";
  LMatchMuOpenModes = LMatchMuOpenModes + "RecoMu Unique Match";
  TString LMatchMuOpenModesBX0 = "";
  LMatchMuOpenModesBX0 = LMatchMuOpenModesBX0 + "RecoMu Unique Match && TrkBX=0";
  TString LMatchMuOpenModesBX0dBX0 = "";
  LMatchMuOpenModesBX0dBX0 = LMatchMuOpenModesBX0dBX0 + "RecoMu Unique Match && TrkBX=0 && TrkdBX=0";
  TString LMatchMuOpenModesBX0Plateau = "";
  LMatchMuOpenModesBX0Plateau = LMatchMuOpenModesBX0Plateau + "RecoMu Unique Match && TrkBX=0 && Plateau";
  LMuOpenModes->AddEntry(SMRecoPtMatchMuOpenModesClone, LMatchMuOpenModes);
  LMuOpenModes->AddEntry(SMRecoPtMatchMuOpenModesBX0Clone, LMatchMuOpenModesBX0);
  LMuOpenModes->AddEntry(SMRecoPtMatchMuOpenModesBX0dBX0Clone, LMatchMuOpenModesBX0dBX0);
  LMuOpenModes->AddEntry(SMRecoPtMatchMuOpenModesBX0PlateauClone, LMatchMuOpenModesBX0Plateau);
  LMuOpenModes->SetFillStyle(0);
  LMuOpenModes->SetBorderSize(0);
  LMuOpenModes->Draw(); 
  CMuOpenModes->Write();
  
  //each mode
  TCanvas *CMode15 = new TCanvas("CMode15","Mode15",700,500);
  THStack *SMode15 = new THStack("SMode15","Mode15");
  CMode15->cd();
  TH1F *SMRecoPtMatchMode15Clone = (TH1F*)SMRecoPtMatchMode15->Clone("SMRecoPtMatchMode15Clone");
  TH1F *SMRecoPtMatchMode15BX0Clone = (TH1F*)SMRecoPtMatchMode15BX0->Clone("SMRecoPtMatchMode15BX0Clone");
  TH1F *SMRecoPtMatchMode15BX0dBX0Clone = (TH1F*)SMRecoPtMatchMode15BX0dBX0->Clone("SMRecoPtMatchMode15BX0dBX0Clone");
  TH1F *SMRecoPtMatchMode15BX0PlateauClone = (TH1F*)SMRecoPtMatchMode15BX0Plateau->Clone("SMRecoPtMatchMode15BX0PlateauClone");
  SMRecoPtMatchMode15Clone->SetLineColor(1);//blk
  SMRecoPtMatchMode15BX0Clone->SetLineColor(2);//red
  SMRecoPtMatchMode15BX0dBX0Clone->SetLineColor(3);//green
  SMRecoPtMatchMode15BX0PlateauClone->SetLineColor(4);//blue
  gStyle->SetOptStat(0);
  SMRecoPtMatchMode15Clone->Divide(SMRecoPt);
  SMRecoPtMatchMode15BX0Clone->Divide(SMRecoPt);
  SMRecoPtMatchMode15BX0dBX0Clone->Divide(SMRecoPt);
  SMRecoPtMatchMode15BX0PlateauClone->Divide(SMRecoPt);
  SMode15->Add(SMRecoPtMatchMode15Clone);
  SMode15->Add(SMRecoPtMatchMode15BX0Clone);
  SMode15->Add(SMRecoPtMatchMode15BX0dBX0Clone);
  SMode15->Add(SMRecoPtMatchMode15BX0PlateauClone);
  SMode15->Draw("nostack");
  SMode15->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMode15->GetYaxis()->SetTitle("Fraction of RECO muons");
  CMode15->Modified();
        
  TLegend* LMode15 = new TLegend(0.6,0.6,0.9,0.9);
  TString LMatchMode15 = "";
  LMatchMode15 = LMatchMode15 + "RecoMu Unique Match";
  TString LMatchMode15BX0 = "";
  LMatchMode15BX0 = LMatchMode15BX0 + "RecoMu Unique Match && TrkBX=0";
  TString LMatchMode15BX0dBX0 = "";
  LMatchMode15BX0dBX0 = LMatchMode15BX0dBX0 + "RecoMu Unique Match && TrkBX=0 && TrkdBX=0";
  TString LMatchMode15BX0Plateau = "";
  LMatchMode15BX0Plateau = LMatchMode15BX0Plateau + "RecoMu Unique Match && TrkBX=0 && Plateau";
  LMode15->AddEntry(SMRecoPtMatchMode15Clone, LMatchMode15);
  LMode15->AddEntry(SMRecoPtMatchMode15BX0Clone, LMatchMode15BX0);
  LMode15->AddEntry(SMRecoPtMatchMode15BX0dBX0Clone, LMatchMode15BX0dBX0);
  LMode15->AddEntry(SMRecoPtMatchMode15BX0PlateauClone, LMatchMode15BX0Plateau);
  LMode15->SetFillStyle(0);
  LMode15->SetBorderSize(0);
  LMode15->Draw(); 
  CMode15->Write();

  TCanvas *CMode14 = new TCanvas("CMode14","Mode14",700,500);
  THStack *SMode14 = new THStack("SMode14","Mode14");
  CMode14->cd();
  TH1F *SMRecoPtMatchMode14Clone = (TH1F*)SMRecoPtMatchMode14->Clone("SMRecoPtMatchMode14Clone");
  TH1F *SMRecoPtMatchMode14BX0Clone = (TH1F*)SMRecoPtMatchMode14BX0->Clone("SMRecoPtMatchMode14BX0Clone");
  TH1F *SMRecoPtMatchMode14BX0dBX0Clone = (TH1F*)SMRecoPtMatchMode14BX0dBX0->Clone("SMRecoPtMatchMode14BX0dBX0Clone");
  TH1F *SMRecoPtMatchMode14BX0PlateauClone = (TH1F*)SMRecoPtMatchMode14BX0Plateau->Clone("SMRecoPtMatchMode14BX0PlateauClone");
  SMRecoPtMatchMode14Clone->SetLineColor(1);//blk
  SMRecoPtMatchMode14BX0Clone->SetLineColor(2);//red
  SMRecoPtMatchMode14BX0dBX0Clone->SetLineColor(3);//green
  SMRecoPtMatchMode14BX0PlateauClone->SetLineColor(4);//blue
  gStyle->SetOptStat(0);
  SMRecoPtMatchMode14Clone->Divide(SMRecoPt);
  SMRecoPtMatchMode14BX0Clone->Divide(SMRecoPt);
  SMRecoPtMatchMode14BX0dBX0Clone->Divide(SMRecoPt);
  SMRecoPtMatchMode14BX0PlateauClone->Divide(SMRecoPt);
  SMode14->Add(SMRecoPtMatchMode14Clone);
  SMode14->Add(SMRecoPtMatchMode14BX0Clone);
  SMode14->Add(SMRecoPtMatchMode14BX0dBX0Clone);
  SMode14->Add(SMRecoPtMatchMode14BX0PlateauClone);
  SMode14->Draw("nostack");
  SMode14->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMode14->GetYaxis()->SetTitle("Fraction of RECO muons");
  CMode14->Modified();
        
  TLegend* LMode14 = new TLegend(0.6,0.6,0.9,0.9);
  TString LMatchMode14 = "";
  LMatchMode14 = LMatchMode14 + "RecoMu Unique Match";
  TString LMatchMode14BX0 = "";
  LMatchMode14BX0 = LMatchMode14BX0 + "RecoMu Unique Match && TrkBX=0";
  TString LMatchMode14BX0dBX0 = "";
  LMatchMode14BX0dBX0 = LMatchMode14BX0dBX0 + "RecoMu Unique Match && TrkBX=0 && TrkdBX=0";
  TString LMatchMode14BX0Plateau = "";
  LMatchMode14BX0Plateau = LMatchMode14BX0Plateau + "RecoMu Unique Match && TrkBX=0 && Plateau";
  LMode14->AddEntry(SMRecoPtMatchMode14Clone, LMatchMode14);
  LMode14->AddEntry(SMRecoPtMatchMode14BX0Clone, LMatchMode14BX0);
  LMode14->AddEntry(SMRecoPtMatchMode14BX0dBX0Clone, LMatchMode14BX0dBX0);
  LMode14->AddEntry(SMRecoPtMatchMode14BX0PlateauClone, LMatchMode14BX0Plateau);
  LMode14->SetFillStyle(0);
  LMode14->SetBorderSize(0);
  LMode14->Draw(); 
  CMode14->Write();
	
  TCanvas *CMode13 = new TCanvas("CMode13","Mode13",700,500);
  THStack *SMode13 = new THStack("SMode13","Mode13");
  CMode13->cd();
  TH1F *SMRecoPtMatchMode13Clone = (TH1F*)SMRecoPtMatchMode13->Clone("SMRecoPtMatchMode13Clone");
  TH1F *SMRecoPtMatchMode13BX0Clone = (TH1F*)SMRecoPtMatchMode13BX0->Clone("SMRecoPtMatchMode13BX0Clone");
  TH1F *SMRecoPtMatchMode13BX0dBX0Clone = (TH1F*)SMRecoPtMatchMode13BX0dBX0->Clone("SMRecoPtMatchMode13BX0dBX0Clone");
  TH1F *SMRecoPtMatchMode13BX0PlateauClone = (TH1F*)SMRecoPtMatchMode13BX0Plateau->Clone("SMRecoPtMatchMode13BX0PlateauClone");
  SMRecoPtMatchMode13Clone->SetLineColor(1);//blk
  SMRecoPtMatchMode13BX0Clone->SetLineColor(2);//red
  SMRecoPtMatchMode13BX0dBX0Clone->SetLineColor(3);//green
  SMRecoPtMatchMode13BX0PlateauClone->SetLineColor(4);//blue
  gStyle->SetOptStat(0);
  SMRecoPtMatchMode13Clone->Divide(SMRecoPt);
  SMRecoPtMatchMode13BX0Clone->Divide(SMRecoPt);
  SMRecoPtMatchMode13BX0dBX0Clone->Divide(SMRecoPt);
  SMRecoPtMatchMode13BX0PlateauClone->Divide(SMRecoPt);
  SMode13->Add(SMRecoPtMatchMode13Clone);
  SMode13->Add(SMRecoPtMatchMode13BX0Clone);
  SMode13->Add(SMRecoPtMatchMode13BX0dBX0Clone);
  SMode13->Add(SMRecoPtMatchMode13BX0PlateauClone);
  SMode13->Draw("nostack");
  SMode13->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMode13->GetYaxis()->SetTitle("Fraction of RECO muons");
  CMode13->Modified();
        
  TLegend* LMode13 = new TLegend(0.6,0.6,0.9,0.9);
  TString LMatchMode13 = "";
  LMatchMode13 = LMatchMode13 + "RecoMu Unique Match";
  TString LMatchMode13BX0 = "";
  LMatchMode13BX0 = LMatchMode13BX0 + "RecoMu Unique Match && TrkBX=0";
  TString LMatchMode13BX0dBX0 = "";
  LMatchMode13BX0dBX0 = LMatchMode13BX0dBX0 + "RecoMu Unique Match && TrkBX=0 && TrkdBX=0";
  TString LMatchMode13BX0Plateau = "";
  LMatchMode13BX0Plateau = LMatchMode13BX0Plateau + "RecoMu Unique Match && TrkBX=0 && Plateau";
  LMode13->AddEntry(SMRecoPtMatchMode13Clone, LMatchMode13);
  LMode13->AddEntry(SMRecoPtMatchMode13BX0Clone, LMatchMode13BX0);
  LMode13->AddEntry(SMRecoPtMatchMode13BX0dBX0Clone, LMatchMode13BX0dBX0);
  LMode13->AddEntry(SMRecoPtMatchMode13BX0PlateauClone, LMatchMode13BX0Plateau);
  LMode13->SetFillStyle(0);
  LMode13->SetBorderSize(0);
  LMode13->Draw(); 
  CMode13->Write();

  TCanvas *CMode11 = new TCanvas("CMode11","Mode11",700,500);
  THStack *SMode11 = new THStack("SMode11","Mode11");
  CMode11->cd();
  TH1F *SMRecoPtMatchMode11Clone = (TH1F*)SMRecoPtMatchMode11->Clone("SMRecoPtMatchMode11Clone");
  TH1F *SMRecoPtMatchMode11BX0Clone = (TH1F*)SMRecoPtMatchMode11BX0->Clone("SMRecoPtMatchMode11BX0Clone");
  TH1F *SMRecoPtMatchMode11BX0dBX0Clone = (TH1F*)SMRecoPtMatchMode11BX0dBX0->Clone("SMRecoPtMatchMode11BX0dBX0Clone");
  TH1F *SMRecoPtMatchMode11BX0PlateauClone = (TH1F*)SMRecoPtMatchMode11BX0Plateau->Clone("SMRecoPtMatchMode11BX0PlateauClone");
  SMRecoPtMatchMode11Clone->SetLineColor(1);//blk
  SMRecoPtMatchMode11BX0Clone->SetLineColor(2);//red
  SMRecoPtMatchMode11BX0dBX0Clone->SetLineColor(3);//green
  SMRecoPtMatchMode11BX0PlateauClone->SetLineColor(4);//blue
  gStyle->SetOptStat(0);
  SMRecoPtMatchMode11Clone->Divide(SMRecoPt);
  SMRecoPtMatchMode11BX0Clone->Divide(SMRecoPt);
  SMRecoPtMatchMode11BX0dBX0Clone->Divide(SMRecoPt);
  SMRecoPtMatchMode11BX0PlateauClone->Divide(SMRecoPt);
  SMode11->Add(SMRecoPtMatchMode11Clone);
  SMode11->Add(SMRecoPtMatchMode11BX0Clone);
  SMode11->Add(SMRecoPtMatchMode11BX0dBX0Clone);
  SMode11->Add(SMRecoPtMatchMode11BX0PlateauClone);
  SMode11->Draw("nostack");
  SMode11->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMode11->GetYaxis()->SetTitle("Fraction of RECO muons");
  CMode11->Modified();
        
  TLegend* LMode11 = new TLegend(0.6,0.6,0.9,0.9);
  TString LMatchMode11 = "";
  LMatchMode11 = LMatchMode11 + "RecoMu Unique Match";
  TString LMatchMode11BX0 = "";
  LMatchMode11BX0 = LMatchMode11BX0 + "RecoMu Unique Match && TrkBX=0";
  TString LMatchMode11BX0dBX0 = "";
  LMatchMode11BX0dBX0 = LMatchMode11BX0dBX0 + "RecoMu Unique Match && TrkBX=0 && TrkdBX=0";
  TString LMatchMode11BX0Plateau = "";
  LMatchMode11BX0Plateau = LMatchMode11BX0Plateau + "RecoMu Unique Match && TrkBX=0 && Plateau";
  LMode11->AddEntry(SMRecoPtMatchMode11Clone, LMatchMode11);
  LMode11->AddEntry(SMRecoPtMatchMode11BX0Clone, LMatchMode11BX0);
  LMode11->AddEntry(SMRecoPtMatchMode11BX0dBX0Clone, LMatchMode11BX0dBX0);
  LMode11->AddEntry(SMRecoPtMatchMode11BX0PlateauClone, LMatchMode11BX0Plateau);
  LMode11->SetFillStyle(0);
  LMode11->SetBorderSize(0);
  LMode11->Draw(); 
  CMode11->Write();
	
  TCanvas *CMode12 = new TCanvas("CMode12","Mode12",700,500);
  THStack *SMode12 = new THStack("SMode12","Mode12");
  CMode12->cd();
  TH1F *SMRecoPtMatchMode12Clone = (TH1F*)SMRecoPtMatchMode12->Clone("SMRecoPtMatchMode12Clone");
  TH1F *SMRecoPtMatchMode12BX0Clone = (TH1F*)SMRecoPtMatchMode12BX0->Clone("SMRecoPtMatchMode12BX0Clone");
  TH1F *SMRecoPtMatchMode12BX0dBX0Clone = (TH1F*)SMRecoPtMatchMode12BX0dBX0->Clone("SMRecoPtMatchMode12BX0dBX0Clone");
  TH1F *SMRecoPtMatchMode12BX0PlateauClone = (TH1F*)SMRecoPtMatchMode12BX0Plateau->Clone("SMRecoPtMatchMode12BX0PlateauClone");
  SMRecoPtMatchMode12Clone->SetLineColor(1);//blk
  SMRecoPtMatchMode12BX0Clone->SetLineColor(2);//red
  SMRecoPtMatchMode12BX0dBX0Clone->SetLineColor(3);//green
  SMRecoPtMatchMode12BX0PlateauClone->SetLineColor(4);//blue
  gStyle->SetOptStat(0);
  SMRecoPtMatchMode12Clone->Divide(SMRecoPt);
  SMRecoPtMatchMode12BX0Clone->Divide(SMRecoPt);
  SMRecoPtMatchMode12BX0dBX0Clone->Divide(SMRecoPt);
  SMRecoPtMatchMode12BX0PlateauClone->Divide(SMRecoPt);
  SMode12->Add(SMRecoPtMatchMode12Clone);
  SMode12->Add(SMRecoPtMatchMode12BX0Clone);
  SMode12->Add(SMRecoPtMatchMode12BX0dBX0Clone);
  SMode12->Add(SMRecoPtMatchMode12BX0PlateauClone);
  SMode12->Draw("nostack");
  SMode12->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMode12->GetYaxis()->SetTitle("Fraction of RECO muons");
  CMode12->Modified();
        
  TLegend* LMode12 = new TLegend(0.6,0.6,0.9,0.9);
  TString LMatchMode12 = "";
  LMatchMode12 = LMatchMode12 + "RecoMu Unique Match";
  TString LMatchMode12BX0 = "";
  LMatchMode12BX0 = LMatchMode12BX0 + "RecoMu Unique Match && TrkBX=0";
  TString LMatchMode12BX0dBX0 = "";
  LMatchMode12BX0dBX0 = LMatchMode12BX0dBX0 + "RecoMu Unique Match && TrkBX=0 && TrkdBX=0";
  TString LMatchMode12BX0Plateau = "";
  LMatchMode12BX0Plateau = LMatchMode12BX0Plateau + "RecoMu Unique Match && TrkBX=0 && Plateau";
  LMode12->AddEntry(SMRecoPtMatchMode12Clone, LMatchMode12);
  LMode12->AddEntry(SMRecoPtMatchMode12BX0Clone, LMatchMode12BX0);
  LMode12->AddEntry(SMRecoPtMatchMode12BX0dBX0Clone, LMatchMode12BX0dBX0);
  LMode12->AddEntry(SMRecoPtMatchMode12BX0PlateauClone, LMatchMode12BX0Plateau);
  LMode12->SetFillStyle(0);
  LMode12->SetBorderSize(0);
  LMode12->Draw(); 
  CMode12->Write();

  TCanvas *CMode10 = new TCanvas("CMode10","Mode10",700,500);
  THStack *SMode10 = new THStack("SMode10","Mode10");
  CMode10->cd();
  TH1F *SMRecoPtMatchMode10Clone = (TH1F*)SMRecoPtMatchMode10->Clone("SMRecoPtMatchMode10Clone");
  TH1F *SMRecoPtMatchMode10BX0Clone = (TH1F*)SMRecoPtMatchMode10BX0->Clone("SMRecoPtMatchMode10BX0Clone");
  TH1F *SMRecoPtMatchMode10BX0dBX0Clone = (TH1F*)SMRecoPtMatchMode10BX0dBX0->Clone("SMRecoPtMatchMode10BX0dBX0Clone");
  TH1F *SMRecoPtMatchMode10BX0PlateauClone = (TH1F*)SMRecoPtMatchMode10BX0Plateau->Clone("SMRecoPtMatchMode10BX0PlateauClone");
  SMRecoPtMatchMode10Clone->SetLineColor(1);//blk
  SMRecoPtMatchMode10BX0Clone->SetLineColor(2);//red
  SMRecoPtMatchMode10BX0dBX0Clone->SetLineColor(3);//green
  SMRecoPtMatchMode10BX0PlateauClone->SetLineColor(4);//blue
  gStyle->SetOptStat(0);
  SMRecoPtMatchMode10Clone->Divide(SMRecoPt);
  SMRecoPtMatchMode10BX0Clone->Divide(SMRecoPt);
  SMRecoPtMatchMode10BX0dBX0Clone->Divide(SMRecoPt);
  SMRecoPtMatchMode10BX0PlateauClone->Divide(SMRecoPt);
  SMode10->Add(SMRecoPtMatchMode10Clone);
  SMode10->Add(SMRecoPtMatchMode10BX0Clone);
  SMode10->Add(SMRecoPtMatchMode10BX0dBX0Clone);
  SMode10->Add(SMRecoPtMatchMode10BX0PlateauClone);
  SMode10->Draw("nostack");
  SMode10->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMode10->GetYaxis()->SetTitle("Fraction of RECO muons");
  CMode10->Modified();
        
  TLegend* LMode10 = new TLegend(0.6,0.6,0.9,0.9);
  TString LMatchMode10 = "";
  LMatchMode10 = LMatchMode10 + "RecoMu Unique Match";
  TString LMatchMode10BX0 = "";
  LMatchMode10BX0 = LMatchMode10BX0 + "RecoMu Unique Match && TrkBX=0";
  TString LMatchMode10BX0dBX0 = "";
  LMatchMode10BX0dBX0 = LMatchMode10BX0dBX0 + "RecoMu Unique Match && TrkBX=0 && TrkdBX=0";
  TString LMatchMode10BX0Plateau = "";
  LMatchMode10BX0Plateau = LMatchMode10BX0Plateau + "RecoMu Unique Match && TrkBX=0 && Plateau";
  LMode10->AddEntry(SMRecoPtMatchMode10Clone, LMatchMode10);
  LMode10->AddEntry(SMRecoPtMatchMode10BX0Clone, LMatchMode10BX0);
  LMode10->AddEntry(SMRecoPtMatchMode10BX0dBX0Clone, LMatchMode10BX0dBX0);
  LMode10->AddEntry(SMRecoPtMatchMode10BX0PlateauClone, LMatchMode10BX0Plateau);
  LMode10->SetFillStyle(0);
  LMode10->SetBorderSize(0);
  LMode10->Draw(); 
  CMode10->Write();
	
  TCanvas *CMode7 = new TCanvas("CMode7","Mode7",700,500);
  THStack *SMode7 = new THStack("SMode7","Mode7");
  CMode7->cd();
  TH1F *SMRecoPtMatchMode7Clone = (TH1F*)SMRecoPtMatchMode7->Clone("SMRecoPtMatchMode7Clone");
  TH1F *SMRecoPtMatchMode7BX0Clone = (TH1F*)SMRecoPtMatchMode7BX0->Clone("SMRecoPtMatchMode7BX0Clone");
  TH1F *SMRecoPtMatchMode7BX0dBX0Clone = (TH1F*)SMRecoPtMatchMode7BX0dBX0->Clone("SMRecoPtMatchMode7BX0dBX0Clone");
  TH1F *SMRecoPtMatchMode7BX0PlateauClone = (TH1F*)SMRecoPtMatchMode7BX0Plateau->Clone("SMRecoPtMatchMode7BX0PlateauClone");
  SMRecoPtMatchMode7Clone->SetLineColor(1);//blk
  SMRecoPtMatchMode7BX0Clone->SetLineColor(2);//red
  SMRecoPtMatchMode7BX0dBX0Clone->SetLineColor(3);//green
  SMRecoPtMatchMode7BX0PlateauClone->SetLineColor(4);//blue
  gStyle->SetOptStat(0);
  SMRecoPtMatchMode7Clone->Divide(SMRecoPt);
  SMRecoPtMatchMode7BX0Clone->Divide(SMRecoPt);
  SMRecoPtMatchMode7BX0dBX0Clone->Divide(SMRecoPt);
  SMRecoPtMatchMode7BX0PlateauClone->Divide(SMRecoPt);
  SMode7->Add(SMRecoPtMatchMode7Clone);
  SMode7->Add(SMRecoPtMatchMode7BX0Clone);
  SMode7->Add(SMRecoPtMatchMode7BX0dBX0Clone);
  SMode7->Add(SMRecoPtMatchMode7BX0PlateauClone);
  SMode7->Draw("nostack");
  SMode7->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMode7->GetYaxis()->SetTitle("Fraction of RECO muons");
  CMode7->Modified();
        
  TLegend* LMode7 = new TLegend(0.6,0.6,0.9,0.9);
  TString LMatchMode7 = "";
  LMatchMode7 = LMatchMode7 + "RecoMu Unique Match";
  TString LMatchMode7BX0 = "";
  LMatchMode7BX0 = LMatchMode7BX0 + "RecoMu Unique Match && TrkBX=0";
  TString LMatchMode7BX0dBX0 = "";
  LMatchMode7BX0dBX0 = LMatchMode7BX0dBX0 + "RecoMu Unique Match && TrkBX=0 && TrkdBX=0";
  TString LMatchMode7BX0Plateau = "";
  LMatchMode7BX0Plateau = LMatchMode7BX0Plateau + "RecoMu Unique Match && TrkBX=0 && Plateau";
  LMode7->AddEntry(SMRecoPtMatchMode7Clone, LMatchMode7);
  LMode7->AddEntry(SMRecoPtMatchMode7BX0Clone, LMatchMode7BX0);
  LMode7->AddEntry(SMRecoPtMatchMode7BX0dBX0Clone, LMatchMode7BX0dBX0);
  LMode7->AddEntry(SMRecoPtMatchMode7BX0PlateauClone, LMatchMode7BX0Plateau);
  LMode7->SetFillStyle(0);
  LMode7->SetBorderSize(0);
  LMode7->Draw(); 
  CMode7->Write();
	
  TCanvas *CMode9 = new TCanvas("CMode9","Mode9",700,500);
  THStack *SMode9 = new THStack("SMode9","Mode9");
  CMode9->cd();
  TH1F *SMRecoPtMatchMode9Clone = (TH1F*)SMRecoPtMatchMode9->Clone("SMRecoPtMatchMode9Clone");
  TH1F *SMRecoPtMatchMode9BX0Clone = (TH1F*)SMRecoPtMatchMode9BX0->Clone("SMRecoPtMatchMode9BX0Clone");
  TH1F *SMRecoPtMatchMode9BX0dBX0Clone = (TH1F*)SMRecoPtMatchMode9BX0dBX0->Clone("SMRecoPtMatchMode9BX0dBX0Clone");
  TH1F *SMRecoPtMatchMode9BX0PlateauClone = (TH1F*)SMRecoPtMatchMode9BX0Plateau->Clone("SMRecoPtMatchMode9BX0PlateauClone");
  SMRecoPtMatchMode9Clone->SetLineColor(1);//blk
  SMRecoPtMatchMode9BX0Clone->SetLineColor(2);//red
  SMRecoPtMatchMode9BX0dBX0Clone->SetLineColor(3);//green
  SMRecoPtMatchMode9BX0PlateauClone->SetLineColor(4);//blue
  gStyle->SetOptStat(0);
  SMRecoPtMatchMode9Clone->Divide(SMRecoPt);
  SMRecoPtMatchMode9BX0Clone->Divide(SMRecoPt);
  SMRecoPtMatchMode9BX0dBX0Clone->Divide(SMRecoPt);
  SMRecoPtMatchMode9BX0PlateauClone->Divide(SMRecoPt);
  SMode9->Add(SMRecoPtMatchMode9Clone);
  SMode9->Add(SMRecoPtMatchMode9BX0Clone);
  SMode9->Add(SMRecoPtMatchMode9BX0dBX0Clone);
  SMode9->Add(SMRecoPtMatchMode9BX0PlateauClone);
  SMode9->Draw("nostack");
  SMode9->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMode9->GetYaxis()->SetTitle("Fraction of RECO muons");
  CMode9->Modified();
        
  TLegend* LMode9 = new TLegend(0.6,0.6,0.9,0.9);
  TString LMatchMode9 = "";
  LMatchMode9 = LMatchMode9 + "RecoMu Unique Match";
  TString LMatchMode9BX0 = "";
  LMatchMode9BX0 = LMatchMode9BX0 + "RecoMu Unique Match && TrkBX=0";
  TString LMatchMode9BX0dBX0 = "";
  LMatchMode9BX0dBX0 = LMatchMode9BX0dBX0 + "RecoMu Unique Match && TrkBX=0 && TrkdBX=0";
  TString LMatchMode9BX0Plateau = "";
  LMatchMode9BX0Plateau = LMatchMode9BX0Plateau + "RecoMu Unique Match && TrkBX=0 && Plateau";
  LMode9->AddEntry(SMRecoPtMatchMode9Clone, LMatchMode9);
  LMode9->AddEntry(SMRecoPtMatchMode9BX0Clone, LMatchMode9BX0);
  LMode9->AddEntry(SMRecoPtMatchMode9BX0dBX0Clone, LMatchMode9BX0dBX0);
  LMode9->AddEntry(SMRecoPtMatchMode9BX0PlateauClone, LMatchMode9BX0Plateau);
  LMode9->SetFillStyle(0);
  LMode9->SetBorderSize(0);
  LMode9->Draw(); 
  CMode9->Write();
	
  TCanvas *CMode6 = new TCanvas("CMode6","Mode6",700,500);
  THStack *SMode6 = new THStack("SMode6","Mode6");
  CMode6->cd();
  TH1F *SMRecoPtMatchMode6Clone = (TH1F*)SMRecoPtMatchMode6->Clone("SMRecoPtMatchMode6Clone");
  TH1F *SMRecoPtMatchMode6BX0Clone = (TH1F*)SMRecoPtMatchMode6BX0->Clone("SMRecoPtMatchMode6BX0Clone");
  TH1F *SMRecoPtMatchMode6BX0dBX0Clone = (TH1F*)SMRecoPtMatchMode6BX0dBX0->Clone("SMRecoPtMatchMode6BX0dBX0Clone");
  TH1F *SMRecoPtMatchMode6BX0PlateauClone = (TH1F*)SMRecoPtMatchMode6BX0Plateau->Clone("SMRecoPtMatchMode6BX0PlateauClone");
  SMRecoPtMatchMode6Clone->SetLineColor(1);//blk
  SMRecoPtMatchMode6BX0Clone->SetLineColor(2);//red
  SMRecoPtMatchMode6BX0dBX0Clone->SetLineColor(3);//green
  SMRecoPtMatchMode6BX0PlateauClone->SetLineColor(4);//blue
  gStyle->SetOptStat(0);
  SMRecoPtMatchMode6Clone->Divide(SMRecoPt);
  SMRecoPtMatchMode6BX0Clone->Divide(SMRecoPt);
  SMRecoPtMatchMode6BX0dBX0Clone->Divide(SMRecoPt);
  SMRecoPtMatchMode6BX0PlateauClone->Divide(SMRecoPt);
  SMode6->Add(SMRecoPtMatchMode6Clone);
  SMode6->Add(SMRecoPtMatchMode6BX0Clone);
  SMode6->Add(SMRecoPtMatchMode6BX0dBX0Clone);
  SMode6->Add(SMRecoPtMatchMode6BX0PlateauClone);
  SMode6->Draw("nostack");
  SMode6->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMode6->GetYaxis()->SetTitle("Fraction of RECO muons");
  CMode6->Modified();
        
  TLegend* LMode6 = new TLegend(0.6,0.6,0.9,0.9);
  TString LMatchMode6 = "";
  LMatchMode6 = LMatchMode6 + "RecoMu Unique Match";
  TString LMatchMode6BX0 = "";
  LMatchMode6BX0 = LMatchMode6BX0 + "RecoMu Unique Match && TrkBX=0";
  TString LMatchMode6BX0dBX0 = "";
  LMatchMode6BX0dBX0 = LMatchMode6BX0dBX0 + "RecoMu Unique Match && TrkBX=0 && TrkdBX=0";
  TString LMatchMode6BX0Plateau = "";
  LMatchMode6BX0Plateau = LMatchMode6BX0Plateau + "RecoMu Unique Match && TrkBX=0 && Plateau";
  LMode6->AddEntry(SMRecoPtMatchMode6Clone, LMatchMode6);
  LMode6->AddEntry(SMRecoPtMatchMode6BX0Clone, LMatchMode6BX0);
  LMode6->AddEntry(SMRecoPtMatchMode6BX0dBX0Clone, LMatchMode6BX0dBX0);
  LMode6->AddEntry(SMRecoPtMatchMode6BX0PlateauClone, LMatchMode6BX0Plateau);
  LMode6->SetFillStyle(0);
  LMode6->SetBorderSize(0);
  LMode6->Draw(); 
  CMode6->Write();
	
  TCanvas *CMode5 = new TCanvas("CMode5","Mode5",700,500);
  THStack *SMode5 = new THStack("SMode5","Mode5");
  CMode5->cd();
  TH1F *SMRecoPtMatchMode5Clone = (TH1F*)SMRecoPtMatchMode5->Clone("SMRecoPtMatchMode5Clone");
  TH1F *SMRecoPtMatchMode5BX0Clone = (TH1F*)SMRecoPtMatchMode5BX0->Clone("SMRecoPtMatchMode5BX0Clone");
  TH1F *SMRecoPtMatchMode5BX0dBX0Clone = (TH1F*)SMRecoPtMatchMode5BX0dBX0->Clone("SMRecoPtMatchMode5BX0dBX0Clone");
  TH1F *SMRecoPtMatchMode5BX0PlateauClone = (TH1F*)SMRecoPtMatchMode5BX0Plateau->Clone("SMRecoPtMatchMode5BX0PlateauClone");
  SMRecoPtMatchMode5Clone->SetLineColor(1);//blk
  SMRecoPtMatchMode5BX0Clone->SetLineColor(2);//red
  SMRecoPtMatchMode5BX0dBX0Clone->SetLineColor(3);//green
  SMRecoPtMatchMode5BX0PlateauClone->SetLineColor(4);//blue
  gStyle->SetOptStat(0);
  SMRecoPtMatchMode5Clone->Divide(SMRecoPt);
  SMRecoPtMatchMode5BX0Clone->Divide(SMRecoPt);
  SMRecoPtMatchMode5BX0dBX0Clone->Divide(SMRecoPt);
  SMRecoPtMatchMode5BX0PlateauClone->Divide(SMRecoPt);
  SMode5->Add(SMRecoPtMatchMode5Clone);
  SMode5->Add(SMRecoPtMatchMode5BX0Clone);
  SMode5->Add(SMRecoPtMatchMode5BX0dBX0Clone);
  SMode5->Add(SMRecoPtMatchMode5BX0PlateauClone);
  SMode5->Draw("nostack");
  SMode5->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMode5->GetYaxis()->SetTitle("Fraction of RECO muons");
  CMode5->Modified();
        
  TLegend* LMode5 = new TLegend(0.6,0.6,0.9,0.9);
  TString LMatchMode5 = "";
  LMatchMode5 = LMatchMode5 + "RecoMu Unique Match";
  TString LMatchMode5BX0 = "";
  LMatchMode5BX0 = LMatchMode5BX0 + "RecoMu Unique Match && TrkBX=0";
  TString LMatchMode5BX0dBX0 = "";
  LMatchMode5BX0dBX0 = LMatchMode5BX0dBX0 + "RecoMu Unique Match && TrkBX=0 && TrkdBX=0";
  TString LMatchMode5BX0Plateau = "";
  LMatchMode5BX0Plateau = LMatchMode5BX0Plateau + "RecoMu Unique Match && TrkBX=0 && Plateau";
  LMode5->AddEntry(SMRecoPtMatchMode5Clone, LMatchMode5);
  LMode5->AddEntry(SMRecoPtMatchMode5BX0Clone, LMatchMode5BX0);
  LMode5->AddEntry(SMRecoPtMatchMode5BX0dBX0Clone, LMatchMode5BX0dBX0);
  LMode5->AddEntry(SMRecoPtMatchMode5BX0PlateauClone, LMatchMode5BX0Plateau);
  LMode5->SetFillStyle(0);
  LMode5->SetBorderSize(0);
  LMode5->Draw(); 
  CMode5->Write();
	
  TCanvas *CMode3 = new TCanvas("CMode3","Mode3",700,500);
  THStack *SMode3 = new THStack("SMode3","Mode3");
  CMode3->cd();
  TH1F *SMRecoPtMatchMode3Clone = (TH1F*)SMRecoPtMatchMode3->Clone("SMRecoPtMatchMode3Clone");
  TH1F *SMRecoPtMatchMode3BX0Clone = (TH1F*)SMRecoPtMatchMode3BX0->Clone("SMRecoPtMatchMode3BX0Clone");
  TH1F *SMRecoPtMatchMode3BX0dBX0Clone = (TH1F*)SMRecoPtMatchMode3BX0dBX0->Clone("SMRecoPtMatchMode3BX0dBX0Clone");
  TH1F *SMRecoPtMatchMode3BX0PlateauClone = (TH1F*)SMRecoPtMatchMode3BX0Plateau->Clone("SMRecoPtMatchMode3BX0PlateauClone");
  SMRecoPtMatchMode3Clone->SetLineColor(1);//blk
  SMRecoPtMatchMode3BX0Clone->SetLineColor(2);//red
  SMRecoPtMatchMode3BX0dBX0Clone->SetLineColor(3);//green
  SMRecoPtMatchMode3BX0PlateauClone->SetLineColor(4);//blue
  gStyle->SetOptStat(0);
  SMRecoPtMatchMode3Clone->Divide(SMRecoPt);
  SMRecoPtMatchMode3BX0Clone->Divide(SMRecoPt);
  SMRecoPtMatchMode3BX0dBX0Clone->Divide(SMRecoPt);
  SMRecoPtMatchMode3BX0PlateauClone->Divide(SMRecoPt);
  SMode3->Add(SMRecoPtMatchMode3Clone);
  SMode3->Add(SMRecoPtMatchMode3BX0Clone);
  SMode3->Add(SMRecoPtMatchMode3BX0dBX0Clone);
  SMode3->Add(SMRecoPtMatchMode3BX0PlateauClone);
  SMode3->Draw("nostack");
  SMode3->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMode3->GetYaxis()->SetTitle("Fraction of RECO muons");
  CMode3->Modified();
        
  TLegend* LMode3 = new TLegend(0.6,0.6,0.9,0.9);
  TString LMatchMode3 = "";
  LMatchMode3 = LMatchMode3 + "RecoMu Unique Match";
  TString LMatchMode3BX0 = "";
  LMatchMode3BX0 = LMatchMode3BX0 + "RecoMu Unique Match && TrkBX=0";
  TString LMatchMode3BX0dBX0 = "";
  LMatchMode3BX0dBX0 = LMatchMode3BX0dBX0 + "RecoMu Unique Match && TrkBX=0 && TrkdBX=0";
  TString LMatchMode3BX0Plateau = "";
  LMatchMode3BX0Plateau = LMatchMode3BX0Plateau + "RecoMu Unique Match && TrkBX=0 && Plateau";
  LMode3->AddEntry(SMRecoPtMatchMode3Clone, LMatchMode3);
  LMode3->AddEntry(SMRecoPtMatchMode3BX0Clone, LMatchMode3BX0);
  LMode3->AddEntry(SMRecoPtMatchMode3BX0dBX0Clone, LMatchMode3BX0dBX0);
  LMode3->AddEntry(SMRecoPtMatchMode3BX0PlateauClone, LMatchMode3BX0Plateau);
  LMode3->SetFillStyle(0);
  LMode3->SetBorderSize(0);
  LMode3->Draw(); 
  CMode3->Write();
	
  //divide histograms for eff
  TCanvas *CBadMatchEff = new TCanvas("CBadMatchEff","CBadMatchEff",700,500);
  THStack *SBadMatchEff = new THStack("SBadMatchEff","SBadMatchEff");
  CBadMatchEff->cd();
  TH1F *SMRecoPtNoMatchClone = (TH1F*)SMRecoPtNoMatch->Clone("SMRecoPtNoMatchClone");
  TH1F *SMRecoPtNoUniqueMatchClone = (TH1F*)SMRecoPtNoUniqueMatch->Clone("SMRecoPtNoUniqueMatchClone");
  TH1F *SMRecoPtUniqueMatchClone = (TH1F*)SMRecoPtUniqueMatch->Clone("SMRecoPtUniqueMatchClone");
  TH1F *SMRecoPtUniqueMatchBX0Clone = (TH1F*)SMRecoPtUniqueMatchBX0->Clone("SMRecoPtUniqueMatchBX0Clone");
  SMRecoPtNoMatchClone->SetLineColor(1);//red
  SMRecoPtNoUniqueMatchClone->SetLineColor(2);//blk
  SMRecoPtUniqueMatchClone->SetLineColor(3);//green
  SMRecoPtUniqueMatchBX0Clone->SetLineColor(4);//blue
  gStyle->SetOptStat(0);
  SMRecoPtNoMatchClone->Divide(SMRecoPt);
  SMRecoPtNoUniqueMatchClone->Divide(SMRecoPt);
  SMRecoPtUniqueMatchClone->Divide(SMRecoPt);
  SMRecoPtUniqueMatchBX0Clone->Divide(SMRecoPt);
  SBadMatchEff->Add(SMRecoPtNoMatchClone);
  SBadMatchEff->Add(SMRecoPtNoUniqueMatchClone);
  SBadMatchEff->Add(SMRecoPtUniqueMatchClone);
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
  TString LUniqueMatch = "";
  LUniqueMatch = LUniqueMatch + "RecoMu Unique Match";
  TString LUniqueMatchBX0 = "";
  LUniqueMatchBX0 = LUniqueMatchBX0 + "RecoMu Unique Match && trk BX=0";
  LBadMatchEff->AddEntry(SMRecoPtNoMatchClone, LNoMatch);
  LBadMatchEff->AddEntry(SMRecoPtNoUniqueMatchClone, LNoUniqueMatch);
  LBadMatchEff->AddEntry(SMRecoPtUniqueMatchClone, LUniqueMatch);
  LBadMatchEff->AddEntry(SMRecoPtUniqueMatchBX0Clone, LUniqueMatchBX0);
  LBadMatchEff->SetFillStyle(0);
  LBadMatchEff->SetBorderSize(0);
  LBadMatchEff->Draw(); 
  CBadMatchEff->Write();

  //Rate plots from ZB data
  TGraph *Rate = new TGraph(30, TrigPT, Count); Rate->SetTitle("Number of tracks above threshold"); Rate->SetLineColor(1);
  TGraph *RatedBX0 = new TGraph(30, TrigPT, CountdBX0); RatedBX0->SetTitle("Rate dBX0"); RatedBX0->SetLineColor(2);
  TGraph *RateRecoSoft = new TGraph(30, TrigPT, CountRecoSoft); RateRecoSoft->SetTitle("Rate RECO + Soft"); RateRecoSoft->SetLineColor(3);
  TGraph *RateRecoOnly = new TGraph(30, TrigPT, CountRecoOnly); RateRecoOnly->SetTitle("Rate RECO + Only"); RateRecoOnly->SetLineColor(4);
  //SingleMu only
  TGraph *RateSingleMuModes = new TGraph(30, TrigPT, CountSingleMuModes); RateSingleMuModes->SetTitle("Rate SingleMuModes");
  TGraph *RateSingleMuModesdBX0 = new TGraph(30, TrigPT, CountSingleMuModesdBX0); RateSingleMuModesdBX0->SetTitle("Rate SingleMuModes dBX0");
  TGraph *RateSingleMuModesRecoSoft = new TGraph(30, TrigPT, CountSingleMuModesRecoSoft); RateSingleMuModesRecoSoft->SetTitle("Rate SingleMuModes RECO + Soft");
  TGraph *RateSingleMuModesRecoOnly = new TGraph(30, TrigPT, CountSingleMuModesRecoOnly); RateSingleMuModesRecoOnly->SetTitle("Rate SingleMuModes RECO + Only");
  //DoubleMu only
  TGraph *RateDoubleMuModes = new TGraph(30, TrigPT, CountDoubleMuModes); RateDoubleMuModes->SetTitle("Rate DoubleMuModes");
  TGraph *RateDoubleMuModesdBX0 = new TGraph(30, TrigPT, CountDoubleMuModesdBX0); RateDoubleMuModesdBX0->SetTitle("Rate DoubleMuModes dBX0");
  TGraph *RateDoubleMuModesRecoSoft = new TGraph(30, TrigPT, CountDoubleMuModesRecoSoft); RateDoubleMuModesRecoSoft->SetTitle("Rate DoubleMuModes RECO + Soft");
  TGraph *RateDoubleMuModesRecoOnly = new TGraph(30, TrigPT, CountDoubleMuModesRecoOnly); RateDoubleMuModesRecoOnly->SetTitle("Rate DoubleMuModes RECO + Only");
  //MuOpen only
  TGraph *RateMuOpenModes = new TGraph(30, TrigPT, CountMuOpenModes); RateMuOpenModes->SetTitle("Rate MuOpenModes");
  TGraph *RateMuOpenModesdBX0 = new TGraph(30, TrigPT, CountMuOpenModesdBX0); RateMuOpenModesdBX0->SetTitle("Rate MuOpenModes dBX0");
  TGraph *RateMuOpenModesRecoSoft = new TGraph(30, TrigPT, CountMuOpenModesRecoSoft); RateMuOpenModesRecoSoft->SetTitle("Rate MuOpenModes RECO + Soft");
  TGraph *RateMuOpenModesRecoOnly = new TGraph(30, TrigPT, CountMuOpenModesRecoOnly); RateMuOpenModesRecoOnly->SetTitle("Rate MuOpenModes RECO + Only");
	
  TGraph *RateMode15 = new TGraph(30, TrigPT, CountMode15); RateMode15->SetTitle("Rate Mode15");
  TGraph *RateMode15dBX0 = new TGraph(30, TrigPT, CountMode15dBX0); RateMode15dBX0->SetTitle("Rate Mode15 dBX0");
  TGraph *RateMode15RecoSoft = new TGraph(30, TrigPT, CountMode15RecoSoft); RateMode15RecoSoft->SetTitle("Rate Mode15 RECO + Soft");
  TGraph *RateMode15RecoOnly = new TGraph(30, TrigPT, CountMode15RecoOnly); RateMode15RecoOnly->SetTitle("Rate Mode15 RECO + Only");
	
  TGraph *RateMode14 = new TGraph(30, TrigPT, CountMode14); RateMode14->SetTitle("Rate Mode14");
  TGraph *RateMode14dBX0 = new TGraph(30, TrigPT, CountMode14dBX0); RateMode14dBX0->SetTitle("Rate Mode14 dBX0");
  TGraph *RateMode14RecoSoft = new TGraph(30, TrigPT, CountMode14RecoSoft); RateMode14RecoSoft->SetTitle("Rate Mode14 RECO + Soft");
  TGraph *RateMode14RecoOnly = new TGraph(30, TrigPT, CountMode14RecoOnly); RateMode14RecoOnly->SetTitle("Rate Mode14 RECO + Only");
 
  TGraph *RateMode13 = new TGraph(30, TrigPT, CountMode13); RateMode13->SetTitle("Rate Mode13");
  TGraph *RateMode13dBX0 = new TGraph(30, TrigPT, CountMode13dBX0); RateMode13dBX0->SetTitle("Rate Mode13 dBX0");
  TGraph *RateMode13RecoSoft = new TGraph(30, TrigPT, CountMode13RecoSoft); RateMode13RecoSoft->SetTitle("Rate Mode13 RECO + Soft");
  TGraph *RateMode13RecoOnly = new TGraph(30, TrigPT, CountMode13RecoOnly); RateMode13RecoOnly->SetTitle("Rate Mode13 RECO + Only");
  
  TGraph *RateMode11 = new TGraph(30, TrigPT, CountMode11); RateMode11->SetTitle("Rate Mode11");
  TGraph *RateMode11dBX0 = new TGraph(30, TrigPT, CountMode11dBX0); RateMode11dBX0->SetTitle("Rate Mode11 dBX0");
  TGraph *RateMode11RecoSoft = new TGraph(30, TrigPT, CountMode11RecoSoft); RateMode11RecoSoft->SetTitle("Rate Mode11 RECO + Soft");
  TGraph *RateMode11RecoOnly = new TGraph(30, TrigPT, CountMode11RecoOnly); RateMode11RecoOnly->SetTitle("Rate Mode11 RECO + Only");
	
  TGraph *RateMode12 = new TGraph(30, TrigPT, CountMode12); RateMode12->SetTitle("Rate Mode12");
  TGraph *RateMode12dBX0 = new TGraph(30, TrigPT, CountMode12dBX0); RateMode12dBX0->SetTitle("Rate Mode12 dBX0");
  TGraph *RateMode12RecoSoft = new TGraph(30, TrigPT, CountMode12RecoSoft); RateMode12RecoSoft->SetTitle("Rate Mode12 RECO + Soft");
  TGraph *RateMode12RecoOnly = new TGraph(30, TrigPT, CountMode12RecoOnly); RateMode12RecoOnly->SetTitle("Rate Mode12 RECO + Only");

  TGraph *RateMode10 = new TGraph(30, TrigPT, CountMode10); RateMode10->SetTitle("Rate Mode10");
  TGraph *RateMode10dBX0 = new TGraph(30, TrigPT, CountMode10dBX0); RateMode10dBX0->SetTitle("Rate Mode10 dBX0");
  TGraph *RateMode10RecoSoft = new TGraph(30, TrigPT, CountMode10RecoSoft); RateMode10RecoSoft->SetTitle("Rate Mode10 RECO + Soft");
  TGraph *RateMode10RecoOnly = new TGraph(30, TrigPT, CountMode10RecoOnly); RateMode10RecoOnly->SetTitle("Rate Mode10 RECO + Only");
  
  TGraph *RateMode7 = new TGraph(30, TrigPT, CountMode7); RateMode7->SetTitle("Rate Mode7");
  TGraph *RateMode7dBX0 = new TGraph(30, TrigPT, CountMode7dBX0); RateMode7dBX0->SetTitle("Rate Mode7 dBX0");
  TGraph *RateMode7RecoSoft = new TGraph(30, TrigPT, CountMode7RecoSoft); RateMode7RecoSoft->SetTitle("Rate Mode7 RECO + Soft");
  TGraph *RateMode7RecoOnly = new TGraph(30, TrigPT, CountMode7RecoOnly); RateMode7RecoOnly->SetTitle("Rate Mode7 RECO + Only");
	
  TGraph *RateMode9 = new TGraph(30, TrigPT, CountMode9); RateMode9->SetTitle("Rate Mode9");
  TGraph *RateMode9dBX0 = new TGraph(30, TrigPT, CountMode9dBX0); RateMode9dBX0->SetTitle("Rate Mode9 dBX0");
  TGraph *RateMode9RecoSoft = new TGraph(30, TrigPT, CountMode9RecoSoft); RateMode9RecoSoft->SetTitle("Rate Mode9 RECO + Soft");
  TGraph *RateMode9RecoOnly = new TGraph(30, TrigPT, CountMode9RecoOnly); RateMode9RecoOnly->SetTitle("Rate Mode9 RECO + Only");
 
  TGraph *RateMode6 = new TGraph(30, TrigPT, CountMode6); RateMode9->SetTitle("Rate Mode6");
  TGraph *RateMode6dBX0 = new TGraph(30, TrigPT, CountMode6dBX0); RateMode6dBX0->SetTitle("Rate Mode6 dBX0");
  TGraph *RateMode6RecoSoft = new TGraph(30, TrigPT, CountMode6RecoSoft); RateMode6RecoSoft->SetTitle("Rate Mode6 RECO + Soft");
  TGraph *RateMode6RecoOnly = new TGraph(30, TrigPT, CountMode6RecoOnly); RateMode6RecoOnly->SetTitle("Rate Mode6 RECO + Only");
  
  TGraph *RateMode5 = new TGraph(30, TrigPT, CountMode5); RateMode5->SetTitle("Rate Mode5");
  TGraph *RateMode5dBX0 = new TGraph(30, TrigPT, CountMode5dBX0); RateMode5dBX0->SetTitle("Rate Mode5 dBX0");
  TGraph *RateMode5RecoSoft = new TGraph(30, TrigPT, CountMode5RecoSoft); RateMode5RecoSoft->SetTitle("Rate Mode5 RECO + Soft");
  TGraph *RateMode5RecoOnly = new TGraph(30, TrigPT, CountMode5RecoOnly); RateMode5RecoOnly->SetTitle("Rate Mode5 RECO + Only");
	
  TGraph *RateMode3 = new TGraph(30, TrigPT, CountMode3); RateMode3->SetTitle("Rate Mode3");
  TGraph *RateMode3dBX0 = new TGraph(30, TrigPT, CountMode3dBX0); RateMode3dBX0->SetTitle("Rate Mode3 dBX0");
  TGraph *RateMode3RecoSoft = new TGraph(30, TrigPT, CountMode3RecoSoft); RateMode3RecoSoft->SetTitle("Rate Mode3 RECO + Soft");
  TGraph *RateMode3RecoOnly = new TGraph(30, TrigPT, CountMode3RecoOnly); RateMode3RecoOnly->SetTitle("Rate Mode3 RECO + Only");
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
	
  double RatioMode15[30]={0};
  double RatioMode15dBX0[30]={0};
  double RatioMode15RecoSoft[30]={0};
  double RatioMode15RecoOnly[30]={0};
  double RatioMode14[30]={0};
  double RatioMode14dBX0[30]={0};
  double RatioMode14RecoSoft[30]={0};
  double RatioMode14RecoOnly[30]={0};
  double RatioMode13[30]={0};
  double RatioMode13dBX0[30]={0};
  double RatioMode13RecoSoft[30]={0};
  double RatioMode13RecoOnly[30]={0};
  double RatioMode11[30]={0};
  double RatioMode11dBX0[30]={0};
  double RatioMode11RecoSoft[30]={0};
  double RatioMode11RecoOnly[30]={0};
  double RatioMode12[30]={0};
  double RatioMode12dBX0[30]={0};
  double RatioMode12RecoSoft[30]={0};
  double RatioMode12RecoOnly[30]={0};
  double RatioMode10[30]={0};
  double RatioMode10dBX0[30]={0};
  double RatioMode10RecoSoft[30]={0};
  double RatioMode10RecoOnly[30]={0};
  double RatioMode7[30]={0};
  double RatioMode7dBX0[30]={0};
  double RatioMode7RecoSoft[30]={0};
  double RatioMode7RecoOnly[30]={0};
  double RatioMode9[30]={0};
  double RatioMode9dBX0[30]={0};
  double RatioMode9RecoSoft[30]={0};
  double RatioMode9RecoOnly[30]={0};
  double RatioMode6[30]={0};
  double RatioMode6dBX0[30]={0};
  double RatioMode6RecoSoft[30]={0};
  double RatioMode6RecoOnly[30]={0};
  double RatioMode5[30]={0};
  double RatioMode5dBX0[30]={0};
  double RatioMode5RecoSoft[30]={0};
  double RatioMode5RecoOnly[30]={0};
  double RatioMode3[30]={0};
  double RatioMode3dBX0[30]={0};
  double RatioMode3RecoSoft[30]={0};
  double RatioMode3RecoOnly[30]={0};
  
  TCanvas *CRate=new TCanvas("CRate","All Modes",700,500);
	
  TCanvas *CRateSingleMuModes=new TCanvas("CRateSingleMuModes","SingleMuModes",700,500);
  TCanvas *CRateDoubleMuModes=new TCanvas("CRateDoubleMuModes","DoubleMuModes",700,500);
  TCanvas *CRateMuOpenModes=new TCanvas("CRateMuOpenModes","MuOpenModes",700,500);
	
  TCanvas *CRateMode15=new TCanvas("CRateMode15","Mode15",700,500);
  TCanvas *CRateMode14=new TCanvas("CRateMode14","Mode14",700,500);
  TCanvas *CRateMode13=new TCanvas("CRateMode13","Mode13",700,500);
  TCanvas *CRateMode11=new TCanvas("CRateMode11","Mode11",700,500);
  TCanvas *CRateMode12=new TCanvas("CRateMode12","Mode12",700,500);
  TCanvas *CRateMode10=new TCanvas("CRateMode10","Mode10",700,500);
  TCanvas *CRateMode7=new TCanvas("CRateMode7","Mode7",700,500);
  TCanvas *CRateMode9=new TCanvas("CRateMode9","Mode9",700,500);
  TCanvas *CRateMode6=new TCanvas("CRateMode6","Mode6",700,500);
  TCanvas *CRateMode5=new TCanvas("CRateMode5","Mode5",700,500);
  TCanvas *CRateMode3=new TCanvas("CRateMode3","Mode3",700,500);
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
	
//each mode
  TMultiGraph *mgMode15=new TMultiGraph();
  for(int i=0;i<30;i++){
	  RatioMode15[i] = 1.0*CountMode15[i]/Count[i];
	  RatioMode15dBX0[i] = 1.0*CountMode15dBX0[i]/Count[i];
          RatioMode15RecoSoft[i] = 1.0*CountMode15RecoSoft[i]/Count[i];
          RatioMode15RecoOnly[i] = 1.0*CountMode15RecoOnly[i]/Count[i];
  }
  TGraph *RateRatioMode15 = new TGraph(30, TrigPT, RatioMode15); RateRatioMode15->SetMarkerStyle(22); RateRatioMode15->SetLineColor(1);
  TGraph *RateRatioMode15dBX0 = new TGraph(30, TrigPT, RatioMode15dBX0); RateRatioMode15dBX0->SetMarkerStyle(22); RateRatioMode15dBX0->SetLineColor(2);
  TGraph *RateRatioMode15RecoSoft = new TGraph(30, TrigPT, RatioMode15RecoSoft); RateRatioMode15RecoSoft->SetMarkerStyle(22); RateRatioMode15RecoSoft->SetLineColor(3);
  TGraph *RateRatioMode15RecoOnly = new TGraph(30, TrigPT, RatioMode15RecoOnly); RateRatioMode15RecoOnly->SetMarkerStyle(22); RateRatioMode15RecoOnly->SetLineColor(4);
  CRateMode15->cd();
  mgMode15->SetTitle("Mode 15");
  mgMode15->Add(RateRatioMode15);
  mgMode15->Add(RateRatioMode15dBX0);
  mgMode15->Add(RateRatioMode15RecoSoft);
  mgMode15->Add(RateRatioMode15RecoOnly);
  mgMode15->Draw("LP");
  mgMode15->Write();
        
  
  TMultiGraph *mgMode14=new TMultiGraph();
  for(int i=0;i<30;i++){
	  RatioMode14[i] = 1.0*CountMode14[i]/Count[i];
	  RatioMode14dBX0[i] = 1.0*CountMode14dBX0[i]/Count[i];
          RatioMode14RecoSoft[i] = 1.0*CountMode14RecoSoft[i]/Count[i];
          RatioMode14RecoOnly[i] = 1.0*CountMode14RecoOnly[i]/Count[i];
  }
  TGraph *RateRatioMode14 = new TGraph(30, TrigPT, RatioMode14); RateRatioMode14->SetMarkerStyle(22); RateRatioMode14->SetLineColor(1);
  TGraph *RateRatioMode14dBX0 = new TGraph(30, TrigPT, RatioMode14dBX0); RateRatioMode14dBX0->SetMarkerStyle(22); RateRatioMode14dBX0->SetLineColor(2);
  TGraph *RateRatioMode14RecoSoft = new TGraph(30, TrigPT, RatioMode14RecoSoft); RateRatioMode14RecoSoft->SetMarkerStyle(22); RateRatioMode14RecoSoft->SetLineColor(3);
  TGraph *RateRatioMode14RecoOnly = new TGraph(30, TrigPT, RatioMode14RecoOnly); RateRatioMode14RecoOnly->SetMarkerStyle(22); RateRatioMode14RecoOnly->SetLineColor(4);
  CRateMode14->cd();
  mgMode14->SetTitle("Mode 14");
  mgMode14->Add(RateRatioMode14);
  mgMode14->Add(RateRatioMode14dBX0);
  mgMode14->Add(RateRatioMode14RecoSoft);
  mgMode14->Add(RateRatioMode14RecoOnly);
  mgMode14->Draw("LP");
  mgMode14->Write();
  
  TMultiGraph *mgMode13=new TMultiGraph();
  for(int i=0;i<30;i++){
	  RatioMode13[i] = 1.0*CountMode13[i]/Count[i];
	  RatioMode13dBX0[i] = 1.0*CountMode13dBX0[i]/Count[i];
          RatioMode13RecoSoft[i] = 1.0*CountMode13RecoSoft[i]/Count[i];
          RatioMode13RecoOnly[i] = 1.0*CountMode13RecoOnly[i]/Count[i];
  }
  TGraph *RateRatioMode13 = new TGraph(30, TrigPT, RatioMode13); RateRatioMode13->SetMarkerStyle(22); RateRatioMode13->SetLineColor(1);
  TGraph *RateRatioMode13dBX0 = new TGraph(30, TrigPT, RatioMode13dBX0); RateRatioMode13dBX0->SetMarkerStyle(22); RateRatioMode13dBX0->SetLineColor(2);
  TGraph *RateRatioMode13RecoSoft = new TGraph(30, TrigPT, RatioMode13RecoSoft); RateRatioMode13RecoSoft->SetMarkerStyle(22); RateRatioMode13RecoSoft->SetLineColor(3);
  TGraph *RateRatioMode13RecoOnly = new TGraph(30, TrigPT, RatioMode13RecoOnly); RateRatioMode13RecoOnly->SetMarkerStyle(22); RateRatioMode13RecoOnly->SetLineColor(4);
  CRateMode13->cd();
  mgMode13->SetTitle("Mode 13");
  mgMode13->Add(RateRatioMode13);
  mgMode13->Add(RateRatioMode13dBX0);
  mgMode13->Add(RateRatioMode13RecoSoft);
  mgMode13->Add(RateRatioMode13RecoOnly);
  mgMode13->Draw("LP");
  mgMode13->Write();
	
  TMultiGraph *mgMode11=new TMultiGraph();
  for(int i=0;i<30;i++){
	  RatioMode11[i] = 1.0*CountMode11[i]/Count[i];
	  RatioMode11dBX0[i] = 1.0*CountMode11dBX0[i]/Count[i];
          RatioMode11RecoSoft[i] = 1.0*CountMode11RecoSoft[i]/Count[i];
          RatioMode11RecoOnly[i] = 1.0*CountMode11RecoOnly[i]/Count[i];
  }
  TGraph *RateRatioMode11 = new TGraph(30, TrigPT, RatioMode11); RateRatioMode11->SetMarkerStyle(22); RateRatioMode11->SetLineColor(1);
  TGraph *RateRatioMode11dBX0 = new TGraph(30, TrigPT, RatioMode11dBX0); RateRatioMode11dBX0->SetMarkerStyle(22); RateRatioMode11dBX0->SetLineColor(2);
  TGraph *RateRatioMode11RecoSoft = new TGraph(30, TrigPT, RatioMode11RecoSoft); RateRatioMode11RecoSoft->SetMarkerStyle(22); RateRatioMode11RecoSoft->SetLineColor(3);
  TGraph *RateRatioMode11RecoOnly = new TGraph(30, TrigPT, RatioMode11RecoOnly); RateRatioMode11RecoOnly->SetMarkerStyle(22); RateRatioMode11RecoOnly->SetLineColor(4);
  CRateMode11->cd();
  mgMode11->SetTitle("Mode 11");
  mgMode11->Add(RateRatioMode11);
  mgMode11->Add(RateRatioMode11dBX0);
  mgMode11->Add(RateRatioMode11RecoSoft);
  mgMode11->Add(RateRatioMode11RecoOnly);
  mgMode11->Draw("LP");
  mgMode11->Write();
	
  TMultiGraph *mgMode12=new TMultiGraph();
  for(int i=0;i<30;i++){
	  RatioMode12[i] = 1.0*CountMode12[i]/Count[i];
	  RatioMode12dBX0[i] = 1.0*CountMode12dBX0[i]/Count[i];
          RatioMode12RecoSoft[i] = 1.0*CountMode12RecoSoft[i]/Count[i];
          RatioMode12RecoOnly[i] = 1.0*CountMode12RecoOnly[i]/Count[i];
  }
  TGraph *RateRatioMode12 = new TGraph(30, TrigPT, RatioMode12); RateRatioMode12->SetMarkerStyle(22); RateRatioMode12->SetLineColor(1);
  TGraph *RateRatioMode12dBX0 = new TGraph(30, TrigPT, RatioMode12dBX0); RateRatioMode12dBX0->SetMarkerStyle(22); RateRatioMode12dBX0->SetLineColor(2);
  TGraph *RateRatioMode12RecoSoft = new TGraph(30, TrigPT, RatioMode12RecoSoft); RateRatioMode12RecoSoft->SetMarkerStyle(22); RateRatioMode12RecoSoft->SetLineColor(3);
  TGraph *RateRatioMode12RecoOnly = new TGraph(30, TrigPT, RatioMode12RecoOnly); RateRatioMode12RecoOnly->SetMarkerStyle(22); RateRatioMode12RecoOnly->SetLineColor(4);
  CRateMode12->cd();
  mgMode12->SetTitle("Mode 12");
  mgMode12->Add(RateRatioMode12);
  mgMode12->Add(RateRatioMode12dBX0);
  mgMode12->Add(RateRatioMode12RecoSoft);
  mgMode12->Add(RateRatioMode12RecoOnly);
  mgMode12->Draw("LP");
  mgMode12->Write();
	
  TMultiGraph *mgMode10=new TMultiGraph();
  for(int i=0;i<30;i++){
	  RatioMode10[i] = 1.0*CountMode10[i]/Count[i];
	  RatioMode10dBX0[i] = 1.0*CountMode10dBX0[i]/Count[i];
          RatioMode10RecoSoft[i] = 1.0*CountMode10RecoSoft[i]/Count[i];
          RatioMode10RecoOnly[i] = 1.0*CountMode10RecoOnly[i]/Count[i];
  }
  TGraph *RateRatioMode10 = new TGraph(30, TrigPT, RatioMode10); RateRatioMode10->SetMarkerStyle(22); RateRatioMode10->SetLineColor(1);
  TGraph *RateRatioMode10dBX0 = new TGraph(30, TrigPT, RatioMode10dBX0); RateRatioMode10dBX0->SetMarkerStyle(22); RateRatioMode10dBX0->SetLineColor(2);
  TGraph *RateRatioMode10RecoSoft = new TGraph(30, TrigPT, RatioMode10RecoSoft); RateRatioMode10RecoSoft->SetMarkerStyle(22); RateRatioMode10RecoSoft->SetLineColor(3);
  TGraph *RateRatioMode10RecoOnly = new TGraph(30, TrigPT, RatioMode10RecoOnly); RateRatioMode10RecoOnly->SetMarkerStyle(22); RateRatioMode10RecoOnly->SetLineColor(4);
  CRateMode10->cd();
  mgMode10->SetTitle("Mode 10");
  mgMode10->Add(RateRatioMode10);
  mgMode10->Add(RateRatioMode10dBX0);
  mgMode10->Add(RateRatioMode10RecoSoft);
  mgMode10->Add(RateRatioMode10RecoOnly);
  mgMode10->Draw("LP");
  mgMode10->Write();

  TMultiGraph *mgMode7=new TMultiGraph();
  for(int i=0;i<30;i++){
	  RatioMode7[i] = 1.0*CountMode7[i]/Count[i];
	  RatioMode7dBX0[i] = 1.0*CountMode7dBX0[i]/Count[i];
          RatioMode7RecoSoft[i] = 1.0*CountMode7RecoSoft[i]/Count[i];
          RatioMode7RecoOnly[i] = 1.0*CountMode7RecoOnly[i]/Count[i];
  }
  TGraph *RateRatioMode7 = new TGraph(30, TrigPT, RatioMode7); RateRatioMode7->SetMarkerStyle(22); RateRatioMode7->SetLineColor(1);
  TGraph *RateRatioMode7dBX0 = new TGraph(30, TrigPT, RatioMode7dBX0); RateRatioMode7dBX0->SetMarkerStyle(22); RateRatioMode7dBX0->SetLineColor(2);
  TGraph *RateRatioMode7RecoSoft = new TGraph(30, TrigPT, RatioMode7RecoSoft); RateRatioMode7RecoSoft->SetMarkerStyle(22); RateRatioMode7RecoSoft->SetLineColor(3);
  TGraph *RateRatioMode7RecoOnly = new TGraph(30, TrigPT, RatioMode7RecoOnly); RateRatioMode7RecoOnly->SetMarkerStyle(22); RateRatioMode7RecoOnly->SetLineColor(4);
  CRateMode7->cd();
  mgMode7->SetTitle("Mode 7");
  mgMode7->Add(RateRatioMode7);
  mgMode7->Add(RateRatioMode7dBX0);
  mgMode7->Add(RateRatioMode7RecoSoft);
  mgMode7->Add(RateRatioMode7RecoOnly);
  mgMode7->Draw("LP");
  mgMode7->Write();
	
  TMultiGraph *mgMode9=new TMultiGraph();
  for(int i=0;i<30;i++){
	  RatioMode9[i] = 1.0*CountMode9[i]/Count[i];
	  RatioMode9dBX0[i] = 1.0*CountMode9dBX0[i]/Count[i];
          RatioMode9RecoSoft[i] = 1.0*CountMode9RecoSoft[i]/Count[i];
          RatioMode9RecoOnly[i] = 1.0*CountMode9RecoOnly[i]/Count[i];
  }
  TGraph *RateRatioMode9 = new TGraph(30, TrigPT, RatioMode9); RateRatioMode9->SetMarkerStyle(22); RateRatioMode9->SetLineColor(1);
  TGraph *RateRatioMode9dBX0 = new TGraph(30, TrigPT, RatioMode9dBX0); RateRatioMode9dBX0->SetMarkerStyle(22); RateRatioMode9dBX0->SetLineColor(2);
  TGraph *RateRatioMode9RecoSoft = new TGraph(30, TrigPT, RatioMode9RecoSoft); RateRatioMode9RecoSoft->SetMarkerStyle(22); RateRatioMode9RecoSoft->SetLineColor(3);
  TGraph *RateRatioMode9RecoOnly = new TGraph(30, TrigPT, RatioMode9RecoOnly); RateRatioMode9RecoOnly->SetMarkerStyle(22); RateRatioMode9RecoOnly->SetLineColor(4);
  CRateMode9->cd();
  mgMode9->SetTitle("Mode 9");
  mgMode9->Add(RateRatioMode9);
  mgMode9->Add(RateRatioMode9dBX0);
  mgMode9->Add(RateRatioMode9RecoSoft);
  mgMode9->Add(RateRatioMode9RecoOnly);
  mgMode9->Draw("LP");
  mgMode9->Write();
	
  TMultiGraph *mgMode6=new TMultiGraph();
  for(int i=0;i<30;i++){
	  RatioMode6[i] = 1.0*CountMode6[i]/Count[i];
	  RatioMode6dBX0[i] = 1.0*CountMode6dBX0[i]/Count[i];
          RatioMode6RecoSoft[i] = 1.0*CountMode6RecoSoft[i]/Count[i];
          RatioMode6RecoOnly[i] = 1.0*CountMode6RecoOnly[i]/Count[i];
  }
  TGraph *RateRatioMode6 = new TGraph(30, TrigPT, RatioMode6); RateRatioMode6->SetMarkerStyle(22); RateRatioMode6->SetLineColor(1);
  TGraph *RateRatioMode6dBX0 = new TGraph(30, TrigPT, RatioMode6dBX0); RateRatioMode6dBX0->SetMarkerStyle(22); RateRatioMode6dBX0->SetLineColor(2);
  TGraph *RateRatioMode6RecoSoft = new TGraph(30, TrigPT, RatioMode6RecoSoft); RateRatioMode6RecoSoft->SetMarkerStyle(22); RateRatioMode6RecoSoft->SetLineColor(3);
  TGraph *RateRatioMode6RecoOnly = new TGraph(30, TrigPT, RatioMode6RecoOnly); RateRatioMode6RecoOnly->SetMarkerStyle(22); RateRatioMode6RecoOnly->SetLineColor(4);
  CRateMode6->cd();
  mgMode6->SetTitle("Mode 6");
  mgMode6->Add(RateRatioMode6);
  mgMode6->Add(RateRatioMode6dBX0);
  mgMode6->Add(RateRatioMode6RecoSoft);
  mgMode6->Add(RateRatioMode6RecoOnly);
  mgMode6->Draw("LP");
  mgMode6->Write();
	
  TMultiGraph *mgMode5=new TMultiGraph();
  for(int i=0;i<30;i++){
	  RatioMode5[i] = 1.0*CountMode5[i]/Count[i];
	  RatioMode5dBX0[i] = 1.0*CountMode5dBX0[i]/Count[i];
          RatioMode5RecoSoft[i] = 1.0*CountMode5RecoSoft[i]/Count[i];
          RatioMode5RecoOnly[i] = 1.0*CountMode5RecoOnly[i]/Count[i];
  }
  TGraph *RateRatioMode5 = new TGraph(30, TrigPT, RatioMode5); RateRatioMode5->SetMarkerStyle(22); RateRatioMode5->SetLineColor(1);
  TGraph *RateRatioMode5dBX0 = new TGraph(30, TrigPT, RatioMode5dBX0); RateRatioMode5dBX0->SetMarkerStyle(22); RateRatioMode5dBX0->SetLineColor(2);
  TGraph *RateRatioMode5RecoSoft = new TGraph(30, TrigPT, RatioMode5RecoSoft); RateRatioMode5RecoSoft->SetMarkerStyle(22); RateRatioMode5RecoSoft->SetLineColor(3);
  TGraph *RateRatioMode5RecoOnly = new TGraph(30, TrigPT, RatioMode5RecoOnly); RateRatioMode5RecoOnly->SetMarkerStyle(22); RateRatioMode5RecoOnly->SetLineColor(4);
  CRateMode5->cd();
  mgMode5->SetTitle("Mode 5");
  mgMode5->Add(RateRatioMode5);
  mgMode5->Add(RateRatioMode5dBX0);
  mgMode5->Add(RateRatioMode5RecoSoft);
  mgMode5->Add(RateRatioMode5RecoOnly);
  mgMode5->Draw("LP");
  mgMode5->Write();
	
  TMultiGraph *mgMode3=new TMultiGraph();
  for(int i=0;i<30;i++){
	  RatioMode3[i] = 1.0*CountMode3[i]/Count[i];
	  RatioMode3dBX0[i] = 1.0*CountMode3dBX0[i]/Count[i];
          RatioMode3RecoSoft[i] = 1.0*CountMode3RecoSoft[i]/Count[i];
          RatioMode3RecoOnly[i] = 1.0*CountMode3RecoOnly[i]/Count[i];
  }
  TGraph *RateRatioMode3 = new TGraph(30, TrigPT, RatioMode3); RateRatioMode3->SetMarkerStyle(22); RateRatioMode3->SetLineColor(1);
  TGraph *RateRatioMode3dBX0 = new TGraph(30, TrigPT, RatioMode3dBX0); RateRatioMode3dBX0->SetMarkerStyle(22); RateRatioMode3dBX0->SetLineColor(2);
  TGraph *RateRatioMode3RecoSoft = new TGraph(30, TrigPT, RatioMode3RecoSoft); RateRatioMode3RecoSoft->SetMarkerStyle(22); RateRatioMode3RecoSoft->SetLineColor(3);
  TGraph *RateRatioMode3RecoOnly = new TGraph(30, TrigPT, RatioMode3RecoOnly); RateRatioMode3RecoOnly->SetMarkerStyle(22); RateRatioMode3RecoOnly->SetLineColor(4);
  CRateMode3->cd();
  mgMode3->SetTitle("Mode 3");
  mgMode3->Add(RateRatioMode3);
  mgMode3->Add(RateRatioMode3dBX0);
  mgMode3->Add(RateRatioMode3RecoSoft);
  mgMode3->Add(RateRatioMode3RecoOnly);
  mgMode3->Draw("LP");
  mgMode3->Write();
 
  //intermidiate plots
  SMRecoPtNoMatch->Write();
  SMRecoPtNoUniqueMatch->Write();
  SMRecoPtUniqueMatch->Write();
  SMRecoPtUniqueMatchBX0->Write();
	
  SMRecoPtMatchSingleMuModes->Write();
  SMRecoPtMatchSingleMuModesBX0->Write();
  SMRecoPtMatchSingleMuModesBX0dBX0->Write();
  SMRecoPtMatchSingleMuModesBX0Plateau->Write();

  SMRecoPtMatchDoubleMuModes->Write();
  SMRecoPtMatchDoubleMuModesBX0->Write();
  SMRecoPtMatchDoubleMuModesBX0dBX0->Write();
  SMRecoPtMatchDoubleMuModesBX0Plateau->Write();
	
  SMRecoPtMatchMuOpenModes->Write();
  SMRecoPtMatchMuOpenModesBX0->Write();
  SMRecoPtMatchMuOpenModesBX0dBX0->Write();
  SMRecoPtMatchMuOpenModesBX0Plateau->Write();
  
  SMRecoPtMatchMode15->Write();
  SMRecoPtMatchMode14->Write();
  SMRecoPtMatchMode13->Write();
  SMRecoPtMatchMode11->Write();
  SMRecoPtMatchMode12->Write();
  SMRecoPtMatchMode10->Write();
  SMRecoPtMatchMode7->Write();
  SMRecoPtMatchMode9->Write();
  SMRecoPtMatchMode6->Write();
  SMRecoPtMatchMode5->Write();
  SMRecoPtMatchMode3->Write();
	
  SMRecoPtMatchMode15BX0->Write();
  SMRecoPtMatchMode14BX0->Write();
  SMRecoPtMatchMode13BX0->Write();
  SMRecoPtMatchMode11BX0->Write();
  SMRecoPtMatchMode12BX0->Write();
  SMRecoPtMatchMode10BX0->Write();
  SMRecoPtMatchMode7BX0->Write();
  SMRecoPtMatchMode9BX0->Write();
  SMRecoPtMatchMode6BX0->Write();
  SMRecoPtMatchMode5BX0->Write();
  SMRecoPtMatchMode3BX0->Write();
	
  SMRecoPtMatchMode15BX0dBX0->Write();
  SMRecoPtMatchMode14BX0dBX0->Write();
  SMRecoPtMatchMode13BX0dBX0->Write();
  SMRecoPtMatchMode11BX0dBX0->Write();
  SMRecoPtMatchMode12BX0dBX0->Write();
  SMRecoPtMatchMode10BX0dBX0->Write();
  SMRecoPtMatchMode7BX0dBX0->Write();
  SMRecoPtMatchMode9BX0dBX0->Write();
  SMRecoPtMatchMode6BX0dBX0->Write();
  SMRecoPtMatchMode5BX0dBX0->Write();
  SMRecoPtMatchMode3BX0dBX0->Write();
	
  SMRecoPtMatchMode15BX0Plateau->Write();
  SMRecoPtMatchMode14BX0Plateau->Write();
  SMRecoPtMatchMode13BX0Plateau->Write();
  SMRecoPtMatchMode11BX0Plateau->Write();
  SMRecoPtMatchMode12BX0Plateau->Write();
  SMRecoPtMatchMode10BX0Plateau->Write();
  SMRecoPtMatchMode7BX0Plateau->Write();
  SMRecoPtMatchMode9BX0Plateau->Write();
  SMRecoPtMatchMode6BX0Plateau->Write();
  SMRecoPtMatchMode5BX0Plateau->Write();
  SMRecoPtMatchMode3BX0Plateau->Write();
  
  //rate
  RateSingleMuModes->Write(); 
  RateSingleMuModesdBX0->Write();
  RateSingleMuModesRecoSoft->Write();
  RateSingleMuModesRecoOnly->Write();
	
  RateDoubleMuModes->Write(); 
  RateDoubleMuModesdBX0->Write();
  RateDoubleMuModesRecoSoft->Write();
  RateDoubleMuModesRecoOnly->Write();

  RateMuOpenModes->Write(); 
  RateMuOpenModesdBX0->Write();
  RateMuOpenModesRecoSoft->Write();
  RateMuOpenModesRecoOnly->Write();
	
  RateMode15->Write(); 
  RateMode15dBX0->Write();
  RateMode15RecoSoft->Write();
  RateMode15RecoOnly->Write();
	
  RateMode14->Write();
  RateMode14dBX0->Write();
  RateMode14RecoSoft->Write();
  RateMode14RecoOnly->Write();
 
  RateMode13->Write();
  RateMode13dBX0->Write();
  RateMode13RecoSoft->Write();
  RateMode13RecoOnly->Write();
  
  RateMode11->Write();
  RateMode11dBX0->Write();
  RateMode11RecoSoft->Write();
  RateMode11RecoOnly->Write();
	
  RateMode12->Write();
  RateMode12dBX0->Write(); 
  RateMode12RecoSoft->Write();
  RateMode12RecoOnly->Write();

  RateMode10->Write();
  RateMode10dBX0->Write();
  RateMode10RecoSoft->Write();
  RateMode10RecoOnly->Write();
  
  RateMode7->Write();
  RateMode7dBX0->Write();
  RateMode7RecoSoft->Write();
  RateMode7RecoOnly->Write();
	
  RateMode9->Write(); 
  RateMode9dBX0->Write();
  RateMode9RecoSoft->Write();
  RateMode9RecoOnly->Write();
 
  RateMode6->Write();
  RateMode6dBX0->Write();
  RateMode6RecoSoft->Write();
  RateMode6RecoOnly->Write();
  
  RateMode5->Write();
  RateMode5dBX0->Write();
  RateMode5RecoSoft->Write();
  RateMode5RecoOnly->Write();
	
  RateMode3->Write();
  RateMode3dBX0->Write();
  RateMode3RecoSoft->Write();
  RateMode3RecoOnly->Write();
  
  myPlot.Close();
  
} // End function: void 
