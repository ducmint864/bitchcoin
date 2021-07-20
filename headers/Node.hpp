#include <vector>
#include <memory>
#ifndef Node_H
    #define Node_H
#endif



class Block;
class Blockchain;
class Stall;
class Marketplace;



class Node
{

    public:
        std::string name;
        float balance;
        double wallet;
        std::shared_ptr<Stall> mystall;

        // constructor
        Node(std::string& n, const float& b, const double& w, const std::string& curr);

        // destructor
        ~Node();

        // methods
        void transferTo(Blockchain& blc, std::vector<std::shared_ptr<Node>>& nl, std::string& receiver, const uint32_t& timestamp);
        
        void startMining(std::string& td, std::string& ph);

        void withdraw(float& amount, std::vector<std::shared_ptr<Node>>& nl); // amount -> amount of bitcoins

        void openMyStall(Marketplace& mp, std::shared_ptr<Node> &s, bool& t, double& a);

        void closeMyStall(Marketplace& mp);

        void visitStall(Blockchain& blc, std::vector<std::shared_ptr<Node>>& nl, Marketplace& mp, uint64_t& stallNum);

};