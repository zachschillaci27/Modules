#include "Analysis.hh"
#include "TGraphErrors.h"
#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <dirent.h>
#include <vector>
#include <time.h>
#include <TAxis.h>
#include <THStack.h>
#include <TMath.h>
#include <IVMeasurementPoint.hh>
#include <TPaletteAxis.h>
#include <TF1.h> 
#include <TApplication.h>
#include <TGClient.h>
#include <TGButton.h>
#include <TGFrame.h>
#include <TGWindow.h>
#include <TString.h>
#include <TGTable.h>
#include <TTreeTableInterface.h>
#include <TFile.h>
#include <TNtuple.h>
#include <TGSimpleTableInterface.h>
#include <TTree.h>
#include <TVirtualTreePlayer.h>
#include <TTreePlayer.h>
#include <TCanvas.h>
#include <sstream>
#include "TLatex.h"

//------------------------------------------
void drawAtlasInternal1D() {
  //write "ATLAS Internal" label on 1D plots

  //position
  float xmin, xmax, ymin, ymax;
  xmin=0.12; xmax=0.9; ymin=0.90; ymax=0.80;

  //ATLAS
  TLatex a;
  a.SetNDC();
  a.SetTextFont(72);
  a.SetTextColor(1);
  a.SetTextSize(0.07);
  a.DrawLatex(xmin,ymax,"ATLAS");

  //internal
  TLatex i;
  i.SetNDC();
  i.SetTextFont(42);
  i.SetTextColor(1);
  i.SetTextSize(0.07);
  i.DrawLatex(xmin+0.16,ymax,"Internal");
}

void drawHistoTitle(std::string title){
  float xmin, xmax, ymin, ymax;
  xmin=0.65; xmax=0.85; ymin=0.60; ymax=0.70;

  //TITLE
  TLatex a;
  a.SetNDC();
  a.SetTextFont(42);
  a.SetTextColor(1);
  a.SetTextSize(0.05);
  a.DrawLatex(xmin,ymax,title.c_str());


}
//------------------------------------------
void drawAtlasInternal2D() {
  //write "ATLAS Internal" label on 2D plots

  //position
  float xmin, xmax, ymin, ymax;
  xmin=0.55; xmax=0.75; ymin=0.70; ymax=0.80;

  //ATLAS
  TLatex a;
  a.SetNDC();
  a.SetTextFont(72);
  a.SetTextColor(1);
  a.SetTextSize(0.07);
  a.DrawLatex(xmin,ymax,"ATLAS");

  //internal
  TLatex i;
  i.SetNDC();
  i.SetTextFont(42);
  i.SetTextColor(1);
  i.SetTextSize(0.07);
  i.DrawLatex(xmin+0.14,ymax,"Internal");
}

//------------------------------------------
void make_nice_plot(TH1D *histo, TString TestID, double min, double max){
  //make 1D histograms look nice

  const Int_t nmodules = 32;

  if (TestID == "Thr_Mean") {
    histo->GetXaxis()->SetTitle("FE ID");
    histo->GetYaxis()->SetTitle("threshold mean [e^{-}]");
    min=2500.;
    max=4000.;
  }
  else if(TestID == "SigmaThr_Mean"){
    histo->GetXaxis()->SetTitle("FE ID");
    histo->GetYaxis()->SetTitle("sigma threshold mean [e^{-}]");
    if(min>0)min=0;
    if(max<350)max=350;
		
  }
  else if(TestID == "ToT_Mean"){
    histo->GetXaxis()->SetTitle("FE ID");
    histo->GetYaxis()->SetTitle("ToT mean [bunch crossings]");
    if(min>0)min=0;
    if(max<10)max=10;
  }

  //style
  gPad->SetTicks(1,1);
  gPad->SetGridy();
  gPad->SetGridx();
  gPad->SetRightMargin(0.02);
  gStyle->SetMarkerStyle(20);
  gStyle->SetMarkerSize(1.2);
  gStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes
  gStyle->SetEndErrorSize(0.);
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);

  //draw
  //x
  histo->GetXaxis()->SetTickLength(0.01);
  histo->GetXaxis()->SetTitleOffset(1.4);

  //y
  histo->GetYaxis()->SetRangeUser(min,max);
  histo->GetYaxis()->SetTickLength(0.01);
  histo->GetYaxis()->SetTitleOffset(1.4);

  //style
  histo->SetMarkerColor(kRed);
  histo->SetMarkerStyle(21);
  histo->LabelsOption("v");
  histo->SetLineWidth(2.);
  histo->Draw("pe1x0");

  //ATLAS internal
  drawAtlasInternal1D();

  /*
    for(int i=0;i<nmodules;i++){
    TLine *line1= new TLine(i+1,min,i+1,max);
    line1->Draw("same");
    }
  */
}

//------------------------------------------
void FicoDraw(TH2* histo){

  //gPad->SetPad(0.01,0.85,0.99,0.99);
  gPad->SetLeftMargin(0.032);
  gPad->SetRightMargin(0.04);
  histo->GetZaxis()->SetTickLength(0.004);
  histo->GetZaxis()->SetLabelSize(0.105);
  //    histoAll->SetNdivisions(16);
  histo->Draw("colz");
  gPad->Update();
  TPaletteAxis *palette_ = (TPaletteAxis*)histo->GetListOfFunctions()->FindObject("palette");
  palette_->SetX2NDC(0.98);
  histo->Draw("colz");

}

//------------------------------------------
void FicoPlot(){
  //make 2D histograms look nice

  const int NRGBs=5;
  const int NCont =255;
  double stops[NRGBs] = {0.00,0.34,0.61,0.84,1.00};
  double red[NRGBs] = {0.00,0.00,0.87,1.00,0.51};
  double green[NRGBs] = {0.00,0.81,1.00,0.20,0.00};
  double blue[NRGBs] ={0.51,1.00,0.12,0.00,0.00};
      
  TColor::CreateGradientColorTable(NRGBs,stops,red,green,blue,NCont);
  gStyle->SetNumberContours(NCont);
  gPad->SetLogz(0);
  gStyle->SetOptStat(0);
  gPad->SetRightMargin(0.15);
}

//------------------------------------------
void ThresModMeanValue(std::vector <HistoReader*> aReader) {
  
  gStyle->SetOptStat(0);
  const Int_t nmodules = 32;
  TString label[nmodules]={"A8_2","A8_1","A7_2","A7_1","A6_2","A6_1","A5_2","A5_1","A4_2","A4_1","A3_2","A3_1","A2_2","A2_1","A1_2","A1_1","C1_1","C1_2","C2_1","C2_2","C3_1","C3_2","C4_1","C4_2","C5_1","C5_2","C6_1","C6_2","C7_1","C7_2","C8_1","C8_2"};
  
  TH1D *histo_thresModMeanValue = new TH1D( "histo_thresModMeanvalue","Threshold Mean Value",32,0,32);
  TString TestID = "Thr_Mean";
  
  double feid[nmodules];
  double feide[nmodules];
  double mean[nmodules];
  double sigma[nmodules];
  
  for(unsigned i=0;i<aReader.size();i++){
    feid[i] = (double)i;
    feide[i] = 0;
    mean[i] = -100;
    sigma[i] = -100;
    if(aReader.at(i)->Thre_threshdist != NULL){
      mean[i] = aReader.at(i)->Thre_threshdist->GetFunction("gauss")->GetParameter(1);
      sigma[i] = aReader.at(i)->Thre_threshdist->GetFunction("gauss")->GetParameter(2);
    }
  }
  
  double mean_max = 0;
  double mean_min = 10000;
  for(int i=0; i<nmodules; i++){
    if(mean[i] > mean_max) mean_max = mean[i];
    if(mean[i] > 0 && mean[i] < mean_min) mean_min = mean[i];
  }
  for (int i=0;i<nmodules; i++){
    histo_thresModMeanValue->SetBinContent(i+1,mean[i]);
    histo_thresModMeanValue->SetBinError(i+1,sigma[i]);
    histo_thresModMeanValue->GetXaxis()->SetBinLabel(i+1,label[i]);
  }
  make_nice_plot(histo_thresModMeanValue,TestID,mean_min*0.8,mean_max*1.2);
}


//------------------------------------------
void ToT_ModMeanValue(std::vector <HistoReader*> aReader) {
  
  gStyle->SetOptStat(0);
  const Int_t nmodules = 32;
  TString label[nmodules]={"A8_2","A8_1","A7_2","A7_1","A6_2","A6_1","A5_2","A5_1","A4_2","A4_1","A3_2","A3_1","A2_2","A2_1","A1_2","A1_1","C1_1","C1_2","C2_1","C2_2","C3_1","C3_2","C4_1","C4_2","C5_1","C5_2","C6_1","C6_2","C7_1","C7_2","C8_1","C8_2"};
  
  TH1D *histo_totModMeanValue = new TH1D( "histo_totModMeanvalue","ToT Mean Value",32,0,32);
  TString TestID = "ToT_Mean";
  
  double feid[nmodules];
  double feide[nmodules];
  double mean[nmodules];
  double sigma[nmodules];
  
  for(unsigned i=0;i<aReader.size();i++){
    feid[i] = (double)i;
    feide[i] = 0;
    mean[i] = -100;
    sigma[i] = -100;
    if(aReader.at(i)->ToT_dist_mod != NULL){
      mean[i] = aReader.at(i)->ToT_dist_mod->GetFunction("gaus")->GetParameter(1);
      sigma[i] = aReader.at(i)->ToT_dist_mod->GetFunction("gaus")->GetParameter(2);
    }
  }
  
  double mean_max = 0;
  double mean_min = 10;
  for(int i=0; i<nmodules; i++){
    if(mean[i] > mean_max) mean_max = mean[i];
    if(mean[i] > 0 && mean[i] < mean_min) mean_min = mean[i];
  }
  for (int i=0;i<nmodules; i++){
    histo_totModMeanValue->SetBinContent(i+1,mean[i]);
    histo_totModMeanValue->SetBinError(i+1,sigma[i]);
    histo_totModMeanValue->GetXaxis()->SetBinLabel(i+1,label[i]);
  }
  make_nice_plot(histo_totModMeanValue,TestID,mean_min,mean_max);
  
}

void ThresModMeanValue(std::vector <HistoReader*> aReader, std::vector <HistoReader*> bReader){
  gStyle->SetOptStat(0);
  const Int_t nmodules = 32;
  TString label[nmodules]={"A8_2","A8_1","A7_2","A7_1","A6_2","A6_1","A5_2","A5_1","A4_2","A4_1","A3_2","A3_1","A2_2","A2_1","A1_2","A1_1","C1_1","C1_2","C2_1","C2_2","C3_1","C3_2","C4_1","C4_2","C5_1","C5_2","C6_1","C6_2","C7_1","C7_2","C8_1","C8_2"};
  
  
  TH1D *histo_thresModMeanValue = new TH1D( "histo_thresModMeanvalue","Threshold Mean Value",32,0,32);
  TH1D *histo_thresModMeanValue_1 = new TH1D( "histo_thresModMeanvalue_1","Threshold Mean Value",32,0,32);
  TString TestID = "Thr_Mean";
  
  double feid[nmodules];
  double feide[nmodules];
  double mean[nmodules];
  double sigma[nmodules];
  double mean2[nmodules];
  double sigma2[nmodules];
  
  for(unsigned i=0;i<aReader.size();i++){
    feid[i] = (double)i;
    feide[i] = 0;
    mean[i] = -100;
    sigma[i] = -100;
    mean2[i] = -100;
    sigma2[i] = -100;
    if(aReader.at(i)->Thre_threshdist != NULL){
      mean[i] = aReader.at(i)->Thre_threshdist->GetFunction("gauss")->GetParameter(1);
      sigma[i] = aReader.at(i)->Thre_threshdist->GetFunction("gauss")->GetParameter(2);
    }
    if(bReader.at(i)->Thre_threshdist != NULL){
      mean2[i] = bReader.at(i)->Thre_threshdist->GetFunction("gauss")->GetParameter(1);
      sigma2[i] = bReader.at(i)->Thre_threshdist->GetFunction("gauss")->GetParameter(2);
    }
  }
  
  double mean_max = 0;
  double mean_min = 10000;
  for(int i=0; i<nmodules; i++){
    if(mean[i] > mean_max) mean_max = mean[i];
    if(mean2[i] > mean_max) mean_max = mean2[i];
    if(mean[i] > 0 && mean[i] < mean_min) mean_min = mean[i];
    if(mean2[i] > 0 && mean2[i] < mean_min) mean_min = mean2[i];
  }
  for(int i=0; i<nmodules; i++){
    histo_thresModMeanValue->SetBinContent(i+1,mean[i]);
    histo_thresModMeanValue->SetBinError(i+1,sigma[i]);
    histo_thresModMeanValue->GetXaxis()->SetBinLabel(i+1,label[i]);
    
    histo_thresModMeanValue_1->SetBinContent(i+1,mean2[i]);
    histo_thresModMeanValue_1->SetBinError(i+1,sigma2[i]);
  }
  
  
  make_nice_plot(histo_thresModMeanValue,TestID,mean_min*0.8,mean_max*1.2);
  histo_thresModMeanValue_1->SetMarkerColor(kBlue);
  histo_thresModMeanValue_1->SetMarkerStyle(21);
  histo_thresModMeanValue_1->Draw("pe1x0same");
	
  TLegend* leg = new TLegend(0.6, 0.7, 0.85, 0.9);
  leg->AddEntry(histo_thresModMeanValue, "Module reception test", "p");
  leg->AddEntry(histo_thresModMeanValue_1, "Stave before thermal cycling", "p");
  leg->Draw();
  
  
}

void ThresModMeanValue(std::vector <HistoReader*> aReader,std::vector <HistoReader*> bReader, std::vector <HistoReader*> cReader){
  gStyle->SetOptStat(0);
  const Int_t nmodules = 32;
  TString label[nmodules]={"A8_2","A8_1","A7_2","A7_1","A6_2","A6_1","A5_2","A5_1","A4_2","A4_1","A3_2","A3_1","A2_2","A2_1","A1_2","A1_1","C1_1","C1_2","C2_1","C2_2","C3_1","C3_2","C4_1","C4_2","C5_1","C5_2","C6_1","C6_2","C7_1","C7_2","C8_1","C8_2"};
  
  TH1D *histo_thresModMeanValue = new TH1D( "histo_thresModMeanvalue","Threshold Mean Value",32,0,32);
  TH1D *histo_thresModMeanValue_1 = new TH1D( "histo_thresModMeanvalue_1","Threshold Mean Value",32,0,32);
  TH1D *histo_thresModMeanValue_2 = new TH1D( "histo_thresModMeanvalue_2","Threshold Mean Value",32,0,32);
  TString TestID = "Thr_Mean";
  
  double feid[nmodules];
  double feide[nmodules];
  double mean[nmodules];
  double sigma[nmodules];
  double mean2[nmodules];
  double sigma2[nmodules];
  double mean3[nmodules];
  double sigma3[nmodules];
  
  for(unsigned i=0;i<aReader.size();i++){
    feid[i] = (double)i;
    feide[i] = 0;
    mean[i] = -100;
    sigma[i] = -100;
    mean2[i] = -100;
    sigma2[i] = -100;
    mean3[i] = -100;
    sigma3[i] = -100;
    if(aReader.at(i)->Thre_threshdist != NULL){
      mean[i] = aReader.at(i)->Thre_threshdist->GetFunction("gauss")->GetParameter(1);
      sigma[i] = aReader.at(i)->Thre_threshdist->GetFunction("gauss")->GetParameter(2);
    }
    if(bReader.at(i)->Thre_threshdist != NULL){
      mean2[i] = bReader.at(i)->Thre_threshdist->GetFunction("gauss")->GetParameter(1);
      sigma2[i] = bReader.at(i)->Thre_threshdist->GetFunction("gauss")->GetParameter(2);
    }
    if(cReader.at(i)->Thre_threshdist != NULL){
      mean3[i] = cReader.at(i)->Thre_threshdist->GetFunction("gauss")->GetParameter(1);
      sigma3[i] = cReader.at(i)->Thre_threshdist->GetFunction("gauss")->GetParameter(2);
    }
    
  }
  
  double mean_max = 0;
  double mean_min = 10000;
  for(int i=0; i<nmodules; i++){
    if(mean[i] > mean_max) mean_max = mean[i];
    if(mean2[i] > mean_max) mean_max = mean2[i];
    if(mean3[i] > mean_max) mean_max = mean3[i];
    if(mean[i] > 0 && mean[i] < mean_min) mean_min = mean[i];
    if(mean2[i] > 0 && mean2[i] < mean_min) mean_min = mean2[i];
    if(mean3[i] > 0 && mean3[i] < mean_min) mean_min = mean3[i];
  }
  for(int i=0; i<nmodules; i++){
    histo_thresModMeanValue->SetBinContent(i+1,mean[i]);
    histo_thresModMeanValue->SetBinError(i+1,sigma[i]);
    histo_thresModMeanValue->GetXaxis()->SetBinLabel(i+1,label[i]);
	  
    histo_thresModMeanValue_1->SetBinContent(i+1,mean2[i]);
    histo_thresModMeanValue_1->SetBinError(i+1,sigma2[i]);
	  
    histo_thresModMeanValue_2->SetBinContent(i+1,mean3[i]);
    histo_thresModMeanValue_2->SetBinError(i+1,sigma3[i]);
	  
  }
	
  make_nice_plot(histo_thresModMeanValue,TestID,mean_min*0.8,mean_max*1.2);
	
  histo_thresModMeanValue_1->SetMarkerColor(kBlue);
  histo_thresModMeanValue_1->SetMarkerStyle(20);
  histo_thresModMeanValue_1->Draw("pe1x0same");
	
  histo_thresModMeanValue_2->SetMarkerColor(kBlack);
  histo_thresModMeanValue_2->SetMarkerStyle(22);
  histo_thresModMeanValue_2->Draw("pe1x0same");
	
  TLegend* leg = new TLegend(0.6, 0.7, 0.85, 0.9);
  leg->AddEntry(histo_thresModMeanValue, "Module reception test", "p");
  leg->AddEntry(histo_thresModMeanValue_1, "Stave before thermal cycling", "p");
  leg->AddEntry(histo_thresModMeanValue_2, "Stave after thermal cycling", "p");
  leg->Draw();
	
}


/************Sigma Mean Value*********************/

void SigmaModMeanValue(std::vector <HistoReader*> aReader){
  gStyle->SetOptStat(0);
  const Int_t nmodules = 32;
  TString label[nmodules]={"A8_2","A8_1","A7_2","A7_1","A6_2","A6_1","A5_2","A5_1","A4_2","A4_1","A3_2","A3_1","A2_2","A2_1","A1_2","A1_1","C1_1","C1_2","C2_1","C2_2","C3_1","C3_2","C4_1","C4_2","C5_1","C5_2","C6_1","C6_2","C7_1","C7_2","C8_1","C8_2"};
	
  
  TH1D *histo_sigmaModMeanValue = new TH1D("histo_sigmaModMeanvalue","Sigma Threshold Mean Value",32,0,32);
  TString TestID = "SigmaThr_Mean";
  
  double feid[nmodules];
  double feide[nmodules];
  double mean[nmodules];
  double sigma[nmodules];
  for(unsigned i=0;i<aReader.size();i++){
    feid[i] = (double)i;
    feide[i] = 0;
    mean[i] = -100;
    sigma[i] = -100;
    if(aReader.at(i)->Thre_sigmadist != NULL && aReader.at(i)->Thre_sigmadist->GetEntries() > 1){
      Fit_noisedistro(aReader.at(i)->Thre_sigmadist);
      mean[i] = aReader.at(i)->Thre_sigmadist->GetFunction("gaus1")->GetParameter(1);
      sigma[i] = aReader.at(i)->Thre_sigmadist->GetFunction("gaus1")->GetParameter(2);
    }
  }
  
  double mean_max = 0;
  double mean_min = 10000;
  for(int i=0; i<nmodules; i++){
    if(mean[i] > mean_max) mean_max = mean[i];
    if(mean[i] > 0 && mean[i] < mean_min) mean_min = mean[i];
  }
  double _m=0,_s=0;
  for(int i=0; i<nmodules; i++){
    histo_sigmaModMeanValue->SetBinContent(i+1,mean[i]);
    histo_sigmaModMeanValue->SetBinError(i+1,sigma[i]);
    histo_sigmaModMeanValue->GetXaxis()->SetBinLabel(i+1,label[i]);
    _m = _m + mean[i];
    _s = _s + sigma[i];
  }
  
  make_nice_plot(histo_sigmaModMeanValue,TestID,mean_min*0.8,mean_max*1.2);
  
  TLegend* leg = new TLegend(0.6, 0.7, 0.85, 0.9);
  leg->AddEntry(histo_sigmaModMeanValue, "Module reception test", "p");
  /*
    double _mm=0,m=0,s=0, _ss=0;
    for(int j=0; j=nmodules; j++){
    _mm = _mm+ mean[j];
    _ss = _ss+ sigma[j];

    }
    m = _mm/32;
    s = _ss/32;*/
  std::cout<<"Noise stave Mean value: "<<_m/32<<"\t Noise mean sigma value"<<_s/32<<std::endl;

  //leg->Draw();
}

void SigmaModMeanValue(std::vector <HistoReader*> aReader,std::vector <HistoReader*> bReader){
  
  gStyle->SetOptStat(0);
  const Int_t nmodules = 32;
  TString label[nmodules]={"A8_2","A8_1","A7_2","A7_1","A6_2","A6_1","A5_2","A5_1","A4_2","A4_1","A3_2","A3_1","A2_2","A2_1","A1_2","A1_1","C1_1","C1_2","C2_1","C2_2","C3_1","C3_2","C4_1","C4_2","C5_1","C5_2","C6_1","C6_2","C7_1","C7_2","C8_1","C8_2"};
	
  TH1D *histo_sigmaModMeanValue = new TH1D("histo_sigmaModMeanvalue","Sigma Threshold Mean Value",32,0,32);
  TH1D *histo_sigmaModMeanValue_1 = new TH1D("histo_sigmaModMeanvalue_1","Sigma Threshold Mean Value",32,0,32);
  TString TestID = "SigmaThr_Mean";
  
  double feid[nmodules];
  double feide[nmodules];
  double mean[nmodules];
  double sigma[nmodules];
  double mean2[nmodules];
  double sigma2[nmodules];
  for(unsigned i=0;i<aReader.size();i++){
    feid[i] = (double)i;
    feide[i] = 0;
    mean[i] = -100;
    sigma[i] = -100;
    if(aReader.at(i)->Thre_sigmadist != NULL  && aReader.at(i)->Thre_sigmadist->GetEntries() > 1){
      Fit_noisedistro(aReader.at(i)->Thre_sigmadist);
      mean[i] = aReader.at(i)->Thre_sigmadist->GetFunction("gaus1")->GetParameter(1);
      sigma[i] = aReader.at(i)->Thre_sigmadist->GetFunction("gaus1")->GetParameter(2);
    }
    if(bReader.at(i)->Thre_sigmadist != NULL  && bReader.at(i)->Thre_sigmadist->GetEntries() > 1){
      Fit_noisedistro(bReader.at(i)->Thre_sigmadist);
      mean2[i] = bReader.at(i)->Thre_sigmadist->GetFunction("gaus1")->GetParameter(1);
      sigma2[i] = bReader.at(i)->Thre_sigmadist->GetFunction("gaus1")->GetParameter(2);
    }
  }
  double mean_max = 0;
  double mean_min = 10000;
  for(int i=0; i<nmodules; i++){
    if(mean[i] > mean_max) mean_max = mean[i];
    if(mean2[i] > mean_max) mean_max = mean2[i];
    if(mean[i] > 0 && mean[i] < mean_min) mean_min = mean[i];
    if(mean2[i] > 0 && mean2[i] < mean_min) mean_min = mean2[i];
  }
  
  for(int i=0; i<nmodules; i++){
    histo_sigmaModMeanValue->SetBinContent(i+1,mean[i]);
    histo_sigmaModMeanValue->SetBinError(i+1,sigma[i]);
    histo_sigmaModMeanValue->GetXaxis()->SetBinLabel(i+1,label[i]);
    
    histo_sigmaModMeanValue_1->SetBinContent(i+1,mean2[i]);
    histo_sigmaModMeanValue_1->SetBinError(i+1,sigma2[i]);
  }
	
	
  make_nice_plot(histo_sigmaModMeanValue,TestID,mean_min*0.8,mean_max*1.2);
	
  histo_sigmaModMeanValue_1->SetMarkerColor(kBlue);
  histo_sigmaModMeanValue_1->SetMarkerStyle(21);
  histo_sigmaModMeanValue_1->Draw("pe1x0same");
	
	
  TLegend* leg = new TLegend(0.6, 0.7, 0.85, 0.9);
  leg->AddEntry(histo_sigmaModMeanValue, "Module reception test", "p");
  leg->AddEntry(histo_sigmaModMeanValue_1, "Stave before thermal cycling", "p");
  leg->Draw();
}


void SigmaModMeanValue(std::vector <HistoReader*> aReader,std::vector <HistoReader*> bReader, std::vector <HistoReader*> cReader){
	
  gStyle->SetOptStat(0);
  const Int_t nmodules = 32;
  TString label[nmodules]={"A8_2","A8_1","A7_2","A7_1","A6_2","A6_1","A5_2","A5_1","A4_2","A4_1","A3_2","A3_1","A2_2","A2_1","A1_2","A1_1","C1_1","C1_2","C2_1","C2_2","C3_1","C3_2","C4_1","C4_2","C5_1","C5_2","C6_1","C6_2","C7_1","C7_2","C8_1","C8_2"};
	
	
  TH1D *histo_sigmaModMeanValue = new TH1D( "histo_sigmaModMeanvalue","Sigma Threshold Mean Value",32,0,32);
  TH1D *histo_sigmaModMeanValue_1 = new TH1D( "histo_sigmaModMeanvalue_1","Sigma Threshold Mean Value",32,0,32);
  TH1D *histo_sigmaModMeanValue_2 = new TH1D( "histo_sigmaModMeanvalue_2","Sigma Threshold Mean Value",32,0,32);
  TString TestID = "SigmaThr_Mean";
	
  double feid[nmodules];
  double feide[nmodules];
  double mean[nmodules];
  double sigma[nmodules];
  double mean2[nmodules];
  double sigma2[nmodules];
  double mean3[nmodules];
  double sigma3[nmodules];
  for(unsigned i=0;i<aReader.size();i++){
    feid[i] = (double)i;
    feide[i] = 0;
    mean[i] = -100;
    sigma[i] = -100;
    if(aReader.at(i)->Thre_sigmadist != NULL  && aReader.at(i)->Thre_sigmadist->GetEntries() > 1){
      Fit_noisedistro(aReader.at(i)->Thre_sigmadist);
      mean[i] = aReader.at(i)->Thre_sigmadist->GetFunction("gaus1")->GetParameter(1);
      sigma[i] = aReader.at(i)->Thre_sigmadist->GetFunction("gaus1")->GetParameter(2);
    }
    if(bReader.at(i)->Thre_sigmadist != NULL  && bReader.at(i)->Thre_sigmadist->GetEntries() > 1){
      Fit_noisedistro(bReader.at(i)->Thre_sigmadist);
      mean2[i] = bReader.at(i)->Thre_sigmadist->GetFunction("gaus1")->GetParameter(1);
      sigma2[i] = bReader.at(i)->Thre_sigmadist->GetFunction("gaus1")->GetParameter(2);
    }
    if(cReader.at(i)->Thre_sigmadist != NULL  && cReader.at(i)->Thre_sigmadist->GetEntries() > 1){
      Fit_noisedistro(cReader.at(i)->Thre_sigmadist);
      mean3[i] = cReader.at(i)->Thre_sigmadist->GetFunction("gaus1")->GetParameter(1);
      sigma3[i] = cReader.at(i)->Thre_sigmadist->GetFunction("gaus1")->GetParameter(2);
    }
		
  }
	
  double mean_max = 0;
  double mean_min = 10000;
  for(int i=0; i<nmodules; i++){
    if(mean[i] > mean_max) mean_max = mean[i];
    if(mean2[i] > mean_max) mean_max = mean2[i];
    if(mean3[i] > mean_max) mean_max = mean3[i];
    if(mean[i] > 0 && mean[i] < mean_min) mean_min = mean[i];
    if(mean2[i] > 0 && mean2[i] < mean_min) mean_min = mean2[i];
    if(mean3[i] > 0 && mean3[i] < mean_min) mean_min = mean3[i];
  }
	
  for(int i=0; i<nmodules; i++){
    histo_sigmaModMeanValue->SetBinContent(i+1,mean[i]);
    histo_sigmaModMeanValue->SetBinError(i+1,sigma[i]);
    histo_sigmaModMeanValue->GetXaxis()->SetBinLabel(i+1,label[i]);
		
    histo_sigmaModMeanValue_1->SetBinContent(i+1,mean2[i]);
    histo_sigmaModMeanValue_1->SetBinError(i+1,sigma2[i]);
		
    histo_sigmaModMeanValue_2->SetBinContent(i+1,mean3[i]);
    histo_sigmaModMeanValue_2->SetBinError(i+1,sigma3[i]);
  }
	
	
  make_nice_plot(histo_sigmaModMeanValue,TestID,mean_min*0.8,mean_max*1.2);
	
  histo_sigmaModMeanValue_1->SetMarkerColor(kBlue);
  histo_sigmaModMeanValue_1->SetMarkerStyle(20);
  histo_sigmaModMeanValue_1->Draw("pe1x0same");
	
  histo_sigmaModMeanValue_2->SetMarkerColor(kBlack);
  histo_sigmaModMeanValue_2->SetMarkerStyle(22);
  histo_sigmaModMeanValue_2->Draw("pe1x0same");
	
	
	
  TLegend* leg = new TLegend(0.6, 0.7, 0.85, 0.9);
  leg->AddEntry(histo_sigmaModMeanValue, "Module reception test", "p");
  leg->AddEntry(histo_sigmaModMeanValue_1, "Stave before thermal cycling", "p");
  leg->AddEntry(histo_sigmaModMeanValue_2, "Stave after thermal cycling", "p");
  leg->Draw();
}

/************ 2D Threshold *********************/

void Thres2d(HistoReader* aReader){
	
  FicoPlot();
  if(aReader->Thre_thresh2d->GetMaximum()>10000)
    aReader->Thre_thresh2d->GetZaxis()->SetRangeUser(0,10000);
  aReader->Thre_thresh2d->Draw("colz");
  drawAtlasInternal2D();
}






void Thres2d_Diff(HistoReader* aReader,HistoReader* bReader){
	
  FicoPlot();
  TH2F *th1 = aReader->Thre_thresh2d;
  TH2F *th2 = bReader->Thre_thresh2d;
  th2->Add(th1,-1);
  th2->Draw("colz");
  drawAtlasInternal2D();
}


void BadPixelsMap(HistoReader* aReader, HistoReader* aReader_noHV){
  //    TCanvas *c = new TCanvas();
    
  THStack *badpixelsmap = new THStack("badpixelsmap","Bad Pixels Map");
    
  TH2D *digimask;
  TH2D *anamask;
  TH2D *thresmask = new TH2D("","",80,0,80,336,0,336);
  TH2I *thresmask_disco;// = new TH2I("","",80,0,80,336,0,336);
  TH2D *crossmask = new TH2D("","",80,0,80,336,0,336);
	
  double sigmamean= 0;
  Fit_noisedistro(aReader->Thre_sigmadist);
  sigmamean = aReader->Thre_sigmadist->GetFunction("gaus1")->GetParameter(1);
  double sigmaerror= aReader->Thre_sigmadist->GetFunction("gaus1")->GetParameter(2);
  double noisylim;
	
  noisylim = 350;
	
  int k,l;
  for(int i=1; i<81; i++){
        
    for(int j=1;j<337;j++){
      if(aReader->Thre_Sigma->GetBinContent(i,j)>noisylim){
	thresmask->SetBinContent(i,j,1);
      }
    }
		
  }
	
	
	
  for(int i=1;i<81;i++){
    for(int j=1;j<337;j++){
      if(aReader->Cros_crosstalk2d->GetBinContent(i,j)!=0){
	crossmask->SetBinContent(i,j,1);
      }
			
    }
  }
	
  digimask = (TH2D*)aReader->Digi_Mask_Mod;
  anamask = (TH2D*)aReader->Anal_Mask_Mod;
  thresmask_disco = TDB_diff(aReader_noHV,aReader);
    
  digimask->SetLineColor(kRed);
  digimask->SetFillColor(kRed);
  anamask->SetLineColor(kBlue);
  anamask->SetFillColor(kBlue);
  thresmask->SetFillColor(kGreen);
  thresmask->SetLineColor(kGreen);
  thresmask_disco->SetFillColor(kOrange);
  thresmask_disco->SetLineColor(kOrange);
  crossmask->SetFillColor(kViolet);
  crossmask->SetLineColor(kViolet);
	
  badpixelsmap->Add(anamask);
  badpixelsmap->Add(digimask);
  badpixelsmap->Add(crossmask);
  badpixelsmap->Add(thresmask_disco);
  badpixelsmap->Add(thresmask);
	
  gPad->SetTheta(85); // default is 30
  gPad->SetPhi(15); // default is 30
  gPad->Update();
    
  gStyle->SetNdivisions(3,"z");
    
  badpixelsmap->Draw("lego10fb");
    
  TLegend *leg = new TLegend(0.77,0.82,0.99,0.95,"Test");
  leg->AddEntry(digimask, "Digital","F");
  leg->AddEntry(anamask, "Analog","F");
  leg->AddEntry(thresmask,"Threshold","F");
  leg->AddEntry(thresmask_disco,"Thr.Disc.Bumps","F");
  leg->AddEntry(crossmask,"CrossTalk","F");
  leg->SetFillColor(kYellow-10);
    
  leg->Draw();	
}

void AnalOccupancy(HistoReader* aReader)
{
  if(aReader->Anal_Occupancy_Point_000 == NULL) return;
	
  FicoPlot();
  aReader->Anal_Occupancy_Point_000->DrawCopy("colz");
  
  if(aReader->Stat_processPosition != PP_UsbPixReceptionTest)
    aReader->Anal_Occupancy_Point_000->DrawCopy("colz");
  else if(aReader->Stat_processPosition == PP_UsbPixReceptionTest){
    
    TH2C *Anal_Occupancy_Point_000_scaled= new TH2C("Occupancy_Point_000","OCCUPANCY",80,0,80,336,0,336);
    for (int i=1;i<=80;i++){
      for (int j=1; j<=336;j++){
	Int_t scale = aReader->Anal_Occupancy_Point_000->GetBinContent(i,j);
	Anal_Occupancy_Point_000_scaled->SetBinContent(i,j,scale/4);
      }
    }
    Anal_Occupancy_Point_000_scaled->DrawCopy("colz");
  }
  drawAtlasInternal2D();	
}

void DigitalOccupancy(HistoReader* aReader)
{
  std::cout<<aReader->Stat_Path<<std::endl;

  if(aReader->Digi_Occupancy_Point_000 == NULL) return;

  FicoPlot();
  TH2C *Digi = aReader->Digi_Occupancy_Point_000;
  Digi->DrawCopy("colz");
	
  if(aReader->Stat_processPosition != PP_UsbPixReceptionTest)
    aReader->Digi_Occupancy_Point_000->DrawCopy("colz");
  else if(aReader->Stat_processPosition == PP_UsbPixReceptionTest){
    TH2C *Digi_Occupancy_Point_000_scaled= new TH2C("Occupancy_Point_000","OCCUPANCY",80,0,80,336,0,336);
    for (int i=1;i<81;i++){
      for (int j=1; j<337; j++){
	Int_t scale =aReader->Digi_Occupancy_Point_000->GetBinContent(i,j);
	Digi_Occupancy_Point_000_scaled->SetBinContent(i,j,scale/4);
      }
    }
    Digi_Occupancy_Point_000_scaled->DrawCopy("colz");
  }
  drawAtlasInternal2D();
}

void CrossTalk2d(HistoReader* aReader)
{
  if(aReader->Cros_crosstalk2d == NULL) return;
	
  FicoPlot();
  aReader->Cros_crosstalk2d->DrawCopy("colz");
  drawAtlasInternal2D();
}

void ToT2d(HistoReader* aReader){
  if(aReader->ToT_2d_mod == NULL) return;

  FicoPlot();
  aReader->ToT_2d_mod->Draw("colz");
  drawAtlasInternal2D();	
}


void CrossTalkdist(HistoReader* aReader)
{
  gStyle->SetOptStat(kTRUE);
  aReader->Cros_crosstalkdist->Draw();
}

void ToTMean_2d(HistoReader* aReader)
{
  if(aReader->ToT_2d_mod == NULL) return;
	
  FicoPlot();
  aReader->ToT_2d_mod->DrawCopy("colz");
  drawAtlasInternal2D();
}

void ToTMean_dist(HistoReader* aReader)
{
  gStyle->SetOptStat(kTRUE);
  aReader->ToT_dist_mod->Draw();
}


// Threshold Disconnected Bumps must be used only in without HV case

TH2I* TDB(HistoReader* aReader_withoutHV)
{
	
  double mean, sigma, sigmalimit = 40;
  TH2I* Thre_disconnectedBumps = new TH2I("Thre_disconnectedBumps","Disconnected Bumps",80,0,80,336,0,336);
  int k,l;
	
  Fit_noisedistro(aReader_withoutHV->Thre_sigmadist);
  mean = aReader_withoutHV->Thre_sigmadist->GetFunction("gaus1")->GetParameter(1);
  sigma = aReader_withoutHV->Thre_sigmadist->GetFunction("gaus1")->GetParameter(2);
  
  std::cout<<"NOISE MEAN IS: "<<mean<<"\t NOISE SIGMA IS: "<<sigma<<std::endl;

  if(sigma>sigmalimit)sigma=sigmalimit;
  int nbinx = (int)aReader_withoutHV->Thre_Sigma->GetNbinsX();
  int nbiny = (int)aReader_withoutHV->Thre_Sigma->GetNbinsY();
  for(int i=1; i<=nbinx;i++){
    for(int j=1; j<=nbiny;j++){
      if(aReader_withoutHV->Thre_Sigma->GetBinContent(i,j)<(mean-5*fabs(sigma)) && aReader_withoutHV->Thre_Sigma->GetBinContent(i,j)<200 && aReader_withoutHV->Thre_Sigma->GetBinContent(i,j)>=1){

	Thre_disconnectedBumps->SetBinContent(i,j,1);
      }
    }
  }
  return Thre_disconnectedBumps;
}

TH2I* TDB_diff(HistoReader* aReader_withoutHV,HistoReader* aReader_HV)
{
	
  double mean, sigma, sigmalimit = 40;
  TH2I* Thre_disconnectedBumps = new TH2I("Thre_disconnectedBumps","Disconnected Bumps",80,0,80,336,0,336);
  int k,l;
	
  Fit_noisedistro(aReader_withoutHV->Thre_sigmadist);
  mean = aReader_withoutHV->Thre_sigmadist->GetFunction("gaus1")->GetParameter(1);
  sigma = aReader_withoutHV->Thre_sigmadist->GetFunction("gaus1")->GetParameter(2);
  if(sigma>sigmalimit)sigma=sigmalimit;
  int nbinx = (int)aReader_withoutHV->Thre_Sigma->GetNbinsX();
  int nbiny = (int)aReader_withoutHV->Thre_Sigma->GetNbinsY();
  for(int i=1; i<=nbinx;i++){
    for(int j=1; j<=nbiny;j++){
      if(aReader_withoutHV->Thre_Sigma->GetBinContent(i+1,j+1)!=0 && aReader_HV->Thre_Sigma->GetBinContent(i+1,j+1)!=0 && abs(aReader_withoutHV->Thre_Sigma->GetBinContent(i+1,j+1) - aReader_HV->Thre_Sigma->GetBinContent(i+1,j+1)) < 20)
	
	{
	  Thre_disconnectedBumps->SetBinContent(i,j,1);
	}
    }
  }
  return Thre_disconnectedBumps;
}

void ThresholdDisconnectedBumps_2(HistoReader* aReader_withoutHV,HistoReader *aReader_HV){
  //THStack* stack = new THStack();	
  //TH2I* tdb = TDB(aReader_withoutHV);
  TH2I* tdb_2 = TDB_diff(aReader_withoutHV, aReader_HV);

  // tdb->SetFillColor(kRed);
  //  tdb->Draw("box");
  tdb_2->SetFillColor(kBlue);
  tdb_2->Draw("box");
    
  /*stack->Add(tdb);
    stack->Add(tdb_2);

    stack->Draw("lego10fb");
  */

}

void ThresholdDisconnectedBumps(HistoReader* aReader_withoutHV){
	
  TH2I* tdb = TDB(aReader_withoutHV);
  tdb->SetFillColor(kRed);
  tdb->Draw("box");
}

void Threshold_sigma_dist(HistoReader* aReader)
{
  if(aReader->Thre_sigmadist == NULL) return;
	
  gStyle->SetOptStat(kTRUE);
  Fit_noisedistro(aReader->Thre_sigmadist);
  aReader->Thre_sigmadist->GetFunction("gaus1")->SetNpx(10000);
  aReader->Thre_sigmadist->DrawCopy();
}

void Threshold_sigma_dist_diff(HistoReader* aReader,HistoReader* bReader)
{
  gStyle->SetOptStat(kTRUE);
  TH1F *th1 = aReader->Thre_sigmadist;
  TH1F *th2 = bReader->Thre_sigmadist;
  th1->SetLineColor(kRed);
  th2->SetLineColor(kBlue);
  Fit_noisedistro(th1);
  Fit_noisedistro(th2);
  th1->GetFunction("gaus1")->SetNpx(10000);
  th2->GetFunction("gaus1")->SetNpx(10000);
	
  th1->GetFunction("gaus1")->SetLineColor(kRed);
  th2->GetFunction("gaus1")->SetLineColor(kBlue);
	
  th1->Draw();
  th2->Draw("same");
}

void Threshold_thres_dist(HistoReader* aReader)
{
  if(aReader->Thre_threshdist == NULL) return;
	
  gStyle->SetOptStat(kTRUE);
  aReader->Thre_threshdist->GetFunction("gauss")->SetNpx(10000);
  aReader->Thre_threshdist->DrawCopy();
}

void Threshold_thres_dist_diff(HistoReader* aReader,HistoReader* bReader)
{
  gStyle->SetOptStat(kTRUE);
  TH1 *th1, *th2;
  th1 = aReader->Thre_threshdist;
  th2 = bReader->Thre_threshdist;
	
	
  th1->SetLineColor(kRed);
  th2->SetLineColor(kBlue);
	
  th1->GetFunction("gauss")->SetNpx(10000);
  th2->GetFunction("gauss")->SetNpx(10000);
	
  th1->GetFunction("gauss")->SetLineColor(kRed);
  th2->GetFunction("gauss")->SetLineColor(kBlue);
	
  th1->Draw();
  th2->Draw("same");
}

void AnalDiff(HistoReader* aReader,HistoReader* bReader)
{
  FicoPlot();
  TH2C *anal_1;
  TH2C *anal_2;
  TH2C *anal_1_scaled= new TH2C("Occupancy_Point_000","OCCUPANCY",80,0,80,336,0,336);
  TH2C *anal_2_scaled= new TH2C("Occupancy_Point_000","OCCUPANCY",80,0,80,336,0,336);
	
  if ((aReader->Stat_processPosition != PP_UsbPixReceptionTest &&
       bReader->Stat_processPosition != PP_UsbPixReceptionTest)||
      (aReader->Stat_processPosition == PP_UsbPixReceptionTest &&
       bReader->Stat_processPosition == PP_UsbPixReceptionTest))
    {
      anal_1 = aReader->Anal_Occupancy_Point_000;
      anal_2 = bReader->Anal_Occupancy_Point_000;
      anal_2->Add(anal_1,-1);
      anal_2->Draw("colz");
    }
  else if(aReader->Stat_processPosition == PP_UsbPixReceptionTest &&
	  bReader->Stat_processPosition != PP_UsbPixReceptionTest)
    {
      for (int i=0;i<29880;i++){
	Int_t scale =aReader->Anal_Occupancy_Point_000->GetBinContent(i);
	anal_1_scaled->SetBinContent(i,scale/4);
      }
      anal_2 = bReader->Anal_Occupancy_Point_000;
      anal_1_scaled->Add(anal_2,-1);
      anal_1_scaled->Draw("colz");
    }
  else if (aReader->Stat_processPosition != PP_UsbPixReceptionTest &&
	   bReader->Stat_processPosition == PP_UsbPixReceptionTest)
    {
      for (int i=0;i<29880;i++){
	Int_t scale =bReader->Anal_Occupancy_Point_000->GetBinContent(i);
	anal_2_scaled->SetBinContent(i,scale/4);
      }
      anal_1 = aReader->Anal_Occupancy_Point_000;
      anal_2_scaled->Add(anal_1,-1);
      anal_2_scaled->Draw("colz");
    }
  drawAtlasInternal2D();
}

void DigiDiff(HistoReader* aReader,HistoReader* bReader)
{
  FicoPlot();
  TH2C *digi_1;
  TH2C *digi_2;
  TH2C *digi_1_scaled= new TH2C("Occupancy_Point_000","OCCUPANCY",80,0,80,336,0,336);
  TH2C *digi_2_scaled= new TH2C("Occupancy_Point_000","OCCUPANCY",80,0,80,336,0,336);
	
  if ((aReader->Stat_processPosition != PP_UsbPixReceptionTest &&
       bReader->Stat_processPosition != PP_UsbPixReceptionTest)||
      (aReader->Stat_processPosition == PP_UsbPixReceptionTest &&
       bReader->Stat_processPosition == PP_UsbPixReceptionTest))
    {
      digi_1 = aReader->Digi_Occupancy_Point_000;
      digi_2 = bReader->Digi_Occupancy_Point_000;
      digi_2->Add(digi_1,-1);
      digi_2->Draw("colz");
    }
  else if(aReader->Stat_processPosition == PP_UsbPixReceptionTest &&
	  bReader->Stat_processPosition != PP_UsbPixReceptionTest)
    {
      for (int i=0;i<29880;i++){
	Int_t scale =aReader->Digi_Occupancy_Point_000->GetBinContent(i);
	digi_1_scaled->SetBinContent(i,scale/4);
      }
      digi_2 = bReader->Digi_Occupancy_Point_000;
      digi_1_scaled->Add(digi_2,-1);
      digi_1_scaled->Draw("colz");
    }
  else if (aReader->Stat_processPosition != PP_UsbPixReceptionTest &&
	   bReader->Stat_processPosition == PP_UsbPixReceptionTest)
    {
      for (int i=0;i<29880;i++){
	Int_t scale =bReader->Digi_Occupancy_Point_000->GetBinContent(i);
	digi_2_scaled->SetBinContent(i,scale/4);
      }
      digi_1 = aReader->Digi_Occupancy_Point_000;
      digi_2_scaled->Add(digi_1,-1);
      digi_2_scaled->Draw("colz");
    }
  drawAtlasInternal2D();
}

void IV_sector(int sector, Astave* stave_BTC,Astave* stave_AFTC){

  std::string staveid = stave_BTC->STAVEID;
  TH1D *histo_iv_BTC, *histo_iv_AFTC,*histo_iv_m1, *histo_iv_m2, *histo_iv_m3, *histo_iv_m4;
  //  double *arrayX_m1, *arrayY_m1,*arrayX_m2, *arrayY_m2, *arrayX_m3, *arrayY_m3,*arrayX_m4, *arrayY_m4;
  std::vector<IVMeasurementPoint> iv;
  HistoReader *module_1, *module_2, *module_3, *module_4;
  std::string s_module_1, s_module_2, s_module_3, s_module_4;
  std::string s_sector;


  int i=0, j=0;
  switch (sector){
  case M4C :
    s_sector = "M4C";
    i=7;
    j=31;
    break;
  case M3C : 
    s_sector = "M3C";
    i=6;
    j=27;
    break;
  case M2C : 
    s_sector = "M2C";
    i=5;
    j=23;
    break;
  case M1C :
    s_sector = "M1C";
    i=4;
    j=19;
    break; 
  case M1A :
    s_sector = "M1A";
    i=3;
    j=15;
    break; 
  case M2A :
    s_sector = "M2A";
    i=2;
    j=11;
    break;
  case M3A :
    s_sector = "M3A";
    i=1;
    j=7;
    break; 
  case M4A :
    s_sector = "M4A";
    i=0;
    j=3;
    break;
  }
  
  s_module_1 = stave_BTC->themodules.at(j-3)->Stat_Module_ID;
  s_module_2 = stave_BTC->themodules.at(j-2)->Stat_Module_ID;
  s_module_3 = stave_BTC->themodules.at(j-1)->Stat_Module_ID;
  s_module_4 = stave_BTC->themodules.at(j)->Stat_Module_ID;

  module_1 = new HistoReader(s_module_1.c_str(),PP_UsbPixReceptionTest,0,0);
  module_2 = new HistoReader(s_module_2.c_str(),PP_UsbPixReceptionTest,0,0);
  module_3 = new HistoReader(s_module_3.c_str(),PP_UsbPixReceptionTest,0,0);
  module_4 = new HistoReader(s_module_4.c_str(),PP_UsbPixReceptionTest,0,0);
    
  if(module_1->IV_Mod_Graph!=NULL)histo_iv_m1 = IVGraph_HistoConverter(module_1->IV_Mod_Graph);
  if(module_2->IV_Mod_Graph!=NULL)histo_iv_m2 = IVGraph_HistoConverter(module_2->IV_Mod_Graph);
  if(module_3->IV_Mod_Graph!=NULL)histo_iv_m3 = IVGraph_HistoConverter(module_3->IV_Mod_Graph);
  if(module_4->IV_Mod_Graph!=NULL)histo_iv_m4 = IVGraph_HistoConverter(module_4->IV_Mod_Graph);

  if(stave_BTC->themodules.size()>0){
    iv = stave_BTC->GetIVStaveVector(staveid, PP_StaveTestBeforeTC_BT, i);
    histo_iv_BTC = stave_BTC->HistoIVLoader(iv);
    iv.clear( );
  }
  bool check=false;

  std::cout<<"the modules AFTC size: "<<stave_AFTC->m_themodules.size()<<std::endl;

  // if(stave_AFTC->mountmodules(staveid, PP_StaveTestAfterTC_BT,1)!=-1){
  iv = stave_AFTC->GetIVStaveVector(staveid, PP_StaveTestAfterTC_BT,i);
  std::cout<<"CHECK IV_0\n";
  histo_iv_AFTC = stave_AFTC->HistoIVLoader(iv);
      
     
  check=true;
  std::cout<<"TRUE CHECK"<<check<<std::endl;
   
  iv.clear( );
  //   }




      
 
  histo_iv_BTC->SetMarkerColor(kBlue);
  histo_iv_BTC->SetMarkerStyle(21);
  histo_iv_BTC->SetLineColor(kBlue);
  histo_iv_BTC->SetLineWidth(2);
  // histo_iv_BTC->Draw();
  if(check==true){
    histo_iv_AFTC->SetMarkerColor(kRed);
    histo_iv_AFTC->SetMarkerStyle(21);
    histo_iv_AFTC->SetLineColor(kRed);
    histo_iv_AFTC->SetLineWidth(2);
  }
  //  histo_iv_AFTC->Draw("same");
  if(module_1->IV_Mod_Graph!=NULL){
    histo_iv_m1->SetMarkerColor(kBlack);
    histo_iv_m1->SetMarkerStyle(22);
    //  histo_iv_m1->SetMarkerSize(0.5);
    histo_iv_m1->SetLineColor(kBlack);
    histo_iv_m1->SetLineWidth(1);
  }
  if(module_2->IV_Mod_Graph!=NULL){
    histo_iv_m2->SetMarkerColor(kBlack);
    histo_iv_m2->SetMarkerStyle(23);
    //  histo_iv_m2->SetMarkerSize(0.5);
    histo_iv_m2->SetLineColor(kBlack);
    histo_iv_m2->SetLineWidth(1);
  }
  if(module_3->IV_Mod_Graph!=NULL){
    histo_iv_m3->SetMarkerColor(kBlack);
    histo_iv_m3->SetMarkerStyle(24);
    //  histo_iv_m3->SetMarkerSize(0.5);
    histo_iv_m3->SetLineColor(kBlack);
    histo_iv_m3->SetLineWidth(1);
  }
  if(module_4->IV_Mod_Graph!=NULL){
    histo_iv_m4->SetMarkerColor(kBlack);
    histo_iv_m4->SetMarkerStyle(25);
    //  histo_iv_m4->SetMarkerSize(0.5);
    histo_iv_m4->SetLineColor(kBlack);
    histo_iv_m4->SetLineWidth(1);
  }
  gPad->SetGrid();

  THStack* IV_sector= new THStack("IV_sector","IV_sector");

  IV_sector->Add(histo_iv_BTC);
  if(check==true)IV_sector->Add(histo_iv_AFTC);
 
  if(module_1->IV_Mod_Graph!=NULL)  IV_sector->Add(histo_iv_m1);
  if(module_2->IV_Mod_Graph!=NULL && module_1->Stat_Module_ID!=module_2->Stat_Module_ID)  IV_sector->Add(histo_iv_m2);
  if(module_3->IV_Mod_Graph!=NULL)  IV_sector->Add(histo_iv_m3);
  if(module_4->IV_Mod_Graph!=NULL && module_3->Stat_Module_ID!=module_4->Stat_Module_ID)  IV_sector->Add(histo_iv_m4);  

  IV_sector->Draw("plnostack");
  IV_sector->GetXaxis()->SetTitle("HV(V)");
  IV_sector->GetYaxis()->SetTitle("I (#muA)"); 


  TLegend* leg = new TLegend(0.1,0.55,0.35,0.9);
  leg->SetHeader("Stave I-V");
  leg->AddEntry(histo_iv_BTC,(s_sector+" BTC").c_str(),"P");
  if(check==true)leg->AddEntry(histo_iv_AFTC,(s_sector+" ATC").c_str(),"P");
  if(module_1->IV_Mod_Graph!=NULL) leg->AddEntry(histo_iv_m1, (module_1->Stat_Module_ID+" RT").c_str(),"P"); 
  if(module_2->IV_Mod_Graph!=NULL && module_1->Stat_Module_ID!=module_2->Stat_Module_ID) leg->AddEntry(histo_iv_m2, (module_2->Stat_Module_ID+" RT").c_str(),"P"); 
  if(module_3->IV_Mod_Graph!=NULL) leg->AddEntry(histo_iv_m3, (module_3->Stat_Module_ID+" RT").c_str(),"P"); 
  if(module_4->IV_Mod_Graph!=NULL && module_3->Stat_Module_ID!=module_4->Stat_Module_ID) leg->AddEntry(histo_iv_m4, (module_4->Stat_Module_ID+" RT").c_str(),"P"); 
  leg->SetFillColor(kYellow-10);
  leg->Draw();


}


void IV_3D(Astave* aStave,
	   const std::string staveid, ProcessPosition asource)
{
  TH1D *histo_iv[2];

  std::vector<IVMeasurementPoint> iv;
  
  for(int i=0;i<2;i++)
    {
      if (i==0) iv = aStave->GetIVStaveVector(staveid, asource, i);
      else iv = aStave->GetIVStaveVector(staveid, asource,7);
      histo_iv[i] = aStave->HistoIVLoader(iv);        
      

      iv.clear( );
    }

  histo_iv[0]->SetMarkerColor(kBlue);
  histo_iv[0]->SetMarkerStyle(21);
  histo_iv[0]->SetLineColor(kBlue);
  histo_iv[0]->SetLineWidth(2);
  histo_iv[1]->SetMarkerColor(kRed);
  histo_iv[1]->SetMarkerStyle(21);
  histo_iv[1]->SetLineColor(kRed);
  histo_iv[1]->SetLineWidth(2);

  gPad->SetGrid();

  THStack* IV_3D= new THStack("IV_3D","IV_3D");

  IV_3D->Add(histo_iv[0]);
  IV_3D->Add(histo_iv[1]);
  IV_3D->Draw("plnostack");
  IV_3D->GetXaxis()->SetTitle("HV(V)");
  IV_3D->GetYaxis()->SetTitle("I (#muA)"); 
  IV_3D->GetYaxis()->SetTitleOffset(1.4);

  
  TLegend* leg = new TLegend(0.1,0.75,0.35,0.9);
  leg->SetHeader("Stave I-V");
  leg->AddEntry(histo_iv[0],"M4A | A8, A7","P");
  leg->AddEntry(histo_iv[1],"M4C | C8, C7","P");
  leg->SetFillColor(kYellow-10);
  leg->Draw();

}

void IV_Planar(Astave* aStave,
	       const std::string staveid, ProcessPosition asource)
{

  std::vector<IVMeasurementPoint> iv;
  TH1D *histo_iv[6];
  
  for (int i=0;i<6;i++)
    {
      iv = aStave->GetIVStaveVector(staveid, asource, i+1);
      histo_iv[i] = aStave->HistoIVLoader(iv);
      iv.clear( );
    }
  histo_iv[0]->SetMarkerColor(kCyan);
  histo_iv[0]->SetMarkerStyle(21);
  histo_iv[0]->SetLineColor(kCyan);
  histo_iv[0]->SetLineWidth(2);

  histo_iv[1]->SetMarkerColor(kViolet);
  histo_iv[1]->SetMarkerStyle(21);
  histo_iv[1]->SetLineColor(kViolet);
  histo_iv[1]->SetLineWidth(2);
    
  histo_iv[2]->SetMarkerColor(kGreen);
  histo_iv[2]->SetMarkerStyle(21);
  histo_iv[2]->SetLineColor(kGreen);
  histo_iv[2]->SetLineWidth(2);

  histo_iv[3]->SetMarkerColor(kBlack);
  histo_iv[3]->SetMarkerStyle(21);
  histo_iv[3]->SetLineColor(kBlack);
  histo_iv[3]->SetLineWidth(2);

  histo_iv[4]->SetMarkerColor(kOrange+8);
  histo_iv[4]->SetMarkerStyle(21);
  histo_iv[4]->SetLineColor(kOrange+8);
  histo_iv[4]->SetLineWidth(2);

    
  histo_iv[5]->SetMarkerColor(kYellow);
  histo_iv[5]->SetMarkerStyle(21);
  histo_iv[0]->SetLineColor(kCyan);
  histo_iv[0]->SetLineWidth(2);
  
  histo_iv[1]->SetLineColor(kViolet);
  histo_iv[1]->SetLineWidth(2);
    
  histo_iv[2]->SetLineColor(kGreen);
  histo_iv[2]->SetLineWidth(2);
    
  histo_iv[3]->SetLineColor(kBlack);
  histo_iv[3]->SetLineWidth(2);
    
  histo_iv[4]->SetLineColor(kOrange+8);
  histo_iv[4]->SetLineWidth(2);
    
  histo_iv[5]->SetLineColor(kYellow);
  histo_iv[5]->SetLineWidth(2);
  
  gPad->SetGrid();
  THStack* IV_planar= new THStack("IV_planar","IV_planar");

  IV_planar->Add(histo_iv[0]);
  IV_planar->Add(histo_iv[1]);
  IV_planar->Add(histo_iv[2]);
  IV_planar->Add(histo_iv[3]);
  IV_planar->Add(histo_iv[4]);
  IV_planar->Add(histo_iv[5]);
  IV_planar->Draw("plnostack");
  IV_planar->GetXaxis()->SetTitle("HV(V)");
  IV_planar->GetYaxis()->SetTitle("I (#muA)"); 
  IV_planar->GetYaxis()->SetTitleOffset(1.4);
 
  TLegend* leg = new TLegend(0.1,0.6,0.35,0.9);
  leg->SetHeader("Stave I-V");
  leg->AddEntry(histo_iv[0],"M3A | A6, A5","P");
  leg->AddEntry(histo_iv[1],"M2A | A4, A3","P");
  leg->AddEntry(histo_iv[2],"M1A | A2, A1","P");
  leg->AddEntry(histo_iv[3],"M1C | C2, C1","P");
  leg->AddEntry(histo_iv[4],"M2C | C4, C3","P");
  leg->AddEntry(histo_iv[5],"M3C | C6, C5","P");
  leg->SetFillColor(kYellow-10);
  leg->Draw();
}

double OperationalCurrent(HistoReader* aReader){
  TGraph *IV;
  double *voltage, *current, op_curr=0;
  int dimension;

  IV = aReader->IV_Mod_Graph;
  if(IV != NULL){
    std::cout<<"IV != NULL"<<std::endl;
    voltage = IV->GetX();
    current = IV->GetY();
    dimension = IV->GetN();

    for(int i=0; i<dimension; i++){
      if(abs(voltage[i])<=20. && abs(voltage[i]) >= 19.){ op_curr = current[i];     std::cout<<"19<volt<=20"<<std::endl;}
      if(abs(voltage[i]) <=80. && abs(voltage[i]) >=79. && DbTools::GetInstance()->IsDoubleChip(aReader->Stat_Module_ID.c_str()) == 1) op_curr = current[i];
    }
  }
  return op_curr;
}

double get_breakdown_stave(Astave *astave,std::string staveid, int channel){

  std::string break_string;
  std::vector<IVMeasurementPoint> iv;
//  std::string staveid = astave->GetStaveID();
  iv = astave->GetIVStaveVector(staveid, PP_StaveTestBeforeTC_AT, channel);
  double break_voltage;
  if(true){
 
    double iv_plateau_slope=2000, iv_plateau_int;
    double iv_break_slope=0, iv_break_int;
    double iv_slope, iv_int;
    double sm_xy, sm_x, sm_y, sm_xx;
    double x_average, y_average;
    int counter;
    bool planar_check = (channel!=7) && (channel!=0);

    int size = iv.size();

    double i_limit = iv.at(size-1).current;
    for(int i=4; i<size; i++){

          sm_xy = 0;
          sm_x = 0;
          sm_y = 0;
          sm_xx = 0;
          counter = 0;


          if(planar_check == true && fabs(iv.at(i).voltage)>50 /*&& fabs(iv.at(i).current)<0.00002*/){

            for (int l=0; l<=4; l++){
          	  int k = i-l;
            	sm_xy += iv.at(k).current*iv.at(k).voltage;
            	sm_x += iv.at(k).voltage;
            	sm_y += iv.at(k).current;
            	sm_xx += iv.at(k).voltage*iv.at(k).voltage;
            	counter++;
              }
	
            iv_slope = ((counter * sm_xy) - (sm_x*sm_y))/((counter * sm_xx) - (sm_x * sm_x));
            iv_int = ((sm_y * sm_xx) - (sm_x * sm_xy))/((counter*sm_xx)- (sm_x*sm_x));
		
            if(iv_slope<iv_plateau_slope && fabs(iv.at(i).current)<fabs(i_limit-0.1*i_limit)){
	            iv_plateau_slope = iv_slope;
	            iv_plateau_int = iv_int;
	
            }else if(iv_slope>iv_break_slope){
	            iv_break_slope = iv_slope;
	            iv_break_int = iv_int;
            }
          }else if(planar_check == false && fabs(iv.at(i).voltage)>15 && fabs(iv.at(i).current)<=fabs(i_limit)){
	
            for (int l=0; l<=2; l++){
	            int k = i-l;
	            sm_xy += iv.at(k).current*iv.at(k).voltage;
	            sm_x += iv.at(k).voltage;
	            sm_y += iv.at(k).current;
	            sm_xx += iv.at(k).voltage*iv.at(k).voltage;
	            counter++;
              }
	
            iv_slope = ((counter * sm_xy) - (sm_x*sm_y))/((counter * sm_xx) - (sm_x * sm_x));
            iv_int = ((sm_y * sm_xx) - (sm_x * sm_xy))/((counter*sm_xx)- (sm_x*sm_x));
	
        	
            if(iv_plateau_slope == NULL && iv_break_slope==NULL){
	            iv_plateau_slope = iv_slope;
	            iv_break_slope = iv_slope;
	            iv_plateau_int = iv_int;
	            iv_break_int = iv_int;
            }else if(iv_slope<iv_plateau_slope && fabs(iv.at(i).current)<fabs(i_limit-0.1*i_limit) && iv_slope>0){
	            iv_plateau_slope = iv_slope;
	            iv_plateau_int = iv_int;
            }else if(iv_slope>iv_break_slope){
              //std::cout<<"I'm in step #"<<i<<std::endl;

	            iv_break_slope = iv_slope;
	            iv_break_int = iv_int;
            }
	
          }
        }
    
    
        double break_limit;
       // if(planar_check == false && astave->themodules.at(channel*4)->Stat_Module_ID.substr(0,1) == "C")break_limit = 100;
        break_limit = 150;
        if(iv_break_slope>break_limit){
        //std::cout<<"Breakdown slope is: "<<iv_break_slope<<std::endl;
      
        break_voltage =  (iv_break_int - iv_plateau_int)/(iv_plateau_slope - iv_break_slope); 
        return break_voltage;      
    }
    else
      {
	if(planar_check == false)break_voltage = 110;
	if(planar_check == true)break_voltage = 360;
	
	return break_voltage;
      }
  }
  else
    {
      break_voltage = -1;
      return break_voltage;
    }


}


void breakdown_staves(){
  THStack *stack = new THStack("Breakdown Distribution","Breakdown Distribution");

  TH1D *break_distro_planar = new TH1D("Planar_Break","Planar_Break",30, 0.5, 360.5);
  TH1D *break_distro_CNM = new TH1D("CNM-Break", "CNM_Break", 30, 0.5, 360.5);
  TH1D *break_distro_FBK = new TH1D("FBK_Break", "FBK_break", 30, 0.5, 360.5);

  std::stringstream ss;  
  std::string stave_id, module_id;
  double breakdown;
  Astave *astave = new Astave(); 

  for(int stave=1; stave<=20; stave++){
    ss.str("");
    ss<<stave;
    stave_id = ss.str();
    astave->mountmodules(stave_id,PP_StaveTestBeforeTC_AT,1 );
    std::cout<<"Ho montato la stave: "<<stave_id<<std::endl;
    for(int channel=0; channel<8; channel++){
      breakdown = get_breakdown_stave(astave,stave_id, channel);
      //if(( astave->themodules.at(channel*4+(stave-1)*32)->Stat_Module_ID).substr(0,1)=="F" )std::cout<<"Breakdown is "<<breakdown<<" - Stave"<<stave_id<<" - Channel "<<channel<<" - ModuleID "<<astave->themodules.at(channel*4+(stave-1)*32)->Stat_Module_ID<<std::endl;
      if((astave->themodules.at(channel*4+(stave-1)*32)->Stat_Module_ID).substr(0,1)=="C")break_distro_CNM->Fill(breakdown);
      else if((astave->themodules.at(channel*4+(stave-1)*32)->Stat_Module_ID).substr(0,1)=="F")break_distro_FBK->Fill(breakdown);
      else break_distro_planar->Fill(breakdown);
      
    }

  }
  
  break_distro_planar->SetFillStyle(3002);
  break_distro_CNM->SetFillStyle(3004);
  break_distro_FBK->SetFillStyle(3005);

  break_distro_planar->SetFillColor(kBlue);
  break_distro_CNM->SetFillColor(kGreen+3);
  break_distro_FBK->SetFillColor(kRed);

  stack->Add(break_distro_planar);
  stack->Add(break_distro_CNM);
  stack->Add(break_distro_FBK);

  stack->Draw("nostack");
  stack->GetYaxis()->SetTitle("Number of groups");
  stack->GetXaxis()->SetTitle("voltage breakdown [V]");


  TLegend* leg = new TLegend(0.75,0.1,0.9,0.3);
  //leg->SetHeader("I-V");
  leg->AddEntry(break_distro_planar,"Planar CiS","F");
  leg->AddEntry(break_distro_CNM,"3D CNM","F");
  leg->AddEntry(break_distro_FBK,"3D FBK","F");
  leg->SetFillColor(kWhite);
  leg->Draw();

  drawAtlasInternal1D();
  drawHistoTitle("Voltage breakdow after loading");
}

std::string IV_Mod(HistoReader* aReader,HistoReader* bReader )
{
  TGraph *IV_Off;
  TGraph *IV_On;
  
  IV_Off = aReader->IV_Mod_Graph;
  IV_On = bReader->IV_Mod_Graph;
  
  std::string break_string;
  
  if(IV_Off !=NULL && IV_On!=NULL){
    
    IV_Off->GetXaxis()->SetTitle("HV(V)");
    IV_Off->GetYaxis()->SetTitle("HV - current (A)");
    IV_Off->SetMarkerColor(kRed);
    IV_Off->SetMarkerSize(1);
    IV_Off->SetMarkerStyle(21);
    IV_Off->SetLineColor(kRed);
    IV_Off->SetLineWidth(2);
    IV_Off->Draw("alp");
    
    
    IV_On->SetMarkerColor(kBlue);
    IV_On->SetMarkerSize(1);
    IV_On->SetMarkerStyle(20);
    IV_On->SetLineColor(kBlue);
    IV_On->SetLineWidth(2);
    
    IV_On->Draw("lp");
    
    TLegend* leg = new TLegend(0.75,0.1,0.9,0.3);
    leg->SetHeader("I-V");
    leg->AddEntry(IV_Off,"FE_Off","P");
    leg->AddEntry(IV_On,"FE_On","P");
    leg->SetFillColor(kYellow-10);
    leg->Draw();
    


  double *iv_voltage = IV_On->GetX();
  double *iv_current = IV_On->GetY();
  double iv_plateau_slope=2000, iv_plateau_int;
  double iv_break_slope=0, iv_break_int;
  double iv_slope, iv_int;
  double sm_xy, sm_x, sm_y, sm_xx;
  double x_average, y_average;
  int counter;


//  double *iv_curr_diff;

  int size = IV_On->GetN();

  double i_limit = iv_current[size-1]-1e-7;
  for(int i=4; i<size; i++){

    if(DbTools::GetInstance()->IsDoubleChip(aReader->Stat_Module_ID.c_str()) == 1 && fabs(iv_voltage[i])>50 && fabs(iv_current[i])<0.00002){

        sm_xy = 0;
        sm_x = 0;
        sm_y = 0;
        sm_xx = 0;
        counter = 0;
	
	
        for (int l=0; l<=4; l++){
	  int k = i-l;
	  sm_xy += iv_current[k]*iv_voltage[k];
	  sm_x += iv_voltage[k];
	  sm_y += iv_current[k];
	  sm_xx += iv_voltage[k]*iv_voltage[k];
	  counter++;
        }
	
        iv_slope = ((counter * sm_xy) - (sm_x*sm_y))/((counter * sm_xx) - (sm_x * sm_x));
        iv_int = ((sm_y * sm_xx) - (sm_x * sm_xy))/((counter*sm_xx)- (sm_x*sm_x));
	
        //iv_slope = (iv_current[i]-iv_current[i-4])/(iv_voltage[i]-iv_voltage[i-4]);
	
        //iv_int = iv_current[i-2] - iv_slope * iv_voltage[i-2];
	
        if(iv_slope<iv_plateau_slope && fabs(iv_current[i])<fabs(i_limit-0.1*i_limit)){
	  iv_plateau_slope = iv_slope;
	  iv_plateau_int = iv_int;
	  //           std::cout<<"Plateau slope is:\t"<<iv_plateau_slope<<"\tat: "<<iv_voltage[i-2]<<std::endl;
        }else if(iv_slope>iv_break_slope){
	  iv_break_slope = iv_slope;
	  iv_break_int = iv_int;
	  //           std::cout<<"Break slope is:\t"<<iv_break_slope<<"\tat: "<<iv_voltage[i-2]<<std::endl;
        }
      }
      else if(DbTools::GetInstance()->IsDoubleChip(aReader->Stat_Module_ID.c_str()) != 1 && fabs(iv_voltage[i])>15 && fabs(iv_current[i])<=fabs(i_limit)){
	
        sm_xy = 0;
        sm_x = 0;
        sm_y = 0;
        sm_xx = 0;
        counter = 0;
	
	
        for (int l=0; l<=2; l++){
	  int k = i-l;
	  sm_xy += iv_current[k]*iv_voltage[k];
	  sm_x += iv_voltage[k];
	  sm_y += iv_current[k];
	  sm_xx += iv_voltage[k]*iv_voltage[k];
	  counter++;
        }
	
        iv_slope = ((counter * sm_xy) - (sm_x*sm_y))/((counter * sm_xx) - (sm_x * sm_x));
        iv_int = ((sm_y * sm_xx) - (sm_x * sm_xy))/((counter*sm_xx)- (sm_x*sm_x));
	
        //iv_slope = (iv_current[i]-iv_current[i-2])/(iv_voltage[i]-iv_voltage[i-2]);
        //iv_int = iv_current[i-1] - iv_slope * iv_voltage[i-1];
	
        if(iv_plateau_slope == NULL && iv_break_slope==NULL){
	  iv_plateau_slope = iv_slope;
	  iv_break_slope = iv_slope;
	  iv_plateau_int = iv_int;
	  iv_break_int = iv_int;
        }else if(iv_slope<iv_plateau_slope && fabs(iv_current[i])<fabs(i_limit-0.1*i_limit) && iv_slope>0){
	  iv_plateau_slope = iv_slope;
	  iv_plateau_int = iv_int;
        }else if(iv_slope>iv_break_slope){
	  iv_break_slope = iv_slope;
	  iv_break_int = iv_int;
        }
	
      }
    }
    
    TF1 *plateau = new TF1("plateau","[0]+[1]*x",-400,0);
    plateau->SetParameter(0,iv_plateau_int);
    plateau->SetParameter(1,iv_plateau_slope);
    plateau->SetLineWidth(1);
    plateau->SetLineColor(kBlack);
    
    plateau->Draw("lsame");
    
    double break_limit;
    if(DbTools::GetInstance()->IsDoubleChip(aReader->Stat_Module_ID.c_str()) != 1 && aReader->Stat_Module_ID.substr(0,1) == "C")break_limit = 5e-8;
    else break_limit = 3e-8;
   

    if(iv_break_slope>break_limit){
      
      TF1 *breakdown = new TF1("breakdown","[0]+[1]*x",-400,0);
      breakdown->SetParameter(0,iv_break_int);
      breakdown->SetParameter(1,iv_break_slope);  
      breakdown->SetLineWidth(1);
      breakdown->SetLineColor(kGreen+2);
      breakdown->Draw("lsame");
      
      double break_voltage = -1 * (iv_break_int - iv_plateau_int)/(iv_plateau_slope - iv_break_slope); 
      
      std::ostringstream s;
      s << break_voltage;
      std::cout<<"Breakdown for module "<<aReader->Stat_Module_ID<<" is: "<<s.str()<<std::endl;
      return s.str();
    }
    else
      {
	if(DbTools::GetInstance()->IsDoubleChip(aReader->Stat_Module_ID.c_str()) != 1)break_string = ">100";
	if(DbTools::GetInstance()->IsDoubleChip(aReader->Stat_Module_ID.c_str()) == 1)break_string = ">350";
	std::cout<<"Breakdown for module "<<aReader->Stat_Module_ID<<" is: "<<break_string<<std::endl;
	return break_string;
      }
  }
  else
    {
      break_string = "no IV";
      std::cout<<"Breakdown for module "<<aReader->Stat_Module_ID<<" is not available"<<std::endl;
      return break_string;
    }
}

void IV_Mod_Norm(HistoReader* aReader,HistoReader* bReader)
{
  double k_boltz = 8.6173324e-5;
  double T_ref = 273.; // Eg @ 273 is equal to:
  double Eg_ref = 1.166 - (4.73e-4*T_ref*T_ref/(T_ref+636));
  double KK_ref = -Eg_ref/(2*k_boltz);
	
  // IV_Off
  int na = aReader->IV_Mod_Graph->GetN();
  double ax_a[na];
  double ay_a[na];
  double Eg_a = 1.166 - (4.73e-4*aReader->GetIV_temp(aReader)*aReader->GetIV_temp(aReader)/(aReader->GetIV_temp(aReader)+636));

  double KK_a = -Eg_a/(2*k_boltz);
  double ratio_a = (T_ref/aReader->GetIV_temp(aReader))*(T_ref/aReader->GetIV_temp(aReader))*exp((KK_ref/T_ref-KK_a/aReader->GetIV_temp(aReader)));
  for(Int_t i=0; i<na; i++) {
    aReader->IV_Mod_Graph->GetPoint(i,ax_a[i],ay_a[i]);
    ay_a[i] = ratio_a*ay_a[i];
  }
	
  TGraph *IV_Off = new TGraph(na,ax_a,ay_a);
  IV_Off->GetXaxis()->SetTitle("HV(V)");
  IV_Off->GetYaxis()->SetTitle("HV - current (A)");
  IV_Off->SetMarkerColor(kRed);
  IV_Off->SetMarkerSize(1);
  IV_Off->SetMarkerStyle(21);
  IV_Off->Draw("ap");
	
  // IV_On
  int nb = bReader->IV_Mod_Graph->GetN();
  double ax_b[nb];
  double ay_b[nb];
  double Eg_b = 1.166 - (4.73e-4*bReader->GetIV_temp(bReader)*bReader->GetIV_temp(bReader)/(bReader->GetIV_temp(bReader)+636));
  double KK_b = -Eg_b/(2*k_boltz);
  double ratio_b = (T_ref/bReader->GetIV_temp(bReader))*(T_ref/bReader->GetIV_temp(bReader))*exp((KK_ref/T_ref-KK_b/bReader->GetIV_temp(bReader)));
  for(Int_t i=0; i<nb; i++) {
    bReader->IV_Mod_Graph->GetPoint(i,ax_b[i],ay_b[i]);
    ay_b[i] = ratio_b*ay_b[i];
  }
	
  TGraph *IV_On = new TGraph(nb,ax_b,ay_b);
  IV_On->SetMarkerColor(kBlue);
  IV_On->SetMarkerSize(1);
  IV_On->SetMarkerStyle(21);
  IV_On->Draw("p");
	
  TLegend* leg = new TLegend(0.75,0.1,0.9,0.3);
  leg->SetHeader("I-V temp-norm");
  leg->AddEntry(IV_Off,"FE_Off","P");
  leg->AddEntry(IV_On,"FE_On","P");
  leg->SetFillColor(kYellow-10);
  leg->Draw();
	
	
}
/* additional function - threshold - */

void Thre_thresh1d(HistoReader* aReader)
{
  gStyle->SetOptStat(kTRUE);
  aReader->Thre_thresh1d->Draw();
}
void Thre_sigma1d(HistoReader* aReader)
{
  gStyle->SetOptStat(kTRUE);
  aReader->Thre_sigma1d->Draw();
}
void Thre_BadPixels(HistoReader* aReader)
{
  aReader->Thre_BadPixels->Draw();
}
/* additional function - crosstalk - */

void Cros_thresh1d(HistoReader* aReader)
{
  gStyle->SetOptStat(kTRUE);
  aReader->Cros_thresh1d->Draw();
}
void Cros_sigma1d(HistoReader* aReader)
{
  gStyle->SetOptStat(kTRUE);
  aReader->Cros_sigma1d->Draw();
}
void Cros_thresh2d(HistoReader* aReader)
{
	
  aReader->Cros_thresh2d->Draw();
}
void Cros_threshdist(HistoReader* aReader)
{
  gStyle->SetOptStat(kTRUE);
  aReader->Cros_threshdist->Draw();
}
void Cros_sigmadist(HistoReader* aReader)
{
  gStyle->SetOptStat(kTRUE);
  aReader->Cros_sigmadist->Draw();
}
void Cros_crosstalk1d(HistoReader* aReader)
{
  gStyle->SetOptStat(kTRUE);
  aReader->Cros_crosstalk1d->Draw();
}

void ThresholdSigma2d(HistoReader *aReader){
  FicoPlot();
  if(aReader->Thre_Sigma->GetMaximum()>2000) aReader->Thre_Sigma->GetZaxis()->SetRangeUser(0,2000);
  aReader->Thre_Sigma->Draw("colz");
  drawAtlasInternal2D();
}

void DigitalAnalogDeadHistory(HistoReader *hrUSBpix, HistoReader *hrRCE_rt, HistoReader *hrRCE_BefThCycle, HistoReader *hrRCE_AftThCycle){
  TH1D *digiDeadHistory = new TH1D("digiDeadHistory","digital Bad pixel",4,0,3);
	
  if (hrUSBpix->Digi_Mask_Mod!=NULL) {
    digiDeadHistory->SetBinContent(1,hrUSBpix->Digi_Mask_Mod->GetEntries());
  }else std::cout<<"USBpix Digi Mask not available for module:\t"<<hrUSBpix->Stat_Module_ID<<std::endl;
  if (hrRCE_rt->Digi_Mask_Mod!=NULL) {
    digiDeadHistory->SetBinContent(2,hrRCE_rt->Digi_Mask_Mod->GetEntries());
  } else std::cout<<"RCE_rt Digi Mask not available for module:\t"<<hrRCE_rt->Stat_Module_ID<<std::endl;
  if (hrRCE_BefThCycle->Digi_Mask_Mod!=NULL) {
    digiDeadHistory->SetBinContent(3,hrRCE_BefThCycle->Digi_Mask_Mod->GetEntries());
  } else std::cout<<"RCE_BefThCycle Digi Mask not available for module:\t"<<hrRCE_BefThCycle->Stat_Module_ID<<std::endl;
  if (hrRCE_AftThCycle->Digi_Mask_Mod!=NULL) {
    digiDeadHistory->SetBinContent(4,hrRCE_AftThCycle->Digi_Mask_Mod->GetEntries());
  } else std::cout<<"RCE_AftThCycle Digi Mask not available for module:\t"<<hrRCE_AftThCycle->Stat_Module_ID<<std::endl;
	
	
	
  digiDeadHistory->SetBinContent(4,hrRCE_AftThCycle->Digi_Mask_Mod->GetEntries());
	
  digiDeadHistory->GetXaxis()->SetBinLabel(1,"USBpix_RT");
  digiDeadHistory->GetXaxis()->SetBinLabel(2,"RCE_RT");
  digiDeadHistory->GetXaxis()->SetBinLabel(3,"Before Th. Cycle");
  digiDeadHistory->GetXaxis()->SetBinLabel(4,"After Th. Cycle");
	
  digiDeadHistory->SetLineColor(kRed);
  digiDeadHistory->Draw();

  TH1D *analogDeadHistory = new TH1D("analogDeadHistory","analog Bad pixel",4,0,3);
	
  analogDeadHistory->SetBinContent(1,hrUSBpix->Digi_Mask_Mod->GetEntries());
  analogDeadHistory->SetBinContent(2,hrRCE_rt->Digi_Mask_Mod->GetEntries());
  analogDeadHistory->SetBinContent(3,hrRCE_BefThCycle->Digi_Mask_Mod->GetEntries());
  analogDeadHistory->SetBinContent(4,hrRCE_AftThCycle->Digi_Mask_Mod->GetEntries());
	
  analogDeadHistory->GetXaxis()->SetBinLabel(1,"USBpix_RT");
  analogDeadHistory->GetXaxis()->SetBinLabel(2,"RCE_RT");
  analogDeadHistory->GetXaxis()->SetBinLabel(3,"Before Th. Cycle");
  analogDeadHistory->GetXaxis()->SetBinLabel(4,"After Th. Cycle");
	
  analogDeadHistory->SetLineColor(kBlue);
  analogDeadHistory->Draw("same");
	
}

std::vector <int> BadPixelMonster2(HistoReader *aReader_HV,HistoReader *aReader_NO_HV){
  int digital_0=0, digital_greater=0, digital_less=0;
  int analog_0=0, analog_greater=0, analog_less=0;
  int thres_0=0, thres_disco=0, thres_noisy=0, total=0;
  int xtalk=0;
  int nbinx = 0, nbiny = 0;
  int usbpix_limit = 200, rce_limit = 50, noise_limit = 350., limit;

  if(aReader_HV->Digi_Occupancy_Point_000!=NULL){
    nbinx = aReader_HV->Digi_Occupancy_Point_000->GetNbinsX();
    nbiny = aReader_HV->Digi_Occupancy_Point_000->GetNbinsY();
    for(int i=0; i<nbinx; i++){
      for(int j=0; j<nbiny; j++){
                
	//NULL Entries
	if((int)aReader_HV->Digi_Occupancy_Point_000->GetBinContent(i+1,j+1)==0)digital_0++;
      	if(aReader_HV->Stat_processPosition==PP_UsbPixReceptionTest){
	  if((int)aReader_HV->Digi_Occupancy_Point_000->GetBinContent(i+1,j+1)>usbpix_limit && (int)aReader_HV->Digi_Occupancy_Point_000->GetBinContent(i+1,j+1)!=0)digital_greater++;
	  if((int)aReader_HV->Digi_Occupancy_Point_000->GetBinContent(i+1,j+1)<usbpix_limit && (int)aReader_HV->Digi_Occupancy_Point_000->GetBinContent(i+1,j+1)!=0)digital_less++;
	}
	else{
	  if((int)aReader_HV->Digi_Occupancy_Point_000->GetBinContent(i+1,j+1)>rce_limit && (int)aReader_HV->Digi_Occupancy_Point_000->GetBinContent(i+1,j+1)!=0)digital_greater++;
	  if((int)aReader_HV->Digi_Occupancy_Point_000->GetBinContent(i+1,j+1)<rce_limit && (int)aReader_HV->Digi_Occupancy_Point_000->GetBinContent(i+1,j+1)!=0 )digital_less++;
	}
      }
    }
  }else{
    digital_greater = -1;
    digital_less=-1;
    digital_0=-1;
    std::cout<<"No digital occupancy found for module:\t"<<aReader_HV->Stat_Module_ID<<std::endl;
  }
  if(aReader_HV->Anal_Occupancy_Point_000!=NULL){
    nbinx = aReader_HV->Anal_Occupancy_Point_000->GetNbinsX();
    nbiny = aReader_HV->Anal_Occupancy_Point_000->GetNbinsY();
    for(int i=0; i<nbinx; i++){
      for(int j=0; j<nbiny; j++){
	
	if((int)aReader_HV->Anal_Occupancy_Point_000->GetBinContent(i+1,j+1)==0 )analog_0++;
	if(aReader_HV->Stat_processPosition==PP_UsbPixReceptionTest){
	  
	  if((int)aReader_HV->Anal_Occupancy_Point_000->GetBinContent(i+1,j+1)>usbpix_limit && (int)aReader_HV->Anal_Occupancy_Point_000->GetBinContent(i+1,j+1)!=0)analog_greater++;
	  if((int)aReader_HV->Anal_Occupancy_Point_000->GetBinContent(i+1,j+1)<usbpix_limit && (int)aReader_HV->Anal_Occupancy_Point_000->GetBinContent(i+1,j+1)!=0)analog_less++;
	}
	else{
	  
	  if((int)aReader_HV->Anal_Occupancy_Point_000->GetBinContent(i+1,j+1)>rce_limit && (int)aReader_HV->Anal_Occupancy_Point_000->GetBinContent(i+1,j+1)!=0)analog_greater++;
	  if((int)aReader_HV->Anal_Occupancy_Point_000->GetBinContent(i+1,j+1)<rce_limit && (int)aReader_HV->Anal_Occupancy_Point_000->GetBinContent(i+1,j+1)!=0)analog_less++;
	}
      }
    }
  }
  else{
    analog_greater=-1;
    analog_less=-1;
    analog_0=-1;
    std::cout<<"No analog occupancy found for module:\t"<<aReader_HV->Stat_Module_ID<<std::endl;
  }
  if(aReader_HV->Thre_thresh2d!=NULL){
    nbinx = aReader_HV->Thre_thresh2d->GetNbinsX();
    nbiny = aReader_HV->Thre_thresh2d->GetNbinsY();
    for(int i=0; i<nbinx; i++){
      for(int j=0; j<nbiny; j++){
	if((int)aReader_HV->Thre_thresh2d->GetBinContent(i+1,j+1)==0)thres_0++;
      }
    }
  }
  else{
    thres_0 = -1;
    std::cout<<"No threshold occupancy (HV ON) found for module:\t"<<aReader_HV->Stat_Module_ID<<std::endl;
  }
  if(aReader_HV->Thre_Sigma!=NULL){
    nbinx = aReader_HV->Thre_Sigma->GetNbinsX();
    nbiny = aReader_HV->Thre_Sigma->GetNbinsY();
    for(int i=0; i<nbinx; i++){
      for(int j=0; j<nbiny; j++){
	if((int)aReader_HV->Thre_Sigma->GetBinContent(i+1,j+1)>=noise_limit)thres_noisy++;
      }
    }
  }
  else{
    thres_noisy=-1;
    std::cout<<"No threshold sigma (HV ON) found for module:\t"<<aReader_HV->Stat_Module_ID<<std::endl;
  }
	
  double mean, sigma, sigmalimit=40;
  if(aReader_NO_HV->Thre_Sigma!=NULL  && aReader_NO_HV->Thre_sigmadist->GetEntries() > 1){
    Fit_noisedistro(aReader_NO_HV->Thre_sigmadist);
    mean = aReader_NO_HV->Thre_sigmadist->GetFunction("gaus1")->GetParameter(1);
    sigma = aReader_NO_HV->Thre_sigmadist->GetFunction("gaus1")->GetParameter(2);
    if(sigma>sigmalimit)sigma=sigmalimit;
    nbinx = aReader_NO_HV->Thre_Sigma->GetNbinsX();
    nbiny = aReader_NO_HV->Thre_Sigma->GetNbinsY();
    for(int i=0; i<nbinx; i++){
      for(int j=0; j<nbiny; j++){

	// GENOVA METHOD
	//if(aReader_NO_HV->Thre_Sigma->GetBinContent(i+1,j+1)!=0 && aReader_HV->Thre_Sigma->GetBinContent(i+1,j+1)!=0 && abs(aReader_NO_HV->Thre_Sigma->GetBinContent(i+1,j+1) - aReader_HV->Thre_Sigma->GetBinContent(i+1,j+1)) < 20)thres_disco++;
	// NORMAL METHOD        
	if(aReader_NO_HV->Thre_Sigma->GetBinContent(i+1,j+1)!=0 && aReader_NO_HV->Thre_Sigma->GetBinContent(i+1,j+1)<mean-5*fabs(sigma) && aReader_NO_HV->Thre_Sigma->GetBinContent(i+1,j+1)<200)thres_disco++;

      }
    }
  }
  else{
    thres_disco=-1;
    std::cout<<"No threshold sigma (HV OFF) found for module:\t"<<aReader_NO_HV->Stat_Module_ID<<std::endl;
  }
  if(aReader_HV->Cros_crosstalk2d!=NULL){
    nbinx = aReader_HV->Cros_crosstalk2d->GetNbinsX();
    nbiny = aReader_HV->Cros_crosstalk2d->GetNbinsY();
    for(int i=0; i<nbinx; i++){
      for(int j=0; j<nbiny; j++){
	if((int)aReader_HV->Cros_crosstalk2d->GetBinContent(i+1,j+1)!=0)xtalk++;
      }
    }
  }
  else{
    xtalk=-1;
    std::cout<<"No x-talk occupancy (HV ON) found for module:\t"<<aReader_HV->Stat_Module_ID<<std::endl;
  }
  if(analog_greater>20000)std::cout<<"#############\nNumber of Analog greater: "<<analog_greater<<std::endl; 
  std::vector <int> badpixelarray;
  
  std::cout << " DIGITAL DEAD " << digital_0 << std::endl;
  badpixelarray.push_back(xtalk);
  badpixelarray.push_back(thres_noisy);
  badpixelarray.push_back(thres_disco);
  badpixelarray.push_back(thres_0);
  badpixelarray.push_back(analog_greater);
  badpixelarray.push_back(analog_less);
  badpixelarray.push_back(analog_0);
  badpixelarray.push_back(digital_greater);
  badpixelarray.push_back(digital_less);
  badpixelarray.push_back(digital_0);
  //{digital_0,digital_less,digital_greater,analog_0,analog_less,analog_greater,thres_0,thres_disco,thres_noisy,xtalk};
  //std::cout<<std::endl<<std::endl<<"BAD DIGITAL: "<<badpixelarray[0]+badpixelarray[1]+badpixelarray[2]<<std::endl;
  //std::cout<<"BAD ANALOG: "<<badpixelarray[3]+badpixelarray[4]+badpixelarray[5]<<std::endl;
  //std::cout<<"BAD THRESHOLD: "<<badpixelarray[6]+badpixelarray[7]+badpixelarray[8]<<std::endl<<std::endl<<std::endl;
  //    aReader_HV->HistoReader::~HistoReader();
  //    aReader_NO_HV->HistoReader::~HistoReader();

  return badpixelarray;
  badpixelarray.clear();
	
}

//------------------------------------------
std::vector <int> BadPixelMonsterHierarchy(HistoReader *aReader_HV,HistoReader *aReader_NO_HV){
  int digital_0=0, digital_greater=0, digital_less=0;
  int analog_0=0, analog_greater=0, analog_less=0;
  int thres_0=0, thres_disco=0, thres_noisy=0, total=0;
  int xtalk=0;
  int nbinx = 0, nbiny = 0;
  int usbpix_limit = 200, rce_limit = 50, noise_limit = 350., limit;
  
  //if(aReader_HV->Stat_processPosition == PP_UsbPixReceptionest) limit = usbpix_limit;
  //else limit = rce_limit;
  
  //------------------------------------------
  // digi
  if (aReader_HV->Digi_Occupancy_Point_000 != NULL) {
    nbinx = aReader_HV->Digi_Occupancy_Point_000->GetNbinsX();
    nbiny = aReader_HV->Digi_Occupancy_Point_000->GetNbinsY();

    for (int i=0; i<nbinx; i++) {
      for (int j=0; j<nbiny; j++) {
	
	//NULL Entries
	if ((int)aReader_HV->Digi_Occupancy_Point_000->GetBinContent(i+1,j+1) == 0) digital_0++;

      	if (aReader_HV->Stat_processPosition == PP_UsbPixReceptionTest) {
	  if ((int)aReader_HV->Digi_Occupancy_Point_000->GetBinContent(i+1,j+1) > usbpix_limit &&
	      (int)aReader_HV->Digi_Occupancy_Point_000->GetBinContent(i+1,j+1) !=0 )
	    digital_greater++;
	  if ((int)aReader_HV->Digi_Occupancy_Point_000->GetBinContent(i+1,j+1) < usbpix_limit &&
	      (int)aReader_HV->Digi_Occupancy_Point_000->GetBinContent(i+1,j+1) !=0 )
	    digital_less++;

	} else {
	  if ((int)aReader_HV->Digi_Occupancy_Point_000->GetBinContent(i+1,j+1) > rce_limit &&
	      (int)aReader_HV->Digi_Occupancy_Point_000->GetBinContent(i+1,j+1) !=0 )
	    digital_greater++;
	  if ((int)aReader_HV->Digi_Occupancy_Point_000->GetBinContent(i+1,j+1) < rce_limit &&
	      (int)aReader_HV->Digi_Occupancy_Point_000->GetBinContent(i+1,j+1) !=0 )
	    digital_less++;
	}

      }//END loop over y bins
    }//END loop over x bins

  } else {
    digital_greater = -1;
    digital_less = -1;
    digital_0 = -1;
    std::cout<<"No digital occupancy found for module:\t"<<aReader_HV->Stat_Module_ID<<std::endl;
  }

  //------------------------------------------
  // anal
  if (aReader_HV->Anal_Occupancy_Point_000 != NULL) {
    nbinx = aReader_HV->Anal_Occupancy_Point_000->GetNbinsX();
    nbiny = aReader_HV->Anal_Occupancy_Point_000->GetNbinsY();

    for (int i=0; i<nbinx; i++) {
      for (int j=0; j<nbiny; j++) {
	
	if (aReader_HV->Stat_processPosition == PP_UsbPixReceptionTest &&
	    (int)aReader_HV->Digi_Occupancy_Point_000->GetBinContent(i+1,j+1) == usbpix_limit ) {
	  
	  if((int)aReader_HV->Anal_Occupancy_Point_000->GetBinContent(i+1,j+1) == 0)analog_0++;
	  if((int)aReader_HV->Anal_Occupancy_Point_000->GetBinContent(i+1,j+1) > usbpix_limit &&
	     (int)aReader_HV->Anal_Occupancy_Point_000->GetBinContent(i+1,j+1) != 0)
	    analog_greater++;
	  if((int)aReader_HV->Anal_Occupancy_Point_000->GetBinContent(i+1,j+1) < usbpix_limit &&
	     (int)aReader_HV->Anal_Occupancy_Point_000->GetBinContent(i+1,j+1) != 0)
	    analog_less++;
	  
	} else if (aReader_HV->Stat_processPosition != PP_UsbPixReceptionTest &&
		   (int)aReader_HV->Digi_Occupancy_Point_000->GetBinContent(i+1,j+1) == rce_limit) {

	  if((int)aReader_HV->Anal_Occupancy_Point_000->GetBinContent(i+1,j+1) == 0) analog_0++;
	  if((int)aReader_HV->Anal_Occupancy_Point_000->GetBinContent(i+1,j+1) > rce_limit &&
	     (int)aReader_HV->Anal_Occupancy_Point_000->GetBinContent(i+1,j+1) != 0)
	    analog_greater++;
	  if((int)aReader_HV->Anal_Occupancy_Point_000->GetBinContent(i+1,j+1) < rce_limit &&
	     (int)aReader_HV->Anal_Occupancy_Point_000->GetBinContent(i+1,j+1) != 0)
	    analog_less++;
	}

      }//END loop over y bins
    }//END loop over x bins

  } else{
    analog_greater=-1;
    analog_less=-1;
    analog_0=-1;
    std::cout<<"No analog occupancy found for module:\t"<<aReader_HV->Stat_Module_ID<<std::endl;
  }
  
  //------------------------------------------
  //thresh
  if(aReader_HV->Thre_thresh2d != NULL){
    nbinx = aReader_HV->Thre_thresh2d->GetNbinsX();
    nbiny = aReader_HV->Thre_thresh2d->GetNbinsY();

    for (int i=0; i<nbinx; i++) {
      for (int j=0; j<nbiny; j++) {
	
	if (aReader_HV->Stat_processPosition == PP_UsbPixReceptionTest &&
	    (int)aReader_HV->Digi_Occupancy_Point_000->GetBinContent(i+1,j+1) == usbpix_limit &&
	    (int)aReader_HV->Anal_Occupancy_Point_000->GetBinContent(i+1,j+1) == usbpix_limit) {
	  if((int)aReader_HV->Thre_thresh2d->GetBinContent(i+1,j+1) == 0) thres_0++;
	  
	} else if (aReader_HV->Stat_processPosition != PP_UsbPixReceptionTest &&
		   (int)aReader_HV->Digi_Occupancy_Point_000->GetBinContent(i+1,j+1) == rce_limit &&
		   (int)aReader_HV->Anal_Occupancy_Point_000->GetBinContent(i+1,j+1) == rce_limit) { 
	  if((int)aReader_HV->Thre_thresh2d->GetBinContent(i+1,j+1) == 0) thres_0++;
	}

      }//END loop over y bins
    }//END loop over x bins

  } else{
    thres_0 = -1;
    std::cout<<"No threshold occupancy (HV ON) found for module:\t"<<aReader_HV->Stat_Module_ID<<std::endl;
  }

  //------------------------------------------
  //thre_sigma with HV
  if (aReader_HV->Thre_Sigma != NULL &&
      aReader_NO_HV->Thre_sigmadist != NULL &&
      aReader_NO_HV->Thre_sigmadist->GetEntries() > 1) {
    nbinx = aReader_HV->Thre_Sigma->GetNbinsX();
    nbiny = aReader_HV->Thre_Sigma->GetNbinsY();
    
    double mean, sigma, sigmalimit=40;  
    
    Fit_noisedistro(aReader_NO_HV->Thre_sigmadist);
    mean = aReader_NO_HV->Thre_sigmadist->GetFunction("gaus1")->GetParameter(1);
    sigma = aReader_NO_HV->Thre_sigmadist->GetFunction("gaus1")->GetParameter(2);
    
    std::cout<<"Noise Fit Mean: "<<mean<<std::endl;
    std::cout<<"Noise Fit Sigma: "<<sigma<<std::endl;
    if (sigma > sigmalimit) sigma=sigmalimit;

    double mm = mean -5*fabs(sigma);

    for (int i=0; i<nbinx; i++) {
      for (int j=0; j<nbiny; j++) {

	if ((int)aReader_HV->Thre_Sigma->GetBinContent(i+1,j+1) >= noise_limit) thres_noisy++;
	//else if(aReader_NO_HV->Thre_Sigma->GetBinContent(i+1,j+1)!=0 && aReader_HV->Thre_Sigma->GetBinContent(i+1,j+1)!=0 && abs(aReader_NO_HV->Thre_Sigma->GetBinContent(i+1,j+1) - aReader_HV->Thre_Sigma->GetBinContent(i+1,j+1)) < 20){
	else if (aReader_NO_HV->Thre_Sigma->GetBinContent(i+1,j+1) < mm &&
		 aReader_NO_HV->Thre_Sigma->GetBinContent(i+1,j+1) >= 1 &&
		 aReader_NO_HV->Thre_Sigma->GetBinContent(i+1,j+1) < 200) {
	  thres_disco++;
	  //   std::cout<<"Row: "<<i+1<<"\tColumn: "<<j+1<<"\tNoise value: "<<aReader_NO_HV->Thre_Sigma->GetBinContent(i+1,j+1)<<std::endl;
	}

      }//END loop over y bins
    }//END loop over x bins

  }

  /* else{
     thres_noisy=-1;
     std::cout<<"No threshold sigma (HV ON) found for module:\t"<<aReader_HV->Stat_Module_ID<<std::endl;
     }*/
  //thre_sigma without HV	
  
  /*  else{
      thres_disco=-1;
      std::cout<<"No threshold sigma (HV OFF) found for module:\t"<<aReader_NO_HV->Stat_Module_ID<<std::endl;
      }*/

  //------------------------------------------
  // xtalk
  if (aReader_HV->Cros_crosstalk2d != NULL) {
    nbinx = aReader_HV->Cros_crosstalk2d->GetNbinsX();
    nbiny = aReader_HV->Cros_crosstalk2d->GetNbinsY();

    for (int i=0; i<nbinx; i++) {
      for (int j=0; j<nbiny; j++) {
	
	if (aReader_HV->Stat_processPosition == PP_UsbPixReceptionTest &&
	    (int)aReader_HV->Digi_Occupancy_Point_000->GetBinContent(i+1,j+1) == usbpix_limit &&
	    (int)aReader_HV->Anal_Occupancy_Point_000->GetBinContent(i+1,j+1) == usbpix_limit) {
	  if ((int)aReader_HV->Cros_crosstalk2d->GetBinContent(i+1,j+1) != 0) xtalk++;
	} else if (aReader_HV->Stat_processPosition != PP_UsbPixReceptionTest &&
		   (int)aReader_HV->Digi_Occupancy_Point_000->GetBinContent(i+1,j+1) == rce_limit &&
		   (int)aReader_HV->Anal_Occupancy_Point_000->GetBinContent(i+1,j+1) == rce_limit) {
	  if ((int)aReader_HV->Cros_crosstalk2d->GetBinContent(i+1,j+1) != 0) xtalk++;
	}
		
      }//END loop over y bins
    }//END loop over x bins
    
  } else {
    xtalk=-1;
    std::cout<<"No x-talk occupancy (HV ON) found for module:\t"<<aReader_HV->Stat_Module_ID<<std::endl;
  }
  
  if (analog_greater > 20000) std::cout<<"#############\nNumber of Analog greater: "<<analog_greater<<std::endl; 
  
  std::vector <int> badpixelarray;
  total = /*xtalk +*/ thres_noisy + thres_disco + thres_0 + analog_greater + analog_less + analog_0 + digital_greater + digital_less + digital_0;
  badpixelarray.push_back(xtalk);
  badpixelarray.push_back(thres_noisy);
  badpixelarray.push_back(thres_disco);
  badpixelarray.push_back(thres_0);
  badpixelarray.push_back(analog_greater);
  badpixelarray.push_back(analog_less);
  badpixelarray.push_back(analog_0);
  badpixelarray.push_back(digital_greater);
  badpixelarray.push_back(digital_less);
  badpixelarray.push_back(digital_0);
  badpixelarray.push_back(total);
  //{digital_0,digital_less,digital_greater,analog_0,analog_less,analog_greater,thres_0,thres_disco,thres_noisy,xtalk};
  //std::cout<<std::endl<<std::endl<<"BAD DIGITAL: "<<badpixelarray[0]+badpixelarray[1]+badpixelarray[2]<<std::endl;
  //std::cout<<"BAD ANALOG: "<<badpixelarray[3]+badpixelarray[4]+badpixelarray[5]<<std::endl;
  //std::cout<<"BAD THRESHOLD: "<<badpixelarray[6]+badpixelarray[7]+badpixelarray[8]<<std::endl<<std::endl<<std::endl;
  //    aReader_HV->HistoReader::~HistoReader();
  //    aReader_NO_HV->HistoReader::~HistoReader();

  return badpixelarray;
  badpixelarray.clear();	
}


//------------------------------------------
void BP_J(std::vector <HistoReader*> stave_hv, std::vector <HistoReader*> stave_nohv){
  
  std::vector <int> bp_j;
  //  int var_histo_0, var_histo_great, var_histo_less, var_histo_disconnectedbump, var_histo_noisy;
  THStack *BP_sum = new THStack("bad_pixel_sum","bad pixels");
  TString label[32]={"A8_2","A8_1","A7_2","A7_1","A6_2","A6_1","A5_2","A5_1","A4_2","A4_1","A3_2","A3_1","A2_2","A2_1","A1_2","A1_1","C1_1","C1_2","C2_1","C2_2","C3_1","C3_2","C4_1","C4_2","C5_1","C5_2","C6_1","C6_2","C7_1","C7_2","C8_1","C8_2"};

  int n_bin = 32;

  TH1I *histo_anal_0 = new TH1I("histo_anal_0","histo_anal_0",n_bin,0,n_bin);
  histo_anal_0->SetFillColor(kViolet);
  TH1I *histo_digi_0 = new TH1I("histo_digi_0","histo_digi_0",n_bin,0,n_bin);
  histo_digi_0->SetFillColor(kAzure);
  TH1I *histo_thresh_0 = new TH1I("histo_thresh_0","histo_tresh_0",n_bin,0,n_bin);
  histo_thresh_0->SetFillColor(kGreen);
  TH1I *histo_anal_great = new TH1I("histo_anal_great","histo_anal_great",n_bin,0,n_bin);
  histo_anal_great->SetFillColor(kViolet+2);  
  TH1I *histo_digi_great = new TH1I("histo_digi_great","histo_digi_great",n_bin,0,n_bin);
  histo_digi_great->SetFillColor(kAzure-1);  
  TH1I *histo_anal_less = new TH1I("histo_anal_less","histo_anal_less",n_bin,0,n_bin);
  histo_anal_less->SetFillColor(kViolet+4);
  TH1I *histo_digi_less = new TH1I("histo_digi_less","histo_digi_less",n_bin,0,n_bin);
  histo_digi_less->SetFillColor(kAzure-2);
  TH1I *histo_disconnectedbump = new TH1I("histo_disconnectedbump","histo_disconnectedbump",n_bin,0,n_bin);  
  histo_disconnectedbump->SetFillColor(kYellow);
  TH1I *histo_xtalk_noisy = new TH1I("histo_xtalk_noisy","histo_xtalk_noisy",n_bin,0,n_bin); 
  histo_xtalk_noisy->SetFillColor(kRed);
  TH1I *histo_thresh_noisy = new TH1I("histo_thresh_noisy","histo_thresh_noisy",n_bin,0,n_bin); 
  histo_thresh_noisy->SetFillColor(kGreen+2);

  for(unsigned i=1;i<stave_hv.size()+1;i++) {
    bp_j= BadPixelMonsterHierarchy(stave_hv[i-1],stave_nohv[i-1]);

    histo_xtalk_noisy->AddBinContent(i, bp_j.at(0));
    histo_thresh_noisy->AddBinContent(i, bp_j.at(1));
    histo_disconnectedbump->AddBinContent(i, bp_j.at(2));
    histo_thresh_0->AddBinContent(i, bp_j.at(3));
    histo_anal_great->AddBinContent(i, bp_j.at(4));
    histo_anal_less->AddBinContent(i, bp_j.at(5));
    histo_anal_0->AddBinContent(i, bp_j.at(6));
    histo_digi_great->AddBinContent(i, bp_j.at(7));
    histo_digi_less->AddBinContent(i, bp_j.at(8));
    histo_digi_0->AddBinContent(i, bp_j.at(9));
    //    std::cout << " BP_J bad ana " << bp_j.at(9) << std::endl;
  }  
  
  //BP_sum->Add(histo_xtalk_noisy);
  BP_sum->Add(histo_thresh_noisy);
  // BP_sum->Add(histo_disconnectedbump);
  BP_sum->Add(histo_thresh_0);
  BP_sum->Add(histo_anal_great);
  BP_sum->Add(histo_anal_less);
  BP_sum->Add(histo_anal_0);
  BP_sum->Add(histo_digi_great);
  BP_sum->Add(histo_digi_less);
  BP_sum->Add(histo_digi_0);
 
  //draw
  //  BP_sum->GetXaxis()->SetTickLength(0.01);
  //  BP_sum->GetXaxis()->SetTitleOffset(1.4);
  //  BP_sum->GetYaxis()->SetRangeUser(min,max);
  //  BP_sum->GetYaxis()->SetTickLength(0.01);
  //  BP_sum->GetYaxis()->SetTitleOffset(1.4);

  //ATLAS style
  gPad->SetTicks(1,1);
  gPad->SetGridy(0);
  gPad->SetGridx(0);
  gPad->SetRightMargin(0.02);
  gStyle->SetMarkerStyle(20);
  gStyle->SetMarkerSize(1.2);
  gStyle->SetHistLineWidth(1.);
  gStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes
  gStyle->SetEndErrorSize(0.);
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);

  BP_sum->Draw("HIST");
  for(int i=0;i<32;i++) BP_sum->GetXaxis()->SetBinLabel(i+1,label[i]);
  BP_sum->GetXaxis()->LabelsOption("v");
  BP_sum->SetMaximum(BP_sum->GetMaximum()*1.2);
  BP_sum->GetYaxis()->SetTitle("bad pixels");
  BP_sum->Draw("HIST");


  //  TLegend* leg = new TLegend(0.1,0.6,0.25,0.9);
  TLegend* leg = new TLegend(0.1,0.75,0.4,0.9);//TEST
  //  leg->SetHeader("bad pixels");
  //leg->AddEntry(histo_xtalk_noisy,"X-talk","F");
  leg->AddEntry(histo_thresh_noisy,"noisy","F");
  // leg->AddEntry(histo_disconnectedbump,"disconnected bumps","F");
  leg->AddEntry(histo_thresh_0,"threshold 0","F");
  leg->AddEntry(histo_anal_great,"analog > 50","F");
  leg->AddEntry(histo_anal_less,"analog < 50","F");
  leg->AddEntry(histo_anal_0,"analog 0","F");
  leg->AddEntry(histo_digi_great,"digital > 50","F");
  leg->AddEntry(histo_digi_less,"digital < 50","F");
  leg->AddEntry(histo_digi_0,"digital 0","F");
  leg->SetFillColor(0);
  leg->SetNColumns(2);
  leg->Draw();

  //make it ATLAS style
  drawAtlasInternal1D();
}
  

//------------------------------------------
void BP_J_disco(std::vector <HistoReader*> stave_hv, std::vector <HistoReader*> stave_nohv){
  
  std::vector <int> bp_j;
  //  int var_histo_0, var_histo_great, var_histo_less, var_histo_disconnectedbump, var_histo_noisy;
  THStack *BP_sum = new THStack("bad_pixel_sum","bad_pixel_sum");
  TString label[32]={"A8_2","A8_1","A7_2","A7_1","A6_2","A6_1","A5_2","A5_1","A4_2","A4_1","A3_2","A3_1","A2_2","A2_1","A1_2","A1_1","C1_1","C1_2","C2_1","C2_2","C3_1","C3_2","C4_1","C4_2","C5_1","C5_2","C6_1","C6_2","C7_1","C7_2","C8_1","C8_2"};

  int n_bin = 32;

  TH1I *histo_anal_0 = new TH1I("histo_anal_0","histo_anal_0",n_bin,0,n_bin);
  histo_anal_0->SetFillColor(kViolet);
  TH1I *histo_digi_0 = new TH1I("histo_digi_0","histo_digi_0",n_bin,0,n_bin);
  histo_digi_0->SetFillColor(kAzure);
  TH1I *histo_thresh_0 = new TH1I("histo_thresh_0","histo_tresh_0",n_bin,0,n_bin);
  histo_thresh_0->SetFillColor(kGreen);
  TH1I *histo_anal_great = new TH1I("histo_anal_great","histo_anal_great",n_bin,0,n_bin);
  histo_anal_great->SetFillColor(kViolet+2);  
  TH1I *histo_digi_great = new TH1I("histo_digi_great","histo_digi_great",n_bin,0,n_bin);
  histo_digi_great->SetFillColor(kAzure-1);  
  TH1I *histo_anal_less = new TH1I("histo_anal_less","histo_anal_less",n_bin,0,n_bin);
  histo_anal_less->SetFillColor(kViolet+4);
  TH1I *histo_digi_less = new TH1I("histo_digi_less","histo_digi_less",n_bin,0,n_bin);
  histo_digi_less->SetFillColor(kAzure-2);
  TH1I *histo_disconnectedbump = new TH1I("histo_disconnectedbump","histo_disconnectedbump",n_bin,0,n_bin);  
  histo_disconnectedbump->SetFillColor(kYellow);
  TH1I *histo_xtalk_noisy = new TH1I("histo_xtalk_noisy","histo_xtalk_noisy",n_bin,0,n_bin); 
  histo_xtalk_noisy->SetFillColor(kRed);
  TH1I *histo_thresh_noisy = new TH1I("histo_thresh_noisy","histo_thresh_noisy",n_bin,0,n_bin); 
  histo_thresh_noisy->SetFillColor(kGreen+2);

  for(unsigned i=1;i<stave_hv.size()+1;i++){

    bp_j= BadPixelMonsterHierarchy(stave_hv[i-1],stave_nohv[i-1]);

    histo_xtalk_noisy->AddBinContent(i, bp_j.at(0));
    histo_thresh_noisy->AddBinContent(i, bp_j.at(1));
    histo_disconnectedbump->AddBinContent(i, bp_j.at(2));
    histo_thresh_0->AddBinContent(i, bp_j.at(3));
    histo_anal_great->AddBinContent(i, bp_j.at(4));
    histo_anal_less->AddBinContent(i, bp_j.at(5));
    histo_anal_0->AddBinContent(i, bp_j.at(6));
    histo_digi_great->AddBinContent(i, bp_j.at(7));
    histo_digi_less->AddBinContent(i, bp_j.at(8));
    histo_digi_0->AddBinContent(i, bp_j.at(9));

  }  
  
  BP_sum->Add(histo_xtalk_noisy);
  BP_sum->Add(histo_thresh_noisy);
  BP_sum->Add(histo_disconnectedbump);
  BP_sum->Add(histo_thresh_0);
  BP_sum->Add(histo_anal_great);
  BP_sum->Add(histo_anal_less);
  BP_sum->Add(histo_anal_0);
  BP_sum->Add(histo_digi_great);
  BP_sum->Add(histo_digi_less);
  BP_sum->Add(histo_digi_0);
 
  BP_sum->Draw("HIST");
  for(int i=0;i<32;i++) BP_sum->GetXaxis()->SetBinLabel(i+1,label[i]);
  BP_sum->GetXaxis()->LabelsOption("v");
  BP_sum->Draw("HIST");


  TLegend* leg = new TLegend(0.1,0.6,0.25,0.9);
  leg->SetHeader("Bad Pixel");
  leg->AddEntry(histo_xtalk_noisy,"X-talk","F");
  leg->AddEntry(histo_thresh_noisy,"noisy","F");
  leg->AddEntry(histo_disconnectedbump,"disconnected bumps","F");
  leg->AddEntry(histo_thresh_0,"threshold 0","F");
  leg->AddEntry(histo_anal_great,"analog > 50","F");
  leg->AddEntry(histo_anal_less,"analog < 50","F");
  leg->AddEntry(histo_anal_0,"analog 0","F");
  leg->AddEntry(histo_digi_great,"digital > 50","F");
  leg->AddEntry(histo_digi_less,"digital < 50","F");
  leg->AddEntry(histo_digi_0,"digital 0","F");
  leg->SetFillColor(kYellow-10);
  leg->Draw();

}
  
int* BadPixelMonster(HistoReader *aReader_HV,HistoReader *aReader_NO_HV){
  int digital_0=0, digital_greater=0, digital_less=0;
  int analog_0=0, analog_greater=0, analog_less=0;
  int thres_0=0, thres_disco=0, thres_noisy=0, total=0;
  int xtalk=0;
    
  int nbinx = 0, nbiny = 0;
    
  double usbpix_limit = 200., rce_limit = 50., noise_limit = 350., limit;
	
	
    
  if(aReader_HV->Digi_Occupancy_Point_000!=NULL){
    nbinx = aReader_HV->Digi_Occupancy_Point_000->GetNbinsX();
    nbiny = aReader_HV->Digi_Occupancy_Point_000->GetNbinsY();
    for(int i=0; i<nbinx; i++){
      for(int j=0; j<nbiny; j++){
                
	//NULL Entries
	if(aReader_HV->Digi_Occupancy_Point_000->GetBinContent(i+1,j+1)==0)digital_0++;
                
                
	if(aReader_HV->Stat_processPosition==PP_UsbPixReceptionTest){
	  if(aReader_HV->Digi_Occupancy_Point_000->GetBinContent(i+1,j+1)>usbpix_limit)digital_greater++;
	  if(aReader_HV->Digi_Occupancy_Point_000->GetBinContent(i+1,j+1)<usbpix_limit)digital_less++;
	}
	else{
	  if(aReader_HV->Digi_Occupancy_Point_000->GetBinContent(i+1,j+1)>rce_limit)digital_greater++;
	  if(aReader_HV->Digi_Occupancy_Point_000->GetBinContent(i+1,j+1)<rce_limit)digital_less++;
	}
				
      }
    }
  }else{
    digital_greater = -1;
    digital_less=-1;
    digital_0=-1;
    std::cout<<"No digital occupancy found for module:\t"<<aReader_HV->Stat_Module_ID<<std::endl;
  }
  if(aReader_HV->Anal_Occupancy_Point_000!=NULL){
    nbinx = aReader_HV->Anal_Occupancy_Point_000->GetNbinsX();
    nbiny = aReader_HV->Anal_Occupancy_Point_000->GetNbinsY();
    for(int i=0; i<nbinx; i++){
      for(int j=0; j<nbiny; j++){
                
                
	if(aReader_HV->Anal_Occupancy_Point_000->GetBinContent(i+1,j+1)==0)analog_0++;
                
	if(aReader_HV->Stat_processPosition==PP_UsbPixReceptionTest){
	  if(aReader_HV->Anal_Occupancy_Point_000->GetBinContent(i+1,j+1)>usbpix_limit)analog_greater++;
	  if(aReader_HV->Anal_Occupancy_Point_000->GetBinContent(i+1,j+1)<usbpix_limit)analog_less++;
	}
	else{
	  if(aReader_HV->Anal_Occupancy_Point_000->GetBinContent(i+1,j+1)>rce_limit)analog_greater++;
	  if(aReader_HV->Anal_Occupancy_Point_000->GetBinContent(i+1,j+1)<rce_limit)analog_less++;
	}
      }
    }
  }
  else{
    analog_greater=-1;
    analog_less=-1;
    analog_0=-1;
    std::cout<<"No analog occupancy found for module:\t"<<aReader_HV->Stat_Module_ID<<std::endl;
  }
  if(aReader_HV->Thre_thresh2d!=NULL){
    nbinx = aReader_HV->Thre_thresh2d->GetNbinsX();
    nbiny = aReader_HV->Thre_thresh2d->GetNbinsY();
    for(int i=0; i<nbinx; i++){
      for(int j=0; j<nbiny; j++){
	if(aReader_HV->Thre_thresh2d->GetBinContent(i+1,j+1)==0)thres_0++;
      }
    }
  }
  else{
    thres_0 = -1;
    std::cout<<"No threshold occupancy (HV ON) found for module:\t"<<aReader_HV->Stat_Module_ID<<std::endl;
  }
  if(aReader_HV->Thre_Sigma!=NULL){
    nbinx = aReader_HV->Thre_Sigma->GetNbinsX();
    nbiny = aReader_HV->Thre_Sigma->GetNbinsY();
    for(int i=0; i<nbinx; i++){
      for(int j=0; j<nbiny; j++){
	if(aReader_HV->Thre_Sigma->GetBinContent(i+1,j+1)>=noise_limit)thres_noisy++;
      }
    }
  }
  else{
    thres_noisy=-1;
    std::cout<<"No threshold sigma (HV ON) found for module:\t"<<aReader_HV->Stat_Module_ID<<std::endl;
  }
	
  double mean, sigma, sigmalimit=40;
  if(aReader_NO_HV->Thre_Sigma!=NULL  && aReader_NO_HV->Thre_sigmadist->GetEntries() > 1){
    Fit_noisedistro(aReader_NO_HV->Thre_sigmadist);
    mean = aReader_NO_HV->Thre_sigmadist->GetFunction("gaus1")->GetParameter(1);
    sigma = aReader_NO_HV->Thre_sigmadist->GetFunction("gaus1")->GetParameter(2);
    if(sigma>sigmalimit)sigma=sigmalimit;
    nbinx = aReader_NO_HV->Thre_Sigma->GetNbinsX();
    nbiny = aReader_NO_HV->Thre_Sigma->GetNbinsY();
    for(int i=0; i<nbinx; i++){
      for(int j=0; j<nbiny; j++){
	if(aReader_NO_HV->Thre_Sigma->GetBinContent(i+1,j+1)<=mean-5*fabs(sigma) && aReader_NO_HV->Thre_Sigma->GetBinContent(i+1,j+1)<=200 && aReader_NO_HV->Thre_Sigma->GetBinContent(i+1,j+1)>=1)thres_disco++;
      }
    }
  }
  else{
    thres_disco=-1;
    std::cout<<"No threshold sigma (HV OFF) found for module:\t"<<aReader_NO_HV->Stat_Module_ID<<std::endl;
  }
  if(aReader_HV->Cros_crosstalk2d!=NULL){
    nbinx = aReader_HV->Cros_crosstalk2d->GetNbinsX();
    nbiny = aReader_HV->Cros_crosstalk2d->GetNbinsY();
    for(int i=0; i<nbinx; i++){
      for(int j=0; j<nbiny; j++){
	if(aReader_HV->Cros_crosstalk2d->GetBinContent(i+1,j+1)!=0)xtalk++;
      }
    }
  }
  else{
    xtalk=-1;
    std::cout<<"No x-talk occupancy (HV ON) found for module:\t"<<aReader_HV->Stat_Module_ID<<std::endl;
  }
       
  int badpixelarray[10]={digital_0,digital_less,digital_greater,analog_0,analog_less,analog_greater,thres_0,thres_disco,thres_noisy,xtalk};
  std::cout<<std::endl<<std::endl<<"BAD DIGITAL: "<<badpixelarray[0]+badpixelarray[1]+badpixelarray[2]<<std::endl;
  std::cout<<"BAD ANALOG: "<<badpixelarray[3]+badpixelarray[4]+badpixelarray[5]<<std::endl;
  std::cout<<"BAD THRESHOLD: "<<badpixelarray[6]+badpixelarray[7]+badpixelarray[8]<<std::endl<<std::endl<<std::endl;
    
  //    aReader_HV->HistoReader::~HistoReader();
  //    aReader_NO_HV->HistoReader::~HistoReader();
  return badpixelarray;
  delete badpixelarray;
}

int TotalAmountBadPixel(HistoReader *aReader_HV,HistoReader *aReader_NO_HV){
  int total=0, nbinx = 0, nbiny = 0;
  double limit,usbpix_limit = 200., rce_limit = 50., noise_limit = 350.;
  bool digibad=false;
	
  std::cout<<"TABP: check 1"<<std::endl;

  switch (aReader_HV->Stat_processPosition) {
  case PP_UsbPixReceptionTest:
    limit = usbpix_limit;
    break;
            
  default:
    limit = rce_limit;
    break;
  }
	
  if(aReader_HV->Digi_Occupancy_Point_000!=NULL && aReader_HV->Anal_Occupancy_Point_000!=NULL && aReader_HV->Thre_thresh2d!=NULL && aReader_HV->Thre_Sigma!=NULL && aReader_NO_HV->Thre_Sigma!=NULL && aReader_NO_HV->Thre_sigmadist->GetEntries() > 1){
    nbinx = aReader_HV->Digi_Occupancy_Point_000->GetNbinsX();
    nbiny = aReader_HV->Digi_Occupancy_Point_000->GetNbinsY();
    double mean, sigma,sigmalimit=40;
    Fit_noisedistro(aReader_NO_HV->Thre_sigmadist);		
    mean = aReader_NO_HV->Thre_sigmadist->GetFunction("gaus1")->GetParameter(1);
    sigma = aReader_NO_HV->Thre_sigmadist->GetFunction("gaus1")->GetParameter(2);
    for(int i=0; i<nbinx; i++){
      for(int j=0; j<nbiny; j++){
	//NULL Entries


	if(sigma>sigmalimit)sigma=sigmalimit;              
	if(aReader_HV->Digi_Occupancy_Point_000->GetBinContent(i+1,j+1)!=limit){
	  total++;
					
                    
	}
	else if(aReader_HV->Digi_Occupancy_Point_000->GetBinContent(i+1,j+1)==limit && aReader_HV->Anal_Occupancy_Point_000->GetBinContent(i+1,j+1)!=limit)total++;
	else if(aReader_HV->Digi_Occupancy_Point_000->GetBinContent(i+1,j+1)==limit && aReader_HV->Anal_Occupancy_Point_000->GetBinContent(i+1,j+1)==limit && aReader_HV->Thre_thresh2d->GetBinContent(i+1,j+1)==0)total++;
	else if(aReader_HV->Thre_Sigma->GetBinContent(i+1,j+1)>=noise_limit)total++;
	//else if(aReader_HV->Digi_Occupancy_Point_000->GetBinContent(i+1,j+1)==limit && aReader_HV->Anal_Occupancy_Point_000->GetBinContent(i+1,j+1)==limit && aReader_HV->Thre_Sigma->GetBinContent(i+1,j+1)>=noise_limit)total++;
	else if(aReader_NO_HV->Thre_Sigma->GetBinContent(i+1,j+1)!=0 && aReader_NO_HV->Thre_Sigma->GetBinContent(i+1,j+1)<=mean-5*fabs(sigma) && aReader_NO_HV->Thre_Sigma->GetBinContent(i+1,j+1)<=200)total++;
	
      }
    }
  }else if(aReader_HV->Digi_Occupancy_Point_000==NULL && aReader_HV->Anal_Occupancy_Point_000==NULL && aReader_HV->Thre_thresh2d==NULL && aReader_HV->Thre_Sigma==NULL && aReader_NO_HV->Thre_Sigma==NULL) total = -1;

  return total;
}

void BadPixelTXT(ProcessPosition PP, std::vector<std::string> modules){
  HistoReader *hr_BP_hvON_1, *hr_BP_hvOFF_1,*hr_BP_hvON_2, *hr_BP_hvOFF_2;
  std::vector <int> BParray_1;
  std::vector <int> BParray_2;
	
  struct tm *ptr;
  time_t lt;
  char date_str[10];
    
  lt = time(NULL);
  ptr = localtime(&lt);
    
  strftime(date_str, 10, "%D", ptr);
    
  std::string filename;
  std::string bp_string = "_BadPixel_";
  std::string ProcessString;
  std::string address = DbTools::GetInstance()->DatabasePath() +"/analysis_result/";
    
  switch (PP) {
  case PP_UsbPixReceptionTest:
    ProcessString = "UsbPixrt";
    break;
  case PP_RCEReceptionTest:
    ProcessString = "RCErt";
    break;
  case PP_StaveTestBeforeTC_AT:
    ProcessString = "StaveBTC_AT";
    break;
  case PP_StaveTestAfterTC_AT:
    ProcessString = "StaveATC_AT";
    break;
  case PP_StaveTestBeforeTC_BT:
    ProcessString = "StaveBTC_BT";
    break;
  case PP_StaveTestAfterTC_BT:
    ProcessString = "StaveATC_BT";
    break;
            
  default:
    break;
  }
    
  filename.append(address);
  filename.append(date_str);
  filename.append(bp_string);
  filename.append(ProcessString);
	
  char *filename_char;
    
  strcpy (filename_char, filename.c_str());
  ofstream infile(filename_char);
  infile<<"ModuleID\tDigital0\tDigiLess\tDigiGreat\tAnalog0\tAnalLess\tAnalGreat\tThresh0\tThresDisco\tThreshNoisy\tX-talk\tTotal"<<std::endl;
    
  char *modIDchar;
    
  for (std::vector<std::string>::iterator iii = modules.begin(); iii != modules.end(); iii++)
    {
		
      if (DbTools::GetInstance()->IsDoubleChip(iii->c_str())==1) {
	hr_BP_hvON_1 = new HistoReader(iii->c_str(), PP, 0,1);
	hr_BP_hvOFF_1 = new HistoReader(iii->c_str(), PP, 0,0);
	BParray_1 = BadPixelMonster2(hr_BP_hvON_1,hr_BP_hvOFF_1);
	strcpy(modIDchar,iii->c_str());
	infile << modIDchar << "_" << 0;
	for (int i=0; i<10; i++) {
	  infile<<"\t"<<BParray_1[i];
	}
	infile<<std::endl;
            
	hr_BP_hvON_2 = new HistoReader(iii->c_str(), PP, 1,1);
	hr_BP_hvOFF_2 = new HistoReader(iii->c_str(), PP, 1,0);
	BParray_2 = BadPixelMonster2(hr_BP_hvON_2,hr_BP_hvOFF_2);
	strcpy(modIDchar,iii->c_str());
	infile << modIDchar << "_" << 1;
	for (int i=0; i<10; i++) {
	  infile<<"\t"<<BParray_2[i];
	}
	infile<<"\t"<<TotalAmountBadPixel(hr_BP_hvON_1,hr_BP_hvOFF_1)<<std::endl;
      }
      else{
	hr_BP_hvON_1 = new HistoReader(iii->c_str(), PP, 0,1);
	hr_BP_hvOFF_1 = new HistoReader(iii->c_str(), PP, 0,0);
	BParray_1 = BadPixelMonster2(hr_BP_hvON_1,hr_BP_hvOFF_1);
	strcpy(modIDchar,iii->c_str());
	infile << modIDchar << "_" << 0;
	for (int i=0; i<10; i++) {  infile<<"\t"<<BParray_1[i]; }
	infile<<"\t"<<TotalAmountBadPixel(hr_BP_hvON_2,hr_BP_hvOFF_2)<<std::endl;
      }
    }
  infile.close();
}

void DigitalSummaryFE(ProcessPosition pp, std::vector<std::string> modules){
  HistoReader *aReader_HV_1, *aReader_HV_2;
  int counter = 0, usbpix_limit = 200, rce_limit = 50, limit;

  if(pp = PP_UsbPixReceptionTest)limit = usbpix_limit;
  else limit = rce_limit;

  TH2D *digibadmap = new TH2D("Overall Map of bad Digital Pixel", "DigiBad2D", 80,0,80,336,0,336);
  for(std::vector <std::string>::iterator iii= modules.begin(); iii!=modules.end(); iii++){
    counter++;
    if(DbTools::GetInstance()->IsDoubleChip(iii->c_str())==1){
      aReader_HV_2 = new HistoReader(iii->c_str(),pp, 1, 1);
      aReader_HV_1 = new HistoReader(iii->c_str(),pp, 0, 1);

      if(aReader_HV_1->Digi_Occupancy_Point_000 !=NULL && aReader_HV_2->Digi_Occupancy_Point_000!=NULL){
	for(int i=0; i<aReader_HV_1->Digi_Occupancy_Point_000->GetNbinsX(); i++){
	  for(int j=0; j<aReader_HV_1->Digi_Occupancy_Point_000->GetNbinsX(); j++){
	    if((int)(aReader_HV_1->Digi_Occupancy_Point_000->GetBinContent(i+1,j+1))!=limit)digibadmap->Fill(i+1,j+1);
	    if((int)(aReader_HV_2->Digi_Occupancy_Point_000->GetBinContent(i+1,j+1))!=limit)digibadmap->Fill(i+1,j+1);
          }
        }
      }
    }else{
      aReader_HV_1 = new HistoReader(iii->c_str(),pp, 0, 1);

      if(aReader_HV_1->Digi_Occupancy_Point_000 !=NULL){
	for(int i=0; i<aReader_HV_1->Digi_Occupancy_Point_000->GetNbinsX(); i++){
	  for(int j=0; j<aReader_HV_1->Digi_Occupancy_Point_000->GetNbinsX(); j++){
	    if((int)(aReader_HV_1->Digi_Occupancy_Point_000->GetBinContent(i+1,j+1))!=limit)digibadmap->Fill(i+1,j+1);
          }
        }
      }
    }
  }
  FicoPlot();
  digibadmap->Draw("colz");
  drawAtlasInternal2D();
}

void AnalogSummaryFE(ProcessPosition pp, std::vector<std::string> modules){
  HistoReader *aReader_HV_1, *aReader_HV_2;
  int counter = 0, usbpix_limit = 200, rce_limit = 50, limit;

  if(pp = PP_UsbPixReceptionTest)limit = usbpix_limit;
  else limit = rce_limit;

  TH2D *analbadmap = new TH2D("Overall Map of bad Analog Pixel", "AnalogBad2D", 80,0,80,336,0,336);
  for(std::vector <std::string>::iterator iii= modules.begin(); iii!=modules.end(); iii++){
    counter++;
    if(DbTools::GetInstance()->IsDoubleChip(iii->c_str())==1){
      aReader_HV_2 = new HistoReader(iii->c_str(),pp, 1, 1);
      aReader_HV_1 = new HistoReader(iii->c_str(),pp, 0, 1);

      if(aReader_HV_1->Anal_Occupancy_Point_000 !=NULL && aReader_HV_2->Anal_Occupancy_Point_000!=NULL){
	for(int i=0; i<aReader_HV_1->Anal_Occupancy_Point_000->GetNbinsX(); i++){
	  for(int j=0; j<aReader_HV_1->Anal_Occupancy_Point_000->GetNbinsY(); j++){
	    if((int)(aReader_HV_1->Anal_Occupancy_Point_000->GetBinContent(i+1,j+1))!=limit)analbadmap->Fill(i+1,j+1);
	    if((int)(aReader_HV_2->Anal_Occupancy_Point_000->GetBinContent(i+1,j+1))!=limit)analbadmap->Fill(i+1,j+1);
          }
        }
      }
    }else{
      aReader_HV_1 = new HistoReader(iii->c_str(),pp, 0, 1);

      if(aReader_HV_1->Anal_Occupancy_Point_000 !=NULL){
	for(int i=0; i<aReader_HV_1->Anal_Occupancy_Point_000->GetNbinsX(); i++){
	  for(int j=0; j<aReader_HV_1->Anal_Occupancy_Point_000->GetNbinsY(); j++){
	    if((int)(aReader_HV_1->Anal_Occupancy_Point_000->GetBinContent(i+1,j+1))!=limit)analbadmap->Fill(i+1,j+1);
          }
        }
      }
    }
  }
  FicoPlot();
  analbadmap->Draw("colz");
  drawAtlasInternal2D();
}

void DisconnectedSummaryFE(ProcessPosition pp, std::vector<std::string> modules){
  HistoReader *aReader_noHV_1, *aReader_noHV_2;
  int counter = 0, usbpix_limit = 200, rce_limit = 50, limit, nbinx, nbiny;

  if(pp = PP_UsbPixReceptionTest)limit = usbpix_limit;
  else limit = rce_limit;

  TH2D *discomap = new TH2D("Overall Map of bad Disconnected Bumps", "Disconnected2D", 80,0,80,336,0,336);
  for(std::vector <std::string>::iterator iii= modules.begin(); iii!=modules.end(); iii++){
    counter++;
    if(DbTools::GetInstance()->IsDoubleChip(iii->c_str())==1){
      aReader_noHV_2 = new HistoReader(iii->c_str(),pp, 1, 0);
      aReader_noHV_1 = new HistoReader(iii->c_str(),pp, 0, 0);


      if(aReader_noHV_1->Thre_Sigma!=NULL && aReader_noHV_1->Thre_sigmadist!=NULL && aReader_noHV_1->Thre_sigmadist->GetEntries() > 1){
        nbinx = aReader_noHV_1->Thre_Sigma->GetNbinsX();
        nbiny = aReader_noHV_1->Thre_Sigma->GetNbinsY();

        double mean, sigma, sigmalimit=40;  
  
        Fit_noisedistro(aReader_noHV_1->Thre_sigmadist);
        mean = aReader_noHV_1->Thre_sigmadist->GetFunction("gaus1")->GetParameter(1);
        sigma = aReader_noHV_1->Thre_sigmadist->GetFunction("gaus1")->GetParameter(2);

        if(sigma>sigmalimit)sigma=sigmalimit;
        double mm = mean -5*fabs(sigma);


        for(int i=0; i<nbinx; i++){
          for(int j=0; j<nbiny; j++){
	    if(aReader_noHV_1->Thre_Sigma->GetBinContent(i+1,j+1)<mm && aReader_noHV_1->Thre_Sigma->GetBinContent(i+1,j+1)>=1 && aReader_noHV_1->Thre_Sigma->GetBinContent(i+1,j+1)<200)discomap->Fill(i+1,j+1);
          }
        }
      }
   
      if(aReader_noHV_2->Thre_Sigma!=NULL && aReader_noHV_2->Thre_sigmadist!=NULL && aReader_noHV_2->Thre_sigmadist->GetEntries() > 1){
        nbinx = aReader_noHV_2->Thre_Sigma->GetNbinsX();
        nbiny = aReader_noHV_2->Thre_Sigma->GetNbinsY();

        double mean, sigma, sigmalimit=40;  
  
        Fit_noisedistro(aReader_noHV_2->Thre_sigmadist);
        mean = aReader_noHV_2->Thre_sigmadist->GetFunction("gaus1")->GetParameter(1);
        sigma = aReader_noHV_2->Thre_sigmadist->GetFunction("gaus1")->GetParameter(2);

        if(sigma>sigmalimit)sigma=sigmalimit;
        double mm = mean -5*fabs(sigma);


        for(int i=0; i<nbinx; i++){
          for(int j=0; j<nbiny; j++){
	    if(aReader_noHV_2->Thre_Sigma->GetBinContent(i+1,j+1)<mm && aReader_noHV_2->Thre_Sigma->GetBinContent(i+1,j+1)>=1 && aReader_noHV_2->Thre_Sigma->GetBinContent(i+1,j+1)<200)discomap->Fill(i+1,j+1);
	  }
	}
      }
    }else{
      aReader_noHV_1 = new HistoReader(iii->c_str(),pp, 0, 0);


      if(aReader_noHV_1->Thre_Sigma!=NULL && aReader_noHV_1->Thre_sigmadist!=NULL && aReader_noHV_1->Thre_sigmadist->GetEntries() > 1){
	nbinx = aReader_noHV_1->Thre_Sigma->GetNbinsX();
	nbiny = aReader_noHV_1->Thre_Sigma->GetNbinsY();
	std::cout<<"module 3d is: "<<iii->c_str()<<std::endl;
	double mean, sigma, sigmalimit=40;  
  
	Fit_noisedistro(aReader_noHV_1->Thre_sigmadist);
	mean = aReader_noHV_1->Thre_sigmadist->GetFunction("gaus1")->GetParameter(1);
	sigma = aReader_noHV_1->Thre_sigmadist->GetFunction("gaus1")->GetParameter(2);

	if(sigma>sigmalimit)sigma=sigmalimit;
	double mm = mean -5*fabs(sigma);


	for(int i=0; i<nbinx; i++){
	  for(int j=0; j<nbiny; j++){
	    if(aReader_noHV_1->Thre_Sigma->GetBinContent(i+1,j+1)<mm && aReader_noHV_1->Thre_Sigma->GetBinContent(i+1,j+1)>=1 && aReader_noHV_1->Thre_Sigma->GetBinContent(i+1,j+1)<200)discomap->Fill(i+1,j+1);
	  }
	}
      }
    }
    
  }
  FicoPlot();
  std::cout<<"Module analysis runned over "<<counter<<" modules"<<std::endl;
  discomap->Draw("colz");
  drawAtlasInternal2D();
}




void SummaryPlot(ProcessPosition PP, std::vector<std::string> modules, TCanvas* module_canvas){
  HistoReader *aReader_HV_1, *aReader_NO_HV_1, *aReader_HV_2, *aReader_NO_HV_2;
  int numbin = 26880;
  THStack* summaryhist_CNM =new THStack("summaryhist_CNM","summaryplot_CNM");
  THStack* summaryhist_DC = new THStack("summaryhist_DC","summaryplot_DC");
  THStack* summaryhist_FBK = new THStack("summaryhist_DC","summaryplot_FBK");

  std::vector <int> badarray;//{0};
  int digibad_DC=0, analbad_DC=0, disconnectedbumps_DC=0,noisy_DC=0,total_DC=0;
  int digibad_FBK=0, analbad_FBK=0, disconnectedbumps_FBK=0,noisy_FBK=0,total_FBK=0;
  int digibad_CNM=0, analbad_CNM=0, disconnectedbumps_CNM=0,noisy_CNM=0,total_CNM=0;

  TH1I *digibadamount_DC = new TH1I("digibadamount","digibadamount",numbin,0,numbin);
  TH1I *analbadamount_DC = new TH1I("analbadamount","analbadamount",numbin,0,numbin);
  TH1I *noisypixel_DC = new TH1I("noisypixel","noisypixel",numbin,0,numbin);
  TH1I *disconnectedbumpspixel_DC = new TH1I("disconnectedbumps","disconnectedbumps",numbin,0,numbin);
  TH1I *totalamountbadpixel_DC = new TH1I("totalamountbadpixel","totalamountbadpixel",numbin,0,numbin);
  TH1I *digibadamount_CNM = new TH1I("digibadamount","digibadamount",numbin,0,numbin);
  TH1I *analbadamount_CNM = new TH1I("analbadamount","analbadamount",numbin,0,numbin);
  TH1I *noisypixel_CNM = new TH1I("noisypixel","noisypixel",numbin,0,numbin);
  TH1I *disconnectedbumpspixel_CNM = new TH1I("disconnectedbumps","disconnectedbumps",numbin,0,numbin);
  TH1I *totalamountbadpixel_CNM = new TH1I("totalamountbadpixel","totalamountbadpixel",numbin,0,numbin);
  TH1I *digibadamount_FBK = new TH1I("digibadamount","digibadamount",numbin,0,numbin);
  TH1I *analbadamount_FBK = new TH1I("analbadamount","analbadamount",numbin,0,numbin);
  TH1I *noisypixel_FBK = new TH1I("noisypixel","noisypixel",numbin,0,numbin);
  TH1I *disconnectedbumpspixel_FBK = new TH1I("disconnectedbumps","disconnectedbumps",numbin,0,numbin);
  TH1I *totalamountbadpixel_FBK = new TH1I("totalamountbadpixel","totalamountbadpixel",numbin,0,numbin);


  int counter = 0;  
  for (std::vector<std::string>::iterator iii = modules.begin(); iii != modules.end(); iii++){
    counter++;


    if(DbTools::GetInstance()->IsDoubleChip(iii->c_str())==1){
      std::cout<<"I'm in DC part"<<std::endl;
      /*HistoReader* */aReader_HV_1 = new HistoReader(iii->c_str(),PP, 0, 1);
      std::cout<<iii->c_str()<<std::endl;
      /*HistoReader* */aReader_NO_HV_1 = new HistoReader(iii->c_str(),PP, 0, 0);
	

      badarray = BadPixelMonsterHierarchy(aReader_HV_1,aReader_NO_HV_1);

      digibad_DC = badarray[9] + badarray[8] + badarray[7];
      analbad_DC = badarray[6] + badarray[5] + badarray[4];
      disconnectedbumps_DC = badarray[2];
      noisy_DC = badarray[1];
      total_DC = badarray[10];
            
      delete aReader_HV_1;
      delete aReader_NO_HV_1;
	

      for (int i=1; i<=numbin; i++) {
	if(digibad_DC<i && digibad_DC>-1)digibadamount_DC->AddBinContent(i);
	if(analbad_DC<i && analbad_DC>-1 && digibad_DC>-1)analbadamount_DC->AddBinContent(i);
	if(noisy_DC<i && noisy_DC>-1 && analbad_DC>-1 && digibad_DC>-1)noisypixel_DC->AddBinContent(i);
	if(disconnectedbumps_DC<i && disconnectedbumps_DC>-1 && analbad_DC>-1 && digibad_DC>-1)disconnectedbumpspixel_DC->AddBinContent(i);
	if(total_DC<i && total_DC>-1 && analbad_DC>-1 && digibad_DC>-1)totalamountbadpixel_DC->AddBinContent(i);
      }

      /*HistoReader* */aReader_HV_2 = new HistoReader(iii->c_str(),PP, 1, 1);
      /*HistoReader* */aReader_NO_HV_2 = new HistoReader(iii->c_str(),PP, 1, 0);
      badarray = BadPixelMonsterHierarchy(aReader_HV_2,aReader_NO_HV_2);
            
      digibad_DC = badarray[9] + badarray[8] + badarray[7];
      analbad_DC = badarray[6] + badarray[5] + badarray[4];
      disconnectedbumps_DC = badarray[2];
      noisy_DC = badarray[1];
      total_DC = badarray[10];
  
      delete aReader_HV_2;
      delete aReader_NO_HV_2;

      for (int i=1; i<=numbin; i++) {
	if(digibad_DC<i && digibad_DC>-1)digibadamount_DC->AddBinContent(i);
	if(analbad_DC<i && analbad_DC>-1 && digibad_DC>-1)analbadamount_DC->AddBinContent(i);
	if(noisy_DC<i && noisy_DC>-1 && analbad_DC>-1 && digibad_DC>-1)noisypixel_DC->AddBinContent(i);
	if(disconnectedbumps_DC<i && disconnectedbumps_DC>-1 && analbad_DC>-1 && digibad_DC>-1)disconnectedbumpspixel_DC->AddBinContent(i);
	if(total_DC<i && total_DC>-1 && analbad_DC>-1 && digibad_DC>-1)totalamountbadpixel_DC->AddBinContent(i);
      }

            
    }
    else{
      HistoReader* aReader_HV_1 = new HistoReader(iii->c_str(),PP, 0, 1);
      HistoReader* aReader_NO_HV_1 = new HistoReader(iii->c_str(),PP, 0, 0);
      if(aReader_HV_1->Stat_Module_ID.substr(0,1)=="C"){
	badarray = BadPixelMonsterHierarchy(aReader_HV_1,aReader_NO_HV_1);

	digibad_CNM = badarray[9] + badarray[8] + badarray[7];
  	analbad_CNM = badarray[6] + badarray[5] + badarray[4];
	disconnectedbumps_CNM = badarray[2];
	noisy_CNM = badarray[1];
	total_CNM = badarray[10];

	delete aReader_HV_1;
	delete aReader_NO_HV_1;

	for (int i=1; i<=numbin; i++) {
	  if(digibad_CNM<i && digibad_CNM>-1)digibadamount_CNM->AddBinContent(i);
	  if(analbad_CNM<i && analbad_CNM>-1 && digibad_CNM>-1)analbadamount_CNM->AddBinContent(i);
	  if(noisy_CNM<i && noisy_CNM>-1 &&  analbad_CNM>-1 && digibad_CNM>-1)noisypixel_CNM->AddBinContent(i);
	  if(disconnectedbumps_CNM<i && disconnectedbumps_CNM>1 && analbad_CNM>-1 && digibad_CNM>-1)disconnectedbumpspixel_CNM->AddBinContent(i);
	  if(total_CNM<i && total_CNM>-1 && disconnectedbumps_CNM>1 && analbad_CNM>-1 && digibad_CNM>-1)totalamountbadpixel_CNM->AddBinContent(i);
	}
      }else if(aReader_HV_1->Stat_Module_ID.substr(0,1)=="F"){

	badarray = BadPixelMonsterHierarchy(aReader_HV_1,aReader_NO_HV_1);

	digibad_FBK = badarray[9] + badarray[8] + badarray[7];
	analbad_FBK = badarray[6] + badarray[5] + badarray[4];
	disconnectedbumps_FBK = badarray[2];
	noisy_FBK = badarray[1];
	total_FBK = badarray[10];

	delete aReader_HV_1;
	delete aReader_NO_HV_1;

	for (int i=1; i<=numbin; i++) {
	  if(digibad_FBK<i && digibad_FBK>-1)digibadamount_FBK->AddBinContent(i);
	  if(analbad_FBK<i && analbad_FBK>-1 && digibad_FBK>-1)analbadamount_FBK->AddBinContent(i);
	  if(noisy_FBK<i && noisy_FBK>-1 &&  analbad_FBK>-1 && digibad_FBK>-1)noisypixel_FBK->AddBinContent(i);
	  if(disconnectedbumps_FBK<i && disconnectedbumps_FBK>1 && analbad_FBK>-1 && digibad_FBK>-1)disconnectedbumpspixel_FBK->AddBinContent(i);
	  if(total_FBK<i && total_FBK>-1 && disconnectedbumps_FBK>1 && analbad_FBK>-1 && digibad_FBK>-1)totalamountbadpixel_FBK->AddBinContent(i);
        }  

      }  
    
    }
        
  }
    
  std::cout<<"Modules analyzed by summary plot function: "<<counter<<std::endl;

  digibadamount_FBK->SetLineColor(kRed);
  digibadamount_FBK->SetLineWidth(3);
  digibadamount_FBK->SetLineStyle(2);
  analbadamount_FBK->SetLineColor(kBlue);
  analbadamount_FBK->SetLineWidth(3);
  analbadamount_FBK->SetLineStyle(2);
  noisypixel_FBK->SetLineColor(kViolet);
  noisypixel_FBK->SetLineWidth(3);
  noisypixel_FBK->SetLineStyle(2);
  disconnectedbumpspixel_FBK->SetLineColor(kGreen);
  disconnectedbumpspixel_FBK->SetLineWidth(3);
  disconnectedbumpspixel_FBK->SetLineStyle(2);
  totalamountbadpixel_FBK->SetLineColor(kBlack);
  totalamountbadpixel_FBK->SetLineWidth(3);
  //  totalamountbadpixel_FBK->SetFillColor(kBlack);
  totalamountbadpixel_FBK->SetLineStyle(2);
    
  digibadamount_CNM->SetLineColor(kRed);
  digibadamount_CNM->SetLineWidth(3);
  digibadamount_CNM->SetLineStyle(9);
  analbadamount_CNM->SetLineColor(kBlue);
  analbadamount_CNM->SetLineWidth(3);
  analbadamount_CNM->SetLineStyle(9);
  noisypixel_CNM->SetLineColor(kViolet);
  noisypixel_CNM->SetLineWidth(3);
  noisypixel_CNM->SetLineStyle(9);
  disconnectedbumpspixel_CNM->SetLineColor(kGreen);
  disconnectedbumpspixel_CNM->SetLineWidth(3);
  disconnectedbumpspixel_CNM->SetLineStyle(9);
  totalamountbadpixel_CNM->SetLineColor(kBlack);
  totalamountbadpixel_CNM->SetLineWidth(3);
  totalamountbadpixel_CNM->SetLineStyle(9);
  //  totalamountbadpixel_CNM->SetFillColor(kBlack);
  //  totalamountbadpixel_CNM->SetFillStyle(3001);	

  digibadamount_DC->SetLineColor(kRed);
  digibadamount_DC->SetLineWidth(3);
  analbadamount_DC->SetLineColor(kBlue);
  analbadamount_DC->SetLineWidth(3);
  noisypixel_DC->SetLineColor(kViolet);
  noisypixel_DC->SetLineWidth(3);
  disconnectedbumpspixel_DC->SetLineColor(kGreen);
  disconnectedbumpspixel_DC->SetLineWidth(3);
  totalamountbadpixel_DC->SetLineColor(kBlack);
  totalamountbadpixel_DC->SetLineWidth(3);
  //  totalamountbadpixel_DC->SetFillColor(kBlack);
  //  totalamountbadpixel_DC->SetFillStyle(3001);

 
  summaryhist_DC->Add(digibadamount_DC);
  summaryhist_DC->Add(analbadamount_DC);
  summaryhist_DC->Add(noisypixel_DC);
  summaryhist_DC->Add(disconnectedbumpspixel_DC);
  summaryhist_DC->Add(totalamountbadpixel_DC);
  gPad->SetGrid();
  summaryhist_DC->Draw("HIST nostack");
  summaryhist_DC->GetXaxis()->SetTitle("Bad Pixel cut");
  summaryhist_DC->GetXaxis()->SetTitleColor(kBlack);
  summaryhist_DC->GetYaxis()->SetTitle("Number of modules");
  summaryhist_DC->GetYaxis()->SetTitleColor(kBlack);
  summaryhist_DC->GetXaxis()->SetRangeUser(0,1000);
 
  summaryhist_CNM->Add(digibadamount_CNM);
  summaryhist_CNM->Add(analbadamount_CNM);
  summaryhist_CNM->Add(noisypixel_CNM);
  summaryhist_CNM->Add(disconnectedbumpspixel_CNM);
  summaryhist_CNM->Add(totalamountbadpixel_CNM);
 
  summaryhist_CNM->Draw("HIST nostack same");
  summaryhist_CNM->GetXaxis()->SetTitle("Bad Pixel cut");
  summaryhist_CNM->GetXaxis()->SetTitleColor(kBlack);
  summaryhist_CNM->GetYaxis()->SetTitle("Number of modules");
  summaryhist_CNM->GetYaxis()->SetTitleColor(kBlack);

  summaryhist_FBK->Add(digibadamount_FBK);
  summaryhist_FBK->Add(analbadamount_FBK);
  summaryhist_FBK->Add(noisypixel_FBK);
  summaryhist_FBK->Add(disconnectedbumpspixel_FBK);
  summaryhist_FBK->Add(totalamountbadpixel_FBK);

  summaryhist_FBK->Draw("HIST nostack same");
  summaryhist_FBK->GetXaxis()->SetTitle("Bad Pixel cut");
  summaryhist_FBK->GetXaxis()->SetTitleColor(kBlack);
  summaryhist_FBK->GetYaxis()->SetTitle("Number of modules");
  summaryhist_FBK->GetYaxis()->SetTitleColor(kBlack);
	
 
    
  TLegend* leg = new TLegend(0.75,0.1,0.9,0.3);
  leg->SetHeader("Summary Plot");
  leg->AddEntry(digibadamount_FBK,"Digital Bad FBK","l");
  leg->AddEntry(analbadamount_FBK,"Analog Bad FBK","l");
  leg->AddEntry(noisypixel_FBK,"Noisy Pixel FBK","l");
  leg->AddEntry(disconnectedbumpspixel_FBK,"Disconnected Bumps FBK","l");
  leg->AddEntry(totalamountbadpixel_FBK,"Total Bad FBK","l");

  leg->AddEntry(digibadamount_CNM,"Digital Bad CNM","l");
  leg->AddEntry(analbadamount_CNM,"Analog Bad CNM","l");
  leg->AddEntry(noisypixel_CNM,"Noisy Pixel CNM","l");
  leg->AddEntry(disconnectedbumpspixel_CNM,"Disconnected Bumps CNM","l");
  leg->AddEntry(totalamountbadpixel_CNM,"Total Bad CNM","l");

  leg->AddEntry(digibadamount_DC,"Digital Bad DC","l");
  leg->AddEntry(analbadamount_DC,"Analog Bad DC","l");
  leg->AddEntry(noisypixel_DC,"Noisy Pixel DC","l");
  leg->AddEntry(disconnectedbumpspixel_DC,"Disconnected Bumps DC","l");
  leg->AddEntry(totalamountbadpixel_DC,"Total Bad DC","l");


  leg->SetFillColor(kYellow-10);
  leg->Draw();

  

  //  delete digibadamount;
  //  delete analbadamount;
  //  delete noisypixel;
  //  delete disconnectedbumpspixel;
  //  delete totalamountbadpixel;
  //  delete summaryhist;
    
}

void DigitalBad_Stave(std::vector <HistoReader*> stavevector,std::vector <HistoReader*> stavevector_noHV){
  
  std::vector <int> badpixelarray;
  THStack* digitalbadstave = new THStack("DigitalBadPixel", "DigitalBadPixel");
    
  TH1I* deaddigital = new TH1I("DeadDigital", "DeadDigital",32,0,32);
  TH1I* greaterdigital = new TH1I("GreaterDigital", "GreaterDigital",32,0,32);
  TH1I* lessdigital = new TH1I("LessDigital", "LessDigital",32,0,32);
  TString label[32]={"A8_2","A8_1","A7_2","A7_1","A6_2","A6_1","A5_2","A5_1","A4_2","A4_1","A3_2","A3_1","A2_2","A2_1","A1_2","A1_1","C1_1","C1_2","C2_1","C2_2","C3_1","C3_2","C4_1","C4_2","C5_1","C5_2","C6_1","C6_2","C7_1","C7_2","C8_1","C8_2"};
      
  for(unsigned i=0;i<stavevector.size();i++){
    badpixelarray = BadPixelMonster2(stavevector[i],stavevector_noHV[i]);
    deaddigital->SetBinContent(i+1,badpixelarray[9]);
    greaterdigital->SetBinContent(i+1,badpixelarray[7]);
    lessdigital->SetBinContent(i+1,badpixelarray[8]);
  }
  deaddigital->SetFillColor(kOrange+10);
  greaterdigital->SetFillColor(kOrange+10);
  lessdigital->SetFillColor(kOrange+10);
  deaddigital->SetLineColor(kOrange+10);
  greaterdigital->SetLineColor(0);
  lessdigital->SetLineColor(0);

  greaterdigital->SetFillStyle(3001);
  lessdigital->SetFillStyle(3002);

  digitalbadstave->Add(deaddigital);
  digitalbadstave->Add(greaterdigital);
  digitalbadstave->Add(lessdigital);

  digitalbadstave->Draw("bartext0");
  
  for(int i=0;i<32;i++) digitalbadstave->GetXaxis()->SetBinLabel(i+1,label[i]);
  digitalbadstave->GetXaxis()->LabelsOption("v");
  digitalbadstave->Draw("bartext0");
  
  TLegend* leg = new TLegend(0.1,0.6,0.25,0.9);
  leg->SetHeader("Bad Digital Pixel");
  leg->AddEntry(deaddigital,"dead digital pixel","F");
  leg->AddEntry(greaterdigital,"digi > 50","F");
  leg->AddEntry(lessdigital,"digi < 50","F");
  leg->SetFillColor(kYellow-10);
  leg->Draw();

}

void DigitalBad_Stave2(std::vector <HistoReader*> stavevector,std::vector <HistoReader*> stavevector_noHV,std::vector <HistoReader*> stavevector2,std::vector <HistoReader*> stavevector2_noHV){
  std::vector <int> badpixelarray;
  std::vector <int> badpixelarray2;
    
  TH1I* deaddigital = new TH1I("DeadDigital", "DeadDigital",32,0,32);
  TH1I* greaterdigital = new TH1I("GreaterDigital", "GreaterDigital",32,0,32);
  TH1I* lessdigital = new TH1I("LessDigital", "LessDigital",32,0,32);
  TH1I* deaddigital2 = new TH1I("DeadDigital", "DeadDigital",32,0,32);
  TH1I* greaterdigital2 = new TH1I("GreaterDigital", "GreaterDigital",32,0,32);
  TH1I* lessdigital2 = new TH1I("LessDigital", "LessDigital",32,0,32);
        
  TString label[32]={"A8_2","A8_1","A7_2","A7_1","A6_2","A6_1","A5_2","A5_1","A4_2","A4_1","A3_2","A3_1","A2_2","A2_1","A1_2","A1_1","C1_1","C1_2","C2_1","C2_2","C3_1","C3_2","C4_1","C4_2","C5_1","C5_2","C6_1","C6_2","C7_1","C7_2","C8_1","C8_2"};
    
  for(unsigned i=0;i<stavevector.size();i++){
    
    badpixelarray = BadPixelMonster2(stavevector[i],stavevector_noHV[i]);
    badpixelarray2 = BadPixelMonster2(stavevector2[i],stavevector2_noHV[i]);
    deaddigital->SetBinContent(i+1,badpixelarray[9]);
    greaterdigital->SetBinContent(i+1,badpixelarray[7]);
    lessdigital->SetBinContent(i+1,badpixelarray[8]);
    deaddigital2->SetBinContent(i+1,badpixelarray2[9]);
    greaterdigital2->SetBinContent(i+1,badpixelarray2[7]);
    lessdigital2->SetBinContent(i+1,badpixelarray2[8]);
    
    //std::cout<<"DIGI_BAD_1: "<<BadPixelMonster2(stavevector[i],stavevector_noHV[i])<<"\tDIGI_BAD_2: "<<BadPixelMonster2(stavevector2[i],stavevector2_noHV[i])<<std::endl;
  }
  deaddigital->SetFillColor(kOrange+10);
  greaterdigital->SetFillColor(kOrange+10);
  lessdigital->SetFillColor(kOrange+10);
  deaddigital->SetLineColor(kOrange+10);
  greaterdigital->SetLineColor(0);
  lessdigital->SetLineColor(0);
  deaddigital->SetBarWidth(0.5);
  greaterdigital->SetBarWidth(0.5);
  lessdigital->SetBarWidth(0.5);
  deaddigital2->SetBarWidth(0.5);
  greaterdigital2->SetBarWidth(0.5);
  lessdigital2->SetBarWidth(0.5);
  deaddigital2->SetBarOffset(0.5);
  greaterdigital2->SetBarOffset(0.5);
  lessdigital2->SetBarOffset(0.5);
  deaddigital2->SetFillColor(kBlue);
  greaterdigital2->SetFillColor(kBlue);
  lessdigital2->SetFillColor(kBlue);
  deaddigital2->SetLineColor(kBlue);
  greaterdigital2->SetLineColor(0);
  lessdigital2->SetLineColor(0);
  greaterdigital->SetFillStyle(3001);
  lessdigital->SetFillStyle(3002);
  greaterdigital2->SetFillStyle(3001);
  lessdigital2->SetFillStyle(3002);

  //   deaddigital->Draw("bar");
  //   deaddigital2->Draw("barsame");
  
  THStack* digitalbadstave = new THStack("DigitalBadPixel", "DigitalBadPixel");
    
  digitalbadstave->Add(deaddigital);
  digitalbadstave->Add(greaterdigital);
  digitalbadstave->Add(lessdigital);
  digitalbadstave->Draw("bartext0");
   
  for(int i=0;i<32;i++) digitalbadstave->GetXaxis()->SetBinLabel(i+1,label[i]);
  digitalbadstave->GetXaxis()->LabelsOption("v");
  digitalbadstave->Draw("bartext0");
  THStack* digitalbadstave2 = new THStack("DigitalBadPixel_2", "DigitalBadPixel_2");

  digitalbadstave2->Add(deaddigital2);
  digitalbadstave2->Add(greaterdigital2);
  digitalbadstave2->Add(lessdigital2);
  digitalbadstave2->Draw("barbartext0same");

  TLegend* leg = new TLegend(0.1,0.6,0.25,0.9);
  leg->SetHeader("Bad Digital Pixel");
  leg->AddEntry(deaddigital,"dead digital pixel RT","F");
  leg->AddEntry(greaterdigital,"digi > 50 RT","F");
  leg->AddEntry(lessdigital,"digi < 50 RT","F");
  leg->AddEntry(deaddigital2,"dead digital pixel BTC","F");
  leg->AddEntry(greaterdigital2,"digi > 50 BTC","F");
  leg->AddEntry(lessdigital2,"digi < 50 BTC","F");
  leg->SetFillColor(kYellow-10);
  leg->Draw();
}

void DigitalBad_Stave3(std::vector <HistoReader*> stavevector,std::vector <HistoReader*> stavevector_noHV,std::vector <HistoReader*> stavevector2,std::vector <HistoReader*> stavevector2_noHV,std::vector <HistoReader*> stavevector3,std::vector <HistoReader*> stavevector3_noHV){
  std::vector <int> badpixelarray;
  std::vector <int> badpixelarray2;
  std::vector <int> badpixelarray3;
    
  TH1I* deaddigital = new TH1I("DeadDigital", "DeadDigital",32,0,32);
  TH1I* greaterdigital = new TH1I("GreaterDigital", "GreaterDigital",32,0,32);
  TH1I* lessdigital = new TH1I("LessDigital", "LessDigital",32,0,32);
  TH1I* deaddigital2 = new TH1I("DeadDigital", "DeadDigital",32,0,32);
  TH1I* greaterdigital2 = new TH1I("GreaterDigital", "GreaterDigital",32,0,32);
  TH1I* lessdigital2 = new TH1I("LessDigital", "LessDigital",32,0,32);
  TH1I* deaddigital3 = new TH1I("DeadDigital", "DeadDigital",32,0,32);
  TH1I* greaterdigital3 = new TH1I("GreaterDigital", "GreaterDigital",32,0,32);
  TH1I* lessdigital3 = new TH1I("LessDigital", "LessDigital",32,0,32);        


  TString label[32]={"A8_2","A8_1","A7_2","A7_1","A6_2","A6_1","A5_2","A5_1","A4_2","A4_1","A3_2","A3_1","A2_2","A2_1","A1_2","A1_1","C1_1","C1_2","C2_1","C2_2","C3_1","C3_2","C4_1","C4_2","C5_1","C5_2","C6_1","C6_2","C7_1","C7_2","C8_1","C8_2"};
    
  for(unsigned i=0;i<stavevector.size();i++){
    
    badpixelarray = BadPixelMonster2(stavevector[i],stavevector_noHV[i]);
    badpixelarray2 = BadPixelMonster2(stavevector2[i],stavevector2_noHV[i]);
    badpixelarray3 = BadPixelMonster2(stavevector3[i],stavevector3_noHV[i]);
    deaddigital->SetBinContent(i+1,badpixelarray[9]);
    greaterdigital->SetBinContent(i+1,badpixelarray[7]);
    lessdigital->SetBinContent(i+1,badpixelarray[8]);
       
    deaddigital2->SetBinContent(i+1,badpixelarray2[9]);
    greaterdigital2->SetBinContent(i+1,badpixelarray2[7]);
    lessdigital2->SetBinContent(i+1,badpixelarray2[8]);
       
    deaddigital3->SetBinContent(i+1,badpixelarray3[9]);
    greaterdigital3->SetBinContent(i+1,badpixelarray3[7]);
    lessdigital3->SetBinContent(i+1,badpixelarray3[8]);

    //std::cout<<"DIGI_BAD_1: "<<BadPixelMonster2(stavevector[i],stavevector_noHV[i])<<"\tDIGI_BAD_2: "<<BadPixelMonster2(stavevector2[i],stavevector2_noHV[i])<<std::endl;
  }
  deaddigital->SetFillColor(kOrange+10);
  greaterdigital->SetFillColor(kOrange+10);
  lessdigital->SetFillColor(kOrange+10);

  deaddigital->SetLineColor(kOrange+10);
  greaterdigital->SetLineColor(0);
  lessdigital->SetLineColor(0);

  deaddigital->SetBarWidth(0.333);
  greaterdigital->SetBarWidth(0.333);
  lessdigital->SetBarWidth(0.333);

  deaddigital2->SetBarWidth(0.333);
  greaterdigital2->SetBarWidth(0.333);
  lessdigital2->SetBarWidth(0.333);

  deaddigital2->SetBarOffset(0.333);
  greaterdigital2->SetBarOffset(0.333);
  lessdigital2->SetBarOffset(0.333);

  
  deaddigital2->SetFillColor(kBlue);
  greaterdigital2->SetFillColor(kBlue);
  lessdigital2->SetFillColor(kBlue);

  deaddigital2->SetLineColor(kBlue);
  greaterdigital2->SetLineColor(0);
  lessdigital2->SetLineColor(0);


  deaddigital3->SetBarWidth(0.333);
  greaterdigital3->SetBarWidth(0.333);
  lessdigital3->SetBarWidth(0.333);

  deaddigital3->SetBarOffset(0.666);
  greaterdigital3->SetBarOffset(0.666);
  lessdigital3->SetBarOffset(0.666);

  
  deaddigital3->SetFillColor(kBlack);
  greaterdigital3->SetFillColor(kBlack);
  lessdigital3->SetFillColor(kBlack);

  deaddigital3->SetLineColor(kBlack);
  greaterdigital3->SetLineColor(0);
  lessdigital3->SetLineColor(0);

  greaterdigital->SetFillStyle(3001);
  lessdigital->SetFillStyle(3002);

  greaterdigital2->SetFillStyle(3001);
  lessdigital2->SetFillStyle(3002);

  greaterdigital3->SetFillStyle(3001);
  lessdigital3->SetFillStyle(3002);

  THStack* digitalbadstave = new THStack("DigitalBadPixel", "DigitalBadPixel");
    
  digitalbadstave->Add(deaddigital);
  digitalbadstave->Add(greaterdigital);
  digitalbadstave->Add(lessdigital);
 
  digitalbadstave->Draw();
  //digitalbadstave->GetYaxis()->SetRangeUser(0.1,27000);
   
  for(int i=0;i<32;i++)
    digitalbadstave->GetXaxis()->SetBinLabel(i+1,label[i]);
  digitalbadstave->GetXaxis()->LabelsOption("v");
  digitalbadstave->Draw("bartext0");
  THStack* digitalbadstave2 = new THStack("DigitalBadPixel_2", "DigitalBadPixel_2");

  digitalbadstave2->Add(deaddigital2);
  digitalbadstave2->Add(greaterdigital2);
  digitalbadstave2->Add(lessdigital2);
  digitalbadstave2->Draw("bartext0same");

  THStack* digitalbadstave3 = new THStack("DigitalBadPixel_3", "DigitalBadPixel_3");

  digitalbadstave3->Add(deaddigital3);
  digitalbadstave3->Add(greaterdigital3);
  digitalbadstave3->Add(lessdigital3);
  digitalbadstave3->Draw("bartext0same");

  TLegend* leg = new TLegend(0.1,0.6,0.25,0.9);
  leg->SetHeader("Bad Digital Pixel");
  
  leg->AddEntry(deaddigital,"dead digital pixel RT","F");
  leg->AddEntry(greaterdigital,"digi > 50 RT","F");
  leg->AddEntry(lessdigital,"digi < 50 RT","F");
  leg->AddEntry(deaddigital2,"dead digital pixel BTC","F");
  leg->AddEntry(greaterdigital2,"digi > 50 BTC","F");
  leg->AddEntry(lessdigital2,"digi < 50 BTC","F");
  leg->AddEntry(deaddigital3,"dead digital pixel ATC","F");
  leg->AddEntry(greaterdigital3,"digi > 50 ATC","F");
  leg->AddEntry(lessdigital3,"digi < 50 ATC","F");
  leg->SetFillColor(kYellow-10);
  leg->Draw();
}

void Fit_noisedistro(TH1* histo){
  TF1* gaussian = new TF1("gaus1","[0]*exp(-0.5*((x-[1])/[2])**2)",0,1000);
  
  histo->Rebin(5);

  double integral=0, mean=0, entries=0;  
  for(int i=2; i<histo->GetNbinsX();i++){
    integral+=(histo->GetBinContent(i))*(histo->GetBinCenter(i));
    entries+=histo->GetBinContent(i);
    // std::cout<<"Integral: "<<integral<<"\tBinCenter: "<<histo->GetBinCenter(i)<<std::endl;
  }

  mean= integral/entries;
  gaussian->SetParameter(1,mean);
  gaussian->SetParameter(2,20);
  histo->Fit("gaus1","Q");

  delete gaussian;

}

void BadDistribution(std::vector <HistoReader*> stavevector,std::vector <HistoReader*> stavevector_noHV){
  std::vector <int> badpixelarray;

  THStack* badstave = new THStack("NumberBadPixelsOnStave", "NumberBadPixelsOnStave");
  TH1I* digi_dead = new TH1I("Dead", "Dead",4,0,4);
  TH1I* ana_dead = new TH1I("Dead", "Dead",4,0,4);
  TH1I* thre_dead = new TH1I("Dead", "Dead",4,0,4);
  TH1I* digi_greater = new TH1I("Greater", "Greater",4,0,4);
  TH1I* ana_greater = new TH1I("Greater", "Greater",4,0,4);
  TH1I* digi_less = new TH1I("Less", "Less",4,0,4);
  TH1I* ana_less = new TH1I("Less", "Less",4,0,4);
  TH1I* disconnected = new TH1I("Disconnected","Disconnected",4,0,4);
  TH1I* noisy = new TH1I("Noisy","Noisy",4,0,4);
  TH1I* merged = new TH1I("Xtalk","Xtalk",4,0,4);

  //  TString label[6]={"Digital","Analog","Threshold","High Noise","X-talk","Disconnected"};
  TString label[4]={"Digital","Analog","Threshold","High Noise"};
    
    
  for(unsigned i=0;i<stavevector.size();i++){

    badpixelarray = BadPixelMonsterHierarchy(stavevector[i],stavevector_noHV[i]);
    digi_dead->AddBinContent(1,badpixelarray[9]);
    digi_greater->AddBinContent(1,badpixelarray[7]);
    digi_less->AddBinContent(1,badpixelarray[8]);
    
    ana_dead->AddBinContent(2,badpixelarray[6]);
    ana_greater->AddBinContent(2,badpixelarray[4]);
    ana_less->AddBinContent(2,badpixelarray[5]);

    thre_dead->AddBinContent(3,badpixelarray[3]);
    // disconnected->AddBinContent(6,badpixelarray[2]);
    noisy->AddBinContent(4,badpixelarray[1]);

    merged->AddBinContent(5,badpixelarray[0]);
  }
  digi_dead->SetFillColor(kAzure);
  digi_greater->SetFillColor(kAzure+1);
  digi_less->SetFillColor(kAzure+2);

  ana_dead->SetFillColor(kViolet);
  ana_greater->SetFillColor(kViolet+2);
  ana_less->SetFillColor(kViolet+4);

  thre_dead->SetFillColor(kGreen);

  //  disconnected->SetFillColor(kViolet);
  noisy->SetFillColor(kGreen+2);
  merged->SetFillColor(kRed);


  badstave->Add(digi_dead);
  badstave->Add(digi_greater);
  badstave->Add(digi_less);

  badstave->Add(ana_dead);
  badstave->Add(ana_greater);
  badstave->Add(ana_less);

  badstave->Add(thre_dead);

  // badstave->Add(disconnected);
  badstave->Add(noisy);
  // badstave->Add(merged);

  digi_dead->SetMarkerSize(1.3);
  digi_greater->SetMarkerSize(1.3);
  digi_less->SetMarkerSize(1.3);

  ana_dead->SetMarkerSize(1.3);
  ana_greater->SetMarkerSize(1.3);
  ana_less->SetMarkerSize(1.3);

 
  disconnected->SetMarkerSize(1.3);
  noisy->SetMarkerSize(1.3);
  merged->SetMarkerSize(1.3);

  badstave->Draw();
  for(int i=0;i<4;i++)
    badstave->GetXaxis()->SetBinLabel(i+1,label[i]);
  badstave->GetXaxis()->LabelsOption("h");

  badstave->Draw("bartext0");


  TLegend* leg = new TLegend(0.1,0.6,0.25,0.9);
  leg->SetHeader("Bad Pixel");
  // leg->AddEntry(merged,"x-talk","F");
  leg->AddEntry(thre_dead,"thre dead pixel","F");
  leg->AddEntry(noisy,"noisy","F");
  // leg->AddEntry(disconnected,"disconnectedbump","F");
  leg->AddEntry(digi_dead,"digi dead pixel","F");
  leg->AddEntry(digi_greater,"digi > 50","F");
  leg->AddEntry(digi_less,"digi < 50","F");
  leg->AddEntry(ana_dead,"ana dead pixel","F");
  leg->AddEntry(ana_greater,"ana > 50","F");
  leg->AddEntry(ana_less,"ana < 50","F");


  leg->SetFillColor(kYellow-10);
  leg->Draw();

}
void BadDistribution_disco(std::vector <HistoReader*> stavevector,std::vector <HistoReader*> stavevector_noHV){
  std::vector <int> badpixelarray;
  
  THStack* badstave = new THStack("NumberBadPixelsOnStave", "NumberBadPixelsOnStave");
  TH1I* dead = new TH1I("Dead", "Dead",6,0,6);
  TH1I* greater = new TH1I("Greater", "Greater",6,0,6);
  TH1I* less = new TH1I("Less", "Less",6,0,6);
  TH1I* disconnected = new TH1I("Disconnected","Disconnected",6,0,6);
  TH1I* noisy = new TH1I("Noisy","Noisy",6,0,6);
  TH1I* merged = new TH1I("Xtalk","Xtalk",6,0,6);

  TString label[6]={"Digital","Analog","Threshold","High Noise","X-talk","Disconnected"};
  
  
  
  for(unsigned i=0;i<stavevector.size();i++){

    badpixelarray = BadPixelMonsterHierarchy(stavevector[i],stavevector_noHV[i]);
    dead->AddBinContent(1,badpixelarray[9]);
    greater->AddBinContent(1,badpixelarray[7]);
    less->AddBinContent(1,badpixelarray[8]);
    
    dead->AddBinContent(2,badpixelarray[6]);
    greater->AddBinContent(2,badpixelarray[4]);
    less->AddBinContent(2,badpixelarray[5]);

    dead->AddBinContent(3,badpixelarray[3]);
    disconnected->AddBinContent(6,badpixelarray[2]);
    noisy->AddBinContent(4,badpixelarray[1]);

    merged->AddBinContent(5,badpixelarray[0]);
  }
  dead->SetFillColor(kOrange+10);
  greater->SetFillColor(kOrange+10);
  less->SetFillColor(kOrange+10);

  disconnected->SetFillColor(kViolet);
  noisy->SetFillColor(kGreen);
  merged->SetFillColor(kYellow);

  greater->SetFillStyle(3001);
  less->SetFillStyle(3002);

  badstave->Add(dead);
  badstave->Add(greater);
  badstave->Add(less);
  badstave->Add(disconnected);
  badstave->Add(noisy);
  badstave->Add(merged);

  dead->SetMarkerSize(1.3);
  greater->SetMarkerSize(1.3);
  less->SetMarkerSize(1.3);
  disconnected->SetMarkerSize(1.3);
  noisy->SetMarkerSize(1.3);
  merged->SetMarkerSize(1.3);

  badstave->Draw();
  for(int i=0;i<6;i++)
    badstave->GetXaxis()->SetBinLabel(i+1,label[i]);
  badstave->GetXaxis()->LabelsOption("h");

  badstave->Draw("bartext0");


  TLegend* leg = new TLegend(0.1,0.6,0.25,0.9);
  leg->SetHeader("Bad Pixel");
  leg->AddEntry(merged,"x-talk","F");
  leg->AddEntry(noisy,"noisy","F");
  leg->AddEntry(disconnected,"disconnectedbump","F");
  leg->AddEntry(dead,"dead pixel","F");
  leg->AddEntry(greater,"digi/ana > 50","F");
  leg->AddEntry(less,"digi/ana < 50","F");
  leg->SetFillColor(kYellow-10);
  leg->Draw();

}
void BadDistribution_Module(HistoReader* aReader,HistoReader* aReader_noHV){
  std::vector <int> badpixelarray;

  THStack* badmodule = new THStack("NumberOfBadPixelsOnModule", "NumberOfBadPixelsOnModule");
  TH1I* dead = new TH1I("Dead", "Dead",6,0,6);
  TH1I* greater = new TH1I("Greater", "Greater",6,0,6);
  TH1I* less = new TH1I("Less", "Less",6,0,6);
  TH1I* disconnected = new TH1I("Disconnected","Disconnected",6,0,6);
  TH1I* noisy = new TH1I("Noisy","Noisy",6,0,6);
  TH1I* merged = new TH1I("X-talk","X-talk",6,0,6);

  TString label[6]={"Digital","Analog","Threshold","High Noise","X-talk","Disconnected"};
  
    
    
 

  badpixelarray = BadPixelMonsterHierarchy(aReader,aReader_noHV);
  dead->AddBinContent(1,badpixelarray[9]);
  greater->AddBinContent(1,badpixelarray[7]);
  less->AddBinContent(1,badpixelarray[8]);
    
  dead->AddBinContent(2,badpixelarray[6]);
  greater->AddBinContent(2,badpixelarray[4]);
  less->AddBinContent(2,badpixelarray[5]);

  dead->AddBinContent(3,badpixelarray[3]);
  disconnected->AddBinContent(6,badpixelarray[2]);
  noisy->AddBinContent(4,badpixelarray[1]);

  merged->AddBinContent(5,badpixelarray[0]);
 
  dead->SetFillColor(kOrange+10);
  greater->SetFillColor(kOrange+10);
  less->SetFillColor(kOrange+10);

  disconnected->SetFillColor(kViolet);
  noisy->SetFillColor(kGreen);
  merged->SetFillColor(kYellow);

  greater->SetFillStyle(3001);
  less->SetFillStyle(3002);

  badmodule->Add(dead);
  badmodule->Add(greater);
  badmodule->Add(less);
  badmodule->Add(disconnected);
  badmodule->Add(noisy);
  //  badmodule->Add(merged);

  dead->SetMarkerSize(1.3);
  greater->SetMarkerSize(1.3);
  less->SetMarkerSize(1.3);
  disconnected->SetMarkerSize(1.3);
  noisy->SetMarkerSize(1.3);
  merged->SetMarkerSize(1.3);

  badmodule->Draw();
  for(int i=0;i<6;i++)
    badmodule->GetXaxis()->SetBinLabel(i+1,label[i]);
  badmodule->GetXaxis()->LabelsOption("h");

  badmodule->Draw("bartext0");


  TLegend* leg = new TLegend(0.1,0.6,0.25,0.9);
  leg->SetHeader("Bad Pixel");
  //  leg->AddEntry(merged,"x-talk","F");
  leg->AddEntry(noisy,"noisy","F");
  leg->AddEntry(disconnected,"disconnectedbump","F");
  leg->AddEntry(dead,"dead pixel","F");
  leg->AddEntry(greater,"digi/ana > 50","F");
  leg->AddEntry(less,"digi/ana < 50","F");
  leg->SetFillColor(kYellow-10);
  leg->Draw();

}





void AnalogBad_Stave(std::vector <HistoReader*> stavevector,std::vector <HistoReader*> stavevector_noHV){
  std::vector <int> badpixelarray;
    
    
  THStack* analogbadstave = new THStack("analogBadPixel", "analogBadPixel");
    
  TH1I* deadanalog = new TH1I("Deadanalog", "Deadanalog",32,0,32);
  TH1I* greateranalog = new TH1I("Greateranalog", "Greateranalog",32,0,32);
  TH1I* lessanalog = new TH1I("Lessanalog", "Lessanalog",32,0,32);
  TString label[32]={"A8_2","A8_1","A7_2","A7_1","A6_2","A6_1","A5_2","A5_1","A4_2","A4_1","A3_2","A3_1","A2_2","A2_1","A1_2","A1_1","C1_1","C1_2","C2_1","C2_2","C3_1","C3_2","C4_1","C4_2","C5_1","C5_2","C6_1","C6_2","C7_1","C7_2","C8_1","C8_2"};
  
    
    
  for(unsigned i=0;i<stavevector.size();i++){

    badpixelarray = BadPixelMonster2(stavevector[i],stavevector_noHV[i]);
    deadanalog->SetBinContent(i+1,badpixelarray[6]);
    greateranalog->SetBinContent(i+1,badpixelarray[4]);
    lessanalog->SetBinContent(i+1,badpixelarray[5]);
       
  }
  deadanalog->SetFillColor(kGreen+2);
  greateranalog->SetFillColor(kGreen+2);
  lessanalog->SetFillColor(kGreen+2);

  deadanalog->SetLineColor(kGreen+2);
  greateranalog->SetLineColor(0);
  lessanalog->SetLineColor(0);
  greateranalog->SetFillStyle(3001);
  lessanalog->SetFillStyle(3002);

  analogbadstave->Add(deadanalog);
  analogbadstave->Add(greateranalog);
  analogbadstave->Add(lessanalog);

  analogbadstave->Draw();
  for(int i=0;i<32;i++)
    analogbadstave->GetXaxis()->SetBinLabel(i+1,label[i]);
  analogbadstave->GetXaxis()->LabelsOption("v");
  analogbadstave->Draw("bartext0");

  TLegend* leg = new TLegend(0.1,0.6,0.25,0.9);
  leg->SetHeader("Bad Analog Pixel");
  
  leg->AddEntry(deadanalog,"dead analog pixel","F");
  leg->AddEntry(greateranalog,"ana > 50","F");
  leg->AddEntry(lessanalog,"ana < 50","F");
  leg->SetFillColor(kYellow-10);
  leg->Draw();
}

void AnalogBad_Stave2(std::vector <HistoReader*> stavevector,std::vector <HistoReader*> stavevector_noHV,std::vector <HistoReader*> stavevector2,std::vector <HistoReader*> stavevector2_noHV){
  std::vector <int> badpixelarray;
  std::vector <int> badpixelarray2;
    
  TH1I* deadanalog = new TH1I("Deadanalog", "Deadanalog",32,0,32);
  TH1I* greateranalog = new TH1I("Greateranalog", "Greateranalog",32,0,32);
  TH1I* lessanalog = new TH1I("Lessanalog", "Lessanalog",32,0,32);
  TH1I* deadanalog2 = new TH1I("Deadanalog", "Deadanalog",32,0,32);
  TH1I* greateranalog2 = new TH1I("Greateranalog", "Greateranalog",32,0,32);
  TH1I* lessanalog2 = new TH1I("Lessanalog", "Lessanalog",32,0,32);
        
  TString label[32]={"A8_2","A8_1","A7_2","A7_1","A6_2","A6_1","A5_2","A5_1","A4_2","A4_1","A3_2","A3_1","A2_2","A2_1","A1_2","A1_1","C1_1","C1_2","C2_1","C2_2","C3_1","C3_2","C4_1","C4_2","C5_1","C5_2","C6_1","C6_2","C7_1","C7_2","C8_1","C8_2"};
    
  for(unsigned i=0;i<stavevector.size();i++){

    badpixelarray = BadPixelMonster2(stavevector[i],stavevector_noHV[i]);
    badpixelarray2 = BadPixelMonster2(stavevector2[i],stavevector2_noHV[i]);
    deadanalog->SetBinContent(i+1,badpixelarray[6]);
    greateranalog->SetBinContent(i+1,badpixelarray[4]);
    lessanalog->SetBinContent(i+1,badpixelarray[5]);
    deadanalog2->SetBinContent(i+1,badpixelarray2[6]);
    greateranalog2->SetBinContent(i+1,badpixelarray2[4]);
    lessanalog2->SetBinContent(i+1,badpixelarray2[5]);
       

    //std::cout<<"DIGI_BAD_1: "<<BadPixelMonster2(stavevector[i],stavevector_noHV[i])<<"\tDIGI_BAD_2: "<<BadPixelMonster2(stavevector2[i],stavevector2_noHV[i])<<std::endl;
  }
  deadanalog->SetFillColor(kGreen+2);
  greateranalog->SetFillColor(kGreen+2);
  lessanalog->SetFillColor(kGreen+2);

  deadanalog->SetLineColor(kGreen+2);
  greateranalog->SetLineColor(0);
  lessanalog->SetLineColor(0);



  deadanalog->SetBarWidth(0.5);
  greateranalog->SetBarWidth(0.5);
  lessanalog->SetBarWidth(0.5);

  deadanalog2->SetBarWidth(0.5);
  greateranalog2->SetBarWidth(0.5);
  lessanalog2->SetBarWidth(0.5);

  deadanalog2->SetBarOffset(0.5);
  greateranalog2->SetBarOffset(0.5);
  lessanalog2->SetBarOffset(0.5);

  
  deadanalog2->SetFillColor(kRed);
  greateranalog2->SetFillColor(kRed);
  lessanalog2->SetFillColor(kRed);

  deadanalog2->SetLineColor(kRed);
  greateranalog2->SetLineColor(0);
  lessanalog2->SetLineColor(0);


  greateranalog->SetFillStyle(3001);
  lessanalog->SetFillStyle(3002);

  greateranalog2->SetFillStyle(3001);
  lessanalog2->SetFillStyle(3002);

  //   deadanalog->Draw("bar");
  //   deadanalog2->Draw("barsame");
  
  THStack* analogbadstave = new THStack("analogBadPixel", "analogBadPixel");
    
  analogbadstave->Add(deadanalog);
  analogbadstave->Add(greateranalog);
  analogbadstave->Add(lessanalog);
  analogbadstave->Draw("bartext0");


   
  for(int i=0;i<32;i++)
    analogbadstave->GetXaxis()->SetBinLabel(i+1,label[i]);
  analogbadstave->GetXaxis()->LabelsOption("v");
  analogbadstave->Draw("bar");
  THStack* analogbadstave2 = new THStack("analogBadPixel_2", "analogBadPixel_2");


  analogbadstave2->Add(deadanalog2);
  analogbadstave2->Add(greateranalog2);
  analogbadstave2->Add(lessanalog2);



  analogbadstave2->Draw("bartext0same");

  TLegend* leg = new TLegend(0.1,0.6,0.25,0.9);
  leg->SetHeader("Bad Analog Pixel");
  
  leg->AddEntry(deadanalog,"dead analog pixel RT","F");
  leg->AddEntry(greateranalog,"ana > 50 RT","F");
  leg->AddEntry(lessanalog,"ana < 50 RT","F");
  leg->AddEntry(deadanalog2,"dead analog pixel BTC","F");
  leg->AddEntry(greateranalog2,"ana > 50 BTC","F");
  leg->AddEntry(lessanalog2,"ana < 50 BTC","F");

  leg->SetFillColor(kYellow-10);
  leg->Draw();
}



void AnalogBad_Stave3(std::vector <HistoReader*> stavevector,std::vector <HistoReader*> stavevector_noHV,std::vector <HistoReader*> stavevector2,std::vector <HistoReader*> stavevector2_noHV,std::vector <HistoReader*> stavevector3,std::vector <HistoReader*> stavevector3_noHV){
  std::vector <int> badpixelarray;
  std::vector <int> badpixelarray2;
  std::vector <int> badpixelarray3;
    
  TH1I* deadanalog = new TH1I("Deadanalog", "Deadanalog",32,0,32);
  TH1I* greateranalog = new TH1I("Greateranalog", "Greateranalog",32,0,32);
  TH1I* lessanalog = new TH1I("Lessanalog", "Lessanalog",32,0,32);
  TH1I* deadanalog2 = new TH1I("Deadanalog", "Deadanalog",32,0,32);
  TH1I* greateranalog2 = new TH1I("Greateranalog", "Greateranalog",32,0,32);
  TH1I* lessanalog2 = new TH1I("Lessanalog", "Lessanalog",32,0,32);
  TH1I* deadanalog3 = new TH1I("Deadanalog", "Deadanalog",32,0,32);
  TH1I* greateranalog3 = new TH1I("Greateranalog", "Greateranalog",32,0,32);
  TH1I* lessanalog3 = new TH1I("Lessanalog", "Lessanalog",32,0,32);        


  TString label[32]={"A8_2","A8_1","A7_2","A7_1","A6_2","A6_1","A5_2","A5_1","A4_2","A4_1","A3_2","A3_1","A2_2","A2_1","A1_2","A1_1","C1_1","C1_2","C2_1","C2_2","C3_1","C3_2","C4_1","C4_2","C5_1","C5_2","C6_1","C6_2","C7_1","C7_2","C8_1","C8_2"};
  
    
    
  for(unsigned i=0;i<stavevector.size();i++){

    badpixelarray = BadPixelMonster2(stavevector[i],stavevector_noHV[i]);
    badpixelarray2 = BadPixelMonster2(stavevector2[i],stavevector2_noHV[i]);
    badpixelarray3 = BadPixelMonster2(stavevector3[i],stavevector3_noHV[i]);
    deadanalog->SetBinContent(i+1,badpixelarray[6]);
    greateranalog->SetBinContent(i+1,badpixelarray[4]);
    lessanalog->SetBinContent(i+1,badpixelarray[5]);
       
    deadanalog2->SetBinContent(i+1,badpixelarray2[6]);
    greateranalog2->SetBinContent(i+1,badpixelarray2[4]);
    lessanalog2->SetBinContent(i+1,badpixelarray2[5]);
       
    deadanalog3->SetBinContent(i+1,badpixelarray3[6]);
    greateranalog3->SetBinContent(i+1,badpixelarray3[4]);
    lessanalog3->SetBinContent(i+1,badpixelarray3[5]);


    //std::cout<<"DIGI_BAD_1: "<<BadPixelMonster2(stavevector[i],stavevector_noHV[i])<<"\tDIGI_BAD_2: "<<BadPixelMonster2(stavevector2[i],stavevector2_noHV[i])<<std::endl;
  }
  deadanalog->SetFillColor(kGreen+2);
  greateranalog->SetFillColor(kGreen+2);
  lessanalog->SetFillColor(kGreen+2);

  deadanalog->SetLineColor(kGreen+2);
  greateranalog->SetLineColor(0);
  lessanalog->SetLineColor(0);



  deadanalog->SetBarWidth(0.333);
  greateranalog->SetBarWidth(0.333);
  lessanalog->SetBarWidth(0.333);

  deadanalog2->SetBarWidth(0.333);
  greateranalog2->SetBarWidth(0.333);
  lessanalog2->SetBarWidth(0.333);

  deadanalog2->SetBarOffset(0.333);
  greateranalog2->SetBarOffset(0.333);
  lessanalog2->SetBarOffset(0.333);

  
  deadanalog2->SetFillColor(kRed);
  greateranalog2->SetFillColor(kRed);
  lessanalog2->SetFillColor(kRed);

  deadanalog2->SetLineColor(kRed);
  greateranalog2->SetLineColor(0);
  lessanalog2->SetLineColor(0);


  deadanalog3->SetBarWidth(0.333);
  greateranalog3->SetBarWidth(0.333);
  lessanalog3->SetBarWidth(0.333);

  deadanalog3->SetBarOffset(0.666);
  greateranalog3->SetBarOffset(0.666);
  lessanalog3->SetBarOffset(0.666);

  
  deadanalog3->SetFillColor(kViolet);
  greateranalog3->SetFillColor(kViolet);
  lessanalog3->SetFillColor(kViolet);

  deadanalog3->SetLineColor(kViolet);
  greateranalog3->SetLineColor(0);
  lessanalog3->SetLineColor(0);
  //    lessanalog3->SetLineWidth(1);


  greateranalog->SetFillStyle(3001);
  lessanalog->SetFillStyle(3002);

  greateranalog2->SetFillStyle(3001);
  lessanalog2->SetFillStyle(3002);

  greateranalog3->SetFillStyle(3001);
  lessanalog3->SetFillStyle(3002);

  //   deadanalog->Draw("bar");
  //   deadanalog2->Draw("barsame");
  
  THStack* analogbadstave = new THStack("analogBadPixel", "analogBadPixel");
    
  analogbadstave->Add(deadanalog);
  analogbadstave->Add(greateranalog);
  analogbadstave->Add(lessanalog);

  //gPad->SetLogy();
 
  analogbadstave->Draw("bartext0");
  //analogbadstave->GetYaxis()->SetRangeUser(0.1,27000);

   
  for(int i=0;i<32;i++)
    analogbadstave->GetXaxis()->SetBinLabel(i+1,label[i]);
  analogbadstave->GetXaxis()->LabelsOption("v");
  analogbadstave->Draw("bar");
  THStack* analogbadstave2 = new THStack("analogBadPixel_2", "analogBadPixel_2");


  analogbadstave2->Add(deadanalog2);
  analogbadstave2->Add(greateranalog2);
  analogbadstave2->Add(lessanalog2);



  analogbadstave2->Draw("bartext0same");

  THStack* analogbadstave3 = new THStack("analogBadPixel_3", "analogBadPixel_3");


  analogbadstave3->Add(deadanalog3);
  analogbadstave3->Add(greateranalog3);
  analogbadstave3->Add(lessanalog3);



  analogbadstave3->Draw("bartext0same");
  

  TLegend* leg = new TLegend(0.1,0.6,0.25,0.9);
  leg->SetHeader("Bad Analog Pixel");
  
  leg->AddEntry(deadanalog,"dead analog pixel RT","F");
  leg->AddEntry(greateranalog,"ana > 50 RT","F");
  leg->AddEntry(lessanalog,"ana < 50 RT","F");
  leg->AddEntry(deadanalog2,"dead analog pixel BTC","F");
  leg->AddEntry(greateranalog2,"ana > 50 BTC","F");
  leg->AddEntry(lessanalog2,"ana < 50 BTC","F");
  leg->AddEntry(deadanalog3,"dead analog pixel ATC","F");
  leg->AddEntry(greateranalog3,"ana > 50 ATC","F");
  leg->AddEntry(lessanalog3,"ana < 50 ATC","F");
  leg->SetFillColor(kYellow-10);
  leg->Draw();
}




void DisconnectedStave(std::vector <HistoReader*> stave_hv, std::vector <HistoReader*> stave_nohv){
  
  std::vector <int> bp_j;
  //  int var_histo_0, var_histo_great, var_histo_less, var_histo_disconnectedbump, var_histo_noisy;
  THStack *BP_sum = new THStack("bad_pixel_sum","bad_pixel_sum");
  TString label[32]={"A8_2","A8_1","A7_2","A7_1","A6_2","A6_1","A5_2","A5_1","A4_2","A4_1","A3_2","A3_1","A2_2","A2_1","A1_2","A1_1","C1_1","C1_2","C2_1","C2_2","C3_1","C3_2","C4_1","C4_2","C5_1","C5_2","C6_1","C6_2","C7_1","C7_2","C8_1","C8_2"};

  int n_bin = 32;


  TH1I *histo_disconnectedbump = new TH1I("histo_disconnectedbump","histo_disconnectedbump",n_bin,0,n_bin);  
  histo_disconnectedbump->SetFillColor(kRed);


  for(unsigned i=1;i<stave_hv.size()+1;i++){

    bp_j= BadPixelMonster2(stave_hv[i-1],stave_nohv[i-1]);
    histo_disconnectedbump->AddBinContent(i, bp_j.at(2));

  }  
  
  
  for(int i=0;i<32;i++) histo_disconnectedbump->GetXaxis()->SetBinLabel(i+1,label[i]);
  histo_disconnectedbump->GetXaxis()->LabelsOption("v");
  histo_disconnectedbump->Draw("bartext0");

}
 

void DisconnectedStave2(std::vector <HistoReader*> stave_hv, std::vector <HistoReader*> stave_nohv,std::vector <HistoReader*> stave_hv2, std::vector <HistoReader*> stave_nohv2){
  

  //  int var_histo_0, var_histo_great, var_histo_less, var_histo_disconnectedbump, var_histo_noisy;
  THStack *DB_sum = new THStack("bad_pixel_sum","bad_pixel_sum");
  TString label[32]={"A8_2","A8_1","A7_2","A7_1","A6_2","A6_1","A5_2","A5_1","A4_2","A4_1","A3_2","A3_1","A2_2","A2_1","A1_2","A1_1","C1_1","C1_2","C2_1","C2_2","C3_1","C3_2","C4_1","C4_2","C5_1","C5_2","C6_1","C6_2","C7_1","C7_2","C8_1","C8_2"};

  int n_bin = 32;

  
  TH1I *histo_disconnectedbump = new TH1I("histo_disconnectedbump","histo_disconnectedbump",n_bin,0,n_bin); 
  TH1I *histo_disconnectedbump2 = new TH1I("histo_disconnectedbump","histo_disconnectedbump",n_bin,0,n_bin); 
  histo_disconnectedbump->SetFillColor(kRed);
  histo_disconnectedbump2->SetFillColor(kBlue);

  for(unsigned i=1;i<stave_hv.size()+1;i++){

 
    histo_disconnectedbump->AddBinContent(i, BadPixelMonster2(stave_hv[i-1],stave_nohv[i-1]).at(2));
 
    histo_disconnectedbump2->AddBinContent(i, BadPixelMonster2(stave_hv2[i-1],stave_nohv2[i-1]).at(2));
  }  
  
  histo_disconnectedbump->SetBarWidth(0.5);
  histo_disconnectedbump2->SetBarWidth(0.5);

  histo_disconnectedbump2->SetBarOffset(0.5);

  DB_sum->Add(histo_disconnectedbump,"bar");
  DB_sum->Add(histo_disconnectedbump2,"bar");
  DB_sum->Draw("barnostack");  
  for(int i=0;i<32;i++) DB_sum->GetXaxis()->SetBinLabel(i+1,label[i]);
  DB_sum->GetXaxis()->LabelsOption("v");
  DB_sum->Draw("barnostack");
}

void AnalCorrelationPlotModules(HistoReader* before, HistoReader* after){
  //FicoPlot();
  int nbin = 26880;
  TH2D* correlationPlot = new TH2D("correlation Plot", "correlation Plot",20,0,60,20,0,60);
  //TF1* normalCorr = new TF1("f1","x",0,100);
  //  TH2D* normalCorrelation = new TH2D("normal correlation","normal correlation",100,0,100,100,0,100);
  TH2C* digibefore = before->Anal_Occupancy_Point_000;
  TH2C* digiafter = after->Anal_Occupancy_Point_000;
  int bin, binusbpix, binusbpix2, bincontent;

  for (int i=1;i<=80;i++){
    for (int j=1; j<=336;j++){

      if(before->Stat_processPosition!=PP_UsbPixReceptionTest && after->Stat_processPosition!=PP_UsbPixReceptionTest)bin = correlationPlot->GetBin(0.33333333*digibefore->GetBinContent(i,j)+1,0.333333*digiafter->GetBinContent(i,j)+1);
      else if(before->Stat_processPosition==PP_UsbPixReceptionTest && after->Stat_processPosition!=PP_UsbPixReceptionTest){
	binusbpix = (int)(digibefore->GetBinContent(i,j)/4);
	bin = correlationPlot->GetBin(0.333333*binusbpix+1,0.333333*(digiafter->GetBinContent(i,j))+1);
      }else if(before->Stat_processPosition!=PP_UsbPixReceptionTest && after->Stat_processPosition==PP_UsbPixReceptionTest){
	binusbpix = (int)(digiafter->GetBinContent(i,j)/4);
	bin = correlationPlot->GetBin(0.333333*(digibefore->GetBinContent(i,j))+1,0.333333*binusbpix+1);
      }else if(before->Stat_processPosition==PP_UsbPixReceptionTest && after->Stat_processPosition==PP_UsbPixReceptionTest){
	binusbpix = (int)(digibefore->GetBinContent(i,j)/4);
	//if(digiafter->GetBinContent(i,j)!=0)std::cout<<"digiafter: "<<after->Anal_Occupancy_Point_000->GetBinContent(i,j)<<std::endl;
	binusbpix2 = (int)(digiafter->GetBinContent(i,j)/4);
	//std::cout<<"binusbpix: "<<binusbpix<<"\tbinusbpix2: "<<binusbpix2<<std::endl;
	bin = correlationPlot->GetBin(0.333333*binusbpix+1, 0.333333*binusbpix2+1);

      } 
      //std::cout<<"BIN: "<<bin<<"\tx="<<digibefore->GetBinContent(i,j)<<"\ty="<<digiafter->GetBinContent(i,j)<<std::endl;
      bincontent = correlationPlot->GetBinContent(bin);
      correlationPlot->SetBinContent(bin,bincontent+1);

    }
  }

  /* for(int k=0;k<100;k++){
     bin = normalCorrelation->GetBin(k+1,k+1);
     //    normalCorrelation->SetBinContent(bin,1);

     }*/
  // std::cout<<"Correlation Plot"<<std::endl;
  /*
    PP_RCEReceptionTest, 
    PP_UsbPixReceptionTest, 
    PP_StaveTestBeforeTC_AT,
    PP_StaveTestAfterTC_AT,
    PP_StaveTestBeforeTC_BT,
    PP_StaveTestAfterTC_BT,
    PP_Unknown
  */
  std::vector<std::string> PP; 
  PP.push_back("_RCE_RT");
  PP.push_back("_USBpix_RT");
  PP.push_back("_BTC_BT");
  PP.push_back("_BTC_AT");
  PP.push_back("_BTC_BT");
  PP.push_back("_ATC_BT");
  PP.push_back("_ATC_AT");

  

  if(before->Stat_processPosition == PP_RCEReceptionTest  && after->Stat_processPosition == PP_RCEReceptionTest){
    correlationPlot->GetXaxis()->SetTitle((before->Stat_Module_ID + PP[0]).c_str());
    correlationPlot->GetYaxis()->SetTitle((after->Stat_Module_ID + PP[0]).c_str());
  }else if(before->Stat_processPosition == PP_UsbPixReceptionTest  && after->Stat_processPosition == PP_UsbPixReceptionTest){
    correlationPlot->GetXaxis()->SetTitle((before->Stat_Module_ID + PP[1]).c_str());
    correlationPlot->GetYaxis()->SetTitle((after->Stat_Module_ID + PP[1]).c_str());
  }else if(before->Stat_processPosition == PP_RCEReceptionTest  && after->Stat_processPosition == PP_UsbPixReceptionTest){
    correlationPlot->GetXaxis()->SetTitle((before->Stat_Module_ID + PP[0]).c_str());
    correlationPlot->GetYaxis()->SetTitle((after->Stat_Module_ID + PP[1]).c_str());
  }else if(before->Stat_processPosition == PP_UsbPixReceptionTest  && after->Stat_processPosition == PP_RCEReceptionTest){
    correlationPlot->GetXaxis()->SetTitle((before->Stat_Module_ID + PP[1]).c_str());
    correlationPlot->GetYaxis()->SetTitle((after->Stat_Module_ID + PP[0]).c_str());
  }else if(before->Stat_processPosition == PP_UsbPixReceptionTest && after->Stat_processPosition == PP_StaveTestBeforeTC_AT){
    correlationPlot->GetXaxis()->SetTitle((before->Stat_Module_ID + PP[1]).c_str());
    correlationPlot->GetYaxis()->SetTitle((after->Stat_Module_ID + PP[3]).c_str());
  }else if(before->Stat_processPosition == PP_StaveTestBeforeTC_AT && after->Stat_processPosition == PP_StaveTestAfterTC_AT ){
    correlationPlot->GetXaxis()->SetTitle((before->Stat_Module_ID + PP[3]).c_str());
    correlationPlot->GetYaxis()->SetTitle((after->Stat_Module_ID + PP[5]).c_str());
  }  
  correlationPlot->SetStats(0);
  correlationPlot->SetFillColor(kRed);
  correlationPlot->SetLineColor(kBlue+2);
  //  normalCorr->SetLineColor(kBlue);
  //  normalCorr->SetLineWidth(1);
  //  normalCorr->SetNpx(10000000);
  //  correlationPlot->Fill(100,100,-1);
  correlationPlot->SetMarkerSize(1.5);
  correlationPlot->Draw("TEXT45");  
  //  normalCorr->Draw("same");
  //  correlationPlot->Draw("boxsame");  
  gPad->SetLogz();
}




void DigiCorrelationPlotModules(HistoReader* before, HistoReader* after){
  //FicoPlot();
  int nbin = 26880;
  TH2D* correlationPlot = new TH2D("correlation Plot", "correlation Plot",20,0,60,20,0,60);
  //TF1* normalCorr = new TF1("f1","x",0,100);
  //  TH2D* normalCorrelation = new TH2D("normal correlation","normal correlation",100,0,100,100,0,100);
  TH2C* digibefore = before->Digi_Occupancy_Point_000;
  TH2C* digiafter = after->Digi_Occupancy_Point_000;
  int bin, binusbpix, binusbpix2, bincontent;

  for (int i=1;i<=80;i++){
    for (int j=1; j<=336;j++){

      if(before->Stat_processPosition!=PP_UsbPixReceptionTest && after->Stat_processPosition!=PP_UsbPixReceptionTest)bin = correlationPlot->GetBin(0.33333333*digibefore->GetBinContent(i,j)+1,0.333333*digiafter->GetBinContent(i,j)+1);
      else if(before->Stat_processPosition==PP_UsbPixReceptionTest && after->Stat_processPosition!=PP_UsbPixReceptionTest){
	binusbpix = (int)(digibefore->GetBinContent(i,j)/4);
	bin = correlationPlot->GetBin(0.333333*binusbpix+1,0.333333*(digiafter->GetBinContent(i,j))+1);
      }else if(before->Stat_processPosition!=PP_UsbPixReceptionTest && after->Stat_processPosition==PP_UsbPixReceptionTest){
	binusbpix = (int)(digiafter->GetBinContent(i,j)/4);
	bin = correlationPlot->GetBin(0.333333*(digibefore->GetBinContent(i,j))+1,0.333333*binusbpix+1);
      }else if(before->Stat_processPosition==PP_UsbPixReceptionTest && after->Stat_processPosition==PP_UsbPixReceptionTest){
	binusbpix = (int)(digibefore->GetBinContent(i,j)/4);
	//if(digiafter->GetBinContent(i,j)!=0)std::cout<<"digiafter: "<<after->Anal_Occupancy_Point_000->GetBinContent(i,j)<<std::endl;
	binusbpix2 = (int)(digiafter->GetBinContent(i,j)/4);
	//std::cout<<"binusbpix: "<<binusbpix<<"\tbinusbpix2: "<<binusbpix2<<std::endl;
	bin = correlationPlot->GetBin(0.333333*binusbpix+1, 0.333333*binusbpix2+1);

      } 
      //std::cout<<"BIN: "<<bin<<"\tx="<<digibefore->GetBinContent(i,j)<<"\ty="<<digiafter->GetBinContent(i,j)<<std::endl;
      bincontent = correlationPlot->GetBinContent(bin);
      correlationPlot->SetBinContent(bin,bincontent+1);

    }
  }

  /* for(int k=0;k<100;k++){
     bin = normalCorrelation->GetBin(k+1,k+1);
     //    normalCorrelation->SetBinContent(bin,1);

     }*/
  // std::cout<<"Correlation Plot"<<std::endl;
  /*
    PP_RCEReceptionTest, 
    PP_UsbPixReceptionTest, 
    PP_StaveTestBeforeTC_AT,
    PP_StaveTestAfterTC_AT,
    PP_StaveTestBeforeTC_BT,
    PP_StaveTestAfterTC_BT,
    PP_Unknown
  */
  std::vector<std::string> PP; 
  PP.push_back("_RCE_RT");
  PP.push_back("_USBpix_RT");
  PP.push_back("_BTC_BT");
  PP.push_back("_BTC_AT");
  PP.push_back("_BTC_BT");
  PP.push_back("_ATC_BT");
  PP.push_back("_ATC_AT");

  

  if(before->Stat_processPosition == PP_RCEReceptionTest  && after->Stat_processPosition == PP_RCEReceptionTest){
    correlationPlot->GetXaxis()->SetTitle((before->Stat_Module_ID + PP[0]).c_str());
    correlationPlot->GetYaxis()->SetTitle((after->Stat_Module_ID + PP[0]).c_str());
  }else if(before->Stat_processPosition == PP_UsbPixReceptionTest  && after->Stat_processPosition == PP_UsbPixReceptionTest){
    correlationPlot->GetXaxis()->SetTitle((before->Stat_Module_ID + PP[1]).c_str());
    correlationPlot->GetYaxis()->SetTitle((after->Stat_Module_ID + PP[1]).c_str());
  }else if(before->Stat_processPosition == PP_RCEReceptionTest  && after->Stat_processPosition == PP_UsbPixReceptionTest){
    correlationPlot->GetXaxis()->SetTitle((before->Stat_Module_ID + PP[0]).c_str());
    correlationPlot->GetYaxis()->SetTitle((after->Stat_Module_ID + PP[1]).c_str());
  }else if(before->Stat_processPosition == PP_UsbPixReceptionTest  && after->Stat_processPosition == PP_RCEReceptionTest){
    correlationPlot->GetXaxis()->SetTitle((before->Stat_Module_ID + PP[1]).c_str());
    correlationPlot->GetYaxis()->SetTitle((after->Stat_Module_ID + PP[0]).c_str());
  }else if(before->Stat_processPosition == PP_UsbPixReceptionTest && after->Stat_processPosition == PP_StaveTestBeforeTC_AT){
    correlationPlot->GetXaxis()->SetTitle((before->Stat_Module_ID + PP[1]).c_str());
    correlationPlot->GetYaxis()->SetTitle((after->Stat_Module_ID + PP[3]).c_str());
  }else if(before->Stat_processPosition == PP_StaveTestBeforeTC_AT && after->Stat_processPosition == PP_StaveTestAfterTC_AT ){
    correlationPlot->GetXaxis()->SetTitle((before->Stat_Module_ID + PP[3]).c_str());
    correlationPlot->GetYaxis()->SetTitle((after->Stat_Module_ID + PP[5]).c_str());
  }  
  correlationPlot->SetStats(0);
  correlationPlot->SetFillColor(kRed);
  correlationPlot->SetLineColor(kBlue+2);
  //  normalCorr->SetLineColor(kBlue);
  //  normalCorr->SetLineWidth(1);
  //  normalCorr->SetNpx(10000000);
  //  correlationPlot->Fill(100,100,-1);
  correlationPlot->SetMarkerSize(1.5);
  correlationPlot->Draw("TEXT45");  
  //  normalCorr->Draw("same");
  //  correlationPlot->Draw("boxsame");  
  gPad->SetLogz();
}






/*
  void DigiCorrelationPlotModules(HistoReader* before, HistoReader* after){
  //FicoPlot();
  int nbin = 26880;
  TH2D* correlationPlot = new TH2D("correlation Plot", "correlation Plot",20,0,60,20,0,60);
  TF1* normalCorr = new TF1("f1","x",0,100);
  //  TH2D* normalCorrelation = new TH2D("normal correlation","normal correlation",100,0,100,100,0,100);
  TH2C* digibefore = before->Digi_Occupancy_Point_000;
  TH2C* digiafter = after->Digi_Occupancy_Point_000;
  int bin, binusbpix, bincontent;

  for (int i=1;i<=80;i++){
  for (int j=1; j<=336;j++){

  if(before->Stat_processPosition!=PP_UsbPixReceptionTest)bin = correlationPlot->GetBin(0.333333*digibefore->GetBinContent(i,j)+1,0.333333*digiafter->GetBinContent(i,j)+1);
  else{
  binusbpix = (int)(digibefore->GetBinContent(i,j)/4);
  bin = correlationPlot->GetBin(0.333333*binusbpix+1,0.333333*digiafter->GetBinContent(i,j)+1);
  } 
  //std::cout<<"BIN: "<<bin<<"\tx="<<digibefore->GetBinContent(i,j)<<"\ty="<<digiafter->GetBinContent(i,j)<<std::endl;
  bincontent = correlationPlot->GetBinContent(bin);
  correlationPlot->SetBinContent(bin,bincontent+1);

  }
  }

  // for(int k=0;k<100;k++){
  //   bin = normalCorrelation->GetBin(k+1,k+1);
  //    normalCorrelation->SetBinContent(bin,1);

  //}
  // std::cout<<"Correlation Plot"<<std::endl;
  if(before->Stat_processPosition!=PP_UsbPixReceptionTest){
  correlationPlot->GetXaxis()->SetTitle("BTC");
  correlationPlot->GetYaxis()->SetTitle("ATC");
  }else{
  correlationPlot->GetXaxis()->SetTitle("RT");
  correlationPlot->GetYaxis()->SetTitle("BTC");
  }
  correlationPlot->SetStats(0);
  correlationPlot->SetFillColor(kRed);
  correlationPlot->SetLineColor(kBlue+2);
  //  normalCorr->SetLineColor(kBlue);
  //  normalCorr->SetLineWidth(1);
  //  normalCorr->SetNpx(10000000);
  correlationPlot->SetMarkerSize(1.5);
  correlationPlot->Draw("TEXT45");  
  //  normalCorr->Draw("same");
  //  correlationPlot->Draw("boxsame");  
  gPad->SetLogz();
  }

*/

void NoiseGreaterThanGaussian(HistoReader* aReader){

  double mean, sigma;
  TH2I* Thre_NoiseOut = new TH2I("NoiseGreaterThanGaussian","Noise greater than Gaussian",80,0,80,336,0,336);
  int k,l;
	
  Fit_noisedistro(aReader->Thre_sigmadist);
  mean = aReader->Thre_sigmadist->GetFunction("gaus1")->GetParameter(1);
  sigma = aReader->Thre_sigmadist->GetFunction("gaus1")->GetParameter(2);
	
  int nbinx = (int)aReader->Thre_Sigma->GetNbinsX();
  int nbiny = (int)aReader->Thre_Sigma->GetNbinsY();
  for(int i=1; i<=nbinx;i++){
    for(int j=1; j<=nbiny;j++){
      if(aReader->Thre_Sigma->GetBinContent(i,j)>(mean+5*fabs(sigma)) && aReader->Thre_Sigma->GetBinContent(i,j)!=0){
	Thre_NoiseOut->SetBinContent(i,j,1);
      }
    }
  }
  Thre_NoiseOut->Draw("colz");

}


TGMainFrame* AvailableModulesSummaryTable(ProcessPosition PP, std::vector<std::string> modules){
  
  HistoReader *module_reader_HV_1, *module_reader_noHV_1,*module_reader_HV_2, *module_reader_noHV_2;  
  

  int disconnectedbumps, bad_digi_ana, high_noise, xtalk;
  std::string break_down;
  std::vector<int> badpixelmonsterhierarchy;
  std::string FE_ID; 


  // FILLING THE VECTORS FOR THE TABLE
  //   TNtuple *ntuple = new TNtuple("ntuple", "DemoNtuple", "Bad_digi_ana:Disconnected:High_noise:Merged:Breakdown");
  TTree *tl = new TTree("tl","a simple TTree with simples variables");
  tl->Branch("FE_ID", &FE_ID);
  tl->Branch("bad_digi_ana",&bad_digi_ana);
  tl->Branch("disconnectedbumps",&disconnectedbumps);
  tl->Branch("high_noise",&high_noise);
  tl->Branch("xtalk",&xtalk);
  tl->Branch("break_down",&break_down);

  int counter = 0;
  for(std::vector<std::string>::iterator iii = modules.begin(); iii != modules.end(); iii++){
  
    if (DbTools::GetInstance()->IsDoubleChip(iii->c_str())==1) {
      module_reader_HV_1 = new HistoReader(iii->c_str(), PP, 0,1);
      module_reader_noHV_1 = new HistoReader(iii->c_str(), PP, 0,0);

      badpixelmonsterhierarchy = BadPixelMonsterHierarchy(module_reader_HV_1, module_reader_noHV_1);
      bad_digi_ana=badpixelmonsterhierarchy[9]+badpixelmonsterhierarchy[8]+badpixelmonsterhierarchy[7]+badpixelmonsterhierarchy[6]+badpixelmonsterhierarchy[5]+badpixelmonsterhierarchy[4];
      disconnectedbumps=badpixelmonsterhierarchy[2];
      xtalk=badpixelmonsterhierarchy[0];
      high_noise=badpixelmonsterhierarchy[1];
      break_down = IV_Mod(module_reader_HV_1, module_reader_noHV_1);
      FE_ID=(*iii+"_0");
      std::cout<<"BREAKDOWN: "<<break_down<<std::endl;

    
      counter++;
      tl->Fill();
      //    ntuple->Fill(bad_digi_ana[counter], disconnectedbumps[counter],merged[counter],high_noise[counter],counter);

      module_reader_HV_2 = new HistoReader(iii->c_str(), PP, 1,1);
      module_reader_noHV_2 = new HistoReader(iii->c_str(), PP, 1,0);

      badpixelmonsterhierarchy = BadPixelMonsterHierarchy(module_reader_HV_2, module_reader_noHV_2); 

      bad_digi_ana=badpixelmonsterhierarchy[9]+badpixelmonsterhierarchy[8]+badpixelmonsterhierarchy[7]+badpixelmonsterhierarchy[6]+badpixelmonsterhierarchy[5]+badpixelmonsterhierarchy[4];
      disconnectedbumps=badpixelmonsterhierarchy[2];
      xtalk=badpixelmonsterhierarchy[0];
      high_noise=badpixelmonsterhierarchy[1];
      break_down = IV_Mod(module_reader_HV_2, module_reader_noHV_2);
      FE_ID = (*iii+"_1");
      std::cout<<"BREAKDOWN: "<<break_down<<std::endl;

    
      counter++;
      tl->Fill();

    
    }else{
      module_reader_HV_1 = new HistoReader(iii->c_str(), PP, 0,1);
      module_reader_noHV_1 = new HistoReader(iii->c_str(), PP, 0,0);

      badpixelmonsterhierarchy = BadPixelMonsterHierarchy(module_reader_HV_1, module_reader_noHV_1);
      bad_digi_ana=badpixelmonsterhierarchy[9]+badpixelmonsterhierarchy[8]+badpixelmonsterhierarchy[7]+badpixelmonsterhierarchy[6]+badpixelmonsterhierarchy[5]+badpixelmonsterhierarchy[4];
      disconnectedbumps=badpixelmonsterhierarchy[2];
      xtalk=badpixelmonsterhierarchy[0];
      high_noise=badpixelmonsterhierarchy[1];
      break_down = IV_Mod(module_reader_HV_1, module_reader_noHV_1);
      std::cout<<"BREAKDOWN:\t"<<break_down<<std::endl;
      FE_ID = (*iii);

    
      counter++;
      tl->Fill();


    }
  }
  


  std::cout<<"SIZE: "<<counter<<std::endl;

  int ncolumns = 6, nrows = counter;//FE_ID.size();
  Double_t** data = new Double_t*[nrows];
  for (int i = 0; i < nrows; i++) {
    data[i] = new Double_t[ncolumns];
  }   


  TGMainFrame* mainframe = new TGMainFrame(0, 400, 500);
  mainframe->SetCleanup(kDeepCleanup);



  TString varexp = "FE_ID:bad_digi_ana:disconnectedbumps:high_noise:xtalk:break_down";
  TString select = "";
  TString options = "";


  // Create an interface
  TTreeTableInterface *iface = new TTreeTableInterface(tl,varexp.Data(),select.Data(),options.Data());



  // Create the table
  TGTable *table = new TGTable(mainframe, 999, iface); 

  // Add the table to the main frame
  mainframe->AddFrame(table, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));

  //Update data
  //   data[5][1] = 3.01; 
  //update the table view
  table->Update(); 

  // Layout and map the main frame
  mainframe->SetWindowName("Available Modules Summary ") ;
  mainframe->MapSubwindows() ;
  mainframe->Layout();
  mainframe->Resize();
  mainframe->MapWindow() ;

  return mainframe;
  

}



void ModulesSummaryTXT(ProcessPosition PP, std::vector<std::string> modules){
  
  HistoReader *module_reader_HV_1, *module_reader_noHV_1,*module_reader_HV_2, *module_reader_noHV_2;  
  double noiseMean, noiseSigma, thrMean, thrSigma;
  std::string address = DbTools::GetInstance()->DatabasePath() +"/modules/DB_modules.txt";
  std::cout<<address<<std::endl;
  ofstream myfile (address.c_str());
  if (myfile.is_open())
    {
      myfile << "FE_ID\tTechnology\tBreakdown\tOperational_Curr\tMean_THR\tSigma_THR\tMean_Noise\tSigma_Noise\n";
      // myfile.close();
    }
  else std::cout << "Unable to open file"<<std::endl;
  int disconnectedbumps, bad_digi_ana, high_noise, xtalk;
  std::string break_down;
  std::vector<int> badpixelmonsterhierarchy;
  std::string FE_ID; 

  int counter = 0, counter2 = 0;
  for(std::vector<std::string>::iterator iii = modules.begin(); iii != modules.end(); iii++)counter2++;
  std::cout<<"counter2 is "<<counter2<<std::endl;
  //  if(counter2>120){
  for(std::vector<std::string>::iterator iii = modules.begin(); iii != modules.end(); iii++){
        
    std::cout<<"COUNTER is "<<counter<<", module is "<<iii->c_str()<<std::endl;
    if (DbTools::GetInstance()->IsDoubleChip(iii->c_str())==1) {
      module_reader_HV_1 = new HistoReader(iii->c_str(), PP, 0,1);
      module_reader_noHV_1 = new HistoReader(iii->c_str(), PP, 0,0);


	
      badpixelmonsterhierarchy = BadPixelMonsterHierarchy(module_reader_HV_1, module_reader_noHV_1);
      bad_digi_ana=badpixelmonsterhierarchy[9]+badpixelmonsterhierarchy[8]+badpixelmonsterhierarchy[7]+badpixelmonsterhierarchy[6]+badpixelmonsterhierarchy[5]+badpixelmonsterhierarchy[4];
      disconnectedbumps=badpixelmonsterhierarchy[2];
      xtalk=badpixelmonsterhierarchy[0];
      high_noise=badpixelmonsterhierarchy[1];
      break_down = IV_Mod(module_reader_HV_1, module_reader_noHV_1);
      FE_ID=(*iii+"_0");


      if(module_reader_HV_1->Thre_sigmadist!=NULL && module_reader_HV_1->Thre_sigmadist->Integral()>20){
	Fit_noisedistro(module_reader_HV_1->Thre_sigmadist);
        thrMean = module_reader_HV_1->Thre_threshdist->GetFunction("gauss")->GetParameter(1);
        thrSigma = module_reader_HV_1->Thre_threshdist->GetFunction("gauss")->GetParameter(2);
        noiseMean = module_reader_HV_1->Thre_sigmadist->GetFunction("gaus1")->GetParameter(1);
        noiseSigma = module_reader_HV_1->Thre_sigmadist->GetFunction("gaus1")->GetParameter(2);
      }else{
        thrMean = 0;
        thrSigma = 0;
        noiseMean = 0;
        noiseSigma = 0;

      }



      if(myfile.is_open())myfile<<FE_ID<<"\t"<<"Planar"<<"\t"<<break_down<<"\t"<<OperationalCurrent(module_reader_HV_1)<<"\t"<<thrMean<<"\t"<<thrSigma<<"\t"<<noiseMean<<"\t"<<noiseSigma<<"\n";
      std::cout<<"BREAKDOWN: "<<break_down<<std::endl;
      delete module_reader_HV_1;
      delete module_reader_noHV_1;
    
      counter++;

      module_reader_HV_2 = new HistoReader(iii->c_str(), PP, 1,1);
      module_reader_noHV_2 = new HistoReader(iii->c_str(), PP, 1,0);

      badpixelmonsterhierarchy = BadPixelMonsterHierarchy(module_reader_HV_2, module_reader_noHV_2); 

      bad_digi_ana=badpixelmonsterhierarchy[9]+badpixelmonsterhierarchy[8]+badpixelmonsterhierarchy[7]+badpixelmonsterhierarchy[6]+badpixelmonsterhierarchy[5]+badpixelmonsterhierarchy[4];
      disconnectedbumps=badpixelmonsterhierarchy[2];
      xtalk=badpixelmonsterhierarchy[0];
      high_noise=badpixelmonsterhierarchy[1];
      break_down = IV_Mod(module_reader_HV_2, module_reader_noHV_2);
      FE_ID = (*iii+"_1");

      if(module_reader_HV_2->Thre_sigmadist!=NULL && module_reader_HV_2->Thre_sigmadist->Integral()>20){
	Fit_noisedistro(module_reader_HV_2->Thre_sigmadist);
        thrMean = module_reader_HV_2->Thre_threshdist->GetFunction("gauss")->GetParameter(1);
        thrSigma = module_reader_HV_2->Thre_threshdist->GetFunction("gauss")->GetParameter(2);
        noiseMean = module_reader_HV_2->Thre_sigmadist->GetFunction("gaus1")->GetParameter(1);
        noiseSigma = module_reader_HV_2->Thre_sigmadist->GetFunction("gaus1")->GetParameter(2);
      }else{
        thrMean = 0;
        thrSigma = 0;
        noiseMean = 0;
        noiseSigma = 0;

      }



      if(myfile.is_open())myfile<<FE_ID<<"\t"<<"Planar"<<"\t"<<break_down<<"\t"<<OperationalCurrent(module_reader_HV_2)<<"\t"<<thrMean<<"\t"<<thrSigma<<"\t"<<noiseMean<<"\t"<<noiseSigma<<"\n";
      std::cout<<"BREAKDOWN: "<<break_down<<std::endl;
      delete module_reader_HV_2;
      delete module_reader_noHV_2;
    
    
      counter++;


    
    }else{
      std::cout<<"III->C_STR() "<<iii->c_str()<<std::endl;
      module_reader_HV_1 = new HistoReader(iii->c_str(), PP, 0,1);
      module_reader_noHV_1 = new HistoReader(iii->c_str(), PP, 0,0);

      badpixelmonsterhierarchy = BadPixelMonsterHierarchy(module_reader_HV_1, module_reader_noHV_1);
      bad_digi_ana=badpixelmonsterhierarchy[9]+badpixelmonsterhierarchy[8]+badpixelmonsterhierarchy[7]+badpixelmonsterhierarchy[6]+badpixelmonsterhierarchy[5]+badpixelmonsterhierarchy[4];
      disconnectedbumps=badpixelmonsterhierarchy[2];
      xtalk=badpixelmonsterhierarchy[0];
      high_noise=badpixelmonsterhierarchy[1];
      break_down = IV_Mod(module_reader_HV_1, module_reader_noHV_1);
      std::cout<<"BREAKDOWN:\t"<<break_down<<std::endl;
      FE_ID = (*iii);

      if(module_reader_HV_1->Thre_sigmadist!=NULL && module_reader_HV_1->Thre_sigmadist->Integral()>20){
	Fit_noisedistro(module_reader_HV_1->Thre_sigmadist);
	thrMean = module_reader_HV_1->Thre_threshdist->GetFunction("gauss")->GetParameter(1);
	thrSigma = module_reader_HV_1->Thre_threshdist->GetFunction("gauss")->GetParameter(2);
	noiseMean = module_reader_HV_1->Thre_sigmadist->GetFunction("gaus1")->GetParameter(1);
	noiseSigma = module_reader_HV_1->Thre_sigmadist->GetFunction("gaus1")->GetParameter(2);
      }else{
	thrMean = 0;
	thrSigma = 0;
	noiseMean = 0;
	noiseSigma = 0;

      }



      if(myfile.is_open())myfile<<FE_ID<<"\t"<<"3D"<<"\t"<<break_down<<"\t"<<OperationalCurrent(module_reader_HV_1)<<"\t"<<thrMean<<"\t"<<thrSigma<<"\t"<<noiseMean<<"\t"<<noiseSigma<<"\n";
      std::cout<<"BREAKDOWN: "<<break_down<<std::endl;
      delete module_reader_HV_1;
      delete module_reader_noHV_1;
    
    
      counter++;
      //    }


    }
  }
  


  myfile.close();

}


//------------------------------------------
void BadPixelAllStaves(ProcessPosition asource_before, ProcessPosition asource_after){

  THStack *BP_allstaves = new THStack("Bad_Pixel_for_All_Staves","Bad Pixel for All Staves");
 
  TH1F *hist_allbadanalog = new TH1F("hist_allbadanalog","hist_allbadanalog",20,0.5,20.5);
  hist_allbadanalog->SetFillColor(kRed);
  hist_allbadanalog->SetLineColor(kRed+2);
  hist_allbadanalog->SetFillStyle(3004);

  TH1F *hist_allbaddigital = new TH1F("hist_allbaddigital","hist_allbaddigital",20,0.5,20.5);
  hist_allbaddigital->SetFillColor(kAzure);
  hist_allbaddigital->SetLineColor(kAzure+2);
  hist_allbaddigital->SetFillStyle(3001);

  TH1F *hist_allbadthresh = new TH1F("hist_allbadthresh","hist_allbadthresh",20,0.5,20.5);
  hist_allbadthresh->SetFillColor(kGreen);
  hist_allbadthresh->SetLineColor(kGreen+2);
  hist_allbadthresh->SetFillStyle(3005);

  TH1F *hist_allbadthresh1 = new TH1F("hist_allbadthresh1","hist_allbadthresh1",20,0.5,20.5);
  hist_allbadthresh1->SetFillColor(kGreen);
  hist_allbadthresh1->SetLineColor(kGreen+2);
  hist_allbadthresh1->SetFillStyle(3005);

  TH1F *hist_allbadnoisy = new TH1F("hist_allbadnoisy","hist_allbadnoisy",20,0.5,20.5); 
  hist_allbadnoisy->SetFillColor(kViolet);
  hist_allbadnoisy->SetLineColor(kViolet+2);
  hist_allbadnoisy->SetFillStyle(3009);

  Astave *onestave_HV = new Astave();
  //Astave *onestave_noHV = new Astave();

  std::vector<Astave*> VectStaves_HV;
  //std::vector<Astave*> VectStaves_noHV;
  std::stringstream ss;

  for(int s=1;s<=20;s++) {
    //std::stringstream ss;
      ss << s;
      std::string staveid = ss.str();
    
      if(staveid=="17")  onestave_HV->mountmodules(staveid,asource_after,1);
      else onestave_HV->mountmodules(staveid,asource_before,1);
      VectStaves_HV.push_back(onestave_HV);
      //onestave_noHV->mountmodules(staveid,asource,0);
      //VectStaves_noHV.push_back(onestave_noHV);
      ss.str(""); 
  }
  
  HistoReader *aReader_HV;
  //HistoReader *aReader_noHV; 
  
  std::vector<HistoReader*> aVectReader_HV;
  //std::vector<HistoReader*> aVectReader_noHV; 
  
  std::vector <int> badallstaves;
  for (int i=0;i<badallstaves.size();i++) badallstaves.push_back(0);
  
  float allbadanalog = 0;
  float allbaddigital = 0;
  float allbadthresh = 0;
  float allbadnoisy = 0;
  /*
  // very bad staff for stave 1 to make the plot equal to the antonello's one who took for the stave1 failing s-curve fit pixel the result from before tc before tuning
  Astave *stave1 = new Astave();
  stave1->mountmodules("1",(ProcessPosition)PP_StaveTestBeforeTC_BT,1);
  HistoReader *aReader_HVstave1;
  std::vector<HistoReader*> aVectReader_HVstave1  = stave1->StaveReaderModule();
  std::vector <int> badstave1;
  for (int i=0;i<badstave1.size();i++) badstave1.push_back(0);
  int allbadthresh1=0;
  */
  // hardcoded badly 
  hist_allbadanalog->AddBinContent(1,(300./8601.60));
  hist_allbaddigital->AddBinContent(1,(80./8601.60));
  hist_allbadnoisy->AddBinContent(1,(104./8601.60));
  hist_allbadthresh->AddBinContent(1,(116./8601.60)); // --> this one was the problem since he had to come from the PP beforetc before tuning to agree with the antonello's plot.
  // As soon as I'll have more memory ran I can fix it using first of all stave with and without HV (to be coherent) and then I can do a function which select the best result in term of bad pixel and use that result as PP
  // Antonello's result come from the BadDistribution function   
  //////////////////////////////////////////////////////////////////////////////
  std::cout << "SONO QUA 1 " <<  std::endl;

  for(int i=2;i<=VectStaves_HV.size();i++) {
    aVectReader_HV = VectStaves_HV.at(i-1)->StaveReaderModule();
    //aVectReader_noHV = VectStaves_noHV.at(i-1)->StaveReaderModule();
    
    for(int j=0;j<32;j++) {
      aReader_HV = aVectReader_HV.at(j+(32*(i-1)));
      //aReader_noHV = aVectReader_noHV.at(j+(32*(i-1)));
      
      std::cout << "SONO QUA 2 " << aReader_HV->Stat_Module_ID <<  std::endl;
      badallstaves = BadPixelMonsterHierarchy(aReader_HV,aReader_HV);
      
      allbadanalog = badallstaves.at(4)+badallstaves.at(5)+badallstaves.at(6);
      allbaddigital = badallstaves.at(7)+badallstaves.at(8)+badallstaves.at(9);
      allbadnoisy = badallstaves.at(1);
      allbadthresh = badallstaves.at(3);
      
      allbadanalog = allbadanalog/8601.60;
      allbaddigital =allbaddigital/8601.60;
      allbadnoisy =allbadnoisy/8601.60;
      allbadthresh =allbadthresh/8601.60;	   
      
      hist_allbadanalog->AddBinContent(i,allbadanalog);
      hist_allbaddigital->AddBinContent(i,allbaddigital);
      hist_allbadnoisy->AddBinContent(i,allbadnoisy);
      hist_allbadthresh->AddBinContent(i,allbadthresh);
      
      std::cout << " ALL BAD ANALOG " << allbadanalog << " " << j << std::endl;
      
    }  
  }
  
  std::cout << " ALL BAD ANALOG 2 " << allbadanalog << std::endl;

  BP_allstaves->Add(hist_allbaddigital);
  BP_allstaves->Add(hist_allbadanalog);
  BP_allstaves->Add(hist_allbadthresh);
  // BP_allstaves->Add(hist_allbadthresh1);
  BP_allstaves->Add(hist_allbadnoisy);
  
  BP_allstaves->Draw("HIST");
  BP_allstaves->GetXaxis()->SetTitle("Staves");
  BP_allstaves->GetYaxis()->SetTitle("Bad Pixels (\%)");
  gPad->Modified();

  TLegend* leg = new TLegend(0.1,0.6,0.25,0.9);
  //leg->SetHeader("Number ot Bad Pixel after Loading");
  leg->AddEntry(hist_allbadnoisy,"Noisy","F");
  leg->AddEntry(hist_allbadthresh,"Failing S-Curve","F");
  leg->AddEntry(hist_allbadanalog,"Bad Analog","F");
  leg->AddEntry(hist_allbaddigital,"Bad Digital","F");
  leg->SetFillColor(kWhite);
  leg->Draw();
  drawAtlasInternal1D();
  VectStaves_HV.clear();  
}


//------------------------------------------
void NoiseMeanStaves(std::vector<Astave*> aStave, ProcessPosition asource){

  // Astave *as;
  // as->mountmodules("20",PP_StaveTestBeforeTC_AT,1);
  std::vector<HistoReader*> aReader;
  TH1D* noisemeanvalue = new TH1D("noisemeanvalue","noisemeanvalue", 20, 0.5, 20.5);
  double mean_staves[20], mean, mean_modules[32] = {0}, mean_stave;
  double sigma_staves[20], sigma, sigma_modules[32] = {0}, sigma_stave;
  int counter = 0;

  //  std::cout<<"aStave size"<<aStave.size()<<std::endl;
 
  for(int staves=1; staves<=aStave.size(); staves++){
    aReader = aStave.at(staves-1)->StaveReaderModule();
    counter=0;
    //mean_modules=[0];
    //sigma_modules[32]={0};
    mean_stave = 0;
    sigma_stave=0;
    for(int i=0;i<32;i++){
      if(aReader.at(i+(32*(staves-1)))->Thre_sigmadist != NULL && aReader.at(i+(32*(staves-1)))->Thre_sigmadist->GetEntries() > 1){
	Fit_noisedistro(aReader.at(i+(32*(staves-1)))->Thre_sigmadist);
	mean = aReader.at(i+(32*(staves-1)))->Thre_sigmadist->GetFunction("gaus1")->GetParameter(1);
	sigma = aReader.at(i+(32*(staves-1)))->Thre_sigmadist->GetFunction("gaus1")->GetParameter(2);
	if(mean!=NULL){counter++;
          std::cout<<"mean: "<<mean<<"\tcounter: "<<counter<<"\tstave:"<<staves<<std::endl;
          mean_modules[i] = mean;
          sigma_modules[i] = sigma;}
                       
      }
        
    }
    for(int j=0;j<32;j++){
      if(counter!=0){
	mean_stave += (mean_modules[j] /counter);
	sigma_stave += (sigma_modules[j]/counter);
	mean_modules[j]=0;
	sigma_modules[j]=0;
      }else{
	mean_stave = 0;
	sigma_stave = 0;
	mean_modules[j]=0;
	sigma_modules[j]=0;
      }
    }
    std::cout<<"STAVE: "<<staves<<"\tMEAN stave: "<<mean_stave<<std::endl;
    noisemeanvalue->SetBinContent(staves,mean_stave);
    noisemeanvalue->SetBinError(staves,sigma_stave);

  }

  noisemeanvalue->Draw();
}


void ENC_distroStaves(std::vector<Astave*> aStave, ProcessPosition asource){

  std::vector<HistoReader*> aReader;
  TH1D* NMV_FBK = new TH1D("noisemeanvalue_FBK","noisemeanvalue_FBK", 50, 100.5, 250.5);
  TH1D* NMV_CNM = new TH1D("noisemeanvalue_CNM","noisemeanvalue_CNM", 50, 100.5, 250.5);
  TH1D* NMV_Planar = new TH1D("noisemeanvalue_Planar","noisemeanvalue_Planar", 50, 100.5, 250.5);
  THStack* noise_distribution = new THStack("noise_distribution","ENC distributions");

  double mean;

  for(int staves=1; staves<=aStave.size(); staves++){
    aReader = aStave.at(staves-1)->StaveReaderModule();
    for(int i=0;i<32;i++){
      if(aReader.at(i+(32*(staves-1)))->Thre_sigmadist != NULL && aReader.at(i+(32*(staves-1)))->Thre_sigmadist->GetEntries() > 1){
        Fit_noisedistro(aReader.at(i+(32*(staves-1)))->Thre_sigmadist);
        mean = aReader.at(i+(32*(staves-1)))->Thre_sigmadist->GetFunction("gaus1")->GetParameter(1);
        if(mean!=0){
          if(aReader.at(i+32*(staves-1))->Stat_Module_ID.substr(0,1)=="D")NMV_Planar->Fill(mean);
          else if(aReader.at(i+32*(staves-1))->Stat_Module_ID.substr(0,1)=="F")NMV_FBK->Fill(mean);
          else if(aReader.at(i+32*(staves-1))->Stat_Module_ID.substr(0,1)=="C")NMV_CNM->Fill(mean);
          }
        }
      }
    }
  NMV_Planar->SetFillColor(kBlue);
  NMV_FBK->SetFillColor(kRed);
  NMV_CNM->SetFillColor(kGreen+3);

  NMV_Planar->SetFillStyle(3002);
  NMV_FBK->SetFillStyle(3004);
  NMV_CNM->SetFillStyle(3005);

  noise_distribution->Add(NMV_Planar);
  noise_distribution->Add(NMV_FBK);
  noise_distribution->Add(NMV_CNM);
  noise_distribution->Draw("nostack");
  noise_distribution->GetXaxis()->SetTitle("Equivalent Noise Charge");
  noise_distribution->GetYaxis()->SetTitle("Number of FE/3#e^-"); 

  TLegend* leg = new TLegend(0.1, 0.1, 0.3, 0.2);
  leg->AddEntry(NMV_Planar, "Planar CiS", "F");
  leg->AddEntry(NMV_FBK, "3D FBK", "F");
  leg->AddEntry(NMV_CNM, "3D CNM", "F");
  leg->SetFillColor(kWhite);


  leg->Draw();
  drawAtlasInternal1D();
  drawHistoTitle("ENC after loading");
}


void IV_all_staves(std::vector<Astave*> aStave, ProcessPosition asource ){
  //void IV_Planar(Astave* aStave,
  //	       const std::string staveid, ProcessPosition asource)
  //{
  Astave *as = new Astave();
  as->mountmodules("20",PP_StaveTestBeforeTC_AT,1);



  std::vector<IVMeasurementPoint> iv;
  std::vector<TH1D*> histo_iv;
  THStack* IVs= new THStack("IV_all_staves","IV_all_staves"); 
  TH1D* trash;
  int counter = 0;
  for(int staves=1; staves<=20; staves++){
    for (int i=0;i<8;i++)
      {
        
        std::stringstream ss;
        ss << staves;
        std::string staveid = ss.str();
        iv = aStave.at(staves-1)->GetIVStaveVector(staveid, asource, i);
        trash = aStave.at(staves-1)->HistoIVLoader(iv);
        histo_iv.push_back(trash);
        if(i>0 && i<7)histo_iv.at(counter)->SetLineColor(kAzure+i%3);
        else histo_iv.at(counter)->SetLineColor(kPink-i);
        iv.clear( );
        IVs->Add(histo_iv.at(counter));
        counter++;
      }
  }  
  /*  for(int i=0; i<8; i++){
      iv = as->GetIVStaveVector("20", asource, i);
      trash = as->HistoIVLoader(iv);
      histo_iv.push_back(trash);
      if(i>0 && i<7)histo_iv.at(counter)->SetLineColor(kAzure+i%3);
      else histo_iv.at(counter)->SetLineColor(kPink-i);
      iv.clear( );
      IVs->Add(histo_iv.at(counter));
      counter++;
      }*/
  gPad->SetGrid();



  IVs->Draw("plnostack");
  IVs->GetXaxis()->SetTitle("HV(V)");
  IVs->GetYaxis()->SetTitle("I (#muA)"); 
  IVs->GetYaxis()->SetTitleOffset(1.4);
  IVs->SetMaximum(100);
  gPad->Update();
  std::cout<<"#Number of sectors:"<<counter<<std::endl;
}





void IV_all_modules(ProcessPosition PP, std::vector<std::string> modules){

  std::vector<TGraph*> iv_plot;
  std::vector<std::string> stringa;
  HistoReader *historeader;
  TLegend* leg = new TLegend(0.1, 0.1, 0.3, 0.2);
  int counter = 0, doublechip_id = 0, CNM_id = 0, FBK_id = 0, counter_FBK=0, counter_CNM=0;


  for(std::vector<std::string>::iterator iii = modules.begin(); iii != modules.end(); iii++){
   
    historeader = new HistoReader(iii->c_str(), PP, 0,1);
    //std::cout<<"COUNTER IS: "<<counter<<std::endl;
    if(historeader->IV_Mod_Graph!=NULL){
      iv_plot.push_back(historeader->IV_Mod_Graph);
      stringa.push_back(historeader->Stat_Module_ID);
      counter++;
    }
  }
  int color = 0;
  for(int i=0; i<counter;i++){
    color = i % 3;
   
    iv_plot.at(i)->SetLineWidth(2);
    std::string ss = stringa[i].substr(0,1);
    if(DbTools::GetInstance()->IsDoubleChip(stringa[i].c_str())==1){
      iv_plot.at(i)->SetLineColor(kAzure+color);
      doublechip_id = i;
    }else if(ss == "C"){
      iv_plot.at(i)->SetLineColor(kGreen+color);
      CNM_id = i;
      counter_CNM++;
     
    }else{
      iv_plot.at(i)->SetLineColor(kPink-color);
      FBK_id = i;
      counter_FBK++;
    }
    //if(i==0)iv_plot.at(i)->Draw("AL");
    iv_plot.at(i)->GetXaxis()->SetLimits(-350,0);
    iv_plot.at(i)->GetYaxis()->SetLimits(-0.00002,0);
    iv_plot.at(i)->GetXaxis()->SetTitle("Voltage (V)");
    iv_plot.at(i)->GetYaxis()->SetTitle("Current (microA)");
   
    if(i==0){
      iv_plot.at(i)->Draw("AL");
      iv_plot.at(i)->GetYaxis()->SetRangeUser(-20e-6,0);
     
    }
   
    else iv_plot.at(i)->Draw("Lsame");
  }
 
 
  leg->AddEntry(iv_plot[doublechip_id], "Double Chip", "l");
  leg->AddEntry(iv_plot[FBK_id], "FBK", "l");
  leg->AddEntry(iv_plot[CNM_id], "CNM", "l");
  leg->SetFillColor(kYellow-10);
 
  std::cout<<"### Number of FBK: "<<counter_FBK<<std::endl;
  std::cout<<"### Number of CNM: "<<counter_CNM<<std::endl;  
 
  leg->Draw();
}

TH1D* IVGraph_HistoConverter(TGraph *graph){
  Double_t* array_x = graph->GetX();
  Double_t* array_y = graph->GetY();
  double a;
  //  int nPoint = sizeof(array_x)/sizeof(double);
  int nPoint = graph->GetN();
  std::cout<<"nPoint is: "<<nPoint<<std::endl;
  double norm_to_micron=0.001;
  double xMax = abs(array_x[nPoint-1]);
  std::cout<<"tensione massima: "<<xMax<<std::endl;
  TH1D* IV_histo = new TH1D(Form("h%d",rand()%1000),"IV_histo",nPoint, 0-0.5*abs(array_x[1]), xMax+0.5*abs(array_x[1]));
  
  for(int i=0; i<nPoint;i++){
    std::cout<<"|||||||Voltage point: "<<array_x[i]<<"\tCurrent point: "<<array_y[i]<<std::endl;
    a = IV_TempScalingFactor(296,289)*array_y[i] *1e9*norm_to_micron;
    IV_histo->Fill(abs(array_x[i]),abs(a));    
    //IV_histo->SetBinContent(i+1,iv.at(i).current);
  }
  return IV_histo;
  delete IV_histo;



}


void IV_DC_modules(ProcessPosition PP, std::vector<std::string> modules){

  std::vector<TGraph*> iv_plot;
  std::vector<std::string> stringa;
  HistoReader *historeader;
  TLegend* leg = new TLegend(0.1, 0.1, 0.3, 0.2);
  int counter = 0, doublechip_id = 0, CNM_id = 0, FBK_id = 0, counter_FBK=0, counter_CNM=0;


  for(std::vector<std::string>::iterator iii = modules.begin(); iii != modules.end(); iii++){
  
    historeader = new HistoReader(iii->c_str(), PP, 0,1);
    //std::cout<<"COUNTER IS: "<<counter<<std::endl;
    if(historeader->IV_Mod_Graph!=NULL && historeader->Stat_Module_ID.substr(0,1) == "D"){
      iv_plot.push_back(historeader->IV_Mod_Graph);
      stringa.push_back(historeader->Stat_Module_ID);
      counter++;
    }
  }
  int color = 0;
  for(int i=0; i<counter;i++){
    color = i % 3;
     
    iv_plot.at(i)->SetLineWidth(2);
    std::string ss = stringa[i].substr(0,1);
    if(DbTools::GetInstance()->IsDoubleChip(stringa[i].c_str())==1){
      iv_plot.at(i)->SetLineColor(kAzure+color);
      doublechip_id = i;
    }else if(ss == "C"){
      iv_plot.at(i)->SetLineColor(kGreen+color);
      CNM_id = i;
      counter_CNM++;

    }else{
      iv_plot.at(i)->SetLineColor(kPink-color);
      FBK_id = i;
      counter_FBK++;
    }
    //if(i==0)iv_plot.at(i)->Draw("AL");
    iv_plot.at(i)->GetXaxis()->SetLimits(-350,0);
    iv_plot.at(i)->GetYaxis()->SetLimits(-0.00002,0);
    iv_plot.at(i)->GetXaxis()->SetTitle("Voltage (V)");
    iv_plot.at(i)->GetYaxis()->SetTitle("Current (microA)");

    if(i==0){
      iv_plot.at(i)->Draw("AL");
      iv_plot.at(i)->GetYaxis()->SetRangeUser(-20e-6,0);
       
    }

    else iv_plot.at(i)->Draw("Lsame");
  }


  leg->AddEntry(iv_plot[doublechip_id], "Double Chip", "l");
  //  leg->AddEntry(iv_plot[FBK_id], "FBK", "l");
  //  leg->AddEntry(iv_plot[CNM_id], "CNM", "l");
  leg->SetFillColor(kYellow-10);

  std::cout<<"### Number of FBK: "<<counter_FBK<<std::endl;
  std::cout<<"### Number of CNM: "<<counter_CNM<<std::endl;  

  leg->Draw();
}



double IV_TempScalingFactor(double T_duringTest, double T_projected){
  double k_boltz = 8.6173324e-5;
  double T_ref = T_projected; // Eg @ 273 is equal to:
  double Eg_ref = 1.166 - (4.73e-4*T_ref*T_ref/(T_ref+636));
  double KK_ref = -Eg_ref/(2*k_boltz);
	
  // IV_Off
  double Eg_a = 1.166 - (4.73e-4*T_duringTest*T_duringTest/(T_duringTest+636));

  double KK_a = -Eg_a/(2*k_boltz);
  double ratio_a = (T_ref/T_duringTest)*(T_ref/T_duringTest)*exp((KK_ref/T_ref-KK_a/T_duringTest));

  std::cout<<"Temperature Scaling Factor IS: "<<ratio_a<<std::endl;

  return ratio_a;


}

void DistroOpCurr(std::vector<std::string> modules){

  HistoReader *historeader;
  TH1I *distroOpCurr = new TH1I("DistroOpCurr", "DistroOpCurr", 20,0,20);

  for(std::vector<std::string>::iterator iii = modules.begin(); iii!=modules.end(); iii++){
    historeader = new HistoReader(iii->c_str(), PP_UsbPixReceptionTest, 0, 1);
    if(DbTools::GetInstance()->IsDoubleChip(iii->c_str())==1){
      std::cout<<"Operational Current: "<<OperationalCurrent(historeader)<<std::endl;
      distroOpCurr->Fill(OperationalCurrent(historeader)*1e6*(-1));
    }

  }
  distroOpCurr->SetFillColor(kRed);
  distroOpCurr->Draw();

}


//------------------------------------------
void DistroBreakDown(std::vector<std::string> modules){
  gPad->SetTickx();
  gPad->SetTicky();
std::vector<TGraph*> iv_plot;
  std::vector<std::string> stringa;
  std::string breakdown, string, module = "module";
  /*  TH1D* histo_break_Planar = new TH1D("histo_break_Planar","histo_break_Planar",73,-5,360);
  TH1D* histo_break_CNM = new TH1D("histo_break_CNM","histo_break_CNM",23,-5,360);
  TH1D* histo_break_FBK = new TH1D("histo_break_FBK","histo_break_FBK",23,-5,360);
  TH1D* histo_break_Ohmic = new TH1D("histo_break_Ohmic","histo_break_Ohmic",1,-5,0);
  */
  TH1D* histo_break_Planar = new TH1D("histo_break_Planar","histo_break_Planar",31,-12,360);
  TH1D* histo_break_CNM = new TH1D("histo_break_CNM","histo_break_CNM",31,-12,360);
  TH1D* histo_break_FBK = new TH1D("histo_break_FBK","histo_break_FBK",31,-12,360);
  TH1D* histo_break_Ohmic = new TH1D("histo_break_Ohmic","histo_break_Ohmic",1,-12,0);

  THStack* hs = new THStack("hs","V_{bd} distributions");

  TString label[2]={"Ohmic",">350"};
  
  HistoReader *historeader;
  TLegend* leg = new TLegend(0.65, 0.85, 0.85, 0.65);
  leg->SetTextSize(0.03);
  leg->SetFillColor(0);
  leg->SetTextSize(0.04);
  leg->SetBorderSize(0);

  int counter = 0, doublechip_id = 0, CNM_id = 0, FBK_id = 0;
  std::string IDMOD; 
  
 
  for(std::vector<std::string>::iterator iii = modules.begin(); iii != modules.end(); iii++){
    
    historeader = new HistoReader(iii->c_str(), PP_UsbPixReceptionTest, 0,1);
    
    std::string IDMOD = (*iii);
    
    if(historeader->IV_Mod_Graph!=NULL){
      
      if(DbTools::GetInstance()->IsDoubleChip(iii->c_str())==1 && historeader->Stat_Module_ID.substr(0,1)=="D"){
	if(IDMOD != "D92-21-03" &&  IDMOD != "D61-25-04" && IDMOD != "D61-10-04" &&
	   IDMOD != "D93-18-04" && IDMOD != "D93-18-01" && IDMOD != "D60-23-04" && IDMOD != "D94-20-01"/* those modules presented a bad IV or they were rejected*/){
	  if(IDMOD != "D12-21-01" && IDMOD != "D12-21-02" && IDMOD != "D12-21-03" && IDMOD != "D12-23-01" &&
	     IDMOD != "D12-12-02" && IDMOD != "D12-23-03" && IDMOD != "D12-23-04" && IDMOD != "D18-18-01" &&
	     IDMOD != "D18-18-02" && IDMOD != "D18-23-02" && IDMOD != "D18-23-03" && IDMOD != "D31-17-01" &&
	     IDMOD != "D31-17-03" && IDMOD != "D18-18-03" /*those module belong to second production*/){
	    if(module != iii->c_str()){
	      breakdown = IV_Mod(historeader, historeader);
	      if(breakdown == ">350")histo_break_Planar->Fill(350);
	      else if(breakdown == "no IV") std::cout << " no IV for module " << (*iii) << std::endl;
	      else 
		{
		  histo_break_Planar->Fill(atof(breakdown.c_str())); 
		}
	      module = (*iii);
	      //  continue;
	    }
	  }
	}
      }
      else if(DbTools::GetInstance()->IsDoubleChip(iii->c_str())!=1 && historeader->Stat_Module_ID.substr(0,1)=="F"){
	if(IDMOD != "F10-06-02" && IDMOD != "F10-08-08" && IDMOD != "F10-20-08" && IDMOD != "F10-22-07" && 
	   IDMOD != "F12-06-01" && IDMOD != "F12-06-05" && IDMOD != "F10-23-06" /*those module belong to second production*/ ){
	  
	  breakdown = IV_Mod(historeader, historeader);

	  if(breakdown == "no IV") std::cout << " no IV for module " << *iii << std::endl;
	  else if(breakdown == ">100") histo_break_FBK->Fill(100);
	  else histo_break_FBK->Fill(atof(breakdown.c_str()));
	}
      } 
      else if(DbTools::GetInstance()->IsDoubleChip(iii->c_str())!=1 && historeader->Stat_Module_ID.substr(0,1)=="C"){
	if(IDMOD !="C81-05-02" && IDMOD != "C42-03-07" && IDMOD != "C37-23-08" && IDMOD != "C36-04-01"/* those modules presented a bad IV or they were rejected*/ ){
	  if (IDMOD != "C36-05-08" && IDMOD != "C36-06-03" && IDMOD != "C36-11-07" && IDMOD != "C37-02-03" && 
	      IDMOD != "C42-02-07" && IDMOD != "C86-02-07" && IDMOD != "C86-06-08" && IDMOD != "C86-09-03" &&
	      IDMOD != "C36-21-01" && IDMOD != "C37-06-08" && IDMOD != "C81-06-06" && IDMOD != "C86-08-05" &&
	      IDMOD != "C85-10-01" && IDMOD != "C86-02-03" /*those module belong to second production*/){
	  
	    breakdown = IV_Mod(historeader, historeader);
	  
	    // these modules present a ohmic behavior
	    if(IDMOD == "C37-22-01") breakdown = "ohmic";
	    if(IDMOD == "C36-21-03") breakdown = "ohmic";
	    if(IDMOD == "C36-21-08") breakdown = "ohmic";
	    if(IDMOD == "C42-03-03") breakdown = "ohmic";
	    if(IDMOD == "C81-07-08") breakdown = "ohmic";
	    if(IDMOD == "C36-21-04") breakdown = "ohmic";
	    if(IDMOD == "C36-11-06") breakdown = "ohmic";
	    if(IDMOD == "C37-22-08") breakdown = "ohmic";
	    if(IDMOD == "C36-21-07") breakdown = "ohmic";
	    if(IDMOD == "C81-05-08") breakdown = "ohmic";
	    if(IDMOD == "C36-21-06") breakdown = "ohmic";
	    if(IDMOD == "C86-09-04") breakdown = "ohmic";
	    if(IDMOD == "C37-22-02") breakdown = "ohmic";
	    if(IDMOD == "C37-02-07") breakdown = "ohmic";
	    if(IDMOD == "C37-07-01") breakdown = "15";
	    if(IDMOD == "C33-17-03") breakdown = "15";
	    if(IDMOD == "C33-17-08") breakdown = "15";
	    
	    // if(breakdown=="ohmic") histo_break_Ohmic->Fill(-2.5);
	    if(breakdown=="ohmic") histo_break_Ohmic->Fill(-6.);
	    else if(breakdown == "no IV") std::cout << " no IV for module " << *iii << std::endl; 
	    else if(breakdown == ">100")  histo_break_CNM->Fill(100); 
	    else histo_break_CNM->Fill(atof(breakdown.c_str()));
	    
	  }
	}
      }
      counter++;
    }
  }
    
  histo_break_CNM->SetFillColor(kRed-10);
  histo_break_CNM->SetLineColor(kBlack);
  // histo_break_CNM->SetFillStyle(3004);
  histo_break_CNM->SetLineWidth(2);
  histo_break_Ohmic->SetFillColor(kRed-10);
  histo_break_Ohmic->SetLineColor(kBlack);
  histo_break_Ohmic->SetLineWidth(2);
  //  histo_break_Ohmic->SetFillStyle(3004);
  histo_break_FBK->SetFillColor(kRed);
  histo_break_FBK->SetLineColor(kBlack);
  histo_break_FBK->SetLineWidth(2);
  histo_break_FBK->SetFillStyle(3354);
  histo_break_Planar->SetFillColor(kBlue);
  histo_break_Planar->SetLineColor(kBlack);
  histo_break_Planar->SetLineWidth(2);
  histo_break_Planar->SetFillStyle(3345);


  /*  histo_break_CNM->Rebin(12);
  histo_break_Ohmic->Rebin(12);
  histo_break_FBK->Rebin(12);
  histo_break_Planar->Rebin(12);*/
  
  hs->Add(histo_break_Planar);
  hs->Add(histo_break_CNM);
  hs->Add(histo_break_FBK);
  hs->Add(histo_break_Ohmic);
 
  
  hs->Draw();
  
  hs->GetXaxis()->SetBinLabel(1,label[0]);
  hs->GetXaxis()->SetBinLabel(31,label[1]);
  hs->GetXaxis()->LabelsOption("h");
  
  /*for(int l=7;l<=71;l+=5){
    std::stringstream strs;
    strs << (l-1)*5;
    std::string temp_str = strs.str();
    char* char_type = (char*)temp_str.c_str();
    hs->GetXaxis()->SetBinLabel(l,char_type);
    }*/
  /* for(int l=5;l<=30;l+=5){
    std::stringstream strs;
    strs << l*10;
    std::string temp_str = strs.str();
    char* char_type = (char*)temp_str.c_str();
    hs->GetXaxis()->SetBinLabel(l,char_type);
    }*/
  hs->GetXaxis()->SetBinLabel(6,"50");
  hs->GetXaxis()->SetBinLabel(10,"100");
  hs->GetXaxis()->SetBinLabel(14,"150");
  hs->GetXaxis()->SetBinLabel(18,"200");
  hs->GetXaxis()->SetBinLabel(22,"250");
  hs->GetXaxis()->SetBinLabel(26,"300");
  hs->GetXaxis()->SetLabelSize(0.05);
  hs->GetXaxis()->SetTitle("Voltage Breakdown [V]");
  hs->GetYaxis()->SetTitle("Number of Modules");
  hs->GetYaxis()->SetTitleOffset(1.55);
  hs->GetXaxis()->SetTitleOffset(1.55);
  hs->SetTitle("");
  hs->Draw("nostack");
  gPad->SetTickx();
  gPad->SetTicky();
  gPad->Modified();
  gPad->Update();
  
  
  std::cout<<"Number of FBK: "<<histo_break_FBK->Integral()<<std::endl;
  std::cout<<"Number of CNM: "<<histo_break_CNM->Integral()<<std::endl;
  
  leg->AddEntry(histo_break_Planar, "Planar CiS", "F");
  leg->AddEntry(histo_break_FBK, "3D FBK", "F");
  leg->AddEntry(histo_break_CNM, "3D CNM", "F");
  leg->SetFillColor(0);

  leg->Draw();
  drawAtlasInternal1D();
  //  drawHistoTitle("Voltage Breakdown for all the modules ");
  
}


//------------------------------------------
/*bool check_usbpix_measurement(HistoReader *aReader_HV_usbpix, HistoReader *aReader_noHV_usbpix)
  {
  if(aReader_HV_usbpix->Digi_Occupancy_Point_000 == NULL || aReader_HV_usbpix->Anal_Occupancy_Point_000 == NULL || 
  aReader_HV_usbpix->Thre_thresh2d == NULL || aReader_HV_usbpix->Thre_Sigma == NULL || aReader_HV_usbpix->Thre_threshdist == NULL || 
  aReader_HV_usbpix->Thre_sigmadist == NULL || aReader_HV_usbpix->Cros_thresh2d == NULL || aReader_HV_usbpix->Cros_thresh1d == NULL || 
  aReader_HV_usbpix->Cros_threshdist == NULL ||  aReader_HV_usbpix->Cros_sigma1d == NULL ||  aReader_HV_usbpix->Cros_sigmadist == NULL ||  
  aReader_HV_usbpix->Cros_crosstalk1d == NULL || aReader_HV_usbpix->Cros_crosstalk2d == NULL ||  aReader_HV_usbpix->Cros_Sigma == NULL ||  
  aReader_HV_usbpix->Cros_ChiSquare == NULL ||  aReader_HV_usbpix->Cros_Mean == NULL )
  {
  std::cout << " NULL SCAN " << std::endl;
  return true;  
  }
  
  else if (aReader_noHV_usbpix->Digi_Occupancy_Point_000 == NULL || aReader_noHV_usbpix->Anal_Occupancy_Point_000 == NULL || 
  aReader_noHV_usbpix->Thre_thresh2d == NULL || aReader_noHV_usbpix->Thre_Sigma == NULL || aReader_noHV_usbpix->Thre_threshdist == NULL || 
  aReader_noHV_usbpix->Thre_sigmadist == NULL || aReader_noHV_usbpix->Cros_thresh2d == NULL || aReader_noHV_usbpix->Cros_thresh1d == NULL || 
  aReader_noHV_usbpix->Cros_threshdist == NULL ||  aReader_noHV_usbpix->Cros_sigma1d == NULL ||  aReader_noHV_usbpix->Cros_sigmadist == NULL ||  
  aReader_noHV_usbpix->Cros_crosstalk1d == NULL || aReader_noHV_usbpix->Cros_crosstalk2d == NULL ||  aReader_noHV_usbpix->Cros_Sigma == NULL ||  
  aReader_noHV_usbpix->Cros_ChiSquare == NULL ||  aReader_noHV_usbpix->Cros_Mean == NULL )
  {
  std::cout << " NULL SCAN " << std::endl;
  return true;  
  }

  else if (aReader_HV_usbpix->Digi_Occupancy_Point_000->Integral()== 0 || aReader_HV_usbpix->Anal_Occupancy_Point_000->Integral() == 0|| 
  aReader_HV_usbpix->Thre_thresh2d->Integral() == 0 || aReader_HV_usbpix->Thre_Sigma->Integral() == 0 || 
  aReader_HV_usbpix->Thre_threshdist->Integral() == 0 || aReader_HV_usbpix->Thre_sigmadist->Integral() == 0 || 
  aReader_HV_usbpix->Cros_thresh2d->Integral() == 0 || aReader_HV_usbpix->Cros_thresh1d->Integral() == 0 || 
  aReader_HV_usbpix->Cros_threshdist->Integral() == 0 || aReader_HV_usbpix->Cros_sigma1d->Integral() == 0 || 
  aReader_HV_usbpix->Cros_sigmadist->Integral() == 0 || aReader_HV_usbpix->Cros_crosstalk1d->Integral() == 0 ||  
  aReader_HV_usbpix->Cros_crosstalk2d->Integral() == 0 || aReader_HV_usbpix->Cros_Sigma->Integral() == 0 ||  
  aReader_HV_usbpix->Cros_ChiSquare->Integral() == 0 || aReader_HV_usbpix->Cros_Mean->Integral() == 0)
  {
  std::cout << " EMPTY SCAN " << std::endl;
  return true;  
  }

  else if (aReader_noHV_usbpix->Digi_Occupancy_Point_000->Integral()== 0 || aReader_noHV_usbpix->Anal_Occupancy_Point_000->Integral() == 0|| 
  aReader_noHV_usbpix->Thre_thresh2d->Integral() == 0 || aReader_noHV_usbpix->Thre_Sigma->Integral() == 0 || 
  aReader_noHV_usbpix->Thre_threshdist->Integral() == 0 || aReader_noHV_usbpix->Thre_sigmadist->Integral() == 0 || 
  aReader_noHV_usbpix->Cros_thresh2d->Integral() == 0 || aReader_noHV_usbpix->Cros_thresh1d->Integral() == 0 || 
  aReader_noHV_usbpix->Cros_threshdist->Integral() == 0 || aReader_noHV_usbpix->Cros_sigma1d->Integral() == 0 ||  
  aReader_noHV_usbpix->Cros_sigmadist->Integral() == 0 || aReader_noHV_usbpix->Cros_crosstalk1d->Integral() == 0 ||  
  aReader_noHV_usbpix->Cros_crosstalk2d->Integral() == 0 || aReader_noHV_usbpix->Cros_Sigma->Integral() == 0 ||  
  aReader_noHV_usbpix->Cros_ChiSquare->Integral() == 0 || aReader_noHV_usbpix->Cros_Mean->Integral() == 0)
    
  {
  std::cout << " EMPTY SCAN " << std::endl;
  return true;  
  }
  else return false;


  }
*/
/*
  char* filename = "~/IBLdata/analysis_result";
  std::string appended_line;
  fstream infile;
  infile.open(filename);
  bool module_check = false;
 
  if(infile){
  while(infile.good()){
  if(infile.get()==aReader_HV->Stat_Module_ID) module_check= true;
  }
  }
  else{
  ofstream infile(filename);
  infile << "ModuleID\tProcessPosition\tDigital_0\tDigital_Less\tDigital_Greater\tAnalog_0\tAnalog_Less\tAnalog_Greater\tThres_0\tThres_Disconnected\tThres_Noise350\n\n";
 
  }
 
  if(module_check==true){
 
  }
  else{
 
  }
*/



/* vim:set shiftwidth=2 tabstop=2 expandtab: */

