
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

	TPad *pad1 = new TPad("pad1", "pad1", 0, 0.66, 1, 1.0);
	pad1->Draw();
	pad1->cd();

	pad1->SetBottomMargin(0);

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
	h->Draw();
	
	C->cd();

        TPad *pad_r = new TPad("pad_r", "pad_r", 0, 0.33, 1, 0.66);
        pad_r->Draw();
        pad_r->cd();

        pad_r->SetTopMargin(0);
        pad_r->SetBottomMargin(0);

        for(int BIN = 0; BIN < NBINS; BIN++)
        {
                stringstream treename;
                treename << "Events with r_quality from " << STEP*BIN << " to " << (BIN + 1) * STEP;
				
		Tree_r[BIN] = (TTree*) f->Get(treename.str().c_str());
		
		Tree_r[BIN]->SetBranchAddress("e_number_r", &e_number_r[BIN]);
		Tree_r[BIN]->SetBranchAddress("r_quality", &r_quality[BIN]);                
        }

        TH1D *h_r = new TH1D("h_r", "Histogram of tracks r_quality", NBINS, 0, 1);	

	for(int BIN = 0; BIN < NBINS; BIN++)
	{
		for(int Entry=0; Entry < Tree_r[BIN]->GetEntries(); Entry++)
		{
			Tree_r[BIN]->GetEntry(Entry);
			h_r->Fill(r_quality[BIN]);
			N_r_q++;
		}
	}

	h_r->GetYaxis()->SetRangeUser(0, 100000);
	h_r->SetTitle("");	
	h_r->SetStats(0);
	h_r->GetYaxis()->SetTitle("N");
	h_r->Draw();

	C->cd();

        TPad *pad_z = new TPad("pad_z", "pad_z", 0, 0.0, 1, 0.33);
        pad_z->Draw();
        pad_z->cd();
        pad_z->SetTopMargin(0);

        for(int BIN = 0; BIN < NBINS; BIN++)
        {
                stringstream treename;
                treename << "Events with z_quality from " << STEP*BIN << " to " << (BIN + 1) * STEP;
		
		Tree_z[BIN] = (TTree*) f->Get(treename.str().c_str());
		
		Tree_z[BIN]->SetBranchAddress("e_number_z", &e_number_z[BIN]);
		Tree_z[BIN]->SetBranchAddress("z_quality", &z_quality[BIN]);                
        }

        TH1D *h_z = new TH1D("h_z", "Histogram of tracks z_quality", NBINS, 0, 1);	

	for(int BIN = 0; BIN < NBINS; BIN++)
	{
		for(int Entry=0; Entry < Tree_z[BIN]->GetEntries(); Entry++)
		{
			Tree_z[BIN]->GetEntry(Entry);
			h_z->Fill(z_quality[BIN]);
			N_z_q++;
		}
	}

	h_z->GetYaxis()->SetRangeUser(0, 100000);
	h_z->SetTitle("");		
	h_z->SetStats(0);
	h_z->GetYaxis()->SetTitle("N");
	h_z->GetXaxis()->SetTitle("quality");
	h_z->Draw();

	
	C->Print("Tracks_quality_histogram.png");
	cout << "N_q = " << N_q << endl;
	cout << "N_r_q = " << N_r_q << endl;
	cout << "N_z_q = " << N_z_q << endl;
	
	return 0;
}
