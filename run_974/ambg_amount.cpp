#include "/sps/nemo/scratch/ikovalen/TKEvent/TKEvent/include/TKEvent.h"

R__LOAD_LIBRARY(/sps/nemo/scratch/ikovalen/TKEvent/TKEvent/lib/libTKEvent.so);

using namespace std;


void ambg_amount(){

        TFile* f = new TFile("/sps/nemo/scratch/ikovalen/TKEvent/runs/Run-974.root");
        TTree* s = (TTree*) f->Get("Event");

        TKEvent* eve = new TKEvent();
        s->SetBranchAddress("Eventdata", &eve);
	
	int N = 100000;	

	int ambg_num  = 0;
	for(int i = 0; i < N; i++)
	{
                s->GetEntry(i);

                eve->set_r("Manchester", "distance");
                eve->set_h();
                eve->reconstruct_ML(0);
	
		for(int j = 0; j < eve->get_no_tracks(); j++)
		{
			if (eve->get_track(j)->get_ambiguity_type() != 0)
			{
				//cout << "Ambiguity type of event " << i << " is " << eve->get_track(j)->get_ambiguity_type() << endl;
				ambg_num++;
				break;
			}
		}

 	}

	cout << endl;
	cout << "Tracks with ambiguity: " << ((double)ambg_num * 100. / (double)N) << "%." << endl;



	return 0;
}


