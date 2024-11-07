#include "learning_gem5/part2/hello_object.hh"

gem5::HelloObject::HelloObject(const HelloObjectParams &params)
    : SimObject(params)
{
    std::cout << "Hello Object constructed!" << std::endl;
}
