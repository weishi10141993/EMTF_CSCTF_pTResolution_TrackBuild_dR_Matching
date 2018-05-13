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
  TH1F *HRateSingleMu2017 = (TH1F*)file0->Get("HRateSingleMu")->Clone("HRateSingleMu2017");
  TH1F *HRateDoubleMuInc2017 = (TH1F*)file0->Get("HRateDoubleMuInc")->Clone("HRateDoubleMuInc2017");
  TH1F *HRateMuOpenInc2017 = (TH1F*)file0->Get("HRateMuOpenInc")->Clone("HRateMuOpenInc2017");
	
  TH1F *MatchBX0SingleMu2017 = (TH1F*)file0->Get("SMRecoPtMatchBX0SingleMu")->Clone("MatchBX0SingleMu2017");
  TH1F *MatchBX0SingleMu2017Log = (TH1F*)file0->Get("SMLogRecoPtMatchBX0SingleMu")->Clone("MatchBX0SingleMu2017Log");
  TH1F *MatchBX0SingleMuPlateau2017 = (TH1F*)file0->Get("SMRecoPtMatchBX0SingleMuPlateau")->Clone("MatchBX0SingleMuPlateau2017");
  TH1F *MatchBX0SingleMuPlateau2017Log = (TH1F*)file0->Get("SMLogRecoPtMatchBX0SingleMuPlateau")->Clone("MatchBX0SingleMuPlateau2017Log");
  TH1F *MatchBX0SingleMuUnique2017 = (TH1F*)file0->Get("SMRecoPtUniqueMatchBX0SingleMu")->Clone("MatchBX0SingleMuUnique2017");
  TH1F *MatchBX0SingleMuUnique2017Log = (TH1F*)file0->Get("SMLogRecoPtUniqueMatchBX0SingleMu")->Clone("MatchBX0SingleMuUnique2017Log");

  TH1F *MatchBX0DoubleMuInc2017 = (TH1F*)file0->Get("SMRecoPtMatchBX0DoubleMuInc")->Clone("MatchBX0DoubleMuInc2017");
  TH1F *MatchBX0DoubleMuInc2017Log = (TH1F*)file0->Get("SMLogRecoPtMatchBX0DoubleMuInc")->Clone("MatchBX0DoubleMuInc2017Log");
  TH1F *MatchBX0DoubleMuIncPlateau2017 = (TH1F*)file0->Get("SMRecoPtMatchBX0DoubleMuIncPlateau")->Clone("MatchBX0DoubleMuIncPlateau2017");
  TH1F *MatchBX0DoubleMuIncPlateau2017Log = (TH1F*)file0->Get("SMLogRecoPtMatchBX0DoubleMuIncPlateau")->Clone("MatchBX0DoubleMuIncPlateau2017Log");
  TH1F *MatchBX0DoubleMuIncUnique2017 = (TH1F*)file0->Get("SMRecoPtUniqueMatchBX0DoubleMuInc")->Clone("MatchBX0DoubleMuIncUnique2017");
  TH1F *MatchBX0DoubleMuIncUnique2017Log = (TH1F*)file0->Get("SMLogRecoPtUniqueMatchBX0DoubleMuInc")->Clone("MatchBX0DoubleMuIncUnique2017Log");
	
  TH1F *MatchBX0MuOpenInc2017 = (TH1F*)file0->Get("SMRecoPtMatchBX0MuOpenInc")->Clone("MatchBX0MuOpenInc2017");
  TH1F *MatchBX0MuOpenInc2017Log = (TH1F*)file0->Get("SMLogRecoPtMatchBX0MuOpenInc")->Clone("MatchBX0MuOpenInc2017Log");
  TH1F *MatchBX0MuOpenIncPlateau2017 = (TH1F*)file0->Get("SMRecoPtMatchBX0MuOpenIncPlateau")->Clone("MatchBX0MuOpenIncPlateau2017");
  TH1F *MatchBX0MuOpenIncPlateau2017Log = (TH1F*)file0->Get("SMLogRecoPtMatchBX0MuOpenIncPlateau")->Clone("MatchBX0MuOpenIncPlateau2017Log");
  TH1F *MatchBX0MuOpenIncUnique2017 = (TH1F*)file0->Get("SMRecoPtUniqueMatchBX0MuOpenInc")->Clone("MatchBX0MuOpenIncUnique2017");
  TH1F *MatchBX0MuOpenIncUnique2017Log = (TH1F*)file0->Get("SMLogRecoPtUniqueMatchBX0MuOpenInc")->Clone("MatchBX0MuOpenIncUnique2017Log");	
	
  TFile *file1 = TFile::Open("2018_dTh4_ModesRateEff_eta_1.25_2.40.root");//2
  TH1F *HRateSingleMu2018_dTh4 = (TH1F*)file1->Get("HRateSingleMu")->Clone("HRateSingleMu2018_dTh4");
  TH1F *HRateDoubleMuInc2018_dTh4 = (TH1F*)file1->Get("HRateDoubleMuInc")->Clone("HRateDoubleMuInc2018_dTh4");
  TH1F *HRateMuOpenInc2018_dTh4 = (TH1F*)file1->Get("HRateMuOpenInc")->Clone("HRateMuOpenInc2018_dTh4");
	
  TH1F *MatchBX0SingleMu2018_dTh4 = (TH1F*)file1->Get("SMRecoPtMatchBX0SingleMu")->Clone("MatchBX0SingleMu2018_dTh4");
  TH1F *MatchBX0SingleMu2018_dTh4Log = (TH1F*)file1->Get("SMLogRecoPtMatchBX0SingleMu")->Clone("MatchBX0SingleMu2018_dTh4Log");
  TH1F *MatchBX0SingleMuPlateau2018_dTh4 = (TH1F*)file1->Get("SMRecoPtMatchBX0SingleMuPlateau")->Clone("MatchBX0SingleMuPlateau2018_dTh4");
  TH1F *MatchBX0SingleMuPlateau2018_dTh4Log = (TH1F*)file1->Get("SMLogRecoPtMatchBX0SingleMuPlateau")->Clone("MatchBX0SingleMuPlateau2018_dTh4Log");
  TH1F *MatchBX0SingleMuUnique2018_dTh4 = (TH1F*)file1->Get("SMRecoPtUniqueMatchBX0SingleMu")->Clone("MatchBX0SingleMuUnique2018_dTh4");
  TH1F *MatchBX0SingleMuUnique2018_dTh4Log = (TH1F*)file1->Get("SMLogRecoPtUniqueMatchBX0SingleMu")->Clone("MatchBX0SingleMuUnique2018_dTh4Log");

  TH1F *MatchBX0DoubleMuInc2018_dTh4 = (TH1F*)file1->Get("SMRecoPtMatchBX0DoubleMuInc")->Clone("MatchBX0DoubleMuInc2018_dTh4");
  TH1F *MatchBX0DoubleMuInc2018_dTh4Log = (TH1F*)file1->Get("SMLogRecoPtMatchBX0DoubleMuInc")->Clone("MatchBX0DoubleMuInc2018_dTh4Log");
  TH1F *MatchBX0DoubleMuIncPlateau2018_dTh4 = (TH1F*)file1->Get("SMRecoPtMatchBX0DoubleMuIncPlateau")->Clone("MatchBX0DoubleMuIncPlateau2018_dTh4");
  TH1F *MatchBX0DoubleMuIncPlateau2018_dTh4Log = (TH1F*)file1->Get("SMLogRecoPtMatchBX0DoubleMuIncPlateau")->Clone("MatchBX0DoubleMuIncPlateau2018_dTh4Log");
  TH1F *MatchBX0DoubleMuIncUnique2018_dTh4 = (TH1F*)file1->Get("SMRecoPtUniqueMatchBX0DoubleMuInc")->Clone("MatchBX0DoubleMuIncUnique2018_dTh4");
  TH1F *MatchBX0DoubleMuIncUnique2018_dTh4Log = (TH1F*)file1->Get("SMLogRecoPtUniqueMatchBX0DoubleMuInc")->Clone("MatchBX0DoubleMuIncUnique2018_dTh4Log");
	
  TH1F *MatchBX0MuOpenInc2018_dTh4 = (TH1F*)file1->Get("SMRecoPtMatchBX0MuOpenInc")->Clone("MatchBX0MuOpenInc2018_dTh4");
  TH1F *MatchBX0MuOpenInc2018_dTh4Log = (TH1F*)file1->Get("SMLogRecoPtMatchBX0MuOpenInc")->Clone("MatchBX0MuOpenInc2018_dTh4Log");
  TH1F *MatchBX0MuOpenIncPlateau2018_dTh4 = (TH1F*)file1->Get("SMRecoPtMatchBX0MuOpenIncPlateau")->Clone("MatchBX0MuOpenIncPlateau2018_dTh4");
  TH1F *MatchBX0MuOpenIncPlateau2018_dTh4Log = (TH1F*)file1->Get("SMLogRecoPtMatchBX0MuOpenIncPlateau")->Clone("MatchBX0MuOpenIncPlateau2018_dTh4Log");
  TH1F *MatchBX0MuOpenIncUnique2018_dTh4 = (TH1F*)file1->Get("SMRecoPtUniqueMatchBX0MuOpenInc")->Clone("MatchBX0MuOpenIncUnique2018_dTh4");
  TH1F *MatchBX0MuOpenIncUnique2018_dTh4Log = (TH1F*)file1->Get("SMLogRecoPtUniqueMatchBX0MuOpenInc")->Clone("MatchBX0MuOpenIncUnique2018_dTh4Log");
	
  TFile *file2 = TFile::Open("2018_dTh6_ModesRateEff_eta_1.25_2.40.root");//3
  TH1F *HRateSingleMu2018_dTh6 = (TH1F*)file2->Get("HRateSingleMu")->Clone("HRateSingleMu2018_dTh6");
  TH1F *HRateDoubleMuInc2018_dTh6 = (TH1F*)file2->Get("HRateDoubleMuInc")->Clone("HRateDoubleMuInc2018_dTh6");
  TH1F *HRateMuOpenInc2018_dTh6 = (TH1F*)file2->Get("HRateMuOpenInc")->Clone("HRateMuOpenInc2018_dTh6");
	
  TH1F *MatchBX0SingleMu2018_dTh6 = (TH1F*)file2->Get("SMRecoPtMatchBX0SingleMu")->Clone("MatchBX0SingleMu2018_dTh6");
  TH1F *MatchBX0SingleMu2018_dTh6Log = (TH1F*)file2->Get("SMLogRecoPtMatchBX0SingleMu")->Clone("MatchBX0SingleMu2018_dTh6Log");
  TH1F *MatchBX0SingleMuPlateau2018_dTh6 = (TH1F*)file2->Get("SMRecoPtMatchBX0SingleMuPlateau")->Clone("MatchBX0SingleMuPlateau2018_dTh6");
  TH1F *MatchBX0SingleMuPlateau2018_dTh6Log = (TH1F*)file2->Get("SMLogRecoPtMatchBX0SingleMuPlateau")->Clone("MatchBX0SingleMuPlateau2018_dTh6Log");
  TH1F *MatchBX0SingleMuUnique2018_dTh6 = (TH1F*)file2->Get("SMRecoPtUniqueMatchBX0SingleMu")->Clone("MatchBX0SingleMuUnique2018_dTh6");
  TH1F *MatchBX0SingleMuUnique2018_dTh6Log = (TH1F*)file2->Get("SMLogRecoPtUniqueMatchBX0SingleMu")->Clone("MatchBX0SingleMuUnique2018_dTh6Log");

  TH1F *MatchBX0DoubleMuInc2018_dTh6 = (TH1F*)file2->Get("SMRecoPtMatchBX0DoubleMuInc")->Clone("MatchBX0DoubleMuInc2018_dTh6");
  TH1F *MatchBX0DoubleMuInc2018_dTh6Log = (TH1F*)file2->Get("SMLogRecoPtMatchBX0DoubleMuInc")->Clone("MatchBX0DoubleMuInc2018_dTh6Log");
  TH1F *MatchBX0DoubleMuIncPlateau2018_dTh6 = (TH1F*)file2->Get("SMRecoPtMatchBX0DoubleMuIncPlateau")->Clone("MatchBX0DoubleMuIncPlateau2018_dTh6");
  TH1F *MatchBX0DoubleMuIncPlateau2018_dTh6Log = (TH1F*)file2->Get("SMLogRecoPtMatchBX0DoubleMuIncPlateau")->Clone("MatchBX0DoubleMuIncPlateau2018_dTh6Log");
  TH1F *MatchBX0DoubleMuIncUnique2018_dTh6 = (TH1F*)file2->Get("SMRecoPtUniqueMatchBX0DoubleMuInc")->Clone("MatchBX0DoubleMuIncUnique2018_dTh6");
  TH1F *MatchBX0DoubleMuIncUnique2018_dTh6Log = (TH1F*)file2->Get("SMLogRecoPtUniqueMatchBX0DoubleMuInc")->Clone("MatchBX0DoubleMuIncUnique2018_dTh6Log");
	
  TH1F *MatchBX0MuOpenInc2018_dTh6 = (TH1F*)file2->Get("SMRecoPtMatchBX0MuOpenInc")->Clone("MatchBX0MuOpenInc2018_dTh6");
  TH1F *MatchBX0MuOpenInc2018_dTh6Log = (TH1F*)file2->Get("SMLogRecoPtMatchBX0MuOpenInc")->Clone("MatchBX0MuOpenInc2018_dTh6Log");
  TH1F *MatchBX0MuOpenIncPlateau2018_dTh6 = (TH1F*)file2->Get("SMRecoPtMatchBX0MuOpenIncPlateau")->Clone("MatchBX0MuOpenIncPlateau2018_dTh6");
  TH1F *MatchBX0MuOpenIncPlateau2018_dTh6Log = (TH1F*)file2->Get("SMLogRecoPtMatchBX0MuOpenIncPlateau")->Clone("MatchBX0MuOpenIncPlateau2018_dTh6Log");
  TH1F *MatchBX0MuOpenIncUnique2018_dTh6 = (TH1F*)file2->Get("SMRecoPtUniqueMatchBX0MuOpenInc")->Clone("MatchBX0MuOpenIncUnique2018_dTh6");
  TH1F *MatchBX0MuOpenIncUnique2018_dTh6Log = (TH1F*)file2->Get("SMLogRecoPtUniqueMatchBX0MuOpenInc")->Clone("MatchBX0MuOpenIncUnique2018_dTh6Log");
	
  TFile *file3 = TFile::Open("2018_dTh8_ModesRateEff_eta_1.25_2.40.root");//4
  TH1F *HRateSingleMu2018_dTh8 = (TH1F*)file3->Get("HRateSingleMu")->Clone("HRateSingleMu2018_dTh8");
  TH1F *HRateDoubleMuInc2018_dTh8 = (TH1F*)file3->Get("HRateDoubleMuInc")->Clone("HRateDoubleMuInc2018_dTh8");
  TH1F *HRateMuOpenInc2018_dTh8 = (TH1F*)file3->Get("HRateMuOpenInc")->Clone("HRateMuOpenInc2018_dTh8");
	
  TH1F *MatchBX0SingleMu2018_dTh8 = (TH1F*)file3->Get("SMRecoPtMatchBX0SingleMu")->Clone("MatchBX0SingleMu2018_dTh8");
  TH1F *MatchBX0SingleMu2018_dTh8Log = (TH1F*)file3->Get("SMLogRecoPtMatchBX0SingleMu")->Clone("MatchBX0SingleMu2018_dTh8Log");
  TH1F *MatchBX0SingleMuPlateau2018_dTh8 = (TH1F*)file3->Get("SMRecoPtMatchBX0SingleMuPlateau")->Clone("MatchBX0SingleMuPlateau2018_dTh8");
  TH1F *MatchBX0SingleMuPlateau2018_dTh8Log = (TH1F*)file3->Get("SMLogRecoPtMatchBX0SingleMuPlateau")->Clone("MatchBX0SingleMuPlateau2018_dTh8Log");
  TH1F *MatchBX0SingleMuUnique2018_dTh8 = (TH1F*)file3->Get("SMRecoPtUniqueMatchBX0SingleMu")->Clone("MatchBX0SingleMuUnique2018_dTh8");
  TH1F *MatchBX0SingleMuUnique2018_dTh8Log = (TH1F*)file3->Get("SMLogRecoPtUniqueMatchBX0SingleMu")->Clone("MatchBX0SingleMuUnique2018_dTh8Log");

  TH1F *MatchBX0DoubleMuInc2018_dTh8 = (TH1F*)file3->Get("SMRecoPtMatchBX0DoubleMuInc")->Clone("MatchBX0DoubleMuInc2018_dTh8");
  TH1F *MatchBX0DoubleMuInc2018_dTh8Log = (TH1F*)file3->Get("SMLogRecoPtMatchBX0DoubleMuInc")->Clone("MatchBX0DoubleMuInc2018_dTh8Log");
  TH1F *MatchBX0DoubleMuIncPlateau2018_dTh8 = (TH1F*)file3->Get("SMRecoPtMatchBX0DoubleMuIncPlateau")->Clone("MatchBX0DoubleMuIncPlateau2018_dTh8");
  TH1F *MatchBX0DoubleMuIncPlateau2018_dTh8Log = (TH1F*)file3->Get("SMLogRecoPtMatchBX0DoubleMuIncPlateau")->Clone("MatchBX0DoubleMuIncPlateau2018_dTh8Log");
  TH1F *MatchBX0DoubleMuIncUnique2018_dTh8 = (TH1F*)file3->Get("SMRecoPtUniqueMatchBX0DoubleMuInc")->Clone("MatchBX0DoubleMuIncUnique2018_dTh8");
  TH1F *MatchBX0DoubleMuIncUnique2018_dTh8Log = (TH1F*)file3->Get("SMLogRecoPtUniqueMatchBX0DoubleMuInc")->Clone("MatchBX0DoubleMuIncUnique2018_dTh8Log");
	
  TH1F *MatchBX0MuOpenInc2018_dTh8 = (TH1F*)file3->Get("SMRecoPtMatchBX0MuOpenInc")->Clone("MatchBX0MuOpenInc2018_dTh8");
  TH1F *MatchBX0MuOpenInc2018_dTh8Log = (TH1F*)file3->Get("SMLogRecoPtMatchBX0MuOpenInc")->Clone("MatchBX0MuOpenInc2018_dTh8Log");
  TH1F *MatchBX0MuOpenIncPlateau2018_dTh8 = (TH1F*)file3->Get("SMRecoPtMatchBX0MuOpenIncPlateau")->Clone("MatchBX0MuOpenIncPlateau2018_dTh8");
  TH1F *MatchBX0MuOpenIncPlateau2018_dTh8Log = (TH1F*)file3->Get("SMLogRecoPtMatchBX0MuOpenIncPlateau")->Clone("MatchBX0MuOpenIncPlateau2018_dTh8Log");
  TH1F *MatchBX0MuOpenIncUnique2018_dTh8 = (TH1F*)file3->Get("SMRecoPtUniqueMatchBX0MuOpenInc")->Clone("MatchBX0MuOpenIncUnique2018_dTh8");
  TH1F *MatchBX0MuOpenIncUnique2018_dTh8Log = (TH1F*)file3->Get("SMLogRecoPtUniqueMatchBX0MuOpenInc")->Clone("MatchBX0MuOpenIncUnique2018_dTh8Log");
	
  TFile *file4 = TFile::Open("2017_dBX_ModesRateEff_eta_1.25_2.40.root");//6
  TH1F *HRateSingleMu2017_dBX = (TH1F*)file4->Get("HRateSingleMu")->Clone("HRateSingleMu2017_dBX");
  TH1F *HRateDoubleMuInc2017_dBX = (TH1F*)file4->Get("HRateDoubleMuInc")->Clone("HRateDoubleMuInc2017_dBX");
  TH1F *HRateMuOpenInc2017_dBX = (TH1F*)file4->Get("HRateMuOpenInc")->Clone("HRateMuOpenInc2017_dBX");
	
  TH1F *MatchBX0SingleMu2017_dBX = (TH1F*)file4->Get("SMRecoPtMatchBX0SingleMu")->Clone("MatchBX0SingleMu2017_dBX");
  TH1F *MatchBX0SingleMu2017_dBXLog = (TH1F*)file4->Get("SMLogRecoPtMatchBX0SingleMu")->Clone("MatchBX0SingleMu2017_dBXLog");
  TH1F *MatchBX0SingleMuPlateau2017_dBX = (TH1F*)file4->Get("SMRecoPtMatchBX0SingleMuPlateau")->Clone("MatchBX0SingleMuPlateau2017_dBX");
  TH1F *MatchBX0SingleMuPlateau2017_dBXLog = (TH1F*)file4->Get("SMLogRecoPtMatchBX0SingleMuPlateau")->Clone("MatchBX0SingleMuPlateau2017_dBXLog");
  TH1F *MatchBX0SingleMuUnique2017_dBX = (TH1F*)file4->Get("SMRecoPtUniqueMatchBX0SingleMu")->Clone("MatchBX0SingleMuUnique2017_dBX");
  TH1F *MatchBX0SingleMuUnique2017_dBXLog = (TH1F*)file4->Get("SMLogRecoPtUniqueMatchBX0SingleMu")->Clone("MatchBX0SingleMuUnique2017_dBXLog");

  TH1F *MatchBX0DoubleMuInc2017_dBX = (TH1F*)file4->Get("SMRecoPtMatchBX0DoubleMuInc")->Clone("MatchBX0DoubleMuInc2017_dBX");
  TH1F *MatchBX0DoubleMuInc2017_dBXLog = (TH1F*)file4->Get("SMLogRecoPtMatchBX0DoubleMuInc")->Clone("MatchBX0DoubleMuInc2017_dBXLog");
  TH1F *MatchBX0DoubleMuIncPlateau2017_dBX = (TH1F*)file4->Get("SMRecoPtMatchBX0DoubleMuIncPlateau")->Clone("MatchBX0DoubleMuIncPlateau2017_dBX");
  TH1F *MatchBX0DoubleMuIncPlateau2017_dBXLog = (TH1F*)file4->Get("SMLogRecoPtMatchBX0DoubleMuIncPlateau")->Clone("MatchBX0DoubleMuIncPlateau2017_dBXLog");
  TH1F *MatchBX0DoubleMuIncUnique2017_dBX = (TH1F*)file4->Get("SMRecoPtUniqueMatchBX0DoubleMuInc")->Clone("MatchBX0DoubleMuIncUnique2017_dBX");
  TH1F *MatchBX0DoubleMuIncUnique2017_dBXLog = (TH1F*)file4->Get("SMLogRecoPtUniqueMatchBX0DoubleMuInc")->Clone("MatchBX0DoubleMuIncUnique2017_dBXLog");
	
  TH1F *MatchBX0MuOpenInc2017_dBX = (TH1F*)file4->Get("SMRecoPtMatchBX0MuOpenInc")->Clone("MatchBX0MuOpenInc2017_dBX");
  TH1F *MatchBX0MuOpenInc2017_dBXLog = (TH1F*)file4->Get("SMLogRecoPtMatchBX0MuOpenInc")->Clone("MatchBX0MuOpenInc2017_dBXLog");
  TH1F *MatchBX0MuOpenIncPlateau2017_dBX = (TH1F*)file4->Get("SMRecoPtMatchBX0MuOpenIncPlateau")->Clone("MatchBX0MuOpenIncPlateau2017_dBX");
  TH1F *MatchBX0MuOpenIncPlateau2017_dBXLog = (TH1F*)file4->Get("SMLogRecoPtMatchBX0MuOpenIncPlateau")->Clone("MatchBX0MuOpenIncPlateau2017_dBXLog");
  TH1F *MatchBX0MuOpenIncUnique2017_dBX = (TH1F*)file4->Get("SMRecoPtUniqueMatchBX0MuOpenInc")->Clone("MatchBX0MuOpenIncUnique2017_dBX");
  TH1F *MatchBX0MuOpenIncUnique2017_dBXLog = (TH1F*)file4->Get("SMLogRecoPtUniqueMatchBX0MuOpenInc")->Clone("MatchBX0MuOpenIncUnique2017_dBXLog");
	
  TFile *file5 = TFile::Open("2017_dBX_2018Quality_ModesRateEff_eta_1.25_2.40.root");
  TH1F *HRateSingleMu2017_dBX_2018Quality = (TH1F*)file5->Get("HRateSingleMu")->Clone("HRateSingleMu2017_dBX_2018Quality");
  TH1F *HRateDoubleMuInc2017_dBX_2018Quality = (TH1F*)file5->Get("HRateDoubleMuInc")->Clone("HRateDoubleMuInc2017_dBX_2018Quality");
  TH1F *HRateMuOpenInc2017_dBX_2018Quality = (TH1F*)file5->Get("HRateMuOpenInc")->Clone("HRateMuOpenInc2017_dBX_2018Quality");
	
  //write to output file
  TFile myPlot("Overlay.root","RECREATE");
  
  TCanvas *C1=new TCanvas("C1","Rate: SingleMu",700,500);
  TCanvas *C2=new TCanvas("C2","Rate: DoubleMu Inclusive",700,500);
  TCanvas *C3=new TCanvas("C3","Rate: MuOpen Inclusive",700,500);
  TCanvas *C4=new TCanvas("C4","Rate ratio: SingleMu",700,500);
  TCanvas *C5=new TCanvas("C5","Rate ratio: DoubleMu Inclusive",700,500);
  TCanvas *C6=new TCanvas("C6","Rate ratio: MuOpen Inclusive",700,500);
	
  TCanvas *C7=new TCanvas("C7","Efficiency: SingleMu",700,500);
  TCanvas *C8=new TCanvas("C8","Efficiency: DoubleleMu Inclusive",700,500);
  TCanvas *C9=new TCanvas("C9","Efficiency: MuOpen Inclusive",700,500);
  TCanvas *C10=new TCanvas("C10","Efficiency: SingleMu (log2pT)",700,500);
  TCanvas *C11=new TCanvas("C11","Efficiency: DoubleleMu Inclusive (log2pT)",700,500);
  TCanvas *C12=new TCanvas("C12","Efficiency: MuOpen Inclusive (log2pT)",700,500);
  //Plateau	
  TCanvas *C13=new TCanvas("C13","Plateau Efficiency: SingleMu",700,500);
  TCanvas *C14=new TCanvas("C14","Plateau Efficiency: DoubleleMu Inclusive",700,500);
  TCanvas *C15=new TCanvas("C15","Plateau Efficiency: MuOpen Inclusive",700,500);
  TCanvas *C16=new TCanvas("C16","Plateau Efficiency: SingleMu (log2pT)",700,500);
  TCanvas *C17=new TCanvas("C17","Plateau Efficiency: DoubleleMu Inclusive (log2pT)",700,500);
  TCanvas *C18=new TCanvas("C18","Plateau Efficiency: MuOpen Inclusive (log2pT)",700,500);
  //Unique
  TCanvas *C19=new TCanvas("C19","Unique Match Efficiency: SingleMu",700,500);
  TCanvas *C20=new TCanvas("C20","Unique Match Efficiency: DoubleleMu Inclusive",700,500);
  TCanvas *C21=new TCanvas("C21","Unique Match Efficiency: MuOpen Inclusive",700,500);
  TCanvas *C22=new TCanvas("C22","Unique Match Efficiency: SingleMu (log2pT)",700,500);
  TCanvas *C23=new TCanvas("C23","Unique Match Efficiency: DoubleleMu Inclusive (log2pT)",700,500);
  TCanvas *C24=new TCanvas("C24","Unique Match Efficiency: MuOpen Inclusive (log2pT)",700,500);
	
  C1->cd();
  HRateSingleMu2017->SetLineColor(1); HRateSingleMu2017->Draw("HIST");
  HRateSingleMu2018_dTh4->SetLineColor(2); HRateSingleMu2018_dTh4->Draw("HISTSAME");
  HRateSingleMu2018_dTh6->SetLineColor(3); HRateSingleMu2018_dTh6->Draw("HISTSAME");
  HRateSingleMu2018_dTh8->SetLineColor(4); HRateSingleMu2018_dTh8->Draw("HISTSAME");
  C1->Write();
	
  C2->cd();
  HRateDoubleMuInc2017->SetLineColor(1); HRateDoubleMuInc2017->Draw("HIST");
  HRateDoubleMuInc2018_dTh4->SetLineColor(2); HRateDoubleMuInc2018_dTh4->Draw("HISTSAME");
  HRateDoubleMuInc2018_dTh6->SetLineColor(3); HRateDoubleMuInc2018_dTh6->Draw("HISTSAME");
  HRateDoubleMuInc2018_dTh8->SetLineColor(4); HRateDoubleMuInc2018_dTh8->Draw("HISTSAME");
  C2->Write();
	
  C3->cd();
  HRateMuOpenInc2017->SetLineColor(1); HRateMuOpenInc2017->Draw("HIST");
  HRateMuOpenInc2018_dTh4->SetLineColor(2); HRateMuOpenInc2018_dTh4->Draw("HISTSAME");
  HRateMuOpenInc2018_dTh6->SetLineColor(3); HRateMuOpenInc2018_dTh6->Draw("HISTSAME");
  HRateMuOpenInc2018_dTh8->SetLineColor(4); HRateMuOpenInc2018_dTh8->Draw("HISTSAME");
  C3->Write();
	
  C4->cd();
  HRateSingleMu2018_dTh4->Divide(HRateSingleMu2017);
  HRateSingleMu2018_dTh4->SetLineColor(2); HRateSingleMu2018_dTh4->Draw("HIST");
  HRateSingleMu2018_dTh6->Divide(HRateSingleMu2017);
  HRateSingleMu2018_dTh6->SetLineColor(3); HRateSingleMu2018_dTh6->Draw("HISTSAME");
  HRateSingleMu2018_dTh8->Divide(HRateSingleMu2017);
  HRateSingleMu2018_dTh8->SetLineColor(4); HRateSingleMu2018_dTh8->Draw("HISTSAME");
  HRateSingleMu2017_dBX->Divide(HRateSingleMu2017);
  HRateSingleMu2017_dBX->SetLineColor(6); HRateSingleMu2017_dBX->Draw("HISTSAME");
  C4->Write();

  C5->cd();
  HRateDoubleMuInc2018_dTh4->Divide(HRateDoubleMuInc2017);
  HRateDoubleMuInc2018_dTh4->SetLineColor(2); HRateDoubleMuInc2018_dTh4->Draw("HIST");
  HRateDoubleMuInc2018_dTh6->Divide(HRateDoubleMuInc2017);
  HRateDoubleMuInc2018_dTh6->SetLineColor(3); HRateDoubleMuInc2018_dTh6->Draw("HISTSAME");
  HRateDoubleMuInc2018_dTh8->Divide(HRateDoubleMuInc2017);
  HRateDoubleMuInc2018_dTh8->SetLineColor(4); HRateDoubleMuInc2018_dTh8->Draw("HISTSAME");
  HRateDoubleMuInc2017_dBX->Divide(HRateDoubleMuInc2017);
  HRateDoubleMuInc2017_dBX->SetLineColor(6); HRateDoubleMuInc2017_dBX->Draw("HISTSAME");
  HRateDoubleMuInc2017_dBX_2018Quality->Divide(HRateDoubleMuInc2017);
  HRateDoubleMuInc2017_dBX_2018Quality->SetLineColor(7); HRateDoubleMuInc2017_dBX_2018Quality->Draw("HISTSAME");//2017 dBX with 2018 mu quality
  C5->Write();
	
  C6->cd();
  HRateMuOpenInc2018_dTh4->Divide(HRateMuOpenInc2017);
  HRateMuOpenInc2018_dTh4->SetLineColor(2); HRateMuOpenInc2018_dTh4->Draw("HIST");
  HRateMuOpenInc2018_dTh6->Divide(HRateMuOpenInc2017);
  HRateMuOpenInc2018_dTh6->SetLineColor(3); HRateMuOpenInc2018_dTh6->Draw("HISTSAME");
  HRateMuOpenInc2018_dTh8->Divide(HRateMuOpenInc2017);
  HRateMuOpenInc2018_dTh8->SetLineColor(4); HRateMuOpenInc2018_dTh8->Draw("HISTSAME");
  HRateMuOpenInc2017_dBX->Divide(HRateMuOpenInc2017);
  HRateMuOpenInc2017_dBX->SetLineColor(6); HRateMuOpenInc2017_dBX->Draw("HISTSAME");
  C6->Write();
	
  C7->cd();
  MatchBX0SingleMu2017->SetLineColor(1); MatchBX0SingleMu2017->Draw();
  MatchBX0SingleMu2018_dTh4->SetLineColor(2); MatchBX0SingleMu2018_dTh4->Draw("SAME");
  MatchBX0SingleMu2018_dTh6->SetLineColor(3); MatchBX0SingleMu2018_dTh6->Draw("SAME");
  MatchBX0SingleMu2018_dTh8->SetLineColor(4); MatchBX0SingleMu2018_dTh8->Draw("SAME");
  C7->Write();
	
  C8->cd();
  MatchBX0DoubleMuInc2017->SetLineColor(1); MatchBX0DoubleMuInc2017->Draw();
  MatchBX0DoubleMuInc2018_dTh4->SetLineColor(2); MatchBX0DoubleMuInc2018_dTh4->Draw("SAME");
  MatchBX0DoubleMuInc2018_dTh6->SetLineColor(3); MatchBX0DoubleMuInc2018_dTh6->Draw("SAME");
  MatchBX0DoubleMuInc2018_dTh8->SetLineColor(4); MatchBX0DoubleMuInc2018_dTh8->Draw("SAME");
  C8->Write();

  C9->cd();
  MatchBX0MuOpenInc2017->SetLineColor(1); MatchBX0MuOpenInc2017->Draw();
  MatchBX0MuOpenInc2018_dTh4->SetLineColor(2); MatchBX0MuOpenInc2018_dTh4->Draw("SAME");
  MatchBX0MuOpenInc2018_dTh6->SetLineColor(3); MatchBX0MuOpenInc2018_dTh6->Draw("SAME");
  MatchBX0MuOpenInc2018_dTh8->SetLineColor(4); MatchBX0MuOpenInc2018_dTh8->Draw("SAME");
  C9->Write();
	
  C10->cd();
  MatchBX0SingleMu2017Log->SetLineColor(1); MatchBX0SingleMu2017Log->Draw();
  MatchBX0SingleMu2018_dTh4Log->SetLineColor(2); MatchBX0SingleMu2018_dTh4Log->Draw("SAME");
  MatchBX0SingleMu2018_dTh6Log->SetLineColor(3); MatchBX0SingleMu2018_dTh6Log->Draw("SAME");
  MatchBX0SingleMu2018_dTh8Log->SetLineColor(4); MatchBX0SingleMu2018_dTh8Log->Draw("SAME");
  C10->Write();
	
  C11->cd();
  MatchBX0DoubleMuInc2017Log->SetLineColor(1); MatchBX0DoubleMuInc2017Log->Draw();
  MatchBX0DoubleMuInc2018_dTh4Log->SetLineColor(2); MatchBX0DoubleMuInc2018_dTh4Log->Draw("SAME");
  MatchBX0DoubleMuInc2018_dTh6Log->SetLineColor(3); MatchBX0DoubleMuInc2018_dTh6Log->Draw("SAME");
  MatchBX0DoubleMuInc2018_dTh8Log->SetLineColor(4); MatchBX0DoubleMuInc2018_dTh8Log->Draw("SAME");
  C11->Write();

  C12->cd();
  MatchBX0MuOpenInc2017Log->SetLineColor(1); MatchBX0MuOpenInc2017Log->Draw();
  MatchBX0MuOpenInc2018_dTh4Log->SetLineColor(2); MatchBX0MuOpenInc2018_dTh4Log->Draw("SAME");
  MatchBX0MuOpenInc2018_dTh6Log->SetLineColor(3); MatchBX0MuOpenInc2018_dTh6Log->Draw("SAME");
  MatchBX0MuOpenInc2018_dTh8Log->SetLineColor(4); MatchBX0MuOpenInc2018_dTh8Log->Draw("SAME");
  C12->Write();	
	
  C13->cd();
  MatchBX0SingleMuPlateau2017->SetLineColor(1); MatchBX0SingleMuPlateau2017->Draw();
  MatchBX0SingleMuPlateau2018_dTh4->SetLineColor(2); MatchBX0SingleMuPlateau2018_dTh4->Draw("SAME");
  MatchBX0SingleMuPlateau2018_dTh6->SetLineColor(3); MatchBX0SingleMuPlateau2018_dTh6->Draw("SAME");
  MatchBX0SingleMuPlateau2018_dTh8->SetLineColor(4); MatchBX0SingleMuPlateau2018_dTh8->Draw("SAME");
  C13->Write();
	
  C14->cd();
  MatchBX0DoubleMuIncPlateau2017->SetLineColor(1); MatchBX0DoubleMuIncPlateau2017->Draw();
  MatchBX0DoubleMuIncPlateau2018_dTh4->SetLineColor(2); MatchBX0DoubleMuIncPlateau2018_dTh4->Draw("SAME");
  MatchBX0DoubleMuIncPlateau2018_dTh6->SetLineColor(3); MatchBX0DoubleMuIncPlateau2018_dTh6->Draw("SAME");
  MatchBX0DoubleMuIncPlateau2018_dTh8->SetLineColor(4); MatchBX0DoubleMuIncPlateau2018_dTh8->Draw("SAME");
  C14->Write();

  C15->cd();
  MatchBX0MuOpenIncPlateau2017->SetLineColor(1); MatchBX0MuOpenIncPlateau2017->Draw();
  MatchBX0MuOpenIncPlateau2018_dTh4->SetLineColor(2); MatchBX0MuOpenIncPlateau2018_dTh4->Draw("SAME");
  MatchBX0MuOpenIncPlateau2018_dTh6->SetLineColor(3); MatchBX0MuOpenIncPlateau2018_dTh6->Draw("SAME");
  MatchBX0MuOpenIncPlateau2018_dTh8->SetLineColor(4); MatchBX0MuOpenIncPlateau2018_dTh8->Draw("SAME");
  C15->Write();

  C16->cd();
  MatchBX0SingleMuPlateau2017Log->SetLineColor(1); MatchBX0SingleMuPlateau2017Log->Draw();
  MatchBX0SingleMuPlateau2018_dTh4Log->SetLineColor(2); MatchBX0SingleMuPlateau2018_dTh4Log->Draw("SAME");
  MatchBX0SingleMuPlateau2018_dTh6Log->SetLineColor(3); MatchBX0SingleMuPlateau2018_dTh6Log->Draw("SAME");
  MatchBX0SingleMuPlateau2018_dTh8Log->SetLineColor(4); MatchBX0SingleMuPlateau2018_dTh8Log->Draw("SAME");
  C16->Write();
	
  C17->cd();
  MatchBX0DoubleMuIncPlateau2017Log->SetLineColor(1); MatchBX0DoubleMuIncPlateau2017Log->Draw();
  MatchBX0DoubleMuIncPlateau2018_dTh4Log->SetLineColor(2); MatchBX0DoubleMuIncPlateau2018_dTh4Log->Draw("SAME");
  MatchBX0DoubleMuIncPlateau2018_dTh6Log->SetLineColor(3); MatchBX0DoubleMuIncPlateau2018_dTh6Log->Draw("SAME");
  MatchBX0DoubleMuIncPlateau2018_dTh8Log->SetLineColor(4); MatchBX0DoubleMuIncPlateau2018_dTh8Log->Draw("SAME");
  C17->Write();

  C18->cd();
  MatchBX0MuOpenIncPlateau2017Log->SetLineColor(1); MatchBX0MuOpenIncPlateau2017Log->Draw();
  MatchBX0MuOpenIncPlateau2018_dTh4Log->SetLineColor(2); MatchBX0MuOpenIncPlateau2018_dTh4Log->Draw("SAME");
  MatchBX0MuOpenIncPlateau2018_dTh6Log->SetLineColor(3); MatchBX0MuOpenIncPlateau2018_dTh6Log->Draw("SAME");
  MatchBX0MuOpenIncPlateau2018_dTh8Log->SetLineColor(4); MatchBX0MuOpenIncPlateau2018_dTh8Log->Draw("SAME");
  C18->Write();
	
  C19->cd();
  MatchBX0SingleMuUnique2017->SetLineColor(1); MatchBX0SingleMuUnique2017->Draw();
  MatchBX0SingleMuUnique2018_dTh4->SetLineColor(2); MatchBX0SingleMuUnique2018_dTh4->Draw("SAME");
  MatchBX0SingleMuUnique2018_dTh6->SetLineColor(3); MatchBX0SingleMuUnique2018_dTh6->Draw("SAME");
  MatchBX0SingleMuUnique2018_dTh8->SetLineColor(4); MatchBX0SingleMuUnique2018_dTh8->Draw("SAME");
  C19->Write();
	
  C20->cd();
  MatchBX0DoubleMuIncUnique2017->SetLineColor(1); MatchBX0DoubleMuIncUnique2017->Draw();
  MatchBX0DoubleMuIncUnique2018_dTh4->SetLineColor(2); MatchBX0DoubleMuIncUnique2018_dTh4->Draw("SAME");
  MatchBX0DoubleMuIncUnique2018_dTh6->SetLineColor(3); MatchBX0DoubleMuIncUnique2018_dTh6->Draw("SAME");
  MatchBX0DoubleMuIncUnique2018_dTh8->SetLineColor(4); MatchBX0DoubleMuIncUnique2018_dTh8->Draw("SAME");
  C20->Write();
	
  C21->cd();
  MatchBX0MuOpenIncUnique2017->SetLineColor(1); MatchBX0MuOpenIncUnique2017->Draw();
  MatchBX0MuOpenIncUnique2018_dTh4->SetLineColor(2); MatchBX0MuOpenIncUnique2018_dTh4->Draw("SAME");
  MatchBX0MuOpenIncUnique2018_dTh6->SetLineColor(3); MatchBX0MuOpenIncUnique2018_dTh6->Draw("SAME");
  MatchBX0MuOpenIncUnique2018_dTh8->SetLineColor(4); MatchBX0MuOpenIncUnique2018_dTh8->Draw("SAME");
  C21->Write();
	
  C22->cd();
  MatchBX0SingleMuUnique2017Log->SetLineColor(1); MatchBX0SingleMuUnique2017Log->Draw();
  MatchBX0SingleMuUnique2018_dTh4Log->SetLineColor(2); MatchBX0SingleMuUnique2018_dTh4Log->Draw("SAME");
  MatchBX0SingleMuUnique2018_dTh6Log->SetLineColor(3); MatchBX0SingleMuUnique2018_dTh6Log->Draw("SAME");
  MatchBX0SingleMuUnique2018_dTh8Log->SetLineColor(4); MatchBX0SingleMuUnique2018_dTh8Log->Draw("SAME");
  C22->Write();
	
  C23->cd();
  MatchBX0DoubleMuIncUnique2017Log->SetLineColor(1); MatchBX0DoubleMuIncUnique2017Log->Draw();
  MatchBX0DoubleMuIncUnique2018_dTh4Log->SetLineColor(2); MatchBX0DoubleMuIncUnique2018_dTh4Log->Draw("SAME");
  MatchBX0DoubleMuIncUnique2018_dTh6Log->SetLineColor(3); MatchBX0DoubleMuIncUnique2018_dTh6Log->Draw("SAME");
  MatchBX0DoubleMuIncUnique2018_dTh8Log->SetLineColor(4); MatchBX0DoubleMuIncUnique2018_dTh8Log->Draw("SAME");
  C23->Write();
	
  C24->cd();
  MatchBX0MuOpenIncUnique2017Log->SetLineColor(1); MatchBX0MuOpenIncUnique2017Log->Draw();
  MatchBX0MuOpenIncUnique2018_dTh4Log->SetLineColor(2); MatchBX0MuOpenIncUnique2018_dTh4Log->Draw("SAME");
  MatchBX0MuOpenIncUnique2018_dTh6Log->SetLineColor(3); MatchBX0MuOpenIncUnique2018_dTh6Log->Draw("SAME");
  MatchBX0MuOpenIncUnique2018_dTh8Log->SetLineColor(4); MatchBX0MuOpenIncUnique2018_dTh8Log->Draw("SAME");
  C24->Write();

  myPlot.Close();
  
} // End function: void 
