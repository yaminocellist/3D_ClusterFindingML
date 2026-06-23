#include <TFile.h>
#include <TTree.h>
#include <iostream>

void flatten_hits() {
    TFile *fin = TFile::Open("macros-offline/detectors/sPHENIX/G4sPHENIX_DSTReader.root");
    if (!fin || fin->IsZombie()) {
        std::cerr << "Could not open input file." << std::endl;
        return;
    }
    TTree *Tin = (TTree*)fin->Get("T");
    if (!Tin) {
        std::cerr << "Could not find tree T." << std::endl;
        return;
    }

    Int_t n_G4HIT_TPC;
    // TPC hit array max size
    const int MAXHITS = 200000;
    Float_t x[MAXHITS][2], y[MAXHITS][2], z[MAXHITS][2], edep[MAXHITS];
    Int_t scint_id[MAXHITS];

    Tin->SetBranchAddress("n_G4HIT_TPC", &n_G4HIT_TPC);
    Tin->SetBranchAddress("G4HIT_TPC.x[2]", x);
    Tin->SetBranchAddress("G4HIT_TPC.y[2]", y);
    Tin->SetBranchAddress("G4HIT_TPC.z[2]", z);
    Tin->SetBranchAddress("G4HIT_TPC.scint_id", scint_id);
    Tin->SetBranchAddress("G4HIT_TPC.edep", edep);

    TFile *fout = new TFile("simulated_ntp_hit.root", "RECREATE");
    TTree *Tout = new TTree("ntp_hit", "Flattened TPC hits");

    Float_t out_x, out_y, out_z, out_edep;
    Float_t out_layer;

    Tout->Branch("x", &out_x, "x/F");
    Tout->Branch("y", &out_y, "y/F");
    Tout->Branch("z", &out_z, "z/F");
    Tout->Branch("layer", &out_layer, "layer/F");
    Tout->Branch("edep", &out_edep, "edep/F");

    Long64_t entries = Tin->GetEntries();
    Long64_t total_hits = 0;
    for (Long64_t i = 0; i < entries; ++i) {
        Tin->GetEntry(i);
        for (Int_t j = 0; j < n_G4HIT_TPC; ++j) {
            out_x = (x[j][0] + x[j][1])/2.0;
            out_y = (y[j][0] + y[j][1])/2.0;
            out_z = (z[j][0] + z[j][1])/2.0;
            out_layer = (Float_t)scint_id[j];
            out_edep = edep[j];
            Tout->Fill();
            total_hits++;
        }
    }

    fout->cd();
    Tout->Write();
    fout->Close();
    fin->Close();

    std::cout << "Successfully flattened " << total_hits << " hits into simulated_ntp_hit.root!" << std::endl;
}
