#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <chrono>
#include <cstdlib>
#include <sstream>
#include <string>
#include <cstring>
#include <thread>
#include <openssl/sha.h>



void update(float* f, float* v) 
{
    std::ifstream infile;
    infile.open("assets/fee");
    std::string line, currency;

    while (1)
    {
        // fetch and update F
        if (!infile.is_open())
        {
            std::cout << "ERROR! CAN'T OPEN FILE TO READ TRANSACTION FEE!\n";
            return;
        }
        infile >> *f;
        infile.close();

        // fetch and update V
        infile.open("assets/values");
        while (std::getline(infile, line))
        {
            std::istringstream ss(line);
            ss >> currency;
            if (currency == "USD")
            {
                ss >> *v;
            }
        }

        // wait 30 seconds before updating again
        std::this_thread::sleep_for(std::chrono::milliseconds(30000));
    }
    
    infile.close();
}



std::string calcHash(char* td, const std::string& mode = "sha256")
{
    std::ostringstream ss;

    if (mode == "sha256")
    {
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, td, strlen(td));
        SHA256_Final(hash, &sha256);

        
        for (uint16_t i = 0; i < SHA256_DIGEST_LENGTH; i++)
        {
            ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
        }
    }

    else 
    {
        std::cout << "Invalid mode : " << mode;
    }

    return ss.str();
}