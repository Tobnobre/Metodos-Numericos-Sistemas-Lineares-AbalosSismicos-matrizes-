#include "utils.h"
#include <vector>
#include <cmath>

using namespace std;

bool Utils::verificarDiagonalDominante(int n, const vector<vector<double>>& A) {
    for (int i = 0; i < n; i++) {
        double soma_linha = 0.0;
        for (int j = 0; j < n; j++) {
            if (i != j) soma_linha += abs(A[i][j]);
        }
        if (abs(A[i][i]) == 0 || abs(A[i][i]) < soma_linha) return false;
    }
    return true;
}

bool Utils::verificarSassenfeld(int n, const vector<vector<double>>& A) {
    vector<double> beta(n);

    for (int i = 0; i < n; i++) {
        double soma = 0.0;

        for (int j = 0; j < n; j++) {
            if (i != j) {
                double val = abs(A[i][j]);
                if (j < i) {
                    soma += val * beta[j];
                } else {
                    soma += val;
                }
            }
        }

        if (abs(A[i][i]) < 1e-12) return false;
        
        beta[i] = soma / abs(A[i][i]);

        if (beta[i] >= 1.0) return false;
    }

    return true;
}