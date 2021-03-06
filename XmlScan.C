#include <iostream>
#include <iomanip>
#include <fstream>
#include "Getline.h"
#include <stdio.h>
#include <string.h>
#include <vector>
#include <stdlib.h>
using namespace std;
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TAttFill.h"
#include "TCanvas.h"
#include "math.h"
#include "TMath.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TPaveStats.h"
#include "TStyle.h"
#include "THStack.h"
//=================================================================
// Plot the cut variables distributions in BDT regression xml file
// Wei Shi @ Feb 14, 2018, CERN
//=================================================================
void XmlScan()
{
        Int_t lxplus=1;
        TString Cluster="";
        
        if(lxplus==1){
                Cluster = "/afs/cern.ch/work/w/wshi/public/EMTFpTResolution/f_MODE_15_invPtTarg_invPtWgt_bitCompr_RPC_BDTG_AWB_Sq.weights/";
                //Cluster= "/afs/cern.ch/work/w/wshi/public/EMTFpTResolution/f_MODE_15_logPtTarg_invPtWgt_noBitCompr.weights/";
        }
        
        TH1F *Var_n = new TH1F("Var_n", "VarId=-1 cuts no weight", 20, -10, 10);//?
        TH1F *Var_0 = new TH1F("Var_0", "VarId=0 cuts no weight", 14, 0, 14);//theta
        TH1F *Var_1 = new TH1F("Var_1", "VarId=1 cuts no weight", 2, 0, 2);//St1 ring2
        TH1F *Var_2 = new TH1F("Var_2", "VarId=2 cuts no weight", 500, 0, 500);//dPhi 12
        TH1F *Var_3 = new TH1F("Var_3", "VarId=3 cuts no weight", 280, -140, 140);//dPhi 23
        TH1F *Var_4 = new TH1F("Var_4", "VarId=4 cuts no weight", 280, -140, 140);//dPhi 34
        TH1F *Var_5 = new TH1F("Var_5", "VarId=5 cuts no weight", 800, -150, 650);//dPhi 13
        TH1F *Var_6 = new TH1F("Var_6", "VarId=6 cuts no weight", 1000, -300, 700);//dPhi 14
        TH1F *Var_7 = new TH1F("Var_7", "VarId=7 cuts no weight", 600, -300, 300);//dphi 24
        TH1F *Var_8 = new TH1F("Var_8", "VarId=8 cuts no weight", 2, 0, 2);//FR1
        TH1F *Var_9 = new TH1F("Var_9", "VarId=9 cuts no weight", 4, 0, 4);//Bend1
        TH1F *Var_10 = new TH1F("Var_10", "VarId=10 cuts no weight", 3500, -1000, 2500);//dPhiSum4
        TH1F *Var_11 = new TH1F("Var_11", "VarId=11 cuts no weight", 2500, 0, 2500);//dPhiSum4A
        TH1F *Var_12 = new TH1F("Var_12", "VarId=12 cuts no weight", 1200, -600, 600);//dPhiSum3
        TH1F *Var_13 = new TH1F("Var_13", "VarId=13 cuts no weight", 600, 0, 600);//dPhiSum3A
        TH1F *Var_14 = new TH1F("Var_14", "VarId=14 cuts no weight", 5, 0, 5);//outStPhi
        TH1F *Var_15 = new TH1F("Var_15", "VarId=15 cuts no weight", 4, 0, 4);//dTh14
        TH1F *Var_16 = new TH1F("Var_16", "VarId=16 cuts no weight", 2, 0, 2);//RPC1
        TH1F *Var_17 = new TH1F("Var_17", "VarId=17 cuts no weight", 2, 0, 2);//RPC2
        TH1F *Var_18 = new TH1F("Var_18", "VarId=18 cuts no weight", 2, 0, 2);//RPC3
        TH1F *Var_19 = new TH1F("Var_19", "VarId=19 cuts no weight", 2, 0, 2);//RPC4
        TH1F *Var_20 = new TH1F("Var_20", "VarId=20 cuts no weight", 50, 0, 1);//inv_GEN_pt_trg
        
        //Add itree weight
        TH1F *WVar_n = new TH1F("WVar_n", "VarId=-1 cuts weighted", 20, -10, 10);//?
        TH1F *WVar_0 = new TH1F("WVar_0", "VarId=0 cuts weighted", 14, 0, 14);//theta
        TH1F *WVar_1 = new TH1F("WVar_1", "VarId=1 cuts weighted", 2, 0, 2);//St1 ring2
        TH1F *WVar_2 = new TH1F("WVar_2", "VarId=2 cuts weighted", 500, 0, 500);//dPhi 12
        TH1F *WVar_3 = new TH1F("WVar_3", "VarId=3 cuts weighted", 280, -140, 140);//dPhi 23
        TH1F *WVar_4 = new TH1F("WVar_4", "VarId=4 cuts weighted", 280, -140, 140);//dPhi 34
        TH1F *WVar_5 = new TH1F("WVar_5", "VarId=5 cuts weighted", 800, -150, 650);//dPhi 13
        TH1F *WVar_6 = new TH1F("WVar_6", "VarId=6 cuts weighted", 1000, -300, 700);//dPhi 14
        TH1F *WVar_7 = new TH1F("WVar_7", "VarId=7 cuts weighted", 600, -300, 300);//dphi 24
        TH1F *WVar_8 = new TH1F("WVar_8", "VarId=8 cuts weighted", 2, 0, 2);//FR1
        TH1F *WVar_9 = new TH1F("WVar_9", "VarId=9 cuts weighted", 4, 0, 4);//Bend1
        TH1F *WVar_10 = new TH1F("WVar_10", "VarId=10 cuts weighted", 3500, -1000, 2500);//dPhiSum4
        TH1F *WVar_11 = new TH1F("WVar_11", "VarId=11 cuts weighted", 2500, 0, 2500);//dPhiSum4A
        TH1F *WVar_12 = new TH1F("WVar_12", "VarId=12 cuts weighted", 1200, -600, 600);//dPhiSum3
        TH1F *WVar_13 = new TH1F("WVar_13", "VarId=13 cuts weighted", 600, 0, 600);//dPhiSum3A
        TH1F *WVar_14 = new TH1F("WVar_14", "VarId=14 cuts weighted", 5, 0, 5);//outStPhi
        TH1F *WVar_15 = new TH1F("WVar_15", "VarId=15 cuts weighted", 4, 0, 4);//dTh14
        TH1F *WVar_16 = new TH1F("WVar_16", "VarId=16 cuts weighted", 2, 0, 2);//RPC1
        TH1F *WVar_17 = new TH1F("WVar_17", "VarId=17 cuts weighted", 2, 0, 2);//RPC2
        TH1F *WVar_18 = new TH1F("WVar_18", "VarId=18 cuts weighted", 2, 0, 2);//RPC3
        TH1F *WVar_19 = new TH1F("WVar_19", "VarId=19 cuts weighted", 2, 0, 2);//RPC4
        TH1F *WVar_20 = new TH1F("WVar_20", "VarId=20 cuts weighted", 50, 0, 1);//inv_GEN_pt_trg
        
        for(int i=0;i<400;i++){
                TString fileName = Cluster + Form("%d", i) + ".xml";//i: 0-399
                string line;
                int getlinecount = 1;
                float weight = 0.0;
                std::ifstream f( fileName );
                if (!f.good( )) { // file not found --> Error
                        std::cout << "*** ERROR: Weight file: " << fileName << " does not exist" << std::endl;
                        return;
                }
                else{
                        std::cout << ">>> Entering file "<<i<<": " << fileName << std::endl;
                }
                //Reading line by line
                while(!f.eof()) {
                        getline(f,line);
                        //cout << "data line " << getlinecount << ": " << line << endl;
                        const char *myline = line.c_str();
                        const char *findWeightStart;
                        const char *findWeightEnd;
                        char Weight[30];
                        const char *findVarStart;
                        const char *findVarEnd;
                        char Var[30];
                        int varId = -2;
                        const char *findCutStart;
                        const char *findCutEnd;
                        char Cut[30];
                        float cut = 0.0;
                        
                        findWeightStart = strstr(myline, "boostWeight=\"");
                        findWeightEnd = strstr(myline, "\" itree");
                        if(findWeightStart && findWeightEnd){
                                std::string r(findWeightStart);
                                std::size_t rlength = r.copy(Weight, strlen(findWeightStart)-strlen(findWeightEnd)-13, 13 );
                                Weight[rlength]='\0';
                                weight = atof(Weight);    
                        }    
                        
                        findVarStart = strstr(myline, "IVar=\"");
                        findVarEnd = strstr(myline, "\" Cut");
                        if(findVarStart && findVarEnd){
                                std::string s(findVarStart);
                                std::size_t slength = s.copy(Var, strlen(findVarStart)-strlen(findVarEnd)-6, 6 );
                                Var[slength]='\0';
                                varId = atoi(Var);        
                        }
                        
                        findCutStart = strstr(myline, "Cut=\"");
                        findCutEnd = strstr(myline, "\" cType");
                        if(findCutStart && findCutEnd){
                                std::string t(findCutStart);
                                std::size_t tlength = t.copy(Cut, strlen(findCutStart)-strlen(findCutEnd)-5, 5 );
                                Cut[tlength]='\0';
                                cut = atof(Cut);
                        }
                        
                        printf("weight: %f, varId: %d, cut: %f\n", weight, varId, cut);
                        
                        //Fill variables
                        switch (varId) {
                            
                        case -1:
                            Var_n->Fill(cut); 
                            WVar_n->Fill(cut,weight);
                            break;
                        case 0:
                            Var_0->Fill(cut); 
                            WVar_0->Fill(cut,weight);
                            break;
                        case 1:
                            Var_1->Fill(cut); 
                            WVar_1->Fill(cut,weight);
                            break;
                        case 2:
                            Var_2->Fill(cut); 
                            WVar_2->Fill(cut,weight);
                            break;
                        case 3:
                            Var_3->Fill(cut); 
                            WVar_3->Fill(cut,weight);
                            break;
                        case 4:
                            Var_4->Fill(cut); 
                            WVar_4->Fill(cut,weight);
                            break;
                        case 5:
                            Var_5->Fill(cut); 
                            WVar_5->Fill(cut,weight);
                            break;
                        case 6:
                            Var_6->Fill(cut); 
                            WVar_6->Fill(cut,weight);
                            break;
                        case 7:
                            Var_7->Fill(cut); 
                            WVar_7->Fill(cut,weight);
                            break;
                        case 8:
                            Var_8->Fill(cut); 
                            WVar_8->Fill(cut,weight);
                            break;
                        case 9:
                            Var_9->Fill(cut); 
                            WVar_9->Fill(cut,weight);
                            break;
                        case 10:
                            Var_10->Fill(cut); 
                            WVar_10->Fill(cut,weight);
                            break;
                        case 11:
                            Var_11->Fill(cut); 
                            WVar_11->Fill(cut,weight);
                            break;
                        case 12:
                            Var_12->Fill(cut); 
                            WVar_12->Fill(cut,weight);
                            break;
                        case 13:
                            Var_13->Fill(cut); 
                            WVar_13->Fill(cut,weight);
                            break;
                        case 14:
                            Var_14->Fill(cut); 
                            WVar_14->Fill(cut,weight);
                            break;
                        case 15:
                            Var_15->Fill(cut); 
                            WVar_15->Fill(cut,weight);
                            break;
                        case 16:
                            Var_16->Fill(cut); 
                            WVar_16->Fill(cut,weight);
                            break;
                        case 17:
                            Var_17->Fill(cut); 
                            WVar_17->Fill(cut,weight);
                            break;
                        case 18:
                            Var_18->Fill(cut); 
                            WVar_18->Fill(cut,weight);
                            break;
                        case 19:
                            Var_19->Fill(cut); 
                            WVar_19->Fill(cut,weight);
                            break;
                        case 20:
                            Var_20->Fill(cut); 
                            WVar_20->Fill(cut,weight);
                            break;
                        default:
                            break;           
                        }//end fill
                        
                        getlinecount++;
                }
       
                f.close();
                
        }//loop over all xmls
        
        TString output="/afs/cern.ch/work/w/wshi/public/EMTFpTResolution/XmlScan.root";
        TFile myPlot(output,"RECREATE");
        
        Var_n->Write();
        WVar_n->Write();
        Var_0->Write();
        WVar_0->Write();
        Var_1->Write();
        WVar_1->Write();
        Var_2->Write();
        WVar_2->Write();
        Var_3->Write();
        WVar_3->Write();
        Var_4->Write();
        WVar_4->Write();
        Var_5->Write();
        WVar_5->Write();
        Var_6->Write();
        WVar_6->Write();
        Var_7->Write();
        WVar_7->Write();
        Var_8->Write();
        WVar_8->Write();
        Var_9->Write();
        WVar_9->Write();
        Var_10->Write();
        WVar_10->Write();
        Var_11->Write();
        WVar_11->Write();
        Var_12->Write();
        WVar_12->Write();
        Var_13->Write();
        WVar_13->Write();
        Var_14->Write();
        WVar_14->Write();
        Var_15->Write();
        WVar_15->Write();
        Var_16->Write();
        WVar_16->Write();
        Var_17->Write();
        WVar_17->Write();
        Var_18->Write();
        WVar_18->Write();
        Var_19->Write();
        WVar_19->Write();
        Var_20->Write();
        WVar_20->Write();
        
}
