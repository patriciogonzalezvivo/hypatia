
%module astro
%include "typemaps.i"

%apply double &OUTPUT { double &_alt, double &_az };
%apply double &OUTPUT { double &_ra, double &_dec };
%apply int &OUTPUT { int &_deg, int &_min, int &_sec};

%{
    #define SWIG_FILE_WITH_INIT
    #include "src/MathOps.h"
    #include "src/TimeOps.h"
    #include "src/ObsInf.h"
    #include "src/AstroOps.h"
    #include "src/PlanetData.h"
    #include "src/Lunar.h"
%}

%include "src/MathOps.h"
%include "src/TimeOps.h"
%include "src/ObsInf.h"
%include "src/AstroOps.h"
%include "src/PlanetData.h"
%include "src/Lunar.h"
