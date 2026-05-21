void PlotEvent74AxisRanges() {
    // Set global style
    gStyle->SetOptStat(1110);

    // Automatically determine project root relative to current directory (assumed to be Visualizing/src)
    TString currentDir = gSystem->pwd();
    TString projectRoot = currentDir + "/../..";
    
    TString filename = projectRoot + "/clusters_seeds_island_79507-0.root_ntuplizer.root";
    const char *selection = "event==74&&adc>0";

    TFile *input = TFile::Open(filename, "READ");
    if (!input || input->IsZombie()) {
        Error("PlotEvent74AxisRanges", "Could not open %s", filename.Data());
        return;
    }

    TNtuple *ntp = (TNtuple*)input->Get("ntp_hit");
    if (!ntp) {
        Error("PlotEvent74AxisRanges", "Could not find TNtuple ntp_hit");
        input->Close();
        return;
    }

    // --- SOLUTION 2: CHECK FOR ENTRIES ---
    // Ensure the selection actually contains data before proceeding.
    long long nEntries = ntp->GetEntries(selection);
    std::cout << "Entries found for event 74 with ADC > 0: " << nEntries << std::endl;
    
    if (nEntries == 0) {
        Warning("PlotEvent74AxisRanges", "No hits found for selection: %s", selection);
        input->Close();
        return;
    }

    TCanvas *c = new TCanvas("c_event74_axes", "Event 74 axis ranges", 1500, 500);
    c->Divide(3, 1);

    // Drawing Layer
    c->cd(1);
    ntp->Draw("layer>>hLayer(54,2.5,56.5)", selection);
    // --- SOLUTION 1: DETACH FROM FILE DIRECTORY ---
    TH1F *hLayer = (TH1F*)gDirectory->Get("hLayer");
    if(hLayer) hLayer->SetDirectory(0); 

    // Drawing PhiBin
    c->cd(2);
    ntp->Draw("phibin>>hPhiBin(2304,-0.5,2303.5)", selection);
    TH1F *hPhiBin = (TH1F*)gDirectory->Get("hPhiBin");
    if(hPhiBin) hPhiBin->SetDirectory(0);

    // Drawing TBin
    c->cd(3);
    ntp->Draw("tbin>>hTBin(1015,-54.5,960.5)", selection);
    TH1F *hTBin = (TH1F*)gDirectory->Get("hTBin");
    if(hTBin) hTBin->SetDirectory(0);

    // --- SOLUTION 3: UPDATE & PAINT BEFORE SAVING ---
    // This forces ROOT to render the graphics buffer into the canvas object.
    c->Modified();
    c->Update();
    TString outputPath = currentDir + "/../images/event74_axis_ranges.png";
    c->SaveAs(outputPath);

    std::cout << "Event 74 nonzero-adc ntp_hit ranges plotted and saved to: " << outputPath << std::endl;

    // Now it is safe to close the file because the histograms are detached (Solution 1)
    input->Close();
}