#include "therm.h"
#include "engine.h"

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
