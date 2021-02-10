//particleTree /dir/file.hipo out.root test_treemaker.cxx => 1 hipo file
//particleTree '/dir/*.hipo' out.root test_treemaker.cxx => all hipo file in directory /dir

{
  //treemaker.SetEntries(1E5);//only process given number of events
  //add event header branch, includes start time
	//treemaker.UseEventData();

	//Using string to define the contente of a branch separately
	string Var0 = "EVNT.StartTime/F";
  
  treemaker.Branch("P.Time/F"); //"P." = p->getVar()
  treemaker.Branch("P.Path/F");
  treemaker.Branch("P.DetEnergy/P.P/F","SampFrac");
  //Create branch names as bank items
  treemaker.Branch("PBANK.Px/F"); //"PBANK." = p->par()->getVar()
  treemaker.Branch("PBANK.Py/F");
  treemaker.Branch("PBANK.Pz/F");
	treemaker.Branch("P.P/F","P");
//	treemaker.Branch("","MissingM");
	treemaker.Branch(Var0.c_str(),"StartTime");	//string definition of the branch
  treemaker.Branch("LTCC.Nphe/F","nphe");
  treemaker.Branch("PBANK.Pid/I","PID");
	treemaker.Branch("PBANK.Chi2Pid/F","chi2pid");
  //treemaker.Branch("PBANK.FTBPid/I");//FT based PID

  //Some kinetic and other cuts
  treemaker.AddParticleCut("P.P<10 && LTCC.Nphe>2");								 //P<10 GeV/c and Nphe from LTCC > 2
	treemaker.AddParticleCut("&& PBANK.Chi2Pid<3 && PBANK.Chi2Pid>0"); //Chi2PID < 3 and non-negative (some values are negative)
	//if cuts are separated, remember "&&" at the beginning of the second (or successive) one

  //Event topology cuts
  treemaker.AddAtLeastPid(211,1); //at least 1 pi+
  treemaker.AddExactPid(11,1);    //exactly 1 electron
//  treemaker.AddZeroOfRestPid();  //nothing else, if not this line any of anything else
  treemaker.Fill();
}
