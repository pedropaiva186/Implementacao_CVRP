#include <iostream>

#include "lib/data.h"

int main() {

    char *args[2];

    // Local onde colocaremos o tipo do problema e a instância que utilizaremos
    args[0] = (char *) "CVRP";
    args[1] = (char *) "instances/X-n101-k25.vrp";

    Data::readData(2, args);

    Data &data = Data::getInstance();

    std::cout << data.dim << std::endl;
    std::cout << data.capacidade << std::endl;

    return 0;
}