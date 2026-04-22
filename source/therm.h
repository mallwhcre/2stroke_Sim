#ifndef THERM_H
#define THERM_H

#include "engine.h"

#define R_air 287.05   // J/(kg·K)
#define T_atm 293.15   // 20 deg c in kelvin
#define P_atm 101325.0 // atmospheric pressure in pascals

typedef struct {
  // State
  double opening;      // 0.0 (closed) to 1.0 (fully open)
  double opening_rate; // d(opening)/dt

  // material
  double youngsModulus; // N/mm² (MPa)

  // Geometry
  double length;    // mm
  double width;     // mm
  double thickness; // mm
  double maxlift;   // mm

  double stiffness;   // k N/mm
  double cracking_dp; // Pa, minimum ΔP to begin opening

  // Flow
  double cd_low;  // discharge coefficient
  double cd_high; // discharge coefficient

} ReedPetal;

typedef struct {

  int num_ports; // number of ports

  int intake; // size of inner diameter of the intake throat from carb
              // measurment

  double length; // port cutout length mm
  double width;  // port cutout width mm

  double angle; // angle of reed block  degrees

  double r;           // corner radius of port cutout
  double stop_height; // mm

} ReedBlock;

double get_pressure(double crankAngle, double volume, double temperature,
                    double mass, EngineSpecs engine);

double get_reed_inertia(ReedPetal reed);
double get_reed_stiffness(ReedPetal reed);
double get_cracking_dp(ReedPetal reed, double crackingThresh);
double get_tip_deflection(ReedPetal reed, double dp); // reed lift

double get_throat_area(ReedBlock block);
double get_pipe_area(ReedBlock block);
double get_port_area(ReedBlock block);

double get_seal_perimeter(ReedBlock block);

double h_crit(ReedBlock block);

#endif