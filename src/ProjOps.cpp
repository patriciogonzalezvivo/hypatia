#include "ProjOps.h"

#include <math.h>

void ProjOps::toPolar( PolarProjId _id, const EqPoint &_eq, double &_x, double &_y ) {
    switch(_id) {
        case POLAR: toPolar(_eq, _x, _y);
            break;
        case FISHEYE: toFisheye(_eq, _x, _y);
            break;
        case ORTHO: toOrtho(_eq, _x, _y);
            break;
    }
}

//  https://github.com/slowe/VirtualSky/blob/gh-pages/virtualsky.js
void ProjOps::toPolar( double _alt, double _az, double &_x, double &_y ) {
    double radius = .5;
    double r = radius * ( MathOps::PI_OVER_TWO - _alt ) / MathOps::PI_OVER_TWO;

    _x = (.5 - r * sin(_az)) - .5;
    _y = (radius - r * cos(_az)) - .5;
}

void ProjOps::toPolar( const EqPoint &_eq, double &_x, double &_y) {
    toPolar( _eq.getAltitudRadians(), _eq.getAzimuthRadians(), _x, _y );
}

void ProjOps::toFisheye(double _alt, double _az, double &_x, double &_y) {
    double radius = .5;
    double r = radius * sin( (MathOps::PI_OVER_TWO - _alt)*.5 ) / 0.70710678;

    _x = (.5 - r * sin(_az)) - .5;
    _y = (radius - r * cos(_az)) - .5;
}

void ProjOps::toFisheye(const EqPoint &_eq, double &_x, double &_y) {
    toFisheye( _eq.getAltitudRadians(), _eq.getAzimuthRadians(), _x, _y );
}

void ProjOps::toOrtho( double _alt, double _az, double &_x, double &_y) {
    double radius = .5;
    double r = radius * cos(_alt);

    _x = (.5 - r * sin(_az)) - .5;
    _y = (radius - r * cos(_az)) - .5;
}

void ProjOps::toOrtho( const EqPoint &_eq, double &_x, double &_y) {
    toOrtho( _eq.getAltitudRadians(), _eq.getAzimuthRadians(), _x, _y );
}

//void ProjOps::toCartesian( CartesianProjId _id, const EqPoint &_eq, double _width ,double _height, double &_x, double &_y ) {
//    switch(_id) {
//        case STEREO: toStereo(_eq, _width, _height, _x, _y);
//            break;
//        case LAMBERT: toLambert(_eq, _width, _height, _x, _y);
//            break;
//        case EQUIRECTANGULAR: toEquirectangular(_eq, _width, _height, _x, _y);
//            break;
//    }
//}
//
//void ProjOps::toStereo( double _alt, double _az, double _width ,double _height, double &_x, double &_y ) {
//    double f = 0.42;
//    double sinel1 = 0.0;
//    double cosel1 = 1.0;
//    double cosaz = cos(_az-MathOps::HD_PI);
//    double sinaz = sin(_az-MathOps::HD_PI);
//    double sinel = sin(_alt);
//    double cosel = cos(_alt);
//    double k = (1.0 + sinel1 * sinel + cosel1 * cosel * cosaz) * 0.5;
//
//    _x = _width * .5 + f * k * cosel * sinaz;
//    _y = _height - f * _height * k * (cosel1 * sinel - sinel1 * cosel * cosaz);
//}
//
//void ProjOps::toStereo( const EqPoint &_eq, double _width ,double _height, double &_x, double &_y ) {
//    toStereo( _eq.getAltitudRadians(), _eq.getAzimuthRadians(), _width, _height, _x, _y );
//}
//
//void ProjOps::toLambert( double _alt, double _az, double _width ,double _height, double &_x, double &_y ) {
//    double cosaz = cos(_az-MathOps::HD_PI);
//    double sinaz = sin(_az-MathOps::HD_PI);
//    double sinel = sin(_alt);
//    double cosel = cos(_alt);
//    double k = sqrt( (1.0 + cosel * cosaz) * 0.5);
//
//    _x = _width * .5 + 0.6 * k * cosel * sinaz ;
//    _y = _height - 0.6 * _height * k * sinel;
//}
//void ProjOps::toLambert( const EqPoint &_eq, double _width ,double _height, double &_x, double &_y ) {
//    toLambert( _eq.getAltitudRadians(), _eq.getAzimuthRadians(), _width, _height, _x, _y );
//}
//
//void ProjOps::toEquirectangular( double _alt, double _az, double _width ,double _height, double &_x, double &_y ) {
//    while (_az < 0) {
//        _az += MathOps::TAU;
//    }
//    _az = fmod(_az, MathOps::TAU);
//    
//    _x = ( (_az-MathOps::HD_PI) / MathOps::PI_OVER_TWO ) * _height + _width*.5;
//    _y = _height - (_alt / MathOps::PI_OVER_TWO) * _height;
//}
//
//void ProjOps::toEquirectangular( const EqPoint &_eq,  double _width ,double _height, double &_x, double &_y ) {
//    toEquirectangular( _eq.getAltitudRadians(), _eq.getAzimuthRadians(), _width, _height, _x, _y );
//}

