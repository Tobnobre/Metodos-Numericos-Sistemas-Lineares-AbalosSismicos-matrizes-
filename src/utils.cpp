#include "utils.h"

bool Utils::verificarDiagonalDominante(int n, const std::vector<std::vector<double>>& A) {
    for (int i = 0; i < n; i++) {
        double soma_linha = 0.0;
        for (int j = 0; j < n; j++) {
            if (i != j) soma_linha += std::abs(A[i][j]);
        }
        if (std::abs(A[i][i]) < soma_linha) return false;
    }
    return true;
}