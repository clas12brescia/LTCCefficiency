// Run with:
// clas12root -b -q simple6.C --in=name_of_input_list.dat/.txt
//
//You can create such a list using:
// ls -1 /path/of/directory/"*".hipo > name_of_input_list.dat/.txt
// ls -1 /path/of/directory/skim#_{005039..005042} > name_of_input_list.dat/.txt
// ls -1 /path/of/directory1/"*".hipo /path/of/directory2/"*".hipo > name_of_input_list.dat/.txt

void simple6(){
	// Create histograms
	TH1F* h_time = new TH1F("h_time","#Deltat (pions)", 200, -100, 150);
	TH1F* h_Nphep = new TH1F("h_Nphep","N_{ phe} (#pi^{+})", 70, 1, -1);
	TH1F* h_Nphem = new TH1F("h_Nphem","N_{ phe} (#pi^{-})", 70, 1, -1);
	TH2F* h_xyp = new TH2F("h_xyp","#pi^{+} positions in xy plane", 80, 1, -1, 80, 1, -1);
	TH2F* h_xym = new TH2F("h_xym","#pi^{-} positions in xy plane", 80,1,-1,80,1,-1);
	
	// Read input files from a list from the terminal
	TString inFile;
	for(int i=1; i<gApplication->Argc();i++){
		TString opt = gApplication->Argv(i);
		if((opt.Contains(".dat")||opt.Contains(".txt"))){
			inFile = opt(5, opt.Sizeof());
		}
	}
	
	// Create HipoChain with the list of files
	HipoChain chain;
	TString nextFile;
	ifstream chainIn(inFile);
	while(chainIn >> nextFile){
		chain.Add(nextFile);
	}
	
	// Loop over files
	for(int ifile = 0; ifile<chain.GetNFiles(); ++ifile){
		// clas12reader from chain
		clas12reader c12{chain.GetFileName(ifile).Data()};
		
		// Select events with exactly:
		c12.addExactPid(11,1);		// 1 electron
		c12.addExactPid(2212,1);	// 1 proton
		
		// Select events with at least:
		c12.addAtLeastPid(-211,1);	// 1 pion minus
		c12.addAtLeastPid(211,1);	// 1 pion plus
		
		// Loop over events
		while(c12.next()==true){
		
		// Delete empty events
		if(c12.getDetParticles().empty()) continue;
		
		auto parts = c12.getDetParticles();
		// Particles' pointers
		auto electron = c12.getByID(11)[0];
		auto pionm = c12.getByID(-211)[0];
		auto pionp = c12.getByID(211)[0];
		
		// Time of particles
		auto time_e = (electron->getTime());
		auto time_pionm = (pionm->getTime());
		auto time_pionp = (pionp->getTime());
		
		// Regions and sectors of pions
		int ppReg = pionp->getRegion();
		int ppSec = pionp->getSector();
		int pmReg = pionm->getRegion();
		int pmSec = pionm->getSector();
		
		// Number of photoelectrons for pions
		double nphem = pionm->che(LTCC)->getNphe();
		double nphep = pionp->che(LTCC)->getNphe();
		
		// Coordinates of pions in LTCC
		double xp = pionp->che(LTCC)->getX();
		double yp = pionp->che(LTCC)->getY();
		double zp = pionp->che(LTCC)->getZ();
		
		double xm = pionm->che(LTCC)->getX();
		double ym = pionm->che(LTCC)->getY();
		double zm = pionm->che(LTCC)->getZ();
		
		// Select event with 1 pion plus in the FD Region ( = 2000)
		// in sectors 3 and 5 (active sectors)
		if( (ppReg == 2000) && ((ppSec == 3) || (ppSec == 5)) ){
			// Fill histograms
			h_time->Fill(time_pionp-time_pionm);
			h_Nphep->Fill(nphep);
			}
		
		// Same for pion minus
		if( (pmReg == 2000) && ((pmSec == 3) || (pmSec == 5)) ){
			h_time->Fill(time_pionp-time_pionm);
			h_Nphem->Fill(nphem);
			}
		
		// Position (x,y) for each pions (same selectiona as before)
		// and number of photoelectrons revealed in (x,y)
		if( (ppReg==2000) & ((ppSec == 3) || (ppSec == 5)) /*&& abs(time_pionp-time_pionm) < 6*/){			// pion plus
			h_xyp->Fill(xp,yp,nphep);
			}
		if( (pmReg == 2000) && ((pmSec == 3) || (pmSec == 5)) /*&& abs(time_pionp - time_pionm) < 6*/){		// pion minus
			h_xym->Fill(xm,ym,nphem);
			}
		} // Loop over events
	} // Loop over files
	
	
	// Create output root file
	TFile* out = new TFile("out_simple6.root", "RECREATE");
	
	// Pions time difference histogram
	h_time->GetXaxis()->SetTitle("#Deltat_{ 2#pi} [ns]");
	h_time->GetYaxis()->SetTitle("Counts [a.u.]");
	h_time->Write();
	
	// Canvas with number of photoelectrons from pions
	TCanvas* c_Nphe = new TCanvas("c_Nphe","Number of photoelectrons", 1250, 500);
	c_Nphe->Divide(2,1);
	c_Nphe->cd(1);
	h_Nphep->GetXaxis()->SetTitle("N_{ phe} (#pi^{+})");
	h_Nphep->GetYaxis()->SetTitle("Counts [a.u.]");
	gPad->SetLogy();
	h_Nphep->Draw();
	c_Nphe->cd(2);
	h_Nphem->GetXaxis()->SetTitle("N_{ phe} (#pi^{-})");
	h_Nphem->GetYaxis()->SetTitle("Counts [a.u.]");
	gPad->SetLogy();
	h_Nphem->Draw();
	
	c_Nphe->Write();
	
	// Canvas with histogram of pions' positions and respective photoelectrons
	TCanvas* c_xy = new TCanvas("c_xy","Positions of pions",1250,500);
	c_xy->Divide(2,1);
	c_xy->cd(1);
	h_xyp->GetXaxis()->SetTitle("x [cm]");
	h_xyp->GetYaxis()->SetTitle("y [cm]");
	gPad->SetLogz();
	h_xyp->Draw("COLZ");
	c_xy->cd(2);
	h_xym->GetXaxis()->SetTitle("x [cm]");
	h_xym->GetYaxis()->SetTitle("y [cm]");
	gPad->SetLogz();
	h_xym->Draw("COLZ");
	
	c_xy->Write();
	
	// Close output root file
	out->Close();
	
	return;
}
