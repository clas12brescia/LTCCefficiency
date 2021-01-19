#include <TH1F.h>

using namespace std;

int view(){  
	TH1F* h=new TH1F("h","h",100,1,-1); 
	
	clas12reader c12("simple.hipo"); 
	
	c12.addAtLeastPid(11,1);  
	
	while(c12.next()==true){  
		auto electron=c12.getByID(11);
		h->Fill(electron[0]->getP());
	}

	h->Draw();
	return 1;
}
