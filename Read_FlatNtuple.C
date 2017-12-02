////////////////////////////////////////////////////////////
///        Simple macro to read EMTF Flat NTuples        ///
///              Andrew Brinkerhoff 29.09.17             ///
///                                                      ///
///   TChain can be used to read multiple files.         ///
///   Format: interface/FlatNtupleBranches.h             ///
////////////////////////////////////////////////////////////

#include "TFile.h"
#include "TSystem.h"
#include "TChain.h"
#include "TTree.h"
#include "TBranch.h"

#include "Read_FlatNtuple.h" // List of input branches and functions to return values

const int MAX_FILES = 1;   // Max number of files to process
const int MAX_EVT   = 100;   // Max number of events to process
const int PRT_EVT   =  1;   // Print every N events
const bool verbose  = true; // Print information about the event and RECO and L1T muons


///////////////////////////////////////////
///  Main function to read the NTuples  ///
///////////////////////////////////////////

void Read_FlatNtuple() {

  // Initialize empty file to access each file in the list
  TFile *file_tmp(0);

  // List of input files
  std::vector<TString> in_file_names;
  TString store = "root://eoscms.cern.ch//store/user/abrinke1/EMTF/Emulator/";
  TString in_dir = "ntuples/HADD/";
  TString file_name;

  file_name.Form("%s/%s/NTuple_ZeroBias8b4e_FlatNtuple_Skim_Run_302674_2017_09_30.root", store.Data(), in_dir.Data());
  std::cout << "Adding file " << file_name.Data() << std::endl;
  in_file_names.push_back(file_name.Data());
  
  // for (int i = 1; i < MAX_FILES+1; i++) {
  //   file_name.Form("%s/%s/tuple_%d.root", store.Data(), in_dir.Data(), i);
  //   std::cout << "Adding file " << file_name.Data() << std::endl;
  //   in_file_names.push_back(file_name.Data());
  // }

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
  TChain *in_chain= new TChain("ntuple/tree");
  for (int i = 0; i < in_file_names.size(); i++) {
    in_chain->Add( in_file_names.at(i) );
  }

  InitializeMaps();
  SetBranchAddresses(in_chain);

  // if (verbose) in_chain->GetListOfBranches()->Print();
  
  std::cout << "\n******* About to loop over the events *******" << std::endl;
  int nEvents = in_chain->GetEntries();
  for (int iEvt = 0; iEvt < nEvents; iEvt++) {
    if (iEvt > MAX_EVT) break;
    if ( (iEvt % PRT_EVT) == 0 ) {
      std::cout << "\n*************************************" << std::endl;
      std::cout << "Looking at event " << iEvt << " out of " << nEvents << std::endl;
      std::cout << "*************************************" << std::endl;
    }

    in_chain->GetEntry(iEvt);
    
    // From Read_FlatNtuple.h, use 'I("branch_name")' to get an integer branch value, 'F("branch_name") to get a float
    if (verbose) std::cout << "\nRun = " << I("evt_run") << ", LS = " << I("evt_LS") << ", event = " << I("evt_event") << std::endl;
    
    // Print info for emulated EMTF hits
    if (verbose) std::cout << "\n" << I("nHits") << " emulated EMTF hits in the event" << std::endl;
    for (int i = 0; i < I("nHits"); i++) {

      if        (I("hit_isCSC", i) == 1) {
    	if (verbose) std::cout << " * CSC LCT with BX = " << I("hit_BX", i) << ", endcap = " << I("hit_endcap", i)
    			       << ", station = " << I("hit_station", i) << ", ring = " << I("hit_ring", i) << std::endl; 
      } else if (I("hit_isRPC", i) == 1) {
    	if (verbose) std::cout << " * RPC hit with BX = " << I("hit_BX", i) << ", endcap = " << I("hit_endcap", i)
    			       << ", station = " << I("hit_station", i) << ", ring = " << I("hit_ring", i) << std::endl; 
      }
    }
    
    // Print info for emulated EMTF tracks
    if (verbose) std::cout << "\n" << I("nTracks") << " emulated EMTF tracks in the event" << std::endl;
    if ( I("nTracks") != VF("trk_pt")->size() )  // From Read_FlatNtuple.h, can use 'VF("branch_name")' to get a vector of floats
      throw std::out_of_range("nTracks does not equal size of vector of tracks");
    for (int i = 0; i < I("nTracks"); i++) {
      // // From Read_FlatNtuple.h, can use '("branch_name", i)' to get the ith element of a vector of ints
      if (verbose) std::cout << " * Mode " << I("trk_mode", i) << " track with BX = " << I("trk_BX", i) 
    			     << ", pT = " << F("trk_pt", i) << ", eta = " << F("trk_eta", i) << ", phi = " << F("trk_phi", i)
			     << ", maximum dPhi_int among hits = " << I("trk_dPhi_int", i) << std::endl;
    }
    
    // Print info for unpacked EMTF tracks
    if (verbose) std::cout << "\n" << I("nUnpTracks") << " unpacked EMTF tracks in the event" << std::endl;
    for (int i = 0; i < I("nUnpTracks"); i++) {
      
      if (verbose) std::cout << " * Mode " << I("unp_trk_mode", i) << " track with BX = " << I("unp_trk_BX", i) 
    			     << ", pT = " << F("unp_trk_pt", i) << ", eta = " << F("unp_trk_eta", i) << ", phi = " << F("unp_trk_phi", i)
			     << ", maximum dPhi_int among hits = " << I("unp_trk_dPhi_int", i) << std::endl;
      
      for (int j = 0; j < I("unp_trk_nHits", i); j++) {
	if (I("unp_trk_found_hits", i) != 1) continue;  // For a very small fraction of unpacked tracks, can't find all hits (mostly BX != 0)
	int iHit = I("unp_trk_iHit", i, j);  // Access the index of each hit in the track
	if        (I("hit_isCSC", iHit) == 1) {
	  if (verbose) std::cout << "  - CSC LCT with BX = " << I("hit_BX", iHit) << ", endcap = " << I("hit_endcap", iHit)
				 << ", station = " << I("hit_station", iHit) << ", ring = " << I("hit_ring", iHit) << std::endl; 
	} else if (I("hit_isRPC", iHit) == 1) {
	  if (verbose) std::cout << "  - RPC hit with BX = " << I("hit_BX", iHit) << ", endcap = " << I("hit_endcap", iHit)
				 << ", station = " << I("hit_station", iHit) << ", ring = " << I("hit_ring", iHit) << std::endl; 
	}
      }
	     
    }
    
  } // End loop: for (int iEvt = 0; iEvt < in_chain->GetEntries(); iEvt++)
  std::cout << "\n******* Finished looping over the events *******" << std::endl;

  delete in_chain;

  std::cout << "\nDone with Read_FlatNtuple(). Exiting.\n" << std::endl;
  
} // End function: void Read_FlatNtuple()
