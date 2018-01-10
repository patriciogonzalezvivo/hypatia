#include "Constellation.h"

#include "AstroOps.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

/*
======================================================================
Ernie Wright  2 Mar 94
Find the constellation for a given position.
First C version by Craig Counterman and Elwood Downey.  Based on a
FORTRAN program by Nancy G. Roman (Roman, N.G. 1987, Publ. Astron.
Soc. Pacific 99, 695).  IAU constellation boundaries transcribed into
machine-readable form by Barry N. Rappaport, New Mexico State Univ.
======================================================================
*/

#define NCNS    89

#define NBOUNDS 357

/* constellation ids */
#define And  0
#define Ant  1
#define Aps  2
#define Aql  3
#define Aqr  4
#define Ara  5
#define Ari  6
#define Aur  7
#define Boo  8
#define CMa  9
#define CMi 10
#define CVn 11
#define Cae 12
#define Cam 13
#define Cap 14
#define Car 15
#define Cas 16
#define Cen 17
#define Cep 18
#define Cet 19
#define Cha 20
#define Cir 21
#define Cnc 22
#define Col 23
#define Com 24
#define CrA 25
#define CrB 26
#define Crt 27
#define Cru 28
#define Crv 29
#define Cyg 30
#define Del 31
#define Dor 32
#define Dra 33
#define Equ 34
#define Eri 35
#define For 36
#define Gem 37
#define Gru 38
#define Her 39
#define Hor 40
#define Hya 41
#define Hyi 42
#define Ind 43
#define LMi 44
#define Lac 45
#define Leo 46
#define Lep 47
#define Lib 48
#define Lup 49
#define Lyn 50
#define Lyr 51
#define Men 52
#define Mic 53
#define Mon 54
#define Mus 55
#define Nor 56
#define Oct 57
#define Oph 58
#define Ori 59
#define Pav 60
#define Peg 61
#define Per 62
#define Phe 63
#define Pic 64
#define PsA 65
#define Psc 66
#define Pup 67
#define Pyx 68
#define Ret 69
#define Scl 70
#define Sco 71
#define Sct 72
#define Se1 73
#define Sex 74
#define Sge 75
#define Sgr 76
#define Tau 77
#define Tel 78
#define TrA 79
#define Tri 80
#define Tuc 81
#define UMa 82
#define UMi 83
#define Vel 84
#define Vir 85
#define Vol 86
#define Vul 87
#define Se2 88

static std::string cns_namemap[ NCNS ] = {
   /*  0 */   "And: Andromeda",
   /*  1 */   "Ant: Antlia",
   /*  2 */   "Aps: Apus",
   /*  3 */   "Aql: Aquila",
   /*  4 */   "Aqr: Aquarius",
   /*  5 */   "Ara: Ara",
   /*  6 */   "Ari: Aries",
   /*  7 */   "Aur: Auriga",
   /*  8 */   "Boo: Bootes",
   /*  9 */   "CMa: Canis Major",
   /* 10 */   "CMi: Canis Minor",
   /* 11 */   "CVn: Canes Venatici",
   /* 12 */   "Cae: Caelum",
   /* 13 */   "Cam: Camelopardalis",
   /* 14 */   "Cap: Capricornus",
   /* 15 */   "Car: Carina",
   /* 16 */   "Cas: Cassiopeia",
   /* 17 */   "Cen: Centaurus",
   /* 18 */   "Cep: Cepheus",
   /* 19 */   "Cet: Cetus",
   /* 20 */   "Cha: Chamaeleon",
   /* 21 */   "Cir: Circinus",
   /* 22 */   "Cnc: Cancer",
   /* 23 */   "Col: Columba",
   /* 24 */   "Com: Coma Berenices",
   /* 25 */   "CrA: Corona Australis",
   /* 26 */   "CrB: Corona Borealis",
   /* 27 */   "Crt: Crater",
   /* 28 */   "Cru: Crux",
   /* 29 */   "Crv: Corvus",
   /* 30 */   "Cyg: Cygnus",
   /* 31 */   "Del: Delphinus",
   /* 32 */   "Dor: Dorado",
   /* 33 */   "Dra: Draco",
   /* 34 */   "Equ: Equuleus",
   /* 35 */   "Eri: Eridanus",
   /* 36 */   "For: Fornax",
   /* 37 */   "Gem: Gemini",
   /* 38 */   "Gru: Grus",
   /* 39 */   "Her: Hercules",
   /* 40 */   "Hor: Horologium",
   /* 41 */   "Hya: Hydra",
   /* 42 */   "Hyi: Hydrus",
   /* 43 */   "Ind: Indus",
   /* 44 */   "LMi: Leo Minor",
   /* 45 */   "Lac: Lacerta",
   /* 46 */   "Leo: Leo",
   /* 47 */   "Lep: Lepus",
   /* 48 */   "Lib: Libra",
   /* 49 */   "Lup: Lupus",
   /* 50 */   "Lyn: Lynx",
   /* 51 */   "Lyr: Lyra",
   /* 52 */   "Men: Mensa",
   /* 53 */   "Mic: Microscopium",
   /* 54 */   "Mon: Monoceros",
   /* 55 */   "Mus: Musca",
   /* 56 */   "Nor: Norma",
   /* 57 */   "Oct: Octans",
   /* 58 */   "Oph: Ophiuchus",
   /* 59 */   "Ori: Orion",
   /* 60 */   "Pav: Pavo",
   /* 61 */   "Peg: Pegasus",
   /* 62 */   "Per: Perseus",
   /* 63 */   "Phe: Phoenix",
   /* 64 */   "Pic: Pictor",
   /* 65 */   "PsA: Piscis Austrinus",
   /* 66 */   "Psc: Pisces",
   /* 67 */   "Pup: Puppis",
   /* 68 */   "Pyx: Pyxis",
   /* 69 */   "Ret: Reticulum",
   /* 70 */   "Scl: Sculptor",
   /* 71 */   "Sco: Scorpius",
   /* 72 */   "Sct: Scutum",
   /* 73 */   "Se1: Serpens Caput",
   /* 74 */   "Sex: Sextans",
   /* 75 */   "Sge: Sagitta",
   /* 76 */   "Sgr: Sagittarius",
   /* 77 */   "Tau: Taurus",
   /* 78 */   "Tel: Telescopium",
   /* 79 */   "TrA: Triangulum Australe",
   /* 80 */   "Tri: Triangulum",
   /* 81 */   "Tuc: Tucana",
   /* 82 */   "UMa: Ursa Major",
   /* 83 */   "UMi: Ursa Minor",
   /* 84 */   "Vel: Vela",
   /* 85 */   "Vir: Virgo",
   /* 86 */   "Vol: Volans",
   /* 87 */   "Vul: Vulpecula",
   /* 88 */   "Se2: Serpens Cauda",
};

static struct {
   unsigned short lower_ra;      /* hours * 1800 */
   unsigned short upper_ra;      /* hours * 1800 */
   short          lower_dec;     /* degrees * 60 */
   short          index;
} cbound[ NBOUNDS ] = {
   {     0, 43200,  5280, UMi },
   { 14400, 26100,  5190, UMi },
   { 37800, 41400,  5170, UMi },
   { 32400, 37800,  5160, UMi },
   {     0, 14400,  5100, Cep },
   { 16500, 19200,  4920, Cam },
   {     0,  9000,  4800, Cep },
   { 19200, 26100,  4800, Cam },
   { 31500, 32400,  4800, UMi },
   { 36300, 37800,  4800, Dra },
   {     0,  6315,  4620, Cep },
   { 20700, 24450,  4620, Cam },
   { 29760, 31500,  4500, UMi },
   { 36300, 37200,  4500, Cep },
   { 14340, 16500,  4410, Cam },
   { 16500, 20400,  4410, Dra },
   { 23400, 29760,  4200, UMi },
   {  5580,  6150,  4080, Cas },
   { 36750, 37200,  4020, Dra },
   { 20400, 21600,  3990, Dra },
   {     0,   600,  3960, Cep },
   { 25200, 28200,  3960, UMi },
   { 42450, 43200,  3960, Cep },
   { 21600, 24300,  3840, Dra },
   { 24300, 25950,  3780, Dra },
   { 41700, 42450,  3780, Cep },
   { 10980, 12600,  3720, Cam },
   { 36000, 36750,  3690, Dra },
   { 36966, 37080,  3655, Cep },
   { 12600, 14340,  3600, Cam },
   { 14340, 15150,  3600, UMa },
   { 35580, 36000,  3570, Dra },
   { 36000, 36966,  3570, Cep },
   { 41160, 41700,  3545, Cep },
   {     0,  4380,  3510, Cas },
   { 34950, 35580,  3480, Dra },
   {  3060,  3435,  3450, Cas },
   {  4380,  5580,  3420, Cas },
   {  5580,  5700,  3420, Cam },
   { 40170, 41160,  3375, Cep },
   {  9000, 10980,  3360, Cam },
   { 25260, 25950,  3330, UMa },
   { 25950, 34950,  3330, Dra },
   {  5700,  6000,  3300, Cam },
   { 39840, 40170,  3300, Cep },
   { 37080, 39540,  3290, Cep },
   {     0,  3060,  3240, Cas },
   { 10980, 11700,  3240, Lyn },
   { 21750, 24300,  3180, UMa },
   { 27450, 28350,  3180, Dra },
   { 39540, 39840,  3165, Cep },
   {  6000,  9000,  3150, Cam },
   { 41160, 42000,  3150, Cas },
   { 28350, 30600,  3090, Dra },
   {  3675,  4530,  3030, Per },
   { 30600, 32820,  3030, Dra },
   {     0,  2460,  3000, Cas },
   {  2460,  3000,  3000, Per },
   { 11700, 12240,  3000, Lyn },
   { 42000, 43200,  3000, Cas },
   { 24300, 25260,  2910, UMa },
   {     0,  2010,  2880, Cas },
   { 42450, 43200,  2880, Cas },
   { 32715, 32820,  2850, Her },
   { 32820, 34350,  2850, Dra },
   { 34350, 34500,  2850, Cyg },
   {  3000,  3675,  2820, Per },
   { 15150, 16500,  2820, UMa },
   {   300,  1560,  2760, Cas },
   { 21600, 21750,  2700, UMa },
   { 12240, 13260,  2670, Lyn },
   { 39435, 39540,  2640, Cyg },
   { 39375, 39435,  2625, Cyg },
   { 34500, 34920,  2610, Cyg },
   { 16500, 18300,  2520, UMa },
   { 18300, 19410,  2400, UMa },
   { 27780, 28350,  2400, Boo },
   { 28350, 29400,  2400, Her },
   { 16650, 17250,  2385, Lyn },
   {     0,  4530,  2205, And },
   {  4530,  4620,  2205, Per },
   { 34845, 34920,  2190, Lyr },
   {  8100,  8445,  2160, Per },
   { 39120, 39375,  2160, Cyg },
   { 39375, 39600,  2160, Lac },
   { 11760, 13260,  2130, Aur },
   { 13260, 13950,  2130, Lyn },
   {     0,  3600,  2100, And },
   { 39600, 41070,  2100, Lac },
   { 41070, 41160,  2070, Lac },
   { 41160, 42300,  2070, And },
   {  4620,  4890,  2040, Per },
   { 19410, 19800,  2040, UMa },
   { 21600, 22200,  2040, CVn },
   { 13950, 16650,  2010, Lyn },
   { 16650, 17790,  2010, LMi },
   {  1290,  2535,  1980, And },
   { 27330, 27780,  1980, Boo },
   { 42300, 42750,  1925, And },
   { 22200, 23850,  1920, CVn },
   { 42750, 43200,  1880, And },
   { 25125, 25260,  1845, CVn },
   {  4350,  4890,  1840, Tri },
   {  4890,  8100,  1840, Per },
   {  8100,  8550,  1800, Aur },
   { 32715, 34845,  1800, Lyr },
   { 19800, 21600,  1740, UMa },
   { 35400, 37650,  1740, Cyg },
   {  8550, 10590,  1710, Aur },
   { 17790, 18900,  1710, LMi },
   { 23850, 25125,  1710, CVn },
   {     0,   120,  1680, And },
   {  2535,  3000,  1680, Tri },
   { 10590, 11760,  1680, Aur },
   { 14190, 14400,  1680, Gem },
   { 37650, 39120,  1680, Cyg },
   { 34665, 35400,  1650, Cyg },
   {  3450,  4350,  1635, Tri },
   { 29100, 29400,  1620, CrB },
   { 27150, 27330,  1560, Boo },
   { 27330, 29100,  1560, CrB },
   { 33060, 33960,  1560, Lyr },
   { 19350, 19800,  1530, LMi },
   { 33960, 34665,  1530, Lyr },
   {  3000,  3450,  1500, Tri },
   {  1290,  1530,  1425, Psc },
   { 18900, 19350,  1410, LMi },
   { 38250, 38550,  1410, Vul },
   { 10260, 10590,  1370, Tau },
   {   120,   255,  1320, And },
   { 28650, 28860,  1320, Se1 },
   { 10590, 11190,  1290, Gem },
   { 35700, 36450,  1275, Vul },
   { 33960, 34650,  1265, Vul },
   {   255,  1530,  1260, And },
   { 36450, 37020,  1230, Vul },
   { 14055, 14190,  1200, Gem },
   { 37020, 38250,  1170, Vul },
   { 34650, 35700,  1150, Vul },
   {  5910,  6060,  1140, Ari },
   { 33960, 34200,  1110, Sge },
   { 10260, 10380,  1080, Ori },
   { 11190, 11355,  1050, Gem },
   { 34200, 35700,   970, Sge },
   {  8940,  9600,   960, Tau },
   { 28650, 28950,   960, Her },
   { 35700, 36450,   945, Sge },
   {  8310,  8940,   930, Tau },
   {  9600, 10080,   930, Tau },
   { 23100, 24300,   900, Com },
   { 31050, 32850,   860, Her },
   { 21360, 23100,   840, Com },
   { 13500, 14055,   810, Gem },
   { 30150, 31050,   770, Her },
   {     0,   255,   750, Peg },
   { 10080, 10380,   750, Tau },
   { 12600, 13500,   750, Gem },
   { 38010, 38400,   750, Peg },
   { 11355, 12480,   720, Gem },
   { 32850, 33960,   720, Her },
   { 37575, 37890,   710, Del },
   { 37890, 38010,   710, Peg },
   { 20730, 21360,   660, Leo },
   { 11235, 11355,   600, Ori },
   { 12480, 12600,   600, Gem },
   { 14055, 14265,   600, Cnc },
   { 42900, 43200,   600, Peg },
   {  3000,  5910,   595, Ari },
   { 36255, 36540,   510, Del },
   { 24300, 27150,   480, Boo },
   { 40950, 42900,   450, Peg },
   { 14265, 16650,   420, Cnc },
   { 16650, 19350,   420, Leo },
   { 32850, 33592,   375, Oph },
   { 33592, 33960,   375, Aql },
   { 37500, 37575,   360, Del },
   { 12600, 12630,   330, CMi },
   { 32850, 33165,   270, Se2 },
   { 28950, 30150,   240, Her },
   { 32850, 33165,   180, Oph },
   { 38640, 39000,   165, Peg },
   {     0,  3600,   120, Psc },
   { 33450, 33960,   120, Se2 },
   { 36540, 37500,   120, Del },
   { 37500, 38400,   120, Equ },
   { 38400, 38640,   120, Peg },
   { 39600, 40950,   120, Peg },
   { 39000, 39600,   105, Peg },
   { 12630, 12960,    90, CMi },
   {  6450,  8310,     0, Tau },
   {  8310,  8400,     0, Ori },
   { 12960, 14550,     0, CMi },
   { 26400, 27150,     0, Vir },
   { 32100, 32850,     0, Oph },
   {  4770,  5910,  -105, Cet },
   {  5910,  6450,  -105, Tau },
   { 27150, 29280,  -195, Se1 },
   {  8400,  9150,  -240, Ori },
   { 10500, 11235,  -240, Ori },
   { 32100, 32340,  -240, Se2 },
   { 32850, 33450,  -240, Se2 },
   { 33450, 33960,  -240, Aql },
   { 40950, 42900,  -240, Psc },
   { 19350, 20730,  -360, Leo },
   { 20730, 21300,  -360, Vir },
   {     0,   600,  -420, Psc },
   { 42900, 43200,  -420, Psc },
   { 25650, 26400,  -480, Vir },
   { 28650, 29280,  -480, Oph },
   { 36000, 36960,  -540, Aql },
   { 38400, 39360,  -540, Aqr },
   { 30900, 32340,  -600, Oph },
   { 10500, 14550,  -660, Mon },
   {  8850,  9150,  -660, Eri },
   {  9150, 10500,  -660, Ori },
   { 14550, 15060,  -660, Hya },
   { 17250, 19350,  -660, Sex },
   { 21300, 23100,  -660, Vir },
   { 31650, 31800,  -700, Oph },
   { 33960, 36000,  -722, Aql },
   {  8700,  8850,  -870, Eri },
   { 36960, 38400,  -900, Aqr },
   { 30900, 32850,  -960, Se2 },
   { 32850, 33960,  -960, Sct },
   { 15060, 15450, -1020, Hya },
   { 29280, 29475, -1095, Oph },
   { 15450, 16350, -1140, Hya },
   { 19350, 19500, -1140, Crt },
   { 29280, 29475, -1155, Sco },
   { 28200, 28650, -1200, Lib },
   { 22650, 23100, -1320, Crv },
   { 23100, 25650, -1320, Vir },
   { 16350, 17550, -1440, Hya },
   {  3000,  4770, -1463, Cet },
   {  4770,  6750, -1463, Eri },
   { 19500, 21300, -1470, Crt },
   { 21300, 22650, -1470, Crv },
   { 25650, 26850, -1470, Lib },
   { 29280, 30150, -1475, Oph },
   {     0,  3000, -1530, Cet },
   { 38400, 39360, -1530, Cap },
   { 39360, 42900, -1530, Aqr },
   { 42900, 43200, -1530, Cet },
   { 17550, 18450, -1590, Hya },
   {  8460,  8700, -1635, Eri },
   {  8700, 11010, -1635, Lep },
   { 36000, 38400, -1680, Cap },
   { 18450, 19050, -1750, Hya },
   { 22650, 26850, -1770, Hya },
   { 26850, 28200, -1770, Lib },
   { 28200, 28800, -1770, Sco },
   {  8250,  8460, -1800, Eri },
   { 30150, 31680, -1800, Oph },
   { 31680, 32100, -1800, Sgr },
   { 19050, 19500, -1870, Hya },
   { 11010, 13260, -1980, CMa },
   { 22050, 22650, -1980, Hya },
   { 19500, 22050, -2100, Hya },
   {  6300,  6750, -2160, For },
   { 15060, 16860, -2205, Pyx },
   {  7680,  8250, -2220, Eri },
   { 32100, 34500, -2220, Sgr },
   { 38400, 41400, -2220, PsA },
   { 41400, 42000, -2220, Scl },
   {  5400,  6300, -2375, For },
   { 16860, 19800, -2385, Ant },
   {     0,  3000, -2400, Scl },
   {  3000,  5400, -2400, For },
   {  6960,  7680, -2400, Eri },
   { 42000, 43200, -2400, Scl },
   { 25500, 26850, -2520, Cen },
   { 28200, 28800, -2520, Lup },
   { 28800, 29557, -2520, Sco },
   {  8700,  9000, -2580, Cae },
   {  9000, 11850, -2580, Col },
   { 14400, 15060, -2580, Pup },
   {  6150,  6960, -2640, Eri },
   { 29557, 32100, -2730, Sco },
   { 32100, 34500, -2730, CrA },
   { 34500, 36600, -2730, Sgr },
   { 36600, 38400, -2730, Mic },
   {  5400,  6150, -2760, Eri },
   {  8100,  8700, -2790, Cae },
   { 27600, 28200, -2880, Lup },
   {     0,  4200, -2890, Phe },
   {  4800,  5400, -2940, Eri },
   {  7350,  7680, -2940, Hor },
   {  7680,  8100, -2940, Cae },
   { 38400, 39600, -3000, Gru },
   { 10800, 14400, -3045, Pup },
   { 14400, 14700, -3045, Vel },
   {  4350,  4800, -3060, Eri },
   {  6900,  7350, -3060, Hor },
   {     0,  3300, -3090, Phe },
   { 10800, 11100, -3150, Car },
   { 14700, 15210, -3180, Vel },
   {  6300,  6900, -3190, Hor },
   {  6900,  7200, -3190, Dor },
   {     0,  2850, -3210, Phe },
   {  3900,  4350, -3240, Eri },
   {  8100,  9000, -3240, Pic },
   { 27090, 27600, -3240, Lup },
   { 15210, 15900, -3270, Vel },
   { 11100, 11700, -3300, Car },
   { 21300, 23100, -3300, Cen },
   { 25500, 27090, -3300, Lup },
   { 27090, 27600, -3300, Nor },
   {  7200,  7800, -3390, Dor },
   { 15900, 19800, -3390, Vel },
   { 19800, 20250, -3390, Cen },
   { 31500, 32400, -3420, Ara },
   { 32400, 36600, -3420, Tel },
   { 39600, 42000, -3420, Gru },
   {  5760,  6300, -3450, Hor },
   {  9000,  9900, -3450, Pic },
   { 11700, 12300, -3480, Car },
   {     0,  2400, -3510, Phe },
   {  2400,  3900, -3510, Eri },
   { 42000, 43200, -3510, Phe },
   {  7800,  8250, -3540, Dor },
   { 27600, 29557, -3600, Nor },
   { 36600, 38400, -3600, Ind },
   {  9900, 10800, -3660, Pic },
   { 27300, 27600, -3660, Cir },
   { 29557, 29850, -3660, Ara },
   { 26850, 27300, -3815, Cir },
   { 29850, 30150, -3815, Ara },
   { 10800, 12300, -3840, Pic },
   { 12300, 16260, -3840, Car },
   { 20250, 21300, -3840, Cen },
   { 21300, 23100, -3840, Cru },
   { 23100, 26160, -3840, Cen },
   { 24300, 24600, -3900, Cir },
   { 30150, 30300, -3900, Ara },
   {  3900,  5760, -4050, Hor },
   {  5760,  8250, -4050, Ret },
   { 26550, 26850, -4050, Cir },
   { 30300, 31500, -4050, Ara },
   { 31500, 32400, -4050, Pav },
   { 39600, 42000, -4050, Tuc },
   {  8250, 11850, -4200, Dor },
   { 24600, 26550, -4200, Cir },
   { 26550, 30600, -4200, TrA },
   {     0,  2400, -4500, Tuc },
   {  6300,  8250, -4500, Hyi },
   { 11850, 16260, -4500, Vol },
   { 16260, 20250, -4500, Car },
   { 20250, 24600, -4500, Mus },
   { 32400, 38400, -4500, Pav },
   { 38400, 42000, -4500, Ind },
   { 42000, 43200, -4500, Tuc },
   {  1350,  2400, -4560, Tuc },
   {     0,  6300, -4950, Hyi },
   { 13800, 24600, -4950, Cha },
   { 24600, 32400, -4950, Aps },
   {  6300, 13800, -5100, Men },
   {     0, 43200, -5400, Oct }
};

static short start[] = {
   355, 352, 343, 340, 332, 320,
   303, 288, 277, 266, 257, 251,
   239, 229, 221, 211, 203, 189,
   177, 163, 149, 136, 124, 104,
    87,  75,  69,  54,  43,  29,
    23,  16,  12,   6,   4,   0,   0
};


/*
======================================================================
constellation_pick()
Do a constellation pick from RA and Dec.
INPUTS
   r        right ascension, radians
   d        declination, radians
   e        epoch to which r and d precessed, as an mjd
RESULTS
   Returns an index for the constellation region that the coordinates
   belong to, or -1 if no constellation pick can be found.
The constellation is identified by linear search.  We look for a
member of cbound[] whose lower_dec is less than the declination of the
pick point and whose lower_ra and upper_ra bracket the pick point's
right ascension.  The data for that cbound[] member describes a sub-
region belonging to the picked constellation.
In geometric terms, the search amounts to starting at the north pole
and traveling south on a line of constant right ascension through the
pick point.  At every declination where a cbound[] member lives, we
wake up, and if (1) the pick point is now behind us, and (2) the
cbound[] segment is crossing our path, we know we've hit the edge of
a constellation subregion that the pick point belongs to.
The cbound[] right ascension and declination values are scaled into
integers; this conserves storage and makes comparisons faster.  The
start[] array, which gives starting points in cbound[] for different
declinations in 5-degree increments, further speeds the search by
skipping early parts of the list for which we know the search can't
succeed--geometrically, we start no more than 5 degrees north of the
pick point, rather than at the north pole.
The data in cbound[] are for epoch 1875.
======================================================================
*/

int cns_pick(double r, double d, double e) {
    unsigned short ra;
    short de, i;

    // double Mjd = TimeOps::MDYtoMJD( 1, 1.0, 1875);
    // AstroOps::precession( e, Mjd, r, d );
    ra = ( unsigned short )( MathOps::toHrs( r ) * 1800 );
    de = ( short )( MathOps::toDegrees( d ) * 60 );
    if (d < 0.0) {
        --de;
    }
    i = ( de + 5400 ) / 300;
    if ( i < 0 || i > 36 ) {
        return -1;
    }
    i = start[ i ];

    for ( ; i < NBOUNDS; i++ )
        if (cbound[ i ].lower_dec <= de &&
            cbound[ i ].upper_ra   > ra &&
            cbound[ i ].lower_ra  <= ra ) break;

    return ( i == NBOUNDS ) ? -1 : ( int ) cbound[ i ].index;
}

/* given a constellation id (as from cns_pick()), return pointer to static
 * storage containg its name in the form "AAA: Name".
 * return "???: ???" if id is invalid.
 */
std::string cns_name (int id) {
    if (id < 0 || id >= NCNS)
        return ("???: ???");
    return (cns_namemap[id]);
}

// /* return cns_namemap index matching first three chars in abbrev[], else -1.
//  */
// int cns_id (char *abbrev) {
//     int i;

//     for (i = 0; i < NCNS; i++)
//         if (strncmp (abbrev, cns_namemap[i], 3) == 0)
//         return (i);
//     return (-1);
// }

Constellation::Constellation() : m_id(-1) {

}

Constellation::Constellation( int _id ) : m_id(_id) {

};

Constellation::Constellation( Body &_body, double _epoch ) {
    double ra = _body.getRightAscensionRadians();
    double dec = _body.getDeclinationRadians(); 
    m_id = cns_pick(ra, dec, _epoch);
}

Constellation::Constellation( Luna &_luna, double _epoch ) {
    double ra = _luna.getRightAscensionRadians();
    double dec = _luna.getDeclinationRadians(); 
    m_id = cns_pick(ra, dec, _epoch);
}

Constellation::Constellation( double _ra, double _dec, double _epoch ) {
    m_id = cns_pick(_ra, _dec, _epoch);
}

std::string Constellation::getName() {
    if (m_id < 0 || m_id >= NCNS)
        return "???";
    return cns_namemap[m_id].substr(5);
}

std::string Constellation::getAbbreviation() {
    if (m_id < 0 || m_id >= NCNS)
        return "???";
    return cns_namemap[m_id].substr(0,3);
}
