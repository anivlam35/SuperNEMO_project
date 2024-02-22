#include "/sps/nemo/scratch/ikovalen/TKEvent/TKEvent/include/TKEvent.h"

R__LOAD_LIBRARY(/sps/nemo/scratch/ikovalen/TKEvent/TKEvent/lib/libTKEvent.so);

using namespace std;
using namespace TMath;

const int X_OUTERMOST_1 = -10;
const int X_OUTERMOST_2 =  10;
const int N_SRCPLN      =  14; 
const int X_BasePlane   =   0;



void DBScan()
{
	int Y = 1;

	TFile* f = new TFile("/sps/nemo/scratch/ikovalen/run_728/V_code/FoilPlane_Tracks.root");

	TTree*   Tree[2][N_SRCPLN];
        double A_Tree[2][N_SRCPLN];
        double B_Tree[2][N_SRCPLN];
        double C_Tree[2][N_SRCPLN];
        double D_Tree[2][N_SRCPLN];

	cout << "Point " << Y << " is OK!" << endl;
	Y++;

        for(int NSOR = 0; NSOR<N_SRCPLN; NSOR++)
        {
                stringstream ss0, ss1;
                ss0 << "Tracks for " << X_BasePlane << " mm x < 0 Source " << NSOR;
                ss1 << "Tracks for " << X_BasePlane << " mm x > 0 Source " << NSOR;

                // Trees (0 for x<0, and 1 for x>0)
                Tree[0][NSOR] = (TTree*) f->Get(ss0.str().c_str());

                Tree[0][NSOR]->SetBranchAddress("A", &A_Tree[0][NSOR]);
                Tree[0][NSOR]->SetBranchAddress("B", &B_Tree[0][NSOR]);
                Tree[0][NSOR]->SetBranchAddress("C", &C_Tree[0][NSOR]);
                Tree[0][NSOR]->SetBranchAddress("D", &D_Tree[0][NSOR]);

                Tree[1][NSOR] = (TTree*) f->Get(ss1.str().c_str());

                Tree[1][NSOR]->SetBranchAddress("A", &A_Tree[1][NSOR]);
                Tree[1][NSOR]->SetBranchAddress("B", &B_Tree[1][NSOR]);
                Tree[1][NSOR]->SetBranchAddress("C", &C_Tree[1][NSOR]);
                Tree[1][NSOR]->SetBranchAddress("D", &D_Tree[1][NSOR]);
        }


	cout << "Point " << Y << " is OK!" << endl;
        Y++;

	int SIDE = 0;
	int NS = 5;
	//int N_tracks = Tree[SIDE][NS]->GetEntries();	
	int N_tracks = 2000;	

	cout << N_tracks << endl;	

	double points_outermost_1[N_tracks][2];
	double points_outermost_2[N_tracks][2];

	for(int track_num = 0; track_num < N_tracks; track_num++)
	{	
		Tree[SIDE][NS]->GetEntry(track_num);
		
		points_outermost_1[track_num][0] = A_Tree[SIDE][NS] * X_OUTERMOST_1 + B_Tree[SIDE][NS];
		points_outermost_1[track_num][1] = C_Tree[SIDE][NS] * X_OUTERMOST_1 + D_Tree[SIDE][NS];

//		cout << "Side 1  Point " << track_num << ": x = " << points_outermost_1[track_num][0] << " mm, y = " << points_outermost_1[track_num][1] << " mm." << endl;
		
		points_outermost_2[track_num][0] = A_Tree[SIDE][NS] * X_OUTERMOST_2 + B_Tree[SIDE][NS];
                points_outermost_2[track_num][1] = C_Tree[SIDE][NS] * X_OUTERMOST_2 + D_Tree[SIDE][NS];
	} 
	
	cout << "Point " << Y << " is OK!" << endl;
        Y++;
	
	const double EPS = 0.1;
	const int MIN_POINTS = 100;
	
	int lables_1[N_tracks];
	int lables_2[N_tracks];	

	for(int i = 0; i < N_tracks; i++)
	{
		lables_1[i] = 0;
		lables_2[i] = 0;
	}

	cout << "Point " << Y << " is OK!" << endl;
        Y++;

	for(int point_num = 0; point_num < N_tracks; point_num++)
	{
		if (lables_1[point_num] != 0) continue;
		
		int* neighbors_list = new int[N_tracks];
		neighbors_list[0] = -1;
		
		int neighbor_index = 0;		
		

	        for(int i = 0; i < N_tracks; i++)
		{
        	        if (Sqrt(Power(points_outermost_1[point_num][0] - points_outermost_1[i][0], 2) + Power(points_outermost_1[point_num][1] - points_outermost_1[i][1], 2)) < EPS)
                	{
                        	neighbors_list[neighbor_index] = i;
                        	neighbors_list[neighbor_index + 1] = -1;
                        	neighbor_index++;
               		}
        	}

		int N = 0;
		int i = 0;
		while (neighbors_list[i] != -1) N++;

		if (N < MIN_POINTS)
		{
			for(int i = 0; i < N; i++)
			{
				lables_1[neighbors_list[i]] = -1;
			}
		}	
		else
		{
			for(int i = 0; i < N; i++)
                        {
                                lables_1[neighbors_list[i]] = 1;
                        }
		}
	}

	cout << "Number of traks: " << N_tracks << endl;

	int S = 0;
	for(int i = 0; i < N_tracks; i++)
	{
		S += lables_1[i];
	}

	cout << "Sum S : " << S << endl;
}
