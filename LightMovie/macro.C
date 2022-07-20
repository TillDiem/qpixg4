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
#include "TGraph.h"
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

void macro(string NEUTFile="nuis_out", string InputDirectory="", string OutputDirectory="" , string DetFile="", string DetSize="")
{

   SetGraphOption();
  //Defining output path for PNG and output file name for ROOT file
  string PNGOutputFolderName, ROOTOutputFileName;
  ROOTOutputFileName=Form("%s/ROOT/NEUTFile_%s.root", OutputDirectory.c_str(), NEUTFile.c_str());
  PNGOutputFolderName=Form("%s/PNGs/%s", OutputDirectory.c_str(), NEUTFile.c_str());

  TTree *InputTree = GetROOTInput(InputDirectory, NEUTFile);
  TTree *HitTree =  GetHitTree(InputDirectory, NEUTFile);
  int NEventsToLoopOver = 1;//InputTree->GetEntries(); // 100000

  double ntime_steps = 100;
  double dT = 1;
  double time_max = dT * ntime_steps;
  int bins = 200;
  TH1D *h_time = new TH1D("h_time", "h_time", 1000, 0, time_max);
 // TH2D *h_xy = new TH2D("h_xy", "h_xy", 100, 0, 200, 100, 0, 200);
   TH2D** array = new TH2D*[ntime_steps]; for (int i=0;i<ntime_steps;i++) { array[i] = new TH2D(Form("h%d",i),"test",bins,0,200,bins,0,200);}

  /* //Loop over events in tree */
  for (int EventIt=0; EventIt < NEventsToLoopOver; EventIt++)
  {
    InputTree->GetEntry(EventIt);
    HitTree->GetEntry(EventIt);
    cout << hit_end_x->size() << endl;
     //cout << photon_final_t->size() << endl;
    vector<vector<double>> x_pos;
    vector<vector<double>> y_pos;
    for(int i = 0; i < ntime_steps; i++){
	x_pos.push_back(vector<double>(1,-1));
	y_pos.push_back(vector<double>(1,-1));
    }
    for (int PhotonIt=0; PhotonIt<photon_final_t->size(); PhotonIt++)
    {
	double z = photon_final_z->at(PhotonIt);
	double t = photon_final_t->at(PhotonIt);
	if(z==0){
		int time_step = (int)(t/dT);
		if(time_step>=ntime_steps || time_step < 0) continue;
		double x = photon_final_x->at(PhotonIt);
		double y = photon_final_y->at(PhotonIt);
		array[time_step]->Fill(x,y);
		x_pos[time_step].push_back(x);
		y_pos[time_step].push_back(y);
		h_time->Fill(t);
	}
   for(int i = 0; i < hit_end_x->size(); i++)
    {
	    int j = hit_end_t->at(i)/dT;
	    if(j > ntime_steps || j < 0) j = ntime_steps-1;
	    int bin = array[j]->FindBin(hit_end_x->at(i), hit_end_y->at(i));
	    array[j]->SetBinContent(bin,999);
    }

    }//End Photon Loop
    cout << " ENDED PHOTON LOOP" << endl;
    for(int i =0; i<ntime_steps; i++){
	    x_pos[i].erase(x_pos[i].begin());
	    y_pos[i].erase(y_pos[i].begin());
    }
    for(int i =0; i<ntime_steps; i++){
	 /* int n = x_pos[i].size(); */
	 /* Double_t x[n]; */
	 /* Double_t y[n]; */
	 /* for(int j = 0; j<n; j++){ */
		/* x[j] = x_pos[i][j]; */
		/* y[j] = y_pos[i][j]; */
	 /* } */
  	/* TGraph *g1 = new TGraph(n, x, y); */
	/* TCanvas *c1 = new TCanvas("","", 1000,800); */
	/* g1->Draw("AP*"); */
	/* c1->SaveAs(Form("%s/%d.png", PNGOutputFolderName.c_str(), i)); */
	/* delete c1; */
	double x_mean = 0;
	double tot = 0;
	double y_mean = 0;
	for(int binx = 0; binx<bins; binx++){
		for(int biny = 0; biny<bins; biny++){
		  double xy = array[i]->GetBinContent(binx,biny);
		  double xc = array[i]->GetXaxis()->GetBinCenter(binx);
		  double yc = array[i]->GetYaxis()->GetBinCenter(biny);
		  tot = tot + xy;
		  x_mean = x_mean + xy*xc;
		  y_mean = y_mean + xy*yc;
		}
	}
	TCanvas *c1 = new TCanvas("","", 700,700);
	array[i]->SetTitle(Form("Time: %d - %d ns", i, i+1));
	array[i]->Draw("colz");
	if(i < 6){
	array[i]->GetZaxis()->SetRangeUser(0,1000);
	gStyle->SetPalette(56);
	}
	else{
		array[i]->GetZaxis()->SetRangeUser(0,10);
		gStyle->SetPalette(90);
		int c = array[i]->GetBin(x_mean/tot,y_mean/tot);

		array[i]->SetBinContent(c,10);
		array[i]->SetBinContent(c+1,10);
		array[i]->SetBinContent(c+2,10);
		array[i]->SetBinContent(c+3,10);
		array[i]->SetBinContent(c+4,10);
		array[i]->SetBinContent(c+(bins+2),10);
		array[i]->SetBinContent(c+(bins+2)+1,10);
		array[i]->SetBinContent(c+(bins+2)+2,10);
		array[i]->SetBinContent(c+(bins+2)+3,10);
		array[i]->SetBinContent(c+(bins+2)+4,10);
		array[i]->SetBinContent(c-(bins+2),10);
		array[i]->SetBinContent(c-(bins+2)+1,10);
		array[i]->SetBinContent(c-(bins+2)+2,10);
		array[i]->SetBinContent(c-(bins+2)+3,10);
		array[i]->SetBinContent(c-(bins+2)+4,10);


	}
	string ss;
	if(i < 10){ ss = "00" + to_string(i); }
	else if( i < 100){ ss = "0" + to_string(i); }
	else{ ss = to_string(i); }
	c1->SaveAs(Form("%s/%s.png", PNGOutputFolderName.c_str(), ss.c_str()));
	delete c1;


    }
    x_pos.clear();
    y_pos.clear();
    TCanvas *c1 = new TCanvas("","", 1000,800);
    h_time->Draw("HIST");
    h_time->GetYaxis()->SetRangeUser(10,3*pow(10,4));
    c1->SetLogy();
    //c1->SetLogx();
    c1->SaveAs(Form("%s/time.png", PNGOutputFolderName.c_str()));
    delete c1;

  }//End Event loop

//InputTree->Branch("SiPM_Timing",&detector_position);
//InputTree->Fill();
//InputTree->Write();
NEUTInputFile1->Close();
}
