#ifndef __CONFIG_H__
#define __CONFIG_H__

const int    RUN_N        = 988;
const double Y_MIN        = -2500.0;
const double Y_MAX        = 2500.0;
const double Y_RECT_SIZE  = 833.34;
const double Z_MIN        = -1487.5;
const double Z_MAX        = 1487.5;
const double Z_RECT_SIZE  = 425;
const int    X_BasePlane  = 0;
const int    X_OBSERV_MIN = -10;
const int    X_OBSERV_MAX = 10;
const double STEP	  = 0.5;
const int    X_OBSERV     = 0;
const int    N_SRCPLN     = 42;
const int    N_ROWS	  = 7;
const int    N_COLS	  = 6;

const double LOW_Q	  = 0.6;
const double UP_Q	  = 0.98;
const double LOW_QZ	  = 0.7;
const double UP_QZ	  = 0.98;
const int    AMBG_MODE	  = 0;

const char   PATH[]	  = "./2770mm_q_0.6_0.98_ambg_0_french/";
const char   SIDE[]	  = "x > 0";
const int    SIDE_NUM     = 1;

#endif
