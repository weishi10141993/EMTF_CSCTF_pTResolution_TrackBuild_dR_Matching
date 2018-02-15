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
        }
        
        TH1F *Var_n = new TH1F("VarId=-1", "VarId=-1 cuts no weight", 20, -10, 10);//?
        TH1F *Var_0 = new TH1F("VarId=0", "VarId=0 cuts no weight", 14, 0, 14);//theta
        TH1F *Var_1 = new TH1F("VarId=1", "VarId=1 cuts no weight", 2, 0, 2);//St1 ring2
        TH1F *Var_2 = new TH1F("VarId=2", "VarId=2 cuts no weight", 500, 0, 500);//dPhi 12
        TH1F *Var_3 = new TH1F("VarId=3", "VarId=3 cuts no weight", 280, -140, 140);//dPhi 23
        TH1F *Var_4 = new TH1F("VarId=4", "VarId=4 cuts no weight", 280, -140, 140);//dPhi 34
        TH1F *Var_5 = new TH1F("VarId=5", "VarId=5 cuts no weight", 800, -150, 650);//dPhi 13
        TH1F *Var_6 = new TH1F("VarId=6", "VarId=6 cuts no weight", 1000, -300, 700);//dPhi 14
        TH1F *Var_7 = new TH1F("VarId=7", "VarId=7 cuts no weight", 600, -300, 300);//dphi 24
        TH1F *Var_8 = new TH1F("VarId=8", "VarId=8 cuts no weight", 2, 0, 2);//FR1
        TH1F *Var_9 = new TH1F("VarId=9", "VarId=9 cuts no weight", 4, 0, 4);//Bend1
        TH1F *Var_10 = new TH1F("VarId=10", "VarId=10 cuts no weight", 3500, -1000, 2500);//dPhiSum4
        TH1F *Var_11 = new TH1F("VarId=11", "VarId=11 cuts no weight", 2500, 0, 2500);//dPhiSum4A
        TH1F *Var_12 = new TH1F("VarId=12", "VarId=12 cuts no weight", 1200, -600, 600);//dPhiSum3
        TH1F *Var_13 = new TH1F("VarId=13", "VarId=13 cuts no weight", 600, 0, 600);//dPhiSum3A
        TH1F *Var_14 = new TH1F("VarId=14", "VarId=14 cuts no weight", 5, 0, 5);//outStPhi
        TH1F *Var_15 = new TH1F("VarId=15", "VarId=15 cuts no weight", 4, 0, 4);//dTh14
        TH1F *Var_16 = new TH1F("VarId=16", "VarId=16 cuts no weight", 2, 0, 2);//RPC1
        TH1F *Var_17 = new TH1F("VarId=17", "VarId=17 cuts no weight", 2, 0, 2);//RPC2
        TH1F *Var_18 = new TH1F("VarId=18", "VarId=18 cuts no weight", 2, 0, 2);//RPC3
        TH1F *Var_19 = new TH1F("VarId=19", "VarId=19 cuts no weight", 2, 0, 2);//RPC4
        TH1F *Var_20 = new TH1F("VarId=20", "VarId=20 cuts no weight", 50, 0, 1);//inv_GEN_pt_trg
        
        //Add itree weight
        TH1F *WVar_n = new TH1F("WVarId=-1", "VarId=-1 cuts weighted", 20, -10, 10);//?
        TH1F *WVar_0 = new TH1F("WVarId=0", "VarId=0 cuts weighted", 14, 0, 14);//theta
        TH1F *WVar_1 = new TH1F("WVarId=1", "VarId=1 cuts weighted", 2, 0, 2);//St1 ring2
        TH1F *WVar_2 = new TH1F("WVarId=2", "VarId=2 cuts weighted", 500, 0, 500);//dPhi 12
        TH1F *WVar_3 = new TH1F("WVarId=3", "VarId=3 cuts weighted", 280, -140, 140);//dPhi 23
        TH1F *WVar_4 = new TH1F("WVarId=4", "VarId=4 cuts weighted", 280, -140, 140);//dPhi 34
        TH1F *WVar_5 = new TH1F("WVarId=5", "VarId=5 cuts weighted", 800, -150, 650);//dPhi 13
        TH1F *WVar_6 = new TH1F("WVarId=6", "VarId=6 cuts weighted", 1000, -300, 700);//dPhi 14
        TH1F *WVar_7 = new TH1F("WVarId=7", "VarId=7 cuts weighted", 600, -300, 300);//dphi 24
        TH1F *WVar_8 = new TH1F("WVarId=8", "VarId=8 cuts weighted", 2, 0, 2);//FR1
        TH1F *WVar_9 = new TH1F("WVarId=9", "VarId=9 cuts weighted", 4, 0, 4);//Bend1
        TH1F *WVar_10 = new TH1F("WVarId=10", "VarId=10 cuts weighted", 3500, -1000, 2500);//dPhiSum4
        TH1F *WVar_11 = new TH1F("WVarId=11", "VarId=11 cuts weighted", 2500, 0, 2500);//dPhiSum4A
        TH1F *WVar_12 = new TH1F("WVarId=12", "VarId=12 cuts weighted", 1200, -600, 600);//dPhiSum3
        TH1F *WVar_13 = new TH1F("WVarId=13", "VarId=13 cuts weighted", 600, 0, 600);//dPhiSum3A
        TH1F *WVar_14 = new TH1F("WVarId=14", "VarId=14 cuts weighted", 5, 0, 5);//outStPhi
        TH1F *WVar_15 = new TH1F("WVarId=15", "VarId=15 cuts weighted", 4, 0, 4);//dTh14
        TH1F *WVar_16 = new TH1F("WVarId=16", "VarId=16 cuts weighted", 2, 0, 2);//RPC1
        TH1F *WVar_17 = new TH1F("WVarId=17", "VarId=17 cuts weighted", 2, 0, 2);//RPC2
        TH1F *WVar_18 = new TH1F("WVarId=18", "VarId=18 cuts weighted", 2, 0, 2);//RPC3
        TH1F *WVar_19 = new TH1F("WVarId=19", "VarId=19 cuts weighted", 2, 0, 2);//RPC4
        TH1F *WVar_20 = new TH1F("WVarId=20", "VarId=20 cuts weighted", 50, 0, 1);//inv_GEN_pt_trg
        
        for(int i=0;i<1;i++){
                TString fileName = Cluster + Form("%d", i) + ".xml";//i: 0-399
                string line;
                int getlinecount = 1;
                float weight = 1.0;
                std::ifstream f( fileName );
                if (!f.good( )) { // file not found --> Error
                        std::cout << "*** ERROR: Weight file: " << fileName << " does not exist" << std::endl;
                        return;
                }
                else{
                        std::cout << "Entering file: " << fileName << std::endl;
                }
                //Reading line by line
                while(!f.eof()) {
                        getline(f,line);
                        cout << "data line " << getlinecount << ": " << line << endl;
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
                                printf("%f\n",weight);
                        }    
                        
                        findVarStart = strstr(myline, "IVar=\"");
                        findVarEnd = strstr(myline, "\" Cut");
                        if(findVarStart && findVarEnd){
                                std::string s(findVarStart);
                                std::size_t slength = s.copy(Var, strlen(findVarStart)-strlen(findVarEnd)-6, 6 );
                                Var[slength]='\0';
                                varId = atoi(Var);    
                                printf("%d\n",varId);
                        }
                        
                        findCutStart = strstr(myline, "Cut=\"");
                        findCutEnd = strstr(myline, "\" cType");
                        if(findCutStart && findCutEnd){
                                std::string t(findCutStart);
                                std::size_t tlength = t.copy(Cut, strlen(findCutStart)-strlen(findCutEnd)-5, 5 );
                                Cut[tlength]='\0';
                                cut = atof(Cut);
                                printf("%f\n",cut);
                        }
                        
                        getlinecount++;
                }
       
                f.close();
                
        }//loop over all xmls
        
        TString output="/afs/cern.ch/work/w/wshi/public/EMTFpTResolution/XmlScan.root";
        TFile myPlot(output,"RECREATE");
        
}
