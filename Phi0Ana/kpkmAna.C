#include "TFile.h"
#include "TH1F.h"
#include "TF1.h"
#include "TProfile.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TGraphMultiErrors.h"
#include "TLatex.h"
#include "TText.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TEllipse.h"
#include "TTree.h"
#include "TDatabasePDG.h"
#include "TDatabasePDG.h"
#include "TLorentzVector.h"
#include <iostream>
#include <fstream>
#include "TFormula.h"
#include "TLatex.h"
#include "TSystem.h"
#include "TString.h"
#include "TRatioPlot.h"

using namespace std;

void kpkmAna()
{
    gStyle->SetPalette(kRainBow); // choose different Palette color scheme
    gStyle->SetOptStat(0); // remove stat box
    gStyle->SetTitleSize(.06, "xyz");
    gStyle->SetLabelSize(.06, "xyz");
    // gStyle->SetTitleOffset(0.01, "y");
    // gStyle->SetTitleOffset(1.7, "X");
    // gStyle->SetLabelOffset(5.0, "y");

    gStyle->SetPadTopMargin(0.12);
    gStyle->SetPadBottomMargin(0.15);
    gStyle->SetPadRightMargin(0.13); // 2d:0.17, 1d:0.05
    gStyle->SetPadLeftMargin(0.18);  // 2d:0.16, 1d:0.15
    gStyle->SetLineWidth(2);

    gStyle->SetPadGridX(true);
    gStyle->SetPadGridY(true);

    TString outDir = "/Users/ahamdi/lbl/upc_loc/Phi0Ana/output_Phi0Ana";
    TFile *f_data = TFile::Open(Form("%s/Phi0Ana_data.root", outDir.Data()));
    TFile *f_mc = TFile::Open(Form("%s/Phi0Ana_mc.root", outDir.Data()));
    TFile *f_lumi = TFile::Open("/Users/ahamdi/lbl/upc_loc/Phi0Ana/lumi_ccup9.root");
    TFile *f_effvar = TFile::Open("/Users/ahamdi/lbl/upc_loc/Phi0Ana/effvar_tpcrejection_nacer.root");
    if(f_data == nullptr || f_mc == nullptr ||  f_lumi == nullptr || f_effvar == nullptr)
        cout<<"something went wrong while opening root files ! abort!"<<endl;
    
    TFile *outputFile = new TFile(Form("%s/kpkmAna.root",outDir.Data()), "RECREATE");

    FILE *tab_sys_m = fopen(Form("%s/tab_sys_m.tex",outDir.Data()), "w");
    fprintf(tab_sys_m,"\\documentclass[10pt]{extarticle}\n \\usepackage[margin=0.1in]{geometry}\n \\usepackage{tabularx}\n \\usepackage{caption} \n \\usepackage{makecell} \n \\usepackage{siunitx} \n \\captionsetup{labelformat=empty}\n \\begin{document}\n \\begin{table}[!htbp]\n \\centering\n \\caption{Systematic errors in pair mass bins}\n \\begin{tabular}{c|c|c|c|c|c|c}\n \\hline\n $m_{K^{+}K^{-}}$ (GeV/$c^2$) & $N_{generated}$ & $N_{MC}$ & $N_{data}$ & $\\sigma$ (mb) & \\thead{dE/dx mean (\\si{\\percent})\\\\($\\pm$ 3 \\si{\\percent})} & \\thead{dE/dx width (\\si{\\percent})\\\\($\\pm$ 3 \\si{\\percent})} \\\\ \n \\hline\n");

    // FILE *tab_eff = fopen(Form("%s/tab_eff.tex",outDir.Data()), "w");
    // fprintf(tab_eff,"\\documentclass[10pt]{extarticle}\n \\usepackage[margin=0.1in]{geometry}\n \\usepackage{tabularx}\n \\usepackage{caption} \n \\usepackage{makecell} \n \\usepackage{siunitx} \n \\captionsetup{labelformat=empty}\n \\begin{document}\n \\begin{table}[!htbp]\n \\centering\n \\caption{Systematic errors in pair mass bins}\n \\begin{tabular}{c|c|c|c|c|c|c}\n \\hline\n $m_{K^{+}K^{-}}$ (GeV/$c^2$) & $N_{generated}$ & $N_{MC}$ & $N_{data}$ & $\\sigma$ (mb) & \\thead{Sig/Bkg range (\\si{\\percent})\\\\(3$\\sigma$ $\\pm$ 3 \\si{\\percent})} & \\thead{Reject $\\pi^\\pm, \\mu^\\pm, e^\\pm$ (\\si{\\percent})\\\\(2$\\sigma$ $\\pm$ 3 \\si{\\percent})} \\\\ \n \\hline\n");

    FILE *tab_sys_theta = fopen(Form("%s/tab_sys_theta.tex",outDir.Data()), "w");
    fprintf(tab_sys_theta,"\\documentclass[10pt]{extarticle}\n \\usepackage[margin=0.1in]{geometry}\n \\usepackage{tabularx}\n \\usepackage{caption} \n \\usepackage{makecell} \n \\usepackage{siunitx} \n \\captionsetup{labelformat=empty}\n \\begin{document}\n \\begin{table}[!htbp]\n \\centering\n \\caption{Systematic errors in pair cos($\\theta$) bins}\n \\begin{tabular}{c|c|c|c|c|c|c}\n \\hline\n $cos(\\theta)$ (rad) & $N_{generated}$ & $N_{MC}$ & $N_{data}$ & $\\sigma$ (mb) & \\thead{Sig/Bkg range (\\si{\\percent})\\\\(3$\\sigma$ $\\pm$ 3 \\si{\\percent})} & \\thead{Reject $\\pi^\\pm, \\mu^\\pm, e^\\pm$ (\\si{\\percent})\\\\(2$\\sigma$ $\\pm$ 3 \\si{\\percent})} \\\\ \n \\hline\n");
    
    // Pair ransverse Momentum 
    auto cPtPair_all = new TCanvas("cPtPair_all", "cPtPair_all", 900, 600);
    cPtPair_all->cd();
    cPtPair_all->SetLogy();
    auto hPtPair_data = (TH1F *)f_data->Get("hPtPair");
    auto hPtPair_mc = (TH1F *)f_mc->Get("hPtPair");
    // hPtPair_data->Rebin(2);
    // hPtPair_mc->Rebin(2);
    hPtPair_data->Scale(1./hPtPair_data->Integral());
    hPtPair_mc->Scale(1./hPtPair_mc->Integral());
    // hPtPair_mc->SetMinimum(1.);
    hPtPair_mc->SetMarkerColor(2);
    hPtPair_mc->SetLineColor(2);
    hPtPair_mc->Draw();
    hPtPair_data->Draw("same");
    auto lPtPair_all = new TLegend(0.75, 0.75, 0.85, 0.88);
    lPtPair_all->SetTextSize(0.03);
    lPtPair_all->SetBorderSize(0);
    lPtPair_all->AddEntry(hPtPair_data, "Data", "l");
    lPtPair_all->AddEntry(hPtPair_mc, "MC", "l");
    lPtPair_all->Draw();
    cPtPair_all->Print(Form("%s/cPtPair_all.root", outDir.Data()),"root");
    cPtPair_all->Print(Form("%s/cPtPair_all.pdf", outDir.Data()),"pdf");

    // Pair Azimuthal angle 
    auto cAzimuthal_all = new TCanvas("cAzimuthal_all", "cAzimuthal_all", 900, 600);
    cAzimuthal_all->cd();
    // padAzimuthal_1->cd();    
    auto hAzimuthal_data = (TH1F *)f_data->Get("hAzimuthal");
    auto hAzimuthal_mc = (TH1F *)f_mc->Get("hAzimuthal");
    hAzimuthal_mc->SetTitle(";#phi (rad);Counts (Normalized)");
    hAzimuthal_data->Rebin(2);
    hAzimuthal_mc->Rebin(2);
    hAzimuthal_data->Scale(1./hAzimuthal_data->Integral());
    hAzimuthal_mc->Scale(1./hAzimuthal_mc->Integral());
    hAzimuthal_data->SetMinimum(0.);
    hAzimuthal_mc->SetMinimum(0.);
    hAzimuthal_mc->SetMarkerColor(2);
    hAzimuthal_mc->SetLineColor(2);
    hAzimuthal_mc->Draw("e");
    hAzimuthal_data->Draw("esame");
    auto lAzimuthal_all = new TLegend(0.75, 0.35, 0.85, 0.5);
    lAzimuthal_all->SetTextSize(0.03);
    lAzimuthal_all->SetBorderSize(0);
    lAzimuthal_all->AddEntry(hAzimuthal_data, "Data", "l");
    lAzimuthal_all->AddEntry(hAzimuthal_mc, "MC", "l");
    lAzimuthal_all->Draw();
    // auto hRatioAzimuthal = new TRatioPlot(hAzimuthal_mc, hAzimuthal_data);
    // hRatioAzimuthal->Draw();
    // hRatioAzimuthal->GetLowerRefYaxis()->SetTitle("ratio");
    // hRatioAzimuthal->GetUpperRefYaxis()->SetTitle("entries");
    cAzimuthal_all->Print(Form("%s/cAzimuthal_all.root", outDir.Data()),"root");
    cAzimuthal_all->Print(Form("%s/cAzimuthal_all.pdf", outDir.Data()),"pdf");

    // Pair VertexZ
    auto cVertexZ_all = new TCanvas("cVertexZ_all", "cVertexZ_all", 900, 600);
    cVertexZ_all->cd();
    auto hVertexZ_data = (TH1F *)f_data->Get("hVertexZ");
    auto hVertexZ_mc = (TH1F *)f_mc->Get("hVertexZ");
    hVertexZ_mc->SetTitle(";Vertex z-axis (cm);Counts (Normalized)");
    hVertexZ_data->Rebin(2);
    hVertexZ_mc->Rebin(2);
    hVertexZ_data->Scale(1./hVertexZ_data->Integral());
    hVertexZ_mc->Scale(1./hVertexZ_mc->Integral());
    hVertexZ_data->SetMinimum(0.);
    hVertexZ_mc->SetMinimum(0.);
    hVertexZ_mc->SetMarkerColor(2);
    hVertexZ_mc->SetLineColor(2);
    hVertexZ_mc->Draw("e");
    hVertexZ_data->Draw("esame");
    // auto hRatioVertexZ = new TRatioPlot(hVertexZ_mc, hVertexZ_data);
    // hRatioVertexZ->Draw();
    // hRatioVertexZ->GetLowerRefYaxis()->SetTitle("ratio");
    // hRatioVertexZ->GetUpperRefYaxis()->SetTitle("entries");
    auto lVertexZ_all = new TLegend(0.75, 0.75, 0.85, 0.88);
    lVertexZ_all->SetTextSize(0.03);
    lVertexZ_all->SetBorderSize(0);
    lVertexZ_all->AddEntry(hVertexZ_data, "Data", "l");
    lVertexZ_all->AddEntry(hVertexZ_mc, "MC", "l");
    lVertexZ_all->Draw();
    cVertexZ_all->Print(Form("%s/cVertexZ_all.root", outDir.Data()),"root");
    cVertexZ_all->Print(Form("%s/cVertexZ_all.pdf", outDir.Data()),"pdf");
    // printf("------------------ No problem up to here ! \n");

    // ------------- TPC dE/dx s.t.d 
    // --- Kaon
    auto cStdDEdxKaon_all = new TCanvas("cStdDEdxKaon_all", "cStdDEdxKaon_all", 900, 600);
    cStdDEdxKaon_all->cd();
    auto hStdDEdxKaon_data = (TH1F *)f_data->Get("hStdDEdxKaon");
    auto hStdDEdxKaon_mc = (TH1F *)f_mc->Get("hStdDEdxKaon");
    hStdDEdxKaon_mc->SetTitle(";#sigma^{K^{#pm}}_{dE/dx};Counts (Normalized)");
    hStdDEdxKaon_data->Scale(1./hStdDEdxKaon_data->Integral());
    hStdDEdxKaon_mc->Scale(1./hStdDEdxKaon_mc->Integral());
    hStdDEdxKaon_data->SetMinimum(0.);
    hStdDEdxKaon_mc->SetMinimum(0.);
    hStdDEdxKaon_mc->SetMarkerColor(2);
    hStdDEdxKaon_mc->SetLineColor(2);
    hStdDEdxKaon_mc->Draw("hist");
    hStdDEdxKaon_data->Draw("hist same");
    auto lStdDEdxKaon_all = new TLegend(0.75, 0.75, 0.85, 0.88);
    lStdDEdxKaon_all->SetTextSize(0.03);
    lStdDEdxKaon_all->SetBorderSize(0);
    lStdDEdxKaon_all->AddEntry(hStdDEdxKaon_data, "Data", "l");
    lStdDEdxKaon_all->AddEntry(hStdDEdxKaon_mc, "MC", "l");
    lStdDEdxKaon_all->Draw();
    cStdDEdxKaon_all->Print(Form("%s/cStdDEdxKaon_all.root", outDir.Data()),"root");
    cStdDEdxKaon_all->Print(Form("%s/cStdDEdxKaon_all.pdf", outDir.Data()),"pdf");
    // --- Pion
    auto cStdDEdxPion_all = new TCanvas("cStdDEdxPion_all", "cStdDEdxPion_all", 900, 600);
    cStdDEdxPion_all->cd();
    auto hStdDEdxPion_data = (TH1F *)f_data->Get("hStdDEdxPion");
    auto hStdDEdxPion_mc = (TH1F *)f_mc->Get("hStdDEdxPion");
    hStdDEdxPion_data->SetTitle(";#sigma^{#pi^{#pm}}_{dE/dx};Counts (Normalized)");
    hStdDEdxPion_data->Scale(1./hStdDEdxPion_data->Integral());
    hStdDEdxPion_mc->Scale(1./hStdDEdxPion_mc->Integral());
    hStdDEdxPion_data->SetMinimum(0.);
    hStdDEdxPion_mc->SetMinimum(0.);
    hStdDEdxPion_mc->SetMarkerColor(2);
    hStdDEdxPion_mc->SetLineColor(2);
    hStdDEdxPion_data->Draw("hist");
    hStdDEdxPion_mc->Draw("hist same");
    auto lStdDEdxPion_all = new TLegend(0.75, 0.75, 0.85, 0.88);
    lStdDEdxPion_all->SetTextSize(0.03);
    lStdDEdxPion_all->SetBorderSize(0);
    lStdDEdxPion_all->AddEntry(hStdDEdxPion_data, "Data", "l");
    lStdDEdxPion_all->AddEntry(hStdDEdxPion_mc, "MC", "l");
    lStdDEdxPion_all->Draw();
    cStdDEdxPion_all->Print(Form("%s/cStdDEdxPion_all.root", outDir.Data()),"root");
    cStdDEdxPion_all->Print(Form("%s/cStdDEdxPion_all.pdf", outDir.Data()),"pdf");
    // --- Muon
    auto cStdDEdxMuon_all = new TCanvas("cStdDEdxMuon_all", "cStdDEdxMuon_all", 900, 600);
    cStdDEdxMuon_all->cd();
    auto hStdDEdxMuon_data = (TH1F *)f_data->Get("hStdDEdxMuon");
    auto hStdDEdxMuon_mc = (TH1F *)f_mc->Get("hStdDEdxMuon");
    hStdDEdxMuon_data->SetTitle(";#sigma^{#mu^{#pm}}_{dE/dx};Counts (Normalized)");
    hStdDEdxMuon_data->Scale(1./hStdDEdxMuon_data->Integral());
    hStdDEdxMuon_mc->Scale(1./hStdDEdxMuon_mc->Integral());
    hStdDEdxMuon_data->SetMinimum(0.);
    hStdDEdxMuon_mc->SetMinimum(0.);
    hStdDEdxMuon_mc->SetMarkerColor(2);
    hStdDEdxMuon_mc->SetLineColor(2);
    hStdDEdxMuon_data->Draw("hist");
    hStdDEdxMuon_mc->Draw("hist same");
    auto lStdDEdxMuon_all = new TLegend(0.75, 0.75, 0.85, 0.88);
    lStdDEdxMuon_all->SetTextSize(0.03);
    lStdDEdxMuon_all->SetBorderSize(0);
    lStdDEdxMuon_all->AddEntry(hStdDEdxMuon_data, "Data", "l");
    lStdDEdxMuon_all->AddEntry(hStdDEdxMuon_mc, "MC", "l");
    lStdDEdxMuon_all->Draw();
    cStdDEdxMuon_all->Print(Form("%s/cStdDEdxMuon_all.root", outDir.Data()),"root");
    cStdDEdxMuon_all->Print(Form("%s/cStdDEdxMuon_all.pdf", outDir.Data()),"pdf");
    // --- Electron
    auto cStdDEdxElectron_all = new TCanvas("cStdDEdxElectron_all", "cStdDEdxElectron_all", 900, 600);
    cStdDEdxElectron_all->cd();
    auto hStdDEdxElectron_data = (TH1F *)f_data->Get("hStdDEdxElectron");
    auto hStdDEdxElectron_mc = (TH1F *)f_mc->Get("hStdDEdxElectron");
    hStdDEdxElectron_data->SetTitle(";#sigma^{e^{#pm}}_{dE/dx};Counts (Normalized)");
    hStdDEdxElectron_data->Scale(1./hStdDEdxElectron_data->Integral());
    hStdDEdxElectron_mc->Scale(1./hStdDEdxElectron_mc->Integral());
    hStdDEdxElectron_data->SetMinimum(0.);
    hStdDEdxElectron_mc->SetMinimum(0.);
    hStdDEdxElectron_mc->SetMarkerColor(2);
    hStdDEdxElectron_mc->SetLineColor(2);
    hStdDEdxElectron_data->Draw("hist");
    hStdDEdxElectron_mc->Draw("hist same");
    auto lStdDEdxElectron_all = new TLegend(0.75, 0.75, 0.85, 0.88);
    lStdDEdxElectron_all->SetTextSize(0.03);
    lStdDEdxElectron_all->SetBorderSize(0);
    lStdDEdxElectron_all->AddEntry(hStdDEdxElectron_data, "Data", "l");
    lStdDEdxElectron_all->AddEntry(hStdDEdxElectron_mc, "MC", "l");
    lStdDEdxElectron_all->Draw();
    cStdDEdxElectron_all->Print(Form("%s/cStdDEdxElectron_all.root", outDir.Data()),"root");
    cStdDEdxElectron_all->Print(Form("%s/cStdDEdxElectron_all.pdf", outDir.Data()),"pdf");

    // Luminosity processed per run
    auto cLumiProc = new TCanvas("cLumiProc", "cLumiProc", 4000, 1500);
    cLumiProc->cd();
    auto hLumiProc = (TH1D *)f_lumi->Get("hLumiProc");
    hLumiProc->SetTitle(";Runs;Luminosity scaled [#mub^{-1}]");
    hLumiProc->Draw("hist");
    TLatex l_LumiProc;
    l_LumiProc.SetTextSize(0.04);
    l_LumiProc.SetTextAlign(13); //align at top
    l_LumiProc.SetNDC();
    l_LumiProc.SetTextColor(kBlue);
    l_LumiProc.DrawLatex(0.62, 0.8, "Processed Luminosity = 0.505 #mub^{-1}");
    cLumiProc->Print(Form("%s/cLumiProc.root", outDir.Data()),"root");
    cLumiProc->Print(Form("%s/cLumiProc.pdf", outDir.Data()),"pdf");

    // // Invarian Mass 
    // auto cMassPairKaonUnLikeCharge_all = new TCanvas("cMassPairKaonUnLikeCharge_all", "cMassPairKaonUnLikeCharge_all", 900, 600);
    // cMassPairKaonUnLikeCharge_all->cd();
    // // cMassPairKaonUnLikeCharge_all->SetLogy();
    // auto hMassPairKaonUnLikeCharge_data = (TH1F *)f_data->Get("hMassPairKaonUnLikeCharge");
    // auto hMassPairKaonUnLikeCharge_mc = (TH1F *)f_mc->Get("hMassPairKaonUnLikeCharge");
    // hMassPairKaonUnLikeCharge_data->Scale(1./hMassPairKaonUnLikeCharge_data->Integral(), "width");
    // hMassPairKaonUnLikeCharge_mc->Scale(1./hMassPairKaonUnLikeCharge_mc->Integral(), "width");
    // hMassPairKaonUnLikeCharge_mc->SetMarkerColor(2);
    // hMassPairKaonUnLikeCharge_mc->SetLineColor(2);
    // hMassPairKaonUnLikeCharge_data->Draw("e");
    // hMassPairKaonUnLikeCharge_mc->Draw("esame");
    // auto lMassPairKaonUnLikeCharge_all = new TLegend(0.75, 0.75, 0.85, 0.88);
    // lMassPairKaonUnLikeCharge_all->SetTextSize(0.03);
    // lMassPairKaonUnLikeCharge_all->SetBorderSize(0);
    // lMassPairKaonUnLikeCharge_all->AddEntry(hMassPairKaonUnLikeCharge_data, "Data", "l");
    // lMassPairKaonUnLikeCharge_all->AddEntry(hMassPairKaonUnLikeCharge_mc, "MC", "l");
    // lMassPairKaonUnLikeCharge_all->Draw();
    // hMassPairKaonUnLikeCharge_data->Write("hMassPairKaonUnLikeCharge_data", TObject::kWriteDelete);
    // hMassPairKaonUnLikeCharge_mc->Write("hMassPairKaonUnLikeCharge_mc", TObject::kWriteDelete);
    // cMassPairKaonUnLikeCharge_all->Print(Form("%s/cMassPairKaonUnLikeCharge_all.root", outDir.Data()),"root");
    // cMassPairKaonUnLikeCharge_all->Print(Form("%s/cMassPairKaonUnLikeCharge_all.pdf", outDir.Data()),"pdf");


    // Begin: ====================  Cross section in mass bins ====================

    // Invariant Mass 
    // ------ MC
    auto cMassPairKaon_all = new TCanvas("cMassPairKaon_all", "cMassPairKaon_all", 900, 600);
    cMassPairKaon_all->cd();
    // cMassPairKaon_all->SetLogy();
    auto hMassPairKaon_mc = (TH1F *)f_mc->Get("hMassPairKaon");
    auto hMassPairKaon_UnLikeCharge_mc = (TH1F *)f_mc->Get("hMassPairKaonUnLikeCharge");
    auto hMassPairKaon_dEdxCut_mc = (TH1F *)f_mc->Get("hMassPairKaondEdxCut");
    auto hMassPairKaon_PtCut01_mc = (TH1F *)f_mc->Get("hMassPairKaonPtCut01");
    auto hMassPairKaon_RejectPionSub_mc = (TH1F *)f_mc->Get("hMassPairKaonRejectPionSub");
    auto hMassPairKaon_RejectPionElectronSub_mc = (TH1F *)f_mc->Get("hMassPairKaonRejectPionElectronSub");
    auto hMassPairKaonRejectAllSub_mc = (TH1F *)f_mc->Get("hMassPairKaonRejectAllSub");
    auto hMassPairKaon_truth = (TH1F *)f_mc->Get("hMassPairKaon_truth");
    // hMassPairKaon_mc->Rebin(6);
    // hMassPairKaon_UnLikeCharge_mc->Rebin(6);
    // hMassPairKaon_dEdxCut_mc->Rebin(6);
    // hMassPairKaon_PtCut01_mc->Rebin(6);
    // hMassPairKaon_RejectPionSub_mc->Rebin(2);
    // hMassPairKaon_RejectPionElectronSub_mc->Rebin(2);
    // hMassPairKaonRejectAllSub_mc->Rebin(2);

    // ------ Data
    auto hMassPairKaon_data = (TH1F *)f_data->Get("hMassPairKaon");
    auto hMassPairKaon_UnLikeCharge_data = (TH1F *)f_data->Get("hMassPairKaonUnLikeCharge");
    auto hMassPairKaon_dEdxCut_data = (TH1F *)f_data->Get("hMassPairKaondEdxCut");
    auto hMassPairKaon_PtCut01_data = (TH1F *)f_data->Get("hMassPairKaonPtCut01");
    auto hMassPairKaon_RejectPionSub_data = (TH1F *)f_data->Get("hMassPairKaonRejectPionSub");
    auto hMassPairKaon_RejectPionElectronSub_data = (TH1F *)f_data->Get("hMassPairKaonRejectPionElectronSub");
    auto hMassPairKaonRejectAllSub_data = (TH1F *)f_data->Get("hMassPairKaonRejectAllSub");
    auto hPtPairKaonRejectAllSub_data = (TH1F *)f_data->Get("hPtPairKaonRejectAllSub");
    // hMassPairKaon_data->Rebin(6);
    // hMassPairKaon_UnLikeCharge_data->Rebin(6);
    // hMassPairKaon_dEdxCut_data->Rebin(6);
    // hMassPairKaon_PtCut01_data->Rebin(6);
    // hMassPairKaon_RejectPionSub_data->Rebin(2);
    // hMassPairKaon_RejectPionElectronSub_data->Rebin(2);
    // hMassPairKaonRejectAllSub_data->Rebin(2);

    printf("nbins_mc = %i, nbins_UnLikeCharge_mc = %i, nbins_dEdxCut_mc = %i, nbins_PtCut01_mc = %i, nbins_RejectPionSub_mc = %i, nbins_RejectPionElectronSub_mc = %i, nbins_RejectAllSub_mc = %i \n", hMassPairKaon_mc->GetNbinsX(), hMassPairKaon_UnLikeCharge_mc->GetNbinsX(), hMassPairKaon_dEdxCut_mc->GetNbinsX(), hMassPairKaon_PtCut01_mc->GetNbinsX(), hMassPairKaon_RejectPionSub_mc->GetNbinsX(), hMassPairKaon_RejectPionElectronSub_mc->GetNbinsX(), hMassPairKaonRejectAllSub_mc->GetNbinsX());
 
    hMassPairKaonRejectAllSub_mc->SetMarkerColor(2);
    hMassPairKaonRejectAllSub_mc->SetLineColor(2);
    hMassPairKaon_truth->SetMarkerColor(1);
    hMassPairKaon_truth->SetLineColor(1);
    hMassPairKaon_truth->Draw("e");
    hMassPairKaonRejectAllSub_mc->Draw("esame");
    hMassPairKaonRejectAllSub_data->Draw("esame");
    auto lMassPairKaon_all = new TLegend(0.75, 0.75, 0.85, 0.88);
    lMassPairKaon_all->SetTextSize(0.03);
    lMassPairKaon_all->SetBorderSize(0);
    lMassPairKaon_all->AddEntry(hMassPairKaonRejectAllSub_data, "Data", "l");
    lMassPairKaon_all->AddEntry(hMassPairKaonRejectAllSub_mc, "MC", "l");
    lMassPairKaon_all->AddEntry(hMassPairKaon_truth, "Generated", "l");
    lMassPairKaon_all->Draw();
    hMassPairKaonRejectAllSub_data->Write("hMassPairKaonRejectAllSub_data", TObject::kWriteDelete);
    hMassPairKaonRejectAllSub_mc->Write("hMassPairKaonRejectAllSub_mc", TObject::kWriteDelete);
    hMassPairKaon_truth->Write("hMassPairKaon_truth", TObject::kWriteDelete);
    cMassPairKaon_all->Print(Form("%s/cMassPairKaon_all.root", outDir.Data()),"root");
    cMassPairKaon_all->Print(Form("%s/cMassPairKaon_all.pdf", outDir.Data()),"pdf");

    // Yields
    // ---- MC
    TGraphErrors *gNsig_mc = new TGraphErrors();
    TGraphErrors *gNsig_UnLikeCharge_mc = new TGraphErrors();
    TGraphErrors *gNsig_dEdxCut_mc = new TGraphErrors();
    TGraphErrors *gNsig_PtCut01_mc = new TGraphErrors();
    TGraphErrors *gNsig_RejectPionSub_mc = new TGraphErrors();
    TGraphErrors *gNsig_RejectPionElectronSub_mc = new TGraphErrors();
    TGraphErrors *gNsig_RejectAllSub_mc = new TGraphErrors();
    gNsig_mc->SetMarkerStyle(kOpenSquare);
    gNsig_UnLikeCharge_mc->SetMarkerStyle(20);
    gNsig_dEdxCut_mc->SetMarkerStyle(20);
    gNsig_PtCut01_mc->SetMarkerStyle(20);
    gNsig_RejectPionSub_mc->SetMarkerStyle(20);
    gNsig_RejectPionElectronSub_mc->SetMarkerStyle(20);
    gNsig_RejectAllSub_mc->SetMarkerStyle(20);
    gNsig_mc->SetMarkerColor(kBlack);
    gNsig_UnLikeCharge_mc->SetMarkerColor(28);
    gNsig_dEdxCut_mc->SetMarkerColor(40);
    gNsig_PtCut01_mc->SetMarkerColor(kRed);
    gNsig_RejectPionSub_mc->SetMarkerColor(kGreen);
    gNsig_RejectPionElectronSub_mc->SetMarkerColor(kMagenta);
    gNsig_RejectAllSub_mc->SetMarkerColor(kBlue);
    gNsig_mc->SetMinimum(0.0);
    gNsig_mc->SetTitle(";m_{K^{+}K^{-}} (GeV/c^{2});Yield");

    // ---- Data
    TGraphErrors *gNsig_data = new TGraphErrors();
    TGraphErrors *gNsig_UnLikeCharge_data = new TGraphErrors();
    TGraphErrors *gNsig_dEdxCut_data = new TGraphErrors();
    TGraphErrors *gNsig_PtCut01_data = new TGraphErrors();
    TGraphErrors *gNsig_RejectPionSub_data = new TGraphErrors();
    TGraphErrors *gNsig_RejectPionElectronSub_data = new TGraphErrors();
    TGraphErrors *gNsig_RejectAllSub_data = new TGraphErrors();
    gNsig_data->SetMarkerStyle(kOpenSquare);
    gNsig_UnLikeCharge_data->SetMarkerStyle(20);
    gNsig_dEdxCut_data->SetMarkerStyle(20);
    gNsig_PtCut01_data->SetMarkerStyle(20);
    gNsig_RejectPionSub_data->SetMarkerStyle(20);
    gNsig_RejectPionElectronSub_data->SetMarkerStyle(20);
    gNsig_RejectAllSub_data->SetMarkerStyle(20);
    gNsig_data->SetMarkerColor(kBlack);
    gNsig_UnLikeCharge_data->SetMarkerColor(28);
    gNsig_dEdxCut_data->SetMarkerColor(40);
    gNsig_PtCut01_data->SetMarkerColor(kRed);
    gNsig_RejectPionSub_data->SetMarkerColor(kGreen);
    gNsig_RejectPionElectronSub_data->SetMarkerColor(kMagenta);
    gNsig_RejectAllSub_data->SetMarkerColor(kBlue);
    gNsig_data->SetMinimum(0.0);
    gNsig_data->SetTitle(";m_{K^{+}K^{-}} (GeV/c^{2});Yield");
    gNsig_RejectAllSub_data->SetTitle(";m_{K^{+}K^{-}} (GeV/c^{2});Yield");

    // Efficiency
    TGraphErrors *gEff = new TGraphErrors();
    TGraphErrors *gEff_UnLikeCharge = new TGraphErrors();
    TGraphErrors *gEff_dEdxCut = new TGraphErrors();
    TGraphErrors *gEff_PtCut01 = new TGraphErrors();
    TGraphErrors *gEff_RejectPionSub = new TGraphErrors();
    TGraphErrors *gEff_RejectPionElectronSub = new TGraphErrors();
    TGraphErrors *gEff_RejectAllSub = new TGraphErrors();
    gEff->SetMarkerStyle(kOpenSquare);
    gEff_UnLikeCharge->SetMarkerStyle(20);
    gEff_dEdxCut->SetMarkerStyle(20);
    gEff_PtCut01->SetMarkerStyle(20);
    gEff_RejectPionSub->SetMarkerStyle(20);
    gEff_RejectPionElectronSub->SetMarkerStyle(20);
    gEff_RejectAllSub->SetMarkerStyle(20);
    gEff->SetMarkerColor(kBlack);
    gEff_UnLikeCharge->SetMarkerColor(28);
    gEff_dEdxCut->SetMarkerColor(40);
    gEff_PtCut01->SetMarkerColor(kRed);
    gEff_RejectPionSub->SetMarkerColor(kGreen);
    gEff_RejectPionElectronSub->SetMarkerColor(kMagenta);
    gEff_RejectAllSub->SetMarkerColor(kBlue);
    gEff->SetMinimum(0.0);
    gEff->SetTitle(";m_{K^{+}K^{-}} (GeV/c^{2});Efficiency (%)");

    // Cross Section
    TGraphErrors *gkpkmXSection = new TGraphErrors();
    gkpkmXSection->SetMarkerStyle(20);
    // gkpkmXSection->SetMarkerSize(1.5);
    gkpkmXSection->SetMarkerColor(kBlue);
    gkpkmXSection->SetMinimum(0.0);
    gkpkmXSection->SetTitle("; m_{K^{+}K^{-}} (GeV/c^{2}); Cross Section (mb/GeV)");

    // Pair Pt after Bkg sub
    TGraphErrors *gPairPtsub = new TGraphErrors();
    gPairPtsub->SetMarkerStyle(20);
    // gPairPtsub->SetMarkerSize(1.5);
    gPairPtsub->SetMarkerColor(kBlue);
    gPairPtsub->SetMinimum(0.0);
    gPairPtsub->SetTitle("; Pair P_{T} (GeV/c); Yield");

    // residual
    TGraph *gresidual = new TGraph();
    gresidual->SetMarkerStyle(20);
    gresidual->SetMarkerColor(kBlue);
    gresidual->SetMinimum(0.0);
    gresidual->SetTitle("; m_{K^{+}K^{-}} (GeV/c^{2}); N_{Truth} - N_{Reco} (GeV/c^{2})");

    // -------- TPC dE/dx mean and width variations

    auto cEffVar = new TCanvas("cEffVar", "cEffVar", 900, 600);
    cEffVar->cd();
    auto hacceff_mkk_0 = (TH1D *)f_effvar->Get("hacceff_mkk_0");
    auto hacceff_mkk_1 = (TH1D *)f_effvar->Get("hacceff_mkk_1");
    auto hacceff_mkk_2 = (TH1D *)f_effvar->Get("hacceff_mkk_2");
    auto hacceff_mkk_3 = (TH1D *)f_effvar->Get("hacceff_mkk_3");
    auto hacceff_mkk_4 = (TH1D *)f_effvar->Get("hacceff_mkk_4");

    // hacceff_mkk_0->Rebin(2);
    // hacceff_mkk_1->Rebin(2);
    // hacceff_mkk_2->Rebin(2);
    // hacceff_mkk_3->Rebin(2);
    // hacceff_mkk_4->Rebin(2);

    printf("hacceff_mkk_0 = %i, hacceff_mkk_1 = %i, hacceff_mkk_2 = %i, hacceff_mkk_3 = %i, hacceff_mkk_4 = %i \n", hacceff_mkk_0->GetNbinsX(), hacceff_mkk_1->GetNbinsX(), hacceff_mkk_2->GetNbinsX(), hacceff_mkk_3->GetNbinsX(), hacceff_mkk_4->GetNbinsX());
 
    hacceff_mkk_0->SetMarkerColor(1);
    hacceff_mkk_0->SetLineColor(1);
    hacceff_mkk_1->SetMarkerColor(2);
    hacceff_mkk_1->SetMarkerStyle(kOpenSquare);
    hacceff_mkk_2->SetMarkerColor(4);
    hacceff_mkk_2->SetMarkerStyle(kFullSquare);
    hacceff_mkk_3->SetMarkerColor(6);
    hacceff_mkk_3->SetMarkerStyle(kOpenCircle);
    hacceff_mkk_4->SetMarkerColor(8);
    hacceff_mkk_4->SetMarkerStyle(kFullCircle);
    hacceff_mkk_0->Draw("e");
    hacceff_mkk_1->Draw("esame");
    hacceff_mkk_2->Draw("esame");
    hacceff_mkk_3->Draw("esame");
    hacceff_mkk_4->Draw("esame");
    auto l_effvar = new TLegend(0.65, 0.70, 0.75, 0.85);
    l_effvar->SetTextSize(0.03);
    l_effvar->SetBorderSize(0);
    l_effvar->AddEntry(hacceff_mkk_0, "base", "p");
    l_effvar->AddEntry(hacceff_mkk_1, "<dE/dx> + 3%", "p");
    l_effvar->AddEntry(hacceff_mkk_2, "<dE/dx> - 3%", "p");
    l_effvar->AddEntry(hacceff_mkk_3, "#sigma_{dE/dx} + 3%", "p");
    l_effvar->AddEntry(hacceff_mkk_4, "#sigma_{dE/dx} - 3%", "p");
    l_effvar->Draw();
    hacceff_mkk_0->Write("hacceff_mkk_0", TObject::kWriteDelete);
    hacceff_mkk_1->Write("hacceff_mkk_1", TObject::kWriteDelete);
    hacceff_mkk_2->Write("hacceff_mkk_2", TObject::kWriteDelete);
    hacceff_mkk_3->Write("hacceff_mkk_3", TObject::kWriteDelete);
    hacceff_mkk_4->Write("hacceff_mkk_4", TObject::kWriteDelete);
    cEffVar->Print(Form("%s/cEffVar.root", outDir.Data()),"root");
    cEffVar->Print(Form("%s/cEffVar.pdf", outDir.Data()),"pdf");

    // =================== Systematic Errors ====================

    const Int_t n_m2k = hMassPairKaonRejectAllSub_mc->GetNbinsX();
    Double_t x_m2k[n_m2k], y_m2k[n_m2k], exl_m2k[n_m2k], exh_m2k[n_m2k], eylstat_m2k[n_m2k], eyhstat_m2k[n_m2k], eylsys_m2k[n_m2k], eyhsys_m2k[n_m2k];

    // --- cross section for diff dE/dx variation
    auto hxsecdEdxVar_0  = new TProfile("hxsecdEdxVar_0","; m_{K^{+}K^{-}} (GeV/c^{2});d#sigma/dm (mb/GeV)",n_m2k,1.1,1.6,-500,500);
    auto hxsecdEdxVar_1  = new TProfile("hxsecdEdxVar_1","; m_{K^{+}K^{-}} (GeV/c^{2});d#sigma/dm (mb/GeV)",n_m2k,1.1,1.6,-500,500);
    auto hxsecdEdxVar_2  = new TProfile("hxsecdEdxVar_2","; m_{K^{+}K^{-}} (GeV/c^{2});d#sigma/dm (mb/GeV)",n_m2k,1.1,1.6,-500,500);
    auto hxsecdEdxVar_3  = new TProfile("hxsecdEdxVar_3","; m_{K^{+}K^{-}} (GeV/c^{2});d#sigma/dm (mb/GeV)",n_m2k,1.1,1.6,-500,500);
    auto hxsecdEdxVar_4  = new TProfile("hxsecdEdxVar_4","; m_{K^{+}K^{-}} (GeV/c^{2});d#sigma/dm (mb/GeV)",n_m2k,1.1,1.6,-500,500);

    // --- Relative error of PID selection
    auto hEffVarRatio_1  = new TProfile("hEffVarRatio_1","; m_{K^{+}K^{-}} (GeV/c^{2}); rel. diff.",n_m2k,1.1,1.6,-20,20);
    auto hEffVarRatio_2  = new TProfile("hEffVarRatio_2","; m_{K^{+}K^{-}} (GeV/c^{2}); rel. diff.",n_m2k,1.1,1.6,-20,20);
    auto hEffVarRatio_3  = new TProfile("hEffVarRatio_3","; m_{K^{+}K^{-}} (GeV/c^{2}); rel. diff.",n_m2k,1.1,1.6,-20,20);
    auto hEffVarRatio_4  = new TProfile("hEffVarRatio_4","; m_{K^{+}K^{-}} (GeV/c^{2}); rel. diff.",n_m2k,1.1,1.6,-20,20);

    double xsec_optimal[]              = {23.821, 5.533, 1.558}; // rad: 9; reject: 2
    double xsec_rad8[]                 = {23.852, 5.537, 1.560}; // regions: 8
    double xsec_rad10[]                = {23.791, 5.531, 1.557}; // regions: 10
    double xsec_rejectLessStd[]        = {23.636, 4.638, 1.492}; // 1.75
    double xsec_rejectMoreStd[]        = {24.008, 5.753, 1.631}; // 2.25

    // all
    // double err_xsec_dEdx_mean[5] = {0.012381, 0.072927, 0.134824, 0.045944, 0.013546}; 
    // double err_xsec_dEdx_width[5] = {0.000960, 0.007780, 0.061392, 0.043450, 0.010984};

    // without muon reject
    double err_xsec_dEdx_mean[5] = {0.012381, 0.072927, 0.145745, 0.030308, 0.056658}; 
    double err_xsec_dEdx_width[5] = {0.000960, 0.007780, 0.075684, 0.032077, 0.029141};

    for (Int_t i = 0; i < n_m2k; ++i)
    {
        double eff = 0, deff = 0, eff_UnLikeCharge = 0, deff_UnLikeCharge = 0, eff_dEdxCut = 0, deff_dEdxCut = 0, eff_PtCut01 = 0, deff_PtCut01 = 0, eff_RejectPionSub = 0, deff_RejectPionSub = 0, eff_RejectPionElectronSub = 0, deff_RejectPionElectronSub = 0, eff_RejectAllSub = 0, deff_RejectAllSub = 0, xsec_kpkm = 0, dxsec_kpkm_stat = 0;
        double lumi_kpkm = 419; // milli barn^{-1}, 505, 0.418779
        double fpileup = 1.111; // from Rho(770) analysis
        double Nsig_truth = hMassPairKaon_truth->GetBinContent(i+1);
        double kpkmMass = hMassPairKaonRejectAllSub_mc->GetXaxis()->GetBinCenter(i+1);
        double dm = hMassPairKaonRejectAllSub_data->GetXaxis()->GetBinWidth(i+1);

        // --- MC
        double Nsig_mc = hMassPairKaon_mc->GetBinContent(i+1);
        double Nsig_UnLikeCharge_mc = hMassPairKaon_UnLikeCharge_mc->GetBinContent(i+1);
        double Nsig_dEdxCut_mc = hMassPairKaon_dEdxCut_mc->GetBinContent(i+1);
        double Nsig_PtCut01_mc = hMassPairKaon_PtCut01_mc->GetBinContent(i+1);
        double Nsig_RejectPionSub_mc = hMassPairKaon_RejectPionSub_mc->GetBinContent(i+1);
        double Nsig_RejectPionElectronSub_mc = hMassPairKaon_RejectPionElectronSub_mc->GetBinContent(i+1);
        double Nsig_RejectAllSub_mc = hMassPairKaonRejectAllSub_mc->GetBinContent(i+1);
        double dNsig_mc = hMassPairKaon_mc->GetBinError(i+1);
        double dNsig_UnLikeCharge_mc = hMassPairKaon_UnLikeCharge_mc->GetBinError(i+1);
        double dNsig_dEdxCut_mc = hMassPairKaon_dEdxCut_mc->GetBinError(i+1);
        double dNsig_PtCut01_mc = hMassPairKaon_PtCut01_mc->GetBinError(i+1);
        double dNsig_RejectPionSub_mc = hMassPairKaon_RejectPionSub_mc->GetBinError(i+1);
        double dNsig_RejectPionElectronSub_mc = hMassPairKaon_RejectPionElectronSub_mc->GetBinError(i+1);
        double dNsig_RejectAllSub_mc = hMassPairKaonRejectAllSub_mc->GetBinError(i+1);
        double diffMcTruth = Nsig_truth - Nsig_RejectAllSub_mc;

        // --- data
        double Nsig_data = hMassPairKaon_data->GetBinContent(i+1);
        double Nsig_UnLikeCharge_data = hMassPairKaon_UnLikeCharge_data->GetBinContent(i+1);
        double Nsig_dEdxCut_data = hMassPairKaon_dEdxCut_data->GetBinContent(i+1);
        double Nsig_PtCut01_data = hMassPairKaon_PtCut01_data->GetBinContent(i+1);
        double Nsig_RejectPionSub_data = hMassPairKaon_RejectPionSub_data->GetBinContent(i+1);
        double Nsig_RejectPionElectronSub_data = hMassPairKaon_RejectPionElectronSub_data->GetBinContent(i+1);
        double Nsig_RejectAllSub_data = hMassPairKaonRejectAllSub_data->GetBinContent(i+1);
        double Nsig_RejectAllSub_Pt_data = hPtPairKaonRejectAllSub_data->GetBinContent(i+1);
        double dNsig_data = hMassPairKaon_data->GetBinError(i+1);
        double dNsig_UnLikeCharge_data = hMassPairKaon_UnLikeCharge_data->GetBinError(i+1);
        double dNsig_dEdxCut_data = hMassPairKaon_dEdxCut_data->GetBinError(i+1);
        double dNsig_PtCut01_data = hMassPairKaon_PtCut01_data->GetBinError(i+1);
        double dNsig_RejectPionSub_data = hMassPairKaon_RejectPionSub_data->GetBinError(i+1);
        double dNsig_RejectPionElectronSub_data = hMassPairKaon_RejectPionElectronSub_data->GetBinError(i+1);
        double dNsig_RejectAllSub_data = hMassPairKaonRejectAllSub_data->GetBinError(i+1);
        double dNsig_RejectAllSub_Pt_data = hPtPairKaonRejectAllSub_data->GetBinError(i+1);

        // --- efficiency after TPC dE/dx variation
        double effVar_0 = hacceff_mkk_0->GetBinContent(i+1);
        double effVar_1 = hacceff_mkk_1->GetBinContent(i+1);
        double effVar_2 = hacceff_mkk_2->GetBinContent(i+1);
        double effVar_3 = hacceff_mkk_3->GetBinContent(i+1);
        double effVar_4 = hacceff_mkk_4->GetBinContent(i+1);

        double xsec_effVar_0 = (Nsig_RejectAllSub_data * fpileup) / (effVar_0 * lumi_kpkm * dm);
        double xsec_effVar_1 = (Nsig_RejectAllSub_data * fpileup) / (effVar_1 * lumi_kpkm * dm);
        double xsec_effVar_2 = (Nsig_RejectAllSub_data * fpileup) / (effVar_2 * lumi_kpkm * dm);
        double xsec_effVar_3 = (Nsig_RejectAllSub_data * fpileup) / (effVar_3 * lumi_kpkm * dm);
        double xsec_effVar_4 = (Nsig_RejectAllSub_data * fpileup) / (effVar_4 * lumi_kpkm * dm);

        hxsecdEdxVar_0->Fill(kpkmMass, xsec_effVar_0);
        hxsecdEdxVar_1->Fill(kpkmMass, xsec_effVar_1);
        hxsecdEdxVar_2->Fill(kpkmMass, xsec_effVar_2);
        hxsecdEdxVar_3->Fill(kpkmMass, xsec_effVar_3);
        hxsecdEdxVar_4->Fill(kpkmMass, xsec_effVar_4);

        double xsec_effVarRatio_1 = (xsec_effVar_1 - xsec_effVar_0)/xsec_effVar_0;
        double xsec_effVarRatio_2 = (xsec_effVar_2 - xsec_effVar_0)/xsec_effVar_0;
        double xsec_effVarRatio_3 = (xsec_effVar_3 - xsec_effVar_0)/xsec_effVar_0;
        double xsec_effVarRatio_4 = (xsec_effVar_4 - xsec_effVar_0)/xsec_effVar_0;

        hEffVarRatio_1->Fill(kpkmMass, xsec_effVarRatio_1);
        hEffVarRatio_2->Fill(kpkmMass, xsec_effVarRatio_2);
        hEffVarRatio_3->Fill(kpkmMass, xsec_effVarRatio_3);
        hEffVarRatio_4->Fill(kpkmMass, xsec_effVarRatio_4);

        if(Nsig_truth != 0)
        {
            if(Nsig_mc != 0)
            {
                eff = Nsig_mc / Nsig_truth; // Efficiency = N_observed/N_generated
                deff = eff * (dNsig_mc / Nsig_mc);
            }
            if(Nsig_UnLikeCharge_mc != 0)
            {
                eff_UnLikeCharge = Nsig_UnLikeCharge_mc / Nsig_truth; // Efficiency = N_observed/N_generated
                deff_UnLikeCharge = eff_UnLikeCharge * (dNsig_UnLikeCharge_mc / Nsig_UnLikeCharge_mc);
            }
            if(Nsig_dEdxCut_mc != 0)
            {
                eff_dEdxCut = Nsig_dEdxCut_mc / Nsig_truth; // Efficiency = N_observed/N_generated
                deff_dEdxCut = eff_dEdxCut * (dNsig_dEdxCut_mc / Nsig_dEdxCut_mc);
            }
            if(Nsig_PtCut01_mc != 0)
            {
                eff_PtCut01 = Nsig_PtCut01_mc / Nsig_truth;
                deff_PtCut01 = eff_PtCut01 * (dNsig_PtCut01_mc / Nsig_PtCut01_mc);
            }
            if (Nsig_RejectPionSub_mc != 0)
            {
                eff_RejectPionSub = Nsig_RejectPionSub_mc / Nsig_truth;
                deff_RejectPionSub = eff_RejectPionSub * (dNsig_RejectPionSub_mc / Nsig_RejectPionSub_mc);
            }
            if (Nsig_RejectPionElectronSub_mc != 0)
            {
                eff_RejectPionElectronSub = Nsig_RejectPionElectronSub_mc / Nsig_truth;
                deff_RejectPionElectronSub = eff_RejectPionElectronSub * (dNsig_RejectPionElectronSub_mc / Nsig_RejectPionElectronSub_mc);
            }
            if (Nsig_RejectAllSub_mc != 0)
            {
                eff_RejectAllSub = Nsig_RejectAllSub_mc / Nsig_truth;
                deff_RejectAllSub = eff_RejectAllSub * (dNsig_RejectAllSub_mc / Nsig_RejectAllSub_mc);
            }

            // if(eff_RejectAllSub != 0 && Nsig_RejectAllSub_data != 0)
            // {
            //     xsec_kpkm = (Nsig_RejectAllSub_data * fpileup) / (eff_RejectAllSub * lumi_kpkm);
            //     dxsec_kpkm_stat = xsec_kpkm * (dNsig_RejectAllSub_data / Nsig_RejectAllSub_data);
            // }
            // else
            // {
            //     xsec_kpkm = 0;
            //     dxsec_kpkm_stat = 0;
            // }

        }

        double ymin = hMassPairKaonRejectAllSub_mc->GetXaxis()->GetBinLowEdge(i+1);
        double ymax = hMassPairKaonRejectAllSub_mc->GetXaxis()->GetBinUpEdge(i+1);
        // hMassPairKaon_py_truth->SetTitle(Form("%.2f<y<%.2f;m_{K^{+}K^{-}} (GeV/c^{2});Counts",ymin,ymax));
        // hMassPairKaon_py_mc->SetTitle(Form("%.2f<y<%.2f;m_{K^{+}K^{-}} (GeV/c^{2});Counts",ymin,ymax));
        // hMassPairKaon_py_data->SetTitle(Form("%.2f<y<%.2f;m_{K^{+}K^{-}} (GeV/c^{2});Counts",ymin,ymax));

        // For all histogram types: nbins, xlow, xup
        // bin = 0;       underflow bin
        // bin = 1;       first bin with low-edge xlow INCLUDED
        // bin = nbins;   last bin with upper-edge xup EXCLUDED
        // bin = nbins+1; overflow bin

        // eff = Nsig_mc / Nsig_truth; // Efficiency = N_observed/N_generated
        // deff = eff * (dNsig_mc / Nsig_mc);

        // =================== Systematic Errors ====================
        double xsec_rad[]      = {xsec_optimal[i], xsec_rad8[i], xsec_rad10[i]};
        double xsec_reject[]   = {xsec_optimal[i], xsec_rejectMoreStd[i], xsec_rejectLessStd[i]};
        double err_xsec_rad    = TMath::RMS(3, xsec_rad)/xsec_optimal[i];
        double err_xsec_reject = TMath::RMS(3, xsec_reject)/xsec_optimal[i];

        // from rho0 analysis
        double err_trk_select  = 0.015;
        double err_trk_match  = 0.040;
        double err_trk_eff  = 0.010;
        double err_incoh  = 0.005;
        double err_SPD_chip  = 0.010;
        double err_PileUp  = 0.038;
        double err_Lumi  = 0.050;

        double err_sys = TMath::Sqrt(err_xsec_dEdx_mean[i]*err_xsec_dEdx_mean[i] + err_xsec_dEdx_width[i]*err_xsec_dEdx_width[i] + err_trk_select*err_trk_select + err_trk_match*err_trk_match + err_trk_eff*err_trk_eff + err_incoh*err_incoh + err_SPD_chip*err_SPD_chip + err_PileUp*err_PileUp + err_Lumi*err_Lumi);

        xsec_kpkm = (Nsig_RejectAllSub_data * fpileup) / (eff_RejectAllSub * lumi_kpkm * dm);
        dxsec_kpkm_stat = xsec_kpkm * (dNsig_RejectAllSub_data / Nsig_RejectAllSub_data);
        double dxsec_phi2pi_sys = xsec_kpkm * TMath::Sqrt((err_sys*err_sys));

        // x_m2k[i] = {-999};
        // y_m2k[i] = {-999};

        // if(xsec_kpkm == 0) continue;

        x_m2k[i]       = kpkmMass;
        y_m2k[i]       = xsec_kpkm;
        exl_m2k[i]     = hMassPairKaonRejectAllSub_mc->GetXaxis()->GetBinWidth(i+1)/2;
        exh_m2k[i]     = hMassPairKaonRejectAllSub_mc->GetXaxis()->GetBinWidth(i+1)/2;
        eylstat_m2k[i] = dxsec_kpkm_stat;
        eyhstat_m2k[i] = dxsec_kpkm_stat;
        eylsys_m2k[i]  = dxsec_phi2pi_sys;
        eyhsys_m2k[i]  = dxsec_phi2pi_sys;

        fprintf(tab_sys_m, "(%0.2f , %0.2f) & %0.f & %0.f & %0.f & %0.2f$\\pm$%0.2f$\\pm$%0.2f & %0.2f & %0.2f \\\\ \n", ymin, ymax, Nsig_truth, Nsig_RejectAllSub_mc, Nsig_RejectAllSub_data, xsec_kpkm, dxsec_kpkm_stat, dxsec_phi2pi_sys, err_xsec_dEdx_mean[i] * 100, err_xsec_dEdx_width[i] * 100);

        // fprintf(tab_eff, "(%0.2f , %0.2f) & %0.f & %0.f & %0.f & %0.2f$\\pm$%0.2f$\\pm$%0.2f & %0.2f & %0.2f \\\\ \n", ymin, ymax, Nsig_truth, Nsig_RejectAllSub_mc, Nsig_RejectAllSub_data, xsec_kpkm, dxsec_kpkm_stat, dxsec_phi2pi_sys, err_xsec_rad * 100, err_xsec_reject * 100);

        // efficiency
        gEff->SetPoint(i, kpkmMass, eff * 100);
        gEff_UnLikeCharge->SetPoint(i, kpkmMass, eff_UnLikeCharge * 100);
        gEff_dEdxCut->SetPoint(i, kpkmMass, eff_dEdxCut * 100);
        gEff_PtCut01->SetPoint(i, kpkmMass, eff_PtCut01 * 100);
        gEff_RejectPionSub->SetPoint(i, kpkmMass, eff_RejectPionSub * 100);
        gEff_RejectPionElectronSub->SetPoint(i, kpkmMass, eff_RejectPionElectronSub * 100);
        gEff_RejectAllSub->SetPoint(i, kpkmMass, eff_RejectAllSub * 100);
        gEff->SetPointError(i, 0, deff * 100);
        gEff_UnLikeCharge->SetPointError(i, 0, deff_UnLikeCharge * 100);
        gEff_dEdxCut->SetPointError(i, 0, deff_dEdxCut * 100);
        gEff_PtCut01->SetPointError(i, 0, deff_PtCut01 * 100);
        gEff_RejectPionSub->SetPointError(i, 0, deff_RejectPionSub * 100);
        gEff_RejectPionElectronSub->SetPointError(i, 0, deff_RejectPionElectronSub * 100);
        gEff_RejectAllSub->SetPointError(i, 0, deff_RejectAllSub * 100);

        // Yield
        // ---- MC
        gNsig_mc->SetPoint(i, kpkmMass, Nsig_mc);
        gNsig_UnLikeCharge_mc->SetPoint(i, kpkmMass, Nsig_UnLikeCharge_mc);
        gNsig_dEdxCut_mc->SetPoint(i, kpkmMass, Nsig_dEdxCut_mc);
        gNsig_PtCut01_mc->SetPoint(i, kpkmMass, Nsig_PtCut01_mc);
        gNsig_RejectPionSub_mc->SetPoint(i, kpkmMass, Nsig_RejectPionSub_mc);
        gNsig_RejectPionElectronSub_mc->SetPoint(i, kpkmMass, Nsig_RejectPionElectronSub_mc);
        gNsig_RejectAllSub_mc->SetPoint(i, kpkmMass, Nsig_RejectAllSub_mc);
        gNsig_mc->SetPointError(i, 0, dNsig_mc);
        gNsig_UnLikeCharge_mc->SetPointError(i, 0, dNsig_UnLikeCharge_mc);
        gNsig_dEdxCut_mc->SetPointError(i, 0, dNsig_dEdxCut_mc);
        gNsig_PtCut01_mc->SetPointError(i, 0, dNsig_PtCut01_mc);
        gNsig_RejectPionSub_mc->SetPointError(i, 0, dNsig_RejectPionSub_mc);
        gNsig_RejectPionElectronSub_mc->SetPointError(i, 0, dNsig_RejectPionElectronSub_mc);
        gNsig_RejectAllSub_mc->SetPointError(i, 0, dNsig_RejectAllSub_mc);

        // ---- Data
        gNsig_data->SetPoint(i, kpkmMass, Nsig_data);
        gNsig_UnLikeCharge_data->SetPoint(i, kpkmMass, Nsig_UnLikeCharge_data);
        gNsig_dEdxCut_data->SetPoint(i, kpkmMass, Nsig_dEdxCut_data);
        gNsig_PtCut01_data->SetPoint(i, kpkmMass, Nsig_PtCut01_data);
        gNsig_RejectPionSub_data->SetPoint(i, kpkmMass, Nsig_RejectPionSub_data);
        gNsig_RejectPionElectronSub_data->SetPoint(i, kpkmMass, Nsig_RejectPionElectronSub_data);
        gNsig_RejectAllSub_data->SetPoint(i, kpkmMass, Nsig_RejectAllSub_data);
        gNsig_data->SetPointError(i, 0, dNsig_data);
        gNsig_UnLikeCharge_data->SetPointError(i, 0, dNsig_UnLikeCharge_data);
        gNsig_dEdxCut_data->SetPointError(i, 0, dNsig_dEdxCut_data);
        gNsig_PtCut01_data->SetPointError(i, 0, dNsig_PtCut01_data);
        gNsig_RejectPionSub_data->SetPointError(i, 0, dNsig_RejectPionSub_data);
        gNsig_RejectPionElectronSub_data->SetPointError(i, 0, dNsig_RejectPionElectronSub_data);
        gNsig_RejectAllSub_data->SetPointError(i, 0, dNsig_RejectAllSub_data);

        gPairPtsub->SetPoint(i, kpkmMass, Nsig_RejectAllSub_Pt_data);
        gPairPtsub->SetPointError(i, 0, dNsig_RejectAllSub_Pt_data);

        gkpkmXSection->SetPoint(i, kpkmMass, xsec_kpkm);
        gkpkmXSection->SetPointError(i, 0, dxsec_kpkm_stat);

        gresidual->SetPoint(i, kpkmMass, diffMcTruth);

        // printf("i = %d, ymin = %f, ymax = %f, Nsig_RejectAllSub_data = %f, eff_RejectAllSub = %f, x_m2k = %f, y_m2k = %f, eylstat_m2k = %f, eylsys_m2k = %f \n", i, ymin, ymax, Nsig_RejectAllSub_data, eff_RejectAllSub, x_m2k[i], y_m2k[i], eylstat_m2k[i], eylsys_m2k[i]);

        printf(" ####### i, ymin, ymaxt = %d, %0.3f , %0.3f \n", i, ymin, ymax);
        printf(" ####### xsec_effVar = %0.3f, %0.3f, %0.3f, %0.3f, %0.3f \n", xsec_effVar_0, xsec_effVar_1, xsec_effVar_2, xsec_effVar_3, xsec_effVar_4);
        // printf(" ####### dxsec_kpkm_stat = %0.3f \n", dxsec_kpkm_stat);
        // printf(" ####### dxsec_phi2pi_sys = %0.3f \n", dxsec_phi2pi_sys);
        // printf(" ####### i, dm = %d, %f\n", i, dm);
    }

    // --- TPC dE/dx systematics
    auto cEffVarRatio = new TCanvas("cEffVarRatio", "cEffVarRatio", 900, 600);
    cEffVarRatio->cd();
    hEffVarRatio_1->SetMarkerColor(2);
    hEffVarRatio_1->SetMarkerStyle(kOpenSquare);
    hEffVarRatio_2->SetMarkerColor(4);
    hEffVarRatio_2->SetMarkerStyle(kFullSquare);
    hEffVarRatio_2->SetMinimum(-0.3);
    hEffVarRatio_2->SetMaximum(0.3);
    hEffVarRatio_3->SetMarkerColor(6);
    hEffVarRatio_3->SetMarkerStyle(kOpenCircle);
    hEffVarRatio_4->SetMarkerColor(8);
    hEffVarRatio_4->SetMarkerStyle(kFullCircle);
    hEffVarRatio_2->Rebin(20);
    hEffVarRatio_1->Rebin(20);
    hEffVarRatio_3->Rebin(20);
    hEffVarRatio_4->Rebin(20);
    hEffVarRatio_2->Draw("P");
    hEffVarRatio_1->Draw("PSAME");
    hEffVarRatio_3->Draw("PSAME");
    hEffVarRatio_4->Draw("PSAME");
    auto l_effvarRatio = new TLegend(0.25, 0.75, 0.35, 0.85);
    l_effvarRatio->SetTextSize(0.03);
    l_effvarRatio->SetBorderSize(0);
    l_effvarRatio->AddEntry(hEffVarRatio_1, "<dE/dx> + 3%", "p");
    l_effvarRatio->AddEntry(hEffVarRatio_2, "<dE/dx> - 3%", "p");
    l_effvarRatio->AddEntry(hEffVarRatio_3, "#sigma_{dE/dx} + 3%", "p");
    l_effvarRatio->AddEntry(hEffVarRatio_4, "#sigma_{dE/dx} - 3%", "p");
    l_effvarRatio->Draw();
    hEffVarRatio_1->Write("hEffVarRatio_1", TObject::kWriteDelete);
    hEffVarRatio_2->Write("hEffVarRatio_2", TObject::kWriteDelete);
    hEffVarRatio_3->Write("hEffVarRatio_3", TObject::kWriteDelete);
    hEffVarRatio_4->Write("hEffVarRatio_4", TObject::kWriteDelete);
    cEffVarRatio->Print(Form("%s/cEffVarRatio.root", outDir.Data()),"root");
    cEffVarRatio->Print(Form("%s/cEffVarRatio.pdf", outDir.Data()),"pdf");

    // --- Cross section for diff TPC dE/dx variations
    auto cxsecdEdxVar = new TCanvas("cxsecdEdxVar", "cxsecdEdxVar", 900, 600);
    cxsecdEdxVar->cd();
    hxsecdEdxVar_0->SetMarkerColor(1);
    hxsecdEdxVar_0->SetMarkerStyle(kFullCircle);
    hxsecdEdxVar_1->SetMarkerColor(2);
    hxsecdEdxVar_1->SetMarkerStyle(kOpenSquare);
    hxsecdEdxVar_2->SetMarkerColor(4);
    hxsecdEdxVar_2->SetMarkerStyle(kFullSquare);
    // hxsecdEdxVar_2->SetMinimum(-1);
    // hxsecdEdxVar_2->SetMaximum(1);
    hxsecdEdxVar_3->SetMarkerColor(6);
    hxsecdEdxVar_3->SetMarkerStyle(kOpenCircle);
    hxsecdEdxVar_4->SetMarkerColor(8);
    hxsecdEdxVar_4->SetMarkerStyle(kFullCircle);
    hxsecdEdxVar_0->Rebin(20);
    hxsecdEdxVar_1->Rebin(20);
    hxsecdEdxVar_2->Rebin(20);
    hxsecdEdxVar_3->Rebin(20);
    hxsecdEdxVar_4->Rebin(20);
    hxsecdEdxVar_0->Draw("P");
    hxsecdEdxVar_1->Draw("PSAME");
    hxsecdEdxVar_2->Draw("PSAME");
    hxsecdEdxVar_3->Draw("PSAME");
    hxsecdEdxVar_4->Draw("PSAME");
    auto l_xsecdEdxVar = new TLegend(0.8, 0.70, 0.9, 0.85);
    l_xsecdEdxVar->SetTextSize(0.03);
    l_xsecdEdxVar->SetBorderSize(0);
    l_xsecdEdxVar->AddEntry(hxsecdEdxVar_0, "base", "p");
    l_xsecdEdxVar->AddEntry(hxsecdEdxVar_1, "<dE/dx> + 3%", "p");
    l_xsecdEdxVar->AddEntry(hxsecdEdxVar_2, "<dE/dx> - 3%", "p");
    l_xsecdEdxVar->AddEntry(hxsecdEdxVar_3, "#sigma_{dE/dx} + 3%", "p");
    l_xsecdEdxVar->AddEntry(hxsecdEdxVar_4, "#sigma_{dE/dx} - 3%", "p");
    l_xsecdEdxVar->Draw();
    hxsecdEdxVar_1->Write("hxsecdEdxVar_0", TObject::kWriteDelete);
    hxsecdEdxVar_1->Write("hxsecdEdxVar_1", TObject::kWriteDelete);
    hxsecdEdxVar_2->Write("hxsecdEdxVar_2", TObject::kWriteDelete);
    hxsecdEdxVar_3->Write("hxsecdEdxVar_3", TObject::kWriteDelete);
    hxsecdEdxVar_4->Write("hxsecdEdxVar_4", TObject::kWriteDelete);
    cxsecdEdxVar->Print(Form("%s/cxsecdEdxVar.root", outDir.Data()),"root");
    cxsecdEdxVar->Print(Form("%s/cxsecdEdxVar.pdf", outDir.Data()),"pdf");

    // --- Extract PID sys. err. for the 5bins of cross section
    // mean dE/dx
    double sys_dEdx_mean_1[3] = {hxsecdEdxVar_0->GetBinContent(1), hxsecdEdxVar_1->GetBinContent(1), hxsecdEdxVar_2->GetBinContent(1)};
    double sys_dEdx_mean_2[3] = {hxsecdEdxVar_0->GetBinContent(2), hxsecdEdxVar_1->GetBinContent(2), hxsecdEdxVar_2->GetBinContent(2)};
    double sys_dEdx_mean_3[3] = {hxsecdEdxVar_0->GetBinContent(3), hxsecdEdxVar_1->GetBinContent(3), hxsecdEdxVar_2->GetBinContent(3)};
    double sys_dEdx_mean_4[3] = {hxsecdEdxVar_0->GetBinContent(4), hxsecdEdxVar_1->GetBinContent(4), hxsecdEdxVar_2->GetBinContent(4)};
    double sys_dEdx_mean_5[3] = {hxsecdEdxVar_0->GetBinContent(5), hxsecdEdxVar_1->GetBinContent(5), hxsecdEdxVar_2->GetBinContent(5)};

    double err_xsec_dEdx_mean_1 = TMath::RMS(3, sys_dEdx_mean_1)/hxsecdEdxVar_0->GetBinContent(1);
    double err_xsec_dEdx_mean_2 = TMath::RMS(3, sys_dEdx_mean_2)/hxsecdEdxVar_0->GetBinContent(1);
    double err_xsec_dEdx_mean_3 = TMath::RMS(3, sys_dEdx_mean_3)/hxsecdEdxVar_0->GetBinContent(1);
    double err_xsec_dEdx_mean_4 = TMath::RMS(3, sys_dEdx_mean_4)/hxsecdEdxVar_0->GetBinContent(1);
    double err_xsec_dEdx_mean_5 = TMath::RMS(3, sys_dEdx_mean_5)/hxsecdEdxVar_0->GetBinContent(1);

    // --- Extract PID sys. err. for the 5bins of cross section
    // width dE/dx
    double sys_dEdx_width_1[3] = {hxsecdEdxVar_0->GetBinContent(1), hxsecdEdxVar_3->GetBinContent(1), hxsecdEdxVar_4->GetBinContent(1)};
    double sys_dEdx_width_2[3] = {hxsecdEdxVar_0->GetBinContent(2), hxsecdEdxVar_3->GetBinContent(2), hxsecdEdxVar_4->GetBinContent(2)};
    double sys_dEdx_width_3[3] = {hxsecdEdxVar_0->GetBinContent(3), hxsecdEdxVar_3->GetBinContent(3), hxsecdEdxVar_4->GetBinContent(3)};
    double sys_dEdx_width_4[3] = {hxsecdEdxVar_0->GetBinContent(4), hxsecdEdxVar_3->GetBinContent(4), hxsecdEdxVar_4->GetBinContent(4)};
    double sys_dEdx_width_5[3] = {hxsecdEdxVar_0->GetBinContent(5), hxsecdEdxVar_3->GetBinContent(5), hxsecdEdxVar_4->GetBinContent(5)};

    double err_xsec_dEdx_width_1 = TMath::RMS(3, sys_dEdx_width_1)/hxsecdEdxVar_0->GetBinContent(1);
    double err_xsec_dEdx_width_2 = TMath::RMS(3, sys_dEdx_width_2)/hxsecdEdxVar_0->GetBinContent(1);
    double err_xsec_dEdx_width_3 = TMath::RMS(3, sys_dEdx_width_3)/hxsecdEdxVar_0->GetBinContent(1);
    double err_xsec_dEdx_width_4 = TMath::RMS(3, sys_dEdx_width_4)/hxsecdEdxVar_0->GetBinContent(1);
    double err_xsec_dEdx_width_5 = TMath::RMS(3, sys_dEdx_width_5)/hxsecdEdxVar_0->GetBinContent(1);

    printf("double err_xsec_dEdx_mean[5] = {%f, %f, %f, %f, %f}; \n", err_xsec_dEdx_mean_1, err_xsec_dEdx_mean_2, err_xsec_dEdx_mean_3, err_xsec_dEdx_mean_4, err_xsec_dEdx_mean_5);
    printf("double err_xsec_dEdx_width[5] = {%f, %f, %f, %f, %f}; \n", err_xsec_dEdx_width_1, err_xsec_dEdx_width_2, err_xsec_dEdx_width_3, err_xsec_dEdx_width_4, err_xsec_dEdx_width_5);

    // double err_xsec_dEdx_mean[5] = {0.012381, 0.072927, 0.134824, 0.045944, 0.013546}; 
    // double err_xsec_dEdx_width[5] = {0.000960, 0.007780, 0.061392, 0.043450, 0.010984};

    // double err_xsec_dEdx_mean[5] = {0.009864, 0.074512, 0.138657, 0.059612, 0.013369}; 
    // double err_xsec_dEdx_width[5] = {0.000441, 0.008398, 0.063546, 0.036764, 0.004275};  

    // Yields
    // --- MC
    TCanvas *cYieldVsMkk_mc = new TCanvas("cYieldVsMkk_mc", "cYieldVsMkk_mc", 900, 600);
    cYieldVsMkk_mc->cd();
    auto lYieldVsMkk_mc = new TLegend(0.6, 0.65, 0.9, 0.95);
    lYieldVsMkk_mc->SetTextSize(0.03);
    lYieldVsMkk_mc->SetBorderSize(0);    
    // gNsig_mc->Draw("AP");
    // gNsig_UnLikeCharge_mc->Draw("PSAME");
    // gNsig_dEdxCut_mc->Draw("PSAME");
    // gNsig_PtCut01_mc->Draw("PSAME");
    // gNsig_RejectPionSub_mc->Draw("AP");
    gNsig_RejectPionElectronSub_mc->Draw("AP");
    gNsig_RejectAllSub_mc->Draw("PSAME");
    // lYieldVsMkk_mc->AddEntry(gNsig_mc, "pair tracks", "p");
    // lYieldVsMkk_mc->AddEntry(gNsig_UnLikeCharge_mc, "opposite charge", "p");
    // lYieldVsMkk_mc->AddEntry(gNsig_dEdxCut_mc, "|#sigma^{K^{#pm}}_{dE/dx}|<3", "p");
    // lYieldVsMkk_mc->AddEntry(gNsig_PtCut01_mc, "|#sigma^{K^{#pm}}_{dE/dx}|<3 + p_{T}<0.1 GeV/c", "p");
    // lYieldVsMkk_mc->AddEntry(gNsig_RejectPionSub_mc, "|#sigma^{(#pi)}_{dE/dx}|>2 + Bkg subtraction", "p");
    lYieldVsMkk_mc->AddEntry(gNsig_RejectPionElectronSub_mc, "|#sigma^{(#pi, e)}_{dE/dx}|>2 + Bkg subtraction", "p");
    lYieldVsMkk_mc->AddEntry(gNsig_RejectAllSub_mc, "|#sigma^{(#pi, e, #mu)}_{dE/dx}|>2 + Bkg subtraction", "p");
    lYieldVsMkk_mc->Draw();
    gNsig_RejectPionElectronSub_mc->Write("gNsig_RejectPionElectronSub_mc", TObject::kWriteDelete);
    gNsig_RejectAllSub_mc->Write("gNsig_RejectAllSub_mc", TObject::kWriteDelete);
    cYieldVsMkk_mc->Print(Form("%s/cYieldVsMkk_mc.root", outDir.Data()),"root");
    cYieldVsMkk_mc->Print(Form("%s/cYieldVsMkk_mc.pdf", outDir.Data()),"pdf");

    // --- Data
    TCanvas *cYieldVsMkk_data = new TCanvas("cYieldVsMkk_data", "cYieldVsMkk_data", 900, 600);
    cYieldVsMkk_data->cd();
    auto lYieldVsMkk_data = new TLegend(0.6, 0.65, 0.9, 0.95);
    lYieldVsMkk_data->SetTextSize(0.03);
    lYieldVsMkk_data->SetBorderSize(0);    
    // gNsig_data->Draw("AP");
    // gNsig_UnLikeCharge_data->Draw("PSAME");
    // gNsig_dEdxCut_data->Draw("PSAME");
    // gNsig_PtCut01_data->Draw("PSAME");
    // gNsig_RejectPionSub_data->Draw("AP");
    gNsig_RejectPionElectronSub_data->Draw("AP");
    gNsig_RejectAllSub_data->Draw("PSAME"); // PSAME
    // lYieldVsMkk_data->AddEntry(gNsig_data, "pair tracks", "p");
    // lYieldVsMkk_data->AddEntry(gNsig_UnLikeCharge_data, "opposite charge", "p");
    // lYieldVsMkk_data->AddEntry(gNsig_dEdxCut_data, "|#sigma^{K^{#pm}}_{dE/dx}|<3", "p");
    // lYieldVsMkk_data->AddEntry(gNsig_PtCut01_data, "|#sigma^{K^{#pm}}_{dE/dx}|<3 + p_{T}<0.1 GeV/c", "p");
    // lYieldVsMkk_data->AddEntry(gNsig_RejectPionSub_data, "|#sigma^{(#pi)}_{dE/dx}|>2 + Bkg subtraction", "p");
    lYieldVsMkk_data->AddEntry(gNsig_RejectPionElectronSub_data, "|#sigma^{(#pi,e)}_{dE/dx}|>2 + Bkg subtraction", "p");
    lYieldVsMkk_data->AddEntry(gNsig_RejectAllSub_data, "|#sigma^{(#pi, e, #mu)}_{dE/dx}|>2 + Bkg subtraction", "p");
    lYieldVsMkk_data->Draw();
    gNsig_RejectPionElectronSub_data->Write("gNsig_RejectPionElectronSub_data", TObject::kWriteDelete);
    gNsig_RejectAllSub_data->Write("gNsig_RejectAllSub_data", TObject::kWriteDelete);
    cYieldVsMkk_data->Print(Form("%s/cYieldVsMkk_data.root", outDir.Data()),"root");
    cYieldVsMkk_data->Print(Form("%s/cYieldVsMkk_data.pdf", outDir.Data()),"pdf");

    // Par Pt after Bkg sub
    TCanvas *cPairPtsub = new TCanvas("cPairPtsub", "cPairPtsub", 900, 600);
    cPairPtsub->cd();
    gPairPtsub->Draw("AP"); // PSAME
    gPairPtsub->Write("gPtPairSub", TObject::kWriteDelete); // Form("h%s_gNsig_dEdxCut", name.Data()), TObject::kWriteDelete
    cPairPtsub->Print(Form("%s/cPairPtsub.root", outDir.Data()),"root");
    cPairPtsub->Print(Form("%s/cPairPtsub.pdf", outDir.Data()),"pdf");

    // Efficiency
    TCanvas *ckpkmEfficiency = new TCanvas("ckpkmEfficiency", "ckpkmEfficiency", 900, 600);
    ckpkmEfficiency->cd();
    auto lkpkmEfficiency = new TLegend(0.18, 0.65, 0.35, 0.95);
    lkpkmEfficiency->SetTextSize(0.03);
    lkpkmEfficiency->SetBorderSize(0);    
    gEff->Draw("AP");
    gEff_UnLikeCharge->Draw("PSAME");
    gEff_dEdxCut->Draw("PSAME");
    gEff_PtCut01->Draw("PSAME");
    gEff_RejectPionSub->Draw("PSAME");
    gEff_RejectPionElectronSub->Draw("PSAME");
    gEff_RejectAllSub->Draw("PSAME");
    lkpkmEfficiency->AddEntry(gEff, "pair tracks", "p");
    lkpkmEfficiency->AddEntry(gEff_UnLikeCharge, "opposite charge", "p");
    lkpkmEfficiency->AddEntry(gEff_dEdxCut, "|#sigma^{K^{#pm}}_{dE/dx}|<3", "p");
    lkpkmEfficiency->AddEntry(gEff_PtCut01, "|#sigma^{K^{#pm}}_{dE/dx}|<3 + p_{T}<0.1 GeV/c", "p");
    lkpkmEfficiency->AddEntry(gEff_RejectPionSub, "p_{T}<0.1. + |#sigma^{(#pi)}_{dE/dx}|>2 + Bkg subtraction", "p");
    lkpkmEfficiency->AddEntry(gEff_RejectPionElectronSub, "p_{T}<0.1 + |#sigma^{(#pi, e)}_{dE/dx}|>2 + Bkg subtraction", "p");
    lkpkmEfficiency->AddEntry(gEff_RejectAllSub, "p_{T}<0.1 + |#sigma^{(#pi, e, #mu)}_{dE/dx}|>2 + Bkg subtraction", "p");
    lkpkmEfficiency->Draw();
    gEff_dEdxCut->Write(); // Form("h%s_gEff_dEdxCut", name.Data()), TObject::kWriteDelete
    ckpkmEfficiency->Print(Form("%s/ckpkmEfficiency.root", outDir.Data()),"root");
    ckpkmEfficiency->Print(Form("%s/ckpkmEfficiency.pdf", outDir.Data()),"pdf");
    
    TCanvas *ckpkmXSection = new TCanvas("ckpkmXSection", "ckpkmXSection", 900, 600);
    ckpkmXSection->cd();
    // ckpkmXSection->SetLogy();
    // gkpkmXSection->RemovePoint(4);
    gkpkmXSection->Draw("AP");
    gkpkmXSection->Write(); // Form("h%s_gkpkmXSection", name.Data()), TObject::kWriteDelete
    ckpkmXSection->Print(Form("%s/ckpkmXSection.root", outDir.Data()),"root");
    ckpkmXSection->Print(Form("%s/ckpkmXSection.pdf", outDir.Data()),"pdf");

    TCanvas *cresidual = new TCanvas("cresidual", "cresidual", 900, 600);
    cresidual->cd();
    gresidual->Draw("AP");
    // gresidual->Write(); // Form("h%s_gresidual", name.Data()), TObject::kWriteDelete
    cresidual->Print(Form("%s/cresidual.root", outDir.Data()),"root");
    cresidual->Print(Form("%s/cresidual.pdf", outDir.Data()),"pdf");

    TCanvas *cdxsectiondm = new TCanvas("cdxsectiondm", "cdxsectiondm", 900, 600);
    cdxsectiondm->cd();

    // cdxsectiondm->SetLogy();
    // gdxsectiondm->Draw("AP");

    // Int_t n_m2k_new = 4;
    // Double_t x_m2k_new[4]      = {x_m2k[0], x_m2k[1], x_m2k[2], x_m2k[3]};
    // Double_t y_m2k_new[4]      = {y_m2k[0], y_m2k[1], y_m2k[2], y_m2k[3]};
    // Double_t exl_m2k_new[4]    = {exl_m2k[0], exl_m2k[1], exl_m2k[2], exl_m2k[3]};
    // Double_t exh_m2k_new[4]     = {exh_m2k[0], exh_m2k[1], exh_m2k[2], exh_m2k[3]};
    // Double_t eylstat_m2k_new[4] = {eylstat_m2k[0], eylstat_m2k[1], eylstat_m2k[2], eylstat_m2k[3]};
    // Double_t eyhstat_m2k_new[4] = {eyhstat_m2k[0], eyhstat_m2k[1], eyhstat_m2k[2], eyhstat_m2k[3]};
    // Double_t eylsys_m2k_new[4]  = {eylsys_m2k[0], eylsys_m2k[1], eylsys_m2k[2], eylsys_m2k[3]};
    // Double_t eyhsys_m2k_new[4] = {eyhsys_m2k[0], eyhsys_m2k[1], eyhsys_m2k[2], eyhsys_m2k[3]};

    TGraphMultiErrors* gdxsectiondm = new TGraphMultiErrors("gdxsectiondm", "", n_m2k, x_m2k, y_m2k, exl_m2k, exh_m2k, eylstat_m2k, eyhstat_m2k);
    gdxsectiondm->AddYError(n_m2k, eylsys_m2k, eyhsys_m2k);
    gdxsectiondm->SetMarkerStyle(20);
    gdxsectiondm->SetMinimum(0.0);
    gdxsectiondm->SetTitle("; m_{K^{+}K^{+}}; d\\sigma/dm (mb)");
    // gdxsectiondm->SetLineColor(kRed);
    // gdxsectiondm->GetAttLine(0)->SetLineColor(kRed);
    gdxsectiondm->GetAttLine(1)->SetLineColor(kGreen);
    gdxsectiondm->GetAttFill(1)->SetFillStyle(1);
    // Graph and x erros drawn with "APS"
    // Stat Errors drawn with "Z"
    // Sys Errors drawn with "5 s=0.5"
    gdxsectiondm->Draw("APS ; Z ; 5 s=1.0"); //APS ; Z ; 6 s=0.5
    cdxsectiondm->Update();

    gdxsectiondm->Write(); // Form("h%s_gdxsectiondy", name.Data()), TObject::kWriteDelete
    cdxsectiondm->Print(Form("%s/cdxsectiondm.root", outDir.Data()),"root");
    cdxsectiondm->Print(Form("%s/cdxsectiondm.pdf", outDir.Data()),"pdf");

    TCanvas *cMassPairKaon = new TCanvas("cMassPairKaon", "cMassPairKaon", 1000, 1000);
    cMassPairKaon->Divide(1, 3);
    cMassPairKaon->cd(1);
    hMassPairKaon_truth->SetLineColor(kBlack);;
    hMassPairKaon_truth->Draw();
    cMassPairKaon->cd(2);
    hMassPairKaonRejectAllSub_mc->Draw();
    cMassPairKaon->cd(3);
    hMassPairKaonRejectAllSub_data->SetLineColor(kBlue);
    hMassPairKaonRejectAllSub_data->Draw();
    cMassPairKaon->Print(Form("%s/cMassPairKaon.root", outDir.Data()),"root");
    cMassPairKaon->Print(Form("%s/cMassPairKaon.pdf", outDir.Data()),"pdf");

    // // cout<<"| hMassPairKaon_mc = "<<hMassPairKaon_mc->GetXaxis()->GetBinWidth(2)<<" | hMassPairKaonRejectAllSub_mc = "<<hMassPairKaonRejectAllSub_mc->GetXaxis()->GetBinWidth(2)<<endl;
    // // cout<<"| hMassPairKaon_mc = "<<hMassPairKaon_mc->GetNbinsX()<<" | hMassPairKaonRejectAllSub_mc = "<<hMassPairKaonRejectAllSub_mc->GetNbinsX()<<endl;

    // fprintf(tab_sys_m, "\\hline\n \\end{tabular}\n \\end{table}\n \\end{document}\n");
    // fclose(tab_sys_m);
    // gSystem->Exec(Form("pdflatex -output-directory %s %s/tab_sys_m.tex", outDir.Data(), outDir.Data()));

    // printf(" ####### mass = %0.3f, %0.3f, %0.3f, %0.3f, %0.3f, %0.3f, %0.3f\n", x[1], x[2], x[3], x[4], x[5], x[6], x[7]);
    // printf(" ####### xsec = %0.3f, %0.3f, %0.3f, %0.3f, %0.3f, %0.3f, %0.3f\n", y_m2k[1], y_m2k[2], y_m2k[3], y_m2k[4], y_m2k[5], y_m2k[6], y_m2k[7]);

    // fprintf(tab_eff, "\\hline\n \\end{tabular}\n \\end{table}\n \\end{document}\n");
    // fclose(tab_eff);
    // gSystem->Exec(Form("pdflatex -output-directory %s %s/tab_eff.tex", outDir.Data(), outDir.Data()));

    fprintf(tab_sys_m, "\\hline\n \\end{tabular}\n \\end{table}\n \\end{document}\n");
    fclose(tab_sys_m);
    gSystem->Exec(Form("pdflatex -output-directory %s %s/tab_sys_m.tex", outDir.Data(), outDir.Data()));

    // End: ====================  Cross section in mass bins ====================

    // =================================================================================
    // =================================================================================
    // =================================================================================

    // Begin: ====================  Cross section in cos(Theta) bins ====================

    auto hMassPairKaonVsCosTheta_truth = (TH2D *)f_mc->Get("hMassPairKaonVsCosTheta_truth");
    auto hMassPairKaonVsCosThetaSub_mc = (TH2D *)f_mc->Get("hMassPairKaonVsCosThetaSub");
    auto hMassPairKaonVsCosThetaSub_data = (TH2D *)f_data->Get("hMassPairKaonVsCosThetaSub");

    // TGraphErrors *gdxsectiondcosTheta = new TGraphErrors();
    // gdxsectiondcosTheta->SetMarkerStyle(20);
    // // gdxsectiondcosTheta->SetMarkerSize(1.5);
    // gdxsectiondcosTheta->SetMarkerColor(kBlue);
    // gdxsectiondcosTheta->SetMinimum(0.0);
    // gdxsectiondcosTheta->SetTitle("; y; d\\sigma/dcosTheta (mb)");

    TCanvas *cMassPairKaonCosTheta = new TCanvas("cMassPairKaonCosTheta", "cMassPairKaonCosTheta", 1000, 1000);
    cMassPairKaonCosTheta->Divide(3, 3);

    // -------------------- Systematic Errors --------------------
    const Int_t n_cosTheta = hMassPairKaonVsCosThetaSub_data->GetNbinsX();
    Double_t x_cosTheta[n_cosTheta], y_cosTheta[n_cosTheta], exl_cosTheta[n_cosTheta], exh_cosTheta[n_cosTheta], eylstat_cosTheta[n_cosTheta], eyhstat_cosTheta[n_cosTheta], eylsys_cosTheta[n_cosTheta], eyhsys_cosTheta[n_cosTheta];

    double xsec_cosTheta_optimal[]              = {18.615, 21.292, 6.646}; // pt < 0.1; rad: 9; reject: 2
    double xsec_cosTheta_rad8[]                 = {18.637, 21.312, 6.656}; // regions: 8
    double xsec_cosTheta_rad10[]                = {18.600, 21.277, 6.636}; // regions: 10
    double xsec_cosTheta_rejectLessStd[]        = {17.458, 20.657, 6.424}; // 1.75
    double xsec_cosTheta_rejectMoreStd[]        = {19.162, 21.885, 6.880}; // 2.25

    for (Int_t i = 1; i <= hMassPairKaonVsCosThetaSub_data->GetNbinsX(); ++i)
    {
        cMassPairKaonCosTheta->cd(i);
        TH1D *hMassPairKaon_py_truth = hMassPairKaonVsCosTheta_truth->ProjectionY(Form("_truth_%d", i), i, i);
        hMassPairKaon_py_truth->Draw();
        cMassPairKaonCosTheta->cd(i+3);
        TH1D *hMassPairKaon_py_mc = hMassPairKaonVsCosThetaSub_mc->ProjectionY(Form("_mc_%d", i), i, i);
        hMassPairKaon_py_mc->Draw();
        cMassPairKaonCosTheta->cd(i+6);
        TH1D *hMassPairKaon_py_data = hMassPairKaonVsCosThetaSub_data->ProjectionY(Form("_data_%d", i), i, i);
        hMassPairKaon_py_data->Draw();

        double ymin = hMassPairKaonVsCosThetaSub_data->GetXaxis()->GetBinLowEdge(i);
        double ymax = hMassPairKaonVsCosThetaSub_data->GetXaxis()->GetBinUpEdge(i);
        hMassPairKaon_py_truth->SetTitle(Form("%.2f<cos(#theta)<%.2f;m_{K^{+}K^{-}} (GeV/c^{2});Counts",ymin,ymax));
        hMassPairKaon_py_mc->SetTitle(Form("%.2f<cos(#theta)<%.2f;m_{K^{+}K^{-}} (GeV/c^{2});Counts",ymin,ymax));
        hMassPairKaon_py_data->SetTitle(Form("%.2f<cos(#theta)<%.2f;m_{K^{+}K^{-}} (GeV/c^{2});Counts",ymin,ymax));

        double eff = 0, deff = 0, xsec_kpkm = 0, dxsec_kpkm_stat = 0;
        double lumi_kpkm = 419; // mili barn^{-1}
        double fpileup = 1.111; // from Rho(770) analysis
        double Nsig_truth = hMassPairKaon_py_truth->Integral();
        double dNsig_mc;
        double Nsig_mc = hMassPairKaon_py_mc->IntegralAndError(1, hMassPairKaon_py_mc->GetNbinsX(), dNsig_mc, "");
        double dNsig_data;
        double Nsig_data = hMassPairKaon_py_data->IntegralAndError(1, hMassPairKaon_py_data->GetNbinsX(), dNsig_data, "");
        double cosTheta = hMassPairKaonVsCosThetaSub_data->GetXaxis()->GetBinCenter(i);
        double dcosTheta = hMassPairKaonVsCosThetaSub_data->GetXaxis()->GetBinWidth(i);

        eff = Nsig_mc / Nsig_truth; // Efficiency = N_observed/N_generated
        deff = eff * (dNsig_mc / Nsig_mc);

        // -------------------- Systematic Errors --------------------
        double xsec_rad[]            = {xsec_cosTheta_optimal[i], xsec_cosTheta_rad8[i], xsec_cosTheta_rad10[i]};
        double xsec_reject[] = {xsec_cosTheta_optimal[i], xsec_cosTheta_rejectLessStd[i], xsec_cosTheta_rejectMoreStd[i]};
        double err_xsec_rad            = TMath::RMS(3, xsec_rad)/xsec_cosTheta_optimal[i];
        double err_xsec_reject         = TMath::RMS(3, xsec_reject)/xsec_cosTheta_optimal[i];

        // from rho0 analysis
        double err_trk_select  = 0.015;
        double err_trk_match  = 0.040;
        double err_trk_eff  = 0.010;
        double err_incoh  = 0.005;
        double err_SPD_chip  = 0.010;
        double err_PileUp  = 0.038;
        double err_Lumi  = 0.050;

        double err_sys = TMath::Sqrt(err_xsec_rad*err_xsec_rad + err_xsec_reject*err_xsec_reject + err_trk_select*err_trk_select + err_trk_match*err_trk_match + err_trk_eff*err_trk_eff + err_incoh*err_incoh + err_SPD_chip*err_SPD_chip + err_PileUp*err_PileUp + err_Lumi*err_Lumi);

        xsec_kpkm = (Nsig_data * fpileup) / (eff * lumi_kpkm * dcosTheta);
        dxsec_kpkm_stat = xsec_kpkm * (dNsig_data / Nsig_data);
        double dxsec_phi2pi_sys = xsec_kpkm * TMath::Sqrt((err_sys*err_sys));

        // gdxsectiondcosTheta->SetPoint(i-1, cosTheta, xsec_kpkm);
        // gdxsectiondcosTheta->SetPointError(i-1, 0, dxsec_kpkm_stat);

        // gdxsectiondcosTheta->SetPoint(i-1, cosTheta, xsec_kpkm);
        // gdxsectiondcosTheta->SetPointError(i-1, 2, 0, 0, dxsec_kpkm_stat, dxsec_kpkm_stat, dxsec_phi2pi_sys, dxsec_phi2pi_sys);

        x_cosTheta[i-1]       = cosTheta;
        y_cosTheta[i-1]       = xsec_kpkm;
        exl_cosTheta[i-1]     = hMassPairKaonVsCosThetaSub_data->GetXaxis()->GetBinWidth(i)/2;
        exh_cosTheta[i-1]     = hMassPairKaonVsCosThetaSub_data->GetXaxis()->GetBinWidth(i)/2;
        eylstat_cosTheta[i-1] = dxsec_kpkm_stat;
        eyhstat_cosTheta[i-1] = dxsec_kpkm_stat;
        eylsys_cosTheta[i-1]  = dxsec_phi2pi_sys;
        eyhsys_cosTheta[i-1]  = dxsec_phi2pi_sys;

        fprintf(tab_sys_theta, "(%0.2f , %0.2f) & %0.f & %0.f & %0.f & %0.2f$\\pm$%0.2f$\\pm$%0.2f & %0.2f & %0.2f \\\\ \n", ymin, ymax, Nsig_truth, Nsig_mc, Nsig_data, xsec_kpkm, dxsec_kpkm_stat, dxsec_phi2pi_sys, err_xsec_rad * 100, err_xsec_reject * 100);

        // printf("i = %d, ymin = %f, ymax = %f, Nsig_data = %f, eff = %f, x_cosTheta = %f, y_cosTheta = %f, eylstat_cosTheta = %f, eylsys_cosTheta = %f \n", i-1, ymin, ymax, Nsig_data, eff, x_cosTheta[i-1], y_cosTheta[i-1], eylstat_cosTheta[i-1], eylsys_cosTheta[i-1]);

        // printf(" ####### ymin, ymaxt = %0.3f , %0.3f \n", ymin, ymax);
        // printf(" ####### xsec_kpkm = %0.3f \n", xsec_kpkm);
        // printf(" ####### dxsec_kpkm_stat = %0.3f \n", dxsec_kpkm_stat);
        // printf(" ####### dxsec_phi2pi_sys = %0.3f \n", dxsec_phi2pi_sys);
    }
    
    TCanvas *cdxsectiondcosTheta = new TCanvas("cdxsectiondcosTheta", "cdxsectiondcosTheta", 900, 600);
    cdxsectiondcosTheta->cd();

    // cdxsectiondcosTheta->SetLogy();
    // gdxsectiondcosTheta->Draw("AP");

    TGraphMultiErrors* gdxsectiondcosTheta = new TGraphMultiErrors("gdxsectiondcosTheta", "", n_cosTheta, x_cosTheta, y_cosTheta, exl_cosTheta, exh_cosTheta, eylstat_cosTheta, eyhstat_cosTheta);
    gdxsectiondcosTheta->AddYError(n_cosTheta, eylsys_cosTheta, eyhsys_cosTheta);
    gdxsectiondcosTheta->SetMarkerStyle(20);
    gdxsectiondcosTheta->SetMinimum(0.0);
    gdxsectiondcosTheta->SetTitle("; cos(#theta); d\\sigma/dcos(#theta) (mb)");
    // gdxsectiondcosTheta->SetLineColor(kRed);
    // gdxsectiondcosTheta->GetAttLine(0)->SetLineColor(kRed);
    gdxsectiondcosTheta->GetAttLine(1)->SetLineColor(kGreen);
    gdxsectiondcosTheta->GetAttFill(1)->SetFillStyle(1);
    // Graph and x erros drawn with "APS"
    // Stat Errors drawn with "Z"
    // Sys Errors drawn with "5 s=0.5"
    gdxsectiondcosTheta->Draw("APS ; Z ; 5 s=1.0"); //APS ; Z ; 6 s=0.5
    cdxsectiondcosTheta->Update();

    gdxsectiondcosTheta->Write(); // Form("h%s_gdxsectiondcosTheta", name.Data()), TObject::kWriteDelete
    cdxsectiondcosTheta->Print(Form("%s/cdxsectiondcosTheta.root", outDir.Data()),"root");
    cdxsectiondcosTheta->Print(Form("%s/cdxsectiondcosTheta.pdf", outDir.Data()),"pdf");

    cMassPairKaonCosTheta->Print(Form("%s/cMassPairKaonCosTheta.root", outDir.Data()),"root");
    cMassPairKaonCosTheta->Print(Form("%s/cMassPairKaonCosTheta.pdf", outDir.Data()),"pdf");

    // cout<<"| hMassPairKaon_mc = "<<hMassPairKaon_mc->GetXaxis()->GetBinWidth(2)<<" | hMassPairKaonRejectAllSub_mc = "<<hMassPairKaonRejectAllSub_mc->GetXaxis()->GetBinWidth(2)<<endl;
    // cout<<"| hMassPairKaon_mc = "<<hMassPairKaon_mc->GetNbinsX()<<" | hMassPairKaonRejectAllSub_mc = "<<hMassPairKaonRejectAllSub_mc->GetNbinsX()<<endl;

    fprintf(tab_sys_theta, "\\hline\n \\end{tabular}\n \\end{table}\n \\end{document}\n");
    fclose(tab_sys_theta);
    gSystem->Exec(Form("pdflatex -output-directory %s %s/tab_sys_theta.tex", outDir.Data(), outDir.Data()));

    printf(" ####### xsec_cosTheta = %0.3f, %0.3f, %0.3f, %0.3f, %0.3f \n", y_cosTheta[0], y_cosTheta[1], y_cosTheta[2], y_cosTheta[3], y_cosTheta[4]);

    // End: ====================  Cross section in cos(Theta) bins ====================

    printf(" ####### xsec_m2k = %0.3f, %0.3f, %0.3f, %0.3f, %0.3f \n", y_m2k[0], y_m2k[1], y_m2k[2], y_m2k[3], y_m2k[4]);
  
/*
    // Begin: ====================  Cross section in rapidity bins ====================

    auto hMassPairKaonVsRapidity_truth = (TH2D *)f_mc->Get("hMassPairKaonVsRapidity_truth");
    auto hMassPairKaonVsRapiditySub_mc = (TH2D *)f_mc->Get("hMassPairKaonVsRapiditySub");
    auto hMassPairKaonVsRapiditySub_data = (TH2D *)f_data->Get("hMassPairKaonVsRapiditySub");

    // TGraphErrors *gdxsectiondy = new TGraphErrors();
    // gdxsectiondy->SetMarkerStyle(20);
    // // gdxsectiondy->SetMarkerSize(1.5);
    // gdxsectiondy->SetMarkerColor(kBlue);
    // gdxsectiondy->SetMinimum(0.0);
    // gdxsectiondy->SetTitle("; y; d\\sigma/dy (mb)");

    TCanvas *cMassPairKaonY = new TCanvas("cMassPairKaonY", "cMassPairKaonY", 1000, 1000);
    cMassPairKaonY->Divide(3, 3);

    // -------------------- Systematic Errors --------------------
    const Int_t np = hMassPairKaonVsRapiditySub_data->GetNbinsX();
    Double_t x[np], y[np], exl[np], exh[np], eylstat[np], eyhstat[np], eylsys[np], eyhsys[np];

    double xsec_optimal[]             = {3.923, 4.965, 3.031}; // pt < 0.1; rad: 9; reject: 2
    double xsec_pt011[]               = {3.834, 4.959, 2.964}; // pt < 0.11
    double xsec_pt009[]               = {4.087, 4.817, 3.166}; // pt < 0.09
    double xsec_rad8[]               = {3.524, 4.992, 3.059}; // regions: 8
    double xsec_rad10[]               = {3.905, 4.947, 3.018}; // regions: 10
    double xsec_rejectPion175std[]     = {3.893, 4.962, 3.009}; // 1.75
    double xsec_rejectPion225std[]     = {3.950, 4.967, 3.044}; // 2.25
    double xsec_rejectMuon175std[]     = {4.917, 4.501, 3.772}; // 1.75
    double xsec_rejectMuon225std[]     = {4.494, 5.592, 2.987}; // 2.25
    double xsec_rejectElectron175std[] = {4.181, 4.510, 2.658}; // 1.75
    double xsec_rejectElectron225std[] = {4.527, 5.591, 2.978}; // 2.25

    for (Int_t i = 1; i <= hMassPairKaonVsRapiditySub_data->GetNbinsX(); ++i)
    {
        cMassPairKaonY->cd(i);
        TH1D *hMassPairKaon_py_truth = hMassPairKaonVsRapidity_truth->ProjectionY(Form("_truth_%d", i), i, i);
        hMassPairKaon_py_truth->Draw();
        cMassPairKaonY->cd(i+3);
        TH1D *hMassPairKaon_py_mc = hMassPairKaonVsRapiditySub_mc->ProjectionY(Form("_mc_%d", i), i, i);
        hMassPairKaon_py_mc->Draw();
        cMassPairKaonY->cd(i+6);
        TH1D *hMassPairKaon_py_data = hMassPairKaonVsRapiditySub_data->ProjectionY(Form("_data_%d", i), i, i);
        hMassPairKaon_py_data->Draw();

        double ymin = hMassPairKaonVsRapiditySub_data->GetXaxis()->GetBinLowEdge(i);
        double ymax = hMassPairKaonVsRapiditySub_data->GetXaxis()->GetBinUpEdge(i);
        hMassPairKaon_py_truth->SetTitle(Form("%.2f<y<%.2f;m_{K^{+}K^{-}} (GeV/c^{2});Counts",ymin,ymax));
        hMassPairKaon_py_mc->SetTitle(Form("%.2f<y<%.2f;m_{K^{+}K^{-}} (GeV/c^{2});Counts",ymin,ymax));
        hMassPairKaon_py_data->SetTitle(Form("%.2f<y<%.2f;m_{K^{+}K^{-}} (GeV/c^{2});Counts",ymin,ymax));

        double eff = 0, deff = 0, xsec_kpkm = 0, dxsec_kpkm_stat = 0;
        double lumi_kpkm = 505; // mili barn^{-1}
        double fpileup = 1.111; // from Rho(770) analysis
        double Nsig_truth = hMassPairKaon_py_truth->Integral();
        double dNsig_mc;
        double Nsig_mc = hMassPairKaon_py_mc->IntegralAndError(1, hMassPairKaon_py_mc->GetNbinsX(), dNsig_mc, "");
        double dNsig_data;
        double Nsig_data = hMassPairKaon_py_data->IntegralAndError(1, hMassPairKaon_py_data->GetNbinsX(), dNsig_data, "");
        double pairRapidity = hMassPairKaonVsRapiditySub_data->GetXaxis()->GetBinCenter(i);

        eff = Nsig_mc / Nsig_truth; // Efficiency = N_observed/N_generated
        deff = eff * (dNsig_mc / Nsig_mc);

        // -------------------- Systematic Errors --------------------
        double xsec_pt[]             = {xsec_optimal[i-1], xsec_pt011[i-1], xsec_pt009[i-1]};
        double xsec_rad[]            = {xsec_optimal[i-1], xsec_rad8[i-1], xsec_rad10[i-1]};
        double xsec_rejectPion[]     = {xsec_optimal[i-1], xsec_rejectPion175std[i-1], xsec_rejectPion225std[i-1]};
        double xsec_rejectMuon[]     = {xsec_optimal[i-1], xsec_rejectMuon175std[i-1], xsec_rejectMuon225std[i-1]};
        double xsec_rejectElectron[] = {xsec_optimal[i-1], xsec_rejectElectron175std[i-1], xsec_rejectElectron225std[i-1]};
        double err_xsec_pt             = TMath::RMS(3, xsec_pt)/xsec_optimal[i-1];
        double err_xsec_rad            = TMath::RMS(3, xsec_rad)/xsec_optimal[i-1];
        double err_xsec_rejectPion     = TMath::RMS(3, xsec_rejectPion)/xsec_optimal[i-1];
        double err_xsec_rejectMuon     = TMath::RMS(3, xsec_rejectMuon)/xsec_optimal[i-1];
        double err_xsec_rejectElectron = TMath::RMS(3, xsec_rejectElectron)/xsec_optimal[i-1];

        // from rho0 analysis
        double err_trk_select  = 0.015;
        double err_trk_match  = 0.040;
        double err_trk_eff  = 0.010;
        double err_SPD_chip  = 0.010;
        double err_PileUp  = 0.038;
        double err_Lumi  = 0.050;

        double err_sys = TMath::Sqrt(err_xsec_pt*err_xsec_pt + err_xsec_rad*err_xsec_rad + err_xsec_rejectPion*err_xsec_rejectPion + err_xsec_rejectMuon*err_xsec_rejectMuon + err_xsec_rejectElectron*err_xsec_rejectElectron + err_trk_select*err_trk_select + err_trk_match*err_trk_match + err_trk_eff*err_trk_eff + err_SPD_chip*err_SPD_chip + err_PileUp*err_PileUp + err_Lumi*err_Lumi);

        xsec_kpkm = (Nsig_data * fpileup) / (eff * lumi_kpkm);
        dxsec_kpkm_stat = xsec_kpkm * (dNsig_data / Nsig_data);
        double dxsec_phi2pi_sys = xsec_kpkm * TMath::Sqrt((err_sys*err_sys));

        // gdxsectiondy->SetPoint(i-1, pairRapidity, xsec_kpkm);
        // gdxsectiondy->SetPointError(i-1, 0, dxsec_kpkm_stat);

        // gdxsectiondy->SetPoint(i-1, pairRapidity, xsec_kpkm);
        // gdxsectiondy->SetPointError(i-1, 2, 0, 0, dxsec_kpkm_stat, dxsec_kpkm_stat, dxsec_phi2pi_sys, dxsec_phi2pi_sys);

        x[i-1]       = pairRapidity;
        y[i-1]       = xsec_kpkm;
        exl[i-1]     = hMassPairKaonVsRapiditySub_data->GetXaxis()->GetBinWidth(i)/2;
        exh[i-1]     = hMassPairKaonVsRapiditySub_data->GetXaxis()->GetBinWidth(i)/2;
        eylstat[i-1] = dxsec_kpkm_stat;
        eyhstat[i-1] = dxsec_kpkm_stat;
        eylsys[i-1]  = dxsec_phi2pi_sys;
        eyhsys[i-1]  = dxsec_phi2pi_sys;

        fprintf(tab_sys_y, "(%0.2f , %0.2f) & %0.1f & %0.1f & %0.1f & %0.1f & %0.1f \\\\ \n", ymin, ymax, err_xsec_pt * 100, err_xsec_rad * 100, err_xsec_rejectPion * 100, err_xsec_rejectMuon * 100, err_xsec_rejectElectron * 100);

        // cout<<" i = "<<i<<" | pairRapidity = "<<pairRapidity<<" | xsec_kpkm = "<<xsec_kpkm<<endl;
        // printf(" i = %i | pairRapidity = %f | xsec_kpkm = %f", i, pairRapidity, xsec_kpkm);

        // printf (" ####### bins: %d, %d, %d, %d, %d, %d \n", hMassPairKaonVsRapiditySub_data->GetNbinsX(), hMassPairKaonVsRapidity_truth->GetNbinsX(), hMassPairKaonVsRapiditySub_mc->GetNbinsX(), hMassPairKaon_py_truth->GetNbinsX(), hMassPairKaon_py_mc->GetNbinsX(), hMassPairKaon_py_data->GetNbinsX());
        // printf("Nsig_truth = %f, Nsig_mc = %f, Nsig_data = %f, eff = %f, xsec_kpkm = %f \n", Nsig_truth, Nsig_mc, Nsig_data, eff, xsec_kpkm);
        // printf(" i = %i | pairRapidity = %f | lumi_kpkm = %f |xsec_kpkm = %f", i, pairRapidity, lumi_kpkm, xsec_kpkm);

        printf(" ####### ymin, ymaxt = %0.3f , %0.3f \n", ymin, ymax);
        printf(" ####### dxsec_kpkm_stat = %0.3f \n", dxsec_kpkm_stat);
        printf(" ####### dxsec_phi2pi_sys = %0.3f \n", dxsec_phi2pi_sys);
    }
    
    TCanvas *cdxsectiondy = new TCanvas("cdxsectiondy", "cdxsectiondy", 900, 600);
    cdxsectiondy->cd();

    // cdxsectiondy->SetLogy();
    // gdxsectiondy->Draw("AP");

    TGraphMultiErrors* gdxsectiondy = new TGraphMultiErrors("gdxsectiondy", "", np, x, y, exl, exh, eylstat, eyhstat);
    gdxsectiondy->AddYError(np, eylsys, eyhsys);
    gdxsectiondy->SetMarkerStyle(20);
    gdxsectiondy->SetMinimum(0.0);
    gdxsectiondy->SetTitle("; y; d\\sigma/dy (mb)");
    // gdxsectiondy->SetLineColor(kRed);
    // gdxsectiondy->GetAttLine(0)->SetLineColor(kRed);
    gdxsectiondy->GetAttLine(1)->SetLineColor(kGreen);
    gdxsectiondy->GetAttFill(1)->SetFillStyle(1);
    // Graph and x erros drawn with "APS"
    // Stat Errors drawn with "Z"
    // Sys Errors drawn with "5 s=0.5"
    gdxsectiondy->Draw("APS ; Z ; 5 s=1.0"); //APS ; Z ; 6 s=0.5
    cdxsectiondy->Update();

    gdxsectiondy->Write(); // Form("h%s_gdxsectiondy", name.Data()), TObject::kWriteDelete
    cdxsectiondy->Print(Form("%s/cdxsectiondy.root", outDir.Data()),"root");
    cdxsectiondy->Print(Form("%s/cdxsectiondy.pdf", outDir.Data()),"pdf");

    cMassPairKaonY->Print(Form("%s/cMassPairKaonY.root", outDir.Data()),"root");
    cMassPairKaonY->Print(Form("%s/cMassPairKaonY.pdf", outDir.Data()),"pdf");

    // cout<<"| hMassPairKaon_mc = "<<hMassPairKaon_mc->GetXaxis()->GetBinWidth(2)<<" | hMassPairKaonRejectAllSub_mc = "<<hMassPairKaonRejectAllSub_mc->GetXaxis()->GetBinWidth(2)<<endl;
    // cout<<"| hMassPairKaon_mc = "<<hMassPairKaon_mc->GetNbinsX()<<" | hMassPairKaonRejectAllSub_mc = "<<hMassPairKaonRejectAllSub_mc->GetNbinsX()<<endl;

    fprintf(tab_sys_y, "\\hline\n \\end{tabular}\n \\end{table}\n \\end{document}\n");
    fclose(tab_sys_y);
    gSystem->Exec(Form("pdflatex -output-directory %s %s/tab_sys_y.tex", outDir.Data(), outDir.Data()));

    printf(" ####### xsec = %0.3f, %0.3f, %0.3f \n", y[0], y[1], y[2]);
    // End: ====================  Cross section in rapidity bins ====================
*/    
}