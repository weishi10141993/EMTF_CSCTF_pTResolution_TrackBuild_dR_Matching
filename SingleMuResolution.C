#include <iostream>
#include <iomanip>
using namespace std;
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
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


//=**************************************************************************************
//=Study the pT resolution performance of some single Mu modes: 15, 14, 7
//=plot EMTF 2017 trigger pT/GEN pT Compare 2016 EMTF trigger pT/GEN pT
//=Mode 15:
//= *Four pT ranges [1, 4], [4, 8], [8, 16], [16, 32] times four abseta ranges: 
//=  [1.2, 1.55], [1.55, 1.85], [1.85, 2.10], [2.10, 2.40]. 
//= *For mode 15 tracks with 1.2 < |eta| < 1.85, in the 4 pT ranges, 
//=  overlay the following 2017 distributions: TRK_mode_RPC == 0, 1, 2, 4, 8.
//=
//=Modes 14 and 7:
//= *In the four pT ranges, overlay 2016, 2017 AbsDev ("BDTG_AWB"), 
//=  Huber ("BDTG_AWB_Hub"), and LeastSquares ("BDTG_AWB_Sq")
//= *Also make the pT 1 - 4 and 4 - 8 plots with a log scale y-axis.
//=
//=Wei Shi @ August 10, 2017 CERN Geneva
//=**************************************************************************************
void SingleMuResolution()
{
    TString MODE[3]={"15","14","7"};
    Double_t mode[3]={15,  14,  7};
    TString PT[5]={"1","4","8","16","32"};//interest pt bin
    Double_t pt[5]={1,  4,  8,  16,  32};
    TString ETA[5]={"1.2","1.55","1.85","2.1","2.4"};//interest eta bin for mode 15
    Double_t eta[5]={1.2,  1.55,  1.85,  2.1,  2.4};
    TString TRACK_MODE_RPC[5]={"0","1","2","4","8"};
    Double_t track_mode_rpc[5]={0,  1,  2,  4,  8};
    Double_t scale_factor_2016=1.4;//2016 pT scale
 
    int MODES;//number of modes
    MODES = sizeof(MODE) / sizeof(TString);
    int PTS;//number of pT bins
    PTS = sizeof(PT) / sizeof(TString) - 1;
    int ETAS;
    ETAS = sizeof(ETA) / sizeof(TString) - 1;
    
    //loop through all modes
    for (int i=0;i<MODES;i++){
        
        //NOTE: for mode 15, I changed the date 06_05 in file name to 05_10 to keep consistency with other modes
        TString fileName="/afs/cern.ch/work/w/wshi/public/EMTFpTResolution/PtRegression_Apr_2017_05_10_invPtTarg_invPtWgt_MODE_" + MODE[i] + "_bitCompr_RPC.root";
        TString directoryName="f_MODE_"+ MODE[i]+ "_invPtTarg_invPtWgt_bitCompr_RPC/TestTree";
        TFile* myFile = new TFile(fileName);
        TTree* myTree = (TTree*) myFile->Get(directoryName);
        
        cout<<"Accessing file:"<<fileName<<endl;
        
        Float_t GEN_pt;
        Float_t GEN_eta;
        Float_t EMTF_pt;
        Float_t BDTG_AWB_Sq;
        Float_t BDTG_AWB_Huber;
        Float_t BDTG_AWB;
        Float_t EMTF_mode;
        Float_t TRK_mode;
        Float_t EMTF_mode_RPC;
        Float_t TRK_mode_RPC;

        //event info: Pull variables from nTuple
        myTree->SetBranchAddress("GEN_pt",&GEN_pt);//GEN pT
        myTree->SetBranchAddress("GEN_eta",&GEN_eta);//GEN eta
        myTree->SetBranchAddress("EMTF_pt",&EMTF_pt);//2016 EMTF
        myTree->SetBranchAddress("BDTG_AWB_Sq",&BDTG_AWB_Sq);//2017: 1/BDTG pT
        myTree->SetBranchAddress("BDTG_AWB_Huber",&BDTG_AWB_Huber);
        myTree->SetBranchAddress("BDTG_AWB",&BDTG_AWB);
        myTree->SetBranchAddress("EMTF_mode",&EMTF_mode);
        myTree->SetBranchAddress("TRK_mode",&TRK_mode);
        myTree->SetBranchAddress("EMTF_mode_RPC",&EMTF_mode_RPC);
        myTree->SetBranchAddress("TRK_mode_RPC",&TRK_mode_RPC);
                                 
        //=****************
        //=make histograms*
        //=****************
        //mode 15: 2016
        if(TRK_mode == mode[0]){
        TH1F *h2016_pT_bin_1_eta_bin_1 = new TH1F("2016 mode "+MODE[i]+" "+PT[0]+"<GEN pT<"+PT[1]+" "+ETA[0]+"<GEN eta<"+ETA[1], "2016 pT Resolution mode "+MODE[i]+" "+PT[0]+"<GEN pT<"+PT[1]+" "+ETA[0]+"<GEN eta<"+ETA[1], 100, 0, 5);
        TH1F *h2016_pT_bin_1_eta_bin_2 = new TH1F("2016 mode "+MODE[i]+" "+PT[0]+"<GEN pT<"+PT[1]+" "+ETA[1]+"<GEN eta<"+ETA[2], "2016 pT Resolution mode "+MODE[i]+" "+PT[0]+"<GEN pT<"+PT[1]+" "+ETA[1]+"<GEN eta<"+ETA[2], 100, 0, 5);
        TH1F *h2016_pT_bin_1_eta_bin_3 = new TH1F("2016 mode "+MODE[i]+" "+PT[0]+"<GEN pT<"+PT[1]+" "+ETA[2]+"<GEN eta<"+ETA[3], "2016 pT Resolution mode "+MODE[i]+" "+PT[0]+"<GEN pT<"+PT[1]+" "+ETA[2]+"<GEN eta<"+ETA[3], 100, 0, 5);
        TH1F *h2016_pT_bin_1_eta_bin_4 = new TH1F("2016 mode "+MODE[i]+" "+PT[0]+"<GEN pT<"+PT[1]+" "+ETA[3]+"<GEN eta<"+ETA[4], "2016 pT Resolution mode "+MODE[i]+" "+PT[0]+"<GEN pT<"+PT[1]+" "+ETA[3]+"<GEN eta<"+ETA[4], 100, 0, 5);
        
        TH1F *h2016_pT_bin_2_eta_bin_1 = new TH1F("2016 mode "+MODE[i]+" "+PT[1]+"<GEN pT<"+PT[2]+" "+ETA[0]+"<GEN eta<"+ETA[1], "2016 pT Resolution mode "+MODE[i]+" "+PT[1]+"<GEN pT<"+PT[2]+" "+ETA[0]+"<GEN eta<"+ETA[1], 100, 0, 5);
        TH1F *h2016_pT_bin_2_eta_bin_2 = new TH1F("2016 mode "+MODE[i]+" "+PT[1]+"<GEN pT<"+PT[2]+" "+ETA[1]+"<GEN eta<"+ETA[2], "2016 pT Resolution mode "+MODE[i]+" "+PT[1]+"<GEN pT<"+PT[2]+" "+ETA[1]+"<GEN eta<"+ETA[2], 100, 0, 5);
        TH1F *h2016_pT_bin_2_eta_bin_3 = new TH1F("2016 mode "+MODE[i]+" "+PT[1]+"<GEN pT<"+PT[2]+" "+ETA[2]+"<GEN eta<"+ETA[3], "2016 pT Resolution mode "+MODE[i]+" "+PT[1]+"<GEN pT<"+PT[2]+" "+ETA[2]+"<GEN eta<"+ETA[3], 100, 0, 5);
        TH1F *h2016_pT_bin_2_eta_bin_4 = new TH1F("2016 mode "+MODE[i]+" "+PT[1]+"<GEN pT<"+PT[2]+" "+ETA[3]+"<GEN eta<"+ETA[4], "2016 pT Resolution mode "+MODE[i]+" "+PT[1]+"<GEN pT<"+PT[2]+" "+ETA[3]+"<GEN eta<"+ETA[4], 100, 0, 5);
        
        TH1F *h2016_pT_bin_3_eta_bin_1 = new TH1F("2016 mode "+MODE[i]+" "+PT[2]+"<GEN pT<"+PT[3]+" "+ETA[0]+"<GEN eta<"+ETA[1], "2016 pT Resolution mode "+MODE[i]+" "+PT[2]+"<GEN pT<"+PT[3]+" "+ETA[0]+"<GEN eta<"+ETA[1], 100, 0, 5);
        TH1F *h2016_pT_bin_3_eta_bin_2 = new TH1F("2016 mode "+MODE[i]+" "+PT[2]+"<GEN pT<"+PT[3]+" "+ETA[1]+"<GEN eta<"+ETA[2], "2016 pT Resolution mode "+MODE[i]+" "+PT[2]+"<GEN pT<"+PT[3]+" "+ETA[1]+"<GEN eta<"+ETA[2], 100, 0, 5);
        TH1F *h2016_pT_bin_3_eta_bin_3 = new TH1F("2016 mode "+MODE[i]+" "+PT[2]+"<GEN pT<"+PT[3]+" "+ETA[2]+"<GEN eta<"+ETA[3], "2016 pT Resolution mode "+MODE[i]+" "+PT[2]+"<GEN pT<"+PT[3]+" "+ETA[2]+"<GEN eta<"+ETA[3], 100, 0, 5);
        TH1F *h2016_pT_bin_3_eta_bin_4 = new TH1F("2016 mode "+MODE[i]+" "+PT[2]+"<GEN pT<"+PT[3]+" "+ETA[3]+"<GEN eta<"+ETA[4], "2016 pT Resolution mode "+MODE[i]+" "+PT[2]+"<GEN pT<"+PT[3]+" "+ETA[3]+"<GEN eta<"+ETA[4], 100, 0, 5);
        
        TH1F *h2016_pT_bin_4_eta_bin_1 = new TH1F("2016 mode "+MODE[i]+" "+PT[3]+"<GEN pT<"+PT[4]+" "+ETA[0]+"<GEN eta<"+ETA[1], "2016 pT Resolution mode "+MODE[i]+" "+PT[3]+"<GEN pT<"+PT[4]+" "+ETA[0]+"<GEN eta<"+ETA[1], 100, 0, 5);
        TH1F *h2016_pT_bin_4_eta_bin_2 = new TH1F("2016 mode "+MODE[i]+" "+PT[3]+"<GEN pT<"+PT[4]+" "+ETA[1]+"<GEN eta<"+ETA[2], "2016 pT Resolution mode "+MODE[i]+" "+PT[3]+"<GEN pT<"+PT[4]+" "+ETA[1]+"<GEN eta<"+ETA[2], 100, 0, 5);
        TH1F *h2016_pT_bin_4_eta_bin_3 = new TH1F("2016 mode "+MODE[i]+" "+PT[3]+"<GEN pT<"+PT[4]+" "+ETA[2]+"<GEN eta<"+ETA[3], "2016 pT Resolution mode "+MODE[i]+" "+PT[3]+"<GEN pT<"+PT[4]+" "+ETA[2]+"<GEN eta<"+ETA[3], 100, 0, 5);
        TH1F *h2016_pT_bin_4_eta_bin_4 = new TH1F("2016 mode "+MODE[i]+" "+PT[3]+"<GEN pT<"+PT[4]+" "+ETA[3]+"<GEN eta<"+ETA[4], "2016 pT Resolution mode "+MODE[i]+" "+PT[3]+"<GEN pT<"+PT[4]+" "+ETA[3]+"<GEN eta<"+ETA[4], 100, 0, 5);
        //mode 15: 2017
        TH1F *h2017_pT_bin_1_eta_bin_1 = new TH1F("2017 mode "+MODE[i]+" "+PT[0]+"<GEN pT<"+PT[1]+" "+ETA[0]+"<GEN eta<"+ETA[1], "2017 pT Resolution mode "+MODE[i]+" "+PT[0]+"<GEN pT<"+PT[1]+" "+ETA[0]+"<GEN eta<"+ETA[1], 100, 0, 5);
        TH1F *h2017_pT_bin_1_eta_bin_2 = new TH1F("2017 mode "+MODE[i]+" "+PT[0]+"<GEN pT<"+PT[1]+" "+ETA[1]+"<GEN eta<"+ETA[2], "2017 pT Resolution mode "+MODE[i]+" "+PT[0]+"<GEN pT<"+PT[1]+" "+ETA[1]+"<GEN eta<"+ETA[2], 100, 0, 5);
        TH1F *h2017_pT_bin_1_eta_bin_3 = new TH1F("2017 mode "+MODE[i]+" "+PT[0]+"<GEN pT<"+PT[1]+" "+ETA[2]+"<GEN eta<"+ETA[3], "2017 pT Resolution mode "+MODE[i]+" "+PT[0]+"<GEN pT<"+PT[1]+" "+ETA[2]+"<GEN eta<"+ETA[3], 100, 0, 5);
        TH1F *h2017_pT_bin_1_eta_bin_4 = new TH1F("2017 mode "+MODE[i]+" "+PT[0]+"<GEN pT<"+PT[1]+" "+ETA[3]+"<GEN eta<"+ETA[4], "2017 pT Resolution mode "+MODE[i]+" "+PT[0]+"<GEN pT<"+PT[1]+" "+ETA[3]+"<GEN eta<"+ETA[4], 100, 0, 5);
        
        TH1F *h2017_pT_bin_2_eta_bin_1 = new TH1F("2017 mode "+MODE[i]+" "+PT[1]+"<GEN pT<"+PT[2]+" "+ETA[0]+"<GEN eta<"+ETA[1], "2017 pT Resolution mode "+MODE[i]+" "+PT[1]+"<GEN pT<"+PT[2]+" "+ETA[0]+"<GEN eta<"+ETA[1], 100, 0, 5);
        TH1F *h2017_pT_bin_2_eta_bin_2 = new TH1F("2017 mode "+MODE[i]+" "+PT[1]+"<GEN pT<"+PT[2]+" "+ETA[1]+"<GEN eta<"+ETA[2], "2017 pT Resolution mode "+MODE[i]+" "+PT[1]+"<GEN pT<"+PT[2]+" "+ETA[1]+"<GEN eta<"+ETA[2], 100, 0, 5);
        TH1F *h2017_pT_bin_2_eta_bin_3 = new TH1F("2017 mode "+MODE[i]+" "+PT[1]+"<GEN pT<"+PT[2]+" "+ETA[2]+"<GEN eta<"+ETA[3], "2017 pT Resolution mode "+MODE[i]+" "+PT[1]+"<GEN pT<"+PT[2]+" "+ETA[2]+"<GEN eta<"+ETA[3], 100, 0, 5);
        TH1F *h2017_pT_bin_2_eta_bin_4 = new TH1F("2017 mode "+MODE[i]+" "+PT[1]+"<GEN pT<"+PT[2]+" "+ETA[3]+"<GEN eta<"+ETA[4], "2017 pT Resolution mode "+MODE[i]+" "+PT[1]+"<GEN pT<"+PT[2]+" "+ETA[3]+"<GEN eta<"+ETA[4], 100, 0, 5);
        
        TH1F *h2017_pT_bin_3_eta_bin_1 = new TH1F("2017 mode "+MODE[i]+" "+PT[2]+"<GEN pT<"+PT[3]+" "+ETA[0]+"<GEN eta<"+ETA[1], "2017 pT Resolution mode "+MODE[i]+" "+PT[2]+"<GEN pT<"+PT[3]+" "+ETA[0]+"<GEN eta<"+ETA[1], 100, 0, 5);
        TH1F *h2017_pT_bin_3_eta_bin_2 = new TH1F("2017 mode "+MODE[i]+" "+PT[2]+"<GEN pT<"+PT[3]+" "+ETA[1]+"<GEN eta<"+ETA[2], "2017 pT Resolution mode "+MODE[i]+" "+PT[2]+"<GEN pT<"+PT[3]+" "+ETA[1]+"<GEN eta<"+ETA[2], 100, 0, 5);
        TH1F *h2017_pT_bin_3_eta_bin_3 = new TH1F("2017 mode "+MODE[i]+" "+PT[2]+"<GEN pT<"+PT[3]+" "+ETA[2]+"<GEN eta<"+ETA[3], "2017 pT Resolution mode "+MODE[i]+" "+PT[2]+"<GEN pT<"+PT[3]+" "+ETA[2]+"<GEN eta<"+ETA[3], 100, 0, 5);
        TH1F *h2017_pT_bin_3_eta_bin_4 = new TH1F("2017 mode "+MODE[i]+" "+PT[2]+"<GEN pT<"+PT[3]+" "+ETA[3]+"<GEN eta<"+ETA[4], "2017 pT Resolution mode "+MODE[i]+" "+PT[2]+"<GEN pT<"+PT[3]+" "+ETA[3]+"<GEN eta<"+ETA[4], 100, 0, 5);
        
        TH1F *h2017_pT_bin_4_eta_bin_1 = new TH1F("2017 mode "+MODE[i]+" "+PT[3]+"<GEN pT<"+PT[4]+" "+ETA[0]+"<GEN eta<"+ETA[1], "2017 pT Resolution mode "+MODE[i]+" "+PT[3]+"<GEN pT<"+PT[4]+" "+ETA[0]+"<GEN eta<"+ETA[1], 100, 0, 5);
        TH1F *h2017_pT_bin_4_eta_bin_2 = new TH1F("2017 mode "+MODE[i]+" "+PT[3]+"<GEN pT<"+PT[4]+" "+ETA[1]+"<GEN eta<"+ETA[2], "2017 pT Resolution mode "+MODE[i]+" "+PT[3]+"<GEN pT<"+PT[4]+" "+ETA[1]+"<GEN eta<"+ETA[2], 100, 0, 5);
        TH1F *h2017_pT_bin_4_eta_bin_3 = new TH1F("2017 mode "+MODE[i]+" "+PT[3]+"<GEN pT<"+PT[4]+" "+ETA[2]+"<GEN eta<"+ETA[3], "2017 pT Resolution mode "+MODE[i]+" "+PT[3]+"<GEN pT<"+PT[4]+" "+ETA[2]+"<GEN eta<"+ETA[3], 100, 0, 5);
        TH1F *h2017_pT_bin_4_eta_bin_4 = new TH1F("2017 mode "+MODE[i]+" "+PT[3]+"<GEN pT<"+PT[4]+" "+ETA[3]+"<GEN eta<"+ETA[4], "2017 pT Resolution mode "+MODE[i]+" "+PT[3]+"<GEN pT<"+PT[4]+" "+ETA[3]+"<GEN eta<"+ETA[4], 100, 0, 5);
        
        //mode 15 compare RPC in different stations 2017 only
        TH1F *h2017_pT_bin_1_RPC_0 = new TH1F("2017 mode "+MODE[i]+" "+PT[0]+"<GEN pT<"+PT[1]+" "+ETA[0]+"<GEN eta<"+ETA[2]+" Track_Mode_RPC"+TRACK_MODE_RPC[0], "2017 pT Resolution mode "+MODE[i]+" "+PT[0]+"<GEN pT<"+PT[1]+" "+ETA[0]+"<GEN eta<"+ETA[2]+" Track_Mode_RPC"+TRACK_MODE_RPC[0], 100, 0, 5);
        TH1F *h2017_pT_bin_1_RPC_1 = new TH1F("2017 mode "+MODE[i]+" "+PT[0]+"<GEN pT<"+PT[1]+" "+ETA[0]+"<GEN eta<"+ETA[2]+" Track_Mode_RPC"+TRACK_MODE_RPC[1], "2017 pT Resolution mode "+MODE[i]+" "+PT[0]+"<GEN pT<"+PT[1]+" "+ETA[0]+"<GEN eta<"+ETA[2]+" Track_Mode_RPC"+TRACK_MODE_RPC[1], 100, 0, 5);
        TH1F *h2017_pT_bin_1_RPC_2 = new TH1F("2017 mode "+MODE[i]+" "+PT[0]+"<GEN pT<"+PT[1]+" "+ETA[0]+"<GEN eta<"+ETA[2]+" Track_Mode_RPC"+TRACK_MODE_RPC[2], "2017 pT Resolution mode "+MODE[i]+" "+PT[0]+"<GEN pT<"+PT[1]+" "+ETA[0]+"<GEN eta<"+ETA[2]+" Track_Mode_RPC"+TRACK_MODE_RPC[2], 100, 0, 5);
        TH1F *h2017_pT_bin_1_RPC_3 = new TH1F("2017 mode "+MODE[i]+" "+PT[0]+"<GEN pT<"+PT[1]+" "+ETA[0]+"<GEN eta<"+ETA[2]+" Track_Mode_RPC"+TRACK_MODE_RPC[3], "2017 pT Resolution mode "+MODE[i]+" "+PT[0]+"<GEN pT<"+PT[1]+" "+ETA[0]+"<GEN eta<"+ETA[2]+" Track_Mode_RPC"+TRACK_MODE_RPC[3], 100, 0, 5);
        TH1F *h2017_pT_bin_1_RPC_4 = new TH1F("2017 mode "+MODE[i]+" "+PT[0]+"<GEN pT<"+PT[1]+" "+ETA[0]+"<GEN eta<"+ETA[2]+" Track_Mode_RPC"+TRACK_MODE_RPC[4], "2017 pT Resolution mode "+MODE[i]+" "+PT[0]+"<GEN pT<"+PT[1]+" "+ETA[0]+"<GEN eta<"+ETA[2]+" Track_Mode_RPC"+TRACK_MODE_RPC[4], 100, 0, 5);
        
        TH1F *h2017_pT_bin_2_RPC_0 = new TH1F("2017 mode "+MODE[i]+" "+PT[1]+"<GEN pT<"+PT[2]+" "+ETA[0]+"<GEN eta<"+ETA[2]+" Track_Mode_RPC"+TRACK_MODE_RPC[0], "2017 pT Resolution mode "+MODE[i]+" "+PT[1]+"<GEN pT<"+PT[2]+" "+ETA[0]+"<GEN eta<"+ETA[2]+" Track_Mode_RPC"+TRACK_MODE_RPC[0], 100, 0, 5);
        TH1F *h2017_pT_bin_2_RPC_1 = new TH1F("2017 mode "+MODE[i]+" "+PT[1]+"<GEN pT<"+PT[2]+" "+ETA[0]+"<GEN eta<"+ETA[2]+" Track_Mode_RPC"+TRACK_MODE_RPC[1], "2017 pT Resolution mode "+MODE[i]+" "+PT[1]+"<GEN pT<"+PT[2]+" "+ETA[0]+"<GEN eta<"+ETA[2]+" Track_Mode_RPC"+TRACK_MODE_RPC[1], 100, 0, 5);
        TH1F *h2017_pT_bin_2_RPC_2 = new TH1F("2017 mode "+MODE[i]+" "+PT[1]+"<GEN pT<"+PT[2]+" "+ETA[0]+"<GEN eta<"+ETA[2]+" Track_Mode_RPC"+TRACK_MODE_RPC[2], "2017 pT Resolution mode "+MODE[i]+" "+PT[1]+"<GEN pT<"+PT[2]+" "+ETA[0]+"<GEN eta<"+ETA[2]+" Track_Mode_RPC"+TRACK_MODE_RPC[2], 100, 0, 5);
        TH1F *h2017_pT_bin_2_RPC_3 = new TH1F("2017 mode "+MODE[i]+" "+PT[1]+"<GEN pT<"+PT[2]+" "+ETA[0]+"<GEN eta<"+ETA[2]+" Track_Mode_RPC"+TRACK_MODE_RPC[3], "2017 pT Resolution mode "+MODE[i]+" "+PT[1]+"<GEN pT<"+PT[2]+" "+ETA[0]+"<GEN eta<"+ETA[2]+" Track_Mode_RPC"+TRACK_MODE_RPC[3], 100, 0, 5);
        TH1F *h2017_pT_bin_2_RPC_4 = new TH1F("2017 mode "+MODE[i]+" "+PT[1]+"<GEN pT<"+PT[2]+" "+ETA[0]+"<GEN eta<"+ETA[2]+" Track_Mode_RPC"+TRACK_MODE_RPC[4], "2017 pT Resolution mode "+MODE[i]+" "+PT[1]+"<GEN pT<"+PT[2]+" "+ETA[0]+"<GEN eta<"+ETA[2]+" Track_Mode_RPC"+TRACK_MODE_RPC[4], 100, 0, 5);
        
        TH1F *h2017_pT_bin_3_RPC_0 = new TH1F("2017 mode "+MODE[i]+" "+PT[2]+"<GEN pT<"+PT[3]+" "+ETA[0]+"<GEN eta<"+ETA[2]+" Track_Mode_RPC"+TRACK_MODE_RPC[0], "2017 pT Resolution mode "+MODE[i]+" "+PT[2]+"<GEN pT<"+PT[3]+" "+ETA[0]+"<GEN eta<"+ETA[2]+" Track_Mode_RPC"+TRACK_MODE_RPC[0], 100, 0, 5);
        TH1F *h2017_pT_bin_3_RPC_1 = new TH1F("2017 mode "+MODE[i]+" "+PT[2]+"<GEN pT<"+PT[3]+" "+ETA[0]+"<GEN eta<"+ETA[2]+" Track_Mode_RPC"+TRACK_MODE_RPC[1], "2017 pT Resolution mode "+MODE[i]+" "+PT[2]+"<GEN pT<"+PT[3]+" "+ETA[0]+"<GEN eta<"+ETA[2]+" Track_Mode_RPC"+TRACK_MODE_RPC[1], 100, 0, 5);
        TH1F *h2017_pT_bin_3_RPC_2 = new TH1F("2017 mode "+MODE[i]+" "+PT[2]+"<GEN pT<"+PT[3]+" "+ETA[0]+"<GEN eta<"+ETA[2]+" Track_Mode_RPC"+TRACK_MODE_RPC[2], "2017 pT Resolution mode "+MODE[i]+" "+PT[2]+"<GEN pT<"+PT[3]+" "+ETA[0]+"<GEN eta<"+ETA[2]+" Track_Mode_RPC"+TRACK_MODE_RPC[2], 100, 0, 5);
        TH1F *h2017_pT_bin_3_RPC_3 = new TH1F("2017 mode "+MODE[i]+" "+PT[2]+"<GEN pT<"+PT[3]+" "+ETA[0]+"<GEN eta<"+ETA[2]+" Track_Mode_RPC"+TRACK_MODE_RPC[3], "2017 pT Resolution mode "+MODE[i]+" "+PT[2]+"<GEN pT<"+PT[3]+" "+ETA[0]+"<GEN eta<"+ETA[2]+" Track_Mode_RPC"+TRACK_MODE_RPC[3], 100, 0, 5);
        TH1F *h2017_pT_bin_3_RPC_4 = new TH1F("2017 mode "+MODE[i]+" "+PT[2]+"<GEN pT<"+PT[3]+" "+ETA[0]+"<GEN eta<"+ETA[2]+" Track_Mode_RPC"+TRACK_MODE_RPC[4], "2017 pT Resolution mode "+MODE[i]+" "+PT[2]+"<GEN pT<"+PT[3]+" "+ETA[0]+"<GEN eta<"+ETA[2]+" Track_Mode_RPC"+TRACK_MODE_RPC[4], 100, 0, 5);
        
        TH1F *h2017_pT_bin_4_RPC_0 = new TH1F("2017 mode "+MODE[i]+" "+PT[3]+"<GEN pT<"+PT[4]+" "+ETA[0]+"<GEN eta<"+ETA[2]+" Track_Mode_RPC"+TRACK_MODE_RPC[0], "2017 pT Resolution mode "+MODE[i]+" "+PT[3]+"<GEN pT<"+PT[4]+" "+ETA[0]+"<GEN eta<"+ETA[2]+" Track_Mode_RPC"+TRACK_MODE_RPC[0], 100, 0, 5);
        TH1F *h2017_pT_bin_4_RPC_1 = new TH1F("2017 mode "+MODE[i]+" "+PT[3]+"<GEN pT<"+PT[4]+" "+ETA[0]+"<GEN eta<"+ETA[2]+" Track_Mode_RPC"+TRACK_MODE_RPC[1], "2017 pT Resolution mode "+MODE[i]+" "+PT[3]+"<GEN pT<"+PT[4]+" "+ETA[0]+"<GEN eta<"+ETA[2]+" Track_Mode_RPC"+TRACK_MODE_RPC[1], 100, 0, 5);
        TH1F *h2017_pT_bin_4_RPC_2 = new TH1F("2017 mode "+MODE[i]+" "+PT[3]+"<GEN pT<"+PT[4]+" "+ETA[0]+"<GEN eta<"+ETA[2]+" Track_Mode_RPC"+TRACK_MODE_RPC[2], "2017 pT Resolution mode "+MODE[i]+" "+PT[3]+"<GEN pT<"+PT[4]+" "+ETA[0]+"<GEN eta<"+ETA[2]+" Track_Mode_RPC"+TRACK_MODE_RPC[2], 100, 0, 5);
        TH1F *h2017_pT_bin_4_RPC_3 = new TH1F("2017 mode "+MODE[i]+" "+PT[3]+"<GEN pT<"+PT[4]+" "+ETA[0]+"<GEN eta<"+ETA[2]+" Track_Mode_RPC"+TRACK_MODE_RPC[3], "2017 pT Resolution mode "+MODE[i]+" "+PT[3]+"<GEN pT<"+PT[4]+" "+ETA[0]+"<GEN eta<"+ETA[2]+" Track_Mode_RPC"+TRACK_MODE_RPC[3], 100, 0, 5);
        TH1F *h2017_pT_bin_4_RPC_4 = new TH1F("2017 mode "+MODE[i]+" "+PT[3]+"<GEN pT<"+PT[4]+" "+ETA[0]+"<GEN eta<"+ETA[2]+" Track_Mode_RPC"+TRACK_MODE_RPC[4], "2017 pT Resolution mode "+MODE[i]+" "+PT[3]+"<GEN pT<"+PT[4]+" "+ETA[0]+"<GEN eta<"+ETA[2]+" Track_Mode_RPC"+TRACK_MODE_RPC[4], 100, 0, 5);
        }
        //mode 14 and 7
        if(TRK_mode == mode[1] || TRK_mode == mode[2]){
            TH1F *h2016_pT_bin_1 = new TH1F("2016 mode "+MODE[i]+" "+PT[0]+"<GEN pT<"+PT[1], "2016 pT Resolution mode "+MODE[i]+" "+PT[0]+"<GEN pT<"+PT[1], 100, 0, 5);
            TH1F *h2017_pT_bin_1_Sq = new TH1F("2017 mode "+MODE[i]+" "+PT[0]+"<GEN pT<"+PT[1], "2017 pT Resolution mode "+MODE[i]+" "+PT[0]+"<GEN pT<"+PT[1]+" LeastSquares", 100, 0, 5);
            TH1F *h2017_pT_bin_1_Huber = new TH1F("2017 mode "+MODE[i]+" "+PT[0]+"<GEN pT<"+PT[1], "2017 pT Resolution mode "+MODE[i]+" "+PT[0]+"<GEN pT<"+PT[1]+" Huber", 100, 0, 5);
            TH1F *h2017_pT_bin_1_AbsDev = new TH1F("2017 mode "+MODE[i]+" "+PT[0]+"<GEN pT<"+PT[1], "2017 pT Resolution mode "+MODE[i]+" "+PT[0]+"<GEN pT<"+PT[1]+" AbsDev", 100, 0, 5);
            
            TH1F *h2016_pT_bin_2 = new TH1F("2016 mode "+MODE[i]+" "+PT[1]+"<GEN pT<"+PT[2], "2016 pT Resolution mode "+MODE[i]+" "+PT[1]+"<GEN pT<"+PT[2], 100, 0, 5);
            TH1F *h2017_pT_bin_2_Sq = new TH1F("2017 mode "+MODE[i]+" "+PT[1]+"<GEN pT<"+PT[2], "2017 pT Resolution mode "+MODE[i]+" "+PT[1]+"<GEN pT<"+PT[2]+" LeastSquares", 100, 0, 5);
            TH1F *h2017_pT_bin_2_Huber = new TH1F("2017 mode "+MODE[i]+" "+PT[1]+"<GEN pT<"+PT[2], "2017 pT Resolution mode "+MODE[i]+" "+PT[1]+"<GEN pT<"+PT[2]+" Huber", 100, 0, 5);
            TH1F *h2017_pT_bin_2_AbsDev = new TH1F("2017 mode "+MODE[i]+" "+PT[1]+"<GEN pT<"+PT[2], "2017 pT Resolution mode "+MODE[i]+" "+PT[1]+"<GEN pT<"+PT[2]+" AbsDev", 100, 0, 5);
            
            TH1F *h2016_pT_bin_3 = new TH1F("2016 mode "+MODE[i]+" "+PT[2]+"<GEN pT<"+PT[3], "2016 pT Resolution mode "+MODE[i]+" "+PT[2]+"<GEN pT<"+PT[3], 100, 0, 5);
            TH1F *h2017_pT_bin_3_Sq = new TH1F("2017 mode "+MODE[i]+" "+PT[2]+"<GEN pT<"+PT[3], "2017 pT Resolution mode "+MODE[i]+" "+PT[2]+"<GEN pT<"+PT[3]+" LeastSquares", 100, 0, 5);
            TH1F *h2017_pT_bin_3_Huber = new TH1F("2017 mode "+MODE[i]+" "+PT[2]+"<GEN pT<"+PT[3], "2017 pT Resolution mode "+MODE[i]+" "+PT[2]+"<GEN pT<"+PT[3]+" Huber", 100, 0, 5);
            TH1F *h2017_pT_bin_3_AbsDev = new TH1F("2017 mode "+MODE[i]+" "+PT[2]+"<GEN pT<"+PT[3], "2017 pT Resolution mode "+MODE[i]+" "+PT[2]+"<GEN pT<"+PT[3]+" AbsDev", 100, 0, 5);
            
            TH1F *h2016_pT_bin_4 = new TH1F("2016 mode "+MODE[i]+" "+PT[3]+"<GEN pT<"+PT[4], "2016 pT Resolution mode "+MODE[i]+" "+PT[3]+"<GEN pT<"+PT[4], 100, 0, 5);
            TH1F *h2017_pT_bin_4_Sq = new TH1F("2017 mode "+MODE[i]+" "+PT[3]+"<GEN pT<"+PT[4], "2017 pT Resolution mode "+MODE[i]+" "+PT[3]+"<GEN pT<"+PT[4]+" LeastSquares", 100, 0, 5);
            TH1F *h2017_pT_bin_4_Huber = new TH1F("2017 mode "+MODE[i]+" "+PT[3]+"<GEN pT<"+PT[4], "2017 pT Resolution mode "+MODE[i]+" "+PT[3]+"<GEN pT<"+PT[4]+" Huber", 100, 0, 5);
            TH1F *h2017_pT_bin_4_AbsDev = new TH1F("2017 mode "+MODE[i]+" "+PT[3]+"<GEN pT<"+PT[4], "2017 pT Resolution mode "+MODE[i]+" "+PT[3]+"<GEN pT<"+PT[4]+" AbsDev", 100, 0, 5);
        }
        
        Long64_t numEvents = myTree->GetEntries();//read the number of entries in myTree
        cout<<">>>>>>>>>>>>>>>>>>>>>"<<endl;
        cout<<numEvents<<" events to process..."<<endl;
        
        for(Long64_t iEntry = 0; iEntry <numEvents; iEntry++){
            //load the i-th event
            myTree->GetEntry(iEntry);
            
            Float_t Ratio2016= EMTF_pt/(GEN_pt*scale_factor_2016);//divide the 2016 scale factor 1.4
            Float_t Ratio2017= 1/(BDTG_AWB_Sq*GEN_pt);
            Float_t Ratio2017Huber= 1/(BDTG_AWB_Huber*GEN_pt);//2017 huber loss function
            Float_t Ratio2017AbsDev= 1/(BDTG_AWB*GEN_pt);//2017 absolute deviation loss func
            
            //ONLY: mode 15, CSC only four pT* four eta bins
            if(TRK_mode == mode[0] && EMTF_mode == TRK_mode && EMTF_mode_RPC == track_mode_rpc[0] && TRK_mode_RPC == track_mode_rpc[0]){
                //pt bin 1
                if(GEN_pt > pt[0] && GEN_pt <= pt[1]){
                    //eta bin 1
                    if( fabs(GEN_eta) > eta[0] && fabs(GEN_eta) <= eta[1] ){
                        h2016_pT_bin_1_eta_bin_1->Fill(Ratio2016);
                        h2017_pT_bin_1_eta_bin_1->Fill(Ratio2017);
                    }
                    if( fabs(GEN_eta) > eta[1] && fabs(GEN_eta) <= eta[2] ){
                        h2016_pT_bin_1_eta_bin_2->Fill(Ratio2016);
                        h2017_pT_bin_1_eta_bin_2->Fill(Ratio2017);
                    }
                    if( fabs(GEN_eta) > eta[2] && fabs(GEN_eta) <= eta[3] ){
                        h2016_pT_bin_1_eta_bin_3->Fill(Ratio2016);
                        h2017_pT_bin_1_eta_bin_3->Fill(Ratio2017);
                    }
                    if( fabs(GEN_eta) > eta[3] && fabs(GEN_eta) <= eta[4] ){
                        h2016_pT_bin_1_eta_bin_4->Fill(Ratio2016);
                        h2017_pT_bin_1_eta_bin_4->Fill(Ratio2017);
                    }
                }//end pt bin 1
                if(GEN_pt > pt[1] && GEN_pt <= pt[2]){
                    //eta bin 1
                    if( fabs(GEN_eta) > eta[0] && fabs(GEN_eta) <= eta[1] ){
                        h2016_pT_bin_2_eta_bin_1->Fill(Ratio2016);
                        h2017_pT_bin_2_eta_bin_1->Fill(Ratio2017);
                    }
                    if( fabs(GEN_eta) > eta[1] && fabs(GEN_eta) <= eta[2] ){
                        h2016_pT_bin_2_eta_bin_2->Fill(Ratio2016);
                        h2017_pT_bin_2_eta_bin_2->Fill(Ratio2017);
                    }
                    if( fabs(GEN_eta) > eta[2] && fabs(GEN_eta) <= eta[3] ){
                        h2016_pT_bin_2_eta_bin_3->Fill(Ratio2016);
                        h2017_pT_bin_2_eta_bin_3->Fill(Ratio2017);
                    }
                    if( fabs(GEN_eta) > eta[3] && fabs(GEN_eta) <= eta[4] ){
                        h2016_pT_bin_2_eta_bin_4->Fill(Ratio2016);
                        h2017_pT_bin_2_eta_bin_4->Fill(Ratio2017);
                    }
                }
                if(GEN_pt > pt[2] && GEN_pt <= pt[3]){
                    //eta bin 1
                    if( fabs(GEN_eta) > eta[0] && fabs(GEN_eta) <= eta[1] ){
                        h2016_pT_bin_3_eta_bin_1->Fill(Ratio2016);
                        h2017_pT_bin_3_eta_bin_1->Fill(Ratio2017);
                    }
                    if( fabs(GEN_eta) > eta[1] && fabs(GEN_eta) <= eta[2] ){
                        h2016_pT_bin_3_eta_bin_2->Fill(Ratio2016);
                        h2017_pT_bin_3_eta_bin_2->Fill(Ratio2017);
                    }
                    if( fabs(GEN_eta) > eta[2] && fabs(GEN_eta) <= eta[3] ){
                        h2016_pT_bin_3_eta_bin_3->Fill(Ratio2016);
                        h2017_pT_bin_3_eta_bin_3->Fill(Ratio2017);
                    }
                    if( fabs(GEN_eta) > eta[3] && fabs(GEN_eta) <= eta[4] ){
                        h2016_pT_bin_3_eta_bin_4->Fill(Ratio2016);
                        h2017_pT_bin_3_eta_bin_4->Fill(Ratio2017);
                    }
                }
                if(GEN_pt > pt[3] && GEN_pt <= pt[4]){
                    //eta bin 1
                    if( fabs(GEN_eta) > eta[0] && fabs(GEN_eta) <= eta[1] ){
                        h2016_pT_bin_4_eta_bin_1->Fill(Ratio2016);
                        h2017_pT_bin_4_eta_bin_1->Fill(Ratio2017);
                    }
                    if( fabs(GEN_eta) > eta[1] && fabs(GEN_eta) <= eta[2] ){
                        h2016_pT_bin_4_eta_bin_2->Fill(Ratio2016);
                        h2017_pT_bin_4_eta_bin_2->Fill(Ratio2017);
                    }
                    if( fabs(GEN_eta) > eta[2] && fabs(GEN_eta) <= eta[3] ){
                        h2016_pT_bin_4_eta_bin_3->Fill(Ratio2016);
                        h2017_pT_bin_4_eta_bin_3->Fill(Ratio2017);
                    }
                    if( fabs(GEN_eta) > eta[3] && fabs(GEN_eta) <= eta[4] ){
                        h2016_pT_bin_4_eta_bin_4->Fill(Ratio2016);
                        h2017_pT_bin_4_eta_bin_4->Fill(Ratio2017);
                    }
                }//end if
                
            }//end if: trk mode selection ONLY MODE 15
            
            //ONLY: mode 15, compare RPC in different stations 2017 only
            if(TRK_mode == mode[0] && fabs(GEN_eta) > eta[0] && fabs(GEN_eta) <= eta[2] ){
                //pT bin 1
                if(GEN_pt > pt[0] && GEN_pt <= pt[1]){
                    if(TRK_mode_RPC == track_mode_rpc[0]){
                        h2017_pT_bin_1_RPC_0->Fill(Ratio2017);
                    }
                    if(TRK_mode_RPC == track_mode_rpc[1]){
                        h2017_pT_bin_1_RPC_1->Fill(Ratio2017);
                    }
                    if(TRK_mode_RPC == track_mode_rpc[2]){
                        h2017_pT_bin_1_RPC_2->Fill(Ratio2017);
                    }
                    if(TRK_mode_RPC == track_mode_rpc[3]){
                        h2017_pT_bin_1_RPC_3->Fill(Ratio2017);
                    }
                    if(TRK_mode_RPC == track_mode_rpc[4]){
                        h2017_pT_bin_1_RPC_4->Fill(Ratio2017);
                    }
                }//end pT bin 1
                //pT bin2
                if(GEN_pt > pt[1] && GEN_pt <= pt[2]){
                    if(TRK_mode_RPC == track_mode_rpc[0]){
                        h2017_pT_bin_2_RPC_0->Fill(Ratio2017);
                    }
                    if(TRK_mode_RPC == track_mode_rpc[1]){
                        h2017_pT_bin_2_RPC_1->Fill(Ratio2017);
                    }
                    if(TRK_mode_RPC == track_mode_rpc[2]){
                        h2017_pT_bin_2_RPC_2->Fill(Ratio2017);
                    }
                    if(TRK_mode_RPC == track_mode_rpc[3]){
                        h2017_pT_bin_2_RPC_3->Fill(Ratio2017);
                    }
                    if(TRK_mode_RPC == track_mode_rpc[4]){
                        h2017_pT_bin_2_RPC_4->Fill(Ratio2017);
                    }
                }
                if(GEN_pt > pt[2] && GEN_pt <= pt[3]){
                    if(TRK_mode_RPC == track_mode_rpc[0]){
                        h2017_pT_bin_3_RPC_0->Fill(Ratio2017);
                    }
                    if(TRK_mode_RPC == track_mode_rpc[1]){
                        h2017_pT_bin_3_RPC_1->Fill(Ratio2017);
                    }
                    if(TRK_mode_RPC == track_mode_rpc[2]){
                        h2017_pT_bin_3_RPC_2->Fill(Ratio2017);
                    }
                    if(TRK_mode_RPC == track_mode_rpc[3]){
                        h2017_pT_bin_3_RPC_3->Fill(Ratio2017);
                    }
                    if(TRK_mode_RPC == track_mode_rpc[4]){
                        h2017_pT_bin_3_RPC_4->Fill(Ratio2017);
                    }
                }
                if(GEN_pt > pt[3] && GEN_pt <= pt[4]){
                    if(TRK_mode_RPC == track_mode_rpc[0]){
                        h2017_pT_bin_4_RPC_0->Fill(Ratio2017);
                    }
                    if(TRK_mode_RPC == track_mode_rpc[1]){
                        h2017_pT_bin_4_RPC_1->Fill(Ratio2017);
                    }
                    if(TRK_mode_RPC == track_mode_rpc[2]){
                        h2017_pT_bin_4_RPC_2->Fill(Ratio2017);
                    }
                    if(TRK_mode_RPC == track_mode_rpc[3]){
                        h2017_pT_bin_4_RPC_3->Fill(Ratio2017);
                    }
                    if(TRK_mode_RPC == track_mode_rpc[4]){
                        h2017_pT_bin_4_RPC_4->Fill(Ratio2017);
                    }
                }//end pT bin 4
            
            }//end if ONLY mode 15 compare RPC in different stations 2017 only
 
            //mode 14,7 : involve 2016, use CSC only
            if(TRK_mode != mode[0] && EMTF_mode == TRK_mode && EMTF_mode_RPC == track_mode_rpc[0] && TRK_mode_RPC == track_mode_rpc[0]){
                //pT bin 1
                if(GEN_pt > pt[0] && GEN_pt <= pt[1]){
                    h2016_pT_bin_1->Fill(Ratio2016);
                    h2017_pT_bin_1_Sq->Fill(Ratio2017);
                    h2017_pT_bin_1_Huber->Fill(Ratio2017Huber);
                    h2017_pT_bin_1_AbsDev->Fill(Ratio2017AbsDev);
                }
                if(GEN_pt > pt[1] && GEN_pt <= pt[2]){
                    h2016_pT_bin_2->Fill(Ratio2016);
                    h2017_pT_bin_2_Sq->Fill(Ratio2017);
                    h2017_pT_bin_2_Huber->Fill(Ratio2017Huber);
                    h2017_pT_bin_2_AbsDev->Fill(Ratio2017AbsDev);
                }
                if(GEN_pt > pt[2] && GEN_pt <= pt[3]){
                    h2016_pT_bin_3->Fill(Ratio2016);
                    h2017_pT_bin_3_Sq->Fill(Ratio2017);
                    h2017_pT_bin_3_Huber->Fill(Ratio2017Huber);
                    h2017_pT_bin_3_AbsDev->Fill(Ratio2017AbsDev);
                }
                if(GEN_pt > pt[3] && GEN_pt <= pt[4]){
                    h2016_pT_bin_4->Fill(Ratio2016);
                    h2017_pT_bin_4_Sq->Fill(Ratio2017);
                    h2017_pT_bin_4_Huber->Fill(Ratio2017Huber);
                    h2017_pT_bin_4_AbsDev->Fill(Ratio2017AbsDev);
                }
            }//end mode 14,7
            
        }//end loop over events
        
        //output
        TString output="/afs/cern.ch/work/w/wshi/public/EMTFpTResolution/SingleMuResolution_Mode_"+MODE[i]+".root";
        TFile myPlot(output,"RECREATE");
        
        myPlot.Close();
        cout<<"***print out***"<<endl;
        cout<<"Mode "<<MODE[i]<<endl;
        cout<<"===Hist Info=="<<endl;
        
        
    }//end loop over modes
    
}
