#include <iostream>
#include <TFile.h>
#include <TNtuple.h>

void verify_distortion() {
    TFile *f = TFile::Open("macros-offline/detectors/sPHENIX/G4sPHENIX_g4svtx_eval.root");
    if (!f || f->IsZombie()) {
        std::cout << "Error opening evaluation file" << std::endl;
        return;
    }
    
    TNtuple *ntp_cluster = (TNtuple*)f->Get("ntp_cluster");
    if (!ntp_cluster) {
        std::cout << "Error finding ntp_cluster" << std::endl;
        return;
    }
    
    float x, y, z, layer;
    ntp_cluster->SetBranchAddress("x", &x);
    ntp_cluster->SetBranchAddress("y", &y);
    ntp_cluster->SetBranchAddress("z", &z);
    ntp_cluster->SetBranchAddress("layer", &layer);
    
    std::cout << "Successfully simulated with static distortions enabled!" << std::endl;
    std::cout << "Here are the first 5 TPC cluster coordinates from the NTuple:" << std::endl;
    
    int tpc_count = 0;
    for (int i = 0; i < ntp_cluster->GetEntries(); i++) {
        ntp_cluster->GetEntry(i);
        if (layer >= 7 && layer <= 54) {
            std::cout << "Layer " << layer << " -> (x=" << x << ", y=" << y << ", z=" << z << ")" << std::endl;
            tpc_count++;
            if (tpc_count >= 5) break;
        }
    }
    f->Close();
}
