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
        int32_t balance;

        // constructor
        Node(std::string& n, int32_t b);
        // : name(n), balance(b)
        // {
        //     logS << "Node created successfully!" << std::endl;
        // }

        // destructor
        ~Node();
        // {
        //     logS << "Node deleted successfully!" << std::endl;
        // }

        void transferTo(Blockchain& blc, std::vector<Node*>& nl, std::string& receiver, uint32_t& amount);
        // {
        //     // transfer money
        //     bool found = 0;
        //     for (auto& n : nl)
        //     {
        //         if (n->name == receiver)
        //         {
        //             found = 1;
        //             n->balance += amount;
        //             this->balance -= amount;
        //         }
        //     }

        //     // if not found
        //     if (found == 0) 
        //     {
        //         std::cout << "Node doesn't exist!" << std::endl;
        //         return;
        //     }

        //     // write to public ledger
        //     std::string tmp = this->name + ' ' + receiver + ' ' + std::to_string(amount) + '\n';

        //     if ( (blc.tmpTransactionsData.length() + tmp.length()) >= MAX_TRANSACTION_SIZE-1 )
        //     {
        //         blc.openCompetition(tmp, nl);
        //     }
            
        //     else
        //     {
        //         blc.tmpTransactionsData += tmp;
        //     }

        // }

        void startMining(std::string& td);
        // {
        //     while (finished == 0)
        //     {
        //     }
        // }  
};