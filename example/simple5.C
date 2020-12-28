// Run with:
// clas12root -b -q simple5.C

void PrintFitInfo(TF1* f_gaus, TF1* f_lorentz);

void simple5(){
	// Create histograms
	TH1F* h_pi0_mass = new TH1F("h_pi0_mass","Mass of #pi^{0}", 100, 0., 0.25);
	TH1F* h_time = new TH1F("h_time","#Deltat (photons)", 100, -5, 5);
	TH2F* h_time2 = new TH2F("h_time2", "#Deltat_{ e#gamma1} vs #Deltat_{ e#gamma2}", 100, 1,-1, 100, 1,-1);
	// Define fitting functions
	TF1* f_gaus = new TF1("f_gaus","gaus",0.1225,0.1475);				// Gaussian: 		Const*exp(-(x-x0)^2/(2*sigma^2))
	TF1* f_lorentz = new TF1("f_lorentz", "[0]/((x-[1])^2+[2])",-4,4);		// Cauchy-Lorentz: 	C/((t-t0)^2+g^2)
	f_lorentz->SetParName(0,"C");
	f_lorentz->SetParName(1,"t0");
	f_lorentz->SetParName(2,"g^2");
	
	// Hipo Chain to read hipo files
	HipoChain chain;
	chain.Add("./skim2_005339_first1000.hipo");
	chain.Add("./skim2_005343_first1000.hipo");
	
	// Define Lorentz vector
	TLorentzVector p4_photon1, p4_photon2;
	
	// Loop over files
	for(int ifile = 0; ifile<chain.GetNFiles(); ++ifile){
		// clas12reader from chain
		clas12reader c12{chain.GetFileName(ifile).Data()};
		
		// Add filters for events
		c12.addExactPid(11,1);		// Add events with 1 electron
		c12.addExactPid(22,2);		// Add events with 2 photons
//		c12.addExactPid(2212,1);	// Add events with 1 proton
		
		// Loop over events
		while(c12.next()==true){
		auto parts = c12.getDetParticles();
		// Particles
		auto electron = c12.getByID(11)[0];
		auto photon1 = c12.getByID(22)[0];
		auto photon2 = c12.getByID(22)[1];
		
		// Define 4-momentum for photons
		p4_photon1.SetXYZM(photon1->par()->getPx(), photon1->par()->getPy(), photon1->par()->getPz(), 0);
		p4_photon2.SetXYZM(photon2->par()->getPx(), photon2->par()->getPy(), photon2->par()->getPz(),0);
		
		// Define 4-momentum of pion0 as sum of the photons 4-momenta (pi0->gamma+gamma decay)
		auto pi0 = p4_photon1 + p4_photon2;
		
		// Time of particles
		auto time_e = (electron->getTime());
		auto time_photon1 = (photon1->getTime());
		auto time_photon2 = (photon2->getTime());
		
		// Select event with 1 electron in the FT Region and constraints on pz of electron
		if((electron->getRegion() == FT)){
			// Fill histograms
			h_pi0_mass->Fill(pi0.M());
			h_time->Fill(time_photon1-time_photon2);
			h_time2->Fill(time_e-time_photon1, time_e-time_photon2);
			}
		}
	}
	
	// Create a canvas of 1D-histograms
	TCanvas* c_hfit = new TCanvas("c_hfit","1D-Histograms from simple5.C macro",1500,500);
	c_hfit->Divide(2,1);
	
	// Pion0 mass histogram
	c_hfit->cd(1);
	h_pi0_mass->GetXaxis()->SetTitle("M_{ #pi0} [GeV]");
	h_pi0_mass->GetYaxis()->SetTitle("Counts [#]");
	f_gaus->SetLineColor(2);
	f_gaus->SetLineStyle(9);
	h_pi0_mass->Fit("f_gaus","R");		// Fit with Gaussian function
	
	// Photons time difference histogram
	c_hfit->cd(2);
	h_time->GetXaxis()->SetTitle("#Deltat_{ 2#gamma} [ns]");
	h_time->GetYaxis()->SetTitle("Counts [#]");
	f_lorentz->SetLineColor(2);
	f_lorentz->SetLineStyle(9);
	h_time->Fit("f_lorentz","R");		// Fit with Cauchy-Lorentz distributioon
	
	// Create plot of 2D-histogram (delta time electron-gammas)
	h_time2->GetXaxis()->SetTitle("#Deltat_{ e#gamma1} [ns]");
	h_time2->GetYaxis()->SetTitle("#Deltat_{ e#gamma2} [ns]");
	
	// Save canvas and plots on "out_simple5.root" root file
	// Access with:
	// root -l out_simple5.root
	// Then:
	// name_of_canvas/plot->Draw()
	TFile* out = new TFile("out_simple5.root", "RECREATE");
	c_hfit->Write();
	h_time2->Write();
	out->Close();
	
	PrintFitInfo(f_gaus, f_lorentz);
	return;
}

void PrintFitInfo(TF1* f_gaus, TF1* f_lorentz){
	// Save on "fit_par.txt" text file the fitting parameters
	FILE* fit_par = fopen("fit_par.txt", "w");
	if(fit_par!=NULL){
	int N1 = f_gaus->GetNpar();
	int N2 = f_lorentz->GetNpar();
	fprintf(fit_par,"Fit for pi0 mass [GeV]:\n");
	fprintf(fit_par,"Function: Gaussian = Const*exp[-(M-Mean)^2/(2*Sigma^2)]\n");
	fprintf(fit_par,"Number of parameters: %d\n", N1);
	fprintf(fit_par,"\n");
	fprintf(fit_par,"Param \t\t Value \t\t Error\n");
		for(int i=0; i<N1;i++){
			float value = f_gaus->GetParameter(i);
			auto name = f_gaus->GetParName(i);
			float error = f_gaus->GetParError(i);
			fprintf(fit_par,"%-3.5s \t\t%f \t%f\n", name, value, error);
		}
	fprintf(fit_par,"_____________________\n");
	fprintf(fit_par,"\n");
	fprintf(fit_par,"Fit for photon times difference [ns]:\n");
	fprintf(fit_par,"Function: Cauchy-Lorentz distribution = C/[(t-t0)^2+g^2]\n");
	fprintf(fit_par,"Number of parameters: %d\n", N2);
	fprintf(fit_par,"\n");
	fprintf(fit_par,"Param \t\t Value \t\t Error\n");
		for(int i=0; i<N2;i++){
			float value = f_lorentz->GetParameter(i);
			auto name = f_lorentz->GetParName(i);
			float error = f_lorentz->GetParError(i);
			fprintf(fit_par,"%-s \t\t%f \t%f\n", name, value, error);
		}
	}
	
	// Close text file
	fclose(fit_par);
}


