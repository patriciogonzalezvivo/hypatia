#pragma once

#include "EqPoint.h"

class ProjOps {
public:
    
    static void toPolar( double _alt, double _az, double &_x, double &_y );
    static void toPolar( const EqPoint &_eq, double &_x, double &_y );
    
    static void toFisheye( double _alt, double _az, double &_x, double &_y );
    static void toFisheye( const EqPoint &_eq, double &_x, double &_y );
    
    static void toOrtho( double _alt, double _az, double &_x, double &_y );
    static void toOrtho( const EqPoint &_eq, double &_x, double &_y );
    
};
