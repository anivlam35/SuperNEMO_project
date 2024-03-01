#include "/sps/nemo/scratch/ikovalen//TKEvent_old/TKEvent/include/TKEvent.h" 

R__LOAD_LIBRARY(/sps/nemo/scratch/ikovalen/TKEvent_old/TKEvent/lib/libTKEvent.so);

using namespace std;
using namespace TMath;


const    int ABS_X_MAX = 10;
const    int ABS_X_MIN = 10;
const    int N_SRCPLN  = 14;
// Set Ranges
const double Y_MIN  = -2500.0; //-2140.0; 
const double Y_MAX  = -834.0;  //-2020.0;
const    int Y_BINS = 833;     //12;
const double Z_MIN  = -1638.0; //-100.0; 
const double Z_MAX  = 1638.0;  //100.0;
const    int Z_BINS = 468;     //20;
const    int N_ROWS = 7;
const    int N_COLS = 2;

int event_selection(TKEvent* event, double low_qlt, double high_qlt, int ambg_mode); 
int choose_ambg_track(TKEvent* event);

void FoilPlane_TracksSaving()
{
	TFile* f = new TFile("/sps/nemo/scratch/ikovalen/TKEvent_old/runs/Run-728.root");
	TTree* s = (TTree*) f->Get("Event");
//	s->Print();

	TKEvent* Eve = new TKEvent(-1,-1);
	s->SetBranchAddress("Eventdata", &Eve);

	TFile *New_file = new TFile("FoilPlane_Tracks.root","RECREATE"); // new root file for received histograms
	

	// Define Tree and Arrays
	TTree*   Tree[2][N_SRCPLN];
	double A_Tree[2][N_SRCPLN];
	double B_Tree[2][N_SRCPLN];
	double C_Tree[2][N_SRCPLN];
	double D_Tree[2][N_SRCPLN];

	
	
	for(int NSOR = 0; NSOR<N_SRCPLN; NSOR++)
	{
		stringstream ss0, ss1;
		ss0 << "Tracks for " << 0 << " mm x < 0 Source " << NSOR; 
		ss1 << "Tracks for " << 0 << " mm x > 0 Source " << NSOR; 
		
		// Trees (0 for x<0, and 1 for x>0)
		Tree[0][NSOR] = new TTree(ss0.str().c_str(), ss0.str().c_str());
		//cout << ss0.str() << endl; 
		TBranch* branchA0 = Tree[0][NSOR]->Branch("A", &A_Tree[0][NSOR], "A/D");
		TBranch* branchB0 = Tree[0][NSOR]->Branch("B", &B_Tree[0][NSOR], "B/D");
		TBranch* branchC0 = Tree[0][NSOR]->Branch("C", &C_Tree[0][NSOR], "C/D");
		TBranch* branchD0 = Tree[0][NSOR]->Branch("D", &D_Tree[0][NSOR], "D/D");

		Tree[1][NSOR] = new TTree(ss1.str().c_str(), ss1.str().c_str());

		TBranch* branchA1 = Tree[1][NSOR]->Branch("A", &A_Tree[1][NSOR], "A/D");
		TBranch* branchB1 = Tree[1][NSOR]->Branch("B", &B_Tree[1][NSOR], "B/D");
		TBranch* branchC1 = Tree[1][NSOR]->Branch("C", &C_Tree[1][NSOR], "C/D");
		TBranch* branchD1 = Tree[1][NSOR]->Branch("D", &D_Tree[1][NSOR], "D/D");
	} 

	// Filling Tree
	// Use s->GetEntries() for all entries in Run
	for(UInt_t i=0; i < s->GetEntries(); i++)	// Loop over events
	{
		s->GetEntry(i);
		Eve->set_r("Manchester", "distance");
		Eve->set_h();
		Eve->reconstruct_ML(0);		
		
		int event_choice = event_selection(Eve, 0.6, 0.98, 1);
		if (event_choice)
		{	
			int track_num = event_choice - 1;
			double Y = Eve->get_track(track_num)->get_b();
			double Z = Eve->get_track(track_num)->get_d();

			if(Z!=0 && Z > Z_MIN && Z < Z_MAX && Y > Y_MIN && Y < Y_MAX)
			{
				int NSOR = N_COLS * ((int)(Z_MAX - Z) / 468) + (int)(Y - Y_MIN) / 833;	
				int side = Eve->get_track(track_num)->get_side();
				
				A_Tree[side][NSOR] = Eve->get_track(track_num)->get_a();
				B_Tree[side][NSOR] = Eve->get_track(track_num)->get_b();
				C_Tree[side][NSOR] = Eve->get_track(track_num)->get_c();
				D_Tree[side][NSOR] = Eve->get_track(0)->get_d();

				Tree[side][NSOR]->Fill();
			}
		}
		if (i % 10000 == 0) cout <<"Event No. " << i << " done!" <<endl;
	}	


	// Writing Histograms
	for(int NSOR = 0; NSOR<N_SRCPLN; NSOR++)
	{
		stringstream ss0, ss1;
		ss0<<"Tracks for " << 0 << " mm x < 0 Source " << NSOR; 
		ss1<<"Tracks for " << 0 << " mm x > 0 Source " << NSOR;  
		
		// write histograms into new root file
		Tree[0][NSOR]->Write(ss0.str().c_str());
		Tree[1][NSOR]->Write(ss1.str().c_str());
	} 

	New_file->Close();
	cout << "DONE!" << endl;
}

int event_selection(TKEvent* event, double low_qlt, double high_qlt, int ambg_mode)
{
	int choice;
	if(event->get_no_tracks() == 0)
	{
		choice = 0;
	}
	else if(event->get_no_tracks() == 1 && event->get_track(0)->get_quality() > low_qlt && event->get_track(0)->get_quality() < high_qlt)
	{
		choice = 1;
	}
	else if(event->get_no_tracks() == 2 && event->get_track(0)->get_quality() > low_qlt && event->get_track(0)->get_quality() < high_qlt)
	{
		switch(ambg_mode)
		{
		case 0:
			choice = 0;
			break;
		case 1:
			choice = choose_ambg_track(event);
			break;
		}
	}
	else
	{
		choice = 0;
	}	
	return choice;
}

int choose_ambg_track(TKEvent* event)
{
	int track;
	double Y1 = event->get_track(0)->get_b();
        double Z1 = event->get_track(0)->get_d();

        double Y2 = event->get_track(1)->get_b();
        double Z2 = event->get_track(1)->get_d();

        if (Z1!=0 && Z2!=0 && Z1 > Z_MIN && Z1 < Z_MAX && Y1 > Y_MIN && Y1 < Y_MAX && Z2 > Z_MIN && Z2 < Z_MAX && Y2 > Y_MIN && Y2 < Y_MAX)
        {
        	int NSOR1 = N_COLS * ((int)(Z_MAX - Z1) / 468) + (int)(Y1 - Y_MIN) / 833;
                int NSOR2 = N_COLS * ((int)(Z_MAX - Z2) / 468) + (int)(Y2 - Y_MIN) / 833;

                double Y_SOR1 = Y_MIN + (NSOR1 % N_COLS + 0.5) * 833;
                double Z_SOR1 = Z_MAX - (NSOR1 / N_COLS + 0.5) * 468;

                double Y_SOR2 = Y_MIN + (NSOR2 % N_COLS + 0.5) * 833;
                double Z_SOR2 = Z_MAX - (NSOR2 / N_COLS + 0.5) * 468;

                double DIST_1 = Sqrt(Power(Y1 - Y_SOR1, 2) + Power(Z1 - Z_SOR1, 2));
                double DIST_2 = Sqrt(Power(Y2 - Y_SOR2, 2) + Power(Z2 - Z_SOR2, 2));
		

		if(DIST_1 < DIST_2) {track = 1;}
		else {track = 2;}
	}
	else
	{
		track = 0;
	}
	
	return track;
}
