#include "/sps/nemo/scratch/ikovalen/TKEvent/TKEvent/include/TKEvent.h" 
#include "config.h"

R__LOAD_LIBRARY(/sps/nemo/scratch/ikovalen/TKEvent_old/TKEvent/lib/libTKEvent.so);

using namespace std;

void Projections_Processing()
{
	gROOT->SetBatch(kTRUE);

	TFile* file = new TFile(Form("Tracks_Run-%d.root", RUN_N));

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

		double YMIN  = Y_MIN + (NSOR % N_COLS + 0.5) * Y_RECT_SIZE - 140;
		double YMAX  = Y_MIN + (NSOR % N_COLS + 0.5) * Y_RECT_SIZE + 140;
		int    YBINS = (YMAX - YMIN) / 3;

		double ZMIN  = Z_MAX - (NSOR / N_COLS + 0.5) * Z_RECT_SIZE - 140;
		double ZMAX  = Z_MAX - (NSOR / N_COLS + 0.5) * Z_RECT_SIZE + 140;
		int    ZBINS = (ZMAX - ZMIN) / 3;

		TH1* Y_pr;
		TH1* Z_pr;

		stringstream hy_name;

		hy_name << "Y projection " << 0 << " mm x < 0 Source " << NSOR;						   
		Y_pr = new TH1F(hy_name.str().c_str(), hy_name.str().c_str(), YBINS, YMIN, YMAX);
		Y_pr->GetXaxis()->SetTitle("y[mm]");
		Y_pr->GetYaxis()->SetTitle("N");
		Y_pr->SetMarkerStyle(106);		

		stringstream hz_name;

                hz_name << "Z projection " << 0 << " mm x < 0 Source " << NSOR;
                Z_pr = new TH1F(hz_name.str().c_str(), hz_name.str().c_str(), ZBINS, ZMIN, ZMAX);
                Z_pr->GetXaxis()->SetTitle("z[mm]");
                Z_pr->GetYaxis()->SetTitle("N");
		Z_pr->SetMarkerStyle(106);
	

		for(int i = 0; i < tr->GetEntries(); i++)
		{
			tr->GetEntry(i);

			verY = parA * X_BasePlane + parB;
			Y_pr->Fill(verY);
			verZ = parC * X_BasePlane + parD;
			Z_pr->Fill(verZ);

		}
		
		Y_pr->Fit("gaus(0)*gaus(3)");
		Z_pr->Fit("gaus(0)*gaus(3)");
		
		Y_pr->GetFunction("gaus")->SetNpx(1000);

		TCanvas* CY = new TCanvas("Canvas", "Canvas", 800, 600);
		Y_pr->Draw();

		CY->Update();
		TPaveStats *stY = (TPaveStats*)Y_pr->FindObject("stats");
		stY->SetX1NDC(0.75);			
		stY->SetX2NDC(0.95);
                stY->SetY1NDC(0.77);
                stY->SetY2NDC(0.95);

		CY->SaveAs(Form("Projections/Y_PR_SRC%02d.png", NSOR));
		
                TCanvas* CZ = new TCanvas("Canvas", "Canvas", 800, 600);
                Z_pr->Draw();

                CZ->Update();
                TPaveStats *stZ = (TPaveStats*)Z_pr->FindObject("stats");
                stZ->SetX1NDC(0.75);
                stZ->SetX2NDC(0.95);
                stZ->SetY1NDC(0.77);
                stZ->SetY2NDC(0.95);

                CZ->SaveAs(Form("Projections/Z_PR_SRC%02d.png", NSOR));
	}
}
