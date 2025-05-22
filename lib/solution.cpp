#include <iostream> // Biblioteca padrão
#include <vector>

#include "solution.h"
#include "data.h"

Solution::Solution() : rotas(), vertSobrando() {}

void Solution::exibirSolucao()
{
    for(rota rot : rotas)
    {   
        std::cout << "Rota: ";

        for(int i : rot.vertices)
        {
            std::cout << i << " ";
        }

        std::cout << std::endl << "Capacidade atual: " << rot.capAtual << std::endl;
        std::cout << "Custo: " << rot.custo << std::endl;
    }
}

Solution Solution::criarSolucaoInicial()
{
    Data & data = Data::getInstance();
    auto & arrayDms = data.arrayDmds;
    auto & matrizAdj = data.matrizAdj;

    // Criando a solução que será retornada
    Solution sol;

    for(int i = 1; i <= data.dim; i++)
    {
        sol.rotas.emplace_back(rota({1, i, 1}, arrayDms[i], 2 * matrizAdj[i][1]));
    }

    return sol;
}