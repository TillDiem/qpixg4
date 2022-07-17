#include <iostream>
#include <string>
#include <fstream>
#include "TLorentzVector.h"
#include "TVector3.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TCut.h"
#include <TRandom3.h>
#include <vector>
#include "TGraph.h"
#include "TLegend.h"
#include "TMath.h"


std::vector<std::vector<int>> opdet_type;
std::vector<int> opdet_direction;
std::vector<std::vector<double>> opdet_position;
std::vector<double> *photon_final_t = nullptr;
std::vector<double> *photon_final_x = nullptr;  // mm
std::vector<double> *photon_final_y = nullptr;  // mm
std::vector<double> *photon_final_z = nullptr;  // mm
TFile *NEUTInputFile1 =nullptr;

TTree* GetROOTInput(string InputDirectory, string NEUTFile){
    char* NEUTInputFileName1;
   NEUTInputFileName1 = Form("%s/%s.root", InputDirectory.c_str(), NEUTFile.c_str());
   NEUTInputFile1 = new TFile(NEUTInputFileName1, "UPDATE");


  TTree *NEUTInputTree = (TTree*)NEUTInputFile1->Get("photon_tree");
  int event = 0;


  NEUTInputTree->SetBranchAddress("event", &event);
  NEUTInputTree->SetBranchAddress("photon_final_t", &photon_final_t);
  NEUTInputTree->SetBranchAddress("photon_final_x", &photon_final_x);
  NEUTInputTree->SetBranchAddress("photon_final_y", &photon_final_y);
  NEUTInputTree->SetBranchAddress("photon_final_z", &photon_final_z);


  return NEUTInputTree;

}

void SetGraphOption()
{
  //Making plots nicer
  gROOT->ForceStyle(1); gStyle->SetPadTopMargin(0.07); gStyle->SetPadRightMargin(0.05); gStyle->SetPadLeftMargin(0.15); gStyle->SetPadBottomMargin(0.16); gStyle->SetLabelSize(0.06,"xyz"); gStyle->SetTitleSize(0.06,"xyz"); gStyle->SetTitleOffset(0.9,"x"); gStyle->SetTitleOffset(1.1,"y"); gStyle->SetTitleOffset(0.9,"z"); gStyle->SetStatX(0.8); gStyle->SetStatW(0.2); gStyle->SetStatY(0.85); gStyle->SetStatH(0.1); gStyle->SetOptStat(0); gStyle->SetHistLineWidth(3); gStyle->SetPadTickX(1); gStyle->SetPadTickY(1); gStyle->SetPadGridX(kTRUE); gStyle->SetPadGridY(kTRUE);

}


void ReadOpDet(){
  std::cout << "Loading Photon Detector positions..." << std::endl;
  std::ifstream detector_positions_file;
  detector_positions_file.open("opticaldetectors_100x100_6.txt");
  if(detector_positions_file.is_open()) std::cout << "File opened successfully" << std::endl;
  else {std::cout << "File not found." << std::endl; exit(1);}
  while(!detector_positions_file.eof()) {
  	int num_opdet, type_opdet, direction; double x_opdet, y_opdet, z_opdet;
  	if(detector_positions_file >> num_opdet >> x_opdet >> y_opdet >> z_opdet >> type_opdet >> direction) {
  	    std::vector<int> type({num_opdet, type_opdet});
  	    std::vector<double> position({x_opdet, y_opdet, z_opdet});  //cm - center of detector
  	    opdet_type.push_back(type);
  	    opdet_position.push_back(position);
  	    opdet_direction.push_back(direction);
  	}
  	else{ break; }
  }
  detector_positions_file.close();
}
