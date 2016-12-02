#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include <vector>
#include "stdio.h"
#include "math.h"
#include "TMath.h"
#include "TGraph.h"
//=**************************************************************************************
//=This version includes Andrew's suggestions on the study after 08/01/2016 EMTF meeting*
//=**************************************************************************************
void EMTFpTResolution_v8()
{
    
    TFile* myFile = new TFile("/afs/cern.ch/work/w/wshi/public/EMTFpTResolution/EMTF_NTuple_274440_443_v3.root");
    TTree* myTree = (TTree*) myFile->Get("ntuple/tree");
    
    vector<float> *RecoPt;//RECO Pt
    vector<float> *RecoEta;//RECO eta
    vector<float> *RecoPhi;//RECO Phi
    vector<float> *TrkPt;//EMTF Pt
    vector<float> *TrkEta;//EMTF eta
    vector<float> *TrkPhi;//EMTF phi
    vector<int> *TrkMode;//EMTF mode
    vector<int> *TrkBx;
    
    //event info: Pull variables from nTuple: pointer pointing to variables
    
    myTree->SetBranchAddress("recoPt",&RecoPt);//RECO
    myTree->SetBranchAddress("recoEta",&RecoEta);
    myTree->SetBranchAddress("recoPhi",&RecoPhi);
    myTree->SetBranchAddress("trkPt",&TrkPt);//EMTF
    myTree->SetBranchAddress("trkEta",&TrkEta);
    myTree->SetBranchAddress("trkPhi",&TrkPhi);
    myTree->SetBranchAddress("trkMode",&TrkMode);
    myTree->SetBranchAddress("trkBx",&TrkBx);//require EMTF tracks in BX=0
    
    //*************************
    //make histograms in modes*
    //*************************
    TH1F *Relative_Residual_all_modes =new TH1F("Relative Residual all modes","Relative Residual all modes",700, -2, 5);
    TH1F *Delta_R_all_modes =new TH1F("dR all modes","dR all modes",100,0,0.3);
    
    TH1F *Relative_Residual_mode_15_plot = new TH1F("Relative Residual mode 15","Relative Residual mode 15", 700, -2, 5);
    TH1F *Relative_Residual_mode_14_plot = new TH1F("Relative Residual mode 14","Relative Residual mode 14", 700, -2, 5);
    TH1F *Relative_Residual_mode_13_plot = new TH1F("Relative Residual mode 13","Relative Residual mode 13", 700, -2, 5);
    TH1F *Relative_Residual_mode_12_plot = new TH1F("Relative Residual mode 12","Relative Residual mode 12", 700, -2, 5);
    TH1F *Relative_Residual_mode_7_plot = new TH1F("Relative Residual mode 7","Relative Residual mode 7", 700, -2, 5);
    
    TH1F *Delta_R_mode_15_plot = new TH1F("dR mode 15","dR mode 15",100,0,0.3);//PLOT THIS JUST FOR FUN
    TH1F *Delta_R_mode_14_plot = new TH1F("dR mode 14","dR mode 14",100,0,0.3);
    TH1F *Delta_R_mode_13_plot = new TH1F("dR mode 13","dR mode 13",100,0,0.3);
    TH1F *Delta_R_mode_12_plot = new TH1F("dR mode 12","dR mode 12",100,0,0.3);
    TH1F *Delta_R_mode_7_plot = new TH1F("dR mode 7","dR mode 7",100,0,0.3);
    
    //RECO pT 0-10 GeV
    TH1F *Relative_Residual_mode_15_plot_RECO_0_10 = new TH1F("Relative Residual mode 15 0<RECO<10","Relative Residual mode 15 0<RECO<10", 700, -2, 5);
    TH1F *Relative_Residual_mode_14_plot_RECO_0_10 = new TH1F("Relative Residual mode 14 0<RECO<10","Relative Residual mode 14 0<RECO<10", 700, -2, 5);
    TH1F *Relative_Residual_mode_13_plot_RECO_0_10 = new TH1F("Relative Residual mode 13 0<RECO<10","Relative Residual mode 13 0<RECO<10", 700, -2, 5);
    TH1F *Relative_Residual_mode_12_plot_RECO_0_10 = new TH1F("Relative Residual mode 12 0<RECO<10","Relative Residual mode 12 0<RECO<10", 700, -2, 5);
    TH1F *Relative_Residual_mode_7_plot_RECO_0_10 = new TH1F("Relative Residual mode 7 0<RECO<10","Relative Residual mode 7 0<RECO<10", 700, -2, 5);
    
    //RECO pT 10-30 GeV
    TH1F *Relative_Residual_mode_15_plot_RECO_10_30 = new TH1F("Relative Residual mode 15 10<RECO<30","Relative Residual mode 15 10<RECO<30", 700, -2, 5);
    TH1F *Relative_Residual_mode_14_plot_RECO_10_30 = new TH1F("Relative Residual mode 14 10<RECO<30","Relative Residual mode 14 10<RECO<30", 700, -2, 5);
    TH1F *Relative_Residual_mode_13_plot_RECO_10_30 = new TH1F("Relative Residual mode 13 10<RECO<30","Relative Residual mode 13 10<RECO<30", 700, -2, 5);
    TH1F *Relative_Residual_mode_12_plot_RECO_10_30 = new TH1F("Relative Residual mode 12 10<RECO<30","Relative Residual mode 12 10<RECO<30", 700, -2, 5);
    TH1F *Relative_Residual_mode_7_plot_RECO_10_30 = new TH1F("Relative Residual mode 7 10<RECO<30","Relative Residual mode 7 10<RECO<30", 700, -2, 5);
    
    //RECO pT 10-30 GeV
    TH1F *Relative_Residual_mode_15_plot_RECO_30_100 = new TH1F("Relative Residual mode 15 30<RECO<100","Relative Residual mode 15 30<RECO<100", 700, -2, 5);
    TH1F *Relative_Residual_mode_14_plot_RECO_30_100 = new TH1F("Relative Residual mode 14 30<RECO<100","Relative Residual mode 14 30<RECO<100", 700, -2, 5);
    TH1F *Relative_Residual_mode_13_plot_RECO_30_100 = new TH1F("Relative Residual mode 13 30<RECO<100","Relative Residual mode 13 30<RECO<100", 700, -2, 5);
    TH1F *Relative_Residual_mode_12_plot_RECO_30_100 = new TH1F("Relative Residual mode 12 30<RECO<100","Relative Residual mode 12 30<RECO<100", 700, -2, 5);
    TH1F *Relative_Residual_mode_7_plot_RECO_30_100 = new TH1F("Relative Residual mode 7 30<RECO<100","Relative Residual mode 7 30<RECO<100", 700, -2, 5);
    
    //RECO pT >100 GeV
    TH1F *Relative_Residual_mode_15_plot_RECO_100_MAX = new TH1F("Relative Residual mode 15 100<RECO<MAX","Relative Residual mode 15 100<RECO<MAX", 700, -2, 5);
    TH1F *Relative_Residual_mode_14_plot_RECO_100_MAX = new TH1F("Relative Residual mode 14 100<RECO<MAX","Relative Residual mode 14 100<RECO<MAX", 700, -2, 5);
    TH1F *Relative_Residual_mode_13_plot_RECO_100_MAX = new TH1F("Relative Residual mode 13 100<RECO<MAX","Relative Residual mode 13 100<RECO<MAX", 700, -2, 5);
    TH1F *Relative_Residual_mode_12_plot_RECO_100_MAX = new TH1F("Relative Residual mode 12 100<RECO<MAX","Relative Residual mode 12 100<RECO<MAX", 700, -2, 5);
    TH1F *Relative_Residual_mode_7_plot_RECO_100_MAX = new TH1F("Relative Residual mode 7 100<RECO<MAX","Relative Residual mode 7 100<RECO<MAX", 700, -2, 5);
    //=******************************************************************************************************************************************************
    
    //=******************************
    //=make histograms in eta ranges*
    //=******************************
    
    //broad range
    TH1F *Relative_Residual_12_eta_155_plot = new TH1F("Relative Residual 1.2<eta<1.55","Relative Residual 1.2<eta<1.55", 700, -2, 5);
    TH1F *Relative_Residual_155_eta_185_plot = new TH1F("Relative Residual 1.55<eta<1.85","Relative Residual 1.55<eta<1.85", 700, -2, 5);
    TH1F *Relative_Residual_185_eta_24_plot = new TH1F("Relative Residual 1.85<eta<2.4","Relative Residual 1.85<eta<2.4", 700, -2, 5);
    
    //broad range
    TH1F *Delta_R_12_eta_155_plot = new TH1F("dR 1.2<eta<1.55","dR 1.2<eta< 1.55", 100, 0, 0.3);//JUST FOR FUN
    TH1F *Delta_R_155_eta_185_plot = new TH1F("dR 1.55<eta<1.85","dR 1.55<eta<1.85", 100, 0, 0.3);
    TH1F *Delta_R_185_eta_24_plot = new TH1F("dR 1.85<eta<2.4","dR 1.85<eta<2.4", 100, 0, 0.3);
    
    //RECO pT 0-10 GeV
    TH1F *Relative_Residual_12_eta_155_plot_RECO_0_10 = new TH1F("Relative Residual 1.2<eta<1.55 0<RECO<10","Relative Residual 1.2<eta<1.55 0<RECO<10", 700, -2, 5);
    TH1F *Relative_Residual_155_eta_185_plot_RECO_0_10 = new TH1F("Relative Residual 1.55<eta<1.85 0<RECO<10","Relative Residual 1.55<eta<1.85 0<RECO<10", 700, -2, 5);
    TH1F *Relative_Residual_185_eta_24_plot_RECO_0_10 = new TH1F("Relative Residual 1.85<eta<2.4 0<RECO<10","Relative Residual 1.85<eta<2.4 0<RECO<10", 700, -2, 5);
    
    //RECO pT 10-30 GeV
    TH1F *Relative_Residual_12_eta_155_plot_RECO_10_30 = new TH1F("Relative Residual 1.2<eta<1.55 10<RECO<30","Relative Residual 1.2<eta<1.55 10<RECO<30", 700, -2, 5);
    TH1F *Relative_Residual_155_eta_185_plot_RECO_10_30 = new TH1F("Relative Residual 1.55<eta<1.85 10<RECO<30","Relative Residual 1.55<eta<1.85 10<RECO<30", 700, -2, 5);
    TH1F *Relative_Residual_185_eta_24_plot_RECO_10_30 = new TH1F("Relative Residual 1.85<eta<2.4 10<RECO<30","Relative Residual 1.88<eta<2.4 10<RECO<30", 700, -2, 5);
    
    //RECO pT 30-100 GeV
    TH1F *Relative_Residual_12_eta_155_plot_RECO_30_100 = new TH1F("Relative Residual 1.2<eta<1.55 30<RECO<100","Relative Residual 1.2<eta<1.55 30<RECO<100", 700, -2, 5);
    TH1F *Relative_Residual_155_eta_185_plot_RECO_30_100 = new TH1F("Relative Residual 1.55<eta<1.85 30<RECO<100","Relative Residual 1.55<eta<1.85 30<RECO<100", 700, -2, 5);
    TH1F *Relative_Residual_185_eta_24_plot_RECO_30_100 = new TH1F("Relative Residual 1.85<eta<2.4 30<RECO<100","Relative Residual 1.85<eta<2.4 30<RECO<100", 700, -2, 5);
    
    //RECO pT > 100 GeV
    TH1F *Relative_Residual_12_eta_155_plot_RECO_100_MAX = new TH1F("Relative Residual 1.2<eta<1.55 100<RECO<MAX","Relative Residual 1.2<eta<1.55 100<RECO<MAX", 700, -2, 5);
    TH1F *Relative_Residual_155_eta_185_plot_RECO_100_MAX = new TH1F("Relative Residual 1.55<eta<1.85 100<RECO<MAX","Relative Residual 1.55<eta<1.85 100<RECO<MAX", 700, -2, 5);
    TH1F *Relative_Residual_185_eta_24_plot_RECO_100_MAX = new TH1F("Relative Residual 1.85<eta<2.4 100<RECO<MAX","Relative Residual 1.85<eta<2.4 100<RECO<MAX", 700, -2, 5);
    //=*************************************************************************************************
    
    //=*********************
    //=unmatched histograms*
    //=*********************
    TH1F *Unmatched_EMTF_pT_plot = new TH1F("Unmatched EMTF pT","Unmatched EMTF pT", 100, 0, 305);
    TH1F *Unmatched_RECO_pT_plot = new TH1F("Unmatched RECO pT","Unmatched RECO pT", 100, 0, 1480);
    
    Long64_t all_emtf_tracks=0;//all trks = matched trks+ unmatched trks
    Long64_t all_reco_tracks=0;
    Long64_t all_well_matched_tracks=0;
    Long64_t all_unwell_matched_emtf_tracks=0;
    Long64_t all_unwell_matched_reco_tracks=0;
    
    Long64_t numEvents = myTree->GetEntries();//read the number of entries in myTree
    cout<<"-----------------------"<<endl;
    cout<<numEvents<<" events to process....."<<endl;
    
    for(Long64_t iEntry = 0; iEntry <numEvents; iEntry++){
        
        myTree->GetEntry(iEntry);//load the ientry-th event
        
        //define vectors to pointers in each event, no need to load pointer to fill histograms, which is quite slow?
        
        vector<float> recoPt= *RecoPt;//RECO Pt
        vector<float> recoEta= *RecoEta;//RECO eta
        vector<float> recoPhi= *RecoPhi;//RECO Phi
        vector<float> trkPt= *TrkPt;//EMTF Pt
        vector<float> trkEta= *TrkEta;//EMTF eta
        vector<float> trkPhi= *TrkPhi;//EMTF phi
        vector<int> trkMode= *TrkMode;//EMTF mode
        vector<int> trkBx= *TrkBx;//EMTF bx
        
        
        //cout<<"-----------------------"<<endl;
        //cout<<"Processing "<<"event #"<<iEntry<<"......"<<endl;
         
        
        all_emtf_tracks+=TrkPt->size();
        all_reco_tracks+=RecoPt->size();
        
        
        //match EMTF with RECO in one event
        //=***************ENSURE this is the RIGHT condition***************
        //=if either trks number is 0, no need to match, all unmatched    *
        //=****************************************************************
        if(TrkPt->size()!=0 || RecoPt->size()!=0){
            
            int trk_flag[100]={0};//mark the matched EMTF trks
            int reco_flag[100]={0};//we need to mark the matched RECO trks. Is 100 a good upper limit for reco tracks in one event?
            int matched_tracks=0;//matched traks in one event
            int well_matched_tracks=0;//well matched trks in one event
            int unwell_matched_reco_tracks=0;//unwell matched reco trks in one event
            int unwell_matched_emtf_tracks=0;//unwell matched emtf trks in one event
            
            //when either trks all matched, stop loop matching in one event
            while (matched_tracks != TrkPt->size() && matched_tracks != RecoPt->size()) {
                //cout<<"-------------------------"<<endl;
                //cout<<"Start "<<matched_tracks+1<<" matching loop(s)..."<<endl;
                Double_t min_dR=1000;//initialize min dR, a big number
                int min_dR_itrk=100;//mark the minimum dR indices of tracks
                int min_dR_jreco=100;
                
                //loop through EMTF tracks;
                for(int itrk=0;itrk<TrkPt->size();itrk++){
                    
                    //loop through RECO tracks;
                    for(int jreco=0;jreco<RecoPt->size();jreco++){
                        
                        //jump over already matched trks;
                        if(trk_flag[itrk]==0 && reco_flag[jreco]==0){
                            
                            //some definition for convenience;only consider absolute eta
                            Double_t Delta_Eta = fabs(trkEta[itrk]) - fabs(recoEta[jreco]);
                            Double_t Delta_Phi = trkPhi[itrk] - recoPhi[jreco];
                            Double_t Delta_R = sqrt( Delta_Eta * Delta_Eta + Delta_Phi * Delta_Phi );
                            
                            //**************
                            //Match process*
                            //**************
                            //find the minimum in all unmatched reco and emtf trks
                            if(min_dR > Delta_R){
                                
                                min_dR = Delta_R;
                                min_dR_itrk = itrk;
                                min_dR_jreco = jreco;
                                
                            }//end if
                            
                        }//end if
                        
                    }//end for jreco
                    
                }//end for itrk
                
                
                //one loop over, set loop condition
                matched_tracks++;
                trk_flag[min_dR_itrk]=1;
                reco_flag[min_dR_jreco]=1;
                
                //apply dR cut for well matched trks, also require EMTF track at BX=0
                if(min_dR < 0.25 && trkBx[min_dR_itrk]==0){
                    //count well matched trks
                    well_matched_tracks++;
                    trk_flag[min_dR_itrk]=2;//re-mark to 2 if passes the strict select
                    reco_flag[min_dR_jreco]=2;//mark with 0 or 1 is not selected
                    //cout<<"************************************"<<endl;
                    //cout<<"Well matched trks: dR = "<< min_dR <<", trkBx= "<<trkBx[min_dR_itrk]<<", trk # = "<< min_dR_itrk <<", trkMode #="<<trkMode[min_dR_itrk]<<endl;
                    //cout<<"Reco # = "<< min_dR_jreco <<", recoEta= "<<recoEta[min_dR_jreco]<</*", abs(recoEta)= "<<fabs(recoEta[min_dR_jreco])<<*/endl;
                    /*
                    if(fabs(recoEta[min_dR_jreco])>=1.2 && fabs(recoEta[min_dR_jreco])<1.55){
                        cout<<"1.2=<Eta<1.55"<<endl;
                    }
                    else if(fabs(recoEta[min_dR_jreco])>=1.85 && fabs(recoEta[min_dR_jreco])<2.4){
                        cout<<"1.85<=Eta<2.4"<<endl;
                    }
                    else if(fabs(recoEta[min_dR_jreco])>=1.55 && fabs(recoEta[min_dR_jreco])<1.85){
                        cout<<"1.55=<Eta<1.85"<<endl;
                    }
                    else{
                        cout<<"Eta not in sepecified regions."<<endl;
                    }*/
                    //cout<<"************************************"<<endl;
                    //cout<<"The size of trkPt is "<<TrkPt->size()<<endl;
                    //cout<<"The size of recoPt is "<<RecoPt->size()<<endl;//make no sense when trkPt->size() < recoPt->size()
                    
                    
                    //this is the residual curvature, not residual pT, is is EMTF trk curvature - RECO trk curvature
                    Double_t Residual = 1/recoPt[min_dR_jreco]-1/trkPt[min_dR_itrk];
                    Double_t Relative_Residual = Residual/(1/trkPt[min_dR_itrk]);//this is actually emtf/reco - 1, as suggested by andrew;
                    
                    //=***************************************
                    //=histograms for all well matched tracks*
                    //=***************************************
                    Relative_Residual_all_modes->Fill(Relative_Residual);
                    Delta_R_all_modes->Fill(min_dR);
                    //cout<<"Relative Residual and Delta R for all modes filled."<<endl;
                    
                    //=******************************************
                    //=histograms in Broad and narrow eta ranges*
                    //=******************************************
                    
                    //broad: positive eta
                    //1.2-1.55
                    if((recoEta[min_dR_jreco]>=1.2 && recoEta[min_dR_jreco]<1.55) || (recoEta[min_dR_jreco]> -1.55 && recoEta[min_dR_jreco]<= -1.2)){
                        
                        Relative_Residual_12_eta_155_plot->Fill(Relative_Residual);
                        
                        Delta_R_12_eta_155_plot->Fill(min_dR);
                        //cout<<"1D: Delta R in 1.2=<Eta<1.55 filled!"<<endl;
                        
                        //separate plots for low and high RECO pT
                        if(recoPt[min_dR_jreco]>0 && recoPt[min_dR_jreco]<=10){
                            Relative_Residual_12_eta_155_plot_RECO_0_10->Fill(Relative_Residual);
                        }
                        else if(recoPt[min_dR_jreco]>10 && recoPt[min_dR_jreco]<=30){
                            Relative_Residual_12_eta_155_plot_RECO_10_30->Fill(Relative_Residual);
                        }
                        else if(recoPt[min_dR_jreco]>30 && recoPt[min_dR_jreco]<=100){
                            Relative_Residual_12_eta_155_plot_RECO_30_100->Fill(Relative_Residual);
                        }
                        else{
                            Relative_Residual_12_eta_155_plot_RECO_100_MAX->Fill(Relative_Residual);
                        }
                        
                    }
                    //1.85-2.4
                    else if((recoEta[min_dR_jreco]>=1.85 && recoEta[min_dR_jreco]<2.4) || (recoEta[min_dR_jreco]> -2.4 && recoEta[min_dR_jreco]<= -1.85)){
                        
                        Relative_Residual_185_eta_24_plot->Fill(Relative_Residual);
                        
                        Delta_R_185_eta_24_plot->Fill(min_dR);
                        //cout<<"1D: Delta R in 1.85=<Eta<2.4 filled!"<<endl;
                        
                        if(recoPt[min_dR_jreco]>0 && recoPt[min_dR_jreco]<=10){
                            Relative_Residual_185_eta_24_plot_RECO_0_10->Fill(Relative_Residual);
                        }
                        else if(recoPt[min_dR_jreco]>10 && recoPt[min_dR_jreco]<=30){
                            Relative_Residual_185_eta_24_plot_RECO_10_30->Fill(Relative_Residual);
                        }
                        else if(recoPt[min_dR_jreco]>30 && recoPt[min_dR_jreco]<=100){
                            Relative_Residual_185_eta_24_plot_RECO_30_100->Fill(Relative_Residual);
                        }
                        else{
                            Relative_Residual_185_eta_24_plot_RECO_100_MAX->Fill(Relative_Residual);
                        }
                    }
                    //1.55-1.85
                    else if((recoEta[min_dR_jreco]>=1.55 && recoEta[min_dR_jreco]<1.85) || (recoEta[min_dR_jreco]> -1.85 && recoEta[min_dR_jreco]<= -1.55)){
                        
                        Relative_Residual_155_eta_185_plot->Fill(Relative_Residual);
                        
                        Delta_R_155_eta_185_plot->Fill(min_dR);
                        //cout<<"1D: Delta R in 1.55=<Eta<1.85 filled!"<<endl;
                        
                        if(recoPt[min_dR_jreco]>0 && recoPt[min_dR_jreco]<=10){
                            Relative_Residual_155_eta_185_plot_RECO_0_10->Fill(Relative_Residual);
                        }
                        else if(recoPt[min_dR_jreco]>10 && recoPt[min_dR_jreco]<=30){
                            Relative_Residual_155_eta_185_plot_RECO_10_30->Fill(Relative_Residual);
                        }
                        else if(recoPt[min_dR_jreco]>30 && recoPt[min_dR_jreco]<=100){
                            Relative_Residual_155_eta_185_plot_RECO_30_100->Fill(Relative_Residual);
                        }
                        else{
                            Relative_Residual_155_eta_185_plot_RECO_100_MAX->Fill(Relative_Residual);
                        }
                    }
                    
                    
                    
                    
                    
                    //=*********************************************************************************
            
                    //=********************
                    //=histograms in modes*
                    //=********************
                    switch (trkMode[min_dR_itrk]) {//15 modes
                            
                        case 15://station 1,2,3,4;
                            
                            Relative_Residual_mode_15_plot->Fill(Relative_Residual);
                            
                            Delta_R_mode_15_plot->Fill(min_dR);
                            //cout<<"1D: Delta R in mode #15 filled!"<<endl;
                            
                            if(recoPt[min_dR_jreco]>0 && recoPt[min_dR_jreco]<=10){
                                Relative_Residual_mode_15_plot_RECO_0_10->Fill(Relative_Residual);
                            }
                            else if(recoPt[min_dR_jreco]>10 && recoPt[min_dR_jreco]<=30){
                                Relative_Residual_mode_15_plot_RECO_10_30->Fill(Relative_Residual);
                            }
                            else if(recoPt[min_dR_jreco]>30 && recoPt[min_dR_jreco]<=100){
                                Relative_Residual_mode_15_plot_RECO_30_100->Fill(Relative_Residual);
                            }
                            else{
                                Relative_Residual_mode_15_plot_RECO_100_MAX->Fill(Relative_Residual);
                            }
                            break;
                            
                        case 14://station 1,2,3;
                            
                            Relative_Residual_mode_14_plot->Fill(Relative_Residual);
                            
                            Delta_R_mode_14_plot->Fill(min_dR);
                            //cout<<"1D: Delta R in mode #14 filled!"<<endl;
                            
                            if(recoPt[min_dR_jreco]>0 && recoPt[min_dR_jreco]<=10){
                                Relative_Residual_mode_14_plot_RECO_0_10->Fill(Relative_Residual);
                            }
                            else if(recoPt[min_dR_jreco]>10 && recoPt[min_dR_jreco]<=30){
                                Relative_Residual_mode_14_plot_RECO_10_30->Fill(Relative_Residual);
                            }
                            else if(recoPt[min_dR_jreco]>30 && recoPt[min_dR_jreco]<=100){
                                Relative_Residual_mode_14_plot_RECO_30_100->Fill(Relative_Residual);
                            }
                            else{
                                Relative_Residual_mode_14_plot_RECO_100_MAX->Fill(Relative_Residual);
                            }
                            break;
                            
                        case 13://station 1,2,4;
                       
                            Relative_Residual_mode_13_plot->Fill(Relative_Residual);
                            
                            Delta_R_mode_13_plot->Fill(min_dR);
                            //cout<<"1D: Delta R in mode #13 filled!"<<endl;
                            
                            if(recoPt[min_dR_jreco]>0 && recoPt[min_dR_jreco]<=10){
                                Relative_Residual_mode_13_plot_RECO_0_10->Fill(Relative_Residual);
                            }
                            else if(recoPt[min_dR_jreco]>10 && recoPt[min_dR_jreco]<=30){
                                Relative_Residual_mode_13_plot_RECO_10_30->Fill(Relative_Residual);
                            }
                            else if(recoPt[min_dR_jreco]>30 && recoPt[min_dR_jreco]<=100){
                                Relative_Residual_mode_13_plot_RECO_30_100->Fill(Relative_Residual);
                            }
                            else{
                                Relative_Residual_mode_13_plot_RECO_100_MAX->Fill(Relative_Residual);
                            }
                            break;
                            
                        case 12://station 1,2;
                            
                            Relative_Residual_mode_12_plot->Fill(Relative_Residual);
                            
                            Delta_R_mode_12_plot->Fill(min_dR);
                            //cout<<"1D: Delta R in mode #12 filled!"<<endl;
                            
                            if(recoPt[min_dR_jreco]>0 && recoPt[min_dR_jreco]<=10){
                                Relative_Residual_mode_12_plot_RECO_0_10->Fill(Relative_Residual);
                            }
                            else if(recoPt[min_dR_jreco]>10 && recoPt[min_dR_jreco]<=30){
                                Relative_Residual_mode_12_plot_RECO_10_30->Fill(Relative_Residual);
                            }
                            else if(recoPt[min_dR_jreco]>30 && recoPt[min_dR_jreco]<=100){
                                Relative_Residual_mode_12_plot_RECO_30_100->Fill(Relative_Residual);
                            }
                            else{
                                Relative_Residual_mode_12_plot_RECO_100_MAX->Fill(Relative_Residual);
                            }
                            break;
                            
                            
                        case 7://station 2,3,4;
                            
                            Relative_Residual_mode_7_plot->Fill(Relative_Residual);
                            
                            Delta_R_mode_7_plot->Fill(min_dR);
                            //cout<<"1D: Delta R in mode #7 filled!"<<endl;
                            
                            if(recoPt[min_dR_jreco]>0 && recoPt[min_dR_jreco]<=10){
                                Relative_Residual_mode_7_plot_RECO_0_10->Fill(Relative_Residual);
                            }
                            else if(recoPt[min_dR_jreco]>10 && recoPt[min_dR_jreco]<=30){
                                Relative_Residual_mode_7_plot_RECO_10_30->Fill(Relative_Residual);
                            }
                            else if(recoPt[min_dR_jreco]>30 && recoPt[min_dR_jreco]<=100){
                                Relative_Residual_mode_7_plot_RECO_30_100->Fill(Relative_Residual);
                            }
                            else{
                                Relative_Residual_mode_7_plot_RECO_100_MAX->Fill(Relative_Residual);
                            }
                            break;
                            
                        default:
                            break;
                    }//end switch
                    
                }//end if dR cut in one event
    
            }//end while
            
            all_well_matched_tracks+=well_matched_tracks;
            //cout<<"Tracks matching in this event finished."<<endl;
            //cout<<matched_tracks<<" matched track(s)!"<<endl;
            //cout<<well_matched_tracks<<" well matched track(s)!"<<endl;
            
            //loop to fill un-well-matched trks
            for(int i=0;i<RecoPt->size();i++){
                
                if(reco_flag[i]!=2){
                    
                    unwell_matched_reco_tracks++;
                    
                    Unmatched_RECO_pT_plot->Fill(recoPt[i]);
                    
                    //cout<<"Unmatched RECO pT #"<<i<<" filled."<<endl;
                }//end if
                
            }//end for i
            all_unwell_matched_reco_tracks+=unwell_matched_reco_tracks;
            
            for(int i=0;i<TrkPt->size();i++){
                
                if(trk_flag[i]!=2){
                    
                    unwell_matched_emtf_tracks++;
                    
                    Unmatched_EMTF_pT_plot->Fill(trkPt[i]);
                    
                    //cout<<"Unmatched EMTF pT #"<<i<<" filled."<<endl;
                }//end if
                
            }//end for i
            all_unwell_matched_emtf_tracks+=unwell_matched_emtf_tracks;
            
           // cout<<unwell_matched_reco_tracks<<" unwell matched RECO track(s)!"<<endl;
           // cout<<unwell_matched_emtf_tracks<<" unwell matched EMTF track(s)!"<<endl;
            
        }// end if for match in one event

    }//END ALL EVENTS
    
    
    cout<<"====================================="<<endl;
    cout<<"Loop on all specified events is over!"<<endl;
    
    cout<<"==============="<<endl;
    cout<<"****Summary****"<<endl;
    cout<<numEvents<<" total events!"<<endl;
    
    cout<<"---------------"<<endl;
    cout<<all_emtf_tracks<<" total EMTF tracks!"<<endl;
    cout<<all_reco_tracks<<" total RECO tracks!"<<endl;
    
    cout<<"---------------"<<endl;

    cout<<all_well_matched_tracks<<" total well matched tracks!"<<endl;
    cout<<"---------------"<<endl;
    
    cout<<all_unwell_matched_emtf_tracks<<" total unwell-matched EMTF tracks!"<<endl;
    cout<<all_unwell_matched_reco_tracks<<" total unwell-matched RECO tracks!"<<endl;
    
    
    int missing_emtf_tracks=0;
    int missing_reco_tracks=0;
    
    missing_emtf_tracks=all_emtf_tracks - all_well_matched_tracks - all_unwell_matched_emtf_tracks;
    missing_reco_tracks=all_reco_tracks - all_well_matched_tracks - all_unwell_matched_reco_tracks;
    
    if(missing_emtf_tracks==0){
        cout<<"All EMTF tracks included!"<<endl;
    }
    else{
        cout<<"Missing EMTF tracks: "<<missing_emtf_tracks<<"!"<<endl;
    }
    
    if(missing_reco_tracks==0){
        cout<<"All RECO tracks included!"<<endl;
    }
    else{
        cout<<"Missing RECO tracks: "<<missing_reco_tracks<<"!"<<endl;
    }
    
    
    cout<<"Create root file and writing plots......"<<endl;
    
    //=********************************************************************************************
    //write into root file
    TFile myPlot("/afs/cern.ch/work/w/wshi/public/EMTFpTResolution/EMTFpTResolution_274440_443_v8.root","RECREATE");
    
    Relative_Residual_all_modes->Write();
    Delta_R_all_modes->Write();
    
    //=******************************
    //=*Residual stack plot in modes*
    //=******************************
    
    Relative_Residual_mode_15_plot->Write();
    Relative_Residual_mode_14_plot->Write();
    Relative_Residual_mode_13_plot->Write();
    Relative_Residual_mode_12_plot->Write();
    Relative_Residual_mode_7_plot->Write();
    
    //RECO pT 0-10 GeV
    Relative_Residual_mode_15_plot_RECO_0_10->Write();
    Relative_Residual_mode_14_plot_RECO_0_10->Write();
    Relative_Residual_mode_13_plot_RECO_0_10->Write();
    Relative_Residual_mode_12_plot_RECO_0_10->Write();
    Relative_Residual_mode_7_plot_RECO_0_10->Write();
    
    //RECO pT 10-30 GeV
    Relative_Residual_mode_15_plot_RECO_10_30->Write();
    Relative_Residual_mode_14_plot_RECO_10_30->Write();
    Relative_Residual_mode_13_plot_RECO_10_30->Write();
    Relative_Residual_mode_12_plot_RECO_10_30->Write();
    Relative_Residual_mode_7_plot_RECO_10_30->Write();
    
    //RECO pT 10-30 GeV
    Relative_Residual_mode_15_plot_RECO_30_100->Write();
    Relative_Residual_mode_14_plot_RECO_30_100->Write();
    Relative_Residual_mode_13_plot_RECO_30_100->Write();
    Relative_Residual_mode_12_plot_RECO_30_100->Write();
    Relative_Residual_mode_7_plot_RECO_30_100->Write();
    
    //RECO pT >100 GeV
    Relative_Residual_mode_15_plot_RECO_100_MAX->Write();
    Relative_Residual_mode_14_plot_RECO_100_MAX->Write();
    Relative_Residual_mode_13_plot_RECO_100_MAX->Write();
    Relative_Residual_mode_12_plot_RECO_100_MAX->Write();
    Relative_Residual_mode_7_plot_RECO_100_MAX->Write();
    //=***********************************************************************************************************
    
    //=******************************
    //=*Delta R stack plots in modes*
    //=******************************
    
    Delta_R_mode_15_plot->Write();
    Delta_R_mode_14_plot->Write();
    Delta_R_mode_13_plot->Write();
    Delta_R_mode_12_plot->Write();
    Delta_R_mode_7_plot->Write();
    
    //=**************************************************************************************************************
    
    //=*******************************************
    //=*Relative Residual stack plot in broad Eta*
    //=*******************************************
    Relative_Residual_12_eta_155_plot->Write();
    Relative_Residual_155_eta_185_plot->Write();
    Relative_Residual_185_eta_24_plot->Write();

    //RECO pT 0-10 GeV
    Relative_Residual_12_eta_155_plot_RECO_0_10->Write();
    Relative_Residual_155_eta_185_plot_RECO_0_10->Write();
    Relative_Residual_185_eta_24_plot_RECO_0_10->Write();
    
    //RECO pT 10-30 GeV
    Relative_Residual_12_eta_155_plot_RECO_10_30->Write();
    Relative_Residual_155_eta_185_plot_RECO_10_30->Write();
    Relative_Residual_185_eta_24_plot_RECO_10_30->Write();
    
    //RECO pT 30-100 GeV
    Relative_Residual_12_eta_155_plot_RECO_30_100->Write();
    Relative_Residual_155_eta_185_plot_RECO_30_100->Write();
    Relative_Residual_185_eta_24_plot_RECO_30_100->Write();
    
    //RECO pT > 100 GeV
    Relative_Residual_12_eta_155_plot_RECO_100_MAX->Write();
    Relative_Residual_155_eta_185_plot_RECO_100_MAX->Write();
    Relative_Residual_185_eta_24_plot_RECO_100_MAX->Write();
    //=**************************************************************************************************************
    
    //=*****************************************
    //=*Matched Delta R stack plot in broad Eta*
    //=*****************************************
    //All in log scale for all axis below
    Delta_R_12_eta_155_plot->Write();
    Delta_R_155_eta_185_plot->Write();
    Delta_R_185_eta_24_plot->Write();
    
    //=***********************************************************************************************************
    
    //=*******************************
    //=*Unmatched EMTF and RECO plots*
    //=*******************************
    Unmatched_EMTF_pT_plot->Write();
    Unmatched_RECO_pT_plot->Write();
    
    //=************************************************************************************************************

    myPlot.Close();
    cout<<"---------------"<<endl;
    cout<<"All plots saved in EMTFpTResolution_274440_443_v8.root file!"<<endl;
    cout<<"Saved path: /afs/cern.ch/work/w/wshi/public/EMTFpTResolution/"<<endl;
    cout<<"****************"<<endl;
}
