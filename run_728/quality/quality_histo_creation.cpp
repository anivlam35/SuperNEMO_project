
#include "/sps/nemo/scratch/ikovalen/TKEvent/TKEvent/include/TKEvent.h"

R__LOAD_LIBRARY(/sps/nemo/scratch/ikovalen/TKEvent/TKEvent/lib/libTKEvent.so);

using namespace std;

const double STEP = 0.01;
const int NBINS = int(1 / STEP);

void quality_histo_creation(){
        TFile* f = new TFile("quality_file.root");
	int N_q = 0;
	int N_r_q = 0;
	int N_z_q = 0;
        TTree* Tree[NBINS];
        TTree* Tree_r[NBINS];
        TTree* Tree_z[NBINS];
        int e_number[NBINS];
        int e_number_r[NBINS];
        int e_number_z[NBINS];
        double quality[NBINS];
        double r_quality[NBINS];
        double z_quality[NBINS];

        TCanvas *C = new TCanvas("C", "Histogram of tracks quality", 1200, 1000);

        for(int BIN = 0; BIN < NBINS; BIN++)
        {
                stringstream treename;
                treename << "Events with quality from " << STEP*BIN << " to " << (BIN + 1) * STEP;
		
		Tree[BIN] = (TTree*) f->Get(treename.str().c_str());
		
		Tree[BIN]->SetBranchAddress("e_number", &e_number[BIN]);
		Tree[BIN]->SetBranchAddress("quality", &quality[BIN]);                
        }

        TH1D *h = new TH1D("h", "Histogram of tracks quality", NBINS, 0, 1);	

	for(int BIN = 0; BIN < NBINS; BIN++)
	{
		for(int Entry=0; Entry < Tree[BIN]->GetEntries(); Entry++)
		{
			Tree[BIN]->GetEntry(Entry);
			h->Fill(quality[BIN]);
			N_q++;
		}
	}
	
	h->GetYaxis()->SetRangeUser(0, 100000);
	h->SetStats(0);
	h->GetYaxis()->SetTitle("N");
	h->GetXaxis()->SetTitle("quality");
	h->Draw();
	
	C->Print("Tracks_quality_histogram.png");
	cout << "N_q = " << N_q << endl;
	cout << "N_r_q = " << N_r_q << endl;
	cout << "N_z_q = " << N_z_q << endl;
	
	return 0;
}
