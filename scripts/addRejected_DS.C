// This script takes as input the list of files in ListMC.txt in this directory
// It then takes the files and adds a DS flag to them based on the livetimes of each dataset
// It also adds in an Included flag based on whether or not that channel was included in the dataset

#include <fstream>
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


bool findChannelInVector(vector<int> &array, int channel)
{
  bool flag = false;
  for (vector<int>::iterator ptr = array.begin(); ptr < array.end(); ptr++) {
    if (*ptr == channel) {flag = true; break;}
  }
  return flag;
}

void addRejected() {
    ifstream listFile("ListMC.txt");
    stringstream ss;
    string line;
    string name;

    vector<TString> names;

    while(getline(listFile, line)) {
        ss.str(line);
        ss >> name;
        names.push_back(name.c_str());
        ss.str("");
        ss.clear();
    }
    listFile.close();

    double lt3021 = 0.0694474153;
    double lt3018 = 0.0572298326;

    double totLT = lt3021 + lt3018;

    int channel;
    short int ch;

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

    TString path = "/projects/cuore/simulation/Majoron/v2/"; 

    bool included;
    int dataset;
    bool III;

    for(int i = 0; i < names.size(); i++) {
        TString fileName = path + names[i];
        cout << fileName << endl;

        TFile* mcFile = new TFile(fileName, "update");
        TTree* mcTree = (TTree*) mcFile->Get("outTree");
        mcTree->SetBranchStatus("*", 0);
        mcTree->SetBranchStatus("Detector", 1);

        TTree* friendTree;
        if( (friendTree = (TTree*) mcFile->Get("exclChTree")) != NULL ) {
            mcTree->RemoveFriend(friendTree);
            friendTree->Delete("all");
        }
        friendTree = new TTree("exclChTree", "");

        III = false;

        TString type = mcTree->GetBranch("Detector")->GetLeaf("Detector")->GetTypeName();
        if( type == "Int_t" ) {
            mcTree->SetBranchAddress("Detector", &channel);
            III = true;
        } else {
            mcTree->SetBranchAddress("Detector", &ch);
        }

        friendTree->Branch("Dataset", &dataset, "Dataset/I");
        friendTree->Branch("Included", &included, "Included/O");

        for(Long64_t e = 0; e < mcTree->GetEntries(); e++) {
            mcTree->GetEntry(e);

	    percent = std::floor(((double)i * 100.0) / mcTree->GetEntries());
	    if ( percent > percent_previous)
	      {
		std::cout << "Entries read: " << percent << "%" << "\r" << std::flush;
		percent_previous = percent;
	      }
	    
            double rnd = rand() * totLT / (double)RAND_MAX;
            if( rnd < lt3021 )
                dataset = 3021;
            else
                dataset = 3018;

            included = true;
            
            if( III ) {
	      if (findChannelInVector(rejectedChannels[dataset], channel)) {
	      //if( std::find(rejectedChannels[dataset].begin(), rejectedChannels[dataset].end(), channel) != rejectedChannels[dataset].end()) {
                    included = false;
		    
                }
            } else {
	      if (findChannelInVector(rejectedChannels[dataset], ch)) {
	      //if( std::find(rejectedChannels[dataset].begin(), rejectedChannels[dataset].end(), ch) != rejectedChannels[dataset].end()) {
                    included = false;
                }
            }

            friendTree->Fill();
        }
        mcTree->SetBranchStatus("*", 1);
        mcTree->AddFriend(friendTree);
        mcTree->Write("", TObject::kOverwrite);
        friendTree->Write();
        mcFile->Close();
        delete mcFile;
    }
}

int main() {
    addRejected();
}
