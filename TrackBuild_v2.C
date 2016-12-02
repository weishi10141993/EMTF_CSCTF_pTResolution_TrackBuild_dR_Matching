#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include <vector>
#include "stdio.h"
#include "math.h"
#include "TMath.h"
#include "TMatrix.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TMultiGraph.h"
#include "TLegend.h"

//=****************************************************************
//=This macro compares EMTF and CSCTF track lcts to RECOCscSegments
//=to find reason of the disagree tracks
//=May need to consider duplicate lcts in next version
//=****************************************************************

void TrackBuild_v2()
{
    //===========
    //=data file=
    //===========
    TFile* myFile = new TFile("/afs/cern.ch/work/w/wshi/public/EMTFpTResolution/EMTF_NTuple_274440_443_v3.root");
    //TFile* myFile = new TFile("~/Documents/EMTFpTResolution/EMTF_NTuple_274440_443_v3.root");
    TTree* myTree = (TTree*) myFile->Get("ntuple/tree");

    //=================
    //=define pointers=
    //=================
    vector<float> *RecoPt;//RECO Pt
    vector<float> *RecoEta;//RECO eta
    vector<float> *RecoPhi;//RECO Phi
    vector<float> *TrkPt;//EMTF Pt
    vector<float> *TrkEta;//EMTF eta
    vector<float> *TrkPhi;//EMTF phi
    vector<int> *TrkMode;//EMTF mode
    vector<int> *TrkBx;//EMTF bx
    vector<float> *Leg_TrkPt;//CSCTF Pt
    vector<float> *Leg_TrkEta;//CSCTF eta
    vector<float> *Leg_TrkPhi;//CSCTF phi
    vector<int> *Leg_TrkMode;//CSCTF mode
    vector<int> *Leg_TrkBx;//CSCTF bx
    
    //===============================
    //=add for version 2.0 08102016
    //=define pointers to pull out recocscsegment and lcts
    //======================================================
    //trkLct_station, leg_trkLct_station are all [8][4]/I arrays with integer # of staions
    //they store at most 8 Lcts's information for one event
    //recoCscSeg_station[numRecoMuons][16]/I
    //check numRecoMuon in TBrowser to find the maximum numRecoMuon in one event
    //stations with no lct is stored as -999
    //there usually are duplicate reco csc segments, just view them as one; could also duplicate Lcts
    Int_t trkLct_station[8][4];//trkLct_station
    Int_t leg_trkLct_station[8][4];//leg_trkLct_station
    Int_t recoCscSeg_station[12][16];//recoCscSeg_station,choose 12 to make sure 12 > numRecoMuons
    
    //========================================
    //=event info: Pull variables from nTuple=
    //========================================
    myTree->SetBranchAddress("recoPt",&RecoPt);//RECO
    myTree->SetBranchAddress("recoEta",&RecoEta);
    myTree->SetBranchAddress("recoPhi",&RecoPhi);
    myTree->SetBranchAddress("trkPt",&TrkPt);//EMTF
    myTree->SetBranchAddress("trkEta",&TrkEta);
    myTree->SetBranchAddress("trkPhi",&TrkPhi);
    myTree->SetBranchAddress("trkMode",&TrkMode);
    myTree->SetBranchAddress("trkBx",&TrkBx);
    myTree->SetBranchAddress("leg_trkPt",&Leg_TrkPt);//CSCTF
    myTree->SetBranchAddress("leg_trkEta",&Leg_TrkEta);
    myTree->SetBranchAddress("leg_trkPhi",&Leg_TrkPhi);
    myTree->SetBranchAddress("leg_trkMode",&Leg_TrkMode);
    myTree->SetBranchAddress("leg_trkBx",&Leg_TrkBx);
    
    //===============================
    //=added for version 2.0 08102016
    //=pull out station info
    //===============================
    myTree->SetBranchAddress("recoCscSeg_station",&recoCscSeg_station);//RECO segments
    myTree->SetBranchAddress("leg_trkLct_station",&leg_trkLct_station);//CSCTF Lcts
    myTree->SetBranchAddress("trkLct_station",&trkLct_station);//EMTF Lcts
    
    
    //===================
    //=create histograms=
    //===================
    TH2F *EMTF_modes_CSCTF_modes = new TH2F("EMTF mode vs CSCTF mode","EMTF mode vs CSCTF mode",17,-1,16,17,-1,16);
    
    TH1F *EMTF_modes_CSCTF_mode_15 = new TH1F("EMTF modes when CSCTF mode=15","EMTF modes when CSCTF mode=15",17,-1,16);
    TH1F *EMTF_modes_CSCTF_mode_14 = new TH1F("EMTF modes when CSCTF mode=14","EMTF modes when CSCTF mode=14",17,-1,16);
    TH1F *EMTF_modes_CSCTF_mode_13 = new TH1F("EMTF modes when CSCTF mode=13","EMTF modes when CSCTF mode=13",17,-1,16);
    TH1F *EMTF_modes_CSCTF_mode_12 = new TH1F("EMTF modes when CSCTF mode=12","EMTF modes when CSCTF mode=12",17,-1,16);
    TH1F *EMTF_modes_CSCTF_mode_11 = new TH1F("EMTF modes when CSCTF mode=11","EMTF modes when CSCTF mode=11",17,-1,16);
    
    TH1F *CSCTF_modes_EMTF_mode_15 = new TH1F("CSCTF modes when EMTF mode=15","CSCTF modes when EMTF mode=15",17,-1,16);
    TH1F *CSCTF_modes_EMTF_mode_14 = new TH1F("CSCTF modes when EMTF mode=14","CSCTF modes when EMTF mode=14",17,-1,16);
    TH1F *CSCTF_modes_EMTF_mode_13 = new TH1F("CSCTF modes when EMTF mode=13","CSCTF modes when EMTF mode=13",17,-1,16);
    TH1F *CSCTF_modes_EMTF_mode_12 = new TH1F("CSCTF modes when EMTF mode=12","CSCTF modes when EMTF mode=12",17,-1,16);
    TH1F *CSCTF_modes_EMTF_mode_11 = new TH1F("CSCTF modes when EMTF mode=11","CSCTF modes when EMTF mode=11",17,-1,16);
    
    //======================
    //=add for v2.0 @ 08102016
    //= Create histograms to fill stations for recoCSCsegment,
    //=EMTF and CSCTF lcts for symmetric modes in mode plot
    //====================================================
    //===========================
    //3-station,4station mode pair
    //===========================
    //According to mode plot we look into (7,15)-(15,7) mode pair: possibly missing station 1
    //others include(11,15),(13,15),(14,15),
    //{{{
    TH1F *EMTF_stations_EMTF_mode_7_CSCTF_mode_15 = new TH1F("trkLct stations when EMTF mode=7,CSCTF mode=15","trkLct stations when EMTF mode=7,CSCTF mode=15",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_7_CSCTF_mode_15 = new TH1F("leg_trkLct stations when EMTF mode=7,CSCTF mode=15","leg_trkLct stations when EMTF mode=7,CSCTF mode=15",6,0,6);
    TH1F *RECO_stations_EMTF_mode_7_CSCTF_mode_15 = new TH1F("recoCscSeg stations when EMTF mode=7,CSCTF mode=15","recoCscSeg stations when EMTF mode=7,CSCTF mode=15",6,0,6);
    TH1F *EMTF_stations_EMTF_mode_15_CSCTF_mode_7 = new TH1F("trkLct stations when EMTF mode=15,CSCTF mode=7","trkLct stations when EMTF mode=15,CSCTF mode=7",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_15_CSCTF_mode_7 = new TH1F("leg_trkLct stations when EMTF mode=15,CSCTF mode=7","leg_trkLct stations when EMTF mode=15,CSCTF mode=7",6,0,6);
    TH1F *RECO_stations_EMTF_mode_15_CSCTF_mode_7 = new TH1F("recoCscSeg stations when EMTF mode=15,CSCTF mode=7","recoCscSeg stations when EMTF mode=15,CSCTF mode=7",6,0,6);
    //(11,15)-(15,11): possibly missing station 2
    TH1F *EMTF_stations_EMTF_mode_11_CSCTF_mode_15 = new TH1F("trkLct stations when EMTF mode=11,CSCTF mode=15","trkLct stations when EMTF mode=11,CSCTF mode=15",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_11_CSCTF_mode_15 = new TH1F("leg_trkLct stations when EMTF mode=11,CSCTF mode=15","leg_trkLct stations when EMTF mode=11,CSCTF mode=15",6,0,6);
    TH1F *RECO_stations_EMTF_mode_11_CSCTF_mode_15 = new TH1F("recoCscSeg stations when EMTF mode=11,CSCTF mode=15","recoCscSeg stations when EMTF mode=11,CSCTF mode=15",6,0,6);
    TH1F *EMTF_stations_EMTF_mode_15_CSCTF_mode_11 = new TH1F("trkLct stations when EMTF mode=15,CSCTF mode=11","trkLct stations when EMTF mode=15,CSCTF mode=11",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_15_CSCTF_mode_11 = new TH1F("leg_trkLct stations when EMTF mode=15,CSCTF mode=11","leg_trkLct stations when EMTF mode=15,CSCTF mode=11",6,0,6);
    TH1F *RECO_stations_EMTF_mode_15_CSCTF_mode_11 = new TH1F("recoCscSeg stations when EMTF mode=15,CSCTF mode=11","recoCscSeg stations when EMTF mode=15,CSCTF mode=11",6,0,6);
    //(13,15)-(15,13): pms 3
    TH1F *EMTF_stations_EMTF_mode_13_CSCTF_mode_15 = new TH1F("trkLct stations when EMTF mode=13,CSCTF mode=15","trkLct stations when EMTF mode=13,CSCTF mode=15",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_13_CSCTF_mode_15 = new TH1F("leg_trkLct stations when EMTF mode=13,CSCTF mode=15","leg_trkLct stations when EMTF mode=13,CSCTF mode=15",6,0,6);
    TH1F *RECO_stations_EMTF_mode_13_CSCTF_mode_15 = new TH1F("recoCscSeg stations when EMTF mode=13,CSCTF mode=15","recoCscSeg stations when EMTF mode=13,CSCTF mode=15",6,0,6);
    TH1F *EMTF_stations_EMTF_mode_15_CSCTF_mode_13 = new TH1F("trkLct stations when EMTF mode=15,CSCTF mode=13","trkLct stations when EMTF mode=15,CSCTF mode=13",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_15_CSCTF_mode_13 = new TH1F("leg_trkLct stations when EMTF mode=15,CSCTF mode=13","leg_trkLct stations when EMTF mode=15,CSCTF mode=13",6,0,6);
    TH1F *RECO_stations_EMTF_mode_15_CSCTF_mode_13 = new TH1F("recoCscSeg stations when EMTF mode=15,CSCTF mode=13","recoCscSeg stations when EMTF mode=15,CSCTF mode=13",6,0,6);
    //(14,15)-(15,14): pms 4
    TH1F *EMTF_stations_EMTF_mode_14_CSCTF_mode_15 = new TH1F("trkLct stations when EMTF mode=14,CSCTF mode=15","trkLct stations when EMTF mode=14,CSCTF mode=15",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_14_CSCTF_mode_15 = new TH1F("leg_trkLct stations when EMTF mode=14,CSCTF mode=15","leg_trkLct stations when EMTF mode=14,CSCTF mode=15",6,0,6);
    TH1F *RECO_stations_EMTF_mode_14_CSCTF_mode_15 = new TH1F("recoCscSeg stations when EMTF mode=14,CSCTF mode=15","recoCscSeg stations when EMTF mode=14,CSCTF mode=15",6,0,6);
    TH1F *EMTF_stations_EMTF_mode_15_CSCTF_mode_14 = new TH1F("trkLct stations when EMTF mode=15,CSCTF mode=14","trkLct stations when EMTF mode=15,CSCTF mode=14",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_15_CSCTF_mode_14 = new TH1F("leg_trkLct stations when EMTF mode=15,CSCTF mode=14","leg_trkLct stations when EMTF mode=15,CSCTF mode=14",6,0,6);
    TH1F *RECO_stations_EMTF_mode_15_CSCTF_mode_14 = new TH1F("recoCscSeg stations when EMTF mode=15,CSCTF mode=14","recoCscSeg stations when EMTF mode=15,CSCTF mode=14",6,0,6);
    //}}}
    
    //==============================
    //2-station, 3-station mode pair
    //==============================
    //{{{
    
    //(3,7)-(7,3): pms 2
    TH1F *EMTF_stations_EMTF_mode_3_CSCTF_mode_7 = new TH1F("trkLct stations when EMTF mode=3,CSCTF mode=7","trkLct stations when EMTF mode=3,CSCTF mode=7",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_3_CSCTF_mode_7 = new TH1F("leg_trkLct stations when EMTF mode=3,CSCTF mode=7","leg_trkLct stations when EMTF mode=3,CSCTF mode=7",6,0,6);
    TH1F *RECO_stations_EMTF_mode_3_CSCTF_mode_7 = new TH1F("recoCscSeg stations when EMTF mode=3,CSCTF mode=7","recoCscSeg stations when EMTF mode=3,CSCTF mode=7",6,0,6);
    TH1F *EMTF_stations_EMTF_mode_7_CSCTF_mode_3 = new TH1F("trkLct stations when EMTF mode=7,CSCTF mode=3","trkLct stations when EMTF mode=7,CSCTF mode=3",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_7_CSCTF_mode_3 = new TH1F("leg_trkLct stations when EMTF mode=7,CSCTF mode=3","leg_trkLct stations when EMTF mode=7,CSCTF mode=3",6,0,6);
    TH1F *RECO_stations_EMTF_mode_7_CSCTF_mode_3 = new TH1F("recoCscSeg stations when EMTF mode=7,CSCTF mode=3","recoCscSeg stations when EMTF mode=7,CSCTF mode=3",6,0,6);
    
    //(3,11)-(11,3):pms 1
    TH1F *EMTF_stations_EMTF_mode_3_CSCTF_mode_11 = new TH1F("trkLct stations when EMTF mode=3,CSCTF mode=11","trkLct stations when EMTF mode=3,CSCTF mode=11",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_3_CSCTF_mode_11 = new TH1F("leg_trkLct stations when EMTF mode=3,CSCTF mode=11","leg_trkLct stations when EMTF mode=3,CSCTF mode=11",6,0,6);
    TH1F *RECO_stations_EMTF_mode_3_CSCTF_mode_11 = new TH1F("recoCscSeg stations when EMTF mode=3,CSCTF mode=11","recoCscSeg stations when EMTF mode=3,CSCTF mode=11",6,0,6);
    TH1F *EMTF_stations_EMTF_mode_11_CSCTF_mode_3 = new TH1F("trkLct stations when EMTF mode=11,CSCTF mode=3","trkLct stations when EMTF mode=11,CSCTF mode=3",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_11_CSCTF_mode_3 = new TH1F("leg_trkLct stations when EMTF mode=11,CSCTF mode=3","leg_trkLct stations when EMTF mode=11,CSCTF mode=3",6,0,6);
    TH1F *RECO_stations_EMTF_mode_11_CSCTF_mode_3 = new TH1F("recoCscSeg stations when EMTF mode=11,CSCTF mode=3","recoCscSeg stations when EMTF mode=11,CSCTF mode=3",6,0,6);
    
    //(5,13)-(13,5): pms 3
    TH1F *EMTF_stations_EMTF_mode_5_CSCTF_mode_13 = new TH1F("trkLct stations when EMTF mode=5,CSCTF mode=13","trkLct stations when EMTF mode=5,CSCTF mode=13",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_5_CSCTF_mode_13 = new TH1F("leg_trkLct stations when EMTF mode=5,CSCTF mode=13","leg_trkLct stations when EMTF mode=5,CSCTF mode=13",6,0,6);
    TH1F *RECO_stations_EMTF_mode_5_CSCTF_mode_13 = new TH1F("recoCscSeg stations when EMTF mode=5,CSCTF mode=13","recoCscSeg stations when EMTF mode=5,CSCTF mode=13",6,0,6);
    TH1F *EMTF_stations_EMTF_mode_13_CSCTF_mode_5 = new TH1F("trkLct stations when EMTF mode=13,CSCTF mode=5","trkLct stations when EMTF mode=13,CSCTF mode=5",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_13_CSCTF_mode_5 = new TH1F("leg_trkLct stations when EMTF mode=13,CSCTF mode=5","leg_trkLct stations when EMTF mode=13,CSCTF mode=5",6,0,6);
    TH1F *RECO_stations_EMTF_mode_13_CSCTF_mode_5 = new TH1F("recoCscSeg stations when EMTF mode=13,CSCTF mode=5","recoCscSeg stations when EMTF mode=13,CSCTF mode=5",6,0,6);
    
    //(5,7)-(7,5):pms 3
    TH1F *EMTF_stations_EMTF_mode_5_CSCTF_mode_7 = new TH1F("trkLct stations when EMTF mode=5,CSCTF mode=7","trkLct stations when EMTF mode=5,CSCTF mode=7",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_5_CSCTF_mode_7 = new TH1F("leg_trkLct stations when EMTF mode=5,CSCTF mode=7","leg_trkLct stations when EMTF mode=5,CSCTF mode=7",6,0,6);
    TH1F *RECO_stations_EMTF_mode_5_CSCTF_mode_7 = new TH1F("recoCscSeg stations when EMTF mode=5,CSCTF mode=7","recoCscSeg stations when EMTF mode=5,CSCTF mode=7",6,0,6);
    TH1F *EMTF_stations_EMTF_mode_7_CSCTF_mode_5 = new TH1F("trkLct stations when EMTF mode=7,CSCTF mode=5","trkLct stations when EMTF mode=7,CSCTF mode=5",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_7_CSCTF_mode_5 = new TH1F("leg_trkLct stations when EMTF mode=7,CSCTF mode=5","leg_trkLct stations when EMTF mode=7,CSCTF mode=5",6,0,6);
    TH1F *RECO_stations_EMTF_mode_7_CSCTF_mode_5 = new TH1F("recoCscSeg stations when EMTF mode=7,CSCTF mode=5","recoCscSeg stations when EMTF mode=7,CSCTF mode=5",6,0,6);
    
    //(6,7)-(7,6):pms 4
    TH1F *EMTF_stations_EMTF_mode_6_CSCTF_mode_7 = new TH1F("trkLct stations when EMTF mode=6,CSCTF mode=7","trkLct stations when EMTF mode=6,CSCTF mode=7",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_6_CSCTF_mode_7 = new TH1F("leg_trkLct stations when EMTF mode=6,CSCTF mode=7","leg_trkLct stations when EMTF mode=6,CSCTF mode=7",6,0,6);
    TH1F *RECO_stations_EMTF_mode_6_CSCTF_mode_7 = new TH1F("recoCscSeg stations when EMTF mode=6,CSCTF mode=7","recoCscSeg stations when EMTF mode=6,CSCTF mode=7",6,0,6);
    TH1F *EMTF_stations_EMTF_mode_7_CSCTF_mode_6 = new TH1F("trkLct stations when EMTF mode=7,CSCTF mode=6","trkLct stations when EMTF mode=7,CSCTF mode=6",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_7_CSCTF_mode_6 = new TH1F("leg_trkLct stations when EMTF mode=7,CSCTF mode=6","leg_trkLct stations when EMTF mode=7,CSCTF mode=6",6,0,6);
    TH1F *RECO_stations_EMTF_mode_7_CSCTF_mode_6 = new TH1F("recoCscSeg stations when EMTF mode=7,CSCTF mode=6","recoCscSeg stations when EMTF mode=7,CSCTF mode=6",6,0,6);
    
    //(6,14)-(14,6):pms 1
    TH1F *EMTF_stations_EMTF_mode_6_CSCTF_mode_14 = new TH1F("trkLct stations when EMTF mode=6,CSCTF mode=14","trkLct stations when EMTF mode=6,CSCTF mode=14",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_6_CSCTF_mode_14 = new TH1F("leg_trkLct stations when EMTF mode=6,CSCTF mode=14","leg_trkLct stations when EMTF mode=6,CSCTF mode=14",6,0,6);
    TH1F *RECO_stations_EMTF_mode_6_CSCTF_mode_14 = new TH1F("recoCscSeg stations when EMTF mode=6,CSCTF mode=14","recoCscSeg stations when EMTF mode=6,CSCTF mode=14",6,0,6);
    TH1F *EMTF_stations_EMTF_mode_14_CSCTF_mode_6 = new TH1F("trkLct stations when EMTF mode=14,CSCTF mode=6","trkLct stations when EMTF mode=14,CSCTF mode=6",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_14_CSCTF_mode_6 = new TH1F("leg_trkLct stations when EMTF mode=14,CSCTF mode=6","leg_trkLct stations when EMTF mode=14,CSCTF mode=6",6,0,6);
    TH1F *RECO_stations_EMTF_mode_14_CSCTF_mode_6 = new TH1F("recoCscSeg stations when EMTF mode=14,CSCTF mode=6","recoCscSeg stations when EMTF mode=14,CSCTF mode=6",6,0,6);
    
    //(9,11)-(11,9): pms 3
    TH1F *EMTF_stations_EMTF_mode_9_CSCTF_mode_11 = new TH1F("trkLct stations when EMTF mode=9,CSCTF mode=11","trkLct stations when EMTF mode=9,CSCTF mode=11",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_9_CSCTF_mode_11 = new TH1F("leg_trkLct stations when EMTF mode=9,CSCTF mode=11","leg_trkLct stations when EMTF mode=9,CSCTF mode=11",6,0,6);
    TH1F *RECO_stations_EMTF_mode_9_CSCTF_mode_11 = new TH1F("recoCscSeg stations when EMTF mode=9,CSCTF mode=11","recoCscSeg stations when EMTF mode=9,CSCTF mode=11",6,0,6);
    TH1F *EMTF_stations_EMTF_mode_11_CSCTF_mode_9 = new TH1F("trkLct stations when EMTF mode=11,CSCTF mode=9","trkLct stations when EMTF mode=11,CSCTF mode=9",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_11_CSCTF_mode_9 = new TH1F("leg_trkLct stations when EMTF mode=11,CSCTF mode=9","leg_trkLct stations when EMTF mode=11,CSCTF mode=9",6,0,6);
    TH1F *RECO_stations_EMTF_mode_11_CSCTF_mode_9 = new TH1F("recoCscSeg stations when EMTF mode=11,CSCTF mode=9","recoCscSeg stations when EMTF mode=11,CSCTF mode=9",6,0,6);
    
    //(9,13)-(13,9): pms 2
    TH1F *EMTF_stations_EMTF_mode_9_CSCTF_mode_13 = new TH1F("trkLct stations when EMTF mode=9,CSCTF mode=13","trkLct stations when EMTF mode=9,CSCTF mode=13",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_9_CSCTF_mode_13 = new TH1F("leg_trkLct stations when EMTF mode=9,CSCTF mode=13","leg_trkLct stations when EMTF mode=9,CSCTF mode=13",6,0,6);
    TH1F *RECO_stations_EMTF_mode_9_CSCTF_mode_13 = new TH1F("recoCscSeg stations when EMTF mode=9,CSCTF mode=13","recoCscSeg stations when EMTF mode=9,CSCTF mode=13",6,0,6);
    TH1F *EMTF_stations_EMTF_mode_13_CSCTF_mode_9 = new TH1F("trkLct stations when EMTF mode=13,CSCTF mode=9","trkLct stations when EMTF mode=13,CSCTF mode=9",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_13_CSCTF_mode_9 = new TH1F("leg_trkLct stations when EMTF mode=13,CSCTF mode=9","leg_trkLct stations when EMTF mode=13,CSCTF mode=9",6,0,6);
    TH1F *RECO_stations_EMTF_mode_13_CSCTF_mode_9 = new TH1F("recoCscSeg stations when EMTF mode=13,CSCTF mode=9","recoCscSeg stations when EMTF mode=13,CSCTF mode=9",6,0,6);
    
    //(10,11)-(11,10) pms 4
    TH1F *EMTF_stations_EMTF_mode_10_CSCTF_mode_11 = new TH1F("trkLct stations when EMTF mode=10,CSCTF mode=11","trkLct stations when EMTF mode=10,CSCTF mode=11",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_10_CSCTF_mode_11 = new TH1F("leg_trkLct stations when EMTF mode=10,CSCTF mode=11","leg_trkLct stations when EMTF mode=10,CSCTF mode=11",6,0,6);
    TH1F *RECO_stations_EMTF_mode_10_CSCTF_mode_11 = new TH1F("recoCscSeg stations when EMTF mode=10,CSCTF mode=11","recoCscSeg stations when EMTF mode=10,CSCTF mode=11",6,0,6);
    TH1F *EMTF_stations_EMTF_mode_11_CSCTF_mode_10 = new TH1F("trkLct stations when EMTF mode=11,CSCTF mode=10","trkLct stations when EMTF mode=11,CSCTF mode=10",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_11_CSCTF_mode_10 = new TH1F("leg_trkLct stations when EMTF mode=11,CSCTF mode=10","leg_trkLct stations when EMTF mode=11,CSCTF mode=10",6,0,6);
    TH1F *RECO_stations_EMTF_mode_11_CSCTF_mode_10 = new TH1F("recoCscSeg stations when EMTF mode=11,CSCTF mode=10","recoCscSeg stations when EMTF mode=11,CSCTF mode=10",6,0,6);
    
    //(10,14)-(14,10) pms2
    TH1F *EMTF_stations_EMTF_mode_10_CSCTF_mode_14 = new TH1F("trkLct stations when EMTF mode=10,CSCTF mode=14","trkLct stations when EMTF mode=10,CSCTF mode=14",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_10_CSCTF_mode_14 = new TH1F("leg_trkLct stations when EMTF mode=10,CSCTF mode=14","leg_trkLct stations when EMTF mode=10,CSCTF mode=14",6,0,6);
    TH1F *RECO_stations_EMTF_mode_10_CSCTF_mode_14 = new TH1F("recoCscSeg stations when EMTF mode=10,CSCTF mode=14","recoCscSeg stations when EMTF mode=10,CSCTF mode=14",6,0,6);
    TH1F *EMTF_stations_EMTF_mode_14_CSCTF_mode_10 = new TH1F("trkLct stations when EMTF mode=14,CSCTF mode=10","trkLct stations when EMTF mode=14,CSCTF mode=10",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_14_CSCTF_mode_10 = new TH1F("leg_trkLct stations when EMTF mode=14,CSCTF mode=10","leg_trkLct stations when EMTF mode=14,CSCTF mode=10",6,0,6);
    TH1F *RECO_stations_EMTF_mode_14_CSCTF_mode_10 = new TH1F("recoCscSeg stations when EMTF mode=14,CSCTF mode=10","recoCscSeg stations when EMTF mode=14,CSCTF mode=10",6,0,6);
    
    //(12,13)-(13,12) pms 4
    TH1F *EMTF_stations_EMTF_mode_12_CSCTF_mode_13 = new TH1F("trkLct stations when EMTF mode=12,CSCTF mode=13","trkLct stations when EMTF mode=12,CSCTF mode=13",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_12_CSCTF_mode_13 = new TH1F("leg_trkLct stations when EMTF mode=12,CSCTF mode=13","leg_trkLct stations when EMTF mode=12,CSCTF mode=13",6,0,6);
    TH1F *RECO_stations_EMTF_mode_12_CSCTF_mode_13 = new TH1F("recoCscSeg stations when EMTF mode=12,CSCTF mode=13","recoCscSeg stations when EMTF mode=12,CSCTF mode=13",6,0,6);
    TH1F *EMTF_stations_EMTF_mode_13_CSCTF_mode_12 = new TH1F("trkLct stations when EMTF mode=13,CSCTF mode=12","trkLct stations when EMTF mode=13,CSCTF mode=12",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_13_CSCTF_mode_12 = new TH1F("leg_trkLct stations when EMTF mode=13,CSCTF mode=12","leg_trkLct stations when EMTF mode=13,CSCTF mode=12",6,0,6);
    TH1F *RECO_stations_EMTF_mode_13_CSCTF_mode_12 = new TH1F("recoCscSeg stations when EMTF mode=13,CSCTF mode=12","recoCscSeg stations when EMTF mode=13,CSCTF mode=12",6,0,6);
    
    //(12,14)-(14,12) pms 3
    TH1F *EMTF_stations_EMTF_mode_12_CSCTF_mode_14 = new TH1F("trkLct stations when EMTF mode=12,CSCTF mode=14","trkLct stations when EMTF mode=12,CSCTF mode=14",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_12_CSCTF_mode_14 = new TH1F("leg_trkLct stations when EMTF mode=12,CSCTF mode=14","leg_trkLct stations when EMTF mode=12,CSCTF mode=14",6,0,6);
    TH1F *RECO_stations_EMTF_mode_12_CSCTF_mode_14 = new TH1F("recoCscSeg stations when EMTF mode=12,CSCTF mode=14","recoCscSeg stations when EMTF mode=12,CSCTF mode=14",6,0,6);
    TH1F *EMTF_stations_EMTF_mode_14_CSCTF_mode_12 = new TH1F("trkLct stations when EMTF mode=14,CSCTF mode=12","trkLct stations when EMTF mode=14,CSCTF mode=12",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_14_CSCTF_mode_12 = new TH1F("leg_trkLct stations when EMTF mode=14,CSCTF mode=12","leg_trkLct stations when EMTF mode=14,CSCTF mode=12",6,0,6);
    TH1F *RECO_stations_EMTF_mode_14_CSCTF_mode_12 = new TH1F("recoCscSeg stations when EMTF mode=14,CSCTF mode=12","recoCscSeg stations when EMTF mode=14,CSCTF mode=12",6,0,6);
    //}}}
    
    //=============================
    //2-station,4-station mode pair
    //=============================
    //{{{
    
    //(3,15)-(15,3)
    TH1F *EMTF_stations_EMTF_mode_3_CSCTF_mode_15 = new TH1F("trkLct stations when EMTF mode=3,CSCTF mode=15","trkLct stations when EMTF mode=3,CSCTF mode=15",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_3_CSCTF_mode_15 = new TH1F("leg_trkLct stations when EMTF mode=3,CSCTF mode=15","leg_trkLct stations when EMTF mode=3,CSCTF mode=15",6,0,6);
    TH1F *RECO_stations_EMTF_mode_3_CSCTF_mode_15 = new TH1F("recoCscSeg stations when EMTF mode=3,CSCTF mode=15","recoCscSeg stations when EMTF mode=3,CSCTF mode=15",6,0,6);
    TH1F *EMTF_stations_EMTF_mode_15_CSCTF_mode_3 = new TH1F("trkLct stations when EMTF mode=15,CSCTF mode=3","trkLct stations when EMTF mode=15,CSCTF mode=3",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_15_CSCTF_mode_3 = new TH1F("leg_trkLct stations when EMTF mode=15,CSCTF mode=3","leg_trkLct stations when EMTF mode=15,CSCTF mode=3",6,0,6);
    TH1F *RECO_stations_EMTF_mode_15_CSCTF_mode_3 = new TH1F("recoCscSeg stations when EMTF mode=15,CSCTF mode=3","recoCscSeg stations when EMTF mode=15,CSCTF mode=3",6,0,6);
    
    //(5,15)-(15,5)
    TH1F *EMTF_stations_EMTF_mode_5_CSCTF_mode_15 = new TH1F("trkLct stations when EMTF mode=5,CSCTF mode=15","trkLct stations when EMTF mode=5,CSCTF mode=15",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_5_CSCTF_mode_15 = new TH1F("leg_trkLct stations when EMTF mode=5,CSCTF mode=15","leg_trkLct stations when EMTF mode=5,CSCTF mode=15",6,0,6);
    TH1F *RECO_stations_EMTF_mode_5_CSCTF_mode_15 = new TH1F("recoCscSeg stations when EMTF mode=5,CSCTF mode=15","recoCscSeg stations when EMTF mode=5,CSCTF mode=15",6,0,6);
    TH1F *EMTF_stations_EMTF_mode_15_CSCTF_mode_5 = new TH1F("trkLct stations when EMTF mode=15,CSCTF mode=5","trkLct stations when EMTF mode=15,CSCTF mode=5",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_15_CSCTF_mode_5 = new TH1F("leg_trkLct stations when EMTF mode=15,CSCTF mode=5","leg_trkLct stations when EMTF mode=15,CSCTF mode=5",6,0,6);
    TH1F *RECO_stations_EMTF_mode_15_CSCTF_mode_5 = new TH1F("recoCscSeg stations when EMTF mode=15,CSCTF mode=5","recoCscSeg stations when EMTF mode=15,CSCTF mode=5",6,0,6);
    
    //(6,15)-(15,6)
    TH1F *EMTF_stations_EMTF_mode_6_CSCTF_mode_15 = new TH1F("trkLct stations when EMTF mode=6,CSCTF mode=15","trkLct stations when EMTF mode=6,CSCTF mode=15",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_6_CSCTF_mode_15 = new TH1F("leg_trkLct stations when EMTF mode=6,CSCTF mode=15","leg_trkLct stations when EMTF mode=6,CSCTF mode=15",6,0,6);
    TH1F *RECO_stations_EMTF_mode_6_CSCTF_mode_15 = new TH1F("recoCscSeg stations when EMTF mode=6,CSCTF mode=15","recoCscSeg stations when EMTF mode=6,CSCTF mode=15",6,0,6);
    TH1F *EMTF_stations_EMTF_mode_15_CSCTF_mode_6 = new TH1F("trkLct stations when EMTF mode=15,CSCTF mode=6","trkLct stations when EMTF mode=15,CSCTF mode=6",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_15_CSCTF_mode_6 = new TH1F("leg_trkLct stations when EMTF mode=15,CSCTF mode=6","leg_trkLct stations when EMTF mode=15,CSCTF mode=6",6,0,6);
    TH1F *RECO_stations_EMTF_mode_15_CSCTF_mode_6 = new TH1F("recoCscSeg stations when EMTF mode=15,CSCTF mode=6","recoCscSeg stations when EMTF mode=15,CSCTF mode=6",6,0,6);
    
    //(9,15)-(15,9)
    TH1F *EMTF_stations_EMTF_mode_9_CSCTF_mode_15 = new TH1F("trkLct stations when EMTF mode=9,CSCTF mode=15","trkLct stations when EMTF mode=9,CSCTF mode=15",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_9_CSCTF_mode_15 = new TH1F("leg_trkLct stations when EMTF mode=9,CSCTF mode=15","leg_trkLct stations when EMTF mode=9,CSCTF mode=15",6,0,6);
    TH1F *RECO_stations_EMTF_mode_9_CSCTF_mode_15 = new TH1F("recoCscSeg stations when EMTF mode=9,CSCTF mode=15","recoCscSeg stations when EMTF mode=9,CSCTF mode=15",6,0,6);
    TH1F *EMTF_stations_EMTF_mode_15_CSCTF_mode_9 = new TH1F("trkLct stations when EMTF mode=15,CSCTF mode=9","trkLct stations when EMTF mode=15,CSCTF mode=9",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_15_CSCTF_mode_9 = new TH1F("leg_trkLct stations when EMTF mode=15,CSCTF mode=9","leg_trkLct stations when EMTF mode=15,CSCTF mode=9",6,0,6);
    TH1F *RECO_stations_EMTF_mode_15_CSCTF_mode_9 = new TH1F("recoCscSeg stations when EMTF mode=15,CSCTF mode=9","recoCscSeg stations when EMTF mode=15,CSCTF mode=9",6,0,6);
    
    //(10,15)-(15,10)
    TH1F *EMTF_stations_EMTF_mode_10_CSCTF_mode_15 = new TH1F("trkLct stations when EMTF mode=10,CSCTF mode=15","trkLct stations when EMTF mode=10,CSCTF mode=15",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_10_CSCTF_mode_15 = new TH1F("leg_trkLct stations when EMTF mode=10,CSCTF mode=15","leg_trkLct stations when EMTF mode=10,CSCTF mode=15",6,0,6);
    TH1F *RECO_stations_EMTF_mode_10_CSCTF_mode_15 = new TH1F("recoCscSeg stations when EMTF mode=10,CSCTF mode=15","recoCscSeg stations when EMTF mode=10,CSCTF mode=15",6,0,6);
    TH1F *EMTF_stations_EMTF_mode_15_CSCTF_mode_10 = new TH1F("trkLct stations when EMTF mode=15,CSCTF mode=10","trkLct stations when EMTF mode=15,CSCTF mode=10",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_15_CSCTF_mode_10 = new TH1F("leg_trkLct stations when EMTF mode=15,CSCTF mode=10","leg_trkLct stations when EMTF mode=15,CSCTF mode=10",6,0,6);
    TH1F *RECO_stations_EMTF_mode_15_CSCTF_mode_10 = new TH1F("recoCscSeg stations when EMTF mode=15,CSCTF mode=10","recoCscSeg stations when EMTF mode=15,CSCTF mode=10",6,0,6);
    
    //(12,15)-(15,12)
    TH1F *EMTF_stations_EMTF_mode_12_CSCTF_mode_15 = new TH1F("trkLct stations when EMTF mode=12,CSCTF mode=15","trkLct stations when EMTF mode=12,CSCTF mode=15",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_12_CSCTF_mode_15 = new TH1F("leg_trkLct stations when EMTF mode=12,CSCTF mode=15","leg_trkLct stations when EMTF mode=12,CSCTF mode=15",6,0,6);
    TH1F *RECO_stations_EMTF_mode_12_CSCTF_mode_15 = new TH1F("recoCscSeg stations when EMTF mode=12,CSCTF mode=15","recoCscSeg stations when EMTF mode=12,CSCTF mode=15",6,0,6);
    TH1F *EMTF_stations_EMTF_mode_15_CSCTF_mode_12 = new TH1F("trkLct stations when EMTF mode=15,CSCTF mode=12","trkLct stations when EMTF mode=15,CSCTF mode=12",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_15_CSCTF_mode_12 = new TH1F("leg_trkLct stations when EMTF mode=15,CSCTF mode=12","leg_trkLct stations when EMTF mode=15,CSCTF mode=12",6,0,6);
    TH1F *RECO_stations_EMTF_mode_15_CSCTF_mode_12 = new TH1F("recoCscSeg stations when EMTF mode=15,CSCTF mode=12","recoCscSeg stations when EMTF mode=15,CSCTF mode=12",6,0,6);
    //}}}
    
    //=======================
    //added to v2.1 @08122016
    //for look into equal mode
    //========================
    //equal modes station plots: 3,5,6,7,9,10,11,12,13,14,15
    
    //{{{
    TH1F *EMTF_stations_EMTF_mode_15_CSCTF_mode_15 = new TH1F("trkLct stations when EMTF mode= CSCTF mode=15","trkLct stations when EMTF mode= CSCTF mode=15",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_15_CSCTF_mode_15 = new TH1F("leg_trkLct stations when EMTF mode= CSCTF mode=15","leg_trkLct stations when EMTF mode= CSCTF mode=15",6,0,6);
    TH1F *RECO_stations_EMTF_mode_15_CSCTF_mode_15 = new TH1F("recoCscSeg stations when EMTF mode= CSCTF mode=15","recoCscSeg stations when EMTF mode= CSCTF mode=15",6,0,6);
    
    TH1F *EMTF_stations_EMTF_mode_14_CSCTF_mode_14 = new TH1F("trkLct stations when EMTF mode= CSCTF mode=14","trkLct stations when EMTF mode= CSCTF mode=14",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_14_CSCTF_mode_14 = new TH1F("leg_trkLct stations when EMTF mode= CSCTF mode=14","leg_trkLct stations when EMTF mode= CSCTF mode=14",6,0,6);
    TH1F *RECO_stations_EMTF_mode_14_CSCTF_mode_14 = new TH1F("recoCscSeg stations when EMTF mode= CSCTF mode=14","recoCscSeg stations when EMTF mode= CSCTF mode=14",6,0,6);
    
    TH1F *EMTF_stations_EMTF_mode_13_CSCTF_mode_13 = new TH1F("trkLct stations when EMTF mode= CSCTF mode=13","trkLct stations when EMTF mode= CSCTF mode=13",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_13_CSCTF_mode_13 = new TH1F("leg_trkLct stations when EMTF mode= CSCTF mode=13","leg_trkLct stations when EMTF mode= CSCTF mode=13",6,0,6);
    TH1F *RECO_stations_EMTF_mode_13_CSCTF_mode_13 = new TH1F("recoCscSeg stations when EMTF mode= CSCTF mode=13","recoCscSeg stations when EMTF mode= CSCTF mode=13",6,0,6);
    
    TH1F *EMTF_stations_EMTF_mode_12_CSCTF_mode_12 = new TH1F("trkLct stations when EMTF mode= CSCTF mode=12","trkLct stations when EMTF mode= CSCTF mode=12",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_12_CSCTF_mode_12 = new TH1F("leg_trkLct stations when EMTF mode= CSCTF mode=12","leg_trkLct stations when EMTF mode= CSCTF mode=12",6,0,6);
    TH1F *RECO_stations_EMTF_mode_12_CSCTF_mode_12 = new TH1F("recoCscSeg stations when EMTF mode= CSCTF mode=12","recoCscSeg stations when EMTF mode= CSCTF mode=12",6,0,6);
    
    TH1F *EMTF_stations_EMTF_mode_11_CSCTF_mode_11 = new TH1F("trkLct stations when EMTF mode= CSCTF mode=11","trkLct stations when EMTF mode= CSCTF mode=11",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_11_CSCTF_mode_11 = new TH1F("leg_trkLct stations when EMTF mode= CSCTF mode=11","leg_trkLct stations when EMTF mode= CSCTF mode=11",6,0,6);
    TH1F *RECO_stations_EMTF_mode_11_CSCTF_mode_11 = new TH1F("recoCscSeg stations when EMTF mode= CSCTF mode=11","recoCscSeg stations when EMTF mode= CSCTF mode=11",6,0,6);
    
    TH1F *EMTF_stations_EMTF_mode_10_CSCTF_mode_10 = new TH1F("trkLct stations when EMTF mode= CSCTF mode=10","trkLct stations when EMTF mode= CSCTF mode=10",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_10_CSCTF_mode_10 = new TH1F("leg_trkLct stations when EMTF mode= CSCTF mode=10","leg_trkLct stations when EMTF mode= CSCTF mode=10",6,0,6);
    TH1F *RECO_stations_EMTF_mode_10_CSCTF_mode_10 = new TH1F("recoCscSeg stations when EMTF mode= CSCTF mode=10","recoCscSeg stations when EMTF mode= CSCTF mode=10",6,0,6);
    
    TH1F *EMTF_stations_EMTF_mode_9_CSCTF_mode_9 = new TH1F("trkLct stations when EMTF mode= CSCTF mode=9","trkLct stations when EMTF mode= CSCTF mode=9",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_9_CSCTF_mode_9 = new TH1F("leg_trkLct stations when EMTF mode= CSCTF mode=9","leg_trkLct stations when EMTF mode= CSCTF mode=9",6,0,6);
    TH1F *RECO_stations_EMTF_mode_9_CSCTF_mode_9 = new TH1F("recoCscSeg stations when EMTF mode= CSCTF mode=9","recoCscSeg stations when EMTF mode= CSCTF mode=9",6,0,6);
    
    TH1F *EMTF_stations_EMTF_mode_7_CSCTF_mode_7 = new TH1F("trkLct stations when EMTF mode= CSCTF mode=7","trkLct stations when EMTF mode= CSCTF mode=7",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_7_CSCTF_mode_7 = new TH1F("leg_trkLct stations when EMTF mode= CSCTF mode=7","leg_trkLct stations when EMTF mode= CSCTF mode=7",6,0,6);
    TH1F *RECO_stations_EMTF_mode_7_CSCTF_mode_7 = new TH1F("recoCscSeg stations when EMTF mode= CSCTF mode=7","recoCscSeg stations when EMTF mode= CSCTF mode=7",6,0,6);
    
    TH1F *EMTF_stations_EMTF_mode_6_CSCTF_mode_6 = new TH1F("trkLct stations when EMTF mode= CSCTF mode=6","trkLct stations when EMTF mode= CSCTF mode=6",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_6_CSCTF_mode_6 = new TH1F("leg_trkLct stations when EMTF mode= CSCTF mode=6","leg_trkLct stations when EMTF mode= CSCTF mode=6",6,0,6);
    TH1F *RECO_stations_EMTF_mode_6_CSCTF_mode_6 = new TH1F("recoCscSeg stations when EMTF mode= CSCTF mode=6","recoCscSeg stations when EMTF mode= CSCTF mode=6",6,0,6);
    
    TH1F *EMTF_stations_EMTF_mode_5_CSCTF_mode_5 = new TH1F("trkLct stations when EMTF mode= CSCTF mode=5","trkLct stations when EMTF mode= CSCTF mode=5",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_5_CSCTF_mode_5 = new TH1F("leg_trkLct stations when EMTF mode= CSCTF mode=5","leg_trkLct stations when EMTF mode= CSCTF mode=5",6,0,6);
    TH1F *RECO_stations_EMTF_mode_5_CSCTF_mode_5 = new TH1F("recoCscSeg stations when EMTF mode= CSCTF mode=5","recoCscSeg stations when EMTF mode= CSCTF mode=5",6,0,6);
    
    TH1F *EMTF_stations_EMTF_mode_3_CSCTF_mode_3 = new TH1F("trkLct stations when EMTF mode= CSCTF mode=3","trkLct stations when EMTF mode= CSCTF mode=3",6,0,6);
    TH1F *CSCTF_stations_EMTF_mode_3_CSCTF_mode_3 = new TH1F("leg_trkLct stations when EMTF mode= CSCTF mode=3","leg_trkLct stations when EMTF mode= CSCTF mode=3",6,0,6);
    TH1F *RECO_stations_EMTF_mode_3_CSCTF_mode_3 = new TH1F("recoCscSeg stations when EMTF mode= CSCTF mode=3","recoCscSeg stations when EMTF mode= CSCTF mode=3",6,0,6);
    //}}}
    
    //===============
    //=initializtion=
    //===============
    //The event selection selects only a emtf and
    //csctf trk matched to the same reco trk,
    //so they have same selected number;
    //Define: all trks = selected trks+ unselected trks
    Long64_t all_emtf_tracks=0;
    Long64_t all_csctf_tracks=0;
    Long64_t all_reco_tracks=0;
    Long64_t all_selected_tracks=0;
    
    Long64_t reco_match_emtf_mode_15_tracks[3]={0};
    Long64_t reco_match_emtf_mode_14_tracks[3]={0};
    Long64_t reco_match_emtf_mode_13_tracks[3]={0};
    Long64_t reco_match_emtf_mode_12_tracks[3]={0};
    Long64_t reco_match_emtf_mode_11_tracks[3]={0};
    
    Long64_t reco_match_csctf_mode_15_tracks[3]={0};
    Long64_t reco_match_csctf_mode_14_tracks[3]={0};
    Long64_t reco_match_csctf_mode_13_tracks[3]={0};
    Long64_t reco_match_csctf_mode_12_tracks[3]={0};
    Long64_t reco_match_csctf_mode_11_tracks[3]={0};
    Long64_t selected_events[200000]={-1};//this only initialize the first value to 1, others are 0
    int iselected_event=0;
    
    Long64_t numEvents = myTree->GetEntries();
    cout<<"----------------------------------"<<endl;
    cout<<numEvents<<" events to process....."<<endl;
    
    
    //==================
    //=loop over events=
    //==================
    for(Long64_t iEntry=0;iEntry<numEvents;iEntry++){
        
        myTree->GetEntry(iEntry);//load the ientry-th event
        
        vector<float> recoPt= *RecoPt;//RECO Pt
        vector<float> recoEta= *RecoEta;//RECO eta
        vector<float> recoPhi= *RecoPhi;//RECO Phi
        
        vector<float> trkPt= *TrkPt;//EMTF Pt
        vector<float> trkEta= *TrkEta;//EMTF eta
        vector<float> trkPhi= *TrkPhi;//EMTF phi
        vector<int> trkMode= *TrkMode;//EMTF mode
        vector<int> trkBx= *TrkBx;
        
        vector<float> leg_trkPt= *Leg_TrkPt;//CSCTF
        vector<float> leg_trkEta= *Leg_TrkEta;
        vector<float> leg_trkPhi= *Leg_TrkPhi;
        vector<int> leg_trkMode= *Leg_TrkMode;
        vector<int> leg_trkBx= *Leg_TrkBx;
        
        
        //cout<<"-----------------------"<<endl;
        //cout<<"Processing "<<"event #"<<iEntry<<"......"<<endl;
        
        all_emtf_tracks+=TrkPt->size();
        all_csctf_tracks+=Leg_TrkPt->size();
        all_reco_tracks+=RecoPt->size();
        int event_flag=0;
        
        //=====================================================
        //=Match EMTF trk and CSCTF trk to the same RECO muon =
        //=====================================================
        //Requires EMTF, CSCTF, RECO all size not 0 and cannot
        //select RECO only match to EMTF or RECO match to CSCTF
        if(TrkPt->size()!=0 && Leg_TrkPt->size()!=0 && RecoPt->size()!=0){
            
            int trk_flag[100]={-1};//mark matched EMTF, the marked # is the jreco index
            for(int i=0;i<100;i++){
                trk_flag[i]=-1;
            }
            int leg_trk_flag[100]={-2};//mark matched CSCTF, the marked # is the jreco index
            for(int i=0;i<100;i++){
                leg_trk_flag[i]=-2;
            }
            int reco_emtf_flag[100]={-3};//mark matched RECO to EMTF, take the index of EMTF
            for(int i=0;i<100;i++){
                reco_emtf_flag[i]=-3;
            }
            int reco_csctf_flag[100]={-4};//mark matched RECO to CSCTF, takes the index of CSCTF
            for(int i=0;i<100;i++){
                reco_csctf_flag[i]=-4;
            }
            int matched_emtf_tracks=0;
            int matched_csctf_tracks=0;
            
            //================================================
            //first find the min dR b/t EMTF and RECO, mark; then
            //find the min dR b/t CSCTF and RECO, mark;
            //then compare find the emtf and csctf matched to the
            //same RECO muon
            //================================================
            //=====================================================================
            //loop through EMTF tracks find the min dR b/t EMTF and RECO, mark
            //=====================================================================
            
            //{{{
            while(matched_emtf_tracks!= TrkPt->size() && matched_emtf_tracks!=RecoPt->size()){
                
                //cout<<"Start "<<matched_emtf_tracks+1<<" EMTF matching loop(s)..."<<endl;
                Double_t min_dR_emtf_reco=999;
                int min_dR_itrk=100;
                int min_dR_jreco=100;
                
                //loop to find min dR in rest trks
                for(int jreco=0;jreco<RecoPt->size();jreco++){
                    
                    for(int itrk=0;itrk<TrkPt->size();itrk++){
                        
                        //jump over already selected tracks, mark the trk_flag with jreco;
                        if(trk_flag[itrk]== -1 && reco_emtf_flag[jreco]== -3){
                            
                            //definition for convenience
                            Double_t Delta_Eta_emtf_reco = fabs(trkEta[itrk]) - fabs(recoEta[jreco]);//need fabs to keep the double type, depends on root version
                            Double_t Delta_Phi_emtf_reco = trkPhi[itrk] - recoPhi[jreco];
                            Double_t Delta_R_emtf_reco = sqrt( Delta_Eta_emtf_reco * Delta_Eta_emtf_reco + Delta_Phi_emtf_reco * Delta_Phi_emtf_reco );
                            
                            //find the minimum dR b/t emtf and reco
                            if(min_dR_emtf_reco > Delta_R_emtf_reco){
                                
                                min_dR_emtf_reco = Delta_R_emtf_reco;
                                min_dR_itrk = itrk;
                                min_dR_jreco = jreco;
                                
                            }//end if finding min_dR_emtf_reco
                            
                        }//end if for jump over already selected tracks
                        
                    }//end for loop through EMTF tracks
                    
                }//end for loop through RECO tracks and EMTF tracks
                
                matched_emtf_tracks++;
                trk_flag[min_dR_itrk]= -5;//simple mark
                reco_emtf_flag[min_dR_jreco]= -5;
                
                //further event selection
                if(min_dR_emtf_reco < 0.25 && trkBx[min_dR_itrk]==0){
                    
                    //more detailed mark
                    trk_flag[min_dR_itrk]=min_dR_jreco;
                    reco_emtf_flag[min_dR_jreco]=min_dR_itrk;
                    //cout<<"Well matched trks: dR = "<< min_dR_emtf_reco <<", trkBx="<<trkBx[min_dR_itrk]<<", trk # = "<< min_dR_itrk <<", reco # = "<< min_dR_jreco <<endl;
                    //cout<<"---------------------------"<<endl;
                }//end if for further selection
                
            }//end while for matching emtf to reco
            //}}}
            
            /*
            //debug: print flag table for EMTF and CSCTF
            cout<<"***EMTF***"<<endl;
            for(int i=0;i<TrkPt->size();i++){
                cout<<"EMTF #"<<i<<"--> RECO #"<<trk_flag[i]<<endl;
            }
            cout<<"**********"<<endl;
            //end debug
            */
            
            //======================================================================
            //loop through CSCTF tracks find the min dR b/t CSCTF and RECO, mark
            //======================================================================
            
            //{{{
            while(matched_csctf_tracks!= Leg_TrkPt->size() && matched_csctf_tracks!=RecoPt->size()){
            
                //cout<<"Start "<<matched_csctf_tracks+1<<" CSCTF matching loop(s)..."<<endl;
                Double_t min_dR_csctf_reco=999;
                int min_dR_kleg_trk=100;
                int min_dR_jreco=100;
                
                for(int jreco=0;jreco<RecoPt->size();jreco++){
                    
                    for(int kleg_trk=0;kleg_trk<Leg_TrkPt->size();kleg_trk++){
                        
                        //jump over already selected tracks
                        if(leg_trk_flag[kleg_trk]==-2 && reco_csctf_flag[jreco]==-4){
                            
                            //definition for convenience
                            Double_t Delta_Eta_csctf_reco = fabs(leg_trkEta[kleg_trk]) - fabs(recoEta[jreco]);
                            Double_t Delta_Phi_csctf_reco = leg_trkPhi[kleg_trk] - recoPhi[jreco];
                            Double_t Delta_R_csctf_reco = sqrt( Delta_Eta_csctf_reco * Delta_Eta_csctf_reco + Delta_Phi_csctf_reco * Delta_Phi_csctf_reco);
                            
                            //find the minimum dR b/t csctf and marked reco
                            if(min_dR_csctf_reco > Delta_R_csctf_reco){
                                
                                min_dR_csctf_reco = Delta_R_csctf_reco;
                                min_dR_kleg_trk = kleg_trk;
                                min_dR_jreco =jreco;
                                
                            }//end if finding min_dR_csctf_reco
                            
                        }//end if jump over already selected tracks
                        
                    }//end for loop over CSCTF tracks
                    
                }//end loop through RECO tracks for matching csctf
                
                matched_csctf_tracks++;
                leg_trk_flag[min_dR_kleg_trk]= -6;//simple mark//
                reco_csctf_flag[min_dR_jreco]= -6;
                
                //further event selection
                if(min_dR_csctf_reco < 0.25 && leg_trkBx[min_dR_kleg_trk]==0){
                    //more detailed mark
                    leg_trk_flag[min_dR_kleg_trk]=min_dR_jreco;
                    reco_csctf_flag[min_dR_jreco]=min_dR_kleg_trk;
                    //cout<<"Well matched trks: dR = "<< min_dR_csctf_reco <<", leg_trkBx="<<leg_trkBx[min_dR_kleg_trk]<<", leg_trk # = "<< min_dR_kleg_trk <<", reco # = "<< min_dR_jreco <<endl;
                    //cout<<"----------------"<<endl;
                }//end if for further event selection
                
            }//end while for matching csctf to reco
            //}}}
            
            /*
            //debug: print flag table for EMTF and CSCTF
            cout<<"***CSCTF***"<<endl;
            for(int j=0;j<Leg_TrkPt->size();j++){
                cout<<"CSCTF #"<<j<<"-->RECO #"<<leg_trk_flag[j]<<endl;
            }
            cout<<"***********"<<endl;
            //end debug
            */
            
            //cout<<"Loop over and print matched table for EMTF and CSCTF."<<endl;
            //cout<<"Compare notes..."<<endl;
            //cout<<"EMTF has "<<TrkPt->size()<<" tracks, CSCTF has "<<Leg_TrkPt->size()<<" tracks, RECO has "<<RecoPt->size()<<" tracks."<<endl;
            
            //======================================
            //compare notes to find the common RECO
            //======================================
            int selected_jreco=-1;
            for(int itrk=0;itrk<TrkPt->size();itrk++){
            
                for(int kleg_trk=0;kleg_trk<Leg_TrkPt->size();kleg_trk++){
                    
                    //===============
                    //=select tracks
                    //===============
                    //=added @ 08122016 to v2.0
                    //=when storing LCTs of EMTF and CSCTF, only first 8 tracks are stored, so need to imply here or will out of index
                    //================================================================================================================
                    if(trk_flag[itrk]==leg_trk_flag[kleg_trk] && itrk<8 && kleg_trk<8){
                        
                        event_flag=1;//mark selected event
                        all_selected_tracks++;//take down all selected tracks, same # for RECO, EMTF and CSCTF
                        selected_jreco=trk_flag[itrk];//get the index of selected reco track
                        //cout<<"---------------------------"<<endl;
                        //cout<<"EMTF track #"<<itrk<<": mode= "<<trkMode[itrk]<<endl;
                        //cout<<"CSCTF track #"<<kleg_trk<<": mode= "<<leg_trkMode[kleg_trk]<<endl;
                        //cout<<"RECO track #"<<selected_jreco<<": recoEta= "<<recoEta[selected_jreco]<<endl;
                        
                        //============
                        //=fill plots
                        //============
                        EMTF_modes_CSCTF_modes->Fill(trkMode[itrk],leg_trkMode[kleg_trk]);
                        
                        //=====================================
                        //add to v2.0 08102016
                        //station plots for symmetric mode pair
                        //=====================================
                        //(3-station mode,4-station mode) pairs
                        
                        //{{{
                        //======
                        //(7,15)
                        //======
                        if(trkMode[itrk]==7 && leg_trkMode[kleg_trk]==15){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(7,15) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_7_CSCTF_mode_15->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling -999 in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_7_CSCTF_mode_15->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_7_CSCTF_mode_15->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                            
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        
                        //======
                        //(15,7)
                        //======
                        if(trkMode[itrk]==15 && leg_trkMode[kleg_trk]==7){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(15,7) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_15_CSCTF_mode_7->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling -999 in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_15_CSCTF_mode_7->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_15_CSCTF_mode_7->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        
                        //======
                        //(11,15)
                        //======
                        if(trkMode[itrk]==11 && leg_trkMode[kleg_trk]==15){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(11,15) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_11_CSCTF_mode_15->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling -999 in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_11_CSCTF_mode_15->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_11_CSCTF_mode_15->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        //======
                        //(15,11)
                        //======
                        if(trkMode[itrk]==15 && leg_trkMode[kleg_trk]==11){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(15,11) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_15_CSCTF_mode_11->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling -999 in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_15_CSCTF_mode_11->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_15_CSCTF_mode_11->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        
                        //======
                        //(13,15)
                        //======
                        if(trkMode[itrk]==13 && leg_trkMode[kleg_trk]==15){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(13,15) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_13_CSCTF_mode_15->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling -999 in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_13_CSCTF_mode_15->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_13_CSCTF_mode_15->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        //======
                        //(15,13)
                        //======
                        if(trkMode[itrk]==15 && leg_trkMode[kleg_trk]==13){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(15,13) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_15_CSCTF_mode_13->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling -999 in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_15_CSCTF_mode_13->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_15_CSCTF_mode_13->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        //======
                        //(14,15)
                        //======
                        if(trkMode[itrk]==14 && leg_trkMode[kleg_trk]==15){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(14,15) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_14_CSCTF_mode_15->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling -999 in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_14_CSCTF_mode_15->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_14_CSCTF_mode_15->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        //======
                        //(15,14)
                        //======
                        if(trkMode[itrk]==15 && leg_trkMode[kleg_trk]==14){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(15,14) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_15_CSCTF_mode_14->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling -999 in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_15_CSCTF_mode_14->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_15_CSCTF_mode_14->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        //}}}
                        
                        //==============================
                        //2-station, 3-station mode pair
                        //==============================
                        //{{{
                        
                        //======
                        //(3,7)
                        //======
                        if(trkMode[itrk]==3 && leg_trkMode[kleg_trk]==7){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(3,7) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_3_CSCTF_mode_7->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling negative number in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_3_CSCTF_mode_7->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_3_CSCTF_mode_7->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        //======
                        //(7,3)
                        //======
                        if(trkMode[itrk]==7 && leg_trkMode[kleg_trk]==3){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(7,3) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_7_CSCTF_mode_3->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling -999 in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_7_CSCTF_mode_3->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_7_CSCTF_mode_3->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        //======
                        //(3,11)
                        //======
                        if(trkMode[itrk]==3 && leg_trkMode[kleg_trk]==11){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(3,11) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_3_CSCTF_mode_11->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling negative number in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_3_CSCTF_mode_11->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_3_CSCTF_mode_11->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        //======
                        //(11,3)
                        //======
                        if(trkMode[itrk]==11 && leg_trkMode[kleg_trk]==3){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(11,3) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_11_CSCTF_mode_3->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling -999 in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_11_CSCTF_mode_3->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_11_CSCTF_mode_3->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        
                        //======
                        //(5,13)
                        //======
                        if(trkMode[itrk]==5 && leg_trkMode[kleg_trk]==13){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(5,13) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_5_CSCTF_mode_13->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling -999 in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_5_CSCTF_mode_13->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_5_CSCTF_mode_13->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        //======
                        //(13,5)
                        //======
                        if(trkMode[itrk]==13 && leg_trkMode[kleg_trk]==5){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(13,5) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_13_CSCTF_mode_5->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling -999 in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_13_CSCTF_mode_5->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_13_CSCTF_mode_5->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        //======
                        //(5,7)
                        //======
                        if(trkMode[itrk]==5 && leg_trkMode[kleg_trk]==7){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(5,7) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_5_CSCTF_mode_7->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling -999 in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_5_CSCTF_mode_7->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_5_CSCTF_mode_7->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        //======
                        //(7,5)
                        //======
                        if(trkMode[itrk]==7 && leg_trkMode[kleg_trk]==5){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(7,5) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_7_CSCTF_mode_5->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling -999 in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_7_CSCTF_mode_5->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_7_CSCTF_mode_5->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        //======
                        //(6,7)
                        //======
                        if(trkMode[itrk]==6 && leg_trkMode[kleg_trk]==7){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(6,7) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_6_CSCTF_mode_7->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling -999 in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_6_CSCTF_mode_7->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_6_CSCTF_mode_7->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        //======
                        //(7,6)
                        //======
                        if(trkMode[itrk]==7 && leg_trkMode[kleg_trk]==6){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(7,6) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_7_CSCTF_mode_6->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling -999 in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_7_CSCTF_mode_6->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_7_CSCTF_mode_6->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        
                        //======
                        //(6,14)
                        //======
                        if(trkMode[itrk]==6 && leg_trkMode[kleg_trk]==14){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(6,14) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_6_CSCTF_mode_14->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling -999 in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_6_CSCTF_mode_14->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_6_CSCTF_mode_14->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        //======
                        //(14,6)
                        //======
                        if(trkMode[itrk]==14 && leg_trkMode[kleg_trk]==6){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(14,6) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_14_CSCTF_mode_6->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling -999 in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_14_CSCTF_mode_6->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_14_CSCTF_mode_6->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        //======
                        //(9,11)
                        //======
                        if(trkMode[itrk]==9 && leg_trkMode[kleg_trk]==11){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(9,11) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_9_CSCTF_mode_11->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling -999 in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_9_CSCTF_mode_11->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_9_CSCTF_mode_11->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        //======
                        //(11,9)
                        //======
                        if(trkMode[itrk]==11 && leg_trkMode[kleg_trk]==9){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(11,9) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_11_CSCTF_mode_9->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling -999 in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_11_CSCTF_mode_9->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_11_CSCTF_mode_9->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        //======
                        //(9,13)
                        //======
                        if(trkMode[itrk]==9 && leg_trkMode[kleg_trk]==13){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(9,13) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_9_CSCTF_mode_13->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling -999 in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_9_CSCTF_mode_13->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_9_CSCTF_mode_13->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        //======
                        //(13,9)
                        //======
                        if(trkMode[itrk]==13 && leg_trkMode[kleg_trk]==9){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(13,9) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_13_CSCTF_mode_9->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling -999 in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_13_CSCTF_mode_9->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_13_CSCTF_mode_9->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        //======
                        //(10,11)
                        //======
                        if(trkMode[itrk]==10 && leg_trkMode[kleg_trk]==11){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(10,11) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_10_CSCTF_mode_11->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling negative in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_10_CSCTF_mode_11->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_10_CSCTF_mode_11->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        //======
                        //(11,10)
                        //======
                        if(trkMode[itrk]==11 && leg_trkMode[kleg_trk]==10){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(11,10) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_11_CSCTF_mode_10->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling negative in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_11_CSCTF_mode_10->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_11_CSCTF_mode_10->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        //======
                        //(10,14)
                        //======
                        if(trkMode[itrk]==10 && leg_trkMode[kleg_trk]==14){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(10,14) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_10_CSCTF_mode_14->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling negative in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_10_CSCTF_mode_14->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_10_CSCTF_mode_14->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        
                        //======
                        //(14,10)
                        //======
                        if(trkMode[itrk]==14 && leg_trkMode[kleg_trk]==10){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(14,10) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_14_CSCTF_mode_10->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling negative in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_14_CSCTF_mode_10->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_14_CSCTF_mode_10->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        //======
                        //(12,13)
                        //======
                        if(trkMode[itrk]==12 && leg_trkMode[kleg_trk]==13){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(12,13) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_12_CSCTF_mode_13->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling negative in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_12_CSCTF_mode_13->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_12_CSCTF_mode_13->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        //======
                        //(13,12)
                        //======
                        if(trkMode[itrk]==13 && leg_trkMode[kleg_trk]==12){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(13,12) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_13_CSCTF_mode_12->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling negative in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_13_CSCTF_mode_12->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_13_CSCTF_mode_12->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        //======
                        //(12,14)
                        //======
                        if(trkMode[itrk]==12 && leg_trkMode[kleg_trk]==14){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(12,14) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_12_CSCTF_mode_14->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling negative in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_12_CSCTF_mode_14->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_12_CSCTF_mode_14->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        
                        //======
                        //(14,12)
                        //======
                        if(trkMode[itrk]==14 && leg_trkMode[kleg_trk]==12){
                            //cout<<"In "<<"event #"<<iEntry<<"......"<<endl;
                            //cout<<"*****************"<<endl;
                            //cout<<"(14,12) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_14_CSCTF_mode_12->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling negative in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_14_CSCTF_mode_12->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_14_CSCTF_mode_12->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        
                        //}}}
                        
                        //==========================
                        //added to v2 @08122016
                        //equal modes EMTF and CSCTF
                        //==========================
                        //{{{
                        
                        //(15,15)
                        if(trkMode[itrk]==15 && leg_trkMode[kleg_trk]==15){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(15,15) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_15_CSCTF_mode_15->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling -999 in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_15_CSCTF_mode_15->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_15_CSCTF_mode_15->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        //======
                        //(14,14)
                        //======
                        if(trkMode[itrk]==14 && leg_trkMode[kleg_trk]==14){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(14,14) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_14_CSCTF_mode_14->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling -999 in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_14_CSCTF_mode_14->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_14_CSCTF_mode_14->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        //======
                        //(13,13)
                        //======
                        if(trkMode[itrk]==13 && leg_trkMode[kleg_trk]==13){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(13,13) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_13_CSCTF_mode_13->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling -999 in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_13_CSCTF_mode_13->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_13_CSCTF_mode_13->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        //======
                        //(12,12)
                        //======
                        if(trkMode[itrk]==12 && leg_trkMode[kleg_trk]==12){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(12,12) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_12_CSCTF_mode_12->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling -999 in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_12_CSCTF_mode_12->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_12_CSCTF_mode_12->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        //======
                        //(11,11)
                        //======
                        if(trkMode[itrk]==11 && leg_trkMode[kleg_trk]==11){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(11,11) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_11_CSCTF_mode_11->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling -999 in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_11_CSCTF_mode_11->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_11_CSCTF_mode_11->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        //======
                        //(10,10)
                        //======
                        if(trkMode[itrk]==10 && leg_trkMode[kleg_trk]==10){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(10,10) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_10_CSCTF_mode_10->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling -999 in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_10_CSCTF_mode_10->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_10_CSCTF_mode_10->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        //======
                        //(9,9)
                        //======
                        if(trkMode[itrk]==9 && leg_trkMode[kleg_trk]==9){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(9,9) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_9_CSCTF_mode_9->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling -999 in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_9_CSCTF_mode_9->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_9_CSCTF_mode_9->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        //======
                        //(7,7)
                        //======
                        if(trkMode[itrk]==7 && leg_trkMode[kleg_trk]==7){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(7,7) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_7_CSCTF_mode_7->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling -999 in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_7_CSCTF_mode_7->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_7_CSCTF_mode_7->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        //======
                        //(6,6)
                        //======
                        if(trkMode[itrk]==6 && leg_trkMode[kleg_trk]==6){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(6,6) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_6_CSCTF_mode_6->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling -999 in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_6_CSCTF_mode_6->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_6_CSCTF_mode_6->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        //======
                        //(5,5)
                        //======
                        if(trkMode[itrk]==5 && leg_trkMode[kleg_trk]==5){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(5,5) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_5_CSCTF_mode_5->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling -999 in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_5_CSCTF_mode_5->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_5_CSCTF_mode_5->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        //======
                        //(3,3)
                        //======
                        if(trkMode[itrk]==3 && leg_trkMode[kleg_trk]==3){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(3,3) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_3_CSCTF_mode_3->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling -999 in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_3_CSCTF_mode_3->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_3_CSCTF_mode_3->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        //}}}
                        
                        //========================
                        //2-station, 4-station mode
                        //========================
                        //{{{
                        
                        //======
                        //(3,15)
                        //======
                        if(trkMode[itrk]==3 && leg_trkMode[kleg_trk]==15){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(3,15) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_3_CSCTF_mode_15->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling negative in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_3_CSCTF_mode_15->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_3_CSCTF_mode_15->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        //======
                        //(15,3)
                        //======
                        if(trkMode[itrk]==15 && leg_trkMode[kleg_trk]==3){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(15,3) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_15_CSCTF_mode_3->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling negative in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_15_CSCTF_mode_3->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_15_CSCTF_mode_3->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        
                        //======
                        //(5,15)
                        //======
                        if(trkMode[itrk]==5 && leg_trkMode[kleg_trk]==15){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(5,15) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_5_CSCTF_mode_15->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling negative in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_5_CSCTF_mode_15->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_5_CSCTF_mode_15->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        //======
                        //(15,5)
                        //======
                        if(trkMode[itrk]==15 && leg_trkMode[kleg_trk]==5){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(15,5) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_15_CSCTF_mode_5->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling negative in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_15_CSCTF_mode_5->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_15_CSCTF_mode_5->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        //======
                        //(6,15)
                        //======
                        if(trkMode[itrk]==6 && leg_trkMode[kleg_trk]==15){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(6,15) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_6_CSCTF_mode_15->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling negative in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_6_CSCTF_mode_15->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_6_CSCTF_mode_15->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        //======
                        //(15,6)
                        //======
                        if(trkMode[itrk]==15 && leg_trkMode[kleg_trk]==6){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(15,6) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_15_CSCTF_mode_6->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling negative in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_15_CSCTF_mode_6->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_15_CSCTF_mode_6->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        //======
                        //(9,15)
                        //======
                        if(trkMode[itrk]==9 && leg_trkMode[kleg_trk]==15){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(9,15) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_9_CSCTF_mode_15->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling negative in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_9_CSCTF_mode_15->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_9_CSCTF_mode_15->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        //======
                        //(15,9)
                        //======
                        if(trkMode[itrk]==15 && leg_trkMode[kleg_trk]==9){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(15,9) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_15_CSCTF_mode_9->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling negative in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_15_CSCTF_mode_9->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_15_CSCTF_mode_9->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        //======
                        //(10,15)
                        //======
                        if(trkMode[itrk]==10 && leg_trkMode[kleg_trk]==15){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(10,15) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_10_CSCTF_mode_15->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling negative in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_10_CSCTF_mode_15->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_10_CSCTF_mode_15->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        //======
                        //(15,10)
                        //======
                        if(trkMode[itrk]==15 && leg_trkMode[kleg_trk]==10){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(15,10) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_15_CSCTF_mode_10->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling negative in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_15_CSCTF_mode_10->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_15_CSCTF_mode_10->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        //======
                        //(12,15)
                        //======
                        if(trkMode[itrk]==12 && leg_trkMode[kleg_trk]==15){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(12,15) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_12_CSCTF_mode_15->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling negative in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_12_CSCTF_mode_15->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_12_CSCTF_mode_15->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        //======
                        //(15,12)
                        //======
                        if(trkMode[itrk]==15 && leg_trkMode[kleg_trk]==12){
                            
                            //cout<<"*****************"<<endl;
                            //cout<<"(15,12) mode pair!"<<endl;
                            
                            //=====================================================
                            //create flag for stations hit for EMTF and CSCTF, RECO
                            //=====================================================
                            int itrkLct_flag[4]={0};//could also have duplicate lcts
                            int kleg_trkLct_flag[4]={0};
                            int jrecoCscSeg_flag[4]={0};//if there's any station has a hit, mark 1, avoid repeat fill histograms
                            
                            for(int itrkLct=0; itrkLct<4; itrkLct++) {
                                
                                if (trkLct_station[itrk][itrkLct]>0 && itrkLct_flag[trkLct_station[itrk][itrkLct]-1]==0) {
                                    
                                    EMTF_stations_EMTF_mode_15_CSCTF_mode_12->Fill(trkLct_station[itrk][itrkLct]);
                                    itrkLct_flag[trkLct_station[itrk][itrkLct]-1]=1;
                                    //cout<<"trkLct #"<<itrkLct<<" in station: "<<trkLct_station[itrk][itrkLct]<<endl;
                                    
                                }//end if for not filling negative in the case station has no hits
                                
                                
                            }//end for itrklct
                            //cout<<"----"<<endl;
                            
                            
                            for(int kleg_trkLct=0; kleg_trkLct<4; kleg_trkLct++) {
                                
                                if (leg_trkLct_station[kleg_trk][kleg_trkLct]>0 && kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]==0) {
                                    
                                    CSCTF_stations_EMTF_mode_15_CSCTF_mode_12->Fill(leg_trkLct_station[kleg_trk][kleg_trkLct]);
                                    kleg_trkLct_flag[leg_trkLct_station[kleg_trk][kleg_trkLct]-1]=1;
                                    //cout<<"leg_trkLct #"<<kleg_trkLct<<" in station: "<<leg_trkLct_station[kleg_trk][kleg_trkLct]<<endl;
                                    
                                }//end if
                                
                            }//end for kleg_trkLct
                            //cout<<"----"<<endl;
                            
                            for(int jrecoCscSeg=0; jrecoCscSeg<16;jrecoCscSeg++){
                                
                                //mark the station that has the first hit
                                if (recoCscSeg_station[selected_jreco][jrecoCscSeg]>0 && jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]==0) {
                                    
                                    RECO_stations_EMTF_mode_15_CSCTF_mode_12->Fill(recoCscSeg_station[selected_jreco][jrecoCscSeg]);
                                    jrecoCscSeg_flag[recoCscSeg_station[selected_jreco][jrecoCscSeg]-1]=1;
                                    //cout<<"recoCscSeg #"<<jrecoCscSeg<<" in station: "<<recoCscSeg_station[selected_jreco][jrecoCscSeg]<<endl;
                                    
                                }//end if
                                
                            }//end for jrecoCscSeg
                            
                            //cout<<"*****************"<<endl;
                        }//end if mode pair
                        
                        //}}}
                        
                        
                        //=======================================
                        //separate plots when CSCTF mode is given
                        //=======================================
                        
                        //{{{
                        switch (leg_trkMode[kleg_trk]) {
                            case 15:
                                EMTF_modes_CSCTF_mode_15->Fill(trkMode[itrk]);
                                break;
                                
                            case 14:
                                EMTF_modes_CSCTF_mode_14->Fill(trkMode[itrk]);
                                break;
                                
                            case 13:
                                EMTF_modes_CSCTF_mode_13->Fill(trkMode[itrk]);
                                break;
                            
                            case 12:
                                EMTF_modes_CSCTF_mode_12->Fill(trkMode[itrk]);
                                break;
                                
                            case 11:
                                EMTF_modes_CSCTF_mode_11->Fill(trkMode[itrk]);
                                break;
                                
                            default:
                                break;
                                
                        }//end switch leg_trkMode
                        
                        //}}}
                        
                        //======================================
                        //=separate plot when EMTF mode is given
                        //======================================
                        
                        //{{{
                        
                        switch (trkMode[itrk]) {
                            case 15:
                                CSCTF_modes_EMTF_mode_15->Fill(leg_trkMode[kleg_trk]);
                                break;
                                
                            case 14:
                                CSCTF_modes_EMTF_mode_14->Fill(leg_trkMode[kleg_trk]);
                                break;
                                
                            case 13:
                                CSCTF_modes_EMTF_mode_13->Fill(leg_trkMode[kleg_trk]);
                                break;
                                
                            case 12:
                                CSCTF_modes_EMTF_mode_12->Fill(leg_trkMode[kleg_trk]);
                                break;
                                
                            case 11:
                                CSCTF_modes_EMTF_mode_11->Fill(leg_trkMode[kleg_trk]);
                                break;
                                
                            default:
                                break;
                        }//end switch trkMode
                        //}}}
                        
                        //==========================================================================
                        //=fraction of RECO muons matched to EMTF/CSCTF modes 11:15 in different eta
                        //==========================================================================
                        //we have three eta region, so just use array of size 3
                        //0 for 1.2-1.55; 1 for 1.55-1.85; 2 for 1.85-2.4
                        //{{{
                        
                        if((recoEta[selected_jreco]>=1.2 && recoEta[selected_jreco]<1.55) || (recoEta[selected_jreco]> -1.55 && recoEta[selected_jreco]<= -1.2)){
                            
                            //EMTF modes
                            switch (trkMode[itrk]) {
                                case 15:
                                    reco_match_emtf_mode_15_tracks[0]++;
                                    break;
                                    
                                case 14:
                                    reco_match_emtf_mode_14_tracks[0]++;
                                    break;
                                    
                                case 13:
                                    reco_match_emtf_mode_13_tracks[0]++;
                                    break;
                                    
                                case 12:
                                    reco_match_emtf_mode_12_tracks[0]++;
                                    break;
                                    
                                case 11:
                                    reco_match_emtf_mode_11_tracks[0]++;
                                    break;
                                    
                                default:
                                    break;
                            }//end switch for EMTF modes
                            
                            //CSCTF
                            switch (leg_trkMode[kleg_trk]) {
                                case 15:
                                    reco_match_csctf_mode_15_tracks[0]++;
                                    break;
                                    
                                case 14:
                                    reco_match_csctf_mode_14_tracks[0]++;
                                    break;
                                    
                                case 13:
                                    reco_match_csctf_mode_13_tracks[0]++;
                                    break;
                                    
                                case 12:
                                    reco_match_csctf_mode_12_tracks[0]++;
                                    break;
                                    
                                case 11:
                                    reco_match_csctf_mode_11_tracks[0]++;
                                    break;
                                    
                                default:
                                    break;
                            }//end switch for CSCTF modes
                            
                        }//end if for eta in 1.2-1.55
                        else if((recoEta[selected_jreco]>=1.55 && recoEta[selected_jreco]<1.85) || (recoEta[selected_jreco]> -1.85 && recoEta[selected_jreco]<= -1.55)){
                            
                            //EMTF modes
                            switch (trkMode[itrk]) {
                                case 15:
                                    reco_match_emtf_mode_15_tracks[1]++;
                                    break;
                                    
                                case 14:
                                    reco_match_emtf_mode_14_tracks[1]++;
                                    break;
                                    
                                case 13:
                                    reco_match_emtf_mode_13_tracks[1]++;
                                    break;
                                    
                                case 12:
                                    reco_match_emtf_mode_12_tracks[1]++;
                                    break;
                                    
                                case 11:
                                    reco_match_emtf_mode_11_tracks[1]++;
                                    break;
                                    
                                default:
                                    break;
                            }//end switch for EMTF modes
                            
                            //CSCTF
                            switch (leg_trkMode[kleg_trk]) {
                                case 15:
                                    reco_match_csctf_mode_15_tracks[1]++;
                                    break;
                                    
                                case 14:
                                    reco_match_csctf_mode_14_tracks[1]++;
                                    break;
                                    
                                case 13:
                                    reco_match_csctf_mode_13_tracks[1]++;
                                    break;
                                    
                                case 12:
                                    reco_match_csctf_mode_12_tracks[1]++;
                                    break;
                                    
                                case 11:
                                    reco_match_csctf_mode_11_tracks[1]++;
                                    break;
                                    
                                default:
                                    break;
                            }//end switch for CSCTF modes
                            
                        }//end if for eta in 1.55-1.85
                        else if((recoEta[selected_jreco]>=1.85 && recoEta[selected_jreco]<2.4) || (recoEta[selected_jreco]> -2.4 && recoEta[selected_jreco]<= -1.85)){
                            
                            //EMTF modes
                            switch (trkMode[itrk]) {
                                case 15:
                                    reco_match_emtf_mode_15_tracks[2]++;
                                    break;
                                    
                                case 14:
                                    reco_match_emtf_mode_14_tracks[2]++;
                                    break;
                                    
                                case 13:
                                    reco_match_emtf_mode_13_tracks[2]++;
                                    break;
                                    
                                case 12:
                                    reco_match_emtf_mode_12_tracks[2]++;
                                    break;
                                    
                                case 11:
                                    reco_match_emtf_mode_11_tracks[2]++;
                                    break;
                                    
                                default:
                                    break;
                            }//end switch for EMTF modes
                            
                            //CSCTF
                            switch (leg_trkMode[kleg_trk]) {
                                case 15:
                                    reco_match_csctf_mode_15_tracks[2]++;
                                    break;
                                    
                                case 14:
                                    reco_match_csctf_mode_14_tracks[2]++;
                                    break;
                                    
                                case 13:
                                    reco_match_csctf_mode_13_tracks[2]++;
                                    break;
                                    
                                case 12:
                                    reco_match_csctf_mode_12_tracks[2]++;
                                    break;
                                    
                                case 11:
                                    reco_match_csctf_mode_11_tracks[2]++;
                                    break;
                                    
                                default:
                                    break;
                            }//end switch for CSCTF modes
                            
                        }//end if for eta in 1.85-2.4
                        
                        //}}}
                        
                        
                    }//end if for selected tracks
                    
                }//end for loop over csctf
                
            }//end for loop over emtf
            //end compare notes
            
            
        }//end if all size!=0
        //end processing one event
        
        //store all selected events
        if(event_flag==1){
        
            selected_events[iselected_event]=iEntry;
            iselected_event++;
            //cout<<"The size of trkPt is "<<TrkPt->size()<<endl;
            //cout<<"The size of leg_trkPt is "<<Leg_TrkPt->size()<<endl;
            //cout<<"The size of recoPt is "<<RecoPt->size()<<endl;
        }
        
    }//end for loop over events by ientry
    
    //cout<<"====================================="<<endl;
    //cout<<"Loop on all specified events is over!"<<endl;
    
    //==================
    //calculate fraction
    //==================
    double iEta[3]={1,2,3};
    double RECO_match_EMTF_mode_15_Fraction[3]={0};
    double RECO_match_EMTF_mode_14_Fraction[3]={0};
    double RECO_match_EMTF_mode_13_Fraction[3]={0};
    double RECO_match_EMTF_mode_12_Fraction[3]={0};
    double RECO_match_EMTF_mode_11_Fraction[3]={0};
    
    double RECO_match_CSCTF_mode_15_Fraction[3]={0};
    double RECO_match_CSCTF_mode_14_Fraction[3]={0};
    double RECO_match_CSCTF_mode_13_Fraction[3]={0};
    double RECO_match_CSCTF_mode_12_Fraction[3]={0};
    double RECO_match_CSCTF_mode_11_Fraction[3]={0};
    
    for(int i=0;i<3;i++){
        RECO_match_EMTF_mode_15_Fraction[i]=reco_match_emtf_mode_15_tracks[i]*1.0/all_selected_tracks;
        RECO_match_EMTF_mode_14_Fraction[i]=reco_match_emtf_mode_14_tracks[i]*1.0/all_selected_tracks;
        RECO_match_EMTF_mode_13_Fraction[i]=reco_match_emtf_mode_13_tracks[i]*1.0/all_selected_tracks;
        RECO_match_EMTF_mode_12_Fraction[i]=reco_match_emtf_mode_12_tracks[i]*1.0/all_selected_tracks;
        RECO_match_EMTF_mode_11_Fraction[i]=reco_match_emtf_mode_11_tracks[i]*1.0/all_selected_tracks;
        
        RECO_match_CSCTF_mode_15_Fraction[i]=reco_match_csctf_mode_15_tracks[i]*1.0/all_selected_tracks;
        RECO_match_CSCTF_mode_14_Fraction[i]=reco_match_csctf_mode_14_tracks[i]*1.0/all_selected_tracks;
        RECO_match_CSCTF_mode_13_Fraction[i]=reco_match_csctf_mode_13_tracks[i]*1.0/all_selected_tracks;
        RECO_match_CSCTF_mode_12_Fraction[i]=reco_match_csctf_mode_12_tracks[i]*1.0/all_selected_tracks;
        RECO_match_CSCTF_mode_11_Fraction[i]=reco_match_csctf_mode_11_tracks[i]*1.0/all_selected_tracks;
    }
    
    //====================================
    //=Create file and graph, write plots=
    //====================================
    //color runs from 2,3,4,6,7 to avoid yellow
    
    //cout<<"====================================="<<endl;
    //cout<<"Creating file and making plots..."<<endl;
    
    TFile myPlot("/afs/cern.ch/work/w/wshi/public/EMTFpTResolution/TrackBuild_v2_274440_443.root","RECREATE");
    //TFile myPlot("~/Documents/EMTFpTResolution/TrackBuild_v2_274440_443.root","RECREATE");
    
    
    //{{{
    TCanvas *C1=new TCanvas("C1","EMTF",700,500);
    TCanvas *C2=new TCanvas("C2","CSCTF",700,500);
    
    TGraph *RECO_match_EMTF_mode_15_Fraction_Reco_eta = new TGraph(3,iEta,RECO_match_EMTF_mode_15_Fraction); RECO_match_EMTF_mode_15_Fraction_Reco_eta->SetMarkerStyle(21); RECO_match_EMTF_mode_15_Fraction_Reco_eta->SetMarkerColor(2);
    TGraph *RECO_match_EMTF_mode_14_Fraction_Reco_eta = new TGraph(3,iEta,RECO_match_EMTF_mode_14_Fraction); RECO_match_EMTF_mode_14_Fraction_Reco_eta->SetMarkerStyle(21); RECO_match_EMTF_mode_14_Fraction_Reco_eta->SetMarkerColor(3);
    TGraph *RECO_match_EMTF_mode_13_Fraction_Reco_eta = new TGraph(3,iEta,RECO_match_EMTF_mode_13_Fraction); RECO_match_EMTF_mode_13_Fraction_Reco_eta->SetMarkerStyle(21); RECO_match_EMTF_mode_13_Fraction_Reco_eta->SetMarkerColor(4);
    TGraph *RECO_match_EMTF_mode_12_Fraction_Reco_eta = new TGraph(3,iEta,RECO_match_EMTF_mode_12_Fraction); RECO_match_EMTF_mode_12_Fraction_Reco_eta->SetMarkerStyle(21); RECO_match_EMTF_mode_12_Fraction_Reco_eta->SetMarkerColor(6);
    TGraph *RECO_match_EMTF_mode_11_Fraction_Reco_eta = new TGraph(3,iEta,RECO_match_EMTF_mode_11_Fraction); RECO_match_EMTF_mode_11_Fraction_Reco_eta->SetMarkerStyle(21); RECO_match_EMTF_mode_11_Fraction_Reco_eta->SetMarkerColor(7);
    
    TMultiGraph *mgEMTF=new TMultiGraph();
    C1->cd();
    mgEMTF->SetTitle("RECO match to EMTF modes 11-15");
    mgEMTF->Add(RECO_match_EMTF_mode_15_Fraction_Reco_eta);
    mgEMTF->Add(RECO_match_EMTF_mode_14_Fraction_Reco_eta);
    mgEMTF->Add(RECO_match_EMTF_mode_13_Fraction_Reco_eta);
    mgEMTF->Add(RECO_match_EMTF_mode_12_Fraction_Reco_eta);
    mgEMTF->Add(RECO_match_EMTF_mode_11_Fraction_Reco_eta);
    mgEMTF->Draw();
    mgEMTF->Write();
    
    TGraph *RECO_match_CSCTF_mode_15_Fraction_Reco_eta = new TGraph(3,iEta,RECO_match_CSCTF_mode_15_Fraction); RECO_match_CSCTF_mode_15_Fraction_Reco_eta->SetMarkerStyle(21); RECO_match_CSCTF_mode_15_Fraction_Reco_eta->SetMarkerColor(2);
    TGraph *RECO_match_CSCTF_mode_14_Fraction_Reco_eta = new TGraph(3,iEta,RECO_match_CSCTF_mode_14_Fraction); RECO_match_CSCTF_mode_14_Fraction_Reco_eta->SetMarkerStyle(21); RECO_match_CSCTF_mode_14_Fraction_Reco_eta->SetMarkerColor(3);
    TGraph *RECO_match_CSCTF_mode_13_Fraction_Reco_eta = new TGraph(3,iEta,RECO_match_CSCTF_mode_13_Fraction); RECO_match_CSCTF_mode_13_Fraction_Reco_eta->SetMarkerStyle(21); RECO_match_CSCTF_mode_13_Fraction_Reco_eta->SetMarkerColor(4);
    TGraph *RECO_match_CSCTF_mode_12_Fraction_Reco_eta = new TGraph(3,iEta,RECO_match_CSCTF_mode_12_Fraction); RECO_match_CSCTF_mode_12_Fraction_Reco_eta->SetMarkerStyle(21); RECO_match_CSCTF_mode_12_Fraction_Reco_eta->SetMarkerColor(6);
    TGraph *RECO_match_CSCTF_mode_11_Fraction_Reco_eta = new TGraph(3,iEta,RECO_match_CSCTF_mode_11_Fraction); RECO_match_CSCTF_mode_11_Fraction_Reco_eta->SetMarkerStyle(21); RECO_match_CSCTF_mode_11_Fraction_Reco_eta->SetMarkerColor(7);
    
    TMultiGraph *mgCSCTF=new TMultiGraph();
    C2->cd();
    mgCSCTF->SetTitle("RECO match to CSCTF modes 11-15");
    mgCSCTF->Add(RECO_match_CSCTF_mode_15_Fraction_Reco_eta);
    mgCSCTF->Add(RECO_match_CSCTF_mode_14_Fraction_Reco_eta);
    mgCSCTF->Add(RECO_match_CSCTF_mode_13_Fraction_Reco_eta);
    mgCSCTF->Add(RECO_match_CSCTF_mode_12_Fraction_Reco_eta);
    mgCSCTF->Add(RECO_match_CSCTF_mode_11_Fraction_Reco_eta);
    mgCSCTF->Draw();
    mgCSCTF->Write();
    
    EMTF_modes_CSCTF_modes->Write();
    
    //3-4 station mode pair
    EMTF_stations_EMTF_mode_7_CSCTF_mode_15->Write();
    CSCTF_stations_EMTF_mode_7_CSCTF_mode_15->Write();
    RECO_stations_EMTF_mode_7_CSCTF_mode_15->Write();
    EMTF_stations_EMTF_mode_15_CSCTF_mode_7->Write();
    CSCTF_stations_EMTF_mode_15_CSCTF_mode_7->Write();
    RECO_stations_EMTF_mode_15_CSCTF_mode_7->Write();
    
    EMTF_stations_EMTF_mode_11_CSCTF_mode_15->Write();
    CSCTF_stations_EMTF_mode_11_CSCTF_mode_15->Write();
    RECO_stations_EMTF_mode_11_CSCTF_mode_15->Write();
    EMTF_stations_EMTF_mode_15_CSCTF_mode_11->Write();
    CSCTF_stations_EMTF_mode_15_CSCTF_mode_11->Write();
    RECO_stations_EMTF_mode_15_CSCTF_mode_11->Write();
    
    EMTF_stations_EMTF_mode_13_CSCTF_mode_15->Write();
    CSCTF_stations_EMTF_mode_13_CSCTF_mode_15->Write();
    RECO_stations_EMTF_mode_13_CSCTF_mode_15->Write();
    EMTF_stations_EMTF_mode_15_CSCTF_mode_13->Write();
    CSCTF_stations_EMTF_mode_15_CSCTF_mode_13->Write();
    RECO_stations_EMTF_mode_15_CSCTF_mode_13->Write();
    
    EMTF_stations_EMTF_mode_14_CSCTF_mode_15->Write();
    CSCTF_stations_EMTF_mode_14_CSCTF_mode_15->Write();
    RECO_stations_EMTF_mode_14_CSCTF_mode_15->Write();
    EMTF_stations_EMTF_mode_15_CSCTF_mode_14->Write();
    CSCTF_stations_EMTF_mode_15_CSCTF_mode_14->Write();
    RECO_stations_EMTF_mode_15_CSCTF_mode_14->Write();
    
    //2-3 station mode pair
    EMTF_stations_EMTF_mode_3_CSCTF_mode_7->Write();
    CSCTF_stations_EMTF_mode_3_CSCTF_mode_7->Write();
    RECO_stations_EMTF_mode_3_CSCTF_mode_7->Write();
    EMTF_stations_EMTF_mode_7_CSCTF_mode_3->Write();
    CSCTF_stations_EMTF_mode_7_CSCTF_mode_3->Write();
    RECO_stations_EMTF_mode_7_CSCTF_mode_3->Write();
    
    EMTF_stations_EMTF_mode_3_CSCTF_mode_11->Write();
    CSCTF_stations_EMTF_mode_3_CSCTF_mode_11->Write();
    RECO_stations_EMTF_mode_3_CSCTF_mode_11->Write();
    EMTF_stations_EMTF_mode_11_CSCTF_mode_3->Write();
    CSCTF_stations_EMTF_mode_11_CSCTF_mode_3->Write();
    RECO_stations_EMTF_mode_11_CSCTF_mode_3->Write();
    
    EMTF_stations_EMTF_mode_5_CSCTF_mode_13->Write();
    CSCTF_stations_EMTF_mode_5_CSCTF_mode_13->Write();
    RECO_stations_EMTF_mode_5_CSCTF_mode_13->Write();
    EMTF_stations_EMTF_mode_13_CSCTF_mode_5->Write();
    CSCTF_stations_EMTF_mode_13_CSCTF_mode_5->Write();
    RECO_stations_EMTF_mode_13_CSCTF_mode_5->Write();
    
    EMTF_stations_EMTF_mode_5_CSCTF_mode_7->Write();
    CSCTF_stations_EMTF_mode_5_CSCTF_mode_7->Write();
    RECO_stations_EMTF_mode_5_CSCTF_mode_7->Write();
    EMTF_stations_EMTF_mode_7_CSCTF_mode_5->Write();
    CSCTF_stations_EMTF_mode_7_CSCTF_mode_5->Write();
    RECO_stations_EMTF_mode_7_CSCTF_mode_5->Write();
    
    EMTF_stations_EMTF_mode_6_CSCTF_mode_7->Write();
    CSCTF_stations_EMTF_mode_6_CSCTF_mode_7->Write();
    RECO_stations_EMTF_mode_6_CSCTF_mode_7->Write();
    EMTF_stations_EMTF_mode_7_CSCTF_mode_6->Write();
    CSCTF_stations_EMTF_mode_7_CSCTF_mode_6->Write();
    RECO_stations_EMTF_mode_7_CSCTF_mode_6->Write();
    
    EMTF_stations_EMTF_mode_6_CSCTF_mode_14->Write();
    CSCTF_stations_EMTF_mode_6_CSCTF_mode_14->Write();
    RECO_stations_EMTF_mode_6_CSCTF_mode_14->Write();
    EMTF_stations_EMTF_mode_14_CSCTF_mode_6->Write();
    CSCTF_stations_EMTF_mode_14_CSCTF_mode_6->Write();
    RECO_stations_EMTF_mode_14_CSCTF_mode_6->Write();
    
    EMTF_stations_EMTF_mode_9_CSCTF_mode_11->Write();
    CSCTF_stations_EMTF_mode_9_CSCTF_mode_11->Write();
    RECO_stations_EMTF_mode_9_CSCTF_mode_11->Write();
    EMTF_stations_EMTF_mode_11_CSCTF_mode_9->Write();
    CSCTF_stations_EMTF_mode_11_CSCTF_mode_9->Write();
    RECO_stations_EMTF_mode_11_CSCTF_mode_9->Write();
    
    EMTF_stations_EMTF_mode_9_CSCTF_mode_13->Write();
    CSCTF_stations_EMTF_mode_9_CSCTF_mode_13->Write();
    RECO_stations_EMTF_mode_9_CSCTF_mode_13->Write();
    EMTF_stations_EMTF_mode_13_CSCTF_mode_9->Write();
    CSCTF_stations_EMTF_mode_13_CSCTF_mode_9->Write();
    RECO_stations_EMTF_mode_13_CSCTF_mode_9->Write();
    
    EMTF_stations_EMTF_mode_10_CSCTF_mode_11->Write();
    CSCTF_stations_EMTF_mode_10_CSCTF_mode_11->Write();
    RECO_stations_EMTF_mode_10_CSCTF_mode_11->Write();
    EMTF_stations_EMTF_mode_11_CSCTF_mode_10->Write();
    CSCTF_stations_EMTF_mode_11_CSCTF_mode_10->Write();
    RECO_stations_EMTF_mode_11_CSCTF_mode_10->Write();
    
    EMTF_stations_EMTF_mode_10_CSCTF_mode_14->Write();
    CSCTF_stations_EMTF_mode_10_CSCTF_mode_14->Write();
    RECO_stations_EMTF_mode_10_CSCTF_mode_14->Write();
    EMTF_stations_EMTF_mode_14_CSCTF_mode_10->Write();
    CSCTF_stations_EMTF_mode_14_CSCTF_mode_10->Write();
    RECO_stations_EMTF_mode_14_CSCTF_mode_10->Write();
    
    EMTF_stations_EMTF_mode_12_CSCTF_mode_13->Write();
    CSCTF_stations_EMTF_mode_12_CSCTF_mode_13->Write();
    RECO_stations_EMTF_mode_12_CSCTF_mode_13->Write();
    EMTF_stations_EMTF_mode_13_CSCTF_mode_12->Write();
    CSCTF_stations_EMTF_mode_13_CSCTF_mode_12->Write();
    RECO_stations_EMTF_mode_13_CSCTF_mode_12->Write();
    
    EMTF_stations_EMTF_mode_12_CSCTF_mode_14->Write();
    CSCTF_stations_EMTF_mode_12_CSCTF_mode_14->Write();
    RECO_stations_EMTF_mode_12_CSCTF_mode_14->Write();
    EMTF_stations_EMTF_mode_14_CSCTF_mode_12->Write();
    CSCTF_stations_EMTF_mode_14_CSCTF_mode_12->Write();
    RECO_stations_EMTF_mode_14_CSCTF_mode_12->Write();
    
    //2-4 station mode pair
    EMTF_stations_EMTF_mode_3_CSCTF_mode_15->Write();
    CSCTF_stations_EMTF_mode_3_CSCTF_mode_15->Write();
    RECO_stations_EMTF_mode_3_CSCTF_mode_15->Write();
    EMTF_stations_EMTF_mode_15_CSCTF_mode_3->Write();
    CSCTF_stations_EMTF_mode_15_CSCTF_mode_3->Write();
    RECO_stations_EMTF_mode_15_CSCTF_mode_3->Write();
    
    EMTF_stations_EMTF_mode_5_CSCTF_mode_15->Write();
    CSCTF_stations_EMTF_mode_5_CSCTF_mode_15->Write();
    RECO_stations_EMTF_mode_5_CSCTF_mode_15->Write();
    EMTF_stations_EMTF_mode_15_CSCTF_mode_5->Write();
    CSCTF_stations_EMTF_mode_15_CSCTF_mode_5->Write();
    RECO_stations_EMTF_mode_15_CSCTF_mode_5->Write();
    
    EMTF_stations_EMTF_mode_6_CSCTF_mode_15->Write();
    CSCTF_stations_EMTF_mode_6_CSCTF_mode_15->Write();
    RECO_stations_EMTF_mode_6_CSCTF_mode_15->Write();
    EMTF_stations_EMTF_mode_15_CSCTF_mode_6->Write();
    CSCTF_stations_EMTF_mode_15_CSCTF_mode_6->Write();
    RECO_stations_EMTF_mode_15_CSCTF_mode_6->Write();
    
    EMTF_stations_EMTF_mode_9_CSCTF_mode_15->Write();
    CSCTF_stations_EMTF_mode_9_CSCTF_mode_15->Write();
    RECO_stations_EMTF_mode_9_CSCTF_mode_15->Write();
    EMTF_stations_EMTF_mode_15_CSCTF_mode_9->Write();
    CSCTF_stations_EMTF_mode_15_CSCTF_mode_9->Write();
    RECO_stations_EMTF_mode_15_CSCTF_mode_9->Write();
    
    EMTF_stations_EMTF_mode_10_CSCTF_mode_15->Write();
    CSCTF_stations_EMTF_mode_10_CSCTF_mode_15->Write();
    RECO_stations_EMTF_mode_10_CSCTF_mode_15->Write();
    EMTF_stations_EMTF_mode_15_CSCTF_mode_10->Write();
    CSCTF_stations_EMTF_mode_15_CSCTF_mode_10->Write();
    RECO_stations_EMTF_mode_15_CSCTF_mode_10->Write();
    
    EMTF_stations_EMTF_mode_12_CSCTF_mode_15->Write();
    CSCTF_stations_EMTF_mode_12_CSCTF_mode_15->Write();
    RECO_stations_EMTF_mode_12_CSCTF_mode_15->Write();
    EMTF_stations_EMTF_mode_15_CSCTF_mode_12->Write();
    CSCTF_stations_EMTF_mode_15_CSCTF_mode_12->Write();
    RECO_stations_EMTF_mode_15_CSCTF_mode_12->Write();
    
    //==========
    //Equal modes
    //==========
    EMTF_stations_EMTF_mode_15_CSCTF_mode_15->Write();
    CSCTF_stations_EMTF_mode_15_CSCTF_mode_15->Write();
    RECO_stations_EMTF_mode_15_CSCTF_mode_15->Write();
    
    EMTF_stations_EMTF_mode_14_CSCTF_mode_14->Write();
    CSCTF_stations_EMTF_mode_14_CSCTF_mode_14->Write();
    RECO_stations_EMTF_mode_14_CSCTF_mode_14->Write();
    
    EMTF_stations_EMTF_mode_13_CSCTF_mode_13->Write();
    CSCTF_stations_EMTF_mode_13_CSCTF_mode_13->Write();
    RECO_stations_EMTF_mode_13_CSCTF_mode_13->Write();
    
    EMTF_stations_EMTF_mode_12_CSCTF_mode_12->Write();
    CSCTF_stations_EMTF_mode_12_CSCTF_mode_12->Write();
    RECO_stations_EMTF_mode_12_CSCTF_mode_12->Write();
    
    EMTF_stations_EMTF_mode_11_CSCTF_mode_11->Write();
    CSCTF_stations_EMTF_mode_11_CSCTF_mode_11->Write();
    RECO_stations_EMTF_mode_11_CSCTF_mode_11->Write();
    
    EMTF_stations_EMTF_mode_10_CSCTF_mode_10->Write();
    CSCTF_stations_EMTF_mode_10_CSCTF_mode_10->Write();
    RECO_stations_EMTF_mode_10_CSCTF_mode_10->Write();
    
    EMTF_stations_EMTF_mode_9_CSCTF_mode_9->Write();
    CSCTF_stations_EMTF_mode_9_CSCTF_mode_9->Write();
    RECO_stations_EMTF_mode_9_CSCTF_mode_9->Write();
    
    EMTF_stations_EMTF_mode_7_CSCTF_mode_7->Write();
    CSCTF_stations_EMTF_mode_7_CSCTF_mode_7->Write();
    RECO_stations_EMTF_mode_7_CSCTF_mode_7->Write();
    
    EMTF_stations_EMTF_mode_6_CSCTF_mode_6->Write();
    CSCTF_stations_EMTF_mode_6_CSCTF_mode_6->Write();
    RECO_stations_EMTF_mode_6_CSCTF_mode_6->Write();
    
    EMTF_stations_EMTF_mode_5_CSCTF_mode_5->Write();
    CSCTF_stations_EMTF_mode_5_CSCTF_mode_5->Write();
    RECO_stations_EMTF_mode_5_CSCTF_mode_5->Write();
    
    EMTF_stations_EMTF_mode_3_CSCTF_mode_3->Write();
    CSCTF_stations_EMTF_mode_3_CSCTF_mode_3->Write();
    RECO_stations_EMTF_mode_3_CSCTF_mode_3->Write();
    
    
    //============
    EMTF_modes_CSCTF_mode_15->Write();
    EMTF_modes_CSCTF_mode_14->Write();
    EMTF_modes_CSCTF_mode_13->Write();
    EMTF_modes_CSCTF_mode_12->Write();
    EMTF_modes_CSCTF_mode_11->Write();
    
    CSCTF_modes_EMTF_mode_15->Write();
    CSCTF_modes_EMTF_mode_14->Write();
    CSCTF_modes_EMTF_mode_13->Write();
    CSCTF_modes_EMTF_mode_12->Write();
    CSCTF_modes_EMTF_mode_11->Write();
    
    //}}}
    
    myPlot.Close();
    cout<<"==============="<<endl;
    cout<<"****Summary****"<<endl;
    cout<<numEvents<<" total events."<<endl;
    cout<<iselected_event<<" selected events."<<endl;
    
    cout<<"---------------"<<endl;
    cout<<all_emtf_tracks<<" total EMTF tracks!"<<endl;
    cout<<all_csctf_tracks<<" total CSCTF tracks!"<<endl;
    cout<<all_reco_tracks<<" total RECO tracks!"<<endl;
    cout<<all_selected_tracks<<" total SELECTED tracks!"<<endl;
    cout<<"---------------"<<endl;
    
    /*
    cout<<"Selected events are:"<<endl;
    for(int i=0;i<iselected_event;i++){
    
        cout<<selected_events[i]<<endl;
    }//end for
     */
    cout<<"Save path:/afs/cern.ch/work/w/wshi/public/EMTFpTResolution/TrackBuild_v2_274440_443.root"<<endl;
    cout<<"****************"<<endl;
}

