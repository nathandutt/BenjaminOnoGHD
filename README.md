# Benjamin-Ono N-Soliton Effective Hydrodynamics

> Numerical study of the effective hydrodynamics of Benjamin-Ono N-soliton solutions via pole dynamics and local kinetic theory.

---

## Overview

This project simulates the **effective hydrodynamics** of Benjamin-Ono (B-O) N-soliton solutions by tracking the positions of poles in the complex plane. Each soliton is characterized by two parameters:

- **Impact parameter** $b_i$ — the transverse offset
- **Velocity** $k_i > 0$ — the asymptotic speed at infinity

The hydrodynamic regime requires that no two solitons share a velocity too close together: the minimum separation $\Delta k$ between any two $k_i$ values must exceed a threshold set in the configuration. When this condition holds, a local Calogero-Moser Lax matrix can be diagonalized in each spatial bin to extract **local rapidities**, the key observable of the kinetic theory.

---

## Getting Started

### Prerequisites

- C++ compiler with C++17 support
- [Eigen](https://eigen.tuxfamily.org/) library (for matrix diagonalization)

### Build

```bash
make
```

This produces three binaries: `random`, `evolve`, and `kinetics`.

---

## Binaries

### 1. `./random` — Generate initial soliton parameters

Draws $N$ soliton parameters $(b_i, k_i)$ at random:

- **Velocities** $k_i$ are sampled from a uniform point process. Any draw too close to a previously drawn value (within $\Delta k$) is rejected and redrawn, enforcing the hydrodynamic validity condition.
- **Impact parameters** $b_i$ are drawn from a uniform distribution with bounds specified in the configuration.

```bash
./random
```

Output: a file containing the initial $(b_i, k_i)$ pairs for all $N$ solitons.

---

### 2. `./evolve` — Time-evolve the soliton poles

Takes a series of timesteps as input and evolves the pole positions by diagonalizing the $N \times N$ pole matrix at each step.

The diagonalization is implemented in a separate `.cpp` file to avoid re-importing Eigen headers on every compilation, keeping incremental build times short.

```bash
./evolve
```

Outputs two `.res` files:

| File | Contents |
|------|----------|
| `poles.res` | Complex pole positions at each timestep |
| `velocities.res` | Pole velocities at each timestep |

---

### 3. `./kinetics` — Hydrodynamic analysis

Reads pole positions at each timestep and performs the effective hydrodynamic analysis:

1. Divides the $x$-axis into spatial bins.
2. Assigns each pole to the bin corresponding to the real part of its position.
3. Constructs a local **Calogero-Moser Lax matrix** within each bin.
4. Diagonalizes each local matrix; eigenvalues become the **local rapidities**.
5. Writes results to a `.json`-formatted output file.

```bash
./kinetics
```

---

## Pipeline

```
./random  →  initial params
               ↓
./evolve  →  poles.res  +  velocities.res
               ↓
./kinetics  →  rapidities.json
```

---

## Configuration

Each binary currently has a local `Config` struct defined in its `main.cpp`. Key parameters include:

| Parameter | Description |
|-----------|-------------|
| `N` | Number of solitons |
| `delta_k` | Minimum allowed velocity separation |
| `b_min`, `b_max` | Bounds for uniform impact parameter distribution |
| `timesteps` | List of times at which to evaluate pole positions |
| `n_bins` | Number of spatial bins for kinetic analysis |

---

## TODO


---

## Project Structure

```
.
├── random/
│   └── main.cpp        # Soliton parameter sampling
├── evolve/
│   ├── main.cpp        # Time evolution driver
│   └── diagonalize.cpp # Eigen-based diagonalization (separate TU)
├── kinetics/
│   └── main.cpp        # Hydrodynamic analysis and binning
└── Makefile
```

---

## Background

The **Benjamin-Ono equation** is an integrable nonlinear dispersive PDE arising in the study of internal waves in deep stratified fluids. Its $N$-soliton solutions admit an exact representation in terms of poles in the complex upper half-plane. The dynamics of these poles are governed by a Calogero-Moser-type system, making them analytically tractable and numerically efficient to evolve.

