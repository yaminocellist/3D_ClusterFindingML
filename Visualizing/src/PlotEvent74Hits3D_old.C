{
  gStyle->SetOptStat(0);
  gStyle->SetPalette(kBird);

  const char *filename = "../../clusters_seeds_island_79507-0.root_ntuplizer.root";
  const int targetEvent = 74;
  const int tBinMin = -54;
  const int tBinMax = 960;
  const int nColorBins = 14;
  const Long64_t maxPointsToDraw = 120000;

  TFile *input = TFile::Open(filename, "READ");
  if (!input || input->IsZombie()) {
    Error("PlotEvent74Hits3D", "Could not open %s", filename);
    return;
  }

  TNtuple *ntp = dynamic_cast<TNtuple *>(input->Get("ntp_hit"));
  if (!ntp) {
    Error("PlotEvent74Hits3D", "Could not find TNtuple ntp_hit in %s", filename);
    input->Close();
    return;
  }

  Float_t event = 0;
  Float_t layer = 0;
  Float_t phi = 0;
  Float_t tbin = 0;
  Float_t adc = 0;

  ntp->SetBranchAddress("event", &event);
  ntp->SetBranchAddress("layer", &layer);
  ntp->SetBranchAddress("phi", &phi);
  ntp->SetBranchAddress("tbin", &tbin);
  ntp->SetBranchAddress("adc", &adc);

  Long64_t nEntries = ntp->GetEntries();
  Long64_t nSelected = 0;
  double maxRadius = 0.0;
  double maxAdc = 0.0;

  for (Long64_t i = 0; i < nEntries; ++i) {
    ntp->GetEntry(i);
    if (static_cast<int>(event) != targetEvent || adc <= 0) {
      continue;
    }
    ++nSelected;
    if (layer > maxRadius) maxRadius = layer;
    if (adc > maxAdc) maxAdc = adc;
  }

  if (nSelected == 0) {
    Error("PlotEvent74Hits3D", "No ntp_hit entries with adc>0 found for event %d", targetEvent);
    input->Close();
    return;
  }

  const Long64_t drawEvery = (nSelected / maxPointsToDraw > 1) ? (nSelected / maxPointsToDraw) : 1;

  std::vector<std::vector<double>> xs(nColorBins), ys(nColorBins), zs(nColorBins);

  Long64_t accepted = 0;
  for (Long64_t i = 0; i < nEntries; ++i) {
    ntp->GetEntry(i);
    if (static_cast<int>(event) != targetEvent || adc <= 0) {
      continue;
    }
    if ((accepted % drawEvery) != 0) {
      ++accepted;
      continue;
    }
    ++accepted;

    const double radius = layer;
    int colorBin = static_cast<int>((nColorBins - 1) * adc / maxAdc);
    if (colorBin < 0) colorBin = 0;
    if (colorBin >= nColorBins) colorBin = nColorBins - 1;

    xs[colorBin].push_back(tbin);
    ys[colorBin].push_back(radius * TMath::Cos(phi));
    zs[colorBin].push_back(radius * TMath::Sin(phi));
  }

  TCanvas *c = new TCanvas("c_event74", "Event 74 3D hits", 1250, 900);
  c->SetTheta(24);
  c->SetPhi(36);

  TH3F *frame = new TH3F(
      "frame",
      ";time bin;x (cm);y (cm)",
      10, tBinMin, tBinMax,
      10, -maxRadius - 2.0, maxRadius + 2.0,
      10, -maxRadius - 2.0, maxRadius + 2.0);
  frame->SetDirectory(nullptr);
  frame->GetXaxis()->CenterTitle();
  frame->GetYaxis()->CenterTitle();
  frame->GetZaxis()->CenterTitle();
  frame->GetXaxis()->SetTitleOffset(1.2);
  frame->GetYaxis()->SetTitleOffset(1.5);
  frame->GetZaxis()->SetTitleOffset(1.5);
  frame->GetXaxis()->SetLabelSize(0.03);
  frame->GetYaxis()->SetLabelSize(0.03);
  frame->GetZaxis()->SetLabelSize(0.03);
  frame->SetLineColorAlpha(kGray + 1, 0.08);
  frame->SetMarkerColorAlpha(kGray + 1, 0.08);
  frame->Draw();

  const int nPalette = gStyle->GetNumberOfColors();
  for (int ibin = 0; ibin < nColorBins; ++ibin) {
    const int nPoints = static_cast<int>(xs[ibin].size());
    if (nPoints == 0) continue;

    TPolyMarker3D *pm = new TPolyMarker3D(nPoints);
    pm->SetMarkerStyle(20);
    pm->SetMarkerSize(0.35);
    const int paletteIndex = static_cast<int>((nPalette - 1.0) * ibin / TMath::Max(1, nColorBins - 1));
    pm->SetMarkerColor(TColor::GetColorPalette(paletteIndex));
    for (int i = 0; i < nPoints; ++i) {
      pm->SetPoint(i, xs[ibin][i], ys[ibin][i], zs[ibin][i]);
    }
    pm->Draw();
  }

  frame->Draw("AXIS SAME");

  // c->SaveAs("event74.pdf");
  input->Close();
}
