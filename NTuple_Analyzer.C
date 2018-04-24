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

  TH1F *PtBX0 = new TH1F("PtBX0", "PtBX0", 100, 0, 100);
  TH1F *PtBX1 = new TH1F("PtBX1", "PtBX1", 100, 0, 100);
  TH1F *PtBX2 = new TH1F("PtBX2", "PtBX2", 100, 0, 100);
  TH1F *PtBX3 = new TH1F("PtBX3", "PtBX3", 100, 0, 100);
  TH1F *PtBXm1 = new TH1F("PtBXm1", "PtBXm1", 100, 0, 100);
  TH1F *PtBXm2 = new TH1F("PtBXm2", "PtBXm2", 100, 0, 100);
  TH1F *PtBXm3 = new TH1F("PtBXm3", "PtBXm3", 100, 0, 100);

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
	
  TH1F *CSCHitBXinBX0Trk = new TH1F("CSCHitBXinBX0Trk", "CSCHitBXinBX0Trk", 8, -4, 4);
  TH1F *CSCHitBXinBX1Trk = new TH1F("CSCHitBXinBX1Trk", "CSCHitBXinBX1Trk", 8, -4, 4);
  TH1F *CSCHitBXinBX2Trk = new TH1F("CSCHitBXinBX2Trk", "CSCHitBXinBX2Trk", 8, -4, 4);
  TH1F *CSCHitBXinBX3Trk = new TH1F("CSCHitBXinBX3Trk", "CSCHitBXinBX3Trk", 8, -4, 4);
  TH1F *CSCHitBXinBXm1Trk = new TH1F("CSCHitBXinBXm1Trk", "CSCHitBXinBXm1Trk", 8, -4, 4);
  TH1F *CSCHitBXinBXm2Trk = new TH1F("CSCHitBXinBXm2Trk", "CSCHitBXinBXm2Trk", 8, -4, 4);
  TH1F *CSCHitBXinBXm3Trk = new TH1F("CSCHitBXinBXm3Trk", "CSCHitBXinBXm3Trk", 8, -4, 4);
	
  TH1F *RPCHitBXinBX0Trk = new TH1F("RPCHitBXinBX0Trk", "RPCHitBXinBX0Trk", 8, -4, 4);
  TH1F *RPCHitBXinBX1Trk = new TH1F("RPCHitBXinBX1Trk", "RPCHitBXinBX1Trk", 8, -4, 4);
  TH1F *RPCHitBXinBX2Trk = new TH1F("RPCHitBXinBX2Trk", "RPCHitBXinBX2Trk", 8, -4, 4);
  TH1F *RPCHitBXinBX3Trk = new TH1F("RPCHitBXinBX3Trk", "RPCHitBXinBX3Trk", 8, -4, 4);
  TH1F *RPCHitBXinBXm1Trk = new TH1F("RPCHitBXinBXm1Trk", "RPCHitBXinBXm1Trk", 8, -4, 4);
  TH1F *RPCHitBXinBXm2Trk = new TH1F("RPCHitBXinBXm2Trk", "RPCHitBXinBXm2Trk", 8, -4, 4);
  TH1F *RPCHitBXinBXm3Trk = new TH1F("RPCHitBXinBXm3Trk", "RPCHitBXinBXm3Trk", 8, -4, 4);
  
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
	    if( I("trk_pt", itrack) > PT_CUT ){
		    
		    switch ( I("trk_BX", itrack) ) {
				    
		        case 0:
			    PtBX0->Fill( F("trk_pt",itrack) );
			    EtaBX0->Fill( F("trk_eta",itrack) );
			    PhiBX0->Fill( F("trk_phi",itrack) );
                            ModeBX0->Fill( I("trk_mode", itrack) );
		            CSCModeBX0->Fill( I("trk_mode_CSC", itrack) );
		            RPCModeBX0->Fill( I("trk_mode_RPC", itrack) );
		            for (int jhit = 0; jhit < I("trk_nHits", itrack); jhit++) {
				    int iHit = I("trk_iHit", itrack, jhit); 
				    if(I("hit_isCSC", iHit)==1){
					    CSCHitBXinBX0Trk->Fill( I("hit_BX", iHit) );
				    }
				    if(I("hit_isRPC", iHit)==1){
					    RPCHitBXinBX0Trk->Fill( I("hit_BX", iHit) );
				    }
					    
			    }//end loop over hits
                            break; 
                        case 1:
			    PtBX1->Fill( F("trk_pt",itrack) );
			    EtaBX1->Fill( F("trk_eta",itrack) );
			    PhiBX1->Fill( F("trk_phi",itrack) );
                            ModeBX1->Fill( I("trk_mode", itrack) );
		            CSCModeBX1->Fill( I("trk_mode_CSC", itrack) );
		            RPCModeBX1->Fill( I("trk_mode_RPC", itrack) );
			    for (int jhit = 0; jhit < I("trk_nHits", itrack); jhit++) {
				    int iHit = I("trk_iHit", itrack, jhit); 
				    if(I("hit_isCSC", iHit)==1){
					    CSCHitBXinBX1Trk->Fill( I("hit_BX", iHit) );
				    }
				    if(I("hit_isRPC", iHit)==1){
					    RPCHitBXinBX1Trk->Fill( I("hit_BX", iHit) );
				    }
					    
			    }//end loop over hits
                            break; 
			case 2:
			    PtBX2->Fill( F("trk_pt",itrack) );
			    EtaBX2->Fill( F("trk_eta",itrack) );
			    PhiBX2->Fill( F("trk_phi",itrack) );
                            ModeBX2->Fill( I("trk_mode", itrack) );
		            CSCModeBX2->Fill( I("trk_mode_CSC", itrack) );
		            RPCModeBX2->Fill( I("trk_mode_RPC", itrack) );
		            for (int jhit = 0; jhit < I("trk_nHits", itrack); jhit++) {
				    int iHit = I("trk_iHit", itrack, jhit); 
				    if(I("hit_isCSC", iHit)==1){
					    CSCHitBXinBX2Trk->Fill( I("hit_BX", iHit) );
				    }
				    if(I("hit_isRPC", iHit)==1){
					    RPCHitBXinBX2Trk->Fill( I("hit_BX", iHit) );
				    }
					    
			    }//end loop over hits
                            break;
			case 3:
			    PtBX3->Fill( F("trk_pt",itrack) );
			    EtaBX3->Fill( F("trk_eta",itrack) );
			    PhiBX3->Fill( F("trk_phi",itrack) );
			    ModeBX3->Fill( I("trk_mode", itrack) );
		            CSCModeBX3->Fill( I("trk_mode_CSC", itrack) );
		            RPCModeBX3->Fill( I("trk_mode_RPC", itrack) );
			    for (int jhit = 0; jhit < I("trk_nHits", itrack); jhit++) {
				    int iHit = I("trk_iHit", itrack, jhit); 
				    if(I("hit_isCSC", iHit)==1){
					    CSCHitBXinBX3Trk->Fill( I("hit_BX", iHit) );
				    }
				    if(I("hit_isRPC", iHit)==1){
					    RPCHitBXinBX3Trk->Fill( I("hit_BX", iHit) );
				    }
					    
			    }//end loop over hits
                            break;
		        case -1:
			    PtBXm1->Fill( F("trk_pt",itrack) );
			    EtaBXm1->Fill( F("trk_eta",itrack) );
			    PhiBXm1->Fill( F("trk_phi",itrack) );
                            ModeBXm1->Fill(I("trk_mode", itrack));
		            CSCModeBXm1->Fill( I("trk_mode_CSC", itrack) );
		            RPCModeBXm1->Fill( I("trk_mode_RPC", itrack) );
			    for (int jhit = 0; jhit < I("trk_nHits", itrack); jhit++) {
				    int iHit = I("trk_iHit", itrack, jhit); 
				    if(I("hit_isCSC", iHit)==1){
					    CSCHitBXinBXm1Trk->Fill( I("hit_BX", iHit) );
				    }
				    if(I("hit_isRPC", iHit)==1){
					    RPCHitBXinBXm1Trk->Fill( I("hit_BX", iHit) );
				    }
					    
			    }//end loop over hits
                            break; 
			case -2:
			    PtBXm2->Fill( F("trk_pt",itrack) );
			    EtaBXm2->Fill( F("trk_eta",itrack) );
			    PhiBXm2->Fill( F("trk_phi",itrack) );
                            ModeBXm2->Fill(I("trk_mode", itrack));
		            CSCModeBXm2->Fill( I("trk_mode_CSC", itrack) );
		            RPCModeBXm2->Fill( I("trk_mode_RPC", itrack) );
			    for (int jhit = 0; jhit < I("trk_nHits", itrack); jhit++) {
				    int iHit = I("trk_iHit", itrack, jhit); 
				    if(I("hit_isCSC", iHit)==1){
					    CSCHitBXinBXm2Trk->Fill( I("hit_BX", iHit) );
				    }
				    if(I("hit_isRPC", iHit)==1){
					    RPCHitBXinBXm2Trk->Fill( I("hit_BX", iHit) );
				    }
					    
			    }//end loop over hits
                            break;
			case -3:
			    PtBXm3->Fill( F("trk_pt",itrack) );
			    EtaBXm3->Fill( F("trk_eta",itrack) );
			    PhiBXm3->Fill( F("trk_phi",itrack) );
			    ModeBXm3->Fill(I("trk_mode", itrack));
		            CSCModeBXm3->Fill( I("trk_mode_CSC", itrack) );
		            RPCModeBXm3->Fill( I("trk_mode_RPC", itrack) );
			    for (int jhit = 0; jhit < I("trk_nHits", itrack); jhit++) {
				    int iHit = I("trk_iHit", itrack, jhit); 
				    if(I("hit_isCSC", iHit)==1){
					    CSCHitBXinBXm3Trk->Fill( I("hit_BX", iHit) );
				    }
				    if(I("hit_isRPC", iHit)==1){
					    RPCHitBXinBXm3Trk->Fill( I("hit_BX", iHit) );
				    }
					    
			    }//end loop over hits
                            break;
		        default:
                            break;
		    }//end switch
				    
	    }//end if high pT
	    
	     
    }//end loop over tracks
    
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

  CSCHitBXinBX0Trk->GetXaxis()->SetTitle("CSC Hit BX");
  CSCHitBXinBX1Trk->GetXaxis()->SetTitle("CSC Hit BX");
  CSCHitBXinBX2Trk->GetXaxis()->SetTitle("CSC Hit BX");
  CSCHitBXinBX3Trk->GetXaxis()->SetTitle("CSC Hit BX");
  CSCHitBXinBXm1Trk->GetXaxis()->SetTitle("CSC Hit BX");
  CSCHitBXinBXm2Trk->GetXaxis()->SetTitle("CSC Hit BX");
  CSCHitBXinBXm3Trk->GetXaxis()->SetTitle("CSC Hit BX");
	
  RPCHitBXinBX0Trk->GetXaxis()->SetTitle("RPC Hit BX");
  RPCHitBXinBX1Trk->GetXaxis()->SetTitle("RPC Hit BX");
  RPCHitBXinBX2Trk->GetXaxis()->SetTitle("RPC Hit BX");
  RPCHitBXinBX3Trk->GetXaxis()->SetTitle("RPC Hit BX");
  RPCHitBXinBXm1Trk->GetXaxis()->SetTitle("RPC Hit BX");
  RPCHitBXinBXm2Trk->GetXaxis()->SetTitle("RPC Hit BX");
  RPCHitBXinBXm3Trk->GetXaxis()->SetTitle("RPC Hit BX");
	
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
	
  CSCHitBXinBX0Trk->Write();
  CSCHitBXinBX1Trk->Write();
  CSCHitBXinBX2Trk->Write();
  CSCHitBXinBX3Trk->Write();
  CSCHitBXinBXm1Trk->Write();
  CSCHitBXinBXm2Trk->Write();
  CSCHitBXinBXm3Trk->Write();
  
  RPCHitBXinBX0Trk->Write();
  RPCHitBXinBX1Trk->Write();
  RPCHitBXinBX2Trk->Write();
  RPCHitBXinBX3Trk->Write();
  RPCHitBXinBXm1Trk->Write();
  RPCHitBXinBXm2Trk->Write();
  RPCHitBXinBXm3Trk->Write();
        
  myPlot.Close();
  
} // End function: void NTuple_Analyzer()
