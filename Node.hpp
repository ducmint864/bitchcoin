#include <iostream>
#include <thread>
#include <vector>
#include <fstream>
#ifndef Node_H
    #define Node_H
#endif

class Block;
class Blockchain;

class Node
{

    public:
        std::string name;
        float balance;

        // constructor
        Node(std::string& n, const float& b);

        // destructor
        ~Node();

        // methods
        void transferTo(Blockchain& blc, std::vector<Node*>& nl, std::string& receiver, float& amount);
        void startMining(std::string& td);
};