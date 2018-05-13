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
  TH1F *HRateSingleMu2017 = (TH1F*)HRateSingleMu->Clone("HRateSingleMu2017");
  TH1F *HRateDoubleMuInc2017 = (TH1F*)HRateDoubleMuInc->Clone("HRateDoubleMuInc2017");
  TH1F *HRateMuOpenInc2017 = (TH1F*)HRateMuOpenInc->Clone("HRateMuOpenInc2017");
	
  TH1F *MatchBX0SingleMu2017 = (TH1F*)SMRecoPtMatchBX0SingleMu->Clone("MatchBX0SingleMu2017");
  TH1F *MatchBX0SingleMu2017Log = (TH1F*)SMLogRecoPtMatchBX0SingleMu->Clone("MatchBX0SingleMu2017Log");
  TH1F *MatchBX0SingleMuPlateau2017 = (TH1F*)SMRecoPtMatchBX0SingleMuPlateau->Clone("MatchBX0SingleMuPlateau2017");
  TH1F *MatchBX0SingleMuPlateau2017Log = (TH1F*)SMLogRecoPtMatchBX0SingleMuPlateau->Clone("MatchBX0SingleMuPlateau2017Log");
  TH1F *MatchBX0SingleMuUnique2017 = (TH1F*)SMRecoPtUniqueMatchBX0SingleMu->Clone("MatchBX0SingleMuUnique2017");
  TH1F *MatchBX0SingleMuUnique2017Log = (TH1F*)SMLogRecoPtUniqueMatchBX0SingleMu->Clone("MatchBX0SingleMuUnique2017Log");

  TH1F *MatchBX0DoubleMuInc2017 = (TH1F*)SMRecoPtMatchBX0DoubleMuInc->Clone("MatchBX0DoubleMuInc2017");
  TH1F *MatchBX0DoubleMuInc2017Log = (TH1F*)SMLogRecoPtMatchBX0DoubleMuInc->Clone("MatchBX0DoubleMuInc2017Log");
  TH1F *MatchBX0DoubleMuIncPlateau2017 = (TH1F*)SMRecoPtMatchBX0DoubleMuIncPlateau->Clone("MatchBX0DoubleMuIncPlateau2017");
  TH1F *MatchBX0DoubleMuIncPlateau2017Log = (TH1F*)SMLogRecoPtMatchBX0DoubleMuIncPlateau->Clone("MatchBX0DoubleMuIncPlateau2017Log");
  TH1F *MatchBX0DoubleMuIncUnique2017 = (TH1F*)SMRecoPtUniqueMatchBX0DoubleMuInc->Clone("MatchBX0DoubleMuIncUnique2017");
  TH1F *MatchBX0DoubleMuIncUnique2017Log = (TH1F*)SMLogRecoPtUniqueMatchBX0DoubleMuInc->Clone("MatchBX0DoubleMuIncUnique2017Log");
	
  TH1F *MatchBX0MuOpenInc2017 = (TH1F*)SMRecoPtMatchBX0MuOpenInc->Clone("MatchBX0MuOpenInc2017");
  TH1F *MatchBX0MuOpenInc2017Log = (TH1F*)SMLogRecoPtMatchBX0MuOpenInc->Clone("MatchBX0MuOpenInc2017Log");
  TH1F *MatchBX0MuOpenIncPlateau2017 = (TH1F*)SMRecoPtMatchBX0MuOpenIncPlateau->Clone("MatchBX0MuOpenIncPlateau2017");
  TH1F *MatchBX0MuOpenIncPlateau2017Log = (TH1F*)SMLogRecoPtMatchBX0MuOpenIncPlateau->Clone("MatchBX0MuOpenIncPlateau2017Log");
  TH1F *MatchBX0MuOpenIncUnique2017 = (TH1F*)SMRecoPtUniqueMatchBX0MuOpenInc->Clone("MatchBX0MuOpenIncUnique2017");
  TH1F *MatchBX0MuOpenIncUnique2017Log = (TH1F*)SMLogRecoPtUniqueMatchBX0MuOpenInc->Clone("MatchBX0MuOpenIncUnique2017Log");	
  file0->Close();
	
  TFile *file1 = TFile::Open("2018_dTh4_ModesRateEff_eta_1.25_2.40.root");//2
  TH1F *HRateSingleMu2018_dTh4 = (TH1F*)HRateSingleMu->Clone("HRateSingleMu2018_dTh4");
  TH1F *HRateDoubleMuInc2018_dTh4 = (TH1F*)HRateDoubleMuInc->Clone("HRateDoubleMuInc2018_dTh4");
  TH1F *HRateMuOpenInc2018_dTh4 = (TH1F*)HRateMuOpenInc->Clone("HRateMuOpenInc2018_dTh4");
	
  TH1F *MatchBX0SingleMu2018_dTh4 = (TH1F*)SMRecoPtMatchBX0SingleMu->Clone("MatchBX0SingleMu2018_dTh4");
  TH1F *MatchBX0SingleMu2018_dTh4Log = (TH1F*)SMLogRecoPtMatchBX0SingleMu->Clone("MatchBX0SingleMu2018_dTh4Log");
  TH1F *MatchBX0SingleMuPlateau2018_dTh4 = (TH1F*)SMRecoPtMatchBX0SingleMuPlateau->Clone("MatchBX0SingleMuPlateau2018_dTh4");
  TH1F *MatchBX0SingleMuPlateau2018_dTh4Log = (TH1F*)SMLogRecoPtMatchBX0SingleMuPlateau->Clone("MatchBX0SingleMuPlateau2018_dTh4Log");
  TH1F *MatchBX0SingleMuUnique2018_dTh4 = (TH1F*)SMRecoPtUniqueMatchBX0SingleMu->Clone("MatchBX0SingleMuUnique2018_dTh4");
  TH1F *MatchBX0SingleMuUnique2018_dTh4Log = (TH1F*)SMLogRecoPtUniqueMatchBX0SingleMu->Clone("MatchBX0SingleMuUnique2018_dTh4Log");

  TH1F *MatchBX0DoubleMuInc2018_dTh4 = (TH1F*)SMRecoPtMatchBX0DoubleMuInc->Clone("MatchBX0DoubleMuInc2018_dTh4");
  TH1F *MatchBX0DoubleMuInc2018_dTh4Log = (TH1F*)SMLogRecoPtMatchBX0DoubleMuInc->Clone("MatchBX0DoubleMuInc2018_dTh4Log");
  TH1F *MatchBX0DoubleMuIncPlateau2018_dTh4 = (TH1F*)SMRecoPtMatchBX0DoubleMuIncPlateau->Clone("MatchBX0DoubleMuIncPlateau2018_dTh4");
  TH1F *MatchBX0DoubleMuIncPlateau2018_dTh4Log = (TH1F*)SMLogRecoPtMatchBX0DoubleMuIncPlateau->Clone("MatchBX0DoubleMuIncPlateau2018_dTh4Log");
  TH1F *MatchBX0DoubleMuIncUnique2018_dTh4 = (TH1F*)SMRecoPtUniqueMatchBX0DoubleMuInc->Clone("MatchBX0DoubleMuIncUnique2018_dTh4");
  TH1F *MatchBX0DoubleMuIncUnique2018_dTh4Log = (TH1F*)SMLogRecoPtUniqueMatchBX0DoubleMuInc->Clone("MatchBX0DoubleMuIncUnique2018_dTh4Log");
	
  TH1F *MatchBX0MuOpenInc2018_dTh4 = (TH1F*)SMRecoPtMatchBX0MuOpenInc->Clone("MatchBX0MuOpenInc2018_dTh4");
  TH1F *MatchBX0MuOpenInc2018_dTh4Log = (TH1F*)SMLogRecoPtMatchBX0MuOpenInc->Clone("MatchBX0MuOpenInc2018_dTh4Log");
  TH1F *MatchBX0MuOpenIncPlateau2018_dTh4 = (TH1F*)SMRecoPtMatchBX0MuOpenIncPlateau->Clone("MatchBX0MuOpenIncPlateau2018_dTh4");
  TH1F *MatchBX0MuOpenIncPlateau2018_dTh4Log = (TH1F*)SMLogRecoPtMatchBX0MuOpenIncPlateau->Clone("MatchBX0MuOpenIncPlateau2018_dTh4Log");
  TH1F *MatchBX0MuOpenIncUnique2018_dTh4 = (TH1F*)SMRecoPtUniqueMatchBX0MuOpenInc->Clone("MatchBX0MuOpenIncUnique2018_dTh4");
  TH1F *MatchBX0MuOpenIncUnique2018_dTh4Log = (TH1F*)SMLogRecoPtUniqueMatchBX0MuOpenInc->Clone("MatchBX0MuOpenIncUnique2018_dTh4Log");
  file1->Close();
	
  TFile *file2 = TFile::Open("2018_dTh6_ModesRateEff_eta_1.25_2.40.root");//3
  TH1F *HRateSingleMu2018_dTh6 = (TH1F*)HRateSingleMu->Clone("HRateSingleMu2018_dTh6");
  TH1F *HRateDoubleMuInc2018_dTh6 = (TH1F*)HRateDoubleMuInc->Clone("HRateDoubleMuInc2018_dTh6");
  TH1F *HRateMuOpenInc2018_dTh6 = (TH1F*)HRateMuOpenInc->Clone("HRateMuOpenInc2018_dTh6");
	
  TH1F *MatchBX0SingleMu2018_dTh6 = (TH1F*)SMRecoPtMatchBX0SingleMu->Clone("MatchBX0SingleMu2018_dTh6");
  TH1F *MatchBX0SingleMu2018_dTh6Log = (TH1F*)SMLogRecoPtMatchBX0SingleMu->Clone("MatchBX0SingleMu2018_dTh6Log");
  TH1F *MatchBX0SingleMuPlateau2018_dTh6 = (TH1F*)SMRecoPtMatchBX0SingleMuPlateau->Clone("MatchBX0SingleMuPlateau2018_dTh6");
  TH1F *MatchBX0SingleMuPlateau2018_dTh6Log = (TH1F*)SMLogRecoPtMatchBX0SingleMuPlateau->Clone("MatchBX0SingleMuPlateau2018_dTh6Log");
  TH1F *MatchBX0SingleMuUnique2018_dTh6 = (TH1F*)SMRecoPtUniqueMatchBX0SingleMu->Clone("MatchBX0SingleMuUnique2018_dTh6");
  TH1F *MatchBX0SingleMuUnique2018_dTh6Log = (TH1F*)SMLogRecoPtUniqueMatchBX0SingleMu->Clone("MatchBX0SingleMuUnique2018_dTh6Log");

  TH1F *MatchBX0DoubleMuInc2018_dTh6 = (TH1F*)SMRecoPtMatchBX0DoubleMuInc->Clone("MatchBX0DoubleMuInc2018_dTh6");
  TH1F *MatchBX0DoubleMuInc2018_dTh6Log = (TH1F*)SMLogRecoPtMatchBX0DoubleMuInc->Clone("MatchBX0DoubleMuInc2018_dTh6Log");
  TH1F *MatchBX0DoubleMuIncPlateau2018_dTh6 = (TH1F*)SMRecoPtMatchBX0DoubleMuIncPlateau->Clone("MatchBX0DoubleMuIncPlateau2018_dTh6");
  TH1F *MatchBX0DoubleMuIncPlateau2018_dTh6Log = (TH1F*)SMLogRecoPtMatchBX0DoubleMuIncPlateau->Clone("MatchBX0DoubleMuIncPlateau2018_dTh6Log");
  TH1F *MatchBX0DoubleMuIncUnique2018_dTh6 = (TH1F*)SMRecoPtUniqueMatchBX0DoubleMuInc->Clone("MatchBX0DoubleMuIncUnique2018_dTh6");
  TH1F *MatchBX0DoubleMuIncUnique2018_dTh6Log = (TH1F*)SMLogRecoPtUniqueMatchBX0DoubleMuInc->Clone("MatchBX0DoubleMuIncUnique2018_dTh6Log");
	
  TH1F *MatchBX0MuOpenInc2018_dTh6 = (TH1F*)SMRecoPtMatchBX0MuOpenInc->Clone("MatchBX0MuOpenInc2018_dTh6");
  TH1F *MatchBX0MuOpenInc2018_dTh6Log = (TH1F*)SMLogRecoPtMatchBX0MuOpenInc->Clone("MatchBX0MuOpenInc2018_dTh6Log");
  TH1F *MatchBX0MuOpenIncPlateau2018_dTh6 = (TH1F*)SMRecoPtMatchBX0MuOpenIncPlateau->Clone("MatchBX0MuOpenIncPlateau2018_dTh6");
  TH1F *MatchBX0MuOpenIncPlateau2018_dTh6Log = (TH1F*)SMLogRecoPtMatchBX0MuOpenIncPlateau->Clone("MatchBX0MuOpenIncPlateau2018_dTh6Log");
  TH1F *MatchBX0MuOpenIncUnique2018_dTh6 = (TH1F*)SMRecoPtUniqueMatchBX0MuOpenInc->Clone("MatchBX0MuOpenIncUnique2018_dTh6");
  TH1F *MatchBX0MuOpenIncUnique2018_dTh6Log = (TH1F*)SMLogRecoPtUniqueMatchBX0MuOpenInc->Clone("MatchBX0MuOpenIncUnique2018_dTh6Log");
  file2->Close();
	
  TFile *file3 = TFile::Open("2018_dTh8_ModesRateEff_eta_1.25_2.40.root");//4
  TH1F *HRateSingleMu2018_dTh8 = (TH1F*)HRateSingleMu->Clone("HRateSingleMu2018_dTh8");
  TH1F *HRateDoubleMuInc2018_dTh8 = (TH1F*)HRateDoubleMuInc->Clone("HRateDoubleMuInc2018_dTh8");
  TH1F *HRateMuOpenInc2018_dTh8 = (TH1F*)HRateMuOpenInc->Clone("HRateMuOpenInc2018_dTh8");
	
  TH1F *MatchBX0SingleMu2018_dTh8 = (TH1F*)SMRecoPtMatchBX0SingleMu->Clone("MatchBX0SingleMu2018_dTh8");
  TH1F *MatchBX0SingleMu2018_dTh8Log = (TH1F*)SMLogRecoPtMatchBX0SingleMu->Clone("MatchBX0SingleMu2018_dTh8Log");
  TH1F *MatchBX0SingleMuPlateau2018_dTh8 = (TH1F*)SMRecoPtMatchBX0SingleMuPlateau->Clone("MatchBX0SingleMuPlateau2018_dTh8");
  TH1F *MatchBX0SingleMuPlateau2018_dTh8Log = (TH1F*)SMLogRecoPtMatchBX0SingleMuPlateau->Clone("MatchBX0SingleMuPlateau2018_dTh8Log");
  TH1F *MatchBX0SingleMuUnique2018_dTh8 = (TH1F*)SMRecoPtUniqueMatchBX0SingleMu->Clone("MatchBX0SingleMuUnique2018_dTh8");
  TH1F *MatchBX0SingleMuUnique2018_dTh8Log = (TH1F*)SMLogRecoPtUniqueMatchBX0SingleMu->Clone("MatchBX0SingleMuUnique2018_dTh8Log");

  TH1F *MatchBX0DoubleMuInc2018_dTh8 = (TH1F*)SMRecoPtMatchBX0DoubleMuInc->Clone("MatchBX0DoubleMuInc2018_dTh8");
  TH1F *MatchBX0DoubleMuInc2018_dTh8Log = (TH1F*)SMLogRecoPtMatchBX0DoubleMuInc->Clone("MatchBX0DoubleMuInc2018_dTh8Log");
  TH1F *MatchBX0DoubleMuIncPlateau2018_dTh8 = (TH1F*)SMRecoPtMatchBX0DoubleMuIncPlateau->Clone("MatchBX0DoubleMuIncPlateau2018_dTh8");
  TH1F *MatchBX0DoubleMuIncPlateau2018_dTh8Log = (TH1F*)SMLogRecoPtMatchBX0DoubleMuIncPlateau->Clone("MatchBX0DoubleMuIncPlateau2018_dTh8Log");
  TH1F *MatchBX0DoubleMuIncUnique2018_dTh8 = (TH1F*)SMRecoPtUniqueMatchBX0DoubleMuInc->Clone("MatchBX0DoubleMuIncUnique2018_dTh8");
  TH1F *MatchBX0DoubleMuIncUnique2018_dTh8Log = (TH1F*)SMLogRecoPtUniqueMatchBX0DoubleMuInc->Clone("MatchBX0DoubleMuIncUnique2018_dTh8Log");
	
  TH1F *MatchBX0MuOpenInc2018_dTh8 = (TH1F*)SMRecoPtMatchBX0MuOpenInc->Clone("MatchBX0MuOpenInc2018_dTh8");
  TH1F *MatchBX0MuOpenInc2018_dTh8Log = (TH1F*)SMLogRecoPtMatchBX0MuOpenInc->Clone("MatchBX0MuOpenInc2018_dTh8Log");
  TH1F *MatchBX0MuOpenIncPlateau2018_dTh8 = (TH1F*)SMRecoPtMatchBX0MuOpenIncPlateau->Clone("MatchBX0MuOpenIncPlateau2018_dTh8");
  TH1F *MatchBX0MuOpenIncPlateau2018_dTh8Log = (TH1F*)SMLogRecoPtMatchBX0MuOpenIncPlateau->Clone("MatchBX0MuOpenIncPlateau2018_dTh8Log");
  TH1F *MatchBX0MuOpenIncUnique2018_dTh8 = (TH1F*)SMRecoPtUniqueMatchBX0MuOpenInc->Clone("MatchBX0MuOpenIncUnique2018_dTh8");
  TH1F *MatchBX0MuOpenIncUnique2018_dTh8Log = (TH1F*)SMLogRecoPtUniqueMatchBX0MuOpenInc->Clone("MatchBX0MuOpenIncUnique2018_dTh8Log");
  file3->Close();
	
  TFile *file4 = TFile::Open("2017_dBX_ModesRateEff_eta_1.25_2.40.root");//6
  TH1F *HRateSingleMu2017_dBX = (TH1F*)HRateSingleMu->Clone("HRateSingleMu2017_dBX");
  TH1F *HRateDoubleMuInc2017_dBX = (TH1F*)HRateDoubleMuInc->Clone("HRateDoubleMuInc2017_dBX");
  TH1F *HRateMuOpenInc2017_dBX = (TH1F*)HRateMuOpenInc->Clone("HRateMuOpenInc2017_dBX");
	
  TH1F *MatchBX0SingleMu2017_dBX = (TH1F*)SMRecoPtMatchBX0SingleMu->Clone("MatchBX0SingleMu2017_dBX");
  TH1F *MatchBX0SingleMu2017_dBXLog = (TH1F*)SMLogRecoPtMatchBX0SingleMu->Clone("MatchBX0SingleMu2017_dBXLog");
  TH1F *MatchBX0SingleMuPlateau2017_dBX = (TH1F*)SMRecoPtMatchBX0SingleMuPlateau->Clone("MatchBX0SingleMuPlateau2017_dBX");
  TH1F *MatchBX0SingleMuPlateau2017_dBXLog = (TH1F*)SMLogRecoPtMatchBX0SingleMuPlateau->Clone("MatchBX0SingleMuPlateau2017_dBXLog");
  TH1F *MatchBX0SingleMuUnique2017_dBX = (TH1F*)SMRecoPtUniqueMatchBX0SingleMu->Clone("MatchBX0SingleMuUnique2017_dBX");
  TH1F *MatchBX0SingleMuUnique2017_dBXLog = (TH1F*)SMLogRecoPtUniqueMatchBX0SingleMu->Clone("MatchBX0SingleMuUnique2017_dBXLog");

  TH1F *MatchBX0DoubleMuInc2017_dBX = (TH1F*)SMRecoPtMatchBX0DoubleMuInc->Clone("MatchBX0DoubleMuInc2017_dBX");
  TH1F *MatchBX0DoubleMuInc2017_dBXLog = (TH1F*)SMLogRecoPtMatchBX0DoubleMuInc->Clone("MatchBX0DoubleMuInc2017_dBXLog");
  TH1F *MatchBX0DoubleMuIncPlateau2017_dBX = (TH1F*)SMRecoPtMatchBX0DoubleMuIncPlateau->Clone("MatchBX0DoubleMuIncPlateau2017_dBX");
  TH1F *MatchBX0DoubleMuIncPlateau2017_dBXLog = (TH1F*)SMLogRecoPtMatchBX0DoubleMuIncPlateau->Clone("MatchBX0DoubleMuIncPlateau2017_dBXLog");
  TH1F *MatchBX0DoubleMuIncUnique2017_dBX = (TH1F*)SMRecoPtUniqueMatchBX0DoubleMuInc->Clone("MatchBX0DoubleMuIncUnique2017_dBX");
  TH1F *MatchBX0DoubleMuIncUnique2017_dBXLog = (TH1F*)SMLogRecoPtUniqueMatchBX0DoubleMuInc->Clone("MatchBX0DoubleMuIncUnique2017_dBXLog");
	
  TH1F *MatchBX0MuOpenInc2017_dBX = (TH1F*)SMRecoPtMatchBX0MuOpenInc->Clone("MatchBX0MuOpenInc2017_dBX");
  TH1F *MatchBX0MuOpenInc2017_dBXLog = (TH1F*)SMLogRecoPtMatchBX0MuOpenInc->Clone("MatchBX0MuOpenInc2017_dBXLog");
  TH1F *MatchBX0MuOpenIncPlateau2017_dBX = (TH1F*)SMRecoPtMatchBX0MuOpenIncPlateau->Clone("MatchBX0MuOpenIncPlateau2017_dBX");
  TH1F *MatchBX0MuOpenIncPlateau2017_dBXLog = (TH1F*)SMLogRecoPtMatchBX0MuOpenIncPlateau->Clone("MatchBX0MuOpenIncPlateau2017_dBXLog");
  TH1F *MatchBX0MuOpenIncUnique2017_dBX = (TH1F*)SMRecoPtUniqueMatchBX0MuOpenInc->Clone("MatchBX0MuOpenIncUnique2017_dBX");
  TH1F *MatchBX0MuOpenIncUnique2017_dBXLog = (TH1F*)SMLogRecoPtUniqueMatchBX0MuOpenInc->Clone("MatchBX0MuOpenIncUnique2017_dBXLog");
  file4->Close();
	
  TFile *file5 = TFile::Open("2017_dBX_2018Quality_ModesRateEff_eta_1.25_2.40.root");
  TH1F *HRateSingleMu2017_dBX_2018Quality = (TH1F*)HRateSingleMu->Clone("HRateSingleMu2017_dBX_2018Quality");
  TH1F *HRateDoubleMuInc2017_dBX_2018Quality = (TH1F*)HRateDoubleMuInc->Clone("HRateDoubleMuInc2017_dBX_2018Quality");
  TH1F *HRateMuOpenInc2017_dBX_2018Quality = (TH1F*)HRateMuOpenInc->Clone("HRateMuOpenInc2017_dBX_2018Quality");
  file5->Close();
	
  //write to output file
  outFile = "Overlay.root";
  TFile myPlot(outFile,"RECREATE");
  
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
	
  C1->cd();
  HRateSingleMu2017->Draw("HIST");
  HRateSingleMu2018_dTh4->Draw("HISTSAME");
  HRateSingleMu2018_dTh6->Draw("HISTSAME");
  HRateSingleMu2018_dTh8->Draw("HISTSAME");
  C1->Write();
	
  C2->cd();
  HRateDoubleMuInc2017->Draw("HIST");
  HRateDoubleMuInc2018_dTh4->Draw("HISTSAME");
  HRateDoubleMuInc2018_dTh6->Draw("HISTSAME");
  HRateDoubleMuInc2018_dTh8->Draw("HISTSAME");
  C2->Write();
	
  C3->cd();
  HRateMuOpenInc2017->Draw("HIST");
  HRateMuOpenInc2018_dTh4->Draw("HISTSAME");
  HRateMuOpenInc2018_dTh6->Draw("HISTSAME");
  HRateMuOpenInc2018_dTh8->Draw("HISTSAME");
  C3->Write();
	
  C4->cd();
  HRateSingleMu2018_dTh4->Divide(HRateSingleMu2017);
  HRateSingleMu2018_dTh4->Draw("HIST");
  HRateSingleMu2018_dTh6->Divide(HRateSingleMu2017);
  HRateSingleMu2018_dTh6->Draw("HISTSAME");
  HRateSingleMu2018_dTh8->Divide(HRateSingleMu2017);
  HRateSingleMu2018_dTh8->Draw("HISTSAME");
  HRateSingleMu2017_dBX->Divide(HRateSingleMu2017);
  HRateSingleMu2017_dBX->Draw("HISTSAME");
  C4->Write();

  C5->cd();
  HRateDoubleMuInc2018_dTh4->Divide(HRateDoubleMuInc2017);
  HRateDoubleMuInc2018_dTh4->Draw("HIST");
  HRateDoubleMuInc2018_dTh6->Divide(HRateDoubleMuInc2017);
  HRateDoubleMuInc2018_dTh6->Draw("HISTSAME");
  HRateDoubleMuInc2018_dTh8->Divide(HRateDoubleMuInc2017);
  HRateDoubleMuInc2018_dTh8->Draw("HISTSAME");
  HRateDoubleMuInc2017_dBX->Divide(HRateDoubleMuInc2017);
  HRateDoubleMuInc2017_dBX->Draw("HISTSAME");
  HRateDoubleMuInc2017_dBX_2018Quality->Divide(HRateDoubleMuInc2017);
  HRateDoubleMuInc2017_dBX_2018Quality->Draw("HISTSAME");//2017 dBX with 2018 mu quality
  C5->Write();
	
  C6->cd();
  HRateMuOpenInc2018_dTh4->Divide(HRateMuOpenInc2017);
  HRateMuOpenInc2018_dTh4->Draw("HIST");
  HRateMuOpenInc2018_dTh6->Divide(HRateMuOpenInc2017);
  HRateMuOpenInc2018_dTh6->Draw("HISTSAME");
  HRateMuOpenInc2018_dTh8->Divide(HRateMuOpenInc2017);
  HRateMuOpenInc2018_dTh8->Draw("HISTSAME");
  HRateMuOpenInc2017_dBX->Divide(HRateMuOpenInc2017);
  HRateMuOpenInc2017_dBX->Draw("HISTSAME");
  C6->Write();
	
  C7->cd();
  MatchBX0SingleMu2017->Draw();
  MatchBX0SingleMu2018_dTh4->Draw("SAME");
  MatchBX0SingleMu2018_dTh6->Draw("SAME");
  MatchBX0SingleMu2018_dTh8->Draw("SAME");
  C7->Write();
	
  C8->cd();
  MatchBX0DoubleMuInc2017->Draw();
  MatchBX0DoubleMuInc2018_dTh4->Draw("SAME");
  MatchBX0DoubleMuInc2018_dTh6->Draw("SAME");
  MatchBX0DoubleMuInc2018_dTh8->Draw("SAME");
  C8->Write();

  C9->cd();
  MatchBX0MuOpenInc2017->Draw();
  MatchBX0MuOpenInc2018_dTh4->Draw("SAME");
  MatchBX0MuOpenInc2018_dTh6->Draw("SAME");
  MatchBX0MuOpenInc2018_dTh8->Draw("SAME");
  C9->Write();
	
  C10->cd();
  MatchBX0SingleMu2017Log->Draw();
  MatchBX0SingleMu2018_dTh4Log->Draw("SAME");
  MatchBX0SingleMu2018_dTh6Log->Draw("SAME");
  MatchBX0SingleMu2018_dTh8Log->Draw("SAME");
  C10->Write();
	
  C11->cd();
  MatchBX0DoubleMuInc2017Log->Draw();
  MatchBX0DoubleMuInc2018_dTh4Log->Draw("SAME");
  MatchBX0DoubleMuInc2018_dTh6Log->Draw("SAME");
  MatchBX0DoubleMuInc2018_dTh8Log->Draw("SAME");
  C11->Write();

  C12->cd();
  MatchBX0MuOpenInc2017Log->Draw();
  MatchBX0MuOpenInc2018_dTh4Log->Draw("SAME");
  MatchBX0MuOpenInc2018_dTh6Log->Draw("SAME");
  MatchBX0MuOpenInc2018_dTh8Log->Draw("SAME");
  C12->Write();	

  myPlot.Close();
  
} // End function: void 
