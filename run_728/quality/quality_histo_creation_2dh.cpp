
#include "/sps/nemo/scratch/ikovalen/TKEvent/TKEvent/include/TKEvent.h"

R__LOAD_LIBRARY(/sps/nemo/scratch/ikovalen/TKEvent/TKEvent/lib/libTKEvent.so);

using namespace std;

const int   RUN_N = 988;
const double STEP = 0.01;
const int   NBINS = int(1 / STEP);

void quality_histo_creation_2dh(){
        TFile* f = new TFile("quality_file_2dh.root");

        TTree* Tree;
        int e_number;
        double quality;
        double r_quality;
        double z_quality;

        TCanvas *C = new TCanvas("C", "Histogram of tracks quality", 1200, 1000);

		
	Tree = (TTree*) f->Get("Events with quality");
		
	Tree->SetBranchAddress("e_number", &e_number);
	Tree->SetBranchAddress("quality", &quality);                
	Tree->SetBranchAddress("r_quality", &r_quality);                
	Tree->SetBranchAddress("z_quality", &z_quality);                

        TH2D *h = new TH2D("h", "Histogram of tracks quality", NBINS, 0, 1, NBINS, 0, 1);

        for(int Entry = 0; Entry < Tree->GetEntries(); Entry++)
        {
		Tree->GetEntry(Entry);
		h->Fill(r_quality, z_quality);
        }
	
	h->SetMaximum(350);
	h->SetTitle("Correlation of r- and z- quality of track");
	h->GetXaxis()->SetTitle("r_quality");
	h->GetYaxis()->SetTitle("z_quality");
	h->SetStats(0);
	h->Draw("COLZ");

	C->SaveAs(Form("Correl_qual_Run-%d.png", RUN_N));
	
	return 0;
}
