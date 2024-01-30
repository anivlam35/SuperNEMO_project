#include "/sps/nemo/scratch/ikovalen/TKEvent/TKEvent/include/TKEvent.h" 

R__LOAD_LIBRARY(/sps/nemo/scratch/ikovalen/TKEvent/TKEvent/lib/libTKEvent.so);

using namespace std;

const int X_BasePlane = 0;
const int X_OBSERV_MIN = -20;
const int X_OBSERV_MAX = 20;
const int X_OBSERV = 0;

// const int SRC_NO = 5;

void FoilPlane_Visualization()
{
	gROOT->SetBatch(kTRUE);

	// 0000000000000000000000000000
	// Build the RMS_Y(x) and RMS_Z(x) for selection ellipses from code Analysis

	// TFile* file = new TFile("FoilPlane_Tracks.root");
	TFile* file = new TFile("Clear_data.root");

	int N_Point = X_OBSERV_MAX - X_OBSERV_MIN + 1;
	double Arr_X[N_Point], Arr_Y[N_Point], Arr_Z[N_Point];
	
	TGraph* grY[14];
	TGraph* grZ[14];

	for(int SRC_NO_i=0; SRC_NO_i<14; SRC_NO_i++)
	{
		stringstream tr_name;

		tr_name << "Tracks for " << X_BasePlane << " mm x < 0 Source " << SRC_NO_i;
		TTree* tr = (TTree*) file->Get(tr_name.str().c_str());

		double parA, parB, parC, parD;
		tr->SetBranchAddress("A", &parA);
		tr->SetBranchAddress("B", &parB);
		tr->SetBranchAddress("C", &parC);
		tr->SetBranchAddress("D", &parD);

		double verY, verZ;

		// Fill histogram of undistorted vertices, and calculate coordinates of mode and the sigmas in both directions

		TFile *New_file = new TFile("Visual_Histos.root","RECREATE");

		double YMIN  = tr->GetMinimum("B") - 20.0;
		double YMAX  = tr->GetMaximum("B") + 20.0;
		int    YBINS = int( (YMAX - YMIN + 40.0)/4.0 );

		double ZMIN = tr->GetMinimum("D") - 20.0;
		double ZMAX = tr->GetMaximum("D") + 20.0;
		int    ZBINS = int( (ZMAX - ZMIN + 40.0)/4.0 );

		TH2D* h_vert_real[2*X_OBSERV_MAX+1];  

		for(int iX = X_OBSERV_MIN; iX<X_OBSERV_MAX+1; iX++)
		{
			stringstream h_name;

			h_name << "Hist for " << iX << " mm x < 0 Source " << SRC_NO_i;						   
			h_vert_real[iX + X_OBSERV_MAX] = new TH2D(h_name.str().c_str(), h_name.str().c_str(), YBINS, YMIN, YMAX, ZBINS, ZMIN, ZMAX);
			h_vert_real[iX + X_OBSERV_MAX]->SetMaximum(tr->GetEntries()/30.0);
			h_vert_real[iX + X_OBSERV_MAX]->GetXaxis()->SetRangeUser(YMIN, YMAX);
			h_vert_real[iX + X_OBSERV_MAX]->GetYaxis()->SetRangeUser(ZMIN, ZMAX);
			h_vert_real[iX + X_OBSERV_MAX]->GetXaxis()->SetTitle("y[mm]");
			h_vert_real[iX + X_OBSERV_MAX]->GetYaxis()->SetTitle("z[mm]");
		}

		for(int i = 0; i < tr->GetEntries(); i++)
		{
			tr->GetEntry(i);

			for(int iX = X_OBSERV_MIN; iX<X_OBSERV_MAX+1; iX++)
			{
				verY = parA*iX + parB;
				verZ = parC*iX + parD;
				h_vert_real[iX + X_OBSERV_MAX]->Fill(verY, verZ);
			}
		}

		for(int iX = X_OBSERV_MIN; iX<X_OBSERV_MAX+1; iX++)
		{
			TCanvas* C0 = new TCanvas("Canvas", "Canvas");
			h_vert_real[iX + X_OBSERV_MAX]->Draw("COLZ"); 
			//h_vert_real[iX + X_OBSERV_MAX]->Write(h_name.str().c_str());

			stringstream hnm;
			hnm << "CUTS/SRC" << SRC_NO_i << "X" << iX - X_OBSERV_MIN << "NEG.png" ;
			C0->SaveAs(hnm.str().c_str());
			
			cout << "X = " << iX << "  " << h_vert_real[iX + X_OBSERV_MAX]->ProjectionX()->GetRMS() << "   " << h_vert_real[iX + X_OBSERV_MAX]->ProjectionY()->GetRMS() <<endl;

			Arr_X[iX-X_OBSERV_MIN] = iX;
			Arr_Y[iX-X_OBSERV_MIN] = h_vert_real[iX + X_OBSERV_MAX]->ProjectionX()->GetRMS();
			Arr_Z[iX-X_OBSERV_MIN] = h_vert_real[iX + X_OBSERV_MAX]->ProjectionY()->GetRMS();
		}

		grY[SRC_NO_i] = new TGraph(N_Point, Arr_X, Arr_Y);
		grZ[SRC_NO_i] = new TGraph(N_Point, Arr_X, Arr_Z);
	}
	
	TCanvas* CY = new TCanvas("GraphY", "GraphY", 2000, 1100);

	TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
	pad1->Draw();
	pad1->cd();

	grY[0]->SetMinimum(6.0);
	grY[0]->SetMaximum(12.0);
	grY[0]->SetName("Y0");
	grY[0]->SetLineColor(1);
	grY[0]->SetLineWidth(3);
	grY[0]->Draw();
	cout << "AAAAAAAAAAAAAAAAAA" << endl;
	for(int SRC_NO_i=1; SRC_NO_i<14; SRC_NO_i++)
	{
		char nm[5];
		sprintf(nm,"Y%i", SRC_NO_i);

		grY[SRC_NO_i]->SetName(nm);
		grY[SRC_NO_i]->SetLineColor(SRC_NO_i + 1);
		grY[SRC_NO_i]->SetLineWidth(3);
		grY[SRC_NO_i]->Draw("same");
	}
	
	CY->cd();
        TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
  	pad2->Draw();
	pad2->cd();

	TH1F *hY = new TH1F("histo", "histo", 40, X_OBSERV_MIN, X_OBSERV_MAX);

        for(int SRC_NO_i=0; SRC_NO_i<14; SRC_NO_i++)
        {
		cout << grY[SRC_NO_i]->GetMinimum() << endl;
		// Get the graph's X and Y arrays
        	const double* xValues = grY[SRC_NO_i]->GetX();
        	const double* yValues = grY[SRC_NO_i]->GetY();

	        // Find the index of the minimum Y value
       		double minY = yValues[0];
       		int minIndex = 0;
        	for (int i = 1; i < grY[SRC_NO_i]->GetN(); ++i) 
    		{
            		if (yValues[i] < minY) 
      			{
                  		minY = yValues[i];
                  		minIndex = i;
            		}
        	}

   		 // Get the x-position corresponding to the minimum Y value
		double minXPosition = xValues[minIndex];
    
		hY->Fill(minXPosition);
	}

	hY->Draw();

	CY->Print("vY.png");

	
cout << "bbbbbbbbbbbbbbbbbbbbbbbb" << endl;
	TCanvas* CZ = new TCanvas("GraphZ", "GraphZ", 2000, 1100);
	grZ[0]->SetMinimum(8.0);
	grZ[0]->SetMaximum(12.0);
	grZ[0]->SetName("Z0");
	grZ[0]->SetLineColor(1);
	grZ[0]->SetLineWidth(3);
	grZ[0]->Draw();

   	auto legend = new TLegend(0.9,0.1,1.0,0.9);
   		legend->SetHeader("BLABLA","C"); // option "C" allows to center the header
		legend->AddEntry("Z0","Z0","l");

	for(int SRC_NO_i=1; SRC_NO_i<14; SRC_NO_i++)
	{
		char nm[5];
		sprintf(nm,"Z%i", SRC_NO_i);

		grZ[SRC_NO_i]->SetName(nm);
		grZ[SRC_NO_i]->SetLineColor(SRC_NO_i + 1);
		grZ[SRC_NO_i]->SetLineWidth(3);
		grZ[SRC_NO_i]->Draw("same");
 		  
		legend->AddEntry(nm,nm,"l");
	}	

   	legend->Draw("same");
	CZ->Print("vZ.png");

	for(int SRC_NO_i=0; SRC_NO_i<14; SRC_NO_i++)
	{	
		char print_name_RMS_Y[50];
		sprintf(print_name_RMS_Y,"RMS_ALL_SOURCE/RMS_Y_Source_%i.png", SRC_NO_i);

		TCanvas* CindY = new TCanvas(print_name_RMS_Y, print_name_RMS_Y);
		grY[SRC_NO_i]->Draw();

		CindY->Print(print_name_RMS_Y);

		char print_name_RMS_Z[50];
		sprintf(print_name_RMS_Z,"RMS_ALL_SOURCE/RMS_Z_Source_%i.png", SRC_NO_i);

		TCanvas* CindZ = new TCanvas(print_name_RMS_Z, print_name_RMS_Z);
		grZ[SRC_NO_i]->Draw();

		CindZ->Print(print_name_RMS_Z);
	}

	// New_file->Close();


	// 0000000000000000000000000000000000000000000
	// Build the RMS_Y(x) and RMS_Z(x) for selection ellipses for different parameter a


	// TFile* file = new TFile("EllipsesSave.root");
	// // a = 50
	// for(int a_par_ii = 2; a_par_ii<50; a_par_ii++)
	// {
	// 	double a_par_i = 2.0 + double(a_par_ii-2)*0.5;

	// 	char tr_name[50];
	// 	sprintf(tr_name,"Tracks for %i mm x < 0 Source %i par a = %2.1f", X_BasePlane, SRC_NO, a_par_i);

	// 	TTree* tr = (TTree*) file->Get(tr_name);

	// 	double parA, parB, parC, parD;
	// 	tr->SetBranchAddress("A", &parA);
	// 	tr->SetBranchAddress("B", &parB);
	// 	tr->SetBranchAddress("C", &parC);
	// 	tr->SetBranchAddress("D", &parD);

	// 	double verY, verZ;

	// 	// Fill histogram of undistorted vertices, and calculate coordinates of mode and the sigmas in both directions

	// 	// Parameters for graps
	// 	int N_Point = X_OBSERV_MAX - X_OBSERV_MIN + 1;
	// 	double Arr_X[N_Point], Arr_Y[N_Point], Arr_Z[N_Point], SDevY[N_Point], SDevZ[N_Point], Chi2Y[N_Point], Chi2Z[N_Point];

	// 	for(int X_OBSERV_i = X_OBSERV_MIN; X_OBSERV_i<X_OBSERV_MAX+1; X_OBSERV_i++)
	// 	{
	// 		char h_name[50];
	// 		sprintf(h_name,"Hist for %i mm x < 0 Source %i par a = %2.1f", X_OBSERV_i, SRC_NO, a_par_i);

	// 		TH2D* h_vert_real = new TH2D(h_name, h_name, int(tr->GetMaximum("B") - tr->GetMinimum("B")+200), tr->GetMinimum("B")-100, tr->GetMaximum("B")+100, 
	// 											int(tr->GetMaximum("D") - tr->GetMinimum("D")+200), tr->GetMinimum("D")-100, tr->GetMaximum("D")+100); 						   

	// 		for(int i = 0; i < tr->GetEntries(); i++)
	// 		{
	// 			tr->GetEntry(i);
	// 			verY = parA*X_OBSERV_i + parB;
	// 			verZ = parC*X_OBSERV_i + parD;

	// 			h_vert_real->Fill(verY, verZ);
	// 		}

	// 		TCanvas* C0 = new TCanvas("C0", "C0");
	// 		h_vert_real->GetXaxis()->SetRangeUser(h_vert_real->ProjectionX()->GetMean()-40, h_vert_real->ProjectionX()->GetMean()+40);
	// 		h_vert_real->GetYaxis()->SetRangeUser(h_vert_real->ProjectionY()->GetMean()-40, h_vert_real->ProjectionY()->GetMean()+40);
	// 		h_vert_real->GetXaxis()->SetTitle("y[mm]");
	// 		h_vert_real->GetYaxis()->SetTitle("z[mm]");
	// 		h_vert_real->Draw("COLZ");

			
	// 		// Part for saving plots of set of points after different selection ellipses
	// 		// TFile *New_file = new TFile("Visual_Histos.root","RECREATE");
	// 		// h_vert_real->Write(h_name.str().c_str());
	// 		// New_file->Close();

	// 		// char print_name[70];
	// 		// sprintf(print_name,"EllipsesSave/Hist_for_%i_mm_x<0_Source_%i_par_a=%2.1f.png", X_OBSERV_i, SRC_NO, a_par_i);
	// 		// cout<<print_name<<endl;
	// 		// C0->Print(print_name);

	// 		// Fill parameters for future graphs
	// 		Arr_X[X_OBSERV_i-X_OBSERV_MIN] = X_OBSERV_i;
	// 		Arr_Y[X_OBSERV_i-X_OBSERV_MIN] = h_vert_real->ProjectionX()->GetRMS();
	// 		Arr_Z[X_OBSERV_i-X_OBSERV_MIN] = h_vert_real->ProjectionY()->GetRMS();

	// 		TF1* fitFuncY = new TF1("fitFuncY", "gaus(0)", h_vert_real->ProjectionX()->GetMean()-40, h_vert_real->ProjectionX()->GetMean()+40);
	// 		h_vert_real->ProjectionX()->Fit(fitFuncY, "Q");
	// 		SDevY[X_OBSERV_i-X_OBSERV_MIN] = fitFuncY->GetParameter(2);
	// 		Chi2Y[X_OBSERV_i-X_OBSERV_MIN] = fitFuncY->GetChisquare();

	// 		TF1* fitFuncZ = new TF1("fitFuncZ", "gaus(0)", h_vert_real->ProjectionX()->GetMean()-40, h_vert_real->ProjectionX()->GetMean()+40);
	// 		h_vert_real->ProjectionY()->Fit(fitFuncZ, "Q");
	// 		SDevZ[X_OBSERV_i-X_OBSERV_MIN] = fitFuncZ->GetParameter(2);
	// 		Chi2Z[X_OBSERV_i-X_OBSERV_MIN] = fitFuncZ->GetChisquare();
	// 	}

	// 	// RMS saving into RMS_a folder
	// 	char print_name_RMS_Y[50];
	// 	sprintf(print_name_RMS_Y,"RMS_a/RMS_Y_Source_%i_par_a=%2.1f.png", SRC_NO, a_par_i);
	// 	TCanvas* CY = new TCanvas("GraphY", "GraphY");
	// 	TGraph* grY = new TGraph(N_Point, Arr_X, Arr_Y);
	// 	grY->Draw("AP*");
	// 	CY->Print(print_name_RMS_Y);

	// 	char print_name_RMS_Z[50];
	// 	sprintf(print_name_RMS_Z,"RMS_a/RMS_Z_Source_%i_par_a=%2.1f.png", SRC_NO, a_par_i);
	// 	TCanvas* CZ = new TCanvas("GraphZ", "GraphZ");
	// 	TGraph* grZ = new TGraph(N_Point, Arr_X, Arr_Z);
	// 	grZ->Draw("AP*");
	// 	CZ->Print(print_name_RMS_Z);

	// 	// Sigma saving into Sigma_a folder
	// 	char print_name_Sigma_Y[50];
	// 	sprintf(print_name_Sigma_Y,"Sigma_a/Sigma_Y_Source_%i_par_a=%2.1f.png", SRC_NO, a_par_i);
	// 	TCanvas* CSigmaY = new TCanvas("SigmaY", "SigmaY");
	// 	TGraph* grSigmaY = new TGraph(N_Point, Arr_X, SDevY);
	// 	grSigmaY->Draw("AP*");
	// 	CSigmaY->Print(print_name_Sigma_Y);

	// 	char print_name_Sigma_Z[50];
	// 	sprintf(print_name_Sigma_Z,"Sigma_a/Sigma_Z_Source_%i_par_a=%2.1f.png", SRC_NO, a_par_i);
	// 	TCanvas* CSigmaZ = new TCanvas("SigmaZ", "SigmaZ");
	// 	TGraph* grSigmaZ = new TGraph(N_Point, Arr_X, SDevZ);
	// 	grSigmaZ->Draw("AP*");
	// 	CSigmaZ->Print(print_name_Sigma_Z);

	// 	// Chi2 saving into Chi2_a folder
	// 	char print_name_C_Y[50];
	// 	sprintf(print_name_Chi2_Y,"Chi2_a/Chi2_Y_Source_%i_par_a=%2.1f.png", SRC_NO, a_par_i);
	// 	TCanvas* CChi2Y = new TCanvas("Chi2Y", "Chi2Y");
	// 	TGraph* grChi2Y = new TGraph(N_Point, Arr_X, Chi2Y);
	// 	grChi2Y->Draw("AP*");
	// 	CChi2Y->Print(print_name_Chi2_Y);

	// 	char print_name_Chi2_Z[50];
	// 	sprintf(print_name_Chi2_Z,"Chi2_a/Chi2_Z_Source_%i_par_a=%2.1f.png", SRC_NO, a_par_i);
	// 	TCanvas* CChi2Z = new TCanvas("Chi2Z", "Chi2Z");
	// 	TGraph* grChi2Z = new TGraph(N_Point, Arr_X, Chi2Z);
	// 	grChi2Z->Draw("AP*");
	// 	CChi2Z->Print(print_name_Chi2_Z);


	// 	cout<<"Done for "<<a_par_i<<endl;
	// }

	// 0000000000000000000000000000000000000000000
	// Just save the plots of different selection elipses for given X plane (X_OBSERV parameter)

	// TFile* file = new TFile("EllipsesSave.root");

	// for(int a_par_ii = 2; a_par_ii<50; a_par_ii++)
	// {
	// 	double a_par_i = 2.0 + double(a_par_ii-2)*0.5;

	// 	char tr_name[50];
	// 	sprintf(tr_name,"Tracks for %i mm x < 0 Source %i par a = %2.1f", X_BasePlane, SRC_NO, a_par_i);

	// 	TTree* tr = (TTree*) file->Get(tr_name);

	// 	double parA, parB, parC, parD;
	// 	tr->SetBranchAddress("A", &parA);
	// 	tr->SetBranchAddress("B", &parB);
	// 	tr->SetBranchAddress("C", &parC);
	// 	tr->SetBranchAddress("D", &parD);

	// 	double verY, verZ;

	// 	// Fill histogram of undistorted vertices, and calculate coordinates of mode and the sigmas in both directions

	// 	char h_name[50];
	// 	sprintf(h_name,"Hist for %i mm x < 0 Source %i par a = %2.1f", X_OBSERV, SRC_NO, a_par_i);

	// 	TH2D* h_vert_real = new TH2D(h_name, h_name, int(tr->GetMaximum("B") - tr->GetMinimum("B")+200), tr->GetMinimum("B")-100, tr->GetMaximum("B")+100, 
	// 										int(tr->GetMaximum("D") - tr->GetMinimum("D")+200), tr->GetMinimum("D")-100, tr->GetMaximum("D")+100); 						   

	// 	for(int i = 0; i < tr->GetEntries(); i++)
	// 	{
	// 		tr->GetEntry(i);
	// 		verY = parA*X_OBSERV + parB;
	// 		verZ = parC*X_OBSERV + parD;

	// 		h_vert_real->Fill(verY, verZ);
	// 	}

	// 	TCanvas* C0 = new TCanvas("C0", "C0");
	// 	h_vert_real->GetXaxis()->SetRangeUser(h_vert_real->ProjectionX()->GetMean()-40, h_vert_real->ProjectionX()->GetMean()+40);
	// 	h_vert_real->GetYaxis()->SetRangeUser(h_vert_real->ProjectionY()->GetMean()-40, h_vert_real->ProjectionY()->GetMean()+40);
	// 	h_vert_real->GetXaxis()->SetTitle("y[mm]");
	// 	h_vert_real->GetYaxis()->SetTitle("z[mm]");
	// 	h_vert_real->Draw("COLZ"); 

	// 	// TFile *New_file = new TFile("Visual_Histos.root","RECREATE");
	// 	// h_vert_real->Write(h_name.str().c_str());
	// 	// New_file->Close();

	// 	char print_name[50];
	// 	sprintf(print_name,"EllipsesSave/Hist_for_%i_mm_x<0_Source_%i_par_a=%2.1f.png", X_OBSERV, SRC_NO, a_par_i);
	// 	cout<<print_name<<endl;
	// 	C0->Print(print_name);
	// }
	

	// for browsing
	// TBrowser* tb = new TBrowser();
}


