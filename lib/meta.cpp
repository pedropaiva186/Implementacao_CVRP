#include "meta.h"

void fleetSizeMinimization(Solution &s, Solution &sC, int numMin)
{
    Data &data = Data::getInstance();

    Solution sT;
    s = sC;

    std::vector<int> absC(data.dim + 1, 0);

    for(int i = 2; i <= data.dim; i++) absC[i] = 0;

    while(numMin)
    {
        ruinAndRecreate(sT, sC);
    }
}