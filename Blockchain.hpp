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

// global variables
// uint16_t DIFFICULTY = 10;
// float TRANSACTION_FEE;
// float VALUE;
// std::ifstream inputS;
// std::ofstream outputS;
// std::ofstream logS("./log.txt");
// bool finished = 0;



// determined threads to update bitcoin transaction fee and values every 30 seconds
// std::thread subSystem1(update, &TRANSACTION_FEE, &VALUE);
// std::thread subSystem2(startNodejsProgram);


class Block
{

    private:
        std::string correctHash;

    public:
        std::string transactionsData;

        // constructor
        Block(std::string& td);
        // : transactionsData(td)
        // {
        //     correctHash = calcHash(transactionsData);

        //     logS << "Block crated successfully!" << std::endl;
            
        // }

        // destructor
        ~Block();
        // {              

        //     logS << "Block deleted successfully!" << std::endl;

        // }

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
        // {


            
        //     logS << "Blockchain crated successfully!" << std::endl;
        // }

        // destructor
        ~Blockchain();
        // {
        //     chain.clear();

        //     logS << "Block deleted successfully!" << std::endl;
        // }

        void openCompetition(std::string& leftOut, std::vector<Node*>& nl);
        // {
        //     ++last_index;
        //     std::unique_ptr<Block> dummyPtr(new Block(tmpTransactionsData));
        //     chain.push_back(std::move(dummyPtr));

        //     tmpTransactionsData = ""; //reset tmpTransactionsData
        //     tmpTransactionsData += leftOut;

        //     // signal nodes to start competing in order to complete the creation of the newly initialized block
        //     for (auto& n : nl)
        //     {
        //         std::thread(Node::startMining, n, chain[last_index]->transactionsData).detach();
        //     }

        //     std::cout << "I managed to get here!" << std::endl;

        //     finished = 0;
        // }
}; 