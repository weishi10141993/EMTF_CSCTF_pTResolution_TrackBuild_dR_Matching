#include <iostream>
#include <iomanip>
using namespace std;
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TAttFill.h"
#include "TCanvas.h"
#include <vector>
#include "stdio.h"
#include <stdlib.h>
#include "math.h"
#include "TMath.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TPaveStats.h"
#include "TStyle.h"
#include "THStack.h"
#include "TFitResultPtr.h"

//****************************************************************************************
//*Cut the rate by tightening dTheta windows
//*
//*Wei Shi @ Dec 2, 2017 CERN Geneva
//****************************************************************************************
//*Each track has up to 6 dTheta values between stations: 1-2, 1-3, 1-4, 2-3, 2-4, and 3-4.  As you can see from some old studies of mine [1], the dTheta bending between stations is quite small, even for fairly-low-pT muons.  In general, the dTheta(1-X) bending can be a bit larger than the dTheta(X-Y) bending, where X > 1.  Right now the maximum allowed dTheta values in a track are <= 8, which is about 2°.  
//*This is a very large window; it was widened last year from <= 4 because the RPC resolution in theta is only about ±2° in station 1.  However, we could easily imagine tightening the window, at least in the "ring 1" region (|eta| > 1.6 or so) where we have the most pileup.  So the idea of the study is this:
// 1) Select unpacked 3- and 4-station EMTF tracks from the ZeroBias8b4e "Skim" file below, with pT >= 22 GeV
//    * Tracks must be in BX+1, 0, or -1, to make sure we have all the hits associated with the track
//    * For this study, select only CSC-only tracks, i.e. unp_trk_nRPC == 0
//    * Also require that the unpacker found all the hits belonging to the track, i.e. unp_trk_found_hits == 1
//2) For each track, compute the maximum absolute value of dTheta(1-X) and dTheta(X-Y) among the station pairs, where X > 1
//3) Determine whether dTheta(1-X) <= cut1 and dTheta(X-Y) <= cut2
//    * For 0 <= cut1 <= 16 and 0 <= cut2 <= 8
//4) Fill a histogram with the number of events passing each pair of cuts, with cut1 on the x-axis, and cut2 on the y-axis
//    * The x and y axes should also have a "no cut" bin
//    * So the "no cut - no cut" bin will have all the tracks with pT >= 22, and the other bins will have a smaller number, based on how many tracks pass both dTheta(1-X) <= cut1 and dTheta(X-Y) <= cut2
//    * Make separate histograms for 4-station tracks and 3-station tracks.  This is because the 4-station tracks may only be demoted, but still pass the SingleMu trigger, while all 3-station tracks that fail the cut would no longer be SingleMu-quality if one of the hits was removed.
//This will give us a nice estimate of how much rate reduction we can achieve by tightening the dTheta windows, without running 16 x 8 = 128 different options in the emulator.
void dThetaWindow()
{
        //USER modify here ONLY//
        //================================================================
        Int_t PT_CUT = 22;
        Float_t ETA_UP = 2.4;
        Float_t ETA_LOW = 1.6;
        Int_t CUT1_UP = 16;//dTheta(1-X)
        Int_t CUT1_LOW = 0;
        Int_t CUT2_UP = 8;//dTheta(X-Y)
        Int_t CUT2_LOW = 0;
        Int_t lxplus=1;//machine: lxplus(1) or bonner(0)?
        //================================================================
        TString Cluster="";
        if(lxplus==1){
                Cluster = "/afs/cern.ch/work/w/wshi/public/EMTFPileUp";
        }
        else{
                Cluster = "/home/ws13/";//bonner
        }
        TString fileName = Cluster + "/NTuple_ZeroBias8b4e_FlatNtuple_Skim_Run_302674_2017_09_30.root";
        TString directoryName = "ntuple/tree";
        TFile* myFile = new TFile(fileName);
        TTree* myTree = (TTree*) myFile->Get(directoryName);
        
        cout<<"Accessing file:"<<fileName<<endl;
        cout<<"Accessing directory:"<<directoryName<<endl;
        
        //gInterpreter->GenerateDictionary("vector<vector<int> >","vector");
        Int_t NUnpTracks;
        vector<float> *Unp_trk_eta;
        vector<float> *Unp_trk_pt;
        vector<int> *Unp_trk_mode;
        vector<int> *Unp_trk_BX;
        vector<int> *Unp_trk_nRPC;
        vector<int> *Unp_trk_found_hits;
        vector<int> *Unp_trk_nHits;
        //vector<vector<int> > *Unp_trk_iHit;//store the hit index in a track i, total unp_trk_nHits in a track.
        vector<int> *Hit_theta;
        vector<int> *Hit_station;
        
        myTree->SetBranchAddress("nUnpTracks",&NUnpTracks);
        myTree->SetBranchAddress("unp_trk_eta",&Unp_trk_eta);
        myTree->SetBranchAddress("unp_trk_pt",&Unp_trk_pt);
        myTree->SetBranchAddress("unp_trk_mode",&Unp_trk_mode);
        myTree->SetBranchAddress("unp_trk_BX",&Unp_trk_BX);
        myTree->SetBranchAddress("unp_trk_nRPC",&Unp_trk_nRPC);
        myTree->SetBranchAddress("unp_trk_found_hits",&Unp_trk_found_hits);
        myTree->SetBranchAddress("unp_trk_nHits",&Unp_trk_nHits);
        myTree->SetBranchAddress("unp_trk_iHit",&Unp_trk_iHit);
        myTree->SetBranchAddress("hit_theta",&Hit_theta);
        myTree->SetBranchAddress("hit_station",&Hit_station);
        
        TH2F *CutTopology4 = new TH2F("CutTopology4", "dTh cuts 4-station tracks", 16, 0, 16, 8, 0, 8);
        TH2F *CutTopology3 = new TH2F("CutTopology3", "dTh cuts 3-station tracks", 16, 0, 16, 8, 0, 8);
        
        Long64_t numEvents = myTree->GetEntries();
        cout<<">>>>>>>>>>>>>>>>>>>>>"<<endl;
        cout<<numEvents<<" events to process..."<<endl;
      
        //loop over cut on dTheta(1-X)
        for(Int_t i=CUT1_LOW;i<=CUT1_UP;i++){
                
                for(Int_t j=CUT2_LOW;j<=CUT2_UP;j++){
                        
                        for(Long64_t iEntry = 0; iEntry <100; iEntry++){
                                myTree->GetEntry(iEntry);
                                
                                cout<<"@@@"<<iEntry<<endl;
                                
                                Int_t nUnpTracks = NUnpTracks;
                                vector<float> unp_trk_eta = *Unp_trk_eta;
                                vector<float> unp_trk_pt = *Unp_trk_pt;
                                vector<int> unp_trk_mode = *Unp_trk_mode;
                                vector<int> unp_trk_BX = *Unp_trk_BX;
                                vector<int> unp_trk_nRPC = *Unp_trk_nRPC;
                                vector<int> unp_trk_found_hits = *Unp_trk_found_hits;
                                vector<int> unp_trk_nHits = *Unp_trk_nHits;
                                //vector<vector<int> > unp_trk_iHit = *Unp_trk_iHit;   
                                vector<int> hit_theta = *Hit_theta;
                                vector<int> hit_station = *Hit_station;
                               
                                cout<<"nUnpTracks:"<<nUnpTracks<<endl;
                                
                                for (int itrack = 0; itrack < nUnpTracks; itrack++) {
                                        if(unp_trk_BX[itrack] >= -1 && unp_trk_BX[itrack]<=1 && unp_trk_nHits[itrack] >= 3 && unp_trk_pt[itrack] >= PT_CUT && unp_trk_nRPC[itrack] == 0 && unp_trk_found_hits[itrack] == 1 && unp_trk_eta[itrack] >= ETA_LOW && unp_trk_eta[itrack] <= ETA_UP){
                                                cout<<"@ track"<<itrack<<" BX:"<<unp_trk_BX[itrack]<<" nHits:"<<unp_trk_nHits[itrack]<<" pt:"<<unp_trk_pt[itrack]<<" nRPC:"<<unp_trk_nRPC[itrack]<<" foundhits:"<<unp_trk_found_hits[itrack]<<" eta:"<<unp_trk_eta[itrack]<<endl;
                                                CutTopology4->Fill(unp_trk_nHits[itrack],unp_trk_eta[itrack]);
                                        }//end if
                                
                                }//end for itrack
   
                        }//end loop over events
                }//end cut2
        }//end on cut1
        
        //write to output file
        TString outFile = Cluster + "dThetaWindow_" + Form("%d", PT_CUT) + ".root";
        TFile myPlot(outFile,"RECREATE");
        
        CutTopology4->GetXaxis()->SetTitle("dTheta(1-X)");
        CutTopology4->GetYaxis()->SetTitle("dTheta(X-Y)");
        CutTopology4->Write();
        
        CutTopology3->GetXaxis()->SetTitle("dTheta(1-X)");
        CutTopology3->GetYaxis()->SetTitle("dTheta(X-Y)");
        CutTopology3->Write();
        
        myPlot.Close();
          
}//end macro
