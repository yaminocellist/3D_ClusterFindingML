void DisplayRawLayer() {
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

  TCanvas *cpt = new TCanvas("cpt","",900,600);
  cpt->SetLogy(0);
  cpt->SetLogx(0);
  cpt->SetLogz(1);
  cpt->Draw();
  cpt->Update();

  TCanvas *clay = new TCanvas("clay","",900,600);
  clay->SetLogy(0);
  clay->SetLogx(0);
  clay->SetLogz(1);
  clay->Draw();
  clay->Update();
  
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
    Error("DisplayRawLayer", "Could not open %s", filename.Data());
    return;
  }

  TNtuple *ntp_hit = (TNtuple*)_file0->Get("ntp_hit");
  TNtuple *ntp_cluster = (TNtuple*)_file0->Get("ntp_cluster");
  TNtuple *ntp_clus_trk = (TNtuple*)_file0->Get("ntp_clus_trk");

  TH2F *h1 = new TH2F("h1","h1",1001,-0.5,1000.5,92,low[sec],hi[sec]);
  TH2F *hall1 = new TH2F("hall1","hall1",1001,-0.5,1000.5,92,low[sec],hi[sec]);
  TH2F *hall1t = new TH2F("hall1t","hall1t",1001,-0.5,1000.5,16,6.5,22.5);

  TH2F *hall1pl   = new TH2F("hall1pl","",92,low[sec],hi[sec],16,6.5,22.5);
  hall1pl->SetMinimum(0);
  hall1pl->SetMaximum(1000);
  hall1pl->GetYaxis()->CenterTitle();
  hall1pl->GetXaxis()->CenterTitle();
  hall1pl->SetXTitle("Phi R1");
  hall1pl->SetYTitle("Layer");

  TH2F *hall2pl   = new TH2F("hall2pl","",128,low[sec],hi[sec],16,22.5,38.5);
  hall2pl->SetMinimum(0);
  hall2pl->SetMaximum(1000);
  hall2pl->GetYaxis()->CenterTitle();
  hall2pl->GetXaxis()->CenterTitle();
  hall2pl->SetXTitle("Phi R2");
  hall2pl->SetYTitle("Layer");
  TH2F *hall3pl   = new TH2F("hall3pl","",192,low[sec],hi[sec],16,38.5,54.5);
  hall3pl->SetMinimum(0);
  hall3pl->SetMaximum(1000);
  hall3pl->GetYaxis()->CenterTitle();
  hall3pl->GetXaxis()->CenterTitle();
  hall3pl->SetXTitle("Phi R3");
  hall3pl->SetYTitle("Layer");
  
  TH2F *h2 = new TH2F("h2","h2",1001,-0.5,1000.5,128,low[sec],hi[sec]);
  TH2F *hall2 = new TH2F("hall2","hall2",1001,-0.5,1000.5,128,low[sec],hi[sec]);
  TH2F *hall2t = new TH2F("hall2t","hall2t",1001,-0.5,1000.5,16,22.5,38.5);
  
  TH2F *h3 = new TH2F("h3","h3",1001,-0.5,1000.5,192,low[sec],hi[sec]);
  TH2F *hall3 = new TH2F("hall3","hall3",1001,-0.5,1000.5,192,low[sec],hi[sec]);
  TH2F *hall3t = new TH2F("hall3t","hall3t",1001,-0.5,1000.5,16,38.5,54.5);
  
  h1->SetMinimum(0);
  h1->SetMaximum(1000);
  h1->GetYaxis()->CenterTitle();
  h1->GetXaxis()->CenterTitle();
  h1->SetXTitle("Time Bin R1");
  h1->SetYTitle("Phi Bin");

  hall1->SetMinimum(0);
  hall1->SetMaximum(1000);
  hall1->GetYaxis()->CenterTitle();
  hall1->GetXaxis()->CenterTitle();
  hall1->SetXTitle("Time Bin R1");
  hall1->SetYTitle("Phi Bin");

  hall1t->SetMinimum(0);
  hall1t->SetMaximum(1000);
  hall1t->GetYaxis()->CenterTitle();
  hall1t->GetXaxis()->CenterTitle();
  hall1t->SetXTitle("Time Bin R1");
  hall1t->SetYTitle("Layer");

  h2->SetMinimum(0);
  h2->SetMaximum(1000);
  h2->GetYaxis()->CenterTitle();
  h2->GetXaxis()->CenterTitle();
  h2->SetXTitle("Time Bin R2");
  h2->SetYTitle("Phi Bin");
  hall2->SetMinimum(0);
  hall2->SetMaximum(1000);
  hall2->GetYaxis()->CenterTitle();
  hall2->GetXaxis()->CenterTitle();
  hall2->SetXTitle("Time Bin R1");
  hall2->SetYTitle("Phi Bin");

  hall2t->SetMinimum(0);
  hall2t->SetMaximum(1000);
  hall2t->GetYaxis()->CenterTitle();
  hall2t->GetXaxis()->CenterTitle();
  hall2t->SetXTitle("Time Bin R2");
  hall2t->SetYTitle("Layer");
  
  h3->SetMinimum(0);
  h3->SetMaximum(1000);
  h3->GetYaxis()->CenterTitle();
  h3->GetXaxis()->CenterTitle();
  h3->SetXTitle("Time Bin R3");
  h3->SetYTitle("Phi Bin");
  hall3->SetMinimum(0);
  hall3->SetMaximum(1000);
  hall3->GetYaxis()->CenterTitle();
  hall3->GetXaxis()->CenterTitle();
  hall3->SetXTitle("Time Bin R3");
  hall3->SetYTitle("Phi Bin");

  hall3t->SetMinimum(0);
  hall3t->SetMaximum(1000);
  hall3t->GetYaxis()->CenterTitle();
  hall3t->GetXaxis()->CenterTitle();
  hall3t->SetXTitle("Time Bin R3");
  hall3t->SetYTitle("Layer");
  
  char cut[256];
  int R = 3;
  if(R==1){
    cpt->cd();
    sprintf(cut,"adc*(event==74&&zelem==0&&tbin<1400&&phielem==%d&&layer>=7&&layer<7+16)",sec);
    ntp_hit->Draw("phi:tbin>>hall1",cut);
    ntp_cluster->SetMarkerSize(0.5);
    ntp_clus_trk->SetMarkerStyle(24);
    
    ntp_cluster->Draw("phi:tbin",cut,"same");
    ntp_clus_trk->Draw("phi:tbin",cut,"same");
    cpt->Update();
    
    clt->cd();
    sprintf(cut,"adc*(event==74&&zelem==0&&layer<55&&layer>=7&&layer<7+16&&phielem==%d)",sec);
    ntp_hit->Draw("layer:tbin>>hall1t",cut,"colz");
    sprintf(cut,"adc*(event==74&&zelem==0&&layer<55&&tbin<1400&&layer>=7&&layer<7+16&&phielem==%d)",sec);
    ntp_cluster->Draw("layer:tbin",cut,"same");
    ntp_clus_trk->Draw("layer:tbin",cut,"same");
    clt->Update();
    
    cpl->cd();
    sprintf(cut,"adc*(event==74&&zelem==0&&layer<55&&layer>=7&&layer<7+16&&phielem==%d)",sec);
    ntp_hit->Draw("layer:phi>>hall1pl",cut,"colz");
    sprintf(cut,"adc*(event==74&&zelem==0&&layer<55&&tbin<1400&&layer>=7&&layer<7+16&&phielem==%d)",sec);
    ntp_cluster->Draw("layer:tbin",cut,"same");
    ntp_clus_trk->Draw("layer:tbin",cut,"same");  
    cpl->Update();

    clay->cd();
    
    for(int layer = 7; layer < 7+16;layer++){
      sprintf(cut,"adc*(event==74&&zelem==0&&tbin<1400&&phielem==%d&&layer==%d)",sec,layer);
      ntp_hit->Draw("phi:tbin>>h1",cut);
      ntp_cluster->SetMarkerSize(0.5);
      ntp_clus_trk->SetMarkerStyle(24);
      
      ntp_cluster->Draw("phi:tbin",cut,"same");
      ntp_clus_trk->Draw("phi:tbin",cut,"same");
      clay->Update();
    }
  }
  if(R==2){
    cpt->cd();
    sprintf(cut,"adc*(event==74&&zelem==0&&tbin<1400&&phielem==%d&&layer>=7+16&&layer<7+32)",sec);
    ntp_hit->Draw("phi:tbin>>hall2",cut);
    ntp_cluster->SetMarkerSize(0.5);
    ntp_clus_trk->SetMarkerStyle(24);
    
    ntp_cluster->Draw("phi:tbin",cut,"same");
    ntp_clus_trk->Draw("phi:tbin",cut,"same");
    cpt->Update();
    
    clt->cd();
    sprintf(cut,"adc*(event==74&&zelem==0&&layer<55&&layer>=7+16&&layer<7+32&&phielem==%d)",sec);
    ntp_hit->Draw("layer:tbin>>hall2t",cut,"colz");
    sprintf(cut,"adc*(event==74&&zelem==0&&layer<55&&tbin<1400&&layer>=7+16&&layer<7+32&&phielem==%d)",sec);
    ntp_cluster->Draw("layer:tbin",cut,"same");
    ntp_clus_trk->Draw("layer:tbin",cut,"same");
    clt->Update();
    
    cpl->cd();
    sprintf(cut,"adc*(event==74&&zelem==0&&layer<55&&layer>=7+16&&layer<7+32&&phielem==%d)",sec);
    ntp_hit->Draw("layer:phi>>hall2pl",cut,"colz");
    sprintf(cut,"adc*(event==74&&zelem==0&&layer<55&&tbin<1400&&layer>=7+16&&layer<7+32&&phielem==%d)",sec);
    ntp_cluster->Draw("layer:tbin",cut,"same");
    ntp_clus_trk->Draw("layer:tbin",cut,"same");  
    cpl->Update();
    
    clay->cd();
    for(int layer = 7+16; layer < 7+32;layer++){
      sprintf(cut,"adc*(event==74&&zelem==0&&tbin<1400&&phielem==%d&&layer==%d)",sec,layer);
      ntp_hit->Draw("phi:tbin>>h2",cut);
      ntp_cluster->SetMarkerSize(0.5);
      ntp_clus_trk->SetMarkerStyle(24);
      
      ntp_cluster->Draw("phi:tbin",cut,"same");
      ntp_clus_trk->Draw("phi:tbin",cut,"same");
      clay->Update();
    }
  }
  if(R==3){
    cpt->cd();
    sprintf(cut,"adc*(event==74&&zelem==0&&tbin<1400&&phielem==%d&&layer>=7+32&&layer<7+48)",sec);
    ntp_hit->Draw("phi:tbin>>hall3",cut);
    ntp_cluster->SetMarkerSize(0.5);
    ntp_clus_trk->SetMarkerStyle(24);
    sprintf(cut,"adc*(event==74&&zelem==0&&tbin<1400&&phielem==%d&&layer>=7+32&&layer<7+48&&phisize>0&&phisize<40)",sec);
    ntp_cluster->Draw("phi:tbin",cut,"same");
    ntp_clus_trk->Draw("phi:tbin",cut,"same");
    cpt->Update();
    
    clt->cd();
    sprintf(cut,"adc*(event==74&&zelem==0&&layer<55&&layer>=7+32&&layer<7+48&&phielem==%d)",sec);
    ntp_hit->Draw("layer:tbin>>hall3t",cut,"colz");
    sprintf(cut,"adc*(event==74&&zelem==0&&layer<55&&tbin<1400&&layer>=7+32&&layer<7+48&&phielem==%d&&phisize>0&&phisize<40)",sec);
    ntp_cluster->Draw("layer:tbin",cut,"same");
    ntp_clus_trk->Draw("layer:tbin",cut,"same");
    clt->Update();

    cpl->cd();
    sprintf(cut,"adc*(event==74&&zelem==0&&layer<55&&layer>=7+32&&layer<7+48&&phielem==%d)",sec);
    ntp_hit->Draw("layer:phi>>hall3pl",cut,"colz");
    sprintf(cut,"adc*(event==74&&zelem==0&&layer<55&&tbin<1400&&layer>=7+32&&layer<7+48&&phielem==%d&&phisize>0&&phisize<40)",sec);
    ntp_cluster->Draw("layer:phi",cut,"same");
    ntp_clus_trk->Draw("layer:phi",cut,"same");  
    cpl->Update();
    
    clay->cd();
    
    for(int layer = 7+32; layer < 7+48;layer+=5){
      sprintf(cut,"adc*(event==74&&zelem==0&&tbin<1400&&phielem==%d&&layer>=%d&&layer<=%d)",sec,layer,layer+4);
      ntp_hit->Draw("phi:tbin>>h3",cut);
      ntp_cluster->SetMarkerSize(0.5);
      ntp_clus_trk->SetMarkerStyle(24);

      sprintf(cut,"adc*(event==74&&zelem==0&&tbin<1400&&phielem==%d&&layer>=%d&&layer<=%d)",sec,layer,layer+4);
      ntp_cluster->Draw("phi:tbin",cut,"same");
      ntp_clus_trk->Draw("phi:tbin",cut,"same");
      clay->Update();
    }
  }  

  cpl->SaveAs(currentDir + "/../images/DisplayRawLayer_cpl.png");
  clt->SaveAs(currentDir + "/../images/DisplayRawLayer_clt.png");
  cpt->SaveAs(currentDir + "/../images/DisplayRawLayer_cpt.png");
  clay->SaveAs(currentDir + "/../images/DisplayRawLayer_clay.png");

  _file0->Close();
}
