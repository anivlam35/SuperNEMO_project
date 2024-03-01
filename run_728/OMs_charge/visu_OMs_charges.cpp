

#include "/sps/nemo/scratch/ikovalen//TKEvent_old/TKEvent/include/TKEvent.h" 

R__LOAD_LIBRARY(/sps/nemo/scratch/ikovalen/TKEvent_old/TKEvent/lib/libTKEvent.so);

using namespace std;
using namespace TMath;

const    int N_OMs  = 712;


void visu_OMs_charges()
{
	TFile* f = new TFile("OMs_charges.root"); // new root file for received histograms
	int N = 0;	
	//for(int N_OM = 5; N_OM < 6; N_OM++)
	for(int N_OM = 0; N_OM < N_OMs; N_OM++)
	{
		stringstream tree_name;
		tree_name << "Depposed charges of OM " << N_OM; 
		
		TTree* tr = (TTree*) f->Get(tree_name.str().c_str());
		int dc;
		tr->SetBranchAddress("CHRG", &dc);
		cout <<	"OM" <<	N_OM <<	" has "	<< tr->GetEntries() << " entries" << endl;
		N += tr->GetEntries();		

		TCanvas* C = new TCanvas("C", "Histogram of deposed charge in OM", 800, 600);
		TH1F* dc_hist = new TH1F(Form("Histo for deposited charges in OM#%d", N_OM),Form("Histo for deposited charges in OM#%d", N_OM), 100, tr->GetMinimum("CHRG"), tr->GetMaximum("CHRG"));
		
		for(int i = 0; i < tr->GetEntries(); i++)
		{
			tr->GetEntry(i);
			dc_hist->Fill(dc);
		}

		dc_hist->Draw();
		C->Print(Form("./histos_OMs_charges/DCharges_OM%d.png", N_OM));
	}
	cout << "DONE!" << endl;
	cout << "Total amount of entries: " << N << endl;
}
