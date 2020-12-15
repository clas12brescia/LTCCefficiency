void simple(){
	
	// create a Hipo chain with multiple files
	
	HipoChain chain;
	chain.Add('./skim2_005339_first1000.hipo');
	chain.Add('./skim2_005343_first1000.hipo');

	// Loop on files

	for(int ifile=0;ifile<chain.GetNFiles();++ifile){

		clas12reader c12{chain.GetFileName(ifile).Data()};

		auto parts=c12.getDetParticles();

		std::cout << parts.size() << std::endl;
	}

}