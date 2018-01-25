#include "ProjOps.h"

#include <math.h>

//  https://github.com/slowe/VirtualSky/blob/gh-pages/virtualsky.js
void ProjOps::ToPolar( double _alt, double _az, double &_x, double &_y ) {
    double radius = .5;
    double r = radius * ( MathOps::PI_OVER_TWO - _alt ) / MathOps::PI_OVER_TWO;
    _x = (.5 - r * sin(_az)) - .5;
    _y = (radius - r * cos(_az)) - .5;
}

void ProjOps::ToPolar( const EqPoint &_eq, double &_x, double &_y) {
    ToPolar( _eq.getAltitudRadians(), _eq.getAzimuthRadians(), _x, _y );
}

void ProjOps::ToFisheye(double _alt, double _az, double &_x, double &_y) {
    double radius = .5;
    double r = radius * sin( (MathOps::PI_OVER_TWO - _alt)*.5 ) / 0.70710678;
    _x = (.5 - r * sin(_az)) - .5;
    _y = (radius - r * cos(_az)) - .5;
}

void ProjOps::ToFisheye(const EqPoint &_eq, double &_x, double &_y) {
    ToFisheye( _eq.getAltitudRadians(), _eq.getAzimuthRadians(), _x, _y );
}

void ProjOps::ToOrtho( double _alt, double _az, double &_x, double &_y) {
    double radius = .5;
    double r = radius * cos(_alt);
    _x = (.5 - r * sin(_az)) - .5;
    _y = (radius - r * cos(_az)) - .5;
}

void ProjOps::ToOrtho( const EqPoint &_eq, double &_x, double &_y) {
    ToOrtho( _eq.getAltitudRadians(), _eq.getAzimuthRadians(), _x, _y );
}
