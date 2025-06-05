#include "solution.h"
#include "data.h"

Solution::Solution() : rotas(), vertSobrando(), posCRotas(), posCVertices() {}

Solution::Solution(std::vector<Rota> &rots, std::unordered_set<int> &vertSobr, std::vector<int> &pCRot, std::vector<int> &pCVert) : 
                   rotas(rots), vertSobrando(vertSobr), posCRotas(pCRot), posCVertices(pCVert) {}

void Solution::exibirSolucao()
{
    for(Rota rot : rotas)
    {   
        std::cout << "Rota: ";

        for(int &i : rot.vertices)
        {
            std::cout << i << " ";
        }

        std::cout << std::endl << "Capacidade atual: " << rot.capAtual << std::endl;
        std::cout << "Custo: " << rot.custo << std::endl;
    }

    std::cout << "Vértices sobrando: ";

    for(auto &v : vertSobrando)
    {
        std::cout << v << " ";
    }

    std::cout << std::endl << "Número de rotas: " << rotas.size() << std::endl;
}

Solution Solution::criarSolucaoInicial()
{
    Data & data = Data::getInstance();

    Solution sol;
    sol.posCRotas = std::vector<int>(data.dim + 1);
    sol.posCVertices = std::vector<int>(data.dim + 1);

    for(int i = 2; i <= data.dim; i++)
    {
        sol.rotas.emplace_back(Rota({1, i, 1}, data.arrayDmds[i], 2 * data.matrizAdj[i][1]));
        sol.posCRotas[i] = i - 2;
        sol.posCVertices[i] = 1;
    }

    return sol;
}