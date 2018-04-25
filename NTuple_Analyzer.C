////////////////////////////////////////////////////////////
///             Adapted from                             ///
///      Simple macro to read EMTF Flat NTuples          ///
///      Andrew Brinkerhoff 29.09.17                     ///
///                                                      ///
///   TChain can be used to read multiple files.         ///
///   Format: interface/FlatNtupleBranches.h             ///
////////////////////////////////////////////////////////////

#include "TFile.h"
#include "TSystem.h"
#include "TChain.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH2.h"

#include "Read_FlatNtuple.h" // List of input branches and functions to return values

//USER modify here ONLY//
//================================================================
const int MAX_FILES = 1;   // Max number of files to process
const int MAX_EVT   = 100000;   // Max number of events to process
const int PRT_EVT   = 10000;   // Print every N events
const bool verbose  = true; // Print information about the event and RECO and L1T muon
const int PT_CUT = 22;
//================================================================

///////////////////////////////////////////
///  Main function to read the NTuples  ///
///////////////////////////////////////////

void NTuple_Analyzer() {
   
  // Initialize empty file to access each file in the list
  TFile *file_tmp(0);

  // List of input files
  std::vector<TString> in_file_names;
  TString store = "/afs/cern.ch/work/w/wshi/public/EMTFAnalyzer/CMSSW_10_1_1/src/EMTFAnalyzer";
  TString in_dir = "NTupleMaker/test";
  TString file_name;

  file_name.Form("%s/%s/EMTF_NTuple_314650.root", store.Data(), in_dir.Data());
  std::cout << "Adding file " << file_name.Data() << std::endl;
  in_file_names.push_back(file_name.Data());

  // Open all input files
  for (int i = 0; i < in_file_names.size(); i++) {
    if ( !gSystem->AccessPathName(in_file_names.at(i)) )
      file_tmp = TFile::Open( in_file_names.at(i) ); // Check if file exists
    if (!file_tmp) {
      std::cout << "ERROR: could not open data file " << in_file_names.at(i) << std::endl;
      return;
    }
  }

  // Add tree from the input files to the TChain
  TChain *in_chain= new TChain("FlatNtupleData/tree");
  for (int i = 0; i < in_file_names.size(); i++) {
    in_chain->Add( in_file_names.at(i) );
  }
 
  TH1F *ModeBX0 = new TH1F("ModeBX0", "ModeBX0", 15, 0, 15);
  TH1F *ModeBX1 = new TH1F("ModeBX1", "ModeBX1", 15, 0, 15);
  TH1F *ModeBX2 = new TH1F("ModeBX2", "ModeBX2", 15, 0, 15);
  TH1F *ModeBX3 = new TH1F("ModeBX3", "ModeBX3", 15, 0, 15);
  TH1F *ModeBXm1 = new TH1F("ModeBXm1", "ModeBXm1", 15, 0, 15);
  TH1F *ModeBXm2 = new TH1F("ModeBXm2", "ModeBXm2", 15, 0, 15);
  TH1F *ModeBXm3 = new TH1F("ModeBXm3", "ModeBXm3", 15, 0, 15);
  
  TH1F *CSCModeBX0 = new TH1F("CSCModeBX0", "CSCModeBX0", 15, 0, 15);
  TH1F *CSCModeBX1 = new TH1F("CSCModeBX1", "CSCModeBX1", 15, 0, 15);
  TH1F *CSCModeBX2 = new TH1F("CSCModeBX2", "CSCModeBX2", 15, 0, 15);
  TH1F *CSCModeBX3 = new TH1F("CSCModeBX3", "CSCModeBX3", 15, 0, 15);
  TH1F *CSCModeBXm1 = new TH1F("CSCModeBXm1", "CSCModeBXm1", 15, 0, 15);
  TH1F *CSCModeBXm2 = new TH1F("CSCModeBXm2", "CSCModeBXm2", 15, 0, 15);
  TH1F *CSCModeBXm3 = new TH1F("CSCModeBXm3", "CSCModeBXm3", 15, 0, 15);
	
  TH1F *RPCModeBX0 = new TH1F("RPCModeBX0", "RPCModeBX0", 15, 0, 15);
  TH1F *RPCModeBX1 = new TH1F("RPCModeBX1", "RPCModeBX1", 15, 0, 15);
  TH1F *RPCModeBX2 = new TH1F("RPCModeBX2", "RPCModeBX2", 15, 0, 15);
  TH1F *RPCModeBX3 = new TH1F("RPCModeBX3", "RPCModeBX3", 15, 0, 15);
  TH1F *RPCModeBXm1 = new TH1F("RPCModeBXm1", "RPCModeBXm1", 15, 0, 15);
  TH1F *RPCModeBXm2 = new TH1F("RPCModeBXm2", "RPCModeBXm2", 15, 0, 15);
  TH1F *RPCModeBXm3 = new TH1F("RPCModeBXm3", "RPCModeBXm3", 15, 0, 15);
	
  TH2F *UnpRPCvsCSCBX0 = new TH2F("UnpRPCvsCSCBX0", "UnpRPCvsCSCBX0", 15, 0, 15, 15, 0, 15);
  TH2F *UnpRPCvsCSCBX1 = new TH2F("UnpRPCvsCSCBX1", "UnpRPCvsCSCBX1", 15, 0, 15, 15, 0, 15);
  TH2F *UnpRPCvsCSCBX2 = new TH2F("UnpRPCvsCSCBX2", "UnpRPCvsCSCBX2", 15, 0, 15, 15, 0, 15);
  TH2F *UnpRPCvsCSCBX3 = new TH2F("UnpRPCvsCSCBX3", "UnpRPCvsCSCBX3", 15, 0, 15, 15, 0, 15);
  TH2F *UnpRPCvsCSCBXm1 = new TH2F("UnpRPCvsCSCBXm1", "UnpRPCvsCSCBXm1", 15, 0, 15, 15, 0, 15);
  TH2F *UnpRPCvsCSCBXm2 = new TH2F("UnpRPCvsCSCBXm2", "UnpRPCvsCSCBXm2", 15, 0, 15, 15, 0, 15);
  TH2F *UnpRPCvsCSCBXm3 = new TH2F("UnpRPCvsCSCBXm3", "UnpRPCvsCSCBXm3", 15, 0, 15, 15, 0, 15);
	
  TH2F *EmuRPCvsCSCBX0 = new TH2F("EmuRPCvsCSCBX0", "EmuRPCvsCSCBX0", 15, 0, 15, 15, 0, 15);
  TH2F *EmuRPCvsCSCBX1 = new TH2F("EmuRPCvsCSCBX1", "EmuRPCvsCSCBX1", 15, 0, 15, 15, 0, 15);
  TH2F *EmuRPCvsCSCBX2 = new TH2F("EmuRPCvsCSCBX2", "EmuRPCvsCSCBX2", 15, 0, 15, 15, 0, 15);
  TH2F *EmuRPCvsCSCBX3 = new TH2F("EmuRPCvsCSCBX3", "EmuRPCvsCSCBX3", 15, 0, 15, 15, 0, 15);
  TH2F *EmuRPCvsCSCBXm1 = new TH2F("EmuRPCvsCSCBXm1", "EmuRPCvsCSCBXm1", 15, 0, 15, 15, 0, 15);
  TH2F *EmuRPCvsCSCBXm2 = new TH2F("EmuRPCvsCSCBXm2", "EmuRPCvsCSCBXm2", 15, 0, 15, 15, 0, 15);
  TH2F *EmuRPCvsCSCBXm3 = new TH2F("EmuRPCvsCSCBXm3", "EmuRPCvsCSCBXm3", 15, 0, 15, 15, 0, 15);

  TH1F *PtBX0 = new TH1F("PtBX0", "PtBX0", 256, 0, 256);
  TH1F *PtBX1 = new TH1F("PtBX1", "PtBX1", 256, 0, 256);
  TH1F *PtBX2 = new TH1F("PtBX2", "PtBX2", 256, 0, 256);
  TH1F *PtBX3 = new TH1F("PtBX3", "PtBX3", 256, 0, 256);
  TH1F *PtBXm1 = new TH1F("PtBXm1", "PtBXm1", 256, 0, 256);
  TH1F *PtBXm2 = new TH1F("PtBXm2", "PtBXm2", 256, 0, 256);
  TH1F *PtBXm3 = new TH1F("PtBXm3", "PtBXm3", 256, 0, 256);

  TH1F *EtaBX0 = new TH1F("EtaBX0", "EtaBX0", 120, -3, 3);
  TH1F *EtaBX1 = new TH1F("EtaBX1", "EtaBX1", 120, -3, 3);
  TH1F *EtaBX2 = new TH1F("EtaBX2", "EtaBX2", 120, -3, 3);
  TH1F *EtaBX3 = new TH1F("EtaBX3", "EtaBX3", 120, -3, 3);
  TH1F *EtaBXm1 = new TH1F("EtaBXm1", "EtaBXm1", 120, -3, 3);
  TH1F *EtaBXm2 = new TH1F("EtaBXm2", "EtaBXm2", 120, -3, 3);
  TH1F *EtaBXm3 = new TH1F("EtaBXm3", "EtaBXm3", 120, -3, 3);
	
  TH1F *PhiBX0 = new TH1F("PhiBX0", "PhiBX0", 100, -200, 200);
  TH1F *PhiBX1 = new TH1F("PhiBX1", "PhiBX1", 100, -200, 200);
  TH1F *PhiBX2 = new TH1F("PhiBX2", "PhiBX2", 100, -200, 200);
  TH1F *PhiBX3 = new TH1F("PhiBX3", "PhiBX3", 100, -200, 200);
  TH1F *PhiBXm1 = new TH1F("PhiBXm1", "PhiBXm1", 100, -200, 200);
  TH1F *PhiBXm2 = new TH1F("PhiBXm2", "PhiBXm2", 100, -200, 200);
  TH1F *PhiBXm3 = new TH1F("PhiBXm3", "PhiBXm3", 100, -200, 200);
	
  TH1F *dBXinBX0Trk = new TH1F("dBXinBX0Trk", "dBXinBX0Trk", 4, 0, 4);
  TH1F *dBXinBX1Trk = new TH1F("dBXinBX1Trk", "dBXinBX1Trk", 4, 0, 4);
  TH1F *dBXinBX2Trk = new TH1F("dBXinBX2Trk", "dBXinBX2Trk", 4, 0, 4);
  TH1F *dBXinBX3Trk = new TH1F("dBXinBX3Trk", "dBXinBX3Trk", 4, 0, 4);
  TH1F *dBXinBXm1Trk = new TH1F("dBXinBXm1Trk", "dBXinBXm1Trk", 4, 0, 4);
  TH1F *dBXinBXm2Trk = new TH1F("dBXinBXm2Trk", "dBXinBXm2Trk", 4, 0, 4);
  TH1F *dBXinBXm3Trk = new TH1F("dBXinBXm3Trk", "dBXinBXm3Trk", 4, 0, 4);
  
  InitializeMaps();
  SetBranchAddresses(in_chain);
	
  std::cout << "\n******* About to loop over the events *******" << std::endl;
  int nEvents = in_chain->GetEntries();
  for (int iEvt = 0; iEvt < nEvents; iEvt++) {
    if (iEvt > MAX_EVT && MAX_EVT!=-1) break;
    if ( (iEvt % PRT_EVT) == 0 ) {
      std::cout << "\n*************************************" << std::endl;
      std::cout << "Looking at event " << iEvt << " out of " << nEvents << std::endl;
      std::cout << "*************************************" << std::endl;
    }

    in_chain->GetEntry(iEvt);
    
    // From Read_FlatNtuple.h, use 'I("branch_name")' to get an integer branch value, 'F("branch_name") to get a float
    if (verbose) std::cout << "\n" << I("nUnpTracks") << " unpacked tracks in the event" << std::endl;
    for (int itrack = 0; itrack < I("nUnpTracks"); itrack++) {
	    if( F("unp_trk_pt", itrack) > PT_CUT ){
		    
		    switch ( I("unp_trk_BX", itrack) ) {
				    
		        case 0:
			    PtBX0->Fill( F("unp_trk_pt",itrack) );
			    EtaBX0->Fill( F("unp_trk_eta",itrack) );
			    PhiBX0->Fill( F("unp_trk_phi",itrack) );
		            dBXinBX0Trk->Fill( I("unp_trk_dBX", itrack) );
                            ModeBX0->Fill( I("unp_trk_mode", itrack) );
		            CSCModeBX0->Fill( I("unp_trk_mode_CSC", itrack) );
		            RPCModeBX0->Fill( I("unp_trk_mode_RPC", itrack) );
			    UnpRPCvsCSCBX0->Fill( I("unp_trk_mode_CSC", itrack), I("unp_trk_mode_RPC", itrack));
                            break; 
                        case 1:
			    PtBX1->Fill( F("unp_trk_pt",itrack) );
			    EtaBX1->Fill( F("unp_trk_eta",itrack) );
			    PhiBX1->Fill( F("unp_trk_phi",itrack) );
			    dBXinBX1Trk->Fill( I("unp_trk_dBX", itrack) );
                            ModeBX1->Fill( I("unp_trk_mode", itrack) );
		            CSCModeBX1->Fill( I("unp_trk_mode_CSC", itrack) );
		            RPCModeBX1->Fill( I("unp_trk_mode_RPC", itrack) );
			    UnpRPCvsCSCBX1->Fill( I("unp_trk_mode_CSC", itrack), I("unp_trk_mode_RPC", itrack));
				    
		            //Print event#, emulated/unpacked tracks/hits
		            cout<<"*** Print out ***"<<endl;
                            cout<<"* Event #"<<I("evt_event",iEvt)<<" * LS #"<<I("evt_LS", iEvt)<<" * BX #"<<I("evt_BX", iEvt)<<" * Orbit #"<<I("evt_orbit",iEvt)<<endl;
                            
			    cout<<"========================== Emulated Tracks ============================"<<endl;
                            cout<<"***********************************************************************"<<endl;
			    cout<<"* BX * dBX * Endcap * Sector * Eta * Phi *  Pt  * CSC Mode * RPC Mode *"<<endl;
			    for (int jtrack = 0; jtrack < I("nTracks"); jtrack++) {
				    cout<< "* "<<setw(2)<<setprecision(2)<< I("trk_BX", jtrack) << " * " <<setw(3)<<setprecision(2)<< I("trk_dBX", jtrack) << " * " 
					       <<setw(6)<<setprecision(2)<< I("trk_endcap", jtrack) <<" * "<<setw(6)<<setprecision(2)<< I("trk_sector", jtrack) << " * "
					       <<setw(3)<<setprecision(3)<< F("trk_eta", jtrack) <<" * "<<setw(3)<<setprecision(3)<< F("trk_phi", jtrack) << " * "
					       <<setw(4)<<setprecision(3)<< F("trk_pt", jtrack) <<" * "<<setw(8)<<setprecision(2)<< I("trk_mode_CSC", jtrack) << " * "
					       <<setw(8)<<setprecision(2)<< I("trk_mode_RPC", jtrack)<<" *"<<endl;                         
			    }
                            cout<<"***********************************************************************"<<endl;
		            
			    cout<<"========================== Unpacked Tracks ============================"<<endl;
                            cout<<"***********************************************************************"<<endl;
			    cout<<"* BX * dBX * Endcap * Sector * Eta * Phi *  Pt  * CSC Mode * RPC Mode *"<<endl;
			    for (int jtrack = 0; jtrack < I("nUnpTracks"); jtrack++) {
				    cout<< "* "<<setw(2)<<setprecision(2)<< I("unp_trk_BX", jtrack) << " * " <<setw(3)<<setprecision(2)<< I("unp_trk_dBX", jtrack) << " * " 
					       <<setw(6)<<setprecision(2)<< I("unp_trk_endcap", jtrack) <<" * "<<setw(6)<<setprecision(2)<< I("unp_trk_sector", jtrack) << " * "
					       <<setw(3)<<setprecision(3)<< F("unp_trk_eta", jtrack) <<" * "<<setw(3)<<setprecision(3)<< F("unp_trk_phi", jtrack) << " * "
					       <<setw(4)<<setprecision(3)<< F("unp_trk_pt", jtrack) <<" * "<<setw(8)<<setprecision(2)<< I("unp_trk_mode_CSC", jtrack) << " * "
					       <<setw(8)<<setprecision(2)<< I("unp_trk_mode_RPC", jtrack)<<" *"<<endl;                         
			    }
                            cout<<"***********************************************************************"<<endl;
				
			    cout<<"============================ Emulated Hits ===================================="<<endl;
                            cout<<"*******************************************************************************"<<endl;
			    cout<<"* BX * isCSC * isRPC * Endcap * Sector * Station * Ring * Chamber * Eta * Phi *"<<endl;
			    for (int jhit = 0; jhit < I("nSimHits"); jhit++) {
				    cout<< "* "<<setw(2)<<setprecision(2)<< I("sim_hit_BX", jhit) << " * " <<setw(5)<<setprecision(2)<< I("sim_hit_isCSC", jhit) << " * " 
					       <<setw(5)<<setprecision(2)<< I("sim_hit_isRPC", jhit) <<" * " <<setw(6)<<setprecision(2)<< I("sim_hit_endcap", jhit) <<" * "
					       <<setw(6)<<setprecision(2)<< I("sim_hit_sector", jhit) << " * "<<setw(7)<<setprecision(2)<< I("sim_hit_station", jhit) << " * "
					       <<setw(4)<<setprecision(2)<< I("sim_hit_ring", jhit) << " * "<<setw(7)<<setprecision(2)<< I("sim_hit_chamber", jhit) << " * "
					       <<setw(3)<<setprecision(3)<< F("sim_hit_eta", jhit) <<" * "<<setw(3)<<setprecision(3)<< F("sim_hit_phi", jhit) << " *"<<endl;                         
			    }
                            cout<<"*******************************************************************************"<<endl;
				    
			    cout<<"============================ Unpacked Hits ===================================="<<endl;
                            cout<<"*******************************************************************************"<<endl;
			    cout<<"* BX * isCSC * isRPC * Endcap * Sector * Station * Ring * Chamber * Eta * Phi *"<<endl;
			    for (int jhit = 0; jhit < I("nHits"); jhit++) {
				    cout<< "* "<<setw(2)<<setprecision(2)<< I("hit_BX", jhit) << " * " <<setw(5)<<setprecision(2)<< I("hit_isCSC", jhit) << " * " 
					       <<setw(5)<<setprecision(2)<< I("hit_isRPC", jhit) <<" * " <<setw(6)<<setprecision(2)<< I("hit_endcap", jhit) <<" * "
					       <<setw(6)<<setprecision(2)<< I("hit_sector", jhit) << " * "<<setw(7)<<setprecision(2)<< I("hit_station", jhit) << " * "
					       <<setw(4)<<setprecision(2)<< I("hit_ring", jhit) << " * "<<setw(7)<<setprecision(2)<< I("hit_chamber", jhit) << " * "
					       <<setw(3)<<setprecision(3)<< F("hit_eta", jhit) <<" * "<<setw(3)<<setprecision(3)<< F("hit_phi", jhit) << " *"<<endl;                         
			    }
                            cout<<"*******************************************************************************"<<endl;
				    
                            break; 
			case 2:
			    PtBX2->Fill( F("unp_trk_pt",itrack) );
			    EtaBX2->Fill( F("unp_trk_eta",itrack) );
			    PhiBX2->Fill( F("unp_trk_phi",itrack) );
			    dBXinBX2Trk->Fill( I("unp_trk_dBX", itrack) );
                            ModeBX2->Fill( I("unp_trk_mode", itrack) );
		            CSCModeBX2->Fill( I("unp_trk_mode_CSC", itrack) );
		            RPCModeBX2->Fill( I("unp_trk_mode_RPC", itrack) );  
			    UnpRPCvsCSCBX2->Fill( I("unp_trk_mode_CSC", itrack), I("unp_trk_mode_RPC", itrack));
                            break;
			case 3:
			    PtBX3->Fill( F("unp_trk_pt",itrack) );
			    EtaBX3->Fill( F("trk_eta",itrack) );
			    PhiBX3->Fill( F("trk_phi",itrack) );
			    dBXinBX3Trk->Fill( I("unp_trk_dBX", itrack) );
			    ModeBX3->Fill( I("trk_mode", itrack) );
		            CSCModeBX3->Fill( I("trk_mode_CSC", itrack) );
		            RPCModeBX3->Fill( I("trk_mode_RPC", itrack) );
			    UnpRPCvsCSCBX3->Fill( I("unp_trk_mode_CSC", itrack), I("unp_trk_mode_RPC", itrack));
                            break;
		        case -1:
			    PtBXm1->Fill( F("unp_trk_pt",itrack) );
			    EtaBXm1->Fill( F("trk_eta",itrack) );
			    PhiBXm1->Fill( F("trk_phi",itrack) );
			    dBXinBXm1Trk->Fill( I("unp_trk_dBX", itrack) );
                            ModeBXm1->Fill(I("trk_mode", itrack));
		            CSCModeBXm1->Fill( I("trk_mode_CSC", itrack) );
		            RPCModeBXm1->Fill( I("trk_mode_RPC", itrack) );
			    UnpRPCvsCSCBXm1->Fill( I("unp_trk_mode_CSC", itrack), I("unp_trk_mode_RPC", itrack));
                            break; 
			case -2:
			    PtBXm2->Fill( F("unp_trk_pt",itrack) );
			    EtaBXm2->Fill( F("trk_eta",itrack) );
			    PhiBXm2->Fill( F("trk_phi",itrack) );
			    dBXinBXm2Trk->Fill( I("unp_trk_dBX", itrack) );
                            ModeBXm2->Fill(I("trk_mode", itrack));
		            CSCModeBXm2->Fill( I("trk_mode_CSC", itrack) );
		            RPCModeBXm2->Fill( I("trk_mode_RPC", itrack) );
			    UnpRPCvsCSCBXm2->Fill( I("unp_trk_mode_CSC", itrack), I("unp_trk_mode_RPC", itrack));
                            break;
			case -3:
			    PtBXm3->Fill( F("unp_trk_pt",itrack) );
			    EtaBXm3->Fill( F("trk_eta",itrack) );
			    PhiBXm3->Fill( F("trk_phi",itrack) );
			    dBXinBXm3Trk->Fill( I("unp_trk_dBX", itrack) );
			    ModeBXm3->Fill(I("trk_mode", itrack));
		            CSCModeBXm3->Fill( I("trk_mode_CSC", itrack) );
		            RPCModeBXm3->Fill( I("trk_mode_RPC", itrack) );
			    UnpRPCvsCSCBXm3->Fill( I("unp_trk_mode_CSC", itrack), I("unp_trk_mode_RPC", itrack));
                            break;
		        default:
                            break;
		    }//end switch
	    
	    }//end if high pT
    
    }//end loop over unpacked tracks
	  
    if (verbose) std::cout << "\n" << I("nTracks") << " emulated tracks in the event" << std::endl;
    for (int itrack = 0; itrack < I("nTracks"); itrack++) {
	    if( F("trk_pt", itrack) > PT_CUT ){
		    
		    switch ( I("trk_BX", itrack) ) {
				    
		        case 0:
			    EmuRPCvsCSCBX0->Fill( I("trk_mode_CSC", itrack), I("trk_mode_RPC", itrack));
                            break; 
                        case 1:
			    EmuRPCvsCSCBX1->Fill( I("trk_mode_CSC", itrack), I("trk_mode_RPC", itrack));
                            break; 
			case 2:
			    EmuRPCvsCSCBX2->Fill( I("trk_mode_CSC", itrack), I("trk_mode_RPC", itrack));
                            break;
			case 3:
			    EmuRPCvsCSCBX3->Fill( I("trk_mode_CSC", itrack), I("trk_mode_RPC", itrack));
                            break;
		        case -1:
			    EmuRPCvsCSCBXm1->Fill( I("trk_mode_CSC", itrack), I("trk_mode_RPC", itrack));
                            break; 
			case -2:
			    EmuRPCvsCSCBXm2->Fill( I("trk_mode_CSC", itrack), I("trk_mode_RPC", itrack));
                            break;
			case -3:
			    EmuRPCvsCSCBXm3->Fill( I("trk_mode_CSC", itrack), I("trk_mode_RPC", itrack));
                            break;
		        default:
                            break;
		    }//end switch
				    
	    }//end if high pT
	     
    }//end loop over emulated tracks
    
  } // End loop events
  std::cout << "\n******* Finished looping over the events *******" << std::endl;

  delete in_chain;
  std::cout << "\nDone with dThetaWindow(). Exiting.\n" << std::endl;
	
  //write to output file
  TString outFile = "/afs/cern.ch/work/w/wshi/public/EMTFAnalyzer/CMSSW_10_1_1/src/EMTFAnalyzer/NTupleMaker/test/Output_314650.root";
  TFile myPlot(outFile,"RECREATE");
  
  PtBX0->GetXaxis()->SetTitle("pT[GeV]");
  PtBX1->GetXaxis()->SetTitle("pT[GeV]");
  PtBX2->GetXaxis()->SetTitle("pT[GeV]");
  PtBX3->GetXaxis()->SetTitle("pT[GeV]");
  PtBXm1->GetXaxis()->SetTitle("pT[GeV]");
  PtBXm2->GetXaxis()->SetTitle("pT[GeV]");
  PtBXm3->GetXaxis()->SetTitle("pT[GeV]");
  
  EtaBX0->GetXaxis()->SetTitle("Eta");
  EtaBX1->GetXaxis()->SetTitle("Eta");
  EtaBX2->GetXaxis()->SetTitle("Eta");
  EtaBX3->GetXaxis()->SetTitle("Eta");
  EtaBXm1->GetXaxis()->SetTitle("Eta");
  EtaBXm2->GetXaxis()->SetTitle("Eta");
  EtaBXm3->GetXaxis()->SetTitle("Eta");
	
  PhiBX0->GetXaxis()->SetTitle("Phi");
  PhiBX1->GetXaxis()->SetTitle("Phi");
  PhiBX2->GetXaxis()->SetTitle("Phi");
  PhiBX3->GetXaxis()->SetTitle("Phi");
  PhiBXm1->GetXaxis()->SetTitle("Phi");
  PhiBXm2->GetXaxis()->SetTitle("Phi");
  PhiBXm3->GetXaxis()->SetTitle("Phi");
	
  dBXinBX0Trk->GetXaxis()->SetTitle("dBX");
  dBXinBX1Trk->GetXaxis()->SetTitle("dBX");
  dBXinBX2Trk->GetXaxis()->SetTitle("dBX");
  dBXinBX3Trk->GetXaxis()->SetTitle("dBX");
  dBXinBXm1Trk->GetXaxis()->SetTitle("dBX");
  dBXinBXm2Trk->GetXaxis()->SetTitle("dBX");
  dBXinBXm3Trk->GetXaxis()->SetTitle("dBX");
	
  ModeBX0->GetXaxis()->SetTitle("Mode");
  ModeBX1->GetXaxis()->SetTitle("Mode");
  ModeBX2->GetXaxis()->SetTitle("Mode");
  ModeBX3->GetXaxis()->SetTitle("Mode");
  ModeBXm1->GetXaxis()->SetTitle("Mode");
  ModeBXm2->GetXaxis()->SetTitle("Mode");
  ModeBXm3->GetXaxis()->SetTitle("Mode");
	
  CSCModeBX0->GetXaxis()->SetTitle("CSC Mode");
  CSCModeBX1->GetXaxis()->SetTitle("CSC Mode");
  CSCModeBX2->GetXaxis()->SetTitle("CSC Mode");
  CSCModeBX3->GetXaxis()->SetTitle("CSC Mode");
  CSCModeBXm1->GetXaxis()->SetTitle("CSC Mode");
  CSCModeBXm2->GetXaxis()->SetTitle("CSC Mode");
  CSCModeBXm3->GetXaxis()->SetTitle("CSC Mode");
	
  RPCModeBX0->GetXaxis()->SetTitle("RPC Mode");
  RPCModeBX1->GetXaxis()->SetTitle("RPC Mode");
  RPCModeBX2->GetXaxis()->SetTitle("RPC Mode");
  RPCModeBX3->GetXaxis()->SetTitle("RPC Mode");
  RPCModeBXm1->GetXaxis()->SetTitle("RPC Mode");
  RPCModeBXm2->GetXaxis()->SetTitle("RPC Mode");
  RPCModeBXm3->GetXaxis()->SetTitle("RPC Mode");

  UnpRPCvsCSCBX0->GetXaxis()->SetTitle("CSC Mode");
  UnpRPCvsCSCBX0->GetYaxis()->SetTitle("RPC Mode");
  UnpRPCvsCSCBX1->GetXaxis()->SetTitle("CSC Mode");
  UnpRPCvsCSCBX1->GetYaxis()->SetTitle("RPC Mode");
  UnpRPCvsCSCBX2->GetXaxis()->SetTitle("CSC Mode");
  UnpRPCvsCSCBX2->GetYaxis()->SetTitle("RPC Mode");
  UnpRPCvsCSCBX3->GetXaxis()->SetTitle("CSC Mode");
  UnpRPCvsCSCBX3->GetYaxis()->SetTitle("RPC Mode");
  UnpRPCvsCSCBXm1->GetXaxis()->SetTitle("CSC Mode");
  UnpRPCvsCSCBXm1->GetYaxis()->SetTitle("RPC Mode");
  UnpRPCvsCSCBXm2->GetXaxis()->SetTitle("CSC Mode");
  UnpRPCvsCSCBXm2->GetYaxis()->SetTitle("RPC Mode");
  UnpRPCvsCSCBXm3->GetXaxis()->SetTitle("CSC Mode");
  UnpRPCvsCSCBXm3->GetYaxis()->SetTitle("RPC Mode");
	
  EmuRPCvsCSCBX0->GetXaxis()->SetTitle("CSC Mode");
  EmuRPCvsCSCBX0->GetYaxis()->SetTitle("RPC Mode");
  EmuRPCvsCSCBX1->GetXaxis()->SetTitle("CSC Mode");
  EmuRPCvsCSCBX1->GetYaxis()->SetTitle("RPC Mode");
  EmuRPCvsCSCBX2->GetXaxis()->SetTitle("CSC Mode");
  EmuRPCvsCSCBX2->GetYaxis()->SetTitle("RPC Mode");
  EmuRPCvsCSCBX3->GetXaxis()->SetTitle("CSC Mode");
  EmuRPCvsCSCBX3->GetYaxis()->SetTitle("RPC Mode");
  EmuRPCvsCSCBXm1->GetXaxis()->SetTitle("CSC Mode");
  EmuRPCvsCSCBXm1->GetYaxis()->SetTitle("RPC Mode");
  EmuRPCvsCSCBXm2->GetXaxis()->SetTitle("CSC Mode");
  EmuRPCvsCSCBXm2->GetYaxis()->SetTitle("RPC Mode");
  EmuRPCvsCSCBXm3->GetXaxis()->SetTitle("CSC Mode");
  EmuRPCvsCSCBXm3->GetYaxis()->SetTitle("RPC Mode");
	
  PtBX0->Write();
  PtBX1->Write();
  PtBX2->Write();
  PtBX3->Write();
  PtBXm1->Write();
  PtBXm2->Write();
  PtBXm3->Write();
	
  EtaBX0->Write();
  EtaBX1->Write();
  EtaBX2->Write();
  EtaBX3->Write();
  EtaBXm1->Write();
  EtaBXm2->Write();
  EtaBXm3->Write();
	
  PhiBX0->Write();
  PhiBX1->Write();
  PhiBX2->Write();
  PhiBX3->Write();
  PhiBXm1->Write();
  PhiBXm2->Write();
  PhiBXm3->Write();
	
  dBXinBX0Trk->Write();
  dBXinBX1Trk->Write();
  dBXinBX2Trk->Write();
  dBXinBX3Trk->Write();
  dBXinBXm1Trk->Write();
  dBXinBXm2Trk->Write();
  dBXinBXm3Trk->Write();
  
  ModeBX0->Write();
  ModeBX1->Write();
  ModeBX2->Write();
  ModeBX3->Write();
  ModeBXm1->Write();
  ModeBXm2->Write();
  ModeBXm3->Write();
	
  CSCModeBX0->Write();
  CSCModeBX1->Write();
  CSCModeBX2->Write();
  CSCModeBX3->Write();
  CSCModeBXm1->Write();
  CSCModeBXm2->Write();
  CSCModeBXm3->Write();

  RPCModeBX0->Write();
  RPCModeBX1->Write();
  RPCModeBX2->Write();
  RPCModeBX3->Write();
  RPCModeBXm1->Write();
  RPCModeBXm2->Write();
  RPCModeBXm3->Write();
	
  UnpRPCvsCSCBX0->Write();
  UnpRPCvsCSCBX1->Write();
  UnpRPCvsCSCBX2->Write();
  UnpRPCvsCSCBX3->Write();
  UnpRPCvsCSCBXm1->Write();
  UnpRPCvsCSCBXm2->Write();
  UnpRPCvsCSCBXm3->Write();

  EmuRPCvsCSCBX0->Write();
  EmuRPCvsCSCBX1->Write();
  EmuRPCvsCSCBX2->Write();
  EmuRPCvsCSCBX3->Write();
  EmuRPCvsCSCBXm1->Write();
  EmuRPCvsCSCBXm2->Write();
  EmuRPCvsCSCBXm3->Write();
	
  myPlot.Close();
  
} // End function: void NTuple_Analyzer()
