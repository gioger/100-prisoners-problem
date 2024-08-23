// multiple simulations

#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <random>
#include <cstdlib> // for std::atoi
#include <fstream>

bool isPositiveInteger(const std::string &str)
{
    // Check for a minus sign at the beginning (probably useless)
    size_t start = 0;
    if (str[0] == '-')
    {
        return false;
    }

    // Ensure the remaining characters are all digits
    for (size_t i = start; i < str.size(); ++i)
    {
        if (!std::isdigit(str[i]))
        {
            return false;
        }
    }

    return true;
}

class Box
{
public:
    int boxNo{};
    int paperNo{};

    void printBox()
    {
        std::cout << '[' << boxNo << ',' << paperNo << ']' << '\n';
    }
};

int main(int argc, char *argv[])
{
    // error handling
    if (argc != 2)
    {
        std::cerr << "You have to specify the number of simulation, as in: " << argv[0] << " <number of simulations>\n";
        return 1;
    }

    int n_simulations{0};
    try
    {
        if (isPositiveInteger(argv[1]))
        {
            n_simulations = std::atoi(argv[1]);
        }
        else
        {
            throw std::invalid_argument("The number of simulations must be a positive integer");
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    int success_count{0}; // counter of successful (inmates win) simulations

    std::ofstream txtFile("failureNumber.txt");
    if (!txtFile.is_open())
    {
        std::cerr << "Error: unable to open file" << std::endl;
        return 1;
    }

    for (int sim = 0; sim < n_simulations; ++sim)
    {
        std::array<Box, 100> boxes;

        std::vector<int> numbers{};
        for (int i = 0; i < 100; ++i)
        {
            numbers.push_back(i);
        }

        std::random_device rd;                           // rng seed
        std::mt19937 g(rd());                            // rng engine
        std::shuffle(numbers.begin(), numbers.end(), g); // shuffle the numbers

        for (int i = 0; i < 100; i++) // assign the shuffled numbers to the boxes
        {
            boxes[i].boxNo = i;
            boxes[i].paperNo = numbers[i];
        }

        bool all_succeed{true};

        for (int i = 0; i < 100; ++i)
        {
            int counter{0}; // counter of opened boxes
            int j{i};

            while (counter < 50)
            {
                if (boxes[j].paperNo == i)
                {
                    break; // i-th prisoner founds his number
                }
                j = boxes[j].paperNo; // follow the chain
                counter++;
            }

            if (counter == 50)
            {
                all_succeed = false;
                txtFile << i + 1 << '\n'; // +1 for constistency with the problem
                break;                    // i-th prisoner failed
            }
        }

        if (all_succeed)
        {
            success_count++;
        }
    }

    // stats
    double success_perc = (static_cast<double>(success_count) / n_simulations) * 100.0;
    std::cout << "Success percentage: " << success_perc << "% (" << success_count << '/' << n_simulations << ')' << '\n';
}
