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
#include "readfiles.cxx"

using namespace std;

int readfiles_mod(HipoChain* chain);

int simple_write(){

	HipoChain chain;
	readfiles(&chain);

	clas12writer c12writer("simple.hipo");
	clas12reader* c12p=chain.GetC12Reader();

	while(chain.Next()==true){
		if(c12p->getDetParticles().empty()) continue;
		
		if(c12p!=chain.GetC12Reader()){
			c12p=chain.GetC12Reader();
			c12writer.assignReader(*c12p);
		}
		
		c12p->addAtLeastPid(11,1);
		auto electron=c12p->getByID(11);
		double Pe=electron[0]->getP();

		if(Pe>9.5 && Pe<9.505){
			c12writer.writeEvent();
			cout<<"+"<<endl;
		}

	}
	c12writer.closeWriter();
	return 1;
}
