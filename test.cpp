#include <iostream>
#include <cstring>
#include <string>
#include "coinutils.hpp"
#include <openssl/sha.h>

int main()
{
    char tD[1000] = "ducminh ducminh2 19\nducminh2 ducminh 1900000\nducminh1 ducminh3 1900000";
    std::cout << "Hash = " << calcHash(tD) << std::endl;
}