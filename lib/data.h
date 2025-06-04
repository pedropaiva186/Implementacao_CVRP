#ifndef DATA_H
#define DATA_H

#include <vector>
#include "solution.h"

struct Vert
{
    int c;
    double dist;
    
    Vert(int c, double dist) : c(c), dist(dist) {}
    Vert() : c(0), dist(0.0) {}
};

class Data
{
    private:
        Data();
        static Data *instance;
        static double calculateDist(double *x1, double *y1, double *x2, double *y2);

    public:
        int dim;
        double **matrizAdj;
        std::vector<std::vector<Vert>> listasAdj;
        int *arrayDmds;
        int capacidade;

        static Data & getInstance();
        static void readData(int argNum, char **args);
};

#endif