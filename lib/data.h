#ifndef DATA_H
#define DATA_H

class Data
{
    private:
        Data();
        static Data *instance;
        static double calculateDist(double *x1, double *y1, double *x2, double *y2);

    public:
        int dim;
        double **matrizAdj;
        int *arrayDmds;
        int capacidade;

        static Data & getInstance();
        static void readData(int argNum, char **args);

};
#endif