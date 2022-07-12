#include <iostream>
#include <fstream>

int main()
{

    // relative to this outermost dir of the project
    std::string paths[9] = {"bitchcoin.cpp", "headers/Blockchain.hpp", "headers/Marketplace.hpp", "headers/Node.hpp", "headers/coinutils.hpp", "headers/timeutils.hpp", "assets/index.js"};
    std::ifstream filein;

    uint64_t count = 0;
    std::string dummy;

    for (auto path : paths)
    {

        filein.open(path);
        while (std::getline(filein, dummy))

        {
            ++count;
        }

        filein.close();

    }

    std::ofstream fileout("bonus/totalLOC");
    fileout << count;
    fileout.close();

    std::cout << "Total LOC: " << count << std::endl;

    return 0;

}