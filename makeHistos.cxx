//makeHistos.cxx

/*
This macro is made to draw and save histograms from a TTree.
As default, the input will be the file named `LTCCefficiency_tree.root`.
Use:
root -l 'makeHistos.cxx("tree_filename.root")'
if you named this file differently from the default value.
*/

// Convert a TH2F in a polar histogram with the correct axes system
//void SetToPolar(TH2F* h, TCanvas* c);

void makeHistos(string treeFile="LTCCefficiency_tree.root"){
	//read the TTree from root file created by LTCCefficiency.cxx
	TFile *ftree = new TFile(treeFile.c_str());
	TTree *treeHisto = (TTree*)ftree->Get("treeHisto");
	
	//define histograms arrays to plot total and selected events
	//total = only kinetic cuts; selected = nphe>1 (+ nphe>2 for 1D)
	TH1F * htot[12];
	TH1F * hsel_1[12];
	TH1F * hsel_2[12];
	TH2F * htot2[10];
	TH2F * hsel2[10];
	TH3F * htot3D[2];
	TH3F * hsel3D[2];
	//histograms to test conditions
	TH1F * htest_status[12];
	TH1F * htest_chi2pid[18];
	//min and max of ranges for variables:
	// P, theta, phi, Theta, Phi, costheta
	double infS3[6]={2.5,5,-90,3,80,0.8};
	double maxS3[6]={9.5,35,-30,33,160,1};
	double infS5[6]={2.5,5,30,3,-160,0.8};
	double maxS5[6]={9.5,35,90,33,-80,1};
	double inf[6]={2.5,5,-180,3,-200,0.8};
	double max[6]={9.5,35,180,33,200,1};
	//ranges for 2D-histograms variables pairs:
	// (x,y), (theta, phi), (Theta,Phi), (costheta, phi), (P, theta)
	double inf2S3[5][2]={{-350,0},{5,-90},{3,80},{0.8,-90},{2.5,3}};
	double max2S3[5][2]={{0,350},{35,-30},{33,160},{1,-30},{9.5,33}};
	double inf2S5[5][2]={{-350,-350},{5,30},{3,-160},{0.8,30},{2.5,3}};
	double max2S5[5][2]={{0,0},{35,90},{33,-80},{1,90},{9.5,33}};
	double inf2[5][2]={{-350,-350},{5,-180},{3,-200},{0.8,-180},{2.5,3}};
	double max2[5][2]={{350,350},{35,180},{33,200},{1,180},{9.5,33}};
	//customize the binning of the histograms
	// P, theta, phi, Theta, Phi, costheta
	int bins[6]={56,90,180,90,180,80};
	// (x,y), (theta, phi), (Theta, Phi), (costheta, phi), (P, theta)
	int bins2[5][2]={{150,150},{120,120},{120,120},{120,180},{56,90}};
	
	// set useful aliases for histograms
	treeHisto->SetAlias("R","sqrt(X*X+Y*Y+Z*Z)");
	treeHisto->SetAlias("theta","acos(Z/R)*180/TMath::Pi()");
	treeHisto->SetAlias("phi","atan(Y/X)*180/TMath::Pi()");
	treeHisto->SetAlias("costheta","cos(theta*TMath::Pi()/180)");
	treeHisto->SetAlias("ThetaV","Theta*180/TMath::Pi()");
	treeHisto->SetAlias("PhiV","Phi*180/TMath::Pi()");
	//title of axes, variables and variables pairs
	string var[6]={"P(GeV/c)","#theta(deg)","#phi(deg)","#theta(deg)","#phi(deg)","cos(#theta)(#)"};
	string varsToProject[6] = {"P", "theta", "phi", "ThetaV", "PhiV", "costheta"};
	string pair[5][2]={{"Y:X","x(cm); y(cm)"},{"phi:theta","#theta(deg); #phi(deg)"},{"PhiV:ThetaV","#theta(deg); #phi(deg)"},{"phi:costheta","cos#theta; #phi(deg)"},{"ThetaV:P","P(GeV/c); #theta(deg)"}};
	
	//create histograms arrays with ranges and title defined before
	for(int j=0; j<6; j++){
		htot[j] = new TH1F(Form("ht%d",j+1),Form("Candidates in LTCC in sector 3 [%s]; %s",varsToProject[j].c_str(),var[j].c_str()),bins[j],infS3[j],maxS3[j]);
		hsel_1[j] = new TH1F(Form("hs1%d",j+1),Form("Candidate hits in LTCC in sector 3 [%s]; %s",varsToProject[j].c_str(),var[j].c_str()),bins[j],infS3[j],maxS3[j]);
		htot[j+6] = new TH1F(Form("ht%d",j+7),Form("Candidates in LTCC in sector 5 [%s]; %s",varsToProject[j].c_str(),var[j].c_str()),bins[j],infS5[j],maxS5[j]);
		hsel_1[j+6] = new TH1F(Form("hs1%d",j+7),Form("Candidate hits in LTCC in sector 5 [%s]; %s",varsToProject[j].c_str(),var[j].c_str()),bins[j],infS5[j],maxS5[j]);
		hsel_2[j] = new TH1F(Form("hs2%d",j+1),Form("Candidate hits in LTCC in sector 3 [%s]; %s",varsToProject[j].c_str(),var[j].c_str()),bins[j],infS3[j],maxS3[j]);
		hsel_2[j+6] = new TH1F(Form("hs2%d",j+7),Form("Candidate hits in LTCC in sector 5 [%s]; %s",varsToProject[j].c_str(),var[j].c_str()),bins[j],infS5[j],maxS5[j]);
	}
	for(int j=0; j<5; j++){
		htot2[j] = new TH2F(Form("ht2%d",j+1),Form("Candidates in LTCC in sector 3 [%s]; %s",pair[j][0].c_str(),pair[j][1].c_str()),bins2[j][0],inf2S3[j][0],max2S3[j][0],bins2[j][1],inf2S3[j][1],max2S3[j][1]);
		hsel2[j] = new TH2F(Form("hsel2%d",j+1),Form("Candidate hits in LTCC in sector 3 [%s]; %s",pair[j][0].c_str(),pair[j][1].c_str()),bins2[j][0],inf2S3[j][0],max2S3[j][0],bins2[j][1],inf2S3[j][1],max2S3[j][1]);
		htot2[j+5] = new TH2F(Form("ht2%d",j+6),Form("Candidates in LTCC in sector 5 [%s]; %s",pair[j][0].c_str(),pair[j][1].c_str()),bins2[j][0],inf2S5[j][0],max2S5[j][0],bins2[j][1],inf2S5[j][1],max2S5[j][1]);
		hsel2[j+5] = new TH2F(Form("hsel2%d",j+6),Form("Candidate hits in LTCC in sector 5 [%s]; %s",pair[j][0].c_str(),pair[j][1].c_str()),bins2[j][0],inf2S5[j][0],max2S5[j][0],bins2[j][1],inf2S5[j][1],max2S5[j][1]);
	}
	
	string varsToProjectTest[3]={"P","ThetaV","PhiV"};
	for(int j=0; j<3; j++){
			htest_status[j] = new TH1F(Form("htests%d",j+1),Form("Test of status; %s",varsToProjectTest[j].c_str()),bins[j],inf[j],max[j]);
			htest_status[j+3] = new TH1F(Form("htests%d",j+4),Form("Test of status (sel); %s",varsToProjectTest[j].c_str()),bins[j],inf[j],max[j]);
			htest_status[j+6] = new TH1F(Form("htests%d",j+7),Form("Test of status; %s",varsToProjectTest[j].c_str()),bins[j],inf[j],max[j]);
			htest_status[j+9] = new TH1F(Form("htests%d",j+10),Form("Test of status (sel); %s",varsToProjectTest[j].c_str()),bins[j],inf[j],max[j]);
			
			htest_chi2pid[j] = new TH1F(Form("htestc%d",j+1),Form("Test of status; %s",varsToProjectTest[j].c_str()),bins[j],inf[j],max[j]);
			htest_chi2pid[j+3] = new TH1F(Form("htestc%d",j+4),Form("Test of status (sel); %s",varsToProjectTest[j].c_str()),bins[j],inf[j],max[j]);
			htest_chi2pid[j+6] = new TH1F(Form("htestc%d",j+7),Form("Test of status; %s",varsToProjectTest[j].c_str()),bins[j],inf[j],max[j]);
			htest_chi2pid[j+9] = new TH1F(Form("htestc%d",j+10),Form("Test of status (sel); %s",varsToProjectTest[j].c_str()),bins[j],inf[j],max[j]);
			htest_chi2pid[j+12] = new TH1F(Form("htestc%d",j+13),Form("Test of status; %s",varsToProjectTest[j].c_str()),bins[j],inf[j],max[j]);
			htest_chi2pid[j+15] = new TH1F(Form("htestc%d",j+16),Form("Test of status (sel); %s",varsToProjectTest[j].c_str()),bins[j],inf[j],max[j]);
	}

	htot3D[0] = new TH3F("ht3D1","Candidates in LTCC [P:theta:phi]; P(GeV/c); #theta(deg); #phi(deg)",56,2.5,9.5,15,3,33,40,80,160);
	hsel3D[0] = new TH3F("hs3D1","Candidate hits in LTCC [P:theta:phi]; P(GeV/c); #theta(deg); #phi(deg)",56,2.5,9.5,15,3,33,40,80,160);
	htot3D[1] = new TH3F("ht3D2","Candidates in LTCC [P:theta:phi]; P(GeV/c); #theta(deg); #phi(deg)",56,2.5,9.5,15,3,33,40,-160,-80);
	hsel3D[1] = new TH3F("hs3D2","Candidate hits in LTCC [P:theta:phi]; P(GeV/c); #theta(deg); #phi(deg)",56,2.5,9.5,15,3,33,40,-160,-80);
	
	// set aliases for histograms' conditions
	// sector 3
	treeHisto->SetAlias("S3","mm>0.88 && mm<1 && abs(chi2pid)<3 && status>2109 && Y>0 && charge==1");							//sector 3
	treeHisto->SetAlias("S3N1","mm>0.88 && mm<1 && abs(chi2pid)<3 && status>2109 && Y>0 && nphe>1 && charge==1");	//sector 3, 1 nphe
	treeHisto->SetAlias("S3N2","mm>0.88 && mm<1 && abs(chi2pid)<3 && status>2109 && Y>0 && nphe>2 && charge==1");	//sector 3, 2 nphe
	// sector 5
	treeHisto->SetAlias("S5","mm>0.88 && mm<1 && abs(chi2pid)<3 && status>2109 && Y<0 && charge==1");							//sector 5
	treeHisto->SetAlias("S5N1","mm>0.88 && mm<1 && abs(chi2pid)<3 && status>2109 && Y<0 && nphe>1 && charge==1");	//sector 5, 1 nphe
	treeHisto->SetAlias("S5N2","mm>0.88 && mm<1 && abs(chi2pid)<3 && status>2109 && Y<0 && nphe>2 && charge==1");	//sector 5, 2 nphe
	// status and chi2pid tests
	treeHisto->SetAlias("status1","mm>0.88 && mm<1 && abs(chi2pid)<3 && status>2109 && charge==1");
	treeHisto->SetAlias("status2","mm>0.88 && mm<1 && abs(chi2pid)<3 && status>2229 && charge==1");
	treeHisto->SetAlias("N2_status1","mm>0.88 && mm<1 && abs(chi2pid)<3 && status>2109 && charge==1 && nphe>2");
	treeHisto->SetAlias("N2_status2","mm>0.88 && mm<1 && abs(chi2pid)<3 && status>2229 && charge==1 && nphe>2");
	
	treeHisto->SetAlias("chi2pid1","mm>0.88 && mm<1 && abs(chi2pid)<3 && status>2109 && charge==1");
	treeHisto->SetAlias("chi2pid2","mm>0.88 && mm<1 && abs(chi2pid)<2 && status>2109 && charge==1");
	treeHisto->SetAlias("chi2pid3","mm>0.88 && mm<1 && abs(chi2pid)<1 && status>2109 && charge==1");
	treeHisto->SetAlias("N2_chi2pid1","mm>0.88 && mm<1 && abs(chi2pid)<3 && status>2109 && charge==1 && nphe>2");
	treeHisto->SetAlias("N2_chi2pid2","mm>0.88 && mm<1 && abs(chi2pid)<2 && status>2109 && charge==1 && nphe>2");
	treeHisto->SetAlias("N2_chi2pid3","mm>0.88 && mm<1 && abs(chi2pid)<1 && status>2109 && charge==1 && nphe>2");

	//fill the histograms arrays with the tree branches
	for(int j=0; j<6; j++){
		//1D
		// sector 3
		treeHisto->Project(Form("ht%d",j+1), varsToProject[j].c_str(), "S3");
		treeHisto->Project(Form("hs1%d",j+1), varsToProject[j].c_str(), "S3N1");	//condition on photoelectrons
		treeHisto->Project(Form("hs2%d",j+1), varsToProject[j].c_str(), "S3N2");	//condition on photoelectrons
		// sector 5
		treeHisto->Project(Form("ht%d",j+7), varsToProject[j].c_str(), "S5");
		treeHisto->Project(Form("hs1%d",j+7), varsToProject[j].c_str(), "S5N1");	//condition on photoelectrons
		treeHisto->Project(Form("hs2%d",j+7), varsToProject[j].c_str(), "S5N2");	//condition on photoelectrons
	}
	for(int j=0; j<5; j++){
		//2D
		// sector 3
		treeHisto->Project(Form("ht2%d",j+1), pair[j][0].c_str(),"S3");
		treeHisto->Project(Form("hsel2%d",j+1), pair[j][0].c_str(),"S3N2");				//condition on photoelectrons
		// sector 5
		treeHisto->Project(Form("ht2%d",j+6), pair[j][0].c_str(),"S5");
		treeHisto->Project(Form("hsel2%d",j+6), pair[j][0].c_str(),"S5N2");
	}
	for(int j=0; j<3; j++){//histograms to test the conditions on status and chi2pid
		//status >= (2110,2230) (abs(chi2pid)<3)
		treeHisto->Project(Form("htests%d",j+1),varsToProjectTest[j].c_str(),"status1");
		treeHisto->Project(Form("htests%d",j+4),varsToProjectTest[j].c_str(),"status2");
		treeHisto->Project(Form("htests%d",j+7),varsToProjectTest[j].c_str(),"N2_status1");
		treeHisto->Project(Form("htests%d",j+10),varsToProjectTest[j].c_str(),"N2_status2");

		//chi2pid < (3,2,1) (status>=2110)
		treeHisto->Project(Form("htestc%d",j+1),varsToProjectTest[j].c_str(),"chi2pid1");
		treeHisto->Project(Form("htestc%d",j+4),varsToProjectTest[j].c_str(),"chi2pid2");
		treeHisto->Project(Form("htestc%d",j+7),varsToProjectTest[j].c_str(),"chi2pid3");
		treeHisto->Project(Form("htestc%d",j+10),varsToProjectTest[j].c_str(),"N2_chi2pid1");
		treeHisto->Project(Form("htestc%d",j+13),varsToProjectTest[j].c_str(),"N2_chi2pid2");
		treeHisto->Project(Form("htestc%d",j+16),varsToProjectTest[j].c_str(),"N2_chi2pid3");
	}
	//3D
	// sector 3
	treeHisto->Project("ht3D1","PhiV:ThetaV:P","S3");
	treeHisto->Project("hs3D1","PhiV:ThetaV:P","S3N2");
	// sector 5
	treeHisto->Project("ht3D2","PhiV:ThetaV:P","S5");
	treeHisto->Project("hs3D2","PhiV:ThetaV:P","S5N2");

	//define histograms arrays for efficiency (ratio total/selected)
	TH1F * hrt[24];
	TH2F * hrt2[10];
	TH3F * hrt3D[2];

	//create the ratio histograms
	for(int j=0; j<6; j++){ 
		//1D
		// sector 3
		//// nphe>1
		hrt[j] = (TH1F*) hsel_1[j]->Clone(Form("hrt%d",j+1));
		hrt[j]->SetTitle(Form("Efficiency in sector 3 [%s]; %s",varsToProject[j].c_str(),var[j].c_str()));
		hrt[j]->SetStats(0);
		hrt[j]->Divide(htot[j]);
		//// nphe>2
		hrt[j+6] = (TH1F*) hsel_2[j]->Clone(Form("hrt%d",j+7));
		hrt[j+6]->SetTitle(Form("Efficiency in sector 3 [%s]; %s",varsToProject[j].c_str(),var[j].c_str()));
		hrt[j+6]->SetStats(0);
		hrt[j+6]->Divide(htot[j]);
		// sector 5
		//// nphe>1
		hrt[j+12] = (TH1F*) hsel_1[j+6]->Clone(Form("hrt%d",j+13));
		hrt[j+12]->SetTitle(Form("Efficiency in sector 5 [%s]; %s",varsToProject[j].c_str(),var[j].c_str()));
		hrt[j+12]->SetStats(0);
		hrt[j+12]->Divide(htot[j+6]);
		//// nphe>2
		hrt[j+18] = (TH1F*) hsel_2[j+6]->Clone(Form("hrt%d",j+19));
		hrt[j+18]->SetTitle(Form("Efficiency in sector 5 [%s]; %s",varsToProject[j].c_str(),var[j].c_str()));
		hrt[j+18]->SetStats(0);
		hrt[j+18]->Divide(htot[j+6]);
	}
	for(int j=0; j<5; j++){
		//2D
		// sector 3
		hrt2[j] = (TH2F*) hsel2[j]->Clone(Form("hrt2%d",j+1));
		hrt2[j]->SetTitle(Form("Efficiency in sector 3 [%s]; %s",pair[j][0].c_str(),pair[j][1].c_str()));
		hrt2[j]->SetStats(0);
		hrt2[j]->Divide(htot2[j]);
		// sector 5
		hrt2[j+5] = (TH2F*) hsel2[j+5]->Clone(Form("hrt2%d",j+6));
		hrt2[j+5]->SetTitle(Form("Efficiency in sector 5 [%s]; %s",pair[j][0].c_str(),pair[j][1].c_str()));
		hrt2[j+5]->SetStats(0);
		hrt2[j+5]->Divide(htot2[j+5]);
	}
	//3D
	// sector 3
	hrt3D[0] = (TH3F*) hsel3D[0]->Clone("hrt3D1");
	hrt3D[0]->SetTitle("Efficiency in sectors 3 [P:theta:phi]; P(GeV/c); #theta(deg); #phi(deg)");
	hrt3D[0]->SetStats(0);
	hrt3D[0]->Divide(htot3D[0]);
	// sector 5
	hrt3D[1] = (TH3F*) hsel3D[1]->Clone("hrt3D2");
	hrt3D[1]->SetTitle("Efficiency in sectors 5 [P:theta:phi]; P(GeV/c); #theta(deg); #phi(deg)");
	hrt3D[1]->SetStats(0);
	hrt3D[1]->Divide(htot3D[1]);
	
	// name the ouput file
	string output = treeFile;
	output.erase(output.begin(),output.begin()+20);
	output.erase(output.end()-5,output.end());
	
	//create an output file `out.root` with all histograms (organized in canvases)
	TFile* out = new TFile(Form("out_%s.root",output.c_str()),"RECREATE");
	//array of canvases pointers
	TCanvas** can = new TCanvas*[11];
	
	gStyle->SetOptStat(11); //only name of histogram and number of entries
	//draw and save canvases
	for(int k=0; k<6; k++){
		//1D HISTOGRAMS
		can[k] = new TCanvas(Form("can%d",k),Form("can%d",k),800,1200);
		can[k]->Divide(2,3);
		//Candidates in LTCC (sec 3)
		can[k]->cd(1);
		htot[k]->SetLineColor(kBlack);
		htot[k]->Draw(); //htot35
		//Candidates in LTCC (sec 5)
		can[k]->cd(2); 
		htot[k+6]->SetLineColor(kBlack);
		htot[k+6]->Draw();
		//Candidates hits in LTCC (sec 3)
		can[k]->cd(3);
		hsel_1[k]->Draw(); //hsel35_1[k]
		hsel_2[k]->SetLineColor(kRed); //hsel35_2[k]
		auto legend1 = new TLegend(0.9,0.9,0.7,0.8);
		legend1->AddEntry(hsel_1[k],"n_{phe}>1","l"); //hsel35_1
   		legend1->AddEntry(hsel_2[k],"n_{phe}>2","l"); //hsel35_2
   		legend1->Draw();
		hsel_2[k]->Draw("SAME"); //hsel35_2
		//Candidates hits in LTCC (sec 5)
   		can[k]->cd(4);
   		hsel_1[k+6]->Draw();
		hsel_2[k+6]->SetLineColor(kRed);
		auto legend = new TLegend(0.9,0.9,0.7,0.8);
		legend->AddEntry(hsel_1[k+6],"n_{phe}>1","l");
   		legend->AddEntry(hsel_2[k+6],"n_{phe}>2","l");
   		legend->Draw();
		hsel_2[k+6]->Draw("SAME");
		//Efficiency in sector 3
		can[k]->cd(5);
		hrt[k]->Draw();
		hrt[k+6]->SetLineColor(kRed);
		auto legend2 = new TLegend(0.9,0.9,0.7,0.8);
		legend2->AddEntry(hrt[k],"n_{phe}>1","l");
   		legend2->AddEntry(hrt[k+6],"n_{phe}>2","l");
   		legend2->Draw();
		hrt[k+6]->Draw("SAME");
		//Efficiency in sector 5
		can[k]->cd(6);
		hrt[k+12]->Draw();
		hrt[k+18]->SetLineColor(kRed);
		auto legend3 = new TLegend(0.9,0.9,0.7,0.8);
		legend3->AddEntry(hrt[k+12],"n_{phe}>1","l");
   		legend3->AddEntry(hrt[k+18],"n_{phe}>2","l");
   		legend3->Draw();
		hrt[k+18]->Draw("SAME");

		can[k]->Write();
	}
	for(int k=0; k<5; k++){
		//2D HISTOGRAMS
		string option;
		option="CONT4Z";
		
		can[k+6] = new TCanvas(Form("can2_%d",k),Form("can2_%d",k),800,1200);
		can[k+6]->Divide(2,3);
		//Candidates in LTCC (sec 3)
		can[k+6]->cd(1);
		htot2[k]->Draw(option.c_str());
		//Candidates in LTCC (sec 5)
		can[k+6]->cd(2);
		htot2[k+5]->Draw(option.c_str());
		//Candidates hits in LTCC (sec 3)
		can[k+6]->cd(3);
		hsel2[k]->Draw(option.c_str());
		//Candidates hits in LTCC (sec 5)
		can[k+6]->cd(4);
		hsel2[k+5]->Draw(option.c_str());
		//Efficiency in sector 3
		can[k+6]->cd(5);
		hrt2[k]->Draw(option.c_str());
		//Efficiency in sector 5
		can[k+6]->cd(6);
		hrt2[k+5]->Draw(option.c_str());

		can[k+6]->Write();

	}
	
	// save in two file (one per sector) the table (P,theta,phi,efficiency)
	// the center of P, theta and phi bins is saved
	ofstream s3out("P_theta_phi_efficiency_s3.dat");
	ofstream s5out("P_theta_phi_efficiency_s5.dat");
	
	//sector 3
	for(int i=0; i<hrt3D[0]->GetNbinsX(); i++){// loop over bins of P
		for(int j=0; j<hrt3D[0]->GetNbinsY(); j++){// loop over bins of theta
			for(int k=0; k<hrt3D[0]->GetNbinsZ(); k++){// loop over bins of phi
				s3out<<hrt3D[0]->GetXaxis()->GetBinCenter(i);
				s3out<<"\t"<<hrt3D[0]->GetYaxis()->GetBinCenter(j);
				s3out<<"\t"<<hrt3D[0]->GetZaxis()->GetBinCenter(k);
				s3out<<"\t"<<hrt3D[0]->GetBinContent(i,j,k)<<endl;
			}
		}
	}
	//sector 5
	for(int i=0; i<hrt3D[1]->GetNbinsX(); i++){
		for(int j=0; j<hrt3D[1]->GetNbinsY(); j++){
			for(int k=0; k<hrt3D[1]->GetNbinsZ(); k++){
				s5out<<hrt3D[1]->GetXaxis()->GetBinCenter(i);
				s5out<<"\t"<<hrt3D[1]->GetYaxis()->GetBinCenter(j);
				s5out<<"\t"<<hrt3D[1]->GetZaxis()->GetBinCenter(k);
				s5out<<"\t"<<hrt3D[1]->GetBinContent(i,j,k)<<endl;
			}
		}
	}
	
	//save the canvases in a unique pdf file
	//one canvas for each page
	can[0]->SaveAs(Form("out_%s.pdf(",output.c_str())); // <-- first page
	can[1]->SaveAs(Form("out_%s.pdf",output.c_str()));
	can[2]->SaveAs(Form("out_%s.pdf",output.c_str()));
	can[3]->SaveAs(Form("out_%s.pdf",output.c_str()));
	can[4]->SaveAs(Form("out_%s.pdf",output.c_str()));
	can[5]->SaveAs(Form("out_%s.pdf",output.c_str()));
	can[6]->SaveAs(Form("out_%s.pdf",output.c_str()));
	can[7]->SaveAs(Form("out_%s.pdf",output.c_str()));
	can[8]->SaveAs(Form("out_%s.pdf",output.c_str()));
	can[9]->SaveAs(Form("out_%s.pdf",output.c_str()));
	can[10]->SaveAs(Form("out_%s.pdf)",output.c_str())); // <-- last page

	out->Close();
	s3out.close();
	s5out.close();

	////////////////////////////////////////////////
	// TESTS FOR CONDITIONS ON STATUS AND CHI2PID //
	////////////////////////////////////////////////
	TCanvas** cant = new TCanvas*[6];
	for(int k=0; k<3; k++){
		cant[k] = new TCanvas(Form("cant%d",k+1),Form("cant%d",k+1),1200,600);
		cant[k]->Divide(2,1);
		cant[k]->cd(1);
		htest_status[k]->Draw();
		htest_status[k+3]->SetLineColor(kRed);
		htest_status[k+3]->Draw("SAME");
		auto legend = new TLegend(0.9,0.9,0.7,0.8);
		legend->AddEntry(htest_status[k],"status #geq 2110","l");
   		legend->AddEntry(htest_status[k+3],"status #geq 2230","l");
   		legend->Draw("SAME");
		cant[k]->cd(2);
		htest_status[k+6]->Draw();
		htest_status[k+9]->SetLineColor(kRed);
		htest_status[k+9]->Draw("SAME");
		auto legend1 = new TLegend(0.9,0.9,0.7,0.8);
		legend1->AddEntry(htest_status[k+6],"status #geq 2110, n_{phe}>2","l");
   		legend1->AddEntry(htest_status[k+9],"status #geq 2230, n_{phe}>2","l");
   		legend1->Draw("SAME");
		
		cant[k+3] = new TCanvas(Form("cant%d",k+4),Form("cant%d",k+4),1200,600);
		cant[k+3]->Divide(2,1);
		cant[k+3]->cd(1);
		htest_chi2pid[k]->Draw();
		htest_chi2pid[k+3]->SetLineColor(kRed);
		htest_chi2pid[k+3]->Draw("SAME");
		htest_chi2pid[k+6]->SetLineColor(kGreen);
		htest_chi2pid[k+6]->Draw("SAME");
		auto legend2 = new TLegend(0.9,0.9,0.7,0.8);
		legend2->AddEntry(htest_chi2pid[k],"abs(#chi^{2}_{PID})<3","l");
   		legend2->AddEntry(htest_chi2pid[k+3],"abs(#chi^{2}_{PID})<2","l");
		legend2->AddEntry(htest_chi2pid[k+6],"abs(#chi^{2}_{PID})<1","l");
   		legend2->Draw("SAME");
		
		cant[k+3]->cd(2);
		htest_chi2pid[k+9]->Draw();
		htest_chi2pid[k+12]->SetLineColor(kRed);
		htest_chi2pid[k+12]->Draw("SAME");
		htest_chi2pid[k+15]->SetLineColor(kGreen);
		htest_chi2pid[k+15]->Draw("SAME");
		auto legend3 = new TLegend(0.9,0.9,0.7,0.8);
		legend3->AddEntry(htest_chi2pid[k+9],"abs(#chi^{2}_{PID})<3, n_{phe}>2","l");
   		legend3->AddEntry(htest_chi2pid[k+12],"abs(#chi^{2}_{PID})<2, n_{phe}>2","l");
		legend3->AddEntry(htest_chi2pid[k+15],"abs(#chi^{2}_{PID})<1, n_{phe}>2","l");
   		legend3->Draw("SAME");
	}

	//save the tests histograms in a pdf file tests.pdf
	cant[0]->SaveAs("tests.pdf(");
	cant[1]->SaveAs("tests.pdf");
	cant[2]->SaveAs("tests.pdf");
	cant[3]->SaveAs("tests.pdf");
	cant[4]->SaveAs("tests.pdf");
	cant[5]->SaveAs("tests.pdf)");

}

//OLD PARTS OF CODE (EVENTUALLY USEFUL)

//Code to obtain histograms for total event (sector 3 AND 5) 
/*	TH1F* htot35[6];
	TH1F* hsel35_1[6];
	TH1F* hsel35_2[6];
	TH2F* htot235[5];
	TH2F* hsel235[5];

	// 1D and 2D histograms both sectors 3 and 5
	for(int l=0; l<6; l++){//1D
		TList* tot35 = new TList;
		htot35[l] = new TH1F(Form("htot35%d",l),Form("Candidates in LTCC [%s]; %s",varsToProject[l].c_str(),var[l].c_str()),bins[l],inf[l],max[l]);
		tot35->Add(htot[l]);
		tot35->Add(htot[l+6]);
		htot35[l]->Merge(tot35);
		delete tot35;

		TList* sel35_1 = new TList;
		hsel35_1[l] = new TH1F(Form("hsel35_1%d",l),Form("Candidate hits in LTCC [%s]; %s",varsToProject[l].c_str(),var[l].c_str()),bins[l],inf[l],max[l]);
		sel35_1->Add(hsel_1[l]);
		sel35_1->Add(hsel_1[l+6]);
		hsel35_1[l]->Merge(sel35_1);
		delete sel35_1;

		TList* sel35_2 = new TList;
		hsel35_2[l] = new TH1F(Form("hsel35_2%d",l),Form("Candidate hits in LTCC [%s]; %s",varsToProject[l].c_str(),var[l].c_str()),bins[l],inf[l],max[l]);
		sel35_2->Add(hsel_2[l]);
		sel35_2->Add(hsel_2[l+6]);
		hsel35_2[l]->Merge(sel35_2);
		delete sel35_2;
	}
	for(int l=0; l<5; l++){//2D
		TList* tot235 = new TList;
		htot235[l] = new TH2F(Form("htot235%d",l),Form("Candidates in LTCC [%s]; %s",pair[l][0].c_str(),pair[l][1].c_str()),bins2[l][0],inf2[l][0],max2[l][0],bins2[l][1],inf2[l][1],max2[l][1]);
		tot235->Add(htot2[l]);
		tot235->Add(htot2[l+5]);
		htot235[l]->Merge(tot235);
		delete tot235;

		TList* sel235 = new TList;
		hsel235[l] = new TH2F(Form("hsel235%d",l),Form("Candidate hits in LTCC [%s]; %s",pair[l][0].c_str(),pair[l][1].c_str()),bins2[l][0],inf2[l][0],max2[l][0],bins2[l][1],inf2[l][1],max2[l][1]);
		sel235->Add(hsel2[l]);
		sel235->Add(hsel2[l+5]);
		hsel235[l]->Merge(sel235);
		delete sel235;
	}*/

//Code for polar coordinates axes
/*void SetToPolar(TH2F* h, TCanvas* c){
	// hide the cartesian axes system
	h->GetXaxis()->SetLabelOffset(999);
	h->GetXaxis()->SetLabelSize(0);
	h->SetAxisColor(kWhite,"x");
	h->GetXaxis()->SetTickLength(0);
	h->GetXaxis()->SetTitle("");
	
	h->GetYaxis()->SetLabelOffset(999);
	h->GetYaxis()->SetLabelSize(0);
	h->SetAxisColor(kWhite,"y");
	h->GetYaxis()->SetTickLength(0);
	h->GetYaxis()->SetTitle("");
	// save min and max value of theta
	double ymax = h->GetYaxis()->GetXmax();
	double ymin = h->GetYaxis()->GetXmin();

	// create a TGraphPolargram
	// radial: theta, polar: phi
	TGraphPolargram* gp = new TGraphPolargram("gp",ymin,ymax,0,360);
	gp->SetToDegree();
	gp->SetNdivPolar(6);
	gp->SetNdivRadial(4);
	gp->SetPolarLabelFont(42);
	gp->SetRadialLabelFont(42);
	gp->Draw();
	c->Modified();
	c->Update();
}*/
