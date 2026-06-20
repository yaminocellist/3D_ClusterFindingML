#!/usr/bin/env bash
#
# run_batch_sim.sh - sPHENIX tracker simulation in batch mode
#

set -euo pipefail

IMG=/cvmfs/sphenix.opensciencegrid.org/singularity/rhic_sl7_ext
SETUP=/cvmfs/sphenix.opensciencegrid.org/gcc-8.3/opt/sphenix/core/bin/sphenix_setup.sh
MACRODIR="$HOME/sPHENIX/3D_ClusterFindingML/macros-offline/detectors/sPHENIX"
MACRO="${1:-plot_trackers_ver2.C}"
CDB_DIR="$HOME/sPHENIX/3D_ClusterFindingML/CDB_offline"

echo "========================================="
echo "      sPHENIX Batch Simulation Config    "
echo "========================================="
echo "Select CDB Alignment Configuration:"
echo "1) OFFLINE mode (No DB, Tracker geometry only, fast event display)"

# Find all .txt files in TRACKINGALIGNMENT and store them in an array
CDB_DIR="./CDB_offline/TRACKINGALIGNMENT"
files=()
if [ -d "$CDB_DIR" ]; then
    while IFS=  read -r -d $'\0'; do
        files+=("$REPLY")
    done < <(find "$CDB_DIR" -type f -name "*.txt" -print0 | sort -z)
fi

# Print dynamic options
idx=2
for file in "${files[@]}"; do
    filename=$(basename "$file")
    # strip hash prefix if it exists (assuming 32 hex chars + underscore)
    display_name=$(echo "$filename" | sed -E 's/^[0-9a-f]{32}_//')
    echo "${idx}) ${display_name}"
    ((idx++))
done

read -p "Choice [1]: " CDB_CHOICE
CDB_CHOICE=${CDB_CHOICE:-1}

if [ "$CDB_CHOICE" == "1" ]; then
    export USE_CDB_OFFLINE=0
elif [ "$CDB_CHOICE" -gt 1 ] && [ "$CDB_CHOICE" -lt "$idx" ]; then
    export USE_CDB_OFFLINE=1
    # Array index is Choice - 2
    arr_idx=$((CDB_CHOICE - 2))
    selected_file="${files[$arr_idx]}"
    echo "--> Copying ${selected_file} to localAlignmentParamsFile.txt"
    cp "${selected_file}" ./localAlignmentParamsFile.txt
else
    echo "Invalid choice. Defaulting to OFFLINE mode."
    export USE_CDB_OFFLINE=0
fi

echo ""
read -e -p "Number of Events [100]: " NEVENTS
NEVENTS=${NEVENTS:-100}

read -e -p "Particle Type (e.g. pi-, e-, gamma) [pi-]: " PARTICLE_TYPE
export PARTICLE_TYPE=${PARTICLE_TYPE:-pi-}

read -e -p "Number of particles per event (e.g. 10, 60, 100) [60]: " PARTICLE_COUNT
export PARTICLE_COUNT=${PARTICLE_COUNT:-60}

read -e -p "Particle pT min (GeV/c) [0.1]: " PARTICLE_PT_MIN
export PARTICLE_PT_MIN=${PARTICLE_PT_MIN:-0.1}

read -e -p "Particle pT max (GeV/c) [20.0]: " PARTICLE_PT_MAX
export PARTICLE_PT_MAX=${PARTICLE_PT_MAX:-20.0}

read -e -p "Magnetic Field (e.g. 1.4, 1.5) [1.5]: " MAG_FIELD
export MAG_FIELD=${MAG_FIELD:-1.5}

read -p "Output file name [G4sPHENIX.root]: " OUTPUT_FILE
OUTPUT_FILE=${OUTPUT_FILE:-G4sPHENIX.root}

echo "========================================="
echo "Starting singularity container for batch mode..."

if [[ ! -e /cvmfs/sphenix.opensciencegrid.org ]]; then
  echo "ERROR: CVMFS not mounted. Try: cvmfs_config probe sphenix.opensciencegrid.org" >&2
  exit 1
fi
if [[ ! -f "$MACRODIR/$MACRO" ]]; then
  echo "ERROR: $MACRODIR/$MACRO not found." >&2
  exit 1
fi

BINDS=(-B /cvmfs:/cvmfs)

# Run ROOT in batch mode (-b -q)
exec singularity exec "${BINDS[@]}" "$IMG" \
  bash -lc "source '$SETUP' -n ana.331 >/dev/null 2>&1; cd '$MACRODIR' && root -b -q '${MACRO}(${NEVENTS}, \"\", \"${OUTPUT_FILE}\")'"
