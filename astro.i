
%module astro
%include "typemaps.i"
%include <std_string.i>
%include <std_vector.i>

%apply double &OUTPUT { double &_alt, double &_az };
%apply double &OUTPUT { double &_ra, double &_dec };

%apply int &OUTPUT { int &_deg, int &_min, double &_sec };
%apply int &OUTPUT { int &_hrs, int &_min, double &_sec };

%apply double &OUTPUT { int &_mont, double &_day, int &_year }

namespace std {
    %template(VectorInt) vector<int>;
};

%{
    #define SWIG_FILE_WITH_INIT
    #include "src/MathOps.h"
    #include "src/TimeOps.h"
    #include "src/Vector.h"
    #include "src/Observer.h"
    #include "src/AstroOps.h"
    #include "src/EqPoint.h"
    #include "src/Body.h"
    #include "src/Luna.h"
    #include "src/Constellation.h"
    #include "src/Star.h"
%}

%include "src/MathOps.h"
%include "src/TimeOps.h"
%include "src/Vector.h"
%include "src/Observer.h"
%include "src/AstroOps.h"
%include "src/EqPoint.h"
%include "src/Body.h"
%include "src/Luna.h"
%include "src/Constellation.h"
%include "src/Star.h"