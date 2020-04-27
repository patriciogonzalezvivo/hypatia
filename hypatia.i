
%module hypatia
%include "typemaps.i"
%include <std_vector.i>
%include <std_string.i>

%ignore *::operator[];
%ignore *::operator=;
%ignore *::operator==;
%ignore *::operator!=;
%ignore *::operator<;
%ignore *::operator<=;
%ignore *::operator>;
%ignore *::operator>=;
%ignore operator<<;

%apply double &OUTPUT { double &_x, double &_y };
%apply double &OUTPUT { double &_lng, double &_lat };

%apply double &OUTPUT { int &_deg, int &_min, double &_sec };
%apply double &OUTPUT { int &_hrs, int &_min, double &_sec };

%apply double &OUTPUT { int &_year, int &_month, double &_day };
%apply int &OUTPUT { int &_day, int &_month, int &_year };
%apply int &OUTPUT { int &_hrs, int &_min, int &_sec };

%{
    #define SWIG_FILE_WITH_INIT
    
    #include "include/hypatia/primitives/Polar.h"
    #include "include/hypatia/primitives/Vector2.h"
    #include "include/hypatia/primitives/Vector3.h"
    #include "include/hypatia/primitives/TimeSpan.h"
    #include "include/hypatia/primitives/DateTime.h"
    #include "include/hypatia/primitives/BoundingBox.h"
    #include "include/hypatia/coordinates/Galactic.h"
    #include "include/hypatia/coordinates/Ecliptic.h"
    #include "include/hypatia/coordinates/Equatorial.h"
    #include "include/hypatia/coordinates/Horizontal.h"
    #include "include/hypatia/coordinates/Geodetic.h"
    #include "include/hypatia/coordinates/ECI.h"
    #include "include/hypatia/coordinates/Tile.h"
    #include "include/hypatia/MathOps.h"
    #include "include/hypatia/TimeOps.h"
    #include "include/hypatia/CoordOps.h"
    #include "include/hypatia/ProjOps.h"
    #include "include/hypatia/Observer.h"
    #include "include/hypatia/Body.h"
    #include "include/hypatia/Luna.h"
    #include "include/hypatia/Star.h"
    #include "include/hypatia/Constellation.h"
    #include "include/hypatia/Satellite.h"
    #include "include/hypatia/models/TLE.h"
%}

%include "include/hypatia/primitives/Polar.h"
%include "include/hypatia/primitives/Vector2.h"
%include "include/hypatia/primitives/Vector3.h"
%include "include/hypatia/primitives/TimeSpan.h"
%include "include/hypatia/primitives/DateTime.h"
%include "include/hypatia/primitives/BoundingBox.h"
%include "include/hypatia/coordinates/Galactic.h"
%include "include/hypatia/coordinates/Ecliptic.h"
%include "include/hypatia/coordinates/Equatorial.h"
%include "include/hypatia/coordinates/Horizontal.h"
%include "include/hypatia/coordinates/Geodetic.h"
%include "include/hypatia/coordinates/ECI.h"
%include "include/hypatia/coordinates/Tile.h"
%include "include/hypatia/MathOps.h"
%include "include/hypatia/TimeOps.h"
%include "include/hypatia/CoordOps.h"
%include "include/hypatia/ProjOps.h"
%include "include/hypatia/Observer.h"
%include "include/hypatia/Body.h"
%include "include/hypatia/Luna.h"
%include "include/hypatia/Star.h"
%include "include/hypatia/Constellation.h"
%include "include/hypatia/Satellite.h"
%include "include/hypatia/models/TLE.h"

namespace std {
    %template(VectorInt) vector<int>;
    %template(VectorEquatorial) vector<Equatorial>;
};
