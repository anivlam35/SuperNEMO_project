#include "/sps/nemo/scratch/ikovalen/TKEvent/TKEvent/include/TKEvent.h"

R__LOAD_LIBRARY(/sps/nemo/scratch/ikovalen/TKEvent/TKEvent/lib/libTKEvent.so);

using namespace std;

const double STEP = 0.01;
const int NBINS = int(1 / STEP);

void high_quality_events_listing()
{
	TFile* f = new TFile("quality_file.root");
	
	double input_lower_quality;
	cout << "Enter lower boundary of quality: ";
	cin >> input_lower_quality;

        double input_upper_quality;
        cout << "Enter upper boundary of quality: ";
        cin >> input_upper_quality;

	int START_BIN = input_lower_quality / STEP;
	double l_qual = START_BIN * STEP;

        int END_BIN = input_upper_quality / STEP;
        double u_qual = END_BIN * STEP;

        TFile *output_file = new TFile(Form("events_with_quality_from_%3.2lf_to_%3.2lf.root", l_qual, u_qual), "RECREATE");
	
	TTree* Out_Tree;
	int e_number;
	double quality;

	string treename = Form("Events with quality from %3.2lf to %3.2lf", l_qual, u_qual);
	Out_Tree = new TTree(treename.c_str(), treename.c_str());

	TBranch* branch1 = Out_Tree->Branch("e_number", &e_number, "event_number/I");
	TBranch* branch2 = Out_Tree->Branch("quality", &quality, "quality/D");
	
        for(int BIN = START_BIN; BIN < END_BIN; BIN++)
        {
                stringstream treename;
                treename << "Events with quality from " << STEP*BIN << " to " << (BIN + 1) * STEP;
		
		TTree* Tree = (TTree*) f->Get(treename.str().c_str());
		
		Tree->SetBranchAddress("e_number", &e_number);
		Tree->SetBranchAddress("quality", &quality);
		for(int Entry = 0; Entry < Tree->GetEntries(); Entry++)
		{
			Tree->GetEntry(Entry);
			Out_Tree->Fill();		
		}                
    		cout << "BIN " << BIN - START_BIN + 1  << "/" << END_BIN - START_BIN << " is DONE!" << endl;
	}
	
	Out_Tree->Write(treename.c_str());	

	output_file->Close();
}
