#include "TFile.h"
#include "TH1F.h"
#include "TF1.h"
#include "TGraph.h"
#include "TGraphErrors.h"
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

using namespace std;

void Phi0Ana()
{
    // Open the file containing the tree.
    TFile *inputFile = TFile::Open("/home/nacer/upc/Phi0/350_20220224-1926/AnalysisResults_350.root");
    TString outDir = "/home/nacer/upc/Phi0Ana/output_Phi0Ana";

    TList* inputList = (TList*)inputFile->Get("Central/Histograms");
    // You can check whether the list was correctly retrieved with:
    if (inputList == nullptr)
        cout<<"something went wrong! abort!"<<endl;
    // If list is valid, you can loop over its contents to make sure they are what you expect:
    // for(auto obj: *inputList)
    //     cout << "obj in list: " << obj->GetName() << endl;

    TTree *inputTree = (TTree *)inputFile->Get("Central/Selected");
    if(inputTree == nullptr)
        cout<<"something went wrong! abort!"<<endl;
    
    // // read histograms inside TList
    // // TFile *f = TFile::Open("sample.root");
    // // TList *list = (TList*)f->Get("chist");
    // TH1F *fHistdEdxVsP_Kaon = (TH1F*)inputList->FindObject("fHistdEdxVsP_Kaon");

    //Save histograms to output ROOT file
    TFile *outputFile = new TFile(Form("%s/Phi0Ana.root",outDir.Data()), "RECREATE");

    gStyle->SetPalette(kRainBow); // choose different Palette color scheme
    gStyle->SetOptStat("e"); // remove stat box
    gStyle->SetTitleSize(.05, "xyz");
    gStyle->SetTitleOffset(1.1, "y");
    gStyle->SetLabelSize(.05, "xyz");
    gStyle->SetPadTopMargin(0.12);
    gStyle->SetPadBottomMargin(0.15);
    gStyle->SetPadRightMargin(0.15); // 2d:0.17 , 1d:0.05
    gStyle->SetPadLeftMargin(0.15);  // 2d:0.16, 1d:0.15
    gStyle->SetLineWidth(2);
    gStyle->SetPadGridX(true);
    gStyle->SetPadGridY(true);

    // displace stat box
    // gStyle->SetOptFit();
    // c1->Modified(); c1->Update();
    // TPaveStats *stats =(TPaveStats*)c1->GetPrimitive("stats");
    // stats->SetName("h1stats");
    // stats->SetY1NDC(.4);
    // stats->SetY2NDC(.6);
    // stats->SetTextColor(2);

    // // create a tree file outputTree.root - create the file, the Tree and a few branches
    // TTree outputTree("tree2k", "track variable in K+K- mass regions");
    // Float_t px1, py1, pz1, px2, py2, pz2;
    // outputTree.Branch("px1", &px1, "px1/F");
    // outputTree.Branch("py1", &py1, "py1/F");
    // outputTree.Branch("pz1", &pz1, "pz1/F");
    // outputTree.Branch("px2", &px2, "px2/F");
    // outputTree.Branch("py2", &py2, "py2/F");
    // outputTree.Branch("pz2", &pz2, "pz2/F");
   
    // define histograms to visualze variables

    // -------------- Number of Tracks --------------
    auto cNumbTracks = new TCanvas("cNumbTracks", "cNumbTracks", 600, 400);
    cNumbTracks->cd();
    cNumbTracks->SetLogy();
    auto hNumbTracks = new TH1F("hNumbTracks", "; Number of Tracks; Counts", 5.,0.,5.);
    hNumbTracks = (TH1F*)(inputList->FindObject("fGoodTracks"));
    // h_NumbTracks->GetXaxis()->SetNdivisions(520);
    hNumbTracks->Draw();
    hNumbTracks->Write();
    cNumbTracks->Print(Form("%s/cNumbTracks.root", outDir.Data()),"root");
    cNumbTracks->Print(Form("%s/cNumbTracks.pdf", outDir.Data()),"pdf");

    // -------------- Pair Azimuthal Angle --------------
    auto cAzimuthal = new TCanvas("cAzimuthal", "cAzimuthal", 600, 400);
    cAzimuthal->cd();
    auto hAzimuthal = new TH1F("hAzimuthal", ";#phi (rad);Counts", 300,-TMath::Pi(), TMath::Pi());
    inputTree->Project("hAzimuthal", "Phi_T");
    hAzimuthal->SetMinimum(0.);
    // h_Azimuthal->GetXaxis()->SetNdivisions(520);
    hAzimuthal->Draw();
    hAzimuthal->Write();
    cAzimuthal->Print(Form("%s/cAzimuthal.root", outDir.Data()),"root");
    cAzimuthal->Print(Form("%s/cAzimuthal.pdf", outDir.Data()),"pdf");

    // -------------- Track PseudoRapidity --------------
    auto cTrack1vs2PseudoRapidity = new TCanvas("cTrack1vs2PseudoRapidity", "cTrack1vs2PseudoRapidity", 600, 400);
    cTrack1vs2PseudoRapidity->cd();
    auto hTrack1vs2PseudoRapidity = new TH2D("hTrack1vs2PseudoRapidity", ";#eta_{1};#eta_{2}", 100,-1.,1., 100,-1.,1.);
    inputTree->Project("hTrack1vs2PseudoRapidity", "TrackEta_T[1]:TrackEta_T[0]");
    hTrack1vs2PseudoRapidity->Draw("colz");
    hTrack1vs2PseudoRapidity->Write();
    cTrack1vs2PseudoRapidity->Print(Form("%s/cTrack1vs2PseudoRapidity.root", outDir.Data()),"root");
    cTrack1vs2PseudoRapidity->Print(Form("%s/cTrack1vs2PseudoRapidity.pdf", outDir.Data()),"pdf");

    // -------------- Primary Vertex Z --------------
    auto cVertexZ = new TCanvas("cVertexZ", "cVertexZ", 600, 400);
    cVertexZ->cd();
    auto hVertexZ = new TH1F("hVertexZ", ";#phi (rad);Counts", 300,-20., 20);
    inputTree->Project("hVertexZ", "VtxZ_T");
    hVertexZ->SetMinimum(0.);
    // h_VertexZ->GetXaxis()->SetNdivisions(520);
    hVertexZ->Draw();
    hVertexZ->Write();
    cVertexZ->Print(Form("%s/cVertexZ.root", outDir.Data()),"root");
    cVertexZ->Print(Form("%s/cVertexZ.pdf", outDir.Data()),"pdf");

    // -------------- Invariant Mass --------------

    //**** Kaons
    auto hMassPairKaon = new TH1F("hMassPairKaon", "; m_{K^{+}K^{-}} (GeV/c^{2}); Counts", 300, 0.95, 2.0);
    inputTree->Project("hMassPairKaon","MassPairKaon_T");
    // 2 pions of different charge 
    auto hMassPairKaonUnLikeCharge = new TH1F("hMassPairKaonUnLikeCharge", "; m_{K^{+}K^{-}} (GeV/c^{2}); Counts", 300, 0.95, 2.0);
    inputTree->Project("hMassPairKaonUnLikeCharge", "MassPairKaon_T", "LikeSign_T == 0");
    hMassPairKaonUnLikeCharge->SetLineColor(6);
    // 2 pions of same charge
    auto hMassPairKaonLikeCharge = new TH1F("hMassPairKaonLikeCharge", "; m_{K^{+}K^{-}} (GeV/c^{2}); Counts", 300, 0.95, 2.0);
    inputTree->Project("hMassPairKaonLikeCharge", "MassPairKaon_T", "LikeSign_T == 1");
    hMassPairKaonLikeCharge->SetLineColor(7);
    // dE/dx cut
    auto hMassPairKaondEdxCut = new TH1F("hMassPairKaondEdxCut", "; m_{K^{+}K^{-}} (GeV/c^{2}); Counts", 300, 0.95, 2.0);
    inputTree->Project("hMassPairKaondEdxCut", "MassPairKaon_T", "LikeSign_T == 0 && abs(PIDTPCKaon_T[0]) < 3 && abs(PIDTPCKaon_T[1]) < 3");
    hMassPairKaondEdxCut->SetLineColor(28);
    // Pt cut < 0.1 GeV
    auto hMassPairKaonPtCut01 = new TH1F("hMassPairKaonPtCut01", "; m_{K^{+}K^{-}} (GeV/c^{2}); Counts", 300, 0.95, 2.0);
    inputTree->Project("hMassPairKaonPtCut01", "MassPairKaon_T", "LikeSign_T == 0 && abs(PIDTPCKaon_T[0]) < 3 && abs(PIDTPCKaon_T[1]) < 3 && Pt_T < 0.1");
    hMassPairKaonPtCut01->SetLineColor(1);
    // // Pt cut < 0.1 GeV
    // auto hMassPairKaonPtCut01bkg = new TH1F("hMassPairKaonPtCut01bkg", "; m_{K^{+}K^{-}} (GeV/c^{2}); Counts", 300, 0.95, 2.0);
    // inputTree->Project("hMassPairKaonPtCut01bkg", "MassPairKaon_T", "LikeSign_T == 0 && abs(PIDTPCKaon_T[0]) < 3 && abs(PIDTPCKaon_T[1]) < 3 && Pt_T > 0.1");
    // hMassPairKaonPtCut01bkg->SetLineColor(2);

    auto cMassPairKaon = new TCanvas("cMassPairKaon", "cMassPairKaon", 600, 400);
    cMassPairKaon->cd();
    // cMassPairKaon->SetLogy();
    auto lMassPairKaon = new TLegend(0.6, 0.7, 0.8, 0.9);
    lMassPairKaon->SetTextSize(0.03);
    lMassPairKaon->SetBorderSize(0);
    hMassPairKaon->SetMinimum(1.0);
    hMassPairKaon->Draw();
    hMassPairKaonUnLikeCharge->Draw("same");
    hMassPairKaonLikeCharge->Draw("same");
    hMassPairKaondEdxCut->Draw("same");
    hMassPairKaonPtCut01->Draw("esame");
    // hMassPairKaonPtCut01bkg->Draw("same");
    lMassPairKaon->AddEntry(hMassPairKaon, "2 track events", "l");
    lMassPairKaon->AddEntry(hMassPairKaonUnLikeCharge, "different charge", "l");
    lMassPairKaon->AddEntry(hMassPairKaonLikeCharge, "same charge", "l");
    lMassPairKaon->AddEntry(hMassPairKaondEdxCut, "-3<#sigma^{K^{#pm}}_{dE/dx}<3 + opposite charge", "lep");
    lMassPairKaon->AddEntry(hMassPairKaonPtCut01, "-3<#sigma^{K^{#pm}}_{dE/dx}<3 + opposite charge + p_{T}<0.1 GeV/c", "lep");
    // lMassPairKaon->AddEntry(hMassPairKaonPtCut01bkg, "-3<#sigma^{K^{#pm}}_{dE/dx}<3 + opposite charge + p_{T}>0.1 GeV/c", "lep");
    lMassPairKaon->Draw();
    hMassPairKaon->Write();
    hMassPairKaonUnLikeCharge->Write();
    hMassPairKaonLikeCharge->Write();
    hMassPairKaondEdxCut->Write();
    hMassPairKaonPtCut01->Write();
    // hMassPairKaonPtCut01bkg->Write();
    cMassPairKaon->Print(Form("%s/cMassPairKaon.root", outDir.Data()),"root");
    cMassPairKaon->Print(Form("%s/cMassPairKaon.pdf", outDir.Data()),"pdf");

    // ---------------------------- select kaon with other particles rejected -------------------------------

    // s.t.d dE/dx cut to reject Pion
    auto hMassPairKaonRejectPion = new TH1F("hMassPairKaonRejectPion", "; m_{K^{+}K^{-}} (GeV/c^{2}); Counts", 300, 0.95, 2.0);
    inputTree->Project("hMassPairKaonRejectPion", "MassPairKaon_T", "LikeSign_T == 0 && abs(PIDTPCPion_T[0]) > 2 && abs(PIDTPCPion_T[1]) > 2");
    hMassPairKaonRejectPion->SetLineColor(kBlue);

    // s.t.d dE/dx cut to reject Pion + Muon
    auto hMassPairKaonRejectPionMuon = new TH1F("hMassPairKaonRejectPionMuon", "; m_{K^{+}K^{-}} (GeV/c^{2}); Counts", 300, 0.95, 2.0);
    inputTree->Project("hMassPairKaonRejectPionMuon", "MassPairKaon_T", "LikeSign_T == 0 && abs(PIDTPCPion_T[0]) > 2 && abs(PIDTPCPion_T[1]) > 2 && abs(PIDTPCMuon_T[0]) > 2 && abs(PIDTPCMuon_T[1]) > 2");
    hMassPairKaonRejectPionMuon->SetLineColor(7);

    // s.t.d dE/dx cut to reject Pion + Muon + Electron
    auto hMassPairKaonRejectAll = new TH1F("hMassPairKaonRejectAll", "; m_{K^{+}K^{-}} (GeV/c^{2}); Counts", 300, 0.95, 2.0);
    inputTree->Project("hMassPairKaonRejectAll", "MassPairKaon_T", "LikeSign_T == 0 && abs(PIDTPCPion_T[0]) > 2 && abs(PIDTPCPion_T[1]) > 2 && abs(PIDTPCMuon_T[0]) > 2 && abs(PIDTPCMuon_T[1]) > 2 && abs(PIDTPCElectron_T[0]) > 2 && abs(PIDTPCElectron_T[1]) > 2");
    hMassPairKaonRejectAll->SetLineColor(28);

    // s.t.d dE/dx cut to reject Pion + Muon + Electron and keep kaon
    auto hMassPairKaon2Std = new TH1F("hMassPairKaon2Std", "; m_{K^{+}K^{-}} (GeV/c^{2}); Counts", 300, 0.95, 2.0);
    inputTree->Project("hMassPairKaon2Std", "MassPairKaon_T", "LikeSign_T == 0 && abs(PIDTPCPion_T[0]) > 2 && abs(PIDTPCPion_T[1]) > 2 && abs(PIDTPCMuon_T[0]) > 2 && abs(PIDTPCMuon_T[1]) > 2 && abs(PIDTPCElectron_T[0]) > 2 && abs(PIDTPCElectron_T[1]) > 2 && abs(PIDTPCKaon_T[0]) < 2 && abs(PIDTPCKaon_T[1]) < 2");
    hMassPairKaon2Std->SetLineColor(1);
    
    auto cMassPairKaonSelect = new TCanvas("cMassPairKaonSelect", "cMassPairKaonSelect", 600, 400);
    cMassPairKaonSelect->cd();
    cMassPairKaonSelect->SetLogy();
    auto lMassPairKaonSelect = new TLegend(0.6, 0.6, 0.8, 0.9);
    lMassPairKaonSelect->SetTextSize(0.03);
    lMassPairKaonSelect->SetBorderSize(0);
    hMassPairKaonUnLikeCharge->SetMinimum(1.0);
    // hMassPairKaonUnLikeCharge->Rebin(2);
    hMassPairKaonUnLikeCharge->Draw();
    hMassPairKaonRejectPion->Draw("same");
    hMassPairKaonRejectPionMuon->Draw("same");
    hMassPairKaonRejectAll->Draw("same");
    hMassPairKaon2Std->Draw("same");
    lMassPairKaonSelect->AddEntry(hMassPairKaonUnLikeCharge, "opposite charge", "l");
    lMassPairKaonSelect->AddEntry(hMassPairKaonRejectPion, "-2>#sigma^{#pi}_{dE/dx}>2", "l");
    lMassPairKaonSelect->AddEntry(hMassPairKaonRejectPionMuon, "-2>#sigma^{(#pi, #mu)}_{dE/dx}>2", "l");
    lMassPairKaonSelect->AddEntry(hMassPairKaonRejectAll, "-2>#sigma^{(#pi, #mu, e)}_{dE/dx}>2", "l");
    lMassPairKaonSelect->AddEntry(hMassPairKaon2Std, "-2>#sigma^{(#pi, #mu, e)}_{dE/dx}>2 &  -2<#sigma^{K}_{dE/dx}<2", "l");
    lMassPairKaonSelect->Draw();
    hMassPairKaonRejectPion->Write();
    hMassPairKaonRejectPionMuon->Write();
    hMassPairKaonRejectAll->Write();
    hMassPairKaon2Std->Write();
    cMassPairKaonSelect->Print(Form("%s/cMassPairKaonSelect.root", outDir.Data()),"root");
    cMassPairKaonSelect->Print(Form("%s/cMassPairKaonSelect.pdf", outDir.Data()),"pdf");
    
    // --------------------------------------------------------------------------------------------------

    // **** Pions
    auto hMassPairPion = new TH1F("hMassPairPion", "; m_{#pi^{+}#pi^{-}} (GeV/c^{2}); Counts", 300, 0.3, 1.4);
    inputTree->Project("hMassPairPion","MassPairPion_T");
    // 2 pions of different charge 
    auto hMassPairPionUnLikeCharge = new TH1F("hMassPairPionUnLikeCharge", "; m_{#pi^{+}#pi^{-}} (GeV/c^{2}); Counts", 300, 0.3, 1.4);
    inputTree->Project("hMassPairPionUnLikeCharge", "MassPairPion_T", "LikeSign_T == 0");
    hMassPairPionUnLikeCharge->SetLineColor(6);
    // 2 pions of same charge
    auto hMassPairPionLikeCharge = new TH1F("hMassPairPionLikeCharge", "; m_{#pi^{+}#pi^{-}} (GeV/c^{2}); Counts", 300, 0.3, 1.4);
    inputTree->Project("hMassPairPionLikeCharge", "MassPairPion_T", "LikeSign_T == 1");
    hMassPairPionLikeCharge->SetLineColor(7);
    // dE/dx cut
    auto hMassPairPiondEdxCut = new TH1F("hMassPairPiondEdxCut", "; m_{#pi^{+}#pi^{-}} (GeV/c^{2}); Counts", 300, 0.3, 1.4);
    inputTree->Project("hMassPairPiondEdxCut", "MassPairPion_T", "LikeSign_T == 0 && abs(PIDTPCPion_T[0]) < 3 && abs(PIDTPCPion_T[1]) < 3");
    hMassPairPiondEdxCut->SetLineColor(28);
    // Pt cut < 0.1 GeV
    auto hMassPairPionPtCut01 = new TH1F("hMassPairPionPtCut01", "; m_{#pi^{+}#pi^{-}} (GeV/c^{2}); Counts", 300, 0.3, 1.4);
    inputTree->Project("hMassPairPionPtCut01", "MassPairPion_T", "LikeSign_T == 0 && abs(PIDTPCPion_T[0]) < 3 && abs(PIDTPCPion_T[1]) < 3 && Pt_T < 0.1");
    hMassPairPionPtCut01->SetLineColor(1);
    // Pt cut > 0.1 GeV
    auto hMassPairPionPtCut01bkg = new TH1F("hMassPairPionPtCut01bkg", "; m_{#pi^{+}#pi^{-}} (GeV/c^{2}); Counts", 300, 0.3, 1.4);
    inputTree->Project("hMassPairPionPtCut01bkg", "MassPairPion_T", "LikeSign_T == 0 && abs(PIDTPCPion_T[0]) < 3 && abs(PIDTPCPion_T[1]) < 3 && Pt_T > 0.1");
    hMassPairPionPtCut01bkg->SetLineColor(2);

    auto cMassPairPion = new TCanvas("cMassPairPion", "cMassPairPion", 600, 400);
    cMassPairPion->cd();
    // cMassPairPion->SetLogy();
    auto lMassPairPion = new TLegend(0.5, 0.5, 0.7, 0.85);
    lMassPairPion->SetTextSize(0.03);
    lMassPairPion->SetBorderSize(0);
    // hMassPairPionPairPion->SetMinimum(1.0);
    hMassPairPion->Draw();
    hMassPairPionUnLikeCharge->Draw("same");
    hMassPairPionLikeCharge->Draw("same");
    hMassPairPiondEdxCut->Draw("same");
    hMassPairPionPtCut01->Draw("esame");
    hMassPairPionPtCut01bkg->Draw("same");
    lMassPairPion->AddEntry(hMassPairPion, "2 track events", "l");
    lMassPairPion->AddEntry(hMassPairPionUnLikeCharge, "different charge", "l");
    lMassPairPion->AddEntry(hMassPairPionLikeCharge, "same charge", "l");
    lMassPairPion->AddEntry(hMassPairPiondEdxCut, "-3<#sigma^{#pi^{#pm}}_{dE/dx}<3 + opposite charge", "l");
    lMassPairPion->AddEntry(hMassPairPionPtCut01, "-3<#sigma^{#pi^{#pm}}_{dE/dx}<3 + opposite charge + p_{T}<0.1 GeV/c", "lep");
    lMassPairPion->AddEntry(hMassPairPionPtCut01bkg, "-3<#sigma^{#pi^{#pm}}_{dE/dx}<3 + opposite charge + p_{T}>0.1 GeV/c", "l");
    lMassPairPion->Draw();
    hMassPairPion->Write();
    hMassPairPionUnLikeCharge->Write();
    hMassPairPionLikeCharge->Write();
    hMassPairPiondEdxCut->Write();
    hMassPairPionPtCut01->Write();
    hMassPairPionPtCut01bkg->Write();
    cMassPairPion->Print(Form("%s/cMassPairPion.root", outDir.Data()),"root");
    cMassPairPion->Print(Form("%s/cMassPairPion.pdf", outDir.Data()),"pdf");

    // **** Muons
    auto hMassPairMuon = new TH1F("hMassPairMuon", "; m_{#mu^{+}#mu^{-}} (GeV/c^{2}); Counts", 300, 0.2, 1.4);
    inputTree->Project("hMassPairMuon","MassPairMuon_T");
    // 2 Muons of different charge 
    auto hMassPairMuonUnLikeCharge = new TH1F("hMassPairMuonUnLikeCharge", "; m_{#mu^{+}#mu^{-}} (GeV/c^{2}); Counts", 300, 0.2, 1.4);
    inputTree->Project("hMassPairMuonUnLikeCharge", "MassPairMuon_T", "LikeSign_T == 0");
    hMassPairMuonUnLikeCharge->SetLineColor(6);
    // 2 Muons of same charge
    auto hMassPairMuonLikeCharge = new TH1F("hMassPairMuonLikeCharge", "; m_{#mu^{+}#mu^{-}} (GeV/c^{2}); Counts", 300, 0.2, 1.4);
    inputTree->Project("hMassPairMuonLikeCharge", "MassPairMuon_T", "LikeSign_T == 1");
    hMassPairMuonLikeCharge->SetLineColor(7);
    // dE/dx cut
    auto hMassPairMuondEdxCut = new TH1F("hMassPairMuondEdxCut", "; m_{#mu^{+}#mu^{-}} (GeV/c^{2}); Counts", 300, 0.2, 1.4);
    inputTree->Project("hMassPairMuondEdxCut", "MassPairMuon_T", "LikeSign_T == 0 && abs(PIDTPCMuon_T[0]) < 3 && abs(PIDTPCMuon_T[1]) < 3");
    hMassPairMuondEdxCut->SetLineColor(28);
    // Pt cut
    auto hMassPairMuonPtCut01 = new TH1F("hMassPairMuonPtCut01", "; m_{#mu^{+}#mu^{-}} (GeV/c^{2}); Counts", 300, 0.2, 1.4);
    inputTree->Project("hMassPairMuonPtCut01", "MassPairMuon_T", "LikeSign_T == 0 && abs(PIDTPCMuon_T[0]) < 3 && abs(PIDTPCMuon_T[1]) < 3 && Pt_T < 0.1");
    hMassPairMuonPtCut01->SetLineColor(1);

    auto cMassPairMuon = new TCanvas("cMassPairMuon", "cMassPairMuon", 600, 400);
    cMassPairMuon->cd();
    // cMassPairMuon->SetLogy();
    auto lMassPairMuon = new TLegend(0.6, 0.6, 0.7, 0.85);
    lMassPairMuon->SetTextSize(0.03);
    lMassPairMuon->SetBorderSize(0);
    // hMassPairMuonPairMuon->SetMinimum(1.0);
    hMassPairMuon->Draw();
    hMassPairMuonUnLikeCharge->Draw("same");
    hMassPairMuonLikeCharge->Draw("same");
    hMassPairMuondEdxCut->Draw("same");
    hMassPairMuonPtCut01->Draw("esame");
    lMassPairMuon->AddEntry(hMassPairMuon, "2 track events", "l");
    lMassPairMuon->AddEntry(hMassPairMuonUnLikeCharge, "different charge", "l");
    lMassPairMuon->AddEntry(hMassPairMuonLikeCharge, "same charge", "l");
    lMassPairMuon->AddEntry(hMassPairMuondEdxCut, "-3<#sigma_{dE/dx}<3 + opposite charge", "l");
    lMassPairMuon->AddEntry(hMassPairMuonPtCut01, "-3<#sigma_{dE/dx}<3 + opposite charge + p_{T}<0.1 GeV/c", "lep");
    lMassPairMuon->Draw();
    hMassPairMuon->Write();
    hMassPairMuonUnLikeCharge->Write();
    hMassPairMuonLikeCharge->Write();
    hMassPairMuondEdxCut->Write();
    hMassPairMuonPtCut01->Write();
    cMassPairMuon->Print(Form("%s/cMassPairMuon.root", outDir.Data()),"root");
    cMassPairMuon->Print(Form("%s/cMassPairMuon.pdf", outDir.Data()),"pdf");

    // **** Electrons
    auto hMassPairElectron = new TH1F("hMassPairElectron", "; m_{e^{+}e^{-}} (GeV/c^{2}); Counts", 300, 0.0, 1.4);
    inputTree->Project("hMassPairElectron","MassPairElectron_T");
    // 2 Electrons of different charge 
    auto hMassPairElectronUnLikeCharge = new TH1F("hMassPairElectronUnLikeCharge", "; m_{e^{+}e^{-}} (GeV/c^{2}); Counts", 300, 0.0, 1.4);
    inputTree->Project("hMassPairElectronUnLikeCharge", "MassPairElectron_T", "LikeSign_T == 0");
    hMassPairElectronUnLikeCharge->SetLineColor(6);
    // 2 Electrons of same charge
    auto hMassPairElectronLikeCharge = new TH1F("hMassPairElectronLikeCharge", "; m_{e^{+}e^{-}} (GeV/c^{2}); Counts", 300, 0.0, 1.4);
    inputTree->Project("hMassPairElectronLikeCharge", "MassPairElectron_T", "LikeSign_T == 1");
    hMassPairElectronLikeCharge->SetLineColor(7);
    // dE/dx cut
    auto hMassPairElectrondEdxCut = new TH1F("hMassPairElectrondEdxCut", "; m_{e^{+}e^{-}} (GeV/c^{2}); Counts", 300, 0.0, 1.4);
    inputTree->Project("hMassPairElectrondEdxCut", "MassPairElectron_T", "LikeSign_T == 0 && abs(PIDTPCElectron_T[0]) < 3 && abs(PIDTPCElectron_T[1]) < 3");
    hMassPairElectrondEdxCut->SetLineColor(28);
    // Pt cut
    auto hMassPairElectronPtCut01 = new TH1F("hMassPairElectronPtCut01", "; m_{e^{+}e^{-}} (GeV/c^{2}); Counts", 300, 0.0, 1.4);
    inputTree->Project("hMassPairElectronPtCut01", "MassPairElectron_T", "LikeSign_T == 0 && abs(PIDTPCElectron_T[0]) < 3 && abs(PIDTPCElectron_T[1]) < 3 && Pt_T < 0.1");
    hMassPairElectronPtCut01->SetLineColor(1);

    auto cMassPairElectron = new TCanvas("cMassPairElectron", "cMassPairElectron", 600, 400);
    cMassPairElectron->cd();
    // cMassPairElectron->SetLogy();
    auto lMassPairElectron = new TLegend(0.6, 0.6, 0.7, 0.85);
    lMassPairElectron->SetTextSize(0.03);
    lMassPairElectron->SetBorderSize(0);
    // hMassPairElectronPairElectron->SetMinimum(1.0);
    hMassPairElectron->Draw();
    hMassPairElectronUnLikeCharge->Draw("same");
    hMassPairElectronLikeCharge->Draw("same");
    hMassPairElectrondEdxCut->Draw("same");
    hMassPairElectronPtCut01->Draw("esame");
    lMassPairElectron->AddEntry(hMassPairElectron, "2 track events", "l");
    lMassPairElectron->AddEntry(hMassPairElectronUnLikeCharge, "different charge", "l");
    lMassPairElectron->AddEntry(hMassPairElectronLikeCharge, "same charge", "l");
    lMassPairElectron->AddEntry(hMassPairElectrondEdxCut, "-3<#sigma_{dE/dx}<3 + opposite charge", "l");
    lMassPairElectron->AddEntry(hMassPairElectronPtCut01, "-3<#sigma_{dE/dx}<3 + opposite charge + p_{T}<0.1 GeV/c", "lep");
    lMassPairElectron->Draw();
    hMassPairElectron->Write();
    hMassPairElectronUnLikeCharge->Write();
    hMassPairElectronLikeCharge->Write();
    hMassPairElectrondEdxCut->Write();
    hMassPairElectronPtCut01->Write();
    cMassPairElectron->Print(Form("%s/cMassPairElectron.root", outDir.Data()),"root");
    cMassPairElectron->Print(Form("%s/cMassPairElectron.pdf", outDir.Data()),"pdf");

    // ***** correlations
    
    // Mass K+K- vs Pi+Pi-
    auto cMassPairKaonVsPion = new TCanvas("cMassPairKaonVsPion", "cMassPairKaonVsPion", 600, 400);
    cMassPairKaonVsPion->cd();
    auto hMassPairKaonVsPion = new TH2D("hMassPairKaonVsPion", "; m_{K^{+}K^{-}} (GeV/c^{2}) ; m_{#pi^{+}#pi^{-}} (GeV/c^{2})", 300, 0.95, 2.0, 300, 0.3, 2.0);
    inputTree->Project("hMassPairKaonVsPion", "MassPairPion_T:MassPairKaon_T", "LikeSign_T == 0 && abs(PIDTPCKaon_T[0]) < 3 && abs(PIDTPCKaon_T[1]) < 3 && abs(PIDTPCPion_T[0]) < 3 && abs(PIDTPCPion_T[1]) < 3");
    hMassPairKaonVsPion->Draw("colz");
    hMassPairKaonVsPion->Write();
    cMassPairKaonVsPion->Print(Form("%s/cMassPairKaonVsPion.root", outDir.Data()),"root");
    cMassPairKaonVsPion->Print(Form("%s/cMassPairKaonVsPion.pdf", outDir.Data()),"pdf");

    // Mass K+K- vs mu+mu-
    auto cMassPairKaonVsMuon = new TCanvas("cMassPairKaonVsMuon", "cMassPairKaonVsMuon", 600, 400);
    cMassPairKaonVsMuon->cd();
    auto hMassPairKaonVsMuon = new TH2D("hMassPairKaonVsMuon", "; m_{K^{+}K^{-}} (GeV/c^{2}) ; m_{#mu^{+}#mu^{-}} (GeV/c^{2})", 300, 0.95, 2.0, 300, 0.2, 2.0);
    inputTree->Project("hMassPairKaonVsMuon", "MassPairMuon_T:MassPairKaon_T", "LikeSign_T == 0 && abs(PIDTPCKaon_T[0]) < 3 && abs(PIDTPCKaon_T[1]) < 3 && abs(PIDTPCMuon_T[0]) < 3 && abs(PIDTPCMuon_T[1]) < 3");
    hMassPairKaonVsMuon->Draw("colz");
    hMassPairKaonVsMuon->Write();
    cMassPairKaonVsMuon->Print(Form("%s/cMassPairKaonVsMuon.root", outDir.Data()),"root");
    cMassPairKaonVsMuon->Print(Form("%s/cMassPairKaonVsMuon.pdf", outDir.Data()),"pdf");

    // Mass K+K- vs e+e-
    auto cMassPairKaonVsElectron = new TCanvas("cMassPairKaonVsElectron", "cMassPairKaonVsElectron", 600, 400);
    cMassPairKaonVsElectron->cd();
    auto hMassPairKaonVsElectron = new TH2D("hMassPairKaonVsElectron", "; m_{K^{+}K^{-}} (GeV/c^{2}) ; m_{e^{+}e^{-}} (GeV/c^{2})", 300, 0.95, 2.0, 300, 0.0, 2.0);
    inputTree->Project("hMassPairKaonVsElectron", "MassPairElectron_T:MassPairKaon_T", "LikeSign_T == 0 && abs(PIDTPCKaon_T[0]) < 3 && abs(PIDTPCKaon_T[1]) < 3 && abs(PIDTPCElectron_T[0]) < 3 && abs(PIDTPCElectron_T[1]) < 3");
    hMassPairKaonVsElectron->Draw("colz");
    hMassPairKaonVsElectron->Write();
    cMassPairKaonVsElectron->Print(Form("%s/cMassPairKaonVsElectron.root", outDir.Data()),"root");
    cMassPairKaonVsElectron->Print(Form("%s/cMassPairKaonVsElectron.pdf", outDir.Data()),"pdf");

    // -------------- Pair transverse momentum --------------

    auto hPtPair = new TH1F("hPtPair", "; p_{T} (GeV/c); Counts", 300, 0., 3.);
    inputTree->Project("hPtPair","Pt_T");
    // 2 tracks of different charge 
    auto hPtUnLikeCharge = new TH1F("hPtUnLikeCharge", "; p_{T} (GeV/c); Counts", 300, 0., 3.);
    inputTree->Project("hPtUnLikeCharge", "Pt_T", "LikeSign_T == 0");
    hPtUnLikeCharge->SetLineColor(6);
    // 2 tracks of same charge
    auto hPtLikeCharge = new TH1F("hPtLikeCharge", "; p_{T} (GeV/c); Counts", 300, 0., 3.);
    inputTree->Project("hPtLikeCharge", "Pt_T", "LikeSign_T == 1");
    hPtLikeCharge->SetLineColor(7);
    // Pion dE/dx cut
    auto hPtPiondEdxCut = new TH1F("hPtPiondEdxCut", "; p_{T} (GeV/c); Counts", 300, 0., 3.);
    inputTree->Project("hPtPiondEdxCut", "Pt_T", "LikeSign_T == 0 && abs(PIDTPCPion_T[0]) < 3 && abs(PIDTPCPion_T[1]) < 3");
    hPtPiondEdxCut->SetLineColor(28);
    // Kaon dE/dx cut
    auto hPtKaondEdxCut = new TH1F("hPtKaondEdxCut", "; p_{T} (GeV/c); Counts", 300, 0.0, 3.0);
    inputTree->Project("hPtKaondEdxCut", "Pt_T", "LikeSign_T == 0 && abs(PIDTPCKaon_T[0]) < 3 && abs(PIDTPCKaon_T[1]) < 3");
    hPtKaondEdxCut->SetLineColor(46);

    auto cPt = new TCanvas("cPt", "cPt", 600, 400);
    cPt->cd();
    cPt->SetLogy();
    auto lPt = new TLegend(0.5, 0.5, 0.7, 0.85);
    lPt->SetTextSize(0.03);
    lPt->SetBorderSize(0);
    // hPtPair->SetMinimum(1.0);
    hPtPair->Draw();
    hPtUnLikeCharge->Draw("same");
    hPtLikeCharge->Draw("same");
    hPtPiondEdxCut->Draw("same");
    hPtKaondEdxCut->Draw("same");
    lPt->AddEntry(hPtPair, "2 track events", "l");
    lPt->AddEntry(hPtUnLikeCharge, "different charge", "l");
    lPt->AddEntry(hPtLikeCharge, "same charge", "l");
    lPt->AddEntry(hPtPiondEdxCut, "-3<#sigma^{#pi^{#pm}}_{dE/dx}<3 + opposite charge", "l");
    lPt->AddEntry(hPtKaondEdxCut, "-3<#sigma^{K^{#pm}}_{dE/dx}<3 + opposite charge", "l");
    lPt->Draw();
    hPtPair->Write();
    hPtUnLikeCharge->Write();
    hPtLikeCharge->Write();
    hPtPiondEdxCut->Write();
    hPtKaondEdxCut->Write();
    cPt->Print(Form("%s/cPt.root", outDir.Data()),"root");
    cPt->Print(Form("%s/cPt.pdf", outDir.Data()),"pdf");

    // ******** select kaon with other particles rejected

    auto hPtPairKaon2Std = new TH1F("hPtPairKaon2Std", "; p_{T} (GeV/c); Counts", 100, 0., 3.);
    inputTree->Project("hPtPairKaon2Std","Pt_T", "LikeSign_T == 0 && abs(PIDTPCPion_T[0]) > 2 && abs(PIDTPCPion_T[1]) > 2 && abs(PIDTPCMuon_T[0]) > 2 && abs(PIDTPCMuon_T[1]) > 2 && abs(PIDTPCElectron_T[0]) > 2 && abs(PIDTPCElectron_T[1]) > 2 && abs(PIDTPCKaon_T[0]) < 2 && abs(PIDTPCKaon_T[1]) < 2");
    // 2 tracks of different charge 
    auto hPtPairKaon2StdLowMass = new TH1F("hPtPairKaon2StdLowMass", "; p_{T} (GeV/c); Counts", 100, 0., 3.);
    inputTree->Project("hPtPairKaon2StdLowMass", "Pt_T", "LikeSign_T == 0 && abs(PIDTPCPion_T[0]) > 2 && abs(PIDTPCPion_T[1]) > 2 && abs(PIDTPCMuon_T[0]) > 2 && abs(PIDTPCMuon_T[1]) > 2 && abs(PIDTPCElectron_T[0]) > 2 && abs(PIDTPCElectron_T[1]) > 2 && abs(PIDTPCKaon_T[0]) < 2 && abs(PIDTPCKaon_T[1]) < 2 && MassPairKaon_T < 1.35");
    hPtPairKaon2StdLowMass->SetLineColor(kRed);
    // 2 tracks of same charge
    auto hPtPairKaon2StdHighMass = new TH1F("hPtPairKaon2StdHighMass", "; p_{T} (GeV/c); Counts", 100, 0., 3.);
    inputTree->Project("hPtPairKaon2StdHighMass", "Pt_T", "LikeSign_T == 0 && abs(PIDTPCPion_T[0]) > 2 && abs(PIDTPCPion_T[1]) > 2 && abs(PIDTPCMuon_T[0]) > 2 && abs(PIDTPCMuon_T[1]) > 2 && abs(PIDTPCElectron_T[0]) > 2 && abs(PIDTPCElectron_T[1]) > 2 && abs(PIDTPCKaon_T[0]) < 2 && abs(PIDTPCKaon_T[1]) < 2 && MassPairKaon_T > 1.35");
    hPtPairKaon2StdHighMass->SetLineColor(kMagenta);

    auto cPtPairKaon = new TCanvas("cPtPairKaon", "cPtPairKaon", 600, 400);
    cPtPairKaon->cd();
    // cPtPairKaon->SetLogy();
    auto lPtPairKaon = new TLegend(0.3, 0.5, 0.7, 0.85);
    lPtPairKaon->SetTextSize(0.03);
    lPtPairKaon->SetBorderSize(0);
    // hPtPair->SetMinimum(1.0);
    hPtPairKaon2Std->Draw();
    hPtPairKaon2StdLowMass->Draw("same");
    hPtPairKaon2StdHighMass->Draw("same");
    lPtPairKaon->AddEntry(hPtPairKaon2Std, "-2>#sigma^{(#pi, #mu, e)}_{dE/dx}>2 & -2<#sigma^{K}_{dE/dx}<2", "l");
    lPtPairKaon->AddEntry(hPtPairKaon2StdLowMass, "-2>#sigma^{(#pi, #mu, e)}_{dE/dx}>2 & -2<#sigma^{K}_{dE/dx}<2 & m_{K^{+}K^{-}} < 1.35 GeV/c^{2}", "l");
    lPtPairKaon->AddEntry(hPtPairKaon2StdHighMass, "-2>#sigma^{(#pi, #mu, e)}_{dE/dx}>2 & -2<#sigma^{K}_{dE/dx}<2 & m_{K^{+}K^{-}} > 1.35 GeV/c^{2}", "l");
    lPtPairKaon->Draw();
    hPtPairKaon2Std->Write();
    hPtPairKaon2StdLowMass->Write();
    hPtPairKaon2StdHighMass->Write();
    cPtPairKaon->Print(Form("%s/cPtPairKaon.root", outDir.Data()),"root");
    cPtPairKaon->Print(Form("%s/cPtPairKaon.pdf", outDir.Data()),"pdf");

    //  -------------- Transverse momentum vs. invariant mass --------------

    // ***** Kaon
    auto cPtvsMassPairKaon = new TCanvas("cPtvsMassPairKaon", "cPtvsMassPairKaon", 600, 400);
    cPtvsMassPairKaon->cd();
    auto hPtvsMassPairKaon = new TH2D("hPtvsMassPairKaon", "; m_{K^{+}K^{-}} (GeV/c^{2}) ; p_{T}(GeV/c)", 1000,0.95,2.0, 1000,0.,3.0);
    inputTree->Project("hPtvsMassPairKaon", "Pt_T:MassPairKaon_T", "LikeSign_T == 0 && abs(PIDTPCKaon_T[0]) < 3 && abs(PIDTPCKaon_T[1]) < 3");
    hPtvsMassPairKaon->Draw("colz");
    hPtvsMassPairKaon->Write();
    cPtvsMassPairKaon->Print(Form("%s/cPtvsMassPairKaon.root", outDir.Data()),"root");
    cPtvsMassPairKaon->Print(Form("%s/cPtvsMassPairKaon.pdf", outDir.Data()),"pdf");

    // ***** select kaon with other particles rejected
    auto cPtvsMassPairKaonSelect = new TCanvas("cPtvsMassPairKaonSelect", "cPtvsMassPairKaonSelect", 600, 400);
    cPtvsMassPairKaonSelect->cd();
    auto hPtvsMassPairKaonSelect = new TH2D("hPtvsMassPairKaonSelect", "; m_{K^{+}K^{-}} (GeV/c^{2}) ; p_{T}(GeV/c)", 1000,0.95,2.0, 1000,0.,3.0);
    inputTree->Project("hPtvsMassPairKaonSelect", "Pt_T:MassPairKaon_T", "LikeSign_T == 0 && abs(PIDTPCPion_T[0]) > 2 && abs(PIDTPCPion_T[1]) > 2 && abs(PIDTPCMuon_T[0]) > 2 && abs(PIDTPCMuon_T[1]) > 2 && abs(PIDTPCElectron_T[0]) > 2 && abs(PIDTPCElectron_T[1]) > 2 && abs(PIDTPCKaon_T[0]) < 2 && abs(PIDTPCKaon_T[1]) < 2");
    hPtvsMassPairKaonSelect->Draw("colz");
    hPtvsMassPairKaonSelect->Write();
    cPtvsMassPairKaonSelect->Print(Form("%s/cPtvsMassPairKaonSelect.root", outDir.Data()),"root");
    cPtvsMassPairKaonSelect->Print(Form("%s/cPtvsMassPairKaonSelect.pdf", outDir.Data()),"pdf");

    // ***** Pion
    auto cPtvsMassPairPion = new TCanvas("cPtvsMassPairPion", "cPtvsMassPairPion", 600, 400);
    cPtvsMassPairPion->cd();
    auto hPtvsMassPairPion = new TH2D("hPtvsMassPairPion", "; m_{#pi^{+}#pi^{-}} (GeV/c^{2}) ; p_{T}(GeV/c)", 1000,0.3,2.0, 1000,0.,3.0);
    inputTree->Project("hPtvsMassPairPion", "Pt_T:MassPairPion_T", "LikeSign_T == 0 && abs(PIDTPCPion_T[0]) < 3 && abs(PIDTPCPion_T[1]) < 3");
    hPtvsMassPairPion->Draw("colz");
    hPtvsMassPairPion->Write();
    cPtvsMassPairPion->Print(Form("%s/cPtvsMassPairPion.root", outDir.Data()),"root");
    cPtvsMassPairPion->Print(Form("%s/cPtvsMassPairPion.pdf", outDir.Data()),"pdf");

    // -------------- dE/dx sigma vs. track momentum
    auto hDEdxVsP = new TH2D("hDEdxVsP", "; p(GeV/c); dE/dx", 1000,0.,5., 1000,0.,600.);
    auto hStdDEdxVsPKaon = new TH2D("hStdDEdxVsPKaon", "; p(GeV/c); #sigma^{K^{#pm}}_{dE/dx}", 1000,0.,5., 1000,-20.,50.);
    auto hStdDEdxVsPPion = new TH2D("hStdDEdxVsPPion", "; p(GeV/c); #sigma^{#pi^{#pm}}_{dE/dx}", 1000,0.,5., 1000,-10.,50.);
    auto hStdDEdxVsPMuon = new TH2D("hStdDEdxVsPMuon", "; p(GeV/c); #sigma^{#mu^{#pm}}_{dE/dx}", 1000,0.,5., 1000,-10.,50.);
    auto hStdDEdxVsPElectron = new TH2D("hStdDEdxVsPElectron", "; p(GeV/c); #sigma^{e^{#pm}}_{dE/dx}", 1000,0.,5., 1000,-10.,50.);

    // dE/dx sigma 1 vs. dE/dx sigma 2

    // Kaon
    auto cStdDEdx2Kaon = new TCanvas("cStdDEdx2Kaon", "cStdDEdx2Kaon", 600, 400);
    auto hStdDEdx2Kaon = new TH2D("hStdDEdx2Kaon", ";#sigma^{K^{+}}_{dE/dx}; #sigma^{K^{-}}_{dE/dx}", 1000,-30.,50., 1000,-30.,50.);
    cStdDEdx2Kaon->cd();
    inputTree->Project("hStdDEdx2Kaon", "PIDTPCKaon_T[0]:PIDTPCKaon_T[1]", "LikeSign_T == 0 && Pt_T < 0.1");
    hStdDEdx2Kaon->Draw("colz");
    hStdDEdx2Kaon->Write();
    cStdDEdx2Kaon->Print(Form("%s/cStdDEdx2Kaon.root", outDir.Data()),"root");
    cStdDEdx2Kaon->Print(Form("%s/cStdDEdx2Kaon.pdf", outDir.Data()),"pdf");

    // Diff n-sigma dE/dx 1-2 vs. m(K+K-)
    auto cDiffStdDEdxVsMassPairKaon = new TCanvas("cDiffStdDEdxVsMassPairKaon", "cDiffStdDEdxVsMassPairKaon", 600, 400);
    auto hDiffStdDEdxVsMassPairKaon = new TH2D("hDiffStdDEdxVsMassPairKaon", ";m_{K^{+}K^{-}} (GeV/c^{2});#Delta#sigma^{K^{#pm}}_{dE/dx}", 1000,0.95,2., 1000,-30.,50.);
    cDiffStdDEdxVsMassPairKaon->cd();
    inputTree->Project("hDiffStdDEdxVsMassPairKaon", "PIDTPCKaon_T[0]-PIDTPCKaon_T[1]:MassPairKaon_T", "LikeSign_T == 0");
    hDiffStdDEdxVsMassPairKaon->Draw("colz");
    hDiffStdDEdxVsMassPairKaon->Write();
    cDiffStdDEdxVsMassPairKaon->Print(Form("%s/cDiffStdDEdxVsMassPairKaon.root", outDir.Data()),"root");
    cDiffStdDEdxVsMassPairKaon->Print(Form("%s/cDiffStdDEdxVsMassPairKaon.pdf", outDir.Data()),"pdf");

    // ---------------------------- select kaon with other particles rejected -----------------------------
    //reject Pion
    auto cStdDEdx2KaonRejectPion = new TCanvas("cStdDEdx2KaonRejectPion", "cStdDEdx2KaonRejectPion", 600, 400);
    auto hStdDEdx2KaonRejectPion = new TH2D("hStdDEdx2KaonRejectPion", ";#sigma^{K^{+}}_{dE/dx}; #sigma^{K^{-}}_{dE/dx}", 1000,-30.,50., 1000,-30.,50.);
    cStdDEdx2KaonRejectPion->cd();
    inputTree->Project("hStdDEdx2KaonRejectPion", "PIDTPCKaon_T[0]:PIDTPCKaon_T[1]", "LikeSign_T == 0 && Pt_T < 0.1 && abs(PIDTPCPion_T[0]) > 2 && abs(PIDTPCPion_T[1]) > 2");
    hStdDEdx2KaonRejectPion->Draw("colz");
    auto el1 = new TEllipse(0.,0.,3.,3.);
    el1->SetFillStyle(0);
    el1->SetLineColor(4);
    auto el2 = new TEllipse(0.,0.,4.,4.);
    el2->SetFillStyle(0);
    el2->SetLineColor(2);
    auto el3 = new TEllipse(0.,0.,5.,5.);
    el3->SetFillStyle(0);
    el3->SetLineColor(2);
    el1->Draw("same");
    el2->Draw("same");
    el3->Draw("same");
    hStdDEdx2KaonRejectPion->Write();
    cStdDEdx2KaonRejectPion->Print(Form("%s/cStdDEdx2KaonRejectPion.root", outDir.Data()),"root");
    cStdDEdx2KaonRejectPion->Print(Form("%s/cStdDEdx2KaonRejectPion.pdf", outDir.Data()),"pdf");
    //reject Pion + Muon
    auto cStdDEdx2KaonRejectPionMuon = new TCanvas("cStdDEdx2KaonRejectPionMuon", "cStdDEdx2KaonRejectPionMuon", 600, 400);
    auto hStdDEdx2KaonRejectPionMuon = new TH2D("hStdDEdx2KaonRejectPionMuon", ";#sigma^{K^{+}}_{dE/dx}; #sigma^{K^{-}}_{dE/dx}", 1000,-30.,50., 1000,-30.,50.);
    cStdDEdx2KaonRejectPionMuon->cd();
    inputTree->Project("hStdDEdx2KaonRejectPionMuon", "PIDTPCKaon_T[0]:PIDTPCKaon_T[1]", "LikeSign_T == 0 && Pt_T < 0.1 && abs(PIDTPCPion_T[0]) > 2 && abs(PIDTPCPion_T[1]) > 2 && abs(PIDTPCMuon_T[0]) > 2 && abs(PIDTPCMuon_T[1]) > 2");
    hStdDEdx2KaonRejectPionMuon->Draw("colz");
    hStdDEdx2KaonRejectPionMuon->Write();
    cStdDEdx2KaonRejectPionMuon->Print(Form("%s/cStdDEdx2KaonRejectPionMuon.root", outDir.Data()),"root");
    cStdDEdx2KaonRejectPionMuon->Print(Form("%s/cStdDEdx2KaonRejectPionMuon.pdf", outDir.Data()),"pdf");
    //reject Pion + Muon + Electron
    auto cStdDEdx2KaonRejectAll = new TCanvas("cStdDEdx2KaonRejectAll", "cStdDEdx2KaonRejectAll", 600, 400);
    auto hStdDEdx2KaonRejectAll = new TH2D("hStdDEdx2KaonRejectAll", ";#sigma^{K^{+}}_{dE/dx}; #sigma^{K^{-}}_{dE/dx}", 1000,-30.,50., 1000,-30.,50.);
    cStdDEdx2KaonRejectAll->cd();
    inputTree->Project("hStdDEdx2KaonRejectAll", "PIDTPCKaon_T[0]:PIDTPCKaon_T[1]", "LikeSign_T == 0 && Pt_T < 0.1 && abs(PIDTPCPion_T[0]) > 2 && abs(PIDTPCPion_T[1]) > 2 && abs(PIDTPCMuon_T[0]) > 2 && abs(PIDTPCMuon_T[1]) > 2 && abs(PIDTPCElectron_T[0]) > 2 && abs(PIDTPCElectron_T[1]) > 2");
    hStdDEdx2KaonRejectAll->Draw("colz");
    hStdDEdx2KaonRejectAll->Write();
    cStdDEdx2KaonRejectAll->Print(Form("%s/cStdDEdx2KaonRejectAll.root", outDir.Data()),"root");
    cStdDEdx2KaonRejectAll->Print(Form("%s/cStdDEdx2KaonRejectAll.pdf", outDir.Data()),"pdf");
    //reject Pion + Muon + Electron while keeping kaon
    auto cStdDEdx2Kaon2Std = new TCanvas("cStdDEdx2Kaon2Std", "cStdDEdx2Kaon2Std", 600, 400);
    auto hStdDEdx2Kaon2Std = new TH2D("hStdDEdx2Kaon2Std", ";#sigma^{K^{+}}_{dE/dx}; #sigma^{K^{-}}_{dE/dx}", 1000,-30.,50., 1000,-30.,50.);
    cStdDEdx2Kaon2Std->cd();
    inputTree->Project("hStdDEdx2Kaon2Std", "PIDTPCKaon_T[0]:PIDTPCKaon_T[1]", "LikeSign_T == 0 && Pt_T < 0.1 && abs(PIDTPCPion_T[0]) > 2 && abs(PIDTPCPion_T[1]) > 2 && abs(PIDTPCMuon_T[0]) > 2 && abs(PIDTPCMuon_T[1]) > 2 && abs(PIDTPCElectron_T[0]) > 2 && abs(PIDTPCElectron_T[1]) > 2 && abs(PIDTPCKaon_T[0]) < 2 && abs(PIDTPCKaon_T[1]) < 2");
    hStdDEdx2Kaon2Std->Draw("colz");
    hStdDEdx2Kaon2Std->Write();
    cStdDEdx2Kaon2Std->Print(Form("%s/cStdDEdx2Kaon2Std.root", outDir.Data()),"root");
    cStdDEdx2Kaon2Std->Print(Form("%s/cStdDEdx2Kaon2Std.pdf", outDir.Data()),"pdf");
    // -----------------------------------------------------------------------------------------------------

    // Pion
    auto cStdDEdx2Pion = new TCanvas("cStdDEdx2Pion", "cStdDEdx2Pion", 600, 400);
    auto hStdDEdx2Pion = new TH2D("hStdDEdx2Pion", ";#sigma^{#pi^{+}}_{dE/dx}; #sigma^{#pi^{-}}_{dE/dx}", 1000,-30.,50., 1000,-30.,50.);
    cStdDEdx2Pion->cd();
    inputTree->Project("hStdDEdx2Pion", "PIDTPCPion_T[0]:PIDTPCPion_T[1]");
    hStdDEdx2Pion->Draw("colz");
    hStdDEdx2Pion->Write();
    cStdDEdx2Pion->Print(Form("%s/cStdDEdx2Pion.root", outDir.Data()),"root");
    cStdDEdx2Pion->Print(Form("%s/cStdDEdx2Pion.pdf", outDir.Data()),"pdf");

    // Muon
    auto cStdDEdx2Muon = new TCanvas("cStdDEdx2Muon", "cStdDEdx2Muon", 600, 400);
    auto hStdDEdx2Muon = new TH2D("hStdDEdx2Muon", ";#sigma^{#mu^{+}}_{dE/dx}; #sigma^{#mu^{-}}_{dE/dx}", 1000,-30.,50., 1000,-30.,50.);
    cStdDEdx2Muon->cd();
    inputTree->Project("hStdDEdx2Muon", "PIDTPCMuon_T[0]:PIDTPCMuon_T[1]");
    hStdDEdx2Muon->Draw("colz");
    hStdDEdx2Muon->Write();
    cStdDEdx2Muon->Print(Form("%s/cStdDEdx2Muon.root", outDir.Data()),"root");
    cStdDEdx2Muon->Print(Form("%s/cStdDEdx2Muon.pdf", outDir.Data()),"pdf");

    // Electron
    auto cStdDEdx2Electron = new TCanvas("cStdDEdx2Electron", "cStdDEdx2Electron", 600, 400);
    auto hStdDEdx2Electron = new TH2D("hStdDEdx2Electron", ";#sigma^{e^{+}}_{dE/dx}; #sigma^{e^{-}}_{dE/dx}", 1000,-30.,50., 1000,-30.,50.);
    cStdDEdx2Electron->cd();
    inputTree->Project("hStdDEdx2Electron", "PIDTPCElectron_T[0]:PIDTPCElectron_T[1]");
    hStdDEdx2Electron->Draw("colz");
    hStdDEdx2Electron->Write();
    cStdDEdx2Electron->Print(Form("%s/cStdDEdx2Electron.root", outDir.Data()),"root");
    cStdDEdx2Electron->Print(Form("%s/cStdDEdx2Electron.pdf", outDir.Data()),"pdf");

    // ------------------------------------ Signal, Bkg and their subtraction

    // +++++++++++++++ Invariant mass

    // Select Kaon
    auto hMassPairKaonSig = new TH1F("hMassPairKaonSig", "; m_{K^{+}K^{-}} (GeV/c^{2}); Counts", 100, 0.95, 2.0);
    inputTree->Project("hMassPairKaonSig", "MassPairKaon_T", "LikeSign_T == 0 && (PIDTPCKaon_T[0]*PIDTPCKaon_T[0] + PIDTPCKaon_T[1]*PIDTPCKaon_T[1]) < 9 && Pt_T < 0.1");
    hMassPairKaonSig->SetLineColor(kBlue);
    auto hMassPairKaonBkg = new TH1F("hMassPairKaonBkg", "; m_{K^{+}K^{-}} (GeV/c^{2}); Counts", 100, 0.95, 2.0);
    inputTree->Project("hMassPairKaonBkg", "MassPairKaon_T", "LikeSign_T == 0 && (PIDTPCKaon_T[0]*PIDTPCKaon_T[0] + PIDTPCKaon_T[1]*PIDTPCKaon_T[1]) > 16 && (PIDTPCKaon_T[0]*PIDTPCKaon_T[0] + PIDTPCKaon_T[1]*PIDTPCKaon_T[1]) < 25 && Pt_T < 0.1");
    hMassPairKaonBkg->SetLineColor(kRed);
    auto hMassPairKaonSub = new TH1F("hMassPairKaonSub", "; m_{K^{+}K^{-}} (GeV/c^{2}); Counts", 100, 0.95, 2.0);
    hMassPairKaonSub->Add(hMassPairKaonSig, hMassPairKaonBkg, 1., -1.);
    hMassPairKaonSub->SetLineColor(1); 
    hMassPairKaonSub->SetMarkerStyle(kOpenCircle);
    
    auto cMassPairKaonSub = new TCanvas("cMassPairKaonSub", "cMassPairKaonSub", 600, 400);
    cMassPairKaonSub->cd();
    // cMassPairKaon->SetLogy();
    auto lMassPairKaonSub = new TLegend(0.6, 0.7, 0.8, 0.9);
    lMassPairKaonSub->SetTextSize(0.03);
    lMassPairKaonSub->SetBorderSize(0);
    // hMassPairKaonSig->SetMinimum(1.0);
    hMassPairKaonSig->Draw("e");
    hMassPairKaonBkg->Draw("esame");
    hMassPairKaonSub->Draw("esame");
    lMassPairKaonSub->AddEntry(hMassPairKaonSig, "#sigma^{2}_{K^{+}} + #sigma^{2}_{K^{-}} < 9", "l");
    lMassPairKaonSub->AddEntry(hMassPairKaonBkg, "16 < #sigma^{2}_{K^{+}} + #sigma^{2}_{K^{-}} < 25", "l");
    lMassPairKaonSub->AddEntry(hMassPairKaonSub, "Subtracted", "p");
    lMassPairKaonSub->Draw();
    hMassPairKaonSig->Write();
    hMassPairKaonBkg->Write();
    hMassPairKaonSub->Write();
    cMassPairKaonSub->Print(Form("%s/cMassPairKaonSub.root", outDir.Data()),"root");
    cMassPairKaonSub->Print(Form("%s/cMassPairKaonSub.pdf", outDir.Data()),"pdf");

    // Select Kaon & Reject Pion

    auto hMassPairKaonRejectPionSig = new TH1F("hMassPairKaonRejectPionSig", "; m_{K^{+}K^{-}} (GeV/c^{2}); Counts", 100, 0.95, 2.0);
    inputTree->Project("hMassPairKaonRejectPionSig", "MassPairKaon_T", "LikeSign_T == 0 && (PIDTPCKaon_T[0]*PIDTPCKaon_T[0] + PIDTPCKaon_T[1]*PIDTPCKaon_T[1]) < 9 && abs(PIDTPCPion_T[0]) > 2 && abs(PIDTPCPion_T[1]) > 2 && Pt_T < 0.1");
    hMassPairKaonRejectPionSig->SetLineColor(kBlue);
    auto hMassPairKaonRejectPionBkg = new TH1F("hMassPairKaonRejectPionBkg", "; m_{K^{+}K^{-}} (GeV/c^{2}); Counts", 100, 0.95, 2.0);
    inputTree->Project("hMassPairKaonRejectPionBkg", "MassPairKaon_T", "LikeSign_T == 0 && (PIDTPCKaon_T[0]*PIDTPCKaon_T[0] + PIDTPCKaon_T[1]*PIDTPCKaon_T[1]) > 16 && (PIDTPCKaon_T[0]*PIDTPCKaon_T[0] + PIDTPCKaon_T[1]*PIDTPCKaon_T[1]) < 25 && abs(PIDTPCPion_T[0]) > 2 && abs(PIDTPCPion_T[1]) > 2 && Pt_T < 0.1");
    hMassPairKaonRejectPionBkg->SetLineColor(kRed);
    auto hMassPairKaonRejectPionSub = new TH1F("hMassPairKaonRejectPionSub", "; m_{K^{+}K^{-}} (GeV/c^{2}); Counts", 100, 0.95, 2.0);
    hMassPairKaonRejectPionSub->Add(hMassPairKaonRejectPionSig, hMassPairKaonRejectPionBkg, 1., -1.);
    hMassPairKaonRejectPionSub->SetLineColor(1); 
    hMassPairKaonRejectPionSub->SetMarkerStyle(kOpenCircle);
    
    auto cMassPairKaonRejectPionSub = new TCanvas("cMassPairKaonRejectPionSub", "cMassPairKaonRejectPionSub", 600, 400);
    cMassPairKaonRejectPionSub->cd();
    // cMassPairKaonRejectPion->SetLogy();
    auto lMassPairKaonRejectPionSub = new TLegend(0.6, 0.7, 0.8, 0.9);
    lMassPairKaonRejectPionSub->SetTextSize(0.03);
    lMassPairKaonRejectPionSub->SetBorderSize(0);
    // hMassPairKaonRejectPionSig->SetMinimum(1.0);
    hMassPairKaonRejectPionSig->Draw("e");
    hMassPairKaonRejectPionBkg->Draw("esame");
    hMassPairKaonRejectPionSub->Draw("esame");
    lMassPairKaonRejectPionSub->AddEntry(hMassPairKaonRejectPionSig, "#sigma^{2}_{K^{+}} + #sigma^{2}_{K^{-}} < 9", "l");
    lMassPairKaonRejectPionSub->AddEntry(hMassPairKaonRejectPionBkg, "16 < #sigma^{2}_{K^{+}} + #sigma^{2}_{K^{-}} < 25", "l");
    lMassPairKaonRejectPionSub->AddEntry(hMassPairKaonRejectPionSub, "Subtracted", "p");
    lMassPairKaonRejectPionSub->Draw();
    hMassPairKaonRejectPionSig->Write();
    hMassPairKaonRejectPionBkg->Write();
    hMassPairKaonRejectPionSub->Write();
    cMassPairKaonRejectPionSub->Print(Form("%s/cMassPairKaonRejectPionSub.root", outDir.Data()),"root");
    cMassPairKaonRejectPionSub->Print(Form("%s/cMassPairKaonRejectPionSub.pdf", outDir.Data()),"pdf");

    // Select Kaon & Reject (Pion + Muon)

    auto hMassPairKaonRejectPionMuonSig = new TH1F("hMassPairKaonRejectPionMuonSig", "; m_{K^{+}K^{-}} (GeV/c^{2}); Counts", 100, 0.95, 2.0);
    inputTree->Project("hMassPairKaonRejectPionMuonSig", "MassPairKaon_T", "LikeSign_T == 0 && (PIDTPCKaon_T[0]*PIDTPCKaon_T[0] + PIDTPCKaon_T[1]*PIDTPCKaon_T[1]) < 9 && abs(PIDTPCPion_T[0]) > 2 && abs(PIDTPCPion_T[1]) > 2 && abs(PIDTPCMuon_T[0]) > 2 && abs(PIDTPCMuon_T[1]) > 2 && Pt_T < 0.1");
    hMassPairKaonRejectPionMuonSig->SetLineColor(kBlue);
    auto hMassPairKaonRejectPionMuonBkg = new TH1F("hMassPairKaonRejectPionMuonBkg", "; m_{K^{+}K^{-}} (GeV/c^{2}); Counts", 100, 0.95, 2.0);
    inputTree->Project("hMassPairKaonRejectPionMuonBkg", "MassPairKaon_T", "LikeSign_T == 0 && (PIDTPCKaon_T[0]*PIDTPCKaon_T[0] + PIDTPCKaon_T[1]*PIDTPCKaon_T[1]) > 16 && (PIDTPCKaon_T[0]*PIDTPCKaon_T[0] + PIDTPCKaon_T[1]*PIDTPCKaon_T[1]) < 25 && abs(PIDTPCPion_T[0]) > 2 && abs(PIDTPCPion_T[1]) > 2 && abs(PIDTPCMuon_T[0]) > 2 && abs(PIDTPCMuon_T[1]) > 2 && Pt_T < 0.1");
    hMassPairKaonRejectPionMuonBkg->SetLineColor(kRed);
    auto hMassPairKaonRejectPionMuonSub = new TH1F("hMassPairKaonRejectPionMuonSub", "; m_{K^{+}K^{-}} (GeV/c^{2}); Counts", 100, 0.95, 2.0);
    hMassPairKaonRejectPionMuonSub->Add(hMassPairKaonRejectPionMuonSig, hMassPairKaonRejectPionMuonBkg, 1., -1.);
    hMassPairKaonRejectPionMuonSub->SetLineColor(1); 
    hMassPairKaonRejectPionMuonSub->SetMarkerStyle(kOpenCircle);
    
    auto cMassPairKaonRejectPionMuonSub = new TCanvas("cMassPairKaonRejectPionMuonSub", "cMassPairKaonRejectPionMuonSub", 600, 400);
    cMassPairKaonRejectPionMuonSub->cd();
    // cMassPairKaonRejectPionMuon->SetLogy();
    auto lMassPairKaonRejectPionMuonSub = new TLegend(0.6, 0.7, 0.8, 0.9);
    lMassPairKaonRejectPionMuonSub->SetTextSize(0.03);
    lMassPairKaonRejectPionMuonSub->SetBorderSize(0);
    // hMassPairKaonRejectPionMuonSig->SetMinimum(1.0);
    hMassPairKaonRejectPionMuonSig->Draw("e");
    hMassPairKaonRejectPionMuonBkg->Draw("esame");
    hMassPairKaonRejectPionMuonSub->Draw("esame");
    lMassPairKaonRejectPionMuonSub->AddEntry(hMassPairKaonRejectPionMuonSig, "#sigma^{2}_{K^{+}} + #sigma^{2}_{K^{-}} < 9", "l");
    lMassPairKaonRejectPionMuonSub->AddEntry(hMassPairKaonRejectPionMuonBkg, "16 < #sigma^{2}_{K^{+}} + #sigma^{2}_{K^{-}} < 25", "l");
    lMassPairKaonRejectPionMuonSub->AddEntry(hMassPairKaonRejectPionMuonSub, "Subtracted", "p");
    lMassPairKaonRejectPionMuonSub->Draw();
    hMassPairKaonRejectPionMuonSig->Write();
    hMassPairKaonRejectPionMuonBkg->Write();
    hMassPairKaonRejectPionMuonSub->Write();
    cMassPairKaonRejectPionMuonSub->Print(Form("%s/cMassPairKaonRejectPionMuonSub.root", outDir.Data()),"root");
    cMassPairKaonRejectPionMuonSub->Print(Form("%s/cMassPairKaonRejectPionMuonSub.pdf", outDir.Data()),"pdf");

    // Select Kaon & Reject (Pion + Muon + Electron)

    auto hMassPairKaonRejectAllSig = new TH1F("hMassPairKaonRejectAllSig", "; m_{K^{+}K^{-}} (GeV/c^{2}); Counts", 100, 0.95, 2.0);
    inputTree->Project("hMassPairKaonRejectAllSig", "MassPairKaon_T", "LikeSign_T == 0 && (PIDTPCKaon_T[0]*PIDTPCKaon_T[0] + PIDTPCKaon_T[1]*PIDTPCKaon_T[1]) < 9 && abs(PIDTPCPion_T[0]) > 2 && abs(PIDTPCPion_T[1]) > 2 && abs(PIDTPCMuon_T[0]) > 2 && abs(PIDTPCMuon_T[1]) > 2 && abs(PIDTPCElectron_T[0]) > 2 && abs(PIDTPCElectron_T[1]) > 2 && Pt_T < 0.1");
    hMassPairKaonRejectAllSig->SetLineColor(kBlue);
    auto hMassPairKaonRejectAllBkg = new TH1F("hMassPairKaonRejectAllBkg", "; m_{K^{+}K^{-}} (GeV/c^{2}); Counts", 100, 0.95, 2.0);
    inputTree->Project("hMassPairKaonRejectAllBkg", "MassPairKaon_T", "LikeSign_T == 0 && (PIDTPCKaon_T[0]*PIDTPCKaon_T[0] + PIDTPCKaon_T[1]*PIDTPCKaon_T[1]) > 16 && (PIDTPCKaon_T[0]*PIDTPCKaon_T[0] + PIDTPCKaon_T[1]*PIDTPCKaon_T[1]) < 25 && abs(PIDTPCPion_T[0]) > 2 && abs(PIDTPCPion_T[1]) > 2 && abs(PIDTPCMuon_T[0]) > 2 && abs(PIDTPCMuon_T[1]) > 2 && abs(PIDTPCElectron_T[0]) > 2 && abs(PIDTPCElectron_T[1]) > 2 && Pt_T < 0.1");
    hMassPairKaonRejectAllBkg->SetLineColor(kRed);
    auto hMassPairKaonRejectAllSub = new TH1F("hMassPairKaonRejectAllSub", "; m_{K^{+}K^{-}} (GeV/c^{2}); Counts", 100, 0.95, 2.0);
    hMassPairKaonRejectAllSub->Add(hMassPairKaonRejectAllSig, hMassPairKaonRejectAllBkg, 1., -1.);
    hMassPairKaonRejectAllSub->SetLineColor(1); 
    hMassPairKaonRejectAllSub->SetMarkerStyle(kOpenCircle);
    
    auto cMassPairKaonRejectAllSub = new TCanvas("cMassPairKaonRejectAllSub", "cMassPairKaonRejectAllSub", 600, 400);
    cMassPairKaonRejectAllSub->cd();
    // cMassPairKaonRejectAll->SetLogy();
    auto lMassPairKaonRejectAllSub = new TLegend(0.6, 0.7, 0.8, 0.9);
    lMassPairKaonRejectAllSub->SetTextSize(0.03);
    lMassPairKaonRejectAllSub->SetBorderSize(0);
    // hMassPairKaonRejectAllSig->SetMinimum(1.0);
    hMassPairKaonRejectAllSig->Draw("e");
    hMassPairKaonRejectAllBkg->Draw("esame");
    hMassPairKaonRejectAllSub->Draw("esame");
    lMassPairKaonRejectAllSub->AddEntry(hMassPairKaonRejectAllSig, "#sigma^{2}_{K^{+}} + #sigma^{2}_{K^{-}} < 9", "l");
    lMassPairKaonRejectAllSub->AddEntry(hMassPairKaonRejectAllBkg, "16 < #sigma^{2}_{K^{+}} + #sigma^{2}_{K^{-}} < 25", "l");
    lMassPairKaonRejectAllSub->AddEntry(hMassPairKaonRejectAllSub, "Subtracted", "p");
    lMassPairKaonRejectAllSub->Draw();
    hMassPairKaonRejectAllSig->Write();
    hMassPairKaonRejectAllBkg->Write();
    hMassPairKaonRejectAllSub->Write();
    cMassPairKaonRejectAllSub->Print(Form("%s/cMassPairKaonRejectAllSub.root", outDir.Data()),"root");
    cMassPairKaonRejectAllSub->Print(Form("%s/cMassPairKaonRejectAllSub.pdf", outDir.Data()),"pdf");

     // +++++++++++++++ Tranverse Momentum

    // Select Kaon
    auto hPtPairKaonSig = new TH1F("hPtPairKaonSig", "; p_{T} (GeV/c); Counts", 100, 0., 0.8);
    inputTree->Project("hPtPairKaonSig", "Pt_T", "LikeSign_T == 0 && (PIDTPCKaon_T[0]*PIDTPCKaon_T[0] + PIDTPCKaon_T[1]*PIDTPCKaon_T[1]) < 9");
    hPtPairKaonSig->SetLineColor(kBlue);
    auto hPtPairKaonBkg = new TH1F("hPtPairKaonBkg", "; p_{T} (GeV/c); Counts", 100, 0., 0.8);
    inputTree->Project("hPtPairKaonBkg", "Pt_T", "LikeSign_T == 0 && (PIDTPCKaon_T[0]*PIDTPCKaon_T[0] + PIDTPCKaon_T[1]*PIDTPCKaon_T[1]) > 16 && (PIDTPCKaon_T[0]*PIDTPCKaon_T[0] + PIDTPCKaon_T[1]*PIDTPCKaon_T[1]) < 25");
    hPtPairKaonBkg->SetLineColor(kRed);
    auto hPtPairKaonSub = new TH1F("hPtPairKaonSub", "; p_{T} (GeV/c); Counts", 100, 0., 0.8);
    hPtPairKaonSub->Add(hPtPairKaonSig, hPtPairKaonBkg, 1., -1.);
    hPtPairKaonSub->SetLineColor(1); 
    hPtPairKaonSub->SetMarkerStyle(kOpenCircle);
    
    auto cPtPairKaonSub = new TCanvas("cPtPairKaonSub", "cPtPairKaonSub", 600, 400);
    cPtPairKaonSub->cd();
    // cPtPairKaon->SetLogy();
    auto lPtPairKaonSub = new TLegend(0.6, 0.7, 0.8, 0.9);
    lPtPairKaonSub->SetTextSize(0.03);
    lPtPairKaonSub->SetBorderSize(0);
    // hPtPairKaonSig->SetMinimum(1.0);
    hPtPairKaonSig->Draw("e");
    hPtPairKaonBkg->Draw("esame");
    hPtPairKaonSub->Draw("esame");
    lPtPairKaonSub->AddEntry(hPtPairKaonSig, "#sigma^{2}_{K^{+}} + #sigma^{2}_{K^{-}} < 9", "l");
    lPtPairKaonSub->AddEntry(hPtPairKaonBkg, "16 < #sigma^{2}_{K^{+}} + #sigma^{2}_{K^{-}} < 25", "l");
    lPtPairKaonSub->AddEntry(hPtPairKaonSub, "Subtracted", "p");
    lPtPairKaonSub->Draw();
    hPtPairKaonSig->Write();
    hPtPairKaonBkg->Write();
    hPtPairKaonSub->Write();
    cPtPairKaonSub->Print(Form("%s/cPtPairKaonSub.root", outDir.Data()),"root");
    cPtPairKaonSub->Print(Form("%s/cPtPairKaonSub.pdf", outDir.Data()),"pdf");

    // Select Kaon & Reject Pion

    auto hPtPairKaonRejectPionSig = new TH1F("hPtPairKaonRejectPionSig", "; p_{T} (GeV/c); Counts", 100, 0., 0.8);
    inputTree->Project("hPtPairKaonRejectPionSig", "Pt_T", "LikeSign_T == 0 && (PIDTPCKaon_T[0]*PIDTPCKaon_T[0] + PIDTPCKaon_T[1]*PIDTPCKaon_T[1]) < 9 && abs(PIDTPCPion_T[0]) > 2 && abs(PIDTPCPion_T[1]) > 2");
    hPtPairKaonRejectPionSig->SetLineColor(kBlue);
    auto hPtPairKaonRejectPionBkg = new TH1F("hPtPairKaonRejectPionBkg", "; p_{T} (GeV/c); Counts", 100, 0., 0.8);
    inputTree->Project("hPtPairKaonRejectPionBkg", "Pt_T", "LikeSign_T == 0 && (PIDTPCKaon_T[0]*PIDTPCKaon_T[0] + PIDTPCKaon_T[1]*PIDTPCKaon_T[1]) > 16 && (PIDTPCKaon_T[0]*PIDTPCKaon_T[0] + PIDTPCKaon_T[1]*PIDTPCKaon_T[1]) < 25 && abs(PIDTPCPion_T[0]) > 2 && abs(PIDTPCPion_T[1]) > 2");
    hPtPairKaonRejectPionBkg->SetLineColor(kRed);
    auto hPtPairKaonRejectPionSub = new TH1F("hPtPairKaonRejectPionSub", "; p_{T} (GeV/c); Counts", 100, 0., 0.8);
    hPtPairKaonRejectPionSub->Add(hPtPairKaonRejectPionSig, hPtPairKaonRejectPionBkg, 1., -1.);
    hPtPairKaonRejectPionSub->SetLineColor(1); 
    hPtPairKaonRejectPionSub->SetMarkerStyle(kOpenCircle);
    
    auto cPtPairKaonRejectPionSub = new TCanvas("cPtPairKaonRejectPionSub", "cPtPairKaonRejectPionSub", 600, 400);
    cPtPairKaonRejectPionSub->cd();
    // cPtPairKaonRejectPion->SetLogy();
    auto lPtPairKaonRejectPionSub = new TLegend(0.6, 0.7, 0.8, 0.9);
    lPtPairKaonRejectPionSub->SetTextSize(0.03);
    lPtPairKaonRejectPionSub->SetBorderSize(0);
    // hPtPairKaonRejectPionSig->SetMinimum(1.0);
    hPtPairKaonRejectPionSig->Draw("e");
    hPtPairKaonRejectPionBkg->Draw("esame");
    hPtPairKaonRejectPionSub->Draw("esame");
    lPtPairKaonRejectPionSub->AddEntry(hPtPairKaonRejectPionSig, "#sigma^{2}_{K^{+}} + #sigma^{2}_{K^{-}} < 9", "l");
    lPtPairKaonRejectPionSub->AddEntry(hPtPairKaonRejectPionBkg, "16 < #sigma^{2}_{K^{+}} + #sigma^{2}_{K^{-}} < 25", "l");
    lPtPairKaonRejectPionSub->AddEntry(hPtPairKaonRejectPionSub, "Subtracted", "p");
    lPtPairKaonRejectPionSub->Draw();
    hPtPairKaonRejectPionSig->Write();
    hPtPairKaonRejectPionBkg->Write();
    hPtPairKaonRejectPionSub->Write();
    cPtPairKaonRejectPionSub->Print(Form("%s/cPtPairKaonRejectPionSub.root", outDir.Data()),"root");
    cPtPairKaonRejectPionSub->Print(Form("%s/cPtPairKaonRejectPionSub.pdf", outDir.Data()),"pdf");

    // Select Kaon & Reject (Pion + Muon)

    auto hPtPairKaonRejectPionMuonSig = new TH1F("hPtPairKaonRejectPionMuonSig", "; p_{T} (GeV/c); Counts", 100, 0., 0.8);
    inputTree->Project("hPtPairKaonRejectPionMuonSig", "Pt_T", "LikeSign_T == 0 && (PIDTPCKaon_T[0]*PIDTPCKaon_T[0] + PIDTPCKaon_T[1]*PIDTPCKaon_T[1]) < 9 && abs(PIDTPCPion_T[0]) > 2 && abs(PIDTPCPion_T[1]) > 2 && abs(PIDTPCMuon_T[0]) > 2 && abs(PIDTPCMuon_T[1]) > 2");
    hPtPairKaonRejectPionMuonSig->SetLineColor(kBlue);
    auto hPtPairKaonRejectPionMuonBkg = new TH1F("hPtPairKaonRejectPionMuonBkg", "; p_{T} (GeV/c); Counts", 100, 0., 0.8);
    inputTree->Project("hPtPairKaonRejectPionMuonBkg", "Pt_T", "LikeSign_T == 0 && (PIDTPCKaon_T[0]*PIDTPCKaon_T[0] + PIDTPCKaon_T[1]*PIDTPCKaon_T[1]) > 16 && (PIDTPCKaon_T[0]*PIDTPCKaon_T[0] + PIDTPCKaon_T[1]*PIDTPCKaon_T[1]) < 25 && abs(PIDTPCPion_T[0]) > 2 && abs(PIDTPCPion_T[1]) > 2 && abs(PIDTPCMuon_T[0]) > 2 && abs(PIDTPCMuon_T[1]) > 2");
    hPtPairKaonRejectPionMuonBkg->SetLineColor(kRed);
    auto hPtPairKaonRejectPionMuonSub = new TH1F("hPtPairKaonRejectPionMuonSub", "; p_{T} (GeV/c); Counts", 100, 0., 0.8);
    hPtPairKaonRejectPionMuonSub->Add(hPtPairKaonRejectPionMuonSig, hPtPairKaonRejectPionMuonBkg, 1., -1.);
    hPtPairKaonRejectPionMuonSub->SetLineColor(1); 
    hPtPairKaonRejectPionMuonSub->SetMarkerStyle(kOpenCircle);
    
    auto cPtPairKaonRejectPionMuonSub = new TCanvas("cPtPairKaonRejectPionMuonSub", "cPtPairKaonRejectPionMuonSub", 600, 400);
    cPtPairKaonRejectPionMuonSub->cd();
    // cPtPairKaonRejectPionMuon->SetLogy();
    auto lPtPairKaonRejectPionMuonSub = new TLegend(0.6, 0.7, 0.8, 0.9);
    lPtPairKaonRejectPionMuonSub->SetTextSize(0.03);
    lPtPairKaonRejectPionMuonSub->SetBorderSize(0);
    // hPtPairKaonRejectPionMuonSig->SetMinimum(1.0);
    hPtPairKaonRejectPionMuonSig->Draw("e");
    hPtPairKaonRejectPionMuonBkg->Draw("esame");
    hPtPairKaonRejectPionMuonSub->Draw("esame");
    lPtPairKaonRejectPionMuonSub->AddEntry(hPtPairKaonRejectPionMuonSig, "#sigma^{2}_{K^{+}} + #sigma^{2}_{K^{-}} < 9", "l");
    lPtPairKaonRejectPionMuonSub->AddEntry(hPtPairKaonRejectPionMuonBkg, "16 < #sigma^{2}_{K^{+}} + #sigma^{2}_{K^{-}} < 25", "l");
    lPtPairKaonRejectPionMuonSub->AddEntry(hPtPairKaonRejectPionMuonSub, "Subtracted", "p");
    lPtPairKaonRejectPionMuonSub->Draw();
    hPtPairKaonRejectPionMuonSig->Write();
    hPtPairKaonRejectPionMuonBkg->Write();
    hPtPairKaonRejectPionMuonSub->Write();
    cPtPairKaonRejectPionMuonSub->Print(Form("%s/cPtPairKaonRejectPionMuonSub.root", outDir.Data()),"root");
    cPtPairKaonRejectPionMuonSub->Print(Form("%s/cPtPairKaonRejectPionMuonSub.pdf", outDir.Data()),"pdf");

    // Select Kaon & Reject (Pion + Muon + Electron)

    auto hPtPairKaonRejectAllSig = new TH1F("hPtPairKaonRejectAllSig", "; p_{T} (GeV/c); Counts", 100, 0., 0.8);
    inputTree->Project("hPtPairKaonRejectAllSig", "Pt_T", "LikeSign_T == 0 && (PIDTPCKaon_T[0]*PIDTPCKaon_T[0] + PIDTPCKaon_T[1]*PIDTPCKaon_T[1]) < 9 && abs(PIDTPCPion_T[0]) > 2 && abs(PIDTPCPion_T[1]) > 2 && abs(PIDTPCMuon_T[0]) > 2 && abs(PIDTPCMuon_T[1]) > 2 && abs(PIDTPCElectron_T[0]) > 2 && abs(PIDTPCElectron_T[1]) > 2");
    hPtPairKaonRejectAllSig->SetLineColor(kBlue);
    auto hPtPairKaonRejectAllBkg = new TH1F("hPtPairKaonRejectAllBkg", "; p_{T} (GeV/c); Counts", 100, 0., 0.8);
    inputTree->Project("hPtPairKaonRejectAllBkg", "Pt_T", "LikeSign_T == 0 && (PIDTPCKaon_T[0]*PIDTPCKaon_T[0] + PIDTPCKaon_T[1]*PIDTPCKaon_T[1]) > 16 && (PIDTPCKaon_T[0]*PIDTPCKaon_T[0] + PIDTPCKaon_T[1]*PIDTPCKaon_T[1]) < 25 && abs(PIDTPCPion_T[0]) > 2 && abs(PIDTPCPion_T[1]) > 2 && abs(PIDTPCMuon_T[0]) > 2 && abs(PIDTPCMuon_T[1]) > 2 && abs(PIDTPCElectron_T[0]) > 2 && abs(PIDTPCElectron_T[1]) > 2");
    hPtPairKaonRejectAllBkg->SetLineColor(kRed);
    auto hPtPairKaonRejectAllSub = new TH1F("hPtPairKaonRejectAllSub", "; p_{T} (GeV/c); Counts", 100, 0., 0.8);
    hPtPairKaonRejectAllSub->Add(hPtPairKaonRejectAllSig, hPtPairKaonRejectAllBkg, 1., -1.);
    hPtPairKaonRejectAllSub->SetLineColor(1); 
    hPtPairKaonRejectAllSub->SetMarkerStyle(kOpenCircle);
    
    auto cPtPairKaonRejectAllSub = new TCanvas("cPtPairKaonRejectAllSub", "cPtPairKaonRejectAllSub", 600, 400);
    cPtPairKaonRejectAllSub->cd();
    // cPtPairKaonRejectAll->SetLogy();
    auto lPtPairKaonRejectAllSub = new TLegend(0.6, 0.7, 0.8, 0.9);
    lPtPairKaonRejectAllSub->SetTextSize(0.03);
    lPtPairKaonRejectAllSub->SetBorderSize(0);
    // hPtPairKaonRejectAllSig->SetMinimum(1.0);
    hPtPairKaonRejectAllSig->Draw("e");
    hPtPairKaonRejectAllBkg->Draw("esame");
    hPtPairKaonRejectAllSub->Draw("esame");
    lPtPairKaonRejectAllSub->AddEntry(hPtPairKaonRejectAllSig, "#sigma^{2}_{K^{+}} + #sigma^{2}_{K^{-}} < 9", "l");
    lPtPairKaonRejectAllSub->AddEntry(hPtPairKaonRejectAllBkg, "16 < #sigma^{2}_{K^{+}} + #sigma^{2}_{K^{-}} < 25", "l");
    lPtPairKaonRejectAllSub->AddEntry(hPtPairKaonRejectAllSub, "Subtracted", "p");
    lPtPairKaonRejectAllSub->Draw();
    hPtPairKaonRejectAllSig->Write();
    hPtPairKaonRejectAllBkg->Write();
    hPtPairKaonRejectAllSub->Write();
    cPtPairKaonRejectAllSub->Print(Form("%s/cPtPairKaonRejectAllSub.root", outDir.Data()),"root");
    cPtPairKaonRejectAllSub->Print(Form("%s/cPtPairKaonRejectAllSub.pdf", outDir.Data()),"pdf");

    // --------- Count sig and Bkg events after different PID cuts
   Int_t NumbKaonSig1 = 0, NumbKaonSig2 = 0, NumbKaonSig3 = 0, NumbKaonSig4 = 0;
   Int_t NumbKaonBkg1 = 0, NumbKaonBkg2 = 0, NumbKaonBkg3 = 0, NumbKaonBkg4 = 0;
   Int_t NumbKaon1PID = 0, NumbKaon2PID = 0;

    // get the variables inside input TTree branches
    Int_t nTracks = 2;
    Float_t Pt_T = -100;
	Float_t Rapidity_T = -100;
	Float_t Phi_T = -100;
	Bool_t LikeSign_T = -100;
    Float_t TrackPx_T[2] = {-100};
    Float_t TrackPy_T[2] = {-100};
    Float_t TrackPz_T[2] = {-100};
    Float_t TrackP_T[2] = {-100};
    Float_t TrackPt_T[2] = {-100};
    Float_t TrackEta_T[2] = {-100};
    Float_t TrackPhi_T[2] = {-100};
    Int_t TPCsignal_T[2] = {-100};
    Float_t PIDTPCKaon_T[2] = {-100};
    Float_t PIDTPCPion_T[2] = {-100};
    Float_t PIDTPCMuon_T[2] = {-100};
    Float_t PIDTPCElectron_T[2] = {-100};
    inputTree->SetBranchAddress("Pt_T", &Pt_T);
    inputTree->SetBranchAddress("Rapidity_T", &Rapidity_T);
    inputTree->SetBranchAddress("Phi_T", &Phi_T);
    inputTree->SetBranchAddress("LikeSign_T", &LikeSign_T);
    inputTree->SetBranchAddress("TrackPx_T", &TrackPx_T);
    inputTree->SetBranchAddress("TrackPy_T", &TrackPy_T);
    inputTree->SetBranchAddress("TrackPz_T", &TrackPz_T);
    inputTree->SetBranchAddress("TrackP_T", &TrackP_T);
    inputTree->SetBranchAddress("TrackPt_T", &TrackPt_T);
    inputTree->SetBranchAddress("TrackEta_T", &TrackEta_T);
    inputTree->SetBranchAddress("TrackPhi_T", &TrackPhi_T);
    inputTree->SetBranchAddress("TPCsignal_T", &TPCsignal_T);
    inputTree->SetBranchAddress("PIDTPCKaon_T", &PIDTPCKaon_T);
    inputTree->SetBranchAddress("PIDTPCPion_T", &PIDTPCPion_T);
    inputTree->SetBranchAddress("PIDTPCMuon_T", &PIDTPCMuon_T);
    inputTree->SetBranchAddress("PIDTPCElectron_T", &PIDTPCElectron_T);

    // grab the pion mass from PDG
    TDatabasePDG *pdgdat = TDatabasePDG::Instance(); 
	TParticlePDG *partKaon = pdgdat->GetParticle(321); // kaon
  	TParticlePDG *partPion = pdgdat->GetParticle( 211 ); // pion
  	TParticlePDG *partMuon = pdgdat->GetParticle(13); // Muon
  	TParticlePDG *partElectron = pdgdat->GetParticle(11); // electron
  	Double_t kaonMass = partKaon->Mass();
  	Double_t pionMass = partPion->Mass();
  	Double_t muonMass = partMuon->Mass();
  	Double_t electronMass = partElectron->Mass();

    TLorentzVector lvKaon[2];
    TLorentzVector lvPion[2];
    TLorentzVector lvMuon[2];
    TLorentzVector lvElectron[2];
    TLorentzVector lvSumKaon;
    TLorentzVector lvSumPion; // pair-4vector
    TLorentzVector lvSumMuon;
    TLorentzVector lvSumElectron;

    // loop over all the events
    for (int i = 0; i < inputTree->GetEntries(); i++)
    {
        inputTree->GetEntry(i);

        // loop over all tracks
        for(Int_t iTrack=0; iTrack<nTracks; iTrack++)
        {
            // creat 4-momenta using Pt, Eta, Phi, Mass per track
            lvKaon[iTrack].SetPtEtaPhiM(TrackPt_T[iTrack], TrackEta_T[iTrack], TrackPhi_T[iTrack], kaonMass);
            lvPion[iTrack].SetPtEtaPhiM(TrackPt_T[iTrack], TrackEta_T[iTrack], TrackPhi_T[iTrack], pionMass);
            lvMuon[iTrack].SetPtEtaPhiM(TrackPt_T[iTrack], TrackEta_T[iTrack], TrackPhi_T[iTrack], muonMass);
            lvElectron[iTrack].SetPtEtaPhiM(TrackPt_T[iTrack], TrackEta_T[iTrack], TrackPhi_T[iTrack], electronMass);

            // if(i%10000==0) cout<<"event = "<<i<<" | TrackP_T["<<iTrack<<"]= "<<TrackP_T[iTrack]<<" | TPCsignal_T["<<iTrack<<"] = "<<TPCsignal_T[iTrack]<<endl;

            // dE/dx n-sigma TPC
            hDEdxVsP->Fill(TrackP_T[iTrack], TPCsignal_T[iTrack]);
            hStdDEdxVsPKaon->Fill(TrackP_T[iTrack], PIDTPCKaon_T[iTrack]);
            hStdDEdxVsPPion->Fill(TrackP_T[iTrack], PIDTPCPion_T[iTrack]);
            hStdDEdxVsPMuon->Fill(TrackP_T[iTrack], PIDTPCMuon_T[iTrack]);
            hStdDEdxVsPElectron->Fill(TrackP_T[iTrack], PIDTPCElectron_T[iTrack]);
        }

	    lvSumKaon = lvKaon[0] + lvKaon[1];
        lvSumPion = lvPion[0] + lvPion[1];
        lvSumMuon = lvMuon[0] + lvMuon[1];
	    lvSumElectron = lvElectron[0] + lvElectron[1];

        if(LikeSign_T == 1) continue; //save only events with opposite charge track pairs

        // --------- Count sig and Bkg events after different PID cuts

        if(Pt_T > 0.1) continue;
        // Sig events
        if((PIDTPCKaon_T[0]*PIDTPCKaon_T[0] + PIDTPCKaon_T[1]*PIDTPCKaon_T[1]) < 9)
        {
            NumbKaonSig1++;
            if(abs(PIDTPCPion_T[0]) > 2 && abs(PIDTPCPion_T[1]) > 2) NumbKaonSig2++;
            if(abs(PIDTPCPion_T[0]) > 2 && abs(PIDTPCPion_T[1]) > 2 && abs(PIDTPCMuon_T[0]) > 2 && abs(PIDTPCMuon_T[1]) > 2) NumbKaonSig3++;
            if(abs(PIDTPCPion_T[0]) > 2 && abs(PIDTPCPion_T[1]) > 2 && abs(PIDTPCMuon_T[0]) > 2 && abs(PIDTPCMuon_T[1]) > 2 && abs(PIDTPCElectron_T[0]) > 2 && abs(PIDTPCElectron_T[1]) > 2) NumbKaonSig4++;
        }

        // Bkg events
        if((PIDTPCKaon_T[0]*PIDTPCKaon_T[0] + PIDTPCKaon_T[1]*PIDTPCKaon_T[1]) > 16 && (PIDTPCKaon_T[0]*PIDTPCKaon_T[0] + PIDTPCKaon_T[1]*PIDTPCKaon_T[1]) < 25)
        {
            NumbKaonBkg1++;
            if(abs(PIDTPCPion_T[0]) > 2 && abs(PIDTPCPion_T[1]) > 2) NumbKaonBkg2++;
            if(abs(PIDTPCPion_T[0]) > 2 && abs(PIDTPCPion_T[1]) > 2 && abs(PIDTPCMuon_T[0]) > 2 && abs(PIDTPCMuon_T[1]) > 2) NumbKaonBkg3++;
            if(abs(PIDTPCPion_T[0]) > 2 && abs(PIDTPCPion_T[1]) > 2 && abs(PIDTPCMuon_T[0]) > 2 && abs(PIDTPCMuon_T[1]) > 2 && abs(PIDTPCElectron_T[0]) > 2 && abs(PIDTPCElectron_T[1]) > 2) NumbKaonBkg4++;
        }

        if(abs(PIDTPCKaon_T[0]) < 3 && abs(PIDTPCKaon_T[1]) < 3)
        {
            NumbKaon2PID++;
        }
        if(abs(PIDTPCKaon_T[0]) < 3 || abs(PIDTPCKaon_T[1]) < 3)
        {
            NumbKaon1PID++;
        }
    }

    // --------- Count sig and Bkg events after different PID cuts
    Int_t cuts[4] = {1, 2, 3, 4};
    Int_t Nsig[4] = {NumbKaonSig1, NumbKaonSig2, NumbKaonSig3, NumbKaonSig4};
    Int_t Nbkg[4] = {NumbKaonBkg1, NumbKaonBkg2, NumbKaonBkg3, NumbKaonBkg4};
    auto cNumbKaon = new TCanvas("cNumbKaon", "cNumbKaon", 600, 400);
    cNumbKaon->cd();
    // auto h = new TH1F("h","test",4,cuts[0],cuts[4-1]);
    // for(int i=1;i<=4;i++) h->GetXaxis()->SetBinLabel(i,cutsLabel[i-1]);
    // h->Draw();
    auto grNumbKaonSig = new TGraph(4, cuts, Nsig);
    grNumbKaonSig->SetMarkerStyle(20);
    grNumbKaonSig->SetMarkerColor(kBlue);
    grNumbKaonSig->SetMinimum(0.0);
    grNumbKaonSig->SetTitle(";Cuts;Number of kaon pair events");
    auto grNumbKaonBkg = new TGraph(4, cuts, Nbkg);
    grNumbKaonBkg->SetMarkerStyle(20);
    grNumbKaonBkg->SetMarkerColor(kRed);
    auto lNumbKaon = new TLegend(0.6, 0.6, 0.75, 0.8);
    lNumbKaon->SetTextSize(0.03);
    lNumbKaon->SetBorderSize(0);
    grNumbKaonSig->Draw("AP");
    grNumbKaonBkg->Draw("PSAME");
    lNumbKaon->AddEntry(grNumbKaonSig, "#sigma^{2}_{K^{+}} + #sigma^{2}_{K^{-}} < 9", "p");
    lNumbKaon->AddEntry(grNumbKaonBkg, "16 < #sigma^{2}_{K^{+}} + #sigma^{2}_{K^{-}} < 25", "p");
    lNumbKaon->Draw();
    grNumbKaonSig->Write();
    grNumbKaonBkg->Write();
    cNumbKaon->Print(Form("%s/cNumbKaon.root", outDir.Data()),"root");
    cNumbKaon->Print(Form("%s/cNumbKaon.pdf", outDir.Data()),"pdf");

    // -------------- S.t.d dE/dx vs. track momentum
    // dE/dx vs p
    auto cDEdxVsP = new TCanvas("cDEdxVsP", "cDEdxVsP", 600, 400);
    cDEdxVsP->cd();
    hDEdxVsP->Draw("colz");
    hDEdxVsP->Write();
    cDEdxVsP->Print(Form("%s/cDEdxVsP.root", outDir.Data()),"root");
    cDEdxVsP->Print(Form("%s/cDEdxVsP.pdf", outDir.Data()),"pdf");
    // Kaon
    auto cStdDEdxVsPKaon = new TCanvas("cStdDEdxVsPKaon", "cStdDEdxVsPKaon", 600, 400);
    cStdDEdxVsPKaon->cd();
    hStdDEdxVsPKaon->Draw("colz");
    hStdDEdxVsPKaon->Write();
    cStdDEdxVsPKaon->Print(Form("%s/cStdDEdxVsPKaon.root", outDir.Data()),"root");
    cStdDEdxVsPKaon->Print(Form("%s/cStdDEdxVsPKaon.pdf", outDir.Data()),"pdf");
    // Pion
    auto cStdDEdxVsPPion = new TCanvas("cStdDEdxVsPPion", "cStdDEdxVsPPion", 600, 400);
    cStdDEdxVsPPion->cd();
    hStdDEdxVsPPion->Draw("colz");
    hStdDEdxVsPPion->Write();
    cStdDEdxVsPPion->Print(Form("%s/cStdDEdxVsPPion.root", outDir.Data()),"root");
    cStdDEdxVsPPion->Print(Form("%s/cStdDEdxVsPPion.pdf", outDir.Data()),"pdf");
    // Muon
    auto cStdDEdxVsPMuon = new TCanvas("cStdDEdxVsPMuon", "cStdDEdxVsPMuon", 600, 400);
    cStdDEdxVsPMuon->cd();
    hStdDEdxVsPMuon->Draw("colz");
    hStdDEdxVsPMuon->Write();
    cStdDEdxVsPMuon->Print(Form("%s/cStdDEdxVsPMuon.root", outDir.Data()),"root");
    cStdDEdxVsPMuon->Print(Form("%s/cStdDEdxVsPMuon.pdf", outDir.Data()),"pdf");
    // Electron
    auto cStdDEdxVsPElectron = new TCanvas("cStdDEdxVsPElectron", "cStdDEdxVsPElectron", 600, 400);
    cStdDEdxVsPElectron->cd();
    hStdDEdxVsPElectron->Draw("colz");
    hStdDEdxVsPElectron->Write();
    cStdDEdxVsPElectron->Print(Form("%s/cStdDEdxVsPElectron.root", outDir.Data()),"root");
    cStdDEdxVsPElectron->Print(Form("%s/cStdDEdxVsPElectron.pdf", outDir.Data()),"pdf");

    cout<<"Nsig[0] = "<<Nsig[0]<<" | Nbkg[0] = "<<Nbkg[0]<<endl;
    cout<<"Nsig[1] = "<<Nsig[1]<<" | Nbkg[1] = "<<Nbkg[1]<<endl;
    cout<<"Nsig[2] = "<<Nsig[2]<<" | Nbkg[2] = "<<Nbkg[2]<<endl;
    cout<<"Nsig[3] = "<<Nsig[3]<<" | Nbkg[3] = "<<Nbkg[3]<<endl;
    // count number of events pairs with only one of them is kaon
    cout<<"-------- Number of event pairs with two of them is kaon = "<<NumbKaon2PID<<endl;
    cout<<"-------- Number of events pairs with only one of them is kaon = "<<NumbKaon1PID<<endl;
/*
    // -------------- Pair kinematics --------------

    // Azimuthal angle 
    auto cAzimuthal = new TCanvas("cAzimuthal", "cAzimuthal", 600, 400);
    cAzimuthal->cd();
    auto hAzimuthal = new TH1F("hAzimuthal", "; pair-#phi (rad); Counts", 300,-TMath::Pi(), TMath::Pi());
    inputTree->Project("hAzimuthal", "Phi_T");
    hAzimuthal->SetMinimum(0.);
    hAzimuthal->Draw();
    hAzimuthal->Write();
    cAzimuthal->Print("/home/nacer/upc/RhoAna/output_RhoAna/cAzimuthal.root","root");
    cAzimuthal->Print("/home/nacer/upc/RhoAna/output_RhoAna/cAzimuthal.pdf","pdf");

    // Rapidity
    auto cRapidity = new TCanvas("cRapidity", "cRapidity", 600, 400);
    cRapidity->cd();
    auto hRapidity = new TH1F("hRapidity", "; pair-y; Counts", 300, -1., 1.);
    inputTree->Project("hRapidity", "Rapidity_T");
    hRapidity->Draw();
    hRapidity->Write();
    cRapidity->Print("/home/nacer/upc/RhoAna/output_RhoAna/cRapidity.root","root");
    cRapidity->Print("/home/nacer/upc/RhoAna/output_RhoAna/cRapidity.pdf","pdf");
    
    //  -------------- attempt to reject pions and electrons in favour of kaons -- 3 sigma

    // Invariant Mass

    // K+K-
    auto hMassKpKm = new TH1F("hMassKpKm", ";Invariant Mass (GeV/c^{2}); Counts", 300, 0.3, 1.4);
    // Pi+Pi-
    auto hMassPipPim = new TH1F("hMassPipPim", ";Invariant Mass (GeV/c^{2}); Counts", 300, 0.3, 1.4);
    hMassPipPim->SetLineColor(2);
    // K+K- 2 sigma dE/dx cut
    auto hMassKpKm2sigmaCut = new TH1F("hMassKpKm2sigmaCut", ";Invariant Mass (GeV/c^{2}); Counts", 300, 0.3, 1.4);
    hMassKpKm2sigmaCut->SetMarkerStyle(7);
    // Pi+Pi- 2 sigma dE/dx cut
    auto hMassPipPim2sigmaCut = new TH1F("hMassPipPim2sigmaCut", ";Invariant Mass (GeV/c^{2}); Counts", 300, 0.3, 1.4);
    hMassPipPim2sigmaCut->SetMarkerColor(2);
    hMassPipPim2sigmaCut->SetMarkerStyle(7);

    //  Transverse momentum

    // K+K- in Phi(1020) region
    auto hPtKpKm = new TH1F("hPtKpKm", ";p_{T} (GeV/c); Counts", 50, 0.,1.);
    hPtKpKm->SetMarkerColor(2);
    hPtKpKm->SetMarkerStyle(kFullCircle);
    // K+K- with high mass
    auto hPtKpKmHighMass = new TH1F("hPtKpKmHighMass", ";p_{T} (GeV/c); Counts", 50, 0.,1.);
    hPtKpKmHighMass->SetMarkerColor(4);
    hPtKpKmHighMass->SetMarkerStyle(kOpenCircle);

    // Study Phi(1020) and higher mass regions
    // Phi(1020) mass region
    // Pair rapidity
    auto hRapidityPhiMass = new TH1F("hRapidityPhiMass", "; Pair y; Counts", 100,-1.,1.);
    hRapidityPhiMass->SetMarkerColor(2);
    hRapidityPhiMass->SetMarkerStyle(kFullCircle);
    // Pair azimuthal
    auto hAzimuthalPhiMass = new TH1F("hAzimuthalPhiMass", "; Pair #phi (rad); Counts", 20,-TMath::Pi(), TMath::Pi());
    hAzimuthalPhiMass->SetMarkerColor(2);
    hAzimuthalPhiMass->SetMarkerStyle(kFullCircle);
    // difference between two track
    auto hDiffAzimuthalPhiMass = new TH1F("hDiffAzimuthalPhiMass", "; #Delta#phi (rad); Counts", 100,-TMath::Pi(), TMath::Pi());
    hDiffAzimuthalPhiMass->SetMarkerColor(2);
    hDiffAzimuthalPhiMass->SetMarkerStyle(kFullCircle);
    // Track pseudo-rapidity
    auto hTrackPseudoRapidityPhiMass = new TH1F("hTrackPseudoRapidityPhiMass", "; Track #eta; Counts", 100,-1.,1.);
    hTrackPseudoRapidityPhiMass->SetMarkerColor(2);
    hTrackPseudoRapidityPhiMass->SetMarkerStyle(kFullCircle);

    // Higher K+K- mass region
    // Pair rapidity
    auto hRapidityHighMass = new TH1F("hRapidityHighMass", "; y; Counts", 100,-1.,1.);
    hRapidityHighMass->SetMarkerColor(4);
    hRapidityHighMass->SetMarkerStyle(kOpenCircle);
    // Pair azimuthal
    auto hAzimuthalHighMass = new TH1F("hAzimuthalHighMass", "; #phi (rad); Counts", 20,-TMath::Pi(), TMath::Pi());
    hAzimuthalHighMass->SetMarkerColor(4);
    hAzimuthalHighMass->SetMarkerStyle(kOpenCircle);
    // difference between two track
    auto hDiffAzimuthalHighMass = new TH1F("hDiffAzimuthalHighMass", "; #Delta#phi (rad); Counts", 100,-TMath::Pi(), TMath::Pi());
    hDiffAzimuthalHighMass->SetMarkerColor(4);
    hDiffAzimuthalHighMass->SetMarkerStyle(kOpenCircle);
    // Track pseudo-rapidity
    auto hTrackPseudoRapidityHighMass = new TH1F("hTrackPseudoRapidityHighMass", "; Track y; Counts", 100,-1.,1.);
    hTrackPseudoRapidityHighMass->SetMarkerColor(4);
    hTrackPseudoRapidityHighMass->SetMarkerStyle(kOpenCircle);

    // dE/dx sigma vs. transverse momentum
    auto hDEdxVsPPion = new TH2D("hDEdxVsPPion", "; p(GeV/c); #sigma_{(dE/dx)_{#pi}}", 1000,0.,5., 1000,-10.,50.);
    auto hDEdxVsPElectron = new TH2D("hDEdxVsPElectron", "; p(GeV/c); #sigma_{(dE/dx)_{e}}", 1000,0.,5., 1000,-10.,50.);

    // dE/dx sigma 1 vs. dE/dx sigma 2
    auto hDEdx1VsDEdx2Pion = new TH2D("hDEdx1VsDEdx2Pion", ";#sigma^{#pi^{+}}_{dE/dx}; #sigma^{#pi^{-}}_{dE/dx}", 1000,-10.,50., 1000,-10.,50.);
    auto hDEdx1VsDEdx2Electron = new TH2D("hDEdx1VsDEdx2Electron", ";#sigma^{e^{+}}_{dE/dx}; #sigma^{e^{-}}_{dE/dx}", 1000,-10.,50., 1000,-10.,50.);
    auto hDEdx1VsDEdx2Kaon = new TH2D("hDEdx1VsDEdx2Kaon", ";#sigma^{e^{+}}_{dE/dx} - f(#sigma^{K^{+}}_{dE/dx}); #sigma^{e^{-}}_{dE/dx} - f(#sigma^{K^{-}}_{dE/dx})", 1000,-50.,50., 1000,-50.,50.);
    // K+K- mass regions
    auto hDEdx1VsDEdx2KaonPhiMass = new TH2D("hDEdx1VsDEdx2KaonPhiMass", ";#sigma^{e^{+}}_{dE/dx} - f(#sigma^{K^{+}}_{dE/dx}); #sigma^{e^{-}}_{dE/dx} - f(#sigma^{K^{-}}_{dE/dx})", 1000,-50.,50., 1000,-50.,50.);
    auto hDEdx1VsDEdx2KaonHighMass = new TH2D("hDEdx1VsDEdx2KaonHighMass", ";#sigma^{e^{+}}_{dE/dx} - f(#sigma^{K^{+}}_{dE/dx}); #sigma^{e^{-}}_{dE/dx} - f(#sigma^{K^{-}}_{dE/dx})", 1000,-50.,50., 1000,-50.,50.);

    // get the variables inside input TTree branches
    Int_t nTracks = 2;
    Float_t Mass_T = -100;
    Float_t Pt_T = -100;
	Float_t Rapidity_T = -100;
	Float_t Phi_T = -100;
	Bool_t LikeSign_T = -100;
    Float_t TrackPx_T[2] = {-100};
    Float_t TrackPy_T[2] = {-100};
    Float_t TrackPz_T[2] = {-100};
    Float_t TrackP_T[2] = {-100};
    Float_t TrackEta_T[2] = {-100};
    Float_t TrackPhi_T[2] = {-100};
    Float_t PIDTPCElectron_T[2] = {-100};
    Float_t PIDTPCPion_T[2] = {-100};
    inputTree->SetBranchAddress("Mass_T", &Mass_T);
    inputTree->SetBranchAddress("Pt_T", &Pt_T);
    inputTree->SetBranchAddress("Rapidity_T", &Rapidity_T);
    inputTree->SetBranchAddress("Phi_T", &Phi_T);
    inputTree->SetBranchAddress("LikeSign_T", &LikeSign_T);
    inputTree->SetBranchAddress("TrackPx_T", &TrackPx_T);
    inputTree->SetBranchAddress("TrackPy_T", &TrackPy_T);
    inputTree->SetBranchAddress("TrackPz_T", &TrackPz_T);
    inputTree->SetBranchAddress("TrackP_T", &TrackP_T);
    inputTree->SetBranchAddress("TrackEta_T", &TrackEta_T);
    inputTree->SetBranchAddress("TrackPhi_T", &TrackPhi_T);
    inputTree->SetBranchAddress("PIDTPCElectron_T", &PIDTPCElectron_T);
    inputTree->SetBranchAddress("PIDTPCPion_T", &PIDTPCPion_T);

    // grab the pion mass from PDG
    TDatabasePDG *pdgdat = TDatabasePDG::Instance(); 
  	TParticlePDG *partPion = pdgdat->GetParticle(211); //211:pion, 321:kaon, 11:electron
  	TParticlePDG *partKaon = pdgdat->GetParticle(321); //211:pion, 321:kaon, 11:electron
  	TParticlePDG *partElectron = pdgdat->GetParticle(11); //211:pion, 321:kaon, 11:electron
  	Double_t pionMass = partPion->Mass();
  	Double_t kaonMass = partKaon->Mass();
  	Double_t electronMass = partElectron->Mass();

    TLorentzVector lvPion[2];
    TLorentzVector lvKaon[2];
    TLorentzVector lvElectron[2];
    TLorentzVector lvSumPion; // pair-4vector
    TLorentzVector lvSumKaon;
    TLorentzVector lvSumElectron;

    // loop over all the events
    for (int i = 0; i < inputTree->GetEntries(); i++)
    {
        inputTree->GetEntry(i);

        // loop over all tracks
        for(Int_t iTrack=0; iTrack<2; iTrack++)
        {
            // creat 4-momenta using Px, Py, Pz, E per track
            // before that compute E (for PID mass hypo)
            float pionEnergy = sqrt(TrackP_T[iTrack]*TrackP_T[iTrack] + pionMass*pionMass);
            float kaonEnergy = sqrt(TrackP_T[iTrack]*TrackP_T[iTrack] + kaonMass * kaonMass);
            float electronEnergy = sqrt(TrackP_T[iTrack]*TrackP_T[iTrack] + electronMass * electronMass);
		    lvPion[iTrack].SetPxPyPzE(TrackPx_T[iTrack], TrackPy_T[iTrack], TrackPz_T[iTrack], pionEnergy);
		    lvKaon[iTrack].SetPxPyPzE(TrackPx_T[iTrack], TrackPy_T[iTrack], TrackPz_T[iTrack], kaonEnergy);
		    lvElectron[iTrack].SetPxPyPzE(TrackPx_T[iTrack], TrackPy_T[iTrack], TrackPz_T[iTrack], electronEnergy);

            // if(i%100000==0) cout<<"event = "<<i<<" | lvPion["<<iTrack<<"].P = "<<lvPion[iTrack].P()<<" | TrackP_T["<<iTrack<<"] = "<<TrackP_T[iTrack]<<endl;
        }

        lvSumPion = lvPion[0] + lvPion[1];
	    lvSumKaon = lvKaon[0] + lvKaon[1];
	    lvSumElectron = lvElectron[0] + lvElectron[1];

        if(LikeSign_T == 1) continue; //save only events with opposite charge track pairs

        // track pseudo-rapidity in K+ K- mass regions
        for(Int_t iTrack=0; iTrack<2; iTrack++)
        {
            // dE/dx n-sigma TPC
            hDEdxVsPPion->Fill(lvPion[iTrack].P(), PIDTPCPion_T[iTrack]);
            hDEdxVsPElectron->Fill(lvElectron[iTrack].P(), PIDTPCElectron_T[iTrack]);

            if(abs(PIDTPCPion_T[iTrack]) > 3 && abs(PIDTPCElectron_T[iTrack]) > 3 && lvSumKaon.M() > 0.95 && lvSumKaon.M() < 1.15 && lvSumKaon.Pt() < 0.1)
                hTrackPseudoRapidityPhiMass->Fill(lvKaon[iTrack].PseudoRapidity());

            if(abs(PIDTPCPion_T[iTrack]) > 3 && abs(PIDTPCElectron_T[iTrack]) > 3 && lvSumKaon.M() > 1.15 && lvSumKaon.Pt() < 0.1)
                hTrackPseudoRapidityHighMass->Fill(lvKaon[iTrack].PseudoRapidity());
        }

        // dE/dx1 vs.dE/dx2
        hDEdx1VsDEdx2Pion->Fill(PIDTPCPion_T[0], PIDTPCPion_T[1]);
        hDEdx1VsDEdx2Electron->Fill(PIDTPCElectron_T[0], PIDTPCElectron_T[1]);

        if(abs(PIDTPCPion_T[0]) > 2 && abs(PIDTPCPion_T[1]) > 2 && abs(PIDTPCElectron_T[0]) > 2 && abs(PIDTPCElectron_T[1]) > 2)
        {
            hMassKpKm2sigmaCut->Fill(lvSumKaon.M());
            hMassPipPim2sigmaCut->Fill(lvSumPion.M());
        }

        if(abs(PIDTPCPion_T[0]) > 3 && abs(PIDTPCPion_T[1]) > 3 && abs(PIDTPCElectron_T[0]) > 3 && abs(PIDTPCElectron_T[1]) > 3)
        {
            hMassKpKm->Fill(lvSumKaon.M());
            hMassPipPim->Fill(lvSumPion.M());

            // fill oupt TTree branch
            px1 = lvKaon[0].Px();
            py1 = lvKaon[0].Py();
            pz1 = lvKaon[0].Pz();
            px2 = lvKaon[1].Px();
            py2 = lvKaon[1].Py();
            pz2 = lvKaon[1].Pz();
            outputTree.Fill();

            if(lvSumKaon.M() > 0.95 && lvSumKaon.M() < 1.15)
            {
                hPtKpKm->Fill(lvSumKaon.Pt());

                if(lvSumKaon.Pt() < 0.1)
                {
                    hRapidityPhiMass->Fill(lvSumKaon.Rapidity());
                    hAzimuthalPhiMass->Fill(lvSumKaon.Phi());
                    hDiffAzimuthalPhiMass->Fill(lvKaon[0].Phi() - lvKaon[1].Phi());
                    hTrackPseudoRapidityPhiMass->Fill(lvKaon[0].PseudoRapidity());
                }
            }
            if(lvSumKaon.M() > 1.15)
            {
                hPtKpKmHighMass->Fill(lvSumKaon.Pt());

                if(lvSumKaon.Pt() < 0.1)
                {
                    hRapidityHighMass->Fill(lvSumKaon.Rapidity());
                    hAzimuthalHighMass->Fill(lvSumKaon.Phi());
                    hDiffAzimuthalHighMass->Fill(lvKaon[0].Phi() - lvKaon[1].Phi());
                    hTrackPseudoRapidityHighMass->Fill(lvKaon[0].PseudoRapidity());
                }
            }
        }
    }

    outputTree.Write();

    //---------- dE/dx n-sigma vs. p

    // pions
    auto cDEdxVsPPion = new TCanvas("cDEdxVsPPion", "cDEdxVsPPion", 600, 400);
    cDEdxVsPPion->cd();
    hDEdxVsPPion->Draw("colz");
    hDEdxVsPPion->Write();
    cDEdxVsPPion->Print("/home/nacer/upc/RhoAna/output_RhoAna/cDEdxVsPPion.root","root");
    cDEdxVsPPion->Print("/home/nacer/upc/RhoAna/output_RhoAna/cDEdxVsPPion.pdf","pdf");

    // electrons
    auto cDEdxVsPElectron = new TCanvas("cDEdxVsPElectron", "cDEdxVsPElectron", 600, 400);
    cDEdxVsPElectron->cd();
    hDEdxVsPElectron->Draw("colz");
    auto fitDEdxVsPElectron = new TF1("fitDEdxVsPElectron","TMath::Exp(-[0]*x + [1]) + [2]",0.,5.);
    fitDEdxVsPElectron->SetParameters(8.5, 6., -5.); // pions: 13., 4.0, -6.0, kaons: 8.5, 6., -5.
    fitDEdxVsPElectron->SetParLimits(0, 8.4, 8.6);
    fitDEdxVsPElectron->SetParLimits(1, 5.9, 6.1);
    fitDEdxVsPElectron->SetParLimits(2, -4.9, -5.1);
    hDEdxVsPElectron->Fit(fitDEdxVsPElectron); //, "R"
    hDEdxVsPElectron->Write();
    cDEdxVsPElectron->Print("/home/nacer/upc/RhoAna/output_RhoAna/cDEdxVsPElectron.root","root");
    cDEdxVsPElectron->Print("/home/nacer/upc/RhoAna/output_RhoAna/cDEdxVsPElectron.pdf","pdf");

    // Evaluate n-sigma dE/dx for Kaons
    for (int i = 0; i < inputTree->GetEntries(); i++)
    {
        inputTree->GetEntry(i);

        for(Int_t iTrack=0; iTrack<2; iTrack++)
        {
            // creat 4-momenta using Px, Py, Pz, E per track
            float kaonEnergy = sqrt(TrackP_T[iTrack]*TrackP_T[iTrack] + kaonMass * kaonMass);
		    lvKaon[iTrack].SetPxPyPzE(TrackPx_T[iTrack], TrackPy_T[iTrack], TrackPz_T[iTrack], kaonEnergy);
        }

	    lvSumKaon = lvKaon[0] + lvKaon[1];

        if(LikeSign_T == 1) continue;

        // Nsigma dEdx vs p for every track.
        hDEdx1VsDEdx2Kaon->Fill(PIDTPCElectron_T[0] - fitDEdxVsPElectron->Eval(TrackP_T[0]), PIDTPCElectron_T[1] - fitDEdxVsPElectron->Eval(TrackP_T[1]));

        // Phi(1020) mass region
        if (abs(PIDTPCPion_T[0]) > 3 && abs(PIDTPCPion_T[1]) > 3 && abs(PIDTPCElectron_T[0]) > 3 && abs(PIDTPCElectron_T[1]) > 3 && lvSumKaon.M() > 0.95 && lvSumKaon.M() < 1.15)
        {
            hDEdx1VsDEdx2KaonPhiMass->Fill(PIDTPCElectron_T[0] - fitDEdxVsPElectron->Eval(TrackP_T[0]), PIDTPCElectron_T[1] - fitDEdxVsPElectron->Eval(TrackP_T[1]));
        }
        if (abs(PIDTPCPion_T[0]) > 3 && abs(PIDTPCPion_T[1]) > 3 && abs(PIDTPCElectron_T[0]) > 3 && abs(PIDTPCElectron_T[1]) > 3 && lvSumKaon.M() > 1.15)
        {
            hDEdx1VsDEdx2KaonHighMass->Fill(PIDTPCElectron_T[0] - fitDEdxVsPElectron->Eval(TrackP_T[0]), PIDTPCElectron_T[1] - fitDEdxVsPElectron->Eval(TrackP_T[1]));
        }
    }

    // dEdx sigma correlations 
    // pions
    auto cDEdx1VsDEdx2Pion = new TCanvas("cDEdx1VsDEdx2Pion", "cDEdx1VsDEdx2Pion", 600, 400);
    cDEdx1VsDEdx2Pion->cd();
    hDEdx1VsDEdx2Pion->Draw("colz");
    hDEdx1VsDEdx2Pion->Write();
    cDEdx1VsDEdx2Pion->Print("/home/nacer/upc/RhoAna/output_RhoAna/cDEdx1VsDEdx2Pion.root","root");
    cDEdx1VsDEdx2Pion->Print("/home/nacer/upc/RhoAna/output_RhoAna/cDEdx1VsDEdx2Pion.pdf","pdf");
    
    // electrons
    auto cDEdx1VsDEdx2Electron = new TCanvas("cDEdx1VsDEdx2Electron", "cDEdx1VsDEdx2Electron", 600, 400);
    cDEdx1VsDEdx2Electron->cd();
    hDEdx1VsDEdx2Electron->Draw("colz");
    hDEdx1VsDEdx2Electron->Write();
    cDEdx1VsDEdx2Electron->Print("/home/nacer/upc/RhoAna/output_RhoAna/cDEdx1VsDEdx2Electron.root","root");
    cDEdx1VsDEdx2Electron->Print("/home/nacer/upc/RhoAna/output_RhoAna/cDEdx1VsDEdx2Electron.pdf","pdf");

    // kons
    auto cDEdx1VsDEdx2Kaon = new TCanvas("cDEdx1VsDEdx2Kaon", "cDEdx1VsDEdx2Kaon", 600, 400);
    cDEdx1VsDEdx2Kaon->cd();
    hDEdx1VsDEdx2Kaon->Draw("colz");
    hDEdx1VsDEdx2Kaon->Write();
    cDEdx1VsDEdx2Kaon->Print("/home/nacer/upc/RhoAna/output_RhoAna/cDEdx1VsDEdx2Kaon.root","root");
    cDEdx1VsDEdx2Kaon->Print("/home/nacer/upc/RhoAna/output_RhoAna/cDEdx1VsDEdx2Kaon.pdf","pdf");
    // K+K- mass regions
    auto cDEdx1VsDEdx2KaonPhiMass = new TCanvas("cDEdx1VsDEdx2KaonPhiMass", "cDEdx1VsDEdx2KaonPhiMass", 600, 400);
    cDEdx1VsDEdx2KaonPhiMass->cd();
    hDEdx1VsDEdx2KaonPhiMass->Draw("colz");
    hDEdx1VsDEdx2KaonPhiMass->Write();
    cDEdx1VsDEdx2KaonPhiMass->Print("/home/nacer/upc/RhoAna/output_RhoAna/cDEdx1VsDEdx2KaonPhiMass.root","root");
    cDEdx1VsDEdx2KaonPhiMass->Print("/home/nacer/upc/RhoAna/output_RhoAna/cDEdx1VsDEdx2KaonPhiMass.pdf","pdf");
    auto cDEdx1VsDEdx2KaonHighMass = new TCanvas("cDEdx1VsDEdx2KaonHighMass", "cDEdx1VsDEdx2KaonHighMass", 600, 400);
    cDEdx1VsDEdx2KaonHighMass->cd();
    hDEdx1VsDEdx2KaonHighMass->Draw("colz");
    hDEdx1VsDEdx2KaonHighMass->Write();
    cDEdx1VsDEdx2KaonHighMass->Print("/home/nacer/upc/RhoAna/output_RhoAna/cDEdx1VsDEdx2KaonHighMass.root","root");
    cDEdx1VsDEdx2KaonHighMass->Print("/home/nacer/upc/RhoAna/output_RhoAna/cDEdx1VsDEdx2KaonHighMass.pdf","pdf");

    //---------- Study Phi(1020) and higher mass regions

    // Pair rapidity
    auto cRapidityKpKm = new TCanvas("cRapidityKpKm", "cRapidityKpKm", 600, 400);
    auto lRapidityKpKm = new TLegend(0.65, 0.7, 0.85, 0.85);
    lRapidityKpKm->SetTextSize(0.03);
    lRapidityKpKm->SetBorderSize(0);
    // cRapidityKpKm->SetLogy();
    cRapidityKpKm->cd();
    hRapidityPhiMass->Draw("e");
    hRapidityHighMass->Draw("esame");
    lRapidityKpKm->AddEntry(hRapidityPhiMass, "0.95 < m_{K^{+}K^{-}} < 1.15 GeV/c^{2} ", "p");
    lRapidityKpKm->AddEntry(hRapidityHighMass, "m_{K^{+}K^{-}} > 1.15 GeV/c^{2}", "p");
    lRapidityKpKm->Draw();
    hRapidityPhiMass->Write();
    hRapidityHighMass->Write();
    cRapidityKpKm->Print("/home/nacer/upc/RhoAna/output_RhoAna/cRapidityKpKm.root","root");
    cRapidityKpKm->Print("/home/nacer/upc/RhoAna/output_RhoAna/cRapidityKpKm.pdf","pdf");

    // Pair azimuthal
    auto cAzimuthalKpKm = new TCanvas("cAzimuthalKpKm", "cAzimuthalKpKm", 600, 400);
    auto lAzimuthalKpKm = new TLegend(0.2, 0.7, 0.4, 0.85);
    lAzimuthalKpKm->SetTextSize(0.03);
    lAzimuthalKpKm->SetBorderSize(0);
    // cAzimuthalKpKm->SetLogy();
    cAzimuthalKpKm->cd();
    hAzimuthalPhiMass->Draw("e");
    hAzimuthalHighMass->Draw("esame");
    lAzimuthalKpKm->AddEntry(hAzimuthalPhiMass, "0.95 < m_{K^{+}K^{-}} < 1.15 GeV/c^{2} ", "p");
    lAzimuthalKpKm->AddEntry(hAzimuthalHighMass, "m_{K^{+}K^{-}} > 1.15 GeV/c^{2}", "p");
    lAzimuthalKpKm->Draw();
    hAzimuthalPhiMass->Write();
    hAzimuthalHighMass->Write();
    cAzimuthalKpKm->Print("/home/nacer/upc/RhoAna/output_RhoAna/cAzimuthalKpKm.root","root");
    cAzimuthalKpKm->Print("/home/nacer/upc/RhoAna/output_RhoAna/cAzimuthalKpKm.pdf","pdf");

    // difference between two track
    auto cDiffAzimuthalKpKm = new TCanvas("cDiffAzimuthalKpKm", "cDiffAzimuthalKpKm", 600, 400);
    auto lDiffAzimuthalKpKm = new TLegend(0.4, 0.7, 0.6, 0.85);
    lDiffAzimuthalKpKm->SetTextSize(0.03);
    lDiffAzimuthalKpKm->SetBorderSize(0);
    // cDiffAzimuthalKpKm->SetLogy();
    cDiffAzimuthalKpKm->cd();
    hDiffAzimuthalPhiMass->Draw("e");
    hDiffAzimuthalHighMass->Draw("esame");
    lDiffAzimuthalKpKm->AddEntry(hDiffAzimuthalPhiMass, "0.95 < m_{K^{+}K^{-}} < 1.15 GeV/c^{2} ", "p");
    lDiffAzimuthalKpKm->AddEntry(hDiffAzimuthalHighMass, "m_{K^{+}K^{-}} > 1.15 GeV/c^{2}", "p");
    lDiffAzimuthalKpKm->Draw();
    hDiffAzimuthalPhiMass->Write();
    hDiffAzimuthalHighMass->Write();
    cDiffAzimuthalKpKm->Print("/home/nacer/upc/RhoAna/output_RhoAna/cDiffAzimuthalKpKm.root","root");
    cDiffAzimuthalKpKm->Print("/home/nacer/upc/RhoAna/output_RhoAna/cDiffAzimuthalKpKm.pdf","pdf");

    // Track pseudo-rapidity
    auto cTrackPseudoRapidityKpKm = new TCanvas("cTrackPseudoRapidityKpKm", "cTrackPseudoRapidityKpKm", 600, 400);
    auto lTrackPseudoRapidityKpKm = new TLegend(0.65, 0.7, 0.85, 0.85);
    lTrackPseudoRapidityKpKm->SetTextSize(0.03);
    lTrackPseudoRapidityKpKm->SetBorderSize(0);
    // cTrackPseudoRapidityKpKm->SetLogy();
    cTrackPseudoRapidityKpKm->cd();
    hTrackPseudoRapidityPhiMass->Draw("e");
    hTrackPseudoRapidityHighMass->Draw("esame");
    lTrackPseudoRapidityKpKm->AddEntry(hTrackPseudoRapidityPhiMass, "0.95 < m_{K^{+}K^{-}} < 1.15 GeV/c^{2} ", "p");
    lTrackPseudoRapidityKpKm->AddEntry(hTrackPseudoRapidityHighMass, "m_{K^{+}K^{-}} > 1.15 GeV/c^{2}", "p");
    lTrackPseudoRapidityKpKm->Draw();
    hTrackPseudoRapidityPhiMass->Write();
    hTrackPseudoRapidityHighMass->Write();
    cTrackPseudoRapidityKpKm->Print("/home/nacer/upc/RhoAna/output_RhoAna/cTrackPseudoRapidityKpKm.root","root");
    cTrackPseudoRapidityKpKm->Print("/home/nacer/upc/RhoAna/output_RhoAna/cTrackPseudoRapidityKpKm.pdf","pdf");

    //  -------------- attempt to reject pions and electrons in favour of kaons -- 3 sigma

    //  Transverse momentum

    auto cPtKpKm = new TCanvas("cPtKpKm", "cPtKpKm", 600, 400);
    auto lPtKpKm = new TLegend(0.3, 0.5, 0.5, 0.85);
    lPtKpKm->SetTextSize(0.03);
    lPtKpKm->SetBorderSize(0);
    // cPtKpKm->SetLogy();
    cPtKpKm->cd();
    hPtKpKm->Draw("e");
    hPtKpKmHighMass->Draw("esame");
    lPtKpKm->AddEntry(hPtKpKm, "0.95 < m_{K^{+}K^{-}} < 1.15 GeV/c^{2}", "p");
    lPtKpKm->AddEntry(hPtKpKmHighMass, "m_{K^{+}K^{-}} > 1.15 GeV/c^{2}", "p");
    lPtKpKm->Draw();
    hPtKpKm->Write();
    hPtKpKmHighMass->Write();
    cPtKpKm->Print("/home/nacer/upc/RhoAna/output_RhoAna/cPtKpKm.root","root");
    cPtKpKm->Print("/home/nacer/upc/RhoAna/output_RhoAna/cPtKpKm.pdf","pdf");

    //  invariant mass

    auto cMassKpKm = new TCanvas("cMassKpKm", "cMassKpKm", 600, 400);
    auto lMassKpKm = new TLegend(0.3, 0.5, 0.5, 0.85);
    lMassKpKm->SetTextSize(0.03);
    lMassKpKm->SetBorderSize(0);
    cMassKpKm->cd();
    hMassKpKm2sigmaCut->Draw("e");
    hMassPipPim2sigmaCut->Draw("esame");
    hMassKpKm->Draw("same");
    hMassPipPim->Draw("same");
    lMassKpKm->AddEntry(hMassKpKm, "K^{+}K^{-} with 3#sigma #pi/e reject", "l");
    lMassKpKm->AddEntry(hMassKpKm2sigmaCut, "K^{+}K^{-} with 2#sigma #pi/e reject", "p");
    lMassKpKm->AddEntry(hMassPipPim, "#pi^{+}#pi^{-} with 3#sigma #pi/e reject", "l");
    lMassKpKm->AddEntry(hMassPipPim2sigmaCut, "#pi^{+}#pi^{-} with 2#sigma #pi/e reject", "p");
    lMassKpKm->Draw();
    hMassKpKm->Write();
    hMassKpKm2sigmaCut->Write();
    hMassPipPim->Write();
    hMassPipPim2sigmaCut->Write();
    cMassKpKm->Print("/home/nacer/upc/RhoAna/output_RhoAna/cMassKpKm.root","root");
    cMassKpKm->Print("/home/nacer/upc/RhoAna/output_RhoAna/cMassKpKm.pdf","pdf");
*/    
}