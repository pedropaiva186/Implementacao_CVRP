#ifndef RUINANDRECREATE_H
#define RUINANDRECREATE_H

#include "solution.h"
#include "data.h"
#include "random.h"

void ruin(Solution &s, Solution &sC);
template <typename Comparator> void recreate(Solution &s, Solution &sC, Comparator tipoOrd);
void ruinAndRecreate(Solution &s, Solution &sC);

#endif