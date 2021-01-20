void makeplot(string treeFile="LTCCefficiency_tree.root"){
	TFile *ftree = new TFile(treeFile.c_str());
	TTree *treeHisto = (TTree*)ftree->Get("treeHisto");
	double P, theta, phi;
	double x,y,costheta;
	double nphe;
	treeHisto->SetBranchAddress("P",&P);
	treeHisto->SetBranchAddress("theta",&theta);
	treeHisto->SetBranchAddress("phi",&phi);
	treeHisto->SetBranchAddress("x",&x);
	treeHisto->SetBranchAddress("y",&y);
	treeHisto->SetBranchAddress("costheta",&costheta);
	treeHisto->SetBranchAddress("nphe",&nphe);

	TH1F** htot = new TH1F*[4];
	TH1F** hsel = new TH1F*[4];
	double inf[4]={0,0,-3.5,0.5};
	double max[4]={10,1,3.5,1};
	char var[4][15]={"P(GeV/c)","#theta(rad)","#phi(rad)","cos(#theta)(#)"};
	for(int j=0;j<4;j++){
	htot[j] = new TH1F(Form("h%d",j+1),Form("Candidates in LTCC [%s]; %s",var[j],var[j]),20,inf[j],max[j]);
	hsel[j] = new TH1F(Form("h%d",j+5),Form("Candidate hits in LTCC [%s]; %s",var[j],var[j]),20,inf[j],max[j]);
	}

	for(int i=0;i<treeHisto->GetEntries();i++){
		treeHisto->GetEntry(i);
		htot[0]->Fill(P);
		htot[1]->Fill(theta);
		htot[2]->Fill(phi);
		htot[3]->Fill(costheta);
		if(nphe>1){
		hsel[0]->Fill(P);
		hsel[1]->Fill(theta);
		hsel[2]->Fill(phi);
		hsel[3]->Fill(costheta);
		}
	}
	
	TH1F** hrt = new TH1F*[4];
	for(int j=0; j<4;j++){ 
		hrt[j] = (TH1F*) hsel[j]->Clone(Form("hrt%d",j+1));
		hrt[j]->SetTitle(Form("Ratio of candidates in sector 3-5 [%s]; %s",var[j],var[j]));
		hrt[j]->SetStats(0);
		hrt[j]->Divide(htot[j]);
	}
	
	TFile* out = new TFile("out.root","RECREATE");

	TCanvas** can = new TCanvas*[4];
	for(int k=0; k<4; k++){
		can[k] = new TCanvas(Form("can%d",k),Form("can%d",k),1500,600);
		can[k]->Divide(3,1);
		can[k]->cd(1);
		htot[k]->Draw();
		can[k]->cd(2);
		hsel[k]->Draw();
		can[k]->cd(3);
		hrt[k]->Draw();

		can[k]->Write();
	}

	out->Close();
}
