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
#include "TGraphErrors.h"
#include "TMultiGraph.h"
#include "TLegend.h"

//=****************************************************************
//=This macro compares EMTF and CSCTF track build ability by      *
//=requiring EMTF track and CSCTF track matched to the same       *
//=RECO muon in one event                                         *
//=****************************************************************

void TrackBuild()
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
                            Double_t Delta_Eta_emtf_reco = fabs(trkEta[itrk]) - fabs(recoEta[jreco]);
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
                leg_trk_flag[min_dR_kleg_trk]= -6;//simple mark
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
            
            //==============
            //compare notes
            //==============
            int selected_jreco=-1;
            for(int itrk=0;itrk<TrkPt->size();itrk++){
            
                for(int kleg_trk=0;kleg_trk<Leg_TrkPt->size();kleg_trk++){
                    
                    //===============
                    //=select tracks
                    //===============
                    if(trk_flag[itrk]==leg_trk_flag[kleg_trk]){
                        
                        event_flag=1;//mark selected event
                        all_selected_tracks++;//take down all selected tracks, same # for RECO, EMTF and CSCTF
                        selected_jreco=trk_flag[itrk];//get the index of selected reco track
                        //cout<<"EMTF track #"<<itrk<<": mode= "<<trkMode[itrk]<<endl;
                        //cout<<"CSCTF track #"<<kleg_trk<<": mode= "<<leg_trkMode[kleg_trk]<<endl;
                        //cout<<"RECO track #"<<selected_jreco<<": recoEta= "<<recoEta[selected_jreco]<<endl;
                        //cout<<"---------------------------"<<endl;
                        
                        //============
                        //=fill plots
                        //============
                        EMTF_modes_CSCTF_modes->Fill(trkMode[itrk],leg_trkMode[kleg_trk]);
                        
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
                        
                        //==========================================================================
                        //=fraction of RECO muons matched to EMTF/CSCTF modes 11:15 in different eta
                        //==========================================================================
                        //we have three eta region, so just use array of size 3
                        //0 for 1.2-1.55; 1 for 1.55-1.85; 2 for 1.85-2.4
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
                        
                        
                    }//end if for selected tracks
                    
                }//end for loop over csctf
                
            }//end for loop over emtf
            //end compare notes
            
            
        }//end if all size!=0
        //end processing one event
        
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
    
    //=================================================================
    //=Fraction of RECO muons matched to each modes track vs. RECO eta=
    //=================================================================
    //color runs from 2,3,4,6,7 to avoid yellow
    
    //cout<<"====================================="<<endl;
    //cout<<"Creating file and making plots..."<<endl;
    
    TFile myPlot("/afs/cern.ch/work/w/wshi/public/EMTFpTResolution/TrackBuild_274440_443.root","RECREATE");
    //TFile myPlot("~/Documents/EMTFpTResolution/TrackBuild_274440_443.root","RECREATE");
    
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
    cout<<"Save path:/afs/cern.ch/work/w/wshi/public/EMTFpTResolution/TrackBuild_274440_443.root"<<endl;
    cout<<"****************"<<endl;
}

