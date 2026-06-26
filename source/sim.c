#include "engine.h"
#include "therm.h"
#include <math.h>
#include <stdio.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// yz125 2005
const EngineSpecs engine = {.bore = 54.0e-3,              // m (was 54.0 mm)
                            .stroke = 54.5e-3,            // m
                            .conRod = 102.0e-3,           // m
                            .crank_throw = 54.5e-3 / 2.0, // m
                            .compRatio = 10.7,
                            .crankCcr =
                                1.3, //!!! Not real data cannot find it!!
                            .ePort_h = 28.0e-3,  // m
                            .tPort_h = 41.0e-3}; // m

ReedPetal reed = {
    .youngsModulus = 70000.0e6, // Pa (was 70000 N/mm² = 70 GPa)
    .length = 43.7e-3,         // m
    .width = 60.0e-3,          // m
    .thickness = 0.5e-3,       // m (assumed value)
    .maxlift = 8.0e-3,         // m (assumed value)

};

ReedBlock block = {
    .num_ports = 2,
    .intake = 38.0e-3,    // m (was 38 mm, from stock carburetor)
    .length = 33.5e-3,    // m
    .width = 39.6e-3,     // m
    .angle = 90,          // degrees (assumed for simplicity)
    .r = 0.919e-3,        // m, radius of semicircle
    .stop_height = 8.0e-3 // m (assumed value)

};

int main() {
  // Volume is now natively m³, no /1e9 conversion needed
  double mass = (P_atm * get_volume(engine, BDC)) / (R_air * T_atm);
  int revs = 1;

  double totalAngle = 0.0;
  double angleStep = 1.0;
  double maxAngle = 360.0 * revs;

  while (totalAngle < maxAngle) {
    double curAngle = fmod(totalAngle, 360.0);
    double pistonPos = get_piston_pos(curAngle, engine);

    double pressure = get_pressure(curAngle, get_volume(engine, curAngle),
                                   T_atm, mass, engine);
    double dp = P_atm - pressure;

    // Display: piston position converted m -> mm for readability
    printf("crank angle is %10.4f  piston position is %10.6f mm  air pressure is "
           "%14.2f Pa\n",
           curAngle, pistonPos * 1000.0,
           get_pressure(curAngle, get_volume(engine, curAngle), T_atm, mass,
                        engine));

    totalAngle += angleStep;
  }
  // Display: volume converted m³ -> mm³ and m³ -> CC
  printf("the max crankcase volume is %f mm^3 (%.1f CC)\n",
         get_crankcase_volume(engine, 0.0) * 1e9,
         get_crankcase_volume(engine, 0.0) * 1e6);

  // --- Reed valve diagnostics ---
  printf("\n=== Reed Valve Diagnostics ===\n");
  printf("Reed Petal:\n");
  // Display: inertia m⁴ -> mm⁴ (×1e12), stiffness N/m -> N/mm (×1e-3)
  printf("  inertia        = %.4f mm^4\n", get_reed_inertia(reed) * 1e12);
  printf("  stiffness      = %.4f N/mm\n", get_reed_stiffness(reed) * 1e-3);
  printf("  cracking_dp    = %.2f Pa\n", get_cracking_dp(reed, 0.1));

  printf("\nReed Block:\n");
  // Display: areas m² -> mm² (×1e6), perimeter/length m -> mm (×1e3)
  printf("  throat area    = %.2f mm^2\n", get_throat_area(block) * 1e6);
  printf("  pipe area      = %.2f mm^2\n", get_pipe_area(block) * 1e6);
  printf("  port area ratio= %.4f\n", get_port_area(block));
  printf("  seal perimeter = %.2f mm\n", get_seal_perimeter(block) * 1e3);
  printf("  h_crit         = %.4f 1/mm\n", h_crit(block) / 1e3);
  printf("==============================\n");
}
