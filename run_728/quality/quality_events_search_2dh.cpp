#include "/sps/nemo/scratch/ikovalen/TKEvent_old/TKEvent/include/TKEvent.h"

R__LOAD_LIBRARY(/sps/nemo/scratch/ikovalen/TKEvent_old/TKEvent/lib/libTKEvent.so);

using namespace std;

const double STEP = 0.01;
const int NBINS = int(1 / STEP);

void quality_events_search_2dh()
{
	TFile* f = new TFile("/sps/nemo/scratch/ikovalen/TKEvent_old/runs/Run-728.root");
	TTree* s = (TTree*) f->Get("Event");

	TKEvent* event = new TKEvent(-1,-1);
	s->SetBranchAddress("Eventdata", &event);

        TFile *quality_file = new TFile("quality_file_2dh.root", "RECREATE");
		
	TTree* Tree;
	int e_number;

	double quality;
	double r_quality;
	double z_quality;
	
	Tree = new TTree("Events with quality", "Events with quality");

	TBranch* branch1 = Tree->Branch("e_number", &e_number, "event_number/I");
	TBranch* branch2 = Tree->Branch("quality", &quality, "quality/D");
	TBranch* branch3 = Tree->Branch("r_quality", &r_quality, "r_quality/D");
	TBranch* branch4 = Tree->Branch("z_quality", &z_quality, "z_quality/D");
	



//	for(int event_number = 0; event_number < 10000; event_number++)
	for(int event_number = 0; event_number < s->GetEntries(); event_number++)
	{	
		s->GetEntry(event_number);
		event->set_r("Manchester", "distance");
		event->set_h();
		event->reconstruct_ML(0);

		if (event->get_no_tracks() == 0){continue;}
		else if ((event->get_no_tracks() == 1) || 
			 (event->get_no_tracks() == 2 && event->get_track(0)->get_ambiguity_type() != 0))
		{	
			e_number = event_number;

			quality = event->get_track(0)->get_quality();
			r_quality = event->get_track(0)->get_quality_R();
			z_quality = event->get_track(0)->get_quality_Z();
			if (z_quality == -1) z_quality = 0;
			
			Tree->Fill();
	

		// h->Fill();
		}
		if (event_number % 10000 == 0) cout << "Event " << event_number << "/" << s->GetEntries() << " is DONE!" << endl; 
	}
	


	Tree->Write("Events with quality");	

	quality_file->Close();


}
