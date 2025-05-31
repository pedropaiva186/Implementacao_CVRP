#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <cmath>
#include <queue>

#include "solution.h"
#include "data.h"
#include "random.h"

// Structs para o uso do priority queue

struct compareByRandom
{
    bool operator()(const int &a, const int &b) {return Random::getReal(0, 1) < Random::getReal(0, 1);}
};

struct compareByDmd
{   
    int* arrayDmd;

    compareByDmd(int *arr) : arrayDmd(arr) {};

    bool operator()(const int &a, const int &b) {return arrayDmd[a] > arrayDmd[b];}
};

struct compareByFar
{
    double **matrizAdj;

    compareByFar(double **matriz) : matrizAdj(matriz) {};

    bool operator()(const int &a, const int &b) {return matrizAdj[1][a] < matrizAdj[1][b];}
};

struct compareByClose
{   
    double **matrizAdj;

    compareByClose(double **matriz) : matrizAdj(matriz) {};

    bool operator()(const int &a, const int &b) {return matrizAdj[1][a] > matrizAdj[1][b];}
};

template <typename Comparator>

std::priority_queue<int>& gerarPrioQueue(Comparator comp)
{
    std::priority_queue<int, std::vector<int>, Comparator> pQ(comp);

    return &pq;
}



Solution::Solution() : rotas(), vertSobrando(), posCRotas(), posCVertices() {}

Solution::Solution(std::vector<rota> &rots, std::unordered_set<int> &vertSobr, std::vector<int> &pCRot, std::vector<int> &pCVert) : 
                   rotas(rots), vertSobrando(vertSobr), posCRotas(pCRot), posCVertices(pCVert) {}

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

    Solution sol;
    sol.posCRotas = std::vector<int>(data.dim + 1);
    sol.posCVertices = std::vector<int>(data.dim + 1);

    for(int i = 2; i <= data.dim; i++)
    {
        sol.rotas.emplace_back(rota({1, i, 1}, arrayDms[i], 2 * matrizAdj[i][1]));
        sol.posCRotas[i] = i - 2;
        sol.posCVertices[i] = 1;
    }

    return sol;
}

void Solution::ruin(Solution &sC, Solution &s)
{   
    Data & data = Data::getInstance();
    auto & matrizAdj = data.matrizAdj;
    auto & vecDmd = data.arrayDmds;

    s = sC;
    auto &rots = s.rotas;

    double alpha=0.01, lMax=10, cMedia=10;

    double media = 0;

    for(rota &rot : rots) media += (double) rot.vertices.size() - 2;

    media /= (double) rots.size();

    double cardMax = std::min(lMax, media);
    double qtdMaxStr = ((4.0 * cMedia) / (1 + cardMax)) - 1;
    int qtdStr = std::floor(Random::getReal(1, qtdMaxStr + 1));

    int elementoC = Random::getInt(2, data.dim);

    std::unordered_set<int> rotasAlteradas;

    int rotasAlt = 0;

    for(vert &vertice : data.listasAdj[elementoC])
    {   
        if(rotasAlt >= qtdStr)
            break;

        int rotaVert = s.posCRotas[vertice.c];

        if(s.posCVertices[vertice.c] == -1 || rotasAlteradas.find(rotaVert) != rotasAlteradas.end()) continue;

        int indexVert = s.posCVertices[vertice.c];
        int cardMaxTour = std::min((double) rots[rotaVert].vertices.size() - 2, cardMax);
        int cardTour = std::floor(Random::getReal(1, cardMaxTour + 1));

        for(int i = 1; i <= cardTour; i++)
        {   
            int indexDel;

            if(indexVert == 1) indexDel = 0;
            else if(indexVert == s.rotas[rotaVert].vertices.size() - 1) indexDel = -1;
            else indexDel = indexVert + Random::getInt((-1 * (1 % i)), 0);

            s.posCRotas[s.rotas[rotaVert].vertices[indexVert]] = -1;
            s.posCVertices[s.rotas[rotaVert].vertices[indexVert]] = -1;

            s.vertSobrando.insert(rots[rotaVert].vertices[indexDel]);

            s.rotas[rotaVert].capAtual -= vecDmd[rots[rotaVert].vertices[indexDel]];

            s.rotas[rotaVert].custo += - matrizAdj[rots[rotaVert].vertices[indexDel - 1]][rots[rotaVert].vertices[indexDel]] 
                                     - matrizAdj[rots[rotaVert].vertices[indexDel]][rots[rotaVert].vertices[indexDel + 1]] 
                                     + matrizAdj[rots[rotaVert].vertices[indexDel - 1]][rots[rotaVert].vertices[indexDel + 1]];

            rots[rotaVert].vertices.erase(rots[rotaVert].vertices.begin() + indexDel);
        }

        if(rots[rotaVert].vertices.size() == 2)
        {
            rots.erase(rots.begin() + rotaVert);
        } else
        {
            rotasAlteradas.insert(rotaVert);
        }        

        rotasAlt++;
    }
}

void Solution::recreate(Solution &sC, Solution &s)
{   
    double beta = 0.01;

    Data &data = Data::getInstance();
    auto &matrizAdj = data.matrizAdj;
    auto &arrDmd = data.arrayDmds;

    s = sC;
    auto &rotas = s.rotas;

    double sortMet = Random::getReal(0.0, 1.1);

    int escolha = -1;

    std::priority_queue<int> vertsSob;

    if(sortMet <= 0.4) vertsSob = gerarPrioQueue(compareByRandom());
    else if(sortMet <= 0.8) vertsSob = gerarPrioQueue(compareByDmd(arrDmd));
    else if(sortMet <= 1.0) vertsSob = gerarPrioQueue(compareByFar(matrizAdj));
    else vertsSob = gerarPrioQueue(compareByClose(matrizAdj));

    for(auto &i : s.vertSobrando) vertsSob.push(i);

    while(!vertsSob.empty())
    {
        int c = vertsSob.top();
        vertsSob.pop();

        int posIndex = -1, posRot = -1, cont, start;
        double custoInsert = 0;

        cont = start = Random::getInt(0, rotas.size() - 1);

        do
        {
            cont %= rotas.size();

            if(rotas[cont].capAtual + arrDmd[c] > data.capacidade) continue;

            for(int i = 0; i < rotas[cont].vertices.size() - 1; i++)
            {
                if(Random::getReal(0, 1) < 1.0 - beta)
                {   
                    double possiInsert = matrizAdj[rotas[cont].vertices[i]][c] + matrizAdj[c][rotas[cont].vertices[i + 1]] 
                                        - matrizAdj[rotas[cont].vertices[i]][rotas[cont].vertices[i + 1]];

                    if(posIndex == -1 || possiInsert < custoInsert)
                    {
                        posIndex = i;
                        posRot = &rotas[cont] - &rotas[0];
                        custoInsert = possiInsert;
                    }
                }
            }
        } while(start != ++cont);

        if(posIndex == -1)
        {
            rotas.emplace_back(rota({1, c, 1}, arrDmd[c], 2 * matrizAdj[1][c]));
            s.posCRotas[c] = rotas.size() - 1;
            s.posCVertices[c] = 1;
        } else
        {
            rotas[posRot].vertices.insert(rotas[posRot].vertices.begin() + posIndex + 1, c);
            rotas[posRot].capAtual += arrDmd[c];
            rotas[posRot].custo += custoInsert;
            s.posCRotas[c] = posIndex;
            s.posCVertices[c] = posRot;
        }

        s.vertSobrando.erase(c);
    }
}