// This script takes in a datafile and adds in a flag of Included to include the information for rejected channels in each dataset

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

using namespace std;

void addRejected() {
    stringstream ss;
    string line;

    int channel;

    map<int, vector<int> > rejectedChannels;
    ifstream dsFile("DS3021_rejected_channel.txt");
    while(getline(dsFile, line)) {
        ss.str(line);
        ss >> channel;

        rejectedChannels[3021].push_back(channel);

        ss.str("");
        ss.clear();
    }
    dsFile.close();
    dsFile.open("DS3518_rejected_channel.txt");
    while(getline(dsFile, line)) {
        ss.str(line);
        ss >> channel;

        rejectedChannels[3018].push_back(channel);

        ss.str("");
        ss.clear();
    }
    dsFile.close();

    bool included;
    int dataset;
    int run;

    TString fileName = "Data/Reduced_bkg_3018_3021_10percent.root";

    TFile* dataFile = new TFile(fileName, "update");
    TTree* dataTree = (TTree*) dataFile->Get("outTree");
    dataTree->SetBranchStatus("*", 0);
    dataTree->SetBranchStatus("Detector", 1);

    TTree* friendTree;
    if( (friendTree = (TTree*) dataFile->Get("exclChTree")) != NULL ) {
        dataTree->RemoveFriend(friendTree);
        friendTree->Delete("all");
    }
    friendTree = new TTree("exclChTree", "");

    dataTree->SetBranchAddress("Detector", &channel);
    dataTree->SetBranchAddress("Run", &run);

    friendTree->Branch("Dataset", &dataset, "Dataset/I");
    friendTree->Branch("Included", &included, "Included/O");

    for(Long64_t e = 0; e < dataTree->GetEntries(); e++) {
        dataTree->GetEntry(e);

	percent = std::floor(((double)i * 100.0) / dataTree->GetEntries());
	if ( percent > percent_previous)
	  {
	    std::cout << "Entries read: " << percent << "%" << "\r" << std::flush;
	    percent_previous = percent;
	  }
	
        if( run < 320000 )
            dataset = 3021;
        else
            dataset = 3018; 

        included = true;

        if( std::find( (rejectedChannels[dataset]).begin(), (rejectedChannels[dataset]).end(), channel ) != (rejectedChannels[dataset]).end() )
            included = false;

        friendTree->Fill();
    }
    dataTree->SetBranchStatus("*", 1);
    dataTree->AddFriend(friendTree);
    dataTree->Write("", TObject::kOverwrite);
    friendTree->Write();
    dataFile->Close();
    delete dataFile;
}

int main() {
    addRejected();
}
