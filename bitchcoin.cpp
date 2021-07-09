#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <memory>
#include <thread>
#include <chrono>
#include <ctime>
#include <cmath>
#include <openssl/sha.h>



const uint32_t MAX_TRANSACTION_SIZE = 1048576; // 1048576 bytes - 1048576 chars (1 MB worth of transactions datas)
const uint16_t DIFFICULTY = 10; // 10 minutes for every block
const uint16_t REWARD = 6.25;
cont



class Block
{

    private:
        std::string transactionData;

};



class Blockchain
{

    private:
        std::vector<std::unique_ptr<Block>> chain;

}; 



class Nodes
{


    
};



class System
{


    
};



int main()
{



}