void check_trees() {
    TFile *f = TFile::Open("/home/yaminocellist/sPHENIX/3D_ClusterFindingML/clusters_seeds_island_79507-0.root_ntuplizer.root");
    if (f) {
        f->ls();
        // Assume there's a tree, let's print the structure of the first tree found
        TIter next(f->GetListOfKeys());
        TKey *key;
        while ((key = (TKey*)next())) {
            TClass *cl = gROOT->GetClass(key->GetClassName());
            if (!cl) continue;
            if (cl->InheritsFrom(TTree::Class())) {
                TTree *T = (TTree*)key->ReadObj();
                T->Print();
                break;
            }
        }
        f->Close();
    }
}
