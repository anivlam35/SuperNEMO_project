#ifndef __CONFIG_H__
#define __CONFIG_H__

const   int     RUN_N       = 988;


const   double  mw_sizex    = 194.0;
const   double  mw_sizey    = 260.0;
const   double  mw_sizez    = 260.0;

const   double  gv_sizex    = 308.0;
const   double  gv_sizey    = 310.0;
const   double  gv_sizez    = 150.0;

const   double  xw_sizex    = 200.0;
const   double  xw_sizey    = 150.0;
const   double  xw_sizez    = 208.5;


const   int     N_OMs       = 712;
const   double  Y_MIN       = -20 * mw_sizey / 2; //-2140.0;
const   double  Y_MAX       = 20 * mw_sizey / 2;  //-2020.0;
const   double  Z_MIN       = -13 * mw_sizez / 2; //-100.0;
const   double  Z_MAX       = 13 * mw_sizez / 2;  //100.0;

const   char    PATH[]      = "./cutted_data_2770mm/";
const   double  eff_len     = 2770.0;

        int     SWCR[4];
        double  xyz[3];

const   double  X_shifts[2] = {-20.0, 20.0};

const   double  k_mean_cut  = 0.5;
const   double  k_mean_spaghetti = 0.2;

const   double  X_OBSERV_MIN = -40.0;
const   double  X_OBSERV_MAX = 40.0;
const   double  STEP         = 10.0;


#endif