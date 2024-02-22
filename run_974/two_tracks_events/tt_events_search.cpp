#include "/sps/nemo/scratch/ikovalen/TKEvent/TKEvent/include/TKEvent.h"

R__LOAD_LIBRARY(/sps/nemo/scratch/ikovalen/TKEvent/TKEvent/lib/libTKEvent.so);

using namespace std;


void tt_events_search(){

	TFile* f = new TFile("/sps/nemo/scratch/ikovalen/TKEvent/runs/Run-974.root");	
	TTree* s = (TTree*) f->Get("Event");

	TKEvent* eve = new TKEvent();	
	s->SetBranchAddress("Eventdata", &eve);
	
	int example_num = 0;
	int j = 0;
	while (example_num != 5)
	{
		s->GetEntry(j);
		
		eve->set_r("Manchester", "distance");
		eve->set_h();
		eve->reconstruct_ML(0);
		
		if (eve->get_no_tracks() > 0)
		{
			if (eve->get_track(0)->get_ambiguity_type() == 4)
			{
				eve->make_top_projection(2, "tt_events_examples");
				example_num++;
			}
		}
		j++;
	}


	
	return 0;
}
