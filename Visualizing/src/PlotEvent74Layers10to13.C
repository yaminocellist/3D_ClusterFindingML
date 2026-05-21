void PlotEvent74Layers10to13() {
  gStyle->SetOptStat(0);
  gStyle->SetPalette(kBird);

  // Automatically determine project root relative to current directory (assumed to be Visualizing/src)
  TString currentDir = gSystem->pwd();
  TString projectRoot = currentDir + "/../..";
  TString filename = projectRoot + "/clusters_seeds_island_79507-0.root_ntuplizer.root";
  const int targetEvent = 74;
  const int firstLayer = 10;
  const int lastLayer = 13;

  TFile *input = TFile::Open(filename, "READ");
  if (!input || input->IsZombie()) {
    Error("PlotEvent74Layers10to13", "Could not open %s", filename.Data());
    return;
  }

  TNtuple *ntp = (TNtuple *)input->Get("ntp_hit");
  if (!ntp) {
    Error("PlotEvent74Layers10to13", "Could not find TNtuple ntp_hit");
    input->Close();
    return;
  }

  TCanvas *c = new TCanvas("c_layers10to13", "Event 74 layers 10 to 13", 1800, 450);
  c->Divide(4, 1, 0.001, 0.001);

  const double xEdges[5] = {0.0, 0.2760, 0.5, 0.7240, 1.0};
  const double labelSize = 0.07;

  for (int layer = firstLayer; layer <= lastLayer; ++layer) {
    const int ipad = layer - firstLayer + 1;
    c->cd(ipad);
    
    gPad->SetPad(xEdges[ipad - 1], 0.0, xEdges[ipad], 1.0);
    gPad->SetLogz(1);

    TString histName = Form("h_layer_%d", layer);
    // Note: phibin:tbin means tbin is X and phibin is Y
    TString drawExpr = Form("phibin:tbin>>%s(200,300,500,200,50,250)", histName.Data());
    TString cut = Form("adc*(event==%d&&layer==%d&&adc>0)", targetEvent, layer);

    ntp->Draw(drawExpr, cut, "COLZ"); // Using COLZ to see the color scale

    TH2F *h = (TH2F *)gDirectory->Get(histName);
    if (h) {
      // --- FIX 1: DETACH FROM FILE ---
      h->SetDirectory(0); 
      
      // --- FIX 2: CONTENT VALIDATION ---
      if (h->GetEntries() == 0) {
          Warning("Plot", "Layer %d has NO hits in the phibin[50-250], tbin[300-500] window!", layer);
      }

      h->SetTitle(Form("Layer %d;Time bin;Phi bin", layer));
      h->GetYaxis()->SetRangeUser(50, 250);
      h->GetXaxis()->SetRangeUser(301, 499);
      // (Rest of your styling remains the same...)
      h->GetXaxis()->SetLabelSize(labelSize);
      h->GetYaxis()->SetLabelSize(ipad == 1 ? labelSize : 0);
    }
  }

  // --- FIX 3: REFRESH BEFORE SAVING ---
  c->Modified();
  c->Update();
  TString outputPath = currentDir + "/../images/event74_layers10to13.png";
  c->SaveAs(outputPath);
  
  // Safe to close now
  input->Close();
}