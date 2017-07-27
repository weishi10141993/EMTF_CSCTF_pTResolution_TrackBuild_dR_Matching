#include <iostream>
#include <iomanip>
using namespace std;
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include <vector>
#include "stdio.h"
#include <stdlib.h>
#include "math.h"
#include "TMath.h"
#include "TGraph.h"
#include <TLegend.h>
//=**************************************************************************************
//=Study the pT resolution performance of 2017 pT assignment.
//=Eta in [1.2, 2.4], plot EMTF 2017 trigger pT/GEN pT Compare 2016 EMTF trigger pT/GEN pT, 
//=and fit both peak area with gaussian. Divide into pT ranges and modes.
//=Wei Shi @ July 27, 2017 CERN Geneva
//=**************************************************************************************
void EMTFpTResolution2017()
{
    //TString MODE[11]={"15","14","13","11","7","12","10","9","6","5","3"};
    TString MODE[1]={"15"};
    
    //loop through all modes
    for (int i=0;i<1;i++){
        
        //NOTE: for mode 15, I changed the date 06_05 in file name to 05_10 to keep consistency with other modes
        TString fileName="/afs/cern.ch/work/w/wshi/public/EMTFpTResolution/PtRegression_Apr_2017_05_10_invPtTarg_invPtWgt_MODE_" + MODE[i] + "_bitCompr_RPC.root";
        TString directoryName="f_MODE_"+ MODE[i]+ "_invPtTarg_invPtWgt_bitCompr_RPC/TestTree";
        TFile* myFile = new TFile(fileName);
        TTree* myTree = (TTree*) myFile->Get(directoryName);
        
        cout<<"Accessing file:"<<fileName<<endl;
        
        
        Float_t GEN_pt;
        Float_t EMTF_pt;
        Float_t BDTG_AWB_Sq;

        //event info: Pull variables from nTuple
        myTree->SetBranchAddress("GEN_pt",&GEN_pt);//GEN pT
        myTree->SetBranchAddress("EMTF_pt",&EMTF_pt);//2016 EMTF
        myTree->SetBranchAddress("BDTG_AWB_Sq",&BDTG_AWB_Sq);//2017: 1/BDTG pT
        
        
        //=*************************
        //=make histograms in modes*
        //=*************************
        //GEN pT 0-10 GeV
        TH1F *h_pT_0_10 = new TH1F("mode "+MODE[i]+" 0<GEN pT<10","pT Resolution mode "+MODE[i]+" 0<GEN pT<10", 100, 0, 5);
        //GEN pT 10-30 GeV
        TH1F *h_pT_10_30 = new TH1F("mode "+MODE[i]+" 10<GEN pT<30","pT Resolution mode "+MODE[i]+" 10<GEN pT<30", 100, 0, 5);
        //GEN pT 30-100 GeV
        TH1F *h_pT_30_100 = new TH1F("mode "+MODE[i]+" 30<GEN pT<100","pT Resolution mode "+MODE[i]+" 30<GEN pT<100", 100, 0, 5);
        //GEN pT >100 GeV muon shower
        TH1F *h_pT_100_1000 = new TH1F("mode "+MODE[i]+" 100<GEN pT<1000","pT Resolution mode "+MODE[i]+" 100<GEN pT<1000", 100, 0, 5);
       
        h_pT_0_10->SetFillColorAlpha(kRed,0.75);
        h_pT_10_30->SetFillColorAlpha(kRed,0.75);
        h_pT_30_100->SetFillColorAlpha(kRed,0.75);
        h_pT_100_1000->SetFillColorAlpha(kRed,0.75);
        
        Long64_t numEvents = myTree->GetEntries();//read the number of entries in myTree
        cout<<">>>>>>>>>>>>>>>>>>>>>"<<endl;
        cout<<numEvents<<" events to process..."<<endl;
    
        for(Long64_t iEntry = 0; iEntry <numEvents; iEntry++){
            //load the i-th event
            myTree->GetEntry(iEntry);
            
            Float_t Ratio= EMTF_pt/GEN_pt;
            
            if(GEN_pt > 0 && GEN_pt <= 10){
                h_pT_0_10->Fill(Ratio);
            }
            if(GEN_pt > 10 && GEN_pt <= 30){
                h_pT_10_30->Fill(Ratio);
            }
            if(GEN_pt > 30 && GEN_pt <= 100){
                h_pT_30_100->Fill(Ratio);
            }
            if(GEN_pt > 100 && GEN_pt <= 1000){
                h_pT_100_1000->Fill(Ratio);
            }//end if
            
        }//end loop over events
        //output
        TFile myPlot("/afs/cern.ch/work/w/wshi/public/EMTFpTResolution/EMTFpTResolution2017.root","RECREATE");
        h_pT_0_10->Write();
        h_pT_10_30->Write();
        h_pT_30_100->Write();
        h_pT_100_1000->Write();
        
    }//end loop over modes
    
    myPlot.Close();
    cout<<"Write to file"<<endl;
    
}//end EMTFpTResolution2017()
