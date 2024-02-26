#include "/sps/nemo/scratch/ikovalen//TKEvent_old/TKEvent/include/TKEvent.h" 

R__LOAD_LIBRARY(/sps/nemo/scratch/ikovalen/TKEvent_old/TKEvent/lib/libTKEvent.so);

using namespace std;
using namespace TMath;

const    int N_OMs  = 712;


void OMs_tdc_extracting()
{
	TFile* f = new TFile("/sps/nemo/scratch/ikovalen/TKEvent_old/runs/Run-728.root");
	TTree* s = (TTree*) f->Get("Event");
//	s->Print();

	TKEvent* Eve = new TKEvent(-1,-1);
	s->SetBranchAddress("Eventdata", &Eve);

	TFile *New_file = new TFile("OMs_charges.root","RECREATE"); // new root file for received histograms
	

	// Define Tree and Arrays
	TTree*   Tree[N_OMs];
	int CHRG_Tree[N_OMs];
	

	for(int N_OM = 0; N_OM < N_OMs; N_OM++)
	{
		stringstream ss0, ss1;
		ss0 << "Depposed charges of OM " << N_OM; 
		
		Tree[N_OM] = new TTree(ss0.str().c_str(), ss0.str().c_str());
		TBranch* branchCHRG = Tree[N_OM]->Branch("CHRG", &CHRG_Tree[N_OM], "CHRG/I");
	}

	// Filling Tree
	// Use s->GetEntries() for all entries in Run
	for(UInt_t i=0; i < 10000; i++)	// Loop over events
	//for(UInt_t i=0; i < s->GetEntries(); i++)	// Loop over events
	{
		s->GetEntry(i);
		Eve->set_r("Manchester", "distance");
		Eve->set_h();
		Eve->reconstruct_ML(0);		

		if (Eve->get_no_tracks() == 1)
		{	
			TKOMhit* OMhit = Eve->get_cluster(0)->get_tr_hits()[0]->get_associated_OMhit();
			int OM_num = OMhit->get_OM_num();
			cout << "TDC: " << OMhit->get_OM_pcell() << endl;
			CHRG_Tree[OM_num] = OMhit->get_OM_TDC();
			Tree[OM_num]->Fill(); 
		}
		else if (Eve->get_no_tracks() == 2)
                {
			if (Eve->get_track(0)->get_ambiguity_type() == 2)
			{
	                        TKOMhit* OMhit = Eve->get_cluster(0)->get_tr_hits()[0]->get_associated_OMhit();
                        	int OM_num = OMhit->get_OM_num();
				cout <<	"TDC: " << OMhit->get_OM_TDC() << endl;
                        	CHRG_Tree[OM_num] = OMhit->get_OM_TDC();
                        	Tree[OM_num]->Fill();
			}
                }
		if (i % 10000 == 0) cout <<"Event No. " << i << " done!" <<endl;
	}	


	// Writing Histograms
        for(int N_OM = 0; N_OM < N_OMs; N_OM++)
        {
                stringstream ss0, ss1;
                ss0 << "Depposed charges of OM " << N_OM;
		
		// write histograms into new root file
		Tree[N_OM]->Write(ss0.str().c_str());
	} 

	New_file->Close();
	cout << "DONE!" << endl;
}
