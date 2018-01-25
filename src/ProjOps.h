#pragma once

#include "EqPoint.h"

class ProjOps {
public:
    
    static void ToPolar( double _alt, double _az, double &_x, double &_y );
    static void ToPolar( const EqPoint &_eq, double &_x, double &_y );
    
    static void ToFisheye( double _alt, double _az, double &_x, double &_y );
    static void ToFisheye( const EqPoint &_eq, double &_x, double &_y );
    
    static void ToOrtho( double _alt, double _az, double &_x, double &_y );
    static void ToOrtho( const EqPoint &_eq, double &_x, double &_y );
    
};
