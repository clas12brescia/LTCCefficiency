// Run with:
// clas12root test7_pion.cxx
// If readfiles1 method provide a text file reader using Argc, Argv:
// clas12root test7_pion.cxx --in=input_filename.dat (or .txt)

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

using namespace std;

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
  Histogram_Vector(){
    TH1F *h0 = new TH1F("histo0","missing mass; mass (GeV/c^{2})",400,0,8);
    TH1F *h1 = new TH1F("histo1","candidates in LTCC; P(GeV/c)",20,0,10);
    TH1F *h2 = new TH1F("histo2","candidates with hits in LTCC; P(GeV/c)",
			20,0,10);  
    VT.push_back(h0);
    VT.push_back(h1);
    VT.push_back(h2);
  }
  int Fill(int i, double a){VT[i]->Fill(a); return 1;}
  int Draw1(int i, TFile* out){
      TCanvas *can = new TCanvas;
      VT[i]->Draw();
			VT[i]->Write();
      return 1;
  }  
  int Draw(TFile* out){
    for (int i=0; i< VT.size(); i++) Draw1(i, out);
    return 1;
  }
  int SetRatio(int i, int j){
    TH1F *hratio = (TH1F*) VT[i]->Clone("hratio");
    hratio->SetTitle("ratio of LTCC-seen to all in sectors 3 and 5; P(GeV/c)");
    hratio->SetStats(0); //statistic panel is useless
    hratio->Divide(VT[j]);
    VT.push_back(hratio);
    return 1;
  }
};

// methods to read hipo files
#include "readfiles1.cxx"
int readfiles1(HipoChain *chain);

//uses 2 identified tracks, electron and pion (using PID)
int test7_pion(){

  system("date");

  bool short_run = 1;
  double E_beam = 10.1998;
  double target_mass = 0.93872;

  int second_pid = 211;

  Histogram_Vector HV;
  
  //pre-load of hipo files
  HipoChain chain;
  readfiles1(&chain);
  
  int ievent = 0;
  
  //loop over files
  for(int ifile=0; ifile<chain.GetNFiles(); ++ifile){

    clas12reader c12{chain.GetFileName(ifile).Data()};
    Starting_Cutoffs cuts(&c12);
    Filtered_Loop loop(&c12, E_beam, target_mass);
    Filtered_Loop_Cutoffs loop_cuts;
    
    c12.addExactPid(11,1);    //1 electron
    c12.addExactPid(second_pid,1);    //1 positive pion
    
    //ciclo sugli eventi 
    while(c12.next()==true){

      if(c12.getDetParticles().empty()) continue; //remove empty events

      if ( cuts.CutsOnElectron() ) continue;
      if ( cuts.CutsOnTestParticle(second_pid) ) continue;

      //emergency cutoff for few-event analysis
      ievent++;
      //if (ievent > 60) break;  

      loop.setLoop();
      
      double missing_mass = loop.getMissingMass();      
      HV.Fill(0,missing_mass);

      if (loop_cuts.CutsOnMissingP(&loop)) continue; //cut on missing P mass etc 
      auto ppp=c12.getByID(second_pid)[0];

      double candidate_P = ppp->getP();

      //before photoelectron check
      HV.Fill(1,candidate_P); 

      //number of photoelectrons in LTCC. Care: not an integer. 
      double candidate_Nphe = ppp->che(LTCC)->getNphe();

      //after photoelecton check
      if (candidate_Nphe > 0.99)
	HV.Fill(2,candidate_P);      

    }//loop over events
  }// loop over files
	TFile* out = new TFile("out_pion.root", "RECREATE");

  //ratio of histograms
  HV.SetRatio(2,1);
  HV.Draw(out);
  
  system("date");
  return 0;
}

