#pragma once

#include "EqPoint.h"

class ProjOps {
public:
    
    static void horizontalToPolar(  double _alt, double _az,
                                    double _width, double _height,
                                    double &_x, double &_y);
    
    static void horizontalToPolar(  const EqPoint &_eqPoint,
                                    double _width, double _height,
                                    double &_x, double &_y);
                                         
};
