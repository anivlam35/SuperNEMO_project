#include "/sps/nemo/scratch/ikovalen/TKEvent/TKEvent/include/TKEvent.h"

R__LOAD_LIBRARY(/sps/nemo/scratch/ikovalen/TKEvent/TKEvent/lib/libTKEvent.so);

using namespace std;

const double STEP = 0.01;
const int NBINS = int(1 / STEP);

void make_quality_examples(){
	int BIN_number = 5;
        cout << "In this configuration there are " << NBINS << " bins. \nEnter the bin to make examples: ";
	cin >> BIN_number;
	
	int num_examples = 2;
	cout << "Enter the number of examples: ";
 	cin >> num_examples;


	TFile* f = new TFile("quality_file.root");
	
        TTree* Tree[NBINS];
        int e_number[NBINS];
        double quality[NBINS];

        for(int BIN = 0; BIN < NBINS; BIN++)
        {
                stringstream treename;
                treename << "Events with quality from " << STEP*BIN << " to " << (BIN + 1) * STEP;
		
		Tree[BIN] = (TTree*) f->Get(treename.str().c_str());
		
		Tree[BIN]->SetBranchAddress("e_number", &e_number[BIN]);
		Tree[BIN]->SetBranchAddress("quality", &quality[BIN]);                
        }

	TFile* rf = new TFile("/sps/nemo/scratch/ikovalen/TKEvent/runs/Run-974.root");	
	TTree* s = (TTree*) rf->Get("Event");

	TKEvent* eve = new TKEvent();	
	s->SetBranchAddress("Eventdata", &eve);


	TRandom3* r = new TRandom3();
        r->SetSeed(42);
	
	
	char dir_name[50];
	strcpy(dir_name, Form("quality_%.3lf_example", double(BIN_number) / double(NBINS)));
	cout << dir_name << endl;

	for(int i = 0; i < num_examples; i++)
	{
		Tree[BIN_number]->GetEntry(r->Integer(Tree[BIN_number]->GetEntries()));
		
		s->GetEntry(e_number[BIN_number]);
		
		eve->set_r("Manchester", "distance");
		eve->set_h();
		eve->reconstruct_ML(0);

		eve->make_top_projection(2, dir_name);
		
	}


	
	return 0;
}
