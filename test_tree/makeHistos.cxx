void makeHistos(string treeFile="LTCCefficiency_tree.root"){
	TFile *ftree = new TFile(treeFile.c_str());
	TTree *treeHisto = (TTree*)ftree->Get("treeHisto");
	
	// Per Stefano: in una macro interattiva non occorre dichiarare tutti i branch
	// perché l'interprete di ROOT quando apre il file già "sa"
	
	TH1F * htot[4];
	TH1F * hsel[4];
	TH2F * htot2[4];
	TH2F * hsel2[4];
	// P, theta, phi, costheta, x, y
	double inf[4]={0,0,-3.5,0.5};
	double max[4]={10,1,3.5,1};
	double inf2[4][2]={{-0.8,-0.7},{0,-3.5},{0,-3.5},{0,0}};
	double max2[4][2]={{0.5,0.7},{1,3.5},{1,3.5},{10,1}};
//double bins[4]={20,20,20,20};		//customize the binning of the histograms
	string var[6]={"P(GeV/c)","#theta(rad)","#phi(rad)","cos(#theta)(#)"};
	string varsToProject[4] = {"P", "theta", "phi", "costheta"};
	string pair[4][2]={{"y:x","x(#); y(#)"},{"phi:theta","#theta(rad); #phi(rad)"},{"phi:costheta","cos#theta(#); #phi(rad)"},{"costheta:P","P(GeV/c); cos#theta(#)"}};
	for(int j=0; j<4; j++){
		htot[j] = new TH1F(Form("h%d",j+1),Form("Candidates in LTCC [%s]; %s",varsToProject[j].c_str(),var[j].c_str()),20,inf[j],max[j]);
		hsel[j] = new TH1F(Form("h%d",j+5),Form("Candidate hits in LTCC [%s]; %s",varsToProject[j].c_str(),var[j].c_str()),20,inf[j],max[j]);
		htot2[j] = new TH2F(Form("h2%d",j+1),Form("Candidates in LTCC [%s]; %s",pair[j][0].c_str(),pair[j][1].c_str()),20,inf2[j][0],max2[j][0],20,inf2[j][1],max2[j][1]);
		hsel2[j] = new TH2F(Form("h2%d",j+5),Form("Candidate hits in LTCC [%s]; %s",pair[j][0].c_str(),pair[j][1].c_str()),20,inf2[j][0],max2[j][0],20,inf2[j][1],max2[j][1]);
	}

	for(int j=0; j<4; j++) {
		treeHisto->Project(Form("h%d",j+1), varsToProject[j].c_str());
		treeHisto->Project(Form("h%d",j+5), varsToProject[j].c_str(), "nphe>1");
		treeHisto->Project(Form("h2%d",j+1), pair[j][0].c_str());
		treeHisto->Project(Form("h2%d",j+5), pair[j][0].c_str(),"nphe>1");
//		std::cout<<"Hey!2\n";
	}

	TH1F * hrt[4];
	TH2F * hrt2[4];

	for(int j=0; j<4;j++){ 
		hrt[j] = (TH1F*) hsel[j]->Clone(Form("hrt%d",j+1));
		hrt[j]->SetTitle(Form("Ratio of candidates in sector 3-5 [%s]; %s",varsToProject[j].c_str(),var[j].c_str()));
		hrt[j]->SetStats(0);
		hrt[j]->Divide(htot[j]);

		hrt2[j] = (TH2F*) hsel2[j]->Clone(Form("hrt%d",j+5));
		hrt2[j]->SetTitle(Form("Ratio of candidates in sector 3-5 [%s]; %s",pair[j][0].c_str(),pair[j][1].c_str()));
		hrt2[j]->SetStats(0);
		hrt2[j]->Divide(htot2[j]);
//		std::cout<<"Hey!3\n";
	}
	
	TFile* out = new TFile("out.root","RECREATE");

	TCanvas** can = new TCanvas*[8];
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

		can[k+4] = new TCanvas(Form("can2_%d",k),Form("can2_%d",k),1500,600);
		can[k+4]->Divide(3,1);
		can[k+4]->cd(1);
		htot2[k]->Draw("CONT4Z");
		can[k+4]->cd(2);
		hsel2[k]->Draw("CONT4Z");
		can[k+4]->cd(3);
		hrt2[k]->Draw("CONT4Z");

		can[k+4]->Write();

	}

	can[0]->SaveAs("out.pdf("); // <-- first page
	can[1]->SaveAs("out.pdf");
	can[2]->SaveAs("out.pdf");
	can[3]->SaveAs("out.pdf");
	can[4]->SaveAs("out.pdf");
	can[5]->SaveAs("out.pdf");
	can[6]->SaveAs("out.pdf");
	can[7]->SaveAs("out.pdf)"); // <-- last page
	

	out->Close();

}
