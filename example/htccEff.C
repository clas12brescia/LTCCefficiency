const int nHTCCXBins = 100;
const int nHTCCYBins = 100;
const float sizeHTCCX = 250;
const float sizeHTCCY =250;
const double stepXHTCC = 2.5;
const double stepYHTCC = 2.5;
const int isData = 1;
void processHipo(TString inputFile);
Double_t fitf(Double_t *x, Double_t *par);
void analyzeHisto(TString inputFile, int isData);

void htccEff(){
  TString inputFile;
  int isHipo = -1;
  for(Int_t i=1;i<gApplication->Argc();i++){
    TString opt=gApplication->Argv(i);
    if((opt.Contains(".dat") || opt.Contains(".txt"))){
      inputFile=opt(5,opt.Sizeof());
      isHipo = 1;
    }
    else if(opt.Contains(".root")){
      inputFile=opt(5,opt.Sizeof());
      isHipo = 0;
    } 
  }
  if(isHipo < 0)  {
    std::cout << " *** please provide a root or text input file name..." << std::endl;
    exit(0);
  }
  if (isHipo == 1) processHipo(inputFile);
  analyzeHisto(inputFile, isData);  
}

void processHipo(TString inputFile){
  TFile f(Form("%sNPE.root",inputFile.Data()), "recreate");
  TH1F *npe1D[nHTCCXBins][nHTCCYBins];
  for (int xC = 0; xC < nHTCCXBins; xC++){
    for (int yC = 0; yC < nHTCCYBins; yC++){
      npe1D[xC][yC] = new TH1F(Form("npe1DX%dY%d", xC, yC), Form("npe1DX%dY%d", xC, yC), 200, 0, 50);
    }
  }
  HipoChain chain;
  TString nextFile;
  ifstream chainIn(inputFile);
  while (chainIn >> nextFile){
    chain.Add(nextFile);
  }
  
  auto config_c12=chain.GetC12Reader(); //in case you want to configure, use this
  config_c12->addAtLeastPid(11,1);  
  auto& c12=chain.C12ref();
  while(chain.Next()==true){
    auto particles = c12->getDetParticles();
    auto electron=c12->getByID(11)[0];
    if (electron){
      double NPE = electron->che(HTCC)->getNphe();
      double htccX = electron->traj(HTCC,1)->getX();
      double htccY = electron->traj(HTCC,1)->getY();
      int htccXBin = (htccX + sizeHTCCX/2)/stepXHTCC;
      int htccYBin = (htccY + sizeHTCCY/2)/stepYHTCC;
      if (htccXBin > 0 && htccXBin < nHTCCXBins && htccYBin > 0 && htccYBin < nHTCCXBins){
	npe1D[htccXBin][htccYBin]->Fill(NPE);
      }
    }
  }
  f.cd();
  for (int xC = 0; xC < nHTCCXBins; xC++){
    for (int yC = 0; yC < nHTCCYBins; yC++){
      npe1D[xC][yC]->Write();
    }
  }

} 


void analyzeHisto(TString inputFile, int isData){
  TH2F *efficiency2D  = new TH2F("efficiency2D","efficiency2D", nHTCCXBins, -150, 150, nHTCCYBins, -150, 150);
  TH1D *efficiency1D = new TH1D("efficiency1D", "efficiency1D", 200, 0.5, 1.1);

  TFile fIn(Form("%sNPE.root", inputFile.Data()));
  TFile fOut(Form("%sRes.root", inputFile.Data()), "recreate");

  TH1F *npe1D[nHTCCXBins][nHTCCYBins];
  for (int xC = 0; xC < nHTCCXBins; xC++){
    for (int yC = 0; yC < nHTCCYBins; yC++){
      npe1D[xC][yC] = (TH1F*) fIn.Get(Form("npe1DX%dY%d", xC, yC));
    }
  }

  TF1 *gTest = new TF1("gTest", "gaus", 0, 20);
  TLatex capX;
  capX.SetTextSize(0.07);
  capX.SetNDC();
  capX.SetTextFont(12);

  TF1 *funcCurrent[nHTCCXBins][nHTCCYBins];
  for (int xC = 0; xC < nHTCCXBins; xC++){
    for (int yC = 0; yC < nHTCCYBins; yC++){
      funcCurrent[xC][yC] = new TF1(Form("fitfCurrentX%dY%d",xC, yC), fitf, 0, 400, 3);
      funcCurrent[xC][yC]->SetLineColor(2);
      funcCurrent[xC][yC]->SetLineWidth(1);
    }
  }

  TCanvas *bbb = new TCanvas("bbb", "bbb", 10, 10, 7200, 7200);
  bbb->Divide(nHTCCXBins, nHTCCYBins, 0.0000001, 0.0000001);
  for (int xC = 0; xC < nHTCCXBins; xC++){
    for (int yC = 0; yC < nHTCCYBins; yC++){
      bbb->cd(xC  + 1 + nHTCCXBins*(yC + 1));
      if (npe1D[xC][yC]->Integral() > 20){
        npe1D[xC][yC]->Draw();
        if (isData == 1){
          gTest->SetParameter(0, 100);
          gTest->SetParameter(1, 14);
          gTest->SetParameter(0, 4);
          npe1D[xC][yC]->Fit("gTest", "QEM", "", 5, 25);
        }

        if (isData == 0){
          gTest->SetParameter(0, 200);
          gTest->SetParameter(1, 15);
          gTest->SetParameter(0, 8);
          npe1D[xC][yC]->Fit("gTest", "QEM", "", 8, 24);
        }

        if (isData == 1) funcCurrent[xC][yC]->SetParameters(gTest->GetParameter(0), gTest->GetParameter(1), gTest->GetParameter(2));
        if (isData == 0) funcCurrent[xC][yC]->SetParameters(npe1D[xC][yC]->GetMaximum(), npe1D[xC][yC]->GetMaximumBin(), gTest->GetParameter(2));

        if (isData == 1) npe1D[xC][yC]->Fit(Form("fitfCurrentX%dY%d",xC, yC), "QEM", "", 2, 30 );
        if (isData == 0) npe1D[xC][yC]->Fit(Form("fitfCurrentX%dY%d",xC, yC), "QEM", "", 2, 26 );

        npe1D[xC][yC]->Draw();
        capX.DrawLatex(0.55, 0.8, Form("NPE = %4.3f", funcCurrent[xC][yC]->GetParameter(1)));
        capX.DrawLatex(0.6, 0.65, Form("X = %3.2f cm", -125 + 0.5*stepXHTCC + stepXHTCC*xC));
        capX.DrawLatex(0.6, 0.58, Form("Y = %3.2f cm", -125 + 0.5*stepYHTCC + stepYHTCC*yC));
        if (funcCurrent[xC][yC]->Integral(0, 50) > 0 && npe1D[xC][yC]->Integral() > 20){
          efficiency2D->SetBinContent(xC, yC, funcCurrent[xC][yC]->Integral(2, 50)/funcCurrent[xC][yC]->Integral(0, 50));
          efficiency1D->Fill(funcCurrent[xC][yC]->Integral(2, 50)/funcCurrent[xC][yC]->Integral(0, 50));
          capX.DrawLatex(0.55, 0.73, Form("Eff = %4.3f%", funcCurrent[xC][yC]->Integral(2, 50)/funcCurrent[xC][yC]->Integral(0, 50)*100));
        }
      }
    }
  }

  if (isData == 1)   bbb->SaveAs(Form("2DEffData%d%s.pdf", isData, inputFile.Data()));



  TCanvas *workCanvas = new TCanvas("workCanvas", "workCanvas", 10, 10, 1000, 800);
  workCanvas->Divide(1, 1);
  workCanvas->cd(1);

  efficiency2D->Draw("COLZ");
  efficiency2D->SetAxisRange(0.88, 1.02, "z");
  capX.DrawLatex(0.2, 0.86, Form("Mean = %4.3f %", 100*efficiency1D->GetMean()));
  if (isData == 1) workCanvas->SaveAs(Form("efficiency2DData%d%s.pdf", isData, inputFile.Data()));


  for (int xC = 0; xC < nHTCCXBins; xC++){
    for (int yC = 0; yC < nHTCCYBins; yC++){
      npe1D[xC][yC]->Draw();
      funcCurrent[xC][yC]->Draw("same");
      capX.DrawLatex(0.55, 0.8, Form("NPE = %4.3f", funcCurrent[xC][yC]->GetParameter(1)));
      capX.DrawLatex(0.6, 0.65, Form("X = %4.3f cm", -125 + 0.5*stepXHTCC + stepXHTCC*xC));
      capX.DrawLatex(0.6, 0.58, Form("Y = %4.3f cm", -125 + 0.5*stepYHTCC + stepYHTCC*yC));
      capX.DrawLatex(0.55, 0.73, Form("Eff = %4.3f%", funcCurrent[xC][yC]->Integral(2, 50)/funcCurrent[xC][yC]->Integral(0, 50)*100));
      if (xC == 0 && yC == 0) workCanvas->Print(Form("allIndFits%d%s.pdf(", isData, inputFile.Data()), "pdf");
      else if (xC ==99 && yC == 99) workCanvas->Print(Form("allIndFits%d%s.pdf)", isData, inputFile.Data()), "pdf");
      else  workCanvas->Print(Form("allIndFits%d%s.pdf", isData, inputFile.Data()), "pdf");
    }
  }
  fOut.cd();
  efficiency2D->Write();
  efficiency1D->Write();
  fOut.Close();
}


Double_t fitf(Double_t *x, Double_t *par){
  Double_t arg = 0.;
  if (par[2] != 0) arg = x[0]/par[2];
  Double_t arg2 = 0.;
  if (TMath::Gamma(arg+1) != 0) arg2 = TMath::Power(par[1],arg)/TMath::Gamma(arg+1);
  Double_t fitval = par[0]*(arg2)*TMath::Exp(-par[1]);
  return fitval;
}
