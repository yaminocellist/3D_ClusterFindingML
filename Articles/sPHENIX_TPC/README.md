# sPHENIX TPC — Reference Collection for Geant4 / Pythia8 Simulation

Collected 2026-06-05 to support p+p (√s = 200 GeV) simulations of the sPHENIX TPC.

**Start here:** [`sPHENIX_TPC_simulation_reference.md`](sPHENIX_TPC_simulation_reference.md)
— a single source-cited sheet of every geometry / B-field / beam number you need,
extracted from the actual sPHENIX simulation source code.

---

## Folder layout

```
sPHENIX_TPC/
├── README.md                              (this file)
├── sPHENIX_TPC_simulation_reference.md    (THE synthesis — exact numbers + citations)
├── papers/                                (PDFs: TDR-level prose, design, results)
└── source_geometry/                       (the real Geant4 geometry source code)
```

---

## papers/ — downloaded documents

| File | What it is | Source URL |
|---|---|---|
| `arXiv_1501.06197_sPHENIX_proposal.pdf` | "An Upgrade Proposal from the PHENIX Collaboration" — the foundational sPHENIX physics + detector concept doc | https://arxiv.org/abs/1501.06197 |
| `arXiv_2110.02082.pdf` | **"Overview and Design of the sPHENIX TPC"** (H. Klest) — gas, quad-GEM, zigzag pads, 1.4 T, \|η\|<1.1 | https://arxiv.org/abs/2110.02082 |
| `arXiv_2212.05541.pdf` | **"A Compact TPC for the sPHENIX Experiment"** (Klest, TIPP2021) — 20<r<78 cm, Ar-CF4, test beam | https://arxiv.org/abs/2212.05541 |
| `arXiv_1801.03087.pdf` | "Design Studies for a TPC Readout Plane Using Zigzag Patterns with Multistage GEM Detectors" — ~40 layers, 250 µm res, 2 mm pads | https://arxiv.org/abs/1801.03087 |
| `arXiv_2410.18031.pdf` | "sPHENIX Highlights: First Results" (SQM2024) — Run-2024 p+p, K0s/Λ via TPC | https://arxiv.org/abs/2410.18031 |
| `PoS_2017_075_sPHENIX_TPC.pdf` | PoS proceedings on the sPHENIX TPC | https://pos.sissa.it/322/075/a1.pdf |
| `sPHENIX_NSS2017_overview.pdf` | sPHENIX detector overview slides (Purschke, NSS 2017) | https://www.phenix.bnl.gov/~purschke/NSS2017/sPHENIX.pdf |
| `sPHENIX_Beam_Use_Proposal_2023.pdf` | Official run plan / luminosity / species / √s — beam & IR config | https://indico.bnl.gov/event/20373/attachments/49393/84303/sPHENIX_Beam_Use_Proposal_2023.pdf |

## source_geometry/ — sPHENIX Geant4 geometry source (the ground truth)

From the official repos (master branch as of download):
- `coresoftware`: https://github.com/sPHENIX-Collaboration/coresoftware
- `macros`: https://github.com/sPHENIX-Collaboration/macros

| File | Repo path |
|---|---|
| `PHG4TpcSubsystem.cc` | `coresoftware/simulation/g4simulation/g4tpc/` |
| `PHG4TpcDetector.cc` | `coresoftware/simulation/g4simulation/g4tpc/` |
| `PHG4TpcEndCapSubsystem.cc` | `coresoftware/simulation/g4simulation/g4tpc/` |
| `PHG4TpcEndCapDetector.cc` | `coresoftware/simulation/g4simulation/g4tpc/` |
| `G4_TrkrVariables.C` | `macros/common/` |
| `G4_TrkrSimulation.C` | `macros/common/` |
| `Trkr_TpcReadoutInit.C` | `macros/common/` |
| `G4_Magnet.C` | `macros/common/` |
| `G4_Pipe.C` | `macros/common/` |
| `GlobalVariables.C` | `macros/common/` |
| `G4_World.C` | `macros/common/` |

---

## Still useful to grab (not auto-downloadable here)

- **sPHENIX Technical Design Report (TDR)** and **TPC TDR** — internal/collaboration
  documents on the sPHENIX docDB / indico. Search the collaboration docDB
  (https://sphenix-docdb.bnl.gov/) or indico.bnl.gov for "TPC TDR".
- The full **Fun4All top macro** `Fun4All_G4_sPHENIX.C` and the **PHPythia8**
  example config (`macros/detectors/sPHENIX/`) for the exact event-generation +
  transport wiring.
- The **field map ROOT file** (sPHENIX 3-D solenoid map) — shipped via the
  sPHENIX CDB / `calibrations` repo.

## How to refresh the geometry source

```bash
cd source_geometry
base=https://raw.githubusercontent.com/sPHENIX-Collaboration
curl -sLO $base/coresoftware/master/simulation/g4simulation/g4tpc/PHG4TpcSubsystem.cc
curl -sLO $base/coresoftware/master/simulation/g4simulation/g4tpc/PHG4TpcDetector.cc
# ...etc (see table above for paths)
```
