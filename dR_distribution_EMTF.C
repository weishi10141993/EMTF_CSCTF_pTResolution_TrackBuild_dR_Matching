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


void dR_distribution_EMTF()
{
    //data file
    TFile* myFile = new TFile("/afs/cern.ch/work/w/wshi/public/EMTFpTResolution/EMTF_NTuple_274440_443_v3.root");
    TTree* myTree = (TTree*) myFile->Get("ntuple/tree");
    
    //define pointers
    vector<float> *RecoPt;//RECO Pt
    vector<float> *RecoEta;//RECO eta
    vector<float> *RecoPhi;//RECO Phi
    vector<float> *TrkPt;//EMTF Pt
    vector<float> *TrkEta;//EMTF eta
    vector<float> *TrkPhi;//EMTF phi
    vector<int> *TrkMode;//EMTF mode
    
    //event info: Pull variables from nTuple: pointer pointing to variables
    
    myTree->SetBranchAddress("recoPt",&RecoPt);//RECO
    myTree->SetBranchAddress("recoEta",&RecoEta);
    myTree->SetBranchAddress("recoPhi",&RecoPhi);
    myTree->SetBranchAddress("trkPt",&TrkPt);//EMTF
    myTree->SetBranchAddress("trkEta",&TrkEta);
    myTree->SetBranchAddress("trkPhi",&TrkPhi);
    myTree->SetBranchAddress("trkMode",&TrkMode);
    
    //create histogram
    TH1F *Matched_Delta_R_plot = new TH1F("Delta R of Matched tracks","Matched trks dR",120,0,1.2);//-->C10, 1.2 is good enough to cover
    TH1F *Unmatched_EMTF_pT_plot = new TH1F("Unmatched EMTF pT","Unmatched EMTF pT", 100, 0, 305);//unmatched for all modes EMTF distribution-->C8
    TH1F *Unmatched_RECO_pT_plot = new TH1F("Unmatched RECO pT","Unmatched RECO pT", 100, 0, 1480);//unmatched for all modes RECO distribution-->C9
    
    Long64_t all_emtf_tracks=0;//all trks = matched trks+ unmatched trks
    Long64_t all_reco_tracks=0;
    Long64_t all_matched_tracks=0;
    Long64_t all_unmatched_emtf_tracks=0;
    Long64_t all_unmatched_reco_tracks=0;
    
    Long64_t numEvents = myTree->GetEntries();//read the number of entries in myTree
    cout<<"----------------------------------"<<endl;
    cout<<numEvents<<" events to process....."<<endl;
    
    //loop over events
    for(Long64_t iEntry = 0; iEntry <numEvents; iEntry++){//replace 100 by numEvents
        
        myTree->GetEntry(iEntry);//load the ientry-th event
        
        //define vectors to pointers in each event, no need to load pointer to fill histograms, which is quite slow
        vector<float> recoPt= *RecoPt;//RECO Pt
        vector<float> recoEta= *RecoEta;//RECO eta
        vector<float> recoPhi= *RecoPhi;//RECO Phi
        vector<float> trkPt= *TrkPt;//EMTF Pt
        vector<float> trkEta= *TrkEta;//EMTF eta
        vector<float> trkPhi= *TrkPhi;//EMTF phi
        vector<int> trkMode= *TrkMode;//EMTF mode
        
        /*
        cout<<"-----------------------"<<endl;
        cout<<"Processing "<<"event #"<<iEntry<<"......"<<endl;
        cout<<"The size of trkPt is "<<TrkPt->size()<<endl;
        cout<<"The size of recoPt is "<<RecoPt->size()<<endl;
        */
        
        all_emtf_tracks+=TrkPt->size();
        all_reco_tracks+=RecoPt->size();
        
        
        //match EMTF with RECO
        //***************ENSURE this is the RIGHT condition***************
        //if either trks number is 0, no need to match, all unmatched    *
        //****************************************************************
        if(TrkPt->size()!=0 || RecoPt->size()!=0){
            
            int trk_flag[100]={0};//mark the matched EMTF trks
            int reco_flag[100]={0};//we need to mark the matched RECO trks. Is 100 a good upper limit for reco tracks in one event?
            int matched_tracks=0;//matched traks in one event
            
            //when either trks all matched, stop loop matching in one event
            while (matched_tracks != TrkPt->size() && matched_tracks != RecoPt->size()) {
                
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
                            
                            //some definition for convenience;
                            //for eta, use abs value, to be consistent with csctf since they have only positive endcap;
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
                
            
            //each loop over, set the flag to 1 for the minimum dR pair of RECO and EMTF trks
            trk_flag[min_dR_itrk]=1;
            reco_flag[min_dR_jreco]=1;
            //cout<<"Current best match: dR = "<< min_dR <<", trk # = "<< min_dR_itrk <<", reco # = "<< min_dR_jreco <<endl;
            Matched_Delta_R_plot->Fill(min_dR);
            //cout<<"Current min dR filled."<<endl;
            matched_tracks++;
                
                
            }//end while
            all_matched_tracks+=matched_tracks;            
            //cout<<"Tracks matching in this event finished."<<endl;
            //cout<<matched_tracks<<"matched track(s)!"<<endl;
            
            //******************************
            //fill unmatched trks histogram*
            //******************************
            if(matched_tracks==TrkPt->size()){
                
                for(int i=0;i<RecoPt->size();i++){
                    
                    if(reco_flag[i]==0){//unmatched reco mark
                        
                        //cout<<"RECO match failure!"<<endl;
                        
                        all_unmatched_reco_tracks++;
                        
                        Unmatched_RECO_pT_plot->Fill(recoPt[i]);
                        
                        //cout<<"Unmatched RECO pT #"<<i<<" filled."<<endl;
                    }//end if
                    
                }//end for i
            
            }//end if
            else{
                
                for(int i=0;i<TrkPt->size();i++){
                    
                    if(trk_flag[i]==0){//unmatched EMTF mark
                        
                        all_unmatched_emtf_tracks++;
                        
                        Unmatched_EMTF_pT_plot->Fill(trkPt[i]);
                        
                        //cout<<"Unmatched EMTF pT #"<<i<<" filled."<<endl;
                    }//end if
                    
                }//end for i
                
            }//end else
            //*****************************************
            
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

    cout<<all_matched_tracks<<" matched tracks!"<<endl;
    cout<<"---------------"<<endl;
    
    cout<<all_unmatched_emtf_tracks<<" unmatched EMTF tracks!"<<endl;
    cout<<all_unmatched_reco_tracks<<" unmatched RECO tracks!"<<endl;
    
    
    int missing_emtf_tracks=0;
    int missing_reco_tracks=0;
    
    missing_emtf_tracks=all_emtf_tracks - all_matched_tracks - all_unmatched_emtf_tracks;
    missing_reco_tracks=all_reco_tracks - all_matched_tracks - all_unmatched_reco_tracks;
    
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
    
    //***********************
    //*plots*
    //***********************
    cout<<"Creating root files......"<<endl;
    TFile myPlot("/afs/cern.ch/work/w/wshi/public/EMTFpTResolution/dR_274440_443_EMTF.root","RECREATE");
    
    //TCanvas *C8=new TCanvas("C8","Unmatched EMTF pT",700,500);
    //TCanvas *C9=new TCanvas("C9","Unmatched RECO pT",700,500);
    //TCanvas *C10=new TCanvas("C10","dR",700,500);
    
    //C8->cd();
    //C8->SetLogy();
    Unmatched_EMTF_pT_plot->GetXaxis()->SetTitle("Unmatched EMTF pT [GeV]");
    Unmatched_EMTF_pT_plot->GetYaxis()->SetTitle("Unmatched EMTF Tracks");
    Unmatched_EMTF_pT_plot->Write();
    //C8->SaveAs("/afs/cern.ch/work/w/wshi/public/EMTFpTResolution/myStudy3/Unmatched_EMTF_pT_plot.png");
    
    //C9->cd();
    //C9->SetLogy();
    Unmatched_RECO_pT_plot->GetXaxis()->SetTitle("Unmatched RECO pT [GeV]");
    Unmatched_RECO_pT_plot->GetYaxis()->SetTitle("Unmatched RECO Tracks");
    Unmatched_RECO_pT_plot->Write();
    //C9->SaveAs("/afs/cern.ch/work/w/wshi/public/EMTFpTResolution/myStudy3/Unmatched_RECO_pT_plot.png");
    
    //C10->cd();
    //C10->SetLogy();
    Matched_Delta_R_plot->GetXaxis()->SetTitle("dR");
    Matched_Delta_R_plot->GetYaxis()->SetTitle("Matched Tracks");
    Matched_Delta_R_plot->Write();
    //C10->SaveAs("/afs/cern.ch/work/w/wshi/public/EMTFpTResolution/myStudy3/Matched_Delta_R_plot.png");

    myPlot.Close();
    cout<<"---------------"<<endl;
    cout<<"All plots saved in /afs/cern.ch/work/w/wshi/public/EMTFpTResolution/dR_274440_443_EMTF.root!"<<endl;
    cout<<"****************"<<endl;
}
