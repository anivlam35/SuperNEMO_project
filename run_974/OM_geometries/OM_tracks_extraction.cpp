#include "/sps/nemo/scratch/ikovalen/TKEvent/TKEvent/include/TKEvent.h"

R__LOAD_LIBRARY(/sps/nemo/scratch/ikovalen/TKEvent/TKEvent/lib/libTKEvent.so);

using namespace std;


void OM_tracks_extraction()
{
	TFile* f = new TFile("/sps/nemo/scratch/ikovalen/TKEvent/runs/Run-974.root");
        TTree* s = (TTree*) f->Get("Event");

        TKEvent* event = new TKEvent(-1,-1);
        s->SetBranchAddress("Eventdata", &event);

	TTree*   Tree[N_SRCPLN];
        double A_Tree[N_SRCPLN];
        double B_Tree[N_SRCPLN];
        double C_Tree[N_SRCPLN];
        double D_Tree[N_SRCPLN];

        for(int NSOR = 0; NSOR<N_SRCPLN; NSOR++)
        {
                stringstream ss0, ss1;
                ss0 << "Tracks for " << X_BasePlane << " mm x < 0 Source " << NSOR;
                ss1 << "Tracks for " << X_BasePlane << " mm x > 0 Source " << NSOR;

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


	
	return 0;
}
