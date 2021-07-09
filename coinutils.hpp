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

uint16_t random2(const uint16_t &min, const uint16_t &max) //range : [min, max]
{
	srand(time(NULL));

   	return min + rand() % (( max + 1 ) - min);
}


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



std::string calcHash(std::string& td, const uint16_t& difficulty = 1, const std::string& mode = "sha256")
{
    std::ostringstream ss;
    uint16_t prefixCharCount = random(difficulty, difficulty*100);

    if (mode == "sha256")
    {
        std::string prefix(prefixCharCount, '0');
        prefix += td;
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, prefix.c_str(), prefix.length());
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

void startNodejsProgram()
{
    system("node assets/index.js &");
}

void guessHash(std::string& td, std::string& name, const uint16_t& difficulty, bool& finished)
{
    std::ofstream fileout("./arena");
    std::string res;

    while (finished == 0)
    {
        res = calcHash(td, difficulty);
        fileout << res << ' ' << name << std::endl;
    }

    fileout.close();
}