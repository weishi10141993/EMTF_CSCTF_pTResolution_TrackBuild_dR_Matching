#include "TFile.h"
#include "TSystem.h"
#include "TChain.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH2.h"
#include "TH1.h"
#include "TCanvas.h"

void PlotOverlayV1() {
	
  TFile *file0 = TFile::Open("ModesRateEff_2018Default.root");//1
  TH1F *HRateSingleMufile0 = (TH1F*)file0->Get("HRateSingleMu")->Clone("HRateSingleMufile0");
  TH1F *HRateDoubleMuIncfile0 = (TH1F*)file0->Get("HRateDoubleMuInc")->Clone("HRateDoubleMuIncfile0");
  TH1F *HRateMuOpenIncfile0 = (TH1F*)file0->Get("HRateMuOpenInc")->Clone("HRateMuOpenIncfile0");

  TH1F *HRateSingleMuNormfile0 = (TH1F*)file0->Get("HRateSingleMuNorm")->Clone("HRateSingleMuNormfile0");
  TH1F *HRateDoubleMuIncNormfile0 = (TH1F*)file0->Get("HRateDoubleMuIncNorm")->Clone("HRateDoubleMuIncNormfile0");
  TH1F *HRateMuOpenIncNormfile0 = (TH1F*)file0->Get("HRateMuOpenIncNorm")->Clone("HRateMuOpenIncNormfile0");
	
  TH1F *MatchBX0SingleMufile0 = (TH1F*)file0->Get("SMRecoPtMatchBX0SingleMu")->Clone("MatchBX0SingleMufile0");
  TH1F *MatchBX0SingleMufile0Log = (TH1F*)file0->Get("SMLogRecoPtMatchBX0SingleMu")->Clone("MatchBX0SingleMufile0Log");
  TH1F *MatchBX0SingleMuPlateaufile0 = (TH1F*)file0->Get("SMRecoPtMatchBX0SingleMuPlateau")->Clone("MatchBX0SingleMuPlateaufile0");
  TH1F *MatchBX0SingleMuPlateaufile0Log = (TH1F*)file0->Get("SMLogRecoPtMatchBX0SingleMuPlateau")->Clone("MatchBX0SingleMuPlateaufile0Log");
  TH1F *MatchBX0SingleMuUniquefile0 = (TH1F*)file0->Get("SMRecoPtUniqueMatchBX0SingleMu")->Clone("MatchBX0SingleMuUniquefile0");
  TH1F *MatchBX0SingleMuUniquefile0Log = (TH1F*)file0->Get("SMLogRecoPtUniqueMatchBX0SingleMu")->Clone("MatchBX0SingleMuUniquefile0Log");

  TH1F *MatchBX0DoubleMuIncfile0 = (TH1F*)file0->Get("SMRecoPtMatchBX0DoubleMuInc")->Clone("MatchBX0DoubleMuIncfile0");
  TH1F *MatchBX0DoubleMuIncfile0Log = (TH1F*)file0->Get("SMLogRecoPtMatchBX0DoubleMuInc")->Clone("MatchBX0DoubleMuIncfile0Log");
  TH1F *MatchBX0DoubleMuIncPlateaufile0 = (TH1F*)file0->Get("SMRecoPtMatchBX0DoubleMuIncPlateau")->Clone("MatchBX0DoubleMuIncPlateaufile0");
  TH1F *MatchBX0DoubleMuIncPlateaufile0Log = (TH1F*)file0->Get("SMLogRecoPtMatchBX0DoubleMuIncPlateau")->Clone("MatchBX0DoubleMuIncPlateaufile0Log");
  TH1F *MatchBX0DoubleMuIncUniquefile0 = (TH1F*)file0->Get("SMRecoPtUniqueMatchBX0DoubleMuInc")->Clone("MatchBX0DoubleMuIncUniquefile0");
  TH1F *MatchBX0DoubleMuIncUniquefile0Log = (TH1F*)file0->Get("SMLogRecoPtUniqueMatchBX0DoubleMuInc")->Clone("MatchBX0DoubleMuIncUniquefile0Log");
	
  TH1F *MatchBX0MuOpenIncfile0 = (TH1F*)file0->Get("SMRecoPtMatchBX0MuOpenInc")->Clone("MatchBX0MuOpenIncfile0");
  TH1F *MatchBX0MuOpenIncfile0Log = (TH1F*)file0->Get("SMLogRecoPtMatchBX0MuOpenInc")->Clone("MatchBX0MuOpenIncfile0Log");
  TH1F *MatchBX0MuOpenIncPlateaufile0 = (TH1F*)file0->Get("SMRecoPtMatchBX0MuOpenIncPlateau")->Clone("MatchBX0MuOpenIncPlateaufile0");
  TH1F *MatchBX0MuOpenIncPlateaufile0Log = (TH1F*)file0->Get("SMLogRecoPtMatchBX0MuOpenIncPlateau")->Clone("MatchBX0MuOpenIncPlateaufile0Log");
  TH1F *MatchBX0MuOpenIncUniquefile0 = (TH1F*)file0->Get("SMRecoPtUniqueMatchBX0MuOpenInc")->Clone("MatchBX0MuOpenIncUniquefile0");
  TH1F *MatchBX0MuOpenIncUniquefile0Log = (TH1F*)file0->Get("SMLogRecoPtUniqueMatchBX0MuOpenInc")->Clone("MatchBX0MuOpenIncUniquefile0Log");	
	
  TFile *file1 = TFile::Open("ModesRateEff_Sep2018Fix.root");//2
  TH1F *HRateSingleMufile1 = (TH1F*)file1->Get("HRateSingleMu")->Clone("HRateSingleMufile1");
  TH1F *HRateDoubleMuIncfile1 = (TH1F*)file1->Get("HRateDoubleMuInc")->Clone("HRateDoubleMuIncfile1");
  TH1F *HRateMuOpenIncfile1 = (TH1F*)file1->Get("HRateMuOpenInc")->Clone("HRateMuOpenIncfile1");

  TH1F *HRateSingleMuNormfile1 = (TH1F*)file1->Get("HRateSingleMuNorm")->Clone("HRateSingleMuNormfile1");
  TH1F *HRateDoubleMuIncNormfile1 = (TH1F*)file1->Get("HRateDoubleMuIncNorm")->Clone("HRateDoubleMuIncNormfile1");
  TH1F *HRateMuOpenIncNormfile1 = (TH1F*)file1->Get("HRateMuOpenIncNorm")->Clone("HRateMuOpenIncNormfile1");
	
  TH1F *MatchBX0SingleMufile1 = (TH1F*)file1->Get("SMRecoPtMatchBX0SingleMu")->Clone("MatchBX0SingleMufile1");
  TH1F *MatchBX0SingleMufile1Log = (TH1F*)file1->Get("SMLogRecoPtMatchBX0SingleMu")->Clone("MatchBX0SingleMufile1Log");
  TH1F *MatchBX0SingleMuPlateaufile1 = (TH1F*)file1->Get("SMRecoPtMatchBX0SingleMuPlateau")->Clone("MatchBX0SingleMuPlateaufile1");
  TH1F *MatchBX0SingleMuPlateaufile1Log = (TH1F*)file1->Get("SMLogRecoPtMatchBX0SingleMuPlateau")->Clone("MatchBX0SingleMuPlateaufile1Log");
  TH1F *MatchBX0SingleMuUniquefile1 = (TH1F*)file1->Get("SMRecoPtUniqueMatchBX0SingleMu")->Clone("MatchBX0SingleMuUniquefile1");
  TH1F *MatchBX0SingleMuUniquefile1Log = (TH1F*)file1->Get("SMLogRecoPtUniqueMatchBX0SingleMu")->Clone("MatchBX0SingleMuUniquefile1Log");

  TH1F *MatchBX0DoubleMuIncfile1 = (TH1F*)file1->Get("SMRecoPtMatchBX0DoubleMuInc")->Clone("MatchBX0DoubleMuIncfile1");
  TH1F *MatchBX0DoubleMuIncfile1Log = (TH1F*)file1->Get("SMLogRecoPtMatchBX0DoubleMuInc")->Clone("MatchBX0DoubleMuIncfile1Log");
  TH1F *MatchBX0DoubleMuIncPlateaufile1 = (TH1F*)file1->Get("SMRecoPtMatchBX0DoubleMuIncPlateau")->Clone("MatchBX0DoubleMuIncPlateaufile1");
  TH1F *MatchBX0DoubleMuIncPlateaufile1Log = (TH1F*)file1->Get("SMLogRecoPtMatchBX0DoubleMuIncPlateau")->Clone("MatchBX0DoubleMuIncPlateaufile1Log");
  TH1F *MatchBX0DoubleMuIncUniquefile1 = (TH1F*)file1->Get("SMRecoPtUniqueMatchBX0DoubleMuInc")->Clone("MatchBX0DoubleMuIncUniquefile1");
  TH1F *MatchBX0DoubleMuIncUniquefile1Log = (TH1F*)file1->Get("SMLogRecoPtUniqueMatchBX0DoubleMuInc")->Clone("MatchBX0DoubleMuIncUniquefile1Log");
	
  TH1F *MatchBX0MuOpenIncfile1 = (TH1F*)file1->Get("SMRecoPtMatchBX0MuOpenInc")->Clone("MatchBX0MuOpenIncfile1");
  TH1F *MatchBX0MuOpenIncfile1Log = (TH1F*)file1->Get("SMLogRecoPtMatchBX0MuOpenInc")->Clone("MatchBX0MuOpenIncfile1Log");
  TH1F *MatchBX0MuOpenIncPlateaufile1 = (TH1F*)file1->Get("SMRecoPtMatchBX0MuOpenIncPlateau")->Clone("MatchBX0MuOpenIncPlateaufile1");
  TH1F *MatchBX0MuOpenIncPlateaufile1Log = (TH1F*)file1->Get("SMLogRecoPtMatchBX0MuOpenIncPlateau")->Clone("MatchBX0MuOpenIncPlateaufile1Log");
  TH1F *MatchBX0MuOpenIncUniquefile1 = (TH1F*)file1->Get("SMRecoPtUniqueMatchBX0MuOpenInc")->Clone("MatchBX0MuOpenIncUniquefile1");
  TH1F *MatchBX0MuOpenIncUniquefile1Log = (TH1F*)file1->Get("SMLogRecoPtUniqueMatchBX0MuOpenInc")->Clone("MatchBX0MuOpenIncUniquefile1Log");
	
  //write to output file
  TFile myPlot("Overlay.root","RECREATE");
  
  TCanvas *C1=new TCanvas("C1","Rate: SingleMu",700,500);
  TCanvas *C2=new TCanvas("C2","Rate: DoubleMu Inclusive",700,500);
  TCanvas *C3=new TCanvas("C3","Rate: MuOpen Inclusive",700,500);
  TCanvas *C1N=new TCanvas("C1N","RateNormalized: SingleMu",700,500);
  TCanvas *C2N=new TCanvas("C2N","RateNormalized: DoubleMu Inclusive",700,500);
  TCanvas *C3N=new TCanvas("C3N","RateNormalized: MuOpen Inclusive",700,500);

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
  HRateSingleMufile0->SetLineColor(1); HRateSingleMufile0->Draw("HIST");
  HRateSingleMufile1->SetLineColor(2); HRateSingleMufile1->Draw("HISTSAME");
  C1->Write();
	
  C2->cd();
  HRateDoubleMuIncfile0->SetLineColor(1); HRateDoubleMuIncfile0->Draw("HIST");
  HRateDoubleMuIncfile1->SetLineColor(2); HRateDoubleMuIncfile1->Draw("HISTSAME");
  C2->Write();
	
  C3->cd();
  HRateMuOpenIncfile0->SetLineColor(1); HRateMuOpenIncfile0->Draw("HIST");
  HRateMuOpenIncfile1->SetLineColor(2); HRateMuOpenIncfile1->Draw("HISTSAME");
  C3->Write();

  C1N->cd();
  HRateSingleMuNormfile0->SetLineColor(1); HRateSingleMuNormfile0->Draw("HIST");
  HRateSingleMuNormfile1->SetLineColor(2); HRateSingleMuNormfile1->Draw("HISTSAME");
  C1N->Write();

  C2N->cd();
  HRateDoubleMuIncNormfile0->SetLineColor(1); HRateDoubleMuIncNormfile0->Draw("HIST");
  HRateDoubleMuIncNormfile1->SetLineColor(2); HRateDoubleMuIncNormfile1->Draw("HISTSAME");
  C2N->Write();

  C3N->cd();
  HRateMuOpenIncNormfile0->SetLineColor(1); HRateMuOpenIncNormfile0->Draw("HIST");
  HRateMuOpenIncNormfile1->SetLineColor(2); HRateMuOpenIncNormfile1->Draw("HISTSAME");
  C3N->Write();
	
  C4->cd();
  HRateSingleMufile1->Divide(HRateSingleMufile0);
  HRateSingleMufile1->SetLineColor(2); HRateSingleMufile1->Draw("HIST");
  C4->Write();

  C5->cd();
  HRateDoubleMuIncfile1->Divide(HRateDoubleMuIncfile0);
  HRateDoubleMuIncfile1->SetLineColor(2); HRateDoubleMuIncfile1->Draw("HIST");
  C5->Write();
	
  C6->cd();
  HRateMuOpenIncfile1->Divide(HRateMuOpenIncfile0);
  HRateMuOpenIncfile1->SetLineColor(2); HRateMuOpenIncfile1->Draw("HIST");
  C6->Write();
	
  C7->cd();
  MatchBX0SingleMufile0->SetLineColor(1); MatchBX0SingleMufile0->Draw();
  MatchBX0SingleMufile1->SetLineColor(2); MatchBX0SingleMufile1->Draw("SAME");
  C7->Write();
	
  C8->cd();
  MatchBX0DoubleMuIncfile0->SetLineColor(1); MatchBX0DoubleMuIncfile0->Draw();
  MatchBX0DoubleMuIncfile1->SetLineColor(2); MatchBX0DoubleMuIncfile1->Draw("SAME");
  C8->Write();

  C9->cd();
  MatchBX0MuOpenIncfile0->SetLineColor(1); MatchBX0MuOpenIncfile0->Draw();
  MatchBX0MuOpenIncfile1->SetLineColor(2); MatchBX0MuOpenIncfile1->Draw("SAME");
  C9->Write();
	
  C10->cd();
  MatchBX0SingleMufile0Log->SetLineColor(1); MatchBX0SingleMufile0Log->Draw();
  MatchBX0SingleMufile1Log->SetLineColor(2); MatchBX0SingleMufile1Log->Draw("SAME");
  C10->Write();
	
  C11->cd();
  MatchBX0DoubleMuIncfile0Log->SetLineColor(1); MatchBX0DoubleMuIncfile0Log->Draw();
  MatchBX0DoubleMuIncfile1Log->SetLineColor(2); MatchBX0DoubleMuIncfile1Log->Draw("SAME");
  C11->Write();

  C12->cd();
  MatchBX0MuOpenIncfile0Log->SetLineColor(1); MatchBX0MuOpenIncfile0Log->Draw();
  MatchBX0MuOpenIncfile1Log->SetLineColor(2); MatchBX0MuOpenIncfile1Log->Draw("SAME");
  C12->Write();	
	
  C13->cd();
  MatchBX0SingleMuPlateaufile0->SetLineColor(1); MatchBX0SingleMuPlateaufile0->Draw();
  MatchBX0SingleMuPlateaufile1->SetLineColor(2); MatchBX0SingleMuPlateaufile1->Draw("SAME");
  C13->Write();
	
  C14->cd();
  MatchBX0DoubleMuIncPlateaufile0->SetLineColor(1); MatchBX0DoubleMuIncPlateaufile0->Draw();
  MatchBX0DoubleMuIncPlateaufile1->SetLineColor(2); MatchBX0DoubleMuIncPlateaufile1->Draw("SAME");
  C14->Write();

  C15->cd();
  MatchBX0MuOpenIncPlateaufile0->SetLineColor(1); MatchBX0MuOpenIncPlateaufile0->Draw();
  MatchBX0MuOpenIncPlateaufile1->SetLineColor(2); MatchBX0MuOpenIncPlateaufile1->Draw("SAME");
  C15->Write();

  C16->cd();
  MatchBX0SingleMuPlateaufile0Log->SetLineColor(1); MatchBX0SingleMuPlateaufile0Log->Draw();
  MatchBX0SingleMuPlateaufile1Log->SetLineColor(2); MatchBX0SingleMuPlateaufile1Log->Draw("SAME");
  C16->Write();
	
  C17->cd();
  MatchBX0DoubleMuIncPlateaufile0Log->SetLineColor(1); MatchBX0DoubleMuIncPlateaufile0Log->Draw();
  MatchBX0DoubleMuIncPlateaufile1Log->SetLineColor(2); MatchBX0DoubleMuIncPlateaufile1Log->Draw("SAME");
  C17->Write();

  C18->cd();
  MatchBX0MuOpenIncPlateaufile0Log->SetLineColor(1); MatchBX0MuOpenIncPlateaufile0Log->Draw();
  MatchBX0MuOpenIncPlateaufile1Log->SetLineColor(2); MatchBX0MuOpenIncPlateaufile1Log->Draw("SAME");
  C18->Write();
	
  C19->cd();
  MatchBX0SingleMuUniquefile0->SetLineColor(1); MatchBX0SingleMuUniquefile0->Draw();
  MatchBX0SingleMuUniquefile1->SetLineColor(2); MatchBX0SingleMuUniquefile1->Draw("SAME");
  C19->Write();
	
  C20->cd();
  MatchBX0DoubleMuIncUniquefile0->SetLineColor(1); MatchBX0DoubleMuIncUniquefile0->Draw();
  MatchBX0DoubleMuIncUniquefile1->SetLineColor(2); MatchBX0DoubleMuIncUniquefile1->Draw("SAME");
  C20->Write();
	
  C21->cd();
  MatchBX0MuOpenIncUniquefile0->SetLineColor(1); MatchBX0MuOpenIncUniquefile0->Draw();
  MatchBX0MuOpenIncUniquefile1->SetLineColor(2); MatchBX0MuOpenIncUniquefile1->Draw("SAME");
  C21->Write();
	
  C22->cd();
  MatchBX0SingleMuUniquefile0Log->SetLineColor(1); MatchBX0SingleMuUniquefile0Log->Draw();
  MatchBX0SingleMuUniquefile1Log->SetLineColor(2); MatchBX0SingleMuUniquefile1Log->Draw("SAME");
  C22->Write();
	
  C23->cd();
  MatchBX0DoubleMuIncUniquefile0Log->SetLineColor(1); MatchBX0DoubleMuIncUniquefile0Log->Draw();
  MatchBX0DoubleMuIncUniquefile1Log->SetLineColor(2); MatchBX0DoubleMuIncUniquefile1Log->Draw("SAME");
  C23->Write();
	
  C24->cd();
  MatchBX0MuOpenIncUniquefile0Log->SetLineColor(1); MatchBX0MuOpenIncUniquefile0Log->Draw();
  MatchBX0MuOpenIncUniquefile1Log->SetLineColor(2); MatchBX0MuOpenIncUniquefile1Log->Draw("SAME");
  C24->Write();

  myPlot.Close();
  
} // End function: void 
