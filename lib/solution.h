#ifndef SOLUTION_H
#define SOLUTION_H

#include <vector>

struct rota
{
    std::vector<int> vertices;
    int capAtual;
    double custo;
    
    rota(std::vector<int> vertices, int capAtual, int custo) : vertices(vertices), capAtual(capAtual), custo(custo) {}
};

class Solution
{   
    private:
        Solution();

    public:
        std::vector<rota> rotas;
        std::vector<int> vertSobrando;
        static Solution criarSolucaoInicial();
        void exibirSolucao();
};

#endif