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
  double youngsModulus; // Pa

  // Geometry
  double length;    // m
  double width;     // m
  double thickness; // m
  double maxlift;   // m

  double stiffness;   // N/m
  double cracking_dp; // Pa, minimum ΔP to begin opening

  // Flow
  double cd_low;  // discharge coefficient
  double cd_high; // discharge coefficient

} ReedPetal;

typedef struct {

  int num_ports; // number of ports

  double intake; // m, inner diameter of intake throat from carb

  double length; // m, port cutout length
  double width;  // m, port cutout width

  double angle; // degrees, angle of reed block

  double r;           // m, corner radius of port cutout
  double stop_height; // m

} ReedBlock;

typedef struct {
  double crankAngle;  // degrees, crank angle at exhaust port closure
  double compRatio;   // trapped compression ratio (unitless)
  double volume;      // m³, trapping volume
  double pressure;    // Pa, trapping pressure
  double temperature; // K, trapping temperature
  double R_tr;        // J/(kg·K), gas constant of trapped mixture

  double mass;      // kg, total trapped mass
  double air_mass;  // kg, trapped air mass
  double fuel_mass; // kg, trapped fuel mass

  double heat_release;  // J, total heat release
  double fuel_vap_rate; // kg/deg, fuel vaporization rate per crank degree

} TrapState;

double get_pressure(double crankAngle, double volume, double temperature,
                    double mass, EngineSpecs engine);

// reed valve
double get_reed_inertia(ReedPetal reed);
double get_reed_stiffness(ReedPetal reed);
double get_cracking_dp(ReedPetal reed, double crackingThresh);
double get_tip_deflection(ReedPetal reed, double dp); // reed lift

double get_throat_area(ReedBlock block);
double get_pipe_area(ReedBlock block);
double get_port_area(ReedBlock block);

double get_seal_perimeter(ReedBlock block);

double h_crit(ReedBlock block);

// trapped
double get_trapped_mass();
double get_trapped_fuel(); // trapped fuel mass
double get_heat_release();
double get_vap_rate(); // evaporation rate

#endif