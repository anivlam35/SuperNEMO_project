#include "/sps/nemo/scratch/ikovalen/TKEvent/TKEvent/include/TKEvent.h"

R__LOAD_LIBRARY(/sps/nemo/scratch/ikovalen/TKEvent/TKEvent/lib/libTKEvent.so);

using namespace std;


void ambg_test(){

	TFile* f = new TFile("/sps/nemo/scratch/ikovalen/TKEvent/runs/Run-974.root");	
	TTree* s = (TTree*) f->Get("Event");

	TKEvent* eve = new TKEvent();	
	s->SetBranchAddress("Eventdata", &eve);
	
	int j = 9;
	
	s->GetEntry(j);
		
	eve->set_r("Manchester", "distance");
	eve->set_h();
	eve->reconstruct_ML(0);

	eve->get_track(0)->get_mirror_image();
		
	eve->make_top_projection(2, "ambg_test_pics");

	


	
	return 0;
}
