#pragma once

#include "EqPoint.h"

enum ProjId {
    POLAR=0, FISHEYE=1, ORTHO=2,
    STEREO=3, LAMBERT=4, EQUIRECTANGULAR=5
};

class ProjOps {
public:
    
    // POLAR
    static void toXY( ProjId _id, double _alt, double _az, double _width, double _height, double &_x, double &_y );
    static void toXY( ProjId _id, const EqPoint &_eq, double _width, double _height, double &_x, double &_y );
    
    static void toPolar( double _alt, double _az, double _width, double _height, double &_x, double &_y );
    static void toPolar( const EqPoint &_eq, double _width, double _height, double &_x, double &_y );
    
    static void toFisheye( double _alt, double _az, double _width, double _height, double &_x, double &_y );
    static void toFisheye( const EqPoint &_eq, double _width, double _height, double &_x, double &_y );
    
    static void toOrtho( double _alt, double _az, double _width, double _height, double &_x, double &_y );
    static void toOrtho( const EqPoint &_eq, double _width, double _height, double &_x, double &_y );
    
    static void toStereo( double _alt, double _az, double _width, double _height, double &_x, double &_y );
    static void toStereo( const EqPoint &_eq, double _width, double _height, double &_x, double &_y );
    
    static void toLambert( double _alt, double _az, double _width, double _height, double &_x, double &_y );
    static void toLambert( const EqPoint &_eq, double _width, double _height, double &_x, double &_y );
    
    static void toEquirectangular( double _alt, double _az, double _width, double _height, double &_x, double &_y );
    static void toEquirectangular( const EqPoint &_eq, double _width, double _height, double &_x, double &_y );
};
