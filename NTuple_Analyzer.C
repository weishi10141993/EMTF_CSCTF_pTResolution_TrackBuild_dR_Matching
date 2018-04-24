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
const int MAX_EVT   = -1;   // Max number of events to process
const int PRT_EVT   = 100000;   // Print every N events
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
  TH1F *ModeBX3 = new TH1F("ModeBX1", "ModeBX1", 15, 0, 15);
  TH1F *ModeBXm1 = new TH1F("ModeBXm1", "ModeBXm1", 15, 0, 15);
  TH1F *ModeBXm2 = new TH1F("ModeBXm2", "ModeBXm2", 15, 0, 15);
  TH1F *ModeBXm3 = new TH1F("ModeBXm3", "ModeBXm3", 15, 0, 15);
  
  TH1F *CSCModeBX0 = new TH1F("CSCModeBX0", "CSCModeBX0", 15, 0, 15);
  TH1F *CSCModeBX1 = new TH1F("CSCModeBX1", "CSCModeBX1", 15, 0, 15);
  TH1F *CSCModeBX2 = new TH1F("CSCModeBX2", "CSCModeBX2", 15, 0, 15);
  TH1F *CSCModeBX3 = new TH1F("CSCModeBX1", "CSCModeBX1", 15, 0, 15);
  TH1F *CSCModeBXm1 = new TH1F("CSCModeBXm1", "CSCModeBXm1", 15, 0, 15);
  TH1F *CSCModeBXm2 = new TH1F("CSCModeBXm2", "CSCModeBXm2", 15, 0, 15);
  TH1F *CSCModeBXm3 = new TH1F("CSCModeBXm3", "CSCModeBXm3", 15, 0, 15);
	
  TH1F *RPCModeBX0 = new TH1F("RPCModeBX0", "RPCModeBX0", 15, 0, 15);
  TH1F *RPCModeBX1 = new TH1F("RPCModeBX1", "RPCModeBX1", 15, 0, 15);
  TH1F *RPCModeBX2 = new TH1F("RPCModeBX2", "RPCModeBX2", 15, 0, 15);
  TH1F *RPCModeBX3 = new TH1F("RPCModeBX1", "RPCModeBX1", 15, 0, 15);
  TH1F *RPCModeBXm1 = new TH1F("RPCModeBXm1", "RPCModeBXm1", 15, 0, 15);
  TH1F *RPCModeBXm2 = new TH1F("RPCModeBXm2", "RPCModeBXm2", 15, 0, 15);
  TH1F *RPCModeBXm3 = new TH1F("RPCModeBXm3", "RPCModeBXm3", 15, 0, 15);
  
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
    if (verbose) std::cout << "\n" << I("nTracks") << " tracks in the event" << std::endl;
    for (int itrack = 0; itrack < I("nTracks"); itrack++) {
	    if( I("trk_pt", itrack)> PT_CUT ){
		    
		    switch ( I("trk_BX", itrack) ) {
				    
		        case 0:
                            ModeBX0->Fill(I("trk_mode", itrack));
		            CSCModeBX0->Fill("trk_mode_CSC", itrack);
		            RPCModeBX0->Fill("trk_mode_RPC", itrack);
                            break; 
                        case 1:
                            ModeBX1->Fill(I("trk_mode", itrack));
		            CSCModeBX1->Fill("trk_mode_CSC", itrack);
		            RPCModeBX1->Fill("trk_mode_RPC", itrack);
                            break; 
			case 2:
                            ModeBX2->Fill(I("trk_mode", itrack));
		            CSCModeBX2->Fill("trk_mode_CSC", itrack);
		            RPCModeBX2->Fill("trk_mode_RPC", itrack);
                            break;
			case 3:
			    ModeBX3->Fill(I("trk_mode", itrack));
		            CSCModeBX3->Fill("trk_mode_CSC", itrack);
		            RPCModeBX3->Fill("trk_mode_RPC", itrack);
                            break;
		        case -1:
                            ModeBXm1->Fill(I("trk_mode", itrack));
		            CSCModeBXm1->Fill("trk_mode_CSC", itrack);
		            RPCModeBXm1->Fill("trk_mode_RPC", itrack);
                            break; 
			case -2:
                            ModeBXm2->Fill(I("trk_mode", itrack));
		            CSCModeBXm2->Fill("trk_mode_CSC", itrack);
		            RPCModeBXm2->Fill("trk_mode_RPC", itrack);
                            break;
			case -3:
			    ModeBXm3->Fill(I("trk_mode", itrack));
		            CSCModeBXm3->Fill("trk_mode_CSC", itrack);
		            RPCModeBXm3->Fill("trk_mode_RPC", itrack);
                            break;
		        default:
                            break;
		    }//end switch
				    
	    }//end if trk BX!=0 && high pT
	    
	     
    }//end loop over tracks
    
  } // End loop events
  std::cout << "\n******* Finished looping over the events *******" << std::endl;

  delete in_chain;
  std::cout << "\nDone with dThetaWindow(). Exiting.\n" << std::endl;
	
  //write to output file
  TString outFile = "/afs/cern.ch/work/w/wshi/public/EMTFAnalyzer/CMSSW_10_1_1/src/EMTFAnalyzer/NTupleMaker/test/Output_314650.root";
  TFile myPlot(outFile,"RECREATE");
  
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
        
  myPlot.Close();
  
} // End function: void NTuple_Analyzer()
