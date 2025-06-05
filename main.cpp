#include <iostream>

#include "lib/data.h"
#include "lib/solution.h"
#include "lib/random.h"
#include "lib/ruinAndRecreate.h"
#include "lib/meta.h"

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

    sol = Solution::criarSolucaoInicial();

    for(int i = 0; i < 1; i++)
    {
        fleetSizeMinimization(sBest, sol, 1000000);
    }

    sBest.exibirSolucao();

    return 0;
}