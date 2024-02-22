#include "/sps/nemo/scratch/ikovalen/TKEvent/TKEvent/include/TKEvent.h" 

R__LOAD_LIBRARY(/sps/nemo/scratch/ikovalen/TKEvent/TKEvent/lib/libTKEvent.so);

using namespace std;
using namespace TMath;


const    int ABS_X_MAX = 10;
const    int ABS_X_MIN = 10;
const double X_STP     = 1.0;
const    int N_SRCPLN  = 42;
// Set Ranges
const double Y_MIN  = -2500.0; //-2140.0; 
const double Y_MAX  = -840.0;  //-2020.0;
const    int Y_BINS = 830;     //12;
const double Z_MIN  = -1638.0; //-100.0; 
const double Z_MAX  = 1638.0;  //100.0;
const    int Z_BINS = 468;     //20;

int X_BasePlane = 0;

void FoilPlane_TracksSaving()
{
	TFile* f = new TFile("/sps/nemo/scratch/ikovalen/TKEvent/runs/Run-974.root");
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
		ss0 << "Tracks for " << X_BasePlane << " mm x < 0 Source " << NSOR; 
		ss1 << "Tracks for " << X_BasePlane << " mm x > 0 Source " << NSOR; 
		
		// Trees (0 for x<0, and 1 for x>0)
		Tree[0][NSOR] = new TTree(ss0.str().c_str(), ss0.str().c_str());

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
		for(int NSOR = 0; NSOR<N_SRCPLN; NSOR++)
		{
			if (Eve->get_no_tracks() == 1)
			{
				double Y = Eve->get_tracks()[0]->get_a()*X_BasePlane*X_STP + Eve->get_tracks()[0]->get_b();
				double Z = Eve->get_tracks()[0]->get_c()*X_BasePlane*X_STP + Eve->get_tracks()[0]->get_d();

				if(Z!=0 && Y_MIN + NSOR%6*830 < Y && Y < Y_MIN + (NSOR%6+1)*830 && Z_MAX-((NSOR/6)+1)*468 < Z && Z < Z_MAX-(NSOR/6)*468)
				{
					if(Eve->get_tracks()[0]->get_side()==0)
					{
						A_Tree[0][NSOR] = Eve->get_tracks()[0]->get_a();
						B_Tree[0][NSOR] = Eve->get_tracks()[0]->get_b();
						C_Tree[0][NSOR] = Eve->get_tracks()[0]->get_c();
						D_Tree[0][NSOR] = Eve->get_tracks()[0]->get_d();

						Tree[0][NSOR]->Fill();
					}
						
					else
					{
						A_Tree[1][NSOR] = Eve->get_tracks()[0]->get_a();
						B_Tree[1][NSOR] = Eve->get_tracks()[0]->get_b();
						C_Tree[1][NSOR] = Eve->get_tracks()[0]->get_c();
						D_Tree[1][NSOR] = Eve->get_tracks()[0]->get_d();

						Tree[1][NSOR]->Fill();
					}
				}
			}
		}
		if (i % 10000 == 0) cout <<"Event No. " << i << " done!" <<endl;
	}	


	// Writing Histograms
	for(int NSOR = 0; NSOR<N_SRCPLN; NSOR++)
	{
		stringstream ss0, ss1;
		ss0<<"Tracks for " << X_BasePlane << " mm x < 0 Source " << NSOR; 
		ss1<<"Tracks for " << X_BasePlane << " mm x > 0 Source " << NSOR;  
		
		// write histograms into new root file
		Tree[0][NSOR]->Write(ss0.str().c_str());
		Tree[1][NSOR]->Write(ss1.str().c_str());
	} 

	New_file->Close();
	cout << "DONE!" << endl;
}
