#ifndef SISMIC_H
#define SISMIC_H

#include "common.h"
#include <vector>

class SismicSystem {
public:
    static ResultadoSismico analisar(
        int n, 
        const std::vector<std::vector<double>>& A, 
        const std::vector<double>& b, 
        double erro_max, 
        bool usarSeidel,
        double limiar
    );
};

#endif