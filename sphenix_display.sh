#!/usr/bin/env bash
#
# sphenix_display.sh - sPHENIX tracker geometry / event display, fully OFFLINE.
#
# WHY THIS EXISTS: modern sPHENIX builds (ana.4xx via "-n") resolve calibrations
# AND even the TPC readout geometry through the BNL conditions DB
# (nopayloaddb.apps.rcf.bnl.gov), which is not reachable without a BNL account.
# So they crash offline. The last fully file-based builds are the frozen gcc-8.3
# 2022 line (ana.331), which need NO conditions DB. This launcher pairs that build
# with era-matched macros (~/sPHENIX/macros-offline, git worktree @ 2022-11-16) so
# the display runs with no network DB. See CVMFS_SETUP.md section 7.
#
# Run this INSIDE your VM desktop session (needs X for the Eve display window).
#
# Usage:
#   ./sphenix_display.sh                 # opens display with plot_trackers.C
#   ./sphenix_display.sh some_macro.C    # run a different macro from macros-offline
#
set -euo pipefail

IMG=/cvmfs/sphenix.opensciencegrid.org/singularity/rhic_sl7_ext
SETUP=/cvmfs/sphenix.opensciencegrid.org/gcc-8.3/opt/sphenix/core/bin/sphenix_setup.sh
MACRODIR="$HOME/sPHENIX/3D_ClusterFindingML/macros-offline/detectors/sPHENIX"
MACRO="${1:-plot_trackers_ver2.C}"

if [[ ! -e /cvmfs/sphenix.opensciencegrid.org ]]; then
  echo "ERROR: CVMFS not mounted. Try: cvmfs_config probe sphenix.opensciencegrid.org" >&2
  exit 1
fi
if [[ ! -f "$MACRODIR/$MACRO" ]]; then
  echo "ERROR: $MACRODIR/$MACRO not found." >&2
  exit 1
fi

# Bind the X11 socket so the Geant4/ROOT Eve window can open on your desktop.
BINDS=(-B /cvmfs:/cvmfs)
[[ -d /tmp/.X11-unix ]] && BINDS+=(-B /tmp/.X11-unix:/tmp/.X11-unix)

# If the window is refused, run `xhost +local:` once on the host first.
exec singularity exec "${BINDS[@]}" --env DISPLAY="${DISPLAY:-:0}" "$IMG" \
  bash -lc "source '$SETUP' -n ana.331 >/dev/null 2>&1; cd '$MACRODIR' && root -l '$MACRO'"
