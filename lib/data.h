#ifndef DATA_H
#define DATA_H

class Data
{
    private:
        Data();
        static Data *instance;
        
    public:
        int dim;
        double **matrizAdj;
        double *arrayDmds;
        int capacidade;

        static Data & getInstance();
        static void readData(int argNum, char **args);

};
#endif