#include "/sps/nemo/scratch/ikovalen/TKEvent_old/TKEvent/include/TKEvent.h"

R__LOAD_LIBRARY(/sps/nemo/scratch/ikovalen/TKEvent_old/TKEvent/lib/libTKEvent.so);


// Define the structure to hold indexes
struct IndexBin {
    vector<int> indexes;
};

void Analysis_v2(){

        TFile* file = new TFile("./V_code/FoilPlane_Tracks.root");

        stringstream tr_name;

        int SRC_NO_i = 5;

        tr_name << "Tracks for " << 0.0 << " mm x < 0 Source " << SRC_NO_i;
        TTree* tr = (TTree*) file->Get(tr_name.str().c_str());

	tr->Print();

        double _iX = 10.0; // mm

        double parA, parB, parC, parD;
        tr->SetBranchAddress("A", &parA);
        tr->SetBranchAddress("B", &parB);
        tr->SetBranchAddress("C", &parC);
        tr->SetBranchAddress("D", &parD);

	int HIST_K = 4;
        int y_min, y_max, z_min, z_max, y_n, z_n;


	double ZOOM = 1 / 3;
        y_min = int(tr->GetMinimum("B") + (tr->GetMaximum("B") - tr->GetMinimum("B")) * ZOOM);
        y_max = int(tr->GetMaximum("B") - (tr->GetMaximum("B") - tr->GetMinimum("B")) * ZOOM);

        z_min = int(tr->GetMinimum("D") + (tr->GetMaximum("D") - tr->GetMinimum("D")) * ZOOM);
        z_max = int(tr->GetMaximum("D") - (tr->GetMaximum("D") - tr->GetMinimum("D")) * ZOOM);

        y_n = y_max - y_min;
        z_n = z_max - z_min;

	TCanvas* C = new TCanvas("EXAMPLE", "GraphY", 1200, 800);

	TH2D* histo_before_cutoff = new TH2D("EXAMPLE", "EXAMPLE", y_n, y_min, y_max, z_n, z_min, z_max);
        vector<vector<IndexBin>> histogram(y_n, vector<IndexBin>(z_n));

        // Loop through each entry and fill the histogram
        for (int i = 0; i < tr->GetEntries(); i++) {
                tr->GetEntry(i);
                double verY = parA * _iX + parB;
                double verZ = parC * _iX + parD;

                // Calculate the bin indices for Y and Z
                int binY = int(verY) - y_min;
                int binZ = int(verZ) - z_min;

                // Check if the bin indices are within the range
                if (binY >= 0 && binY < y_n && binZ >= 0 && binZ < z_n) {
                        // Add the index to the appropriate bin
                        histogram[binY][binZ].indexes.push_back(i);
                }

		histo_before_cutoff->Fill(verY, verZ);
        }
	
	histo_before_cutoff->SetStats(0);
	histo_before_cutoff ->Draw("COLZ");
	C->Print("example_before_cutoff.png");

	// Now histogram contains the indexes of entries falling into each bin
        // You can access them like histogram[binY][binZ].indexes

        // Calculate the average number of indexes in each bin
        double max_density = 0;
        for (int i = 0; i < y_n; i++) {
                for (int j = 0; j < z_n; j++) {
                        double density_ij = histogram[i][j].indexes.size();
                	if( density_ij > max_density) max_density = density_ij;
		}
        }

	double density_threshold = max_density / 100;
        for (int i = 0; i < y_n; i++) {
                for (int j = 0; j < z_n; j++) {
                        double density_ij = histogram[i][j].indexes.size();
                        if( density_ij < density_threshold) histogram[i][j].indexes.clear();
                }
        }




        TCanvas* C2 = new TCanvas("EXAMPLE AFTER CUTOFF", "GraphY", 1200, 800);

        TH2D* histo_after_cutoff = new TH2D("EXAMPLE", "EXAMPLE", y_n, y_min, y_max, z_n, z_min, z_max);

        // Loop through each entry and fill the histogram
        for (int i = 0; i < y_n; i++) {
		for(int j = 0; j < z_n; j++)
		{
			for(int k = 0; k < histogram[i][j].indexes.size(); k++)
			{
				int entry = histogram[i][j].indexes[k];
                		tr->GetEntry(i);
                		
				double verY = parA * _iX + parB;
               			double verZ = parC * _iX + parD;

                		histo_after_cutoff->Fill(verY, verZ);
			}
		}
        }

	histo_after_cutoff->SetStats(0);
	histo_after_cutoff->Draw("COLZ");
        C2->Print("example_after_cutoff.png");



}

