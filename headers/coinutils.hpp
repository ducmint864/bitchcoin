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


void update(float* ar_FEE, float* ar_VALUE, float* ar_BUY_VALUE, float* ar_SELL_VALUE, const bool* ar_FINISHED) 
{

    system("kitty node assets/index.js &");

    std::ifstream filein;
    std::string line, currency;
    uint16_t count = 0, tmp;
    bool firstTime = 1;

    while (*ar_FINISHED == 0)
    {

        if (firstTime != 1)
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

                while (*ar_FINISHED == 0 && count < 35000)
                {

                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    count += 500;

                }

            }

            firstTime = 0;
            
        }
        
        // fetch and update fees
        filein.open("assets/fee");
        if (!filein.is_open())
        {

            std::cout << "ERROR! CAN'T OPEN FILE TO READ TRANSACTION FEE!\n";
            return;

        }

        filein >> *ar_FEE;
        filein.close();

        // fetch and update values
        filein.open("assets/values");
        while (std::getline(filein, line))
        {

            std::istringstream ss(line);
            ss >> currency;
            if (currency == "USD")
            {
                
                ss >> *ar_BUY_VALUE;
                ss >> *ar_SELL_VALUE;
                *ar_VALUE = (*ar_BUY_VALUE + *ar_SELL_VALUE) / 2;

            }


        }

        count = 0;
        while (*ar_FINISHED == 0 && count < 35000)
        {

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            count+=500;

        }

    }

    system("killall -q node"); // kill nodejs fetcher
    filein.close();

}



std::string calcHash(std::string& ar_transactionsData, std::string& ar_prevHash, const uint16_t& ar_DIFFICULTY = 1, const std::string& ar_mode = "sha256")
{

    std::ostringstream ss;
    uint16_t prefixCharCount = random2(ar_DIFFICULTY, pow(ar_DIFFICULTY, 2));

    if (ar_mode == "sha256")
    {

        std::string prefix(prefixCharCount, ('0' + random(0, 9)) );
        prefix += (ar_transactionsData + ar_prevHash);
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

        std::cout << "Invalid mode : " << ar_mode;

    }

    return ss.str();

}


void guessHash(std::string ar_transactionsData, std::string ar_prevHash, std::string ar_name, const uint16_t ar_DIFFICULTY, bool* ar_FINISHED)
{
    
    std::ofstream fileout("./assets/arena");
    std::string res;

    while (*ar_FINISHED == 0)
    {

        fileout << calcHash(ar_transactionsData, ar_prevHash, ar_DIFFICULTY) << ' ' << ar_name << std::endl;
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