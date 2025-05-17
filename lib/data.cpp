#include <iostream>
#include <fstream>

#include "data.h"

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

    file.close();
}