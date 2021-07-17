#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <iterator>
#include <memory>
#include <thread>
#include <chrono>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <openssl/sha.h>
#include "./headers/timeutils.hpp"
#include "./headers/coinutils.hpp"
#include "./headers/Node.hpp"
#include "./headers/Blockchain.hpp"
#include "./headers/Marketplace.hpp"



#define NODE_COUNT 15
#define MAX_TRANSACTION_SIZE_IN_BYTES 128 // 128Kbs of transactions data
#define REWARD 6.25
#define TOTAL = 21000000 
#define clrscr() std::cout << "\e[1;1H\e[2J"



// global variables
const uint16_t DIFFICULTY = 1000;
const uint16_t INITIAL_AMOUNT = NODE_COUNT;
float TRANSACTION_FEE;
float VALUE;
float BUY_VALUE;
float SELL_VALUE;
std::ofstream logS("./.log");
bool FINISHED = 0;



// determined threads to update bitcoin transaction fee and values every 30 seconds
std::thread subsystem(update, &TRANSACTION_FEE, &VALUE, &BUY_VALUE, &SELL_VALUE, &FINISHED);
// std::thread subSystem2(startNodejsProgram);



// judge
std::string whoisthewinner(std::string& correctHash)
{

    std::ifstream infile("./assets/arena");
    std::string line, tmpHash, winner;

    while (std::getline(infile, line))
    {

        std::istringstream ss(line);
        ss >> tmpHash;

        if (tmpHash == correctHash)
        {

            FINISHED = 1;
            ss >> winner;
            break;

        }
        
    }

    infile.close();
    return winner;
    
}



/* useful fucntions */

bool whereOurCoinsAt(std::vector<Node*>& nl)
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
        
        nl[i]->balance = std::stof(line);
        ++i;

    }

    filein.close();   
    return extremelyPoor;

}



void storeOurCoins(std::vector<Node*> nl)
{

    std::ofstream outfile("./assets/.tmpYourCoinsAreHere");

    if (!outfile.is_open())
    {

        std::cout << "\n ->> ERROR! DO YOU HAVE PROPER READ / WRITE PERMISSION FOR THIS DIRECTORY" << std::endl;
        return;

    }

    for (Node* &n : nl)    
    {

        outfile << n->balance << std::endl;
        
    }

    std::remove("./assets/yourCoinsAreHere");
    std::rename("./assets/.tmpYourCoinsAreHere", "./assets/yourCoinsAreHere");

    outfile.close();

}



void checkCoinBucket(std::vector<Node*>& nl)
{
    std::ifstream filein;
    std::string line;
    filein.open("./assets/yourCoinsAreHere");

    if (!filein.is_open()) 
    {

        std::cout << "ERROR! DO YOU HAVE PROPER READ / WRITE PERMISSION FOR THIS DIRECTORY?" << std::endl;
        return;

    }

    if (fileIsEmpty(filein) || whereOurCoinsAt(nl) == 1) // meaning, bool extremelyPoor = 1
    {
        
        std::cout << "\n->>MONEY NOT FOUND! DETECTED POVERTY INSTEAD!" << std::endl;
        std::cout << "->>To fix poverty, I'm giving out a few coins to a random lucky person in many of you" << std::endl;
        nl[random(0, NODE_COUNT-1)]->balance = INITIAL_AMOUNT;
        storeOurCoins(nl); // save coins of the lucky person

    }

    else
    {

        std::cout << "\n->>MONEY FOUND! BE PATIENT, I'M WORKING HARD TO GIVE EVERYONE HIS / HER / THEIR BELOVED COINS BACK" << std::endl;

    }

    filein.close();

}



void whereOurWalletsAt(std::vector<Node*>& nl)
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
        ss >> tmp; nl[i]->wallet = std::stod(tmp);

        ++i;

    }

    filein.close();

}



void storeOurWallets(std::vector<Node*>& nl)
{

    std::ofstream fileout("./assets/.tmpYourWalletsAreHere");

    for (const auto& n : nl)
    {

        fileout << n->wallet << std::endl;

    }

    std::remove("./assets/yourWalletsAreHere");
    std::rename("./assets/.tmpYourWalletsAreHere", "./assets/yourWalletsAreHere");
    fileout.close();

}

/* end useful functions */



/* class methods */

// constructor
Block::Block(std::string& td)
: transactionsData(td)
{

    correctHash = calcHash(transactionsData, DIFFICULTY);
    logS << "Block created successfully!" << std::endl;
    
}



// empty constructor
Block::Block()
{

    logS << "Block created successfully!" << std::endl;

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
            
    logS << "Blockchain crated successfully!" << std::endl;

}



//destructor
Blockchain::~Blockchain()
{
    
    this->chain.clear();
    logS << "Blockchain freed from memory successfully!" << std::endl;

}



// constructor
Stall::Stall(Node* &s, bool& t, double& a)
: seller(s), type(t), amount(a)
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

    for (const auto& s : market)    
        delete s;
    
    this->market.clear();
    logS << "Marketplace deleted successfully" << std::endl;

}



//methods
void Blockchain::openCompetition(std::string& leftOut, std::vector<Node*>& nl)
{

    ++last_index; // first and foremost

    std::unique_ptr<Block> dummyPtr(new Block(tmpTransactionsData));
    chain.push_back(std::move(dummyPtr));
	chain.shrink_to_fit(); // shrink the capacity of vector to its true size to avoid wasting memory
    tmpTransactionsData.clear(); //reset tmpTransactionsData
    tmpTransactionsData += leftOut;

    // signal nodes to start competing in order to complete the creation of the newly initialized block
    for (auto& n : nl)
    {

        n->startMining(chain[last_index]->transactionsData);
        
    }

    std::cout << "\n --> All nodes have started mining\nWaiting for the correct hash to be generated" << std::endl;
    std::string winner;

    while (FINISHED == 0)
    {
        
        winner = whoisthewinner(chain[last_index]->correctHash);

    } 
        
    std::cout << "\n --> The correct hash has been generated by node : '" << winner << "' " << " | Winner will be rewarded with " << REWARD << " (reward) + " << TRANSACTION_FEE << " (transactions fee)  bitcoins" << std::endl;

    // sending reward
    for (auto& n : nl)
    {

        if (n->name == winner)
        {

            n->balance += (REWARD + TRANSACTION_FEE);
            
        }

    }
    
    std::cout << "\n*Signaling miners to stop\n" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1500)); // waiting for miners to realize the competition is over 
    FINISHED = 0; // reset the boolean state for later use

    syncDatabase(1); // write changes to database

}



void Blockchain::syncDatabase(const uint16_t& mode)
{

    // mode 0 | read from database
    if (mode == 0)
    {

        std::ifstream filein("./assets/database");
        std::string line;
        std::string tmp1; // responsible for collecting the hash of the block
        std::string tmp2; // responsible for collecting the transactions data of the block
        
        while (filein.peek() != EOF)
        {
            
            ++last_index;
            
            tmp2 = ""; //reset

            // create empty block first then fullfill its data later

            // read in the first line, which is the hash of the block
            std::getline(filein, line);
            tmp1 = line;

            // read in the next 3 lines, which is the transactions datas
            std::getline(filein, line);
            tmp2 += (line + '\n');

            std::getline(filein, line);
            tmp2 += (line + '\n');

            std::getline(filein, line);
            tmp2 += (line + '\n');

            // skip the remaining blank line
            std::getline(filein, line);

            // create an empty block
            std::unique_ptr<Block> dummyPtr(new Block());
            this->chain.push_back(std::move(dummyPtr));
			chain.shrink_to_fit(); // shrink the capacity of vector to its true size to avoid wasting memory

            // fullfill datas of block
            this->chain[last_index]->correctHash = tmp1;
            this->chain[last_index]->transactionsData = tmp2;

        }

        filein.close();
    }

    // mode 1 | write to database
    else if (mode == 1)
    {
        
        std::ofstream fileout("./assets/database", std::ios::app);
        
        if (!fileout.is_open())
        {

            std::cout << "\nERROR! DO YOU HAVE THE RIGHT READ / WRITE PERMISSION FOR DATABASE" << std::endl;
            return;
            
        }

        fileout << this->chain[last_index]->correctHash << std::endl << this->chain[last_index]->transactionsData << std::endl;
        
        fileout.close();

    }

}



void Blockchain::showAllBlocks()
{

    std::cout << "\n----------------------------------------------------------------\n";
    
    for (uint64_t i = 0; i < this->chain.size(); i++)
    {

        std::cout << "Block number " << i+1 << ":" << std::endl;
        std::cout << "\t--> Hash of block : " << this->chain[i]->correctHash << std::endl;
        std::cout << "\t--> Transactions data :\n" << this->chain[i]->transactionsData << std::endl;
        std::cout << "----------------------------------------------------------------\n";

    }

}



//constructor
Node::Node(std::string& n, const float& b, const double& w, const std::string& curr)
: name(n), balance(b), wallet(w), mystall(NULL)
{

    logS << "Node created successfully!" << std::endl;

}



// destructor
Node::~Node()
{
    
    logS << "Node deleted successfully!" << std::endl;
    
}



void Node::transferTo(Blockchain& blc, std::vector<Node*>& nl, std::string& receiver, const uint32_t& timestamp)
{
    
    // initial checking step
    if (receiver == this->name)
    {
        
        std::cout << "\n->> Can't transfer money to yourserlf you fishing prick!" << std::endl;
        return;

    }

    // transfer money
    float amount;
    bool found = 0;

    for (uint64_t i = 0; i < NODE_COUNT; i++)
    {

        if (nl[i]->name == receiver)
            found = 1;
            

        if (i == (NODE_COUNT - 1) && found == 0)
        {

            std::cout << "\n->> Node doesn't exit" << std::endl;
            return;

        }

        if (found)
        {
            
            std::cout << "\nEnter the amount of bitcoins to transfer :\n>>> ";
            std::cin >> amount;

            // if not enough bitcoins to transfer
            if (amount > this->balance)
            {

                char option;
                std::cout << "\n->> Your total account balance isn't sufficient for this transfer : " << this->balance << " / " << amount << std::endl;
                std::cout << "Do you want to apply an amount of debt to your account balance? | (Y)es, (N)o\n>>> ";
                std::cin >> option; std::cin.ignore();
                
                if (option == 'y' || option == 'Y')
                {

                    std::cout << "\nAre you sure ? | (Y)es, (N)o\n>>> ";
                    std::cin >> option; std::cin.ignore();

                    if (option == 'y' || option == 'Y')
                    {

                        std::cout << "\n*Sure!" << std::endl;
                        nl[i]->balance += amount;
                        this->balance -= amount;
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

                nl[i]->balance += amount;
                this->balance -= amount;
                break;

            }
            

        }


    }

    // write to public ledger
    std::string tmp = this->name + ' ' + receiver + ' ' + std::to_string(amount) + ' ' + std::to_string(timestamp) + '\n';

    if ( (blc.tmpTransactionsData.length() + tmp.length()) > MAX_TRANSACTION_SIZE_IN_BYTES)
    {

        blc.openCompetition(tmp, nl);

    }

     
    else
    {
        blc.tmpTransactionsData += tmp;
    }

}

void Node::startMining(std::string& td)
{
    
    std::thread worker(guessHash, td, this->name, DIFFICULTY, &FINISHED);
    worker.detach();

}



void Node::withdraw(float& amount, std::vector<Node*>& nl)
{

    // withdraw money into node's wallet
    this->wallet += (VALUE * amount);
    this->balance -= amount;
    storeOurWallets(nl); // saving
    storeOurCoins(nl); // saving        

}



void Node::openMyStall(Marketplace& mp, Node*& s, bool& t, double& a)
{
    
    // each node can only open at most one stall at a time
    if (this->mystall != nullptr)
    {

        std::cout << "Pardon, marketplace rule number 1 : first and foremost, you can't own and operate more than a single stall at the same time | Close the current stall first" << std::endl;
        return;
        
    }

    this->mystall = new Stall(s, t, a);
    mp.market.push_back(std::move(mystall));
	mp.market.shrink_to_fit(); // shrink the capacity of vector to its true size to avoid wasting memory
    (t == 0) ? s->balance -= a : s->wallet -= a;
    
    std::cout << "\nOpened a lovely stall in the marketplace. Yours is stall number " << mp.market.size() << std::endl;

}



void Node::closeMyStall(Marketplace& mp)
{

    // if node doesn't even have a stall
    if (this->mystall == nullptr)
    {

        std::cout << "\nYou don't own a stall, maybe you used to!" << std::endl;
        return;

    }

    // using lambda function with std::find_if() to find position of this node's stall in the market
    std::vector<Stall*>::iterator iter = std::find_if(mp.market.begin(), mp.market.end(), 
    [this](const Stall* s) { return (s->seller->name == this->name); }
    );

    // delete stall's data from memory
    auto index = (iter - mp.market.begin()); // get index of found stall in market
    delete mp.market[index];
    mp.market[index] = nullptr;

    // delete stall from market
    mp.market.erase(iter);

    // finally
    std::cout << "\n--> Stall closed successfully!" << std::endl;

}



// visit to view stall, negotiate, make offers, and perform trades, or perhaps just leave the stall
void Node::visitStall(Blockchain& blc, std::vector<Node*>& nl, Marketplace& mp, uint64_t& stallNum)
{

    // initial checking to see if stall exist
    if (stallNum > mp.market.size())
    {

        std::cout << "\nStall doesn't exist! Might have bene deleted not long ago!" << std::endl;
        return;

    }

    uint64_t index = stallNum - 1;
    char option;


    // sellers can't perform  trading with themself
    if (mp.market[index]->seller->name == this->name)
    {

        std::cout << "You can't trade with yourself you fishing prick!" << std::endl;
        return;

    }

    // first and foremost, show stall info
    std::cout << "stall number " << stallNum << " : " << std::endl;
    std::cout << "\t--> seller : " << mp.market[index]->seller->name << std::endl;

    std::string tmp = (mp.market[index]->type == 0) ? " in exchange of money" : "[USD] in exchange of bitcoins";
    std::cout << "\t--> selling : " << mp.market[index]->amount << tmp << std::endl;

    std::cout << "----------------------------------------------------------------\n";

    // decide whether to take the deal or leave
    std::cout << "seller (" << mp.market[index]->seller->name <<") : '(c)rop or (d)rop buddy?'\n>>> ";
    std::cin >> option; std::cin.ignore();

    if (option == 'c' || option == 'C')
    {
        
        #define focusedStall mp.market[index] // alias to save writing time
    
        // 0: sell bitcoins for money | 1: sell money for bitcoins
        if (focusedStall->type == 0)
        {

            focusedStall->seller->balance -= focusedStall->amount;
            focusedStall->seller->wallet += (focusedStall->amount * SELL_VALUE);

            this->balance += focusedStall->amount;
            this->wallet -= (focusedStall->amount * BUY_VALUE);
            
            // data to be written to public ledger, trading is technically transferring
            // <from> <to> <amount>
            tmp = focusedStall->seller->name + ' ' + this->name + ' ' + std::to_string(focusedStall->amount) + ' ' + std::to_string(time(0)) + '\n';

        }

        else
        {

            double excess = fmod(focusedStall->seller->wallet, BUY_VALUE); // left-over money
            float tradableBitcoins = (focusedStall->amount / BUY_VALUE);

            focusedStall->seller->wallet -= (focusedStall->amount - excess);
            focusedStall->seller->balance += (tradableBitcoins);

            this->wallet += ( (focusedStall->amount / SELL_VALUE) * SELL_VALUE);
            this->balance -= tradableBitcoins;

            // data to be written to public ledger, trading is technically trasnfering
            // <from> <to> <amount>
            tmp =  this->name + ' ' + focusedStall->seller->name + ' ' + std::to_string(focusedStall->amount) + ' ' + std::to_string(time(0)) + '\n';
        }

        //write to public ledger, trading is technically trasnfering
        if ( (blc.tmpTransactionsData.length() + tmp.length()) > MAX_TRANSACTION_SIZE_IN_BYTES)
        {

            blc.openCompetition(tmp, nl);

        }

        else
        {

            blc.tmpTransactionsData += tmp;

        }

        
        mp.market[index]->seller->closeMyStall(mp); // force the stall owner to close stall as trade complete; 
        storeOurCoins(nl);        // saving
        storeOurWallets(nl);     // saving

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
    for (Stall*& s : market)
    {

        std::cout << "stall number " << i + 1 << " : " << std::endl;
        std::cout << "\t--> seller : " << s->seller->name << std::endl;

        tmp = (s->type == 0) ? " bitcoins in exchange of money" : "[USD] in exchange of bitcoins";
        std::cout << "\t--> selling : " << s->amount << tmp << std::endl;

        std::cout << "----------------------------------------------------------------\n";

        ++i;

    }

};

/* end class methods */



int main()
{
    // std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // pause the main thread for 1s to wait for values to be updated;
   
    std::vector<Node*> nodesList; // a vector of <NODE> as a method for the nodes to interact with one another
    Blockchain mychain;             // a unique | one-and-only <Blockchain> object of the whole program and is the life of it (kind of)
    Marketplace mymarketplace;       // a unique | one-and-only <Marketplace> object of the whole program

    char option;              // these 2 vars are for 
    char option2;             // storing user's input queries
    
        
    std::string tmpName;      // these 5 vars are for later use in the while loop  as temporary 
    float tmpAmount;         // input values
    double tmpAmount2;      //
    uint64_t tmpAmount3;   //
    short tmpCount = 0;   //

    bool flag = 1; // flag to stop the while loop when the user query is 'q'

    // create new nodes
    for (uint64_t i = 0; i < NODE_COUNT; i++)
    {

        tmpName = "node" + std::to_string(i+1);
        Node* dummyPtr = new Node(tmpName, 0, 0, "NULL");
        nodesList.push_back(dummyPtr);
		nodesList.shrink_to_fit(); // shrink vector's capacity to its true size to avoid wasting memory

    }  
    
    // restore saved values from previous runtime
    checkCoinBucket(nodesList);
    whereOurWalletsAt(nodesList);

    // pick a random node for the player
    Node* whoami = nodesList[random2(0, NODE_COUNT - 1)];
    // whoami->balance = 100; // give player 100 bitcons to start transferring

    std::cout << "You are now controlling " << whoami->name << " | You can change if you wish to\nPress enter to continue to program...";
    std::cin.get();
    clrscr();


    // interactions
    while (flag)
    {

        std::cout << "Queries | (t)ransfer), (s)witch node, enter (m)arketplace, (q)uit\n>>> ";
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
                        mychain.showAllBlocks();
                        break;

                    case 'n':
                        std::cout << "\n --> Current node's name : " << whoami->name << std::endl;
                        break;

                    case 'b':
                        std::cout << "\n --> Current node's balance : " << whoami->balance << std::endl;
                        break;

                    case 'f':
                        std::cout << "\n --> Current transaction fee of bitcoin is : " << TRANSACTION_FEE << " [USD]" << std::endl;
                        break;
    
                    case 'v':
                        std::cout << "\n --> Current bitcoin's value is : " << std::endl;
                        printAllValues();
                        break;

                    case 't':
                        std::cout << "\n --> Current temporary ledger data : \n" << mychain.tmpTransactionsData << std::endl;
                        break;

                    case 'w':
                        std::cout << "\n --> You have " << whoami->wallet << "[USD]" << std::endl;
                        break;

                    default:
                        std::cout << "\n --> Inavlid option : " << option2 << std::endl;
                        break;

                } 
                
                break;

            case 't':
                std::cout << "Enter the name of the receiver:\n>>> ";
                std::cin >> tmpName; std::cin.ignore();
                whoami->transferTo(mychain, nodesList, tmpName, time(0));
                std::thread(storeOurCoins, nodesList).detach(); // dedicated thread working in background to write bitcoin records to database

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
                        whoami->openMyStall(mymarketplace, whoami, tmp, tmpAmount2);

                    }

                    else if (option2 == 'b' || option2 == 'B')
                    {

                        std::cout << "Awesome! These stalls are currently available for trading :\n";
                        mymarketplace.showAllStalls();
                        std::cout << "\nWhich one seems appealing to you? | Enter stall's number to visit stall\n>>> ";
                        std::cin >> tmpAmount3; std::cin.ignore();
                        whoami->visitStall(mychain, nodesList, mymarketplace, tmpAmount3);

                    }

                    else if (option2 == 'v' || option2 == 'V')
                        mymarketplace.showAllStalls();

                    else if (option2 == 'c' || option2 == 'C')
                        whoami->closeMyStall(mymarketplace);

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

                whoami->withdraw(tmpAmount, nodesList);
                break;
             
            case 's':
                std::cout << "Enter the name of the node you want to control:\n>>> ";
                std::cin >> tmpName; std::cin.ignore();

                if (tmpName == whoami->name)
                {

                    std::cout << " ->> You are already controlling '" <<tmpName << "' | Sigh... I know ADHD is hard for you.\n" << std::endl;
                    break;
                    
                }

                for (auto n : nodesList)
                {

                    if (tmpName == n->name)
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
    for (Node* &node : nodesList)
    {

        delete node;

    } nodesList.clear();

    // kill nodejs fetcher
    FINISHED = 1;
    subsystem.join();

    return 0;
    
}
