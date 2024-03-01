
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

void Projections_processing()
{
	gROOT->SetBatch(kTRUE);

	TFile* file = new TFile("FoilPlane_Tracks.root");

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

		double YMIN  = Y_MIN + (NSOR % N_COLS + 0.5) * 833 - 140;
		double YMAX  = Y_MIN + (NSOR % N_COLS + 0.5) * 833 + 140;
		int    YBINS = (YMAX - YMIN) / 4.0;

		double ZMIN  = Z_MAX - (NSOR / N_COLS + 0.5) * 468 - 140;
		double ZMAX  = Z_MAX - (NSOR / N_COLS + 0.5) * 468 + 140;
		int    ZBINS = (ZMAX - ZMIN) / 4.0;

		TH1* Y_pr;
		TH1* Z_pr;

		stringstream hy_name;

		hy_name << "Y projection " << 0 << " mm x < 0 Source " << NSOR;						   
		Y_pr = new TH1F(hy_name.str().c_str(), hy_name.str().c_str(), YBINS, YMIN, YMAX);
		Y_pr->GetXaxis()->SetTitle("y[mm]");
		Y_pr->GetYaxis()->SetTitle("N");
		
		stringstream hz_name;

                hz_name << "Z projection " << 0 << " mm x < 0 Source " << NSOR;
                Z_pr = new TH1F(hz_name.str().c_str(), hz_name.str().c_str(), ZBINS, ZMIN, ZMAX);
                Z_pr->GetXaxis()->SetTitle("z[mm]");
                Z_pr->GetYaxis()->SetTitle("N");

	

		for(int i = 0; i < tr->GetEntries(); i++)
		{
			tr->GetEntry(i);

			verY = parA * X_BasePlane + parB;
			Y_pr->Fill(verY);
			verZ = parC * X_BasePlane + parD;
			Z_pr->Fill(verZ);

		}
		
		Y_pr->Fit("gaus");
		Z_pr->Fit("gaus");

		TCanvas* CY = new TCanvas("Canvas", "Canvas", 800, 600);
		Y_pr->Draw("COLZ");

		CY->Update();
		TPaveStats *stY = (TPaveStats*)Y_pr->FindObject("stats");
		stY->SetX1NDC(0.75);			
		stY->SetX2NDC(0.95);
                stY->SetY1NDC(0.77);
                stY->SetY2NDC(0.95);

		stringstream hnmY;
		hnmY << "Projections/Y_PR_SRC" << NSOR << ".png" ;
		CY->SaveAs(hnmY.str().c_str());
		
                TCanvas* CZ = new TCanvas("Canvas", "Canvas", 800, 600);
                Z_pr->Draw("COLZ");

                CZ->Update();
                TPaveStats *stZ = (TPaveStats*)Z_pr->FindObject("stats");
                stZ->SetX1NDC(0.75);
                stZ->SetX2NDC(0.95);
                stZ->SetY1NDC(0.77);
                stZ->SetY2NDC(0.95);

                stringstream hnmZ;
                hnmZ << "Projections/Z_PR_SRC" << NSOR << ".png" ;
                CZ->SaveAs(hnmZ.str().c_str());
	}
}
