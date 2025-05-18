#include <iostream>

#include "lib/data.h"

int main() {

    char *args[2];

    // Local onde colocaremos o tipo do problema e a instância que utilizaremos
    args[0] = (char *) "CVRP";
    args[1] = (char *) "instances/X-n1001-k43.vrp";

    Data::readData(2, args);

    Data &data = Data::getInstance();

    return 0;
}