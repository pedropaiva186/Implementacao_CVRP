#include <iostream>
#include <fstream>

struct Data
{
    // Definindo a variável que guardará as informações da instância
    Data * Data::instance = nullptr;

    // Retornando uma referência para as informações da instância
    Data & getInstance()
    {
        return *instance;
    }
};