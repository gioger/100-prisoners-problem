#include "TH1F.h"
#include <fstream>
#include <iostream>

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

    h1->Draw();
    std::cout << "Overflow: " << h1->GetBinContent(range + 1) << '\n';
}