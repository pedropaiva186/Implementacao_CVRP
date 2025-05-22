#include <iostream>
#include <fstream>
#include <cmath>

#include "data.h"

double Data::calculateDist(double *x1, double *y1, double *x2, double *y2)
{
    double delta1 = *x1 - *x2;
    double delta2 = *y1 - *y2;

    return (1.0 / 100) * floor(100 * sqrt(pow(delta1, 2) + pow(delta2, 2)));
}

// Definindo uma variável global que guardará as informações da instância
Data * Data::instance = nullptr;

// Construtor do objeto
Data::Data()
{
    dim = -1;
    matrizAdj = nullptr;
    arrayDmds = nullptr;
    capacidade = -1;
}

// Retornando uma referência para as informações da instância
Data & Data::getInstance()
{
    if(instance == nullptr)
        instance = new Data();
    return *instance;
}

// Função que lerá o arquivo e armazenará as informações contidas nele
void Data::readData(int argNum, char **args)
{   
    instance = new Data();
    
    if(argNum != 2)
    {
        std::cout << "Número de elementos inválido" << std::endl;
        return;
    }

    // Abrindo o arquivo
    std::fstream file(args[1]);

    // Caso não seja possível abrir o arquivo
    if(!file.is_open())
    {
        std::cout << "Não foi possível abrir o arquivo" << std::endl;
        return;
    }

    std::string in;

    // Retirando os elementos desnecessários até o tipo do arquivo
    while(1)
    {
        file >> in;
        if(!in.compare("TYPE"))
        {
            file >> in;
            break;
        }
    }

    // Recolhendo o tipo do arquivo
    file >> in;

    // Verificando se são tipos de arquivos diferentes
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

    // Recolhendo o número de instâncias
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

    // Recolhendo a capacidade máxima
    file >> instance->capacidade;

    // Criando a matriz de distâncias
    while(in.compare("NODE_COORD_SECTION"))
    {
        file >> in;
    }

    // Elementos que servirão para execução dos cálculos
    int pos1 = file.tellg();
    double x1, y1, x2, y2;

    // Criando a matriz que armazenará os elementos
    instance->matrizAdj = new double*[instance->dim + 1];

    for(int i = 1; i < instance->dim + 1; i++)
    {
        instance->matrizAdj[i] = new double[instance->dim + 1];
    }

    // Lendo o arquivo e inserindo os valores nos atributos do objeto
    for(int count1 = 1; count1 <= instance->dim; count1++)
    {   
        // Posicionando a stream
        file.seekg(pos1);

        // Lendo o indíce disponível, mas não faremos nada com ele
        file >> in;

        // Lendo os valores nos quais serão executados os cálculos
        file >> x1;
        file >> y1;

        // Salvando a posição no arquivo, para ele retornar depois
        pos1 = file.tellg();

        for(int count2 = count1; count2 <= instance->dim; count2++)
        {   
            if(count1 == count2)
            {
                instance->matrizAdj[count1][count2] = 0;
                continue;
            }

            // Fazendo a leitura do indície
            file >> in;

            // Salvando os valores
            file >> x2;
            file >> y2;

            // Salvando na matriz de distâncias
            double valor = Data::calculateDist(&x1, &y1, &x2, &y2);

            instance->matrizAdj[count1][count2] = valor;
            instance->matrizAdj[count2][count1] = valor;
        }
    }

    // Lendo das demandas
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
        // Executando a leitura dos indíces
        file >> in;

        // Resgatando o valor
        file >> valor;

        instance->arrayDmds[count] = valor;
    }

    file.close();
}