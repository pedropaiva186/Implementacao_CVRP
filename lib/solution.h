#ifndef SOLUTION_H
#define SOLUTION_H

#include <vector>
#include <unordered_set>

struct Rota
{
    std::vector<int> vertices;
    int capAtual;
    double custo;
    
    Rota(std::vector<int> vertices, int capAtual, double custo) : vertices(vertices), capAtual(capAtual), custo(custo) {}
};

class Solution
{   
    public:
        std::vector<Rota> rotas;
        std::unordered_set<int> vertSobrando;
        std::vector<int> posCRotas;
        std::vector<int> posCVertices;
        Solution();
        Solution(std::vector<Rota> &rots, std::unordered_set<int> &vertSobr, std::vector<int> &pCRot, std::vector<int> &pCVert);
        static Solution criarSolucaoInicial();
        void exibirSolucao();
        static void fleetSizeMinimization(Solution &s, Solution &sC, int numMin);
};

#endif