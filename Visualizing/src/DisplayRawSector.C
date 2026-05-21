void DisplayRawSector() {
  // Automatically determine project root relative to current directory (assumed to be Visualizing/src)
  TString currentDir = gSystem->pwd();
  TString projectRoot = currentDir + "/../..";
  TString filename = projectRoot + "/clusters_seeds_island_79507-0.root_ntuplizer.root";

  TCanvas *cpl = new TCanvas("cpl","",900,600);
  cpl->SetLogy(0);
  cpl->SetLogx(0);
  cpl->SetLogz(1);
  cpl->Draw();
  cpl->Update();

  TCanvas *clt = new TCanvas("clt","",900,600);
  clt->SetLogy(0);
  clt->SetLogx(0);
  clt->SetLogz(1);
  clt->Draw();
  clt->Update();

  TCanvas *cpt1 = new TCanvas("cpt1","",900,600);
  cpt1->SetLogy(0);
  cpt1->SetLogx(0);
  cpt1->SetLogz(1);
  cpt1->Draw();
  cpt1->Update();

  TCanvas *cpt2 = new TCanvas("cpt2","",900,600);
  cpt2->SetLogy(0);
  cpt2->SetLogx(0);
  cpt2->SetLogz(1);
  cpt2->Draw();
  cpt2->Update();

  TCanvas *cpt3 = new TCanvas("cpt3","",900,600);
  cpt3->SetLogy(0);
  cpt3->SetLogx(0);
  cpt3->SetLogz(1);
  cpt3->Draw();
  cpt3->Update();
  
  Float_t low[12] = {0};
  Float_t hi[12] = {0};
  low[0] = TMath::Pi()/12 * 9;  hi[0] = TMath::Pi()/12 * 11;
  low[1] = TMath::Pi()/12 * 7;  hi[1] = TMath::Pi()/12 * 9;
  low[2] = TMath::Pi()/12 * 5;  hi[2] = TMath::Pi()/12 * 7;
  low[3] = TMath::Pi()/12 * 3;  hi[3] = TMath::Pi()/12 * 5;
  low[4] = TMath::Pi()/12 * 1;  hi[4] = TMath::Pi()/12 * 3;
  low[5] = TMath::Pi()/12 * -1;  hi[5] = TMath::Pi()/12 * 1;
  low[6] = TMath::Pi()/12 * -3;  hi[6] = TMath::Pi()/12 * -1;
  low[7] = TMath::Pi()/12 * -5;  hi[7] = TMath::Pi()/12 * -3;
  low[8] = TMath::Pi()/12 * -7;  hi[8] = TMath::Pi()/12 * -5;
  low[9] = TMath::Pi()/12 * -9;  hi[9] = TMath::Pi()/12 * -7;
  low[10] = TMath::Pi()/12 *-11; hi[10] = TMath::Pi()/12 * -9;

  int sec = 8;
  TFile *_file0 = TFile::Open(filename);
  if (!_file0 || _file0->IsZombie()) {
    Error("DisplayRawSector", "Could not open %s", filename.Data());
    return;
  }
  
  TNtuple *ntp_hit = (TNtuple*)_file0->Get("ntp_hit");
  TNtuple *ntp_cluster = (TNtuple*)_file0->Get("ntp_cluster");
  TNtuple *ntp_clus_trk = (TNtuple*)_file0->Get("ntp_clus_trk");

  TH2F *h1   = new TH2F("h1","h1",92,low[sec],hi[sec],16,6.5,22.5);
  TH2F *h2    = new TH2F("h2","h2",128,low[sec],hi[sec],16,22.5,38.5);
  TH2F *h3 = new TH2F("h3","h3",192,low[sec],hi[sec],16,38.5,54.5);
  TH2F *hall   = new TH2F("hall","",192,low[sec],hi[sec],48,6.5,54.5);
  
  hall->SetMinimum(0);
  hall->SetMaximum(1000);
  hall->GetYaxis()->CenterTitle();
  hall->GetXaxis()->CenterTitle();
  hall->SetXTitle("phi");
  hall->SetYTitle("Layer");

  TH2F *hpz1   = new TH2F("hpz1","hpz1",1001,-0.5,1000.5,92,low[sec],hi[sec]);
  hpz1->GetYaxis()->CenterTitle();
  hpz1->GetXaxis()->CenterTitle();
  hpz1->SetYTitle("phi R1");
  hpz1->SetXTitle("Time Bin");
  hpz1->SetMinimum(0);
  hpz1->SetMaximum(1000);

  TH2F *hpz2    = new TH2F("hpz2","hpz2",1001,-0.5,1000.5,128,low[sec],hi[sec]);
  hpz2->GetYaxis()->CenterTitle();
  hpz2->GetXaxis()->CenterTitle();
  hpz2->SetYTitle("phi R2");
  hpz2->SetXTitle("Time Bin");
  hpz2->SetMinimum(0);
  hpz2->SetMaximum(1000);
  
  TH2F *hpz3 = new TH2F("hpz3","hpz3",1001,-0.5,1000.5,192,low[sec],hi[sec]);
  hpz3->GetYaxis()->CenterTitle();
  hpz3->GetXaxis()->CenterTitle();
  hpz3->SetYTitle("phi R3");
  hpz3->SetXTitle("Time Bin");
  hpz3->SetMinimum(0);
  hpz3->SetMaximum(1000);
  
  TH2F *hzall   = new TH2F("hzall","",1001,-0.5,1000.5,48,6.5,54.5);
  
  hzall->SetMinimum(0);
  hzall->SetMaximum(1000);
  hzall->GetYaxis()->CenterTitle();
  hzall->GetXaxis()->CenterTitle();
  hzall->SetXTitle("Time Bin");
  hzall->SetYTitle("Layer");
  
  char cut[256];
  sprintf(cut,"adc*(event==74&&zelem==0&&layer<55&&tbin<1400&&layer>=7&&layer<7+16&&phielem==%d)",sec);
  ntp_hit->Draw("layer:phi>>h1",cut);
  sprintf(cut,"adc*(event==74&&zelem==0&&layer<55&&tbin<1400&&layer>=7+16&&layer<7+32&&phielem==%d)",sec);
  ntp_hit->Draw("layer:phi>>h2",cut);
  sprintf(cut,"adc*(event==74&&zelem==0&&layer<55&&tbin<1400&&layer>=7+32&&layer<7+48&&phielem==%d)",sec);
  ntp_hit->Draw("layer:phi>>h3",cut);

  cpl->cd();
  hall->Draw();
  h1->Draw("samecolz");
  h2->Draw("samecolz");
  h3->Draw("samecolz");
  ntp_cluster->SetMarkerSize(0.5);
  ntp_clus_trk->SetMarkerStyle(24);
  sprintf(cut,"adc*(event==74&&zelem==0&&layer<55&&tbin<1400&&layer>=7&&layer<7+48&&phielem==%d)",sec);
  ntp_cluster->Draw("layer:phi",cut,"same");
  ntp_clus_trk->Draw("layer:phi",cut,"same");
  cpl->Update();

  clt->cd();
  sprintf(cut,"adc*(event==74&&zelem==0&&layer<55&&layer>=7&&layer<7+48&&phielem==%d)",sec);
  ntp_hit->Draw("layer:tbin>>hzall",cut,"colz");
  sprintf(cut,"adc*(event==74&&zelem==0&&layer<55&&tbin<1400&&layer>=7&&layer<7+48&&phielem==%d)",sec);
  ntp_cluster->Draw("layer:tbin",cut,"same");
  ntp_clus_trk->Draw("layer:tbin",cut,"same");
  clt->Update();
  
  cpt1->cd();
  sprintf(cut,"adc*(event==74&&zelem==0&&layer<55&&layer>=7&&layer<7+16&&phielem==%d)",sec);
  ntp_hit->Draw("phi:tbin>>hpz1",cut);
  hpz1->Draw("colz");
  sprintf(cut,"adc*(event==74&&zelem==0&&layer<55&&tbin<1400&&layer>=7&&layer<7+16&&phielem==%d)",sec);
  ntp_cluster->Draw("phi:tbin",cut,"same");
  ntp_clus_trk->Draw("phi:tbin",cut,"same");

  cpt2->cd();
  sprintf(cut,"adc*(event==74&&zelem==0&&layer<55&&layer>=7+16&&layer<7+32&&phielem==%d)",sec);
  ntp_hit->Draw("phi:tbin>>hpz2",cut);
  hpz2->Draw("colz");
  sprintf(cut,"adc*(event==74&&zelem==0&&layer<55&&tbin<1400&&layer>=7+16&&layer<7+32&&phielem==%d)",sec);
  ntp_cluster->Draw("phi:tbin",cut,"same");
  ntp_clus_trk->Draw("phi:tbin",cut,"same");
  cpt2->Update();

  cpt3->cd();
  sprintf(cut,"adc*(event==74&&zelem==0&&layer<55&&layer>=7+32&&layer<7+48&&phielem==%d)",sec);
  ntp_hit->Draw("phi:tbin>>hpz3",cut);
  hpz3->Draw("colz");
  sprintf(cut,"adc*(event==74&&zelem==0&&layer<55&&tbin<1400&&layer>=7+32&&layer<7+48&&phielem==%d)",sec);
  ntp_cluster->Draw("phi:tbin",cut,"same");
  ntp_clus_trk->Draw("phi:tbin",cut,"same");
  cpt3->Update();

  cpl->SaveAs(currentDir + "/../images/DisplayRawSector_cpl.png");
  clt->SaveAs(currentDir + "/../images/DisplayRawSector_clt.png");
  cpt1->SaveAs(currentDir + "/../images/DisplayRawSector_cpt1.png");
  cpt2->SaveAs(currentDir + "/../images/DisplayRawSector_cpt2.png");
  cpt3->SaveAs(currentDir + "/../images/DisplayRawSector_cpt3.png");

  _file0->Close();
}
