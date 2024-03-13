#include "/sps/nemo/scratch/ikovalen//TKEvent/TKEvent/include/TKEvent.h" 
#include "config.h"

R__LOAD_LIBRARY(/sps/nemo/scratch/ikovalen/TKEvent/TKEvent/lib/libTKEvent.so);

using namespace std;
using namespace TMath;

void test()
{
	TFile* f = new TFile(Form("/sps/nemo/scratch/ikovalen/TKEvent/runs/Run-%d.root", RUN_N));
	TTree* s = (TTree*) f->Get("Event");
//	s->Print();

	TKEvent* Eve = new TKEvent(-1,-1);
	s->SetBranchAddress("Eventdata", &Eve);
	
	s->GetEntry(7623708);
	
	Eve->set_r("Manchester", "distance");
	Eve->set_h();
	Eve->reconstruct_ML(0);

	Eve->print();
	Eve->make_top_projection(2, "test_folder");

}
