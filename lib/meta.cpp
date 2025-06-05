#include "meta.h"

template <typename ArmazemVerts>
int sumAbs(ArmazemVerts &vertSob, std::vector<int> &listAbs)
{
    int count = 0;

    for(auto &i : vertSob) count += listAbs[i];

    return count;
}

void fleetSizeMinimization(Solution &s, Solution &sC, int numMin)
{
    Data &data = Data::getInstance();

    Solution sT;
    s = sC;

    std::vector<int> absList(data.dim + 1, 0);

    while(numMin--)
    {
        ruinAndRecreate(sT, sC, false);

        if(sT.vertSobrando.size() < sC.vertSobrando.size() || sumAbs(sT.vertSobrando, absList) < sumAbs(sC.vertSobrando, absList)) sC = sT;

        if(!sT.vertSobrando.size()){
            s = sT;

            int minCount = INT32_MAX, posRota = 0;

            for(int i = 0; i < sC.rotas.size(); i++)
            {
                int count = sumAbs(sC.rotas[i].vertices, absList);

                if(count < minCount)
                {
                    minCount = count;
                    posRota = i;
                }
            }

            for(int c = 1; c < sC.rotas[posRota].vertices.size() - 1; c++)
            {
                sC.posCVertices[sC.rotas[posRota].vertices[c]] = -1;
                sC.vertSobrando.insert(sC.rotas[posRota].vertices[c]);
            }
            
            for(int &posC : sC.posCRotas) if(posC > posRota) posC--;

            sC.rotas.erase(sC.rotas.begin() + posRota);
        }

        for(auto &c : sT.vertSobrando) absList[c]++;
    }
}

void simulatedAnealling(Solution &s, Solution &sC)
{

}