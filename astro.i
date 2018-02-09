
%module astro
%include "typemaps.i"
%include <std_vector.i>

%ignore *::operator[];
%ignore *::operator=;
%ignore operator<<;

%apply double &OUTPUT { double &_alt, double &_az };
%apply double &OUTPUT { double &_ra, double &_dec };
%apply double &OUTPUT { double &_x, double &_y };

%apply double &OUTPUT { int &_deg, int &_min, double &_sec };
%apply double &OUTPUT { int &_hrs, int &_min, double &_sec };

%apply double &OUTPUT { int &_year, int &_month, double &_day };
%apply int &OUTPUT { int &_day, int &_month, int &_year };
%apply int &OUTPUT { int &_hrs, int &_min, int &_sec };

namespace std {
    %template(VectorInt) vector<int>;
};

%{
    #define SWIG_FILE_WITH_INIT
    #include "src/MathOps.h"
    #include "src/TimeOps.h"
    #include "src/Vector.h"
    #include "src/GeoPoint.h"
    #include "src/Observer.h"
    #include "src/AstroOps.h"
    #include "src/HorPoint.h"
    #include "src/EqPoint.h"
    #include "src/EcPoint.h"
    #include "src/ProjOps.h"
    #include "src/Body.h"
    #include "src/Luna.h"
    #include "src/Constellation.h"
    #include "src/Star.h"
%}

%include "src/MathOps.h"
%include "src/TimeOps.h"
%include "src/Vector.h"
%include "src/GeoPoint.h"
%include "src/Observer.h"
%include "src/AstroOps.h"
%include "src/HorPoint.h"
%include "src/EqPoint.h"
%include "src/EcPoint.h"
%include "src/ProjOps.h"
%include "src/Body.h"
%include "src/Luna.h"
%include "src/Constellation.h"
%include "src/Star.h"
