#include "/sps/nemo/scratch/ikovalen/TKEvent_old/TKEvent/include/TKEvent.h"

R__LOAD_LIBRARY(/sps/nemo/scratch/ikovalen/TKEvent_old/TKEvent/lib/libTKEvent.so);

using namespace std;

const double STEP = 0.01;
const int NBINS = int(1 / STEP);

void quality_events_search()
{
	TFile* f = new TFile("/sps/nemo/scratch/ikovalen/TKEvent_old/runs/Run-728.root");
	TTree* s = (TTree*) f->Get("Event");

	TKEvent* event = new TKEvent(-1,-1);
	s->SetBranchAddress("Eventdata", &event);

        TFile *quality_file = new TFile("quality_file.root", "RECREATE");
		
	TTree* Tree[NBINS];
	TTree* Tree_r[NBINS];
	TTree* Tree_z[NBINS];

	int e_number[NBINS];
	int e_number_r[NBINS];
	int e_number_z[NBINS];

	double quality[NBINS];
	double r_quality[NBINS];
	double z_quality[NBINS];
	
	for(int BIN = 0; BIN < NBINS; BIN++)
	{
		stringstream treename;
		treename << "Events with quality from " << STEP*BIN << " to " << (BIN + 1) * STEP;
		Tree[BIN] = new TTree(treename.str().c_str(), treename.str().c_str());

		TBranch* branch1 = Tree[BIN]->Branch("e_number", &e_number[BIN], "event_number/I");
		TBranch* branch2 = Tree[BIN]->Branch("quality", &quality[BIN], "quality/D");
	}

        for(int BIN = 0; BIN < NBINS; BIN++)
        {
		stringstream treename;
                treename << "Events with r_quality from " << STEP*BIN << " to " << (BIN + 1) * STEP;
                Tree_r[BIN] = new TTree(treename.str().c_str(), treename.str().c_str());

                TBranch* branch1 = Tree_r[BIN]->Branch("e_number_r", &e_number_r[BIN], "event_number_r/I");
                TBranch* branch2 = Tree_r[BIN]->Branch("r_quality", &r_quality[BIN], "r_quality/D");
        }

        for(int BIN = 0; BIN < NBINS; BIN++)
        {
		stringstream treename;
                treename << "Events with z_quality from " << STEP*BIN << " to " << (BIN + 1) * STEP;
                Tree_z[BIN] = new TTree(treename.str().c_str(), treename.str().c_str());

                TBranch* branch1 = Tree_z[BIN]->Branch("e_number_z", &e_number_z[BIN], "event_number_z/I");
                TBranch* branch2 = Tree_z[BIN]->Branch("z_quality", &z_quality[BIN], "z_quality/D");
        }


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
			double q = event->get_track(0)->get_quality();
			int bin = int(q / STEP);
			if ( bin == 100 ) bin = 99;
			e_number[bin] = event_number;
			quality[bin] = q;
			Tree[bin]->Fill();
	
			double r_q = event->get_track(0)->get_quality_R();
			int r_bin = int(r_q / STEP);
			if ( r_bin == 100 ) r_bin = 99; 
			e_number_r[r_bin] = event_number;
			r_quality[r_bin] = r_q;
			Tree_r[r_bin]->Fill();
						
			double z_q = event->get_track(0)->get_quality_Z();
			if (z_q == -1)
			{
				z_q = 0;
			}	
			int z_bin = int(z_q / STEP);
			if ( z_bin == 100 ) z_bin = 99;
			e_number_z[z_bin] = event_number;
			z_quality[z_bin] = z_q;
			Tree_z[z_bin]->Fill();
			
		// h->Fill();
		}
		if (event_number % 10000 == 0) cout << "Event " << event_number << "/" << s->GetEntries() << " is DONE!" << endl; 
	}
	


	for(int BIN = 0; BIN < NBINS; BIN++)
	{
        	stringstream treename;
               	treename << "Events with quality from " << STEP*BIN << " to " << (BIN + 1) * STEP;
		Tree[BIN]->Write(treename.str().c_str());	
	}

        for(int BIN = 0; BIN < NBINS; BIN++)
        {
                stringstream treename;
                treename << "Events with r_quality from " << STEP*BIN << " to " << (BIN + 1) * STEP;
                Tree_r[BIN]->Write(treename.str().c_str());
        }


        for(int BIN = 0; BIN < NBINS; BIN++)
        {
                stringstream treename;
                treename << "Events with z_quality from " << STEP*BIN << " to " << (BIN + 1) * STEP;
                Tree_z[BIN]->Write(treename.str().c_str());
        }

	quality_file->Close();


}
