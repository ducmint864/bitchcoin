#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <memory>
#include <thread>
#include <chrono>
#include <openssl/sha.h>
#include "../headers/timeutils.hpp"
#include "../headers/coinutils.hpp"
#include "../headers/Node.hpp"
#include "../headers/Blockchain.hpp"
#include "../headers/Marketplace.hpp"



// global variables

    /* constants */ 
const uint16_t         gc_NODE_COUNT                         = 15;
const uint16_t         gc_DIFFICULTY                         = 1000;
const uint16_t         gc_INITIAL_AMOUNT                     = gc_NODE_COUNT;
const uint16_t         gc_MAX_TRANSACTION_DATA_SIZE_IN_BYTES = 128;
const uint32_t         gc_TOTAL                              = 21000000;
const float            gc_REWARD                             = 6.25;
const std::string      gc_HASH_OF_DEATH                      = "0000000000000000000000000000000000000000000000000000000000000000";
     
    /* non-constants */ 
bool                   g_FINISHED                            = 0;
float                  g_TRANSACTION_FEE;
float                  g_VALUE;
float                  g_BUY_VALUE;
float                  g_SELL_VALUE;
std::ofstream          logS("./.log");



// determined threads to update bitcoin transaction fee and values every 30 seconds
std::thread g_subsystem(update, &g_TRANSACTION_FEE, &g_VALUE, &g_BUY_VALUE, &g_SELL_VALUE, &g_FINISHED);



// judge
std::string whoisthewinner(std::string& ar_correctHash)
{

    std::ifstream infile("./assets/arena");
    std::string line, tmpHash, winner;

    while (std::getline(infile, line))
    {

        std::istringstream ss(line);
        ss >> tmpHash;

        if (tmpHash == ar_correctHash)
        {

            g_FINISHED = 1;
            ss >> winner;
            break;

        }
        
    }

    infile.close();
    return winner;
    
}



/* useful fucntions */

void clrscr()
{
    std::cout << "\e[1;1H\e[2J";
}

bool whereOurCoinsAt(std::vector<std::shared_ptr<Node>>& ar_nodesList)
{

    bool extremelyPoor = 1;
    
    std::ifstream filein;
    filein.open("./assets/yourCoinsAreHere");
    if (!filein.is_open())
    {
        
        std::cout << "\n ->> ERROR! CAN'T GET ACCESS TO COINS BUCKET!" << std::endl;
        return 0;

    }

    std::string line;
    float tmp;
    uint64_t i = 0;

    while (std::getline(filein, line))
    {        
        if (extremelyPoor)
        {
            if (stof(line) != 0)
                extremelyPoor = 0;
        }
        
        ar_nodesList[i]->m_balance = std::stof(line);
        ++i;

    }

    filein.close();   
    return extremelyPoor;

}



void storeOurCoins(std::vector<std::shared_ptr<Node>> ar_nodesList)
{

    std::ofstream outfile("./assets/.tmpYourCoinsAreHere");

    if (!outfile.is_open())
    {

        std::cout << "\n ->> ERROR! DO YOU HAVE PROPER READ / WRITE PERMISSION FOR THIS DIRECTORY" << std::endl;
        return;

    }

    for (std::shared_ptr<Node> &n : ar_nodesList)    
    {

        outfile << n->m_balance << std::endl;
        
    }

    std::remove("./assets/yourCoinsAreHere");
    std::rename("./assets/.tmpYourCoinsAreHere", "./assets/yourCoinsAreHere");

    outfile.close();

}



void checkCoinBucket(std::vector<std::shared_ptr<Node>>& ar_nodesList)
{
    std::ifstream filein;
    std::string line;
    filein.open("./assets/yourCoinsAreHere");

    if (!filein.is_open()) 
    {

        std::cout << "ERROR! DO YOU HAVE PROPER READ / WRITE PERMISSION FOR THIS DIRECTORY?" << std::endl;
        return;

    }

    if (fileIsEmpty(filein) || whereOurCoinsAt(ar_nodesList) == 1) // meaning, bool extremelyPoor = 1
    {
        
        std::cout << "\n->>MONEY NOT FOUND! DETECTED POVERTY INSTEAD!" << std::endl;
        std::cout << "->>To fix poverty, I'm giving out a few coins to a random lucky person in many of you" << std::endl;
        ar_nodesList[random(0, gc_NODE_COUNT-1)]->m_balance = gc_INITIAL_AMOUNT;
        storeOurCoins(ar_nodesList); // save coins of the lucky person

    }

    else
    {

        std::cout << "\n->>MONEY FOUND! BE PATIENT, I'M WORKING HARD TO GIVE EVERYONE HIS / HER / THEIR BELOVED COINS BACK" << std::endl;

    }

    filein.close();

}



void whereOurWalletsAt(std::vector<std::shared_ptr<Node>>& ar_nodesList)
{

    std::ifstream filein("./assets/yourWalletsAreHere");
    if (!filein.is_open())
    {

        std::cout << "ERROR! DO YOU HAVE PROPER READ / WRITE PERMISSION FOR THIS DIRECTORY" << std::endl;
        return;

    }

    uint64_t i = 0;
    std::string line, tmp;

    while (std::getline(filein, line))
    {

        std::istringstream ss(line);
        ss >> tmp; ar_nodesList[i]->m_wallet = std::stod(tmp);

        ++i;

    }

    filein.close();

}



void storeOurWallets(std::vector<std::shared_ptr<Node>>& ar_nodesList)
{

    std::ofstream fileout("./assets/.tmpYourWalletsAreHere");

    for (const auto& n : ar_nodesList)
    {

        fileout << n->m_wallet << std::endl;

    }

    std::remove("./assets/yourWalletsAreHere");
    std::rename("./assets/.tmpYourWalletsAreHere", "./assets/yourWalletsAreHere");
    fileout.close();

}

/* end useful functions */



/* class methods */

// constructor
Block::Block(std::string& ar_transactionsData, std::string& ar_prevHash)
{

    m_transactionsData = ar_transactionsData;
    m_prevHash = ar_prevHash;

    m_correctHash = calcHash(m_transactionsData, m_prevHash, gc_DIFFICULTY);
    logS << "Block created successfully!" << std::endl;
    
}



// empty constructor
Block::Block()
{

    logS << "Empty Block created successfully!" << std::endl;

}



// destructor
Block::~Block()
{             

    logS << "Block freed from memory successfully!" << std::endl;

}



//constructor
Blockchain::Blockchain()
{
    
    syncDatabase(); // read values from database
    m_chain.reserve(1);

    logS << "Blockchain crated successfully!" << std::endl;

}



//destructor
Blockchain::~Blockchain()
{
    
    m_chain.clear();
    logS << "Blockchain freed from memory successfully!" << std::endl;

}



// constructor
Stall::Stall(std::shared_ptr<Node> &arp_seller, bool& ar_type, double& ar_amount)
: mp_seller(arp_seller), m_type(ar_type), m_amount(ar_amount)
{

    logS << "Stall object created sucessfully!" << std::endl;

}



// destructor
Stall::~Stall()
{

    logS << "Stall obj deleted successfully!" << std::endl;

}



// constructor
Marketplace::Marketplace()
{

    logS << "Marketplace created successfully!" << std::endl;

}



// destructor
Marketplace::~Marketplace()
{
    
    m_market.clear();
    logS << "Marketplace deleted successfully" << std::endl;

}



//methods
void Blockchain::openCompetition(std::string& ar_leftOut, std::vector<std::shared_ptr<Node>>& ar_nodesList)
{
    m_currentIndex++; // first and foremost

	if (m_chain.size() % 10 == 0) // expand capacity every creation of (multiples of 10)th block
	{

		m_chain.reserve(10 + m_chain.size());

	}

    std::string tmpPrevHash;    
    if (m_currentIndex == 0)
    {
        
        tmpPrevHash = gc_HASH_OF_DEATH;

    }

    else
    {

        tmpPrevHash = m_chain[m_currentIndex-1]->m_correctHash;

    }
    
    m_chain.push_back(std::make_unique<Block>(m_tmpTransactionsData, tmpPrevHash));
    m_tmpTransactionsData.clear(); //reset tmpTransactionsData
    m_tmpTransactionsData += ar_leftOut;

    // signal nodes to start competing in order to complete the creation of the newly initialized block
    for (auto& n : ar_nodesList)
    {

        n->startMining(m_chain[m_currentIndex]->m_transactionsData, m_chain[m_currentIndex]->m_prevHash);
        
    }

    std::cout << "\n --> All nodes have started mining\nWaiting for the correct hash to be generated" << std::endl;
    std::string winner;

    while (g_FINISHED == 0)
    {
        
        winner = whoisthewinner(m_chain[m_currentIndex]->m_correctHash);

    } 
        
    std::cout << "\n --> The correct hash has been generated by node : '" << winner << "' " << " | Winner will be rewarded with " << gc_REWARD << " (reward) + " << g_TRANSACTION_FEE << " (transactions fee)  bitcoins" << std::endl;

    // sending reward
    for (auto& n : ar_nodesList)
    {

        if (n->m_name == winner)
        {

            n->m_balance += (gc_REWARD + g_TRANSACTION_FEE);
            
        }

    }
    
    std::cout << "\n*Signaling miners to stop\n" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1500)); // waiting for miners to realize the competition is over 
    g_FINISHED = 0; // reset the boolean state for later use

    syncDatabase(1); // write changes to database

}



void Blockchain::syncDatabase(const uint16_t& ar_mode)
{

    // mode 0 | read from database
    if (ar_mode == 0)
    {

        std::ifstream filein("./assets/database");
        std::string line;
        std::string tmp1; // responsible for collecting the prevHash of the block
        std::string tmp2; // responsible for collecting the correctHash of the block
        std::string tmp3; // responsible for collecting the transactions data of the block
        
        while (filein.peek() != EOF)
        {
            
            ++m_currentIndex;
            
            tmp3 = ""; //reset

            // read in the first 2 lines, which are the hash, previous hash of the block, respectively
            std::getline(filein, line);
            tmp1 = line;
            std::getline(filein, line);
            tmp2 = line;

            // read in the next 3 lines, which is the transactions datas | the amount of lines to read equals to 3 because 128 bits can store roughly 3 lines of transactions data
            std::getline(filein, line);
            tmp3 += (line + '\n');

            std::getline(filein, line);
            tmp3 += (line + '\n');

            std::getline(filein, line);
            tmp3 += (line + '\n');

            // skip the remaining blank line
            std::getline(filein, line);

            // add block to blockchain
            std::unique_ptr<Block> dummyPtr(std::make_unique<Block>(tmp1 , tmp3));
			
			// if (chain.size() % 10 == 0) // expand capacity every creation of (multiples of 10)th element
			// {
			// 	chain.reserve(10 + chain.size());
			// }
            m_chain.push_back(std::move(dummyPtr));

            // fullfill datas of block
            // chain[current_index]->prevHash = tmp1;
            // chain[current_index]->correctHash = tmp2;
            // chain[current_index]->transactionsData = tmp3;

        }

        filein.close();
    }

    // mode 1 | write to database
    else if (ar_mode == 1)
    {
        
        std::ofstream fileout("./assets/database", std::ios::app);
        
        if (!fileout.is_open())
        {

            std::cout << "\nERROR! DO YOU HAVE THE RIGHT READ / WRITE PERMISSION FOR DATABASE" << std::endl;
            return;
            
        }

        fileout << m_chain[m_currentIndex]->m_prevHash << std::endl << m_chain[m_currentIndex]->m_correctHash << std::endl << m_chain[m_currentIndex]->m_transactionsData << std::endl;
        
        fileout.close();

    }

}



void Blockchain::showAllBlocks()
{

    std::cout << "\n----------------------------------------------------------------\n";
    
    for (uint64_t i = 0; i < m_chain.size(); i++)
    {

        std::cout << "Block number " << i+1 << ":" << std::endl;
        std::cout << "\t--> Hash of previous block : " << m_chain[i]->m_prevHash << std::endl;
        std::cout << "\t--> Hash of block : " << m_chain[i]->m_correctHash << std::endl;
        std::cout << "\t--> Transactions data :\n" << m_chain[i]->m_transactionsData << std::endl;
        std::cout << "----------------------------------------------------------------\n";

    }

}



//constructor
Node::Node(std::string& ar_name, const float& ar_balance, const double& ar_wallet)
: m_name(ar_name), m_balance(ar_balance), m_wallet(ar_wallet), m_stall(NULL)
{

    logS << "Node created successfully!" << std::endl;

}



// destructor
Node::~Node()
{
    
    logS << "Node deleted successfully!" << std::endl;
    
}



void Node::transferTo(Blockchain& ar_blockchain, std::vector<std::shared_ptr<Node>>& ar_nodesList, std::string& ar_receiver, const uint32_t& ar_timestamp)
{
    
    // initial checking step
    if (ar_receiver == m_name)
    {
        
        std::cout << "\n->> Can't transfer money to yourserlf you fishing prick!" << std::endl;
        return;

    }

    // transfer money
    float amount;
    bool found = 0;

    for (uint64_t i = 0; i < gc_NODE_COUNT; i++)
    {

        if (ar_nodesList[i]->m_name == ar_receiver)
            found = 1;
            

        if (i == (gc_NODE_COUNT - 1) && found == 0)
        {

            std::cout << "\n->> Node doesn't exit" << std::endl;
            return;

        }

        if (found)
        {
            
            std::cout << "\nEnter the amount of bitcoins to transfer :\n>>> ";
            std::cin >> amount;

            // if not enough bitcoins to transfer
            if (amount > m_balance)
            {

                char option;
                std::cout << "\n->> Your total account balance isn't sufficient for this transfer : " << m_balance << " / " << amount << std::endl;
                std::cout << "Do you want to apply an amount of debt to your account balance? | (Y)es, (N)o\n>>> ";
                std::cin >> option; std::cin.ignore();
                
                if (option == 'y' || option == 'Y')
                {

                    std::cout << "\nAre you sure ? | (Y)es, (N)o\n>>> ";
                    std::cin >> option; std::cin.ignore();

                    if (option == 'y' || option == 'Y')
                    {

                        std::cout << "\n*Sure!" << std::endl;
                        ar_nodesList[i]->m_balance += amount;
                        m_balance -= amount;
                        break;

                    }

                    else
                    {

                        std::cout << "\n*Okay! Abort transaction!" << std::endl;
                        break;

                    }

                }

                else
                {

                    std::cout << "\n*Okay! Abort transaction!" << std::endl;
                    break;

                }
            }
            

            // if enough coins for transferring
            else
            {

                ar_nodesList[i]->m_balance += amount;
                m_balance -= amount;
                break;

            }
            

        }


    }

    // write to public ledger
    std::string tmp = m_name + ' ' + ar_receiver + ' ' + std::to_string(amount) + ' ' + std::to_string(ar_timestamp) + '\n';

    if ( (ar_blockchain.m_tmpTransactionsData.length() + tmp.length()) > gc_MAX_TRANSACTION_DATA_SIZE_IN_BYTES)
    {

        ar_blockchain.openCompetition(tmp, ar_nodesList);

    }

     
    else
    {
        ar_blockchain.m_tmpTransactionsData += tmp;
    }

}

void Node::startMining(std::string& ar_transactionsData, std::string& ar_prevHash)
{
    
    std::thread worker(guessHash, ar_transactionsData, ar_prevHash, m_name, gc_DIFFICULTY, &g_FINISHED);
    worker.detach();

}



void Node::withdraw(float& ar_amount, std::vector<std::shared_ptr<Node>>& ar_nodesList)
{

    // withdraw money into node's wallet
    m_wallet += (g_VALUE * ar_amount);
    m_balance -= ar_amount;
    storeOurWallets(ar_nodesList); // saving
    storeOurCoins(ar_nodesList); // saving        

}



void Node::openMyStall(Marketplace& ar_marketplace, std::shared_ptr<Node>& arp_seller, bool& ar_type, double& ar_amount)
{
    
    // each node can only open at most one stall at a time
    if (m_stall != nullptr)
    {

        std::cout << "Pardon, marketplace rule number 1 : first and foremost, you can't own and operate more than a single stall at the same time | Close the current stall first" << std::endl;
        return;
        
    }

    m_stall = std::make_shared<Stall>(arp_seller, ar_type, ar_amount);

	// if (mp.market.size() % 10 == 0)
	// {
	// 	mp.market.reserve(10 + mp.market.size()); // 10 is not the amount of bytes, but the amount of elements
	// }
    ar_marketplace.m_market.push_back(std::move(m_stall));

    (ar_type == 0) ? arp_seller->m_balance -= ar_amount : arp_seller->m_wallet -= ar_amount;
    
    std::cout << "\nOpened a lovely stall in the marketplace. Yours is stall number " << ar_marketplace.m_market.size() << std::endl;

}



void Node::closeMyStall(Marketplace& ar_marketplace)
{

    // if node doesn't even have a stall
    if (m_stall == nullptr)
    {

        std::cout << "\nYou don't own a stall, maybe you used to!" << std::endl;
        return;

    }

    // using lambda function with std::find_if() to find position of this node's stall in the market
    std::vector<std::shared_ptr<Stall>>::iterator iter = std::find_if ( ar_marketplace.m_market.begin(), ar_marketplace.m_market.end(),

    [this](const std::shared_ptr<Stall> s) { return (s->mp_seller->m_name == m_name); }

    );

    // delete stall's data from memory
    auto index = (iter - ar_marketplace.m_market.begin()); // get index of found stall in market
    ar_marketplace.m_market[index].reset(); // delete from memory the stall which is pointed to by a shared_pointer
    ar_marketplace.m_market[index] = nullptr;

    // delete stall from market
    ar_marketplace.m_market.erase(iter);

    // finally
    std::cout << "\n--> Stall closed successfully!" << std::endl;

}



// visit to view stall, negotiate, make offers, and perform trades, or perhaps just leave the stall
void Node::visitStall(Blockchain& ar_blockchain, std::vector<std::shared_ptr<Node>>& ar_nodesList, Marketplace& ar_marketplace, uint64_t& ar_stallNumber)
{

    // initial checking to see if stall exist
    if (ar_stallNumber > ar_marketplace.m_market.size())
    {

        std::cout << "\nStall doesn't exist! Might have bene deleted not long ago!" << std::endl;
        return;

    }

    uint64_t index = ar_stallNumber - 1;
    char option;


    // sellers can't perform  trading with themself
    if (ar_marketplace.m_market[index]->mp_seller->m_name == m_name)
    {

        std::cout << "You can't trade with yourself you fishing prick!" << std::endl;
        return;

    }

    // first and foremost, show stall info
    std::cout << "stall number " << ar_stallNumber << " : " << std::endl;
    std::cout << "\t--> seller : " << ar_marketplace.m_market[index]->mp_seller->m_name << std::endl;

    std::string tmp = (ar_marketplace.m_market[index]->m_type == 0) ? " in exchange of money" : "[USD] in exchange of bitcoins";
    std::cout << "\t--> selling : " << ar_marketplace.m_market[index]->m_amount << tmp << std::endl;

    std::cout << "----------------------------------------------------------------\n";

    // decide whether to take the deal or leave
    std::cout << "seller (" << ar_marketplace.m_market[index]->mp_seller->m_name <<") : '(c)rop or (d)rop buddy?'\n>>> ";
    std::cin >> option; std::cin.ignore();

    if (option == 'c' || option == 'C')
    {
        
        #define focusedStall ar_marketplace.m_market[index] // alias to save writing time
    
        // 0: sell bitcoins for money | 1: sell money for bitcoins
        if (focusedStall->m_type == 0)
        {

            focusedStall->mp_seller->m_balance -= focusedStall->m_amount;
            focusedStall->mp_seller->m_wallet += (focusedStall->m_amount * g_SELL_VALUE);

            m_balance += focusedStall->m_amount;
            m_wallet -= (focusedStall->m_amount * g_BUY_VALUE);
            
            // data to be written to public ledger, trading is technically transferring
            // <from> <to> <amount>
            tmp = focusedStall->mp_seller->m_name + ' ' + m_name + ' ' + std::to_string(focusedStall->m_amount) + ' ' + std::to_string(time(0)) + '\n';

        }

        else
        {

            double excess = fmod(focusedStall->mp_seller->m_wallet, g_BUY_VALUE); // left-over money
            float tradableBitcoins = (focusedStall->m_amount / g_BUY_VALUE);

            focusedStall->mp_seller->m_wallet -= (focusedStall->m_amount - excess);
            focusedStall->mp_seller->m_balance += (tradableBitcoins);

            m_wallet += ( (focusedStall->m_amount / g_SELL_VALUE) * g_SELL_VALUE);
            m_balance -= tradableBitcoins;

            // data to be written to public ledger, trading is technically trasnfering
            // <from> <to> <amount>
            tmp =  m_name + ' ' + focusedStall->mp_seller->m_name + ' ' + std::to_string(focusedStall->m_amount) + ' ' + std::to_string(time(0)) + '\n';
        }

        //write to public ledger, trading is technically trasnfering
        if ( (ar_blockchain.m_tmpTransactionsData.length() + tmp.length()) > gc_MAX_TRANSACTION_DATA_SIZE_IN_BYTES)
        {

            ar_blockchain.openCompetition(tmp, ar_nodesList);

        }

        else
        {

            ar_blockchain.m_tmpTransactionsData += tmp;

        }

        
        ar_marketplace.m_market[index]->mp_seller->closeMyStall(ar_marketplace); // force the stall owner to close stall as trade complete; 
        storeOurCoins(ar_nodesList);        // saving
        storeOurWallets(ar_nodesList);     // saving

        std::cout << "\n--> Trading complete! Have a great day!" << std::endl;
        return;

    }

    else if (option == 'd' || option == 'D')
    {

        std::cout << "\n-->Understandable, have a great day!" << std::endl;
        return;

    }

    else 
    {
        std::cout << "Option invalid : " << option << std::endl;
        return;
    }


}



void Marketplace::showAllStalls()
{

    uint64_t i = 0;
    std::string tmp;
    
    std::cout << "----------------------------------------------------------------\n";
    for (const auto& s : m_market)
    {

        std::cout << "stall number " << i + 1 << " : " << std::endl;
        std::cout << "\t--> seller : " << s->mp_seller->m_name << std::endl;

        tmp = (s->m_type == 0) ? " bitcoins in exchange of money" : "[USD] in exchange of bitcoins";
        std::cout << "\t--> selling : " << s->m_amount << tmp << std::endl;

        std::cout << "----------------------------------------------------------------\n";

        ++i;

    }

};

/* end class methods */



int main()
{

    std::vector<std::shared_ptr<Node>> l_nodesList;   // a vector of <NODE> as a method for the nodes to interact with one another
    Blockchain                         l_blockchain;  // a unique | one-and-only <Blockchain> object of the whole program and is the life of it (kind of)
    Marketplace                        l_marketplace; // a unique | one-and-only <Marketplace> object of the whole program

    char option;                                      // these 2 vars are for 
    char option2;                                     // storing user's input queries
    
        
    std::string tmpName;                              // these 5 vars are for later use in the while loop  as temporary 
    float       tmpAmount;                            // input values
    double      tmpAmount2;                           //
    uint64_t    tmpAmount3;                           //
    short       tmpCount = 0;                         //
    bool flag = 1;                                    // flag to stop the while loop when the user query is 'q'

    // create new nodes
    for (uint64_t i = 0; i < gc_NODE_COUNT; i++)
    {

        tmpName = "node" + std::to_string(i+1);
        std::shared_ptr<Node> dummyPtr = std::make_shared<Node>(tmpName, 0, 0);

		// if (nodesList.size() % 10 == 0)
		// {
		// 	nodesList.reserve(10 + nodesList.size()); // 10 is not the amount of bytes, but the amount of elements
		// }
        l_nodesList.push_back(dummyPtr);

    }  
    
    // restore saved values from previous runtime
    checkCoinBucket(l_nodesList);
    whereOurWalletsAt(l_nodesList);

    // pick a random node for the player
    std::shared_ptr<Node> whoami = l_nodesList[random2(0, gc_NODE_COUNT - 1)];
    // whoami->balance = 100; // give player 100 bitcons to start transferring

    std::cout << "You are now controlling " << whoami->m_name << " | You can change if you wish to\nPress enter to continue to program...";
    std::cin.get();
    clrscr();


    // interactions
    while (flag)
    {

        std::cout << "Queries | (t)ransfer), (s)witch node, enter (m)arketplace, (p)rint information, (q)uit\n>>> ";
        scanf(" %c", &option);

        switch (option)
        {

            case 'p':
                std::cout << "Print mode selected" << std::endl << "\nWhat to print? | (n)ame, (b)alance, transaction (f)ee, (v)alues, (t)emp TD, (a)ll blocks, (w)allet\n>>> ";
                scanf(" %c", &option2);

                //
                switch (option2)
                {
                    
                    case 'a':
                        l_blockchain.showAllBlocks();
                        break;

                    case 'n':
                        std::cout << "\n --> Current node's name : " << whoami->m_name << std::endl;
                        break;

                    case 'b':
                        std::cout << "\n --> Current node's balance : " << whoami->m_balance << std::endl;
                        break;

                    case 'f':
                        std::cout << "\n --> Current transaction fee of bitcoin is : " << g_TRANSACTION_FEE << " [USD]" << std::endl;
                        break;
    
                    case 'v':
                        std::cout << "\n --> Current bitcoin's value is : " << std::endl;
                        printAllValues();
                        break;

                    case 't':
                        std::cout << "\n --> Current temporary ledger data : \n" << l_blockchain.m_tmpTransactionsData << std::endl;
                        break;

                    case 'w':
                        std::cout << "\n --> You have " << whoami->m_wallet << "[USD]" << std::endl;
                        break;

                    default:
                        std::cout << "\n --> Inavlid option : " << option2 << std::endl;
                        break;

                } 
                
                break;

            case 't':
                std::cout << "Enter the name of the receiver:\n>>> ";
                std::cin >> tmpName; std::cin.ignore();
                whoami->transferTo(l_blockchain, l_nodesList, tmpName, time(0));
                std::thread(storeOurCoins, l_nodesList).detach(); // dedicated thread working in background to write bitcoin records to database

                break;

            case 'm':
                while (1) // use loop so that player doesn't get out of marketplace after action
                {
                    std::string greet = (tmpCount == 0) ? "\nWelcome to the marketplace! | Do you intend to (s)ell, (b)uy, (v)iew all stalls, (c)lose your stall, or (l)eave?\n>>> " : "\nDo you intend to (s)ell, (b)uy, (v)iew all stalls?, (c)lose your stall, or (l)eave\n>>> ";
                    std::cout << greet;

                    std::cin >> option2; std::cin.ignore();
                    if (option2 == 's' || option2 == 'S')
                    {
                        bool tmp;
                        std::cout << "\nWill you (0) sell bitcoins in exchange of money of (1) sell money in exchange of bitcoins?\n>>> ";
                        std::cin >> tmp; std::cin.ignore();
                        std::cout << "\nEnter amount :\n>>> ";
                        std::cin >> tmpAmount2; std::cin.ignore();
                        whoami->openMyStall(l_marketplace, whoami, tmp, tmpAmount2);

                    }

                    else if (option2 == 'b' || option2 == 'B')
                    {

                        std::cout << "Awesome! These stalls are currently available for trading :\n";
                        l_marketplace.showAllStalls();
                        std::cout << "\nWhich one seems appealing to you? | Enter stall's number to visit stall\n>>> ";
                        std::cin >> tmpAmount3; std::cin.ignore();
                        whoami->visitStall(l_blockchain, l_nodesList, l_marketplace, tmpAmount3);

                    }

                    else if (option2 == 'v' || option2 == 'V')
                        l_marketplace.showAllStalls();

                    else if (option2 == 'c' || option2 == 'C')
                        whoami->closeMyStall(l_marketplace);

                    else if (option2 == 'l' || option2 == 'L' || option2 == 'q' || option2 == 'Q')
                        break;

                    else
                        std::cout << "Invalid option : " << option2 << std::endl;


                    tmpCount = 1; // avoid node entering marketplace so many times that it crash the program
                }
                tmpCount = 0; //reset

                break;

            case 'w':                
                std::cout << "Enter in amount of bitcoins you want to withdraw?\n>>> ";
                std::cin >> tmpAmount;

                whoami->withdraw(tmpAmount, l_nodesList);
                break;
             
            case 's':
                std::cout << "Enter the name of the node you want to control:\n>>> ";
                std::cin >> tmpName; std::cin.ignore();

                if (tmpName == whoami->m_name)
                {

                    std::cout << " ->> You are already controlling '" <<tmpName << "' | Sigh... I know ADHD is hard for you.\n" << std::endl;
                    break;
                    
                }

                for (auto n : l_nodesList)
                {

                    if (tmpName == n->m_name)
                    {

                        whoami = n;
                        
                    } 

                }                

                break;

            case 'q':
                std::cout << "\n--> Terminating!" << std::endl; // break out of switch - case statement
                flag = 0;
                break;
            
            case 'c':
                clrscr();
                break;

            default:
                std::cout << "Invalid option : " << option << std::endl;
                break;

        }

    }

    // memory cleaning
    l_nodesList.clear();

    // stopping the fetcher
    g_FINISHED = 1;
    g_subsystem.join();

    return 0;
    
}