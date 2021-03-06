// This script takes in the unprocessed MC file to determine the summed energy spectrum of each of the Majoron spectral indices
// In order to only consider the summed electron spectrum, only events with 2 events in the chain are considered


#include "TROOT.h"
#include "TTree.h"
#include "TSystem.h"
#include "TFile.h"
#include "TH1.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TGraph.h"
#include "TGraphAsymmErrors.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TCut.h"
#include <iostream>
#include <sstream>
#include "THStack.h"
#include "TMath.h"
#include <stdio.h>
#include "TComplex.h"

TH1F event_collection(TTree *tree) {

  TH1F * ESum_hist = new TH1F ("ESum_hist", "Electron Summed Energy", 130, 0, 2.6);
  
  ULong64_t Chain_Number;
  double Energy;
  
  tree->SetBranchAddress("ChainNumber", &Chain_Number);
  tree->SetBranchAddress("ParticleInputEnergy", &Energy);
  
  int percent_previous = -1;
  double percent = 0;

  int Chain_Previous = 0;
  double Energy_Sum = 0;
  int Events_in_Chain = 0;
  
  for (int i = 0; i < tree->GetEntries(); i++) {
    tree->GetEntry(i);

    // Output what percent of file is read through
    percent = std::floor(((double)i * 1000) / tree->GetEntries());
    if ( percent > percent_previous)
      {
	std::cout << "Entries read: " << percent/10.0 << "%" << "\r" << std::flush;
	percent_previous = percent;
      }

    // Sum energies if particles are in the same chain
    if ((int)Chain_Number == Chain_Previous)
      {
	Events_in_Chain++;
	Energy_Sum += Energy;
      }
    else
      {
	// End once if  particles in the chain
	if (Events_in_Chain == 2) ESum_hist->Fill(Energy_Sum);
	
	Events_in_Chain = 1; // reset number of events in the chain
	Chain_Previous = Chain_Number; // add to new chain
	Energy_Sum = Energy; //first of the two electrons
      }


  }
  // Add an extra line to close off the std::flush from earlier
  std::cout << std::endl;
  double bin_width = ESum_hist->GetBinCenter(10) - ESum_hist->GetBinCenter(9);
  
  ESum_hist->Scale(1.0 / (bin_width*ESum_hist->GetEntries()));
  return *ESum_hist;
}

int foo() {

  TFile * f1_n1 = new TFile("/projects/cuore/simulation/Majoron/v2/Majoron_n1.root");
  TFile * f2_n2 = new TFile("/projects/cuore/simulation/Majoron/v2/Majoron_n2.root");
  TFile * f3_n3 = new TFile("/projects/cuore/simulation/Majoron/v2/Majoron_n3.root");
  TFile * f4_n5 = new TFile("/projects/cuore/simulation/Majoron/v2/Majoron_n5.root");
  TFile * f5_n7 = new TFile("/projects/cuore/simulation/Majoron/v2/Majoron_n7.root");

  TTree * tree_n1 = (TTree*)f1_n1->Get("qtree");
  TTree * tree_n2 = (TTree*)f2_n2->Get("qtree");
  TTree * tree_n3 = (TTree*)f3_n3->Get("qtree");
  TTree * tree_n5 = (TTree*)f4_n5->Get("qtree");
  TTree * tree_n7 = (TTree*)f5_n7->Get("qtree");
  
  int Chain_Previous = 0;
  double Energy_Sum = 0;

  std::cout << "Spectral Index 1" << std::endl;
  TH1F * ESum_hist_n1 = (TH1F*)event_collection(tree_n1).Clone();
  std::cout << "Spectral Index 2" << std::endl;
  TH1F * ESum_hist_n2 = (TH1F*)event_collection(tree_n2).Clone();
  std::cout << "Spectral Index 3" << std::endl;
  TH1F * ESum_hist_n3 = (TH1F*)event_collection(tree_n3).Clone();
  std::cout << "Spectral Index 5" << std::endl;
  TH1F * ESum_hist_n5 = (TH1F*)event_collection(tree_n5).Clone();
  std::cout << "Spectral Index 7" << std::endl;
  TH1F * ESum_hist_n7 = (TH1F*)event_collection(tree_n7).Clone();

  
  ESum_hist_n7->Draw();
  ESum_hist_n7->SetTitle("Spectral Index 7");
  ESum_hist_n7->SetLineColor(kRed);
  ESum_hist_n7->SetLineStyle(7);
  ESum_hist_n1->Draw("SAME");
  ESum_hist_n1->SetTitle("Spectral Index 1");
  ESum_hist_n1->SetLineColor(kMagenta);
  ESum_hist_n1->SetLineStyle(1);
  ESum_hist_n2->Draw("SAME");
  ESum_hist_n2->SetTitle("Spectral Index 2");
  ESum_hist_n2->SetLineColor(kBlue);
  ESum_hist_n2->SetLineStyle(2);
  ESum_hist_n3->Draw("Same");
  ESum_hist_n3->SetTitle("Spectral Index 3");
  ESum_hist_n3->SetLineColor(kBlack);
  ESum_hist_n3->SetLineStyle(3);
  ESum_hist_n5->SetTitle("Spectral Index 5 2#nu#beta#beta");
  ESum_hist_n5->SetLineColor(kCyan);
  ESum_hist_n5->Draw("Same");

  ESum_hist_n7->GetXaxis()->SetTitle("Summed Electron Energy [MeV]");
  ESum_hist_n7->GetYaxis()->SetTitle("Rate [Arbitrary Units]");

  TLegend *legend = new TLegend(0.65, 0.70, 0.88, 0.88);
  legend->AddEntry(ESum_hist_n1, "Spectral Index 1 (0#nu#beta#beta#chi)", "l");
  legend->AddEntry(ESum_hist_n2, "Spectral Index 2 (0#nu#beta#beta#chi)", "l");
  legend->AddEntry(ESum_hist_n3, "Spectral Index 3 (0#nu#beta#beta#chi(#chi))", "l");
  legend->AddEntry(ESum_hist_n5, "Spectral Index 5 (2#nu#beta#beta)", "l");
  legend->AddEntry(ESum_hist_n7, "Spectral Index 7 (0#nu#beta#beta#chi#chi)", "l");

  legend->Draw();

  return 0;
  
}


