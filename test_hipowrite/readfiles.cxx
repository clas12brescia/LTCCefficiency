#include <fstream>
#include <TApplication.h>

void readfiles(TChain* chain){
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
	}
	else if(isHipo==1){
		chain->Add(inFile);
		}
}
