/*
	Show the histos from the LTTCefficiecy analysys
*/
#include <string>

void viewHistos(string inFile = "out_LTCCefficiency.root") {

	// Retrieve the histos from the root file

	TFile * file = new TFile(inFile.c_str(), "READ");

	TH1F * histo0 = (TH1F *) file->Get("histo0");
	TH1F * histo1 = (TH1F *) file->Get("histo1");
	TH1F * histo2 = (TH1F *) file->Get("histo2");
	TH1F * histo3 = (TH1F *) file->Get("histo3");
	TH1F * histo4 = (TH1F *) file->Get("histo4");
	TH1F * histo5 = (TH1F *) file->Get("histo5");
	TH1F * histo6 = (TH1F *) file->Get("histo6");
	TH1F * histo3bis = (TH1F *) file->Get("histo3bis");
	TH1F * histo4bis = (TH1F *) file->Get("histo4bis");
	
	TH2F * histo7 = (TH2F *) file->Get("histo7");
	TH2F * histo8 = (TH2F *) file->Get("histo8");
	TH2F * histo9 = (TH2F *) file->Get("histo9");
	TH2F * histo10 = (TH2F *) file->Get("histo10");
	TH2F * histo11 = (TH2F *) file->Get("histo11");
	TH2F * histo12 = (TH2F *) file->Get("histo12");
	TH2F * histo13 = (TH2F *) file->Get("histo13");
	TH2F * histo14 = (TH2F *) file->Get("histo14");
	TH2F * histo15 = (TH2F *) file->Get("histo15");
	TH2F * histo16 = (TH2F *) file->Get("histo16");
	
	TH1F * hratio = (TH1F *) file->Get("hratio");
	TH1F * htheta = (TH1F *) file->Get("htheta");
	TH1F * hphi = (TH1F *) file->Get("hphi");
	TH1F * hcos = (TH1F *) file->Get("hcos");
	
	TH2F * hxy = (TH2F *) file->Get("hxy");
	TH2F * hangles = (TH2F *) file->Get("hangles");
	TH2F * hphicos = (TH2F *) file->Get("hphicos");
	TH2F * hptheta = (TH2F *) file->Get("hptheta");
	TH2F * hpcos = (TH2F *) file->Get("hpcos");


	// Show the histos
	
	//1D-histograms
	TCanvas * c1 = new TCanvas("c1", "c1", 1000, 600);
	c1->Divide(2,2);
	c1->cd(1) ; histo0->Draw();
	c1->cd(2) ; histo1->Draw();
	c1->cd(3) ; histo2->Draw();
	c1->cd(4) ; hratio->Draw();

	////histo num, den, ratio
	TCanvas * c2 = new TCanvas("c2", "c2", 1500, 600);
	c2->Divide(3,1);
	c2->cd(1)	;	histo3->Draw();
	c2->cd(2) ; histo4->Draw();
	c2->cd(3) ; htheta->Draw();

	TCanvas * c3 = new TCanvas("c3", "c3", 1500, 600);
	c3->Divide(3,1);
	c3->cd(1)	;	histo5->Draw();
	c3->cd(2)	;	histo6->Draw();
	c3->cd(3)	;	hphi->Draw();

	TCanvas * c4 = new TCanvas("c4", "c4", 1500, 600);
	c4->Divide(3,1);
	c4->cd(1)	;	histo3bis->Draw();
	c4->cd(2)	;	histo4bis->Draw();
	c4->cd(3)	;	hcos->Draw();

	//2D-histograms
	TCanvas * ca1 = new TCanvas("ca1", "ca1", 1500, 600);
	ca1->Divide(3,1);
	ca1->cd(1) ; histo7->Draw();
	ca1->cd(2) ; histo8->Draw();
	ca1->cd(3) ; hxy->Draw();

	TCanvas * ca2 = new TCanvas("ca2", "ca2", 1500, 600);
	ca2->Divide(3,1);
	ca2->cd(1) ; histo9->Draw();
	ca2->cd(2) ; histo10->Draw();
	ca2->cd(3) ; hangles->Draw();

	TCanvas * ca3 = new TCanvas("ca3", "ca3", 1500, 600);
	ca3->Divide(3,1);
	ca3->cd(1) ; histo11->Draw();
	ca3->cd(2) ; histo12->Draw();
	ca3->cd(3) ; hphicos->Draw();

	TCanvas * ca4 = new TCanvas("ca4", "ca4", 1500, 600);
	ca4->Divide(3,1);
	ca4->cd(1) ; histo13->Draw();
	ca4->cd(2) ; histo14->Draw();
	ca4->cd(3) ; hptheta->Draw();

	TCanvas * ca5 = new TCanvas("ca5", "ca5", 1500, 600);
	ca5->Divide(3,1);
	ca5->cd(1) ; histo15->Draw();
	ca5->cd(2) ; histo16->Draw();
	ca5->cd(3) ; hpcos->Draw();

	// Save a pdf file. Use the input file name replacing ".root" with ".pdf"
	string input = inFile.c_str();

	input.erase(input.begin()+18,input.end());
	string outPdfName = input.append(".pdf") ;
	c1->SaveAs(outPdfName.c_str());

	input.erase(input.begin()+18,input.end());
	string outPdfName2 = input.append("_theta.pdf") ;
	c2->SaveAs(outPdfName2.c_str());

	input.erase(input.begin()+18,input.end());
	string outPdfName3 = input.append("_phi.pdf") ;
	c3->SaveAs(outPdfName3.c_str());

	input.erase(input.begin()+18,input.end());
	string outPdfName4 = input.append("_costheta.pdf") ;
	c4->SaveAs(outPdfName4.c_str());

	input.erase(input.begin()+18,input.end());
	string outPdfName5 = input.append("_xy.pdf");
	ca1->SaveAs(outPdfName5.c_str());

	input.erase(input.begin()+18,input.end());
	string outPdfName6 = input.append("_phitheta.pdf");
	ca2->SaveAs(outPdfName6.c_str());

	input.erase(input.begin()+18,input.end());
	string outPdfName7 = input.append("_phicostheta.pdf");
	ca3->SaveAs(outPdfName7.c_str());

	input.erase(input.begin()+18,input.end());
	string outPdfName8 = input.append("_ptheta.pdf");
	ca4->SaveAs(outPdfName8.c_str());

	input.erase(input.begin()+18,input.end());
	string outPdfName9 = inFile.append("_pcostheta.pdf");
	ca5->SaveAs(outPdfName9.c_str());

	return;
}
