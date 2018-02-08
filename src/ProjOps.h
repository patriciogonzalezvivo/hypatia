#pragma once

#include "HorPoint.h"

enum ProjId {
    POLAR=0, FISHEYE=1, ORTHO=2,
    STEREO=3, LAMBERT=4, EQUIRECTANGULAR=5
};

class ProjOps {
public:
    
    // POLAR
    static void toXY( ProjId _id, double _alt, double _az, double _width, double _height, double &_x, double &_y );
    static void toXY( ProjId _id, const HorPoint &_eq, double _width, double _height, double &_x, double &_y );
    
    static void toPolar( double _alt, double _az, double _width, double _height, double &_x, double &_y );
    static void toPolar( const HorPoint &_eq, double _width, double _height, double &_x, double &_y );
    
    static void toFisheye( double _alt, double _az, double _width, double _height, double &_x, double &_y );
    static void toFisheye( const HorPoint &_eq, double _width, double _height, double &_x, double &_y );
    
    static void toOrtho( double _alt, double _az, double _width, double _height, double &_x, double &_y );
    static void toOrtho( const HorPoint &_eq, double _width, double _height, double &_x, double &_y );
    
    static void toStereo( double _alt, double _az, double _width, double _height, double &_x, double &_y );
    static void toStereo( const HorPoint &_eq, double _width, double _height, double &_x, double &_y );
    
    static void toLambert( double _alt, double _az, double _width, double _height, double &_x, double &_y );
    static void toLambert( const HorPoint &_eq, double _width, double _height, double &_x, double &_y );
    
    static void toEquirectangular( double _alt, double _az, double _width, double _height, double &_x, double &_y );
    static void toEquirectangular( const HorPoint &_eq, double _width, double _height, double &_x, double &_y );
};
