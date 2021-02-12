//makeHistos.cxx

/*
This macro is made to draw and save histograms from a TTree.
As default, the input will be the file named `LTCCefficiency_tree.root`.
Use:
root -l 'makeHistos.cxx("tree_filename.root")'
if you named this file differently from the default value.
*/

const double rad_deg = 57.2958;

void makeHistos(string treeFile="LTCCefficiency_tree.root"){
	//read the TTree from root file created by LTCCefficiency.cxx
	TFile *ftree = new TFile(treeFile.c_str());
	TTree *treeHisto = (TTree*)ftree->Get("treeHisto");
	
	//define histograms arrays to plot total and selected events
	//total = only kinetic cuts; selected = nphe>1 (+ nphe>2 for 1D)
	TH1F * htot[8];
	TH1F * hsel_1[8];
	TH1F * hsel_2[8];
	TH2F * htot2[8];
	TH2F * hsel2[8];
	TH3F * htot3D = new TH3F("ht3D","Candidates in LTCC [P:theta:phi]; P(GeV/c); #theta(deg); #phi(deg)",12,2.5,8.5,10,0,40,100,-200,200);
	TH3F * hsel3D = new TH3F("hs3D","Candidate hits in LTCC [P:theta:phi]; P(GeV/c); #theta(deg); #phi(deg)",12,2.5,8.5,10,0,40,100,-200,200);
	//min and max of ranges for variables:
	// P, theta, phi, costheta
	double inf[4]={0,0,-200,0.8};
	double max[4]={10,40,200,1};
	//ranges for 2D-histograms variables pairs:
	// (x,y), (theta, phi), (costheta, phi), (P, costheta)
	double inf2[4][2]={{-500,-500},{-200,0},{0.8,-200},{0,0.8}};
	double max2[4][2]={{500,500},{200,40},{1,200},{10,1}};
	//customize the binning of the histograms
	// P, theta, phi, costheta
	//double bins[4]={20,20,20,20};
	// (x,y), (theta, phi), (costheta, phi), (P, costheta)
	//double bins[4][2]={{20,20},{20,20},{20,20},{20,20}};
	
	//title of axes, variables and variables pairs
	string var[6]={"P(GeV/c)","#theta(deg)","#phi(deg)","cos(#theta)(#)"};
	string varsToProject[4] = {"P", "theta", "phi", "costheta"};
	string pair[4][2]={{"Y:X","x(cm); y(cm)"},{"theta:phi","#phi(deg);#theta(deg)"},{"phi:costheta","cos#theta(#); #phi(deg)"},{"costheta:P","P(GeV/c); cos#theta(#)"}};
	
	//create histograms arrays with ranges and title defined before
	for(int j=0; j<4; j++){
		htot[j] = new TH1F(Form("ht%d",j+1),Form("Candidates in LTCC [%s]; %s",varsToProject[j].c_str(),var[j].c_str()),20,inf[j],max[j]);
		hsel_1[j] = new TH1F(Form("hs1%d",j+1),Form("Candidate hits in LTCC [%s]; %s",varsToProject[j].c_str(),var[j].c_str()),20,inf[j],max[j]);
		htot[j+4] = new TH1F(Form("ht%d",j+5),Form("Candidates in LTCC [%s]; %s",varsToProject[j].c_str(),var[j].c_str()),20,inf[j],max[j]);
		hsel_1[j+4] = new TH1F(Form("hs1%d",j+5),Form("Candidate hits in LTCC [%s]; %s",varsToProject[j].c_str(),var[j].c_str()),20,inf[j],max[j]);
		hsel_2[j] = new TH1F(Form("hs2%d",j+1),Form("Candidate hits in LTCC [%s]; %s",varsToProject[j].c_str(),var[j].c_str()),20,inf[j],max[j]);
		hsel_2[j+4] = new TH1F(Form("hs2%d",j+5),Form("Candidate hits in LTCC [%s]; %s",varsToProject[j].c_str(),var[j].c_str()),20,inf[j],max[j]);
		htot2[j] = new TH2F(Form("h2%d",j+1),Form("Candidates in LTCC [%s]; %s",pair[j][0].c_str(),pair[j][1].c_str()),20,inf2[j][0],max2[j][0],20,inf2[j][1],max2[j][1]);
		hsel2[j] = new TH2F(Form("h2%d",j+5),Form("Candidate hits in LTCC [%s]; %s",pair[j][0].c_str(),pair[j][1].c_str()),20,inf2[j][0],max2[j][0],20,inf2[j][1],max2[j][1]);
		htot2[j+4] = new TH2F(Form("h2%d",j+9),Form("Candidates in LTCC [%s]; %s",pair[j][0].c_str(),pair[j][1].c_str()),20,inf2[j][0],max2[j][0],20,inf2[j][1],max2[j][1]);
		hsel2[j+4] = new TH2F(Form("h2%d",j+13),Form("Candidate hits in LTCC [%s]; %s",pair[j][0].c_str(),pair[j][1].c_str()),20,inf2[j][0],max2[j][0],20,inf2[j][1],max2[j][1]);
	}
	//fill the histograms arrays with the tree branches
	for(int j=0; j<4; j++){
		//1D
		// sector 3
		treeHisto->Project(Form("ht%d",j+1), varsToProject[j].c_str(), "abs(chi2pid)<3 && status>2109 && y>0");
		treeHisto->Project(Form("hs1%d",j+1), varsToProject[j].c_str(), "abs(chi2pid)<3 && status>2109 && y>0 && nphe>1");	//condition on photoelectrons
		treeHisto->Project(Form("hs2%d",j+1), varsToProject[j].c_str(), "abs(chi2pid)<3 && status>2109 && y>0 && nphe>2");	//condition on photoelectrons
		// sector 5
		treeHisto->Project(Form("ht%d",j+5), varsToProject[j].c_str(), "abs(chi2pid)<3 && status>2109 && y<0");
		treeHisto->Project(Form("hs1%d",j+5), varsToProject[j].c_str(), "abs(chi2pid)<3 && status>2109 && y<0 && nphe>1");	//condition on photoelectrons
		treeHisto->Project(Form("hs2%d",j+5), varsToProject[j].c_str(), "abs(chi2pid)<3 && status>2109 && y<0 && nphe>2");	//condition on photoelectrons

		//2D
		// sector 3
		treeHisto->Project(Form("h2%d",j+1), pair[j][0].c_str(),"abs(chi2pid)<3 && status>2109 && y>0");
		treeHisto->Project(Form("h2%d",j+5), pair[j][0].c_str(),"abs(chi2pid)<3 && status>2109 && nphe>2 && y>0");	//condition on photoelectrons
		// sector 5
		treeHisto->Project(Form("h2%d",j+9), pair[j][0].c_str(),"abs(chi2pid)<3 && status>2109 && y<0");
		treeHisto->Project(Form("h2%d",j+13), pair[j][0].c_str(),"abs(chi2pid)<3 && status>2109 && nphe>2 && y<0");
	}
	
	//3D
	treeHisto->Project("ht3D","phi:theta:P","abs(chi2pid)<3 && status>2109");
	treeHisto->Project("hs3D","phi:theta:P","abs(chi2pid)<3 && status>2109 && nphe>2");

	//define histograms arrays for efficiency (ratio total/selected)
	TH1F * hrt[16];
	TH2F * hrt2[8];
	TH3F * hrt3D;

	//create the ratio histograms
	for(int j=0; j<4;j++){ 
		//1D
		hrt[j] = (TH1F*) hsel_1[j]->Clone(Form("hrt%d",j+1));
		hrt[j]->SetTitle(Form("Efficiency in sector 3 [%s]; %s",varsToProject[j].c_str(),var[j].c_str()));
		hrt[j]->SetStats(0);
		hrt[j]->Divide(htot[j]);

		hrt[j+4] = (TH1F*) hsel_2[j]->Clone(Form("hrt%d",j+5));
		hrt[j+4]->SetTitle(Form("Efficiency in sector 3 [%s]; %s",varsToProject[j].c_str(),var[j].c_str()));
		hrt[j+4]->SetStats(0);
		hrt[j+4]->Divide(htot[j]);

		hrt[j+8] = (TH1F*) hsel_1[j+4]->Clone(Form("hrt%d",j+9));
		hrt[j+8]->SetTitle(Form("Efficiency in sector 5 [%s]; %s",varsToProject[j].c_str(),var[j].c_str()));
		hrt[j+8]->SetStats(0);
		hrt[j+8]->Divide(htot[j+4]);

		hrt[j+12] = (TH1F*) hsel_2[j+4]->Clone(Form("hrt%d",j+13));
		hrt[j+12]->SetTitle(Form("Efficiency in sector 5 [%s]; %s",varsToProject[j].c_str(),var[j].c_str()));
		hrt[j+12]->SetStats(0);
		hrt[j+12]->Divide(htot[j+4]);
		//2D
		hrt2[j] = (TH2F*) hsel2[j]->Clone(Form("hrt2%d",j+1));
		hrt2[j]->SetTitle(Form("Efficiency in sector 3 [%s]; %s",pair[j][0].c_str(),pair[j][1].c_str()));
		hrt2[j]->SetStats(0);
		hrt2[j]->Divide(htot2[j]);

		hrt2[j+4] = (TH2F*) hsel2[j+4]->Clone(Form("hrt2%d",j+5));
		hrt2[j+4]->SetTitle(Form("Efficiency in sector 5 [%s]; %s",pair[j][0].c_str(),pair[j][1].c_str()));
		hrt2[j+4]->SetStats(0);
		hrt2[j+4]->Divide(htot2[j+4]);
	}
	//3D
	hrt3D = (TH3F*) hsel3D->Clone("hrt3D");
	hrt3D->SetTitle("Efficiency in sectors 3 and 5 [P:theta:phi]; P(GeV/c); #theta(deg); #phi(deg)");
	hrt3D->SetStats(0);
	hrt3D->Divide(htot3D);
	
	//name the ouput file
	string output = treeFile;
	output.erase(output.begin(),output.begin()+20);
	output.erase(output.end()-5,output.end());
	
	TH1F* htot35[4];
	TH1F* hsel35_1[4];
	TH1F* hsel35_2[4];
	TH2F* htot235[4];
	TH2F* hsel235[4];

	// 1D and 2D histograms both sectors 3 and 5
	for(int l=0; l<4; l++){
		htot35[l] = (TH1F*) htot[l]->Clone("htot35");
		htot35[l]->Add(htot[l+4]);
		hsel35_1[l] = (TH1F*) hsel_1[l]->Clone("hsel35_1");
		hsel35_1[l]->Add(hsel_1[l+4]);
		hsel35_2[l] = (TH1F*) hsel_2[l]->Clone("hsel35_2");
		hsel35_2[l]->Add(hsel_2[l+4]);

		htot235[l] = (TH2F*) htot2[l]->Clone("htot235");
		htot235[l]->Add(htot2[l+4]);
		hsel235[l] = (TH2F*) hsel2[l]->Clone("hsel235");
		hsel235[l]->Add(hsel2[l+4]);
	}
	//create an output file `out.root` with all histograms (organized in canvases)
	TFile* out = new TFile(Form("out_%s.root",output.c_str()),"RECREATE");
	//array of canvases pointers
	TCanvas** can = new TCanvas*[8];
	//draw and save canvases
	for(int k=0; k<4; k++){
		//1D (from can0 to can3)
		can[k] = new TCanvas(Form("can%d",k),Form("can%d",k),800,800);
		can[k]->Divide(2,2);
		can[k]->cd(1);
		htot35[k]->Draw();
		can[k]->cd(2);
		hsel35_1[k]->Draw();
		hsel35_2[k]->SetLineColor(kRed);
		auto legend = new TLegend(0.9,0.9,0.7,0.8);
		legend->AddEntry(hsel35_1[k],"n_{phe}>1","l");
   	legend->AddEntry(hsel35_2[k],"n_{phe}>2","l");
   	legend->Draw();
		hsel35_2[k]->Draw("SAME");
		can[k]->cd(3);
		hrt[k]->Draw();
		hrt[k+4]->SetLineColor(kRed);
		auto legend2 = new TLegend(0.9,0.9,0.7,0.8);
		legend2->AddEntry(hrt[k],"n_{phe}>1","l");
   	legend2->AddEntry(hrt[k+4],"n_{phe}>2","l");
   	legend2->Draw();
		hrt[k+4]->Draw("SAME");
		can[k]->cd(4);
		hrt[k+8]->Draw();
		hrt[k+12]->SetLineColor(kRed);
		auto legend3 = new TLegend(0.9,0.9,0.7,0.8);
		legend3->AddEntry(hrt[k],"n_{phe}>1","l");
   	legend3->AddEntry(hrt[k+4],"n_{phe}>2","l");
   	legend3->Draw();
		hrt[k+12]->Draw("SAME");

		can[k]->Write();

		//2D (from can2_0 to can2_3)
		string option;
		if(k==1){
			option="CONT4Z POL";
		}
		else{
			option="CONT4Z";
		}
		can[k+4] = new TCanvas(Form("can2_%d",k),Form("can2_%d",k),800,800);
		can[k+4]->Divide(2,2);
		can[k+4]->cd(1);
		htot235[k]->Draw(option.c_str());
		can[k+4]->cd(2);
		hsel235[k]->Draw(option.c_str());
		can[k+4]->cd(3);
		hrt2[k]->Draw(option.c_str());
		can[k+4]->cd(4);
		hrt2[k+4]->Draw(option.c_str());

		can[k+4]->Write();

	}
	/*TCanvas* cancan = new TCanvas("cancan","cancan",1500,600);
	cancan->Divide(3,1);
	cancan->cd(1);
	htot3D->Draw("SURF1Z");
	cancan->cd(2);
	hsel3D->Draw("SURF1Z");
	cancan->cd(3);
	hrt3D->Draw("SURF1Z");*/
	
	ofstream fout("P_theta_phi_efficiency.dat");
	for(int i=0; i<12; i++){
		for(int j=0; j<10; j++){
			for(int k=0; k<100; k++){
				fout<<hrt3D->GetXaxis()->GetBinCenter(i);
				fout<<"\t"<<hrt3D->GetYaxis()->GetBinCenter(j);
				fout<<"\t"<<hrt3D->GetZaxis()->GetBinCenter(k);
				fout<<"\t"<<hrt3D->GetBinContent(i,j,k)<<endl;
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
	can[7]->SaveAs(Form("out_%s.pdf)",output.c_str())); // <-- last page

	out->Close();
	fout.close();

}
