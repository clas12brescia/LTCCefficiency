// Run with:
// clas12root LTCCefficiency.cxx
// If readfiles method provide a text file reader using Argc, Argv:
// clas12root LTCCefficiency.cxx --in=input_filename.dat (or .txt)
// For compilable version, use "+" after ".cxx".
// Histograms are saved in 'out_LTCCefficiency.root' file.

#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <list>
#include <queue>
#include <vector>
#include <map>
#include <algorithm>
#include <iterator>
#include <typeinfo>
#include <string>
#include <numeric>
#include <functional>
#include <sstream>
#include <complex>

#include <TLorentzVector.h>
#include "clas12reader.h"
#include <TH1F.h>
#include <TH2F.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TChain.h>

using namespace clas12;

const double rad_deg = 57.2958;

class Filtered_Particle {
public:
  int Pindex, PID;
  TLorentzVector P;
};

//builds a list of selected variables ans particles in an event. 
class Filtered_Particle_List {
protected:
  clas12reader *c12;
public:
  list<Filtered_Particle> lip;
  Filtered_Particle_List(clas12reader *C12) {c12=C12;}
  int setIPList(){
    lip.clear();
    auto parts=c12->getDetParticles();
    for (int i=0; i<parts.size(); i++) {
      int PID = parts[i]->getPid();
      if (PID==0) continue;
      Filtered_Particle exon;
      exon.Pindex = i;
      exon.PID = PID;      
      TLorentzVector P1;
      P1[0]=parts[i]->par()->getPx();
      P1[1]=parts[i]->par()->getPy();
      P1[2]=parts[i]->par()->getPz();
      if (parts[i]->getP()< 0.1) {P1[3] = 0; continue;}
      if (parts[i]->getBeta()< 0.0001) {P1[3] = 0; continue;}
      P1[3]=parts[i]->getP()/parts[i]->getBeta();
      exon.P = P1;
      lip.push_back(exon);
    }//end loop particles
    return 1;
  }
};

//builds the missing P and the total final P for the event. 
class Event_Global_Momenta {
protected: 
  TLorentzVector p0, pf, pmiss; 
public:
  TLorentzVector getMissingP(){return pmiss;}
  TLorentzVector getPF(){return pf;}
  double getMissingMass(){return pmiss.M();  }
  double getMissingEnergy(){return pmiss[3];  }
  Event_Global_Momenta(double E_beam, double target_mass){
    for (int i=0; i<4; i++){
      p0[i]=0; pf[i]=0; pmiss[i]=0;
    }      
    p0[2] = E_beam; p0[3] = target_mass + E_beam;
  }
  int setEventMomenta(list<Filtered_Particle> lpf) {
    for (int i=0; i<4; i++) {pmiss[i] = 0; pf[i]=0;}
    for (int i=0; i<4; i++) pmiss[i] += p0[i];    
    for (list<Filtered_Particle>::iterator lit=lpf.begin();
	 lit != lpf.end(); lit++) {
      TLorentzVector tlv = lit->P;
      for (int i=0; i<4; i++) {
	pf[i] += tlv[i];
	pmiss[i] -= tlv[i];
      }
    }
    return 1;
  }
};

class Filtered_Loop :
  public Filtered_Particle_List, public Event_Global_Momenta
{
public:
  Filtered_Loop(clas12reader *C12, double E_beam, double target_mass) :
    Filtered_Particle_List(C12), Event_Global_Momenta(E_beam,target_mass){}
  int setLoop(){
    setIPList();
    setEventMomenta(lip);
    return 1;
  }
};

class Filtered_Loop_Cutoffs {
  const double MM_min = 0.88;
  const double MM_max = 1.;  
public:
  int CutsOnMissingP(Filtered_Loop *IPL){
    int n = 0;
    if (IPL->getMissingMass() < MM_min) n++;
    if (IPL->getMissingMass() > MM_max) n++;
    if (IPL->getMissingEnergy() < IPL->getMissingMass() ) n++;
    return n;
  }  
};

class Starting_Cutoffs {
  const int wanted_region = 2000;
  const int in_out_LTCC = -1; //0 = electron everywhere, +1 = in LTCC, -1 = out
  const double pt_min = 0.8; //alternative with the following
  const double min_q_angle = 0.12;
  clas12reader *c12;
public:
  Starting_Cutoffs(clas12reader *C12) {c12=C12;}
  int CutsOnElectron(){
    auto electron=c12->getByID(11)[0];
    int region = electron->getRegion();
    if (region != wanted_region) return 1;
    //in / not in the LTCC
    int sector = electron->getSector(); //1-6
    if (in_out_LTCC>0) //I want e in LTCC only 
      if (!( (sector==3)|| (sector==5) )) return 1;
    if (in_out_LTCC<0) //I do not want e in LTCC
      if ( (sector==3)|| (sector==5) ) return 1;
   
    double px = electron->par()->getPx();
    double py = electron->par()->getPy();
    double pz = electron->par()->getPz();
    double pt = sqrt(px*px+py*py);
    if ( (pz < 0.5) || (pz > 10) ) return 1;
    double qz = 10.2-pz;
    double q_ratio = pt/qz;
    //if (pt < pt_min) return 1;
    if (q_ratio < min_q_angle) return 1;
    return 0;
  }
  int CutsOnTestParticle(int pid){
    auto ppp=c12->getByID(pid)[0];
    int region = ppp->getRegion();
    //in LTCC
    if (region != 2000) return 1;
    int sector = ppp->getSector(); 
    if (!( (sector==3)|| (sector==5) )) return 1;
    return 0;
  }  
};

class Cuts_On_Second_Charged_Track {
  const bool no_neutral = true; 
  const int no_abs_pid1 = 0;
  const int no_abs_pid2 = 11;
  const int no_abs_pid3 = 45;
  const int upper_abs_pid = 2000;
  const int requested_region = 2000;
  const int sectorA = 3;
  const int sectorB = 5;
public:
  int operator()(region_part_ptr Particle){
    bool cond0 = Particle->par()->getCharge() == 0;
    if (no_neutral && cond0) return 0;  
    int abs_pid = abs(Particle->par()->getPid()); 
    bool cond1 = abs_pid == no_abs_pid1; if (cond1) return 0;    
    bool cond2 = abs_pid == no_abs_pid2; if (cond2) return 0;    
    bool cond3 = abs_pid == no_abs_pid3; if (cond3) return 0;    
    bool cond4 = abs_pid > upper_abs_pid; if (cond4) return 0;    
    int region = Particle->getRegion();
    if (region != requested_region) return 0; 
    int sector = Particle->getSector(); 
    if (!( (sector==sectorA) || (sector==sectorB) )) return 0;
    return 1;
  }
};

//compacts some histograms always required in our analysis.
class Histogram_Vector {
public:
  vector<TH1F*> VT;
	vector<TH2F*> V2;
  Histogram_Vector(){
    TH1F *h0 = new TH1F("histo0","missing mass; mass (GeV/c^{2})",400,0,8);
    TH1F *h1 = new TH1F("histo1","candidates in LTCC; P(GeV/c)",20,0,10);
    TH1F *h2 = new TH1F("histo2","candidates with hits in LTCC; P(GeV/c)",
			20,0,10);  
		TH1F *h3 = new TH1F("histo3","candidates in LTCC; #theta(deg)",20,0, 1*rad_deg);
    TH1F *h4 = new TH1F("histo4","candidates with hits in LTCC; #theta(deg)",
			20,0, 1*rad_deg);
		TH1F *h5 = new TH1F("histo5","candidates in LTCC; #phi(deg)",20, -3.15*rad_deg, 3.15*rad_deg);
    TH1F *h6 = new TH1F("histo6","candidates with hits in LTCC; #phi(deg)",
			20,-3.15*rad_deg, 3.15*rad_deg);
		TH1F *h3bis = new TH1F("histo3bis","candidates in LTCC; cos(#theta)(#)",20, 0, 0.5);
		TH1F *h4bis = new TH1F("histo4bis","candidates with hits in LTCC; cos(#theta)(#)",20, 0, 0.5);
		
		TH2F *h7 = new TH2F("histo7","'x' vs 'y' of candidates in LTCC", 20, -0.8,0.5,20,-0.7,0.7);
		TH2F *h8 = new TH2F("histo8","'x' vs 'y' of candidates with hits in LTCC",20,-0.8,0.5,20,-0.7,0.7);
		TH2F *h9 = new TH2F("histo9","#phi vs #theta (in LTCC); #theta(deg); #phi(deg)",20,0,1*rad_deg,20,-3.5*rad_deg, 3.5*rad_deg);
		TH2F *h10 = new TH2F("histo10","#phi vs #theta (hits in LTCC); #theta(deg); #phi(deg)",20,0,1*rad_deg,20,-3.5*rad_deg, 3.5*rad_deg);
		TH2F *h11 = new TH2F("histo11","#phi vs cos(#theta) (in LTCC); cos(#theta)(#); #phi(deg)",20,0,0.5,20,-3.5*rad_deg,3.5*rad_deg);
		TH2F *h12 = new TH2F("histo12","#phi vs cos(#theta) (hits in LTCC); cos(#theta)(#); #phi(deg)",20,0,0.5,20,-3.5*rad_deg,3.5*rad_deg);
		TH2F *h13 = new TH2F("histo13","P vs #theta (in LTCC); #theta(deg); P(GeV/c)",10,0,1*rad_deg,10,0,10);
		TH2F *h14 = new TH2F("histo14","P vs #theta (hits in LTCC); #theta(deg); P(GeV/c)",10,0,1*rad_deg,10,0,10);
		TH2F *h15 = new TH2F("histo15","P vs cos(#theta) (in LTCC); cos(#theta)(#); P(GeV/c)",20,0,0.5,20,0,10);
		TH2F *h16 = new TH2F("histo16","P vs cos(#theta) (hits in LTCC); cos(#theta)(#); P(GeV/c)",20,0,0.5,20,0,10);
		
    VT.push_back(h0);
    VT.push_back(h1);
    VT.push_back(h2);
		VT.push_back(h3);
		VT.push_back(h4);
		VT.push_back(h5);
		VT.push_back(h6);
		VT.push_back(h3bis);
		VT.push_back(h4bis);
		
		V2.push_back(h7);
		V2.push_back(h8);
		V2.push_back(h9);
		V2.push_back(h10);
		V2.push_back(h11);
		V2.push_back(h12);
		V2.push_back(h13);
		V2.push_back(h14);
		V2.push_back(h15);
		V2.push_back(h16);
  }
  void Fill(int i, double a){VT[i]->Fill(a);}
	void Fill(int j, double a, double b){V2[j]->Fill(a,b);}
  void Draw1(int i, TFile* out){
      TCanvas *can = new TCanvas;
      VT[i]->Draw();
			VT[i]->Write();
  }
	void Draw2(int j, TFile* out){
		TCanvas *can = new TCanvas;
		V2[j]->Draw();
		V2[j]->Write();
	}
  void Draw(TFile* out){
    for (int i=0; i< VT.size(); i++) Draw1(i, out);
		for (int j=0; j< V2.size(); j++) Draw2(j, out);
  }
  void SetRatio(int i, int j){
    TH1F *hratio = (TH1F*) VT[i]->Clone("hratio");
    hratio->SetTitle("ratio of LTCC-seen to all in sectors 3 and 5; P(GeV/c)");
    hratio->SetStats(0); //statistic panel is useless
    hratio->Divide(VT[j]);
    VT.push_back(hratio);

		TH1F *htheta = (TH1F*) VT[i+2]->Clone("htheta");
    htheta->SetTitle("ratio of LTCC-seen to all in sectors 3 and 5; #theta(rad)");
    htheta->SetStats(0); //statistic panel is useless
    htheta->Divide(VT[j+2]);
    VT.push_back(htheta);

		TH1F *hphi = (TH1F*) VT[i+4]->Clone("hphi");
    hphi->SetTitle("ratio of LTCC-seen to all in sectors 3 and 5; #phi(rad)");
    hphi->SetStats(0); //statistic panel is useless
    hphi->Divide(VT[j+4]);
    VT.push_back(hphi);

		TH1F *hcos = (TH1F*) VT[i+6]->Clone("hcos");
    hcos->SetTitle("ratio of LTCC-seen to all in sectors 3 and 5; #phi(rad)");
    hcos->SetStats(0); //statistic panel is useless
    hcos->Divide(VT[j+6]);
    VT.push_back(hcos);

		TH2F *hxy = (TH2F*) V2[1]->Clone("hxy");
		hxy->SetTitle("ratio of LTCC-seen to all in sectors 3 and 5; 'x' (#) vs 'y' (#)");
		hxy->SetStats(0);
		hxy->Divide(V2[0]);
		V2.push_back(hxy);

		TH2F *hangles = (TH2F*) V2[3]->Clone("hangles");
		hangles->SetTitle("ratio of LTCC-seen to all in sectors 3 and 5; #theta(deg); #phi(deg)");
		hangles->SetStats(0);
		hangles->Divide(V2[2]);
		V2.push_back(hangles);

		TH2F *hphicos = (TH2F*) V2[5]->Clone("hphicos");
		hphicos->SetTitle("ratio of LTCC-seen to all in sectors 3 and 5; cos(#theta)(#); #phi(deg)");
		hphicos->SetStats(0);
		hphicos->Divide(V2[4]);
		V2.push_back(hphicos);

		TH2F *hptheta = (TH2F*) V2[7]->Clone("hptheta");
		hptheta->SetTitle("ratio of LTCC-seen to all in sectors 3 and 5; #theta(deg); P(GeV/c)");
		hptheta->SetStats(0);
		hptheta->Divide(V2[6]);
		V2.push_back(hptheta);

		TH2F *hpcos = (TH2F*) V2[9]->Clone("hpcos");
		hpcos->SetTitle("ratio of LTCC-seen to all in sectors 3 and 5; cos(#theta); P(GeV/c)");
		hpcos->SetStats(0);
		hpcos->Divide(V2[8]);
		V2.push_back(hpcos);
  }
};

//////////////////////////////////////////////////////////
// For more detailed descriptions of the classes above	//
// see 'classes_description.md'													//
//////////////////////////////////////////////////////////


// methods to read hipo files
#include "readfiles.cxx"
int readfiles(TChain *chain);

//uses only 1 identified pid (electron) + 1 charged track only
int LTCCefficiency(){

	// check starting time
  system("date");

	// Enable/disable particles selection using PID
	// Pre-defined values: false, PID = 211
	bool option_identified_pid=false;
  int identified_pid=211; 

//  bool short_run = 1; //1 = few hipo files, 0 = all list (NOT implemented in the present readfiles method)
  double E_beam = 10.1998;
  double target_mass = 0.93872;

	// create a vector of useful histograms
  Histogram_Vector HV;

	// histograms for charged particle
  TH1F *h9 = new TH1F("histo9","charge of second track",5,-2.5,2.5);
  TH1F *h9b = new TH1F("histo9b","charge of second track after mm cut",5,-2.5,2.5);

  // pre-loading of data files
  TChain chain;
  readfiles(&chain);
  
	// index of all processed events
  int ievent = 0;
  
  //loop over hipo files
  for(int ifile=0; ifile<chain.GetListOfFiles()->GetEntries(); ++ifile){

    clas12reader c12(chain.GetListOfFiles()->At(ifile)->GetTitle(), {0});

		// cuts on always present and identified particles 
		// (e.g. leading electron)
    Starting_Cutoffs cuts(&c12);
		
		// reduced list of useful properties of particles
		// Pindex, Px, Py, Pz, E
    Filtered_Loop loop(&c12, E_beam, target_mass);

		// cuts on the overall event 4-momentum
		// (e.g. missing mass)
    Filtered_Loop_Cutoffs loop_cuts;
    
    c12.addAtLeastPid(11,1);    //at least 1 electron
    
    // loop over events
    while(c12.next()==true){

      if(c12.getDetParticles().empty()) continue; // remove empty events

			// application of cuts on the electron
      if ( cuts.CutsOnElectron() ) continue;

			// vector of all the particles of the event
      // (auto -> vector<region_part_ptr>)
      auto parts=c12.getDetParticles();
      
			// count of the charged particles that passed cuts and filters
      int ipp = count_if(parts.begin(),parts.end(),[] (region_part_ptr Particle)
		     {return abs(Particle->par()->getCharge()); });
			// discard events with more than 2 charged particles
      if (ipp > 2) continue;

      //at this point, only events with one or two charged tracks 
			// are left, and one must be the electron
      
			// object ready to check selection criterions on the second 
			// charged track
      Cuts_On_Second_Charged_Track SelectCandidate;

			// find among all the event particles the first one that satisfies 
			// the previous selection criterions. 
			// If no candidates satisfy these, the event is discarded.
      auto candidate = find_if(parts.begin(), parts.end(), SelectCandidate);
      if (candidate == parts.end()) continue;
      
      // at this point, only events with an electron and a second track
			// satisfying all the previous cuts and filters have survived.

      //emergency cutoff for few-event analysis
      ievent++;
      //if (ievent > 60) break;  

			////////////////////////////////////////////////////////////////
			// if selection by PID is enabled, select only particles with	//
			// the PID assigned before																		//
			// if disabled, the selection without PID is executed					//
			if (option_identified_pid)																		//
    	if ((*candidate)->getPid() != identified_pid) continue;				//
			////////////////////////////////////////////////////////////////

			// initialization of the reduced list of particle properties
			// determination of missing momentum and total final momentum
      loop.setLoop();
      
			// calculate missing mass and fill the histogram
      double missing_mass = loop.getMissingMass();      
      HV.Fill(0,missing_mass);

			// histogram of charge of candidates
      h9->Fill( (*candidate)->par()->getCharge()  ); 

      // cuts on missing P variables:
			// missing mass, missing energy > missing mass
			if (loop_cuts.CutsOnMissingP(&loop)) continue;

			// histogram of charge of candidates after cuts
      h9b->Fill( (*candidate)->par()->getCharge()  );

			// survived candidates momentum magnitude
      double candidate_P = (*candidate)->getP();
			double candidate_theta = ((*candidate)->getTheta());
			double candidate_phi = ((*candidate)->getPhi());
			double x_false = sin(candidate_theta)*cos(candidate_phi);
			double y_false = sin(candidate_theta)*sin(candidate_phi);
			double costheta = cos(candidate_theta);

      // before photoelectron check
      HV.Fill(1,candidate_P);
			HV.Fill(3,candidate_theta*rad_deg);
			HV.Fill(5,candidate_phi*rad_deg);
			HV.Fill(7,costheta);
			HV.Fill(0,x_false, y_false);
			HV.Fill(2,candidate_theta*rad_deg,candidate_phi*rad_deg);
			HV.Fill(4,costheta,candidate_phi*rad_deg);
			HV.Fill(6,candidate_P,candidate_theta*rad_deg);
			HV.Fill(8,candidate_P,costheta);

      // number of photoelectrons produced by the candidates in LTCC.
			// Care: not an integer.
      double candidate_Nphe = (*candidate)->che(LTCC)->getNphe();
			
      // require at least 1 photoelectron and fill histogram
      if (candidate_Nphe > 0.99){
				HV.Fill(2,candidate_P);
				HV.Fill(4,candidate_theta*rad_deg);
				HV.Fill(6,candidate_phi*rad_deg);
				HV.Fill(8,costheta);
				HV.Fill(1,x_false, y_false);
				HV.Fill(3,candidate_theta*rad_deg,candidate_phi*rad_deg);
				HV.Fill(5,costheta,candidate_phi*rad_deg);
				HV.Fill(7,candidate_P,candidate_theta*rad_deg);
				HV.Fill(9,candidate_P,costheta);
			}
    }//loop over events
  }// loop over files

	// root output file
	TFile* out = new TFile("out_LTCCefficiency.root","RECREATE");

	// draw and save charge distribution histograms
  TCanvas *can9 = new TCanvas;
  h9->Draw();
	h9->Write();
  TCanvas *can9b = new TCanvas;
  h9b->Draw();
	h9b->Write();

  // set and create the ratio of histograms 1 and 2 (efficiency)
	// candidates in LTCC with photoelectrons / all candidates in LTCC
  HV.SetRatio(2,1);

	// draw and save the four histograms:
	// missing mass, candidates, candidates with photoelectrons, ratio
  HV.Draw(out);

	// close the root file
	out->Close();
  
  system("date");
  return 0;
}
