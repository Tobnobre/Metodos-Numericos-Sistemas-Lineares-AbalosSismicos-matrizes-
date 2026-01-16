#include "jacobi.h"
#include <cmath>
#include <algorithm>

using namespace std;

pair<vector<double>, int> JacobiSolver::resolver(int n, const vector<vector<double>>& A, const vector<double>& b, double erro_max) {
    vector<double> x(n);
    
    // Chute inicial seguro
    for (int i = 0; i < n; i++) {
        if (abs(A[i][i]) > 1e-12) x[i] = b[i] / A[i][i];
        else x[i] = 0.0;
    }
    
    vector<double> x_novo = x;
    double erro = 10.0;
    int iter = 0;
    int max_iter = 1000;
    
    while (erro > erro_max) {
        // CORREÇÃO AQUI: Retorna -2 para Limite ou Divergência
        if (iter >= max_iter || erro > 1e15 || std::isnan(erro) || std::isinf(erro)) {
            return {x, -2}; 
        }

        for (int i = 0; i < n; i++) {
            double soma = 0;
            for (int j = 0; j < n; j++) {
                if (i != j) soma += A[i][j] * x[j];
            }
            
            // CORREÇÃO AQUI: Retorna -1 apenas se o pivô for zero
            if (abs(A[i][i]) < 1e-12) return {x, -1}; 
            
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