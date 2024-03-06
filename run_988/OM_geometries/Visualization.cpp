#include "/sps/nemo/scratch/ikovalen/TKEvent_old/TKEvent/include/TKEvent.h"
#include "config.h"

using namespace std;

R__LOAD_LIBRARY(/sps/nemo/scratch/ikovalen/TKEvent_old/TKEvent/lib/libTKEvent.so);

void Visualization(){
	TFile* f = new TFile(Form("OMs_Tracks_Run-%d.root", RUN_N));

		
}
