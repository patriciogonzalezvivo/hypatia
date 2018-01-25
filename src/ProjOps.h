#pragma once

#include "EqPoint.h"

enum ProjId {
    POLAR=0, FISHEYE=1, ORTHO=2,
    STEREO=3, LAMBERT=4
};

class ProjOps {
public:
    
    static void toCarthesian( ProjId _id, const EqPoint &_eq, double &_x, double &_y );
    
    static void toPolar( double _alt, double _az, double &_x, double &_y );
    static void toPolar( const EqPoint &_eq, double &_x, double &_y );
    
    static void toFisheye( double _alt, double _az, double &_x, double &_y );
    static void toFisheye( const EqPoint &_eq, double &_x, double &_y );
    
    static void toOrtho( double _alt, double _az, double &_x, double &_y );
    static void toOrtho( const EqPoint &_eq, double &_x, double &_y );
    
    static void toStereo( double _alt, double _az, double &_x, double &_y );
    static void toStereo( const EqPoint &_eq, double &_x, double &_y );
    
    static void toLambert( double _alt, double _az, double &_x, double &_y );
    static void toLambert( const EqPoint &_eq, double &_x, double &_y );
};
