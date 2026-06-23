source /cvmfs/sphenix.opensciencegrid.org/gcc-12.1.0/opt/sphenix/core/bin/sphenix_setup.sh -n ana.494
export USE_CDB_OFFLINE=0
cd /home/yaminocellist/sPHENIX/3D_ClusterFindingML/macros-gcc12/detectors/sPHENIX/
root -l -b -q 'plot_trackers_gcc12.C(1, "pi-")'
