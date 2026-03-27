#ifndef THERM_H
#define THERM_H

#include "engine.h"

#define R_air 287.05   // mm^3
#define T_atm 293.15   // 20 deg c in kelvin
#define P_atm 101325.0 // atmospheric pressure in pascals

typedef struct {
  // State
  double opening;      // 0.0 (closed) to 1.0 (fully open)
  double opening_rate; // d(opening)/dt

  // material
  double youngsModulus;

  // Geometry
  double length;    // mm
  double width;     // mm
  double thickness; // mm
  double max_lift;  // mm

  double stiffness;   // k M/mm
  double cracking_dp; // Pa, minimum ΔP to begin opening

  // Flow
  double cd;             // discharge coefficient
  double effective_area; // mm², full-open area (length × width)
} ReedPetal;

double get_pressure(double crankAngle, double volume, double temperature,
                    double mass, EngineSpecs engine);

double get_reed_inertia(ReedPetal reed);
double get_reed_stiffness(ReedPetal reed);
#endif