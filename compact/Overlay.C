#include "TH1.h"
#include "TH1F.h"

int dolog=0;
void Overlay()
{
  char* atitle = "timing weird units";


  TFile *f1 = new TFile("./hist.root");


  gStyle->SetOptStat(0);

  TString canvName = "Fig_";
  canvName += "hptdp_A_B";
  


  int W = 800;
  int H = 600;
  TCanvas* canv = new TCanvas(canvName,canvName,50,50,W,H);
  // references for T, B, L, R
  float T = 0.08*H;
  float B = 0.12*H;
  float L = 0.12*W;
  float R = 0.04*W;

  //canv = new TCanvas(canvName,canvName,50,50,W,H);
  canv->SetFillColor(0);
  canv->SetBorderMode(0);
  canv->SetFrameFillStyle(0);
  canv->SetFrameBorderMode(0);
  canv->SetLeftMargin( L/W );
  canv->SetRightMargin( R/W );
  canv->SetTopMargin( T/H );
  canv->SetBottomMargin( B/H );
  canv->SetTickx(0);
  canv->SetTicky(0);

  if (dolog) canv->SetLogy();


  TLatex latex;

  int n_ = 2;

  float x1_l = 1.2;

  float y1_l = 0.80;

  float dx_l = 0.60;
  float dy_l = 0.1;
  float x0_l = x1_l-dx_l;
  float y0_l = y1_l-dy_l;

  TLegend *lgd = new TLegend(x0_l,y0_l,x1_l, y1_l);
  lgd->SetBorderSize(0); lgd->SetTextSize(0.04); lgd->SetTextFont(62); lgd->SetFillColor(0);


  std::cout<<"getting first"<<std::endl;
  TH1F *A_pt = static_cast<TH1F*>(f1->Get("hcertime")->Clone());
  A_pt->SetDirectory(0);
  double aaA = A_pt->Integral();
  std::cout<<" first entries is "<<aaA<<std::endl;
  A_pt->Scale(1./aaA);


  std::cout<<"getting second"<<std::endl;
  TH1F *B_pt = static_cast<TH1F*>(f1->Get("hscinttime")->Clone());
  B_pt->SetDirectory(0);
  double aaB = B_pt->Integral();
  std::cout<<" second entries is "<<aaB<<std::endl;
  B_pt->Scale(1/aaB);


  float max = std::max(A_pt->GetMaximum(),B_pt->GetMaximum());
  A_pt->SetMaximum(max*1.3);

  A_pt->GetYaxis()->SetTitle("percent ");
  A_pt->GetYaxis()->SetTitleSize(0.05);
  A_pt->GetXaxis()->SetTitle(atitle);
  A_pt->GetXaxis()->SetTitleSize(0.05);



  A_pt->SetLineColor(3);
  A_pt->SetLineWidth(3);
  A_pt->SetStats(0);
  A_pt->Draw("");



  B_pt->SetLineColor(2);
  B_pt->SetLineWidth(3);
  B_pt->SetStats(0);

  //B_pt->Draw("esame");
  B_pt->Draw("same");

  canv->Update();
  canv->RedrawAxis();
  canv->GetFrame()->Draw();
  lgd->Draw();


  if (dolog) {
    canv->Print(canvName+"_log.pdf",".pdf");
    canv->Print(canvName+"_log.png",".png");}
  else{
    canv->Print(canvName+".pdf",".pdf");
    canv->Print(canvName+".png",".png");}
  return;
}



