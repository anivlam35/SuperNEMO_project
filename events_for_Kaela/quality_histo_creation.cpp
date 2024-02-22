
#include "/sps/nemo/scratch/ikovalen/TKEvent/TKEvent/include/TKEvent.h"

R__LOAD_LIBRARY(/sps/nemo/scratch/ikovalen/TKEvent/TKEvent/lib/libTKEvent.so);

using namespace std;

const double STEP = 0.01;
const int NBINS = int(1 / STEP);

void quality_histo_creation(){
        TFile* f = new TFile("quality_file.root");
	
        TTree* Tree[NBINS];
        int e_number[NBINS];
        double quality[NBINS];

        for(int BIN = 0; BIN < NBINS; BIN++)
        {
                stringstream treename;
                treename << "Events with quality from " << STEP*BIN << " to " << (BIN + 1) * STEP;
		
		Tree[BIN] = (TTree*) f->Get(treename.str().c_str());
		
		Tree[BIN]->SetBranchAddress("e_number", &e_number[BIN]);
		Tree[BIN]->SetBranchAddress("quality", &quality[BIN]);                
        }

        TCanvas *C = new TCanvas("C", "Histogram of tracks quality", 800, 600);

        TH1D *h = new TH1D("h", "Histogram of tracks quality", NBINS, 0, 1);	

	for(int BIN = 0; BIN < NBINS; BIN++)
	{
		for(int Entry=0; Entry < Tree[BIN]->GetEntries(); Entry++)
		{
			Tree[BIN]->GetEntry(Entry);
			h->Fill(quality[BIN]);
		}
	}

	h->Draw();
	
	C->Print("Tracks_quality_histogram.png");

	
	return 0;
}
