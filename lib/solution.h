#ifndef SOLUTION_H
#define SOLUTION_H

#include <vector>
#include <unordered_set>

struct rota
{
    std::vector<int> vertices;
    int capAtual;
    double custo;
    
    rota(std::vector<int> vertices, int capAtual, double custo) : vertices(vertices), capAtual(capAtual), custo(custo) {}
};

class Solution
{   
    private:
        Solution();
        Solution(std::vector<rota> &rots, std::unordered_set<int> &vertSobr, std::vector<int> &pCRot, std::vector<int> &pCVert);

    public:
        std::vector<rota> rotas;
        std::unordered_set<int> vertSobrando;
        std::vector<int> posCRotas;
        std::vector<int> posCVertices;
        static Solution criarSolucaoInicial();
        void exibirSolucao();
        static void ruin(Solution &sC, Solution &s);
        static void recreate(Solution &sC, Solution &s);
};

#endif