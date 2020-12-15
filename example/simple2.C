/*	
	Based on the cheatsheet by Derek Glazier in the clas12root repo
	https://github.com/JeffersonLab/clas12root/blob/master/AccesssingBankDataInCpp.txt

	run with
	clas12root -b -q simple2.C
*/

void simple2(){
	
  // create 1 histogram
  TH1F * multip = new TH1F("multip", "Number of particles per event",100, 0, 100);

  // create a clas12reader
  clas12reader c12("./skim2_005339_first1000.hipo"); 

  //loop over the reader (events)

  while(c12.next()) { 

    auto particles = c12.getDetParticles(); //particles is now a std::vector of particles for this event

    // fill the histo
    multip->Fill(particles.size());
    
  }


  // save the histo to an output file
  TFile *outf = new TFile("out_histos.root", "RECREATE");
  multip->Write();
  outf->Close();
  return;
}
