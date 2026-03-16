#include <math.h>
#include <stdio.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define R_air 287.05   // mm^3
#define T_atm 293.15   // 20 deg c in kelvin
#define P_atm 101325.0 // atmospheric pressure in pascals

#define BDC 180.0
#define TDC 0.0
// yz125 2005

typedef struct {

  double bore;
  double stroke;
  double conRod;
  double throw;
  double compRatio;

} EngineSpecs;

const EngineSpecs engine = {.bore = 54.0,
                            .stroke = 54.5,
                            .conRod = 102.0,
                            .throw = 54.5 / 2.0,
                            .compRatio = 10.7};

double get_piston_pos(double crankAngle, EngineSpecs engine);
double get_swept(EngineSpecs engine);
double get_clearance(EngineSpecs engine);
double get_volume(EngineSpecs engine, double crankAngle);
double get_pressure(double crankAngle, double volume, double temperature,
                    double mass);

int main() {
  double mass = (P_atm * (get_volume(engine, BDC) / 1e9)) / (R_air * T_atm);

  int revs = 1;

  double totalAngle = 0.0;
  double angleStep = 10.0;
  double maxAngle = 360.0 * revs;

  while (totalAngle < maxAngle) {
    double curAngle = fmod(totalAngle, 360.0);
    double pistonPos = get_piston_pos(curAngle, engine);
    printf("%.2f %.4f\n", curAngle, pistonPos);
    printf("air pressure at %f is %f\n", curAngle,
           get_pressure(curAngle, get_volume(engine, curAngle), T_atm, mass));
    totalAngle += angleStep;
  }

  printf("the mass of the air is %f\n", mass);
  // printf("the swept volume is %f\n", get_swept(engine));
  // printf("the clearence volume is %f\n", get_clearance(engine));
  // printf("the volume at 180 degrees is %f\n", get_volume(engine, 180));
}

double get_piston_pos(double crankAngle, EngineSpecs engine) {
  double h;

  double rad = crankAngle * M_PI / 180.0; // convert to radians

  h = engine.conRod + engine.throw *(1 - cos(rad)) -
      sqrt(pow(engine.conRod, 2) - pow(engine.throw *sin(rad), 2));

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

double get_pressure(double crankAngle, double volume, double temperature,
                    double mass) {
  double pressure;

  pressure = (mass * R_air * T_atm) / (volume / 1e9);

  return pressure;
}