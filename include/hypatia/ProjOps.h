#pragma once

#include "coordinates/Horizontal.h"
#include "coordinates/Geodetic.h"
#include "coordinates/Tile.h"

enum ProjId {
    POLAR=0, FISHEYE=1, ORTHO=2,
    STEREO=3, LAMBERT=4, EQUIRECTANGULAR=5
};

class ProjOps {
public:
    
    static void toXY( ProjId _id, double _alt, double _az, double _width, double _height, double &_x, double &_y );
    static void toXY( ProjId _id, const Horizontal& _coord, double _width, double _height, double &_x, double &_y );
    
    static void toPolar( double _alt, double _az, double _width, double _height, double &_x, double &_y );
    static void toPolar( const Horizontal& _coord, double _width, double _height, double &_x, double &_y );
    
    static void toFisheye( double _alt, double _az, double _width, double _height, double &_x, double &_y );
    static void toFisheye( const Horizontal& _coord, double _width, double _height, double &_x, double &_y );
    
    static void toOrtho( double _alt, double _az, double _width, double _height, double &_x, double &_y );
    static void toOrtho( const Horizontal& _coord, double _width, double _height, double &_x, double &_y );
    
    static void toStereo( double _alt, double _az, double _width, double _height, double &_x, double &_y );
    static void toStereo( const Horizontal& _coord, double _width, double _height, double &_x, double &_y );
    
    static void toLambert( double _alt, double _az, double _width, double _height, double &_x, double &_y );
    static void toLambert( const Horizontal& _coord, double _width, double _height, double &_x, double &_y );
    
    static void toEquirectangular( double _alt, double _az, double _width, double _height, double &_x, double &_y );
    static void toEquirectangular( const Horizontal& _coord, double _width, double _height, double &_x, double &_y );

// Geografic projections

    static void toMercator( double _lng, double _lat, double &_x, double &_y);
    static void toMercator( const Geodetic& _coord, double &_x, double &_y);

    // static void toMercator( const Tile& _coord, double &_x, double &_y);

    // static void fromMercator( double _x, double _y, double& _lng, double& _lat);
};
