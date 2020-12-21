// run with: clas12root -b -q simple3.C

void simple3(){
	// create histograms
	TH1F* mom = new TH1F("mom", "Momentum (electrons)", 100, 1, -1);
	TH1F* phi = new TH1F("phi", "#phi distribution", 100, 1, -1);
	TH1F* theta = new TH1F("theta", "#theta distribution", 100, 1, -1);
	
	// create a clas12reader
	clas12reader c12("./skim2_005339_first1000.hipo");

	// loop over events
	while(c12.next()){
		auto particles = c12.getDetParticles();

		// loop over particles
		for(int i=0; i<particles.size();i++){
			int PID = particles[i]->getPid();
			if(PID == 11){					// select only electrons (PID = 11)
				mom->Fill(particles[i]->getP());	// momentum of electrons
				phi->Fill(particles[i]->getPhi());	// angle phi of electrons
				theta->Fill(particles[i]->getTheta());	// angle theta of electrons
			}
		}
	}

	// create root file
	TFile *output = new TFile("out_simple3.root","RECREATE");
	mom->Write();
	phi->Write();
	theta->Write();
	output->Close();
	return;

}
