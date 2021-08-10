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
        std::string m_name;
        float m_balance;
        double m_wallet;
        std::shared_ptr<Stall> m_stall;

        // constructor
        Node(std::string& ar_name, const float& ar_balance, const double& ar_wallet);

        // destructor
        ~Node();

        // methods
        void transferTo(Blockchain& ar_blockchain, std::vector<std::shared_ptr<Node>>& arp_nodesList, std::string& ar_receiver, const uint32_t& ar_timestamp);
        
        void startMining(std::string& ar_transactionData, std::string& ar_prevHash);

        void withdraw(float& ar_amount, std::vector<std::shared_ptr<Node>>& ar_nodesList); // amount -> amount of bitcoins

        void openMyStall(Marketplace& ar_marketplace, std::shared_ptr<Node>& arp_seller, bool& ar_type, double& ar_amount);

        void closeMyStall(Marketplace& ar_marketplace);

        void visitStall(Blockchain& ar_blockchain, std::vector<std::shared_ptr<Node>>& ar_nodesList, Marketplace& ar_marketplace, uint64_t& ar_stallNumber);

};