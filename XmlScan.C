#include <iostream>
#include <iomanip>
#include <fstream>
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
        else{
                Cluster = "/home/ws13/TMVA/TMVA/";//bonner
        }
        
        for(int i=0;i<1;i++){
                float boostWeight=-1;
                TString fileName = Cluster + Form("%d", i) + ".xml";//i: 0-399
                TString line;
                int getlinecount = 1;
                std::ifstream f( fileName );
                if (!f.good( )) { // file not found --> Error
                        std::cout << "*** ERROR: Weight file: " << fileName << " does not exist" << std::endl;
                        return;
                }
                //Reading line by line
                while(getline(f, line)) {
                        cout << "data line " << getlinecount << ": " << line << endl;
                        getlinecount++;
                }
       
                f.close();
                
        }//loop over all xmls
        
}
