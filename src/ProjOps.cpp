#include "ProjOps.h"

#include <math.h>

//  https://github.com/slowe/VirtualSky/blob/gh-pages/virtualsky.js
void ProjOps::horizontalToPolar( double _alt, double _az, double _width, double _height, double &_x, double &_y ) {
    double radius = _height * .5;
    double r = radius * ( MathOps::PI_OVER_TWO - _alt ) / MathOps::PI_OVER_TWO;
    _x = (_width * .5 - r * sin(_az));
    _y = (radius - r * cos(_az));
}

void ProjOps::horizontalToPolar( const EqPoint &_eq, double _width, double _height, double &_x, double &_y) {
    horizontalToPolar( _eq.getAltitudRadians(), _eq.getAzimuthRadians(), _width, _height, _x, _y );
}
