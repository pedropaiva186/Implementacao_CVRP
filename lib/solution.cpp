#include <iostream> // Biblioteca padrão
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <cmath>
#include <random>

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

    for(int i = 2; i <= data.dim; i++)
    {
        sol.rotas.emplace_back(rota({1, i, 1}, arrayDms[i], 2 * matrizAdj[i][1]));
    }

    return sol;
}

// Função que consultará qual a rota de determinado vértice
int Solution::consultaRota(int &vertice)
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
int Solution::consultaPos(int &vertice, int &rota)
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

    // Definindo o consumidor que será o C seed
    int elementoC = Random::getInt(2, data.dim);

    std::unordered_set<int> rotasAlteradas;
    std::vector<vert> eleAdj;

    // Preenchendo o vetor de adjacência
    for(int i = 1; i <= data.dim; i++)
    {
        eleAdj.emplace_back(vert(i, matrizAdj[elementoC][i]));
    }

    std::sort(eleAdj.begin(), eleAdj.end(), compareByDist);

    int rotasAlt = 0;

    for(vert &vertice : eleAdj)
    {   
        if(rotasAlt >= qtdStr)
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

        for(int i = 1; i <= cardTour; i++)
        {   
            // Decidindo o elemento que será deletado
            int indexDel = indexVert + Random::getInt((-1 * (1 % i)), 0);

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

        // Significa que a rota está vazia, portanto ele será apagado das rotas
        if(rotMomento.size() == 2)
        {
            rots.erase(rots.begin() + rotaVert);
        } else // Inserindo a rota deletada no set de rotas
        {
            rotasAlteradas.insert(rotaVert);
        }        

        // Somando ao número de rotas alteradas
        rotasAlt++;
    }

    return s;
}

Solution Solution::recreate(Solution &sC)
{   
    // Definindo o parâmetro que é posto pelo programador
    double beta = 0.1;

    Data &data = Data::getInstance();
    auto &matrizAdj = data.matrizAdj;
    auto &arrDmd = data.arrayDmds;

    // Criando uma seed aleatória
    std::random_device rd;
    std::mt19937 g(rd());

    // Definindo a solução que será retornada
    Solution s = sC;
    auto &rotas = s.rotas;

    std::vector<int> vertsSob;

    // Passando os elementos do unordened set para um vector
    for(auto &i : s.vertSobrando)
    {
        vertsSob.push_back(i);
    }

    // Escolhendo uma maneira de ordenação para os vértices que estão sobrando
    double sortMet = Random::getReal(0.0, 1.1);

    if(sortMet <= 0.4)
        std::shuffle(vertsSob.begin(), vertsSob.end(), g);
    else if(sortMet <= 0.8)
        std::sort(vertsSob.begin(), vertsSob.end(), 
            [arrDmd](const int &a, const int &b) 
            {
                return arrDmd[a] < arrDmd[b];
            });
    else if(sortMet <= 1.0)
        std::sort(vertsSob.begin(), vertsSob.end(),
        [matrizAdj](const int &a, const int &b)
        {
            return matrizAdj[a][1] > matrizAdj[b][1];
        });
    else
        std::sort(vertsSob.begin(), vertsSob.end(), 
        [matrizAdj](const int &a, const int &b)
        {
            return matrizAdj[a][1] < matrizAdj[b][1];
        });

    // Vector que servirá como auxiliar para a seleção das rotas aleatórias
    std::vector<int> indicesRotas;

    for(int i = 0; i < rotas.size(); i++)
    {
        indicesRotas.push_back(i);
    }

    for(int &c : vertsSob)
    {
        // A posição no qual o "c" será inserido
        int posIndex = -1, posRot = -1;
        double custoInsert = 0;

        // Aleatorizando o vector auxiliar das rotas
        std::shuffle(indicesRotas.begin(), indicesRotas.end(), g);

        for(int &i : indicesRotas)
        {   
            rota &rota = rotas[i];

            // Se não for possível inserir o consumidor
            if(rota.capAtual + arrDmd[c] > data.capacidade)
                continue;

            auto &tour = rota.vertices;

            for(int i = 0; i < rota.vertices.size() - 1; i++)
            {
                if(Random::getReal(0, 1) < 1.0 - beta)
                {   
                    double possiInsert = matrizAdj[tour[i]][c] + matrizAdj[c][tour[i + 1]] - matrizAdj[tour[i]][tour[i + 1]];

                    if(posIndex == -1 || possiInsert < custoInsert)
                    {
                        // Atualizando os elementos
                        posIndex = i;
                        posRot = &rota - &rotas[0];
                        custoInsert = possiInsert;
                    }
                }
            }
        }

        // Caso não for possível inserir, é criado uma nova rota
        if(posIndex == -1)
        {
            rotas.emplace_back(rota({1, c, 1}, arrDmd[c], 2 * matrizAdj[1][c]));
        } else // Senão, apenas inserimos o elemento na rota escolhida
        {
            rota &rota = rotas[posRot];
            rota.vertices.insert(rota.vertices.begin() + posIndex + 1, c);
            rota.capAtual += arrDmd[c];
            rota.custo += custoInsert;
        }

        // Deletando o elemento após sua inserção na rota
        s.vertSobrando.erase(c);
    }

    return s;
}