#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <cmath>

class Utils {
public:
    // Critério das Linhas (para Jacobi)
    static bool verificarDiagonalDominante(int n, const std::vector<std::vector<double>>& A);
    
    // NOVO: Critério de Sassenfeld (para Seidel)
    static bool verificarSassenfeld(int n, const std::vector<std::vector<double>>& A);
};

#endif