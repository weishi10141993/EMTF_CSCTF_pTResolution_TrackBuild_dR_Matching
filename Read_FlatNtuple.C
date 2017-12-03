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
const int MAX_EVT   = 1000000;   // Max number of events to process
const int PRT_EVT   =  1;   // Print every N events
const bool verbose  = true; // Print information about the event and RECO and L1T muon
const int PT_CUT = 22;
const float ETA_UP = 2.4;
const float ETA_LOW = 1.6;
const int CUT1_UP = 16;//dTheta(1-X)
const int CUT1_LOW = 0;
const int CUT2_UP = 8;//dTheta(X-Y)
const int CUT2_LOW = 0;
//================================================================

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
  TString Topology4Title="";
  Topology4Title = Topology4Title + "dTh cuts 4-station tracks: pT>="+ Form("%d", PT_CUT) +"GeV, eta ["+Form("%.1f", ETA_LOW)+","+ Form("%.1f", ETA_UP)+"]";
  TString Topology3Title="";
  Topology3Title = Topology3Title + "dTh cuts 3-station tracks: pT>="+ Form("%d", PT_CUT) +"GeV, eta ["+Form("%.1f", ETA_LOW)+","+ Form("%.1f", ETA_UP)+"]";
  TH2F *CutTopology4 = new TH2F("CutTopology4", Topology4Title, CUT1_UP-CUT1_LOW+1, CUT1_LOW, CUT1_UP+1, CUT2_UP-CUT2_LOW+1, CUT2_LOW, CUT2_UP+1);
  TH2F *CutTopology3 = new TH2F("CutTopology3", Topology3Title, CUT1_UP-CUT1_LOW+1, CUT1_LOW, CUT1_UP+1, CUT2_UP-CUT2_LOW+1, CUT2_LOW, CUT2_UP+1);
  
  InitializeMaps();
  SetBranchAddresses(in_chain);
	
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
    // Print info for unpacked EMTF tracks
    if (verbose) std::cout << "\n" << I("nUnpTracks") << " unpacked tracks in the event" << std::endl;
    for (int itrack = 0; itrack < I("nUnpTracks"); itrack++) {
	    
	    float th1=-99;
	    float th2=-99;
	    float th3=-99;
	    float th4=-99;
	    float dTh12=-99;
	    float dTh13=-99;
	    float dTh14=-99;
	    float dTh23=-99;
	    float dTh24=-99;
	    float dTh34=-99;
	    float dTh1X=-99;
	    float dThXY=-99;

	    if(I("unp_trk_BX", itrack) >= -1 && I("unp_trk_BX", itrack)<=1 && I("unp_trk_nHits",itrack) >= 3 && F("unp_trk_pt", itrack) >= PT_CUT && I("unp_trk_nRPC",itrack) == 0 && I("unp_trk_found_hits",itrack) == 1 && F("unp_trk_eta",itrack) >= ETA_LOW && F("unp_trk_eta",itrack) <= ETA_UP){
		    
		    if (verbose) std::cout << " * Mode " << I("unp_trk_mode", itrack) << " nHits:"<<I("unp_trk_nHits",itrack)<< " track with BX = " << I("unp_trk_BX", itrack) << ", pT = " << F("unp_trk_pt", itrack) << ", eta = " << F("unp_trk_eta", itrack) << ", phi = " << F("unp_trk_phi", itrack) << ", max dPhi_int among hits = " << I("unp_trk_dPhi_int", itrack) << std::endl;
		    
	            for (int jhit = 0; jhit < I("unp_trk_nHits", itrack); jhit++) {
			    
			    int iHit = I("unp_trk_iHit", itrack, jhit);  // Access the index of each hit in the selected track
			    if (verbose) std::cout << "  - LCT with BX = " << I("hit_BX", iHit) << ", endcap = " << I("hit_endcap", iHit) << ", station = " << I("hit_station", iHit) << ", ring = " << I("hit_ring", iHit) <<", theta = "<< F("hit_theta", iHit)<< std::endl; 
				    
			    if( I("hit_station", iHit) ==1 ){
				    th1=F("hit_theta", iHit);
		            }
			    else if ( I("hit_station", iHit) == 2 ){
			            th2=F("hit_theta", iHit);
			    }
			    else if ( I("hit_station", iHit) == 3 ){
				    th3=F("hit_theta", iHit);
			    }
		            else if ( I("hit_station", iHit) == 4 ){
			            th4=F("hit_theta", iHit);
			    }
		    }//end loop over hits in selected unpacked track
		    //4-station
		    if (I("unp_trk_mode", itrack) == 15){
			    dTh12=fabs(th1-th2);
			    dTh13=fabs(th1-th3);
			    dTh14=fabs(th1-th4);
			    dTh23=fabs(th2-th3);
	                    dTh24=fabs(th2-th4);
	                    dTh34=fabs(th3-th4);
			    dTh1X=dTh12>dTh13?dTh12:dTh13;
			    dTh1X=dTh1X>dTh14?dTh1X:dTh14;
			    dThXY=dTh23>dTh24?dTh23:dTh24;
			    dThXY=dThXY>dTh34?dThXY:dTh34;
			    //loop over cut on dTheta(1-X)
                            for(Int_t i=CUT1_LOW;i<=CUT1_UP;i++){
				    //loop over cut on dTheta(X-Y)
  			            for(Int_t j=CUT2_LOW;j<=CUT2_UP;j++){
					    if(dTh1X<=i && dThXY<=j){
						    CutTopology4->Fill(i,j);
					    }
	  			    }//end cut2
  			    }//end cut1
			    
		    }//end mode 15
		    //station 123
		    if (I("unp_trk_mode", itrack) == 14){
			    dTh12=fabs(th1-th2);
			    dTh13=fabs(th1-th3);
			    dTh23=fabs(th2-th3);
			    dTh1X=dTh12>dTh13?dTh12:dTh13;
			    dThXY=dTh23;
			    //loop over cut on dTheta(1-X)
                            for(Int_t i=CUT1_LOW;i<=CUT1_UP;i++){
				    //loop over cut on dTheta(X-Y)
  			            for(Int_t j=CUT2_LOW;j<=CUT2_UP;j++){
					    if(dTh1X<=i && dThXY<=j){
						    CutTopology3->Fill(i,j);
					    }
	  			    }//end cut2
  			    }//end cut1
			    
		    }
		    //station 124
		    if (I("unp_trk_mode", itrack) == 13){
			    dTh12=fabs(th1-th2);
			    dTh14=fabs(th1-th4);
	                    dTh24=fabs(th2-th4);
			    dTh1X=dTh12>dTh14?dTh12:dTh14;
			    dThXY=dTh24;
			    //loop over cut on dTheta(1-X)
                            for(Int_t i=CUT1_LOW;i<=CUT1_UP;i++){
				    //loop over cut on dTheta(X-Y)
  			            for(Int_t j=CUT2_LOW;j<=CUT2_UP;j++){
					    if(dTh1X<=i && dThXY<=j){
						    CutTopology3->Fill(i,j);
					    }
	  			    }//end cut2
  			    }//end cut1
			    
		    }
		    //station 134
		    if (I("unp_trk_mode", itrack) == 11){
			    dTh13=fabs(th1-th3);
			    dTh14=fabs(th1-th4);
	                    dTh34=fabs(th3-th4);
			    dTh1X=dTh13>dTh14?dTh13:dTh14;
			    dThXY=dTh34;
			    //loop over cut on dTheta(1-X)
                            for(Int_t i=CUT1_LOW;i<=CUT1_UP;i++){
				    //loop over cut on dTheta(X-Y)
  			            for(Int_t j=CUT2_LOW;j<=CUT2_UP;j++){
					    if(dTh1X<=i && dThXY<=j){
						    CutTopology3->Fill(i,j);
					    }
	  			    }//end cut2
  			    }//end cut1
		    }
		    //station 234
		    if (I("unp_trk_mode", itrack) == 7){
			    dTh23=fabs(th2-th3);
	                    dTh24=fabs(th2-th4);
	                    dTh34=fabs(th3-th4);
			    dThXY=dTh23>dTh24?dTh23:dTh24;
			    dThXY=dThXY>dTh34?dThXY:dTh34;
			    //loop over cut on dTheta(1-X)
                            for(Int_t i=CUT1_LOW;i<=CUT1_UP;i++){
				    //loop over cut on dTheta(X-Y)
  			            for(Int_t j=CUT2_LOW;j<=CUT2_UP;j++){
					    if(dThXY<=j){
						    CutTopology3->Fill(i,j);
					    }
	  			    }//end cut2
  			    }//end cut1
		    }
		    
		    
	    }//end selection on track
	     
    }//end loop over unpacked tracks
    
  } // End loop events
  std::cout << "\n******* Finished looping over the events *******" << std::endl;

  delete in_chain;
  std::cout << "\nDone with Read_FlatNtuple(). Exiting.\n" << std::endl;
	
  //write to output file
  TString outFile = "/afs/cern.ch/work/w/wshi/public/EMTFPileUp/dThetaWindow_pt_";
  outFile = outFile + Form("%d", PT_CUT) +"_eta_"+Form("%.1f", ETA_LOW)+"_"+ Form("%.1f", ETA_UP)+ ".root";
  TFile myPlot(outFile,"RECREATE");
        
  CutTopology4->GetXaxis()->SetTitle("dTheta(1-X)");
  CutTopology4->GetYaxis()->SetTitle("dTheta(X-Y)");
  CutTopology4->Write();
        
  CutTopology3->GetXaxis()->SetTitle("dTheta(1-X)");
  CutTopology3->GetYaxis()->SetTitle("dTheta(X-Y)");
  CutTopology3->Write();
        
  myPlot.Close();
  
} // End function: void Read_FlatNtuple()
