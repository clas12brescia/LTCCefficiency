/*	
	Based on the cheatsheet by Derek Glazier in the clas12root repo
	https://github.com/JeffersonLab/clas12root/blob/master/AccesssingBankDataInCpp.txt

	run with
	clas12root -b -q simple.C
*/

void simple(){
	


  // create a clas12reader
  clas12reader c12("./skim2_005339_first1000.hipo"); 

  //loop over the reader (events)

  while(c12.next()) { 

    auto particles = c12.getDetParticles(); //particles is now a std::vector of particles for this event

    // print the vector size, that is the number of particles per event

    std::cout << particles.size() << std::endl;

  }
}
