#include <iostream> // Biblioteca padrão
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <cmath>

#include "solution.h"
#include "data.h"
#include "random.h"

bool compareByDist(const vert &a, const vert &b)
{
    return a.dist < b.dist;
}

Solution::Solution() : rotas(), vertSobrando() {}

void Solution::exibirSolucao()
{
    for(rota rot : rotas)
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

    std::cout << std::endl;
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

// Função que consultará qual a rota de determinado vértice
int Solution::consultaRota(int vertice)
{
    for(int i = 0; i < rotas.size(); i++)
    {
        rota &r = rotas[i];

        for(int j = 1; j < r.vertices.size() - 1; j++)
        {
            if(vertice == r.vertices[j])
                return i;
        }
    }

    return -1;
}

// Função que consulturá a posição de determinado vértice numa rota
int Solution::consultaPos(int vertice, int rota)
{
    auto &rot = rotas[rota].vertices;

    for(int i = 1; i < rot.size() - 1; i++)
    {
        if(rot[i] == vertice)
            return i;
    }

    return -1;
}

Solution Solution::ruin(Solution &sC)
{   
    Data & data = Data::getInstance();
    auto & matrizAdj = data.matrizAdj;
    auto & vecDmd = data.arrayDmds;

    // A solução que será retornada
    Solution s = sC;
    auto & rots = s.rotas;

    // Parâmetros definidos pelo programador
    double alpha=0.01, lMax=10, cMedia=10;

    // Calculando a média do tamanho das rotas de cada rota da solução
    double media;

    for(rota &rot : rots)
    {
        // O "-2" está aqui para retirar o depósito
        media += rot.vertices.size() - 2;
    }

    media /= rots.size();

    // Definindo as variáveis necessárias para o cálculo
    double cardMax = std::min(lMax, media);
    double qtdMaxStr = ((4.0 * cMedia) / (1 + cardMax)) - 1;
    int qtdStr = std::floor(Random::getReal(1, qtdMaxStr + 1));

    // Escolhendo a rota em que o elemento será escolhido
    int indexRota = Random::getInt(0, rots.size() - 1);

    // Escolhendo o C seed, onde ele vai desconsiderar o depósito
    int indexElemento = Random::getInt(1, rots[indexRota].vertices.size() - 2);

    // Definindo o consumidor que será o C seed
    int elementoC = rots[indexRota].vertices[indexElemento];

    std::unordered_set<int> rotasAlteradas;
    std::vector<vert> eleAdj;

    // Preenchendo o vetor de adjacência
    for(int i = 1; i <= data.dim; i++)
    {
        eleAdj.emplace_back(vert(i, matrizAdj[elementoC][i]));
    }

    std::sort(eleAdj.begin(), eleAdj.end(), compareByDist);

    for(vert &vertice : eleAdj)
    {   
        if(rotasAlteradas.size() >= qtdStr)
            break;

        int rotaVert = s.consultaRota(vertice.c);

        if(s.vertSobrando.find(vertice.c) != s.vertSobrando.end() || rotasAlteradas.find(rotaVert) != rotasAlteradas.end())
            continue;

        std::vector<int> &rotMomento = rots[rotaVert].vertices;

        // Calculando o tamanho do tour
        int cardMaxTour = std::min((double) rotMomento.size() - 2, cardMax);
        int cardTour = std::floor(Random::getReal(1, cardMaxTour + 1));

        // Armazenando a posição do elemento em relação a rota que ele está
        int indexVert = s.consultaPos(vertice.c, rotaVert);

        std::cout << "Elemento que será excluído: " << vertice.c << std::endl;
        std::cout << "Posição dele na rota: " << indexVert << std::endl;

        // Deletando os elementos em si
        for(int i = 1; i <= cardTour; i++)
        {   
            // Decidindo o elemento que será deletado
            int indexDel = indexVert + Random::getInt((-1 * (1 % i)), 0);

            std::cout << (-1 * (1 % i)) << std::endl;

            // Salvando-o no set de vértices que estão sobrando
            s.vertSobrando.insert(rotMomento[indexDel]);

            // Diminuindo a demanda desse elemento da rota
            s.rotas[rotaVert].capAtual -= vecDmd[rotMomento[indexDel]];

            // Excluindo a distância dele da rota
            s.rotas[rotaVert].custo -= matrizAdj[rotMomento[indexDel - 1]][rotMomento[indexDel]] 
                                     + matrizAdj[rotMomento[indexDel]][rotMomento[indexDel + 1]] 
                                     - matrizAdj[rotMomento[indexDel - 1]][rotMomento[indexDel + 1]];

            // Deletando-o da rota
            rotMomento.erase(rotMomento.begin() + indexDel);
        }

        // Inserindo a rota deletada no set de rotas
        rotasAlteradas.insert(rotaVert);
    }

    return s;
}

Solution Solution::recreate(Solution &sC)
{
    Solution s = sC;

    return s;
}