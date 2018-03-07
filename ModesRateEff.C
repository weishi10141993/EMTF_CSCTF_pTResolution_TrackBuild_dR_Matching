#include "TFile.h"
#include "TSystem.h"
#include "TChain.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH2.h"
// List of input branches and functions to return values
#include "Read_FlatNtuple.h" //make sure it's up-to-date with the Ntuple


//USER modify here ONLY//
//================================================================
const bool verbose  = true; // Print information
const int PT_UP = 30;//Reco pT range
const int PT_LOW = 0;
const float ETA_UP = 2.4;//Reco eta @station 2
const float ETA_LOW = 1.25;
const int MAX_EVT   = 1000000;   // Max number of events to process
const int PRT_EVT   =  1;   // Print every N events
//================================================================

void ModesRateEff() {
   
  // Initialize empty file to access each file in the list
  TFile *file_tmp(0);

  // List of input files
  std::vector<TString> SM_in_file_names;//SingleMu
  std::vector<TString> ZB_in_file_names;//Zerobias
  TString store = "root://eoscms.cern.ch//store/user/abrinke1/EMTF/Emulator/";
  TString in_dir = "ntuples/HADD/";
  TString SM_file_name;
  TString ZB_file_name;

  TString SingleMu_files[3] = { 
	  		    "NTuple_SingleMuon_FlatNtuple_Run_306092_2018_03_02_SingleMu.root",
   			    "NTuple_SingleMuon_FlatNtuple_Run_306135_2018_03_02_SingleMu.root",
   			    "NTuple_SingleMuon_FlatNtuple_Run_306154_2018_03_02_SingleMu.root"
  };   
  TString ZeroBias_files[4] = { 
	  		    "NTuple_ZeroBias1_FlatNtuple_Run_306091_2018_03_02_ZB1.root",
		            "NTuple_ZeroBias2_FlatNtuple_Run_306091_2018_03_02_ZB2.root",
		            "NTuple_ZeroBias3_FlatNtuple_Run_306091_2018_03_02_ZB3.root", 
		 	    "NTuple_ZeroBias4_FlatNtuple_Run_306091_2018_03_02_ZB4.root"
  };
  for (int i = 0; i < 3; i++) {
	  SM_file_name.Form( "%s/%s/%s", store.Data(), in_dir.Data(), SingleMu_files[i].Data() );
  	  std::cout << "Adding file " << SM_file_name.Data() << std::endl;
          SM_in_file_names.push_back(SM_file_name.Data());
  }
  for (int i = 0; i < 4; i++) {
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
  TH1F *SMRecoPt = new TH1F("SMRecoPt", SMRecoPtTitle, 0, 30, 30);
  TH1F *SMRecoPtNoMatch = new TH1F("SMRecoPtNoMatch", "NoMatch "+ SMRecoPtTitle, 0, 30, 30);
  TH1F *SMRecoPtNoUniqueMatch = new TH1F("SMRecoPtNoUniqueMatch", "NoUniqueMatch "+ SMRecoPtTitle, 0, 30, 30);
  
  //SingluMu quality: Q>=12
  TH1F *SMRecoPtMatchMode15 = new TH1F("SMRecoPtMatchMode15", "Match Mode15" + SMRecoPtTitle, 0, 30, 30);
  TH1F *SMRecoPtMatchMode14 = new TH1F("SMRecoPtMatchMode14", "Match Mode14" + SMRecoPtTitle, 0, 30, 30);
  TH1F *SMRecoPtMatchMode13 = new TH1F("SMRecoPtMatchMode13", "Match Mode13" + SMRecoPtTitle, 0, 30, 30);
  TH1F *SMRecoPtMatchMode11 = new TH1F("SMRecoPtMatchMode11", "Match Mode11" + SMRecoPtTitle, 0, 30, 30);
  //DoubleMu quality: Q>=8
  TH1F *SMRecoPtMatchMode12 = new TH1F("SMRecoPtMatchMode12", "Match Mode12" + SMRecoPtTitle, 0, 30, 30);
  TH1F *SMRecoPtMatchMode10 = new TH1F("SMRecoPtMatchMode10", "Match Mode10" + SMRecoPtTitle, 0, 30, 30);
  TH1F *SMRecoPtMatchMode7 = new TH1F("SMRecoPtMatchMode7", "Match Mode7" + SMRecoPtTitle, 0, 30, 30);
  //MuOpen quality: Q>=4
  TH1F *SMRecoPtMatchMode9 = new TH1F("SMRecoPtMatchMode9", "Match Mode9" + SMRecoPtTitle, 0, 30, 30);
  TH1F *SMRecoPtMatchMode6 = new TH1F("SMRecoPtMatchMode6", "Match Mode6" + SMRecoPtTitle, 0, 30, 30);
  TH1F *SMRecoPtMatchMode5 = new TH1F("SMRecoPtMatchMode5", "Match Mode5" + SMRecoPtTitle, 0, 30, 30);
  TH1F *SMRecoPtMatchMode3 = new TH1F("SMRecoPtMatchMode3", "Match Mode3" + SMRecoPtTitle, 0, 30, 30);

  //SingluMu quality: Q>=12
  TH1F *SMRecoPtMatchMode15BX0 = new TH1F("SMRecoPtMatchMode15BX0", "Match Mode15BX0" + SMRecoPtTitle, 0, 30, 30);
  TH1F *SMRecoPtMatchMode14BX0 = new TH1F("SMRecoPtMatchMode14BX0", "Match Mode14BX0" + SMRecoPtTitle, 0, 30, 30);
  TH1F *SMRecoPtMatchMode13BX0 = new TH1F("SMRecoPtMatchMode13BX0", "Match Mode13BX0" + SMRecoPtTitle, 0, 30, 30);
  TH1F *SMRecoPtMatchMode11BX0 = new TH1F("SMRecoPtMatchMode11BX0", "Match Mode11BX0" + SMRecoPtTitle, 0, 30, 30);
  //DoubleMu quality: Q>=8
  TH1F *SMRecoPtMatchMode12BX0 = new TH1F("SMRecoPtMatchMode12BX0", "Match Mode12BX0" + SMRecoPtTitle, 0, 30, 30);
  TH1F *SMRecoPtMatchMode10BX0 = new TH1F("SMRecoPtMatchMode10BX0", "Match Mode10BX0" + SMRecoPtTitle, 0, 30, 30);
  TH1F *SMRecoPtMatchMode7BX0 = new TH1F("SMRecoPtMatchMode7BX0", "Match Mode7BX0" + SMRecoPtTitle, 0, 30, 30);
  //MuOpen quality: Q>=4
  TH1F *SMRecoPtMatchMode9BX0 = new TH1F("SMRecoPtMatchMode9BX0", "Match Mode9BX0" + SMRecoPtTitle, 0, 30, 30);
  TH1F *SMRecoPtMatchMode6BX0 = new TH1F("SMRecoPtMatchMode6BX0", "Match Mode6BX0" + SMRecoPtTitle, 0, 30, 30);
  TH1F *SMRecoPtMatchMode5BX0 = new TH1F("SMRecoPtMatchMode5BX0", "Match Mode5BX0" + SMRecoPtTitle, 0, 30, 30);
  TH1F *SMRecoPtMatchMode3BX0 = new TH1F("SMRecoPtMatchMode3BX0", "Match Mode3BX0" + SMRecoPtTitle, 0, 30, 30);

  //SingluMu quality: Q>=12
  TH1F *SMRecoPtMatchMode15BX0dBX0 = new TH1F("SMRecoPtMatchMode15BX0dBX0", "Match Mode15BX0dBX0" + SMRecoPtTitle, 0, 30, 30);
  TH1F *SMRecoPtMatchMode14BX0dBX0 = new TH1F("SMRecoPtMatchMode14BX0dBX0", "Match Mode14BX0dBX0" + SMRecoPtTitle, 0, 30, 30);
  TH1F *SMRecoPtMatchMode13BX0dBX0 = new TH1F("SMRecoPtMatchMode13BX0dBX0", "Match Mode13BX0dBX0" + SMRecoPtTitle, 0, 30, 30);
  TH1F *SMRecoPtMatchMode11BX0dBX0 = new TH1F("SMRecoPtMatchMode11BX0dBX0", "Match Mode11BX0dBX0" + SMRecoPtTitle, 0, 30, 30);
  //DoubleMu quality: Q>=8
  TH1F *SMRecoPtMatchMode12BX0dBX0 = new TH1F("SMRecoPtMatchMode12BX0dBX0", "Match Mode12BX0dBX0" + SMRecoPtTitle, 0, 30, 30);
  TH1F *SMRecoPtMatchMode10BX0dBX0 = new TH1F("SMRecoPtMatchMode10BX0dBX0", "Match Mode10BX0dBX0" + SMRecoPtTitle, 0, 30, 30);
  TH1F *SMRecoPtMatchMode7BX0dBX0 = new TH1F("SMRecoPtMatchMode7BX0dBX0", "Match Mode7BX0dBX0" + SMRecoPtTitle, 0, 30, 30);
  //MuOpen quality: Q>=4
  TH1F *SMRecoPtMatchMode9BX0dBX0 = new TH1F("SMRecoPtMatchMode9BX0dBX0", "Match Mode9BX0dBX0" + SMRecoPtTitle, 0, 30, 30);
  TH1F *SMRecoPtMatchMode6BX0dBX0 = new TH1F("SMRecoPtMatchMode6BX0dBX0", "Match Mode6BX0dBX0" + SMRecoPtTitle, 0, 30, 30);
  TH1F *SMRecoPtMatchMode5BX0dBX0 = new TH1F("SMRecoPtMatchMode5BX0dBX0", "Match Mode5BX0dBX0" + SMRecoPtTitle, 0, 30, 30);
  TH1F *SMRecoPtMatchMode3BX0dBX0 = new TH1F("SMRecoPtMatchMode3BX0dBX0", "Match Mode3BX0dBX0" + SMRecoPtTitle, 0, 30, 30);

  //SingluMu quality: Q>=12
  TH1F *SMRecoPtMatchMode15BX0Plateau = new TH1F("SMRecoPtMatchMode15BX0Plateau", "Match Mode15BX0Plateau" + SMRecoPtTitle, 0, 30, 30);
  TH1F *SMRecoPtMatchMode14BX0Plateau = new TH1F("SMRecoPtMatchMode14BX0Plateau", "Match Mode14BX0Plateau" + SMRecoPtTitle, 0, 30, 30);
  TH1F *SMRecoPtMatchMode13BX0Plateau = new TH1F("SMRecoPtMatchMode13BX0Plateau", "Match Mode13BX0Plateau" + SMRecoPtTitle, 0, 30, 30);
  TH1F *SMRecoPtMatchMode11BX0Plateau = new TH1F("SMRecoPtMatchMode11BX0Plateau", "Match Mode11BX0Plateau" + SMRecoPtTitle, 0, 30, 30);
  //DoubleMu quality: Q>=8
  TH1F *SMRecoPtMatchMode12BX0Plateau = new TH1F("SMRecoPtMatchMode12BX0Plateau", "Match Mode12BX0Plateau" + SMRecoPtTitle, 0, 30, 30);
  TH1F *SMRecoPtMatchMode10BX0Plateau = new TH1F("SMRecoPtMatchMode10BX0Plateau", "Match Mode10BX0Plateau" + SMRecoPtTitle, 0, 30, 30);
  TH1F *SMRecoPtMatchMode7BX0Plateau = new TH1F("SMRecoPtMatchMode7BX0Plateau", "Match Mode7BX0Plateau" + SMRecoPtTitle, 0, 30, 30);
  //MuOpen quality: Q>=4
  TH1F *SMRecoPtMatchMode9BX0Plateau = new TH1F("SMRecoPtMatchMode9BX0Plateau", "Match Mode9BX0Plateau" + SMRecoPtTitle, 0, 30, 30);
  TH1F *SMRecoPtMatchMode6BX0Plateau = new TH1F("SMRecoPtMatchMode6BX0Plateau", "Match Mode6BX0Plateau" + SMRecoPtTitle, 0, 30, 30);
  TH1F *SMRecoPtMatchMode5BX0Plateau = new TH1F("SMRecoPtMatchMode5BX0Plateau", "Match Mode5BX0Plateau" + SMRecoPtTitle, 0, 30, 30);
  TH1F *SMRecoPtMatchMode3BX0Plateau = new TH1F("SMRecoPtMatchMode3BX0Plateau", "Match Mode3BX0Plateau" + SMRecoPtTitle, 0, 30, 30);
  
  InitializeMaps();
  SetBranchAddresses(SM_in_chain);
  SetBranchAddresses(ZB_in_chain);
	
  std::cout << "\n******* About to loop over the SingleMu events *******" << std::endl;
  int nEvents = SM_in_chain->GetEntries();
  for (int iEvt = 0; iEvt < nEvents; iEvt++) {
    if (iEvt > MAX_EVT) break;
    if ( (iEvt % PRT_EVT) == 0 ) {
      std::cout << "\n*************************************" << std::endl;
      std::cout << "Looking at event " << iEvt << " out of " << nEvents << std::endl;
      std::cout << "*************************************" << std::endl;
    }

    SM_in_chain->GetEntry(iEvt);
    
    // From Read_FlatNtuple.h, use 'I("branch_name")' to get an integer branch value, 'F("branch_name") to get a float
    // Print info for unpacked EMTF tracks
    if (verbose) std::cout << "\n" << I("nRecoMuons") << " reco muons in the event" << std::endl;
    for (int ireco = 0; ireco < I("nRecoMuons"); ireco++) {
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
			switch ( I("trk_mode", I("reco_dR_match_iTrk", ireco) ) ) {//all trk modes
                            
                        case 15:
                            SMRecoPtMatchMode15->Fill( F("reco_pt", ireco) ); 
                            break; 
			case 14:
                            SMRecoPtMatchMode14->Fill( F("reco_pt", ireco) ); 
                            break;
		        case 13:
                            SMRecoPtMatchMode13->Fill( F("reco_pt", ireco) ); 
                            break; 
			case 11:
                            SMRecoPtMatchMode11->Fill( F("reco_pt", ireco) ); 
                            break;
			case 12:
                            SMRecoPtMatchMode12->Fill( F("reco_pt", ireco) ); 
                            break; 
			case 10:
                            SMRecoPtMatchMode10->Fill( F("reco_pt", ireco) ); 
                            break;
			case 7:
                            SMRecoPtMatchMode7->Fill( F("reco_pt", ireco) ); 
                            break; 
			case 9:
                            SMRecoPtMatchMode9->Fill( F("reco_pt", ireco) ); 
                            break;
			case 6:
                            SMRecoPtMatchMode6->Fill( F("reco_pt", ireco) ); 
                            break; 
			case 5:
                            SMRecoPtMatchMode5->Fill( F("reco_pt", ireco) ); 
                            break;
			case 3:
                            SMRecoPtMatchMode3->Fill( F("reco_pt", ireco) ); 
                            break;
                        default:
                            break;           
                        }//end switch
			
			if( I("trk_BX", I("reco_dR_match_iTrk", ireco) )  == 0 ){
				switch ( I("trk_mode", I("reco_dR_match_iTrk", ireco) ) ) {//all trk modes
                            
                        	case 15:
					SMRecoPtMatchMode15BX0->Fill( F("reco_pt", ireco) ); 
                            	        break; 
				case 14:
                           		SMRecoPtMatchMode14BX0->Fill( F("reco_pt", ireco) ); 
                            		break;
		        	case 13:
	                                SMRecoPtMatchMode13BX0->Fill( F("reco_pt", ireco) ); 
                            		break; 
				case 11:
                            		SMRecoPtMatchMode11BX0->Fill( F("reco_pt", ireco) ); 
                            		break;
				case 12:
                            		SMRecoPtMatchMode12BX0->Fill( F("reco_pt", ireco) ); 
                            		break; 
				case 10:
                            		SMRecoPtMatchMode10BX0->Fill( F("reco_pt", ireco) ); 
                            		break;
				case 7:
                            		SMRecoPtMatchMode7BX0->Fill( F("reco_pt", ireco) ); 
                            		break; 
				case 9:
                            		SMRecoPtMatchMode9BX0->Fill( F("reco_pt", ireco) ); 
                            		break;
				case 6:
                            		SMRecoPtMatchMode6BX0->Fill( F("reco_pt", ireco) ); 
                            		break; 
				case 5:
                            		SMRecoPtMatchMode5BX0->Fill( F("reco_pt", ireco) ); 
                            		break;
				case 3:
                            		SMRecoPtMatchMode3BX0->Fill( F("reco_pt", ireco) ); 
                            		break;
                        	default:
                            		break;           
                        	}//end switch
				
				if( I("trk_dBX", I("reco_dR_match_iTrk", ireco) )  == 0 ){
					switch ( I("trk_mode", I("reco_dR_match_iTrk", ireco) ) ) {//all trk modes
                            
                        			case 15:
							SMRecoPtMatchMode15BX0dBX0->Fill( F("reco_pt", ireco) ); 
                            	        		break; 
						case 14:
                           				SMRecoPtMatchMode14BX0dBX0->Fill( F("reco_pt", ireco) ); 
                            				break;
		        			case 13:
	                                		SMRecoPtMatchMode13BX0dBX0->Fill( F("reco_pt", ireco) ); 
                            				break; 
						case 11:
                            				SMRecoPtMatchMode11BX0dBX0->Fill( F("reco_pt", ireco) ); 
                            				break;
						case 12:
                            				SMRecoPtMatchMode12BX0dBX0->Fill( F("reco_pt", ireco) ); 
                            				break; 
						case 10:
                            				SMRecoPtMatchMode10BX0dBX0->Fill( F("reco_pt", ireco) ); 
                            				break;
						case 7:
                            				SMRecoPtMatchMode7BX0dBX0->Fill( F("reco_pt", ireco) ); 
                            				break; 
						case 9:
                            				SMRecoPtMatchMode9BX0dBX0->Fill( F("reco_pt", ireco) ); 
                            				break;
						case 6:
                            				SMRecoPtMatchMode6BX0dBX0->Fill( F("reco_pt", ireco) ); 
                            				break; 
						case 5:
                            				SMRecoPtMatchMode5BX0dBX0->Fill( F("reco_pt", ireco) ); 
                            				break;
						case 3:
                            				SMRecoPtMatchMode3BX0dBX0->Fill( F("reco_pt", ireco) ); 
                            				break;
                        			default:
                            				break;           
                        			}//end switch
					
				}//require trk_dBX=0
				
				if( F("trk_pt", I("reco_dR_match_iTrk", ireco) )  >  F("reco_pt", ireco) * 7.0/8.0){
					switch ( I("trk_mode", I("reco_dR_match_iTrk", ireco) ) ) {//all trk modes
                            
                        			case 15:
							SMRecoPtMatchMode15BX0Plateau->Fill( F("reco_pt", ireco) ); 
                            	        		break; 
						case 14:
                           				SMRecoPtMatchMode14BX0Plateau->Fill( F("reco_pt", ireco) ); 
                            				break;
		        			case 13:
	                                		SMRecoPtMatchMode13BX0Plateau->Fill( F("reco_pt", ireco) ); 
                            				break; 
						case 11:
                            				SMRecoPtMatchMode11BX0Plateau->Fill( F("reco_pt", ireco) ); 
                            				break;
						case 12:
                            				SMRecoPtMatchMode12BX0Plateau->Fill( F("reco_pt", ireco) ); 
                            				break; 
						case 10:
                            				SMRecoPtMatchMode10BX0Plateau->Fill( F("reco_pt", ireco) ); 
                            				break;
						case 7:
                            				SMRecoPtMatchMode7BX0Plateau->Fill( F("reco_pt", ireco) ); 
                            				break; 
						case 9:
                            				SMRecoPtMatchMode9BX0Plateau->Fill( F("reco_pt", ireco) ); 
                            				break;
						case 6:
                            				SMRecoPtMatchMode6BX0Plateau->Fill( F("reco_pt", ireco) ); 
                            				break; 
						case 5:
                            				SMRecoPtMatchMode5BX0Plateau->Fill( F("reco_pt", ireco) ); 
                            				break;
						case 3:
                            				SMRecoPtMatchMode3BX0Plateau->Fill( F("reco_pt", ireco) ); 
                            				break;
                        			default:
                            				break;           
                        			}//end switch
					
				}//require trk_pt > reco_pt * (7/8)
				
			}//require trk_BX=0
			   
		   }//matched to unique EMTF trk
		   
	    }//selection on reco mu
    }//end loop over reco muons
    
  } // End loop events
  std::cout << "\n******* Finished looping over the SingleMu events *******" << std::endl;
  
  std::cout << "\n******* About to loop over the Zerobias events *******" << std::endl;
  int nEvents = ZB_in_chain->GetEntries();
  for (int iEvt = 0; iEvt < nEvents; iEvt++) {
    if (iEvt > MAX_EVT) break;
    if ( (iEvt % PRT_EVT) == 0 ) {
      std::cout << "\n*************************************" << std::endl;
      std::cout << "Looking at event " << iEvt << " out of " << nEvents << std::endl;
      std::cout << "*************************************" << std::endl;
    }

    ZB_in_chain->GetEntry(iEvt);
    
    // From Read_FlatNtuple.h, use 'I("branch_name")' to get an integer branch value, 'F("branch_name") to get a float
    // Print info for unpacked EMTF tracks
    if (verbose) std::cout << "\n" << I("nUnpTracks") << " unpacked tracks in the event" << std::endl;
    for (int itrack = 0; itrack < I("nUnpTracks"); itrack++) {
	     
    }//end loop over unpacked tracks
    
  } // End loop events
  std::cout << "\n******* Finished looping over the Zerobias events *******" << std::endl;

  delete SM_in_chain;
  delete ZB_in_chain;
  std::cout << "\nDone with macro. Exiting.\n" << std::endl;
	
  //write to output file
  TString outFile = "/afs/cern.ch/work/w/wshi/public/EMTFPileUp/ModesRateEff";
  outFile = outFile + "_eta_" + Form("%.2f", ETA_LOW) + "_" + Form("%.2f", ETA_UP) + ".root";
  TFile myPlot(outFile,"RECREATE");
        
  SMRecoPt->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtNoMatch->GetXaxis()->SetTitle("RECO pT[GeV]");
  SMRecoPtNoUniqueMatch->GetXaxis()->SetTitle("RECO pT[GeV]");
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
        
  myPlot.Close();
  
} // End function: void 
