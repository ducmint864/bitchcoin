#include <iostream>
#include <fstream>

uint16_t updateValues()
{   
    uint16_t res;
    std::ifstream infile("assets/fee");

    infile >> res;
    return res;
} 
