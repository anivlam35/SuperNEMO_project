#include "/sps/nemo/scratch/ikovalen/TKEvent/TKEvent/include/TKEvent.h"

R__LOAD_LIBRARY(/sps/nemo/scratch/ikovalen/TKEvent/TKEvent/lib/libTKEvent.so);

using namespace std;

const double STEP = 0.01;
const int NBINS = int(1 / STEP);

void high_quality_events_listing()
{
	TFile* f = new TFile("quality_file.root");
	
	double input_quality;
	cout << "Enter lower boundary of quality: ";
	cin >> input_quality;

	int START_BIN = input_quality / STEP;
	double l_qual = START_BIN * STEP;

        TFile *output_file = new TFile(Form("events_with_quality_more_%3.2lf.root", l_qual), "RECREATE");
	
	TTree* Out_Tree;
	int e_number;
	double quality;

	string treename = Form("Events with quality more than %3.2lf", l_qual);
	Out_Tree = new TTree(treename.c_str(), treename.c_str());

	TBranch* branch1 = Out_Tree->Branch("e_number", &e_number, "event_number/I");
	TBranch* branch2 = Out_Tree->Branch("quality", &quality, "quality/D");
	
        for(int BIN = START_BIN; BIN < NBINS; BIN++)
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
    		cout << "BIN " << BIN - START_BIN + 1  << "/" << NBINS - START_BIN << " is DONE!" << endl;
	}
	
	Out_Tree->Write(treename.c_str());	

	output_file->Close();
	
//	output_file = new TFile(Form("events_with_quality_more_%3.2lf.root", l_qual));
//	TTree* Tree = (TTree*)output_file->Get(Form("Events with quality more than %3.2lf", l_qual));
//      Tree->SetBranchAddress("e_number", &e_number);
//      Tree->SetBranchAddress("quality", &quality);
//	for(int Entry = 100000; Entry < 101000; Entry++)
//	{
//		Tree->GetEntry(Entry);
//		cout << "Event number: " << e_number << ". Quality: " << quality << "." << endl; 	
//	}
}
