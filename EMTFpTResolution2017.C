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
//=Study the pT resolution performance of 2017 pT assignment.
//=Eta in [1.2, 2.4], plot EMTF 2017 trigger pT/GEN pT Compare 2016 EMTF trigger pT/GEN pT, 
//=and fit both peak area with gaussian. Divide into pT ranges and modes.
//=Wei Shi @ July 27, 2017 CERN Geneva
//=**************************************************************************************
void EMTFpTResolution2017()
{
    TString MODE[11]={"15","14","13","11","7","12","10","9","6","5","3"};
    Double_t mode[11]={15,  14,  13,  11,  7,  12,  10,  9,  6,  5,  3};
    //TString MODE[1]={"15"};
    TString PT[5]={"1","4","8","16","32"};//interest pt bin
    Double_t pt[5]={1,  4,  8,  16,  32};
    
    int MODES;//number of modes
    MODES = sizeof(MODE) / sizeof(TString);
    int PTS;//number of pT bins
    PTS = sizeof(PT) / sizeof(TString) - 1;
    
    //gauss fit range 2016
    Double_t xmin2016_pT_bin_1[11]={0.65,0.40,0.55,0.65,0.50,0.50,0.50,0.65,0.60,0.55,0.55};
    Double_t xmax2016_pT_bin_1[11]={1.15,1.05,1.15,1.10,1.10,1.10,1.10,1.05,1.10,1.10,1.10};
    Double_t xmin2016_pT_bin_2[11]={0.60,0.60,0.50,0.50,0.55,0.55,0.55,0.55,0.50,0.55,0.55};
    Double_t xmax2016_pT_bin_2[11]={1.05,1.10,1.10,1.05,1.10,1.10,1.10,1.10,1.05,1.05,1.05};
    Double_t xmin2016_pT_bin_3[11]={0.70,0.55,0.60,0.60,0.60,0.60,0.60,0.60,0.55,0.60,0.60};
    Double_t xmax2016_pT_bin_3[11]={1.10,1.10,1.20,1.20,1.20,1.20,1.20,1.20,1.20,1.15,1.15};
    Double_t xmin2016_pT_bin_4[11]={0.45,0.45,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00};
    Double_t xmax2016_pT_bin_4[11]={1.00,1.20,2.50,2.50,2.50,2.50,2.50,2.50,2.50,2.50,2.50};
    //2017 fit ratio range
    Double_t xmin2017_pT_bin_1[11]={0.65,0.70,0.65,0.65,0.65,0.65,0.65,0.45,0.80,0.75,0.75};
    Double_t xmax2017_pT_bin_1[11]={1.20,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.20,1.20,1.20};
    Double_t xmin2017_pT_bin_2[11]={0.60,0.70,0.65,0.40,0.30,0.30,0.30,0.30,0.20,0.25,0.25};
    Double_t xmax2017_pT_bin_2[11]={1.10,1.15,1.15,1.15,0.85,0.85,0.85,0.90,0.60,0.55,0.55};
    Double_t xmin2017_pT_bin_3[11]={0.30,0.55,0.55,0.25,0.10,0.10,0.10,0.10,0.00,0.00,0.50};
    Double_t xmax2017_pT_bin_3[11]={1.10,1.15,1.15,0.85,0.35,0.35,0.35,0.85,1.50,1.50,1.20};
    Double_t xmin2017_pT_bin_4[11]={0.45,0.40,0.40,0.40,0.40,0.40,0.40,0.40,0.40,0.40,0.40};
    Double_t xmax2017_pT_bin_4[11]={1.10,1.10,1.10,1.10,1.10,1.10,1.10,1.10,1.10,1.10,1.10};
    
    //extract the mean, sigma and their ratio from the fit
    //2016
    Double_t Mean2016_pT_bin_1[11]={0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00};
    Double_t Sigma2016_pT_bin_1[11]={0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00};
    Double_t MeanOverSigma2016_pT_bin_1[11]={0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00};
    Double_t Mean2016_pT_bin_2[11]={0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00};
    Double_t Sigma2016_pT_bin_2[11]={0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00};
    Double_t MeanOverSigma2016_pT_bin_2[11]={0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00};
    Double_t Mean2016_pT_bin_3[11]={0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00};
    Double_t Sigma2016_pT_bin_3[11]={0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00};
    Double_t MeanOverSigma2016_pT_bin_3[11]={0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00};
    Double_t Mean2016_pT_bin_4[11]={0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00};
    Double_t Sigma2016_pT_bin_4[11]={0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00};
    Double_t MeanOverSigma2016_pT_bin_4[11]={0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00};
    //2017
    Double_t Mean2017_pT_bin_1[11]={0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00};
    Double_t Sigma2017_pT_bin_1[11]={0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00};
    Double_t MeanOverSigma2017_pT_bin_1[11]={0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00};
    Double_t Mean2017_pT_bin_2[11]={0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00};
    Double_t Sigma2017_pT_bin_2[11]={0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00};
    Double_t MeanOverSigma2017_pT_bin_2[11]={0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00};
    Double_t Mean2017_pT_bin_3[11]={0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00};
    Double_t Sigma2017_pT_bin_3[11]={0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00};
    Double_t MeanOverSigma2017_pT_bin_3[11]={0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00};
    Double_t Mean2017_pT_bin_4[11]={0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00};
    Double_t Sigma2017_pT_bin_4[11]={0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00};
    Double_t MeanOverSigma2017_pT_bin_4[11]={0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00};
    
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
                                 
        //=*************************
        //=make histograms in modes*
        //=*************************
        //**2016**
        //GEN pT bin1
        TH1F *h2016_pT_bin_1 = new TH1F("2016 mode "+MODE[i]+" "+PT[0]+"<GEN pT<"+PT[1], "2016 pT Resolution mode "+MODE[i]+" "+PT[0]+"<GEN pT<"+PT[1], 100, 0, 5);
        //GEN pT bin2
        TH1F *h2016_pT_bin_2 = new TH1F("2016 mode "+MODE[i]+" "+PT[1]+"<GEN pT<"+PT[2], "2016 pT Resolution mode "+MODE[i]+" "+PT[1]+"<GEN pT<"+PT[2], 100, 0, 5);
        //GEN pT bin3
        TH1F *h2016_pT_bin_3 = new TH1F("2016 mode "+MODE[i]+" "+PT[2]+"<GEN pT<"+PT[3], "2016 pT Resolution mode "+MODE[i]+" "+PT[2]+"<GEN pT<"+PT[3], 100, 0, 5);
        //GEN pT bin4
        TH1F *h2016_pT_bin_4 = new TH1F("2016 mode "+MODE[i]+" "+PT[3]+"<GEN pT<"+PT[4], "2016 pT Resolution mode "+MODE[i]+" "+PT[3]+"<GEN pT<"+PT[4], 100, 0, 5);
        //**2017**
        //GEN pT bin1
        TH1F *h2017_pT_bin_1 = new TH1F("2017 mode "+MODE[i]+" "+PT[0]+"<GEN pT<"+PT[1], "2017 pT Resolution mode "+MODE[i]+" "+PT[0]+"<GEN pT<"+PT[1], 100, 0, 5);
        //GEN pT bin2
        TH1F *h2017_pT_bin_2 = new TH1F("2017 mode "+MODE[i]+" "+PT[1]+"<GEN pT<"+PT[2], "2017 pT Resolution mode "+MODE[i]+" "+PT[1]+"<GEN pT<"+PT[2], 100, 0, 5);
        //GEN pT bin3
        TH1F *h2017_pT_bin_3 = new TH1F("2017 mode "+MODE[i]+" "+PT[2]+"<GEN pT<"+PT[3], "2017 pT Resolution mode "+MODE[i]+" "+PT[2]+"<GEN pT<"+PT[3], 100, 0, 5);
        //GEN pT bin4
        TH1F *h2017_pT_bin_4 = new TH1F("2017 mode "+MODE[i]+" "+PT[3]+"<GEN pT<"+PT[4], "2017 pT Resolution mode "+MODE[i]+" "+PT[3]+"<GEN pT<"+PT[4], 100, 0, 5);
        
        Long64_t numEvents = myTree->GetEntries();//read the number of entries in myTree
        cout<<">>>>>>>>>>>>>>>>>>>>>"<<endl;
        cout<<numEvents<<" events to process..."<<endl;
    
        for(Long64_t iEntry = 0; iEntry <numEvents; iEntry++){
            //load the i-th event
            myTree->GetEntry(iEntry);
            
            Float_t Ratio2016= EMTF_pt/(GEN_pt*1.4);//divide the 2016 scale factor 1.4
            Float_t Ratio2017= 1/(BDTG_AWB_Sq*GEN_pt);
            
            if(EMTF_mode == TRK_mode && EMTF_mode_RPC == 0 && TRK_mode_RPC == 0){
                
                if(GEN_pt > pt[0] && GEN_pt <= pt[1]){
                    h2016_pT_bin_1->Fill(Ratio2016);
                    h2017_pT_bin_1->Fill(Ratio2017);
                }
                if(GEN_pt > pt[1] && GEN_pt <= pt[2]){
                    h2016_pT_bin_2->Fill(Ratio2016);
                    h2017_pT_bin_2->Fill(Ratio2017);
                }
                if(GEN_pt > pt[2] && GEN_pt <= pt[3]){
                    h2016_pT_bin_3->Fill(Ratio2016);
                    h2017_pT_bin_3->Fill(Ratio2017);
                }
                if(GEN_pt > pt[3] && GEN_pt <= pt[4]){
                    h2016_pT_bin_4->Fill(Ratio2016);
                    h2017_pT_bin_4->Fill(Ratio2017);
                }//end if
                
            }//end if: trk mode selection
            
        }//end loop over events
        //output
        TString output="/afs/cern.ch/work/w/wshi/public/EMTFpTResolution/EMTFpTResolution2017_Mode_"+MODE[i]+".root";
        TFile myPlot(output,"RECREATE");
        
        TCanvas *C1=new TCanvas(MODE[i]+"C1",MODE[i]+"C1",700,500);
        TCanvas *C2=new TCanvas(MODE[i]+"C2",MODE[i]+"C2",700,500);
        TCanvas *C3=new TCanvas(MODE[i]+"C3",MODE[i]+"C3",700,500);
        TCanvas *C4=new TCanvas(MODE[i]+"C4",MODE[i]+"C4",700,500);
        THStack *hs1 = new THStack(MODE[i]+"hs1","Mode "+MODE[i]+" pT Resolution "+PT[0]+"<GEN pT<"+PT[1]+" GeV");
        THStack *hs2 = new THStack(MODE[i]+"hs2","Mode "+MODE[i]+" pT Resolution "+PT[1]+"<GEN pT<"+PT[2]+" GeV");
        THStack *hs3 = new THStack(MODE[i]+"hs3","Mode "+MODE[i]+" pT Resolution "+PT[2]+"<GEN pT<"+PT[3]+" GeV");
        THStack *hs4 = new THStack(MODE[i]+"hs4","Mode "+MODE[i]+" pT Resolution "+PT[3]+"<GEN pT<"+PT[4]+" GeV");
        
        C1->cd();
        h2016_pT_bin_1->SetLineColor(kRed);
        h2016_pT_bin_1->SetLineWidth(2);
        h2016_pT_bin_1->Fit("gaus","","",xmin2016_pT_bin_1[i],xmax2016_pT_bin_1[i]);
        Mean2016_pT_bin_1[i]= h2016_pT_bin_1->GetFunction("gaus")->GetParameter(1);//get 2nd parameter Mean
        Sigma2016_pT_bin_1[i]= h2016_pT_bin_1->GetFunction("gaus")->GetParameter(2);//get 3rd parameter Sigma
        MeanOverSigma2016_pT_bin_1[i]= Sigma2016_pT_bin_1[i]/Mean2016_pT_bin_1[i];// sigma/mean metric
        h2016_pT_bin_1->GetFunction("gaus")->SetLineColor(kRed);
        h2016_pT_bin_1->GetFunction("gaus")->SetLineStyle(2);//dash"----"
        //gStyle->SetOptFit(1);
        gStyle->SetOptStat(0);
        
        h2017_pT_bin_1->SetLineColor(kBlue);
        h2017_pT_bin_1->SetLineWidth(2);
        h2017_pT_bin_1->Fit("gaus","","",xmin2017_pT_bin_1[i],xmax2017_pT_bin_1[i]);
        Mean2017_pT_bin_1[i]= h2017_pT_bin_1->GetFunction("gaus")->GetParameter(1);
        Sigma2017_pT_bin_1[i]= h2017_pT_bin_1->GetFunction("gaus")->GetParameter(2);
        MeanOverSigma2017_pT_bin_1[i]= Sigma2017_pT_bin_1[i]/Mean2017_pT_bin_1[i];
        h2017_pT_bin_1->GetFunction("gaus")->SetLineColor(kBlue);
        h2017_pT_bin_1->GetFunction("gaus")->SetLineStyle(2);
        //gStyle->SetOptFit(1);
        gStyle->SetOptStat(0);
        
        //TPaveStats *st1 = (TPaveStats*)h2017_pT_bin_1->FindObject("stats");
        //st1->SetY1NDC(0.2);
        //st1->SetY2NDC(0.55);
        hs1->Add(h2016_pT_bin_1);
        hs1->Add(h2017_pT_bin_1);
        hs1->Draw("nostack");
        hs1->GetXaxis()->SetTitle("Trigger pT/GEN pT");
        C1->Modified();
        TLegend* L1 = new TLegend(0.4,0.7,0.6,0.9);
        L1->AddEntry(h2016_pT_bin_1,"2016");
        L1->AddEntry(h2017_pT_bin_1,"2017");
        L1->Draw();
        C1->Write();
        
        C2->cd();
        h2016_pT_bin_2->SetLineColor(kRed);
        h2016_pT_bin_2->SetLineWidth(2);
        h2016_pT_bin_2->Fit("gaus","","",xmin2016_pT_bin_2[i],xmax2016_pT_bin_2[i]);
        Mean2016_pT_bin_2[i]= h2016_pT_bin_2->GetFunction("gaus")->GetParameter(1);//get 2nd parameter Mean
        Sigma2016_pT_bin_2[i]= h2016_pT_bin_2->GetFunction("gaus")->GetParameter(2);//get 3rd parameter Sigma
        MeanOverSigma2016_pT_bin_2[i]= Sigma2016_pT_bin_2[i]/Mean2016_pT_bin_2[i];// sigma/mean metric
        h2016_pT_bin_2->GetFunction("gaus")->SetLineColor(kRed);
        h2016_pT_bin_2->GetFunction("gaus")->SetLineStyle(2);//dash"----"
        //gStyle->SetOptFit(1);
        gStyle->SetOptStat(0);
        
        h2017_pT_bin_2->SetLineColor(kBlue);
        h2017_pT_bin_2->SetLineWidth(2);
        h2017_pT_bin_2->Fit("gaus","","",xmin2017_pT_bin_2[i],xmax2017_pT_bin_2[i]);
        Mean2017_pT_bin_2[i]= h2017_pT_bin_2->GetFunction("gaus")->GetParameter(1);
        Sigma2017_pT_bin_2[i]= h2017_pT_bin_2->GetFunction("gaus")->GetParameter(2);
        MeanOverSigma2017_pT_bin_2[i]= Sigma2017_pT_bin_2[i]/Mean2017_pT_bin_2[i];
        h2017_pT_bin_2->GetFunction("gaus")->SetLineColor(kBlue);
        h2017_pT_bin_2->GetFunction("gaus")->SetLineStyle(2);
        //gStyle->SetOptFit(1);
        gStyle->SetOptStat(0);
        
        //TPaveStats *st2 = (TPaveStats*)h2017_pT_bin_2->FindObject("stats");
        //st2->SetY1NDC(0.2);
        //st2->SetY2NDC(0.55);
        hs2->Add(h2016_pT_bin_2);
        hs2->Add(h2017_pT_bin_2);
        hs2->Draw("nostack");
        hs2->GetXaxis()->SetTitle("Trigger pT/GEN pT");
        C2->Modified();
        TLegend* L2 = new TLegend(0.4,0.7,0.6,0.9);
        L2->AddEntry(h2016_pT_bin_2,"2016");
        L2->AddEntry(h2017_pT_bin_2,"2017");
        L2->Draw();
        C2->Write();
        
        C3->cd();
        h2016_pT_bin_3->SetLineColor(kRed);
        h2016_pT_bin_3->SetLineWidth(2);
        h2016_pT_bin_3->Fit("gaus","","",xmin2016_pT_bin_3[i],xmax2016_pT_bin_3[i]);
        Mean2016_pT_bin_3[i]= h2016_pT_bin_3->GetFunction("gaus")->GetParameter(1);//get 2nd parameter Mean
        Sigma2016_pT_bin_3[i]= h2016_pT_bin_3->GetFunction("gaus")->GetParameter(2);//get 3rd parameter Sigma
        MeanOverSigma2016_pT_bin_3[i]= Sigma2016_pT_bin_3[i]/Mean2016_pT_bin_3[i];// sigma/mean metric
        h2016_pT_bin_3->GetFunction("gaus")->SetLineColor(kRed);
        h2016_pT_bin_3->GetFunction("gaus")->SetLineStyle(2);//dash"----"
        //gStyle->SetOptFit(1);
        gStyle->SetOptStat(0);
        
        h2017_pT_bin_3->SetLineColor(kBlue);
        h2017_pT_bin_3->SetLineWidth(2);
        h2017_pT_bin_3->Fit("gaus","","",xmin2017_pT_bin_3[i],xmax2017_pT_bin_3[i]);
        Mean2017_pT_bin_3[i]= h2017_pT_bin_3->GetFunction("gaus")->GetParameter(1);
        Sigma2017_pT_bin_3[i]= h2017_pT_bin_3->GetFunction("gaus")->GetParameter(2);
        MeanOverSigma2017_pT_bin_3[i]= Sigma2017_pT_bin_3[i]/Mean2017_pT_bin_3[i];
        h2017_pT_bin_3->GetFunction("gaus")->SetLineColor(kBlue);
        h2017_pT_bin_3->GetFunction("gaus")->SetLineStyle(2);
        gStyle->SetOptStat(0);
        
        //TPaveStats *st3 = (TPaveStats*)h2017_pT_bin_3->FindObject("stats");
        //st3->SetY1NDC(0.2);
        //st3->SetY2NDC(0.55);
        hs3->Add(h2016_pT_bin_3);
        hs3->Add(h2017_pT_bin_3);
        hs3->Draw("nostack");
        hs3->GetXaxis()->SetTitle("Trigger pT/GEN pT");
        C3->Modified();
        TLegend* L3 = new TLegend(0.4,0.7,0.6,0.9);
        L3->AddEntry(h2016_pT_bin_3,"2016");
        L3->AddEntry(h2017_pT_bin_3,"2017");
        L3->Draw();
        C3->Write();
        
        C4->cd();
        h2016_pT_bin_4->SetLineColor(kRed);
        h2016_pT_bin_4->SetLineWidth(2);
        h2016_pT_bin_4->Fit("gaus","","",xmin2016_pT_bin_4[i],xmax2016_pT_bin_4[i]);
        Mean2016_pT_bin_4[i]= h2016_pT_bin_4->GetFunction("gaus")->GetParameter(1);//get 2nd parameter Mean
        Sigma2016_pT_bin_4[i]= h2016_pT_bin_4->GetFunction("gaus")->GetParameter(2);//get 3rd parameter Sigma
        MeanOverSigma2016_pT_bin_4[i]= Sigma2016_pT_bin_4[i]/Mean2016_pT_bin_4[i];// sigma/mean metric
        h2016_pT_bin_4->GetFunction("gaus")->SetLineColor(kRed);
        h2016_pT_bin_4->GetFunction("gaus")->SetLineStyle(2);//dash"----"
        gStyle->SetOptStat(0);
        
        h2017_pT_bin_4->SetLineColor(kBlue);
        h2017_pT_bin_4->SetLineWidth(2);
        h2017_pT_bin_4->Fit("gaus","","",xmin2017_pT_bin_4[i],xmax2017_pT_bin_4[i]);
        Mean2017_pT_bin_4[i]= h2017_pT_bin_4->GetFunction("gaus")->GetParameter(1);
        Sigma2017_pT_bin_4[i]= h2017_pT_bin_4->GetFunction("gaus")->GetParameter(2);
        MeanOverSigma2017_pT_bin_4[i]= Sigma2017_pT_bin_4[i]/Mean2017_pT_bin_4[i];
        h2017_pT_bin_4->GetFunction("gaus")->SetLineColor(kBlue);
        h2017_pT_bin_4->GetFunction("gaus")->SetLineStyle(2);
        gStyle->SetOptStat(0);
        
        //TPaveStats *st4 = (TPaveStats*)h2017_pT_bin_4->FindObject("stats");
        //st4->SetY1NDC(0.2);
        //st4->SetY2NDC(0.55);
        hs4->Add(h2016_pT_bin_4);
        hs4->Add(h2017_pT_bin_4);
        hs4->Draw("nostack");
        hs4->GetXaxis()->SetTitle("Trigger pT/GEN pT");
        C4->Modified();
        TLegend* L4 = new TLegend(0.4,0.7,0.6,0.9);
        L4->AddEntry(h2016_pT_bin_4,"2016");
        L4->AddEntry(h2017_pT_bin_4,"2017");
        L4->Draw();
        C4->Write();
        
        myPlot.Close();
        cout<<"===print out"<<endl;
        cout<<"===Mode "<<MODE[i]<<endl;
        cout<<"***********************************************************************************************************"<<endl;
        cout<<"* GEN pT Range[GeV] * 2016 Mean * 2017 Mean * 2016 Sigma * 2017 Sigma * 2016 Sigma/Mean * 2017 Sigma/Mean *"<<endl;
        cout<<"* ("<<setw(6)<<setprecision(2)<<PT[0]<<", "<<setw(7)<<setprecision(2)<<PT[1]<<"] * "<<setw(9)<<setprecision(3)<<Mean2016_pT_bin_1[i]<<" * "<<setw(9)<<setprecision(3)<<Mean2017_pT_bin_1[i]<<" * "<<setw(10)<<setprecision(3)<<Sigma2016_pT_bin_1[i]<<" * "<<setw(10)<<setprecision(3)<<Sigma2017_pT_bin_1[i]<<" * "<<setw(15)<<setprecision(3)<<MeanOverSigma2016_pT_bin_1[i]<<" * "<<setw(15)<<setprecision(3)<<MeanOverSigma2017_pT_bin_1[i]<<" *"<<endl;
        cout<<"* ("<<setw(6)<<setprecision(2)<<PT[1]<<", "<<setw(7)<<setprecision(2)<<PT[2]<<"] * "<<setw(9)<<setprecision(3)<<Mean2016_pT_bin_2[i]<<" * "<<setw(9)<<setprecision(3)<<Mean2017_pT_bin_2[i]<<" * "<<setw(10)<<setprecision(3)<<Sigma2016_pT_bin_2[i]<<" * "<<setw(10)<<setprecision(3)<<Sigma2017_pT_bin_2[i]<<" * "<<setw(15)<<setprecision(3)<<MeanOverSigma2016_pT_bin_2[i]<<" * "<<setw(15)<<setprecision(3)<<MeanOverSigma2017_pT_bin_2[i]<<" *"<<endl;
        cout<<"* ("<<setw(6)<<setprecision(2)<<PT[2]<<", "<<setw(7)<<setprecision(2)<<PT[3]<<"] * "<<setw(9)<<setprecision(3)<<Mean2016_pT_bin_3[i]<<" * "<<setw(9)<<setprecision(3)<<Mean2017_pT_bin_3[i]<<" * "<<setw(10)<<setprecision(3)<<Sigma2016_pT_bin_3[i]<<" * "<<setw(10)<<setprecision(3)<<Sigma2017_pT_bin_3[i]<<" * "<<setw(15)<<setprecision(3)<<MeanOverSigma2016_pT_bin_3[i]<<" * "<<setw(15)<<setprecision(3)<<MeanOverSigma2017_pT_bin_3[i]<<" *"<<endl;
        cout<<"* ("<<setw(6)<<setprecision(2)<<PT[3]<<", "<<setw(7)<<setprecision(2)<<PT[4]<<"] * "<<setw(9)<<setprecision(3)<<Mean2016_pT_bin_4[i]<<" * "<<setw(9)<<setprecision(3)<<Mean2017_pT_bin_4[i]<<" * "<<setw(10)<<setprecision(3)<<Sigma2016_pT_bin_4[i]<<" * "<<setw(10)<<setprecision(3)<<Sigma2017_pT_bin_4[i]<<" * "<<setw(15)<<setprecision(3)<<MeanOverSigma2016_pT_bin_4[i]<<" * "<<setw(15)<<setprecision(3)<<MeanOverSigma2017_pT_bin_4[i]<<" *"<<endl;
        cout<<"***********************************************************************************************************"<<endl;
    }//end loop over modes

    cout<<"Write to all files."<<endl;
    
}//end EMTFpTResolution2017()
