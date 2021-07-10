#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <sstream>
#include <fstream>
#include <vector>
#include <memory>
#include <thread>
#include <chrono>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <openssl/sha.h>
#ifndef Blockchain_H
    #define Blockchain_H
#endif



class Node;



class Block
{

    private:
        std::string correctHash;

    public:
        std::string transactionsData;
        uint32_t timestamp;

        // constructor
        Block(std::string& td);

        //empty constructor
        Block();
        
        // destructor
        ~Block();
        
    friend class Blockchain;

};


class Blockchain
{

    public:
        std::vector<std::unique_ptr<Block>> chain;
        std::string tmpTransactionsData;
        uint64_t last_index = -1;

        // constructor
        Blockchain ();

        // destructor
        ~Blockchain();

        // methods
        void openCompetition(std::string& leftOut, std::vector<Node*>& nl);

        void syncDatabase(const uint16_t& mode = 0);

        void showAllBlocks();

}; 