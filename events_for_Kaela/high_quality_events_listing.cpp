#include "/sps/nemo/scratch/ikovalen/TKEvent/TKEvent/include/TKEvent.h"

R__LOAD_LIBRARY(/sps/nemo/scratch/ikovalen/TKEvent/TKEvent/lib/libTKEvent.so);

using namespace std;

const double STEP = 0.01;
const int NBINS = int(1 / STEP);

void high_quality_events_listing()
{
	TFile* f = new TFile("/sps/nemo/scratch/ikovalen/TKEvent/runs/Run-974.root");
	TTree* s = (TTree*) f->Get("Event");

	TKEvent* event = new TKEvent(-1,-1);
	s->SetBranchAddress("Eventdata", &event);
	
        TFile *quality_file = new TFile("events_with_quality_more_0_5.root", "RECREATE");
	
	TTree* Tree;
	int e_number;
	double quality;

	string treename = "Events with quality more than 0.5";
	Tree = new TTree(treename.c_str(), treename.c_str());

	TBranch* branch1 = Tree->Branch("e_number", &e_number, "event_number/I");
	TBranch* branch2 = Tree->Branch("quality", &quality, "quality/D");
	

	for(int event_number = 0; event_number < s->GetEntries(); event_number++)
	{	
		s->GetEntry(event_number);
		event->set_r("Manchester", "distance");
		event->set_h();
		event->reconstruct_ML(0);
		
		if (event->get_no_tracks() == 1)
		{	
			if (event->get_track(0)->get_quality() > 0.5)
			{
				quality = event->get_track(0)->get_quality();
				e_number = event_number;
				Tree->Fill();
			}
		}
		if (event_number % 10000 == 0) cout << "Event " << event_number << "/" << s->GetEntries() << " is DONE!" << endl; 
	}
	
	Tree->Write(treename.c_str());	

	quality_file->Close();

}
