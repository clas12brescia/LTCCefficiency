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

#include "readfiles.cxx"

using namespace clas12;

void readfiles(TChain* chain);

void simple_write2(){

	TChain chain;
	readfiles(&chain);	//read from list of hipo files

	clas12writer c12writer("simple2.hipo");	//clas12writer: write on simple2.hipo file

	//loop over files
	for(int ifile=0;ifile<chain.GetListOfFiles()->GetEntries();++ifile){
		
		//define clas12reader from TChain
		clas12reader c12p(chain.GetListOfFiles()->At(ifile)->GetTitle(),{0});
		c12p.addAtLeastPid(11,1); //at least 1 electron
		
		//assign clas12reader to clas12writer
		c12writer.assignReader(c12p);
		
		//loop over events
		while(c12p.next()==true){
			
			//delete empty events
			if(c12p.getDetParticles().empty()) continue;
			
			auto electron=c12p.getByID(11)[0];
			double Pe=electron->getP();

			//cuts
			if(Pe>9.5 && Pe<9.505){
				//write events
				c12writer.writeEvent();
				cout<<"+"<<endl;
			}//end if
		}//end loop over events
	}//end loop over files

	//close clas12writer
	c12writer.closeWriter();
}
