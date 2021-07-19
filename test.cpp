#include <iostream>

class Person
{
  public:
    std::string name;
    Person(std::string& n)
    : name(n)
    {
        // nothing here
    }
    
};

int main()
{
    std::string test;
    Person* arr[3];
    int index = 0;

    try
    {
        arr[index]->name = arr[index - 1]->name;
    }
    catch(const std::exception& e)
    {
        goto IF_FAIL;
    }
    

    IF_FAIL:
        arr[index]->name = "ANONYMOUS";


    std::cout << arr[index]->name;

    return 0;
}
