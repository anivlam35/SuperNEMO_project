#include "/sps/nemo/scratch/ikovalen//TKEvent/TKEvent/include/TKEvent.h" 
#include "config.h"

R__LOAD_LIBRARY(/sps/nemo/scratch/ikovalen/TKEvent/TKEvent/lib/libTKEvent.so);

using namespace std;
using namespace TMath;

void test()
{
	cout << (int)3.999999 / 2 << endl;
}
