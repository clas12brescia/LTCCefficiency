void simple(){
	
	// create a Hipo chain with multiple files
	
	HipoChain chain;
	chain.Add('./skim2_005339_first1000.hipo');
	chain.Add('./skim2_005343_first1000.hipo');


	auto config_c12=chain.GetC12Reader(); //in case you want to configure, use this
  	auto& c12=chain.C12ref();
  	while(chain.Next()==true){

	    auto particles = c12->getDetParticles();
    	std::cout << particles.size() << std::endl;
	}

}