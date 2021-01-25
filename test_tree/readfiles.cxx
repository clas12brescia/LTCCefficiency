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

void readfiles(TChain* chain){
		TString inFile;
		int isHipo;		//int to identify an hipo file
		//read the input string and use it if is a .dat, .txt or .hipo file
		for(int i=0; i<gApplication->Argc(); i++){
		TString opt = gApplication->Argv(i);
		if((opt.Contains(".dat"))||(opt.Contains(".txt"))){
			inFile = opt(5, opt.Sizeof());
			isHipo=0;
		}
		else if((opt.Contains(".hipo"))){
		inFile = opt(5, opt.Sizeof());
		isHipo=1;
		}
	}

	//if not .hipo file, add each file contained in .txt/.dat list in
	//the TChain.
	if(isHipo==0){
	TString nextFile;
	ifstream chainIn(inFile);
	while(chainIn>>nextFile){
		chain->Add(nextFile);
		}
	}
	//if .hipo file, add directly this file to the chain
	else if(isHipo==1){
		chain->Add(inFile);
		}
}
