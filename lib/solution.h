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

struct vert
{
    int c;
    double dist;
    
    vert(int c, double dist) : c(c), dist(dist) {}
};

class Solution
{   
    private:
        Solution();
        int consultaRota(int &vertice);
        int consultaPos(int &vertice, int &rota);

    public:
        std::vector<rota> rotas;
        // Usando essa estrutura de dados pois torna a consulta e outras operações O(1)
        std::unordered_set<int> vertSobrando;
        static Solution criarSolucaoInicial();
        void exibirSolucao();
        static Solution ruin(Solution &sC);
        static Solution recreate(Solution &sC);
};

#endif