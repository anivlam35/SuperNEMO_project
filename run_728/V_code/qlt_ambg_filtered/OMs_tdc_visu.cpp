
#include "/sps/nemo/scratch/ikovalen//TKEvent_old/TKEvent/include/TKEvent.h" 

R__LOAD_LIBRARY(/sps/nemo/scratch/ikovalen/TKEvent_old/TKEvent/lib/libTKEvent.so);

using namespace std;
using namespace TMath;

const    int N_OMs  = 712;


void OMs_tdc_visu()
{
	TFile* f = new TFile("OMs_charges.root","RECREATE"); // new root file for received histograms
	
	for(int N_OM = 5; N_OM < 6; N_OM++)
	//for(int N_OM = 0; N_OM < N_OMs; N_OM++)
	{
		stringstream tree_name;
		tree_name << "Depposed charges of OM " << N_OM; 
		
		TTree* tr = (TTree*) f->Get(tree_name.str().c_str());

		int tdc;
		tr->SetBranchAddress("CHRG", &tdc);
		
		TCanvas* C = new TCanvas("C", "Histogram of deposed charge in OM", 800, 600);
		TH1F* tdc_hist = new TH1F(Form("Histo for TDC in OM#%d", N_OM),Form("Histo for TDC in OM#%d", N_OM), 0, tr->GetMaximum("CHRG"), 100);
		
		for(int i = 0; i < tr->GetEntries(); i++)
		{
			tr->GetEntry(i);
			tdc_hist->Fill(tdc);
		}

		tdc_hist->Draw();
		C->Print(Form("./OMs_tdc_visu/TDC_OM%d.png", N_OM));
	}
	cout << "DONE!" << endl;
}
