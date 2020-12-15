//variante per le figure sull'angolo di tilt


int main1(){
  
  TH1F *h0x = new TH1F("histo0","time(ns)",100,-5,5);

  //pre-loading of data files

  HipoChain chain;
  chain.Add("/home/andrea/clasdoc/trains/skim2_5039.hipo");
  chain.Add("/home/andrea/clasdoc/trains/skim2_5040.hipo");
  chain.Add("/home/andrea/clasdoc/trains/skim2_5190.hipo"); 
  //if at jlab
  //chain.Add("/work/clas12/rg-a/trains/v16_v2/skim2_ft_pi0/skim2_5124.hipo");  
  //chain.Add("/work/clas12/rg-a/trains/v16_v2/skim2_ft_pi0/skim2_5125.hipo");
  
  //loop sui files
  for(int ifile=0;ifile<chain.GetNFiles();++ifile){
    clas12reader c12{chain.GetFileName(ifile).Data()};

    //filtro sugli eventi (esperienza: stare attenti, aveva dei bug)
    //indispensabile solo se si usa un'altra istruzione in seguito.
    c12.addExactPid(11,1);    //exactly 1 electron
    //c12.addExactPid(22,2);    //exactly 2 gamma
    //c12.addZeroOfRestPid(); // non funzionava qualche mese fa. 
    
    //loop su tutti gli eventi del file.
    //attenzione: ogni evento contiene un numero imprecisato di record    
    while(c12.next()==true){

      //l'evidenza è che son presenti eventi vuoti. 
      if(c12.getDetParticles().empty()) 
	continue;

      //questo non serve più, ma era indispensabile perchè root non azzera
      //i vettori in occasione di eventi vuoti. Lo lascio come remind.
      //for (int index=0; index<3; index++){
      //px[index]=0;
      //py[index]=0;
      //pz[index]=0;
      //}

      double time_e = 0;
      
      //questa funziona solo associata alla precedente AddExactPid
      //(lui deve sapere quante particelle in un evento hanno lo stesso PID)
      //Non è indeispensabile, ma può far comodo per una "preanalisi" di
      //alcune particelle sempre presenti. L'analisi vera e propria è
      //più avanti. 

      auto electron=c12.getByID(11)[0];

      time_e = electron->getTime();
      //notate che molte variabili passano attraverso "par"
      //px = electron->par()->getPx(); //non mi serve ora, ma è utile 

      //la seguente è la lista completa dei puntatori alle particelle
      //dell'evento (ad ogni particella è associato un set di variabili)
      auto parts=c12.getDetParticles(); 

      //Taglio: butto via un evento con oltre 5 particelle.
      //Indirettamente la funzione "size" mi dice che lui sta usando un
      //contenitore STDL (lista, vector, boh), presumibilmente anche
      //altre funzioni STDL possono essere utilizzate. 
      if (parts.size()>5){
	continue;
      }

      //loop su tutte le particelle della lista parts.
      //Seleziono solo i fotoni (pid 22) nel FT
      
      for (int i=0; i<parts.size(); i++) {
	if ((parts[i]->getPid()==22) && (parts[i]->getRegion()==FT)) { 
	  double time_gamma = parts[i]->getTime();
	  double delta_t =  time_gamma-time_e;
	  h0x->Fill(delta_t);
	}
      }//end for (i in parts.size)
      
    }// fine loop sugli eventi (cominciava con "while")
  }// fine loop sui files for(int ifiles....)
  
  TCanvas *can0x = new TCanvas;
  h0x->Draw();
  
  return 0;
}
