/*****************************************************************************\
 * Pluto.cpp
 *
 * Pluto is a class that can calculate the orbit of Pluto.
 *
 * author: mark huss (mark@mhuss.com)
 * Based on Bill Gray's open-source code at projectpluto.com
 *
\*****************************************************************************/

#include "../MathOps.h"
#include "../AstroOps.h"
#include "Pluto.h"

#include <math.h>

// long coefficients

long plutoLongCoeff[7][6] = {
    {  68955876,  66867334, -14974876,  -5453098,  19848454, -19798886 },
    {   -333765, -11826086,   1672673,   3527363,  -4955707,    897499 },
    {  -1439953,   1593657,    327763,  -1050939,   1210521,    610820 },
    {    482443,    -18948,   -291925,    178691,   -189719,   -341639 },
    {    -85576,    -66634,    100448,     18763,    -34863,    129027 },
    {     -5765,     30841,    -25838,    -30594,     31061,    -38215 },
    {     45637,       105,       201,       157,       747,      7049 }
};

// pluto coefficients

PlutoCoeffs plutoCoeff[N_COEFFS] = {
    {  0,  1, -1, 0, 20349, -9886,  4965, 11263, -6140, 22254 },
    {  0,  1,  0, 0, -4045, -4904,   310,  -132,  4434,  4443 },
    {  0,  1,  1, 0, -5885, -3238,  2036,  -947, -1518,   641 },
    {  0,  1,  2, 0, -3812,  3011,    -2,  -674,    -5,   792 },
    {  0,  1,  3, 0,  -601,  3468,  -329,  -563,   518,   518 },
    {  0,  2, -2, 0,  1237,   463,   -64,    39,   -13,  -221 },
    {  0,  2, -1, 0,  1086,  -911,   -94,   210,   837,  -494 },
    {  0,  2,  0, 0,   595, -1229,    -8,  -160,  -281,   616 },
    {  1, -1,  0, 0,  2484,  -485,  -177,   259,   260,  -395 },
    {  1, -1,  1, 0,   839, -1414,    17,   234,  -191,  -396 },
    {  1,  0, -3, 0,  -964,  1059,   582,  -285, -3218,   370 },
    {  1,  0, -2, 0, -2303, -1038,  -298,   692,  8019, -7869 },
    {  1,  0,  0, 0,  1179,  -358,   304,   825,  8623,  8444 },
    {  1,  0,  1, 0,   393,   -63,  -124,   -29,  -896,  -801 },
    {  1,  0,  2, 0,   111,  -268,    15,     8,   208,  -122 },
    {  1,  0,  3, 0,   -52,  -154,     7,    15,  -133,    65 },
    {  1,  0,  4, 0,   -78,   -30,     2,     2,   -16,     1 },
    {  1,  1, -3, 0,   -34,   -26,     4,     2,   -22,     7 },
    {  1,  1, -2, 0,   -43,     1,     3,     0,    -8,    16 },
    {  1,  1, -1, 0,   -15,    21,     1,    -1,     2,     9 },
    {  1,  1,  0, 0,    -1,    15,     0,    -2,    12,     5 },
    {  1,  1,  1, 0,     4,     7,     1,     0,     1,    -3 },
    {  1,  1,  3, 0,     1,     5,     1,    -1,     1,     0 },
    {  2,  0, -6, 0,     8,     3,    -2,    -3,     9,     5 },
    {  2,  0, -5, 0,    -3,     6,     1,     2,     2,    -1 },
    {  2,  0, -4, 0,     6,   -13,    -8,     2,    14,    10 },
    {  2,  0, -3, 0,    10,    22,    10,    -7,   -65,    12 },
    {  2,  0, -2, 0,   -57,   -32,     0,    21,   126,  -233 },
    {  2,  0, -1, 0,   157,   -46,     8,     5,   270,  1068 },
    {  2,  0,  0, 0,    12,   -18,    13,    16,   254,   155 },
    {  2,  0,  1, 0,    -4,     8,    -2,    -3,   -26,    -2 },
    {  2,  0,  2, 0,    -5,     0,     0,     0,     7,     0 },
    {  2,  0,  3, 0,     3,     4,     0,     1,   -11,     4 },
    {  3,  0, -2, 0,    -1,    -1,     0,     1,     4,   -14 },
    {  3,  0, -1, 0,     6,    -3,     0,     0,    18,    35 },
    {  3,  0,  0, 0,    -1,    -2,     0,     1,    13,     3 }
};

// t is in julian centuries from J2000.0
void Pluto::calcAllLocs (double& lon, double& lat, double& rad, const double t) {

    // jupiter's mean longitude
    double mlJup =  MathOps::toRadians(34.35 + 3034.9057 * t);

    // saturn's mean longitude
    double mlSat =  MathOps::toRadians(50.08 + 1222.1138 * t);

    // pluto's mean longitude
    double mlPl = MathOps::toRadians(238.96 +  144.9600 * t);

    // use local vars for retvals, hoping to encourage FP reg optimizations
    double lon_ = 238.956785 + 144.96 * t;
    double lat_ = -3.908202;
    double rad_ = 407.247248;  // temporarily in tenths of AUs; fixed at the end

    double arg;
    for (int i=0; i<7; i++ ) {
        if (i == 6)
            arg = mlJup - mlPl;
        else
            arg = (double)(i + 1) * mlPl;

        double cosArg = cos(arg) * 1.e-6;
        double sinArg = sin(arg) * 1.e-6;
        long* plc = plutoLongCoeff[i];

        lon_ += (double)(plc[0]) * sinArg + (double)(plc[1]) * cosArg;
        lat_ += (double)(plc[2]) * sinArg + (double)(plc[3]) * cosArg;
        rad_   += (double)(plc[4]) * sinArg + (double)(plc[5]) * cosArg;
    }

    PlutoCoeffs* pc = plutoCoeff;
    for (int i=0; i<N_COEFFS; i++ ) {
        if (pc->lon_a || pc->lon_b ||
            pc->lat_a || pc->lat_b ||
            pc->rad_a || pc->rad_b) {
            if( 0 == pc->j )
                arg = 0.;
            else
                arg = ((1 == pc->j) ? mlJup : mlJup * (double)pc->j);

            if( pc->s < 0)
                arg -= (-1 == pc->s) ? mlSat : mlSat + mlSat;

            if( pc->s > 0)
                arg += (1 == pc->s) ? mlSat : mlSat + mlSat;

            if( pc->p)
                arg += mlPl * (double)pc->p;

            double cosArg = cos(arg) * 1.e-6;
            double sinArg = sin(arg) * 1.e-6;
            lon_ += sinArg * (double)(pc->lon_a) + cosArg * (double)(pc->lon_b);
            lat_ += sinArg * (double)(pc->lat_a) + cosArg * (double)(pc->lat_b);
            rad_ += sinArg * (double)(pc->rad_a) + cosArg * (double)(pc->rad_b);
        }
        pc++;
    }
    lon = MathOps::toRadians(lon_);
    lat = MathOps::toRadians(lat_);
    rad = rad_ / 10.;                  // convert back to AUs
}
