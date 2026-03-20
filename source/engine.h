#ifndef ENGINE_H
#define ENGINE_H

#define BDC 180.0
#define TDC 0.0

typedef struct {

  double bore;
  double stroke;
  double conRod;
  double crank_throw;
  double compRatio;
  double crankCcr; // crankcase comp ratio
  double ePort_h;
  double tPort_h;

} EngineSpecs;

double get_piston_pos(double crankAngle, EngineSpecs engine);
double get_swept(EngineSpecs engine);
double get_clearance(EngineSpecs engine);
double get_volume(EngineSpecs engine, double crankAngle);
double get_port_angle(double portHeight, EngineSpecs engine);
double get_crankcase_volume(EngineSpecs engine, double crankAngle);

#endif
