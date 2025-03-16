
%begin %{
#if defined( __WIN32__ ) || defined( _WIN32 )
	#include <cmath>
#endif
%}

%module hypatia

%include <typemaps.i>
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

%{
    #define SWIG_FILE_WITH_INIT
    #include "hypatia/MathOps.h"
    #include "hypatia/primitives/Polar.h"
    #include "hypatia/primitives/Vector2.h"
    #include "hypatia/primitives/Vector3.h"
    #include "hypatia/primitives/Matrix3x3.h"
    #include "hypatia/primitives/TimeSpan.h"
    #include "hypatia/primitives/DateTime.h"
    #include "hypatia/coordinates/Equatorial.h"
    #include "hypatia/coordinates/PrecessionMatrix.h"
    #include "hypatia/coordinates/Galactic.h"
    #include "hypatia/coordinates/Ecliptic.h"
    #include "hypatia/coordinates/Horizontal.h"
    #include "hypatia/coordinates/Geodetic.h"
    #include "hypatia/coordinates/ECI.h"
    #include "hypatia/coordinates/Tile.h"
    #include "hypatia/coordinates/UTM.h"
    #include "hypatia/TimeOps.h"
    #include "hypatia/GeoOps.h"
    #include "hypatia/Observer.h"
    #include "hypatia/CoordOps.h"
    #include "hypatia/ProjOps.h"
    #include "hypatia/Body.h"
    #include "hypatia/Luna.h"
    #include "hypatia/Star.h"
    #include "hypatia/Constellation.h"
    #include "hypatia/Satellite.h"
    #include "hypatia/models/TLE.h"
%}

%include "numpy.i"
%init %{
    import_array();
%}

%apply double &OUTPUT { double &_x, double &_y };
%apply double &OUTPUT { double &_lng, double &_lat };

%apply double &OUTPUT { int &_deg, int &_min, double &_sec };
%apply double &OUTPUT { int &_hrs, int &_min, double &_sec };

%apply double &OUTPUT { int &_year, int &_month, double &_day };
%apply int &OUTPUT { int &_day, int &_month, int &_year };
%apply int &OUTPUT { int &_hrs, int &_min, int &_sec };

namespace std {
    %template(IntVector) vector<int>;
    %template(EquatorialVector) vector<Equatorial>;
    %template(TileList) vector<Tile>;
};

%include "include/hypatia/MathOps.h"
%include "include/hypatia/primitives/Polar.h"
%include "include/hypatia/primitives/Vector2.h"
%include "include/hypatia/primitives/Vector3.h"
%include "include/hypatia/primitives/Matrix3x3.h"
%include "include/hypatia/primitives/TimeSpan.h"
%include "include/hypatia/primitives/DateTime.h"
%include "include/hypatia/coordinates/Equatorial.h"
%include "include/hypatia/coordinates/PrecessionMatrix.h"
%include "include/hypatia/coordinates/Galactic.h"
%include "include/hypatia/coordinates/Ecliptic.h"
%include "include/hypatia/coordinates/Horizontal.h"
%include "include/hypatia/coordinates/Geodetic.h"
%include "include/hypatia/coordinates/ECI.h"
%include "include/hypatia/coordinates/Tile.h"
%include "include/hypatia/coordinates/UTM.h"
%include "include/hypatia/TimeOps.h"
%include "include/hypatia/GeoOps.h"
%include "include/hypatia/Observer.h"
%include "include/hypatia/CoordOps.h"
%include "include/hypatia/ProjOps.h"
%include "include/hypatia/Body.h"
%include "include/hypatia/Luna.h"
%include "include/hypatia/Star.h"
%include "include/hypatia/Constellation.h"
%include "include/hypatia/Satellite.h"
%include "include/hypatia/models/TLE.h"
