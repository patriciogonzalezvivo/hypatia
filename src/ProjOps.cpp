#include "ProjOps.h"

#include <math.h>

void ProjOps::toCarthesian( ProjId _id, const EqPoint &_eq, double &_x, double &_y ) {
    switch(_id) {
        case POLAR: toPolar(_eq, _x, _y);
            break;
        case FISHEYE: toFisheye(_eq, _x, _y);
            break;
        case ORTHO: toOrtho(_eq, _x, _y);
            break;
        case STEREO: toStereo(_eq, _x, _y);
            break;
        case LAMBERT: toLambert(_eq, _x, _y);
            break;
    }
}

//  https://github.com/slowe/VirtualSky/blob/gh-pages/virtualsky.js
void ProjOps::toPolar( double _alt, double _az, double &_x, double &_y ) {
    double radius = .5;
    double r = radius * ( MathOps::PI_OVER_TWO - _alt ) / MathOps::PI_OVER_TWO;
    //    _x = ( w / 2 - r * sin(_az));
    //    _y = (radius - r * cos(_az));
    _x = (.5 - r * sin(_az)) - .5;
    _y = (radius - r * cos(_az)) - .5;
}

void ProjOps::toPolar( const EqPoint &_eq, double &_x, double &_y) {
    toPolar( _eq.getAltitudRadians(), _eq.getAzimuthRadians(), _x, _y );
}

void ProjOps::toFisheye(double _alt, double _az, double &_x, double &_y) {
    double radius = .5;
    double r = radius * sin( (MathOps::PI_OVER_TWO - _alt)*.5 ) / 0.70710678;
    //    _x = ( w / 2 - r * sin(_az));
    //    _y = (radius - r * cos(_az));
    _x = (.5 - r * sin(_az)) - .5;
    _y = (radius - r * cos(_az)) - .5;
}

void ProjOps::toFisheye(const EqPoint &_eq, double &_x, double &_y) {
    toFisheye( _eq.getAltitudRadians(), _eq.getAzimuthRadians(), _x, _y );
}

void ProjOps::toOrtho( double _alt, double _az, double &_x, double &_y) {
    double radius = .5;
    double r = radius * cos(_alt);
//    _x = ( w / 2 - r * sin(_az));
//    _y = (radius - r * cos(_az));
    _x = (.5 - r * sin(_az)) - .5;
    _y = (radius - r * cos(_az)) - .5;
}

void ProjOps::toOrtho( const EqPoint &_eq, double &_x, double &_y) {
    toOrtho( _eq.getAltitudRadians(), _eq.getAzimuthRadians(), _x, _y );
}

void ProjOps::toStereo( double _alt, double _az, double &_x, double &_y ) {
    double f = 0.42;
    double sinel1 = 0.0;
    double cosel1 = 1.0;
    double cosaz = cos(_az-MathOps::HD_PI);
    double sinaz = sin(_az-MathOps::HD_PI);
    double sinel = sin(_alt);
    double cosel = cos(_alt);
    double k = (1.0 + sinel1 * sinel + cosel1 * cosel * cosaz) * 0.5;
//    _x = w/2 + f * k * h * cosel * sinaz;
//    _y = h - f * k * h * (cosel1 * sinel - sinel1 * cosel * cosaz);
    _x = .5 + f * k * cosel * sinaz;
    _y = 1. - f * k * (cosel1 * sinel - sinel1 * cosel * cosaz);
}

void ProjOps::toStereo( const EqPoint &_eq, double &_x, double &_y ) {
    toStereo( _eq.getAltitudRadians(), _eq.getAzimuthRadians(), _x, _y );
}

void ProjOps::toLambert( double _alt, double _az, double &_x, double &_y ) {
    double cosaz = cos(_az-MathOps::HD_PI);
    double sinaz = sin(_az-MathOps::HD_PI);
    double sinel = sin(_alt);
    double cosel = cos(_alt);
    double k = sqrt( (1.0 + cosel * cosaz) * 0.5);
//    _x = w / 2 + 0.6 * h * k * cosel * sinaz;
//    _y = h - 0.6 * h * k * sinel;
    _x = .5 + 0.6 * k * cosel * sinaz ;
    _y = 1. - 0.6 * k * sinel;
}
void ProjOps::toLambert( const EqPoint &_eq, double &_x, double &_y ) {
    toLambert( _eq.getAltitudRadians(), _eq.getAzimuthRadians(), _x, _y );
}
