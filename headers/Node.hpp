#include <iostream>
#include <thread>
#include <vector>
#include <fstream>
#ifndef Node_H
    #define Node_H
#endif

class Block;
class Blockchain;



struct wallet
{

    double amount;
    std::string currency;

}; typedef struct wallet wallet;



class Node
{

    public:
        std::string name;
        float balance;
        wallet mywallet;

        // constructor
        Node(std::string& n, const float& b, const float& w, const std::string& curr);

        // destructor
        ~Node();

        // methods
        void transferTo(Blockchain& blc, std::vector<Node*>& nl, std::string& receiver, const uint32_t& timestamp);
        
        void startMining(std::string& td);

        void withdraw(float& amount, std::string& currency, std::vector<Node*>& nl); // amount -> amount of bitcoins

};