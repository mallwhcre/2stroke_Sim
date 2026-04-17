#include "engine.h"
#include "therm.h"
#include <math.h>
#include <stdio.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// yz125 2005
const EngineSpecs engine = {.bore = 54.0, // all in mm
                            .stroke = 54.5,
                            .conRod = 102.0,
                            .crank_throw = 54.5 / 2.0,
                            .compRatio = 10.7,
                            .crankCcr =
                                1.3, //!!! Not real data cannot find it!!
                            .ePort_h = 28.0,
                            .tPort_h = 41.0};

ReedPetal reed = {
    .youngsModulus = 70, // assumed value for woven carbon fiber in GPa
    .length = 43.7,
    .width = 60.0,
    .thickness = 0.5, // assumed value
    .maxlift = 8.0,   // assumed value

};

ReedBlock block = {
    .num_ports = 2,
    .length = 33.5,
    .width = 39.6,
    .angle = 90,     // assumed for simplicity of calculations.
    .r = 0.919,      // radius of semicircle
    .stop_height = 8 // assumed value
};

int main() {
  double mass = (P_atm * (get_volume(engine, BDC) / 1e9)) / (R_air * T_atm);
  int revs = 1;

  double totalAngle = 0.0;
  double angleStep = 1.0;
  double maxAngle = 360.0 * revs;

  while (totalAngle < maxAngle) {
    double curAngle = fmod(totalAngle, 360.0);
    double pistonPos = get_piston_pos(curAngle, engine);

    printf("crank angle is %10.4f  piston position is %10.6f  air pressure is "
           "%14.2f\n",
           curAngle, pistonPos,
           get_pressure(curAngle, get_volume(engine, curAngle), T_atm, mass,
                        engine));

    totalAngle += angleStep;
  }
  printf("the max crankcase volume is %f mm^3 (%.1f CC)\n",
         get_crankcase_volume(engine, 0.0),
         get_crankcase_volume(engine, 0.0) / 1000.0);
}
