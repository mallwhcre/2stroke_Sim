# YZ125 2-Stroke Engine Simulator

A zero-dimensional (0D) thermodynamic simulator for a 2-stroke engine, modelled after the **2005 Yamaha YZ125**. Written in C, this project simulates in-cylinder pressure and piston kinematics across the crank cycle.

> **Reference:** Blair, G. P. — *Design and Simulation of Two-Stroke Engines* (SAE International)
---

## Notes
 
> ⚠️ **Estimated Parameters**
>
> The following values are currently based on estimations or derived calculations, not direct physical measurement, and will be replaced with direct measurements soon:
>
> - **Crankcase Volume** — the crankcase compression ratio (CCR) and resulting crankcase volume are estimated.
> - **Reed Valve Assembly** — reed valve geometry (petal dimensions, cage angle, stop plate clearance), also estimated referncing measurements from different aftermarket assemblies.
>
> If you are building on this simulator, treat any results sensitive to crankcase dynamics with caution until these values are verified.
 
---
## What is a 0D Simulator?

A zero-dimensional simulator treats the cylinder as a single, spatially uniform control volume — no spatial gradients, no CFD. Every quantity (pressure, temperature, mass) is assumed uniform throughout the volume at each crank angle step. This is the standard starting point for engine cycle simulation and is the approach used throughout Blair's textbook.

---

## Engine Specifications — 2005 Yamaha YZ125

| Parameter | Value |
|---|---|
| Bore | 54.0 mm |
| Stroke | 54.5 mm |
| Connecting Rod Length | 102.0 mm |
| Crank Throw | 27.25 mm |
| Compression Ratio | 10.7 : 1 |
| Exhaust Port Height from Top | 28.0 mm |
| Transfer Port Height from Top | 41.0 mm |

---

## Reed Valve Specifications — V302A VForce3

Reed valve and reed block geometry are referenced from the **V302A VForce3** aftermarket reed valve assembly. The reed block and reed valve are assumed to have the same measurements. Reed block dimensions describe the **port cutout** geometry.

### Reed Petal

| Parameter | Value | Notes |
|---|---|---|
| Young's Modulus | 70 GPa | Assumed — woven carbon fiber |
| Length | 43.7 mm | |
| Width | 60.0 mm | |
| Thickness | 0.5 mm | Assumed |
| Max Lift | 8.0 mm | Assumed |

### Reed Block (Port Cutout)

| Parameter | Value | Notes |
|---|---|---|
| Number of Ports | 2 | |
| Length | 33.5 mm | Port cutout length |
| Width | 39.6 mm | Port cutout width |
| Angle | 90° | Assumed — simplified for calculations |
| Corner Radius | 0.919 mm | Semicircle radius of port cutout |
| Stop Height | 8 mm | Assumed |

---

## Physics & Modelling

### Piston Position

Piston displacement from TDC is computed from the classic slider-crank relation:

```
h(θ) = r(1 − cos θ) + l − √(l² − r²sin²θ)
```

where `r` is the crank throw, `l` is the connecting rod length, and `θ` is the crank angle.

### Cylinder Volume

```
V(θ) = V_clearance + A_piston × h(θ)
```

Clearance volume is derived from the compression ratio:

```
V_clearance = V_swept / (CR − 1)
```

### Crankcase Volume

The crankcase volume is calculated to model the fresh charge induction phase. Its maximum volume (at TDC) is derived from the swept volume and the crankcase compression ratio (CCR):

```
V_crankcase_max = (V_swept / (CCR − 1)) + V_swept
```

### Port Timing

Port opening angles are calculated geometrically by inverting the piston position equation. Given a port height measured from the top of the cylinder, the crank angle at which the piston uncovers the port is:

```
θ_port = arccos((( l + r − h_port)² − l² + r²) / (2r(l + r − h_port)))
```

Both the **exhaust port** and **transfer port** opening angles are computed this way.

### In-Cylinder Pressure

Pressure is modelled with the ideal gas law:

```
P = mRT / V
```

During the closed portion of the cycle (both ports covered), mass `m`, gas constant `R`, and temperature `T` are held constant, giving isentropic-like compression/expansion. Once either port opens, the pressure is set to atmospheric — a placeholder for scavenging flow modelling to be added later.

---

## Project Structure

```
.
├── source/
│   ├── sim.c       # Entry point — simulation loop, output
│   ├── engine.c    # Engine kinematics and geometry math
│   ├── engine.h    # EngineSpecs struct and constants
│   ├── therm.c     # Gas state and pressure logic
│   └── therm.h     # Atmospheric constants
├── Makefile        # Incremental build script
└── README.md
```

The codebase is modularized cleanly into geometry (`engine`) and thermodynamic (`therm`) files.

---

## Building & Running

This project uses a standard `Makefile` for incremental builds.

```bash
# Build the project
make

# Run the simulator
./sim

# Clean build artifacts
make clean
```

### Output

The simulator prints one line per crank angle degree:

```
crank angle is    180.0000  piston position is   54.500000  air pressure is      101325.00
```

Columns:
- **Crank angle** — degrees (0 = TDC, 180 = BDC)
- **Piston position** — mm from TDC
- **Air pressure** — Pa

---

## Current Limitations & Planned Work

This is an early-stage simulator. The following are known simplifications that will be addressed:

- **Isothermal assumption** — temperature is currently fixed at 293.15 K. A proper heat release / combustion model is needed.
- **Open cycle pressure** — when ports open, pressure is snapped to `P_atm`. Real scavenging involves gas dynamics and should use a filling-and-emptying or method-of-characteristics approach (per Blair Ch. 2–5).
- **No fuel/air mixture** — currently pure air. Fuel properties and equivalence ratio need to be introduced.
- **No exhaust pipe acoustics** — the power valve and tuned expansion chamber are central to the YZ125's performance and are not yet modelled.
- **Single-zone combustion** — no ignition timing, burn rate (Wiebe function), or knock model yet.

---

## References

- Blair, G. P. *Design and Simulation of Two-Stroke Engines.* SAE International
- 2005 Yamaha YZ125 factory service manual (geometry reference).
- https://www.structx.com/Beam_Formulas_019.html

