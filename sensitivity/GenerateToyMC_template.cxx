#include <TRandom3.h>
#include "TMath.h"
#include <fstream>
#include <algorithm>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TLeaf.h"
#include "TString.h"


void GenerateToyMC(){
  
  // Get the file to generate from
  TFile *MC_file = new TFile("/projects/cuore/simulation/BkgSimulations/ntp/__filename__.root");
  // Get the file to be created
  TFile *out_file = new TFile("/projects/cuore/simulation/Majoron/sensitivity/__filename__.root", "RECREATE");
  
  TTree *MC_tree = (TTree*)MC_file->Get("outTree");
  TTree *MC_friend_tree = (TTree*)MC_file->Get("exclChTree");
  // The normalization factor
  double normalization_factor = __normalization_factor__;
  // The livetime factor
  double livetime_factor = __livetime_factor__;
  // number of events to generate
  double sampled_factor = normalization_factor * livetime_factor;

  // Grab events, and round down the number of events to sample
  int sampled_events = TMath::Floor(MC_tree->GetEntries() * sampled_factor);

  
  // The output trees
  TTree * outTree = new TTree("outTree", "");
  TTree * friendTree = new TTree("exclChTree", "");

  if (sampled_factor > 1)
    {
      std::cout << "Sampling more events than in histogram for file: " << MC_file << std::endl;
      std::cout << "Sampling factor: " << sampled_factor << std::endl;
    }
  std::cout << "Sampling " << sampled_events << " events of " << MC_tree->GetEntries() << " from file " << "__filename__" << std::endl;

  // Get the interesting branches
  int Channel;
  int Detector;
  double Ener2;
  double ESum2;
  short int Multiplicity;
  int Layer;
  int Tower;
  int Floor;
  int Dataset;
  bool Included;

  //MC_tree->SetBranchAddress("Channel", &Channel);
  //MC_tree->SetBranchAddress("Detector", &Detector);
  MC_tree->SetBranchAddress("Ener2", &Ener2);
  MC_tree->SetBranchAddress("ESum2", &ESum2);
  MC_tree->SetBranchAddress("Multiplicity", &Multiplicity);
  MC_tree->SetBranchAddress("Layer", &Layer);
  //MC_tree->SetBranchAddress("Tower", &Tower);
  //MC_tree->SetBranchAddress("Floor", &Floor);

  MC_friend_tree->SetBranchAddress("Dataset", &Dataset);
  MC_friend_tree->SetBranchAddress("Included", &Included);

  
  //outTree->Branch("Channel", &Channel, "Channel/I");
  //outTree->Branch("Detector", &Detector, "Detector/I");
  outTree->Branch("Ener2", &Ener2, "Ener2/D");
  outTree->Branch("ESum2", &ESum2, "ESum2/D");
  outTree->Branch("Multiplicity", &Multiplicity, "Multiplicity/S");
  outTree->Branch("Layer", &Layer, "Layer/I");
  //outTree->Branch("Tower", &Tower, "Tower/I");
  //outTree->Branch("Floor", &Floor, "Floor/I");

  friendTree->Branch("Dataset", &Dataset, "Dataset/I");
  friendTree->Branch("Included", &Included, "Included/O");

  // Start random number generator
  TRandom3 *rand = new TRandom3();
  rand->SetSeed();

  // Sample events from the histogram
  for (int i = 0; i < sampled_events; i++) {
    int event_to_sample = rand->Integer(sampled_events);
    MC_tree->GetEntry(i);
    outTree->Fill();
    MC_friend_tree->GetEntry(i);
    friendTree->Fill();
  }



  MC_file->Close();
  outTree->AddFriend(friendTree);
  outTree->Write();
  friendTree->Write();
  out_file->Close();
  
}
