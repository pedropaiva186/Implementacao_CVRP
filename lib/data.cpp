#include "data.h"

double Data::calculateDist(double *x1, double *y1, double *x2, double *y2)
{
    double delta1 = *x1 - *x2;
    double delta2 = *y1 - *y2;

    return (1.0 / 100) * floor(100 * sqrt(pow(delta1, 2) + pow(delta2, 2)));
}

// Definindo uma variável global que guardará as informações da instância
Data * Data::instance = nullptr;

Data::Data()
{
    dim = -1;
    matrizAdj = nullptr;
    arrayDmds = nullptr;
    listasAdj = {};
    capacidade = -1;
}

Data & Data::getInstance()
{
    if(instance == nullptr)
        instance = new Data();
    return *instance;
}

void Data::readData(int argNum, char **args)
{   
    instance = new Data();
    
    if(argNum != 2)
    {
        std::cout << "Número de elementos inválido" << std::endl;
        return;
    }

    std::fstream file(args[1]);

    if(!file.is_open())
    {
        std::cout << "Não foi possível abrir o arquivo" << std::endl;
        return;
    }

    std::string in;

    while(1)
    {
        file >> in;
        if(!in.compare("TYPE"))
        {
            file >> in;
            break;
        }
    }

    file >> in;

    if(in.compare(args[0]))
    {
        std::cout << "Tipos diferentes" << std::endl;
        return;
    }

    while(1)
    {
        file >> in;
        if(!in.compare("DIMENSION"))
        {
            file >> in;
            break;
        }
    }

    file >> instance->dim;

    while(1)
    {
        file >> in;
        if(!in.compare("CAPACITY"))
        {
            file >> in;
            break;
        }
    }

    file >> instance->capacidade;

    while(in.compare("NODE_COORD_SECTION"))
    {
        file >> in;
    }

    int pos1 = file.tellg();
    double x1, y1, x2, y2;

    instance->matrizAdj = new double*[instance->dim + 1];

    for(int i = 1; i <= instance->dim; i++)
    {
        instance->matrizAdj[i] = new double[instance->dim + 1];
    }

    instance->listasAdj = std::vector<std::vector<Vert>>(instance->dim + 1, std::vector<Vert>(instance->dim));

    for(int count1 = 1; count1 <= instance->dim; count1++)
    {   
        file.seekg(pos1);

        file >> in;
        file >> x1;
        file >> y1;

        pos1 = file.tellg();

        for(int count2 = count1; count2 <= instance->dim; count2++)
        {   
            if(count1 == count2)
            {
                instance->matrizAdj[count1][count2] = 0;
                instance->listasAdj[count1][count2 - 1] = Vert(count1, 0);
                continue;
            }

            file >> in;
            file >> x2;
            file >> y2;

            double valor = Data::calculateDist(&x1, &y1, &x2, &y2);

            instance->matrizAdj[count1][count2] = valor;
            instance->matrizAdj[count2][count1] = valor;
            instance->listasAdj[count1][count2 - 1] = Vert(count2, valor);
            instance->listasAdj[count2][count1 - 1] = Vert(count1, valor);
        }

        std::sort(instance->listasAdj[count1].begin(), instance->listasAdj[count1].end(), [](const Vert &a, const Vert &b)
        {
            return a.dist < b.dist;
        });
    }

    while(1)
    {
        file >> in;
        if(!in.compare("DEMAND_SECTION"))
        {
            break;
        }
    }

    instance->arrayDmds = new int[instance->dim + 1];

    int valor;

    for(int count = 1; count <= instance->dim; count++)
    {
        file >> in;
        file >> valor;

        instance->arrayDmds[count] = valor;
    }

    file.close();
}