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
        
        for(int i=0;i<1;i++){
                TString fileName = Cluster + Form("%d", i) + ".xml";//i: 0-399
                string line;
                int getlinecount = 1;
                std::ifstream f( fileName );
                if (!f.good( )) { // file not found --> Error
                        std::cout << "*** ERROR: Weight file: " << fileName << " does not exist" << std::endl;
                        return;
                }
                //Reading line by line
                while(!f.eof()) {
                        getline(f,line);
                        cout << "data line " << getlinecount << ": " << line << endl;
                        const char *myline = line.c_str();
                        const char *findWeightStart;
                        const char *findWeightEnd;
                        char *Weight;
                        const char *findVarStart;
                        const char *findVarEnd;
                        char *Var;
                        const char *findCutStart;
                        const char *findCutEnd;
                        char *Cut;
                        
                        findWeightStart = strstr(myline, "boostWeight=\"");
                        findWeightEnd = strstr(myline, "\" itree");
                        if(findWeightStart && findWeightEnd){
                                std::string s(findWeightStart);
                                s.copy(Weight, strlen(findWeightStart)-strlen(findWeightEnd)-13, 13 );
                                puts (Weight);
                        }    
                        
                        findVarStart = strstr(myline, "IVar=\"");
                        findVarEnd = strstr(myline, "\" Cut");
                        if(findVarStart && findVarEnd){
                                std::string s(findVarStart);
                                s.copy(Var, strlen(findVarStart)-strlen(findVarEnd)-6, 6 );
                                puts (Var);
                        }
                        
                        findCutStart = strstr(myline, "Cut=\"");
                        findCutEnd = strstr(myline, "\" cType");
                        if(findCutStart && findCutEnd){
                                std::string s(findCutStart);
                                s.copy(Cut, strlen(findCutStart)-strlen(findCutEnd)-5, 5 );
                                puts (Cut);
                        }
                        
                        getlinecount++;
                }
       
                f.close();
                
        }//loop over all xmls
        
}
