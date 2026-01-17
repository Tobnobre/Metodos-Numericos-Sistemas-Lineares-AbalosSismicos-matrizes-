#include "jacobi.h"
#include <cmath>
#include <algorithm>

using namespace std;

// Adicionado parametro 'erro_final' por referência
pair<vector<double>, int> JacobiSolver::resolver(int n, const vector<vector<double>>& A, const vector<double>& b, double erro_max, double &erro_final) {
    vector<double> x(n);
    
    // Chute inicial
    for (int i = 0; i < n; i++) {
        if (abs(A[i][i]) > 1e-12) x[i] = b[i] / A[i][i];
        else x[i] = 0.0;
    }
    
    vector<double> x_novo = x;
    double erro = 10.0; // Erro inicial alto
    int iter = 0;
    int max_iter = 1000;
    
    while (erro > erro_max) {
        if (iter >= max_iter || erro > 1e15 || std::isnan(erro) || std::isinf(erro)) {
            erro_final = erro; // Salva o erro antes de sair
            return {x, -2}; 
        }

        for (int i = 0; i < n; i++) {
            double soma = 0;
            for (int j = 0; j < n; j++) {
                if (i != j) soma += A[i][j] * x[j];
            }
            
            if (abs(A[i][i]) < 1e-12) return {x, -1}; 
            
            x_novo[i] = (b[i] - soma) / A[i][i];
        }
        
        double max_diff = 0;
        double max_val = 0; 

        for (int i = 0; i < n; i++) {
            max_diff = max(max_diff, abs(x_novo[i] - x[i]));
            max_val = max(max_val, abs(x_novo[i])); 
            x[i] = x_novo[i];
        }
        
        if (max_val > 1e-12) {
            erro = max_diff / max_val;
        } else {
            erro = max_diff; 
        }
    
        iter++;
    }
    
    erro_final = erro; // Salva o erro final alcançado
    return {x, iter};
}