#include "therm.h"
#include "engine.h"
#include <math.h>

double get_pressure(double crankAngle, double volume, double temperature,
                    double mass, EngineSpecs engine) {
  double pressure;

  double ePort_angle = get_port_angle(engine.ePort_h, engine);
  double tPort_angle = get_port_angle(engine.tPort_h, engine);

  int ePort_open = 0;
  int tPort_open = 0;

  volume = get_volume(engine, crankAngle);

  if (crankAngle > ePort_angle)
    ePort_open = 1;
  if (crankAngle > tPort_angle)
    tPort_open = 1;

  if (!ePort_open && !tPort_open)
    pressure = (mass * R_air * T_atm) / (volume / 1e9);

  else
    pressure = P_atm;

  return pressure;
}

double get_reed_inertia(ReedPetal reed) {
  double inertia;

  inertia = (reed.width * pow(reed.length, 2)) / 12;
  return inertia;
}

double get_reed_stiffness(ReedPetal reed) {
  double stiffness;

  stiffness =
      (3 * reed.youngsModulus * get_reed_inertia(reed)) / pow(reed.length, 3);
  return stiffness;
}

double get_cracking_dp(ReedPetal reed, double crackingThresh) {
  double dp;

  dp = (8 * reed.youngsModulus * get_reed_inertia(reed) * crackingThresh) /
       (reed.width * pow(reed.length, 4));
  return dp;
}