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

#include <TChain.h>
#include <TApplication.h>
#include "clas12reader.h"

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

#include "readfiles.cxx"

using namespace clas12;

void readfiles(TChain* chain);

void customHipo(){

	// Enable/disable particles selection using PID
	// Pre-defined values: false, PID = 211
	bool option_identified_pid=false;
  int identified_pid=211; 

	//  bool short_run = 1; //1 = few hipo files, 0 = all list (NOT implemented in the present readfiles method)
  double E_beam = 10.1998;
  double target_mass = 0.93872;

	TChain chain;
	readfiles(&chain);	//read from list of hipo files

	// index of all processed events
  int ievent = 0;

	string input;
	for(int i=0;i<gApplication->Argc();++i){
		TString opt = gApplication->Argv(i);
		if(opt.Contains(".dat")||opt.Contains(".txt")){
			input = opt(5, opt.Sizeof());
		}
	}
  input.erase(input.begin(),input.begin()+6);
  input.erase(input.end()-4,input.end());
	clas12writer c12writer(Form("custom_%s.hipo",input.c_str()));	//clas12writer: write on custom_%s.hipo file

	//loop over files
	for(int ifile=0;ifile<chain.GetListOfFiles()->GetEntries();++ifile){
		
		//define clas12reader from TChain
		clas12reader c12p(chain.GetListOfFiles()->At(ifile)->GetTitle(),{0});
		//assign clas12reader to clas12writer
		c12writer.assignReader(c12p);

		// cuts on always present and identified particles 
		// (e.g. leading electron)
    Starting_Cutoffs cuts(&c12p);
		
		// reduced list of useful properties of particles
		// Pindex, Px, Py, Pz, E
    Filtered_Loop loop(&c12p, E_beam, target_mass);

		// cuts on the overall event 4-momentum
		// (e.g. missing mass)
    Filtered_Loop_Cutoffs loop_cuts;
		
		c12p.addAtLeastPid(11,1); //at least 1 electron
		
		//loop over events
		while(c12p.next()==true){
			
			//delete empty events
			if(c12p.getDetParticles().empty()) continue;
			
			if(c12p.getDetParticles().empty()) continue; // remove empty events

			// application of cuts on the electron
      if ( cuts.CutsOnElectron() ) continue;

			// vector of all the particles of the event
      // (auto -> vector<region_part_ptr>)
      auto parts=c12p.getDetParticles();
      
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

      // cuts on missing P variables:
			// missing mass, missing energy > missing mass
//			if (loop_cuts.CutsOnMissingP(&loop)) continue;

			//write events
			c12writer.writeEvent();

		}//end loop over events
	}//end loop over files

	//close clas12writer
	c12writer.closeWriter();

	cout<<"No. Events: "<<ievent<<endl;
}
