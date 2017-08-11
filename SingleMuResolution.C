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
        myTree->SetBranchAddress("EMTF_mode",&EMTF_mode);
        myTree->SetBranchAddress("TRK_mode",&TRK_mode);
        myTree->SetBranchAddress("EMTF_mode_RPC",&EMTF_mode_RPC);
        myTree->SetBranchAddress("TRK_mode_RPC",&TRK_mode_RPC);
        //mode 14 and 7
        if(MODE[i] != MODE[0]){
            myTree->SetBranchAddress("BDTG_AWB_Huber",&BDTG_AWB_Huber);
            myTree->SetBranchAddress("BDTG_AWB",&BDTG_AWB);
        }
                                 
        //=****************
        //=make histograms*
        //=****************
        //mode 15: 2016
        //if(MODE[i] == MODE[0]){
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
       // }
        //mode 14 and 7
        //if(MODE[i] != MODE[0]){
            TH1F *h2016_pT_bin_1 = new TH1F("2016 mode "+MODE[i]+" "+PT[0]+"<GEN pT<"+PT[1], "2016 pT Resolution mode "+MODE[i]+" "+PT[0]+"<GEN pT<"+PT[1], 100, 0, 5);
            TH1F *h2017_pT_bin_1_Sq = new TH1F("2017 mode "+MODE[i]+" "+PT[0]+"<GEN pT<"+PT[1]+" Sq", "2017 pT Resolution mode "+MODE[i]+" "+PT[0]+"<GEN pT<"+PT[1]+" LeastSquares", 100, 0, 5);
            TH1F *h2017_pT_bin_1_Huber = new TH1F("2017 mode "+MODE[i]+" "+PT[0]+"<GEN pT<"+PT[1]+" Huber", "2017 pT Resolution mode "+MODE[i]+" "+PT[0]+"<GEN pT<"+PT[1]+" Huber", 100, 0, 5);
            TH1F *h2017_pT_bin_1_AbsDev = new TH1F("2017 mode "+MODE[i]+" "+PT[0]+"<GEN pT<"+PT[1]+" AbsDev", "2017 pT Resolution mode "+MODE[i]+" "+PT[0]+"<GEN pT<"+PT[1]+" AbsDev", 100, 0, 5);
            
            TH1F *h2016_pT_bin_2 = new TH1F("2016 mode "+MODE[i]+" "+PT[1]+"<GEN pT<"+PT[2], "2016 pT Resolution mode "+MODE[i]+" "+PT[1]+"<GEN pT<"+PT[2], 100, 0, 5);
            TH1F *h2017_pT_bin_2_Sq = new TH1F("2017 mode "+MODE[i]+" "+PT[1]+"<GEN pT<"+PT[2]+" Sq", "2017 pT Resolution mode "+MODE[i]+" "+PT[1]+"<GEN pT<"+PT[2]+" LeastSquares", 100, 0, 5);
            TH1F *h2017_pT_bin_2_Huber = new TH1F("2017 mode "+MODE[i]+" "+PT[1]+"<GEN pT<"+PT[2]+" Huber", "2017 pT Resolution mode "+MODE[i]+" "+PT[1]+"<GEN pT<"+PT[2]+" Huber", 100, 0, 5);
            TH1F *h2017_pT_bin_2_AbsDev = new TH1F("2017 mode "+MODE[i]+" "+PT[1]+"<GEN pT<"+PT[2]+" AbsDev", "2017 pT Resolution mode "+MODE[i]+" "+PT[1]+"<GEN pT<"+PT[2]+" AbsDev", 100, 0, 5);
            
            TH1F *h2016_pT_bin_3 = new TH1F("2016 mode "+MODE[i]+" "+PT[2]+"<GEN pT<"+PT[3], "2016 pT Resolution mode "+MODE[i]+" "+PT[2]+"<GEN pT<"+PT[3], 100, 0, 5);
            TH1F *h2017_pT_bin_3_Sq = new TH1F("2017 mode "+MODE[i]+" "+PT[2]+"<GEN pT<"+PT[3]+" Sq", "2017 pT Resolution mode "+MODE[i]+" "+PT[2]+"<GEN pT<"+PT[3]+" LeastSquares", 100, 0, 5);
            TH1F *h2017_pT_bin_3_Huber = new TH1F("2017 mode "+MODE[i]+" "+PT[2]+"<GEN pT<"+PT[3]+" Huber", "2017 pT Resolution mode "+MODE[i]+" "+PT[2]+"<GEN pT<"+PT[3]+" Huber", 100, 0, 5);
            TH1F *h2017_pT_bin_3_AbsDev = new TH1F("2017 mode "+MODE[i]+" "+PT[2]+"<GEN pT<"+PT[3]+" AbsDev", "2017 pT Resolution mode "+MODE[i]+" "+PT[2]+"<GEN pT<"+PT[3]+" AbsDev", 100, 0, 5);
            
            TH1F *h2016_pT_bin_4 = new TH1F("2016 mode "+MODE[i]+" "+PT[3]+"<GEN pT<"+PT[4], "2016 pT Resolution mode "+MODE[i]+" "+PT[3]+"<GEN pT<"+PT[4], 100, 0, 5);
            TH1F *h2017_pT_bin_4_Sq = new TH1F("2017 mode "+MODE[i]+" "+PT[3]+"<GEN pT<"+PT[4]+" Sq", "2017 pT Resolution mode "+MODE[i]+" "+PT[3]+"<GEN pT<"+PT[4]+" LeastSquares", 100, 0, 5);
            TH1F *h2017_pT_bin_4_Huber = new TH1F("2017 mode "+MODE[i]+" "+PT[3]+"<GEN pT<"+PT[4]+" Huber", "2017 pT Resolution mode "+MODE[i]+" "+PT[3]+"<GEN pT<"+PT[4]+" Huber", 100, 0, 5);
            TH1F *h2017_pT_bin_4_AbsDev = new TH1F("2017 mode "+MODE[i]+" "+PT[3]+"<GEN pT<"+PT[4]+" AbsDev", "2017 pT Resolution mode "+MODE[i]+" "+PT[3]+"<GEN pT<"+PT[4]+" AbsDev", 100, 0, 5);
       // }
        
        Long64_t numEvents = myTree->GetEntries();//read the number of entries in myTree
        cout<<">>>>>>>>>>>>>>>>>>>>>"<<endl;
        cout<<numEvents<<" events to process..."<<endl;
        
        for(Long64_t iEntry = 0; iEntry <numEvents; iEntry++){
            //load the i-th event
            myTree->GetEntry(iEntry);
            Float_t Ratio2016= EMTF_pt/(GEN_pt*scale_factor_2016);//divide the 2016 scale factor 1.4
            Float_t Ratio2017= 1/(BDTG_AWB_Sq*GEN_pt);
            Float_t Ratio2017Huber=-1;
            Float_t Ratio2017AbsDev=-1;
            if(MODE[i] != MODE[0]){
                Ratio2017Huber= 1/(BDTG_AWB_Huber*GEN_pt);//2017 huber loss function
                Ratio2017AbsDev= 1/(BDTG_AWB*GEN_pt);//2017 absolute deviation loss func
            }
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
        
        //Customize for mode 15
        if(MODE[i] == MODE[0]){
            //pt bin1 eta bin1
            TCanvas *A1=new TCanvas(MODE[i]+"A1",MODE[i]+"A1",700,500);
            //eta bin2
            TCanvas *A2=new TCanvas(MODE[i]+"A2",MODE[i]+"A2",700,500);
            TCanvas *A3=new TCanvas(MODE[i]+"A3",MODE[i]+"A3",700,500);
            TCanvas *A4=new TCanvas(MODE[i]+"A4",MODE[i]+"A4",700,500);
            //pt bin2
            TCanvas *B1=new TCanvas(MODE[i]+"B1",MODE[i]+"B1",700,500);
            TCanvas *B2=new TCanvas(MODE[i]+"B2",MODE[i]+"B2",700,500);
            TCanvas *B3=new TCanvas(MODE[i]+"B3",MODE[i]+"B3",700,500);
            TCanvas *B4=new TCanvas(MODE[i]+"B4",MODE[i]+"B4",700,500);
            //pt bin3
            TCanvas *C1=new TCanvas(MODE[i]+"C1",MODE[i]+"C1",700,500);
            TCanvas *C2=new TCanvas(MODE[i]+"C2",MODE[i]+"C2",700,500);
            TCanvas *C3=new TCanvas(MODE[i]+"C3",MODE[i]+"C3",700,500);
            TCanvas *C4=new TCanvas(MODE[i]+"C4",MODE[i]+"C4",700,500);
            //pt bin4
            TCanvas *D1=new TCanvas(MODE[i]+"D1",MODE[i]+"D1",700,500);
            TCanvas *D2=new TCanvas(MODE[i]+"D2",MODE[i]+"D2",700,500);
            TCanvas *D3=new TCanvas(MODE[i]+"D3",MODE[i]+"D3",700,500);
            TCanvas *D4=new TCanvas(MODE[i]+"D4",MODE[i]+"D4",700,500);
            //different RPCs in mode 15 
            TCanvas *E1=new TCanvas(MODE[i]+"E1",MODE[i]+"E1",700,500);
            TCanvas *E2=new TCanvas(MODE[i]+"E2",MODE[i]+"E2",700,500);
            TCanvas *E3=new TCanvas(MODE[i]+"E3",MODE[i]+"E3",700,500);
            TCanvas *E4=new TCanvas(MODE[i]+"E4",MODE[i]+"E4",700,500);
           
            THStack *hsA1 = new THStack(MODE[i]+"hsA1","Mode "+MODE[i]+" pT Resolution "+PT[0]+"<GEN pT<"+PT[1]+" GeV "+ETA[0]+"<GEN eta<"+ETA[1]);
            THStack *hsA2 = new THStack(MODE[i]+"hsA2","Mode "+MODE[i]+" pT Resolution "+PT[0]+"<GEN pT<"+PT[1]+" GeV "+ETA[1]+"<GEN eta<"+ETA[2]);
            THStack *hsA3 = new THStack(MODE[i]+"hsA3","Mode "+MODE[i]+" pT Resolution "+PT[0]+"<GEN pT<"+PT[1]+" GeV "+ETA[2]+"<GEN eta<"+ETA[3]);
            THStack *hsA4 = new THStack(MODE[i]+"hsA4","Mode "+MODE[i]+" pT Resolution "+PT[0]+"<GEN pT<"+PT[1]+" GeV "+ETA[3]+"<GEN eta<"+ETA[4]);
        
            THStack *hsB1 = new THStack(MODE[i]+"hsB1","Mode "+MODE[i]+" pT Resolution "+PT[1]+"<GEN pT<"+PT[2]+" GeV "+ETA[0]+"<GEN eta<"+ETA[1]);
            THStack *hsB2 = new THStack(MODE[i]+"hsB2","Mode "+MODE[i]+" pT Resolution "+PT[1]+"<GEN pT<"+PT[2]+" GeV "+ETA[1]+"<GEN eta<"+ETA[2]);
            THStack *hsB3 = new THStack(MODE[i]+"hsB3","Mode "+MODE[i]+" pT Resolution "+PT[1]+"<GEN pT<"+PT[2]+" GeV "+ETA[2]+"<GEN eta<"+ETA[3]);
            THStack *hsB4 = new THStack(MODE[i]+"hsB4","Mode "+MODE[i]+" pT Resolution "+PT[1]+"<GEN pT<"+PT[2]+" GeV "+ETA[3]+"<GEN eta<"+ETA[4]);
            
            THStack *hsC1 = new THStack(MODE[i]+"hsC1","Mode "+MODE[i]+" pT Resolution "+PT[2]+"<GEN pT<"+PT[3]+" GeV "+ETA[0]+"<GEN eta<"+ETA[1]);
            THStack *hsC2 = new THStack(MODE[i]+"hsC2","Mode "+MODE[i]+" pT Resolution "+PT[2]+"<GEN pT<"+PT[3]+" GeV "+ETA[1]+"<GEN eta<"+ETA[2]);
            THStack *hsC3 = new THStack(MODE[i]+"hsC3","Mode "+MODE[i]+" pT Resolution "+PT[2]+"<GEN pT<"+PT[3]+" GeV "+ETA[2]+"<GEN eta<"+ETA[3]);
            THStack *hsC4 = new THStack(MODE[i]+"hsC4","Mode "+MODE[i]+" pT Resolution "+PT[2]+"<GEN pT<"+PT[3]+" GeV "+ETA[3]+"<GEN eta<"+ETA[4]);
            
            THStack *hsD1 = new THStack(MODE[i]+"hsD1","Mode "+MODE[i]+" pT Resolution "+PT[3]+"<GEN pT<"+PT[4]+" GeV "+ETA[0]+"<GEN eta<"+ETA[1]);
            THStack *hsD2 = new THStack(MODE[i]+"hsD2","Mode "+MODE[i]+" pT Resolution "+PT[3]+"<GEN pT<"+PT[4]+" GeV "+ETA[1]+"<GEN eta<"+ETA[2]);
            THStack *hsD3 = new THStack(MODE[i]+"hsD3","Mode "+MODE[i]+" pT Resolution "+PT[3]+"<GEN pT<"+PT[4]+" GeV "+ETA[2]+"<GEN eta<"+ETA[3]);
            THStack *hsD4 = new THStack(MODE[i]+"hsD4","Mode "+MODE[i]+" pT Resolution "+PT[3]+"<GEN pT<"+PT[4]+" GeV "+ETA[3]+"<GEN eta<"+ETA[4]);
            //different RPCs in mode 15 
            THStack *hsE1 = new THStack(MODE[i]+"hsE1","Mode "+MODE[i]+" pT Resolution "+PT[0]+"<GEN pT<"+PT[1]+" GeV "+ETA[0]+"<GEN eta<"+ETA[2]);
            THStack *hsE2 = new THStack(MODE[i]+"hsE2","Mode "+MODE[i]+" pT Resolution "+PT[1]+"<GEN pT<"+PT[2]+" GeV "+ETA[0]+"<GEN eta<"+ETA[2]);
            THStack *hsE3 = new THStack(MODE[i]+"hsE3","Mode "+MODE[i]+" pT Resolution "+PT[2]+"<GEN pT<"+PT[3]+" GeV "+ETA[0]+"<GEN eta<"+ETA[2]);
            THStack *hsE4 = new THStack(MODE[i]+"hsE4","Mode "+MODE[i]+" pT Resolution "+PT[3]+"<GEN pT<"+PT[4]+" GeV "+ETA[0]+"<GEN eta<"+ETA[2]);
            //pT bin1
            A1->cd();
            h2016_pT_bin_1_eta_bin_1->SetLineColor(kRed);
            h2016_pT_bin_1_eta_bin_1->SetLineWidth(2);
            gStyle->SetOptStat(0);
            h2017_pT_bin_1_eta_bin_1->SetLineColor(kBlue);
            h2017_pT_bin_1_eta_bin_1->SetLineWidth(2);
            gStyle->SetOptStat(0);
            hsA1->Add(h2016_pT_bin_1_eta_bin_1);
            hsA1->Add(h2017_pT_bin_1_eta_bin_1);
            hsA1->Draw("nostack");
            hsA1->GetXaxis()->SetTitle("Trigger pT/GEN pT");
            A1->Modified();
            TLegend* LA1 = new TLegend(0.4,0.4,0.9,0.9);
            TString S2016_bin_A1="";
            S2016_bin_A1= S2016_bin_A1 + "2016: "+"Mean "+Form("%f", h2016_pT_bin_1_eta_bin_1->GetMean())+" RMS "+Form("%f", h2016_pT_bin_1_eta_bin_1->GetRMS());
            TString S2017_bin_A1="";
            S2017_bin_A1= S2017_bin_A1 + "2017: "+"Mean "+Form("%f", h2017_pT_bin_1_eta_bin_1->GetMean())+" RMS "+Form("%f", h2017_pT_bin_1_eta_bin_1->GetRMS());
            LA1->AddEntry(h2016_pT_bin_1_eta_bin_1, S2016_bin_A1);
            LA1->AddEntry(h2017_pT_bin_1_eta_bin_1, S2017_bin_A1);
            LA1->Draw();
            A1->Write();
            
            A2->cd();
            h2016_pT_bin_1_eta_bin_2->SetLineColor(kRed);
            h2016_pT_bin_1_eta_bin_2->SetLineWidth(2);
            gStyle->SetOptStat(0);
            h2017_pT_bin_1_eta_bin_2->SetLineColor(kBlue);
            h2017_pT_bin_1_eta_bin_2->SetLineWidth(2);            
            gStyle->SetOptStat(0);
            hsA2->Add(h2016_pT_bin_1_eta_bin_2);
            hsA2->Add(h2017_pT_bin_1_eta_bin_2);
            hsA2->Draw("nostack");
            hsA2->GetXaxis()->SetTitle("Trigger pT/GEN pT");
            A2->Modified();
            TLegend* LA2 = new TLegend(0.4,0.4,0.9,0.9);
            TString S2016_bin_A2="";
            S2016_bin_A2= S2016_bin_A2 + "2016: "+"Mean "+Form("%f", h2016_pT_bin_1_eta_bin_2->GetMean())+" RMS "+Form("%f", h2016_pT_bin_1_eta_bin_2->GetRMS());
            TString S2017_bin_A2="";
            S2017_bin_A2= S2017_bin_A2 + "2017: "+"Mean "+Form("%f", h2017_pT_bin_1_eta_bin_2->GetMean())+" RMS "+Form("%f", h2017_pT_bin_1_eta_bin_2->GetRMS());
            LA2->AddEntry(h2016_pT_bin_1_eta_bin_2, S2016_bin_A2);
            LA2->AddEntry(h2017_pT_bin_1_eta_bin_2, S2017_bin_A2);
            LA2->Draw();
            A2->Write();
            
            A3->cd();
            h2016_pT_bin_1_eta_bin_3->SetLineColor(kRed);
            h2016_pT_bin_1_eta_bin_3->SetLineWidth(2);
            gStyle->SetOptStat(0);
            h2017_pT_bin_1_eta_bin_3->SetLineColor(kBlue);
            h2017_pT_bin_1_eta_bin_3->SetLineWidth(2);            
            gStyle->SetOptStat(0);
            hsA3->Add(h2016_pT_bin_1_eta_bin_3);
            hsA3->Add(h2017_pT_bin_1_eta_bin_3);
            hsA3->Draw("nostack");
            hsA3->GetXaxis()->SetTitle("Trigger pT/GEN pT");
            A3->Modified();
            TLegend* LA3 = new TLegend(0.4,0.4,0.9,0.9);
            TString S2016_bin_A3="";
            S2016_bin_A3= S2016_bin_A3 + "2016: "+"Mean "+Form("%f", h2016_pT_bin_1_eta_bin_3->GetMean())+" RMS "+Form("%f", h2016_pT_bin_1_eta_bin_3->GetRMS());
            TString S2017_bin_A3="";
            S2017_bin_A3= S2017_bin_A3 + "2017: "+"Mean "+Form("%f", h2017_pT_bin_1_eta_bin_3->GetMean())+" RMS "+Form("%f", h2017_pT_bin_1_eta_bin_3->GetRMS());
            LA3->AddEntry(h2016_pT_bin_1_eta_bin_3, S2016_bin_A3);
            LA3->AddEntry(h2017_pT_bin_1_eta_bin_3, S2017_bin_A3);
            LA3->Draw();
            A3->Write();
            
            A4->cd();
            h2016_pT_bin_1_eta_bin_4->SetLineColor(kRed);
            h2016_pT_bin_1_eta_bin_4->SetLineWidth(2);
            gStyle->SetOptStat(0);
            h2017_pT_bin_1_eta_bin_4->SetLineColor(kBlue);
            h2017_pT_bin_1_eta_bin_4->SetLineWidth(2);            
            gStyle->SetOptStat(0);
            hsA4->Add(h2016_pT_bin_1_eta_bin_4);
            hsA4->Add(h2017_pT_bin_1_eta_bin_4);
            hsA4->Draw("nostack");
            hsA4->GetXaxis()->SetTitle("Trigger pT/GEN pT");
            A4->Modified();
            TLegend* LA4 = new TLegend(0.4,0.4,0.9,0.9);
            TString S2016_bin_A4="";
            S2016_bin_A4= S2016_bin_A4 + "2016: "+"Mean "+Form("%f", h2016_pT_bin_1_eta_bin_4->GetMean())+" RMS "+Form("%f", h2016_pT_bin_1_eta_bin_4->GetRMS());
            TString S2017_bin_A4="";
            S2017_bin_A4= S2017_bin_A4 + "2017: "+"Mean "+Form("%f", h2017_pT_bin_1_eta_bin_4->GetMean())+" RMS "+Form("%f", h2017_pT_bin_1_eta_bin_4->GetRMS());
            LA4->AddEntry(h2016_pT_bin_1_eta_bin_4, S2016_bin_A4);
            LA4->AddEntry(h2017_pT_bin_1_eta_bin_4, S2017_bin_A4);
            LA4->Draw();
            A4->Write();
            
            //pT bin2
            B1->cd();
            h2016_pT_bin_2_eta_bin_1->SetLineColor(kRed);
            h2016_pT_bin_2_eta_bin_1->SetLineWidth(2);
            gStyle->SetOptStat(0);
            h2017_pT_bin_2_eta_bin_1->SetLineColor(kBlue);
            h2017_pT_bin_2_eta_bin_1->SetLineWidth(2);
            gStyle->SetOptStat(0);
            hsB1->Add(h2016_pT_bin_2_eta_bin_1);
            hsB1->Add(h2017_pT_bin_2_eta_bin_1);
            hsB1->Draw("nostack");
            hsB1->GetXaxis()->SetTitle("Trigger pT/GEN pT");
            B1->Modified();
            TLegend* LB1 = new TLegend(0.4,0.4,0.9,0.9);
            TString S2016_bin_B1="";
            S2016_bin_B1= S2016_bin_B1 + "2016: "+"Mean "+Form("%f", h2016_pT_bin_2_eta_bin_1->GetMean())+" RMS "+Form("%f", h2016_pT_bin_2_eta_bin_1->GetRMS());
            TString S2017_bin_B1="";
            S2017_bin_B1= S2017_bin_B1 + "2017: "+"Mean "+Form("%f", h2017_pT_bin_2_eta_bin_1->GetMean())+" RMS "+Form("%f", h2017_pT_bin_2_eta_bin_1->GetRMS());
            LB1->AddEntry(h2016_pT_bin_2_eta_bin_1, S2016_bin_B1);
            LB1->AddEntry(h2017_pT_bin_2_eta_bin_1, S2017_bin_B1);
            LB1->Draw();
            B1->Write();
            
            B2->cd();
            h2016_pT_bin_2_eta_bin_2->SetLineColor(kRed);
            h2016_pT_bin_2_eta_bin_2->SetLineWidth(2);
            gStyle->SetOptStat(0);
            h2017_pT_bin_2_eta_bin_2->SetLineColor(kBlue);
            h2017_pT_bin_2_eta_bin_2->SetLineWidth(2);            
            gStyle->SetOptStat(0);
            hsB2->Add(h2016_pT_bin_2_eta_bin_2);
            hsB2->Add(h2017_pT_bin_2_eta_bin_2);
            hsB2->Draw("nostack");
            hsB2->GetXaxis()->SetTitle("Trigger pT/GEN pT");
            B2->Modified();
            TLegend* LB2 = new TLegend(0.4,0.4,0.9,0.9);
            TString S2016_bin_B2="";
            S2016_bin_B2= S2016_bin_B2 + "2016: "+"Mean "+Form("%f", h2016_pT_bin_2_eta_bin_2->GetMean())+" RMS "+Form("%f", h2016_pT_bin_2_eta_bin_2->GetRMS());
            TString S2017_bin_B2="";
            S2017_bin_B2= S2017_bin_B2 + "2017: "+"Mean "+Form("%f", h2017_pT_bin_2_eta_bin_2->GetMean())+" RMS "+Form("%f", h2017_pT_bin_2_eta_bin_2->GetRMS());
            LB2->AddEntry(h2016_pT_bin_2_eta_bin_2, S2016_bin_B2);
            LB2->AddEntry(h2017_pT_bin_2_eta_bin_2, S2017_bin_B2);
            LB2->Draw();
            B2->Write();
            
            B3->cd();
            h2016_pT_bin_2_eta_bin_3->SetLineColor(kRed);
            h2016_pT_bin_2_eta_bin_3->SetLineWidth(2);
            gStyle->SetOptStat(0);
            h2017_pT_bin_2_eta_bin_3->SetLineColor(kBlue);
            h2017_pT_bin_2_eta_bin_3->SetLineWidth(2);            
            gStyle->SetOptStat(0);
            hsB3->Add(h2016_pT_bin_2_eta_bin_3);
            hsB3->Add(h2017_pT_bin_2_eta_bin_3);
            hsB3->Draw("nostack");
            hsB3->GetXaxis()->SetTitle("Trigger pT/GEN pT");
            B3->Modified();
            TLegend* LB3 = new TLegend(0.4,0.4,0.9,0.9);
            TString S2016_bin_B3="";
            S2016_bin_B3= S2016_bin_B3 + "2016: "+"Mean "+Form("%f", h2016_pT_bin_2_eta_bin_3->GetMean())+" RMS "+Form("%f", h2016_pT_bin_2_eta_bin_3->GetRMS());
            TString S2017_bin_B3="";
            S2017_bin_B3= S2017_bin_B3 + "2017: "+"Mean "+Form("%f", h2017_pT_bin_2_eta_bin_3->GetMean())+" RMS "+Form("%f", h2017_pT_bin_2_eta_bin_3->GetRMS());
            LB3->AddEntry(h2016_pT_bin_2_eta_bin_3, S2016_bin_B3);
            LB3->AddEntry(h2017_pT_bin_2_eta_bin_3, S2017_bin_B3);
            LB3->Draw();
            B3->Write();
            
            B4->cd();
            h2016_pT_bin_2_eta_bin_4->SetLineColor(kRed);
            h2016_pT_bin_2_eta_bin_4->SetLineWidth(2);
            gStyle->SetOptStat(0);
            h2017_pT_bin_2_eta_bin_4->SetLineColor(kBlue);
            h2017_pT_bin_2_eta_bin_4->SetLineWidth(2);            
            gStyle->SetOptStat(0);
            hsB4->Add(h2016_pT_bin_2_eta_bin_4);
            hsB4->Add(h2017_pT_bin_2_eta_bin_4);
            hsB4->Draw("nostack");
            hsB4->GetXaxis()->SetTitle("Trigger pT/GEN pT");
            B4->Modified();
            TLegend* LB4 = new TLegend(0.4,0.4,0.9,0.9);
            TString S2016_bin_B4="";
            S2016_bin_B4= S2016_bin_B4 + "2016: "+"Mean "+Form("%f", h2016_pT_bin_2_eta_bin_4->GetMean())+" RMS "+Form("%f", h2016_pT_bin_2_eta_bin_4->GetRMS());
            TString S2017_bin_B4="";
            S2017_bin_B4= S2017_bin_B4 + "2017: "+"Mean "+Form("%f", h2017_pT_bin_2_eta_bin_4->GetMean())+" RMS "+Form("%f", h2017_pT_bin_2_eta_bin_4->GetRMS());
            LB4->AddEntry(h2016_pT_bin_2_eta_bin_4, S2016_bin_B4);
            LB4->AddEntry(h2017_pT_bin_2_eta_bin_4, S2017_bin_B4);
            LB4->Draw();
            B4->Write();
            
            //pT bin3
            C1->cd();
            h2016_pT_bin_3_eta_bin_1->SetLineColor(kRed);
            h2016_pT_bin_3_eta_bin_1->SetLineWidth(2);
            gStyle->SetOptStat(0);
            h2017_pT_bin_3_eta_bin_1->SetLineColor(kBlue);
            h2017_pT_bin_3_eta_bin_1->SetLineWidth(2);
            gStyle->SetOptStat(0);
            hsC1->Add(h2016_pT_bin_3_eta_bin_1);
            hsC1->Add(h2017_pT_bin_3_eta_bin_1);
            hsC1->Draw("nostack");
            hsC1->GetXaxis()->SetTitle("Trigger pT/GEN pT");
            C1->Modified();
            TLegend* LC1 = new TLegend(0.4,0.4,0.9,0.9);
            TString S2016_bin_C1="";
            S2016_bin_C1= S2016_bin_C1 + "2016: "+"Mean "+Form("%f", h2016_pT_bin_3_eta_bin_1->GetMean())+" RMS "+Form("%f", h2016_pT_bin_3_eta_bin_1->GetRMS());
            TString S2017_bin_C1="";
            S2017_bin_C1= S2017_bin_C1 + "2017: "+"Mean "+Form("%f", h2017_pT_bin_3_eta_bin_1->GetMean())+" RMS "+Form("%f", h2017_pT_bin_3_eta_bin_1->GetRMS());
            LC1->AddEntry(h2016_pT_bin_3_eta_bin_1, S2016_bin_C1);
            LC1->AddEntry(h2017_pT_bin_3_eta_bin_1, S2017_bin_C1);
            LC1->Draw();
            C1->Write();
            
            C2->cd();
            h2016_pT_bin_3_eta_bin_2->SetLineColor(kRed);
            h2016_pT_bin_3_eta_bin_2->SetLineWidth(2);
            gStyle->SetOptStat(0);
            h2017_pT_bin_3_eta_bin_2->SetLineColor(kBlue);
            h2017_pT_bin_3_eta_bin_2->SetLineWidth(2);            
            gStyle->SetOptStat(0);
            hsC2->Add(h2016_pT_bin_3_eta_bin_2);
            hsC2->Add(h2017_pT_bin_3_eta_bin_2);
            hsC2->Draw("nostack");
            hsC2->GetXaxis()->SetTitle("Trigger pT/GEN pT");
            C2->Modified();
            TLegend* LC2 = new TLegend(0.4,0.4,0.9,0.9);
            TString S2016_bin_C2="";
            S2016_bin_C2= S2016_bin_C2 + "2016: "+"Mean "+Form("%f", h2016_pT_bin_3_eta_bin_2->GetMean())+" RMS "+Form("%f", h2016_pT_bin_3_eta_bin_2->GetRMS());
            TString S2017_bin_C2="";
            S2017_bin_C2= S2017_bin_C2 + "2017: "+"Mean "+Form("%f", h2017_pT_bin_3_eta_bin_2->GetMean())+" RMS "+Form("%f", h2017_pT_bin_3_eta_bin_2->GetRMS());
            LC2->AddEntry(h2016_pT_bin_3_eta_bin_2, S2016_bin_C2);
            LC2->AddEntry(h2017_pT_bin_3_eta_bin_2, S2017_bin_C2);
            LC2->Draw();
            C2->Write();
            
            C3->cd();
            h2016_pT_bin_3_eta_bin_3->SetLineColor(kRed);
            h2016_pT_bin_3_eta_bin_3->SetLineWidth(2);
            gStyle->SetOptStat(0);
            h2017_pT_bin_3_eta_bin_3->SetLineColor(kBlue);
            h2017_pT_bin_3_eta_bin_3->SetLineWidth(2);            
            gStyle->SetOptStat(0);
            hsC3->Add(h2016_pT_bin_3_eta_bin_3);
            hsC3->Add(h2017_pT_bin_3_eta_bin_3);
            hsC3->Draw("nostack");
            hsC3->GetXaxis()->SetTitle("Trigger pT/GEN pT");
            C3->Modified();
            TLegend* LC3 = new TLegend(0.4,0.4,0.9,0.9);
            TString S2016_bin_C3="";
            S2016_bin_C3= S2016_bin_C3 + "2016: "+"Mean "+Form("%f", h2016_pT_bin_3_eta_bin_3->GetMean())+" RMS "+Form("%f", h2016_pT_bin_3_eta_bin_3->GetRMS());
            TString S2017_bin_C3="";
            S2017_bin_C3= S2017_bin_C3 + "2017: "+"Mean "+Form("%f", h2017_pT_bin_3_eta_bin_3->GetMean())+" RMS "+Form("%f", h2017_pT_bin_3_eta_bin_3->GetRMS());
            LC3->AddEntry(h2016_pT_bin_3_eta_bin_3, S2016_bin_C3);
            LC3->AddEntry(h2017_pT_bin_3_eta_bin_3, S2017_bin_C3);
            LC3->Draw();
            C3->Write();
            
            C4->cd();
            h2016_pT_bin_3_eta_bin_4->SetLineColor(kRed);
            h2016_pT_bin_3_eta_bin_4->SetLineWidth(2);
            gStyle->SetOptStat(0);
            h2017_pT_bin_3_eta_bin_4->SetLineColor(kBlue);
            h2017_pT_bin_3_eta_bin_4->SetLineWidth(2);            
            gStyle->SetOptStat(0);
            hsC4->Add(h2016_pT_bin_3_eta_bin_4);
            hsC4->Add(h2017_pT_bin_3_eta_bin_4);
            hsC4->Draw("nostack");
            hsC4->GetXaxis()->SetTitle("Trigger pT/GEN pT");
            C4->Modified();
            TLegend* LC4 = new TLegend(0.4,0.4,0.9,0.9);
            TString S2016_bin_C4="";
            S2016_bin_C4= S2016_bin_C4 + "2016: "+"Mean "+Form("%f", h2016_pT_bin_3_eta_bin_4->GetMean())+" RMS "+Form("%f", h2016_pT_bin_3_eta_bin_4->GetRMS());
            TString S2017_bin_C4="";
            S2017_bin_C4= S2017_bin_C4 + "2017: "+"Mean "+Form("%f", h2017_pT_bin_3_eta_bin_4->GetMean())+" RMS "+Form("%f", h2017_pT_bin_3_eta_bin_4->GetRMS());
            LC4->AddEntry(h2016_pT_bin_3_eta_bin_4, S2016_bin_C4);
            LC4->AddEntry(h2017_pT_bin_3_eta_bin_4, S2017_bin_C4);
            LC4->Draw();
            C4->Write();
            
            //pT bin4
            D1->cd();
            h2016_pT_bin_4_eta_bin_1->SetLineColor(kRed);
            h2016_pT_bin_4_eta_bin_1->SetLineWidth(2);
            gStyle->SetOptStat(0);
            h2017_pT_bin_4_eta_bin_1->SetLineColor(kBlue);
            h2017_pT_bin_4_eta_bin_1->SetLineWidth(2);
            gStyle->SetOptStat(0);
            hsD1->Add(h2016_pT_bin_4_eta_bin_1);
            hsD1->Add(h2017_pT_bin_4_eta_bin_1);
            hsD1->Draw("nostack");
            hsD1->GetXaxis()->SetTitle("Trigger pT/GEN pT");
            D1->Modified();
            TLegend* LD1 = new TLegend(0.4,0.4,0.9,0.9);
            TString S2016_bin_D1="";
            S2016_bin_D1= S2016_bin_D1 + "2016: "+"Mean "+Form("%f", h2016_pT_bin_4_eta_bin_1->GetMean())+" RMS "+Form("%f", h2016_pT_bin_4_eta_bin_1->GetRMS());
            TString S2017_bin_D1="";
            S2017_bin_D1= S2017_bin_D1 + "2017: "+"Mean "+Form("%f", h2017_pT_bin_4_eta_bin_1->GetMean())+" RMS "+Form("%f", h2017_pT_bin_4_eta_bin_1->GetRMS());
            LD1->AddEntry(h2016_pT_bin_4_eta_bin_1, S2016_bin_D1);
            LD1->AddEntry(h2017_pT_bin_4_eta_bin_1, S2017_bin_D1);
            LD1->Draw();
            D1->Write();
            
            D2->cd();
            h2016_pT_bin_4_eta_bin_2->SetLineColor(kRed);
            h2016_pT_bin_4_eta_bin_2->SetLineWidth(2);
            gStyle->SetOptStat(0);
            h2017_pT_bin_4_eta_bin_2->SetLineColor(kBlue);
            h2017_pT_bin_4_eta_bin_2->SetLineWidth(2);            
            gStyle->SetOptStat(0);
            hsD2->Add(h2016_pT_bin_4_eta_bin_2);
            hsD2->Add(h2017_pT_bin_4_eta_bin_2);
            hsD2->Draw("nostack");
            hsD2->GetXaxis()->SetTitle("Trigger pT/GEN pT");
            D2->Modified();
            TLegend* LD2 = new TLegend(0.4,0.4,0.9,0.9);
            TString S2016_bin_D2="";
            S2016_bin_D2= S2016_bin_D2 + "2016: "+"Mean "+Form("%f", h2016_pT_bin_4_eta_bin_2->GetMean())+" RMS "+Form("%f", h2016_pT_bin_4_eta_bin_2->GetRMS());
            TString S2017_bin_D2="";
            S2017_bin_D2= S2017_bin_D2 + "2017: "+"Mean "+Form("%f", h2017_pT_bin_4_eta_bin_2->GetMean())+" RMS "+Form("%f", h2017_pT_bin_4_eta_bin_2->GetRMS());
            LD2->AddEntry(h2016_pT_bin_4_eta_bin_2, S2016_bin_D2);
            LD2->AddEntry(h2017_pT_bin_4_eta_bin_2, S2017_bin_D2);
            LD2->Draw();
            D2->Write();
            
            D3->cd();
            h2016_pT_bin_4_eta_bin_3->SetLineColor(kRed);
            h2016_pT_bin_4_eta_bin_3->SetLineWidth(2);
            gStyle->SetOptStat(0);
            h2017_pT_bin_4_eta_bin_3->SetLineColor(kBlue);
            h2017_pT_bin_4_eta_bin_3->SetLineWidth(2);            
            gStyle->SetOptStat(0);
            hsD3->Add(h2016_pT_bin_3_eta_bin_3);
            hsD3->Add(h2017_pT_bin_3_eta_bin_3);
            hsD3->Draw("nostack");
            hsD3->GetXaxis()->SetTitle("Trigger pT/GEN pT");
            D3->Modified();
            TLegend* LD3 = new TLegend(0.4,0.4,0.9,0.9);
            TString S2016_bin_D3="";
            S2016_bin_D3= S2016_bin_D3 + "2016: "+"Mean "+Form("%f", h2016_pT_bin_4_eta_bin_3->GetMean())+" RMS "+Form("%f", h2016_pT_bin_4_eta_bin_3->GetRMS());
            TString S2017_bin_D3="";
            S2017_bin_D3= S2017_bin_D3 + "2017: "+"Mean "+Form("%f", h2017_pT_bin_4_eta_bin_3->GetMean())+" RMS "+Form("%f", h2017_pT_bin_4_eta_bin_3->GetRMS());
            LD3->AddEntry(h2016_pT_bin_4_eta_bin_3, S2016_bin_D3);
            LD3->AddEntry(h2017_pT_bin_4_eta_bin_3, S2017_bin_D3);
            LD3->Draw();
            D3->Write();
            
            D4->cd();
            h2016_pT_bin_4_eta_bin_4->SetLineColor(kRed);
            h2016_pT_bin_4_eta_bin_4->SetLineWidth(2);
            gStyle->SetOptStat(0);
            h2017_pT_bin_4_eta_bin_4->SetLineColor(kBlue);
            h2017_pT_bin_4_eta_bin_4->SetLineWidth(2);            
            gStyle->SetOptStat(0);
            hsD4->Add(h2016_pT_bin_4_eta_bin_4);
            hsD4->Add(h2017_pT_bin_4_eta_bin_4);
            hsD4->Draw("nostack");
            hsD4->GetXaxis()->SetTitle("Trigger pT/GEN pT");
            D4->Modified();
            TLegend* LD4 = new TLegend(0.4,0.4,0.9,0.9);
            TString S2016_bin_D4="";
            S2016_bin_D4= S2016_bin_D4 + "2016: "+"Mean "+Form("%f", h2016_pT_bin_4_eta_bin_4->GetMean())+" RMS "+Form("%f", h2016_pT_bin_4_eta_bin_4->GetRMS());
            TString S2017_bin_D4="";
            S2017_bin_D4= S2017_bin_D4 + "2017: "+"Mean "+Form("%f", h2017_pT_bin_4_eta_bin_4->GetMean())+" RMS "+Form("%f", h2017_pT_bin_4_eta_bin_4->GetRMS());
            LD4->AddEntry(h2016_pT_bin_4_eta_bin_4, S2016_bin_D4);
            LD4->AddEntry(h2017_pT_bin_4_eta_bin_4, S2017_bin_D4);
            LD4->Draw();
            D4->Write();
            
            //different RPCs, pT bin1
            E1->cd();
            h2017_pT_bin_1_RPC_0->SetLineColor(kBlue);
            h2017_pT_bin_1_RPC_0->SetLineWidth(2);
            gStyle->SetOptStat(0);
            h2017_pT_bin_1_RPC_1->SetLineColor(kRed);
            h2017_pT_bin_1_RPC_1->SetLineWidth(2);
            gStyle->SetOptStat(0);
            h2017_pT_bin_1_RPC_2->SetLineColor(kGreen);
            h2017_pT_bin_1_RPC_2->SetLineWidth(2);
            gStyle->SetOptStat(0);
            h2017_pT_bin_1_RPC_3->SetLineColor(kYellow);
            h2017_pT_bin_1_RPC_3->SetLineWidth(2);
            gStyle->SetOptStat(0);
            h2017_pT_bin_1_RPC_4->SetLineColor(kPink);
            h2017_pT_bin_1_RPC_4->SetLineWidth(2);
            gStyle->SetOptStat(0);
            hsE1->Add(h2017_pT_bin_1_RPC_0);
            hsE1->Add(h2017_pT_bin_1_RPC_1);
            hsE1->Add(h2017_pT_bin_1_RPC_2);
            hsE1->Add(h2017_pT_bin_1_RPC_3);
            hsE1->Add(h2017_pT_bin_1_RPC_4);
            hsE1->Draw("nostack");
            hsE1->GetXaxis()->SetTitle("Trigger pT/GEN pT");
            E1->Modified();
            TLegend* LE1 = new TLegend(0.4,0.4,0.9,0.9);
            TString S2017_bin_E1_0="";
            S2017_bin_E1_0= S2017_bin_E1_0 + "Track_Mode_RPC="+TRACK_MODE_RPC[0]+": "+"Mean "+Form("%f", h2017_pT_bin_1_RPC_0->GetMean())+" RMS "+Form("%f", h2017_pT_bin_1_RPC_0->GetRMS());
            TString S2017_bin_E1_1="";
            S2017_bin_E1_1= S2017_bin_E1_1 + "Track_Mode_RPC="+TRACK_MODE_RPC[1]+": "+"Mean "+Form("%f", h2017_pT_bin_1_RPC_1->GetMean())+" RMS "+Form("%f", h2017_pT_bin_1_RPC_1->GetRMS());
            TString S2017_bin_E1_2="";
            S2017_bin_E1_2= S2017_bin_E1_2 + "Track_Mode_RPC="+TRACK_MODE_RPC[2]+": "+"Mean "+Form("%f", h2017_pT_bin_1_RPC_2->GetMean())+" RMS "+Form("%f", h2017_pT_bin_1_RPC_2->GetRMS());
            TString S2017_bin_E1_3="";
            S2017_bin_E1_3= S2017_bin_E1_3 + "Track_Mode_RPC="+TRACK_MODE_RPC[3]+": "+"Mean "+Form("%f", h2017_pT_bin_1_RPC_3->GetMean())+" RMS "+Form("%f", h2017_pT_bin_1_RPC_3->GetRMS());
            TString S2017_bin_E1_4="";
            S2017_bin_E1_4= S2017_bin_E1_4 + "Track_Mode_RPC="+TRACK_MODE_RPC[4]+": "+"Mean "+Form("%f", h2017_pT_bin_1_RPC_4->GetMean())+" RMS "+Form("%f", h2017_pT_bin_1_RPC_4->GetRMS());
            LE1->AddEntry(h2017_pT_bin_1_RPC_0, S2017_bin_E1_0);
            LE1->AddEntry(h2017_pT_bin_1_RPC_1, S2017_bin_E1_1);
            LE1->AddEntry(h2017_pT_bin_1_RPC_2, S2017_bin_E1_2);
            LE1->AddEntry(h2017_pT_bin_1_RPC_3, S2017_bin_E1_3);
            LE1->AddEntry(h2017_pT_bin_1_RPC_4, S2017_bin_E1_4);
            LE1->Draw();
            E1->Write();
            
            E2->cd();
            h2017_pT_bin_2_RPC_0->SetLineColor(kBlue);
            h2017_pT_bin_2_RPC_0->SetLineWidth(2);
            gStyle->SetOptStat(0);
            h2017_pT_bin_2_RPC_1->SetLineColor(kRed);
            h2017_pT_bin_2_RPC_1->SetLineWidth(2);
            gStyle->SetOptStat(0);
            h2017_pT_bin_2_RPC_2->SetLineColor(kGreen);
            h2017_pT_bin_2_RPC_2->SetLineWidth(2);
            gStyle->SetOptStat(0);
            h2017_pT_bin_2_RPC_3->SetLineColor(kYellow);
            h2017_pT_bin_2_RPC_3->SetLineWidth(2);
            gStyle->SetOptStat(0);
            h2017_pT_bin_2_RPC_4->SetLineColor(kPink);
            h2017_pT_bin_2_RPC_4->SetLineWidth(2);
            gStyle->SetOptStat(0);
            hsE2->Add(h2017_pT_bin_2_RPC_0);
            hsE2->Add(h2017_pT_bin_2_RPC_1);
            hsE2->Add(h2017_pT_bin_2_RPC_2);
            hsE2->Add(h2017_pT_bin_2_RPC_3);
            hsE2->Add(h2017_pT_bin_2_RPC_4);
            hsE2->Draw("nostack");
            hsE2->GetXaxis()->SetTitle("Trigger pT/GEN pT");
            E2->Modified();
            TLegend* LE2 = new TLegend(0.4,0.4,0.9,0.9);
            TString S2017_bin_E2_0="";
            S2017_bin_E2_0= S2017_bin_E2_0 + "Track_Mode_RPC="+TRACK_MODE_RPC[0]+": "+"Mean "+Form("%f", h2017_pT_bin_2_RPC_0->GetMean())+" RMS "+Form("%f", h2017_pT_bin_2_RPC_0->GetRMS());
            TString S2017_bin_E2_1="";
            S2017_bin_E2_1= S2017_bin_E2_1 + "Track_Mode_RPC="+TRACK_MODE_RPC[1]+": "+"Mean "+Form("%f", h2017_pT_bin_2_RPC_1->GetMean())+" RMS "+Form("%f", h2017_pT_bin_2_RPC_1->GetRMS());
            TString S2017_bin_E2_2="";
            S2017_bin_E2_2= S2017_bin_E2_2 + "Track_Mode_RPC="+TRACK_MODE_RPC[2]+": "+"Mean "+Form("%f", h2017_pT_bin_2_RPC_2->GetMean())+" RMS "+Form("%f", h2017_pT_bin_2_RPC_2->GetRMS());
            TString S2017_bin_E2_3="";
            S2017_bin_E2_3= S2017_bin_E2_3 + "Track_Mode_RPC="+TRACK_MODE_RPC[3]+": "+"Mean "+Form("%f", h2017_pT_bin_2_RPC_3->GetMean())+" RMS "+Form("%f", h2017_pT_bin_2_RPC_3->GetRMS());
            TString S2017_bin_E2_4="";
            S2017_bin_E2_4= S2017_bin_E2_4 + "Track_Mode_RPC="+TRACK_MODE_RPC[4]+": "+"Mean "+Form("%f", h2017_pT_bin_2_RPC_4->GetMean())+" RMS "+Form("%f", h2017_pT_bin_2_RPC_4->GetRMS());
            LE2->AddEntry(h2017_pT_bin_2_RPC_0, S2017_bin_E2_0);
            LE2->AddEntry(h2017_pT_bin_2_RPC_1, S2017_bin_E2_1);
            LE2->AddEntry(h2017_pT_bin_2_RPC_2, S2017_bin_E2_2);
            LE2->AddEntry(h2017_pT_bin_2_RPC_3, S2017_bin_E2_3);
            LE2->AddEntry(h2017_pT_bin_2_RPC_4, S2017_bin_E2_4);
            LE2->Draw();
            E2->Write();
            
            E3->cd();
            h2017_pT_bin_3_RPC_0->SetLineColor(kBlue);
            h2017_pT_bin_3_RPC_0->SetLineWidth(2);
            gStyle->SetOptStat(0);
            h2017_pT_bin_3_RPC_1->SetLineColor(kRed);
            h2017_pT_bin_3_RPC_1->SetLineWidth(2);
            gStyle->SetOptStat(0);
            h2017_pT_bin_3_RPC_2->SetLineColor(kGreen);
            h2017_pT_bin_3_RPC_2->SetLineWidth(2);
            gStyle->SetOptStat(0);
            h2017_pT_bin_3_RPC_3->SetLineColor(kYellow);
            h2017_pT_bin_3_RPC_3->SetLineWidth(2);
            gStyle->SetOptStat(0);
            h2017_pT_bin_3_RPC_4->SetLineColor(kPink);
            h2017_pT_bin_3_RPC_4->SetLineWidth(2);
            gStyle->SetOptStat(0);
            hsE3->Add(h2017_pT_bin_3_RPC_0);
            hsE3->Add(h2017_pT_bin_3_RPC_1);
            hsE3->Add(h2017_pT_bin_3_RPC_2);
            hsE3->Add(h2017_pT_bin_3_RPC_3);
            hsE3->Add(h2017_pT_bin_3_RPC_4);
            hsE3->Draw("nostack");
            hsE3->GetXaxis()->SetTitle("Trigger pT/GEN pT");
            E3->Modified();
            TLegend* LE3 = new TLegend(0.4,0.4,0.9,0.9);
            TString S2017_bin_E3_0="";
            S2017_bin_E3_0= S2017_bin_E3_0 + "Track_Mode_RPC="+TRACK_MODE_RPC[0]+": "+"Mean "+Form("%f", h2017_pT_bin_3_RPC_0->GetMean())+" RMS "+Form("%f", h2017_pT_bin_3_RPC_0->GetRMS());
            TString S2017_bin_E3_1="";
            S2017_bin_E3_1= S2017_bin_E3_1 + "Track_Mode_RPC="+TRACK_MODE_RPC[1]+": "+"Mean "+Form("%f", h2017_pT_bin_3_RPC_1->GetMean())+" RMS "+Form("%f", h2017_pT_bin_3_RPC_1->GetRMS());
            TString S2017_bin_E3_2="";
            S2017_bin_E3_2= S2017_bin_E3_2 + "Track_Mode_RPC="+TRACK_MODE_RPC[2]+": "+"Mean "+Form("%f", h2017_pT_bin_3_RPC_2->GetMean())+" RMS "+Form("%f", h2017_pT_bin_3_RPC_2->GetRMS());
            TString S2017_bin_E3_3="";
            S2017_bin_E3_3= S2017_bin_E3_3 + "Track_Mode_RPC="+TRACK_MODE_RPC[3]+": "+"Mean "+Form("%f", h2017_pT_bin_3_RPC_3->GetMean())+" RMS "+Form("%f", h2017_pT_bin_3_RPC_3->GetRMS());
            TString S2017_bin_E3_4="";
            S2017_bin_E3_4= S2017_bin_E3_4 + "Track_Mode_RPC="+TRACK_MODE_RPC[4]+": "+"Mean "+Form("%f", h2017_pT_bin_3_RPC_4->GetMean())+" RMS "+Form("%f", h2017_pT_bin_3_RPC_4->GetRMS());
            LE3->AddEntry(h2017_pT_bin_3_RPC_0, S2017_bin_E3_0);
            LE3->AddEntry(h2017_pT_bin_3_RPC_1, S2017_bin_E3_1);
            LE3->AddEntry(h2017_pT_bin_3_RPC_2, S2017_bin_E3_2);
            LE3->AddEntry(h2017_pT_bin_3_RPC_3, S2017_bin_E3_3);
            LE3->AddEntry(h2017_pT_bin_3_RPC_4, S2017_bin_E3_4);
            LE3->Draw();
            E3->Write();
            
            E4->cd();
            h2017_pT_bin_4_RPC_0->SetLineColor(kBlue);
            h2017_pT_bin_4_RPC_0->SetLineWidth(2);
            gStyle->SetOptStat(0);
            h2017_pT_bin_4_RPC_1->SetLineColor(kRed);
            h2017_pT_bin_4_RPC_1->SetLineWidth(2);
            gStyle->SetOptStat(0);
            h2017_pT_bin_4_RPC_2->SetLineColor(kGreen);
            h2017_pT_bin_4_RPC_2->SetLineWidth(2);
            gStyle->SetOptStat(0);
            h2017_pT_bin_4_RPC_3->SetLineColor(kYellow);
            h2017_pT_bin_4_RPC_3->SetLineWidth(2);
            gStyle->SetOptStat(0);
            h2017_pT_bin_4_RPC_4->SetLineColor(kPink);
            h2017_pT_bin_4_RPC_4->SetLineWidth(2);
            gStyle->SetOptStat(0);
            hsE4->Add(h2017_pT_bin_4_RPC_0);
            hsE4->Add(h2017_pT_bin_4_RPC_1);
            hsE4->Add(h2017_pT_bin_4_RPC_2);
            hsE4->Add(h2017_pT_bin_4_RPC_3);
            hsE4->Add(h2017_pT_bin_4_RPC_4);
            hsE4->Draw("nostack");
            hsE4->GetXaxis()->SetTitle("Trigger pT/GEN pT");
            E4->Modified();
            TLegend* LE4 = new TLegend(0.4,0.4,0.9,0.9);
            TString S2017_bin_E4_0="";
            S2017_bin_E4_0= S2017_bin_E4_0 + "Track_Mode_RPC="+TRACK_MODE_RPC[0]+": "+"Mean "+Form("%f", h2017_pT_bin_4_RPC_0->GetMean())+" RMS "+Form("%f", h2017_pT_bin_4_RPC_0->GetRMS());
            TString S2017_bin_E4_1="";
            S2017_bin_E4_1= S2017_bin_E4_1 + "Track_Mode_RPC="+TRACK_MODE_RPC[1]+": "+"Mean "+Form("%f", h2017_pT_bin_4_RPC_1->GetMean())+" RMS "+Form("%f", h2017_pT_bin_4_RPC_1->GetRMS());
            TString S2017_bin_E4_2="";
            S2017_bin_E4_2= S2017_bin_E4_2 + "Track_Mode_RPC="+TRACK_MODE_RPC[2]+": "+"Mean "+Form("%f", h2017_pT_bin_4_RPC_2->GetMean())+" RMS "+Form("%f", h2017_pT_bin_4_RPC_2->GetRMS());
            TString S2017_bin_E4_3="";
            S2017_bin_E4_3= S2017_bin_E4_3 + "Track_Mode_RPC="+TRACK_MODE_RPC[3]+": "+"Mean "+Form("%f", h2017_pT_bin_4_RPC_3->GetMean())+" RMS "+Form("%f", h2017_pT_bin_4_RPC_3->GetRMS());
            TString S2017_bin_E4_4="";
            S2017_bin_E4_4= S2017_bin_E4_4 + "Track_Mode_RPC="+TRACK_MODE_RPC[4]+": "+"Mean "+Form("%f", h2017_pT_bin_4_RPC_4->GetMean())+" RMS "+Form("%f", h2017_pT_bin_4_RPC_4->GetRMS());
            LE4->AddEntry(h2017_pT_bin_4_RPC_0, S2017_bin_E4_0);
            LE4->AddEntry(h2017_pT_bin_4_RPC_1, S2017_bin_E4_1);
            LE4->AddEntry(h2017_pT_bin_4_RPC_2, S2017_bin_E4_2);
            LE4->AddEntry(h2017_pT_bin_4_RPC_3, S2017_bin_E4_3);
            LE4->AddEntry(h2017_pT_bin_4_RPC_4, S2017_bin_E4_4);
            LE4->Draw();
            E4->Write();
            
        }//end if mode 15
        
        //mode 14 and 7
        if(MODE[i] != MODE[0]){
            //mode 14 or 7 different loss function
            TCanvas *F1=new TCanvas(MODE[i]+"F1",MODE[i]+"F1",700,500);
            TCanvas *F2=new TCanvas(MODE[i]+"F2",MODE[i]+"F2",700,500);
            TCanvas *F3=new TCanvas(MODE[i]+"F3",MODE[i]+"F3",700,500);
            TCanvas *F4=new TCanvas(MODE[i]+"F4",MODE[i]+"F4",700,500);
            
            //mode 14 or 7 different loss function
            THStack *hsF1 = new THStack(MODE[i]+"hsF1","Mode "+MODE[i]+" pT Resolution "+PT[0]+"<GEN pT<"+PT[1]+" GeV");
            THStack *hsF2 = new THStack(MODE[i]+"hsF2","Mode "+MODE[i]+" pT Resolution "+PT[1]+"<GEN pT<"+PT[2]+" GeV");
            THStack *hsF3 = new THStack(MODE[i]+"hsF3","Mode "+MODE[i]+" pT Resolution "+PT[2]+"<GEN pT<"+PT[3]+" GeV");
            THStack *hsF4 = new THStack(MODE[i]+"hsF4","Mode "+MODE[i]+" pT Resolution "+PT[3]+"<GEN pT<"+PT[4]+" GeV");
            
            //pT bin1 loss funcs
            F1->cd();
            h2016_pT_bin_1->SetLineColor(kRed);
            h2016_pT_bin_1->SetLineWidth(2);
            gStyle->SetOptStat(0);
            h2017_pT_bin_1_Sq->SetLineColor(kBlue);
            h2017_pT_bin_1_Sq->SetLineWidth(2);
            gStyle->SetOptStat(0);
            h2017_pT_bin_1_Huber->SetLineColor(kGreen);
            h2017_pT_bin_1_Huber->SetLineWidth(2);
            gStyle->SetOptStat(0);
            h2017_pT_bin_1_AbsDev->SetLineColor(kPink);
            h2017_pT_bin_1_AbsDev->SetLineWidth(2);
            gStyle->SetOptStat(0);
            hsF1->Add(h2016_pT_bin_1);
            hsF1->Add(h2017_pT_bin_1_Sq);
            hsF1->Add(h2017_pT_bin_1_Huber);
            hsF1->Add(h2017_pT_bin_1_AbsDev);
            hsF1->Draw("nostack");
            hsF1->GetXaxis()->SetTitle("Trigger pT/GEN pT");
            F1->Modified();
            TLegend* LF1 = new TLegend(0.4,0.4,0.9,0.9);
            TString S2016_bin_F1="";
            S2016_bin_F1= S2016_bin_F1 + "2016: "+"Mean "+Form("%f", h2016_pT_bin_1->GetMean())+" RMS "+Form("%f", h2016_pT_bin_1->GetRMS());
            TString S2017_bin_F1_Sq="";
            S2017_bin_F1_Sq= S2017_bin_F1_Sq + "2017 Least Squares: "+"Mean "+Form("%f", h2017_pT_bin_1_Sq->GetMean())+" RMS "+Form("%f", h2017_pT_bin_1_Sq->GetRMS());
            TString S2017_bin_F1_Huber="";
            S2017_bin_F1_Huber= S2017_bin_F1_Huber + "2017 Huber: "+"Mean "+Form("%f", h2017_pT_bin_1_Huber->GetMean())+" RMS "+Form("%f", h2017_pT_bin_1_Huber->GetRMS());
            TString S2017_bin_F1_AbsDev="";
            S2017_bin_F1_AbsDev= S2017_bin_F1_AbsDev + "2017 Absolute Deviation: "+"Mean "+Form("%f", h2017_pT_bin_1_AbsDev->GetMean())+" RMS "+Form("%f", h2017_pT_bin_1_AbsDev->GetRMS());
            LF1->AddEntry(h2016_pT_bin_1, S2016_bin_F1);
            LF1->AddEntry(h2017_pT_bin_1_Sq, S2017_bin_F1_Sq);
            LF1->AddEntry(h2017_pT_bin_1_Huber, S2017_bin_F1_Huber);
            LF1->AddEntry(h2017_pT_bin_1_AbsDev, S2017_bin_F1_AbsDev);
            LF1->Draw();
            F1->Write();
            
            F2->cd();
            h2016_pT_bin_2->SetLineColor(kRed);
            h2016_pT_bin_2->SetLineWidth(2);
            gStyle->SetOptStat(0);
            h2017_pT_bin_2_Sq->SetLineColor(kBlue);
            h2017_pT_bin_2_Sq->SetLineWidth(2);
            gStyle->SetOptStat(0);
            h2017_pT_bin_2_Huber->SetLineColor(kGreen);
            h2017_pT_bin_2_Huber->SetLineWidth(2);
            gStyle->SetOptStat(0);
            h2017_pT_bin_2_AbsDev->SetLineColor(kPink);
            h2017_pT_bin_2_AbsDev->SetLineWidth(2);
            gStyle->SetOptStat(0);
            hsF2->Add(h2016_pT_bin_2);
            hsF2->Add(h2017_pT_bin_2_Sq);
            hsF2->Add(h2017_pT_bin_2_Huber);
            hsF2->Add(h2017_pT_bin_2_AbsDev);
            hsF2->Draw("nostack");
            hsF2->GetXaxis()->SetTitle("Trigger pT/GEN pT");
            F2->Modified();
            TLegend* LF2 = new TLegend(0.4,0.4,0.9,0.9);
            TString S2016_bin_F2="";
            S2016_bin_F2= S2016_bin_F2 + "2016: "+"Mean "+Form("%f", h2016_pT_bin_2->GetMean())+" RMS "+Form("%f", h2016_pT_bin_2->GetRMS());
            TString S2017_bin_F2_Sq="";
            S2017_bin_F2_Sq= S2017_bin_F2_Sq + "2017 Least Squares: "+"Mean "+Form("%f", h2017_pT_bin_2_Sq->GetMean())+" RMS "+Form("%f", h2017_pT_bin_2_Sq->GetRMS());
            TString S2017_bin_F2_Huber="";
            S2017_bin_F2_Huber= S2017_bin_F2_Huber + "2017 Huber: "+"Mean "+Form("%f", h2017_pT_bin_2_Huber->GetMean())+" RMS "+Form("%f", h2017_pT_bin_2_Huber->GetRMS());
            TString S2017_bin_F2_AbsDev="";
            S2017_bin_F2_AbsDev= S2017_bin_F2_AbsDev + "2017 Absolute Deviation: "+"Mean "+Form("%f", h2017_pT_bin_2_AbsDev->GetMean())+" RMS "+Form("%f", h2017_pT_bin_2_AbsDev->GetRMS());
            LF2->AddEntry(h2016_pT_bin_2, S2016_bin_F2);
            LF2->AddEntry(h2017_pT_bin_2_Sq, S2017_bin_F2_Sq);
            LF2->AddEntry(h2017_pT_bin_2_Huber, S2017_bin_F2_Huber);
            LF2->AddEntry(h2017_pT_bin_2_AbsDev, S2017_bin_F2_AbsDev);
            LF2->Draw();
            F2->Write();
            
            F3->cd();
            h2016_pT_bin_3->SetLineColor(kRed);
            h2016_pT_bin_3->SetLineWidth(2);
            gStyle->SetOptStat(0);
            h2017_pT_bin_3_Sq->SetLineColor(kBlue);
            h2017_pT_bin_3_Sq->SetLineWidth(2);
            gStyle->SetOptStat(0);
            h2017_pT_bin_3_Huber->SetLineColor(kGreen);
            h2017_pT_bin_3_Huber->SetLineWidth(2);
            gStyle->SetOptStat(0);
            h2017_pT_bin_3_AbsDev->SetLineColor(kPink);
            h2017_pT_bin_3_AbsDev->SetLineWidth(2);
            gStyle->SetOptStat(0);
            hsF3->Add(h2016_pT_bin_3);
            hsF3->Add(h2017_pT_bin_3_Sq);
            hsF3->Add(h2017_pT_bin_3_Huber);
            hsF3->Add(h2017_pT_bin_3_AbsDev);
            hsF3->Draw("nostack");
            hsF3->GetXaxis()->SetTitle("Trigger pT/GEN pT");
            F3->Modified();
            TLegend* LF3 = new TLegend(0.4,0.4,0.9,0.9);
            TString S2016_bin_F3="";
            S2016_bin_F3= S2016_bin_F3 + "2016: "+"Mean "+Form("%f", h2016_pT_bin_3->GetMean())+" RMS "+Form("%f", h2016_pT_bin_3->GetRMS());
            TString S2017_bin_F3_Sq="";
            S2017_bin_F3_Sq= S2017_bin_F3_Sq + "2017 Least Squares: "+"Mean "+Form("%f", h2017_pT_bin_3_Sq->GetMean())+" RMS "+Form("%f", h2017_pT_bin_3_Sq->GetRMS());
            TString S2017_bin_F3_Huber="";
            S2017_bin_F3_Huber= S2017_bin_F3_Huber + "2017 Huber: "+"Mean "+Form("%f", h2017_pT_bin_3_Huber->GetMean())+" RMS "+Form("%f", h2017_pT_bin_3_Huber->GetRMS());
            TString S2017_bin_F3_AbsDev="";
            S2017_bin_F3_AbsDev= S2017_bin_F3_AbsDev + "2017 Absolute Deviation: "+"Mean "+Form("%f", h2017_pT_bin_3_AbsDev->GetMean())+" RMS "+Form("%f", h2017_pT_bin_3_AbsDev->GetRMS());
            LF3->AddEntry(h2016_pT_bin_3, S2016_bin_F3);
            LF3->AddEntry(h2017_pT_bin_3_Sq, S2017_bin_F3_Sq);
            LF3->AddEntry(h2017_pT_bin_3_Huber, S2017_bin_F3_Huber);
            LF3->AddEntry(h2017_pT_bin_3_AbsDev, S2017_bin_F3_AbsDev);
            LF3->Draw();
            F3->Write();
            
            F4->cd();
            h2016_pT_bin_4->SetLineColor(kRed);
            h2016_pT_bin_4->SetLineWidth(2);
            gStyle->SetOptStat(0);
            h2017_pT_bin_4_Sq->SetLineColor(kBlue);
            h2017_pT_bin_4_Sq->SetLineWidth(2);
            gStyle->SetOptStat(0);
            h2017_pT_bin_4_Huber->SetLineColor(kGreen);
            h2017_pT_bin_4_Huber->SetLineWidth(2);
            gStyle->SetOptStat(0);
            h2017_pT_bin_4_AbsDev->SetLineColor(kPink);
            h2017_pT_bin_4_AbsDev->SetLineWidth(2);
            gStyle->SetOptStat(0);
            hsF4->Add(h2016_pT_bin_4);
            hsF4->Add(h2017_pT_bin_4_Sq);
            hsF4->Add(h2017_pT_bin_4_Huber);
            hsF4->Add(h2017_pT_bin_4_AbsDev);
            hsF4->Draw("nostack");
            hsF4->GetXaxis()->SetTitle("Trigger pT/GEN pT");
            F4->Modified();
            TLegend* LF4 = new TLegend(0.4,0.4,0.9,0.9);
            TString S2016_bin_F4="";
            S2016_bin_F4= S2016_bin_F4 + "2016: "+"Mean "+Form("%f", h2016_pT_bin_4->GetMean())+" RMS "+Form("%f", h2016_pT_bin_4->GetRMS());
            TString S2017_bin_F4_Sq="";
            S2017_bin_F4_Sq= S2017_bin_F4_Sq + "2017 Least Squares: "+"Mean "+Form("%f", h2017_pT_bin_4_Sq->GetMean())+" RMS "+Form("%f", h2017_pT_bin_4_Sq->GetRMS());
            TString S2017_bin_F4_Huber="";
            S2017_bin_F4_Huber= S2017_bin_F4_Huber + "2017 Huber: "+"Mean "+Form("%f", h2017_pT_bin_4_Huber->GetMean())+" RMS "+Form("%f", h2017_pT_bin_4_Huber->GetRMS());
            TString S2017_bin_F4_AbsDev="";
            S2017_bin_F4_AbsDev= S2017_bin_F4_AbsDev + "2017 Absolute Deviation: "+"Mean "+Form("%f", h2017_pT_bin_4_AbsDev->GetMean())+" RMS "+Form("%f", h2017_pT_bin_4_AbsDev->GetRMS());
            LF4->AddEntry(h2016_pT_bin_4, S2016_bin_F4);
            LF4->AddEntry(h2017_pT_bin_4_Sq, S2017_bin_F4_Sq);
            LF4->AddEntry(h2017_pT_bin_4_Huber, S2017_bin_F4_Huber);
            LF4->AddEntry(h2017_pT_bin_4_AbsDev, S2017_bin_F4_AbsDev);
            LF4->Draw();
            F4->Write();
            
        } //end if
        
        myPlot.Close();
        cout<<"***print out***"<<endl;
        cout<<"Mode "<<MODE[i]<<endl;
        cout<<"===Hist Info=="<<endl;
        
    }//end loop over modes
    
}
