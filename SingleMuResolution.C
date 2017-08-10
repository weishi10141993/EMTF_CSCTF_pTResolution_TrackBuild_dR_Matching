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
        Float_t EMTF_pt;
        Float_t BDTG_AWB_Sq;
        Float_t EMTF_mode;
        Float_t TRK_mode;
        Float_t EMTF_mode_RPC;
        Float_t TRK_mode_RPC;

        //event info: Pull variables from nTuple
        myTree->SetBranchAddress("GEN_pt",&GEN_pt);//GEN pT
        myTree->SetBranchAddress("EMTF_pt",&EMTF_pt);//2016 EMTF
        myTree->SetBranchAddress("BDTG_AWB_Sq",&BDTG_AWB_Sq);//2017: 1/BDTG pT
        myTree->SetBranchAddress("EMTF_mode",&EMTF_mode);
        myTree->SetBranchAddress("TRK_mode",&TRK_mode);
        myTree->SetBranchAddress("EMTF_mode_RPC",&EMTF_mode_RPC);
        myTree->SetBranchAddress("TRK_mode_RPC",&TRK_mode_RPC);
                                 
        //=****************
        //=make histograms*
        //=****************
        
    }//end loop over modes
    
}
