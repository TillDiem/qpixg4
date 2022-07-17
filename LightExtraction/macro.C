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
#include "macro.h"

using namespace std;

double round_2(double number)
{
  double result = number;
  result *= 100;
  result = (int)result;
  result /= 100;
  return result;
}

void macro(string NEUTFile="nuis_out", string InputDirectory="", string OutputDirectory="" )
{

   SetGraphOption();
  //Defining output path for PNG and output file name for ROOT file
  string PNGOutputFolderName, ROOTOutputFileName;
  ROOTOutputFileName=Form("%s/ROOT/NEUTFile_%s.root", OutputDirectory.c_str(), NEUTFile.c_str());
  PNGOutputFolderName=Form("%s/PNGs/%s", OutputDirectory.c_str(), NEUTFile.c_str());

  ReadOpDet();
  int num_opdet=opdet_position.size();
  cout << num_opdet << endl;

  TTree *InputTree = GetROOTInput(InputDirectory, NEUTFile);
  int NEventsToLoopOver = 1;//NEUTInputTree->GetEntries(); // 100000

  /* //Loop over events in tree */
  double detector_size=100*100;
  vector<vector<double>> detector_position;
  for(int i = 0; i < num_opdet; i++) {
    vector<double> row(1,-999);
    detector_position.push_back(row);
  }

  for (int EventIt=0; EventIt < NEventsToLoopOver; EventIt++)
  {
    InputTree->GetEntry(EventIt);
    cout << photon_final_t->size() << endl;
    for (int PhotonIt=0; PhotonIt<photon_final_t->size(); PhotonIt++)
    {
	    bool flag = false;
	    for(int opdetIt = 0; opdetIt < num_opdet; opdetIt++)
	    {
                    double det_x = opdet_position[opdetIt][0]*100;
		    double det_y = opdet_position[opdetIt][1]*100;
		    double det_z = opdet_position[opdetIt][2]*100;
		    int det_direction = opdet_direction[opdetIt];
		switch (det_direction)  {
		    case 3: // We are on the Z= const planes
			if(abs(photon_final_z->at(PhotonIt)-det_z)>1e-6) break;
			if(photon_final_x->at(PhotonIt) <= det_x+detector_size/2
					&& photon_final_x->at(PhotonIt) >= det_x-detector_size/2
					&& photon_final_y->at(PhotonIt) <= det_y+detector_size/2
					&& photon_final_y->at(PhotonIt) >= det_y-detector_size/2)
			{
			    detector_position.at(opdetIt).push_back(photon_final_t->at(PhotonIt));
			    flag = true;
			}
			break;

		    case 2:
		    	if(abs(photon_final_y->at(PhotonIt)-det_y)>1e-6) break;
			if(photon_final_x->at(PhotonIt) <= det_x+detector_size/2
					&& photon_final_x->at(PhotonIt) >= det_x-detector_size/2
					&& photon_final_z->at(PhotonIt) <= det_z+detector_size/2
					&& photon_final_z->at(PhotonIt) >= det_z-detector_size/2)
			{
			    detector_position.at(opdetIt).push_back(photon_final_t->at(PhotonIt));
			    flag = true;
			}
			break;

		    case 1:
			if(abs(photon_final_x->at(PhotonIt)-det_x)>1e-6) break;
			if(photon_final_y->at(PhotonIt) <= det_y+detector_size/2
					&& photon_final_y->at(PhotonIt) >= det_y-detector_size/2
					&& photon_final_z->at(PhotonIt) <= det_z+detector_size/2
					&& photon_final_z->at(PhotonIt) >= det_z-detector_size/2)
			{
			    detector_position.at(opdetIt).push_back(photon_final_t->at(PhotonIt));
			    flag = true;
			}
			break;

		    default:
			cout << " WARNING - Optical Detector seems to have no valid orientation" << endl;
			break;
		}//End Switch
		if(flag == true) break;
	    } // End Optical Lopo
	    if(flag == false)
	    {
		    cout << "Failed to include Photon at " << photon_final_x->at(PhotonIt) << " " << photon_final_y->at(PhotonIt) << " " << photon_final_z->at(PhotonIt) << " into an Optical Detector." << endl;
	    }
    }//End Photon Loop

  }//End Event loop

int np = 0;
for(int i = 0 ; i<detector_position.size(); i++){
	detector_position.at(i).erase(detector_position.at(i).begin());
	np+=detector_position.at(i).size();
}
assert(np!=photon_final_t->size());
TH1F** array = new TH1F*[detector_position.size()]; for (int i=0;i<detector_position.size();i++) { array[i] = new TH1F(Form("h%d",i),"test",1000,0,1000); }
for(int i = 0; i<detector_position.size(); i++){
	for(int j = 0; j<detector_position.at(i).size(); j++){
		array[i]->Fill(detector_position.at(i).at(j)/10);
	}
}
for(int i = 0; i<detector_position.size(); i++){
  TCanvas *cCanvas_Flux = new TCanvas("cCanvas_Flux","cCanvas_Flux",1000,800);
  array[i]->SetTitle(Form("Time Recorded at (%f %f %f)", opdet_position.at(i).at(0), opdet_position.at(i).at(1), opdet_position.at(i).at(2)));
  array[i]->GetXaxis()->SetTitle("t");
  array[i]->GetXaxis()->SetRangeUser(1,1000);
  cCanvas_Flux->SetLogx();
  cCanvas_Flux->SetLogy();
  array[i]->Draw("HIST");
  cCanvas_Flux->SaveAs(Form("%s/%d.png",PNGOutputFolderName.c_str(),i));
   delete cCanvas_Flux;
}

InputTree->Branch("SiPM_Timing",&detector_position);
InputTree->Fill();
InputTree->Write();
NEUTInputFile1->Close();
}
