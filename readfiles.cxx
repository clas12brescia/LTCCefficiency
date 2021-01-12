#include <fstream>
#include <TApplication.h>

/*
The input file paths depend on where the code is used (at my home, on 
some ifarm, on fall2018 or spring2019 files, etc). So we have chosen to 
let the hipo file input be handled by a function that has been put 
on this file. 
TChain chain must be defined in LTCCefficiency(), but a pointer to chain 
may be given to readfiles(). Care: a pointer not a copy must be used.
*/

int readfiles(TChain* chain){

	TString inFile;
	// read the input string and use it if is a .dat or .txt file
	for(int i=0; i<gApplication->Argc(); i++){
		TString opt = gApplication->Argv(i);
		if((opt.Contains(".dat"))||(opt.Contains(".txt"))){
			inFile = opt(5, opt.Sizeof());
		}
	}

	TString nextFile;
	ifstream chainIn(inFile);
	// read the list of files contained the input file
	while(chainIn>>nextFile){
		chain->Add(nextFile);		// add to the chain
	}
	
	return 1;
}
