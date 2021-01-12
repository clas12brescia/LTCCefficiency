#include <fstream>

int readfiles1(HipoChain* chain){
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
