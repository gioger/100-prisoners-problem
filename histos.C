#include "TH1F.h"
#include <fstream>
#include <iostream>
#include <TF1.h>

void iris()
{
    const int range{10};

    TH1F *h1 = new TH1F("h1", "Prisoner that fails", range, 1, range);

    std::ifstream inputFile("failureNumber.txt");

    if (!inputFile.is_open())
    {
        std::cout << "Failed to open input file!\n";
        return;
    }

    int value;
    while (inputFile >> value)
    {
        h1->Fill(value);
    }

    inputFile.close();

    // geometric fit
    TF1 *f1 = new TF1("f1", "[1]*[0]*((1-[0])**(x-1))");
    f1->SetParameter(0, 0.5);
    h1->Fit("f1", "Q");

    // histogram cosmetic
    h1->SetFillColor(38);
    h1->SetFillStyle(3001);
    h1->SetLineColor(9);
    h1->SetLineWidth(2);
    h1->SetTitle("Distribuzione del primo prigioniero a fallire");
    h1->GetXaxis()->SetTitle("Numero del prigioniero");
    h1->GetYaxis()->SetTitle("Occorrenze");

    // fit cosmetic
    f1->SetLineColor(2);
    f1->SetLineWidth(2);

    h1->Draw();
    f1->Draw("same");

    std::cout << "Overflow: " << h1->GetBinContent(range + 1) << '\n';
    std::cout << "p = " << f1->GetParameter(0) << " +/- " << f1->GetParError(0) << '\n';
    std::cout << "chi^2 ridotto = " << f1->GetChisquare() / f1->GetNDF() << '\n';
}