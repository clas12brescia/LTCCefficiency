#include <fstream>
#include <TApplication.h>

int readfiles_mod(clas12root::HipoChain* chain){
	TString inFile;
	for(int i=0; i<gApplication->Argc(); i++){
		TString opt = gApplication->Argv(i);
		if((opt.Contains(".dat"))||(opt.Contains(".txt"))){
			inFile = opt(5, opt.Sizeof());
		}
	}

	TString nextFile;
	ifstream chainIn(inFile);
	while(chainIn>>nextFile){
		chain->Add(nextFile);
	}
	
	return 1;
}

int readfiles_mod(TChain* chain){
		TString inFile;
		int isHipo;
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
	if(isHipo==0){
	TString nextFile;
	ifstream chainIn(inFile);
	while(chainIn>>nextFile){
		chain->Add(nextFile);
	}
	return 1;
	}
	else if(isHipo==1){
		chain->Add(inFile);
	return 1;
	}
	return 1;
}
