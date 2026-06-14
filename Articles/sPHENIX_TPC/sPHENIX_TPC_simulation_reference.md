# sPHENIX TPC — Simulation Reference (Geometry, B-field, pp Beam/EM Config)

**Purpose:** A single, source-cited sheet of the exact numbers needed to build a
Geant4 / Pythia8 simulation of the sPHENIX TPC for **p+p collisions at
√s = 200 GeV** at RHIC.

**Authority note.** Where possible every number below is taken from the *actual
sPHENIX simulation source code* (the `coresoftware` and `macros` repositories that
drive the official Fun4All Geant4 simulation), not from prose in a paper. Those
files are mirrored locally in `source_geometry/`. Paper PDFs (TDR-level prose,
motivation, test-beam validation) are in `papers/`. Citations are given as
`file:line` for code and as arXiv IDs for papers.

> ⚠️ The sPHENIX detector evolved. The **as-built / current** gas is
> **Ar:CF₄:isobutane = 75:20:5**, *not* the Ne-CF₄ from early design papers. Both
> are documented below — use the as-built numbers for a realistic 2023+ simulation.

---

## 1. TPC active gas volume (the tracking volume)

All from `source_geometry/PHG4TpcSubsystem.cc` `SetDefaultParameters()` and
`PHG4TpcDetector.cc`.

| Quantity | Value | Source |
|---|---|---|
| Gas inner radius | **21.6 cm** | `PHG4TpcSubsystem.cc:142` (`gas_inner_radius`) |
| Gas outer radius | **76.4 cm** | `PHG4TpcSubsystem.cc:143` (`gas_outer_radius`) |
| Full TPC length (z) | **205.21 cm** | `PHG4TpcSubsystem.cc:150` (`tpc_length`) = 2 × (102.325 + 0.28) |
| Max drift length (per side) | **102.325 cm** (CM face → top of GEM stack) | `PHG4TpcSubsystem.cc:220`, `G4_TrkrVariables.C:213` |
| Pseudorapidity acceptance | **\|η\| < 1.1** | arXiv:2110.02082, arXiv:2212.05541 |
| Gas temperature | **15 °C** | `PHG4TpcSubsystem.cc:263` |
| Gas pressure | **1.0 atm** | `PHG4TpcSubsystem.cc:264` |

The active volume is a cylinder split into a north and south drift region by the
central membrane (cathode) at z = 0. Drift is toward the two endcaps.

### Active readout region (where pads actually sit)

The instrumented radial span is divided into three radial module rings (R1/R2/R3),
each with 16 layers → **48 readout layers total**.

| Module | Layers | r_min (cm) | r_max (cm) | φ-sector size (rad) | φ-bins (total) | Source |
|---|---|---|---|---|---|---|
| Inner (R1) | 16 | **31.105** | **40.249** | 0.5024 | 1128 (= 94 × 12) | `PHG4TpcSubsystem.cc:207,212,216,255,259` |
| Mid (R2) | 16 | **41.153** | **57.475** | 0.5087 | 1536 (= 128 × 12) | `PHG4TpcSubsystem.cc:208,213,217,256,260` |
| Outer (R3) | 16 | **58.367** | **75.911** | 0.5097 | 2304 (= 192 × 12) | `PHG4TpcSubsystem.cc:209,214,218,257,261` |

- 12 azimuthal sectors per ring (`n_sectors = 12`), 3 radial modules per sector
  (`n_radial_modules = 3`) → 36 readout modules per endcap.
  (`PHG4TpcEndCapSubsystem.cc:137-138`)
- Nominal pad pitch ≈ **2 mm**, with **zigzag-patterned pads** for charge sharing
  to reach ~150–250 µm rφ resolution (arXiv:1801.03087, arXiv:2110.02082).
- `tpc_minlayer_inner = 7` is the global layer-numbering offset (TPC layers are
  numbered 7…54 in the combined tracker, after MVTX+INTT). (`PHG4TpcSubsystem.cc:210`)

---

## 2. Gas mixtures (drift + diffusion)

From `G4_TrkrVariables.C` namespace `G4TPC` and `PHG4TpcSubsystem.cc`.

**As-built / current default — `ArCF4Isobutane`** (`G4_TrkrVariables.C:170,204-211`,
`PHG4TpcSubsystem.cc:265-269`):

| Component | Fraction |
|---|---|
| Ar | **0.75** |
| CF₄ | **0.20** |
| isobutane (iC₄H₁₀) | **0.05** |
| Ne | 0.00 |
| N₂ | 0.00 |

- Drift velocity (sim): **0.00755 cm/ns** (= 7.55 µm/ns ≈ 75.5 µm/ns? → 7.55 cm/µs)
  (`G4_TrkrVariables.C:173,204`)
- Longitudinal diffusion: 0.014596 ; Transverse diffusion: 0.005313
  (`G4_TrkrVariables.C:205-206`)

**Other gas options defined in the code** (for reference / older studies):

| Mixture | Ne | Ar | CF₄ | N₂ | iso | v_drift (cm/ns) |
|---|---|---|---|---|---|---|
| `NeCF4` (early design) | 0.50 | 0 | 0.50 | 0 | 0 | 0.008 |
| `ArCF4` | 0 | 0.60 | 0.40 | 0 | 0 | 0.008 |
| `ArCF4N2` | 0 | 0.65 | 0.25 | 0.10 | 0 | 0.006965 |
| `ArCF4Isobutane` ✅ | 0 | 0.75 | 0.20 | 0 | 0.05 | 0.00755 |

(`G4_TrkrVariables.C:177-211`)

**Component gas densities used in Geant4** (`PHG4TpcDetector.cc:138-147`):
CF₄ from `sphenix_constants::CF4_density`; N₂ = 1.165 mg/cm³; isobutane = 2.59 mg/cm³;
mixed at T = 15 °C, P = 1 atm.

---

## 3. Field cage, central membrane, and support (passive material)

### Central membrane (cathode, "window") — at z = 0

Total thickness **0.56 cm** (`window_thickness`, `PHG4TpcSubsystem.cc:195`),
half-width 0.28 cm. Layered "ENIG / FR4 / ROHACELL foam" sandwich
(`PHG4TpcDetector.cc:181-259`):

| Layer | Material | Thickness | Source |
|---|---|---|---|
| Outer skins (×2) | G4_Cu (ENIG surface) | 8.9 µm each | `PHG4TpcSubsystem.cc:199` |
| 2nd skins (×2) | FR4 | 127 µm each | `PHG4TpcSubsystem.cc:202` |
| Core | ROHACELL_FOAM_51 | remainder | `PHG4TpcSubsystem.cc:194` |

### Field cage walls (inner + outer, 9 layers each)

From `PHG4TpcSubsystem.cc:159-184`, built radially in `PHG4TpcDetector.cc:400-461`.
The inner cage wall sits just inside r = 21.6 cm; the outer just outside r = 76.4 cm.

| # | Material | Thickness (cm) |
|---|---|---|
| 1 | G4_Cu | 0.00347/2 = 0.001735 |
| 2 | FR4 | 0.025 |
| 3 | NOMEX (honeycomb) | 0.5 × 2.54 = 1.27 |
| 4 | G4_Cu | 0.001735 |
| 5 | FR4 | 0.025 |
| 6 | G4_KAPTON | 0.2286 |
| 7 | G4_Cu | 0.001735 |
| 8 | G4_KAPTON | 0.05 (50 µm) |
| 9 | G4_Cu | 0.001735 |

### Mechanical support (in `PHG4TpcDetector.cc`)
- 12 carbon-fiber tie rods, OD ~1", wall 1/8", at r ≈ 31.5" (`PHG4TpcDetector.cc:363-385`)
- Hanger beams / supports in stainless (`PHG4TpcDetector.cc:300-346`)

---

## 4. Endcap: GEM stack + readout + electronics (per side)

From `PHG4TpcEndCapDetector.cc` and `PHG4TpcEndCapSubsystem.cc`.

- **Quadruple-GEM** stack: `n_GEM_layers = 4` (`PHG4TpcEndCapSubsystem.cc:133`).
- Per-GEM effective buildup (`PHG4TpcEndCapDetector.cc:131-152`):
  Cu 2×5 µm + Kapton 50 µm + gas 2 mm, modeled as composite `GEMeffective`, ×4.
- Readout PCB: Cu 35 µm/layer + FR4 board (7 mil × n layers)
  (`PHG4TpcEndCapDetector.cc:157-160`).
- Endcap envelope: r_min 20 cm, r_max 76.27 cm, front face at z = 205.21/2 = 102.605 cm
  (`PHG4TpcEndCapSubsystem.cc:129-131`).
- Aluminum "wagon wheel" support frame, 12 sectors, with R1/R2/R3 radial frame
  boundaries (`PHG4TpcEndCapSubsystem.cc:140-167`).
- Front-end electronics (SAMPA-based): nFEE R1 = 6, R2 = 8, R3 = 12 per sector
  (`PHG4TpcEndCapSubsystem.cc:170-172`).

---

## 5. Magnetic field

- **Nominal solenoid field: 1.4 T** (the former BaBar superconducting solenoid),
  axial, along beam (z). Confirmed in arXiv:2110.02082, arXiv:2212.05541.
- In simulation the field is a **3-D field map**, not a constant:
  `FIELDMAP_TRACKING` / `FIELDMAP_GAP` / `FIELDMAP_STEEL`
  (`G4_Magnet.C:42-50`). A rescale factor `magfield_rescale` (default 1.0) scales
  the map (`G4_Magnet.C:36-39`, `GlobalVariables.C:101`).
- Reco drift velocity tied to field/gas: `tpc_drift_velocity_reco = 0.00755 cm/ns`
  (`GlobalVariables.C:120`).
- Solenoid cryostat geometry (`G4_Magnet.C:29-32, 80-126`):
  inner cryostat wall r ≈ 142 cm, coil r ≈ 150.8 cm (thick 9.38 cm),
  outer cryostat wall r = 174.5 cm (thick 2.5 cm), magnet length 379 cm,
  material Al5083 (~1 X₀).

**For a constant-field Geant4 quick start:** use **B = (0, 0, 1.4 T)** uniform
inside r < 142 cm. For realistic tracking, use the sPHENIX field map (ships with
`calibrations`/CDB; map file referenced by the macros).

---

## 6. Beampipe & interaction region (p+p)

From `G4_Pipe.C` (mid-rapidity Be section is the relevant one for the TPC).

| Quantity | Value | Source |
|---|---|---|
| Be pipe inner radius | **2.00025 cm** (⌀ 4.0005 cm) | `G4_Pipe.C:32` |
| Be pipe wall thickness | **0.0762 cm = 0.762 mm** | `G4_Pipe.C:33` |
| Be pipe material | **G4_Be** (beryllium), vacuum (G4_Galactic) inside | `G4_Pipe.C:116,133` |
| Be pipe length | **120.015 cm** (47.25") | `G4_Pipe.C:34` |
| Be pipe z-shift (south) | −41.1639 cm | `G4_Pipe.C:35` |

Beyond the Be section the pipe transitions to aluminum (`al_pipe`, r same 2.00025 cm,
wall 1.397 mm, `G4_Pipe.C:37-41`) then conical extensions toward the forward region
(`G4_Pipe.C:46-52`). Forward beamline elements are in `G4_BeamLine.C`.

### Collision / vertex configuration for p+p
- **√s = 200 GeV** p+p (also Au+Au √s_NN = 200 GeV). RHIC Run-2024 was the p+p run.
  (arXiv:2410.18031; `papers/sPHENIX_Beam_Use_Proposal_2023.pdf`)
- Interaction point at the geometric center (z = 0); the primary-vertex z is
  smeared per-event (Gaussian) in the event generator stage — set the σ_z to match
  the RHIC bunch (typically σ_z ≈ 5–10 cm for p+p; the exact value is configured in
  the event-generation macro / vertex generator, not fixed in geometry).
- **Pileup:** at p+p collision rates, configure pileup via the Fun4All pileup
  generator. Space-charge / distortion modeling assumes a collision rate handle
  `SC_COLLISIONRATE = 50 kHz` in tracking (`G4_TrkrVariables.C:225`).
- Luminosity / run plan: see `papers/sPHENIX_Beam_Use_Proposal_2023.pdf`.

---

## 7. Pythia8 + Geant4 wiring (how sPHENIX does it)

- **Event generation:** sPHENIX uses **PHPythia8** (a Fun4All wrapper around
  Pythia8) for p+p at 200 GeV. A minimal config: `Beams:eCM = 200`,
  `Beams:idA = 2212`, `Beams:idB = 2212`, with `SoftQCD:nonDiffractive = on` (MB)
  or `HardQCD`/`PromptPhoton`/`Charmonium`/`Bottomonium` for signal samples
  (jets, Υ). The Pythia8 card lives in the macros (e.g. `phpythia8.cfg`).
- **Vertex:** a vertex generator smears the primary vertex (Gaussian in x,y,z)
  before tracking.
- **Detector + transport:** `Fun4All_G4_sPHENIX.C` (top macro) calls the per-detector
  builders. The TPC chain is enabled via `Enable::TPC` / `Enable::TPC_ENDCAP` and
  built by `G4_TrkrSimulation.C` → `PHG4TpcSubsystem` / `PHG4TpcEndCapSubsystem`.
- Digitization: drift + diffusion + SAMPA response + zigzag charge sharing modeled
  in `g4tpc` (PHG4TpcElectronDrift, PHG4TpcPadPlane, etc.).

---

## 8. Quick-reference constants block (copy/paste)

```text
# --- TPC active gas volume ---
gas_inner_radius     = 21.6   cm
gas_outer_radius     = 76.4   cm
tpc_full_length_z    = 205.21 cm   (±102.605 cm)
max_drift_length     = 102.325 cm  (per side)
eta_acceptance       = |eta| < 1.1
n_readout_layers     = 48          (16 inner + 16 mid + 16 outer)
T, P                 = 15 C, 1.0 atm

# --- readout rings (r in cm) ---
R1 (inner): 31.105 -> 40.249 ,  1128 phi-bins
R2 (mid)  : 41.153 -> 57.475 ,  1536 phi-bins
R3 (outer): 58.367 -> 75.911 ,  2304 phi-bins
sectors = 12 ,  radial_modules = 3 ,  GEM_layers = 4

# --- gas (as-built) ---
mixture   = Ar:CF4:isobutane = 0.75 : 0.20 : 0.05
v_drift   = 0.00755 cm/ns
diff_long = 0.014596 ,  diff_trans = 0.005313

# --- magnetic field ---
B = 1.4 T (axial, +z) ; realistic = sPHENIX 3D field map

# --- beampipe (mid-rapidity) ---
be_pipe_inner_radius = 2.00025 cm
be_pipe_thickness    = 0.0762  cm  (0.762 mm)
be_pipe_material     = beryllium (G4_Be), vacuum inside
be_pipe_length       = 120.015 cm

# --- collisions ---
system  = p+p ,  sqrt(s) = 200 GeV ,  vertex at z=0 (Gaussian-smeared)
```

---

## 9. Source files (saved locally)

**Geometry source code** → `source_geometry/`
- `PHG4TpcSubsystem.cc` — TPC default parameters (radii, layers, gas, φ-bins)
- `PHG4TpcDetector.cc` — TPC G4 volume construction (gas, field cage, CM, supports)
- `PHG4TpcEndCapSubsystem.cc` / `PHG4TpcEndCapDetector.cc` — GEM stack, wagon wheel, FEE
- `G4_TrkrVariables.C` — gas mixtures, drift velocities, layer counts (macro level)
- `G4_TrkrSimulation.C` — TPC subsystem wiring into Fun4All
- `Trkr_TpcReadoutInit.C` — readout/clock init
- `G4_Magnet.C` — solenoid geometry + field map selection
- `G4_Pipe.C` — beampipe (Be + Al sections)
- `GlobalVariables.C`, `G4_World.C` — global field/drift handles, world volume

**Papers / documents** → `papers/` (see `../README.md` for full citation list)
