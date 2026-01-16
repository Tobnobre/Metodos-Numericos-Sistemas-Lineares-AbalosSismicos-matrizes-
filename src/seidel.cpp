#include "seidel.h"
#include <cmath>
#include <algorithm>

using namespace std;

pair<vector<double>, int> SeidelSolver::resolver(int n, const vector<vector<double>>& A, const vector<double>& b, double erro_max) {
    vector<double> x(n);
    
    for (int i = 0; i < n; i++) {
        if (abs(A[i][i]) > 1e-12) x[i] = b[i] / A[i][i];
        else x[i] = 0.0;
    }
    
    vector<double> x_antigo(n); 
    double erro = 10.0;
    int iter = 0;
    int max_iter = 1000;
    
    while (erro > erro_max) {
        // CORREÇÃO: Limite de iterações retorna -2
        if (iter >= max_iter) return {x, -2};

        for (int i = 0; i < n; i++) x_antigo[i] = x[i];
        
        for (int i = 0; i < n; i++) {
            double soma = 0;
            for (int j = 0; j < n; j++) {
                if (i != j) soma += A[i][j] * x[j];
            }
            
            // CORREÇÃO: Pivô zero retorna -1
            if (abs(A[i][i]) < 1e-12) return {x, -1};
            
            x[i] = (b[i] - soma) / A[i][i];
        }
        
        double max_diff = 0;
        for (int i = 0; i < n; i++) max_diff = max(max_diff, abs(x[i] - x_antigo[i]));
        erro = max_diff;

        // CORREÇÃO: Divergência (números infinitos) retorna -2
        if (erro > 1e15 || std::isnan(erro) || std::isinf(erro)) return {x, -2};

        iter++;
    }
    return {x, iter};
}