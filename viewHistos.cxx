/*
	Show the histos from the LTTCefficiecy analysys
*/

void viewHistos(string inFile = "out_charged.root") {

	// Retrieve the histos from the root file

	TFile * file = new TFile(inFile.c_str(), "READ");

	TH1F * histo0 = (TH1F *) file->Get("histo0");
	TH1F * histo1 = (TH1F *) file->Get("histo1");
	TH1F * histo2 = (TH1F *) file->Get("histo2");
	TH1F * hratio = (TH1F *) file->Get("hratio");


	// Show the histos
	
	TCanvas * c = new TCanvas("c", "c", 1000, 600);
	c->Divide(2,2);

	c->cd(1) ; histo0->Draw();
	c->cd(2) ; histo1->Draw();
	c->cd(3) ; histo2->Draw();
	c->cd(4) ; hratio->Draw();

	// Save a pdf file. Use the input file name replacing ".root" with ".pdf"
	
	string outPdfName = inFile.replace(inFile.end()-4,inFile.end(),"pdf") ;
	c->SaveAs(outPdfName.c_str());

	return;
}