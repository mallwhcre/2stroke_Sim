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

  inertia = (reed.width * pow(reed.thickness, 3)) / 12;
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

double get_throat_area(ReedBlock block) {
  double angle_rad = block.angle * M_PI / 180.0;

  double throat;
  throat = block.num_ports * (block.length * block.width -
                              pow(block.r, 2) * (4 - M_PI) / sin(angle_rad));

  return throat;
}

double get_pipe_area(ReedBlock block) {
  double pipe;

  pipe = (M_PI / 4) * pow(block.intake, 2);

  return pipe;
}

double get_port_area(ReedBlock block) {
  double port_area;

  port_area = get_throat_area(block) / get_pipe_area(block);

  return port_area;
}

double get_seal_perimeter(ReedBlock block) {
  double perimeter;

  perimeter = block.width; // following blaire's assumtion no flow from the
                           // sides

  return perimeter;
}

double h_crit(ReedBlock block) {
  double h_crit;

  h_crit = get_port_area(block) / get_seal_perimeter(block);

  return h_crit;
}

double get_tip_deflection(ReedPetal reed, double dp) {
  double deflection;

  double I = get_reed_inertia(reed);

  deflection =
      (dp * reed.length * pow(reed.length, 4)) / (8 * reed.youngsModulus * I);

  if (deflection > reed.maxlift)
    deflection = reed.maxlift;

  return deflection
}
