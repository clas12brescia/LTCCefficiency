// Useful constants
const float E_beam = 10.4; 	// Beam energy [GeV]
const float m_p = 0.93872; 	// Proton mass [GeV]
const float m_e = 0.000511; 	// Electron mass [GeV]

void simple4(){
	// HipoChain: add more than one hipo file
	HipoChain chain;
	chain.Add("./skim2_005339_first1000.hipo");
	chain.Add("./skim2_005343_first1000.hipo");

	// Create histograms
	TH2F* pxy = new TH2F("pxy", "p_{y} vs p_{x}", 100, 1, -1, 100, 1, -1); // Scatterplot of px and py of electrons
	TH1F* q2_histo = new TH1F("q2_histo", "q^{2} distribution", 100, 1, -1); // Transferred momentum distribution
	TH1F* qt2_histo = new TH1F("qt2_histo", "q_{ t}^{2} distribution", 100, 1, -1); // Transverse transferred momentum distribution
	TH1F* t = new TH1F("t", "t_{ e}-t_{ #gamma}", 200, -30, 10); // Delta time (electrons-gammas) distribution
	
	auto c12=chain.GetC12Reader();	// Create a clas12reader from chain
	c12->addExactPid(11,1);		// Filtering: 1 electron in the event
	
	float px, py, pz, q2, qt2;	// Define momentum component and transferred momentum squared
	TLorentzVector q;		// Define transferred 4-momentum
	
	// Loop over file in chain
	while(chain.Next()){
		c12=chain.GetC12Reader();
		auto particles = c12->getDetParticles();	// Get particles vector
		auto electrons = c12->getByID(11)[0];		// Electrons vector
		auto gamma = c12->getByID(22)[0];		// Photons vector

		float t1 = electrons->getTime();	// Time of electrons
		float t2 = gamma->getTime();		// Time of photons
		float deltat = t1-t2;			// Difference
		t->Fill(deltat);			// Fill histogram (Delta time distribution)
		
		// Loop over particles
		for(int i=0; i<particles.size(); i++){
			// Select electrons in FT, with |Delta time| < 2 [ns]
			if((electrons->getRegion()==FT) && abs(deltat) < 2 && (particles[i]->getPid()==11)){
				// Momentum of electrons in FT
				px = particles[i]->par()->getPx();
				py = particles[i]->par()->getPy();
				pz = particles[i]->par()->getPz();
				// Define transferred 4-momentum q
				q[0] = 0.- px;
				q[1] = 0.- py;
				q[2] = E_beam - pz;
				q[3] = E_beam - m_e;
				// q squared
				q2 = q[3]*q[3]-(q[0]*q[0]+q[1]*q[1]+q[2]*q[2]);
				// transverse q squared
				qt2 = q[0]*q[0]+q[1]*q[1];
				
				// Fill histograms
				pxy->Fill(px,py);
				q2_histo->Fill(q2);
				qt2_histo->Fill(qt2);
				}
			}
		}
	// Create root file for histograms
	TFile* out = new TFile("out_simple4.root", "RECREATE");
	// Titles of x and y axes
	pxy->GetXaxis()->SetTitle("p_{x} [GeV]");
	pxy->GetYaxis()->SetTitle("p_{y} [GeV]");
	
	q2_histo->GetXaxis()->SetTitle("q^{2} [GeV^{2}]");
	q2_histo->GetYaxis()->SetTitle("Counts [#]");
	
	qt2_histo->GetXaxis()->SetTitle("q_{ t}^{2} [GeV^{2}]");
	qt2_histo->GetYaxis()->SetTitle("Counts [#]");
	
	t->GetXaxis()->SetTitle("#Deltat [ns]");
	t->GetYaxis()->SetTitle("Counts [#]");
	// Write
	pxy->Write();
	q2_histo->Write();
	qt2_histo->Write();
	t->Write();
	
	out->Close();
	return;
}
