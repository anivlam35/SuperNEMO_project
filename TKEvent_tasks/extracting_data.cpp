#include "../../TKEvent/TKEvent/include/TKEvent.h"
#include "TMath.h"

R__LOAD_LIBRARY(../TKEvent/lib/libTKEvent.so);

void extracting_data(){
	int run_number = 974;
	int event_number = 9;
	// Uploading necessary run 
	TFile* file = new TFile(Form("../runs/Run-%d.root", run_number));
	TTree* tree = (TTree*) file->Get("Event");
	
	// Uploading necessary event
	TKEvent* event = new TKEvent();
	tree->SetBranchAddress("Eventdata", &event);	
	tree->GetEntry(event_number);
	event->set_r("Manchester", "distance");
	event->reconstruct_ML(0);

	// Extracting reconstructed tracks, defining variables for tree
	std::vector<TKtrack *> tracks = event->get_tracks();	

	Float_t x, y, r, h;
	Float_t rdtl; // Real distance to a track line
	Double_t a, b, c, d;

	// TTree for track lines parameters
        TTree *track_lines = new TTree("track_lines", "Parameters of track lines");
        track_lines->Branch("a", &a, "a/D");
        track_lines->Branch("b", &b, "b/D");
        track_lines->Branch("c", &c, "c/D");
        track_lines->Branch("d", &d, "d/D");

	// Vector of ttrees of hits parameters (x, y, r, h), every ttree regards to some track 
	std::vector<TTree *> hits_data;
	
	for (int i=0; i<tracks.size(); i++){
		a = tracks[i]->get_a();
                b = tracks[i]->get_b();
                c = tracks[i]->get_c();
                d = tracks[i]->get_d();
		track_lines->Fill();
		
		TTree *hd = new TTree("hd", "Temporary hits data");
		hd->Branch("x", &x, "x/F");
                hd->Branch("y",	&y, "y/F");
                hd->Branch("h", &h, "h/F");
                hd->Branch("r",	&r, "r/F");
		hd->Branch("rdtl", &rdtl, "rdtl/F");
		
		std::vector<TKtrhit *> hits = tracks[i]->get_associated_tr_hits();
		
		for (int j=0; j<hits.size(); j++){
			x = hits[j]->get_xy('x');
			y = hits[j]->get_xy('y');
			r = hits[j]->get_r();
                        h = hits[j]->get_h();
			rdtl = TMath::Abs(a * x - y + b) / TMath::Sqrt(a * a + 1);
			hd->Fill();
		}
		
		hits_data.push_back(hd);
	}
	
	for (int i=0; i<tracks.size(); i++){
		cout << Form("\n\n########## TRACK %d HITS ##########\n", i+1) << endl;	
		hits_data[i]->Scan();
	}
}
