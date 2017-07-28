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
    int MODES;
    MODES = sizeof(MODE) / sizeof(TString);
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

        //event info: Pull variables from nTuple
        myTree->SetBranchAddress("GEN_pt",&GEN_pt);//GEN pT
        myTree->SetBranchAddress("EMTF_pt",&EMTF_pt);//2016 EMTF
        myTree->SetBranchAddress("BDTG_AWB_Sq",&BDTG_AWB_Sq);//2017: 1/BDTG pT
        
        
        //=*************************
        //=make histograms in modes*
        //=*************************
        //**2016**
        //GEN pT 0-10 GeV
        TH1F *h_pT_0_10_2016 = new TH1F("2016 mode "+MODE[i]+" 0<GEN pT<10","2016 pT Resolution mode "+MODE[i]+" 0<GEN pT<10", 100, 0, 5);
        //GEN pT 10-30 GeV
        TH1F *h_pT_10_30_2016 = new TH1F("2016 mode "+MODE[i]+" 10<GEN pT<30","2016 pT Resolution mode "+MODE[i]+" 10<GEN pT<30", 100, 0, 5);
        //GEN pT 30-100 GeV
        TH1F *h_pT_30_100_2016 = new TH1F("2016 mode "+MODE[i]+" 30<GEN pT<100","2016 pT Resolution mode "+MODE[i]+" 30<GEN pT<100", 100, 0, 5);
        //GEN pT >100 GeV muon shower
        TH1F *h_pT_100_1000_2016 = new TH1F("2016 mode "+MODE[i]+" 100<GEN pT<1000","2016 pT Resolution mode "+MODE[i]+" 100<GEN pT<1000", 100, 0, 5);
        //**2017**
        //GEN pT 0-10 GeV
        TH1F *h_pT_0_10_2017 = new TH1F("2017 mode "+MODE[i]+" 0<GEN pT<10","2017 pT Resolution mode "+MODE[i]+" 0<GEN pT<10", 100, 0, 5);
        //GEN pT 10-30 GeV
        TH1F *h_pT_10_30_2017 = new TH1F("2017 mode "+MODE[i]+" 10<GEN pT<30","2017 pT Resolution mode "+MODE[i]+" 10<GEN pT<30", 100, 0, 5);
        //GEN pT 30-100 GeV
        TH1F *h_pT_30_100_2017 = new TH1F("2017 mode "+MODE[i]+" 30<GEN pT<100","2017 pT Resolution mode "+MODE[i]+" 30<GEN pT<100", 100, 0, 5);
        //GEN pT >100 GeV muon shower
        TH1F *h_pT_100_1000_2017 = new TH1F("2017 mode "+MODE[i]+" 100<GEN pT<1000","2017 pT Resolution mode "+MODE[i]+" 100<GEN pT<1000", 100, 0, 5);
        
        Long64_t numEvents = myTree->GetEntries();//read the number of entries in myTree
        cout<<">>>>>>>>>>>>>>>>>>>>>"<<endl;
        cout<<numEvents<<" events to process..."<<endl;
    
        for(Long64_t iEntry = 0; iEntry <numEvents; iEntry++){
            //load the i-th event
            myTree->GetEntry(iEntry);
            
            Float_t Ratio2016= EMTF_pt/GEN_pt;
            Float_t Ratio2017= 1/(BDTG_AWB_Sq*GEN_pt);
            
            
            if(GEN_pt > 0 && GEN_pt <= 10){
                
                h_pT_0_10_2016->Fill(Ratio2016);
                h_pT_0_10_2016->SetLineColor(kRed);
                
                h_pT_0_10_2017->Fill(Ratio2017);
                h_pT_0_10_2017->SetLineColor(kBlue);
            }
            if(GEN_pt > 10 && GEN_pt <= 30){
                
                h_pT_10_30_2016->Fill(Ratio2016);
                h_pT_10_30_2016->SetLineColor(kRed);
                
                h_pT_10_30_2017->Fill(Ratio2017);
                h_pT_10_30_2017->SetLineColor(kBlue);
            }
            if(GEN_pt > 30 && GEN_pt <= 100){
                
                h_pT_30_100_2016->Fill(Ratio2016);
                h_pT_30_100_2016->SetLineColor(kRed);
                
                h_pT_30_100_2017->Fill(Ratio2017);
                h_pT_30_100_2017->SetLineColor(kBlue);
            }
            if(GEN_pt > 100 && GEN_pt <= 1000){
                
                h_pT_100_1000_2016->Fill(Ratio2016);
                h_pT_100_1000_2016->SetLineColor(kRed);
                
                h_pT_100_1000_2017->Fill(Ratio2017);
                h_pT_100_1000_2017->SetLineColor(kBlue);
            }//end if
            
        }//end loop over events
        //output
        TString output="/afs/cern.ch/work/w/wshi/public/EMTFpTResolution/EMTFpTResolution2017_Mode_"+MODE[i]+".root";
        TFile myPlot(output,"RECREATE");
        
        TCanvas *C1=new TCanvas(MODE[i]+"C1",MODE[i]+"C1",700,500);
        TCanvas *C2=new TCanvas(MODE[i]+"C2",MODE[i]+"C2",700,500);
        TCanvas *C3=new TCanvas(MODE[i]+"C3",MODE[i]+"C3",700,500);
        TCanvas *C4=new TCanvas(MODE[i]+"C4",MODE[i]+"C4",700,500);
        THStack *hs1 = new THStack(MODE[i]+"hs1","Mode "+MODE[i]+" pT Resolution 0<GEN pT<10 GeV");
        THStack *hs2 = new THStack(MODE[i]+"hs2","Mode "+MODE[i]+" pT Resolution 10<GEN pT<30 GeV");
        THStack *hs3 = new THStack(MODE[i]+"hs3","Mode "+MODE[i]+" pT Resolution 30<GEN pT<100 GeV");
        THStack *hs4 = new THStack(MODE[i]+"hs4","Mode "+MODE[i]+" pT Resolution 100<GEN pT<1000 GeV");
        
        C1->cd();
        h_pT_0_10_2016->Fit("gaus","","",0.75,1.55);
        gStyle->SetOptFit(1);
        h_pT_0_10_2017->Fit("gaus","","",0,1.25);
        gStyle->SetOptFit(1);
        TPaveStats *st1 = (TPaveStats*)h_pT_0_10_2017->FindObject("stats")
        st1->SetY1NDC(0.6);
        st1->SetY2NDC(0.4);
        hs1->Add(h_pT_0_10_2016);
        hs1->Add(h_pT_0_10_2017);
        hs1->Draw("nostack");
        hs1->GetXaxis()->SetTitle("Trigger pT/GEN pT");
        C1->Modified();
        TLegend* L1 = new TLegend(0.6,0.2,0.9,0.4);
        L1->AddEntry(h_pT_0_10_2016,"2016");
        L1->AddEntry(h_pT_0_10_2017,"2017");
        L1->Draw();
        C1->Write();
        
        C2->cd();
        hs2->Add(h_pT_10_30_2016);
        hs2->Add(h_pT_10_30_2017);
        hs2->Draw("nostack");
        hs2->GetXaxis()->SetTitle("Trigger pT/GEN pT");
        C2->Modified();
        TLegend* L2 = new TLegend(0.6,0.7,0.9,0.9);
        L2->AddEntry(h_pT_10_30_2016,"2016");
        L2->AddEntry(h_pT_10_30_2017,"2017");
        L2->Draw();
        C2->Write();
        
        C3->cd();
        hs3->Add(h_pT_30_100_2016);
        hs3->Add(h_pT_30_100_2017);
        hs3->Draw("nostack");
        hs3->GetXaxis()->SetTitle("Trigger pT/GEN pT");
        C3->Modified();
        TLegend* L3 = new TLegend(0.6,0.7,0.9,0.9);
        L3->AddEntry(h_pT_30_100_2016,"2016");
        L3->AddEntry(h_pT_30_100_2017,"2017");
        L3->Draw();
        C3->Write();
        
        C4->cd();
        hs4->Add(h_pT_100_1000_2016);
        hs4->Add(h_pT_100_1000_2017);
        hs4->Draw("nostack");
        hs4->GetXaxis()->SetTitle("Trigger pT/GEN pT");
        C4->Modified();
        TLegend* L4 = new TLegend(0.6,0.7,0.9,0.9);
        L4->AddEntry(h_pT_100_1000_2016,"2016");
        L4->AddEntry(h_pT_100_1000_2017,"2017");
        L4->Draw();
        C4->Write();
        
        myPlot.Close();
        cout<<"Finished processing this file..."<<endl;
    }//end loop over modes

    cout<<"Write to all files."<<endl;
    
}//end EMTFpTResolution2017()
