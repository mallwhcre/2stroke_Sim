#ifndef THERM_H
#define THERM_H

#include "engine.h"

#define R_air 287.05   // mm^3
#define T_atm 293.15   // 20 deg c in kelvin
#define P_atm 101325.0 // atmospheric pressure in pascals

double get_pressure(double crankAngle, double volume, double temperature,
                    double mass, EngineSpecs engine);

#endif