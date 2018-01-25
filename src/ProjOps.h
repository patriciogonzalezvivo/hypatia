#pragma once

#include "EqPoint.h"

class ProjOps {
public:
    
    static void horizontalToPolar(  double _alt, double _az,
                                    double &_x, double &_y);
    
    static void horizontalToPolar(  const EqPoint &_eq,
                                    double &_x, double &_y);
    
    static void horizontalToFisheye(double _alt, double _az,
                                    double &_x, double &_y);
    
    static void horizontalToFisheye(const EqPoint &_eq,
                                    double &_x, double &_y);
    
    static void horizontalToOrtho(  double _alt, double _az,
                                    double &_x, double &_y);
    
    static void horizontalToOrtho(  const EqPoint &_eq,
                                    double &_x, double &_y);
};
