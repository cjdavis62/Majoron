// This script takes in a reduced data file and creates a new file with 10% of the data of the original

#include "TROOT.h"
#include "TSystem.h"
#include "TMath.h"
#include "TString.h"
#include "TCut.h"
#include "TTree.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TLine.h"
#include "TFile.h"
#include "TF1.h"
#include "TH1F.h"
#include <math.h>
#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include "TComplex"

void frac_reduce() {

  TFile * inpFile = new TFile("/projects/cuore/data/ds3018_3021/Reduced_bkg_3018_3021.root");
  TFile * outFile = new TFile("/projects/cuore/data/ds3018_3021/Reduced_bkg_3018_3021_50percent.root", "Recreate");
    
  TTree * inpTree = (TTree*)inpFile->Get("outTree");

  TTree * outTree = new TTree("outTree", "fractional reduced file");
  
  int Channel, Detector;
  int Multiplicity;
  int Run;
  double Energy;
  double TotalEnergy;
  double Time;
  int MultipletIndex;
  bool PSA;
  int Layer;
  int Tower;
  int Floor;

  int test_val = 0;


  inpTree->SetBranchAddress("Channel", &Channel);
  inpTree->SetBranchAddress("Multiplicity", &Multiplicity);
  inpTree->SetBranchAddress("Run", &Run);
  inpTree->SetBranchAddress("Energy", &Energy);
  inpTree->SetBranchAddress("TotalEnergy", &TotalEnergy);
  inpTree->SetBranchAddress("Time", &Time);
  inpTree->SetBranchAddress("MultipletIndex", &MultipletIndex);
  inpTree->SetBranchAddress("PSA", &PSA);
  inpTree->SetBranchAddress("Layer", &Layer);
  inpTree->SetBranchAddress("Tower", &Tower);
  inpTree->SetBranchAddress("Floor", &Floor);


  outTree->Branch("Channel", &Channel, "Channel/I");
  outTree->Branch("Detector", &Detector, "Detector/I");
  outTree->Branch("Multiplicity", &Multiplicity, "Multiplicity/I");
  outTree->Branch("Run", &Run, "Run/I");
  outTree->Branch("Energy", &Energy, "Energy/D");
  outTree->Branch("TotalEnergy", &TotalEnergy, "TotalEnergy/D");
  outTree->Branch("Time", &Time, "Time/D");
  outTree->Branch("MultipletIndex", &MultipletIndex, "MultipletIndex/I");
  outTree->Branch("PSA", &PSA, "PSA/B");
  outTree->Branch("Layer", &Layer, "Layer/I");
  outTree->Branch("Tower", &Tower, "Tower/I");
  outTree->Branch("Floor", &Floor, "Floor/I");

  outTree->SetAlias("Ener2", "Energy");
  outTree->SetAlias("ESum2", "TotalEnergy");
  
  int seed = 1000;

  TRandom3 * random = new TRandom3(seed);

  int percent_previous = -1;
  for (int i = 0; i < inpTree->GetEntries(); i++) {
    inpTree->GetEntry(i);

    int percent = std::floor(((double)i * 100.0) / inpTree->GetEntries());
    if ( percent > percent_previous)
      {
	std::cout << "Entries read: " << percent << "%" << "\r" << std::flush;
	percent_previous = percent;
      }
    std::cout <<"\n" << std::endl;
    
    int j = random->Integer(2);
    if (j == 0) // take 50 percent of the values
    {
      Detector = Channel;
      //std::cout << j << std::endl;
      outTree->Fill();
      // write the outTree
    }
    
  }

    outTree->Write();
    outFile->Close();
}
