#include "engine.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

double get_piston_pos(double crankAngle, EngineSpecs engine) {
  double h;

  double rad = crankAngle * M_PI / 180.0; // convert to radians

  h = engine.conRod + engine.crank_throw *(1 - cos(rad)) -
      sqrt(pow(engine.conRod, 2) - pow(engine.crank_throw *sin(rad), 2));

  return h;
}

double get_swept(EngineSpecs engine) {
  double swept;

  swept = 1 * (M_PI / 4) * pow(engine.bore, 2) * engine.stroke;

  return swept;
}

double get_clearance(EngineSpecs engine) {
  double clearance;

  clearance = get_swept(engine) /
              (engine.compRatio - 1); // clearance = swept / (CR − 1)

  return clearance;
}

double get_volume(EngineSpecs engine, double crankAngle) {
  double volume;
  double pistonArea = (M_PI / 4) * pow(engine.bore, 2);
  double pistonPos = get_piston_pos(crankAngle, engine);

  volume = get_clearance(engine) + pistonArea * (pistonPos);

  return volume;
}

double get_port_angle(double portHeight, EngineSpecs engine) {
  double angle;

  angle = acos((pow((engine.conRod + engine.crank_throw -portHeight), 2) -
                pow(engine.conRod, 2) + pow(engine.crank_throw, 2)) /
               (2 * engine.crank_throw *(engine.conRod + engine.crank_throw -portHeight)));

  return angle * 180 / M_PI;
}

double get_crankcase_volume(EngineSpecs engine) {
  double volume;
  // Maximum crankcase volume (at TDC) = Clearance + Swept
  volume = (get_swept(engine) / (engine.crankCcr - 1)) + get_swept(engine);

  return volume;
}
