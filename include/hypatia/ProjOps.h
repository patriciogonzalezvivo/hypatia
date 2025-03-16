#pragma once

#include "coordinates/Horizontal.h"
#include "coordinates/Geodetic.h"

#include "primitives/Vector2.h"

enum ProjId {
    POLAR=0, FISHEYE=1, ORTHO=2,
    STEREO=3, LAMBERT=4, EQUIRECTANGULAR=5
};

class ProjOps {
public:
    
    static void toXY( ProjId _id, double _alt, double _az, double _width, double _height, double &_x, double &_y );
    static Vector2 toVector2( ProjId _id, const Horizontal& _coord, double _width, double _height);
    
    static void toPolar( double _alt, double _az, double _width, double _height, double &_x, double &_y );
    static Vector2 toPolar( const Horizontal& _coord, double _width, double _height );
    
    static void toFisheye( double _alt, double _az, double _width, double _height, double &_x, double &_y );
    static Vector2 toFisheye( const Horizontal& _coord, double _width, double _height );
    
    static void toOrtho( double _alt, double _az, double _width, double _height, double &_x, double &_y );
    static Vector2 toOrtho( const Horizontal& _coord, double _width, double _height );
    
    static void toStereo( double _alt, double _az, double _width, double _height, double &_x, double &_y );
    static Vector2 toStereo( const Horizontal& _coord, double _width, double _height );
    
    static void toLambert( double _alt, double _az, double _width, double _height, double &_x, double &_y );
    static Vector2 toLambert( const Horizontal& _coord, double _width, double _height );
    
    static void toEquirectangular( double _alt, double _az, double _width, double _height, double &_x, double &_y );
    static Vector2 toEquirectangular( const Horizontal& _coord, double _width, double _height );
};
