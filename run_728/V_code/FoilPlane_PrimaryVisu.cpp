
#include "/sps/nemo/scratch/ikovalen/TKEvent_old/TKEvent/include/TKEvent.h" 

R__LOAD_LIBRARY(/sps/nemo/scratch/ikovalen/TKEvent_old/TKEvent/lib/libTKEvent.so);

using namespace std;

const double 	 Y_MIN = -2500.0; //-2140.0; 
const double     Y_MAX = -834.0;  //-2020.0;
const int 	Y_BINS = 833;     //12;
const double 	 Z_MIN = -1638.0; //-100.0; 
const double 	 Z_MAX = 1638.0;  //100.0;
const int	Z_BINS = 468;     //20;
const int  X_BasePlane = 0;
const int X_OBSERV_MIN = -20;
const int X_OBSERV_MAX = 20;
const int     X_OBSERV = 0;
const int     N_SRCPLN = 14;
const int	N_ROWS = 7;
const int 	N_COLS = 2;

void FoilPlane_PrimaryVisu()
{
	gROOT->SetBatch(kTRUE);

	// 0000000000000000000000000000
	// Build the RMS_Y(x) and RMS_Z(x) for selection ellipses from code Analysis

	TFile* file = new TFile("FoilPlane_Tracks.root");
	// TFile* file = new TFile("Clear_data.root");

	int N_Point = X_OBSERV_MAX - X_OBSERV_MIN + 1;
	double Arr_X[N_Point], Arr_Y[N_Point], Arr_Z[N_Point];
	
	TGraph* grY[N_SRCPLN];
	TGraph* grZ[N_SRCPLN];

	for(int NSOR=0; NSOR<N_SRCPLN; NSOR++)
	{
		stringstream tr_name;

		tr_name << "Tracks for " << X_BasePlane << " mm x < 0 Source " << NSOR;
		TTree* tr = (TTree*) file->Get(tr_name.str().c_str());

		double parA, parB, parC, parD;
		tr->SetBranchAddress("A", &parA);
		tr->SetBranchAddress("B", &parB);
		tr->SetBranchAddress("C", &parC);
		tr->SetBranchAddress("D", &parD);

		double verY, verZ;

		double YMIN  = Y_MIN + (NSOR % N_COLS + 0.5) * 833 - 120;
		double YMAX  = Y_MIN + (NSOR % N_COLS + 0.5) * 833 + 120;
		int    YBINS = (YMAX - YMIN) / 4.0;

		double ZMIN  = Z_MAX - (NSOR / N_COLS + 0.5) * 468 - 120;
		double ZMAX  = Z_MAX - (NSOR / N_COLS + 0.5) * 468 + 120;
		int    ZBINS = (ZMAX - ZMIN) / 4.0;

		TH2D* h_vert_real[2*X_OBSERV_MAX+1];  

		for(int iX = X_OBSERV_MIN; iX<X_OBSERV_MAX+1; iX+=10)
		{
			stringstream h_name;

			h_name << "Hist for " << iX << " mm x < 0 Source " << NSOR;						   
			h_vert_real[iX + X_OBSERV_MAX] = new TH2D(h_name.str().c_str(), h_name.str().c_str(), YBINS, YMIN, YMAX, ZBINS, ZMIN, ZMAX);
			h_vert_real[iX + X_OBSERV_MAX]->SetMaximum(600);
			h_vert_real[iX + X_OBSERV_MAX]->GetXaxis()->SetRangeUser(YMIN, YMAX);
			h_vert_real[iX + X_OBSERV_MAX]->GetYaxis()->SetRangeUser(ZMIN, ZMAX);
			h_vert_real[iX + X_OBSERV_MAX]->GetXaxis()->SetTitle("y[mm]");
			h_vert_real[iX + X_OBSERV_MAX]->GetYaxis()->SetTitle("z[mm]");
		}

		for(int i = 0; i < tr->GetEntries(); i++)
		{
			tr->GetEntry(i);

			for(int iX = X_OBSERV_MIN; iX < X_OBSERV_MAX+1; iX+=10)
			{
				verY = parA*iX + parB;
				verZ = parC*iX + parD;
				h_vert_real[iX + X_OBSERV_MAX]->Fill(verY, verZ);
			}
		}

		for(int iX = X_OBSERV_MIN; iX < X_OBSERV_MAX+1; iX+=10)
		{
			TCanvas* C0 = new TCanvas("Canvas", "Canvas");
			h_vert_real[iX + X_OBSERV_MAX]->Draw("COLZ"); 
			//h_vert_real[iX + X_OBSERV_MAX]->Write(h_name.str().c_str());

			stringstream hnm;
			hnm << "HISTOS/SRC" << NSOR << "X" << iX - X_OBSERV_MIN << "NEG.png" ;
			C0->SaveAs(hnm.str().c_str());
			
			cout << "X = " << iX << "  " << h_vert_real[iX + X_OBSERV_MAX]->ProjectionX()->GetRMS() << "   " << h_vert_real[iX + X_OBSERV_MAX]->ProjectionY()->GetRMS() <<endl;
		}
	}
}


