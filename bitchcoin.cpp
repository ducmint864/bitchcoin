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
#include "timeutils.hpp"
#include "coinutils.hpp"



#define MAX_TRANSACTION_SIZE 1048576
#define REWARD 6.25
#define TOTAL = 21000000 
#define clrscr() std::cout << "\e[1;1H\e[2J"




// global variables
uint16_t DIFFICULTY = 10;
float TRANSACTION_FEE;
float VALUE;
std::ifstream inputS;
std::ofstream outputS;
std::ofstream logS("./log.txt");



// determined worker thread to update bitcoin transaction fee and values every 30 seconds
std::thread worker(update, &TRANSACTION_FEE, &VALUE);



class Block
{

    private:
        std::string correctHash;

    public:
        char transactionsData[MAX_TRANSACTION_SIZE];

        // constructor
        Block()
        {
            
            correctHash = calcHash(transactionsData);

            logS << "Block crated successfully!" << std::endl;
            
        }

        // destructor
        ~Block()
        {              

            logS << "Block deleted successfully!" << std::endl;

        }

    friend class Blockchain;

};



class Blockchain
{

    public:
        std::vector<std::unique_ptr<Block>> chain;
        char tmpTD[MAX_TRANSACTION_SIZE] = "";

        // constructor
        Blockchain ()
        {


            
            logS << "Blockchain crated successfully!" << std::endl;
        }

        // destructor
        ~Blockchain()
        {
            chain.clear();

            logS << "Block deleted successfully!" << std::endl;
        }
}; 



class Node
{

    public:
        std::string name;
        int32_t balance;

        // constructor
        Node(std::string& n, int32_t b)
        : name(n), balance(b)
        {
            logS << "Node created successfully!" << std::endl;
        }

        // destructor
        ~Node()
        {
            logS << "Node deleted successfully!" << std::endl;
        }

        void transferTo(Blockchain& blc, std::vector<Node*>& nl,std::string& receiver, uint32_t amount)
        {
            // transfer money
            bool found = 0;
            for (auto& n : nl)
            {
                if (n->name == receiver)
                {
                    found = 1;
                    n->balance += amount;
                    this->balance -= amount;
                }
            }

            // if not found
            if (found == 0) 
            {
                std::cout << "Node doesn't exist!" << std::endl;
                return;
            }

            // write to public ledger
            std::string tmp = this->name + ' ' + receiver + ' ' + std::to_string(amount) + '\n';
            strcat(blc.tmpTD, tmp.c_str());

        }

        void startMining()
        {

        }
    
};



int main()
{
    // pause the main thread for 1s to wait for values to be updated;

    std::this_thread::sleep_for(std::chrono::milliseconds(1000)); 
    // a vector of NODES(ptr) as a method for the nodes to interact with one another
    std::vector<Node*> nodesList;
    Blockchain chain;
    std::string tmpName;

    for (uint64_t i = 0; i < 10; i++)
    {
        tmpName = "node" + std::to_string(i+1);
        Node* dummyPtr = new Node(tmpName, 0);
        nodesList.push_back(dummyPtr);
    }
    
    Node* whoami = nodesList[0];
    std::cout << "You are now controlling node 1 You can change if you wish to\nPress enter to continue to program...";
    std::cin.get();
    clrscr();

    char option;
    while (1)
    {
        std::cout << "Queries | (t)ransfer, (n)ew node, (s)witch node, (q)uit" << std::endl;
        scanf(" %c", &option);
        switch (option)
        {
            case 't':
                uint32_t tmpAmount;
                std::cout << "Enter the name of the receiver:\n>>> ";
                std::cin >> tmpName;
                std::cout << "Enter amount of bitcoin to transfer:\n>>> ";
                std::cin >> tmpAmount;
                whoami->transferTo(chain, nodesList, tmpName, tmpAmount);
                break;

            case 'n':
                std::cout << "Enter name of new node :\n>>> ";
                std::cin >> tmpName;
                break;

            case 's':
                std::cout << "Enter the name of the node you want to control:\n>>> ";
                std::cin >> tmpName;
                for (auto n : nodesList)
                {
                    if (tmpName == n->name)
                    {
                        whoami = n;
                    } 
                }                
                break;

            case 'q':
                exit(EXIT_SUCCESS);
                break;

            default:
                std::cout << "Invalid option : " << option << std::endl;
                break;
        }
    }

    return 0;

}