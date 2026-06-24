#!/usr/bin/env bash
#
# run_batch_sim.sh - sPHENIX tracker simulation in batch mode
#

set -euo pipefail

IMG=/cvmfs/sphenix.opensciencegrid.org/singularity/rhic_sl7_ext
SETUP=/cvmfs/sphenix.opensciencegrid.org/gcc-8.3/opt/sphenix/core/bin/sphenix_setup.sh
MACRODIR="$HOME/sPHENIX/3D_ClusterFindingML/macros-offline/detectors/sPHENIX"
COMMON="$HOME/sPHENIX/3D_ClusterFindingML/macros-offline/common"
MACRO="${1:-Fun4All_DistortionSim.C}"
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
    # The batch run cd's to $MACRODIR before launching root, and AlignmentTransformation
    # reads "./localAlignmentParamsFile.txt" from that CWD, so stage it there.
    #
    # TRANSCODE for ana.331 compatibility: newer dumps (ana494/ana537/...) write some
    # rows in the 10-column format (hitsetkey + 6 local params + 3 NEW global-rotation
    # params). ana.331's AlignmentTransformation only understands the 7-column format and
    # SIGSEGVs on the extra columns. We keep only the first 7 fields (the real local
    # alignment values are preserved; the keys are identical across builds), producing a
    # file ana.331 reads natively. ana331_* files are already 7-column, so this is a no-op
    # for them. This makes ANY alignment option safe -- no crash regardless of build.
    echo "--> Transcoding $(basename "${selected_file}") -> 7-column ana.331 alignment"
    awk 'NF>=7{print $1, $2, $3, $4, $5, $6, $7}' "${selected_file}" > "$MACRODIR/localAlignmentParamsFile.txt"
    echo "    wrote ${MACRODIR}/localAlignmentParamsFile.txt ($(wc -l < "$MACRODIR/localAlignmentParamsFile.txt") rows)"
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

# Run ROOT in batch mode (-b -q).
# ROOT_INCLUDE_PATH must include macros-offline/common so the LOCAL G4_TPC.C (distortions
# ON, merged static+module-edge map) is used instead of the CVMFS default (distortions OFF).
exec singularity exec "${BINDS[@]}" "$IMG" \
  bash -lc "source '$SETUP' -n ana.331 >/dev/null 2>&1; export ROOT_INCLUDE_PATH='$COMMON':'$MACRODIR':\$ROOT_INCLUDE_PATH; cd '$MACRODIR' && root -b -q '${MACRO}(${NEVENTS}, \"\", \"${OUTPUT_FILE}\")'"
