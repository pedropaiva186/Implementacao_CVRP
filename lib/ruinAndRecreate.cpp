#include <iostream>
#include <algorithm>
#include <queue>

#include "ruinAndRecreate.h"

// Declarando os parâmetros

constexpr double ALPHA = 0.01;
constexpr double L_MAX = 10;
constexpr double C_MEDIA = 10;
constexpr double BETA = 0.01;

// Structs para o uso do priority queue

struct CompareByRandom
{
    bool operator()(const int &a, const int &b) {return Random::getReal(0, 1) < Random::getReal(0, 1);}
};

struct CompareByDmd
{   
    int* arrayDmd;

    CompareByDmd(int *arr) : arrayDmd(arr) {};

    bool operator()(const int &a, const int &b) {return arrayDmd[a] > arrayDmd[b];}
};

struct CompareByFar
{
    double **matrizAdj;

    CompareByFar(double **matriz) : matrizAdj(matriz) {};

    bool operator()(const int &a, const int &b) {return matrizAdj[1][a] < matrizAdj[1][b];}
};

struct CompareByClose
{   
    double **matrizAdj;

    CompareByClose(double **matriz) : matrizAdj(matriz) {};

    bool operator()(const int &a, const int &b) {return matrizAdj[1][a] > matrizAdj[1][b];}
};

template <typename Comparator>
std::priority_queue<int, std::vector<int>, Comparator> gerarPrioQueue(Comparator comp)
{
    return std::priority_queue<int, std::vector<int>, Comparator>(comp);
}

void ruin(Solution &s, Solution &sC)
{   
    Data & data = Data::getInstance();

    s = sC;
    auto &rots = s.rotas;

    double media = 0;

    for(Rota &rot : rots) media += (double) rot.vertices.size() - 2;

    media /= (double) rots.size();

    double cardMax = std::min((double) L_MAX, media);
    double qtdMaxStr = ((4.0 * C_MEDIA) / (1 + cardMax)) - 1;
    int qtdStr = std::floor(Random::getReal(1, qtdMaxStr + 1));

    int elementoC = Random::getInt(2, data.dim);

    std::unordered_set<int> rotasAlteradas;

    int rotasAlt = 0;

    for(Vert &vertice : data.listasAdj[elementoC])
    {
        if(rotasAlt >= qtdStr) break;

        int rotaVert = s.posCRotas[vertice.c];

        Rota &rotAlt = rots[rotaVert];

        if(s.posCVertices[vertice.c] == -1 || rotasAlteradas.find(rotaVert) != rotasAlteradas.end() || vertice.c == 1) continue;

        int indexVert = s.posCVertices[vertice.c];
        int cardMaxTour = std::min((double) rotAlt.vertices.size() - 2, cardMax);
        int cardTour = std::floor(Random::getReal(1, cardMaxTour + 1));

        int strNumEsquerda = 0, strNumDireita = 0;

        if(0) // Split String removal "Random::getInt(0, 1)"
        {
            int m = 1, mMax = rotAlt.vertices.size() - cardTour - 2;

            while(m < mMax && Random::getReal(0, 1) < ALPHA) m++;

            strNumEsquerda = Random::getInt(std::max((double) cardTour + m - rotAlt.vertices.size() + indexVert + 1, 0.0), indexVert - 1);
            
            strNumDireita = cardTour + m - strNumEsquerda - 1;

        } else // String removal
        {   
            strNumEsquerda = Random::getInt(std::max((int)(cardTour - rotAlt.vertices.size() + indexVert + 1), 0), std::min(cardTour - 1, indexVert - 1));
            
            strNumDireita = cardTour - strNumEsquerda - 1;

            double deltaCusto = 0;

            for(int i = indexVert - strNumEsquerda; i <= indexVert + strNumDireita; i++)
            {
                int &eleAlt = rotAlt.vertices[i];

                rotAlt.capAtual -= data.arrayDmds[eleAlt];
                deltaCusto += data.matrizAdj[rotAlt.vertices[i - 1]][eleAlt];

                s.vertSobrando.insert(eleAlt);
                s.posCRotas[eleAlt] = -1;
                s.posCVertices[eleAlt] = -1;
            }

            for(int i = indexVert + strNumDireita + 1; i < rotAlt.vertices.size() - 1; i++) s.posCVertices[rotAlt.vertices[i]] -= cardTour;

            deltaCusto += data.matrizAdj[rotAlt.vertices[indexVert + strNumDireita]][rotAlt.vertices[indexVert + strNumDireita + 1]] 
                          - data.matrizAdj[rotAlt.vertices[indexVert - strNumEsquerda - 1]][rotAlt.vertices[indexVert + strNumDireita + 1]];

            rotAlt.custo -= deltaCusto;

            rotAlt.vertices.erase(rotAlt.vertices.begin() + indexVert - strNumEsquerda, rotAlt.vertices.begin() + indexVert + strNumDireita + 1);
        }

        if(rotAlt.vertices.size() == 2)
        {   
            rots.erase(rots.begin() + rotaVert);
            for(int &v : s.posCRotas) if(v >= rotaVert) v--;
        }
        else rotasAlteradas.insert(rotaVert);

        rotasAlt++;
    }
}

template <typename Comparator>
void recreate(Solution &s, Solution &sC, Comparator tipoOrd)
{   
    Data &data = Data::getInstance();

    s = sC;
    auto &rotas = s.rotas;

    std::priority_queue<int, std::vector<int>, Comparator> vertsSob(tipoOrd);

    for(auto &i : s.vertSobrando) vertsSob.push(i);

    while(!vertsSob.empty())
    {
        int c = vertsSob.top();
        vertsSob.pop();

        int posIndex = -1, posRot = -1, cont, start;
        double custoInsert = 0;

        cont = start = Random::getInt(0, rotas.size() - 1);
        cont++;

        do
        {
            cont %= rotas.size();

            if(rotas[cont].capAtual + data.arrayDmds[c] > data.capacidade) continue;

            for(int i = 0; i < rotas[cont].vertices.size() - 1; i++)
            {
                if(Random::getReal(0, 1) < 1.0 - BETA)
                {   
                    double possiInsert = data.matrizAdj[rotas[cont].vertices[i]][c] + data.matrizAdj[c][rotas[cont].vertices[i + 1]] 
                                        - data.matrizAdj[rotas[cont].vertices[i]][rotas[cont].vertices[i + 1]];

                    if(posIndex == -1 || possiInsert < custoInsert)
                    {
                        posIndex = i;
                        posRot = cont;
                        custoInsert = possiInsert;
                    }
                }
            }
        } while(start != cont++);

        if(posIndex == -1)
        {
            rotas.emplace_back(Rota({1, c, 1}, data.arrayDmds[c], 2 * data.matrizAdj[1][c]));
            s.posCRotas[c] = rotas.size() - 1;
            s.posCVertices[c] = 1;
        }
        else
        {
            rotas[posRot].vertices.insert(rotas[posRot].vertices.begin() + posIndex + 1, c);
            rotas[posRot].capAtual += data.arrayDmds[c];
            rotas[posRot].custo += custoInsert;
            s.posCVertices[c] = posIndex + 1;
            s.posCRotas[c] = posRot;

            for(int i = posIndex + 2; i < rotas[posRot].vertices.size() - 1; i++) s.posCVertices[rotas[posRot].vertices[i]]++;
        }

        s.vertSobrando.erase(c);
    }
}

void ruinAndRecreate(Solution &s, Solution &sC)
{
    Data &data = Data::getInstance();
    ruin(s, sC);
    
    double sortMet = Random::getReal(0.0, 1.1);

    if(sortMet <= 0.4)
    {
        auto c = CompareByRandom();
        recreate(sC, s, c);
    } 
    else if(sortMet <= 0.8)
    {
        auto c = CompareByDmd(data.arrayDmds);
        recreate(sC, s, c);
    }
    else if(sortMet <= 1.0)
    {
        auto c = CompareByFar(data.matrizAdj);
        recreate(sC, s, c);
    }
    else
    {
        auto c = CompareByClose(data.matrizAdj);
        recreate(sC, s, c);
    }
}