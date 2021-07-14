#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <chrono>
#include <cstdlib>
#include <cmath>
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


void update(float* f, float* v, float* bv, float* sv, const bool* b) 
{

    system("kitty node assets/index.js &");

    std::ifstream filein;
    std::string line, currency;
    uint16_t count = 0, tmp;
    bool firtTime = 1;

    while (*b == 0)
    {

        if (firtTime != 1)
        {

            // check if fetcher is still working properly, if fetcherState has code 0, then the fetcher is down
            filein.open("./assets/fetcherState");
            if (!filein.is_open())
            {

                std::cout << "ERROR! CAN'T OPEN FILE TO READ TRANSACTION FEE!\n";
                return;

            }

            filein >> tmp;
            if (tmp == 0)
            {

                while (*b == 0 && count < 35000)
                {

                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    count += 500;

                }

            }

            firtTime = 0;
            
        }
        
        // fetch and update F
        filein.open("assets/fee");
        if (!filein.is_open())
        {

            std::cout << "ERROR! CAN'T OPEN FILE TO READ TRANSACTION FEE!\n";
            return;

        }

        filein >> *f;
        filein.close();

        // fetch and update V
        filein.open("assets/values");
        while (std::getline(filein, line))
        {

            std::istringstream ss(line);
            ss >> currency;
            if (currency == "USD")
            {
                
                ss >> *bv;
                ss >> *sv;
                *v = (*bv + *sv) / 2;

            }


        }

        count = 0;
        while (*b == 0 && count < 35000)
        {

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            count+=500;

        }

    }

    system("killall -q node"); // kill nodejs fetcher
    filein.close();

}



std::string calcHash(std::string& td, const uint16_t& difficulty = 1, const std::string& mode = "sha256")
{

    std::ostringstream ss;
    uint16_t prefixCharCount = random2(difficulty, pow(difficulty, 2));

    if (mode == "sha256")
    {

        std::string prefix(prefixCharCount, ('0' + random(0, 9)) );
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


void guessHash(std::string td, std::string name, const uint16_t difficulty, bool* finished)
{
    
    std::ofstream fileout("./assets/arena");
    std::string res;

    while (*finished == 0)
    {

        fileout << calcHash(td, difficulty) << ' ' << name << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // lower interval for the judge to read correctly

    }

    fileout.close();

}


void startNodejsProgram()
{

    system("node assets/index.js &");

}

void printAllValues()
{

    std::ifstream filein("./assets/values");

    std::string line;
    std::string tmp;
    std::cout << "----------------------------------------------------------------\n";
    std::cout << "\t--> <currency>" << ' ' << ' ' <<  "<buy value>" << ' ' << "<sell value>" << std::endl;

    while (std::getline(filein, line))
    {

        std::istringstream ss(line);

        // <currency>
        ss >> tmp;
        std::cout << "\t--> " << tmp << ' ';

        // <buy value>
        ss >> tmp;
        std::cout << tmp << ' ';
        
        // <sell value>
        ss >> tmp;
        std::cout << tmp << std::endl;

    }

    std::cout << "----------------------------------------------------------------\n";

    filein.close();
    
}