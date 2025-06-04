#include <iostream>
#include <random>

#include "lib/data.h"
#include "lib/solution.h"
#include "lib/random.h"
#include "lib/ruinAndRecreate.h"

int main() {
    std::random_device rd;
    Random::randomize(rd());

    char *args[2];

    // Local onde colocaremos o tipo do problema e a instância que utilizaremos
    args[0] = (char *) "CVRP";
    args[1] = (char *) "instances/X-n101-k25.vrp";

    Data::readData(2, args);

    Data &data = Data::getInstance();

    Solution sol, sBest;

    sBest = Solution::criarSolucaoInicial();

    for(int i = 0; i < 30000000; i++)
    {
        ruinAndRecreate(sol, sBest);
    }

    sBest.exibirSolucao();

    return 0;
}