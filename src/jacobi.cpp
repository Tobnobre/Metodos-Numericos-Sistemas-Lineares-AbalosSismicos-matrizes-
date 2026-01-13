#include "jacobi.h"
#include <cmath>
#include <algorithm>

using namespace std;

// Cálculo por Jacobi
pair<vector<double>, int> JacobiSolver::resolver(int n, const vector<vector<double>>& A, const vector<double>& b, double erro_max) {
    vector<double> x(n);
    
    // Chute inicial
    for (int i = 0; i < n; i++) {
        if (abs(A[i][i]) > 1e-12) x[i] = b[i] / A[i][i];
        else x[i] = 0.0;
    }
    
    vector<double> x_novo = x;
    double erro = 10.0;
    int iter = 0;
    
    // Loop principal
    while (erro > erro_max && iter < 1000) {
        for (int i = 0; i < n; i++) {
            double soma = 0;
            for (int j = 0; j < n; j++) {
                if (i != j) soma += A[i][j] * x[j];
            }
            if (abs(A[i][i]) < 1e-12) return {x, -1}; // Erro caso pivô zero
            x_novo[i] = (b[i] - soma) / A[i][i];
        }
        
        double max_diff = 0;
        for (int i = 0; i < n; i++) {
            max_diff = max(max_diff, abs(x_novo[i] - x[i]));
            x[i] = x_novo[i];
        }
        erro = max_diff;
        iter++;
    }
    return {x, iter};
}