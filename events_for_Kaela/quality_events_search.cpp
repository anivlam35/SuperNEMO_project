#include "/sps/nemo/scratch/ikovalen/TKEvent/TKEvent/include/TKEvent.h"

R__LOAD_LIBRARY(/sps/nemo/scratch/ikovalen/TKEvent/TKEvent/lib/libTKEvent.so);

using namespace std;

const double STEP = 0.01;
const int NBINS = int(1 / STEP);

void quality_events_search()
{
	TFile* f = new TFile("/sps/nemo/scratch/ikovalen/TKEvent/runs/Run-988.root");
	TTree* s = (TTree*) f->Get("Event");

	TKEvent* event = new TKEvent(-1,-1);
	s->SetBranchAddress("Eventdata", &event);
	
        TFile *quality_file = new TFile("quality_file.root", "RECREATE");
	
	TTree* Tree[NBINS];
	int e_number[NBINS];
	double quality[NBINS];

	for(int BIN = 0; BIN < NBINS; BIN++)
	{
		stringstream treename;
		treename << "Events with quality from " << STEP*BIN << " to " << (BIN + 1) * STEP;
		Tree[BIN] = new TTree(treename.str().c_str(), treename.str().c_str());

		TBranch* branch1 = Tree[BIN]->Branch("e_number", &e_number[BIN], "event_number/I");
		TBranch* branch2 = Tree[BIN]->Branch("quality", &quality[BIN], "quality/D");
	}

	//TCanvas *C = new TCanvas("C", "Histogram of tracks quality", 800, 600);	

	//TH1D *h = new TH1D("h", "Histogram of tracks quality", NBINS, 0, 1);
	for(int event_number = 0; event_number < s->GetEntries(); event_number++)
	//for(int event_number = 0; event_number < 100000; event_number++)
	{	
		s->GetEntry(event_number);
		event->set_r("Manchester", "distance");
		event->set_h();
		event->reconstruct_ML(0);
		
		if (event->get_no_tracks() == 1)
		{	
			double q = event->get_tracks()[0]->get_quality();
			int bin = int(q / STEP);

			e_number[bin] = event_number;
			quality[bin] = q;
			Tree[bin]->Fill();
			// h->Fill(quality);
		}
		if (event_number % 10000 == 0) cout << "Event " << event_number << "/" << s->GetEntries() << " is DONE!" << endl; 
	}
	
	//h->Draw();
	//C->Print("Tracks_quality_histogram.png");
		
	for(int BIN = 0; BIN < NBINS; BIN++)
	{
        	stringstream treename;
               	treename << "Events with quality from " << STEP*BIN << " to " << (BIN + 1) * STEP;
		
		Tree[BIN]->Write(treename.str().c_str());	
	}

	quality_file->Close();

}
