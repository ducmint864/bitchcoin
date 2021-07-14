#include <iostream>
#include <thread>
#include <vector>
#include <fstream>
#ifndef Node_H
    #define Node_H
#endif



class Block;
class Blockchain;
class Stall;
class Marketplace;



// struct wallet
// {

//     double amount;
//     std::string currency;

// }; typedef struct wallet wallet;



class Node
{

    public:
        std::string name;
        float balance;
        double wallet;
        Stall* mystall;

        // constructor
        Node(std::string& n, const float& b, const double& w, const std::string& curr);

        // destructor
        ~Node();

        // methods
        void transferTo(Blockchain& blc, std::vector<Node*>& nl, std::string& receiver, const uint32_t& timestamp);
        
        void startMining(std::string& td);

        void withdraw(float& amount, std::vector<Node*>& nl); // amount -> amount of bitcoins

        void openMyStall(Marketplace& mp, Node* &s, bool& t, double& a);

        void closeMyStall(Marketplace& mp);

        void visitStall(Blockchain& blc, std::vector<Node*>& nl, Marketplace& mp, uint64_t& stallNum);

};