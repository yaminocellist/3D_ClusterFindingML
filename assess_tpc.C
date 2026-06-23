#include <TFile.h>
#include <TTree.h>
#include <iostream>

void print_tpc_stats(const char* filename, const char* tree_name) {
    TFile *f = TFile::Open(filename);
    if (!f || f->IsZombie()) {
        std::cerr << "Error opening " << filename << std::endl;
        return;
    }
    
    TTree *t = nullptr;
    if (f->GetListOfKeys()->Contains(tree_name)) {
        t = (TTree*)f->Get(tree_name);
    } else {
        TIter next(f->GetListOfKeys());
        TKey *key;
        while ((key = (TKey*)next())) {
            TClass *cl = gROOT->GetClass(key->GetClassName());
            if (cl && cl->InheritsFrom(TTree::Class())) {
                t = (TTree*)key->ReadObj();
                std::cout << "Found tree: " << key->GetName() << std::endl;
                break;
            }
        }
    }
    
    if (!t) {
        std::cerr << "No tree found in " << filename << std::endl;
        f->Close();
        return;
    }
    
    std::cout << "--- Stats for " << filename << " ---" << std::endl;
    std::cout << "Tree name: " << t->GetName() << std::endl;
    std::cout << "Entries: " << t->GetEntries() << std::endl;
    
    // Check for G4DSTreader n_G4HIT_TPC
    if (t->GetBranch("n_G4HIT_TPC")) {
        Int_t n_G4HIT_TPC;
        t->SetBranchAddress("n_G4HIT_TPC", &n_G4HIT_TPC);
        Long64_t total_hits = 0;
        Long64_t entries = t->GetEntries();
        for (Long64_t i = 0; i < entries; ++i) {
            t->GetEntry(i);
            total_hits += n_G4HIT_TPC;
        }
        std::cout << "Total TPC Hits: " << total_hits << std::endl;
        std::cout << "Avg TPC Hits per event: " << (double)total_hits / entries << std::endl;
    } 
    // Check for old ntp_hit style tree
    else if (t->GetBranch("layer")) {
        Long64_t entries = t->GetEntries();
        Float_t layer;
        t->SetBranchAddress("layer", &layer);
        Long64_t tpc_hits = 0;
        for (Long64_t i = 0; i < entries; ++i) {
            t->GetEntry(i);
            if (layer >= 7 && layer <= 54) {
                tpc_hits++;
            }
        }
        std::cout << "Total TPC Hits: " << tpc_hits << std::endl;
        std::cout << "Hits per event: " << (double)tpc_hits / t->GetEntries() << std::endl;
    }
    else {
        std::cout << "Could not find recognizable TPC branches ('layer', 'n_G4HIT_TPC')." << std::endl;
    }
    
    f->Close();
}

void assess_tpc() {
    std::cout << "=========================================" << std::endl;
    print_tpc_stats("clusters_seeds_island_79507-0.root_ntuplizer.root", "ntp_hit");
    std::cout << "=========================================" << std::endl;
    print_tpc_stats("macros-offline/detectors/sPHENIX/G4sPHENIX_g4svtx_eval.root", "ntp_hit");
    std::cout << "=========================================" << std::endl;
}
