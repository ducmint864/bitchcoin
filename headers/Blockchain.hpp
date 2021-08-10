#include <vector>
#include <memory>
#ifndef Blockchain_H
    #define Blockchain_H
#endif



class Node;



class Block
{

    private:
        std::string m_correctHash;
        std::string m_prevHash; // hash of previous block in blockchain

    public:
        std::string m_transactionsData;
        uint32_t m_timestamp;

        // constructor
        Block(std::string& ar_transactionData, std::string& ar_prevHash);

        // empty constructor
        Block();
        
        // destructor
        ~Block();
        
    friend class Blockchain;

};


class Blockchain
{

    public:
        std::vector<std::unique_ptr<Block>> m_chain;
        std::string m_tmpTransactionsData;
        uint64_t m_currentIndex = -1; // current index is also always the last index in chain (if nothing went wrong)

        // constructor
        Blockchain ();

        // destructor
        ~Blockchain();

        // methods
        void openCompetition(std::string& ar_leftOut, std::vector<std::shared_ptr<Node>>& ar_nodesList);

        void syncDatabase(const uint16_t& ar_mode = 0);

        void showAllBlocks();

};
