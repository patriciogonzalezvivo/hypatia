#include "hypatia/ProjOps.h"
#include "hypatia/CoordOps.h"

#include <math.h>

void ProjOps::toXY( ProjId _id, double _alt, double _az, double _width, double _height, double &_x, double &_y ) {
    switch(_id) {
        case POLAR: ProjOps::toPolar(_alt, _az, _width, _height, _x, _y);
            break;
        case FISHEYE: ProjOps::toFisheye(_alt, _az, _width, _height, _x, _y);
            break;
        case ORTHO: ProjOps::toOrtho(_alt, _az, _width, _height, _x, _y);
            break;
        case STEREO: ProjOps::toStereo(_alt, _az, _width, _height, _x, _y);
            break;
        case LAMBERT: ProjOps::toLambert(_alt, _az, _width, _height, _x, _y);
            break;
        case EQUIRECTANGULAR: ProjOps::toEquirectangular(_alt, _az, _width, _height, _x, _y);
            break;
    }
}

void ProjOps::toXY( ProjId _id, const Horizontal& _coord, double _width, double _height, double &_x, double &_y ) {
    ProjOps::toXY(_id, _coord.getAltitud(RADS), _coord.getAzimuth(RADS), _width, _height, _x, _y);
}

//  https://github.com/slowe/VirtualSky/blob/gh-pages/virtualsky.js
void ProjOps::toPolar( double _alt, double _az, double _width, double _height, double &_x, double &_y ) {
    double radius = _height * .5;
    double r = radius * ( MathOps::PI_OVER_TWO - _alt ) / MathOps::PI_OVER_TWO;

    _x = (_width*.5 - r * sin(_az));
    _y = (radius - r * cos(_az)) - .5;
}

void ProjOps::toPolar( const Horizontal& _coord, double _width, double _height, double &_x, double &_y) {
    ProjOps::toPolar( _coord.getAltitud(RADS), _coord.getAzimuth(RADS), _width, _height, _x, _y );
}

void ProjOps::toFisheye(double _alt, double _az, double _width, double _height, double &_x, double &_y) {
    double radius = _height * .5;
    double r = radius * sin( (MathOps::PI_OVER_TWO - _alt)*.5 ) / 0.70710678;

    _x = (_width*.5 - r * sin(_az)) - .5;
    _y = (radius - r * cos(_az)) - .5;
}

void ProjOps::toFisheye(const Horizontal& _coord, double _width, double _height, double &_x, double &_y) {
    ProjOps::toFisheye( _coord.getAltitud(RADS), _coord.getAzimuth(RADS), _width, _height, _x, _y );
}

void ProjOps::toOrtho( double _alt, double _az, double _width, double _height, double &_x, double &_y) {
    double radius = _height * .5;
    double r = radius * cos(_alt);

    _x = (_width*.5 - r * sin(_az)) - .5;
    _y = (radius - r * cos(_az)) - .5;
}

void ProjOps::toOrtho( const Horizontal& _coord, double _width, double _height, double &_x, double &_y) {
    ProjOps::toOrtho( _coord.getAltitud(RADS), _coord.getAzimuth(RADS), _width, _height, _x, _y );
}

void ProjOps::toStereo( double _alt, double _az, double _width ,double _height, double &_x, double &_y ) {
    double f = 0.42;
    double sinel1 = 0.0;
    double cosel1 = 1.0;
    double cosaz = cos(_az-MathOps::PI);
    double sinaz = sin(_az-MathOps::PI);
    double sinel = sin(_alt);
    double cosel = cos(_alt);
    double k = 2.0/(1.0 + sinel1 * sinel + cosel1 * cosel * cosaz);

    _x = _width * .5 + f * k * _height * cosel * sinaz;
    _y = _height - f * _height * k * (cosel1 * sinel - sinel1 * cosel * cosaz);
}

void ProjOps::toStereo( const Horizontal& _coord, double _width ,double _height, double &_x, double &_y ) {
    ProjOps::toStereo( _coord.getAltitud(RADS), _coord.getAzimuth(RADS), _width, _height, _x, _y );
}

void ProjOps::toLambert( double _alt, double _az, double _width ,double _height, double &_x, double &_y ) {
    double cosaz = cos(_az-MathOps::PI);
    double sinaz = sin(_az-MathOps::PI);
    double sinel = sin(_alt);
    double cosel = cos(_alt);
    double k = sqrt( 2/(1.0 + cosel * cosaz));

    _x = _width * .5 + 0.6 * _height * k * cosel * sinaz;
    _y = _height - 0.6 * _height * k * sinel;
}
void ProjOps::toLambert( const Horizontal& _coord, double _width ,double _height, double &_x, double &_y ) {
    ProjOps::toLambert( _coord.getAltitud(RADS), _coord.getAzimuth(RADS), _width, _height, _x, _y );
}

void ProjOps::toEquirectangular( double _alt, double _az, double _width ,double _height, double &_x, double &_y ) {
    while (_az < 0) {
        _az += MathOps::TAU;
    }
    _az = fmod(_az, MathOps::TAU);
    
    _x = ( (_az-MathOps::PI) / MathOps::PI_OVER_TWO ) * _height + _width*.5;
    _y = _height - (_alt / MathOps::PI_OVER_TWO) * _height;
}

void ProjOps::toEquirectangular( const Horizontal& _coord,  double _width ,double _height, double &_x, double &_y ) {
    ProjOps::toEquirectangular( _coord.getAltitud(RADS), _coord.getAzimuth(RADS), _width, _height, _x, _y );
}

void ProjOps::toMercator( double _lng, double _lat, double& _x, double& _y) {
    _x = _lng * CoordOps::EARTH_EQUATORIAL_RADIUS_M;
    _y = log(tan(MathOps::PI * 0.25 + _lat * 0.5)) * CoordOps::EARTH_EQUATORIAL_RADIUS_M;
}

void ProjOps::toMercator( const Geodetic& _coord, double& _x, double& _y) {
    ProjOps::toMercator( _coord.getLongitude(RADS), _coord.getLatitude(RADS), _x, _y);
}

// void ProjOps::toMercator( const Tile& _coord, double& _x, double& _y) {
//     double metersPerTile = _coord.getMetersPerTile();
//     _x = _coord.x * metersPerTile - CoordOps::EARTH_EQUATORIAL_HALF_CIRCUMFERENCE_M;
//     _y = CoordOps::EARTH_EQUATORIAL_HALF_CIRCUMFERENCE_M - _coord.y * metersPerTile;
// }
