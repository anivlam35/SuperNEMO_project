#ifndef __CONFIG_H__
#define __CONFIG_H__

const int    RUN_N  = 988;
const int    N_OMs  = 712;
const double Y_MIN  = -2560.0; //-2140.0;
const double Y_MAX  = 2560.0;  //-2020.0;
const double Z_MIN  = -1664.0; //-100.0;
const double Z_MAX  = 1664.0;  //100.0;
const int    N_ROWS = 7;
const int    N_COLS = 6;

const double mw_sizex = 194.0;
const double mw_sizey = 256.0;
const double mw_sizez = 256.0;

const double gv_sizex = 308.0;
const double gv_sizey = 310.0;
const double gv_sizez = 150.0;

const double xw_sizex = 200.0;
const double xw_sizey = 150.0;
const double xw_sizez = 208.5;

int SWCR[4];
double xyz[3];

#endif
