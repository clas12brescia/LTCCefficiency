void simple(){
	
  // create a clas12reader
  clas12reader c12("./skim2_005339_first1000.hipo"); 

  while(c12.next()) { //loop over events

    auto particles = c12.getDetParticles(); //particles is now a std::vector of particles for this event

    //If you just want to loop over all particles, best to use a c++11 range based for loop
    std::cout << particles.size() << std::endl;

  }
}
