#include "seidel.h"
#include <cmath>
#include <algorithm>

using namespace std;

// Calculo por Seidel
pair<vector<double>, int> SeidelSolver::resolver(int n, const vector<vector<double>>& A, const vector<double>& b, double erro_max) {
    vector<double> x(n);
    
    // Chute inicial
    for (int i = 0; i < n; i++) {
        if (abs(A[i][i]) > 1e-12) x[i] = b[i] / A[i][i];
        else x[i] = 0.0;
    }
    
    vector<double> x_antigo(n); 
    double erro = 10.0;
    int iter = 0;
    
    //Loop principal
    while (erro > erro_max && iter < 1000) {
        for (int i = 0; i < n; i++) x_antigo[i] = x[i];
        
        for (int i = 0; i < n; i++) {
            double soma = 0;
            for (int j = 0; j < n; j++) {
                if (i != j) soma += A[i][j] * x[j];
            }
            if (abs(A[i][i]) < 1e-12) return {x, -1}; // Erro caso pivô zero
            x[i] = (b[i] - soma) / A[i][i];
        }
        
        double max_diff = 0;
        for (int i = 0; i < n; i++) max_diff = max(max_diff, abs(x[i] - x_antigo[i]));
        erro = max_diff;
        iter++;
    }
    return {x, iter};
}