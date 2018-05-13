#include "TFile.h"
#include "TSystem.h"
#include "TChain.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH2.h"
#include "TH1.h"
#include "TCanvas.h"

void PlotOverlay() {
  TFile *file0 = TFile::Open("2017_ModesRateEff_eta_1.25_2.40.root");//1
  TH1F *HRateMuOpenInc2018dTh8Clone = (TH1F*)HRateMuOpenInc->Clone("HRateMuOpenInc2018dTh8Clone");
  TFile *file1 = TFile::Open("2018_dTh4_ModesRateEff_eta_1.25_2.40.root");//2
  TFile *file2 = TFile::Open("2018_dTh6_ModesRateEff_eta_1.25_2.40.root");//3
  TFile *file3 = TFile::Open("2018_dTh8_ModesRateEff_eta_1.25_2.40.root");//4
  TFile *file4 = TFile::Open("2017_dBX_ModesRateEff_eta_1.25_2.40.root");//6
	
  //write to output file
  outFile = "Overlay.root";
  TFile myPlot(outFile,"RECREATE");
        
 
  myPlot.Close();
  
} // End function: void 
