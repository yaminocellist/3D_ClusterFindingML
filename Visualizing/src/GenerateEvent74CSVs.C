void GenerateEvent74CSVs() {
  TString scriptPath = gSystem->DirName(gInterpreter->GetCurrentMacroName());
  if (scriptPath.IsNull()) scriptPath = ".";
  
  TString filename = scriptPath + "/../../clusters_seeds_island_79507-0.root_ntuplizer.root";
  const int targetEvent = 74;

  TFile *input = TFile::Open(filename, "READ");
  if (!input || input->IsZombie()) {
    printf("Could not open %s\n", filename.Data());
    return;
  }

  auto processTuple = [&](const char* tupleName, const char* csvOutputName) {
    TString csvOutputPath = scriptPath + "/../csv/" + csvOutputName;
    TNtuple *ntp = dynamic_cast<TNtuple *>(input->Get(tupleName));
    if (!ntp) {
      printf("Could not find TNtuple %s in %s\n", tupleName, filename.Data());
      return;
    }

    Float_t event, x, y, z, adc, zelem, layer;
    ntp->SetBranchAddress("event", &event);
    ntp->SetBranchAddress("x", &x);
    ntp->SetBranchAddress("y", &y);
    ntp->SetBranchAddress("z", &z);
    ntp->SetBranchAddress("adc", &adc);
    ntp->SetBranchAddress("zelem", &zelem);
    ntp->SetBranchAddress("layer", &layer);

    std::ofstream csv(csvOutputPath.Data());
    if (!csv) {
      printf("Could not open %s for writing\n", csvOutputPath.Data());
      return;
    }
    csv << "event,x,y,z,adc,zelem,layer\n";

    Long64_t nEntries = ntp->GetEntries();
    for (Long64_t i = 0; i < nEntries; ++i) {
      ntp->GetEntry(i);
      if (static_cast<int>(event) != targetEvent) continue;
      
      csv << targetEvent << ","
          << x << ","
          << y << ","
          << z << ","
          << adc << ","
          << zelem << ","
          << layer << "\n";
    }
    csv.close();
    printf("Generated %s from %s\n", csvOutputPath.Data(), tupleName);
  };

  processTuple("ntp_cluster", "event74_ntp_cluster.csv");
  processTuple("ntp_clus_trk", "event74_ntp_clus_trk.csv");

  input->Close();
}
