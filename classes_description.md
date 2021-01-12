# Classes description

Useful descriptions and comments about the classes used in the macro `LTCCefficiency.cxx`.

## Filtered_Particle

This class is only a structure containing those particle variables that we think we need in a reanalysis of all the particles of an event.
Adding new variables does not changer the following. Removing of course creates compilation problems, since they may be used in later functions.

The variables selected here are:
* Pindex
* PID
* 4-momentum

```c++
class Filtered_Particle {
public: 
	int Pindex, PID;
	TLorentzVector P;
};
```
## Filtered_Particle_List

This creates a list of "Filtered_Particle" objects associated to one evenet.
Care: Since the class contains a pointer to the general clas12reader that is given at construction stage, the functions of this class do not need to receive information on each event.
The function `SetPList()` automatically clears the old list and fills a new one.
Care: This function contains some filtering criteria, that is it discard some particles. 
These can be modified to satisfy further requirements.
Care: The energy of each particle is calculated as P/beta using the clas12root functions `parts[i]->getBeta()` and `parts[i]->getP()`. 
PIDs are not used. Of course users could prefer different methods.

```c++
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
```
## Event_Global_Momenta

This works on a `Filtered_Particle_List` object (previous class) and 
calculates the components of some relevant kinematic variables that 
do not belong to a single particle but to the whole event. At present, 
the overall 4-momentum PF of all the final particles, and the missing 
4-momentum MissingP = P_beam + P_target - P_final. 
These can be retrieved via the functions `getPF()` and `getMissingP()` that 
return a TLorentzVector.  
Also `getMissingMass()` and `getMissingEnergy()` that directly return a double 
are present. 
At construction it only requires the target mass and the beam energy.
At each event the function `setEventMomenta(list<Filtered_Particle> lpf)` 
receives as argument an object of those built by the previous class,
and extracts from it the information to build the event variables. 
Care: check how the previous class reconstructs the energy of each 
particle.

```c++
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
```

## Filtered_Loop

This is a "fragment of LTCCefficiency()" with two instructions that always appear 
together:
 
1. Calculating the total final momentum + the missing momentum 
of an event (`setEventMomenta()`), using
1. a list that must have been already built by `setPList()`.
 
Because of this "fragment" organized as a class, LTCCefficiency() never 
directly sees the above defined classes. 
In LTCCefficiency() the construction instruction is of the kind: 
`Filtered_Loop loop(&c12, E_beam, target_mass);`
In the later event analysis in LTCCefficiency(), the instuction 
`loop.setLoop();` 
builds a filtered particle list and calculates final momentum and missing momentum over this list. 
These can be retrieved via the functions `get...` of the above class `Event_Global_Momenta` that are inherited by `Filtered_Loop`.

```c++
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
```

## Cutoff classes

The following classes handle the cutoffs to be used in this analysis and their parameters as constants.

### Filtered_Loop_Cutoffs 

Cutoffs on the set of overall event momenta (like e.g. missing mass).
Presently only neutron missing mass and missing energy not below mass.

The applied cutoffs are:
* Missing mass between 0.88 and 1 GeV;
* Missing mass < Missing energy.

```c++
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
```
### Starting_Cutoffs

Cutffs that in clas12root can be imposed BEFORE beginning a loop 
on the event particles, exploiting requirements like   
`c12.addAtLeastPid(11,1)` that have been left in LTCCefficiency(). The 
standard sequence in LTCCefficiency() is:
```c++ 
clas12reader c12(chain.GetListOfFiles()->At(ifile)->GetTitle(), {0}); //defines c12
...
c12.addAtLeastPid(11,1); //guarantees that an electron is present 
...
auto electron=c12->getByID(11)[0];
```

The last instruction is the starting point of the function `CutsOnElectron` of this class. 
The function `CutsOnTestParticle` is similar, but assumes that an 
additional identified particle (e.g. PID=211) has been guaranteed by an 
instruction `c12.addAtLeastPid(...)` in LTCCefficiency(). This is the case in 
some of our codes but not all. 
Care: by default (LTCC) this class requires the electron to be in FD but 
out of LTCC, while the possible additional test particle must be in. Modify 
this if you need them somewhere else. 
Care: the electron cuts presently require the transferred momentum not 
to be too beam-aligned. 

```c++
class Starting_Cutoffs {
  const int wanted_region = 2000;
  const int in_out_LTCC = -1; //0 = electron everywhere, +1 = in LTCC, -1 = out
  const double pt_min = 0.8; //alternative with the following
  const double min_q_angle = 0.12; //presently used
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
```

### Cuts_On_Second_Charged_Track

This is for the case when the test particle (e.g. a &pi;<sup>+</sup>) is not chosen 
on the basis of the PID identification by clas12root. This class is a 
function that receives a clas12root object "particle" (more precisely 
the type is `region_part_ptr`) and returns 0 if one of several 
conditions on this particle is not true. Of course, these conditions 
are frequently modified.  
Remark: it does not handle the full particle vector, but ony one particle 
whose pointer is in argument.

```c++
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
```

## Histogram_Vector

Since 4 specific histograms always appear in our codes on LTCC efficiency,
we have regrouped all the operations concerning them in the functions of this class. 
It creates 3 of them, and the last one that is the ratio of two of the previous ones. 
The function Fill depends on an index to fill the required histogram.  
`Draw()` collects all the printing isntructions in one. When use additional histograms apart for these 4, their creation etc 
appears explicitely in LTCCefficiency().

```c++
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
  int Draw1(int i){
      TCanvas *can = new TCanvas;
      VT[i]->Draw();
      return 1;
  }  
  int Draw(){
    for (int i=0; i< VT.size(); i++) Draw1(i);
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
```

