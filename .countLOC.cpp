#include <iostream>
#include <fstream>

int main()
{

    // relative to this outermost dir of the project
    std::string paths[9] = {"test.cpp", "bitchcoin.cpp", "headers/Blockchain.hpp", "headers/Marketplace.hpp", "headers/Node.hpp", "headers/coinutils.hpp", "headers/timeutils.hpp"};
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

    std::ofstream fileout("assets/totalLOC");
    fileout << count;
    fileout.close();

    return 0;

}